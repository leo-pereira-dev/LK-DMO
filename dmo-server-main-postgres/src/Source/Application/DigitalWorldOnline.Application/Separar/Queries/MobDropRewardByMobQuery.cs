using DigitalWorldOnline.Commons.DTOs.Config;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Queries
{
    public sealed class MobDropRewardByMobQuery : IRequest<MobDropRewardConfigDTO>
    {
        public int MapId { get; }
        public int MobType { get; }
        public int X { get; }
        public int Y { get; }
        public long RuntimeMobId { get; }

        public MobDropRewardByMobQuery(int mapId, int mobType, int x, int y, long runtimeMobId)
        {
            MapId = mapId;
            MobType = mobType;
            X = x;
            Y = y;
            RuntimeMobId = runtimeMobId;
        }
    }
}
