using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record WorldMapBinData(
    IReadOnlyList<WorldMapRecord> Worlds,
    IReadOnlyList<AreaMapRecord> Areas);

public sealed record WorldMapRecord(
    ushort WorldId,
    string Name,
    string Comment,
    byte WorldType,
    ushort UiX,
    ushort UiY);

public sealed record AreaMapRecord(
    ushort MapId,
    string Name,
    string Comment,
    byte AreaType,
    byte FieldType,
    int FieldDetail,
    ushort UiX,
    ushort UiY,
    float GaussianBlur1,
    float GaussianBlur2,
    float GaussianBlur3);

public sealed class WorldMapBinLoader : IBinLoader
{
    private const int WorldMapNameLen = 48;
    private const int WorldMapCommentLen = 256;
    private const int TCharSize = 2;
    private const int WorldInfoSize = 616;
    private const int AreaInfoSize = 632;
    private const int MaxReasonableCount = 100000;

    public string TableName => "CsWorldMapMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        BinFileSnapshot snapshot = CountPrefixedBinInspector.Inspect(TableName, binPath, data);
        if (!TryParse(data, out WorldMapBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out WorldMapBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!TryReadWorlds(reader, out List<WorldMapRecord>? worlds) ||
            worlds is null ||
            !TryReadAreas(reader, out List<AreaMapRecord>? areas) ||
            areas is null)
        {
            return false;
        }

        parsed = new WorldMapBinData(worlds, areas);
        return true;
    }

    private static bool TryReadWorlds(SafeBinaryReader reader, out List<WorldMapRecord>? worlds)
    {
        worlds = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * WorldInfoSize > reader.Remaining)
        {
            return false;
        }

        worlds = new List<WorldMapRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(WorldInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseWorld(recordBytes, out WorldMapRecord? world) ||
                world is null)
            {
                return false;
            }

            worlds.Add(world);
        }

        return true;
    }

    private static bool TryParseWorld(ReadOnlyMemory<byte> bytes, out WorldMapRecord? world)
    {
        world = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt16(out ushort worldId) ||
            !reader.TryReadBytes(WorldMapNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(WorldMapCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadByte(out byte worldType) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort uiX) ||
            !reader.TryReadUInt16(out ushort uiY))
        {
            return false;
        }

        world = new WorldMapRecord(
            worldId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span),
            worldType,
            uiX,
            uiY);
        return true;
    }

    private static bool TryReadAreas(SafeBinaryReader reader, out List<AreaMapRecord>? areas)
    {
        areas = null;
        if (!TryReadCount(reader, MaxReasonableCount, out int count) ||
            (long)count * AreaInfoSize > reader.Remaining)
        {
            return false;
        }

        areas = new List<AreaMapRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(AreaInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                !TryParseArea(recordBytes, out AreaMapRecord? area) ||
                area is null)
            {
                return false;
            }

            areas.Add(area);
        }

        return true;
    }

    private static bool TryParseArea(ReadOnlyMemory<byte> bytes, out AreaMapRecord? area)
    {
        area = null;
        var reader = new SafeBinaryReader(bytes);
        if (!reader.TryReadUInt16(out ushort mapId) ||
            !reader.TryReadBytes(WorldMapNameLen * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(WorldMapCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadByte(out byte areaType) ||
            !reader.TryReadByte(out byte fieldType) ||
            !reader.TryReadInt32(out int fieldDetail) ||
            !reader.TryReadUInt16(out ushort uiX) ||
            !reader.TryReadUInt16(out ushort uiY) ||
            !reader.TryReadSingle(out float gaussianBlur1) ||
            !reader.TryReadSingle(out float gaussianBlur2) ||
            !reader.TryReadSingle(out float gaussianBlur3))
        {
            return false;
        }

        area = new AreaMapRecord(
            mapId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedUtf16String(commentBytes.Span),
            areaType,
            fieldType,
            fieldDetail,
            uiX,
            uiY,
            gaussianBlur1,
            gaussianBlur2,
            gaussianBlur3);
        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, int maxCount, out int count)
    {
        count = 0;
        if (!reader.TryReadInt32(out int value) || value < 0 || value > maxCount)
        {
            return false;
        }

        count = value;
        return true;
    }
}
