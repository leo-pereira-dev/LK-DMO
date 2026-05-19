#include "base.h"
#include "asio.hpp"

#include "cClient.h"

#include <thread>

#include "asio/system_error.hpp"
#include "../LibProj/CsFunc/CsMessageBox.h"
#include "../LibProj/CsFunc/CrashLogger.h"

#include <mutex>
#include <fstream>
#include <chrono>
#include <ctime>

//#include "lib_netsockets/Include/json_message.hh"

using namespace asio::ip;

static asio::io_context io_ctx;
static std::once_flag io_thread_once;

// Work guard keeps io_ctx.run() from returning when the current cClient
// instance's pending operations all drain (which happens on logout when the
// game cClient is destroyed and its socket is cancel()'d).  Without it:
//
//   1. Game session ends, ~cClient cancels its socket, all in-flight async
//      ops complete with operation_aborted, io_ctx goes idle.
//   2. The detached worker thread's io_ctx.run() returns; thread exits.
//   3. Server-select self-heal creates a new cCliAccount and calls Run() →
//      starts a new worker thread that calls io_ctx.run().
//   4. But io_ctx is in the "stopped/exhausted" state — run() returns
//      immediately without pumping any completion handlers.
//   5. async_connect's TCP handshake still finishes at the OS level (Windows
//      IOCP completes the kernel-side connect regardless), and async_write
//      bytes still reach the wire — but the connect-completion handler never
//      fires, so OnConnected → SendLogin is never called, and the server's
//      handshake/reply bytes pile up in the IOCP queue with nobody to dispatch
//      them.  Result: server log shows the post-logout TCP connection +
//      client-sent ClusterList request, but never sees a Login packet, and
//      the client never processes the reply → empty server-select list.
//
// The work guard holds a "fake" outstanding work item so io_ctx.run() blocks
// indefinitely.  We only release it at process exit.
static auto io_work = asio::make_work_guard(io_ctx);

static void NetTrace(const char* fmt, ...)
{
	try
	{
		CreateDirectoryA("logs", NULL);

		char msg[1024] = { 0, };
		va_list args;
		va_start(args, fmt);
		vsnprintf_s(msg, sizeof(msg), _TRUNCATE, fmt, args);
		va_end(args);

		auto now = std::chrono::system_clock::now();
		auto tt = std::chrono::system_clock::to_time_t(now);
		tm localTime = {};
		localtime_s(&localTime, &tt);

		char stamp[64] = { 0, };
		strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", &localTime);

		std::ofstream log("logs\\network_pgsql.log", std::ios::app);
		log << stamp << " " << msg << std::endl;
	}
	catch (...) {}
}

cClient::cClient(uint16_t) : enc(false), m_v(0), m_timets(0), m_a(false), m_sb(false), m_alive(std::make_shared<std::atomic_bool>(true)), m_sock(io_ctx)
{

}

bool cClient::Connect(const char* ip, uint16_t port)
{
	//CsMessageBoxA(MB_OK, "\n connecting to %s::%d\n", ip, port);
	NetTrace("connect begin ip=%s port=%u", ip, port);

	if (port == 7029)
	{
		DBG("\nConnecting to Auth Server on %s:%d\n", ip, port);
	}
	else if (port == 7050)
	{
		DBG("\nConnecting to Character Server on %s:%d\n", ip, port);
	}
	else
	{
		DBG("\nConnecting to Game Server on %s:%d\n", ip, port);
	}

	if (m_a)
	{
		m_sock.cancel();
		m_sock.close();
	}

	auto alive = m_alive;
	auto traceIp = std::string(ip);
	m_sock.async_connect(tcp::endpoint(address().from_string(trim(ip)), port), [this, alive, traceIp, port](const std::error_code& ec)
		{
			if (!alive->load())
				return;

			if (ec)
			{
				if (ec == asio::error::operation_aborted)
					return;

				if (ec.value() == 10061)
				{
					DBG("\nError %d: Server not opened\n", ec);
					OnDisconnected((char*)"Disconnecting with error 10061");
				}
				else
				{
					DBG("\nError connecting to AuthServer: %d\n", ec);
					OnDisconnected((char*)"Disconnecting with error 10003");
				}

				m_a = false;
			}
			else
			{
				NetTrace("connect ok ip=%s port=%u", traceIp.c_str(), port);
				OnConnected();
				StartRead();
			}
		});

	m_a = true;
	return true;
}

void cClient::EnableEncryption(unsigned int version)
{
	// TODO
}

void cClient::DoSend(cPacket& p)
{
	if (m_sb) {

		#ifdef DEBUG_NETWORK
				CsMessageBoxA(MB_OK, "Requests disabled for now");
		#endif
		return;
	}

	if (!m_a)
	{
		uint16_t packetId = 0;
		if (p.real_len >= 4)
		{
			auto data = static_cast<const uint8_t*>(p.m_buf.data().data());
			memcpy_s(&packetId, sizeof(packetId), data + 2, sizeof(packetId));
		}
		NetTrace("send skipped inactive socket packet=%u bytes=%u",
			static_cast<unsigned>(packetId),
			static_cast<unsigned>(p.real_len));
		p.m_buf.consume(p.real_len);
		return;
	}

	// Original code captured `&p` by reference and called `p.m_buf.consume(bt)`
	// inside the async_write completion handler. Callers (SendXxx) build the
	// cPacket on their stack and return immediately after DoSend, so by the time
	// the io_context thread fires the handler, `p` has been destroyed — `m_buf`
	// is freed memory and `consume()` blows up at +0x13/+0x15 inside
	// asio::basic_streambuf::consume.  That's the recurring teardown crash.
	//
	// Fix: copy the wire bytes into a heap-owned vector that the lambda holds
	// via shared_ptr, and consume the caller's streambuf SYNCHRONOUSLY here.
	// The caller's cPacket can now safely go out of scope while the async write
	// is in flight.
	auto bytes = std::make_shared<std::vector<uint8_t>>(p.real_len);
	uint16_t packetId = 0;
	if (p.real_len >= 4)
	{
		auto data = static_cast<const uint8_t*>(p.m_buf.data().data());
		memcpy_s(&packetId, sizeof(packetId), data + 2, sizeof(packetId));
	}
	unsigned localPort = 0;
	try
	{
		localPort = static_cast<unsigned>(m_sock.local_endpoint().port());
	}
	catch (...) {}

	NetTrace("send begin active=%d local_port=%u packet=%u bytes=%u",
		m_a ? 1 : 0,
		localPort,
		static_cast<unsigned>(packetId),
		static_cast<unsigned>(p.real_len));
	asio::buffer_copy(asio::buffer(*bytes), p.m_buf.data(), p.real_len);
	p.m_buf.consume(p.real_len);

	asio::async_write(m_sock, asio::buffer(*bytes), asio::transfer_exactly(bytes->size()),
		[this, bytes, alive = m_alive](const std::error_code& ec, size_t /*bt*/)
	{
		if (!alive->load())
			return;

		if (ec)
		{
			// Same teardown-cancel guard as the read lambdas — operation_aborted
			// fires when ~cClient calls m_sock.cancel(); cClient may be torn
			// down by the time we get here, so skip the virtual OnDisconnected.
			if (ec == asio::error::operation_aborted)
				return;
			if (ec.value() == 10058 || !m_a)
			{
				NetTrace("send ignored closed socket ec=%d message=%s bytes=%u",
					ec.value(),
					ec.message().c_str(),
					static_cast<unsigned>(bytes->size()));
				return;
			}
#ifdef DEBUG_NETWORK
			CsMessageBoxA(MB_OK, "Error while transfering bytes %d", ec);
#endif
			NetTrace("send error ec=%d message=%s bytes=%u", ec.value(), ec.message().c_str(), static_cast<unsigned>(bytes->size()));
			OnDisconnected((char*)"10003");
			m_a = false;
		}
		else
		{
			NetTrace("send ok bytes=%u", static_cast<unsigned>(bytes->size()));
		}
	});
}

void cClient::DoDisconnect()
{
	try
	{
		m_sock.shutdown(m_sock.shutdown_send);
	}
	catch (...) {}

	m_a = false;
	DBG("\n");
	OnDisconnected((char*)"Disconecting from server");
	DBG("\n");
}

void cClient::Stop()
{

}

bool cClient::Bind()
{
	return true;
}

cClient::~cClient()
{
	m_alive->store(false);

	// Inverted condition fix: original had `if (!m_sock.is_open()) m_sock.close();`
	// — closing only when NOT open. With an OPEN socket left untouched, the
	// in-flight async_read/async_write callbacks (which capture `this` by
	// reference / shared_ptr) continued firing on freed memory after this
	// destructor returned.
	//
	// Cancel pending async ops first so their lambdas fire with
	// operation_aborted before we tear down. Use the error_code overloads to
	// avoid throwing inside a destructor.
	asio::error_code ec;
	m_sock.cancel(ec);
	if (m_sock.is_open())
	{
		m_sock.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		m_sock.close(ec);
	}

	if (m_t.joinable())
		m_t.join();
}

bool cClient::Run(int unk)
{
	std::call_once(io_thread_once, []()
	{
		std::thread([]()
		{
			io_ctx.run();
		}).detach();
	});
	return true;
}

void cClient::DoExecute()
{

}

void cClient::StartRead()
{
	asio::async_read(m_sock, m_read.m_buf.prepare(2), asio::transfer_exactly(2), [this, alive = m_alive](asio::error_code ec, std::size_t)
	{
		if (!alive->load())
			return;

		if (ec)
		{
			// operation_aborted = teardown cancel from ~cClient. The cClient
			// instance may already be destroyed by the time this fires (the
			// io_ctx worker thread is detached); calling OnDisconnected (virtual)
			// would __purecall on a torn-down vtable. Quiet bail.
			if (ec == asio::error::operation_aborted)
				return;
			DBG("\nError Code: %d\n", ec);
			OnDisconnected((char*)"10003");
			m_a = false;
			return;
		}

		m_read.m_buf.commit(2);

		uint16_t len;
		m_read.pop(len);

		m_read.real_len = len;
		NetTrace("recv header len=%u", static_cast<unsigned>(len));

		int len2 = len - 2;
	 
		if (len2 > 0) {
			ReadAll(len2);
		}
		else{
			StartRead();
			}
	});
}

void cClient::ReadAll(int b)
{
	asio::async_read(m_sock, m_read.m_buf.prepare(b), asio::transfer_exactly(b), [this, alive = m_alive](asio::error_code ec, std::size_t bt)
		{
			if (!alive->load())
				return;

			if (ec)
			{
				// Same teardown-cancel guard as StartRead — see comment there.
				if (ec == asio::error::operation_aborted)
					return;
				DBG("\nError Code: %d\n", ec);
				OnDisconnected((char*)"10003");
				m_a = false;
				return;
			}

			m_read.m_buf.commit(bt);

			m_read.pop(m_sproto);
			NetTrace("recv body packet=%u body_bytes=%u total_len=%u",
				static_cast<unsigned>(m_sproto),
				static_cast<unsigned>(bt),
				static_cast<unsigned>(m_read.real_len));
			nsCSDEBUG::CrashLogger::LogMessage("NET recv packet=%u body_bytes=%u total_len=%u",
				static_cast<unsigned>(m_sproto),
				static_cast<unsigned>(bt),
				static_cast<unsigned>(m_read.real_len));

			NetTrace("execute begin packet=%u", static_cast<unsigned>(m_sproto));
			OnExecute();
			NetTrace("execute end packet=%u", static_cast<unsigned>(m_sproto));

			m_read.m_buf.consume(bt); // force consume everything else

			StartRead();
		});
}


void cClient::push(void* data, size_t len, bool wlen)
{
	m_write.push(data, len, wlen);
}

void cClient::pop(void* data, size_t len, bool wlen)
{
	m_read.pop(data, len, wlen);
}

void cClient::mark(void* data, size_t len)
{
	m_write.mark(data, len); // TODO: what is this?
}

void cClient::endp(uint16_t id)
{
	m_write.endp(id);
}

void cClient::newp(uint16_t id)
{
	m_write.newp(id);
}
