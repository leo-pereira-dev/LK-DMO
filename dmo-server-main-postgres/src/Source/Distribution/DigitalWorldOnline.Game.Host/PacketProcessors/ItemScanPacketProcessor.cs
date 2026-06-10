using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.DTOs.Base;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;
using System.Net.Mime;


namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class ItemScanPacketProcessor : IGamePacketProcessor
    {
        private const int TutorialSuppliesKitItemId = 70259;
        private const int TutorialReturnItemId = 70260;

        public GameServerPacketEnum Type => GameServerPacketEnum.ItemScan;

        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly AccessoryEnchantService _accessoryEnchantService;

        public ItemScanPacketProcessor(
            AssetsLoader assets,
            MapServer mapServer,
            ISender sender,
            ILogger logger,
            AccessoryEnchantService accessoryEnchantService)
        {
            _assets = assets;
            _mapServer = mapServer;
            _sender = sender;
            _logger = logger;
            _accessoryEnchantService = accessoryEnchantService;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            int remaining = (packet.Length - 2) - (int)packet.Packet.Position;
            byte vipEnabled = 0;
            int portableIdx = 0;
            int npcId;
            int slotToScan;
            short amountToScan;

            if (remaining == 14)
            {
                portableIdx = packet.ReadInt();
                npcId = packet.ReadInt();
                slotToScan = packet.ReadInt();
                amountToScan = packet.ReadShort();
            }
            else if (remaining == 15)
            {
                vipEnabled = packet.ReadByte();
                portableIdx = packet.ReadInt();
                npcId = packet.ReadInt();
                slotToScan = packet.ReadInt();
                amountToScan = packet.ReadShort();
            }
            else if (remaining == 10)
            {
                npcId = packet.ReadInt();
                slotToScan = packet.ReadInt();
                amountToScan = packet.ReadShort();
            }
            else if (remaining == 11)
            {
                vipEnabled = packet.ReadByte();
                npcId = packet.ReadInt();
                slotToScan = packet.ReadInt();
                amountToScan = packet.ReadShort();
            }
            else
            {
                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new ItemScanFailPacket(client.Tamer.Inventory.Bits, 0, 0).Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
                    )
                );
                _logger.Warning("Scan: invalid payload length {Length} for tamer {TamerId}.", remaining, client.TamerId);
                return;
            }

            var scannedItem = client.Tamer.Inventory.FindItemBySlot(slotToScan);
            if (scannedItem == null || scannedItem.ItemId == 0 || scannedItem.ItemInfo == null)
            {
                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new SystemMessagePacket($"Invalid item at slot {slotToScan}.").Serialize(),
                        new ItemScanFailPacket(client.Tamer.Inventory.Bits, slotToScan, scannedItem?.ItemId ?? 0).Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
                    )
                );
                var invSlots = string.Join(",", client.Tamer.Inventory.Items
                    .Where(i => i.ItemId > 0)
                    .Select(i => $"slot={i.Slot}/item={i.ItemId}"));
                _logger.Warning(
                    "Scan: client sent slot={Slot} vip={Vip} portable={Portable} npc={Npc} count={Count}; tamer {TamerId} inventory items: [{Inv}]",
                    slotToScan, vipEnabled, portableIdx, npcId, amountToScan, client.TamerId, invSlots);
                return;
            }

            var scanAsset = _assets.ScanDetail.FirstOrDefault(x => x.ItemId == scannedItem.ItemId)
                ?? TryBuildTutorialScanAsset(scannedItem.ItemId);

            if (scanAsset == null)
            {
                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new SystemMessagePacket($"No scan configuration for item id {scannedItem.ItemId}.").Serialize(),
                        new ItemScanFailPacket(client.Tamer.Inventory.Bits, slotToScan, scannedItem.ItemId).Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
                    )
                );
                _logger.Warning($"No scan configuration for item id {scannedItem.ItemId}");
                return;
            }

            var receivedRewards = new Dictionary<int, ItemModel>();
            short scannedItens = 0;
            long cost = 0;
            var error = false;
            var scannedItemId = scannedItem.ItemId;

            while (scannedItens < amountToScan && !error)
            {
                if (!scanAsset.Rewards.Any())
                {
                    _logger.Warning($"Scan config for item {scanAsset.ItemId} has incorrect rewards configuration.");
                    client.Send(new SystemMessagePacket($"Scan config for item {scanAsset.ItemId} has incorrect rewards configuration."));
                    break;
                }

                var possibleRewards = scanAsset.Rewards.OrderBy(x => Guid.NewGuid()).ToList();
                foreach (var possibleReward in possibleRewards)
                {
                    if (cost + scannedItem.ItemInfo.ScanPrice > client.Tamer.Inventory.Bits)
                    {
                        _logger.Warning($"No more bits after start scanning for tamer {client.TamerId}.");
                        error = true;
                        break;
                    }

                    if (possibleReward.Chance >= UtilitiesFunctions.RandomDouble())
                    {
                        var itemRewardAmount = UtilitiesFunctions.RandomInt(possibleReward.MinAmount, possibleReward.MaxAmount);

                        var contentItem = new ItemModel();
                        contentItem.SetItemId(possibleReward.ItemId);
                        contentItem.SetAmount(itemRewardAmount);
                        contentItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == possibleReward.ItemId));
    
                        if (contentItem.ItemInfo == null)
                        {
                            _logger.Warning($"Invalid item info for item {possibleReward.ItemId} in tamer {client.TamerId} scan.");
                            client.Send(new SystemMessagePacket($"Invalid item info for item {possibleReward.ItemId}."));
                            error = true;
                            break;
                        }

                        if (contentItem.ItemInfo.Type == 52)
                        {
                            _ = _accessoryEnchantService.ApplyRandomChipsetStats(contentItem);
                        }


                        if (contentItem.IsTemporary)
                            contentItem.SetRemainingTime((uint)contentItem.ItemInfo.UsageTimeMinutes);

                        var targetSlot = client.Tamer.Inventory.FindAvailableSlot(contentItem);

                        if (targetSlot != client.Tamer.Inventory.GetEmptySlot)
                        {
                            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(targetSlot);
                            if (inventoryItem == null)
                            {
                                error = true;
                                break;
                            }

                            var tempItem = new ItemModel(inventoryItem.ItemId, inventoryItem.Amount + contentItem.Amount)
                            {
                                ItemInfo = inventoryItem.ItemInfo,
                                Power = inventoryItem.Power,
                                RerollLeft = inventoryItem.RerollLeft,
                                FamilyType = inventoryItem.FamilyType,
                                Duration = inventoryItem.Duration,
                                EndDate = inventoryItem.EndDate,
                                FirstExpired = inventoryItem.FirstExpired
                            };

                            if (!receivedRewards.ContainsKey(targetSlot))
                                receivedRewards.Add(targetSlot, tempItem);
                            else
                                receivedRewards[targetSlot].IncreaseAmount(contentItem.Amount);
                        }
                        else
                        {
                            var tempItem = new ItemModel(contentItem.ItemId, contentItem.Amount)
                            {
                                ItemInfo = contentItem.ItemInfo,
                                Power = contentItem.Power,
                                RerollLeft = contentItem.RerollLeft,
                                FamilyType = contentItem.FamilyType,
                                Duration = contentItem.Duration,
                                EndDate = contentItem.EndDate,
                                FirstExpired = contentItem.FirstExpired
                            };

                            if (!receivedRewards.ContainsKey(targetSlot))
                                receivedRewards.Add(targetSlot, tempItem);
                            else
                                receivedRewards[targetSlot].IncreaseAmount(contentItem.Amount);
                        }

                        if (client.Tamer.Inventory.AddItem(contentItem))
                        {

                            if (possibleReward.Rare)
                                _mapServer.BroadcastForChannel(client.Tamer.Channel, new NeonMessagePacket(NeonMessageTypeEnum.Item, client.Tamer.Name, scanAsset.ItemId, possibleReward.ItemId).Serialize());

                            cost += scannedItem.ItemInfo.ScanPrice;
                            scannedItens++;
                        }
                        else
                        {
                            _logger.Warning($"No more space after start scanning for tamer {client.TamerId}.");
                            error = true;
                            break;
                        }
                    }

                    if (scannedItens >= amountToScan || error)
                        break;
                }
            }

            var dropList = string.Join(',', receivedRewards.Select(x => $"{x.Value.ItemId} x{x.Value.Amount}"));

            if (vipEnabled == 1)
            {
                _logger.Information(
                    "Scan: tamer {TamerId} scanned item {ItemId} x{Amount} with VIP. slot={Slot} portable={Portable} npc={Npc} payload={Payload} rewards=[{Rewards}]",
                    client.TamerId,
                    scannedItemId,
                    scannedItens,
                    slotToScan,
                    portableIdx,
                    npcId,
                    remaining,
                    dropList);
            }
            else
            {
                _logger.Information(
                    "Scan: tamer {TamerId} scanned item {ItemId} x{Amount} at {Location} with NPC {Npc}. slot={Slot} portable={Portable} payload={Payload} rewards=[{Rewards}]",
                    client.TamerId,
                    scannedItemId,
                    scannedItens,
                    client.TamerLocation,
                    npcId,
                    slotToScan,
                    portableIdx,
                    remaining,
                    dropList);
            }

            client.Tamer.Inventory.RemoveBits(cost);
            client.Tamer.Inventory.RemoveOrReduceItem(scannedItem, scannedItens, slotToScan);

            await UpdateClientActionQuestProgress(client, scannedItemId, "scan");

            await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            client.Send(
                UtilitiesFunctions.GroupPackets(
                    new ItemScanSuccessPacket(
                        cost,
                        client.Tamer.Inventory.Bits,
                        slotToScan,
                        scannedItemId,
                        scannedItens,
                        receivedRewards).Serialize(),
                    new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
                )
            );
        }

        private static ScanDetailAssetModel? TryBuildTutorialScanAsset(int itemId)
        {
            if (itemId != TutorialSuppliesKitItemId)
                return null;

            return new ScanDetailAssetModel
            {
                Id = -TutorialSuppliesKitItemId,
                MinAmount = 1,
                MaxAmount = 1,
                ItemId = TutorialSuppliesKitItemId,
                ItemName = "Supplies Kit",
                Rewards = new List<ScanRewardDetailAssetModel>
                {
                    new()
                    {
                        Id = -TutorialReturnItemId,
                        ItemId = TutorialReturnItemId,
                        ItemName = "Return Item [Tutorial]",
                        MinAmount = 1,
                        MaxAmount = 1,
                        Chance = 100.0,
                        Rare = false,
                        ScanDetailAssetId = -TutorialSuppliesKitItemId,
                    }
                }
            };
        }

        private async Task UpdateClientActionQuestProgress(GameClient client, int itemId, string actionName)
        {
            if (!client.Tamer.Progress.InProgressQuestData.Any())
            {
                _logger.Information(
                    "Quest {ActionName}: no in-progress quests for tamer {TamerId} while processing item {ItemId}.",
                    actionName,
                    client.TamerId,
                    itemId);
                return;
            }

            foreach (var questInProgress in client.Tamer.Progress.InProgressQuestData)
            {
                var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                if (questInfo == null)
                {
                    _logger.Warning(
                        "Quest {QuestId} is in progress for tamer {TamerId}, but no asset was loaded while processing item {ActionName}.",
                        questInProgress.QuestId,
                        client.TamerId,
                        actionName);
                    continue;
                }

                var goalIndex = questInfo.QuestGoals.FindIndex(x =>
                    x.GoalType == QuestGoalTypeEnum.ClientAction &&
                    x.GoalId == 6 &&
                    x.CurTypeCount == itemId);

                if (goalIndex < 0)
                {
                    var actionGoals = string.Join(", ", questInfo.QuestGoals
                        .Select((goal, index) => new { goal, index })
                        .Where(x => x.goal.GoalType == QuestGoalTypeEnum.ClientAction && x.goal.GoalId == 6)
                        .Select(x => $"idx={x.index}/item={x.goal.CurTypeCount}/amount={x.goal.GoalAmount}"));

                    _logger.Information(
                        "Quest {ActionName}: no matching client-action goal for tamer {TamerId}, quest {QuestId}, item {ItemId}. Available item-action goals: [{Goals}]",
                        actionName,
                        client.TamerId,
                        questInProgress.QuestId,
                        itemId,
                        actionGoals);
                    continue;
                }

                var currentGoalValue = questInProgress.GetGoalValue(goalIndex);
                var targetGoalValue = questInfo.QuestGoals[goalIndex].GoalAmount;
                if (currentGoalValue >= targetGoalValue)
                {
                    _logger.Information(
                        "Quest {ActionName}: goal already complete for tamer {TamerId}, quest {QuestId}, item {ItemId}, goal {GoalIndex}, value {Current}/{Target}.",
                        actionName,
                        client.TamerId,
                        questInProgress.QuestId,
                        itemId,
                        goalIndex,
                        currentGoalValue,
                        targetGoalValue);
                    return;
                }

                var updatedGoalValue = (byte)Math.Min(byte.MaxValue, Math.Min(targetGoalValue, currentGoalValue + 1));

                questInProgress.UpdateCondition(goalIndex, updatedGoalValue);
                client.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, updatedGoalValue));
                await _sender.Send(new UpdateCharacterInProgressCommand(questInProgress));

                _logger.Information(
                    "Quest {QuestId} {ActionName} goal updated for tamer {TamerId}: item {ItemId}, goal {GoalIndex}, value {Current}/{Target}.",
                    questInProgress.QuestId,
                    actionName,
                    client.TamerId,
                    itemId,
                    goalIndex,
                    updatedGoalValue,
                    targetGoalValue);
                return;
            }
        }

    }
}
