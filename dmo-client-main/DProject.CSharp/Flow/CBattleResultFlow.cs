namespace DProject.Flow;

public sealed class CBattleResultFlow : CFlow
{
    public CBattleResultFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.GlobalData.LastBattleResult = "Pending";
        Context.GameStateSync.LeaveCombat("CBattleResultFlow.Initialize");
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_BATTLE_SYSTEM);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (Context.Config.Porting.AutoAdvanceFlow)
        {
            Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_MAINGAME);
        }
    }
}
