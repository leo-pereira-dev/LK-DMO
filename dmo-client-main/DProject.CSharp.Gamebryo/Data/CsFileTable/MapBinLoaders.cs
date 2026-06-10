using System.Text;
using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record MapListBinData(IReadOnlyList<MapListRecord> Maps);

public sealed record MapListRecord(
    uint MapId,
    string MapName,
    string MapPath,
    string BgSound,
    int Width,
    int Height,
    string Description,
    string EnglishDescription,
    uint ResurrectionMapId,
    ushort MapRegionId,
    ushort FatigueType,
    ushort FatigueDebuff,
    ushort FatigueStartTime,
    ushort FatigueAddTime,
    short FatigueAddPoint,
    short CameraMaxLevel,
    bool XgConsumeType,
    bool BattleTagUse);

public sealed record MapPortalBinData(IReadOnlyList<MapPortalRecord> Portals);

public sealed record MapPortalRecord(
    uint PortalId,
    uint PortalType,
    uint SourceMapId,
    int SourceTargetX,
    int SourceTargetY,
    int SourceRadius,
    uint DestinationMapId,
    int DestinationTargetX,
    int DestinationTargetY,
    int DestinationRadius,
    int PortalActorType,
    uint UniqueObjectId,
    int PortalKindIndex,
    int ViewTargetX,
    int ViewTargetY);

public sealed record MapNpcBinData(IReadOnlyList<MapNpcRecord> Npcs);

public sealed record MapNpcRecord(
    uint NpcId,
    uint MapId,
    int InitPosX,
    int InitPosY,
    float Rotation);

public sealed record MapMonsterBinData(IReadOnlyList<MapMonsterRecord> Monsters);

public sealed record MapMonsterRecord(
    uint FileTableId,
    uint MapGroupId,
    uint MapId,
    uint MonsterTableId,
    int CenterX,
    int CenterY,
    int Radius,
    int Count,
    int RespawnTime,
    int KillgenMonsterFileTableId,
    int KillgenCount,
    int KillgenViewCount,
    int MoveType,
    bool InstanceRespawn);

public sealed record MapRegionBinData(
    IReadOnlyList<MapRegionRecord> Regions,
    IReadOnlyList<MapLimitEvoRecord> LimitEvoRegions,
    IReadOnlyList<MapBuffRegionRecord> BuffRegions);

public sealed record MapRegionRecord(
    uint MapId,
    int CenterX,
    int CenterY,
    int Radius,
    string Description,
    string EnglishDescription,
    string BgSound,
    ushort FatigueType,
    ushort FatigueDebuff,
    ushort FatigueStartTime,
    ushort FatigueAddTime,
    short FatigueAddPoint);

public sealed record MapLimitEvoRecord(
    uint MapId,
    int CenterX,
    int CenterY,
    int Radius);

public sealed record MapBuffRegionRecord(
    int RegionIndex,
    uint MapId,
    int CenterX,
    int CenterY,
    int Radius,
    int Act,
    int BuffIndex,
    int Time,
    int LayerNo,
    int ApplyType,
    int Requirement1,
    int Requirement2,
    int ObjectUniqueId);

public sealed record MapCharLightBinData(IReadOnlyList<MapCharLightRecord> Lights);

public sealed record MapCharLightRecord(
    uint MapId,
    bool Char,
    bool Shadow,
    int Type,
    bool EnableLight,
    IReadOnlyList<float> Diffuse,
    IReadOnlyList<float> Ambient,
    IReadOnlyList<float> Specular,
    float Power,
    IReadOnlyList<float> Rotation,
    IReadOnlyList<float> Position,
    float Scale,
    float Constant,
    float Linear,
    float Quadratic,
    float Range);

public sealed record WeatherBinData(IReadOnlyList<WeatherRecord> Weathers);

public sealed record WeatherRecord(
    uint MapId,
    int CenterX,
    int CenterY,
    int Radius,
    uint TotalLoopMinutes,
    IReadOnlyList<WeatherElementRecord> Elements);

public sealed record WeatherElementRecord(
    int TimeMinutes,
    int Fatigue,
    byte Type,
    byte Level);

public sealed record MapObjectBinData(IReadOnlyList<MapObjectGroupRecord> Groups);

public sealed record MapObjectGroupRecord(
    uint MapId,
    IReadOnlyList<MapObjectEntryRecord> Objects);

public sealed record MapObjectEntryRecord(
    uint ObjectId,
    IReadOnlyList<MapObjectInstanceRecord> Instances);

public sealed record MapObjectInstanceRecord(
    int OrderIndex,
    IReadOnlyList<MapObjectFactorRecord> Factors);

public sealed record MapObjectFactorRecord(
    int OpenType,
    int FactorCount,
    uint Factor);

public sealed record MapTableObjBinData(IReadOnlyList<MapTableObjGroupRecord> Groups);

public sealed record MapTableObjGroupRecord(
    uint MapId,
    IReadOnlyList<MapTableObjNodeRecord> Nodes);

public sealed record MapTableObjNodeRecord(
    int NodeIndex,
    IReadOnlyList<MapTableObjRecord> Objects);

public sealed record MapTableObjRecord(ReadOnlyMemory<byte> Data);

internal static class MapBinFormat
{
    public const int MaxReasonableCount = 100000;
    public const int MaxStringLength = 2048;

    public static bool TryReadCount(SafeBinaryReader reader, out int count)
    {
        count = 0;
        return reader.TryReadInt32(out count) &&
            count >= 0 &&
            count <= MaxReasonableCount;
    }

    public static bool TryReadAnsiString(SafeBinaryReader reader, out string value)
    {
        value = string.Empty;
        if (!TryReadCount(reader, out int length) ||
            length > MaxStringLength ||
            !reader.TryReadBytes(length, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        value = Encoding.Default.GetString(bytes.Span);
        return true;
    }

    public static bool TryReadUtf16String(SafeBinaryReader reader, out string value)
    {
        value = string.Empty;
        if (!TryReadCount(reader, out int length) ||
            length > MaxStringLength ||
            !reader.TryReadBytes(length * 2, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        value = Encoding.Unicode.GetString(bytes.Span);
        return true;
    }
}

public sealed class MapListBinLoader : IBinLoader
{
    public string TableName => "CsMapListMng";

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
        if (!TryParse(data, out MapListBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapListBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int count))
        {
            return false;
        }

        var records = new List<MapListRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!TryParseMap(reader, out MapListRecord? map) || map is null)
            {
                return false;
            }

            if (seen.Add(map.MapId))
            {
                records.Add(map);
            }
        }

        parsed = new MapListBinData(records);
        return true;
    }

    private static bool TryParseMap(SafeBinaryReader reader, out MapListRecord? map)
    {
        map = null;
        if (!reader.TryReadUInt32(out uint mapId) ||
            !MapBinFormat.TryReadAnsiString(reader, out string mapName) ||
            !MapBinFormat.TryReadAnsiString(reader, out string mapPath) ||
            !MapBinFormat.TryReadAnsiString(reader, out string bgSound) ||
            !reader.TryReadInt32(out int width) ||
            !reader.TryReadInt32(out int height) ||
            !MapBinFormat.TryReadUtf16String(reader, out string description) ||
            !MapBinFormat.TryReadUtf16String(reader, out string englishDescription) ||
            !reader.TryReadUInt32(out uint resurrectionMapId) ||
            !reader.TryReadUInt16(out ushort mapRegionId) ||
            !reader.TryReadUInt16(out ushort fatigueType) ||
            !reader.TryReadUInt16(out ushort fatigueDebuff) ||
            !reader.TryReadUInt16(out ushort fatigueStartTime) ||
            !reader.TryReadUInt16(out ushort fatigueAddTime) ||
            !reader.TryReadInt16(out short fatigueAddPoint) ||
            !reader.TryReadInt16(out short cameraMaxLevel) ||
            !reader.TryReadBoolean(out bool xgConsumeType) ||
            !reader.TryReadBoolean(out bool battleTagUse))
        {
            return false;
        }

        map = new MapListRecord(
            mapId,
            mapName,
            mapPath,
            bgSound,
            width,
            height,
            description,
            englishDescription,
            resurrectionMapId,
            mapRegionId,
            fatigueType,
            fatigueDebuff,
            fatigueStartTime,
            fatigueAddTime,
            fatigueAddPoint,
            cameraMaxLevel,
            xgConsumeType,
            battleTagUse);
        return true;
    }
}

public sealed class MapPortalBinLoader : IBinLoader
{
    private const int MapPortalInfoSize = 60;

    public string TableName => "CsMapPortalMng";

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
        if (!TryParse(data, out MapPortalBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapPortalBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<MapPortalRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int portalCount) ||
                (long)portalCount * MapPortalInfoSize > reader.Remaining)
            {
                return false;
            }

            for (int portal = 0; portal < portalCount; portal++)
            {
                if (!reader.TryReadBytes(MapPortalInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                    !TryParsePortal(recordBytes.Span, out MapPortalRecord? record) ||
                    record is null)
                {
                    return false;
                }

                records.Add(record);
            }
        }

        parsed = new MapPortalBinData(records);
        return true;
    }

    private static bool TryParsePortal(ReadOnlySpan<byte> bytes, out MapPortalRecord? portal)
    {
        portal = null;
        if (bytes.Length != MapPortalInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint portalId) ||
            !reader.TryReadUInt32(out uint portalType) ||
            !reader.TryReadUInt32(out uint sourceMapId) ||
            !reader.TryReadInt32(out int sourceTargetX) ||
            !reader.TryReadInt32(out int sourceTargetY) ||
            !reader.TryReadInt32(out int sourceRadius) ||
            !reader.TryReadUInt32(out uint destinationMapId) ||
            !reader.TryReadInt32(out int destinationTargetX) ||
            !reader.TryReadInt32(out int destinationTargetY) ||
            !reader.TryReadInt32(out int destinationRadius) ||
            !reader.TryReadInt32(out int portalActorType) ||
            !reader.TryReadUInt32(out uint uniqueObjectId) ||
            !reader.TryReadInt32(out int portalKindIndex) ||
            !reader.TryReadInt32(out int viewTargetX) ||
            !reader.TryReadInt32(out int viewTargetY))
        {
            return false;
        }

        portal = new MapPortalRecord(
            portalId,
            portalType,
            sourceMapId,
            sourceTargetX,
            sourceTargetY,
            sourceRadius,
            destinationMapId,
            destinationTargetX,
            destinationTargetY,
            destinationRadius,
            portalActorType,
            uniqueObjectId,
            portalKindIndex,
            viewTargetX,
            viewTargetY);
        return reader.Position == MapPortalInfoSize;
    }
}

public sealed class MapNpcBinLoader : IBinLoader
{
    private const int MapNpcInfoSize = 20;

    public string TableName => "CsMapNpcMng";

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
        if (!TryParse(data, out MapNpcBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapNpcBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int count) ||
            (long)count * MapNpcInfoSize > reader.Remaining)
        {
            return false;
        }

        var records = new List<MapNpcRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadUInt32(out uint npcId) ||
                !reader.TryReadUInt32(out uint mapId) ||
                !reader.TryReadInt32(out int initPosX) ||
                !reader.TryReadInt32(out int initPosY) ||
                !reader.TryReadSingle(out float rotation))
            {
                return false;
            }

            records.Add(new MapNpcRecord(npcId, mapId, initPosX, initPosY, rotation));
        }

        parsed = new MapNpcBinData(records);
        return true;
    }
}

public sealed class MapMonsterBinLoader : IBinLoader
{
    private const int MapMonsterInfoSize = 48;

    public string TableName => "CsMapMonsterMng";

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
        if (!TryParse(data, out MapMonsterBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapMonsterBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int fileTableCount))
        {
            return false;
        }

        var records = new List<MapMonsterRecord>();
        for (int fileTable = 0; fileTable < fileTableCount; fileTable++)
        {
            if (!reader.TryReadUInt32(out uint fileTableId) ||
                !MapBinFormat.TryReadCount(reader, out int mapCount))
            {
                return false;
            }

            for (int map = 0; map < mapCount; map++)
            {
                if (!reader.TryReadUInt32(out uint mapGroupId) ||
                    !MapBinFormat.TryReadCount(reader, out int monsterCount) ||
                    (long)monsterCount * MapMonsterInfoSize > reader.Remaining)
                {
                    return false;
                }

                for (int monster = 0; monster < monsterCount; monster++)
                {
                    if (!reader.TryReadBytes(MapMonsterInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                        !TryParseMonster(fileTableId, mapGroupId, recordBytes.Span, out MapMonsterRecord? record) ||
                        record is null)
                    {
                        return false;
                    }

                    records.Add(record);
                }
            }
        }

        parsed = new MapMonsterBinData(records);
        return true;
    }

    private static bool TryParseMonster(
        uint fileTableId,
        uint mapGroupId,
        ReadOnlySpan<byte> bytes,
        out MapMonsterRecord? monster)
    {
        monster = null;
        if (bytes.Length != MapMonsterInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadUInt32(out uint monsterTableId) ||
            !reader.TryReadInt32(out int centerX) ||
            !reader.TryReadInt32(out int centerY) ||
            !reader.TryReadInt32(out int radius) ||
            !reader.TryReadInt32(out int count) ||
            !reader.TryReadInt32(out int respawnTime) ||
            !reader.TryReadInt32(out int killgenMonsterFileTableId) ||
            !reader.TryReadInt32(out int killgenCount) ||
            !reader.TryReadInt32(out int killgenViewCount) ||
            !reader.TryReadInt32(out int moveType) ||
            !reader.TryReadBoolean(out bool instanceRespawn) ||
            !reader.TrySkip(3))
        {
            return false;
        }

        monster = new MapMonsterRecord(
            fileTableId,
            mapGroupId,
            mapId,
            monsterTableId,
            centerX,
            centerY,
            radius,
            count,
            respawnTime,
            killgenMonsterFileTableId,
            killgenCount,
            killgenViewCount,
            moveType,
            instanceRespawn);
        return reader.Position == MapMonsterInfoSize;
    }
}

public sealed class MapRegionBinLoader : IBinLoader
{
    private const int MaxFilename = 64;
    private const int RegionDescriptionLen = 128;
    private const int TCharSize = 2;
    private const int MapRegionInfoSize = 604;
    private const int MapLimitEvoInfoSize = 16;
    private const int MapBuffRegionInfoSize = 52;

    public string TableName => "CsMapRegionMng";

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
        if (!TryParse(data, out MapRegionBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapRegionBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);

        if (!TryParseRegions(reader, out List<MapRegionRecord>? regions) || regions is null ||
            !TryParseLimitEvoRegions(reader, out List<MapLimitEvoRecord>? limitEvoRegions) || limitEvoRegions is null ||
            !TryParseBuffRegions(reader, out List<MapBuffRegionRecord>? buffRegions) || buffRegions is null)
        {
            return false;
        }

        parsed = new MapRegionBinData(regions, limitEvoRegions, buffRegions);
        return true;
    }

    private static bool TryParseRegions(SafeBinaryReader reader, out List<MapRegionRecord>? regions)
    {
        regions = null;
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<MapRegionRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int regionCount) ||
                (long)regionCount * MapRegionInfoSize > reader.Remaining)
            {
                return false;
            }

            for (int region = 0; region < regionCount; region++)
            {
                if (!reader.TryReadBytes(MapRegionInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                    !TryParseRegion(recordBytes.Span, out MapRegionRecord? record) ||
                    record is null)
                {
                    return false;
                }

                records.Add(record);
            }
        }

        regions = records;
        return true;
    }

    private static bool TryParseLimitEvoRegions(SafeBinaryReader reader, out List<MapLimitEvoRecord>? regions)
    {
        regions = null;
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<MapLimitEvoRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int regionCount) ||
                (long)regionCount * MapLimitEvoInfoSize > reader.Remaining)
            {
                return false;
            }

            for (int region = 0; region < regionCount; region++)
            {
                if (!reader.TryReadBytes(MapLimitEvoInfoSize, out ReadOnlyMemory<byte> recordBytes))
                {
                    return false;
                }

                var recordReader = new SafeBinaryReader(recordBytes);
                if (!recordReader.TryReadUInt32(out uint mapId) ||
                    !recordReader.TryReadInt32(out int centerX) ||
                    !recordReader.TryReadInt32(out int centerY) ||
                    !recordReader.TryReadInt32(out int radius) ||
                    recordReader.Position != MapLimitEvoInfoSize)
                {
                    return false;
                }

                records.Add(new MapLimitEvoRecord(mapId, centerX, centerY, radius));
            }
        }

        regions = records;
        return true;
    }

    private static bool TryParseBuffRegions(SafeBinaryReader reader, out List<MapBuffRegionRecord>? regions)
    {
        regions = null;
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<MapBuffRegionRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int regionCount) ||
                (long)regionCount * MapBuffRegionInfoSize > reader.Remaining)
            {
                return false;
            }

            for (int region = 0; region < regionCount; region++)
            {
                if (!reader.TryReadBytes(MapBuffRegionInfoSize, out ReadOnlyMemory<byte> recordBytes) ||
                    !TryParseBuffRegion(recordBytes.Span, out MapBuffRegionRecord? record) ||
                    record is null)
                {
                    return false;
                }

                records.Add(record);
            }
        }

        regions = records;
        return true;
    }

    private static bool TryParseRegion(ReadOnlySpan<byte> bytes, out MapRegionRecord? region)
    {
        region = null;
        if (bytes.Length != MapRegionInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadInt32(out int centerX) ||
            !reader.TryReadInt32(out int centerY) ||
            !reader.TryReadInt32(out int radius) ||
            !reader.TryReadBytes(RegionDescriptionLen * TCharSize, out ReadOnlyMemory<byte> descriptionBytes) ||
            !reader.TryReadBytes(RegionDescriptionLen * TCharSize, out ReadOnlyMemory<byte> englishDescriptionBytes) ||
            !reader.TryReadBytes(MaxFilename, out ReadOnlyMemory<byte> bgSoundBytes) ||
            !reader.TryReadUInt16(out ushort fatigueType) ||
            !reader.TryReadUInt16(out ushort fatigueDebuff) ||
            !reader.TryReadUInt16(out ushort fatigueStartTime) ||
            !reader.TryReadUInt16(out ushort fatigueAddTime) ||
            !reader.TryReadInt16(out short fatigueAddPoint) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        region = new MapRegionRecord(
            mapId,
            centerX,
            centerY,
            radius,
            reader.ReadFixedUtf16String(descriptionBytes.Span),
            reader.ReadFixedUtf16String(englishDescriptionBytes.Span),
            reader.ReadFixedAnsiString(bgSoundBytes.Span),
            fatigueType,
            fatigueDebuff,
            fatigueStartTime,
            fatigueAddTime,
            fatigueAddPoint);
        return reader.Position == MapRegionInfoSize;
    }

    private static bool TryParseBuffRegion(ReadOnlySpan<byte> bytes, out MapBuffRegionRecord? region)
    {
        region = null;
        if (bytes.Length != MapBuffRegionInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadInt32(out int regionIndex) ||
            !reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadInt32(out int centerX) ||
            !reader.TryReadInt32(out int centerY) ||
            !reader.TryReadInt32(out int radius) ||
            !reader.TryReadInt32(out int act) ||
            !reader.TryReadInt32(out int buffIndex) ||
            !reader.TryReadInt32(out int time) ||
            !reader.TryReadInt32(out int layerNo) ||
            !reader.TryReadInt32(out int applyType) ||
            !reader.TryReadInt32(out int requirement1) ||
            !reader.TryReadInt32(out int requirement2) ||
            !reader.TryReadInt32(out int objectUniqueId))
        {
            return false;
        }

        region = new MapBuffRegionRecord(
            regionIndex,
            mapId,
            centerX,
            centerY,
            radius,
            act,
            buffIndex,
            time,
            layerNo,
            applyType,
            requirement1,
            requirement2,
            objectUniqueId);
        return reader.Position == MapBuffRegionInfoSize;
    }
}

public sealed class MapCharLightBinLoader : IBinLoader
{
    private const int DirectionalLightType = 0;
    private const int PointLightType = 1;
    private const int BaseInfoSize = 56;
    private const int DirectionalInfoSize = 108;
    private const int PointInfoSize = 84;

    public string TableName => "CsMapCharLightMng";

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
        if (!TryParse(data, out MapCharLightBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapCharLightBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<MapCharLightRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int lightCount))
            {
                return false;
            }

            for (int light = 0; light < lightCount; light++)
            {
                if (!reader.TryReadInt32(out int type) ||
                    !TryParseLight(reader, type, out MapCharLightRecord? record) ||
                    record is null)
                {
                    return false;
                }

                records.Add(record);
            }
        }

        parsed = new MapCharLightBinData(records);
        return true;
    }

    private static bool TryParseLight(SafeBinaryReader reader, int type, out MapCharLightRecord? record)
    {
        record = null;
        return type switch
        {
            DirectionalLightType => TryReadDirectionalLight(reader, out record),
            PointLightType => TryReadPointLight(reader, out record),
            _ => false
        };
    }

    private static bool TryReadDirectionalLight(SafeBinaryReader reader, out MapCharLightRecord? record)
    {
        record = null;
        if (!reader.TryReadBytes(DirectionalInfoSize, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        var recordReader = new SafeBinaryReader(bytes);
        if (!TryReadBaseInfo(recordReader, out MapCharLightBaseInfo baseInfo) ||
            baseInfo.Type != DirectionalLightType)
        {
            return false;
        }

        var rotation = new List<float>(9);
        for (int i = 0; i < 9; i++)
        {
            if (!recordReader.TryReadSingle(out float value))
            {
                return false;
            }

            rotation.Add(value);
        }

        if (!TryReadFloatList(recordReader, 3, out List<float>? position) ||
            position is null ||
            !recordReader.TryReadSingle(out float scale) ||
            recordReader.Position != DirectionalInfoSize)
        {
            return false;
        }

        record = new MapCharLightRecord(
            baseInfo.MapId,
            baseInfo.Char,
            baseInfo.Shadow,
            baseInfo.Type,
            baseInfo.EnableLight,
            baseInfo.Diffuse,
            baseInfo.Ambient,
            baseInfo.Specular,
            baseInfo.Power,
            rotation,
            position,
            scale,
            0,
            0,
            0,
            0);
        return true;
    }

    private static bool TryReadPointLight(SafeBinaryReader reader, out MapCharLightRecord? record)
    {
        record = null;
        if (!reader.TryReadBytes(PointInfoSize, out ReadOnlyMemory<byte> bytes))
        {
            return false;
        }

        var recordReader = new SafeBinaryReader(bytes);
        if (!TryReadBaseInfo(recordReader, out MapCharLightBaseInfo baseInfo) ||
            baseInfo.Type != PointLightType ||
            !TryReadFloatList(recordReader, 3, out List<float>? position) ||
            position is null ||
            !recordReader.TryReadSingle(out float constant) ||
            !recordReader.TryReadSingle(out float linear) ||
            !recordReader.TryReadSingle(out float quadratic) ||
            !recordReader.TryReadSingle(out float range) ||
            recordReader.Position != PointInfoSize)
        {
            return false;
        }

        record = new MapCharLightRecord(
            baseInfo.MapId,
            baseInfo.Char,
            baseInfo.Shadow,
            baseInfo.Type,
            baseInfo.EnableLight,
            baseInfo.Diffuse,
            baseInfo.Ambient,
            baseInfo.Specular,
            baseInfo.Power,
            [],
            position,
            0,
            constant,
            linear,
            quadratic,
            range);
        return true;
    }

    private static bool TryReadBaseInfo(SafeBinaryReader reader, out MapCharLightBaseInfo info)
    {
        info = default;
        if (!reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadBoolean(out bool isChar) ||
            !reader.TryReadBoolean(out bool shadow) ||
            !reader.TrySkip(2) ||
            !reader.TryReadInt32(out int type) ||
            !reader.TryReadBoolean(out bool enableLight) ||
            !reader.TrySkip(3) ||
            !TryReadFloatList(reader, 3, out List<float>? diffuse) ||
            diffuse is null ||
            !TryReadFloatList(reader, 3, out List<float>? ambient) ||
            ambient is null ||
            !TryReadFloatList(reader, 3, out List<float>? specular) ||
            specular is null ||
            !reader.TryReadSingle(out float power) ||
            reader.Position != BaseInfoSize)
        {
            return false;
        }

        info = new MapCharLightBaseInfo(mapId, isChar, shadow, type, enableLight, diffuse, ambient, specular, power);
        return true;
    }

    private static bool TryReadFloatList(SafeBinaryReader reader, int count, out List<float>? values)
    {
        values = null;
        var records = new List<float>(count);
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadSingle(out float value))
            {
                return false;
            }

            records.Add(value);
        }

        values = records;
        return true;
    }

    private readonly record struct MapCharLightBaseInfo(
        uint MapId,
        bool Char,
        bool Shadow,
        int Type,
        bool EnableLight,
        IReadOnlyList<float> Diffuse,
        IReadOnlyList<float> Ambient,
        IReadOnlyList<float> Specular,
        float Power);
}

public sealed class WeatherBinLoader : IBinLoader
{
    private const int WeatherInfoSize = 24;
    private const int WeatherElementInfoSize = 12;

    public string TableName => "CsWeatherMng";

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
        if (!TryParse(data, out WeatherBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out WeatherBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            return false;
        }

        var records = new List<WeatherRecord>();
        for (int group = 0; group < groupCount; group++)
        {
            if (!MapBinFormat.TryReadCount(reader, out int weatherCount))
            {
                return false;
            }

            for (int weather = 0; weather < weatherCount; weather++)
            {
                if (!reader.TryReadBytes(WeatherInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                    !TryParseWeather(infoBytes.Span, reader, out WeatherRecord? record) ||
                    record is null)
                {
                    return false;
                }

                records.Add(record);
            }
        }

        parsed = new WeatherBinData(records);
        return true;
    }

    private static bool TryParseWeather(
        ReadOnlySpan<byte> bytes,
        SafeBinaryReader parentReader,
        out WeatherRecord? weather)
    {
        weather = null;
        if (bytes.Length != WeatherInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadInt32(out int centerX) ||
            !reader.TryReadInt32(out int centerY) ||
            !reader.TryReadInt32(out int radius) ||
            !MapBinFormat.TryReadCount(reader, out int elementCount) ||
            !reader.TryReadUInt32(out uint totalLoopMinutes) ||
            reader.Position != WeatherInfoSize ||
            (long)elementCount * WeatherElementInfoSize > parentReader.Remaining)
        {
            return false;
        }

        var elements = new List<WeatherElementRecord>(elementCount);
        for (int i = 0; i < elementCount; i++)
        {
            if (!parentReader.TryReadBytes(WeatherElementInfoSize, out ReadOnlyMemory<byte> elementBytes) ||
                !TryParseWeatherElement(elementBytes.Span, out WeatherElementRecord? element) ||
                element is null)
            {
                return false;
            }

            elements.Add(element);
        }

        weather = new WeatherRecord(mapId, centerX, centerY, radius, totalLoopMinutes, elements);
        return true;
    }

    private static bool TryParseWeatherElement(ReadOnlySpan<byte> bytes, out WeatherElementRecord? element)
    {
        element = null;
        if (bytes.Length != WeatherElementInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadInt32(out int timeMinutes) ||
            !reader.TryReadInt32(out int fatigue) ||
            !reader.TryReadByte(out byte type) ||
            !reader.TryReadByte(out byte level) ||
            !reader.TrySkip(2) ||
            reader.Position != WeatherElementInfoSize)
        {
            return false;
        }

        element = new WeatherElementRecord(timeMinutes, fatigue, type, level);
        return true;
    }
}

public sealed class MapObjectBinLoader : IBinLoader
{
    private const int FactorInfoSize = 12;

    public string TableName => "CsMapObjectMng";

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
        if (!TryParse(data, out MapObjectBinData? parsed) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out MapObjectBinData? parsed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!MapBinFormat.TryReadCount(reader, out int mapCount))
        {
            return false;
        }

        var groups = new List<MapObjectGroupRecord>(mapCount);
        for (int map = 0; map < mapCount; map++)
        {
            if (!reader.TryReadUInt32(out uint mapId) ||
                !MapBinFormat.TryReadCount(reader, out int objectCount))
            {
                return false;
            }

            var objects = new List<MapObjectEntryRecord>(objectCount);
            for (int obj = 0; obj < objectCount; obj++)
            {
                if (!reader.TryReadUInt32(out uint objectId) ||
                    !MapBinFormat.TryReadCount(reader, out int instanceCount))
                {
                    return false;
                }

                var instances = new List<MapObjectInstanceRecord>(instanceCount);
                for (int instance = 0; instance < instanceCount; instance++)
                {
                    if (!reader.TryReadInt32(out int orderIndex) ||
                        !MapBinFormat.TryReadCount(reader, out int factorCount) ||
                        (long)factorCount * FactorInfoSize > reader.Remaining)
                    {
                        return false;
                    }

                    var factors = new List<MapObjectFactorRecord>(factorCount);
                    for (int factor = 0; factor < factorCount; factor++)
                    {
                        if (!reader.TryReadInt32(out int openType) ||
                            !reader.TryReadInt32(out int factorCountValue) ||
                            !reader.TryReadUInt32(out uint factorValue))
                        {
                            return false;
                        }

                        factors.Add(new MapObjectFactorRecord(openType, factorCountValue, factorValue));
                    }

                    instances.Add(new MapObjectInstanceRecord(orderIndex, factors));
                }

                objects.Add(new MapObjectEntryRecord(objectId, instances));
            }

            groups.Add(new MapObjectGroupRecord(mapId, objects));
        }

        parsed = new MapObjectBinData(groups);
        return true;
    }
}

public sealed class MapTableObjBinLoader : IBinLoader
{
    private const string TableObjBin = "Data\\Map\\TableObj.bin";
    private const string TableObjBin2 = "Data\\Map\\TableObj2.bin";
    private const int MaxObjectDataSize = 16 * 1024 * 1024;

    public string TableName => "CsMapTableObjMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        var groups = new List<MapTableObjGroupRecord>();
        var snapshots = new List<BinSectionSnapshot>();
        int totalBytes = 0;
        bool foundAny = false;

        foreach (string path in new[] { TableObjBin, TableObjBin2 })
        {
            if (!TryReadTableObj(path, fileType, out byte[] data))
            {
                continue;
            }

            foundAny = true;
            totalBytes += data.Length;
            if (!TryParseOne(path, data, out List<MapTableObjGroupRecord>? parsedGroups, out BinSectionSnapshot snapshot) ||
                parsedGroups is null)
            {
                return allowMissingTables
                    ? new BinLoadResult(BinLoadStatus.Placeholder, path, data.Length)
                    : new BinLoadResult(BinLoadStatus.Missing, path, data.Length);
            }

            groups.AddRange(parsedGroups);
            snapshots.Add(snapshot);
        }

        string combinedPath = $"{TableObjBin};{TableObjBin2}";
        if (!foundAny)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, combinedPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, combinedPath, 0);
        }

        var snapshotFile = new BinFileSnapshot(TableName, combinedPath, totalBytes, snapshots);
        return new BinLoadResult(
            BinLoadStatus.Loaded,
            combinedPath,
            totalBytes,
            snapshotFile,
            new MapTableObjBinData(groups));
    }

    private static bool TryReadTableObj(string path, eFILE_TYPE fileType, out byte[] data)
    {
        if (fileType == eFILE_TYPE.FT_FILEPACK &&
            CsFilePack.CsFPSystem.TryGetFileData(path, out data))
        {
            return true;
        }

        if (File.Exists(path))
        {
            data = File.ReadAllBytes(path);
            return true;
        }

        data = [];
        return false;
    }

    private static bool TryParseOne(
        string sourcePath,
        ReadOnlyMemory<byte> data,
        out List<MapTableObjGroupRecord>? groups,
        out BinSectionSnapshot snapshot)
    {
        groups = null;
        var reader = new SafeBinaryReader(data);
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int groupCount))
        {
            snapshot = new BinSectionSnapshot(sourcePath, offset, 0, null, 0, false);
            return false;
        }

        var records = new List<MapTableObjGroupRecord>(groupCount);
        for (int group = 0; group < groupCount; group++)
        {
            if (!TryParseGroup(reader, out MapTableObjGroupRecord? record) || record is null)
            {
                snapshot = new BinSectionSnapshot(sourcePath, offset, groupCount, null, reader.Position - offset, false);
                return false;
            }

            records.Add(record);
        }

        groups = records;
        snapshot = new BinSectionSnapshot(sourcePath, offset, groupCount, null, reader.Position - offset, true);
        return true;
    }

    private static bool TryParseGroup(SafeBinaryReader reader, out MapTableObjGroupRecord? group)
    {
        group = null;
        if (!reader.TryReadUInt32(out uint mapId) ||
            !MapBinFormat.TryReadCount(reader, out int nodeCount))
        {
            return false;
        }

        var nodes = new List<MapTableObjNodeRecord>(nodeCount);
        for (int node = 0; node < nodeCount; node++)
        {
            if (!reader.TryReadInt32(out int nodeIndex) ||
                !MapBinFormat.TryReadCount(reader, out int objectCount))
            {
                return false;
            }

            var objects = new List<MapTableObjRecord>(objectCount);
            for (int obj = 0; obj < objectCount; obj++)
            {
                if (!reader.TryReadInt32(out int dataSize) ||
                    dataSize <= 0 ||
                    dataSize > MaxObjectDataSize ||
                    !reader.TryReadBytes(dataSize, out ReadOnlyMemory<byte> objectData))
                {
                    return false;
                }

                objects.Add(new MapTableObjRecord(objectData));
            }

            nodes.Add(new MapTableObjNodeRecord(nodeIndex, objects));
        }

        group = new MapTableObjGroupRecord(mapId, nodes);
        return true;
    }
}
