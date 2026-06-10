using DProject.Gamebryo;
using DProject.Runtime;

namespace DProject.Game;

public sealed class CsC_EffectObject : CsC_AvObject
{
    public string EffectPath { get; private set; } = string.Empty;
    public float Scale { get; private set; } = 1.0f;
    public float LifeTime { get; private set; }

    public void InitEffect(uint uid, string effectPath, NiPoint3 position, float scale)
    {
        EffectPath = effectPath;
        Scale = scale;
        LifeTime = 0.0f;
        Init(uid, uid & 0x3FFF, 0, 0, position, 0.0f, effectPath);
    }

    public override void Update(float deltaTime, bool animation = true)
    {
        LifeTime += deltaTime;
    }
}

public readonly record struct EffectRenderCommand(uint Uid, string EffectPath, NiPoint3 Position, float Scale);

public sealed class CEffectMng : CsC_AvObject
{
    private readonly List<CsC_EffectObject> _effects = [];
    private readonly List<sMARBLE_INFO> _marbles = [];
    private readonly Queue<sMARBLE_INFO>[] _marblePool;
    private uint _nextEffectUid = 1;

    public CEffectMng()
    {
        _marblePool = Enumerable.Range(0, (int)eMARBLE_TYPE.MT_MAX_CNT)
            .Select(static _ => new Queue<sMARBLE_INFO>())
            .ToArray();
    }

    public enum eMARBLE_TYPE
    {
        MT_EVOLUTION,
        MT_MAX_CNT
    }

    public IReadOnlyList<CsC_EffectObject> Effects => _effects;
    public IReadOnlyList<sMARBLE_INFO> Marbles => _marbles;

    public void Reset()
    {
        foreach (CsC_EffectObject effect in _effects)
        {
            effect.Delete();
        }

        _effects.Clear();
        ResetMarble();
    }

    public override void Update(float fDeltaTime, bool animation = true)
    {
        foreach (CsC_EffectObject effect in _effects)
        {
            effect.Update(fDeltaTime, animation);
        }

        UpdateMarble(fDeltaTime);
    }

    public void Render(RenderCommandQueue? renderCommands = null)
    {
        foreach (CsC_EffectObject effect in _effects)
        {
            EffectRenderCommand command = new(effect.GetUniqID(), effect.EffectPath, effect.GetPos(), effect.Scale);
            renderCommands?.Enqueue(() =>
            {
                _ = command;
                // Placeholder until CsC_EffectObject/Gamebryo rendering is ported.
            });
        }

        RenderMarble(renderCommands);
    }

    public CsC_EffectObject AddEffect(string cEffectPath, NiPoint3 vPos, float fScale)
    {
        var effect = new CsC_EffectObject();
        effect.InitEffect(_nextEffectUid++, cEffectPath, vPos, fScale);
        _effects.Add(effect);
        return effect;
    }

    public void DeleteEffect(CsC_AvObject pDelEffect)
    {
        _effects.RemoveAll(effect => effect.GetUniqID() == pDelEffect.GetUniqID());
        pDelEffect.Delete();
    }

    public void Init_Marble()
    {
    }

    public void Destroy_Marble()
    {
        ResetMarble();
        foreach (Queue<sMARBLE_INFO> pool in _marblePool)
        {
            pool.Clear();
        }
    }

    public void AddEffect_Marble(eMARBLE_TYPE mt, NiPoint3 vSrc, CsC_AvObject pOwner, int nCnt)
    {
        for (int i = 0; i < nCnt; i++)
        {
            _marbles.Add(NewMarble(mt, vSrc, pOwner));
        }
    }

    private void UpdateMarble(float fDeltaTime)
    {
        for (int i = _marbles.Count - 1; i >= 0; i--)
        {
            sMARBLE_INFO marble = _marbles[i];
            marble.s_fAniTime += fDeltaTime;

            switch (marble.s_eStep)
            {
                case sMARBLE_INFO.eSTEP.CREATE:
                    UpdateMarbleCreate(marble);
                    break;
                case sMARBLE_INFO.eSTEP.FALLOW:
                    UpdateMarbleFollow(marble, fDeltaTime);
                    break;
                case sMARBLE_INFO.eSTEP.DIE:
                    if (UpdateMarbleDie(marble, fDeltaTime))
                    {
                        _marbles.RemoveAt(i);
                        DelMarble(marble);
                    }
                    break;
            }
        }
    }

    private static void UpdateMarbleCreate(sMARBLE_INFO marble)
    {
        float rate = MathF.Min(1.0f, marble.s_fAniTime * 2.4f);
        marble.s_vCurPos = Lerp(marble.s_vInitPos, marble.s_vTrPos, rate);
        if (rate >= 1.0f)
        {
            marble.s_eStep = sMARBLE_INFO.eSTEP.FALLOW;
            marble.s_fAniTime = 0.0f;
        }
    }

    private static void UpdateMarbleFollow(sMARBLE_INFO marble, float deltaTime)
    {
        if (!marble.s_pOwner.IsLoad())
        {
            marble.s_eStep = sMARBLE_INFO.eSTEP.DIE;
            marble.s_fOnTrTime = 0.2f;
            return;
        }

        NiPoint3 dest = marble.s_pOwner.GetPos().WithZ(marble.s_pOwner.GetPos().Z + (marble.s_pOwner.GetToolHeight() * 0.5f));
        NiPoint3 delta = dest - marble.s_vCurPos;
        float distance = delta.Length();
        if (distance <= 0.001f)
        {
            marble.s_eStep = sMARBLE_INFO.eSTEP.DIE;
            marble.s_fOnTrTime = 0.2f;
            return;
        }

        float speed = MathF.Pow(marble.s_fAniTime + 1.0f, 6.0f) * 60.0f * deltaTime + 50.0f;
        if (distance <= speed)
        {
            marble.s_vCurPos = dest;
            marble.s_eStep = sMARBLE_INFO.eSTEP.DIE;
            marble.s_fOnTrTime = 0.2f;
            return;
        }

        marble.s_vCurPos += delta.Unitize() * speed;
    }

    private static bool UpdateMarbleDie(sMARBLE_INFO marble, float deltaTime)
    {
        if (!marble.s_pOwner.IsLoad())
        {
            return true;
        }

        marble.s_fOnTrTime -= deltaTime;
        NiPoint3 ownerPos = marble.s_pOwner.GetPos();
        marble.s_vCurPos = ownerPos.WithZ(ownerPos.Z + (marble.s_pOwner.GetToolHeight() * 0.5f));
        return marble.s_fOnTrTime <= 0.0f;
    }

    private void RenderMarble(RenderCommandQueue? renderCommands)
    {
        foreach (sMARBLE_INFO marble in _marbles)
        {
            EffectRenderCommand command = new((uint)marble.GetHashCode(), marble.s_eType.ToString(), marble.s_vCurPos, 1.0f);
            renderCommands?.Enqueue(() =>
            {
                _ = command;
                // Placeholder until marble NiNode rendering is ported.
            });
        }
    }

    private sMARBLE_INFO NewMarble(eMARBLE_TYPE mt, NiPoint3 vSrc, CsC_AvObject pOwner)
    {
        Queue<sMARBLE_INFO> pool = _marblePool[(int)mt];
        sMARBLE_INFO marble = pool.Count > 0 ? pool.Dequeue() : new sMARBLE_INFO();
        marble.s_eType = mt;
        marble.s_eStep = sMARBLE_INFO.eSTEP.CREATE;
        marble.s_pOwner = pOwner;
        marble.s_fAniTime = 0.0f;
        marble.s_fOnTrTime = Random.Shared.Next(500, 1000) * 0.001f;
        marble.s_vInitPos = vSrc;
        marble.s_vTrPos = PickTransitionPoint(vSrc, pOwner.GetPos());
        marble.s_vSpline1 = vSrc + new NiPoint3(0.0f, 0.0f, -100.0f);
        marble.s_vSpline2 = marble.s_vTrPos + new NiPoint3(0.0f, 0.0f, -Random.Shared.Next(1000, 3000));
        marble.s_vCurPos = vSrc;
        return marble;
    }

    private void ResetMarble()
    {
        foreach (sMARBLE_INFO marble in _marbles)
        {
            DelMarble(marble);
        }

        _marbles.Clear();
    }

    private void DelMarble(sMARBLE_INFO pInfo)
    {
        _marblePool[(int)pInfo.s_eType].Enqueue(pInfo);
    }

    private static NiPoint3 PickTransitionPoint(NiPoint3 source, NiPoint3 owner)
    {
        NiPoint3 direction = (owner - source).WithZ(0.0f).Unitize();
        if (direction == NiPoint3.Zero)
        {
            direction = new NiPoint3(0.0f, 1.0f, 0.0f);
        }

        float angle = Random.Shared.Next(-60, 60) * (MathF.PI / 180.0f);
        float cos = MathF.Cos(angle);
        float sin = MathF.Sin(angle);
        NiPoint3 rotated = new(
            (direction.X * cos) - (direction.Y * sin),
            (direction.X * sin) + (direction.Y * cos),
            0.0f);
        float distance = Random.Shared.Next(400, 600);
        return source + (rotated * distance) + new NiPoint3(0.0f, 0.0f, Random.Shared.Next(50, 210));
    }

    private static NiPoint3 Lerp(NiPoint3 start, NiPoint3 end, float rate)
    {
        return start + ((end - start) * rate);
    }

    public sealed class sMARBLE_INFO
    {
        public enum eSTEP
        {
            CREATE,
            ON_TR,
            FALLOW,
            DIE
        }

        public eMARBLE_TYPE s_eType;
        public eSTEP s_eStep;
        public CsC_AvObject s_pOwner = new();
        public float s_fAniTime;
        public float s_fOnTrTime;
        public NiPoint3 s_vInitPos;
        public NiPoint3 s_vTrPos;
        public NiPoint3 s_vSpline1;
        public NiPoint3 s_vSpline2;
        public NiPoint3 s_vCurPos;
    }
}
