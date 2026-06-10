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
    public class HatchInsertBackupDiskPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchInsertBackup;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public HatchInsertBackupDiskPacketProcessor(
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

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(itemSlot);
            if (inventoryItem == null)
            {
                _logger.Warning($"Invalid hatch backup disk slot {itemSlot} for character {client.TamerId}.");
                return;
            }

            _logger.Information(
                "Hatch backup insert request: tamer {TamerId} itemSlot {ItemSlot} npc {NpcId} item {ItemId} currentEgg {EggId} currentBackup {BackupDiskId}.",
                client.TamerId,
                itemSlot,
                npcId,
                inventoryItem.ItemId,
                client.Tamer.Incubator.EggId,
                client.Tamer.Incubator.BackupDiskId);

            var hatchableItem = _assets.Hatchs.FirstOrDefault(x => x.ItemId == inventoryItem.ItemId);
            if (hatchableItem != null)
            {
                _logger.Warning(
                    "Rejected hatch backup insert for tamer {TamerId}: item {ItemId} from slot {ItemSlot} is a DigiEgg, not a backup disk.",
                    client.TamerId,
                    inventoryItem.ItemId,
                    itemSlot);
                client.Send(new SystemMessagePacket($"Item {inventoryItem.ItemId} is a DigiEgg, not a backup disk."));
                return;
            }

            var itemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == inventoryItem.ItemId);
            if (itemInfo?.Section != 17101 && itemInfo?.Section != 17102)
            {
                _logger.Warning(
                    "Rejected hatch backup insert for tamer {TamerId}: item {ItemId} section {Section} is not a hatch backup disk.",
                    client.TamerId,
                    inventoryItem.ItemId,
                    itemInfo?.Section);
                client.Send(new SystemMessagePacket($"Item {inventoryItem.ItemId} is not a hatch backup disk."));
                return;
            }

            client.Tamer.Incubator.InsertBackupDisk(inventoryItem.ItemId);

            _logger.Information($"Character {client.TamerId} inserted backup disk {inventoryItem.ItemId} into incubator.");

            client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, itemSlot);

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));
        }
    }
}
