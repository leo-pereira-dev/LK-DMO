using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class QuestAcceptPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.QuestAccept;

        private const short TutorialHatchQuestId = 4022;
        private const int TutorialBackupDiskItemId = 9074;
        private const int TutorialBrokenInsectoidDataItemId = 9077;
        private const int TutorialInsectoidDataItemId = 97071;
        private const int TutorialRequiredLowDataAmount = 3;
        private const short TutorialCraftQuestId = 4054;
        private const int TutorialCraftMaterialItemId = 70261;

        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public QuestAcceptPacketProcessor(
            AssetsLoader assets,
            ILogger logger,
            ISender sender)
        {
            _assets = assets;
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var questId = packet.ReadShort();

            if (client.Tamer.Progress.AcceptQuest(questId))
            {
                var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questId);
                if (questInfo == null)
                {
                    _logger.Error($"Unknown quest id {questId}.");
                    client.Send(new SystemMessagePacket($"Unknown quest id {questId}."));
                    client.Tamer.Progress.RemoveQuest(questId);
                    return;
                }

                var inventoryChanged = false;

                foreach (var questSupply in questInfo.QuestSupplies)
                {
                    if (questId == TutorialHatchQuestId && questSupply.ItemId == TutorialBackupDiskItemId)
                        continue;

                    var itemId = questId == TutorialHatchQuestId && questSupply.ItemId == TutorialBrokenInsectoidDataItemId
                        ? TutorialInsectoidDataItemId
                        : questSupply.ItemId;
                    var amount = questId == TutorialHatchQuestId && itemId == TutorialInsectoidDataItemId
                        ? Math.Max(questSupply.Amount, TutorialRequiredLowDataAmount)
                        : questSupply.Amount;

                    var item = new ItemModel();
                    item.SetItemId(itemId);
                    item.SetAmount(amount);
                    item.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == item.ItemId));

                    if (item.ItemInfo == null)
                    {
                        _logger.Error($"Item information not found for item {item.ItemId}.");
                        client.Send(new SystemMessagePacket($"Item information not found for item {item.ItemId}."));
                        client.Tamer.Progress.RemoveQuest(questId);
                        return;
                    }

                    var itemClone = (ItemModel)item.Clone();
                    if (!client.Tamer.Inventory.AddItem(itemClone))
                    {
                        client.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                        client.Tamer.Progress.RemoveQuest(questId);
                        return;
                    }             

                    inventoryChanged = true;

                }

                if (questId == TutorialCraftQuestId &&
                    !client.Tamer.Inventory.FindItemsById(TutorialCraftMaterialItemId).Any())
                {
                    var item = new ItemModel(TutorialCraftMaterialItemId, 1);
                    item.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == item.ItemId));
                    item.SetDefaultRemainingTime();

                    if (item.ItemInfo == null)
                    {
                        _logger.Error($"Item information not found for tutorial craft material {item.ItemId}.");
                        client.Send(new SystemMessagePacket($"Item information not found for tutorial craft material {item.ItemId}."));
                        client.Tamer.Progress.RemoveQuest(questId);
                        return;
                    }

                    if (!client.Tamer.Inventory.AddItem(item))
                    {
                        client.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                        client.Tamer.Progress.RemoveQuest(questId);
                        return;
                    }

                    inventoryChanged = true;
                }

                _logger.Verbose($"Character {client.TamerId} accepted quest {questId}.");

                foreach (var questSupply in questInfo.QuestSupplies)
                {
                    if (questId == TutorialHatchQuestId && questSupply.ItemId == TutorialBackupDiskItemId)
                        continue;

                    var itemId = questId == TutorialHatchQuestId && questSupply.ItemId == TutorialBrokenInsectoidDataItemId
                        ? TutorialInsectoidDataItemId
                        : questSupply.ItemId;
                    var amount = questId == TutorialHatchQuestId && itemId == TutorialInsectoidDataItemId
                        ? Math.Max(questSupply.Amount, TutorialRequiredLowDataAmount)
                        : questSupply.Amount;

                    _logger.Verbose($"Character {client.TamerId} received quest {questId} supply item {itemId} x{amount}.");
                }

                if (questId == TutorialCraftQuestId)
                    _logger.Verbose($"Character {client.TamerId} received quest {questId} tutorial craft material {TutorialCraftMaterialItemId} x1.");
         
                if (inventoryChanged)
                    client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                await _sender.Send(new AddCharacterProgressCommand(client.Tamer.Progress));
            }
        }
    }
}
