using DProject.Data.Binary;

namespace DProject.Data.CsFileTable;

public sealed record SceneDataBinData(
    IReadOnlyList<SceneRecord> Scenes,
    IReadOnlyList<SceneTargetRecord> Targets);

public sealed record SceneRecord(
    SceneInfoRecord Info,
    IReadOnlyList<SceneStepRecord> Steps);

public sealed record SceneInfoRecord(
    uint SceneId,
    bool Use,
    string Title,
    int MapId,
    int RegionId,
    bool Skip,
    string Music);

public sealed record SceneStepRecord(
    uint MainTargetId,
    IReadOnlyList<uint> TargetIds,
    uint AnimationId,
    string Talk,
    string Effect,
    string Sound);

public sealed record SceneTargetRecord(
    uint Uid,
    uint ModelId,
    float Distance,
    float Height,
    string Name);

public sealed class SceneDataBinLoader : IBinLoader
{
    private const int MaxSceneTargets = 5;
    private const int SceneInfoSize = 212;
    private const int SceneStepSize = 2204;
    private const int TargetDataSize = 80;
    private const int TitleBytes = 64 * 2;
    private const int TalkBytes = 1024 * 2;
    private const int NameBytes = 32 * 2;
    private const int AnsiPathBytes = 64;

    public string TableName => "CsSceneDataMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = Path.Combine(languagePath, TableFileNameCatalog.GetBinFileName(TableName));
        if (!BinDataSource.TryRead(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, out SceneDataBinData? parsed, out BinFileSnapshot snapshot) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        out SceneDataBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        var sections = new List<BinSectionSnapshot>();

        int sceneOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int sceneCount))
        {
            snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length,
                [new BinSectionSnapshot("Scenes", sceneOffset, 0, null, 0, false)]);
            return false;
        }

        var scenes = new List<SceneRecord>(sceneCount);
        for (int i = 0; i < sceneCount; i++)
        {
            if (!reader.TryReadBytes(SceneInfoSize, out ReadOnlyMemory<byte> infoBytes) ||
                !TryParseSceneInfo(infoBytes.Span, out SceneInfoRecord? info) ||
                info is null ||
                !MapBinFormat.TryReadCount(reader, out int stepCount) ||
                (long)stepCount * SceneStepSize > reader.Remaining)
            {
                sections.Add(new BinSectionSnapshot("Scenes", sceneOffset, sceneCount, null, reader.Position - sceneOffset, false));
                snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length, sections);
                return false;
            }

            var steps = new List<SceneStepRecord>(stepCount);
            for (int step = 0; step < stepCount; step++)
            {
                if (!reader.TryReadBytes(SceneStepSize, out ReadOnlyMemory<byte> stepBytes) ||
                    !TryParseSceneStep(stepBytes.Span, out SceneStepRecord? sceneStep) ||
                    sceneStep is null)
                {
                    sections.Add(new BinSectionSnapshot("Scenes", sceneOffset, sceneCount, null, reader.Position - sceneOffset, false));
                    snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length, sections);
                    return false;
                }

                steps.Add(sceneStep);
            }

            scenes.Add(new SceneRecord(info, steps));
        }

        sections.Add(new BinSectionSnapshot("Scenes", sceneOffset, sceneCount, null, reader.Position - sceneOffset, true));

        int targetOffset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int targetCount) ||
            (long)targetCount * TargetDataSize > reader.Remaining)
        {
            sections.Add(new BinSectionSnapshot("Targets", targetOffset, 0, TargetDataSize, reader.Position - targetOffset, false));
            snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length, sections);
            return false;
        }

        var targets = new List<SceneTargetRecord>(targetCount);
        for (int i = 0; i < targetCount; i++)
        {
            if (!reader.TryReadBytes(TargetDataSize, out ReadOnlyMemory<byte> targetBytes) ||
                !TryParseTarget(targetBytes.Span, out SceneTargetRecord? target) ||
                target is null)
            {
                sections.Add(new BinSectionSnapshot("Targets", targetOffset, targetCount, TargetDataSize, reader.Position - targetOffset, false));
                snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length, sections);
                return false;
            }

            targets.Add(target);
        }

        bool complete = reader.EndOfData;
        sections.Add(new BinSectionSnapshot("Targets", targetOffset, targetCount, TargetDataSize, reader.Position - targetOffset, complete));
        snapshot = new BinFileSnapshot("CsSceneDataMng", TableFileNameCatalog.GetBinFileName("CsSceneDataMng"), data.Length, sections);
        parsed = new SceneDataBinData(scenes, targets);
        return complete;
    }

    private static bool TryParseSceneInfo(ReadOnlySpan<byte> bytes, out SceneInfoRecord? info)
    {
        info = null;
        if (bytes.Length != SceneInfoSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint sceneId) ||
            !reader.TryReadBoolean(out bool use) ||
            !reader.TrySkip(1) ||
            !reader.TryReadBytes(TitleBytes, out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TrySkip(2) ||
            !reader.TryReadInt32(out int mapId) ||
            !reader.TryReadInt32(out int regionId) ||
            !reader.TryReadBoolean(out bool skip) ||
            !reader.TryReadBytes(AnsiPathBytes, out ReadOnlyMemory<byte> musicBytes) ||
            !reader.TrySkip(3) ||
            reader.Position != SceneInfoSize)
        {
            return false;
        }

        info = new SceneInfoRecord(
            sceneId,
            use,
            reader.ReadFixedUtf16String(titleBytes.Span),
            mapId,
            regionId,
            skip,
            reader.ReadFixedAnsiString(musicBytes.Span));
        return true;
    }

    private static bool TryParseSceneStep(ReadOnlySpan<byte> bytes, out SceneStepRecord? step)
    {
        step = null;
        if (bytes.Length != SceneStepSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint mainTargetId))
        {
            return false;
        }

        var targetIds = new List<uint>(MaxSceneTargets);
        for (int i = 0; i < MaxSceneTargets; i++)
        {
            if (!reader.TryReadUInt32(out uint targetId))
            {
                return false;
            }

            targetIds.Add(targetId);
        }

        if (!reader.TryReadUInt32(out uint animationId) ||
            !reader.TryReadBytes(TalkBytes, out ReadOnlyMemory<byte> talkBytes) ||
            !reader.TryReadBytes(AnsiPathBytes, out ReadOnlyMemory<byte> effectBytes) ||
            !reader.TryReadBytes(AnsiPathBytes, out ReadOnlyMemory<byte> soundBytes) ||
            reader.Position != SceneStepSize)
        {
            return false;
        }

        step = new SceneStepRecord(
            mainTargetId,
            targetIds,
            animationId,
            reader.ReadFixedUtf16String(talkBytes.Span),
            reader.ReadFixedAnsiString(effectBytes.Span),
            reader.ReadFixedAnsiString(soundBytes.Span));
        return true;
    }

    private static bool TryParseTarget(ReadOnlySpan<byte> bytes, out SceneTargetRecord? target)
    {
        target = null;
        if (bytes.Length != TargetDataSize)
        {
            return false;
        }

        var reader = new SafeBinaryReader(bytes.ToArray());
        if (!reader.TryReadUInt32(out uint uid) ||
            !reader.TryReadUInt32(out uint modelId) ||
            !reader.TryReadSingle(out float distance) ||
            !reader.TryReadSingle(out float height) ||
            !reader.TryReadBytes(NameBytes, out ReadOnlyMemory<byte> nameBytes) ||
            reader.Position != TargetDataSize)
        {
            return false;
        }

        target = new SceneTargetRecord(
            uid,
            modelId,
            distance,
            height,
            reader.ReadFixedUtf16String(nameBytes.Span));
        return true;
    }
}
