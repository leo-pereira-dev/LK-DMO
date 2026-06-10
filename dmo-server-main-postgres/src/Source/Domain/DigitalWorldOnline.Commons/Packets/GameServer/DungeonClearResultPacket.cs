using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class DungeonClearResultPacket : PacketWriter
    {
        private const int PacketNumber = 4135;

        public DungeonClearResultPacket(
            int dungeonId,
            int entryPortalId,
            byte rank,
            ushort difficulty,
            int elapsedSeconds,
            ushort partyCount,
            ushort partyMax,
            ushort failCount,
            ushort failMax,
            int earnedExp,
            int earnedBits,
            string dungeonName,
            IReadOnlyList<DungeonClearRewardPacketItem> defaultRewards,
            IReadOnlyList<DungeonClearRewardPacketItem> extraRewards,
            IReadOnlyList<DungeonClearDetailPacketItem> detailResults)
        {
            Type(PacketNumber);
            WriteInt(dungeonId);
            WriteInt(entryPortalId);
            WriteByte(rank);
            WriteUShort(difficulty);
            WriteInt(elapsedSeconds);
            WriteUShort(partyCount);
            WriteUShort(partyMax);
            WriteUShort(failCount);
            WriteUShort(failMax);
            WriteInt(earnedExp);
            WriteInt(earnedBits);
            WriteString(dungeonName ?? string.Empty);

            WriteRewardList(defaultRewards);
            WriteRewardList(extraRewards);
            WriteDetailList(detailResults);
        }

        private void WriteRewardList(IReadOnlyList<DungeonClearRewardPacketItem> rewards)
        {
            var safeRewards = rewards ?? Array.Empty<DungeonClearRewardPacketItem>();
            WriteUShort((ushort)Math.Min(ushort.MaxValue, safeRewards.Count));

            foreach (var reward in safeRewards.Take(ushort.MaxValue))
            {
                WriteInt(reward.ItemId);
                WriteInt(reward.Amount);
            }
        }

        private void WriteDetailList(IReadOnlyList<DungeonClearDetailPacketItem> details)
        {
            var safeDetails = details ?? Array.Empty<DungeonClearDetailPacketItem>();
            WriteUShort((ushort)Math.Min(ushort.MaxValue, safeDetails.Count));

            foreach (var detail in safeDetails.Take(ushort.MaxValue))
            {
                WriteByte(detail.Category);
                WriteInt(detail.TamerModelId);
                WriteInt(detail.DigimonModelId);
                WriteUShort(detail.TamerLevel);
                WriteUShort(detail.DigimonLevel);
                WriteInt(detail.Value);
                WriteString(detail.TamerName ?? string.Empty);
                WriteString(detail.DigimonName ?? string.Empty);
            }
        }
    }

    public sealed record DungeonClearRewardPacketItem(int ItemId, int Amount);
    public sealed record DungeonClearDetailPacketItem(
        byte Category,
        int TamerModelId,
        int DigimonModelId,
        ushort TamerLevel,
        ushort DigimonLevel,
        int Value,
        string TamerName,
        string DigimonName);
}
