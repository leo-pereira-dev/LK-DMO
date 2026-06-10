using DProject.Config;
using DProject.GlobalData;

namespace DProject.Data.CsFileTable;

public enum eFILE_TYPE
{
    FT_EXCEL = 0,
    FT_CLONE,
    FT_BIN,
    FT_FILEPACK
}

public enum eLANGUAGE
{
    KOREA_TRANSLATION = 100,
    KOREA_ORIGINAL,
    ENGLISH,
    THAILAND,
    ENGLISH_A,
    TAIWAN,
    HONGKONG
}

public static class nsCsFileTable
{
    public static eFILE_TYPE g_eFileType = eFILE_TYPE.FT_FILEPACK;
    public static eLANGUAGE g_eLanguage = eLANGUAGE.ENGLISH;
    public static readonly CsFileTableMng g_FileTableMng = new();

    public static bool g_bUseQuest = true;
    public static bool g_bUseDigimon = true;
    public static bool g_bUseMonster = true;
    public static bool g_bUseNpc = true;
    public static bool g_bUseMap = true;
    public static bool g_bUseItem = true;
    public static bool g_bUseBase = true;
    public static bool g_bUseSkill = true;
    public static bool g_bUseNature = true;
    public static bool g_bUseCuid = true;
    public static bool g_bUseEvoExtra;
    public static bool g_bUseEvoBattle;
    public static bool g_bUseHelp = true;
    public static bool g_bUseMoveObject = true;
    public static bool g_bUseCashShop = true;
    public static bool g_bUseAchieve = true;
    public static bool g_bSceneDataMng = true;
    public static bool g_bBuffMng = true;
    public static bool g_bUseGotcha = true;

    public static void ApplyFeatures(FileTableFeatureOptions features)
    {
        g_bUseQuest = features.UseQuest;
        g_bUseDigimon = features.UseDigimon;
        g_bUseMonster = features.UseMonster;
        g_bUseNpc = features.UseNpc;
        g_bUseMap = features.UseMap;
        g_bUseItem = features.UseItem;
        g_bUseBase = features.UseBase;
        g_bUseSkill = features.UseSkill;
        g_bUseNature = features.UseNature;
        g_bUseCuid = features.UseCuid;
        g_bUseHelp = features.UseHelp;
        g_bUseMoveObject = features.UseMoveObject;
        g_bUseCashShop = features.UseCashShop;
        g_bUseAchieve = features.UseAchieve;
        g_bSceneDataMng = features.UseSceneData;
        g_bBuffMng = features.UseBuff;
        g_bUseGotcha = features.UseGotcha;
        g_bUseEvoExtra = features.UseEvoExtra;
        g_bUseEvoBattle = features.UseEvoBattle;
    }

    public static eFILE_TYPE ParseFileType(string source, bool useFilePack)
    {
        if (Enum.TryParse(source, ignoreCase: true, out eFILE_TYPE parsed))
        {
            return parsed;
        }

        return source.Equals("Bin", StringComparison.OrdinalIgnoreCase) || !useFilePack
            ? eFILE_TYPE.FT_BIN
            : eFILE_TYPE.FT_FILEPACK;
    }

    public static eLANGUAGE ParseLanguage(string language, GameLanguage fallback)
    {
        if (Enum.TryParse(language, ignoreCase: true, out eLANGUAGE parsed))
        {
            return parsed;
        }

        return language.ToLowerInvariant() switch
        {
            "korean" or "korea" or "korea_translation" => eLANGUAGE.KOREA_TRANSLATION,
            "korea_original" => eLANGUAGE.KOREA_ORIGINAL,
            "english_a" or "aeria" => eLANGUAGE.ENGLISH_A,
            "thailand" or "thai" => eLANGUAGE.THAILAND,
            "taiwan" => eLANGUAGE.TAIWAN,
            "hongkong" or "hong_kong" => eLANGUAGE.HONGKONG,
            "english" or "gsp" or "steam" => eLANGUAGE.ENGLISH,
            _ => fallback switch
            {
                GameLanguage.Korean => eLANGUAGE.KOREA_TRANSLATION,
                GameLanguage.Hongkong => eLANGUAGE.HONGKONG,
                GameLanguage.Taiwan => eLANGUAGE.TAIWAN,
                GameLanguage.Thailand => eLANGUAGE.THAILAND,
                _ => eLANGUAGE.ENGLISH
            }
        };
    }
}
