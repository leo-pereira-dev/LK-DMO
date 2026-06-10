namespace DProject.Flow;

public sealed class CLoginFlow : CFlow
{
    public CLoginFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.GlobalData.Clear2ndPass();
        Context.Engine.SetGaussianBlurVal(0.9f, 0.25f, 0.3f);
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_LOGIN);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (Context.Config.Porting.AutoAdvanceFlow && Context.Network.Login(Context.GlobalData.AccountId, Context.GlobalData.AccountPassword))
        {
            Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_SERVERSEL);
        }
    }
}

