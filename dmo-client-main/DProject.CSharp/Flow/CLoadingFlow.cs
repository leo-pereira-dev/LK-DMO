namespace DProject.Flow;

public sealed class CLoadingFlow : CFlow
{
    public CLoadingFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_LOADING);
        Context.GameStateSync.RequestMapTransition(0, "Loading", "CLoadingFlow.Initialize");
        Context.FileTableLoading.Start();
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (Context.Config.Porting.AutoAdvanceFlow && Context.FileTableLoading.State == Data.Loading.FileTableLoadState.Succeeded)
        {
            Context.Network.SendGameReady(Context.GlobalData.StartPositionX, Context.GlobalData.StartPositionY);
            Context.GameStateSync.CompleteMapTransition(0, Context.GlobalData.SelectedServerName);
            Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_MAINGAME);
        }

        if (Context.FileTableLoading.State == Data.Loading.FileTableLoadState.Failed)
        {
            throw new InvalidOperationException(Context.FileTableLoading.Result?.ErrorMessage ?? "File table loading failed.");
        }
    }
}
