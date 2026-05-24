#pragma once

#include <array>
#include <string_view>

namespace item_upgrade_management_original {

struct OriginalFunctionRef {
    std::string_view entry;
    std::string_view clean_name;
    std::string_view copied_path;
    std::string_view role;
};

struct OriginalAssetRef {
    std::string_view path;
    std::string_view usage;
    std::string_view proof;
};

struct OriginalTextKeyRef {
    std::string_view key;
    std::string_view portuguese_label;
    std::string_view usage;
};

class ItemUpgradeManagementOriginalWindow {
public:
    static constexpr int kOfficialWidth = 650;
    static constexpr int kOfficialHeight = 700;

    static std::string_view InternalWindowName();
    static std::string_view AdjacentTradeWindowName();

    static const std::array<OriginalFunctionRef, 15>& CoreFunctions();
    static const std::array<OriginalAssetRef, 14>& CoreAssets();
    static const std::array<OriginalTextKeyRef, 4>& CoreTextKeys();
};

} // namespace item_upgrade_management_original
