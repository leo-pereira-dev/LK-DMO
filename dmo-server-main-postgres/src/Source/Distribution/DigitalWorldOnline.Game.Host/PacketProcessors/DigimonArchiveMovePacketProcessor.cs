using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Utils;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class DigimonArchiveMovePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DigimonArchiveMove;

        private readonly ISender _sender;
        private readonly ILogger _logger;

        public DigimonArchiveMovePacketProcessor(ISender sender, ILogger logger)
        {
            _sender = sender;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var vipEnabled = false;
            if (packetData.Length > 18)
                vipEnabled = Convert.ToBoolean(packet.ReadByte());

            var sourceArchiveSlot = packet.ReadInt() - 1000;
            var destinationArchiveSlot = packet.ReadInt() - 1000;
            var npcId = packet.ReadInt();

            _logger.Information(
                "ARCHIVE 3243 request tamer={TamerId} vip={Vip} sourceSlot={SourceSlot} destinationSlot={DestinationSlot} npc={NpcId} packetLen={PacketLength} archive=[{ArchiveItems}]",
                client.TamerId,
                vipEnabled,
                sourceArchiveSlot,
                destinationArchiveSlot,
                npcId,
                packetData.Length,
                string.Join(",", client.Tamer.DigimonArchive.DigimonArchives
                    .OrderBy(x => x.Slot)
                    .Where(x => x.DigimonId > 0)
                    .Select(x => $"{x.Slot}:{x.DigimonId}:{x.Digimon?.Name}")));

            if (!IsValidArchiveSlot(client, sourceArchiveSlot) || !IsValidArchiveSlot(client, destinationArchiveSlot))
            {
                _logger.Warning(
                    "ARCHIVE 3243 rejected invalid slot. tamer={TamerId} sourceSlot={SourceSlot} destinationSlot={DestinationSlot} openedSlots={OpenedSlots}",
                    client.TamerId,
                    sourceArchiveSlot,
                    destinationArchiveSlot,
                    client.Tamer.DigimonArchive.Slots);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            if (sourceArchiveSlot == destinationArchiveSlot)
            {
                client.Send(new DigimonArchiveMovePacket(sourceArchiveSlot, destinationArchiveSlot));
                return;
            }

            var sourceArchiveItem = client.Tamer.DigimonArchive.DigimonArchives.FirstOrDefault(x => x.Slot == sourceArchiveSlot);
            if (sourceArchiveItem == null || sourceArchiveItem.DigimonId <= 0)
            {
                _logger.Warning(
                    "ARCHIVE 3243 rejected empty source. tamer={TamerId} sourceSlot={SourceSlot} destinationSlot={DestinationSlot}",
                    client.TamerId,
                    sourceArchiveSlot,
                    destinationArchiveSlot);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            var destinationArchiveItem = client.Tamer.DigimonArchive.DigimonArchives.FirstOrDefault(x => x.Slot == destinationArchiveSlot);
            if (destinationArchiveItem == null)
            {
                destinationArchiveItem = new CharacterDigimonArchiveItemModel(destinationArchiveSlot);
                client.Tamer.DigimonArchive.DigimonArchives.Add(destinationArchiveItem);
                await _sender.Send(new CreateCharacterDigimonArchiveSlotCommand(
                    destinationArchiveItem,
                    client.Tamer.DigimonArchive.Id));
            }

            var sourceDigimonId = sourceArchiveItem.DigimonId;
            var sourceDigimon = sourceArchiveItem.Digimon;
            var destinationDigimonId = destinationArchiveItem.DigimonId;
            var destinationDigimon = destinationArchiveItem.Digimon;

            if (destinationDigimonId > 0)
            {
                sourceArchiveItem.AddDigimon(destinationDigimonId);
                sourceArchiveItem.SetDigimonInfo(destinationDigimon);
            }
            else
            {
                sourceArchiveItem.RemoveDigimon();
            }

            destinationArchiveItem.AddDigimon(sourceDigimonId);
            destinationArchiveItem.SetDigimonInfo(sourceDigimon);

            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(sourceArchiveItem));
            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(destinationArchiveItem));

            _logger.Information(
                "ARCHIVE 3243 moved tamer={TamerId} sourceSlot={SourceSlot} destinationSlot={DestinationSlot} movedDigimon={MovedDigimonId}:{MovedDigimonName} swappedDigimon={SwappedDigimonId}:{SwappedDigimonName}",
                client.TamerId,
                sourceArchiveSlot,
                destinationArchiveSlot,
                sourceDigimonId,
                sourceDigimon?.Name,
                destinationDigimonId,
                destinationDigimon?.Name);

            client.Send(new DigimonArchiveMovePacket(sourceArchiveSlot, destinationArchiveSlot));
        }

        private static bool IsValidArchiveSlot(GameClient client, int archiveSlot)
        {
            return archiveSlot >= 0 && archiveSlot < client.Tamer.DigimonArchive.Slots;
        }
    }
}
