#pragma once

#include <cstdint>
#include <string_view>

namespace digimon_advance_original {

struct DecompiledFunctionRef {
    std::string_view address;
    std::string_view cleanName;
    std::string_view area;
};

struct TableRef {
    std::string_view name;
    bool loadedDirectlyByOfficialLoader;
};

struct AssetRef {
    std::string_view path;
    std::string_view classification;
};

class DigimonAdvanceOriginalWindowMap {
public:
    static constexpr std::string_view kMainRuntimeClass = "EnchantTactics";
    static constexpr std::string_view kTopSelectorClass = "DigiPowerEnchant";
    static constexpr std::string_view kOptionPopupClass = "EnchantOption";
    static constexpr std::string_view kOptionResultPopupClass = "EnchantOptionResultUI";
    static constexpr std::string_view kHelpPopupClass = "DigimonTranscendenceHelp";
    static constexpr std::string_view kPageName = "DigimonEnchantPage";

    static constexpr std::uint32_t kCreateEnchantTacticsFactory = 0x01283910;
    static constexpr std::uint32_t kCreateDigiPowerEnchantFactory = 0x0128c990;
    static constexpr std::uint32_t kCreateEnchantOptionFactory = 0x0128c5d0;
    static constexpr std::uint32_t kCreateEnchantOptionResultFactory = 0x0128c7f0;
    static constexpr std::uint32_t kCreateTranscendenceHelpFactory = 0x0128bfa0;
    static constexpr std::uint32_t kRegisterTables = 0x0161e970;

    static constexpr std::string_view kCopiedFunctionManifest =
        "DigimonAdvanceFeature/docs/CopiedFunctionFiles.csv";
    static constexpr std::string_view kCoreAssetManifest =
        "DigimonAdvanceFeature/assets/manifest/CoreAndPopupAssetReferences.csv";
    static constexpr std::string_view kAllAssetManifest =
        "DigimonAdvanceFeature/assets/manifest/RequiredAssetsTablesAndSounds.csv";
};

} // namespace digimon_advance_original
