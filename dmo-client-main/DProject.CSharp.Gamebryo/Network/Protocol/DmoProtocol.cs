namespace DProject.Network.Protocol;

public static class nScope
{
    public const ushort User = 100;
    public const ushort Gate = 900;
    public const ushort Game = 1000;
    public const ushort Tamer = 1300;
    public const ushort Svr = 1700;
    public const ushort Net = 1900;
    public const ushort Digimon = 3200;
    public const ushort Login = 3300;
    public const ushort Pass2 = 9800;
    public const ushort Shutdown = 9850;
    public const ushort Master = 9900;
    public const ushort Certify = 10000;
    public const ushort GameTwo = 16000;
}

public static class DmoVersion
{
    public const uint UsaNetVersion = 19052801;
    public const uint LegacyFallbackNetVersion = 12102301;
}

public static class nClass
{
    public const uint None = 0;
    public const uint Digimon = 1;
    public const uint Tamer = 2;
    public const uint Item = 3;
    public const uint Monster = 4;
    public const uint Npc = 5;
    public const uint Party = 6;
    public const uint CommissionShop = 7;
}

public static class pLimit
{
    public const int OpenedTamerSlot = 10;
    public const int MaxTamerSlot = 12;
    public const int TamerEquipCount = 14;
    public const int ItemDataBytesCompat487 = 68;
    public const int TamerEquipBytesCompat487 = TamerEquipCount * ItemDataBytesCompat487;
}

public static class pLogin
{
    public const ushort Begin = nScope.Login;
    public const ushort Request = Begin + 1;
    public const ushort Success = Begin + 2;
    public const ushort Failure = Begin + 3;
    public const ushort OnTheAnvil = Begin + 4;
    public const ushort WaitTime = Begin + 5;
    public const ushort FailureGSP = Begin + 6;
    public const ushort RequestAeriaChannel = Begin + 7;
    public const ushort AccountBan = Begin + 8;
    public const ushort Birthday = Begin + 9;
}

public static class pSvr
{
    public const ushort Begin = nScope.Svr;
    public const ushort ClusterList = Begin + 1;
    public const ushort SelectCluster = Begin + 2;
    public const ushort Change = Begin + 3;
    public const ushort GoBackGate = Begin + 4;
    public const ushort GoBackAccount = Begin + 5;
    public const ushort AccessCode = Begin + 6;
    public const ushort SelectPortal = Begin + 9;
    public const ushort SelectPortalFailure = Begin + 10;
    public const ushort LocalPortal = Begin + 11;
    public const ushort ChannelInfo = Begin + 13;
    public const ushort SelectCharacter = Begin + 20;
}

public static class nLIB
{
    public const int SVR_NONE = 0;
    public const int SVR_GAME = 1;
    public const int SVR_BATTLE = 2;
    public const int SVR_DUNGEON = 3;
}

public static class pGame
{
    public const ushort Begin = nScope.Game;
    public const ushort Ready = Begin + 1;
    public const ushort SvrInfo = Begin + 2;
    public const ushort InitData = Begin + 3;
    public const ushort MoveTo = Begin + 4;
    public const ushort NewPath = Begin + 5;
    public const ushort Sync = Begin + 6;
    public const ushort HpRate = Begin + 7;
    public const ushort Chat = Begin + 8;
    public const ushort Whisper = Begin + 9;
    public const ushort WhisperFailure = Begin + 10;
    public const ushort MsgAll = Begin + 11;
    public const ushort MsgSystem = Begin + 12;
    public const ushort Attack = Begin + 13;
    public const ushort AttackMiss = Begin + 14;
    public const ushort Skill = Begin + 15;
    public const ushort SetTarget = Begin + 16;
    public const ushort TargetInfo = Begin + 17;
    public const ushort GetExp = Begin + 18;
    public const ushort LevelUp = Begin + 19;
    public const ushort Die = Begin + 20;
    public const ushort SkillDie = Begin + 21;
    public const ushort DieConfirm = Begin + 22;
    public const ushort HealUp = Begin + 23;
    public const ushort BattleOn = Begin + 34;
    public const ushort BattleOff = Begin + 35;
    public const ushort DeletePartner = Begin + 42;
    public const ushort AllStat = Begin + 43;
    public const ushort RefreshScreen = Begin + 46;
    public const ushort StartMessage = Begin + 47;
    public const ushort ChangeChannel = Begin + 50;
    public const ushort OpenRegion = Begin + 51;
    public const ushort DiedAlready = Begin + 52;
    public const ushort Ting = Begin + 53;
    public const ushort ChangeState = Begin + 54;
    public const ushort Emoticon = Begin + 58;
    public const ushort AdvertBossCount = Begin + 59;
    public const ushort AdvertBossCreate = Begin + 60;
    public const ushort AdvertBossDie = Begin + 61;
    public const ushort AwayTime = Begin + 69;
    public const ushort SyncCondition = Begin + 70;
}

public static class pGameTwo
{
    public const ushort Begin = nScope.GameTwo;
    public const ushort GameGuardCheck = Begin + 9;
    public const ushort XignCodeCheck = Begin + 35;
}

public static class pSync
{
    public const byte None = 0;
    public const byte New = 1;
    public const byte Delete = 2;
    public const byte In = 3;
    public const byte Out = 4;
    public const byte Walk = 5;
    public const byte Move = 6;
    public const byte MsgChat = 7;
    public const byte MsgWhisper = 8;
    public const byte MsgAll = 9;
    public const byte MsgSystem = 10;
    public const byte MsgShout = 11;
    public const byte MsgAllByItem = 12;
    public const byte Condition = 13;
    public const byte PartyBooster = 14;
    public const byte Achievement = 15;
    public const byte BuffSync = 16;
    public const byte Rotation = 0x7f;
    public const byte State = 0x80;
    public const byte MsgGuildShout = 0x81;
}

public static class pGate
{
    public const ushort Begin = nScope.Gate;
    public const ushort SvrInfo = Begin + 1;
    public const ushort CheckName = Begin + 2;
    public const ushort ExistName = Begin + 3;
    public const ushort WaitTime = Begin + 4;
}

public static class pTamer
{
    public const ushort Start = nScope.Tamer;
    public const ushort List = Start + 1;
    public const ushort CheckDoubleName = Start + 2;
    public const ushort Create = Start + 3;
    public const ushort Remove = Start + 4;
    public const ushort Select = Start + 5;
    public const ushort CreateSuccess = Start + 6;
    public const ushort CreateFailure = Start + 7;
    public const ushort SelectSuccess = Start + 8;
    public const ushort SelectFailure = Start + 9;
    public const ushort CreateWaitTime = Start + 15;
}

public static class pPass2
{
    public const ushort Begin = nScope.Pass2;
    public const ushort Register = Begin + 1;
    public const ushort On = Begin + 2;
    public const ushort Off = Begin + 3;
    public const ushort Check = Begin + 4;
    public const ushort Change = Begin + 5;
    public const ushort ChangePass = Begin + 6;
}

public static class nPass2
{
    public const ushort NewSet = 0;
    public const ushort Registered = 1;
    public const ushort Certified = 2;
    public const ushort Skiped = 3;
}

public static class pNet
{
    public const ushort Begin = nScope.Net;
    public const ushort Check = Begin + 1;
    public const ushort LockCluster = Begin + 2;
    public const ushort OpenCluster = Begin + 3;
    public const ushort VersionError = Begin + 4;
}

public static class pShutdown
{
    public const ushort Begin = nScope.Shutdown;
    public const ushort WeekCheck = Begin + 1;
    public const ushort WeekCheckError = Begin + 2;
}

public static class pMaster
{
    public const ushort Start = nScope.Master;
    public const ushort DiscUser = Start + 23;
    public const ushort BanUser = Start + 39;
}

public static class pCertify
{
    public const ushort Begin = nScope.Certify;
    public const ushort IntegrityHash = Begin + 1;
}
