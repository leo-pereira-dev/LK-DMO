namespace DProject.Flow;

public sealed class CFlowStack
{
    private readonly List<CFlow.FLOW_IDS> _stack = new();

    public bool IsExistID(CFlow.FLOW_IDS id) => _stack.Contains(id);

    public bool Push(CFlow.FLOW_IDS id)
    {
        if (IsExistID(id))
        {
            return false;
        }

        _stack.Add(id);
        return true;
    }

    public CFlow.FLOW_IDS Top()
    {
        CFlow.FLOW_IDS id = GetTopAt();
        if (id == CFlow.FLOW_IDS.FLW_NONE)
        {
            throw new InvalidOperationException("Flow stack is empty.");
        }

        _stack.RemoveAt(_stack.Count - 1);
        return id;
    }

    public void Pop()
    {
        if (_stack.Count > 0)
        {
            _stack.RemoveAt(_stack.Count - 1);
        }
    }

    public CFlow.FLOW_IDS GetTopAt()
    {
        return _stack.Count == 0 ? CFlow.FLOW_IDS.FLW_NONE : _stack[^1];
    }

    public CFlow.FLOW_IDS GetPosAt(int position)
    {
        if ((uint)position >= (uint)_stack.Count)
        {
            throw new ArgumentOutOfRangeException(nameof(position));
        }

        return _stack[position];
    }

    public int GetSize() => _stack.Count;

    public bool IsEmpty() => _stack.Count == 0;

    public void Clear() => _stack.Clear();
}

