using System.Collections.Concurrent;

namespace DProject.Flow;

public sealed class CFlowCmdQueue
{
    public enum CMDTYPE
    {
        CMD_PUSH,
        CMD_POP,
        CMD_CHANGE,
        CMD_CHANGE_POP_ALL,
        CMD_POP_ALL
    }

    public readonly record struct FLOWCMD(CMDTYPE CmdType, CFlow.FLOW_IDS Id, float Delay = 0.0f);

    private readonly ConcurrentQueue<FLOWCMD> _queue = new();

    public void PushFlow(CFlow.FLOW_IDS id) => _queue.Enqueue(new FLOWCMD(CMDTYPE.CMD_PUSH, id));

    public void ChangeFlow(CFlow.FLOW_IDS id) => _queue.Enqueue(new FLOWCMD(CMDTYPE.CMD_CHANGE, id));

    public void PopFlow(CFlow.FLOW_IDS id) => _queue.Enqueue(new FLOWCMD(CMDTYPE.CMD_POP, id));

    public void PopAllFlow() => _queue.Enqueue(new FLOWCMD(CMDTYPE.CMD_POP_ALL, CFlow.FLOW_IDS.FLW_NONE));

    public void ChangePopAllFlow(CFlow.FLOW_IDS id) => _queue.Enqueue(new FLOWCMD(CMDTYPE.CMD_CHANGE_POP_ALL, id));

    public bool TryGetAt(out FLOWCMD command) => _queue.TryDequeue(out command);

    public bool TryFront(out FLOWCMD command) => _queue.TryPeek(out command);

    public int GetSize() => _queue.Count;

    public bool IsEmpty() => _queue.IsEmpty;

    public void Clear()
    {
        while (_queue.TryDequeue(out _))
        {
        }
    }

    public bool IsFrontPushFlow(CFlow.FLOW_IDS id) => IsFront(CMDTYPE.CMD_PUSH, id);

    public bool IsFrontChange(CFlow.FLOW_IDS id) => IsFront(CMDTYPE.CMD_CHANGE, id);

    public bool IsFrontChangePopAll(CFlow.FLOW_IDS id) => IsFront(CMDTYPE.CMD_CHANGE_POP_ALL, id);

    public bool IsFrontPop(CFlow.FLOW_IDS id) => IsFront(CMDTYPE.CMD_POP, id);

    public bool IsFrontPopAll()
    {
        return TryFront(out FLOWCMD command) && command.CmdType == CMDTYPE.CMD_POP_ALL;
    }

    private bool IsFront(CMDTYPE type, CFlow.FLOW_IDS id)
    {
        return TryFront(out FLOWCMD command) && command.CmdType == type && command.Id == id;
    }
}

