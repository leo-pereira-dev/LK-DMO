using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using MediatR;
using Serilog;
using System.Collections.Generic;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class QuestGiveUpPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.QuestGiveUp;

        private const short TutorialHatchQuestId = 4022;
        private const int TutorialHatchRequiredItemId = 41002;
        private static readonly Dictionary<int, (int DataItemId, int DataAmount)> TutorialHatchRecoveryItems =
            new Dictionary<int, (int DataItemId, int DataAmount)>
            {
                { 114240, (98431, 25) },
                { 114241, (98421, 15) },
                { 114242, (98441, 25) },
                { 114243, (98401, 50) },
                { 114244, (98471, 25) },
                { 114245, (98401, 25) }
            };

        private readonly ILogger _logger;
        private readonly AssetsLoader _assets;
        private readonly ISender _sender;

        public QuestGiveUpPacketProcessor(
            ILogger logger,
            AssetsLoader assets,
            ISender sender)
        {
            _logger = logger;
            _assets = assets;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var payloadLength = (packet.Length - 2) - (int)packet.Packet.Position;
            if (payloadLength < 2)
            {
                _logger.Warning(
                    "Quest give up ignored: invalid payload length {Payload} for tamer {TamerId}.",
                    payloadLength,
                    client.TamerId);
                return;
            }

            var questId = packet.ReadShort();
            var activeBefore = FormatActiveQuests(client);

            _logger.Information(
                "Quest give up requested: tamer {TamerId}, quest {QuestId}, payload={Payload}, activeBefore=[{ActiveBefore}].",
                client.TamerId,
                questId,
                payloadLength,
                activeBefore);

            var id = client.Tamer.Progress.RemoveQuest(questId);

            await _sender.Send(new RemoveActiveQuestCommand(id));

            if (questId == TutorialHatchQuestId)
                await RecoverTutorialHatchRequirement(client);

            client.Send(new QuestCanceledPacket());

            _logger.Information(
                "Quest give up completed: tamer {TamerId}, quest {QuestId}, removedId={ProgressQuestId}, activeAfter=[{ActiveAfter}].",
                client.TamerId,
                questId,
                id,
                FormatActiveQuests(client));
        }

        private async Task RecoverTutorialHatchRequirement(GameClient client)
        {
            var restoredAny = false;

            if (!client.Tamer.Inventory.Items.Any(x => x.ItemId == TutorialHatchRequiredItemId && x.Amount > 0))
                restoredAny |= RestoreTutorialItem(client, TutorialHatchRequiredItemId, 1);

            var tutorialEggId = client.Tamer.Incubator.EggId;
            if (!TutorialHatchRecoveryItems.ContainsKey(tutorialEggId))
            {
                var inventoryTutorialEgg = client.Tamer.Inventory.Items
                    .FirstOrDefault(x => TutorialHatchRecoveryItems.ContainsKey(x.ItemId) && x.Amount > 0);

                if (inventoryTutorialEgg != null)
                    tutorialEggId = inventoryTutorialEgg.ItemId;
            }

            if (TutorialHatchRecoveryItems.TryGetValue(tutorialEggId, out var recoveryItem))
            {
                var currentDataAmount = client.Tamer.Inventory.Items
                    .Where(x => x.ItemId == recoveryItem.DataItemId && x.Amount > 0)
                    .Sum(x => x.Amount);

                if (currentDataAmount < recoveryItem.DataAmount)
                    restoredAny |= RestoreTutorialItem(client, recoveryItem.DataItemId, recoveryItem.DataAmount - currentDataAmount);
            }
            else
            {
                _logger.Warning(
                    "Tutorial hatch recovery skipped egg/data restore for tamer {TamerId}: no selected tutorial egg was found.",
                    client.TamerId);
            }

            if (!restoredAny)
                return;

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
        }

        private bool RestoreTutorialItem(GameClient client, int itemId, int amount)
        {
            var itemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemId);
            if (itemInfo == null)
            {
                _logger.Warning(
                    "Tutorial hatch recovery skipped: item info {ItemId} not loaded for tamer {TamerId}.",
                    itemId,
                    client.TamerId);
                return false;
            }

            var item = new ItemModel();
            item.SetItemId(itemId);
            item.SetAmount(amount);
            item.SetItemInfo(itemInfo);

            if (!client.Tamer.Inventory.AddItem((ItemModel)item.Clone()))
            {
                _logger.Warning(
                    "Tutorial hatch recovery failed: inventory full while restoring item {ItemId} x{Amount} to tamer {TamerId}.",
                    itemId,
                    amount,
                    client.TamerId);
                return false;
            }

            _logger.Information(
                "Tutorial hatch recovery restored item {ItemId} x{Amount} for tamer {TamerId} after quest give up.",
                itemId,
                amount,
                client.TamerId);

            return true;
        }

        private static string FormatActiveQuests(GameClient client)
        {
            return string.Join(", ", client.Tamer.Progress.InProgressQuestData
                .Select(x => $"{x.QuestId}:{x.FirstCondition}/{x.SecondCondition}/{x.ThirdCondition}/{x.FourthCondition}/{x.FifthCondition}"));
        }
    }
}
