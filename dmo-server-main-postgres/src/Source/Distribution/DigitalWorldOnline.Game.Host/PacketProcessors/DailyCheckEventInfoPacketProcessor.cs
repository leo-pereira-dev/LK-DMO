using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class DailyCheckEventInfoPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DailyCheckEventInfo;

        private readonly DailyCheckService _dailyCheck;
        private readonly ILogger _logger;

        public DailyCheckEventInfoPacketProcessor(DailyCheckService dailyCheck, ILogger logger)
        {
            _dailyCheck = dailyCheck;
            _logger = logger;
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            _logger.Debug("Sending daily-check event panel for tamer {TamerId}.", client.TamerId);
            client.Send(new DailyCheckEventInfoPacket(_dailyCheck.GetSnapshot(client.TamerId, DateTime.UtcNow)));
            return Task.CompletedTask;
        }
    }
}
