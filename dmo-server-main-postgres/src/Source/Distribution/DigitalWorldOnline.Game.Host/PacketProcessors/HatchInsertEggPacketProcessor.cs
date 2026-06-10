using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchInsertEggPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchInsertEgg;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public HatchInsertEggPacketProcessor(
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

            var itemSlot = packet.ReadInt();
            var npcId = packet.ReadInt(); // NPC id; the current client sends it after the slot.
            _logger.Information(
                "Hatch insert request: tamer {TamerId} itemSlot {ItemSlot} npc {NpcId} currentEgg {CurrentEggId} hatchLevel {HatchLevel}.",
                client.TamerId,
                itemSlot,
                npcId,
                client.Tamer.Incubator.EggId,
                client.Tamer.Incubator.HatchLevel);

            var resolvedSlot = itemSlot;
            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(resolvedSlot);
            if (inventoryItem == null)
            {
                _logger.Warning(
                    "Invalid hatch egg slot {ItemSlot} for character {TamerId}; packetLength {PacketLength}.",
                    itemSlot,
                    client.TamerId,
                    packetData.Length);
                return;
            }

            if (client.Tamer.Incubator.EggId > 0)
            {
                _logger.Warning(
                    "Rejected hatch insert for tamer {TamerId}: incubator already has egg {EggId}.",
                    client.TamerId,
                    client.Tamer.Incubator.EggId);
                client.Send(new SystemMessagePacket("Incubator already has a DigiEgg."));
                return;
            }

            var hatchInfo = _assets.Hatchs.FirstOrDefault(x => x.ItemId == inventoryItem.ItemId);
            if (hatchInfo == null && itemSlot > 0)
            {
                var previousSlot = itemSlot - 1;
                var previousSlotItem = client.Tamer.Inventory.FindItemBySlot(previousSlot);
                var previousSlotHatchInfo = previousSlotItem == null
                    ? null
                    : _assets.Hatchs.FirstOrDefault(x => x.ItemId == previousSlotItem.ItemId);

                if (previousSlotHatchInfo != null)
                {
                    _logger.Warning(
                        "Hatch egg slot corrected: tamer {TamerId} packetSlot {PacketSlot} packetItem {PacketItemId} resolvedSlot {ResolvedSlot} resolvedEgg {EggId}.",
                        client.TamerId,
                        itemSlot,
                        inventoryItem.ItemId,
                        previousSlot,
                        previousSlotItem!.ItemId);

                    resolvedSlot = previousSlot;
                    inventoryItem = previousSlotItem;
                    hatchInfo = previousSlotHatchInfo;
                }
            }

            if (hatchInfo == null)
            {
                _logger.Warning(
                    "Rejected hatch insert for tamer {TamerId}: item {ItemId} from slot {ItemSlot} is not a hatchable DigiEgg.",
                    client.TamerId,
                    inventoryItem.ItemId,
                    resolvedSlot);
                client.Send(new SystemMessagePacket($"Item {inventoryItem.ItemId} is not a hatchable DigiEgg."));
                return;
            }

            client.Tamer.Incubator.InsertEgg(inventoryItem.ItemId);

            _logger.Information(
                "Hatch insert accepted: tamer {TamerId} itemSlot {ItemSlot} egg {EggId} amountBefore {Amount}.",
                client.TamerId,
                resolvedSlot,
                inventoryItem.ItemId,
                inventoryItem.Amount);

            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, resolvedSlot);

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));
        }
    }
}

