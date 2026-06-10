namespace DProject.Flow;

public sealed class CCharacterSelectFlow : CFlow
{
    public CCharacterSelectFlow(FLOW_IDS id, FlowContext context) : base(id, context)
    {
    }

    protected override bool Initialize()
    {
        Context.ContentsSystem.InitializeContents(ContentsSystem.eContentsType.E_CT_CHARACTER_SELECT);
        return true;
    }

    protected override void UpdateFrame()
    {
        Context.ContentsSystem.Update(1.0f / 60.0f);
        if (!Context.Config.Porting.AutoAdvanceFlow)
        {
            return;
        }

        string? characterName = Context.Network.GetCharacterList().FirstOrDefault();
        if (string.IsNullOrWhiteSpace(characterName))
        {
            Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_CHARCREATE);
            return;
        }

        Context.GlobalData.SelectedCharacterName = characterName;
        Context.Network.SelectCharacter(characterName);
        Context.FlowMgr.ChangeFlow(FLOW_IDS.FLW_LOADING);
    }
}
