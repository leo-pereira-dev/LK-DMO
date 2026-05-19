using System.Text.Json;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Models.Asset;
using Serilog;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class PortalDestinationResolver
    {
        private const int SourceRadiusTolerance = 2500;
        private readonly MapBinLoader _mapBin;
        private readonly ILogger _logger;
        private readonly Dictionary<(int PortalId, int SourceMapId), PortalOverride> _overrides;

        public PortalDestinationResolver(MapBinLoader mapBin, ILogger logger)
        {
            _mapBin = mapBin;
            _logger = logger;
            _overrides = LoadOverrides();
        }

        public PortalResolution Resolve(GameClient client, PortalAssetModel portal)
        {
            var sourceMapId = client.Tamer.Location.MapId;
            var record = FindPortalRecord(portal.Id, sourceMapId);
            if (record == null)
            {
                return PortalResolution.Invalid(
                    portal.Id,
                    sourceMapId,
                    $"Portal {portal.Id} does not belong to source map {sourceMapId}.");
            }

            var maxDistance = record.SourceRadius + SourceRadiusTolerance;
            var distance = Distance(client.Tamer.Location.X, client.Tamer.Location.Y, record.SourceX, record.SourceY);
            if (distance > maxDistance)
            {
                _logger.Warning(
                    "Portal distance check exceeded; continuing with client-selected portal. tamer={TamerId} portal={PortalId} map={MapId} x={X} y={Y} sourceX={SourceX} sourceY={SourceY} distance={Distance:0} allowed={Allowed}",
                    client.TamerId,
                    portal.Id,
                    sourceMapId,
                    client.Tamer.Location.X,
                    client.Tamer.Location.Y,
                    record.SourceX,
                    record.SourceY,
                    distance,
                    maxDistance);
            }

            if (_overrides.TryGetValue((portal.Id, sourceMapId), out var portalOverride))
            {
                return PortalResolution.Valid(
                    portal.Id,
                    sourceMapId,
                    portalOverride.DestinationMapId,
                    portalOverride.DestinationX,
                    portalOverride.DestinationY,
                    "Override");
            }

            return PortalResolution.Valid(
                portal.Id,
                sourceMapId,
                portal.DestinationMapId,
                portal.DestinationX,
                portal.DestinationY,
                "MapPortal.bin");
        }

        private MapPortalRecord? FindPortalRecord(int portalId, int sourceMapId)
        {
            if (!_mapBin.Data.PortalsBySourceMapId.TryGetValue(sourceMapId, out var portals))
                return null;

            return portals.FirstOrDefault(x => x.PortalId == portalId);
        }

        private Dictionary<(int PortalId, int SourceMapId), PortalOverride> LoadOverrides()
        {
            var path = ResolveOverridePath();
            if (path == null)
                return new Dictionary<(int, int), PortalOverride>();

            try
            {
                var json = File.ReadAllText(path);
                var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                var overrides = JsonSerializer.Deserialize<List<PortalOverride>>(json, options) ?? new List<PortalOverride>();

                var result = new Dictionary<(int, int), PortalOverride>();
                foreach (var entry in overrides)
                {
                    if (entry.PortalId <= 0 || entry.SourceMapId <= 0 || entry.DestinationMapId <= 0)
                    {
                        _logger.Warning("Ignoring invalid portal override: {@Override}", entry);
                        continue;
                    }

                    result[(entry.PortalId, entry.SourceMapId)] = entry;
                }

                _logger.Information("Loaded {Count} portal overrides from {Path}", result.Count, path);
                return result;
            }
            catch (Exception ex)
            {
                _logger.Error(ex, "Could not load portal overrides from {Path}", path);
                return new Dictionary<(int, int), PortalOverride>();
            }
        }

        private static string? ResolveOverridePath()
        {
            var candidates = new[]
            {
                Path.Combine(AppContext.BaseDirectory, "data", "portal-overrides.json"),
                Path.Combine(Directory.GetCurrentDirectory(), "data", "portal-overrides.json")
            };

            return candidates.FirstOrDefault(File.Exists);
        }

        private static double Distance(int ax, int ay, int bx, int by)
        {
            var dx = (long)ax - bx;
            var dy = (long)ay - by;
            return Math.Sqrt((dx * dx) + (dy * dy));
        }
    }

    public sealed class PortalOverride
    {
        public int PortalId { get; set; }
        public int SourceMapId { get; set; }
        public int DestinationMapId { get; set; }
        public int DestinationX { get; set; }
        public int DestinationY { get; set; }
        public string Reason { get; set; } = string.Empty;
    }

    public sealed class PortalResolution
    {
        private PortalResolution(
            bool success,
            int portalId,
            int sourceMapId,
            int destinationMapId,
            int destinationX,
            int destinationY,
            string source,
            string? failureReason)
        {
            Success = success;
            PortalId = portalId;
            SourceMapId = sourceMapId;
            DestinationMapId = destinationMapId;
            DestinationX = destinationX;
            DestinationY = destinationY;
            Source = source;
            FailureReason = failureReason;
        }

        public bool Success { get; }
        public int PortalId { get; }
        public int SourceMapId { get; }
        public int DestinationMapId { get; }
        public int DestinationX { get; }
        public int DestinationY { get; }
        public string Source { get; }
        public string? FailureReason { get; }

        public static PortalResolution Valid(
            int portalId,
            int sourceMapId,
            int destinationMapId,
            int destinationX,
            int destinationY,
            string source)
            => new(true, portalId, sourceMapId, destinationMapId, destinationX, destinationY, source, null);

        public static PortalResolution Invalid(int portalId, int sourceMapId, string reason)
            => new(false, portalId, sourceMapId, 0, 0, 0, "Invalid", reason);
    }
}
