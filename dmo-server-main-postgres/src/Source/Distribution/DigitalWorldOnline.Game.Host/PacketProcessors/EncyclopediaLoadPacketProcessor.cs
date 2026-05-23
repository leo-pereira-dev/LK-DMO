using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class EncyclopediaLoadPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.EncyclopediaLoad;

        public EncyclopediaLoadPacketProcessor() { }

        public Task Process(GameClient client, byte[] packetData)
        {
            var archivedDigimons = client.Tamer.DigimonArchive?.DigimonArchives?
                .Select(x => x.Digimon)
                .Where(x => x != null)
                .Cast<DigimonModel>() ?? Enumerable.Empty<DigimonModel>();

            var digimonRecords = client.Tamer.Digimons
                .Concat(archivedDigimons)
                .Where(x => x.BaseType > 0)
                .GroupBy(x => x.BaseType)
                .Select(group => new
                {
                    Digimon = group.OrderByDescending(x => x.Level).First(),
                    SlotOpened = group.Aggregate(0UL, (mask, digimon) => mask | GetSlotOpenedMask(digimon))
                })
                .ToList();

            PacketWriter writer = new();
            writer.Type(3234);
            writer.WriteInt(digimonRecords.Count);

            foreach (var record in digimonRecords)
            {
                var digimon = record.Digimon;
                writer.WriteInt(digimon.BaseType);
                writer.WriteShort(digimon.Level);

                writer.WriteUInt64(record.SlotOpened);

                writer.WriteShort(digimon.Digiclone.ATLevel);
                writer.WriteShort(digimon.Digiclone.BLLevel);
                writer.WriteShort(digimon.Digiclone.CTLevel);
                writer.WriteShort(digimon.Digiclone.EVLevel);
                writer.WriteShort(digimon.Digiclone.HPLevel);

                writer.WriteShort(digimon.Size);
                writer.WriteByte(0);
            }

            client.Send(writer.Serialize());
            return Task.CompletedTask;
        }

        private static ulong GetSlotOpenedMask(DigimonModel digimon)
        {
            ulong slotOpened = 0;
            var maxSlot = Math.Min(digimon.Evolutions.Count, 64);

            for (var slot = 0; slot < maxSlot; slot++)
            {
                if (digimon.Evolutions[slot].Unlocked > 0)
                    slotOpened |= 1UL << slot;
            }

            return slotOpened;
        }
    }
}
