using System.Buffers.Binary;
using System.IO;

namespace ItemEditor.Services;

public static class PackArchive
{
    public static string ResolveBasePath(string inputPath)
    {
        if (string.IsNullOrWhiteSpace(inputPath))
            throw new ArgumentException("Pack path is empty.");

        var path = inputPath.Trim().Trim('"');
        if (File.Exists(path))
        {
            var extension = Path.GetExtension(path);
            if (extension.Equals(".pf", StringComparison.OrdinalIgnoreCase) ||
                extension.Equals(".hf", StringComparison.OrdinalIgnoreCase))
            {
                return Path.Combine(Path.GetDirectoryName(path)!, Path.GetFileNameWithoutExtension(path));
            }
        }

        if (Directory.Exists(path))
        {
            foreach (var packName in new[] { "Pack01", "Pack03" })
            {
                var direct = Path.Combine(path, packName);
                if (HasPackPair(direct))
                    return direct;
            }
        }

        if (HasPackPair(path))
            return path;

        throw new FileNotFoundException($"Could not resolve pack base path from: {inputPath}");
    }

    public static bool LooksLikePack(string inputPath)
    {
        try
        {
            _ = ResolveBasePath(inputPath);
            return true;
        }
        catch
        {
            return false;
        }
    }

    public static byte[] ReadFile(string packBasePath, string relativePath)
    {
        var basePath = ResolveBasePath(packBasePath);
        var entries = ReadHashEntries(ResolveHashPath(basePath));
        var hash = GetPackHash(relativePath);
        if (!entries.TryGetValue(hash, out var entry))
            throw new FileNotFoundException($"Pack entry not found: {relativePath} hash 0x{hash:X8}");

        using var fs = File.Open(basePath + ".pf", FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        fs.Position = (long)entry.Offset;
        var data = new byte[entry.DataSize];
        var read = fs.Read(data, 0, data.Length);
        if (read != data.Length)
            throw new EndOfStreamException($"Could not read full pack entry: {relativePath}");

        return data;
    }

    public static IReadOnlyList<string> ListFiles(string packBasePath)
    {
        var basePath = ResolveBasePath(packBasePath);
        var entries = ReadHashEntries(ResolveHashPath(basePath));
        var result = new List<string>(entries.Count);

        using var fs = File.Open(basePath + ".pf", FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        var meta = new byte[8];
        foreach (var entry in entries.Values.OrderBy(x => x.Offset))
        {
            var headerOffset = checked((long)entry.Offset) - 268;
            if (headerOffset < 0 || headerOffset + 268 > fs.Length)
                continue;

            fs.Position = headerOffset;
            var encodedName = new byte[260];
            if (fs.Read(encodedName, 0, encodedName.Length) != encodedName.Length)
                continue;

            if (fs.Read(meta) != meta.Length)
                continue;

            var length = BinaryPrimitives.ReadInt32LittleEndian(meta[4..]);
            if (length <= 0 || length > encodedName.Length)
                continue;

            var nameBytes = new byte[length];
            for (var i = 0; i < length; i++)
                nameBytes[i] = (byte)(encodedName[i] ^ 0xD0);

            var relative = System.Text.Encoding.ASCII.GetString(nameBytes).TrimEnd('\0');
            if (relative.Length == 0)
                continue;

            if (GetPackHash(relative) == entry.Hash)
                result.Add(relative);
        }

        return result
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(x => x, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public static (string pfBackup, string hfBackup) PatchBytes(string packBasePath, IReadOnlyDictionary<string, byte[]> files)
    {
        var basePath = ResolveBasePath(packBasePath);
        var pfPath = basePath + ".pf";
        var hfPath = ResolveHashPath(basePath);
        var stamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        var pfBackup = pfPath + ".bak_itemeditor_" + stamp;
        var hfBackup = hfPath + ".bak_itemeditor_" + stamp;
        File.Copy(pfPath, pfBackup, overwrite: true);
        File.Copy(hfPath, hfBackup, overwrite: true);

        var entries = ReadHashEntries(hfPath);
        using (var fs = File.Open(pfPath, FileMode.Open, FileAccess.ReadWrite, FileShare.None))
        using (var writer = new BinaryWriter(fs))
        {
            foreach (var pair in files)
            {
                var relative = pair.Key;
                var data = pair.Value;
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

    private static bool HasPackPair(string basePath)
        => File.Exists(basePath + ".pf") && File.Exists(ResolveHashPathOrNull(basePath));

    private static string ResolveHashPath(string basePath)
        => ResolveHashPathOrNull(basePath) ??
           throw new FileNotFoundException($"Could not resolve pack hash file for: {basePath}");

    private static string? ResolveHashPathOrNull(string basePath)
    {
        var direct = basePath + ".hf";
        if (File.Exists(direct))
            return direct;

        var suffixed = basePath + "1.hf";
        if (File.Exists(suffixed))
            return suffixed;

        return null;
    }

    private static Dictionary<uint, PackEntry> ReadHashEntries(string hfPath)
    {
        var bytes = File.ReadAllBytes(hfPath);
        var version = BinaryPrimitives.ReadUInt32LittleEndian(bytes.AsSpan(0, 4));
        if (version != 0x10)
            throw new InvalidDataException($"Unsupported pack hash version 0x{version:X8}.");

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
