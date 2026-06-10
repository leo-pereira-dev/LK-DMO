using System.Text.Json;
using DProject.Config;

namespace DProject.Resist;

public sealed class cResist
{
    public const string RES_GLOBAL_PATH = "UserData\\Global.grf";
    public const int RES_GLOBAL_VERSION = 0x0000010d;
    public const int RES_ACCOUNT_GLOBAL_VERSION = 0x00000100;
    public const int RES_ACCOUNT_SERVER_VERSION = 0x00000100;

    private static readonly (int Width, int Height)[] SupportedResolutions =
    [
        (1024, 768),
        (1280, 720),
        (1280, 768),
        (1366, 768),
        (1600, 900),
        (1920, 1080)
    ];

    private readonly AppConfig _config;
    private bool _enablePortal;
    private bool _movePortal;

    public cResist(AppConfig config)
    {
        _config = config;
        m_Global = new sGLOBAL();
        m_AccountGlobal = new sACCOUNT_GLOBAL();
        m_AccountServer = new sACCOUNT_SERVER();
    }

    public string m_szID { get; private set; } = string.Empty;
    public string m_szServer { get; private set; } = string.Empty;
    public string m_szChar { get; private set; } = string.Empty;
    public bool m_bLoadChar { get; private set; }
    public int m_nDeleteCharIndex { get; private set; } = -1;
    public sGLOBAL m_Global { get; }
    public sACCOUNT_GLOBAL m_AccountGlobal { get; }
    public sACCOUNT_SERVER m_AccountServer { get; }

    public void Init()
    {
        m_Global.Reset();
        m_AccountGlobal.Reset();
        m_AccountServer.Reset();
        ApplyConfig(_config);
    }

    public void Delete()
    {
        m_bLoadChar = false;
        _enablePortal = false;
        _movePortal = false;
    }

    public void ApplyConfig(AppConfig config)
    {
        SetAccount(config.Login.Id, config.Debug.Server);
        m_Global.s_bSaveID = config.Login.RememberPassword;
        m_Global.s_szLastConnectID = config.Login.Id;
        m_Global.s_nResolutionWidth = config.Window.Width;
        m_Global.s_nResolutionHeight = config.Window.Height;
        m_Global.s_bFullScreen = config.Window.Fullscreen;
        m_Global.CheckResolution();
    }

    public void SetAccount(string id, string server)
    {
        m_szID = id;
        m_szServer = server;
    }

    public void SetCharacter(string characterName)
    {
        m_szChar = characterName;
        m_bLoadChar = !string.IsNullOrWhiteSpace(characterName);
    }

    public bool IsFullScreen() => m_Global.s_bFullScreen;

    public void SetFullScreen(bool value) => m_Global.s_bFullScreen = value;

    public bool IsEnablePortal() => _enablePortal;

    public void SetEnablePortal(bool value) => _enablePortal = value;

    public bool IsMovePortal() => _movePortal;

    public void SetMovePortal(bool value) => _movePortal = value;

    public bool LoadGlobal()
    {
        string path = GetJsonSidecarPath(RES_GLOBAL_PATH);
        if (!File.Exists(path))
        {
            return false;
        }

        var options = new JsonSerializerOptions { IncludeFields = true, PropertyNameCaseInsensitive = true };
        sGLOBAL? loaded = JsonSerializer.Deserialize<sGLOBAL>(File.ReadAllText(path), options);
        if (loaded is null || loaded.s_dwVersion != RES_GLOBAL_VERSION)
        {
            return false;
        }

        m_Global.CopyFrom(loaded);
        m_Global.CheckResolution();
        return true;
    }

    public void SaveGlobal()
    {
        string path = GetJsonSidecarPath(RES_GLOBAL_PATH);
        string? directory = Path.GetDirectoryName(path);
        if (!string.IsNullOrWhiteSpace(directory))
        {
            Directory.CreateDirectory(directory);
        }

        var options = new JsonSerializerOptions { IncludeFields = true, WriteIndented = true };
        File.WriteAllText(path, JsonSerializer.Serialize(m_Global, options));
    }

    private static string GetJsonSidecarPath(string legacyPath) => legacyPath + ".json";

    public class sSTRUCT_HEADER
    {
        public int s_dwVersion;
        public int s_dwStructSize;
    }

    public sealed class sGLOBAL : sSTRUCT_HEADER
    {
        public enum eSHADOW
        {
            SHADOW_OFF,
            SHADOW_ON
        }

        public enum eATT_EFF_MODE
        {
            AEM_ALL_OFF,
            AEM_ONLY_NORATT_ON,
            AEM_ALL_ON
        }

        public enum eLANGUAGE
        {
            KOREA_TRANSLATION,
            ENGLISH,
            HONGKONG,
            TAIWAN,
            THAILAND
        }

        public int s_nLeft;
        public int s_nTop;
        public bool s_bFullScreen;
        public bool s_bMusic;
        public float s_fMusic;
        public bool s_bSound;
        public float s_fSound;
        public bool s_bSystem;
        public float s_fSystem;
        public bool s_bSaveID;
        public string s_szLastConnectID = string.Empty;
        public int s_nResolutionWidth;
        public int s_nResolutionHeight;
        public int s_eTexFilter;
        public int s_nShadowType;
        public uint s_nAntialicing;
        public bool s_bCharOutLine;
        public bool s_bHSync;
        public bool s_bCell;
        public float s_fObjectSightRate;
        public int s_nOptionDefIndex;
        public bool s_bMyRenderName;
        public bool s_bOtherRenderName;
        public bool s_bMonsterRenderName;
        public bool s_bNpcRenderName;
        public bool s_bViewHpBar;
        public bool s_bChatBalloone;
        public bool s_bTipBalloone;
        public bool s_bDigimonBalloone;
        public bool s_bFigureTamer;
        public bool s_bFigureDigimon;
        public bool s_bFigurePat;
        public eLANGUAGE s_eFTLanguage;
        public bool s_bRightHandMouse;
        public bool s_bEnableVoice;
        public bool s_bUseMyShader;
        public bool s_bTradeActive;
        public bool s_bPartyActive;
        public bool s_bViewLevelEffect;
        public bool s_bUseBarcode;
        public bool s_bCameraEnemyCritical;
        public int s_nWeather;
        public bool s_bUseBright;
        public float s_fBright;
        public eATT_EFF_MODE s_eAttackEffectViewMode;
        public bool s_bRightToolTip;
        public bool s_bBossWarning;
        public uint s_nRefreshTime;
        public int s_nPostEffect;
        public bool s_bFigureEmployment;
        public bool s_bEnableSound_3DMove;
        public bool s_bDualActive;
        public bool s_bGuildInvitation;
        public bool s_bMoveDigimon;
        public bool s_bMove_Mouse;
        public bool s_bMove_KeyBoard;
        public uint s_SkipSPWTime;

        public void Reset()
        {
            s_dwVersion = RES_GLOBAL_VERSION;
            s_dwStructSize = 0;
            s_nLeft = 0;
            s_nTop = 0;
            s_bFullScreen = false;
            s_bSaveID = true;
            s_szLastConnectID = string.Empty;
            s_bCharOutLine = true;
            s_nShadowType = (int)eSHADOW.SHADOW_ON;
            s_nWeather = 1;
            s_eTexFilter = 0;
            s_nAntialicing = 2;
            s_bHSync = true;
            s_bCell = true;
            s_fObjectSightRate = 1.0f;
            s_bUseBright = false;
            s_fBright = 1.0f;
            s_nOptionDefIndex = -1;
            s_bUseMyShader = true;
            s_bUseBarcode = false;
            s_bRightToolTip = true;
            s_bBossWarning = true;
            s_nRefreshTime = 0;
            s_nPostEffect = 0;
            s_SkipSPWTime = 0;
            ResetSound();
            ResetResolution();
            ResetInterface();
        }

        public void ResetSound()
        {
            s_fMusic = 0.7f;
            s_bMusic = true;
            s_fSound = 1.0f;
            s_bSound = true;
            s_fSystem = 1.0f;
            s_bSystem = true;
            s_bEnableVoice = true;
            s_bEnableSound_3DMove = false;
        }

        public void ResetResolution()
        {
            s_nResolutionWidth = 1024;
            s_nResolutionHeight = 768;
        }

        public bool CheckResolution()
        {
            if (SupportedResolutions.Any(r => r.Width == s_nResolutionWidth && r.Height == s_nResolutionHeight))
            {
                return true;
            }

            (int width, int height) = SupportedResolutions[0];
            s_nResolutionWidth = width;
            s_nResolutionHeight = height;
            return false;
        }

        public void ResetInterface()
        {
            s_bMyRenderName = true;
            s_bOtherRenderName = true;
            s_bMonsterRenderName = true;
            s_bNpcRenderName = true;
            s_bViewHpBar = true;
            s_bChatBalloone = true;
            s_bTipBalloone = true;
            s_bDigimonBalloone = true;
            s_bFigureTamer = false;
            s_bFigureDigimon = false;
            s_bFigureEmployment = false;
            s_bFigurePat = false;
            s_eFTLanguage = eLANGUAGE.ENGLISH;
            s_bRightHandMouse = true;
            s_bTradeActive = true;
            s_bPartyActive = true;
            s_bDualActive = true;
            s_bGuildInvitation = true;
            s_bViewLevelEffect = true;
            s_bCameraEnemyCritical = true;
            s_eAttackEffectViewMode = eATT_EFF_MODE.AEM_ALL_ON;
            s_bRightToolTip = true;
            s_bBossWarning = true;
            s_bMoveDigimon = false;
            s_bMove_KeyBoard = true;
            s_bMove_Mouse = true;
        }

        public void SetSkip2ndPassword(bool value)
        {
            s_SkipSPWTime = value ? (uint)DateTimeOffset.UtcNow.ToUnixTimeSeconds() : 0;
        }

        public void ReleaseLastConnectID()
        {
            s_szLastConnectID = string.Empty;
        }

        public void CopyFrom(sGLOBAL source)
        {
            s_dwVersion = source.s_dwVersion;
            s_dwStructSize = source.s_dwStructSize;
            s_nLeft = source.s_nLeft;
            s_nTop = source.s_nTop;
            s_bFullScreen = source.s_bFullScreen;
            s_bMusic = source.s_bMusic;
            s_fMusic = source.s_fMusic;
            s_bSound = source.s_bSound;
            s_fSound = source.s_fSound;
            s_bSystem = source.s_bSystem;
            s_fSystem = source.s_fSystem;
            s_bSaveID = source.s_bSaveID;
            s_szLastConnectID = source.s_szLastConnectID;
            s_nResolutionWidth = source.s_nResolutionWidth;
            s_nResolutionHeight = source.s_nResolutionHeight;
            s_eTexFilter = source.s_eTexFilter;
            s_nShadowType = source.s_nShadowType;
            s_nAntialicing = source.s_nAntialicing;
            s_bCharOutLine = source.s_bCharOutLine;
            s_bHSync = source.s_bHSync;
            s_bCell = source.s_bCell;
            s_fObjectSightRate = source.s_fObjectSightRate;
            s_nOptionDefIndex = source.s_nOptionDefIndex;
            s_bMyRenderName = source.s_bMyRenderName;
            s_bOtherRenderName = source.s_bOtherRenderName;
            s_bMonsterRenderName = source.s_bMonsterRenderName;
            s_bNpcRenderName = source.s_bNpcRenderName;
            s_bViewHpBar = source.s_bViewHpBar;
            s_bChatBalloone = source.s_bChatBalloone;
            s_bTipBalloone = source.s_bTipBalloone;
            s_bDigimonBalloone = source.s_bDigimonBalloone;
            s_bFigureTamer = source.s_bFigureTamer;
            s_bFigureDigimon = source.s_bFigureDigimon;
            s_bFigurePat = source.s_bFigurePat;
            s_eFTLanguage = source.s_eFTLanguage;
            s_bRightHandMouse = source.s_bRightHandMouse;
            s_bEnableVoice = source.s_bEnableVoice;
            s_bUseMyShader = source.s_bUseMyShader;
            s_bTradeActive = source.s_bTradeActive;
            s_bPartyActive = source.s_bPartyActive;
            s_bViewLevelEffect = source.s_bViewLevelEffect;
            s_bUseBarcode = source.s_bUseBarcode;
            s_bCameraEnemyCritical = source.s_bCameraEnemyCritical;
            s_nWeather = source.s_nWeather;
            s_bUseBright = source.s_bUseBright;
            s_fBright = source.s_fBright;
            s_eAttackEffectViewMode = source.s_eAttackEffectViewMode;
            s_bRightToolTip = source.s_bRightToolTip;
            s_bBossWarning = source.s_bBossWarning;
            s_nRefreshTime = source.s_nRefreshTime;
            s_nPostEffect = source.s_nPostEffect;
            s_bFigureEmployment = source.s_bFigureEmployment;
            s_bEnableSound_3DMove = source.s_bEnableSound_3DMove;
            s_bDualActive = source.s_bDualActive;
            s_bGuildInvitation = source.s_bGuildInvitation;
            s_bMoveDigimon = source.s_bMoveDigimon;
            s_bMove_Mouse = source.s_bMove_Mouse;
            s_bMove_KeyBoard = source.s_bMove_KeyBoard;
            s_SkipSPWTime = source.s_SkipSPWTime;
        }
    }

    public sealed class sACCOUNT_GLOBAL : sSTRUCT_HEADER
    {
        public int s_nLastConnetServer;
        public string s_szServerName = string.Empty;

        public void Reset()
        {
            s_dwVersion = RES_ACCOUNT_GLOBAL_VERSION;
            s_dwStructSize = 0;
            s_nLastConnetServer = 0;
            s_szServerName = string.Empty;
        }
    }

    public sealed class sACCOUNT_SERVER : sSTRUCT_HEADER
    {
        public int s_nLastSelChar;

        public void Reset()
        {
            s_dwVersion = RES_ACCOUNT_SERVER_VERSION;
            s_dwStructSize = 0;
            s_nLastSelChar = -1;
        }
    }
}
