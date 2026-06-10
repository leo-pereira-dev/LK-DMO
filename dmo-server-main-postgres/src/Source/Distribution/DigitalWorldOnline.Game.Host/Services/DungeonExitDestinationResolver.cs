using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Models.Map;
using DigitalWorldOnline.Commons.Utils;
using Serilog;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class DungeonExitDestinationResolver
    {
        private readonly MapBinLoader _mapBin;
        private readonly ILogger _logger;

        public DungeonExitDestinationResolver(MapBinLoader mapBin, ILogger logger)
        {
            _mapBin = mapBin;
            _logger = logger;
        }

        public DungeonExitDestination? Resolve(MapInstance? instance, int currentMapId)
        {
            if (instance != null)
            {
                var entryPortal = ResolveEntryPortalSource(instance);
                if (entryPortal != null)
                    return entryPortal;

                return ResolveFallback(instance.MapId, instance.DungeonEntranceMapId);
            }

            return ResolveFallback(currentMapId, entranceMapId: 0);
        }

        private DungeonExitDestination? ResolveEntryPortalSource(MapInstance instance)
        {
            if (instance.DungeonEntryPortalId <= 0 || instance.DungeonEntranceMapId <= 0)
                return null;

            if (!_mapBin.Data.PortalsBySourceMapId.TryGetValue(instance.DungeonEntranceMapId, out var portals))
            {
                _logger.Warning(
                    "Dungeon exit fallback: entrance map {MapId} has no portal records for dungeon map {DungeonMapId}.",
                    instance.DungeonEntranceMapId,
                    instance.MapId);
                return null;
            }

            var portal = portals.FirstOrDefault(x => x.PortalId == instance.DungeonEntryPortalId);
            if (portal == null)
            {
                _logger.Warning(
                    "Dungeon exit fallback: entry portal {PortalId} was not found on entrance map {MapId} for dungeon map {DungeonMapId}.",
                    instance.DungeonEntryPortalId,
                    instance.DungeonEntranceMapId,
                    instance.MapId);
                return null;
            }

            return new DungeonExitDestination(instance.DungeonEntranceMapId, portal.SourceX, portal.SourceY, "EntryPortal");
        }

        private DungeonExitDestination? ResolveFallback(int currentMapId, int entranceMapId)
        {
            var mapId = entranceMapId > 0 ? entranceMapId : UtilitiesFunctions.MapGroup(currentMapId);
            if (mapId <= 0 &&
                _mapBin.Data.MapsById.TryGetValue(currentMapId, out var mapRecord) &&
                mapRecord.ResurrectionMapId > 0)
            {
                mapId = mapRecord.ResurrectionMapId;
            }

            if (mapId <= 0)
                mapId = 3;

            if (!_mapBin.Data.RegionsByMapId.TryGetValue(mapId, out var regions) || regions.Count == 0)
                return null;

            var destination = regions[0];
            return new DungeonExitDestination(mapId, destination.CenterX, destination.CenterY, "MapRegion");
        }
    }

    public sealed record DungeonExitDestination(int MapId, int X, int Y, string Source);
}
