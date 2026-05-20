/*
	RECODED NLIB, FOR BUILDING DMO LEAKED SOURCES

	@author Arves100
	@date 17/02/2022
	@file packet.cpp
	@brief asio stream packet
*/
#include "base.h"
#include "cpacket.h"

// TODO: replace literally all of this with libnpk

/*
	Packet format:
		length
		id
		[data]
		checksum (length ^ 6716)
*/

cPacket::cPacket() : real_len(0) {}
cPacket::~cPacket() = default;

void cPacket::newp(uint16_t id)
{
	real_len = 0;
	auto b = m_buf.prepare(4); // len + id
	memcpy_s((uint8_t*)b.data() + 2, 2, &id, 2);
	m_buf.commit(4);
	real_len += 4;
}

void cPacket::mark(void* data, size_t len)
{
	push(data, len, false); // TODO: what is this?
}

void cPacket::endp(uint16_t)
{
	real_len += 2;

	// length
	((uint8_t*)m_buf.data().data())[1] = (real_len >> 8) & 0xFF;
	((uint8_t*)m_buf.data().data())[0] = real_len & 0xFF;

	auto checksum = real_len ^ 6716;

	auto b = m_buf.prepare(2);
	memcpy_s(b.data(), 2, &checksum, 2);
	m_buf.commit(2);
}

void cPacket::push(void* data, size_t len, bool wlen)
{
	if (data == nullptr || len == 0)
		return;

	if (wlen && len > 0xFF)
		len = 0xFF;

	if (len > (size_t)UINT16_MAX - real_len - (wlen ? 1 : 0))
		return;

	if (wlen)
	{
		auto p = m_buf.prepare(1);
		((uint8_t*)p.data())[0] = len & 0xFF;
		m_buf.commit(1);
		real_len++;
	}

	auto p = m_buf.prepare(len);
	memcpy_s(p.data(), len, data, len);
	m_buf.commit(len);
	real_len += (uint16_t)len;
}

void cPacket::pop(void* data, size_t len, bool wlen)
{
	if (data == nullptr)
		return;

	if (wlen)
	{
		if (m_buf.size() < 1)
			return;

		len = *(uint8_t*)m_buf.data().data();
		m_buf.consume(1);
	}

	auto available = m_buf.size();
	if (len > available)
	{
		if (wlen)
			((char*)data)[0] = '\0';
		return;
	}

	if (len == 0)
	{
		if (wlen)
			((char*)data)[0] = '\0';
		return;
	}

	memcpy_s(data, len, m_buf.data().data(), len);
	if (wlen)
		((char*)data)[len] = '\0';
	m_buf.consume(len);
}
