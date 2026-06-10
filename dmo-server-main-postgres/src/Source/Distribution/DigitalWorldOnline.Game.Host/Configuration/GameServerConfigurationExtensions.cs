using Microsoft.Extensions.Configuration;

namespace DigitalWorldOnline.Game.Configuration
{
    public static class GameServerConfigurationExtensions
    {
        private const string GameServerPublicPort = "GameServer:PublicPort";
        private const string GameServerPort = "GameServer:Port";

        public static string? GetPublicGameServerPort(this IConfiguration configuration)
        {
            return configuration[GameServerPublicPort] ?? configuration[GameServerPort];
        }
    }
}
