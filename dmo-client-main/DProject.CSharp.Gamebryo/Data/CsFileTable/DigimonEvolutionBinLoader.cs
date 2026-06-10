using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record DigimonEvolutionBinData(
    IReadOnlyList<DigimonEvolutionInfoRecord> Evolutions,
    IReadOnlyList<BattleEvolutionInfoRecord> BattleEvolutions);

public sealed record DigimonEvolutionInfoRecord(
    uint DigimonId,
    uint BattleEvoType,
    IReadOnlyDictionary<uint, DigimonEvolutionObjectRecord> Objects);

public sealed record EvolutionSlotRecord(
    ushort Slot,
    ushort Type,
    uint DigimonId);

public sealed record IntPointRecord(int X, int Y);

public sealed record DigimonEvolutionObjectRecord(
    uint Id,
    ushort EvoSlot,
    IReadOnlyList<EvolutionSlotRecord> EvolutionList,
    IntPointRecord IconPosition,
    ushort EnableSlot,
    ushort OpenQualification,
    ushort OpenLevel,
    ushort OpenQuest,
    ushort OpenItemTypeS,
    ushort OpenItemCount,
    ushort UseItem,
    ushort UseItemCount,
    ushort Intimacy,
    ushort OpenCrest,
    ushort EvoCard1,
    ushort EvoCard2,
    ushort EvoCard3,
    ushort EvoDigimental,
    ushort EvoTamerDS,
    ushort Dummy,
    int Render,
    IntPointRecord TargetStartPosition,
    int StartHeight,
    int StartRotation,
    IntPointRecord TargetEndPosition,
    int EndHeight,
    int EndRotation,
    int Speed,
    uint AnimationId,
    double StartTime,
    double EndTime,
    int Red,
    int Green,
    int Blue,
    string LevelEffect,
    string EnchantEffect,
    string SizeEffect,
    int EvolutionTree,
    uint JoGressQuestCheck,
    ushort ChipsetType,
    ushort ChipsetTypeC,
    ushort ChipsetCount,
    ushort ChipsetTypeP,
    ushort JoGressesCount,
    IReadOnlyList<int> JoGressTacticses);

public sealed record BattleEvolutionInfoRecord(
    ushort MaxEP,
    IReadOnlyList<BattleEvolutionElementRecord> Elements);

public sealed record BattleEvolutionElementRecord(
    bool Enable,
    ushort EvolStep,
    ushort RequiredEvoEP,
    ushort DecreaseEP);

public sealed class DigimonEvolutionBinLoader : IBinLoader
{
    private const string DigimonEvoBin = "DigimonEvo.bin";
    private const int EvolutionInfoSize = 8;
    private const int EvolutionObjectSize = 328;
    private const int EvolutionSlotCount = 9;
    private const int EvolutionEffectLen = 32;
    private const int JointTacticsLen = 3;
    private const int BattleEvoTypeCount = 5;
    private const int BattleEvoElementCount = 6;
    private const int BattleEvoInfoSize = 50;

    public string TableName => "CDigimonEvolution";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, DigimonEvoBin);
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, binPath, out DigimonEvolutionBinData? parsed, out BinFileSnapshot snapshot) ||
            parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        string binPath,
        out DigimonEvolutionBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        int evolutionOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int evolutionCount))
        {
            snapshot = Snapshot(binPath, data.Length, [new BinSectionSnapshot("EvolutionInfos", evolutionOffset, 0, null, 0, false)]);
            return false;
        }

        var evolutions = new List<DigimonEvolutionInfoRecord>(evolutionCount);
        for (int i = 0; i < evolutionCount; i++)
        {
            if (!reader.TryReadBytes(EvolutionInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseEvolutionInfo(infoBytes, out uint digimonId, out uint battleEvoType) ||
                !MapBinFormat.TryReadCount(reader, out int objectCount) ||
                (long)objectCount * EvolutionObjectSize > reader.Remaining)
            {
                sections.Add(new BinSectionSnapshot("EvolutionInfos", evolutionOffset, evolutionCount, null, reader.Position - evolutionOffset, false));
                snapshot = Snapshot(binPath, data.Length, sections);
                return false;
            }

            var objects = new Dictionary<uint, DigimonEvolutionObjectRecord>();
            for (int obj = 0; obj < objectCount; obj++)
            {
                if (!reader.TryReadBytes(EvolutionObjectSize, out ReadOnlyMemory<byte> objectBytes) ||
                    !TryParseEvolutionObject(objectBytes, out DigimonEvolutionObjectRecord? evolutionObject) ||
                    evolutionObject is null)
                {
                    sections.Add(new BinSectionSnapshot("EvolutionInfos", evolutionOffset, evolutionCount, null, reader.Position - evolutionOffset, false));
                    snapshot = Snapshot(binPath, data.Length, sections);
                    return false;
                }

                objects.TryAdd(evolutionObject.Id, evolutionObject);
            }

            evolutions.Add(new DigimonEvolutionInfoRecord(digimonId, battleEvoType, objects));
        }

        sections.Add(new BinSectionSnapshot("EvolutionInfos", evolutionOffset, evolutionCount, null, reader.Position - evolutionOffset, true));

        int battleOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int battleCount) ||
            battleCount < 0 ||
            battleCount > BattleEvoTypeCount ||
            (long)battleCount * BattleEvoInfoSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("BattleEvolutions", battleOffset, 0, BattleEvoInfoSize, reader.Position - battleOffset, false));
            snapshot = Snapshot(binPath, data.Length, sections);
            return false;
        }

        var battleEvolutions = new List<BattleEvolutionInfoRecord>(battleCount);
        for (int i = 0; i < battleCount; i++)
        {
            if (!reader.TryReadBytes(BattleEvoInfoSize, out ReadOnlyMemory<byte> battleBytes) ||
                !TryParseBattleEvolution(battleBytes, out BattleEvolutionInfoRecord? battleInfo) ||
                battleInfo is null)
            {
                sections.Add(new BinSectionSnapshot("BattleEvolutions", battleOffset, battleCount, BattleEvoInfoSize, reader.Position - battleOffset, false));
                snapshot = Snapshot(binPath, data.Length, sections);
                return false;
            }

            battleEvolutions.Add(battleInfo);
        }

        bool complete = reader.EndOfData;
        sections.Add(new BinSectionSnapshot("BattleEvolutions", battleOffset, battleCount, BattleEvoInfoSize, reader.Position - battleOffset, complete));
        snapshot = Snapshot(binPath, data.Length, sections);
        parsed = new DigimonEvolutionBinData(evolutions, battleEvolutions);
        return complete;
    }

    private static bool TryParseEvolutionInfo(ReadOnlyMemory<byte> data, out uint digimonId, out uint battleEvoType)
    {
        digimonId = 0;
        battleEvoType = 0;
        var reader = new SafeBinaryReader(data);
        return reader.TryReadUInt32(out digimonId) &&
            reader.TryReadUInt32(out battleEvoType) &&
            reader.EndOfData;
    }

    private static bool TryParseEvolutionObject(
        ReadOnlyMemory<byte> data,
        out DigimonEvolutionObjectRecord? evolutionObject)
    {
        evolutionObject = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint id) ||
            !reader.TryReadUInt16(out ushort evoSlot) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        var evolutionList = new List<EvolutionSlotRecord>(EvolutionSlotCount);
        for (int i = 0; i < EvolutionSlotCount; i++)
        {
            if (!reader.TryReadUInt16(out ushort slot) ||
                !reader.TryReadUInt16(out ushort type) ||
                !reader.TryReadUInt32(out uint digimonId))
            {
                return false;
            }

            evolutionList.Add(new EvolutionSlotRecord(slot, type, digimonId));
        }

        if (!TryReadPoint(reader, out IntPointRecord iconPosition) ||
            !reader.TryReadUInt16(out ushort enableSlot) ||
            !reader.TryReadUInt16(out ushort openQualification) ||
            !reader.TryReadUInt16(out ushort openLevel) ||
            !reader.TryReadUInt16(out ushort openQuest) ||
            !reader.TryReadUInt16(out ushort openItemTypeS) ||
            !reader.TryReadUInt16(out ushort openItemCount) ||
            !reader.TryReadUInt16(out ushort useItem) ||
            !reader.TryReadUInt16(out ushort useItemCount) ||
            !reader.TryReadUInt16(out ushort intimacy) ||
            !reader.TryReadUInt16(out ushort openCrest) ||
            !reader.TryReadUInt16(out ushort evoCard1) ||
            !reader.TryReadUInt16(out ushort evoCard2) ||
            !reader.TryReadUInt16(out ushort evoCard3) ||
            !reader.TryReadUInt16(out ushort evoDigimental) ||
            !reader.TryReadUInt16(out ushort evoTamerDS) ||
            !reader.TryReadUInt16(out ushort dummy) ||
            !reader.TryReadInt32(out int render) ||
            !TryReadPoint(reader, out IntPointRecord targetStartPosition) ||
            !reader.TryReadInt32(out int startHeight) ||
            !reader.TryReadInt32(out int startRotation) ||
            !TryReadPoint(reader, out IntPointRecord targetEndPosition) ||
            !reader.TryReadInt32(out int endHeight) ||
            !reader.TryReadInt32(out int endRotation) ||
            !reader.TryReadInt32(out int speed) ||
            !reader.TryReadUInt32(out uint animationId) ||
            !reader.TrySkip(4) ||
            !reader.TryReadDouble(out double startTime) ||
            !reader.TryReadDouble(out double endTime) ||
            !reader.TryReadInt32(out int red) ||
            !reader.TryReadInt32(out int green) ||
            !reader.TryReadInt32(out int blue) ||
            !reader.TryReadBytes(EvolutionEffectLen, out ReadOnlyMemory<byte> levelEffectBytes) ||
            !reader.TryReadBytes(EvolutionEffectLen, out ReadOnlyMemory<byte> enchantEffectBytes) ||
            !reader.TryReadBytes(EvolutionEffectLen, out ReadOnlyMemory<byte> sizeEffectBytes) ||
            !reader.TryReadInt32(out int evolutionTree) ||
            !reader.TryReadUInt32(out uint joGressQuestCheck) ||
            !reader.TryReadUInt16(out ushort chipsetType) ||
            !reader.TryReadUInt16(out ushort chipsetTypeC) ||
            !reader.TryReadUInt16(out ushort chipsetCount) ||
            !reader.TryReadUInt16(out ushort chipsetTypeP) ||
            !reader.TryReadUInt16(out ushort joGressesCount) ||
            !reader.TrySkip(2))
        {
            return false;
        }

        var joGressTacticses = new List<int>(JointTacticsLen);
        for (int i = 0; i < JointTacticsLen; i++)
        {
            if (!reader.TryReadInt32(out int tacticsId))
            {
                return false;
            }

            joGressTacticses.Add(tacticsId);
        }

        if (!reader.TrySkip(4) || !reader.EndOfData)
        {
            return false;
        }

        evolutionObject = new DigimonEvolutionObjectRecord(
            id,
            evoSlot,
            evolutionList,
            iconPosition,
            enableSlot,
            openQualification,
            openLevel,
            openQuest,
            openItemTypeS,
            openItemCount,
            useItem,
            useItemCount,
            intimacy,
            openCrest,
            evoCard1,
            evoCard2,
            evoCard3,
            evoDigimental,
            evoTamerDS,
            dummy,
            render,
            targetStartPosition,
            startHeight,
            startRotation,
            targetEndPosition,
            endHeight,
            endRotation,
            speed,
            animationId,
            startTime,
            endTime,
            red,
            green,
            blue,
            reader.ReadFixedAnsiString(levelEffectBytes.Span),
            reader.ReadFixedAnsiString(enchantEffectBytes.Span),
            reader.ReadFixedAnsiString(sizeEffectBytes.Span),
            evolutionTree,
            joGressQuestCheck,
            chipsetType,
            chipsetTypeC,
            chipsetCount,
            chipsetTypeP,
            joGressesCount,
            joGressTacticses);
        return true;
    }

    private static bool TryParseBattleEvolution(
        ReadOnlyMemory<byte> data,
        out BattleEvolutionInfoRecord? battleInfo)
    {
        battleInfo = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt16(out ushort maxEP))
        {
            return false;
        }

        var elements = new List<BattleEvolutionElementRecord>(BattleEvoElementCount);
        for (int i = 0; i < BattleEvoElementCount; i++)
        {
            if (!reader.TryReadBoolean(out bool enable) ||
                !reader.TrySkip(1) ||
                !reader.TryReadUInt16(out ushort evolStep) ||
                !reader.TryReadUInt16(out ushort requiredEvoEP) ||
                !reader.TryReadUInt16(out ushort decreaseEP))
            {
                return false;
            }

            elements.Add(new BattleEvolutionElementRecord(enable, evolStep, requiredEvoEP, decreaseEP));
        }

        battleInfo = new BattleEvolutionInfoRecord(maxEP, elements);
        return reader.EndOfData;
    }

    private static bool TryReadPoint(SafeBinaryReader reader, out IntPointRecord point)
    {
        point = new IntPointRecord(0, 0);
        if (!reader.TryReadInt32(out int x) ||
            !reader.TryReadInt32(out int y))
        {
            return false;
        }

        point = new IntPointRecord(x, y);
        return true;
    }

    private static BinFileSnapshot Snapshot(
        string path,
        int byteCount,
        IReadOnlyList<BinSectionSnapshot> sections)
    {
        return new BinFileSnapshot("CDigimonEvolution", path, byteCount, sections);
    }
}
