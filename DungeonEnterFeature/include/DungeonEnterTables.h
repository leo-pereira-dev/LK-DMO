#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace dungeon_enter
{
struct DungeonTableFile
{
    std::string_view xmlFile;
    std::string_view binFile;
    std::string_view purpose;
};

struct DungeonImageRecord
{
    std::uint32_t dungeonId;
    std::uint16_t difficulty;
    std::uint16_t group;
    std::string_view imagePath;
};

inline constexpr std::array<DungeonTableFile, 9> kDungeonTables{{
    {"DungeonList.xml", "DungeonList.bin", "Dungeon id, difficulty/group fields, preview image path"},
    {"DungeonQualification.xml", "DungeonQualification.bin", "entry requirement rows"},
    {"DungeonObtainable.xml", "DungeonObtainable.bin", "monster reward item ids shown in the right panel"},
    {"DungeonValue.xml", "DungeonValue.bin", "global dungeon values"},
    {"DungeonReward.xml", "DungeonReward.bin", "clear reward item groups"},
    {"DungeonStepInfo.xml", "DungeonStepInfo.bin", "step/progression info"},
    {"DungeonClearInfo.xml", "DungeonClearInfo.bin", "clear condition rows"},
    {"Dungeon_Old.xml", "Dungeon_Old.bin", "legacy dungeon marker table"},
    {"MapDungeonRegion.xml", "MapDungeonRegion.bin", "map-region binding for dungeon entry"},
}};

inline constexpr std::array<DungeonImageRecord, 4> kScreenshotRelevantRecords{{
    {5001, 2, 1, "DungeonUI\\DungeonImage\\Dungeon_50.png"},
    {300701, 2, 1, "DungeonUI\\DungeonImage\\Dungeon_70.png"},
    {300801, 3, 1, "DungeonUI\\DungeonImage\\Dungeon_407.png"},
    {300901, 3, 1, "DungeonUI\\DungeonImage\\Dungeon_1701.png"},
}};

inline constexpr std::uint32_t kTrainingGankoomonCandidateDungeonId = 5001;
inline constexpr std::uint32_t kTrainingGankoomonRequiredDigimonLevel = 150;
} // namespace dungeon_enter
