namespace DProject.Flow;

public sealed class CDigimonCreateFlow : CFlow
{
    public CDigimonCreateFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_DIGIMON_CREATE);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (!Context.Config.Porting.AutoAdvanceFlow)
        {
            return;
        }

        Context.GlobalData.PendingPartnerName = string.IsNullOrWhiteSpace(Context.GlobalData.PendingPartnerName)
            ? "Agumon"
            : Context.GlobalData.PendingPartnerName;
        Context.GlobalData.SelectedCharacterName = Context.GlobalData.PendingCharacterName;
        Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_LOADING);
    }
}
