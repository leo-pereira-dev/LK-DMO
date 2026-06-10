namespace DProject.Flow;

public sealed class CBattleFlow : CFlow
{
    public CBattleFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.GameStateSync.EnterCombat("CBattleFlow.Initialize");
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_BATTLE_SYSTEM);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (Context.Config.Porting.AutoAdvanceFlow)
        {
            Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_BATTLE_RESULT);
        }
    }

    protected override void Terminate()
    {
        Context.GameStateSync.LeaveCombat("CBattleFlow.Terminate");
    }
}
