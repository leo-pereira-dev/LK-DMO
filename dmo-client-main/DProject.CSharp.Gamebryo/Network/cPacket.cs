using System.Buffers.Binary;
using System.Text;

namespace DProject.Network;

public sealed class cPacket
{
    private readonly List<byte> _buffer = new();
    private int _readOffset;

    public cPacket()
    {
    }

    public cPacket(ReadOnlySpan<byte> data)
    {
        _buffer.AddRange(data.ToArray());
    }

    public int Length => _buffer.Count;
    public int Remaining => _buffer.Count - _readOffset;

    public ReadOnlyMemory<byte> Data => _buffer.ToArray();

    public byte[] ToArray() => _buffer.ToArray();

    public void PushByte(byte value) => _buffer.Add(value);
    public void PushU1(byte value) => PushByte(value);
    public void PushS1(sbyte value) => PushByte(unchecked((byte)value));

    public void PushBytes(ReadOnlySpan<byte> value) => _buffer.AddRange(value.ToArray());

    public void PushInt16(short value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(short)];
        BinaryPrimitives.WriteInt16LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushUInt16(ushort value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(ushort)];
        BinaryPrimitives.WriteUInt16LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushU2(ushort value) => PushUInt16(value);
    public void PushS2(short value) => PushInt16(value);

    public void PushInt32(int value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(int)];
        BinaryPrimitives.WriteInt32LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushUInt32(uint value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(uint)];
        BinaryPrimitives.WriteUInt32LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushU4(uint value) => PushUInt32(value);
    public void PushS4(int value) => PushInt32(value);

    public void PushSingle(float value)
    {
        PushInt32(BitConverter.SingleToInt32Bits(value));
    }

    public void PushInt64(long value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(long)];
        BinaryPrimitives.WriteInt64LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushUInt64(ulong value)
    {
        Span<byte> bytes = stackalloc byte[sizeof(ulong)];
        BinaryPrimitives.WriteUInt64LittleEndian(bytes, value);
        _buffer.AddRange(bytes.ToArray());
    }

    public void PushU8(ulong value) => PushUInt64(value);
    public void PushS8(long value) => PushInt64(value);

    public void PushString(string value, bool skipEmpty = false, bool writeTerminator = false)
    {
        byte[] bytes = Encoding.UTF8.GetBytes(value);
        if (bytes.Length == 0 && skipEmpty)
        {
            return;
        }

        if (bytes.Length > byte.MaxValue)
        {
            bytes = bytes[..byte.MaxValue];
        }

        PushByte((byte)bytes.Length);
        PushBytes(bytes);
        if (writeTerminator)
        {
            PushByte(0);
        }
    }

    public byte PopByte()
    {
        EnsureAvailable(sizeof(byte));
        return _buffer[_readOffset++];
    }

    public byte PopU1() => PopByte();
    public sbyte PopS1() => unchecked((sbyte)PopByte());

    public byte[] PopBytes(int length)
    {
        EnsureAvailable(length);
        byte[] value = _buffer.GetRange(_readOffset, length).ToArray();
        _readOffset += length;
        return value;
    }

    public short PopInt16()
    {
        EnsureAvailable(sizeof(short));
        short value = BinaryPrimitives.ReadInt16LittleEndian(_buffer.GetRange(_readOffset, sizeof(short)).ToArray());
        _readOffset += sizeof(short);
        return value;
    }

    public ushort PopUInt16()
    {
        EnsureAvailable(sizeof(ushort));
        ushort value = BinaryPrimitives.ReadUInt16LittleEndian(_buffer.GetRange(_readOffset, sizeof(ushort)).ToArray());
        _readOffset += sizeof(ushort);
        return value;
    }

    public ushort PopU2() => PopUInt16();
    public short PopS2() => PopInt16();

    public int PopInt32()
    {
        EnsureAvailable(sizeof(int));
        int value = BinaryPrimitives.ReadInt32LittleEndian(_buffer.GetRange(_readOffset, sizeof(int)).ToArray());
        _readOffset += sizeof(int);
        return value;
    }

    public uint PopUInt32()
    {
        EnsureAvailable(sizeof(uint));
        uint value = BinaryPrimitives.ReadUInt32LittleEndian(_buffer.GetRange(_readOffset, sizeof(uint)).ToArray());
        _readOffset += sizeof(uint);
        return value;
    }

    public uint PopU4() => PopUInt32();
    public int PopS4() => PopInt32();

    public float PopSingle()
    {
        return BitConverter.Int32BitsToSingle(PopInt32());
    }

    public long PopInt64()
    {
        EnsureAvailable(sizeof(long));
        long value = BinaryPrimitives.ReadInt64LittleEndian(_buffer.GetRange(_readOffset, sizeof(long)).ToArray());
        _readOffset += sizeof(long);
        return value;
    }

    public ulong PopUInt64()
    {
        EnsureAvailable(sizeof(ulong));
        ulong value = BinaryPrimitives.ReadUInt64LittleEndian(_buffer.GetRange(_readOffset, sizeof(ulong)).ToArray());
        _readOffset += sizeof(ulong);
        return value;
    }

    public ulong PopU8() => PopUInt64();
    public long PopS8() => PopInt64();

    public string PopString(bool consumeTrailingNull = false)
    {
        int length = PopByte();
        byte[] bytes = PopBytes(length);
        if (consumeTrailingNull && Remaining > 0)
        {
            byte terminator = _buffer[_readOffset];
            if (terminator == 0)
            {
                _readOffset++;
            }
        }

        return Encoding.UTF8.GetString(bytes);
    }

    private void EnsureAvailable(int size)
    {
        if (Remaining < size)
        {
            throw new InvalidOperationException($"Packet underflow. Requested {size}, remaining {Remaining}.");
        }
    }
}
