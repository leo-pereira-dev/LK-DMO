using System.Xml.Linq;
using DProject.Data.CsFilePack;
using DProject.Resist;

namespace DProject.Option;

public sealed class COptionMng
{
    public const int eOPT_UI = 0;
    public const int eOPT_RESULT = 1;
    public const int eOTP_MAX = 2;
    public const string OPTIONFILE_NAME = "OPTION.info";

    private const string OPTION_UI = "OPTION_UI";
    private readonly cResist _resist;
    private readonly CPlayerUIOptionDocument _playerUI = new();

    public COptionMng(cResist resist)
    {
        _resist = resist;
    }

    public bool GetCharacterPath(out string path)
    {
        path = string.Empty;
        if (string.IsNullOrWhiteSpace(_resist.m_szID) ||
            string.IsNullOrWhiteSpace(_resist.m_szServer) ||
            string.IsNullOrWhiteSpace(_resist.m_szChar))
        {
            return false;
        }

        uint hashCode = CsFPSystem.GetHashCode(_resist.m_szChar);
        path = Path.Combine("UserData", _resist.m_szID, _resist.m_szServer, hashCode.ToString());
        Directory.CreateDirectory(path);
        return true;
    }

    public void LoadMachineOption()
    {
        _resist.LoadGlobal();
    }

    public void LoadCharOption(string wStr)
    {
        if (!GetCharacterPath(out string characterPath))
        {
            _playerUI.LoadToPlayerUIRecord(null, OPTION_UI);
            return;
        }

        string filePath = Path.Combine(characterPath, wStr);
        if (!File.Exists(filePath))
        {
            _playerUI.LoadToPlayerUIRecord(null, OPTION_UI);
            return;
        }

        XDocument document = XDocument.Load(filePath);
        _playerUI.LoadToPlayerUIRecord(document, OPTION_UI);
    }

    public void SaveMachineOption()
    {
        _resist.SaveGlobal();
    }

    public void SaveCharOption(string wStr)
    {
        if (!GetCharacterPath(out string characterPath))
        {
            return;
        }

        string filePath = Path.Combine(characterPath, wStr);
        XDocument document = new(new XDeclaration("1.0", "UTF-8", string.Empty));
        _playerUI.SaveToPlayerUIRecord(document, OPTION_UI);
        document.Save(filePath);
    }

    public COptionUIChat LoadPlayerUIChatClass(int type, bool bLoad = true)
    {
        return _playerUI.GetUIChatClass(type, bLoad);
    }

    public ST_CHAT_OPTION LoadPlayerUIOption(int type, bool bLoad = true)
    {
        return _playerUI.GetUIOption(type, bLoad);
    }

    public void SavePlayerUIOption(int type, ST_CHAT_OPTION pThis)
    {
        ArgumentNullException.ThrowIfNull(pThis);
        _playerUI.SetUIOption(type, pThis);
    }

    public int GetPlayerUIOptionValue(int type, eOPTION_UI_VALUE_TYPE optID, bool bLoad = true)
    {
        return _playerUI.GetUIOptionValue(type, optID, bLoad);
    }

    public ST_CHAT_OPTION GetPlayerUIOption(int type, bool bLoad = true)
    {
        return _playerUI.GetUIOption(type, bLoad);
    }
}

public enum eOPTION_UI_VALUE_TYPE
{
    Unknown,
    ChatAlpha,
    ChatLock,
    ChatVisible
}

public sealed class COptionUIChat
{
    public int Type { get; init; }
    public bool IsLoaded { get; set; }
}

public sealed class ST_CHAT_OPTION
{
    public Dictionary<eOPTION_UI_VALUE_TYPE, int> Values { get; } = new();

    public int GetValue(eOPTION_UI_VALUE_TYPE optionId)
    {
        return Values.TryGetValue(optionId, out int value) ? value : 0;
    }
}

internal sealed class CPlayerUIOptionDocument
{
    private readonly Dictionary<int, COptionUIChat> _chatClasses = new();
    private readonly Dictionary<int, ST_CHAT_OPTION> _options = new();

    public void LoadToPlayerUIRecord(XDocument? document, string rootName)
    {
        _chatClasses.Clear();
        _options.Clear();

        XElement? root = document?.Element(rootName);
        if (root is null)
        {
            EnsureDefaults();
            return;
        }

        foreach (XElement element in root.Elements("Option"))
        {
            int type = (int?)element.Attribute("Type") ?? COptionMng.eOPT_UI;
            var option = new ST_CHAT_OPTION();
            foreach (XElement valueElement in element.Elements("Value"))
            {
                string? idText = (string?)valueElement.Attribute("Id");
                if (Enum.TryParse(idText, out eOPTION_UI_VALUE_TYPE optionId))
                {
                    option.Values[optionId] = (int?)valueElement.Attribute("Value") ?? 0;
                }
            }

            _options[type] = option;
            _chatClasses[type] = new COptionUIChat { Type = type, IsLoaded = true };
        }

        EnsureDefaults();
    }

    public void SaveToPlayerUIRecord(XDocument document, string rootName)
    {
        var root = new XElement(rootName);
        foreach ((int type, ST_CHAT_OPTION option) in _options)
        {
            var element = new XElement("Option", new XAttribute("Type", type));
            foreach ((eOPTION_UI_VALUE_TYPE id, int value) in option.Values)
            {
                element.Add(new XElement(
                    "Value",
                    new XAttribute("Id", id),
                    new XAttribute("Value", value)));
            }

            root.Add(element);
        }

        document.Add(root);
    }

    public COptionUIChat GetUIChatClass(int type, bool bLoad)
    {
        if (!_chatClasses.TryGetValue(type, out COptionUIChat? chat) && bLoad)
        {
            chat = new COptionUIChat { Type = type, IsLoaded = true };
            _chatClasses[type] = chat;
        }

        return chat ?? new COptionUIChat { Type = type, IsLoaded = false };
    }

    public ST_CHAT_OPTION GetUIOption(int type, bool bLoad)
    {
        if (!_options.TryGetValue(type, out ST_CHAT_OPTION? option) && bLoad)
        {
            option = CreateDefaultOption();
            _options[type] = option;
        }

        return option ?? new ST_CHAT_OPTION();
    }

    public void SetUIOption(int type, ST_CHAT_OPTION option)
    {
        _options[type] = option;
    }

    public int GetUIOptionValue(int type, eOPTION_UI_VALUE_TYPE optionId, bool bLoad)
    {
        return GetUIOption(type, bLoad).GetValue(optionId);
    }

    private void EnsureDefaults()
    {
        _chatClasses.TryAdd(COptionMng.eOPT_UI, new COptionUIChat { Type = COptionMng.eOPT_UI, IsLoaded = true });
        _options.TryAdd(COptionMng.eOPT_UI, CreateDefaultOption());
    }

    private static ST_CHAT_OPTION CreateDefaultOption()
    {
        var option = new ST_CHAT_OPTION();
        option.Values[eOPTION_UI_VALUE_TYPE.ChatAlpha] = 100;
        option.Values[eOPTION_UI_VALUE_TYPE.ChatLock] = 0;
        option.Values[eOPTION_UI_VALUE_TYPE.ChatVisible] = 1;
        return option;
    }
}
