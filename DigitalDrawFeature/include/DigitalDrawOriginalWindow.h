#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace digital_draw_original {

struct Rect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

struct MainUiLayout {
    uint32_t classSize;
    uint32_t contentsOffset;
    uint32_t bottomUiOffset;
    uint32_t activeContentOffset;
    uint32_t selectUiOffset;
    uint32_t rateInfoUiOffset;
    uint32_t resultUiOffset;
    uint32_t sceneUiOffset;
    uint32_t ticketSelectUiOffset;
};

struct ClassSize {
    std::string_view className;
    uint32_t size;
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

std::string_view ReusedRuntimeWindowName();
std::string_view ExclusiveTableName();
std::string_view MissingOfficialTableBin();

const MainUiLayout& GetMainUiLayout();

std::array<ClassSize, 7> ReusedClassSizes();
std::array<DecompiledFunctionRef, 19> KeyFunctions();
std::array<RuntimeAssetRef, 17> VisibleCoreAssets();

} // namespace digital_draw_original
