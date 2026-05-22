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
            _ = packet.ReadInt(); // NPC id; the current client sends it after the slot.

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(itemSlot);
            if (inventoryItem == null)
            {
                _logger.Warning($"Invalid hatch egg slot {itemSlot} for character {client.TamerId}.");
                return;
            }

            client.Tamer.Incubator.InsertEgg(inventoryItem.ItemId);

            _logger.Verbose($"Character {client.TamerId} inserted egg {inventoryItem.ItemId} into incubator.");

            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, itemSlot);

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));
        }
    }
}
