namespace DProject.Flow;

public abstract class CFlow
{
    public enum FLOW_IDS
    {
        FLW_NONE = -1,
        FLW_INTRO,
        FLW_LOGO,
        FLW_LOGIN,
        FLW_SERVERSEL,
        FLW_SECURITY,
        FLW_CHARSEL,
        FLW_CHARCREATE,
        FLW_DIGIMON_CREATE,
        FLW_LOADING,
        FLW_MAINGAME,
        FLW_DATSCENTER,
        FLW_DATATRADECENTER,
        FLW_BATTLE_REGISTER,
        FLW_BATTLE,
        FLW_BATTLE_RESULT,
        FLW_SERVER_RELOCATE,
        FLW_ID_COUNT
    }

    protected CFlow(FLOW_IDS id, FlowContext context)
    {
        Id = id;
        Context = context;
    }

    public FLOW_IDS Id { get; }
    public bool IsInitialized { get; private set; }
    protected FlowContext Context { get; }

    public virtual void OnEnter()
    {
        if (!IsInitialized)
        {
            IsInitialized = Initialize();
        }
    }

    public virtual void OnExit(FLOW_IDS nextFlowId)
    {
        Terminate();
        IsInitialized = false;
    }

    public virtual void OnIdle()
    {
        GlobalUpdate();
        InputFrame();
        UpdateFrame();
        CullFrame();
        DrawFrame();
    }

    public virtual void ReservedChangeFlow(FLOW_IDS nextFlowId) { }
    public virtual void ReservedPushFlow(FLOW_IDS nextFlowId) { }
    public virtual void ReservedPopFlow() { }

    protected virtual bool Initialize() => true;
    protected virtual void Terminate() { }
    protected virtual void GlobalUpdate() { }
    protected virtual void InputFrame() { }
    protected virtual void UpdateFrame() { }
    protected virtual void CullFrame() { }

    protected virtual void DrawFrame()
    {
        Context.RenderCommands.ExecuteAll();

        if (!BeginFrame())
        {
            return;
        }

        BeginRenderTarget();
        RenderSceneFrame();
        RenderScreenFrame();
        RenderBackScreenFrame();
        RenderUIFrame();
        EndRenderTarget();
        EndFrame();
        DisplayFrame();
    }

    protected virtual bool BeginFrame() => Context.Engine.BeginFrame();
    protected virtual bool BeginRenderTarget() => true;
    protected virtual void RenderSceneFrame() { }
    protected virtual void RenderScreenFrame() { }
    protected virtual void RenderBackScreenFrame() { }
    protected virtual void RenderUIFrame() { }
    protected virtual void EndRenderTarget() { }
    protected virtual void EndFrame() => Context.Engine.EndFrame();
    protected virtual void DisplayFrame() => Context.Engine.DisplayFrame();
}
