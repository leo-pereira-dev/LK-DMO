namespace DProject.ContentsSystem;

public abstract class BasicContents : IUIContentBase
{
    protected BasicContents(eContentsType contentsType)
    {
        ContentsType = contentsType;
    }

    public eContentsType ContentsType { get; }
    public bool IsInitialized { get; private set; }

    public virtual void Initialize()
    {
        IsInitialized = true;
    }

    public virtual void Update(float elapsedTime)
    {
    }
}

public sealed class LogoContents : BasicContents
{
    public LogoContents() : base(eContentsType.E_CT_LOGO) { }
}

public sealed class LoginContents : BasicContents
{
    public LoginContents() : base(eContentsType.E_CT_LOGIN) { }
}

public sealed class ServerSelectContents : BasicContents
{
    public ServerSelectContents() : base(eContentsType.E_CT_SERVER_SELECT) { }
}

public sealed class CharacterSelectContents : BasicContents
{
    public CharacterSelectContents() : base(eContentsType.E_CT_CHARACTER_SELECT) { }
}

public sealed class SecondPasswordContents : BasicContents
{
    public SecondPasswordContents() : base(eContentsType.E_CT_SECOND_PASSWORD) { }
}

public sealed class CharacterCreateContents : BasicContents
{
    public CharacterCreateContents() : base(eContentsType.E_CT_CHARACTER_CREATE) { }
}

public sealed class DigimonCreateContents : BasicContents
{
    public DigimonCreateContents() : base(eContentsType.E_CT_DIGIMON_CREATE) { }
}

public sealed class LoadingContents : BasicContents
{
    public LoadingContents() : base(eContentsType.E_CT_LOADING) { }
}

public sealed class MainFrameContents : BasicContents
{
    public MainFrameContents() : base(eContentsType.E_CT_MAINFRAME_CONTENTS) { }
}

public sealed class BattleSystemContents : BasicContents
{
    public BattleSystemContents() : base(eContentsType.E_CT_BATTLE_SYSTEM) { }
}

public sealed class InventoryContents : BasicContents
{
    public InventoryContents() : base(eContentsType.E_CT_INVENTORY_CONTENTS) { }
}

public sealed class MapContents : BasicContents
{
    public MapContents() : base(eContentsType.E_CT_MAP_INTERFACE) { }
}

public sealed class ChatContents : BasicContents
{
    public ChatContents() : base(eContentsType.E_CT_CHATTING_STANDARDIZATION) { }
}

public sealed class WareHouseContents : BasicContents
{
    public WareHouseContents() : base(eContentsType.E_CT_WAREHOUSE) { }
}

public sealed class MailContents : BasicContents
{
    public MailContents() : base(eContentsType.E_CT_MAIL_SYSTEM) { }
}

public sealed class PartyContents : BasicContents
{
    public PartyContents() : base(eContentsType.E_CT_PARTYSYSTEM) { }
}

public sealed class TutorialContents : BasicContents
{
    public TutorialContents() : base(eContentsType.E_CT_TUTORIAL_CONTENTS) { }
}

public sealed class MacroProtectContents : BasicContents
{
    public MacroProtectContents() : base(eContentsType.E_CT_MACROPROTECT) { }
}

public sealed class DatsCenterContents : BasicContents
{
    public DatsCenterContents() : base(eContentsType.E_CT_DATS_CENTER) { }
}

public sealed class CharacterServerRelocateContents : BasicContents
{
    public CharacterServerRelocateContents() : base(eContentsType.E_CT_SERVERCHANGE_CONTENTS) { }
}
