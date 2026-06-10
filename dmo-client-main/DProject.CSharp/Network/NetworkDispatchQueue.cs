using System.Collections.Concurrent;

namespace DProject.Network;

public sealed class NetworkDispatchQueue
{
    private readonly ConcurrentQueue<NetworkPacketEnvelope> _queue = new();

    public int Count => _queue.Count;

    public void Enqueue(NetworkPacketEnvelope packet)
    {
        _queue.Enqueue(packet);
    }

    public int Drain(int maxPackets, Action<NetworkPacketEnvelope> dispatch)
    {
        int processed = 0;
        while (processed < maxPackets && _queue.TryDequeue(out NetworkPacketEnvelope? packet))
        {
            dispatch(packet);
            processed++;
        }

        return processed;
    }
}
