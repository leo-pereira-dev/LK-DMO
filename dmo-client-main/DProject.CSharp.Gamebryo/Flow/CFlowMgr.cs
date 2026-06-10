namespace DProject.Flow;

public sealed class CFlowMgr
{
    private readonly CFlowFactory _factory;
    private readonly CFlowCmdQueue _flowCmdQueue = new();
    private readonly CFlowStack _flowStack = new();
    private readonly Dictionary<CFlow.FLOW_IDS, CFlow> _flowInstances = new();
    private FlowContext? _context;
    private bool _flowLock;
    private bool _processingLock;

    public CFlowMgr(CFlowFactory factory)
    {
        _factory = factory;
    }

    public CFlow.FLOW_IDS CurrentFlowId => GetCurTopFlowID();

    public bool IsProcessingCommands => _processingLock;

    public void Create(FlowContext context)
    {
        _context = context;
    }

    public void SetFlowLock(bool locked)
    {
        _flowLock = locked;
    }

    public void Destroy()
    {
        CurFlowImmediatelyDestroy();
        _flowCmdQueue.Clear();
        _flowStack.Clear();
        _flowInstances.Clear();
    }

    public void StartFlow(CFlow.FLOW_IDS id)
    {
        EnsureContext();
        if (!_flowStack.IsEmpty())
        {
            throw new InvalidOperationException("StartFlow requires an empty stack.");
        }

        PushFlow(id);
    }

    public void PushFlow(CFlow.FLOW_IDS id)
    {
        if (!_flowCmdQueue.IsEmpty())
        {
            return;
        }

        foreach (CFlow flow in CurrentFlows())
        {
            flow.ReservedPushFlow(id);
        }

        _flowCmdQueue.PushFlow(id);
    }

    public void ChangeFlow(CFlow.FLOW_IDS id)
    {
        if (!_flowCmdQueue.IsEmpty() || _flowCmdQueue.IsFrontChange(id))
        {
            return;
        }

        foreach (CFlow flow in CurrentFlows())
        {
            flow.ReservedChangeFlow(id);
        }

        _flowCmdQueue.ChangeFlow(id);
    }

    public void ChangePopAllFlow(CFlow.FLOW_IDS id)
    {
        if (!_flowCmdQueue.IsEmpty())
        {
            return;
        }

        foreach (CFlow flow in CurrentFlows())
        {
            flow.ReservedChangeFlow(id);
        }

        _flowCmdQueue.ChangePopAllFlow(id);
    }

    public void PopFlow(CFlow.FLOW_IDS id)
    {
        if (!_flowCmdQueue.IsEmpty())
        {
            return;
        }

        foreach (CFlow flow in CurrentFlows())
        {
            flow.ReservedPopFlow();
        }

        _flowCmdQueue.PopFlow(id);
    }

    public void PopAllFlow()
    {
        if (_flowCmdQueue.IsFrontPopAll())
        {
            return;
        }

        _flowCmdQueue.PopAllFlow();
    }

    public void OnIdle()
    {
        CMDUpdate();

        foreach (CFlow flow in CurrentFlows())
        {
            flow.OnIdle();
        }

        CMDUpdate();
    }

    private void CMDUpdate()
    {
        while (!_flowCmdQueue.IsEmpty())
        {
            if (_flowLock)
            {
                return;
            }

            _processingLock = true;
            if (!_flowCmdQueue.TryGetAt(out CFlowCmdQueue.FLOWCMD command))
            {
                _processingLock = false;
                return;
            }

            switch (command.CmdType)
            {
                case CFlowCmdQueue.CMDTYPE.CMD_PUSH:
                    ProcessPush(command.Id);
                    break;
                case CFlowCmdQueue.CMDTYPE.CMD_CHANGE:
                    ProcessChange(command.Id);
                    break;
                case CFlowCmdQueue.CMDTYPE.CMD_CHANGE_POP_ALL:
                    ProcessChangePopAll(command.Id);
                    break;
                case CFlowCmdQueue.CMDTYPE.CMD_POP:
                    ProcessPop(command.Id);
                    break;
                case CFlowCmdQueue.CMDTYPE.CMD_POP_ALL:
                    CurFlowImmediatelyDestroy();
                    break;
            }

            _processingLock = false;
        }
    }

    private void ProcessPush(CFlow.FLOW_IDS id)
    {
        CFlow.FLOW_IDS currentId = GetCurTopFlowID();
        if (currentId == id)
        {
            return;
        }

        if (TryGetCurTopFlow(out CFlow current))
        {
            current.OnExit(id);
        }

        RegisterFlow(id);
        if (TryGetCurTopFlow(out CFlow next))
        {
            next.OnEnter();
        }
    }

    private void ProcessChange(CFlow.FLOW_IDS id)
    {
        CFlow.FLOW_IDS currentId = GetCurTopFlowID();
        if (currentId == id)
        {
            return;
        }

        if (TryGetCurTopFlow(out CFlow current))
        {
            current.OnExit(id);
            EraseFlow(currentId);
        }

        RegisterFlow(id);
        if (TryGetCurTopFlow(out CFlow next))
        {
            next.OnEnter();
        }
    }

    private void ProcessChangePopAll(CFlow.FLOW_IDS id)
    {
        CurFlowImmediatelyDestroy(id);
        RegisterFlow(id);
        if (TryGetCurTopFlow(out CFlow next))
        {
            next.OnEnter();
        }
    }

    private void ProcessPop(CFlow.FLOW_IDS id)
    {
        CFlow.FLOW_IDS currentId = GetCurTopFlowID();
        if (currentId == CFlow.FLOW_IDS.FLW_NONE)
        {
            return;
        }

        if (TryGetCurTopFlow(out CFlow current))
        {
            current.OnExit(id);
            EraseFlow(currentId);
        }

        if (TryGetCurTopFlow(out CFlow resumed))
        {
            resumed.OnEnter();
        }
    }

    private void RegisterFlow(CFlow.FLOW_IDS id)
    {
        if (_flowStack.IsExistID(id))
        {
            return;
        }

        CFlow flow = _factory.CreateFlow(id, EnsureContext());
        _flowInstances[id] = flow;
        _flowStack.Push(id);
    }

    private void EraseFlow(CFlow.FLOW_IDS id)
    {
        if (id == CFlow.FLOW_IDS.FLW_NONE)
        {
            return;
        }

        _flowInstances.Remove(id);
        if (_flowStack.GetTopAt() == id)
        {
            _flowStack.Pop();
        }
    }

    private void CurFlowImmediatelyDestroy(CFlow.FLOW_IDS nextId = CFlow.FLOW_IDS.FLW_NONE)
    {
        while (!_flowStack.IsEmpty())
        {
            CFlow.FLOW_IDS id = _flowStack.GetTopAt();
            if (_flowInstances.TryGetValue(id, out CFlow? flow))
            {
                flow.OnExit(nextId);
            }

            EraseFlow(id);
        }
    }

    private CFlow.FLOW_IDS GetCurTopFlowID()
    {
        return _flowStack.GetTopAt();
    }

    private bool TryGetCurTopFlow(out CFlow flow)
    {
        CFlow.FLOW_IDS id = GetCurTopFlowID();
        if (_flowInstances.TryGetValue(id, out CFlow? found))
        {
            flow = found;
            return true;
        }

        flow = null!;
        return false;
    }

    private IEnumerable<CFlow> CurrentFlows()
    {
        int size = _flowStack.GetSize();
        for (int i = 0; i < size; i++)
        {
            CFlow.FLOW_IDS id = _flowStack.GetPosAt(i);
            if (_flowInstances.TryGetValue(id, out CFlow? flow))
            {
                yield return flow;
            }
        }
    }

    private FlowContext EnsureContext()
    {
        return _context ?? throw new InvalidOperationException("FlowMgr was not created.");
    }
}
