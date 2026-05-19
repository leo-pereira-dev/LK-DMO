using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DailyCheckEventClaimResultPacket : PacketWriter
    {
        private const int PacketNumber = 3137;

        public const int ResultSuccess = 0;
        public const int ResultAlreadyClaimed = 30597;
        public const int ResultNotInTime = 30598;

        public DailyCheckEventClaimResultPacket(
            int result,
            uint groupNo,
            uint currentDay,
            uint nextLeftSeconds,
            int itemCount = 0)
        {
            Type(PacketNumber);
            WriteInt(result);
            WriteUInt(groupNo);
            WriteUInt(currentDay);
            WriteUInt(nextLeftSeconds);
            WriteInt(itemCount);
        }
    }
}
