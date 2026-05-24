using System.Buffers.Binary;

namespace DigitalWorldOnline.Game.Tests.Support;

internal sealed class PacketCursor
{
    private readonly byte[] _packet;

    public PacketCursor(byte[] packet, int offset = 0)
    {
        _packet = packet;
        Offset = offset;
    }

    public int Offset { get; private set; }

    public short ReadInt16()
    {
        var value = BinaryPrimitives.ReadInt16LittleEndian(_packet.AsSpan(Offset, sizeof(short)));
        Offset += sizeof(short);
        return value;
    }

    public ushort ReadUInt16()
    {
        var value = BinaryPrimitives.ReadUInt16LittleEndian(_packet.AsSpan(Offset, sizeof(ushort)));
        Offset += sizeof(ushort);
        return value;
    }

    public int ReadInt32()
    {
        var value = BinaryPrimitives.ReadInt32LittleEndian(_packet.AsSpan(Offset, sizeof(int)));
        Offset += sizeof(int);
        return value;
    }

    public long ReadInt64()
    {
        var value = BinaryPrimitives.ReadInt64LittleEndian(_packet.AsSpan(Offset, sizeof(long)));
        Offset += sizeof(long);
        return value;
    }

    public byte ReadByte()
    {
        return _packet[Offset++];
    }

    public void Skip(int bytes)
    {
        Offset += bytes;
    }
}
