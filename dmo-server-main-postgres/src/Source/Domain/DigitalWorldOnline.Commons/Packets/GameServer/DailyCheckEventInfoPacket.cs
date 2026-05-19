using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DailyCheckEventInfoPacket : PacketWriter
    {
        private const int PacketNumber = 3136;

        public DailyCheckEventInfoPacket(IEnumerable<DailyCheckEventGroupState> groups)
        {
            Type(PacketNumber);
            var list = groups.ToList();
            WriteInt(list.Count);

            foreach (var group in list)
            {
                WriteUInt(group.GroupNo);
                WriteUInt(group.CurrentDay);
                WriteUInt(group.NextLeftSeconds);
                WriteBytes(group.CheckedBits);
            }
        }
    }

    public sealed record DailyCheckEventGroupState(
        uint GroupNo,
        uint CurrentDay,
        uint NextLeftSeconds,
        byte[] CheckedBits);
}
