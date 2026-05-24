using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchInsertEggPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchInsertEgg;

        private readonly ISender _sender;
        private readonly ILogger _logger;

        public HatchInsertEggPacketProcessor(
            ISender sender,
            ILogger logger)
        {
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

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(itemSlot);
            if (inventoryItem == null)
            {
                _logger.Warning(
                    "Invalid hatch egg slot {ItemSlot} for character {TamerId}; packetLength {PacketLength}.",
                    itemSlot,
                    client.TamerId,
                    packetData.Length);
                return;
            }

            client.Tamer.Incubator.InsertEgg(inventoryItem.ItemId);

            _logger.Information(
                "Hatch insert accepted: tamer {TamerId} itemSlot {ItemSlot} egg {EggId} amountBefore {Amount}.",
                client.TamerId,
                itemSlot,
                inventoryItem.ItemId,
                inventoryItem.Amount);

            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, itemSlot);

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));
        }
    }
}
