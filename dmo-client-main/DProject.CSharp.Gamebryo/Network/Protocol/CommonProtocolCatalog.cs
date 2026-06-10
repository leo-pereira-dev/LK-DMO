namespace DProject.Network.Protocol;

public enum DmoEndian
{
    Little
}

public enum DmoPrimitiveType
{
    u1,
    u2,
    u4,
    u8,
    s1,
    s2,
    s4,
    s8,
    f4,
    str
}

public enum DmoPacketScope
{
    User,
    Gate,
    Game,
    Tamer,
    Svr,
    Net,
    Digimon,
    Login,
    Pass2,
    Shutdown,
    Master,
    Certify,
    GameTwo
}

public sealed record DmoPacketDefinition(
    string Name,
    DmoPacketScope Scope,
    ushort PacketId,
    string SourceHeader);

public sealed record ProtocolValidationIssue(string Code, string Message, DmoPacketDefinition? Packet = null);

public sealed record ProtocolValidationResult(
    DmoEndian Endian,
    IReadOnlyDictionary<DmoPrimitiveType, int> PrimitiveSizes,
    IReadOnlyList<DmoPacketDefinition> Packets,
    IReadOnlyList<ProtocolValidationIssue> Issues)
{
    public bool IsValid => Issues.Count == 0;
}

public static class CommonProtocolCatalog
{
    public static readonly IReadOnlyDictionary<DmoPrimitiveType, int> PrimitiveSizes =
        new Dictionary<DmoPrimitiveType, int>
        {
            [DmoPrimitiveType.u1] = sizeof(byte),
            [DmoPrimitiveType.u2] = sizeof(ushort),
            [DmoPrimitiveType.u4] = sizeof(uint),
            [DmoPrimitiveType.u8] = sizeof(ulong),
            [DmoPrimitiveType.s1] = sizeof(sbyte),
            [DmoPrimitiveType.s2] = sizeof(short),
            [DmoPrimitiveType.s4] = sizeof(int),
            [DmoPrimitiveType.s8] = sizeof(long),
            [DmoPrimitiveType.f4] = sizeof(float),
            [DmoPrimitiveType.str] = sizeof(byte)
        };

    private static readonly DmoPacketDefinition[] PacketDefinitions =
    [
        new("pLogin.Request", DmoPacketScope.Login, pLogin.Request, "common_vs2019/Protocol/Login_Protocol.h"),
        new("pLogin.Success", DmoPacketScope.Login, pLogin.Success, "common_vs2019/Protocol/Login_Protocol.h"),
        new("pLogin.Failure", DmoPacketScope.Login, pLogin.Failure, "common_vs2019/Protocol/Login_Protocol.h"),
        new("pLogin.WaitTime", DmoPacketScope.Login, pLogin.WaitTime, "common_vs2019/Protocol/Login_Protocol.h"),
        new("pLogin.RequestAeriaChannel", DmoPacketScope.Login, pLogin.RequestAeriaChannel, "common_vs2019/Protocol/Login_Protocol.h"),
        new("pLogin.AccountBan", DmoPacketScope.Login, pLogin.AccountBan, "common_vs2019/Protocol/Login_Protocol.h"),

        new("pSvr.ClusterList", DmoPacketScope.Svr, pSvr.ClusterList, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.SelectCluster", DmoPacketScope.Svr, pSvr.SelectCluster, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.Change", DmoPacketScope.Svr, pSvr.Change, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.GoBackGate", DmoPacketScope.Svr, pSvr.GoBackGate, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.GoBackAccount", DmoPacketScope.Svr, pSvr.GoBackAccount, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.AccessCode", DmoPacketScope.Svr, pSvr.AccessCode, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.SelectPortal", DmoPacketScope.Svr, pSvr.SelectPortal, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.SelectPortalFailure", DmoPacketScope.Svr, pSvr.SelectPortalFailure, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.LocalPortal", DmoPacketScope.Svr, pSvr.LocalPortal, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.ChannelInfo", DmoPacketScope.Svr, pSvr.ChannelInfo, "common_vs2019/Protocol/Svr_Protocol.h"),
        new("pSvr.SelectCharacter", DmoPacketScope.Svr, pSvr.SelectCharacter, "common_vs2019/Protocol/Svr_Protocol.h"),

        new("pGame.Ready", DmoPacketScope.Game, pGame.Ready, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.SvrInfo", DmoPacketScope.Game, pGame.SvrInfo, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.InitData", DmoPacketScope.Game, pGame.InitData, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.MoveTo", DmoPacketScope.Game, pGame.MoveTo, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.NewPath", DmoPacketScope.Game, pGame.NewPath, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.Sync", DmoPacketScope.Game, pGame.Sync, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.HpRate", DmoPacketScope.Game, pGame.HpRate, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.Chat", DmoPacketScope.Game, pGame.Chat, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.Attack", DmoPacketScope.Game, pGame.Attack, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.Skill", DmoPacketScope.Game, pGame.Skill, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.SetTarget", DmoPacketScope.Game, pGame.SetTarget, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.TargetInfo", DmoPacketScope.Game, pGame.TargetInfo, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.AllStat", DmoPacketScope.Game, pGame.AllStat, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.ChangeChannel", DmoPacketScope.Game, pGame.ChangeChannel, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGame.SyncCondition", DmoPacketScope.Game, pGame.SyncCondition, "common_vs2019/Protocol/Game_Protocol.h"),

        new("pGameTwo.GameGuardCheck", DmoPacketScope.GameTwo, pGameTwo.GameGuardCheck, "common_vs2019/Protocol/Game_Protocol.h"),
        new("pGameTwo.XignCodeCheck", DmoPacketScope.GameTwo, pGameTwo.XignCodeCheck, "common_vs2019/Protocol/Game_Protocol.h"),

        new("pGate.SvrInfo", DmoPacketScope.Gate, pGate.SvrInfo, "common_vs2019/Protocol/Gate_Protocol.h"),
        new("pGate.CheckName", DmoPacketScope.Gate, pGate.CheckName, "common_vs2019/Protocol/Gate_Protocol.h"),
        new("pGate.ExistName", DmoPacketScope.Gate, pGate.ExistName, "common_vs2019/Protocol/Gate_Protocol.h"),
        new("pGate.WaitTime", DmoPacketScope.Gate, pGate.WaitTime, "common_vs2019/Protocol/Gate_Protocol.h"),

        new("pTamer.List", DmoPacketScope.Tamer, pTamer.List, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.CheckDoubleName", DmoPacketScope.Tamer, pTamer.CheckDoubleName, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.Create", DmoPacketScope.Tamer, pTamer.Create, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.Remove", DmoPacketScope.Tamer, pTamer.Remove, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.Select", DmoPacketScope.Tamer, pTamer.Select, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.SelectSuccess", DmoPacketScope.Tamer, pTamer.SelectSuccess, "common_vs2019/Protocol/Tamer_Protocol.h"),
        new("pTamer.SelectFailure", DmoPacketScope.Tamer, pTamer.SelectFailure, "common_vs2019/Protocol/Tamer_Protocol.h"),

        new("pPass2.Register", DmoPacketScope.Pass2, pPass2.Register, "common_vs2019/Protocol/SecondPass_Protocol.h"),
        new("pPass2.On", DmoPacketScope.Pass2, pPass2.On, "common_vs2019/Protocol/SecondPass_Protocol.h"),
        new("pPass2.Off", DmoPacketScope.Pass2, pPass2.Off, "common_vs2019/Protocol/SecondPass_Protocol.h"),
        new("pPass2.Check", DmoPacketScope.Pass2, pPass2.Check, "common_vs2019/Protocol/SecondPass_Protocol.h"),
        new("pPass2.Change", DmoPacketScope.Pass2, pPass2.Change, "common_vs2019/Protocol/SecondPass_Protocol.h"),
        new("pPass2.ChangePass", DmoPacketScope.Pass2, pPass2.ChangePass, "common_vs2019/Protocol/SecondPass_Protocol.h"),

        new("pNet.Check", DmoPacketScope.Net, pNet.Check, "common_vs2019/Protocol/Net_Protocol.h"),
        new("pNet.VersionError", DmoPacketScope.Net, pNet.VersionError, "common_vs2019/Protocol/Net_Protocol.h"),
        new("pShutdown.WeekCheck", DmoPacketScope.Shutdown, pShutdown.WeekCheck, "common_vs2019/Protocol/Shutdown_Protocol.h"),
        new("pMaster.DiscUser", DmoPacketScope.Master, pMaster.DiscUser, "common_vs2019/Protocol/Master_Protocol.h"),
        new("pMaster.BanUser", DmoPacketScope.Master, pMaster.BanUser, "common_vs2019/Protocol/Master_Protocol.h"),
        new("pCertify.IntegrityHash", DmoPacketScope.Certify, pCertify.IntegrityHash, "common_vs2019/Protocol/Certify_Protocol.h")
    ];

    public static IReadOnlyList<DmoPacketDefinition> Packets => PacketDefinitions;
    public static DmoEndian Endian => DmoEndian.Little;

    public static ProtocolValidationResult Validate()
    {
        var issues = new List<ProtocolValidationIssue>();

        foreach (KeyValuePair<DmoPrimitiveType, int> primitive in PrimitiveSizes)
        {
            if (primitive.Value <= 0)
            {
                issues.Add(new ProtocolValidationIssue("PRIMITIVE_SIZE", $"{primitive.Key} has invalid size {primitive.Value}."));
            }
        }

        foreach (IGrouping<ushort, DmoPacketDefinition> duplicate in PacketDefinitions.GroupBy(static packet => packet.PacketId))
        {
            if (duplicate.Count() <= 1)
            {
                continue;
            }

            string names = string.Join(", ", duplicate.Select(static packet => packet.Name));
            issues.Add(new ProtocolValidationIssue("DUPLICATE_PACKET_ID", $"Packet id {duplicate.Key} is shared by {names}."));
        }

        foreach (DmoPacketDefinition packet in PacketDefinitions)
        {
            ushort scopeBase = ScopeBase(packet.Scope);
            if (packet.PacketId < scopeBase)
            {
                issues.Add(new ProtocolValidationIssue(
                    "PACKET_SCOPE",
                    $"{packet.Name} id {packet.PacketId} is below scope base {scopeBase}.",
                    packet));
            }
        }

        return new ProtocolValidationResult(Endian, PrimitiveSizes, PacketDefinitions, issues);
    }

    private static ushort ScopeBase(DmoPacketScope scope)
    {
        return scope switch
        {
            DmoPacketScope.User => nScope.User,
            DmoPacketScope.Gate => nScope.Gate,
            DmoPacketScope.Game => nScope.Game,
            DmoPacketScope.Tamer => nScope.Tamer,
            DmoPacketScope.Svr => nScope.Svr,
            DmoPacketScope.Net => nScope.Net,
            DmoPacketScope.Digimon => nScope.Digimon,
            DmoPacketScope.Login => nScope.Login,
            DmoPacketScope.Pass2 => nScope.Pass2,
            DmoPacketScope.Shutdown => nScope.Shutdown,
            DmoPacketScope.Master => nScope.Master,
            DmoPacketScope.Certify => nScope.Certify,
            DmoPacketScope.GameTwo => nScope.GameTwo,
            _ => 0
        };
    }
}
