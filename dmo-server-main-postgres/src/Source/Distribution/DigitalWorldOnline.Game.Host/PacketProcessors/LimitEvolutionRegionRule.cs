using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    internal static class LimitEvolutionRegionRule
    {
        public static bool ShouldStopRideMode(AssetsLoader assets, MapBinLoader mapBin, GameClient client)
        {
            if (client.Partner == null)
                return false;

            return !IsCurrentPartnerCapsule(assets, client) &&
                IsInsideLimitEvolutionRegion(mapBin, client);
        }

        public static bool ShouldDevolveToBase(AssetsLoader assets, MapBinLoader mapBin, GameClient client)
        {
            if (client.Partner == null ||
                client.Partner.CurrentType == client.Partner.BaseType)
            {
                return false;
            }

            return !IsCurrentPartnerCapsule(assets, client) &&
                IsInsideLimitEvolutionRegion(mapBin, client);
        }

        public static bool IsInsideLimitEvolutionRegion(MapBinLoader mapBin, GameClient client)
        {
            if (!mapBin.IsLoaded)
                return false;

            var mapId = client.Tamer.Location.MapId;
            if (!mapBin.Data.LimitEvoByMapId.TryGetValue(mapId, out var regions) || regions.Count == 0)
                return false;

            var x = client.Partner?.Location?.X ?? client.Tamer.Location.X;
            var y = client.Partner?.Location?.Y ?? client.Tamer.Location.Y;

            foreach (var region in regions)
            {
                long dx = x - region.CenterX;
                long dy = y - region.CenterY;
                long distanceSquared = dx * dx + dy * dy;
                long radiusSquared = (long)region.Radius * region.Radius;

                if (distanceSquared < radiusSquared)
                    return true;
            }

            return false;
        }

        private static bool IsCurrentPartnerCapsule(AssetsLoader assets, GameClient client)
        {
            var currentRank = (EvolutionRankEnum)(assets.DigimonBaseInfo
                .FirstOrDefault(x => x.Type == client.Partner.CurrentType)?.EvolutionType ?? 0);

            return currentRank == EvolutionRankEnum.Capsule;
        }
    }
}
