using System.Buffers.Binary;
using System.Text;

namespace DProject.Data.CsFilePack;

public enum PackMetadataStatus
{
    Loaded,
    Missing,
    Invalid
}

public sealed record PackHashHeader(uint Version, int FileCount);

public sealed record PackFileHeader(uint Version, uint Flag);

public sealed record PackHashInfo(
    int Flag,
    uint DataSize,
    uint AllocSize,
    uint Hash,
    ulong Offset);

public sealed class PackPackageMetadata
{
    private const int FileHashVersion = 0x00000010;
    private const int FileHashVersionSecurity = 0x00000011;
    private const int FileHashVersionSecurityNew = 0x00000012;
    private const int FilePackVersion = 0x00000010;
    private const int HashInfoSize = 24;
    private const int ChunkSize = 268;
    private const byte ChunkXorValue = 0xd0;

    private readonly Dictionary<uint, PackHashInfo> _hashMap = new();

    private PackPackageMetadata(string name, string basePath)
    {
        Name = name;
        BasePath = basePath;
        PackPath = basePath + ".pf";
        HashPath = basePath + ".hf";
    }

    public string Name { get; }
    public string BasePath { get; }
    public string PackPath { get; }
    public string HashPath { get; }
    public PackMetadataStatus Status { get; private set; }
    public PackHashHeader? HashHeader { get; private set; }
    public PackFileHeader? PackHeader { get; private set; }
    public string? ErrorMessage { get; private set; }
    public IReadOnlyDictionary<uint, PackHashInfo> HashMap => _hashMap;

    public static PackPackageMetadata Load(string name, string basePath, bool allowUnsupportedHashFormat)
    {
        var metadata = new PackPackageMetadata(name, basePath);
        metadata.LoadInternal(allowUnsupportedHashFormat);
        return metadata;
    }

    public bool Contains(string path)
    {
        uint hash = CsFPSystem.GetHashCode(path);
        return hash != 0 && _hashMap.ContainsKey(hash);
    }

    public bool TryGetHashInfo(string path, out PackHashInfo hashInfo)
    {
        uint hash = CsFPSystem.GetHashCode(path);
        return _hashMap.TryGetValue(hash, out hashInfo!);
    }

    public bool TryReadFile(string path, out byte[] data)
    {
        data = [];
        if (!TryGetHashInfo(path, out PackHashInfo hashInfo))
        {
            return false;
        }

        return TryReadFile(hashInfo, out data);
    }

    public bool TryReadFile(PackHashInfo hashInfo, out byte[] data)
    {
        data = [];
        if (hashInfo.DataSize == 0 || !File.Exists(PackPath))
        {
            return false;
        }

        using var stream = File.OpenRead(PackPath);
        if (hashInfo.Offset > (ulong)stream.Length ||
            hashInfo.DataSize > stream.Length - (long)hashInfo.Offset)
        {
            return false;
        }

        data = new byte[hashInfo.DataSize];
        stream.Position = (long)hashInfo.Offset;
        return stream.Read(data, 0, data.Length) == data.Length;
    }

    public IReadOnlyList<string> GetFileList()
    {
        if (!File.Exists(PackPath))
        {
            return [];
        }

        var files = new List<string>();
        using var stream = File.OpenRead(PackPath);
        foreach (PackHashInfo hashInfo in _hashMap.Values)
        {
            if (TryReadChunkPath(stream, hashInfo.Offset, out string? path) && path is not null)
            {
                files.Add(path);
            }
        }

        return files;
    }

    private void LoadInternal(bool allowUnsupportedHashFormat)
    {
        if (!File.Exists(PackPath) || !File.Exists(HashPath))
        {
            Status = PackMetadataStatus.Missing;
            ErrorMessage = "Pack or hash file is missing.";
            return;
        }

        if (!TryLoadPackHeader())
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage ??= "Invalid pack header.";
            return;
        }

        if (!TryLoadHashFile())
        {
            return;
        }

        Status = PackMetadataStatus.Loaded;
    }

    private bool TryLoadPackHeader()
    {
        using var stream = File.OpenRead(PackPath);
        if (stream.Length < 8)
        {
            ErrorMessage = "Pack file is shorter than FPHeader.";
            return false;
        }

        Span<byte> buffer = stackalloc byte[8];
        stream.ReadExactly(buffer);
        uint version = BinaryPrimitives.ReadUInt32LittleEndian(buffer[..4]);
        uint flag = BinaryPrimitives.ReadUInt32LittleEndian(buffer[4..8]);
        PackHeader = new PackFileHeader(version, flag);

        if (version != FilePackVersion)
        {
            ErrorMessage = $"Unsupported pack version 0x{version:x8}.";
            return false;
        }

        return true;
    }

    private bool TryLoadHashFile()
    {
        using var stream = File.OpenRead(HashPath);
        if (stream.Length < 8)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Hash file is shorter than FHHeader.";
            return false;
        }

        Span<byte> header = stackalloc byte[8];
        stream.ReadExactly(header);
        uint version = BinaryPrimitives.ReadUInt32LittleEndian(header[..4]);

        if (version == FileHashVersion)
        {
            int fileCount = BinaryPrimitives.ReadInt32LittleEndian(header[4..8]);
            HashHeader = new PackHashHeader(version, fileCount);
            return TryLoadPlainHashEntries(stream, fileCount);
        }

        if (version is FileHashVersionSecurity or FileHashVersionSecurityNew)
        {
            uint protectedSize = BinaryPrimitives.ReadUInt32LittleEndian(header[4..8]);
            return TryLoadProtectedHashEntries(stream, version, protectedSize);
        }

        Status = PackMetadataStatus.Invalid;
        ErrorMessage = $"Unsupported hash version 0x{version:x8}.";
        HashHeader = new PackHashHeader(version, 0);
        return false;
    }

    private bool TryLoadProtectedHashEntries(Stream stream, uint version, uint protectedSize)
    {
        if (protectedSize == 0 || protectedSize > stream.Length - stream.Position)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Protected hash payload size is invalid.";
            HashHeader = new PackHashHeader(version, 0);
            return false;
        }

        byte[] protectedPayload = new byte[protectedSize];
        stream.ReadExactly(protectedPayload);

        if (!PackSecurity.TryDecryptProtectedHash(version, protectedPayload, out byte[] plainPayload))
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = $"Protected hash version 0x{version:x8} decrypt/decompress failed.";
            HashHeader = new PackHashHeader(version, 0);
            return false;
        }

        if (plainPayload.Length < sizeof(int))
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Protected hash payload is shorter than file count.";
            HashHeader = new PackHashHeader(version, 0);
            return false;
        }

        int fileCount = BinaryPrimitives.ReadInt32LittleEndian(plainPayload.AsSpan(0, sizeof(int)));
        HashHeader = new PackHashHeader(version, fileCount);
        return TryLoadProtectedHashEntries(plainPayload.AsSpan(sizeof(int)), fileCount);
    }

    private bool TryLoadProtectedHashEntries(ReadOnlySpan<byte> entries, int fileCount)
    {
        if (fileCount < 0)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Negative file count in protected hash file.";
            return false;
        }

        long requiredBytes = (long)fileCount * HashInfoSize;
        if (requiredBytes > entries.Length)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Protected hash payload ended before all sINFONew entries.";
            return false;
        }

        for (int i = 0; i < fileCount; i++)
        {
            ReadOnlySpan<byte> entry = entries.Slice(i * HashInfoSize, HashInfoSize);
            uint hash = BinaryPrimitives.ReadUInt32LittleEndian(entry[..4]);
            int flag = BinaryPrimitives.ReadInt32LittleEndian(entry[4..8]);
            ulong offset = BinaryPrimitives.ReadUInt64LittleEndian(entry[8..16]);
            uint dataSize = BinaryPrimitives.ReadUInt32LittleEndian(entry[16..20]);
            uint allocSize = BinaryPrimitives.ReadUInt32LittleEndian(entry[20..24]);

            if ((flag & 0xff) == 0x00000001)
            {
                _hashMap[hash] = new PackHashInfo(flag, dataSize, allocSize, hash, offset);
            }
        }

        return true;
    }

    private bool TryLoadPlainHashEntries(Stream stream, int fileCount)
    {
        if (fileCount < 0)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Negative file count in hash file.";
            return false;
        }

        long requiredBytes = (long)fileCount * HashInfoSize;
        if (requiredBytes > stream.Length - stream.Position)
        {
            Status = PackMetadataStatus.Invalid;
            ErrorMessage = "Hash file ended before all sINFO entries.";
            return false;
        }

        Span<byte> entry = stackalloc byte[HashInfoSize];
        for (int i = 0; i < fileCount; i++)
        {
            stream.ReadExactly(entry);
            int flag = BinaryPrimitives.ReadInt32LittleEndian(entry[..4]);
            uint dataSize = BinaryPrimitives.ReadUInt32LittleEndian(entry[4..8]);
            uint allocSize = BinaryPrimitives.ReadUInt32LittleEndian(entry[8..12]);
            uint hash = BinaryPrimitives.ReadUInt32LittleEndian(entry[12..16]);
            ulong offset = BinaryPrimitives.ReadUInt64LittleEndian(entry[16..24]);

            if ((flag & 0xff) == 0x00000001)
            {
                _hashMap[hash] = new PackHashInfo(flag, dataSize, allocSize, hash, offset);
            }
        }

        return true;
    }

    private static bool TryReadChunkPath(Stream stream, ulong dataOffset, out string? path)
    {
        path = null;
        if (dataOffset < ChunkSize || dataOffset > (ulong)stream.Length)
        {
            return false;
        }

        long chunkOffset = (long)dataOffset - ChunkSize;
        if (ChunkSize > stream.Length - chunkOffset)
        {
            return false;
        }

        byte[] chunk = new byte[ChunkSize];
        stream.Position = chunkOffset;
        stream.ReadExactly(chunk);
        int nameLen = BinaryPrimitives.ReadInt32LittleEndian(chunk.AsSpan(264, 4));
        if (nameLen <= 0 || nameLen > 260)
        {
            return false;
        }

        for (int i = 0; i < nameLen; i++)
        {
            chunk[i] ^= ChunkXorValue;
        }

        path = Encoding.Default.GetString(chunk, 0, nameLen);
        return true;
    }
}
