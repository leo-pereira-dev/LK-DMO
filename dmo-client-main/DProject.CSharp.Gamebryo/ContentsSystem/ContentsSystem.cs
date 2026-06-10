namespace DProject.ContentsSystem;

public sealed class ContentsSystem
{
    private readonly Dictionary<eContentsType, IUIContentBase> _contents = new();
    private IUIContentBase? _activeContent;

    public void BuildContents()
    {
        Register(new LogoContents());
        Register(new LoginContents());
        Register(new ServerSelectContents());
        Register(new SecondPasswordContents());
        Register(new CharacterSelectContents());
        Register(new CharacterCreateContents());
        Register(new DigimonCreateContents());
        Register(new LoadingContents());
        Register(new MainFrameContents());
        Register(new BattleSystemContents());
        Register(new InventoryContents());
        Register(new MapContents());
        Register(new ChatContents());
        Register(new WareHouseContents());
        Register(new MailContents());
        Register(new PartyContents());
        Register(new TutorialContents());
        Register(new MacroProtectContents());
        Register(new DatsCenterContents());
        Register(new CharacterServerRelocateContents());
    }

    public void IntraConnection()
    {
    }

    public void InitializeContents(eContentsType type)
    {
        if (_contents.TryGetValue(type, out IUIContentBase? content))
        {
            _activeContent = content;
            content.Initialize();
        }
    }

    public void Update(float elapsedTime)
    {
        _activeContent?.Update(elapsedTime);
    }

    private void Register(IUIContentBase content)
    {
        _contents[content.ContentsType] = content;
    }
}
