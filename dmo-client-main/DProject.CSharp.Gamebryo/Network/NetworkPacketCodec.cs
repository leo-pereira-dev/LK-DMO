using System.Buffers.Binary;
using System.Runtime.InteropServices;

namespace DProject.Network;

public sealed class NetworkPacketCodec
{
    public const ushort ChecksumSeed = 6716;
    public const int LengthSize = sizeof(ushort);
    public const int PacketIdSize = sizeof(ushort);
    public const int ChecksumSize = sizeof(ushort);
    public const int MinimumFrameSize = LengthSize + PacketIdSize + ChecksumSize;

    private readonly int _maxFrameSize;

    public NetworkPacketCodec(int maxFrameSize = ushort.MaxValue)
    {
        _maxFrameSize = Math.Clamp(maxFrameSize, MinimumFrameSize, ushort.MaxValue);
    }

    public byte[] Encode(ushort packetId, cPacket packet)
    {
        byte[] payload = packet.ToArray();
        int totalLength = MinimumFrameSize + payload.Length;
        if (totalLength > _maxFrameSize)
        {
            throw new InvalidOperationException($"Packet {packetId} exceeds max frame size {_maxFrameSize}.");
        }

        byte[] frame = new byte[totalLength];
        BinaryPrimitives.WriteUInt16LittleEndian(frame.AsSpan(0, LengthSize), (ushort)totalLength);
        BinaryPrimitives.WriteUInt16LittleEndian(frame.AsSpan(LengthSize, PacketIdSize), packetId);
        payload.CopyTo(frame.AsSpan(LengthSize + PacketIdSize));
        BinaryPrimitives.WriteUInt16LittleEndian(frame.AsSpan(totalLength - ChecksumSize, ChecksumSize), BuildChecksum((ushort)totalLength));
        return frame;
    }

    public bool TryDecode(List<byte> receiveBuffer, string connectionName, ref long sequence, Queue<NetworkPacketEnvelope> packets)
    {
        bool decodedAny = false;

        while (receiveBuffer.Count >= LengthSize)
        {
            ushort frameLength = BinaryPrimitives.ReadUInt16LittleEndian(CollectionsMarshal.AsSpan(receiveBuffer)[..LengthSize]);
            if (frameLength < MinimumFrameSize)
            {
                throw new InvalidOperationException($"Invalid frame length {frameLength} from {connectionName}.");
            }

            if (frameLength > _maxFrameSize)
            {
                throw new InvalidOperationException($"Frame length {frameLength} from {connectionName} exceeds max {_maxFrameSize}.");
            }

            if (receiveBuffer.Count < frameLength)
            {
                break;
            }

            byte[] frame = receiveBuffer.GetRange(0, frameLength).ToArray();
            receiveBuffer.RemoveRange(0, frameLength);

            ushort packetId = BinaryPrimitives.ReadUInt16LittleEndian(frame.AsSpan(LengthSize, PacketIdSize));
            int payloadOffset = LengthSize + PacketIdSize;
            int payloadLength = frameLength - MinimumFrameSize;
            ushort receivedChecksum = BinaryPrimitives.ReadUInt16LittleEndian(frame.AsSpan(frameLength - ChecksumSize, ChecksumSize));
            bool checksumValid = receivedChecksum == BuildChecksum(frameLength);
            var payload = new cPacket(frame.AsSpan(payloadOffset, payloadLength));

            packets.Enqueue(new NetworkPacketEnvelope(
                connectionName,
                packetId,
                payload,
                Interlocked.Increment(ref sequence),
                DateTimeOffset.UtcNow,
                checksumValid,
                frameLength));

            decodedAny = true;
        }

        return decodedAny;
    }

    private static ushort BuildChecksum(ushort frameLength)
    {
        return (ushort)(frameLength ^ ChecksumSeed);
    }
}
