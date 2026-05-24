#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace union_combine_fusion_select_original {

struct Rect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

struct DecompiledFunctionRef {
    std::string_view cleanName;
    uint32_t entry;
    std::string_view copiedFile;
    std::string_view role;
};

struct RuntimeAssetRef {
    std::string_view path;
    std::string_view firstFunction;
    std::string_view role;
};

struct TableRef {
    std::string_view tableName;
    std::string_view xmlFile;
    std::string_view binFile;
    std::string_view role;
};

struct MainUiLayout {
    uint32_t classSize;
    uint32_t activeContentOffset;
    uint32_t registerUiOffset;
    uint32_t rateInfoUiOffset;
    uint32_t resultUiOffset;
    uint32_t sceneUiOffset;
};

struct RegisterLayout {
    int32_t materialRows;
    int32_t materialsPerRow;
    Rect addOrCancelButton;
    Rect rewardButton;
    Rect rateInfoButton;
    Rect autoRegisterButton;
    Rect combineButton;
    Rect exitButton;
};

std::string_view InternalWindowName();
std::string_view MainClassName();

const MainUiLayout& GetMainUiLayout();
const RegisterLayout& GetRegisterLayout();

std::array<std::string_view, 8> RankFilterLabels();
std::array<std::string_view, 7> RankLabels();

std::array<DecompiledFunctionRef, 28> DecompiledFunctions();
std::array<TableRef, 9> RequiredTables();
std::array<RuntimeAssetRef, 20> VisibleCoreAssets();

} // namespace union_combine_fusion_select_original
