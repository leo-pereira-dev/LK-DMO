using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record DigimonBinData(
    IReadOnlyList<DigimonBinRecord> Digimons,
    IReadOnlyList<RideBinRecord> Rides,
    IReadOnlyList<AttributeTypeUiRecord> AttributeTypes,
    IReadOnlyList<NatureTypeUiRecord> NatureTypes);

public sealed record DigimonSkillSlot(
    uint SkillId,
    int RequiredPreviousSkillLevel);

public sealed record DigimonBaseStats(
    ushort HP,
    ushort DS,
    ushort DefPower,
    ushort Evasion,
    ushort MoveSpeed,
    ushort CriticalRate,
    ushort AttPower,
    ushort AttSpeed,
    ushort AttRange,
    ushort HitRate);

public sealed record DigimonBinRecord(
    uint DigimonId,
    uint ModelId,
    string Name,
    string SoundDirName,
    float SelectScale,
    string EvoEffectDir,
    int EvolutionType,
    int AttributeType,
    IReadOnlyList<int> FamilyTypes,
    int BaseNatureType,
    IReadOnlyList<int> BaseNatureTypes,
    uint BaseLevel,
    DigimonBaseStats BaseStats,
    byte DigimonType,
    ushort CharSize,
    IReadOnlyList<DigimonSkillSlot> Skills,
    float WalkLength,
    float RunLength,
    float AttackRunLength,
    string Form,
    int DigimonRank);

public sealed record RideOpenInfo(
    int ItemType,
    int NeedCount);

public sealed record RideBinRecord(
    uint DigimonId,
    uint ChangeRide,
    float MoveSpeed,
    string Comment,
    int RideType,
    float AniRateRun,
    IReadOnlyList<RideOpenInfo> OpenInfos);

public sealed record AttributeTypeUiRecord(
    uint AttributeId,
    string Path,
    int X,
    int Y,
    int BarX,
    int BarY);

public sealed record NatureTypeUiRecord(
    uint NatureId,
    string Path,
    int X,
    int Y,
    int GaugeX,
    int GaugeY,
    float Angle);

public sealed class DigimonBinLoader : IBinLoader
{
    private const int MaxFilename = 64;
    private const int TCharSize = 2;
    private const int DigimonMaxSkillCount = 4;
    private const int DigimonMaxFamily = 3;
    private const int DigimonMaxNature = 3;
    private const int RideOpenInfoCount = 2;
    private const int ItemCommentLen = 512;
    private const int ElementUiPathLen = 128;

    private const int DigimonInfoSize = 572;
    private const int RideInfoSize = 1060;
    private const int AttributeTypeUiInfoSize = 148;
    private const int NatureTypeUiInfoSize = 152;

    public string TableName => "CsDigimonMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string digimonPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(digimonPath, fileType, out byte[] digimonData))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, digimonPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, digimonPath, 0);
        }

        BinFileSnapshot snapshot = CountPrefixedBinInspector.Inspect(TableName, digimonPath, digimonData);
        if (!TryParseDigimons(digimonData, out List<DigimonBinRecord>? digimons) || digimons is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, digimonPath, digimonData.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, digimonPath, digimonData.Length, snapshot);
        }

        string ridePath = Path.Combine(languagePath, "Ride.bin");
        List<RideBinRecord> rides = [];
        if (BinDataSource.TryRead(ridePath, fileType, out byte[] rideData))
        {
            if (!TryParseRides(rideData, out List<RideBinRecord>? parsedRides) || parsedRides is null)
            {
                return allowMissingTables
                    ? new BinLoadResult(BinLoadStatus.Placeholder, ridePath, rideData.Length, snapshot)
                    : new BinLoadResult(BinLoadStatus.Missing, ridePath, rideData.Length, snapshot);
            }

            rides = parsedRides;
        }
        else if (!allowMissingTables)
        {
            return new BinLoadResult(BinLoadStatus.Missing, ridePath, 0, snapshot);
        }

        string elementPath = Path.Combine(languagePath, "New_Element.bin");
        List<AttributeTypeUiRecord> attributeTypes = [];
        List<NatureTypeUiRecord> natureTypes = [];
        if (BinDataSource.TryRead(elementPath, fileType, out byte[] elementData))
        {
            if (!TryParseElementUi(
                    elementData,
                    out List<AttributeTypeUiRecord>? parsedAttributeTypes,
                    out List<NatureTypeUiRecord>? parsedNatureTypes) ||
                parsedAttributeTypes is null ||
                parsedNatureTypes is null)
            {
                return allowMissingTables
                    ? new BinLoadResult(BinLoadStatus.Placeholder, elementPath, elementData.Length, snapshot)
                    : new BinLoadResult(BinLoadStatus.Missing, elementPath, elementData.Length, snapshot);
            }

            attributeTypes = parsedAttributeTypes;
            natureTypes = parsedNatureTypes;
        }
        else if (!allowMissingTables)
        {
            return new BinLoadResult(BinLoadStatus.Missing, elementPath, 0, snapshot);
        }

        return new BinLoadResult(
            BinLoadStatus.Loaded,
            digimonPath,
            digimonData.Length,
            snapshot,
            new DigimonBinData(digimons, rides, attributeTypes, natureTypes));
    }

    private static bool TryParseDigimons(ReadOnlyMemory<byte> data, out List<DigimonBinRecord>? digimons)
    {
        digimons = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<DigimonBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(DigimonInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseDigimon(recordBytes.Span, out DigimonBinRecord? digimon) || digimon is null)
            {
                return false;
            }

            if (seen.Add(digimon.DigimonId))
            {
                records.Add(digimon);
            }
        }

        digimons = records;
        return true;
    }

    private static bool TryParseDigimon(ReadOnlySpan<byte> bytes, out DigimonBinRecord? digimon)
    {
        digimon = null;
        if (bytes.Length != DigimonInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint digimonId) ||
            !reader.TryReadUInt32(out uint modelId) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> nameBytes) ||
            !reader.TryReadBytes(MaxFilename, out ReadOnlyMemory<byte> soundBytes) ||
            !reader.TryReadSingle(out float selectScale) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> evoEffectBytes) ||
            !reader.TryReadInt32(out int evolutionType) ||
            !reader.TryReadInt32(out int attributeType))
        {
            return false;
        }

        var familyTypes = new List<int>(DigimonMaxFamily);
        for (int i = 0; i < DigimonMaxFamily; i++)
        {
            if (!reader.TryReadInt32(out int familyType))
            {
                return false;
            }

            familyTypes.Add(familyType);
        }

        if (!reader.TryReadInt32(out int baseNatureType))
        {
            return false;
        }

        var baseNatureTypes = new List<int>(DigimonMaxNature);
        for (int i = 0; i < DigimonMaxNature; i++)
        {
            if (!reader.TryReadInt32(out int natureType))
            {
                return false;
            }

            baseNatureTypes.Add(natureType);
        }

        if (!reader.TryReadUInt32(out uint baseLevel) ||
            !TryReadBaseStats(reader, out DigimonBaseStats? baseStats) || baseStats is null ||
            !reader.TryReadByte(out byte digimonType) ||
            !reader.TrySkip(1) ||
            !reader.TryReadUInt16(out ushort charSize))
        {
            return false;
        }

        var skills = new List<DigimonSkillSlot>(DigimonMaxSkillCount);
        for (int i = 0; i < DigimonMaxSkillCount; i++)
        {
            if (!reader.TryReadUInt32(out uint skillId) ||
                !reader.TryReadInt32(out int requiredPreviousSkillLevel))
            {
                return false;
            }

            skills.Add(new DigimonSkillSlot(skillId, requiredPreviousSkillLevel));
        }

        if (!reader.TryReadSingle(out float walkLength) ||
            !reader.TryReadSingle(out float runLength) ||
            !reader.TryReadSingle(out float attackRunLength) ||
            !reader.TryReadBytes(MaxFilename * TCharSize, out ReadOnlyMemory<byte> formBytes) ||
            !reader.TryReadInt32(out int digimonRank))
        {
            return false;
        }

        digimon = new DigimonBinRecord(
            digimonId,
            modelId,
            reader.ReadFixedUtf16String(nameBytes.Span),
            reader.ReadFixedAnsiString(soundBytes.Span),
            selectScale,
            reader.ReadFixedUtf16String(evoEffectBytes.Span),
            evolutionType,
            attributeType,
            familyTypes,
            baseNatureType,
            baseNatureTypes,
            baseLevel,
            baseStats,
            digimonType,
            charSize,
            skills,
            walkLength,
            runLength,
            attackRunLength,
            reader.ReadFixedUtf16String(formBytes.Span),
            digimonRank);
        return reader.Position == DigimonInfoSize;
    }

    private static bool TryReadBaseStats(SafeBinaryReader reader, out DigimonBaseStats? stats)
    {
        stats = null;
        if (!reader.TryReadUInt16(out ushort hp) ||
            !reader.TryReadUInt16(out ushort ds) ||
            !reader.TryReadUInt16(out ushort defPower) ||
            !reader.TryReadUInt16(out ushort evasion) ||
            !reader.TryReadUInt16(out ushort moveSpeed) ||
            !reader.TryReadUInt16(out ushort criticalRate) ||
            !reader.TryReadUInt16(out ushort attPower) ||
            !reader.TryReadUInt16(out ushort attSpeed) ||
            !reader.TryReadUInt16(out ushort attRange) ||
            !reader.TryReadUInt16(out ushort hitRate))
        {
            return false;
        }

        stats = new DigimonBaseStats(
            hp,
            ds,
            defPower,
            evasion,
            moveSpeed,
            criticalRate,
            attPower,
            attSpeed,
            attRange,
            hitRate);
        return true;
    }

    private static bool TryParseRides(ReadOnlyMemory<byte> data, out List<RideBinRecord>? rides)
    {
        rides = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadInt32(out int count) || count < 0)
        {
            return false;
        }

        var records = new List<RideBinRecord>(count);
        var seen = new HashSet<uint>();
        for (int i = 0; i < count; i++)
        {
            if (!reader.TryReadBytes(RideInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseRide(recordBytes.Span, out RideBinRecord? ride) || ride is null)
            {
                return false;
            }

            if (seen.Add(ride.DigimonId))
            {
                records.Add(ride);
            }
        }

        rides = records;
        return true;
    }

    private static bool TryParseRide(ReadOnlySpan<byte> bytes, out RideBinRecord? ride)
    {
        ride = null;
        if (bytes.Length != RideInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint digimonId) ||
            !reader.TryReadUInt32(out uint changeRide) ||
            !reader.TryReadSingle(out float moveSpeed) ||
            !reader.TryReadBytes(ItemCommentLen * TCharSize, out ReadOnlyMemory<byte> commentBytes) ||
            !reader.TryReadInt32(out int rideType) ||
            !reader.TryReadSingle(out float aniRateRun))
        {
            return false;
        }

        var openInfos = new List<RideOpenInfo>(RideOpenInfoCount);
        for (int i = 0; i < RideOpenInfoCount; i++)
        {
            if (!reader.TryReadInt32(out int itemType) ||
                !reader.TryReadInt32(out int needCount))
            {
                return false;
            }

            openInfos.Add(new RideOpenInfo(itemType, needCount));
        }

        ride = new RideBinRecord(
            digimonId,
            changeRide,
            moveSpeed,
            reader.ReadFixedUtf16String(commentBytes.Span),
            rideType,
            aniRateRun,
            openInfos);
        return reader.Position == RideInfoSize;
    }

    private static bool TryParseElementUi(
        ReadOnlyMemory<byte> data,
        out List<AttributeTypeUiRecord>? attributeTypes,
        out List<NatureTypeUiRecord>? natureTypes)
    {
        attributeTypes = null;
        natureTypes = null;
        var reader = new SafeBinaryReader(data);

        if (!reader.TryReadInt32(out int attributeCount) || attributeCount < 0)
        {
            return false;
        }

        var attributes = new List<AttributeTypeUiRecord>(attributeCount);
        var seenAttributes = new HashSet<uint>();
        for (int i = 0; i < attributeCount; i++)
        {
            if (!reader.TryReadBytes(AttributeTypeUiInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseAttributeType(recordBytes.Span, out AttributeTypeUiRecord? attributeType) ||
                attributeType is null)
            {
                return false;
            }

            if (seenAttributes.Add(attributeType.AttributeId))
            {
                attributes.Add(attributeType);
            }
        }

        if (!reader.TryReadInt32(out int natureCount) || natureCount < 0)
        {
            return false;
        }

        var natures = new List<NatureTypeUiRecord>(natureCount);
        var seenNatures = new HashSet<uint>();
        for (int i = 0; i < natureCount; i++)
        {
            if (!reader.TryReadBytes(NatureTypeUiInfoSize, out ReadOnlyMemory<byte> recordBytes))
            {
                return false;
            }

            if (!TryParseNatureType(recordBytes.Span, out NatureTypeUiRecord? natureType) || natureType is null)
            {
                return false;
            }

            if (seenNatures.Add(natureType.NatureId))
            {
                natures.Add(natureType);
            }
        }

        attributeTypes = attributes;
        natureTypes = natures;
        return true;
    }

    private static bool TryParseAttributeType(ReadOnlySpan<byte> bytes, out AttributeTypeUiRecord? attributeType)
    {
        attributeType = null;
        if (bytes.Length != AttributeTypeUiInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint attributeId) ||
            !reader.TryReadBytes(ElementUiPathLen, out ReadOnlyMemory<byte> pathBytes) ||
            !reader.TryReadInt32(out int x) ||
            !reader.TryReadInt32(out int y) ||
            !reader.TryReadInt32(out int barX) ||
            !reader.TryReadInt32(out int barY))
        {
            return false;
        }

        attributeType = new AttributeTypeUiRecord(
            attributeId,
            reader.ReadFixedAnsiString(pathBytes.Span),
            x,
            y,
            barX,
            barY);
        return reader.Position == AttributeTypeUiInfoSize;
    }

    private static bool TryParseNatureType(ReadOnlySpan<byte> bytes, out NatureTypeUiRecord? natureType)
    {
        natureType = null;
        if (bytes.Length != NatureTypeUiInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint natureId) ||
            !reader.TryReadBytes(ElementUiPathLen, out ReadOnlyMemory<byte> pathBytes) ||
            !reader.TryReadInt32(out int x) ||
            !reader.TryReadInt32(out int y) ||
            !reader.TryReadInt32(out int gaugeX) ||
            !reader.TryReadInt32(out int gaugeY) ||
            !reader.TryReadSingle(out float angle))
        {
            return false;
        }

        natureType = new NatureTypeUiRecord(
            natureId,
            reader.ReadFixedAnsiString(pathBytes.Span),
            x,
            y,
            gaugeX,
            gaugeY,
            angle);
        return reader.Position == NatureTypeUiInfoSize;
    }
}
