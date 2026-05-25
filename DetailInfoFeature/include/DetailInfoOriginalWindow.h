#pragma once

#include <array>
#include <string_view>

namespace detail_info_feature {

enum class DetailInfoTab {
    Tamer,
    Digimon,
};

struct OriginalFunctionRef {
    std::string_view address;
    std::string_view original_name;
    std::string_view clean_name;
    std::string_view copied_file;
    std::string_view purpose;
};

struct OriginalAssetRef {
    std::string_view asset_path;
    std::string_view owning_function;
    std::string_view purpose;
};

struct OriginalStatTextRef {
    int index;
    std::string_view text_key;
    std::string_view screen_label_hint;
};

const std::array<OriginalFunctionRef, 70>& GetOriginalFunctionMap();
const std::array<OriginalAssetRef, 42>& GetOriginalAssetMap();
const std::array<OriginalStatTextRef, 13>& GetObtainedAttributeStatTextMap();

} // namespace detail_info_feature
