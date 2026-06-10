using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;

using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class MovimentationPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.TamerMovimentation;

        private readonly PartyManager _partyManager;
        private readonly PvpServer _pvpServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly MapServer _mapServer;
        private readonly ISender _sender;
        private readonly AssetsLoader _assets;
        private readonly MapBinLoader _mapBin;
        private readonly StatusManager _statusManager;
        private readonly DUnitCollectionService _dUnitCollections;
        private readonly ILogger _logger;

        public MovimentationPacketProcessor(
            PartyManager partyManager,
            PvpServer pvpServer,
            MapServer mapServer,
            ISender sender,
            DungeonsServer dungeonServer,
            AssetsLoader assets,
            MapBinLoader mapBin,
            StatusManager statusManager,
            DUnitCollectionService dUnitCollections,
            ILogger logger)
        {
            _partyManager = partyManager;
            _pvpServer = pvpServer;
            _mapServer = mapServer;
            _sender = sender;
            _dungeonServer = dungeonServer;
            _assets = assets;
            _mapBin = mapBin;
            _statusManager = statusManager;
            _dUnitCollections = dUnitCollections;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var ticks = packet.ReadInt();
            var handler = packet.ReadInt();
            var newX = packet.ReadInt();
            var newY = packet.ReadInt();
            var newZ = packet.ReadFloat();

            if (client.PvpMap)
            {
                if (client.Tamer.PreviousCondition == ConditionEnum.Ride && client.Tamer.CurrentCondition == ConditionEnum.Away)
                {
                    client.Tamer.ResetAfkNotifications();
                    client.Tamer.UpdateCurrentCondition(ConditionEnum.Ride);
                    _pvpServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                }

                if (client.Tamer.Riding)
                {
                    client.Tamer.NewLocation(newX, newY, newZ);
                    client.Tamer.Partner.NewLocation(newX, newY, newZ);

                    _pvpServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    _pvpServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                }
                else
                {
                    if (client.Tamer.CurrentCondition == ConditionEnum.Away)
                    {
                        client.Tamer.ResetAfkNotifications();
                        client.Tamer.UpdateCurrentCondition(ConditionEnum.Default);
                        _pvpServer.BroadcastForTargetTamers(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                    }

                    if (handler >= short.MaxValue)
                    {
                        client.Tamer.NewLocation(newX, newY, newZ);
                        _pvpServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    }
                    else
                    {
                        client.Tamer.Partner.NewLocation(newX, newY, newZ);
                        _pvpServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                    }
                }

                await TryApplyLimitEvolutionRegionEffects(client,
                    packet => _pvpServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet));
            }
            else if(client.DungeonMap)
            {
                if (client.Tamer.PreviousCondition == ConditionEnum.Ride && client.Tamer.CurrentCondition == ConditionEnum.Away)
                {
                    client.Tamer.ResetAfkNotifications();
                    client.Tamer.UpdateCurrentCondition(ConditionEnum.Ride);
                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                }

                if (client.Tamer.Riding)
                {
                    client.Tamer.NewLocation(newX, newY, newZ);
                    client.Tamer.Partner.NewLocation(newX, newY, newZ);

                    _dungeonServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    _dungeonServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                }
                else
                {
                    if (client.Tamer.CurrentCondition == ConditionEnum.Away)
                    {
                        client.Tamer.ResetAfkNotifications();
                        client.Tamer.UpdateCurrentCondition(ConditionEnum.Default);
                        _dungeonServer.BroadcastForTargetTamers(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                    }

                    if (handler >= short.MaxValue)
                    {
                        client.Tamer.NewLocation(newX, newY, newZ);
                        _dungeonServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    }
                    else
                    {
                        client.Tamer.Partner.NewLocation(newX, newY, newZ);
                        _dungeonServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                    }
                }

                await TryApplyLimitEvolutionRegionEffects(client,
                    packet => _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet));

                var party = _partyManager.FindParty(client.TamerId);
                if (party != null)
                {
                    party.UpdateMember(party[client.TamerId]);

                    _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                        new PartyMemberMovimentationPacket(party[client.TamerId]).Serialize());

                    _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                       new PartyMemberMovimentationPacket(party[client.TamerId]).Serialize());
                }

            }
            else
            {
                if (client.Tamer.PreviousCondition == ConditionEnum.Ride && client.Tamer.CurrentCondition == ConditionEnum.Away)
                {
                    client.Tamer.ResetAfkNotifications();
                    client.Tamer.UpdateCurrentCondition(ConditionEnum.Ride);
                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                }

                if (client.Tamer.Riding)
                {
                    client.Tamer.NewLocation(newX, newY, newZ);
                    client.Tamer.Partner.NewLocation(newX, newY, newZ);

                    _mapServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    _mapServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                }
                else
                {
                    if (client.Tamer.CurrentCondition == ConditionEnum.Away)
                    {
                        client.Tamer.ResetAfkNotifications();
                        client.Tamer.UpdateCurrentCondition(ConditionEnum.Default);
                        _mapServer.BroadcastForTargetTamers(client.TamerId, new SyncConditionPacket(client.Tamer.GeneralHandler, client.Tamer.CurrentCondition).Serialize());
                    }

                    if (handler >= short.MaxValue)
                    {
                        client.Tamer.NewLocation(newX, newY, newZ);
                        _mapServer.BroadcastForTargetTamers(client.TamerId, new TamerWalkPacket(client.Tamer).Serialize());
                    }
                    else
                    {
                        client.Tamer.Partner.NewLocation(newX, newY, newZ);
                        _mapServer.BroadcastForTargetTamers(client.TamerId, new DigimonWalkPacket(client.Tamer.Partner).Serialize());
                    }
                }

                await TryApplyLimitEvolutionRegionEffects(client,
                    packet => _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet));

                var party = _partyManager.FindParty(client.TamerId);
                if (party != null)
                {
                    party.UpdateMember(party[client.TamerId]);

                    _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                        new PartyMemberMovimentationPacket(party[client.TamerId]).Serialize());

                    _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                       new PartyMemberMovimentationPacket(party[client.TamerId]).Serialize());
                }

                await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));
                await _sender.Send(new UpdateDigimonLocationCommand(client.Partner.Location)); 
            }
            
        }

        private async Task TryApplyLimitEvolutionRegionEffects(GameClient client, Action<byte[]> broadcastForViewsAndSelf)
        {
            if (LimitEvolutionRegionRule.ShouldDevolveToBase(_assets, _mapBin, client))
                await DevolvePartnerToBaseByLimitEvolutionRegion(client, broadcastForViewsAndSelf);

            TryStopRideModeByLimitEvolutionRegion(client, broadcastForViewsAndSelf);
        }

        private async Task DevolvePartnerToBaseByLimitEvolutionRegion(GameClient client, Action<byte[]> broadcastForViewsAndSelf)
        {
            var previousType = client.Partner.CurrentType;

            client.Tamer.ActiveEvolution.SetDs(0);
            client.Tamer.ActiveEvolution.SetXg(0);

            var passiveBuffIdsToRemove = client.Tamer.Partner.BuffList.Buffs
                .Where(x => x.SkillId / 1000000 == 8 && x.Duration == 0)
                .Select(x => x.BuffId)
                .Distinct()
                .ToList();

            foreach (var buffIdToRemove in passiveBuffIdsToRemove)
                broadcastForViewsAndSelf(new RemoveBuffPacket(client.Partner.GeneralHandler, buffIdToRemove).Serialize());

            client.Tamer.RemovePartnerPassiveBuff();
            await _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));

            var currentHp = client.Partner.CurrentHp;
            var currentMaxHp = client.Partner.HP;
            var currentDs = client.Partner.CurrentDs;
            var currentMaxDs = client.Partner.DS;

            client.Partner.UpdateCurrentType(client.Partner.BaseType);

            broadcastForViewsAndSelf(
                new DigimonEvolutionSucessPacket(
                    client.Tamer.GeneralHandler,
                    client.Partner.GeneralHandler,
                    client.Partner.CurrentType,
                    (byte)DigimonEvolutionEffectEnum.Back).Serialize());

            client.Partner.SetBaseInfo(_statusManager.GetDigimonBaseInfo(client.Partner.CurrentType));
            client.Partner.SetBaseStatus(
                _statusManager.GetDigimonBaseStatus(
                    client.Partner.CurrentType,
                    client.Partner.Level,
                    client.Partner.Size));
            client.Partner.SetSealStatus(_assets.SealInfo);

            client.Tamer.SetPartnerPassiveBuff();
            client.Partner.AdjustHpAndDs(currentHp, currentMaxHp, currentDs, currentMaxDs);

            foreach (var buff in client.Tamer.Partner.BuffList.ActiveBuffs)
            {
                buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x =>
                    x.SkillCode == buff.SkillId && buff.BuffInfo == null ||
                    x.DigimonSkillCode == buff.SkillId && buff.BuffInfo == null));
            }

            var buffToApply = client.Tamer.Partner.BuffList.Buffs
                .Where(x => x.Duration == 0)
                .ToList();

            foreach (var buff in buffToApply)
            {
                broadcastForViewsAndSelf(
                    new AddBuffPacket(
                        client.Tamer.Partner.GeneralHandler,
                        buff.BuffId,
                        buff.SkillId,
                        (short)buff.TypeN,
                        0).Serialize());
            }

            var dUnitResult = _dUnitCollections.CalculateDUnitCollectionBonus(client.Tamer);
            _dUnitCollections.ApplyBonuses(client.Tamer, dUnitResult);
            client.Send(new UpdateStatusPacket(client.Tamer));
            client.Send(new XmlUnionCollectionInfoPacket(dUnitResult));

            var party = _partyManager.FindParty(client.TamerId);
            if (party != null)
            {
                party.UpdateMember(party[client.TamerId]);

                _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberInfoPacket(party[client.TamerId]).Serialize());

                _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberInfoPacket(party[client.TamerId]).Serialize());
            }

            await _sender.Send(new UpdatePartnerCurrentTypeCommand(client.Partner));
            await _sender.Send(new UpdateCharacterActiveEvolutionCommand(client.Tamer.ActiveEvolution));
            await _sender.Send(new UpdateCharacterBasicInfoCommand(client.Tamer));
            await _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));

            _logger.Information(
                "Partner auto-devolved by limit evolution region. Tamer {TamerId}, partner {PartnerId}, previousType {PreviousType}, baseType {BaseType}, map {MapId}, x {X}, y {Y}.",
                client.TamerId,
                client.Partner.Id,
                previousType,
                client.Partner.BaseType,
                client.Tamer.Location.MapId,
                client.Partner.Location.X,
                client.Partner.Location.Y);
        }

        private void TryStopRideModeByLimitEvolutionRegion(GameClient client, Action<byte[]> broadcastForViewsAndSelf)
        {
            if (!LimitEvolutionRegionRule.ShouldStopRideMode(_assets, _mapBin, client))
                return;

            client.Tamer.StopRideMode();
            broadcastForViewsAndSelf(new UpdateMovementSpeedPacket(client.Tamer).Serialize());
            broadcastForViewsAndSelf(new RideModeStopPacket(client.Tamer.GeneralHandler, client.Partner.GeneralHandler).Serialize());

            _logger.Information(
                "Ride mode stopped by limit evolution region. Tamer {TamerId}, partner {PartnerId}, currentType {CurrentType}, map {MapId}, x {X}, y {Y}.",
                client.TamerId,
                client.Partner.Id,
                client.Partner.CurrentType,
                client.Tamer.Location.MapId,
                client.Partner.Location.X,
                client.Partner.Location.Y);
        }
    }
}
