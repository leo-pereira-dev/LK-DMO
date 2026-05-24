using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;
using System;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class PartnerEvolutionPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartnerEvolution;
        private const byte DevolutionStage = 8;

        private readonly PartyManager _partyManager;
        private readonly StatusManager _statusManager;
        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly MapBinLoader _mapBin;
        private readonly DUnitCollectionService _dUnitCollections;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public PartnerEvolutionPacketProcessor(
            PartyManager partyManager,
            StatusManager statusManager,
            AssetsLoader assets,
            MapBinLoader mapBin,
            DUnitCollectionService dUnitCollections,
            MapServer mapServer,
            ISender sender,
            ILogger logger,
            DungeonsServer dungeonServer)
        {
            _partyManager = partyManager;
            _statusManager = statusManager;
            _assets = assets;
            _mapBin = mapBin;
            _dUnitCollections = dUnitCollections;
            _mapServer = mapServer;
            _sender = sender;
            _logger = logger;
            _dungeonServer = dungeonServer;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            try
            {
            var packet = new GamePacketReader(packetData);

            if (client.Partner == null)
            {
                _logger.Warning("Evolution request ignored because tamer {TamerId} has no active partner. PacketLength {PacketLength}.", client.TamerId, packetData.Length);
                client.Send(new DigimonEvolutionFailPacket());
                return;
            }

            var digimonHandle = packet.ReadInt();
            var evoStage = packet.ReadByte();
            var isDevolveToBase = evoStage == DevolutionStage;

            var evolutionTree = _assets.EvolutionInfo.FirstOrDefault(x => x.Type == client.Partner.BaseType);
            var evoInfo = evolutionTree?
                .Lines.FirstOrDefault(x => x.Type == client.Partner.CurrentType)
                ?? evolutionTree?.Lines.FirstOrDefault(x => x.Type == client.Partner.BaseType)
                ?? evolutionTree?.Lines.FirstOrDefault();
            var evoLine = evoInfo?.Stages;

            if (!isDevolveToBase && (evoInfo == null || evoLine == null || !evoLine.Any()))
            {
                _logger.Warning(
                    "Evolution request rejected for tamer {TamerId}, partner {PartnerId}. Missing evolution line. BaseType {BaseType}, CurrentType {CurrentType}, Stage {Stage}, Handle {Handle}, PacketLength {PacketLength}.",
                    client.TamerId,
                    client.Partner.Id,
                    client.Partner.BaseType,
                    client.Partner.CurrentType,
                    evoStage,
                    digimonHandle,
                    packetData.Length);
                client.Send(new DigimonEvolutionFailPacket());
                return;
            }

            int targetType;

            if (isDevolveToBase)
            {
                targetType = client.Partner.BaseType;
            }
            else
            {
                if (evoStage >= evoLine.Count)
                {
                    _logger.Warning(
                        "Evolution request rejected for tamer {TamerId}, partner {PartnerId}. Stage {Stage} is outside line count {LineCount}. BaseType {BaseType}, CurrentType {CurrentType}, Handle {Handle}.",
                        client.TamerId,
                        client.Partner.Id,
                        evoStage,
                        evoLine.Count,
                        client.Partner.BaseType,
                        client.Partner.CurrentType,
                        digimonHandle);
                    client.Send(new DigimonEvolutionFailPacket());
                    return;
                }

                targetType = evoLine[evoStage].Type;

                if (targetType <= 0)
                {
                    _logger.Warning(
                        "Evolution request rejected for tamer {TamerId}, partner {PartnerId}. Empty target type at stage {Stage}. BaseType {BaseType}, CurrentType {CurrentType}, Handle {Handle}.",
                        client.TamerId,
                        client.Partner.Id,
                        evoStage,
                        client.Partner.BaseType,
                        client.Partner.CurrentType,
                        digimonHandle);
                    client.Send(new DigimonEvolutionFailPacket());
                    return;
                }

                var targetRank = (EvolutionRankEnum)(_assets.DigimonBaseInfo.FirstOrDefault(x => x.Type == targetType)?.EvolutionType ?? 0);
                var isSameBaseEvolution = targetType == client.Partner.BaseType;
                var isCapsuleEvo = targetRank == EvolutionRankEnum.Capsule;

                if (!isSameBaseEvolution && !isCapsuleEvo && IsInsideLimitEvolutionRegion(client))
                {
                    client.Send(new DigimonEvolutionFailPacket());
                    return;
                }
            }

            var targetEvoInfo = !isDevolveToBase
                ? evolutionTree?.Lines.FirstOrDefault(x => x.Type == targetType)
                : null;

            _logger.Debug(
                "Evolution request tamer {TamerId}, partner {PartnerId}, handle {Handle}, base {BaseType}, current {CurrentType}, stage {Stage}, target {TargetType}, devolve {Devolve}, packetLength {PacketLength}.",
                client.TamerId,
                client.Partner.Id,
                digimonHandle,
                client.Partner.BaseType,
                client.Partner.CurrentType,
                evoStage,
                targetType,
                isDevolveToBase,
                packetData.Length);

            if (targetType != client.Partner.BaseType)
            {
                var targetEvo = client.Partner.Evolutions.FirstOrDefault(x => x.Type == targetType);

                if (targetEvo == null || targetEvo.Unlocked == 0)
                {
                    _logger.Warning($"Character {client.TamerId} tryied to evolve {client.Partner.Id} into {targetType} without unlocking the evo.");
                    client.Send(new DigimonEvolutionFailPacket());
                    return;
                }
            }

            if (!HasRequiredEvolutionUseItem(client, targetEvoInfo, targetType, evoStage))
                return;


            var buffToRemove = client.Tamer.Partner.BuffList.TamerBaseSkill();

            if (buffToRemove != null)
            {
                if (client.DungeonMap)
                {
                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new RemoveBuffPacket(client.Partner.GeneralHandler, buffToRemove.BuffId).Serialize());
                }
                else
                {
                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new RemoveBuffPacket(client.Partner.GeneralHandler, buffToRemove.BuffId).Serialize());

                }
            }

            client.Tamer.RemovePartnerPassiveBuff();
            await _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));

            DigimonEvolutionEffectEnum evoEffect;

            if (evoStage == 8)
            {
                evoEffect = DigimonEvolutionEffectEnum.Back;
                client.Tamer.ActiveEvolution.SetDs(0);
                client.Tamer.ActiveEvolution.SetXg(0);
            }
            else
            {
                var targetBaseInfo = _assets.DigimonBaseInfo.FirstOrDefault(x => x.Type == targetType);

                if (targetBaseInfo == null)
                {
                    _logger.Warning(
                        "Evolution request rejected for tamer {TamerId}, partner {PartnerId}. Missing DigimonBaseInfo for target type {TargetType}. Stage {Stage}, base {BaseType}, current {CurrentType}.",
                        client.TamerId,
                        client.Partner.Id,
                        targetType,
                        evoStage,
                        client.Partner.BaseType,
                        client.Partner.CurrentType);
                    client.Send(new DigimonEvolutionFailPacket());
                    return;
                }

                var evolutionType = targetBaseInfo.EvolutionType;
                switch ((EvolutionRankEnum)evolutionType)
                {
                    case EvolutionRankEnum.Rookie:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;
                            client.Tamer.ActiveEvolution.SetDs(0);
                            client.Tamer.ActiveEvolution.SetXg(0);
                        }
                        break;

                    case EvolutionRankEnum.Champion:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(20))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ActiveEvolution.SetDs(8);
                        }
                        break;

                    case EvolutionRankEnum.Ultimate:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(50))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ActiveEvolution.SetDs(10);
                        }
                        break;

                    case EvolutionRankEnum.Mega:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(152))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ActiveEvolution.SetDs(12);
                        }
                        break;

                    case EvolutionRankEnum.BurstMode:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.BurstMode;

                            var accelerator = client.Tamer.Inventory.FindItemById(9400);

                            if (accelerator == null)
                                accelerator = client.Tamer.Inventory.FindItemById(41002);
                            if (evoInfo.RequiredItem > 0)
                            {
                            if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(148)
                                || !client.Tamer.Inventory.RemoveOrReduceItem(accelerator, 3))
                                {
                                    client.Send(new DigimonEvolutionFailPacket());
                                    return;
                                }
                            }
                            else
                            {
                                if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(148))
                                {
                                    client.Send(new DigimonEvolutionFailPacket());
                                    return;
                                }
                            }

                            client.Tamer.ActiveEvolution.SetDs(40);
                            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                        }
                        break;

                    case EvolutionRankEnum.Jogress:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (!HasRequiredJogressChipsetEquipped(client, evoInfo.RequiredItem))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            if (evoInfo.RequiredItem > 0)
                            {
                                var accelerator = client.Tamer.Inventory.FindItemById(evoInfo.RequiredItem);

                                if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(180)
                                    || !client.Tamer.Inventory.RemoveOrReduceItem(accelerator, 1))
                                {
                                    client.Send(new DigimonEvolutionFailPacket());
                                    return;
                                }
                            }
                            else
                            {
                                if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(180))
                                {
                                    client.Send(new DigimonEvolutionFailPacket());
                                    return;
                                }

                            }

                            client.Tamer.ActiveEvolution.SetDs(80);
                        }
                        break;

                    case EvolutionRankEnum.RookieX:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (client.Partner.Level < evoInfo.UnlockLevel)
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ConsumeXg(68);
                            client.Tamer.ActiveEvolution.SetXg(1);
                        }
                        break;

                    case EvolutionRankEnum.ChampionX:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;

                            if (client.Partner.Level < evoInfo.UnlockLevel)
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ConsumeXg(92);
                            client.Tamer.ActiveEvolution.SetXg(1);
                        }
                        break;

                    case EvolutionRankEnum.UltimateX:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;
                           
                            if (client.Partner.Level < evoInfo.UnlockLevel)
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }
                            client.Tamer.ConsumeXg(130);

                            client.Tamer.ActiveEvolution.SetXg(1);
                        }
                        break;

                    case EvolutionRankEnum.MegaX:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Default;
                            
                          
                            if (client.Partner.Level < evoInfo.UnlockLevel)
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ConsumeXg(174);

                            client.Tamer.ActiveEvolution.SetXg(1);
                        }
                        break;

                    case EvolutionRankEnum.Capsule:
                        {
                            evoEffect = DigimonEvolutionEffectEnum.Unknown;

                            if (client.Partner.Level < evoInfo.UnlockLevel || !client.Tamer.ConsumeDs(75))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ActiveEvolution.SetDs(3);

                        }
                        break;

                    case EvolutionRankEnum.JogressX:
                    case EvolutionRankEnum.BurstModeX:
                        {
                            //TODO: Teste BM X (custo e efeito)
                            evoEffect = DigimonEvolutionEffectEnum.BurstMode;

                            if ((EvolutionRankEnum)evolutionType == EvolutionRankEnum.JogressX &&
                                !HasRequiredJogressChipsetEquipped(client, evoInfo.RequiredItem))
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            if (client.Partner.Level < evoInfo.UnlockLevel)
                            {
                                client.Send(new DigimonEvolutionFailPacket());
                                return;
                            }

                            client.Tamer.ConsumeXg(280);
                            client.Tamer.ActiveEvolution.SetXg(1);
                        }
                        break;

                    default:
                        {
                            client.Send(new DigimonEvolutionFailPacket());
                            return;
                        }
                }

                if (client.Tamer.HasXai)
                {

                    client.Send(new XaiInfoPacket(client.Tamer.Xai));
                    client.Send(new TamerXaiResourcesPacket(client.Tamer.XGauge, client.Tamer.XCrystals));
                }
            }

            if (!ConsumeRequiredEvolutionUseItem(client, targetEvoInfo, targetType, evoStage))
                return;

            if (evoStage == 8)
                _logger.Verbose($"Character {client.TamerId} devolved partner {client.Partner.Id} from {client.Partner.CurrentType} to {targetType}.");
            else
                _logger.Verbose($"Character {client.TamerId} evolved partner {client.Partner.Id} from {client.Partner.CurrentType} to {targetType}.");

            client.Partner.UpdateCurrentType(targetType);


            if (client.DungeonMap)
            {
                if (client.Tamer.Riding)
                {
                    client.Tamer.StopRideMode();

                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                        new UpdateMovementSpeedPacket(client.Tamer).Serialize());

                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                        new RideModeStopPacket(client.Tamer.GeneralHandler, client.Partner.GeneralHandler).Serialize());
                }

                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                    new DigimonEvolutionSucessPacket(
                        client.Tamer.GeneralHandler,
                        client.Partner.GeneralHandler,
                        client.Partner.CurrentType,
                        evoStage
                    ).Serialize()
                );
            }
            else
            {
                if (client.Tamer.Riding)
                {
                    client.Tamer.StopRideMode();

                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                        new UpdateMovementSpeedPacket(client.Tamer).Serialize());

                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                        new RideModeStopPacket(client.Tamer.GeneralHandler, client.Partner.GeneralHandler).Serialize());
                }

                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId,
                    new DigimonEvolutionSucessPacket(
                        client.Tamer.GeneralHandler,
                        client.Partner.GeneralHandler,
                        client.Partner.CurrentType,
                        evoStage
                    ).Serialize()
                );
            }
            UpdateSkillCooldown(client);



            var currentHp = client.Partner.CurrentHp;
            var currentMaxHp = client.Partner.HP;
            var currentDs = client.Partner.CurrentDs;
            var currentMaxDs = client.Partner.DS;

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
            client.Tamer.SetPartnerPassiveBuff();

            if (evoStage != 8)
                client.Partner.FullHeal();
            else
                client.Partner.AdjustHpAndDs(currentHp, currentMaxHp, currentDs, currentMaxDs);


            var currentTitleBuff = _assets.AchievementAssets.FirstOrDefault(x => x.QuestId == client.Tamer.CurrentTitle && x.BuffId > 0);

            if (currentTitleBuff != null)
            {
                foreach (var buff in client.Tamer.Partner.BuffList.ActiveBuffs.Where(x => x.BuffId != currentTitleBuff.BuffId))
                    buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x => x.SkillCode == buff.SkillId && buff.BuffInfo == null || x.DigimonSkillCode == buff.SkillId && buff.BuffInfo == null));

                if (client.Tamer.Partner.BuffList.TamerBaseSkill() != null)
                {
                    var buffToApply = client.Tamer.Partner.BuffList.Buffs
                                .Where(x => x.Duration == 0 && x.BuffId != currentTitleBuff.BuffId)
                                .ToList();


                    buffToApply.ForEach(buffToApply =>
                    {
                        if (client.DungeonMap)
                        {
                            _dungeonServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffId, buffToApply.SkillId, (short)buffToApply.TypeN, 0).Serialize());
                        }
                        else
                        {
                            _mapServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffId, buffToApply.SkillId, (short)buffToApply.TypeN, 0).Serialize());
                        }
                    });

                }
            }
            else
            {
                foreach (var buff in client.Tamer.Partner.BuffList.ActiveBuffs)
                    buff.SetBuffInfo(_assets.BuffInfo.FirstOrDefault(x => x.SkillCode == buff.SkillId && buff.BuffInfo == null || x.DigimonSkillCode == buff.SkillId && buff.BuffInfo == null));

                if (client.Tamer.Partner.BuffList.TamerBaseSkill() != null)
                {
                    var buffToApply = client.Tamer.Partner.BuffList.Buffs
                                .Where(x => x.Duration == 0)
                                .ToList();


                    buffToApply.ForEach(buffToApply =>
                    {
                        if (client.DungeonMap)
                        {
                            _dungeonServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffId, buffToApply.SkillId, (short)buffToApply.TypeN, 0).Serialize());
                        }
                        else
                        {
                            _mapServer.BroadcastForTamerViewsAndSelf(client.Tamer.Id, new AddBuffPacket(client.Tamer.Partner.GeneralHandler, buffToApply.BuffId, buffToApply.SkillId, (short)buffToApply.TypeN, 0).Serialize());

                        }
                    });


                }
            }

            var dUnitResult = _dUnitCollections.CalculateDUnitCollectionBonus(client.Tamer);
            _dUnitCollections.ApplyBonuses(client.Tamer, dUnitResult);

            client.Send(new UpdateStatusPacket(client.Tamer));
            client.Send(new XmlUnionCollectionInfoPacket(dUnitResult));


            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

            var party = _partyManager.FindParty(client.TamerId);

            if (party != null)
            {
                party.UpdateMember(party[client.TamerId]);

                _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberInfoPacket(party[client.TamerId]).Serialize());

                _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                  new PartyMemberInfoPacket(party[client.TamerId]).Serialize());
            }


            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdatePartnerCurrentTypeCommand(client.Partner));
            await _sender.Send(new UpdateCharacterActiveEvolutionCommand(client.Tamer.ActiveEvolution));
            await _sender.Send(new UpdateCharacterBasicInfoCommand(client.Tamer));
            await _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Unhandled PartnerEvolution packet for tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, packetLength {PacketLength}.",
                    client.TamerId,
                    client.Partner?.Id,
                    client.Partner?.BaseType,
                    client.Partner?.CurrentType,
                    packetData.Length);
                client.Send(new DigimonEvolutionFailPacket());
            }
        }

        private static bool HasRequiredJogressChipsetEquipped(GameClient client, int requiredItemId)
        {
            if (requiredItemId <= 0)
                return true;

            var equippedChipset = client.Tamer.JogressChipSet.FindItemBySlot(0);
            return equippedChipset is not null && equippedChipset.ItemId == requiredItemId && equippedChipset.Amount > 0;
        }

        private bool HasRequiredEvolutionUseItem(GameClient client, EvolutionLineAssetModel? targetEvoInfo, int targetType, byte evoStage)
        {
            if (!TryGetRequiredEvolutionUseItem(targetEvoInfo, out var itemSection, out var requiredAmount))
                return true;

            var availableAmount = client.Tamer.Inventory.FindItemsBySection(itemSection).Sum(x => x.Amount);

            _logger.Debug(
                "Evolution item check tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, target {TargetType}, stage {Stage}, itemSection {ItemSection}, required {RequiredAmount}, available {AvailableAmount}.",
                client.TamerId,
                client.Partner.Id,
                client.Partner.BaseType,
                client.Partner.CurrentType,
                targetType,
                evoStage,
                itemSection,
                requiredAmount,
                availableAmount);

            if (availableAmount >= requiredAmount)
                return true;

            _logger.Warning(
                "Evolution request rejected because required use item is missing. Tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, target {TargetType}, stage {Stage}, itemSection {ItemSection}, required {RequiredAmount}, available {AvailableAmount}.",
                client.TamerId,
                client.Partner.Id,
                client.Partner.BaseType,
                client.Partner.CurrentType,
                targetType,
                evoStage,
                itemSection,
                requiredAmount,
                availableAmount);

            client.Send(new DigimonEvolutionFailPacket());
            return false;
        }

        private bool ConsumeRequiredEvolutionUseItem(GameClient client, EvolutionLineAssetModel? targetEvoInfo, int targetType, byte evoStage)
        {
            if (!TryGetRequiredEvolutionUseItem(targetEvoInfo, out var itemSection, out var requiredAmount))
                return true;

            if (client.Tamer.Inventory.RemoveOrReduceItemsBySection(itemSection, requiredAmount))
            {
                _logger.Information(
                    "Evolution consumed required use item. Tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, target {TargetType}, stage {Stage}, itemSection {ItemSection}, amount {RequiredAmount}.",
                    client.TamerId,
                    client.Partner.Id,
                    client.Partner.BaseType,
                    client.Partner.CurrentType,
                    targetType,
                    evoStage,
                    itemSection,
                    requiredAmount);

                return true;
            }

            _logger.Warning(
                "Evolution request rejected because required use item could not be consumed. Tamer {TamerId}, partner {PartnerId}, base {BaseType}, current {CurrentType}, target {TargetType}, stage {Stage}, itemSection {ItemSection}, required {RequiredAmount}.",
                client.TamerId,
                client.Partner.Id,
                client.Partner.BaseType,
                client.Partner.CurrentType,
                targetType,
                evoStage,
                itemSection,
                requiredAmount);

            client.Send(new DigimonEvolutionFailPacket());
            return false;
        }

        private static bool TryGetRequiredEvolutionUseItem(EvolutionLineAssetModel? targetEvoInfo, out int itemSection, out int requiredAmount)
        {
            itemSection = targetEvoInfo?.UnlockItemSection ?? 0;
            requiredAmount = targetEvoInfo?.UnlockItemSectionAmount ?? 0;

            return itemSection > 0 && requiredAmount > 0;
        }

        private bool IsInsideLimitEvolutionRegion(GameClient client)
        {
            if (!_mapBin.IsLoaded)
                return false;

            int mapId = client.Tamer.Location.MapId;
            if (!_mapBin.Data.LimitEvoByMapId.TryGetValue(mapId, out var regions) || regions.Count == 0)
                return false;

            int x = client.Partner?.Location?.X ?? client.Tamer.Location.X;
            int y = client.Partner?.Location?.Y ?? client.Tamer.Location.Y;

            foreach (var region in regions)
            {
                long dx = x - region.CenterX;
                long dy = y - region.CenterY;
                long distanceSquared = dx * dx + dy * dy;
                long radiusSquared = (long)region.Radius * region.Radius;
                if (distanceSquared < radiusSquared)
                    return true;
            }

            return false;
        }

        private void UpdateSkillCooldown(GameClient client)
        {

            if (client.Tamer.Partner.HasActiveSkills())
            {

                foreach (var evolution in client.Tamer.Partner.Evolutions)
                {
                    foreach (var skill in evolution.Skills)
                    {
                        if (skill.Duration > 0 && skill.Expired)
                        {
                            skill.ResetCooldown();
                        }
                    }

                    _sender.Send(new UpdateEvolutionCommand(evolution));
                }

                List<int> SkillIds = new List<int>(5);
                var packetEvolution = client.Tamer.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Tamer.Partner.CurrentType);

                if (packetEvolution != null)
                {

                    var slot = -1;

                    foreach (var item in packetEvolution.Skills)
                    {
                        slot++;

                        var skillInfo = _assets.DigimonSkillInfo.FirstOrDefault(x => x.Type == client.Partner.CurrentType && x.Slot == slot);
                        if (skillInfo != null)
                        {
                            SkillIds.Add(skillInfo.SkillId);
                        }
                    }

                    client?.Send(new SkillUpdateCooldownPacket(client.Tamer.Partner.GeneralHandler, client.Tamer.Partner.CurrentType, packetEvolution, SkillIds));

                }
            }
        }
    }
}
