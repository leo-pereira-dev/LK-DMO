using System.Buffers.Binary;
using System.IO.Compression;

namespace DProject.Data.CsFilePack;

internal enum PackSecurityKind : byte
{
    Xor = 1,
    Twist = 2,
    RvsFull = 8,
    RvsHalf = 16,
    Shuffle = 32
}

internal static class PackSecurity
{
    private const int MaxKeyVecSize = 16;
    private const uint XorKey = 127;

    public static bool TryDecryptProtectedHash(uint version, ReadOnlySpan<byte> encrypted, out byte[] uncompressed)
    {
        uncompressed = [];
        byte flags = version switch
        {
            0x00000011 => (byte)(PackSecurityKind.Xor | PackSecurityKind.Twist | PackSecurityKind.RvsFull),
            0x00000012 => (byte)(PackSecurityKind.Shuffle | PackSecurityKind.RvsFull | PackSecurityKind.Twist | (PackSecurityKind)0x04),
            _ => 0
        };

        if (flags == 0)
        {
            return false;
        }

        bool decrypted = version == 0x00000012
            ? TryDecryptV18(flags, encrypted, out byte[] compressed)
            : TryDecryptV17(flags, encrypted, out compressed);

        return decrypted && TryZlibUncompress(compressed, out uncompressed);
    }

    private static bool TryDecryptV17(byte flags, ReadOnlySpan<byte> input, out byte[] output)
    {
        byte[] current = input.ToArray();

        if (HasFlag(flags, PackSecurityKind.Shuffle))
        {
            if (!TryReadPackedUInt32Vector(current, out uint[] key, out byte[] data) || key.Length == 0)
            {
                output = [];
                return false;
            }

            current = TypeShuffle(key, data, encrypt: false);
        }

        if (HasFlag(flags, PackSecurityKind.RvsHalf))
        {
            current = TypeRvsHalf(current);
        }

        if (HasFlag(flags, PackSecurityKind.Xor))
        {
            if (!TryReadPackedUInt32Vector(current, out uint[] key, out byte[] data) || key.Length == 0)
            {
                output = [];
                return false;
            }

            current = TypeXor(key, data);
        }

        if (HasFlag(flags, PackSecurityKind.Twist))
        {
            current = TypeTwist(current, encrypt: false);
        }

        if (HasFlag(flags, PackSecurityKind.RvsFull))
        {
            current = TypeRvsFull(current);
        }

        output = current;
        return true;
    }

    private static bool TryDecryptV18(byte flags, ReadOnlySpan<byte> input, out byte[] output)
    {
        byte[] current = input.ToArray();

        if (HasFlag(flags, PackSecurityKind.Twist))
        {
            current = TypeTwist(current, encrypt: false);
        }

        if (HasFlag(flags, PackSecurityKind.Xor))
        {
            if (!TryReadPackedUInt32Vector(current, out uint[] key, out byte[] data) || key.Length == 0)
            {
                output = [];
                return false;
            }

            current = TypeXor(key, data);
        }

        if (HasFlag(flags, PackSecurityKind.RvsHalf))
        {
            current = TypeRvsHalf(current);
        }

        if (HasFlag(flags, PackSecurityKind.Shuffle))
        {
            if (!TryReadPackedUInt32Vector(current, out uint[] key, out byte[] data) || key.Length == 0)
            {
                output = [];
                return false;
            }

            current = TypeShuffle(key, data, encrypt: false);
        }

        if (HasFlag(flags, PackSecurityKind.RvsFull))
        {
            current = TypeRvsFull(current);
        }

        output = current;
        return true;
    }

    private static bool HasFlag(byte flags, PackSecurityKind kind)
    {
        return (flags & (byte)kind) != 0;
    }

    private static bool TryReadPackedUInt32Vector(ReadOnlySpan<byte> data, out uint[] key, out byte[] payload)
    {
        key = [];
        payload = [];
        int offset = 0;
        if (!TryReadUInt32Vector(data, ref offset, out key) ||
            !TryReadByteVector(data, ref offset, out payload))
        {
            return false;
        }

        return true;
    }

    private static bool TryReadUInt32Vector(ReadOnlySpan<byte> data, ref int offset, out uint[] values)
    {
        values = [];
        if (!TryReadSize(data, ref offset, out int count) ||
            count < 0 ||
            count > (data.Length - offset) / sizeof(uint))
        {
            return false;
        }

        values = new uint[count];
        for (int i = 0; i < count; i++)
        {
            values[i] = BinaryPrimitives.ReadUInt32LittleEndian(data.Slice(offset, sizeof(uint)));
            offset += sizeof(uint);
        }

        return true;
    }

    private static bool TryReadByteVector(ReadOnlySpan<byte> data, ref int offset, out byte[] values)
    {
        values = [];
        if (!TryReadSize(data, ref offset, out int count) ||
            count < 0 ||
            count > data.Length - offset)
        {
            return false;
        }

        values = data.Slice(offset, count).ToArray();
        offset += count;
        return true;
    }

    private static bool TryReadSize(ReadOnlySpan<byte> data, ref int offset, out int count)
    {
        count = 0;
        if (offset < 0 || data.Length - offset < sizeof(uint))
        {
            return false;
        }

        uint raw = BinaryPrimitives.ReadUInt32LittleEndian(data.Slice(offset, sizeof(uint)));
        if (raw > int.MaxValue)
        {
            return false;
        }

        count = (int)raw;
        offset += sizeof(uint);
        return true;
    }

    private static byte[] TypeXor(IReadOnlyList<uint> key, ReadOnlySpan<byte> input)
    {
        byte[] output = new byte[input.Length];
        int blockCount = input.Length / sizeof(uint);
        int remain = input.Length % sizeof(uint);

        for (int i = 0; i < blockCount; i++)
        {
            uint source = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice(i * sizeof(uint), sizeof(uint)));
            uint value = (source ^ key[i % key.Count]) ^ XorKey;
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan(i * sizeof(uint), sizeof(uint)), value);
        }

        if (remain != 0)
        {
            Span<byte> tmp = stackalloc byte[sizeof(uint)];
            int lack = sizeof(uint) - remain;
            input[^remain..].CopyTo(tmp[lack..]);
            uint value = BinaryPrimitives.ReadUInt32LittleEndian(tmp) ^ key[blockCount % key.Count];
            BinaryPrimitives.WriteUInt32LittleEndian(tmp, value);
            tmp[lack..].CopyTo(output.AsSpan(output.Length - remain));
        }

        return output;
    }

    private static byte[] TypeTwist(ReadOnlySpan<byte> input, bool encrypt)
    {
        byte[] output = new byte[input.Length];
        int blockCount = input.Length / sizeof(uint);
        int remain = input.Length % sizeof(uint);
        const int maxBit = sizeof(uint) * 8;

        for (int i = 0, shift = 1; i < blockCount; i++, shift++)
        {
            if (shift >= maxBit)
            {
                shift = 1;
            }

            uint source = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice(i * sizeof(uint), sizeof(uint)));
            uint value = encrypt
                ? (source << shift) | ((source & (0xffffffffu << (maxBit - shift))) >> (maxBit - shift))
                : (source >> shift) | ((source & (0xffffffffu >> (maxBit - shift))) << (maxBit - shift));
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan(i * sizeof(uint), sizeof(uint)), value);
        }

        if (remain != 0)
        {
            input[^remain..].CopyTo(output.AsSpan(output.Length - remain));
        }

        return output;
    }

    private static byte[] TypeRvsFull(ReadOnlySpan<byte> input)
    {
        byte[] output = new byte[input.Length];
        int blockCount = input.Length / sizeof(uint);
        int remain = input.Length % sizeof(uint);
        bool odd = (blockCount % 2) != 0;

        for (int i = 0; i < blockCount / 2; i++)
        {
            uint front = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice(i * sizeof(uint), sizeof(uint)));
            uint back = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice((blockCount - 1 - i) * sizeof(uint), sizeof(uint)));
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan(i * sizeof(uint), sizeof(uint)), ~back);
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan((blockCount - 1 - i) * sizeof(uint), sizeof(uint)), ~front);
        }

        if (odd)
        {
            int middle = blockCount / 2;
            uint source = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice(middle * sizeof(uint), sizeof(uint)));
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan(middle * sizeof(uint), sizeof(uint)), ~source);
        }

        if (remain != 0)
        {
            input[^remain..].CopyTo(output.AsSpan(output.Length - remain));
        }

        return output;
    }

    private static byte[] TypeRvsHalf(ReadOnlySpan<byte> input)
    {
        byte[] output = new byte[input.Length];
        int blockCount = input.Length / sizeof(uint);
        int remain = input.Length % sizeof(uint);

        for (int i = 0; i < blockCount; i++)
        {
            uint source = BinaryPrimitives.ReadUInt32LittleEndian(input.Slice(i * sizeof(uint), sizeof(uint)));
            uint mask = (i % 2) == 0 ? 0xffff0000u : 0x0000ffffu;
            uint masked = source & mask;
            uint value = (source & ~mask) | (~masked & mask);
            BinaryPrimitives.WriteUInt32LittleEndian(output.AsSpan(i * sizeof(uint), sizeof(uint)), value);
        }

        if (remain != 0)
        {
            input[^remain..].CopyTo(output.AsSpan(output.Length - remain));
        }

        return output;
    }

    private static byte[] TypeShuffle(IReadOnlyList<uint> key, ReadOnlySpan<byte> input, bool encrypt)
    {
        byte[] output = input.ToArray();
        byte[] keyBytes = new byte[key.Count * sizeof(uint)];
        for (int i = 0; i < key.Count; i++)
        {
            BinaryPrimitives.WriteUInt32LittleEndian(keyBytes.AsSpan(i * sizeof(uint), sizeof(uint)), key[i]);
        }

        int shuffleBlockCount = output.Length / MaxKeyVecSize;
        for (int block = 0; block < shuffleBlockCount; block++)
        {
            Span<byte> tinyBlock = output.AsSpan(block * MaxKeyVecSize, MaxKeyVecSize);
            if (!encrypt)
            {
                for (int j = 0; j < keyBytes.Length; j++)
                {
                    Swap(tinyBlock, keyBytes[j] & 0x0f, (keyBytes[j] & 0xf0) >> 4);
                }
            }
            else
            {
                for (int j = 0; j < keyBytes.Length; j++)
                {
                    byte keyByte = keyBytes[keyBytes.Length - 1 - j];
                    Swap(tinyBlock, (keyByte & 0xf0) >> 4, keyByte & 0x0f);
                }
            }
        }

        return output;
    }

    private static void Swap(Span<byte> buffer, int left, int right)
    {
        (buffer[left], buffer[right]) = (buffer[right], buffer[left]);
    }

    private static bool TryZlibUncompress(byte[] compressed, out byte[] uncompressed)
    {
        uncompressed = [];
        if (compressed.Length == 0)
        {
            return false;
        }

        try
        {
            using var input = new MemoryStream(compressed);
            using var zlib = new ZLibStream(input, CompressionMode.Decompress);
            using var output = new MemoryStream();
            zlib.CopyTo(output);
            uncompressed = output.ToArray();
            return uncompressed.Length != 0;
        }
        catch (InvalidDataException)
        {
            return false;
        }
    }
}
