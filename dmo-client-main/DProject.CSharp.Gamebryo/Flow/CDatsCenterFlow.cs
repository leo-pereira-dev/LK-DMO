namespace DProject.Flow;

public sealed class CDatsCenterFlow : CFlow
{
    public CDatsCenterFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_DATS_CENTER);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
    }
}
