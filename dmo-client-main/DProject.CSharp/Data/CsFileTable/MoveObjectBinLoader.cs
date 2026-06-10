using DProject.Data.Binary;
using DProject.Data.CsFilePack;

namespace DProject.Data.CsFileTable;

public enum MoveObjectKind
{
    Unknown = 0,
    Npc = 100000,
    Monster = 200000
}

public enum MoveObjectActType
{
    Start = 0,
    Wait = 1,
    Move = 2,
    GotoFirstLoop = 3
}

public sealed record MoveObjectBinData(IReadOnlyList<MoveObjectRecord> Objects);

public sealed record MoveObjectRecord(
    int MoveObjectId,
    MoveObjectKind Kind,
    int FileTableId,
    uint MapId,
    float TotalTime,
    IReadOnlyList<MoveObjectActRecord> Acts);

public sealed record MoveObjectActRecord(
    MoveObjectActType ActType,
    int PosX,
    int PosY,
    uint AnimationId,
    int TimeOrVelocity);

public sealed class MoveObjectBinLoader : IBinLoader
{
    private const string MoveObjectBinPath = "Data\\MO.bin";
    private const int MaxActCount = 100000;

    public string TableName => "CsMoveObjectMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        if (!TryReadMoveObjectData(fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, MoveObjectBinPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, MoveObjectBinPath, 0);
        }

        if (!TryParse(data, out MoveObjectBinData? parsed, out BinFileSnapshot snapshot) || parsed is null)
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, MoveObjectBinPath, data.Length, snapshot)
                : new BinLoadResult(BinLoadStatus.Missing, MoveObjectBinPath, data.Length, snapshot);
        }

        return new BinLoadResult(BinLoadStatus.Loaded, MoveObjectBinPath, data.Length, snapshot, parsed);
    }

    private static bool TryReadMoveObjectData(eFILE_TYPE fileType, out byte[] data)
    {
        if (fileType == eFILE_TYPE.FT_FILEPACK &&
            CsFPSystem.TryGetFileData(MoveObjectBinPath, out data))
        {
            return true;
        }

        if (File.Exists(MoveObjectBinPath))
        {
            data = File.ReadAllBytes(MoveObjectBinPath);
            return true;
        }

        data = [];
        return false;
    }

    private static bool TryParse(
        ReadOnlyMemory<byte> data,
        out MoveObjectBinData? parsed,
        out BinFileSnapshot snapshot)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        int offset = reader.Position;
        if (!MapBinFormat.TryReadCount(reader, out int objectCount))
        {
            snapshot = new BinFileSnapshot(
                "CsMoveObjectMng",
                MoveObjectBinPath,
                data.Length,
                [new BinSectionSnapshot("MoveObjects", offset, 0, null, 0, false)]);
            return false;
        }

        var objects = new List<MoveObjectRecord>(objectCount);
        for (int i = 0; i < objectCount; i++)
        {
            if (!reader.TryReadInt32(out int moveObjectId) ||
                !TryParseObject(reader, moveObjectId, out MoveObjectRecord? record) ||
                record is null)
            {
                snapshot = new BinFileSnapshot(
                    "CsMoveObjectMng",
                    MoveObjectBinPath,
                    data.Length,
                    [new BinSectionSnapshot("MoveObjects", offset, objectCount, null, reader.Position - offset, false)]);
                return false;
            }

            objects.Add(record);
        }

        bool complete = reader.EndOfData;
        snapshot = new BinFileSnapshot(
            "CsMoveObjectMng",
            MoveObjectBinPath,
            data.Length,
            [new BinSectionSnapshot("MoveObjects", offset, objectCount, null, reader.Position - offset, complete)]);
        parsed = new MoveObjectBinData(objects);
        return complete;
    }

    private static bool TryParseObject(
        SafeBinaryReader reader,
        int moveObjectId,
        out MoveObjectRecord? record)
    {
        record = null;
        if (!MapBinFormat.TryReadCount(reader, out int actCount) ||
            actCount > MaxActCount ||
            !reader.TryReadUInt32(out uint mapId) ||
            !reader.TryReadSingle(out float totalTime))
        {
            return false;
        }

        var acts = new List<MoveObjectActRecord>(actCount);
        for (int i = 0; i < actCount; i++)
        {
            if (!reader.TryReadInt32(out int rawActType) ||
                !TryParseAct(reader, rawActType, out MoveObjectActRecord? act) ||
                act is null)
            {
                return false;
            }

            acts.Add(act);
        }

        record = new MoveObjectRecord(
            moveObjectId,
            ToKind(moveObjectId),
            ToFileTableId(moveObjectId),
            mapId,
            totalTime,
            acts);
        return true;
    }

    private static bool TryParseAct(
        SafeBinaryReader reader,
        int rawActType,
        out MoveObjectActRecord? act)
    {
        act = null;
        if (!Enum.IsDefined(typeof(MoveObjectActType), rawActType))
        {
            return false;
        }

        var actType = (MoveObjectActType)rawActType;
        switch (actType)
        {
            case MoveObjectActType.Start:
                if (!reader.TryReadInt32(out int startX) ||
                    !reader.TryReadInt32(out int startY))
                {
                    return false;
                }

                act = new MoveObjectActRecord(actType, startX, startY, 0, 0);
                return true;

            case MoveObjectActType.Wait:
                if (!reader.TryReadUInt32(out uint waitAnimation) ||
                    !reader.TryReadInt32(out int waitSeconds))
                {
                    return false;
                }

                act = new MoveObjectActRecord(actType, 0, 0, waitAnimation, waitSeconds);
                return true;

            case MoveObjectActType.Move:
                if (!reader.TryReadInt32(out int moveX) ||
                    !reader.TryReadInt32(out int moveY) ||
                    !reader.TryReadUInt32(out uint moveAnimation) ||
                    !reader.TryReadInt32(out int velocity))
                {
                    return false;
                }

                act = new MoveObjectActRecord(actType, moveX, moveY, moveAnimation, velocity);
                return true;

            case MoveObjectActType.GotoFirstLoop:
                if (!reader.TryReadUInt32(out uint loopAnimation) ||
                    !reader.TryReadInt32(out int loopVelocity))
                {
                    return false;
                }

                act = new MoveObjectActRecord(actType, 0, 0, loopAnimation, loopVelocity);
                return true;

            default:
                return false;
        }
    }

    internal static int ToMoveObjectId(MoveObjectKind kind, int fileTableId) => (int)kind + fileTableId;

    internal static MoveObjectKind ToKind(int moveObjectId)
    {
        int kindValue = moveObjectId / (int)MoveObjectKind.Npc * (int)MoveObjectKind.Npc;
        return Enum.IsDefined(typeof(MoveObjectKind), kindValue)
            ? (MoveObjectKind)kindValue
            : MoveObjectKind.Unknown;
    }

    internal static int ToFileTableId(int moveObjectId) => moveObjectId % (int)MoveObjectKind.Npc;
}
