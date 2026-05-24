using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class ItemReturnPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.ItemReturn;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public ItemReturnPacketProcessor(
            AssetsLoader assets,
            ISender sender,
            ILogger logger)
        {
            _assets = assets;
            _sender = sender;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            int remaining = (packet.Length - 2) - (int)packet.Packet.Position;
            byte vipEnabled = 0;
            int portableReturnPosition = 0;
            int npcId;
            int itemSlot;

            if (remaining == 12)
            {
                portableReturnPosition = packet.ReadInt();
                npcId = packet.ReadInt();
                itemSlot = packet.ReadInt();
            }
            else if (remaining == 13)
            {
                vipEnabled = packet.ReadByte();
                portableReturnPosition = packet.ReadInt();
                npcId = packet.ReadInt();
                itemSlot = packet.ReadInt();
            }
            else if (remaining == 8)
            {
                npcId = packet.ReadInt();
                itemSlot = packet.ReadInt();
            }
            else if (remaining == 9)
            {
                vipEnabled = packet.ReadByte();
                npcId = packet.ReadInt();
                itemSlot = packet.ReadInt();
            }
            else
            {
                client.Send(new ItemReturnPacket(0, client.Tamer.Inventory.Bits));
                _logger.Warning(
                    "Return: invalid payload length {Length} for tamer {TamerId}.",
                    remaining,
                    client.TamerId);
                return;
            }

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(itemSlot);
            if (inventoryItem == null || inventoryItem.ItemId == 0 || inventoryItem.ItemInfo == null)
            {
                client.Send(new ItemReturnPacket(0, client.Tamer.Inventory.Bits));
                var invSlots = string.Join(",", client.Tamer.Inventory.Items
                    .Where(i => i.ItemId > 0)
                    .Select(i => $"slot={i.Slot}/item={i.ItemId}"));
                _logger.Warning(
                    "Return: client sent slot={Slot} vip={Vip} portable={Portable} npc={Npc} payload={Length}; tamer {TamerId} inventory items: [{Inv}]",
                    itemSlot,
                    vipEnabled,
                    portableReturnPosition,
                    npcId,
                    remaining,
                    client.TamerId,
                    invSlots);
                return;
            }

            var totalGain = (int)(inventoryItem.Amount * inventoryItem.ItemInfo.SellPrice);

            _logger.Information(
                "Return: tamer {TamerId} sold item {ItemId} x{Amount} from slot {Slot} for {Bits} bits. vip={Vip} portable={Portable} npc={Npc} payload={Length}",
                client.TamerId,
                inventoryItem.ItemId,
                inventoryItem.Amount,
                itemSlot,
                totalGain,
                vipEnabled,
                portableReturnPosition,
                npcId,
                remaining);

            await UpdateClientActionQuestProgress(client, inventoryItem.ItemId);

            client.Tamer.Inventory.AddBits(totalGain);
            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, inventoryItem.Amount, itemSlot);

            await _sender.Send(new UpdateItemCommand(inventoryItem));
            await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));

            client.Send(new ItemReturnPacket(totalGain, client.Tamer.Inventory.Bits));
        }

        private async Task UpdateClientActionQuestProgress(GameClient client, int itemId)
        {
            foreach (var questInProgress in client.Tamer.Progress.InProgressQuestData)
            {
                var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                if (questInfo == null)
                {
                    _logger.Warning(
                        "Quest {QuestId} is in progress for tamer {TamerId}, but no asset was loaded while processing item return.",
                        questInProgress.QuestId,
                        client.TamerId);
                    continue;
                }

                var goalIndex = questInfo.QuestGoals.FindIndex(x =>
                    x.GoalType == QuestGoalTypeEnum.ClientAction &&
                    x.GoalId == 6 &&
                    x.CurTypeCount == itemId);

                if (goalIndex < 0)
                    continue;

                var currentGoalValue = questInProgress.GetGoalValue(goalIndex);
                var targetGoalValue = questInfo.QuestGoals[goalIndex].GoalAmount;
                if (currentGoalValue >= targetGoalValue)
                    return;

                var updatedGoalValue = (byte)Math.Min(byte.MaxValue, Math.Min(targetGoalValue, currentGoalValue + 1));

                questInProgress.UpdateCondition(goalIndex, updatedGoalValue);
                client.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, updatedGoalValue));
                await _sender.Send(new UpdateCharacterInProgressCommand(questInProgress));

                _logger.Information(
                    "Quest {QuestId} return goal updated for tamer {TamerId}: item {ItemId}, goal {GoalIndex}, value {Current}/{Target}.",
                    questInProgress.QuestId,
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
