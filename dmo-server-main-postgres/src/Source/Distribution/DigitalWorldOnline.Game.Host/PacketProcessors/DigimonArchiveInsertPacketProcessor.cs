using AutoMapper;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using MediatR;
using Serilog;

using DigitalWorldOnline.Application.Separar.Commands.Create;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class DigimonArchiveInsertPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DigimonArchiveInsert;

        private readonly StatusManager _statusManager;
        private readonly IMapper _mapper;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public DigimonArchiveInsertPacketProcessor(
            StatusManager statusManager,
            IMapper mapper,
            ILogger logger,
            ISender sender)
        {
            _statusManager = statusManager;
            _mapper = mapper;
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            // Some client builds do not include the optional VIP byte in this packet.
            // Packet length 18 is the non-VIP shape: slot, archive slot, npc id.
            var vipEnabled = false;
            if (packetData.Length > 18)
                vipEnabled = Convert.ToBoolean(packet.ReadByte());

            var digiviceSlot = packet.ReadInt();
            var archiveSlot = packet.ReadInt() - 1000;
            var npcId = packet.ReadInt();

            var digivicePartner = client.Tamer.Digimons.FirstOrDefault(x => x.Slot == digiviceSlot);
            var archivePartner = client.Tamer.DigimonArchive.DigimonArchives.FirstOrDefault(x => x.Slot == archiveSlot);

            _logger.Verbose(
                "Digimon archive move request: tamer={TamerId} vip={Vip} digiviceSlot={DigiviceSlot} archiveSlot={ArchiveSlot} npc={NpcId} packetLen={PacketLength}.",
                client.TamerId, vipEnabled, digiviceSlot, archiveSlot, npcId, packetData.Length);

            if (archiveSlot < 0 || archiveSlot >= client.Tamer.DigimonArchive.Slots)
            {
                _logger.Warning(
                    "Character {TamerId} requested invalid digimon archive slot {ArchiveSlot}; opened slots={OpenedSlots}.",
                    client.TamerId, archiveSlot, client.Tamer.DigimonArchive.Slots);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            if (archivePartner == null)
            {
                archivePartner = new CharacterDigimonArchiveItemModel(archiveSlot);
                client.Tamer.DigimonArchive.DigimonArchives.Add(archivePartner);
                await _sender.Send(new CreateCharacterDigimonArchiveSlotCommand(
                    archivePartner,
                    client.Tamer.DigimonArchive.Id
                ));
            }

            var price = client.Tamer.DigimonArchive.ArchivePrice(digivicePartner?.Level);

            if (digivicePartner == null && archivePartner.DigimonId == 0)
            {
                _logger.Warning(
                    "Character {TamerId} tried to move empty digivice slot {DigiviceSlot} with empty archive slot {ArchiveSlot}.",
                    client.TamerId, digiviceSlot, archiveSlot);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            if (digivicePartner == null)
            {
                await MovePartnerToDigivice(client, digiviceSlot, archiveSlot, digivicePartner, archivePartner);
            }
            else if (archivePartner.DigimonId == 0)
            {
                await MovePartnerToArchive(client, digiviceSlot, archiveSlot, digivicePartner, archivePartner, price);
            }
            else
            {
                client.Tamer.RemoveDigimon((byte)digiviceSlot, false);
                archivePartner.AddDigimon(digivicePartner.Id);

                digivicePartner.SetSlot(byte.MaxValue);
                await _sender.Send(new UpdateDigimonSlotCommand(digivicePartner.Id, digivicePartner.Slot));

                archivePartner.Digimon!.SetSlot((byte)digiviceSlot);
                
                client.Tamer.AddDigimon(archivePartner.Digimon);
                
                client.Tamer.Inventory.RemoveBits(price);

                await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
                await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
                await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

                _logger.Verbose($"Character {client.Tamer} swapped partner {digivicePartner.Id}({digivicePartner.BaseType}) with partner " +
                    $"{archivePartner.Id}({archivePartner.Digimon!.BaseType}) on digivice slot {digiviceSlot} and archive slot {archiveSlot} for {price} bits.");
            }

            client.Send(new DigimonArchiveManagePacket(digiviceSlot, archiveSlot, price));
        }

        private async Task MovePartnerToDigivice(
            GameClient client,
            int digiviceSlot,
            int archiveSlot,
            DigimonModel? digivicePartner,
            CharacterDigimonArchiveItemModel archivePartner)
        {
            digivicePartner = _mapper.Map<DigimonModel>(
                await _sender.Send(
                    new GetDigimonByIdQuery(archivePartner.DigimonId)
                )
            );

            digivicePartner.SetBaseInfo(
                _statusManager.GetDigimonBaseInfo(
                    digivicePartner.BaseType
                )
            );

            digivicePartner.SetBaseStatus(
                _statusManager.GetDigimonBaseStatus(
                    digivicePartner.BaseType,
                    digivicePartner.Level,
                    digivicePartner.Size
                )
            );

            digivicePartner.SetSlot((byte)digiviceSlot);

            archivePartner.RemoveDigimon();

            client.Tamer.AddDigimon(digivicePartner);

            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

            _logger.Verbose($"Character {client.Tamer} moved partner {digivicePartner.Id}({digivicePartner.BaseType}) " +
                $"from archive slot {archiveSlot} to digivice slot {digiviceSlot}.");
        }

        private async Task MovePartnerToArchive(
            GameClient client,
            int digiviceSlot,
            int archiveSlot,
            DigimonModel? digivicePartner,
            CharacterDigimonArchiveItemModel archivePartner,
            int price)
        {
            archivePartner.AddDigimon(digivicePartner.Id);
            digivicePartner.SetSlot(byte.MaxValue);

            client.Tamer.Inventory.RemoveBits(price);

            await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

            _logger.Verbose($"Character {client.Tamer} moved partner {digivicePartner.Id}({digivicePartner.BaseType}) " +
                $"to digimon archive at slot {archiveSlot} for {price} bits.");

            client.Tamer.RemoveDigimon(byte.MaxValue);
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
        }
    }
}
