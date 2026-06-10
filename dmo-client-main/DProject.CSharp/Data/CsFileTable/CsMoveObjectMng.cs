namespace DProject.Data.CsFileTable;

public sealed record MoveObjectPositionSample(
    float X,
    float Y,
    float DirectionX,
    float DirectionY,
    uint AnimationId,
    int MoveSpeed,
    bool IsMoving);

public sealed class CsMoveObjectMng : IBinFileTableManager
{
    private readonly MoveObjectBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<int, MoveObjectRecord> _objects = new();

    public CsMoveObjectMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsMoveObjectMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public IReadOnlyDictionary<int, MoveObjectRecord> Objects => _objects;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is MoveObjectBinData data)
        {
            foreach (MoveObjectRecord obj in data.Objects)
            {
                _objects.TryAdd(obj.MoveObjectId, obj);
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _objects.Clear();
        IsLoaded = false;
    }

    public bool IsObject(MoveObjectKind kind, int fileTableId)
    {
        return fileTableId < (int)MoveObjectKind.Npc &&
            IsObject(MoveObjectBinLoader.ToMoveObjectId(kind, fileTableId));
    }

    public bool IsObject(int moveObjectId) => _objects.ContainsKey(moveObjectId);

    public MoveObjectRecord GetObject(MoveObjectKind kind, int fileTableId)
    {
        return GetObject(MoveObjectBinLoader.ToMoveObjectId(kind, fileTableId));
    }

    public MoveObjectRecord GetObject(int moveObjectId)
    {
        if (_objects.TryGetValue(moveObjectId, out MoveObjectRecord? obj))
        {
            return obj;
        }

        obj = new MoveObjectRecord(
            moveObjectId,
            MoveObjectBinLoader.ToKind(moveObjectId),
            MoveObjectBinLoader.ToFileTableId(moveObjectId),
            0,
            0,
            []);
        _objects[moveObjectId] = obj;
        return obj;
    }

    public bool TryGetPosition(
        int moveObjectId,
        float accumulatedTime,
        out MoveObjectPositionSample sample)
    {
        sample = new MoveObjectPositionSample(0, 0, 0, 0, uint.MaxValue, 0, false);
        if (!_objects.TryGetValue(moveObjectId, out MoveObjectRecord? obj) ||
            obj.TotalTime <= 0 ||
            obj.Acts.Count == 0)
        {
            return false;
        }

        float time = accumulatedTime;
        if (time < 0)
        {
            time = 0;
        }
        else if (time > obj.TotalTime)
        {
            time %= obj.TotalTime;
        }

        return TrySamplePosition(obj, time, out sample);
    }

    public (int X, int Y) GetFirstMovePoint(int moveObjectId)
    {
        MoveObjectRecord obj = GetObject(moveObjectId);
        foreach (MoveObjectActRecord act in obj.Acts)
        {
            if (act.ActType is MoveObjectActType.Start or MoveObjectActType.Move)
            {
                return (act.PosX, act.PosY);
            }
        }

        return (0, 0);
    }

    public (int X, int Y) GetLastMovePoint(int moveObjectId)
    {
        MoveObjectRecord obj = GetObject(moveObjectId);
        int x = 0;
        int y = 0;
        foreach (MoveObjectActRecord act in obj.Acts)
        {
            if (act.ActType is MoveObjectActType.Start or MoveObjectActType.Move)
            {
                x = act.PosX;
                y = act.PosY;
            }
        }

        return (x, y);
    }

    private static bool TrySamplePosition(
        MoveObjectRecord obj,
        float accumulatedTime,
        out MoveObjectPositionSample sample)
    {
        int startX = 0;
        int startY = 0;
        int previousX = 0;
        int previousY = 0;
        int nextX = 0;
        int nextY = 0;
        int velocity = 0;
        uint animationId = uint.MaxValue;
        bool hasPoint = false;
        bool foundAct = false;
        float remainingTime = accumulatedTime;
        float currentActTime = accumulatedTime;

        foreach (MoveObjectActRecord act in obj.Acts)
        {
            currentActTime = remainingTime;
            switch (act.ActType)
            {
                case MoveObjectActType.Start:
                    startX = previousX = nextX = act.PosX;
                    startY = previousY = nextY = act.PosY;
                    hasPoint = true;
                    break;

                case MoveObjectActType.Wait:
                    if ((remainingTime -= act.TimeOrVelocity) < 0)
                    {
                        animationId = act.AnimationId;
                        velocity = 0;
                        foundAct = true;
                    }
                    break;

                case MoveObjectActType.Move:
                    nextX = act.PosX;
                    nextY = act.PosY;
                    velocity = act.TimeOrVelocity;
                    if (!hasPoint)
                    {
                        previousX = nextX;
                        previousY = nextY;
                        hasPoint = true;
                    }

                    if ((remainingTime -= GetMoveTime(previousX, previousY, nextX, nextY, velocity)) < 0)
                    {
                        animationId = act.AnimationId;
                        foundAct = true;
                    }
                    else
                    {
                        previousX = nextX;
                        previousY = nextY;
                    }
                    break;

                case MoveObjectActType.GotoFirstLoop:
                    nextX = startX;
                    nextY = startY;
                    velocity = act.TimeOrVelocity;
                    if ((remainingTime -= GetMoveTime(previousX, previousY, nextX, nextY, velocity)) < 0)
                    {
                        animationId = act.AnimationId;
                        foundAct = true;
                    }
                    else
                    {
                        previousX = nextX;
                        previousY = nextY;
                    }
                    break;
            }

            if (foundAct)
            {
                break;
            }
        }

        if (!foundAct)
        {
            animationId = uint.MaxValue;
        }

        if (velocity == 0 || !foundAct)
        {
            sample = new MoveObjectPositionSample(nextX, nextY, 0, 0, animationId, 0, false);
            return foundAct || hasPoint;
        }

        float dirX = nextX - previousX;
        float dirY = nextY - previousY;
        float length = MathF.Sqrt(dirX * dirX + dirY * dirY);
        if (length <= 0)
        {
            sample = new MoveObjectPositionSample(nextX, nextY, 0, 0, animationId, 0, false);
            return true;
        }

        float reciprocal = 1.0f / length;
        float normX = dirX * reciprocal;
        float normY = dirY * reciprocal;
        float x = previousX + normX * currentActTime * velocity;
        float y = previousY + normY * currentActTime * velocity;
        sample = new MoveObjectPositionSample(x, y, normX, normY, animationId, velocity, true);
        return true;
    }

    private static float GetMoveTime(int fromX, int fromY, int toX, int toY, int velocity)
    {
        if (velocity <= 0)
        {
            return 0;
        }

        int dx = toX - fromX;
        int dy = toY - fromY;
        return MathF.Sqrt(dx * dx + dy * dy) / velocity;
    }
}
