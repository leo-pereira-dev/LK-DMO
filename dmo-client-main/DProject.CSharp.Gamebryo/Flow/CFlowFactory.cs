namespace DProject.Flow;

public sealed class CFlowFactory
{
    public CFlow CreateFlow(CFlow.FLOW_IDS id, FlowContext context)
    {
        return id switch
        {
            CFlow.FLOW_IDS.FLW_LOGO => new CLogoFlow(id, context),
            CFlow.FLOW_IDS.FLW_LOGIN => new CLoginFlow(id, context),
            CFlow.FLOW_IDS.FLW_SERVERSEL => new CServerSelectFlow(id, context),
            CFlow.FLOW_IDS.FLW_SECURITY => new CSecondPasswordFlow(id, context),
            CFlow.FLOW_IDS.FLW_CHARSEL => new CCharacterSelectFlow(id, context),
            CFlow.FLOW_IDS.FLW_CHARCREATE => new CCharacterCreateFlow(id, context),
            CFlow.FLOW_IDS.FLW_DIGIMON_CREATE => new CDigimonCreateFlow(id, context),
            CFlow.FLOW_IDS.FLW_LOADING => new CLoadingFlow(id, context),
            CFlow.FLOW_IDS.FLW_MAINGAME => new CInGameFlow(id, context),
            CFlow.FLOW_IDS.FLW_DATSCENTER => new CDatsCenterFlow(id, context),
            CFlow.FLOW_IDS.FLW_BATTLE_REGISTER => new CBattleRegisterFlow(id, context),
            CFlow.FLOW_IDS.FLW_BATTLE => new CBattleFlow(id, context),
            CFlow.FLOW_IDS.FLW_BATTLE_RESULT => new CBattleResultFlow(id, context),
            CFlow.FLOW_IDS.FLW_SERVER_RELOCATE => new CCharacterServerRelocateFlow(id, context),
            _ => new CPlaceholderFlow(id, context)
        };
    }
}
