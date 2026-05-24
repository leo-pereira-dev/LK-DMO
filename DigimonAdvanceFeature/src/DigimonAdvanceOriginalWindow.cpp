#include "../include/DigimonAdvanceOriginalWindow.h"

namespace digimon_advance_original {

constexpr TableRef kOfficialTables[] = {
    {"EvoDigitama", true},
    {"EnchantItemInfo", true},
    {"EnchantStatInfo", true},
    {"TranscendNeed", true},
    {"TranscendMaterial", true},
    {"TranscendCorrect", true},
    {"TranscendCorrectEx", true},
    {"ItemData", false},
    {"ItemType", false},
    {"ItemRankData", false},
    {"ItemRankEffectData", false},
    {"QuestItemData", false},
    {"DEvolutionAdvice", false},
};

constexpr DecompiledFunctionRef kPrimaryWindowFunctions[] = {
    {"01283910", "CreateEnchantTacticsWindowFactory", "EnchantTacticsUI"},
    {"0128c990", "CreateDigiPowerEnchantWindow", "DigiPowerEnchantUI"},
    {"0128c5d0", "CreateEnchantOptionWindow", "EnchantOptionUI"},
    {"0128c7f0", "CreateEnchantOptionResultWindow", "EnchantOptionResultUI"},
    {"0128bfa0", "CreateDigimonTranscendenceHelpWindow", "DigimonTranscendenceHelpUI"},
    {"0161e970", "RegisterDigimonAdvanceTables", "DigimonAdvanceTables"},
};

constexpr AssetRef kCoreVisibleAssetRefs[] = {
    {"Enchant_Room.mp3", "CoreSoundOrEffect"},
    {"EnchantTactics\\card_base.tga", "CoreEnchantTacticsAsset"},
    {"EnchantTactics\\card_base2.tga", "CoreEnchantTacticsAsset"},
    {"EnchantTactics\\NO.tga", "CoreEnchantTacticsAsset"},
    {"DetailInfo\\Enchant_AT.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"DetailInfo\\Enchant_bl.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"DetailInfo\\Enchant_ct.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"DetailInfo\\Enchant_ev.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"DetailInfo\\Enchant_HP.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"DigimonStatus_New\\Digimon_Status_Enchant_01.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"TacticsHouse\\CheckOnline.tga", "CoreStatusAndTacticsHouseUiAsset"},
    {"Breakthrough\\Breakthrough_Main_BG.tga", "CoreTranscendenceUiAsset"},
    {"Breakthrough\\Breakthrough_Digimon_info_BG.tga", "CoreTranscendenceUiAsset"},
    {"system\\LevelEffect\\Enchant_K45.nif", "CoreEnchantLevelEffect"},
    {"system\\LevelEffect\\Enchant_K60.nif", "CoreEnchantLevelEffect"},
    {"System\\transcendental.wav", "CoreSoundOrEffect"},
};

} // namespace digimon_advance_original
