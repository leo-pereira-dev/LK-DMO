using DigitalWorldOnline.Commons.DTOs.Config;

namespace DigitalWorldOnline.Commons.Interfaces
{
    public interface IConfigQueriesRepository
    {
        Task<IReadOnlyList<MobConfigDTO>> GetMobDropRewardsAsync(CancellationToken cancellationToken);
    }
}
