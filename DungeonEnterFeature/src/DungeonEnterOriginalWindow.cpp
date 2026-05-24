#include "../include/DungeonEnterOriginalWindow.h"
#include "../include/DungeonEnterTables.h"

namespace dungeon_enter
{
const WindowLayout& GetOfficialDungeonEnterLayout()
{
    return kOfficialWindowLayout;
}

std::string_view GetCandidateTrainingGankoomonImagePath()
{
    for (const auto& record : kScreenshotRelevantRecords)
    {
        if (record.dungeonId == kTrainingGankoomonCandidateDungeonId)
            return record.imagePath;
    }

    return {};
}
} // namespace dungeon_enter
