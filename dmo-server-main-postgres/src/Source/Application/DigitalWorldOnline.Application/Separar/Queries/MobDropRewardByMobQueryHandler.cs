using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.DTOs.Config;
using DigitalWorldOnline.Commons.Interfaces;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Queries
{
    public sealed class MobDropRewardByMobQueryHandler : IRequestHandler<MobDropRewardByMobQuery, MobDropRewardConfigDTO>
    {
        private readonly IConfigQueriesRepository _configQueriesRepository;

        public MobDropRewardByMobQueryHandler(IConfigQueriesRepository configQueriesRepository)
        {
            _configQueriesRepository = configQueriesRepository;
        }

        public async Task<MobDropRewardConfigDTO> Handle(MobDropRewardByMobQuery request, CancellationToken cancellationToken)
        {
            var dropLookup = MobDropRewardBinder.BuildLookup(
                await _configQueriesRepository.GetMobDropRewardsAsync(cancellationToken),
                request.MapId);

            var spawn = new MapMonsterSpawnMaterializer.SpawnPoint(
                request.MobType,
                request.X,
                request.Y,
                0);

            return MobDropRewardBinder.MatchDropReward(
                spawn,
                request.RuntimeMobId,
                dropLookup,
                new HashSet<long>());
        }
    }
}
