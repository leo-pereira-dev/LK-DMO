using DProject.Gamebryo;

namespace DProject.Game;

public sealed record sTCUnit(string FilePath, CsC_AvObject? LoadedObject = null);

public sealed class sCHAR_IMAGE
{
    public sCHAR_IMAGE(uint modelId)
    {
        ModelID = modelId;
    }

    public uint ModelID { get; }
    public int RefCount { get; private set; } = 1;
    public float LastUsedTime { get; private set; }

    public void AddRef() => RefCount++;

    public void Release()
    {
        if (RefCount > 0)
        {
            RefCount--;
        }
    }

    public void Touch(float time) => LastUsedTime = time;
}

public sealed class CCharResMng
{
    private readonly Queue<CsC_AvObject> _queueDeleteReady = new();
    private readonly Dictionary<uint, sCHAR_IMAGE> _mapCharImage = new();
    private readonly List<sNPC_MARK> _npcMarks = [];
    private readonly List<sMONSTER_CREATE_SCENE_INFO> _monsterCreateScenes = [];
    private readonly List<sFIGURE_INFO> _figures = [];
    private readonly List<sTCUnit> _loadRequests = [];
    private float _time;

    public CsC_AvObject? m_pMouseOnLastObject { get; private set; }
    public CsC_AvObject? m_pMouseOnObject { get; private set; }
    public float m_fMouseOnObjectAlpha { get; private set; }
    public float m_fMouseOnObjectDimmer { get; private set; }
    public NiLight? m_pDefaultLight { get; private set; }
    public bool m_bMovePointRender { get; private set; }
    public bool m_bEnableMovePoint { get; private set; }
    public NiPoint3 m_vMovePoint { get; private set; }
    public CsC_AvObject? m_pTargetObject { get; private set; }
    public IReadOnlyList<sNPC_MARK> NpcMarks => _npcMarks;
    public IReadOnlyList<sMONSTER_CREATE_SCENE_INFO> MonsterCreateScenes => _monsterCreateScenes;
    public IReadOnlyList<sFIGURE_INFO> Figures => _figures;
    public IReadOnlyList<sTCUnit> LoadRequests => _loadRequests;

    public void Init()
    {
        CreateDefaultLight();
        m_bEnableMovePoint = true;
    }

    public void Destroy()
    {
        Reset();
        DeleteDefaultLight();
    }

    public void Reset()
    {
        _queueDeleteReady.Clear();
        _npcMarks.Clear();
        _monsterCreateScenes.Clear();
        _figures.Clear();
        _loadRequests.Clear();
        _mapCharImage.Clear();
        m_pMouseOnLastObject = null;
        m_pMouseOnObject = null;
        m_pTargetObject = null;
        ReleaseMovePoint();
    }

    public void PostLoadMap()
    {
        _figures.Clear();
        _npcMarks.Clear();
    }

    public void LoadChar(sTCUnit pUnit)
    {
        _loadRequests.Add(pUnit);
        pUnit.LoadedObject?.SetPresent(true);
    }

    public void Update(float deltaTime)
    {
        _time += deltaTime;
        UpdateDeleteReady();
        UpdateCharImage();
        UpdateTargetMark();
    }

    public void Render_PostTR()
    {
    }

    public void Render()
    {
    }

    public void Render_PostChar()
    {
    }

    public void ResetDevice()
    {
    }

    public void DeleteChar(CsC_AvObject? pObject)
    {
        pObject?.PreDelete();
    }

    public void ThreadDeleteChar(CsC_AvObject? pObject)
    {
        if (pObject is null)
        {
            return;
        }

        pObject.Delete();
    }

    public void InsertDeleteReady(CsC_AvObject? pObject)
    {
        if (pObject is not null)
        {
            _queueDeleteReady.Enqueue(pObject);
        }
    }

    public void CharImageResDelete(ref sCHAR_IMAGE? ppCharImage)
    {
        if (ppCharImage is null)
        {
            return;
        }

        ppCharImage.Release();
        if (ppCharImage.RefCount <= 0)
        {
            _mapCharImage.Remove(ppCharImage.ModelID);
        }

        ppCharImage = null;
    }

    public sCHAR_IMAGE CharImageResLoad(uint nModelID)
    {
        if (!_mapCharImage.TryGetValue(nModelID, out sCHAR_IMAGE? image))
        {
            image = new sCHAR_IMAGE(nModelID);
            _mapCharImage[nModelID] = image;
        }
        else
        {
            image.AddRef();
        }

        image.Touch(_time);
        return image;
    }

    public NiLight? GetDefaultLight() => m_pDefaultLight;

    public void SetMovePoint(NiPoint3 vMovePoint)
    {
        m_vMovePoint = vMovePoint;
        m_bMovePointRender = true;
    }

    public void ReleaseMovePoint()
    {
        m_bMovePointRender = false;
        m_vMovePoint = NiPoint3.Zero;
    }

    public void SetEnableMovePoint(bool bEnable) => m_bEnableMovePoint = bEnable;
    public bool IsEnableMovePoint() => m_bEnableMovePoint;

    public void SetTargetMark(CsC_AvObject? pObject, bool bOnEvent = true)
    {
        m_pTargetObject = pObject;
    }

    public void ReleaseTargetMark(bool bAbsoluteRelease)
    {
        if (bAbsoluteRelease || m_pTargetObject?.IsPresent != true)
        {
            m_pTargetObject = null;
        }
    }

    public CsC_AvObject? GetTargetMark() => m_pTargetObject;

    public void NpcMarkInsert(int nNpcIDX, NiPoint3 vPos, int eDispType)
    {
        _npcMarks.RemoveAll(mark => mark.s_nNpcIDX == nNpcIDX);
        _npcMarks.Add(new sNPC_MARK(nNpcIDX, vPos, true, eDispType));
    }

    public void MonsterCreateSceneInsert(NiPoint3 vPos, float fScale, CsC_AvObject? pTarget)
    {
        _monsterCreateScenes.Add(new sMONSTER_CREATE_SCENE_INFO(vPos, 0.0f, fScale, pTarget?.GetUniqID() ?? 0));
    }

    public void FigureInsert(NiPoint3 vPos, float fRot, float fAniTime, sFIGURE_INFO.eFIGURETYPE eFigureType)
    {
        _figures.Add(new sFIGURE_INFO((int)eFigureType, vPos, fRot, fAniTime));
    }

    private void UpdateDeleteReady()
    {
        while (_queueDeleteReady.TryDequeue(out CsC_AvObject? obj))
        {
            ThreadDeleteChar(obj);
        }
    }

    private void UpdateCharImage()
    {
        foreach (uint modelId in _mapCharImage
            .Where(pair => pair.Value.RefCount <= 0 || (_time - pair.Value.LastUsedTime) > 90.0f)
            .Select(pair => pair.Key)
            .ToArray())
        {
            _mapCharImage.Remove(modelId);
        }
    }

    private void UpdateTargetMark()
    {
        if (m_pTargetObject?.IsPresent == false)
        {
            m_pTargetObject = null;
        }
    }

    private void CreateDefaultLight()
    {
        m_pDefaultLight = new NiLight();
        m_pDefaultLight.SetName("DefaultLight");
    }

    private void DeleteDefaultLight()
    {
        m_pDefaultLight = null;
    }

    public sealed record sNPC_MARK(int s_nNpcIDX, NiPoint3 s_vPos, bool s_bExistOwner, int s_eDiplayType);
    public sealed record sMONSTER_CREATE_SCENE_INFO(NiPoint3 s_vPos, float s_fAniTime, float s_fScale, uint s_nTargetUID);

    public sealed record sFIGURE_INFO(int s_nType, NiPoint3 s_vPos, float s_fRot, float s_fAniTime)
    {
        public enum eFIGURETYPE
        {
            FT_TAMER,
            FT_DIGIMON,
            FT_EMPLOYMENT,
            FT_PAT
        }
    }
}
