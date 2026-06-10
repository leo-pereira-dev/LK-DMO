using DigitalWorldOnline.Commons.DTOs.Config;
using DigitalWorldOnline.Commons.Interfaces;
using Microsoft.EntityFrameworkCore;

namespace DigitalWorldOnline.Infraestructure.Repositories.Config
{
    public class ConfigQueriesRepository : IConfigQueriesRepository
    {
        private static readonly SemaphoreSlim DropRewardsCacheLock = new(1, 1);
        private static IReadOnlyList<MobConfigDTO>? _mobDropRewardsCache;
        private static readonly SemaphoreSlim KillSpawnsCacheLock = new(1, 1);
        private static IReadOnlyList<KillSpawnConfigDTO>? _killSpawnsCache;

        private readonly DatabaseContext _context;

        public ConfigQueriesRepository(DatabaseContext context)
        {
            _context = context;
        }

        public async Task<IReadOnlyList<MobConfigDTO>> GetMobDropRewardsAsync(CancellationToken cancellationToken)
        {
            if (_mobDropRewardsCache != null)
                return _mobDropRewardsCache;

            await DropRewardsCacheLock.WaitAsync(cancellationToken);
            try
            {
                if (_mobDropRewardsCache != null)
                    return _mobDropRewardsCache;

                var binMobs = MobDropRewardBinLoader.LoadIfExists();
                if (binMobs != null)
                {
                    _mobDropRewardsCache = binMobs;
                    return _mobDropRewardsCache;
                }

                var mobs = await _context.MobConfig
                    .AsNoTracking()
                    .Include(x => x.GameMapConfig)
                    .Include(x => x.Location)
                    .Include(x => x.DropReward)
                        .ThenInclude(x => x.Drops)
                    .Include(x => x.DropReward)
                        .ThenInclude(x => x.BitsDrop)
                    .Where(x => x.DropReward != null)
                    .ToListAsync(cancellationToken);

                _mobDropRewardsCache = mobs
                    .Select(CloneMobDropReward)
                    .Where(x => x.DropReward != null)
                    .ToList();

                return _mobDropRewardsCache;
            }
            finally
            {
                DropRewardsCacheLock.Release();
            }
        }

        public async Task<IReadOnlyList<KillSpawnConfigDTO>> GetKillSpawnsAsync(CancellationToken cancellationToken)
        {
            if (_killSpawnsCache != null)
                return _killSpawnsCache;

            await KillSpawnsCacheLock.WaitAsync(cancellationToken);
            try
            {
                if (_killSpawnsCache != null)
                    return _killSpawnsCache;

                _killSpawnsCache = await _context.KillSpawnConfig
                    .AsNoTracking()
                    .Include(x => x.SourceMobs)
                    .Include(x => x.TargetMobs)
                    .Select(x => new KillSpawnConfigDTO
                    {
                        Id = x.Id,
                        GameMapConfigId = x.GameMapConfigId,
                        ShowOnMinimap = x.ShowOnMinimap,
                        SourceMobs = x.SourceMobs
                            .Select(source => new KillSpawnSourceMobConfigDTO
                            {
                                Id = source.Id,
                                KillSpawnId = source.KillSpawnId,
                                SourceMobType = source.SourceMobType,
                                SourceMobRequiredAmount = source.SourceMobRequiredAmount
                            })
                            .ToList(),
                        TargetMobs = x.TargetMobs
                            .Select(target => new KillSpawnTargetMobConfigDTO
                            {
                                Id = target.Id,
                                KillSpawnId = target.KillSpawnId,
                                TargetMobType = target.TargetMobType,
                                TargetMobAmount = target.TargetMobAmount
                            })
                            .ToList()
                    })
                    .ToListAsync(cancellationToken);

                return _killSpawnsCache;
            }
            finally
            {
                KillSpawnsCacheLock.Release();
            }
        }

        private static MobConfigDTO CloneMobDropReward(MobConfigDTO mob)
        {
            var mapId = mob.GameMapConfig?.MapId ?? mob.Location?.MapId ?? 0;

            return new MobConfigDTO
            {
                Id = mob.Id,
                Type = mob.Type,
                Model = mob.Model,
                Name = mob.Name,
                Level = mob.Level,
                GameMapConfigId = mob.GameMapConfigId,
                GameMapConfig = new MapConfigDTO
                {
                    Id = mob.GameMapConfigId,
                    MapId = mapId,
                    Name = mob.GameMapConfig?.Name ?? string.Empty,
                    Type = mob.GameMapConfig?.Type ?? default,
                    Mobs = new List<MobConfigDTO>(),
                    KillSpawns = new List<KillSpawnConfigDTO>()
                },
                Location = mob.Location == null
                    ? new MobLocationConfigDTO { MapId = (short)mapId }
                    : new MobLocationConfigDTO
                    {
                        Id = mob.Location.Id,
                        MobConfigId = mob.Location.MobConfigId,
                        MapId = mob.Location.MapId,
                        X = mob.Location.X,
                        Y = mob.Location.Y,
                        Z = mob.Location.Z
                    },
                DropReward = mob.DropReward == null
                    ? null
                    : new MobDropRewardConfigDTO
                    {
                        Id = mob.DropReward.Id,
                        MobId = mob.Id,
                        MinAmount = mob.DropReward.MinAmount,
                        MaxAmount = mob.DropReward.MaxAmount,
                        BitsDrop = mob.DropReward.BitsDrop == null
                            ? new BitsDropConfigDTO()
                            : new BitsDropConfigDTO
                            {
                                Id = mob.DropReward.BitsDrop.Id,
                                DropRewardId = mob.DropReward.BitsDrop.DropRewardId,
                                MinAmount = mob.DropReward.BitsDrop.MinAmount,
                                MaxAmount = mob.DropReward.BitsDrop.MaxAmount,
                                Chance = mob.DropReward.BitsDrop.Chance
                            },
                        Drops = mob.DropReward.Drops
                            .Select(drop => new ItemDropConfigDTO
                            {
                                Id = drop.Id,
                                DropRewardId = drop.DropRewardId,
                                ItemId = drop.ItemId,
                                MinAmount = drop.MinAmount,
                                MaxAmount = drop.MaxAmount,
                                Chance = drop.Chance,
                                Rank = drop.Rank
                            })
                            .ToList()
                    }
            };
        }
    }
}
