using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;
using System;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class PartnerSwitchPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartnerSwitch;

        private readonly PartyManager _partyManager;
        private readonly StatusManager _statusManager;
        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly EquipmentSetBonusService _equipmentSetBonusService;

        public PartnerSwitchPacketProcessor(
            PartyManager partyManager,
            StatusManager statusManager,
            AssetsLoader assets,
            MapServer mapServer,
            DungeonsServer dungeonServer,
            ILogger logger,
            ISender sender,
            EquipmentSetBonusService equipmentSetBonusService
        )
        {
            _partyManager = partyManager;
            _statusManager = statusManager;
            _assets = assets;
            _mapServer = mapServer;
            _dungeonServer = dungeonServer;
            _logger = logger;
            _sender = sender;
            _equipmentSetBonusService = equipmentSetBonusService;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            try
            {
                var packet = new GamePacketReader(packetData);

                var slot = packet.ReadByte();

                if (client.Partner == null)
                {
                    _logger.Warning("Partner switch rejected for tamer {TamerId}. No active partner. RequestedSlot {Slot}, PacketLength {PacketLength}.", client.TamerId, slot, packetData.Length);
                    client.Send(new PartnerSwitchFailurePacket());
                    return;
                }

                var previousPartner = client.Partner;
                var previousId = previousPartner.Id;
                var previousType = previousPartner.CurrentType;

                var newPartner = ResolvePartnerSwitchTarget(client, slot, out var targetRealSlot);

                if (slot == 0 || newPartner == null)
                {
                    _logger.Warning(
                        "Partner switch rejected for tamer {TamerId}. Invalid requested slot {Slot}. ActivePartner {PartnerId}, available slots {Slots}, PacketLength {PacketLength}.",
                        client.TamerId,
                        slot,
                        previousPartner.Id,
                        string.Join(",", client.Tamer.Digimons.Select(x => x.Slot)),
                        packetData.Length);
                    client.Send(new PartnerSwitchFailurePacket());
                    return;
                }

                if (targetRealSlot != slot)
                {
                    _logger.Information(
                        "Partner switch target slot resolved for tamer {TamerId}. requestedVisualSlot {VisualSlot}, realSlot {RealSlot}, target {TargetId}.",
                        client.TamerId,
                        slot,
                        targetRealSlot,
                        newPartner.Id);
                }

                _logger.Debug(
                    "Partner switch request tamer {TamerId}. Active {PreviousId} base {PreviousBaseType} current {PreviousCurrentType} slot {PreviousSlot}; target {TargetId} base {TargetBaseType} current {TargetCurrentType} realSlot {TargetRealSlot} visualSlot {TargetVisualSlot}; packetLength {PacketLength}.",
                    client.TamerId,
                    previousPartner.Id,
                    previousPartner.BaseType,
                    previousPartner.CurrentType,
                    previousPartner.Slot,
                    newPartner.Id,
                    newPartner.BaseType,
                    newPartner.CurrentType,
                    targetRealSlot,
                    slot,
                    packetData.Length);

                client.Tamer.RemovePartnerPassiveBuff();
                await _sender.Send(new UpdateDigimonBuffListCommand(previousPartner.BuffList));

                if (previousPartner.CurrentType != previousPartner.BaseType)
                {
                    _logger.Debug(
                        "Resetting switched-out partner {PartnerId} from current type {CurrentType} to base type {BaseType} before partner switch.",
                        previousPartner.Id,
                        previousPartner.CurrentType,
                        previousPartner.BaseType);
                    previousPartner.UpdateCurrentType(previousPartner.BaseType);
                }

                if (client.DungeonMap)
                {
                    _dungeonServer.SwapDigimonHandlers(client.Tamer.Location.MapId, previousPartner, newPartner);
                }
                else
                {
                    _mapServer.SwapDigimonHandlers(client.Tamer.Location.MapId, previousPartner, newPartner, client.TamerId);
                }

                client.Tamer.SwitchPartner(targetRealSlot);
                client.Partner.UpdateCurrentType(client.Partner.BaseType);
                client.Partner.SetTamer(client.Tamer);
                client.Partner.NewLocation(client.Tamer.Location.MapId, client.Tamer.Location.X, client.Tamer.Location.Y);

                client.Tamer.Partner.SetBaseInfo(
                    _statusManager.GetDigimonBaseInfo(
                        client.Tamer.Partner.CurrentType
                    )
                );

                client.Tamer.Partner.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        client.Tamer.Partner.CurrentType,
                        client.Tamer.Partner.Level,
                        client.Tamer.Partner.Size
                    )
                );

                client.Partner.SetSealStatus(_assets.SealInfo);

                if (client.Tamer.InBattle)
                {
                    var battleTagItem = client.Tamer.Inventory.FindItemBySection(16400);

                    if (client.Tamer.Inventory.RemoveOrReduceItem(battleTagItem, 1))
                    {
                        _logger.Verbose($"Character {client.TamerId} consumed battle tag {battleTagItem.ItemId}.");
                        client.Send(new PartnerSwitchInBattlePacket(slot, client.Tamer.Model.GetHashCode()));
                    }
                }

                client.Tamer.SetPartnerPassiveBuff();
                _equipmentSetBonusService.SyncPartnerPassiveBuffs(client);

                foreach (var buff in client.Tamer.Partner.BuffList.ActiveBuffs)
                    buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x => x.SkillCode == buff.SkillId && buff.BuffInfo == null || x.DigimonSkillCode == buff.SkillId && buff.BuffInfo == null));

                var partnerSwitchPacket = new PartnerSwitchPacket(client.Tamer.GenericHandler, previousType, client.Partner, slot).Serialize();
                _logger.Information(
                    "Partner switch send: tamer {TamerId} uid {Handler} prevType {PreviousType} slot {Slot} target {PartnerId} name {PartnerName} base {BaseType} level {Level} size {Size} buffs {BuffCount} bytes {Bytes}.",
                    client.TamerId,
                    client.Tamer.GenericHandler,
                    previousType,
                    slot,
                    client.Partner.Id,
                    client.Partner.Name,
                    client.Partner.BaseType,
                    client.Partner.Level,
                    client.Partner.Size,
                    client.Partner.BuffList.ActiveBuffs.Count,
                    partnerSwitchPacket.Length);

                if (client.DungeonMap)
                {
                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, partnerSwitchPacket);
                }
                else
                {
                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, partnerSwitchPacket);
                }

                if (client.Tamer.Partner.BuffList.Buffs.Any())
                {

                    var buffToApply = client.Tamer.Partner.BuffList.Buffs;


                    buffToApply.ForEach(buffToApply =>
                    {
                        var Ts = 0;

                        if (buffToApply.Duration != 0)
                            Ts = Math.Max(1, buffToApply.RemainingSeconds);
                        else
                            Ts = unchecked((int)uint.MaxValue);

                        if (client.DungeonMap)
                        {
                            _dungeonServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffInfo, (short)buffToApply.TypeN, Ts).Serialize());
                        }
                        else
                        {
                            _mapServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffInfo, (short)buffToApply.TypeN, Ts).Serialize());
                        }

                    });

                }


                client.Send(new UpdateStatusPacket(client.Tamer));

                if (client.Tamer.HasXai)
                {
                    client.Send(new XaiInfoPacket(client.Tamer.Xai));
                    client.Send(new TamerXaiResourcesPacket(client.Tamer.XGauge,client.Tamer.XCrystals));
                }

                var party = _partyManager.FindParty(client.TamerId);

                if (party != null)
                {
                    party.UpdateMember(party[client.TamerId]);

                    _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                  new PartyMemberPartnerSwitchPacket(party[client.TamerId]).Serialize());

                    _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberPartnerSwitchPacket(party[client.TamerId]).Serialize());

                }

                await _sender.Send(new UpdatePartnerCurrentTypeCommand(previousPartner));
                await _sender.Send(new UpdatePartnerCurrentTypeCommand(client.Partner));
                await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
                await _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));

                _logger.Verbose($"Character {client.TamerId} switched partner {previousId}({previousType}) with {client.Partner.Id}({client.Partner.BaseType}).");
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Unhandled PartnerSwitch packet for tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, packetLength {PacketLength}.",
                    client.TamerId,
                    client.Partner?.Id,
                    client.Partner?.BaseType,
                    client.Partner?.CurrentType,
                    packetData.Length);
                client.Send(new PartnerSwitchFailurePacket());
            }
        }

        private static DigitalWorldOnline.Commons.Models.Digimon.DigimonModel? ResolvePartnerSwitchTarget(
            GameClient client,
            byte requestedVisualSlot,
            out byte targetRealSlot)
        {
            targetRealSlot = requestedVisualSlot;

            if (requestedVisualSlot == 0)
            {
                return null;
            }

            // Client sends 1..8 from the visible mercenary row. Database slots can
            // have gaps, so position 3 may point to real slot 5, for example.
            var visualPartner = client.Tamer.Digimons
                .Where(x => x.Slot != byte.MaxValue)
                .OrderBy(x => x.Slot)
                .ElementAtOrDefault(requestedVisualSlot);

            if (visualPartner != null)
            {
                targetRealSlot = visualPartner.Slot;
                return visualPartner;
            }

            return client.Tamer.Digimons.FirstOrDefault(x => x.Slot == requestedVisualSlot);
        }
    }
}
