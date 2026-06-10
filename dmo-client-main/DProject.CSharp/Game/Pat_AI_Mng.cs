namespace DProject.Game;

public sealed class CPat_AI_Mng
{
    private readonly Dictionary<int, sAI_Container> _containers = new();
    private readonly Random _random = new();

    public CPat_AI_Mng()
    {
        m_nInitContainerID = 0;
    }

    public enum eCHECK_TYPE
    {
        CHK_NONE,
        CHK_TIME,
        CHK_ANI,
        CHK_MOVE
    }

    public enum eMODUL
    {
        MODUL_Container,
        MODUL_Element
    }

    public int m_nInitContainerID { get; private set; }
    public IReadOnlyDictionary<int, sAI_Container> Containers => _containers;

    public void Init()
    {
    }

    public void Delete()
    {
        foreach (sAI_Container container in _containers.Values)
        {
            container.Delete();
        }

        _containers.Clear();
    }

    public sAI_Container CreateContatiner(int nCon)
    {
        if (_containers.ContainsKey(nCon))
        {
            throw new InvalidOperationException($"Pat AI container already exists: {nCon}");
        }

        var container = new sAI_Container(this) { s_nID = nCon };
        _containers[nCon] = container;
        return container;
    }

    public sAI_Container GetContainer(int con)
    {
        if (!_containers.TryGetValue(con, out sAI_Container? container))
        {
            throw new KeyNotFoundException($"Pat AI container was not found: {con}");
        }

        return container;
    }

    public void SetContainerStartID(int nID) => m_nInitContainerID = nID;
    public int GetContainerStartID() => m_nInitContainerID;
    public sAI_Element NewTime(int nID, int ani, float fSec) => sAI_Element.NewTime(this, nID, ani, fSec);
    public sAI_Element NewAnimation(int nID, int ani, int nLoopCount) => sAI_Element.NewAnimation(this, nID, ani, nLoopCount);
    public sAI_Element NewMove(int nID, int act, int ani) => sAI_Element.NewMove(this, nID, act, ani);

    public abstract class sAI_Base
    {
        private readonly CPat_AI_Mng _manager;
        private readonly List<sNEXT> _next = [];

        protected sAI_Base(CPat_AI_Mng manager)
        {
            _manager = manager;
        }

        public int s_nID { get; set; }
        public int s_nNextTotalRate { get; private set; }
        public IReadOnlyList<sNEXT> s_vpNext => _next;

        public abstract eMODUL GetModul();

        public virtual void Delete()
        {
            foreach (sNEXT next in _next)
            {
                if (next.s_pNext.GetModul() == eMODUL.MODUL_Element)
                {
                    next.s_pNext.Delete();
                }
            }

            _next.Clear();
            s_nNextTotalRate = 0;
        }

        public sAI_Base? FindElement(int nID)
        {
            foreach (sNEXT next in _next)
            {
                sAI_Base candidate = next.s_pNext;
                if (candidate.GetModul() == eMODUL.MODUL_Element && candidate.s_nID == nID)
                {
                    return candidate;
                }

                sAI_Base? nested = candidate.FindElement(nID);
                if (nested is not null)
                {
                    return nested;
                }
            }

            return null;
        }

        public sAI_Base GetNext()
        {
            if (_next.Count == 0 || s_nNextTotalRate <= 0)
            {
                return _manager.GetContainer(_manager.GetContainerStartID()).GetNext();
            }

            int rate = _manager._random.Next(s_nNextTotalRate);
            foreach (sNEXT next in _next)
            {
                if (rate < next.s_nRate)
                {
                    return next.s_pNext.GetModul() == eMODUL.MODUL_Element
                        ? next.s_pNext
                        : next.s_pNext.GetNext();
                }

                rate -= next.s_nRate;
            }

            return _next[0].s_pNext;
        }

        public sAI_Base Add(sAI_Base pNext, int nRate)
        {
            if (nRate <= 0)
            {
                throw new ArgumentOutOfRangeException(nameof(nRate), "Pat AI transition rate must be positive.");
            }

            s_nNextTotalRate += nRate;
            _next.Add(new sNEXT(pNext, nRate));
            return pNext;
        }
    }

    public sealed class sAI_Element : sAI_Base
    {
        private sAI_Element(CPat_AI_Mng manager) : base(manager)
        {
        }

        public eCHECK_TYPE s_eCheckType { get; private set; }
        public int s_eAnimation { get; private set; }
        public sTIME s_Time { get; private set; }
        public sANIMATION s_Animation { get; private set; }
        public sMOVE s_Move { get; private set; }

        public override eMODUL GetModul() => eMODUL.MODUL_Element;

        public static sAI_Element NewTime(CPat_AI_Mng manager, int nID, int ani, float fSec)
        {
            if (fSec <= 0.0f)
            {
                throw new ArgumentOutOfRangeException(nameof(fSec), "Pat AI time check must be positive.");
            }

            return new sAI_Element(manager)
            {
                s_eCheckType = eCHECK_TYPE.CHK_TIME,
                s_nID = nID,
                s_eAnimation = ani,
                s_Time = new sTIME(fSec)
            };
        }

        public static sAI_Element NewAnimation(CPat_AI_Mng manager, int nID, int ani, int nLoopCount)
        {
            return new sAI_Element(manager)
            {
                s_eCheckType = eCHECK_TYPE.CHK_ANI,
                s_nID = nID,
                s_eAnimation = ani,
                s_Animation = new sANIMATION(nLoopCount)
            };
        }

        public static sAI_Element NewMove(CPat_AI_Mng manager, int nID, int act, int ani)
        {
            return new sAI_Element(manager)
            {
                s_eCheckType = eCHECK_TYPE.CHK_MOVE,
                s_nID = nID,
                s_eAnimation = ani,
                s_Move = new sMOVE(act)
            };
        }
    }

    public sealed class sAI_Container : sAI_Base
    {
        public sAI_Container(CPat_AI_Mng manager) : base(manager)
        {
        }

        public override eMODUL GetModul() => eMODUL.MODUL_Container;
    }

    public readonly record struct sNEXT(sAI_Base s_pNext, int s_nRate);
    public readonly record struct sTIME(float s_fTimeSec);
    public readonly record struct sANIMATION(int s_nLoopCount);
    public readonly record struct sMOVE(int s_nAct);
}
