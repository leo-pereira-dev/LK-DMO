using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class DirectHatchPacketProcessor : IGamePacketProcessor
    {
        private const int DirectHatchItemType = 177;
        private const int MercenaryDigiEggItemType = 93;
        private const int DirectHatchLevel = 5;

        public GameServerPacketEnum Type => GameServerPacketEnum.DirectHatch;

        private readonly StatusManager _statusManager;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public DirectHatchPacketProcessor(
            StatusManager statusManager,
            MapServer mapServer,
            AssetsLoader assets,
            ILogger logger,
            ISender sender,
            DungeonsServer dungeonsServer
        )
        {
            _statusManager = statusManager;
            _mapServer = mapServer;
            _assets = assets;
            _logger = logger;
            _sender = sender;
            _dungeonServer = dungeonsServer;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var requestedItemId = packet.ReadInt();
            var itemSlot = packet.ReadInt();
            var digiName = NormalizeHatchName(packet.ReadString(), client.Tamer.Name);
            var inventorySlot = (short)itemSlot;

            _logger.Information(
                "Direct hatch request: tamer {TamerId} slot {Slot} requestedItem {RequestedItemId} name {DigimonName}.",
                client.TamerId,
                itemSlot,
                requestedItemId,
                digiName);

            if (client.Partner == null)
            {
                _logger.Warning("Rejected direct hatch for tamer {TamerId}: invalid partner.", client.TamerId);
                client.Send(new DirectHatchPacket());
                return;
            }

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(itemSlot);
            if (inventoryItem == null)
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: invalid slot {Slot}.",
                    client.TamerId,
                    itemSlot);
                client.Send(new DirectHatchPacket());
                return;
            }

            if (inventoryItem.ItemId != requestedItemId)
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: packet item {RequestedItemId} does not match slot item {SlotItemId} at slot {Slot}.",
                    client.TamerId,
                    requestedItemId,
                    inventoryItem.ItemId,
                    itemSlot);
                client.Send(new DirectHatchPacket());
                return;
            }

            if (!IsDirectHatchEgg(inventoryItem.ItemInfo?.Type, inventoryItem.ItemInfo?.Name))
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: item {ItemId} has type {ItemType} and name {ItemName}.",
                    client.TamerId,
                    inventoryItem.ItemId,
                    inventoryItem.ItemInfo?.Type,
                    inventoryItem.ItemInfo?.Name);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket("This DigiEgg cannot be hatched by right-click."));
                return;
            }

            var hatchInfo = _assets.Hatchs.FirstOrDefault(x => x.ItemId == inventoryItem.ItemId);
            if (hatchInfo == null)
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: item {ItemId} has no hatch asset.",
                    client.TamerId,
                    inventoryItem.ItemId);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Unknown hatch info for egg {inventoryItem.ItemId}."));
                return;
            }

            client.Tamer.CompactActiveDigimonSlots();
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));

            var activeSlotCount = client.Tamer.GetNextActiveDigimonSlot();
            if (activeSlotCount >= client.Tamer.DigimonSlots)
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: no available digivice slot for item {ItemId}.",
                    client.TamerId,
                    inventoryItem.ItemId);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket("No available digivice slot for the hatched Digimon."));
                return;
            }

            var targetSlot = (byte)activeSlotCount;
            var digimonBaseInfo = _assets.DigimonBaseInfo.FirstOrDefault(x => x.Type == hatchInfo.HatchType);
            var evolutionInfo = _assets.EvolutionInfo.FirstOrDefault(x => x.Type == hatchInfo.HatchType);

            if (digimonBaseInfo == null || evolutionInfo == null || !evolutionInfo.Lines.Any())
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: item {ItemId} resolves to digimon {BaseType}, but baseInfo={HasBaseInfo}, evolutionInfo={HasEvolutionInfo}, evolutionLines={EvolutionLines}.",
                    client.TamerId,
                    inventoryItem.ItemId,
                    hatchInfo.HatchType,
                    digimonBaseInfo != null,
                    evolutionInfo != null,
                    evolutionInfo?.Lines.Count ?? 0);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Unknown digimon info for {hatchInfo.HatchType}."));
                return;
            }

            var newDigimon = DigimonModel.Create(
                digiName,
                hatchInfo.HatchType,
                hatchInfo.HatchType,
                DigimonHatchGradeEnum.Perfect,
                UtilitiesFunctions.RandomShort(11800, 13000),
                targetSlot
            );

            newDigimon.NewLocation(
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y
            );

            newDigimon.SetBaseInfo(digimonBaseInfo);

            try
            {
                newDigimon.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        newDigimon.BaseType,
                        newDigimon.Level,
                        newDigimon.Size
                    )
                );
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Rejected direct hatch for tamer {TamerId}: could not resolve base status for digimon {BaseType}, level {Level}, size {Size}.",
                    client.TamerId,
                    newDigimon.BaseType,
                    newDigimon.Level,
                    newDigimon.Size);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Unknown digimon status for {newDigimon.BaseType}."));
                return;
            }

            newDigimon.FullHeal();

            newDigimon.AddEvolutions(evolutionInfo);

            if (newDigimon.BaseInfo == null || newDigimon.BaseStatus == null || !newDigimon.Evolutions.Any())
            {
                _logger.Warning("Rejected direct hatch for tamer {TamerId}: unknown digimon info for {BaseType}.", client.TamerId, newDigimon.BaseType);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Unknown digimon info for {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetTamer(client.Tamer);

            DigitalWorldOnline.Commons.DTOs.Digimon.DigimonDTO? digimonInfo;
            try
            {
                digimonInfo = await _sender.Send(new CreateDigimonCommand(newDigimon));
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Rejected direct hatch for tamer {TamerId}: failed to persist digimon {BaseType}.",
                    client.TamerId,
                    newDigimon.BaseType);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Could not hatch digimon {newDigimon.BaseType}."));
                return;
            }

            if (digimonInfo == null)
            {
                _logger.Warning(
                    "Rejected direct hatch for tamer {TamerId}: could not persist digimon {BaseType}.",
                    client.TamerId,
                    newDigimon.BaseType);
                client.Send(new DirectHatchPacket());
                client.Send(new SystemMessagePacket($"Could not hatch digimon {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetId(digimonInfo.Id);
            var evolutionSlot = -1;

            foreach (var digimon in newDigimon.Evolutions)
            {
                evolutionSlot++;

                if (evolutionSlot >= digimonInfo.Evolutions.Count)
                {
                    _logger.Warning(
                        "Direct hatch persisted fewer evolutions than expected for tamer {TamerId}, digimon {DigimonId}: expected slot {EvolutionSlot}, persisted count {PersistedCount}.",
                        client.TamerId,
                        newDigimon.Id,
                        evolutionSlot,
                        digimonInfo.Evolutions.Count);
                    continue;
                }

                var evolution = digimonInfo.Evolutions[evolutionSlot];
                if (evolution == null)
                    continue;

                digimon.SetId(evolution.Id);

                var skillSlot = -1;
                foreach (var skill in digimon.Skills)
                {
                    skillSlot++;

                    if (skillSlot >= evolution.Skills.Count)
                    {
                        _logger.Warning(
                            "Direct hatch persisted fewer evolution skills than expected for tamer {TamerId}, digimon {DigimonId}, evolution {EvolutionId}: expected slot {SkillSlot}, persisted count {PersistedCount}.",
                            client.TamerId,
                            newDigimon.Id,
                            evolution.Id,
                            skillSlot,
                            evolution.Skills.Count);
                        continue;
                    }

                    var dtoSkill = evolution.Skills[skillSlot];
                    skill.SetId(dtoSkill.Id);
                }
            }

            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, inventorySlot);
            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));

            client.Tamer.AddDigimon(newDigimon);

            var displaySlot = newDigimon.Slot;
            var directHatchPacket = new DirectHatchPacket(
                inventorySlot,
                newDigimon,
                (uint)(client.Partner.GeneralHandler + 1000),
                displaySlot
            ).Serialize();

            client.Send(directHatchPacket);
            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

            await UpdateClientActionQuestProgress(client, 7, newDigimon.BaseType, "direct-hatch");

            if (client.Tamer.Incubator.PerfectSize(newDigimon.HatchGrade, newDigimon.Size))
            {
                _mapServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
                _dungeonServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
            }

            _logger.Information(
                "Direct hatch completed: tamer {TamerId} item {ItemId} digimon {DigimonId} base {BaseType} slot {Slot} level {HatchLevel} size {Size} bytes {Bytes}.",
                client.TamerId,
                inventoryItem.ItemId,
                newDigimon.Id,
                newDigimon.BaseType,
                newDigimon.Slot,
                DirectHatchLevel,
                newDigimon.Size,
                directHatchPacket.Length);
        }

        private async Task UpdateClientActionQuestProgress(GameClient client, int actionId, int actionValue, string actionName)
        {
            if (!client.Tamer.Progress.InProgressQuestData.Any())
            {
                _logger.Information(
                    "Quest {ActionName}: no in-progress quests for tamer {TamerId} while processing action {ActionId}/{ActionValue}.",
                    actionName,
                    client.TamerId,
                    actionId,
                    actionValue);
                return;
            }

            foreach (var questInProgress in client.Tamer.Progress.InProgressQuestData)
            {
                var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                if (questInfo == null)
                {
                    _logger.Warning(
                        "Quest {QuestId} is in progress for tamer {TamerId}, but no asset was loaded while processing action {ActionName}.",
                        questInProgress.QuestId,
                        client.TamerId,
                        actionName);
                    continue;
                }

                var goalIndex = questInfo.QuestGoals.FindIndex(x =>
                    x.GoalType == QuestGoalTypeEnum.ClientAction &&
                    x.GoalId == actionId &&
                    (x.CurTypeCount == 0 || x.CurTypeCount == actionValue));

                if (goalIndex < 0)
                {
                    continue;
                }

                var currentGoalValue = questInProgress.GetGoalValue(goalIndex);
                var targetGoalValue = questInfo.QuestGoals[goalIndex].GoalAmount;
                if (currentGoalValue >= targetGoalValue)
                {
                    return;
                }

                var updatedGoalValue = (byte)Math.Min(byte.MaxValue, Math.Min(targetGoalValue, currentGoalValue + 1));

                questInProgress.UpdateCondition(goalIndex, updatedGoalValue);
                client.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, updatedGoalValue));
                await _sender.Send(new UpdateCharacterInProgressCommand(questInProgress));
                return;
            }
        }

        private static bool IsDirectHatchEgg(int? itemType, string? itemName)
        {
            if (itemType == DirectHatchItemType)
                return true;

            if (itemType != MercenaryDigiEggItemType || string.IsNullOrWhiteSpace(itemName))
                return false;

            var normalizedName = itemName.ToLowerInvariant();
            return normalizedName.Contains("level 5") ||
                   normalizedName.Contains("level5") ||
                   normalizedName.Contains("lv.5") ||
                   normalizedName.Contains("lv5");
        }

        private static string NormalizeHatchName(string? value, string fallbackName)
        {
            var name = string.IsNullOrEmpty(value)
                ? string.Empty
                : value.Replace("\0", string.Empty).Trim();

            if (string.IsNullOrWhiteSpace(name))
            {
                name = string.IsNullOrWhiteSpace(fallbackName)
                    ? "Digimon"
                    : fallbackName.Replace("\0", string.Empty).Trim();
            }

            if (string.IsNullOrWhiteSpace(name))
                name = "Digimon";

            const int maxDigimonNameLength = 20;
            if (name.Length > maxDigimonNameLength)
                name = name.Substring(0, maxDigimonNameLength);

            return name;
        }
    }
}
