namespace DProject.Flow;

public sealed class CCharacterCreateFlow : CFlow
{
    public CCharacterCreateFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_CHARACTER_CREATE);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (!Context.Config.Porting.AutoAdvanceFlow)
        {
            return;
        }

        Context.GlobalData.PendingCharacterName = string.IsNullOrWhiteSpace(Context.GlobalData.PendingCharacterName)
            ? "PortedTamer"
            : Context.GlobalData.PendingCharacterName;
        Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_DIGIMON_CREATE);
    }
}
