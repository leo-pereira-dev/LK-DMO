using AutoMapper;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using MediatR;
using Serilog;

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

            var clientDigiviceSlot = packet.ReadInt();
            var archiveSlot = packet.ReadInt() - 1000;
            var npcId = packet.ReadInt();

            var digivicePartner = ClientDigimonSlotResolver.Resolve(client, clientDigiviceSlot, out var resolvedDigiviceSlot);

            var archivePartner = client.Tamer.DigimonArchive.DigimonArchives.FirstOrDefault(x => x.Slot == archiveSlot);

            _logger.Information(
                "ARCHIVE 3201 request tamer={TamerId} vip={Vip} clientDigiviceSlot={ClientDigiviceSlot} resolvedDigiviceSlot={ResolvedDigiviceSlot} slotContract=0-active/1-N-mercenary archiveSlot={ArchiveSlot} npc={NpcId} packetLen={PacketLength} digivicePartner={DigivicePartnerId}:{DigivicePartnerName}:{DigivicePartnerSlot} archiveItem={ArchiveItemId}:{ArchiveDigimonId}:{ArchiveDigimonName} tamerSlots=[{TamerSlots}]",
                client.TamerId,
                vipEnabled,
                clientDigiviceSlot,
                resolvedDigiviceSlot,
                archiveSlot,
                npcId,
                packetData.Length,
                digivicePartner?.Id,
                digivicePartner?.Name,
                digivicePartner?.Slot,
                archivePartner?.Id,
                archivePartner?.DigimonId,
                archivePartner?.Digimon?.Name,
                string.Join(",", client.Tamer.Digimons.Select(x => $"{x.Id}:{x.Slot}:{x.BaseType}:{x.Name}")));

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
                _logger.Warning(
                    "Character {TamerId} requested opened digimon archive slot {ArchiveSlot}, but no archive item row exists. opened slots={OpenedSlots}; creating missing row.",
                    client.TamerId, archiveSlot, client.Tamer.DigimonArchive.Slots);
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
                    "Character {TamerId} tried to move empty digivice slot client={ClientDigiviceSlot} resolved={ResolvedDigiviceSlot} with empty archive slot {ArchiveSlot}.",
                    client.TamerId, clientDigiviceSlot, resolvedDigiviceSlot, archiveSlot);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            if (digivicePartner != null &&
                resolvedDigiviceSlot == 0 &&
                archivePartner.DigimonId == 0 &&
                client.Tamer.Digimons.Count(x => x.Slot != byte.MaxValue && x.Slot < client.Tamer.DigimonSlots) <= 1)
            {
                _logger.Warning(
                    "ARCHIVE 3201 rejected last active partner move. tamer={TamerId} digimon={DigimonId}:{DigimonName} clientDigiviceSlot={ClientDigiviceSlot} resolvedDigiviceSlot={ResolvedDigiviceSlot} archiveSlot={ArchiveSlot}",
                    client.TamerId,
                    digivicePartner.Id,
                    digivicePartner.Name,
                    clientDigiviceSlot,
                    resolvedDigiviceSlot,
                    archiveSlot);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return;
            }

            var responseClientDigiviceSlot = clientDigiviceSlot;

            if (digivicePartner == null)
            {
                resolvedDigiviceSlot = await MovePartnerToDigivice(client, archiveSlot, archivePartner);
                if (resolvedDigiviceSlot < 0)
                    return;

                responseClientDigiviceSlot = resolvedDigiviceSlot;
            }
            else if (archivePartner.DigimonId == 0)
            {
                await MovePartnerToArchive(client, resolvedDigiviceSlot, archiveSlot, digivicePartner, archivePartner, price);
            }
            else
            {
                var archivedDigimon = await EnsureArchiveDigimonLoaded(archivePartner);
                if (archivedDigimon == null)
                {
                    _logger.Warning(
                        "ARCHIVE 3201 swap failed because archive digimon could not be loaded. tamer={TamerId} archiveSlot={ArchiveSlot} archiveDigimonId={ArchiveDigimonId}",
                        client.TamerId,
                        archiveSlot,
                        archivePartner.DigimonId);
                    client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                    return;
                }

                client.Tamer.RemoveDigimon((byte)resolvedDigiviceSlot, false);
                archivePartner.AddDigimon(digivicePartner);

                digivicePartner.SetSlot(byte.MaxValue);
                await _sender.Send(new UpdateDigimonSlotCommand(digivicePartner.Id, digivicePartner.Slot));

                archivedDigimon.SetSlot((byte)resolvedDigiviceSlot);
                
                client.Tamer.AddDigimon(archivedDigimon);
                
                client.Tamer.Inventory.RemoveBits(price);

                await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
                await _sender.Send(new UpdateDigimonSlotCommand(archivedDigimon.Id, archivedDigimon.Slot));
                await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

                _logger.Information(
                    "ARCHIVE 3201 swapped tamer={TamerId} outgoing={OutgoingId}:{OutgoingName} incoming={IncomingId}:{IncomingName} digiviceSlot={DigiviceSlot} archiveSlot={ArchiveSlot} price={Price}",
                    client.TamerId,
                    digivicePartner.Id,
                    digivicePartner.Name,
                    archivedDigimon.Id,
                    archivedDigimon.Name,
                    resolvedDigiviceSlot,
                    archiveSlot,
                    price);
            }

            client.Send(UtilitiesFunctions.GroupPackets(
                new DigimonArchiveManagePacket(responseClientDigiviceSlot, archiveSlot, price).Serialize(),
                new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive).Serialize()));
        }

        private async Task<int> MovePartnerToDigivice(
            GameClient client,
            int archiveSlot,
            CharacterDigimonArchiveItemModel archivePartner)
        {
            var digivicePartner = await EnsureArchiveDigimonLoaded(archivePartner);
            if (digivicePartner == null)
            {
                _logger.Warning(
                    "ARCHIVE 3201 move to digivice failed because archive digimon could not be loaded. tamer={TamerId} archiveSlot={ArchiveSlot} archiveDigimonId={ArchiveDigimonId}",
                    client.TamerId,
                    archiveSlot,
                    archivePartner.DigimonId);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return -1;
            }

            client.Tamer.CompactActiveDigimonSlots();
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));

            var digiviceSlot = client.Tamer.GetNextActiveDigimonSlot();
            if (digiviceSlot >= client.Tamer.DigimonSlots)
            {
                _logger.Warning(
                    "ARCHIVE 3201 move to digivice failed because tamer has no available active slot. tamer={TamerId} archiveSlot={ArchiveSlot} openedSlots={OpenedSlots} activeSlots={ActiveSlots}",
                    client.TamerId,
                    archiveSlot,
                    client.Tamer.DigimonSlots,
                    digiviceSlot);
                client.Send(new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive));
                return -1;
            }

            digivicePartner.SetSlot((byte)digiviceSlot);

            archivePartner.RemoveDigimon();

            client.Tamer.AddDigimon(digivicePartner);

            await _sender.Send(new UpdateDigimonSlotCommand(digivicePartner.Id, digivicePartner.Slot));
            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

            _logger.Information(
                "ARCHIVE 3201 moved to digivice tamer={TamerId} digimon={DigimonId}:{DigimonName} archiveSlot={ArchiveSlot} digiviceSlot={DigiviceSlot}",
                client.TamerId,
                digivicePartner.Id,
                digivicePartner.Name,
                archiveSlot,
                digiviceSlot);

            return digiviceSlot;
        }

        private async Task MovePartnerToArchive(
            GameClient client,
            int digiviceSlot,
            int archiveSlot,
            DigimonModel? digivicePartner,
            CharacterDigimonArchiveItemModel archivePartner,
            int price)
        {
            archivePartner.AddDigimon(digivicePartner);
            client.Tamer.RemoveDigimon((byte)digiviceSlot);
            digivicePartner.SetSlot(byte.MaxValue);

            client.Tamer.Inventory.RemoveBits(price);

            await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
            await _sender.Send(new UpdateDigimonSlotCommand(digivicePartner.Id, digivicePartner.Slot));
            await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(archivePartner));

            _logger.Information(
                "ARCHIVE 3201 moved to archive tamer={TamerId} digimon={DigimonId}:{DigimonName} digiviceSlot={DigiviceSlot} archiveSlot={ArchiveSlot} price={Price}",
                client.TamerId,
                digivicePartner.Id,
                digivicePartner.Name,
                digiviceSlot,
                archiveSlot,
                price);
        }

        private async Task<DigimonModel?> EnsureArchiveDigimonLoaded(CharacterDigimonArchiveItemModel archivePartner)
        {
            if (archivePartner.Digimon != null && archivePartner.Digimon.Id == archivePartner.DigimonId)
                return archivePartner.Digimon;

            if (archivePartner.DigimonId <= 0)
                return null;

            var digimon = _mapper.Map<DigimonModel>(
                await _sender.Send(new GetDigimonByIdQuery(archivePartner.DigimonId)));

            if (digimon == null)
                return null;

            digimon.SetBaseInfo(_statusManager.GetDigimonBaseInfo(digimon.BaseType));
            digimon.SetBaseStatus(_statusManager.GetDigimonBaseStatus(
                digimon.BaseType,
                digimon.Level,
                digimon.Size));

            archivePartner.SetDigimonInfo(digimon);

            return digimon;
        }
    }
}
