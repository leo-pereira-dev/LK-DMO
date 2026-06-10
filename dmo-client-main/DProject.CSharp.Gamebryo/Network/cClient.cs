using System.Collections.Concurrent;
using System.Net.Sockets;

namespace DProject.Network;

public abstract class cClient : IDisposable
{
    private readonly NetworkPacketCodec _codec = new();
    private readonly NetworkDispatchQueue _dispatchQueue = new();
    private readonly ConcurrentQueue<byte[]> _sendQueue = new();
    private readonly SemaphoreSlim _sendSignal = new(0);
    private readonly object _decodeLock = new();
    private readonly List<byte> _receiveBuffer = new();
    private BlockingCollection<byte[]>? _decodeQueue;
    private CancellationTokenSource? _cts;
    private TcpClient? _socket;
    private Task? _socketTask;
    private Task? _decodeTask;
    private long _sequence;
    private int _disconnectRaised;
    private bool _useRealSockets;
    private bool _decodePacketsOnWorker;

    protected cClient(string name)
    {
        Name = name;
    }

    public string Name { get; }
    public string Ip { get; protected set; } = "127.0.0.1";
    public int Port { get; protected set; }
    public bool IsStarted { get; private set; }
    public bool IsSocketConnected => _socket?.Connected == true;

    public bool IsConnected
    {
        get
        {
            if (!IsStarted)
            {
                return false;
            }

            return !_useRealSockets || IsSocketConnected;
        }
    }

    public ushort CurrentProtocol { get; private set; }
    public int PendingDispatchCount => _dispatchQueue.Count;
    public int PendingSendCount => _sendQueue.Count;

    public virtual bool Start(string ip, int port)
    {
        return Start(ip, port, useRealSockets: false, decodePacketsOnWorker: true);
    }

    public virtual bool Start(string ip, int port, bool useRealSockets, bool decodePacketsOnWorker)
    {
        if (IsStarted)
        {
            return true;
        }

        Ip = ip.Trim();
        Port = port;
        _useRealSockets = useRealSockets;
        _decodePacketsOnWorker = decodePacketsOnWorker;
        _cts = new CancellationTokenSource();
        IsStarted = true;
        Interlocked.Exchange(ref _disconnectRaised, 0);

        if (_decodePacketsOnWorker)
        {
            StartDecodeWorker(_cts.Token);
        }

        if (!_useRealSockets)
        {
            OnConnected();
            return true;
        }

        _socketTask = Task.Run(() => RunSocketAsync(_cts.Token));
        return true;
    }

    public virtual void Stop(string reason = "client stopped")
    {
        if (!IsStarted)
        {
            return;
        }

        IsStarted = false;
        _cts?.Cancel();
        _decodeQueue?.CompleteAdding();
        _sendSignal.Release();

        try
        {
            _socket?.Close();
            _socket?.Dispose();
        }
        catch (ObjectDisposedException)
        {
        }

        RaiseDisconnected(reason);
    }

    public virtual void OnConnected()
    {
    }

    public virtual int OnExecute(int maxPackets = 256)
    {
        return _dispatchQueue.Drain(maxPackets, packet =>
        {
            try
            {
                CurrentProtocol = packet.PacketId;
                if (!packet.ChecksumValid)
                {
                    OnPacketError(packet, new InvalidOperationException($"Invalid checksum for packet {packet.PacketId}."));
                    return;
                }

                OnPacket(packet);
            }
            catch (Exception ex) when (ex is InvalidOperationException or ArgumentOutOfRangeException)
            {
                OnPacketError(packet, ex);
            }
        });
    }

    public virtual void OnDisconnected(string message)
    {
    }

    public void InjectReceived(ReadOnlySpan<byte> bytes)
    {
        byte[] copy = bytes.ToArray();
        if (_decodePacketsOnWorker && _decodeQueue is not null && !_decodeQueue.IsAddingCompleted)
        {
            _decodeQueue.Add(copy);
            return;
        }

        DecodeBytes(copy);
    }

    protected virtual void OnPacket(NetworkPacketEnvelope packet)
    {
    }

    protected virtual void OnPacketError(NetworkPacketEnvelope packet, Exception exception)
    {
        RaiseDisconnected($"packet {packet.PacketId} error: {exception.Message}");
    }

    protected void Send(cPacket packet)
    {
        Send(0, packet);
    }

    protected void Send(ushort packetId, cPacket packet)
    {
        byte[] frame = _codec.Encode(packetId, packet);
        if (!_useRealSockets)
        {
            return;
        }

        _sendQueue.Enqueue(frame);
        _sendSignal.Release();
    }

    private void StartDecodeWorker(CancellationToken cancellationToken)
    {
        _decodeQueue = new BlockingCollection<byte[]>();
        _decodeTask = Task.Run(() =>
        {
            try
            {
                foreach (byte[] chunk in _decodeQueue.GetConsumingEnumerable(cancellationToken))
                {
                    DecodeBytes(chunk);
                }
            }
            catch (OperationCanceledException)
            {
            }
        }, cancellationToken);
    }

    private async Task RunSocketAsync(CancellationToken cancellationToken)
    {
        try
        {
            using var socket = new TcpClient();
            _socket = socket;
            await socket.ConnectAsync(Ip, Port, cancellationToken).ConfigureAwait(false);
            OnConnected();

            NetworkStream stream = socket.GetStream();
            Task receive = RunReceiveLoopAsync(stream, cancellationToken);
            Task send = RunSendLoopAsync(stream, cancellationToken);
            Task completed = await Task.WhenAny(receive, send).ConfigureAwait(false);
            await completed.ConfigureAwait(false);
        }
        catch (OperationCanceledException)
        {
        }
        catch (SocketException ex)
        {
            RaiseDisconnected($"socket error {ex.SocketErrorCode}: {ex.Message}");
        }
        catch (IOException ex)
        {
            RaiseDisconnected($"socket io error: {ex.Message}");
        }
        catch (ObjectDisposedException)
        {
        }
        finally
        {
            IsStarted = false;
        }
    }

    private async Task RunReceiveLoopAsync(NetworkStream stream, CancellationToken cancellationToken)
    {
        byte[] buffer = new byte[8192];
        while (!cancellationToken.IsCancellationRequested)
        {
            int read = await stream.ReadAsync(buffer, cancellationToken).ConfigureAwait(false);
            if (read == 0)
            {
                RaiseDisconnected("remote socket closed");
                return;
            }

            byte[] chunk = buffer.AsSpan(0, read).ToArray();
            if (_decodePacketsOnWorker && _decodeQueue is not null && !_decodeQueue.IsAddingCompleted)
            {
                _decodeQueue.Add(chunk, cancellationToken);
            }
            else
            {
                DecodeBytes(chunk);
            }
        }
    }

    private async Task RunSendLoopAsync(NetworkStream stream, CancellationToken cancellationToken)
    {
        while (!cancellationToken.IsCancellationRequested)
        {
            await _sendSignal.WaitAsync(cancellationToken).ConfigureAwait(false);
            while (_sendQueue.TryDequeue(out byte[]? frame))
            {
                await stream.WriteAsync(frame, cancellationToken).ConfigureAwait(false);
            }
        }
    }

    private void DecodeBytes(byte[] bytes)
    {
        try
        {
            lock (_decodeLock)
            {
                _receiveBuffer.AddRange(bytes);
                var decoded = new Queue<NetworkPacketEnvelope>();
                _codec.TryDecode(_receiveBuffer, Name, ref _sequence, decoded);
                while (decoded.TryDequeue(out NetworkPacketEnvelope? packet))
                {
                    _dispatchQueue.Enqueue(packet);
                }
            }
        }
        catch (InvalidOperationException ex)
        {
            RaiseDisconnected($"decode error: {ex.Message}");
        }
    }

    private void RaiseDisconnected(string message)
    {
        if (Interlocked.Exchange(ref _disconnectRaised, 1) == 1)
        {
            return;
        }

        OnDisconnected(message);
    }

    public void Dispose()
    {
        Stop();
        _cts?.Dispose();
        _sendSignal.Dispose();
        _decodeQueue?.Dispose();
    }
}
