using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.DTOs.Config;

namespace DigitalWorldOnline.Application.Separar.Queries;

internal static class MapKillSpawnBuilder
{
    public static List<KillSpawnConfigDTO> Build(int mapId, IReadOnlyList<MapMonsterRecord> mapMobs)
    {
        var killSpawnRows = mapMobs
            .Where(x => x.KillGenMonsterTableId > 0 && x.KillGenCount > 0)
            .ToList();

        var result = new List<KillSpawnConfigDTO>(killSpawnRows.Count);
        long killSpawnId = 1;
        long sourceId = 1;
        long targetId = 1;

        foreach (var row in killSpawnRows)
        {
            var requiredKills = (byte)Math.Clamp(row.KillGenCount, 1, byte.MaxValue);
            var targetAmount = (byte)Math.Clamp(row.Count, 1, byte.MaxValue);

            result.Add(new KillSpawnConfigDTO
            {
                Id = killSpawnId,
                GameMapConfigId = mapId,
                ShowOnMinimap = row.KillGenViewCount > 0,
                SourceMobs = new List<KillSpawnSourceMobConfigDTO>
                {
                    new()
                    {
                        Id = sourceId++,
                        KillSpawnId = killSpawnId,
                        SourceMobType = row.KillGenMonsterTableId,
                        SourceMobRequiredAmount = requiredKills
                    }
                },
                TargetMobs = new List<KillSpawnTargetMobConfigDTO>
                {
                    new()
                    {
                        Id = targetId++,
                        KillSpawnId = killSpawnId,
                        TargetMobType = row.MonsterTableId,
                        TargetMobAmount = targetAmount
                    }
                }
            });

            killSpawnId++;
        }

        return result;
    }
}
