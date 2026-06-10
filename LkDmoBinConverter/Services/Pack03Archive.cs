using System.Buffers.Binary;
using System.IO;
using System.IO.Compression;

namespace LkDmoBinConverter.Services;

internal static class Pack03Archive
{
    public static string ResolveBasePath(string inputPath)
    {
        if (string.IsNullOrWhiteSpace(inputPath))
            throw new ArgumentException("Pack path is empty.");

        var path = inputPath.Trim().Trim('"');
        if (File.Exists(path))
        {
            var fileName = Path.GetFileName(path);
            if (fileName.Equals("Pack03.pf", StringComparison.OrdinalIgnoreCase) ||
                fileName.Equals("Pack03.hf", StringComparison.OrdinalIgnoreCase))
            {
                return Path.Combine(Path.GetDirectoryName(path)!, "Pack03");
            }
        }

        if (Directory.Exists(path))
        {
            var direct = Path.Combine(path, "Pack03");
            if (File.Exists(direct + ".pf") && File.Exists(direct + ".hf"))
                return direct;
        }

        if (File.Exists(path + ".pf") && File.Exists(path + ".hf"))
            return path;

        throw new FileNotFoundException($"Could not resolve Pack03 base path from: {inputPath}");
    }

    public static byte[] ReadFile(string packBasePath, string relativePath)
    {
        var basePath = ResolveBasePath(packBasePath);
        var entries = ReadHashEntries(basePath + ".hf");
        var hash = GetPackHash(relativePath);
        if (!entries.TryGetValue(hash, out var entry))
            throw new FileNotFoundException($"Pack entry not found: {relativePath} hash 0x{hash:X8}");

        using var fs = File.Open(basePath + ".pf", FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        fs.Position = (long)entry.Offset;
        var data = new byte[entry.DataSize];
        var read = fs.Read(data, 0, data.Length);
        if (read != data.Length)
            throw new EndOfStreamException($"Could not read full pack entry: {relativePath}");

        if (entry.AllocSize > entry.DataSize)
            return DecompressEntry(data, entry.AllocSize, relativePath);

        if (LooksLikeZlib(data) && TryDecompressEntry(data, out var decompressed))
            return decompressed;

        return data;
    }

    public static (string pfBackup, string hfBackup) PatchFiles(string packBasePath, IReadOnlyDictionary<string, string> sourceFiles)
    {
        var basePath = ResolveBasePath(packBasePath);
        var pfPath = basePath + ".pf";
        var hfPath = basePath + ".hf";
        var stamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        var pfBackup = pfPath + ".bak_binconverter_" + stamp;
        var hfBackup = hfPath + ".bak_binconverter_" + stamp;
        File.Copy(pfPath, pfBackup, overwrite: true);
        File.Copy(hfPath, hfBackup, overwrite: true);

        var entries = ReadHashEntries(hfPath);
        using (var fs = File.Open(pfPath, FileMode.Open, FileAccess.ReadWrite, FileShare.None))
        using (var writer = new BinaryWriter(fs))
        {
            foreach (var pair in sourceFiles)
            {
                var relative = pair.Key;
                var data = File.ReadAllBytes(pair.Value);
                fs.Position = fs.Length;
                WriteChunkHeader(writer, relative);
                var dataOffset = (ulong)fs.Position;
                writer.Write(data);
                var hash = GetPackHash(relative);
                entries[hash] = new PackEntry(1, (uint)data.Length, (uint)data.Length, hash, dataOffset);
            }
        }

        WriteHashEntries(hfPath, entries.Values.OrderBy(e => e.Hash).ToList());
        return (pfBackup, hfBackup);
    }

    public static uint GetPackHash(string path)
    {
        var lower = path.ToLowerInvariant();
        ulong result = 5381;
        foreach (var ch in lower)
        {
            if (ch is '.' or '\\')
                continue;

            result = ((result << 5) + result + (byte)ch) & 0xFFFFFFFF;
        }

        return (uint)result;
    }

    private static void WriteChunkHeader(BinaryWriter writer, string relativePath)
    {
        var bytes = System.Text.Encoding.ASCII.GetBytes(relativePath);
        if (bytes.Length > 259)
            throw new InvalidDataException($"Pack path is too long: {relativePath}");

        var buffer = new byte[260];
        for (var i = 0; i < bytes.Length; i++)
            buffer[i] = (byte)(bytes[i] ^ 0xD0);

        writer.Write(buffer);
        writer.Write((uint)0);
        writer.Write(bytes.Length);
    }

    private static byte[] DecompressEntry(byte[] data, uint expectedSize, string relativePath)
    {
        if (!TryDecompressEntry(data, out var result))
            throw new InvalidDataException($"Pack entry {relativePath} could not be decompressed.");

        if (result.Length != expectedSize)
            throw new InvalidDataException($"Pack entry {relativePath} decompressed to {result.Length:N0} bytes, expected {expectedSize:N0}.");

        return result;
    }

    private static bool LooksLikeZlib(byte[] data)
    {
        if (data.Length < 2 || data[0] != 0x78)
            return false;

        var header = (data[0] << 8) + data[1];
        return header % 31 == 0;
    }

    private static bool TryDecompressEntry(byte[] data, out byte[] result)
    {
        try
        {
            using var input = new MemoryStream(data);
            using var zlib = new ZLibStream(input, CompressionMode.Decompress);
            using var output = new MemoryStream();
            zlib.CopyTo(output);
            result = output.ToArray();
            return result.Length > 0;
        }
        catch (InvalidDataException)
        {
            result = [];
            return false;
        }
    }

    private static Dictionary<uint, PackEntry> ReadHashEntries(string hfPath)
    {
        var bytes = File.ReadAllBytes(hfPath);
        var version = BinaryPrimitives.ReadUInt32LittleEndian(bytes.AsSpan(0, 4));
        if (version != 0x10)
            throw new InvalidDataException($"Unsupported Pack03 hash version 0x{version:X8}.");

        var count = BinaryPrimitives.ReadInt32LittleEndian(bytes.AsSpan(4, 4));
        var entries = new Dictionary<uint, PackEntry>();
        for (var i = 0; i < count; i++)
        {
            var offset = 8 + (i * 24);
            var plag = BinaryPrimitives.ReadInt32LittleEndian(bytes.AsSpan(offset, 4));
            var dataSize = BinaryPrimitives.ReadUInt32LittleEndian(bytes.AsSpan(offset + 4, 4));
            var allocSize = BinaryPrimitives.ReadUInt32LittleEndian(bytes.AsSpan(offset + 8, 4));
            var hash = BinaryPrimitives.ReadUInt32LittleEndian(bytes.AsSpan(offset + 12, 4));
            var dataOffset = BinaryPrimitives.ReadUInt64LittleEndian(bytes.AsSpan(offset + 16, 8));
            entries[hash] = new PackEntry(plag, dataSize, allocSize, hash, dataOffset);
        }

        return entries;
    }

    private static void WriteHashEntries(string hfPath, IReadOnlyList<PackEntry> entries)
    {
        using var fs = File.Open(hfPath, FileMode.Create, FileAccess.Write, FileShare.None);
        using var writer = new BinaryWriter(fs);
        writer.Write((uint)0x10);
        writer.Write(entries.Count);
        foreach (var entry in entries)
        {
            writer.Write(entry.Plag);
            writer.Write(entry.DataSize);
            writer.Write(entry.AllocSize);
            writer.Write(entry.Hash);
            writer.Write(entry.Offset);
        }
    }

    private readonly record struct PackEntry(int Plag, uint DataSize, uint AllocSize, uint Hash, ulong Offset);
}
