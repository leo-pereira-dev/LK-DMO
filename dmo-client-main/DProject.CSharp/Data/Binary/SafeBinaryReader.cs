using System.Buffers.Binary;
using System.Text;

namespace DProject.Data.Binary;

public sealed class SafeBinaryReader
{
    private readonly ReadOnlyMemory<byte> _data;

    public SafeBinaryReader(ReadOnlyMemory<byte> data)
    {
        _data = data;
    }

    public int Position { get; private set; }

    public int Length => _data.Length;

    public int Remaining => Length - Position;

    public bool EndOfData => Remaining == 0;

    public bool TryReadInt32(out int value)
    {
        value = 0;
        if (Remaining < sizeof(int))
        {
            return false;
        }

        value = BinaryPrimitives.ReadInt32LittleEndian(_data.Span.Slice(Position, sizeof(int)));
        Position += sizeof(int);
        return true;
    }

    public bool TryReadUInt32(out uint value)
    {
        value = 0;
        if (Remaining < sizeof(uint))
        {
            return false;
        }

        value = BinaryPrimitives.ReadUInt32LittleEndian(_data.Span.Slice(Position, sizeof(uint)));
        Position += sizeof(uint);
        return true;
    }

    public bool TryReadInt64(out long value)
    {
        value = 0;
        if (Remaining < sizeof(long))
        {
            return false;
        }

        value = BinaryPrimitives.ReadInt64LittleEndian(_data.Span.Slice(Position, sizeof(long)));
        Position += sizeof(long);
        return true;
    }

    public bool TryReadUInt16(out ushort value)
    {
        value = 0;
        if (Remaining < sizeof(ushort))
        {
            return false;
        }

        value = BinaryPrimitives.ReadUInt16LittleEndian(_data.Span.Slice(Position, sizeof(ushort)));
        Position += sizeof(ushort);
        return true;
    }

    public bool TryReadInt16(out short value)
    {
        value = 0;
        if (Remaining < sizeof(short))
        {
            return false;
        }

        value = BinaryPrimitives.ReadInt16LittleEndian(_data.Span.Slice(Position, sizeof(short)));
        Position += sizeof(short);
        return true;
    }

    public bool TryReadSingle(out float value)
    {
        value = 0;
        if (Remaining < sizeof(float))
        {
            return false;
        }

        int bits = BinaryPrimitives.ReadInt32LittleEndian(_data.Span.Slice(Position, sizeof(float)));
        value = BitConverter.Int32BitsToSingle(bits);
        Position += sizeof(float);
        return true;
    }

    public bool TryReadDouble(out double value)
    {
        value = 0;
        if (Remaining < sizeof(double))
        {
            return false;
        }

        long bits = BinaryPrimitives.ReadInt64LittleEndian(_data.Span.Slice(Position, sizeof(double)));
        value = BitConverter.Int64BitsToDouble(bits);
        Position += sizeof(double);
        return true;
    }

    public bool TryReadBoolean(out bool value)
    {
        value = false;
        if (!TryReadByte(out byte byteValue))
        {
            return false;
        }

        value = byteValue != 0;
        return true;
    }

    public bool TryReadByte(out byte value)
    {
        value = 0;
        if (Remaining < 1)
        {
            return false;
        }

        value = _data.Span[Position];
        Position++;
        return true;
    }

    public bool TryReadBytes(int count, out ReadOnlyMemory<byte> bytes)
    {
        bytes = default;
        if (count < 0 || Remaining < count)
        {
            return false;
        }

        bytes = _data.Slice(Position, count);
        Position += count;
        return true;
    }

    public bool TrySkip(int count)
    {
        if (count < 0 || Remaining < count)
        {
            return false;
        }

        Position += count;
        return true;
    }

    public bool TrySeek(int position)
    {
        if ((uint)position > (uint)Length)
        {
            return false;
        }

        Position = position;
        return true;
    }

    public string ReadFixedAnsiString(ReadOnlySpan<byte> bytes)
    {
        int end = bytes.IndexOf((byte)0);
        ReadOnlySpan<byte> slice = end >= 0 ? bytes[..end] : bytes;
        return Encoding.Default.GetString(slice);
    }

    public string ReadFixedUtf16String(ReadOnlySpan<byte> bytes)
    {
        int end = -1;
        for (int i = 0; i + 1 < bytes.Length; i += 2)
        {
            if (bytes[i] == 0 && bytes[i + 1] == 0)
            {
                end = i;
                break;
            }
        }

        ReadOnlySpan<byte> slice = end >= 0 ? bytes[..end] : bytes;
        return Encoding.Unicode.GetString(slice);
    }
}
