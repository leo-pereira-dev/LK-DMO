namespace DProject.Flow;

public sealed class CServerSelectFlow : CFlow
{
    public CServerSelectFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.Engine.SetGaussianBlurVal(0.9f, 0.25f, 0.3f);
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_SERVER_SELECT);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (!Context.Config.Porting.AutoAdvanceFlow)
        {
            return;
        }

        string serverName = Context.Network.GetServerList().FirstOrDefault() ?? "default";
        Context.GlobalData.SelectedServerName = serverName;
        Context.Network.SelectServerByName(serverName);
        Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_SECURITY);
    }
}
