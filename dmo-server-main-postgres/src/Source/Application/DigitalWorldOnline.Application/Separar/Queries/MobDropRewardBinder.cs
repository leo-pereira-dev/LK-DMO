using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.DTOs.Config;

namespace DigitalWorldOnline.Application.Separar.Queries
{
    internal static class MobDropRewardBinder
    {
        private const int SafeMercenaryDigiEggMinItemId = 7001;
        private const int SafeMercenaryDigiEggMaxItemId = 8080;
        private const int MercenaryDigiEggItemIdOffset = 93000;

        public static Dictionary<int, List<MobConfigDTO>> BuildLookup(
            IEnumerable<MobConfigDTO> dropMobs,
            int mapId)
        {
            return dropMobs
                .Where(x => x.DropReward != null && GetMapId(x) == mapId)
                .GroupBy(x => x.Type)
                .ToDictionary(
                    x => x.Key,
                    x => x.OrderBy(y => y.Id).ToList());
        }

        public static MobDropRewardConfigDTO MatchDropReward(
            MapMonsterSpawnMaterializer.SpawnPoint spawn,
            long runtimeMobId,
            Dictionary<int, List<MobConfigDTO>> dropLookup,
            HashSet<long> usedDropMobs)
        {
            if (!dropLookup.TryGetValue(spawn.MonsterTableId, out var candidates) || candidates.Count == 0)
                return Empty(runtimeMobId);

            var candidate = candidates
                .Where(x => !usedDropMobs.Contains(x.Id))
                .OrderBy(x => DistanceSquared(spawn, x))
                .FirstOrDefault();

            candidate ??= candidates
                .OrderBy(x => DistanceSquared(spawn, x))
                .FirstOrDefault();

            if (candidate?.DropReward == null)
                return Empty(runtimeMobId);

            usedDropMobs.Add(candidate.Id);
            return CloneDropReward(candidate.DropReward, runtimeMobId);
        }

        private static int GetMapId(MobConfigDTO mob)
        {
            if (mob.GameMapConfig?.MapId > 0)
                return mob.GameMapConfig.MapId;

            return mob.Location?.MapId ?? 0;
        }

        private static long DistanceSquared(
            MapMonsterSpawnMaterializer.SpawnPoint spawn,
            MobConfigDTO mob)
        {
            if (mob.Location == null)
                return long.MaxValue;

            var dx = (long)spawn.X - mob.Location.X;
            var dy = (long)spawn.Y - mob.Location.Y;
            return dx * dx + dy * dy;
        }

        private static MobDropRewardConfigDTO Empty(long runtimeMobId)
        {
            return new MobDropRewardConfigDTO
            {
                Id = runtimeMobId,
                MobId = runtimeMobId,
                MinAmount = 0,
                MaxAmount = 0
            };
        }

        private static MobDropRewardConfigDTO CloneDropReward(
            MobDropRewardConfigDTO source,
            long runtimeMobId)
        {
            var drops = source.Drops
                .Where(x => x.Chance > 0)
                .Select(CloneItemDrop)
                .ToList();

            var minAmount = source.MinAmount;
            var maxAmount = source.MaxAmount;
            if (drops.Count > 0 && maxAmount == 0)
            {
                minAmount = 1;
                maxAmount = 1;
            }
            else if (maxAmount < minAmount)
            {
                maxAmount = minAmount;
            }

            return new MobDropRewardConfigDTO
            {
                Id = source.Id,
                MobId = runtimeMobId,
                MinAmount = minAmount,
                MaxAmount = maxAmount,
                BitsDrop = CloneBitsDrop(source.BitsDrop),
                Drops = drops
            };
        }

        private static ItemDropConfigDTO CloneItemDrop(ItemDropConfigDTO source)
        {
            var minAmount = Math.Max(1, source.MinAmount);
            var maxAmount = Math.Max(1, source.MaxAmount);
            if (maxAmount < minAmount)
                maxAmount = minAmount;

            return new ItemDropConfigDTO
            {
                Id = source.Id,
                DropRewardId = source.DropRewardId,
                ItemId = NormalizeDropItemId(source.ItemId),
                MinAmount = minAmount,
                MaxAmount = maxAmount,
                Chance = source.Chance,
                Rank = source.Rank
            };
        }

        private static int NormalizeDropItemId(int itemId)
        {
            if (itemId >= SafeMercenaryDigiEggMinItemId && itemId <= SafeMercenaryDigiEggMaxItemId)
                return itemId + MercenaryDigiEggItemIdOffset;

            return itemId;
        }

        private static BitsDropConfigDTO CloneBitsDrop(BitsDropConfigDTO? source)
        {
            if (source == null)
                return new BitsDropConfigDTO();

            var minAmount = Math.Max(0, source.MinAmount);
            var maxAmount = Math.Max(0, source.MaxAmount);
            if (maxAmount < minAmount)
                maxAmount = minAmount;

            return new BitsDropConfigDTO
            {
                Id = source.Id,
                DropRewardId = source.DropRewardId,
                MinAmount = minAmount,
                MaxAmount = maxAmount,
                Chance = source.Chance
            };
        }
    }
}
