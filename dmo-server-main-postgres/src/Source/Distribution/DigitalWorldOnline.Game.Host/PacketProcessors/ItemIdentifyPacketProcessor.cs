using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Services;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class ItemIdentifyPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.ItemIdentify;

        private readonly AccessoryEnchantService _accessoryEnchantService;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public ItemIdentifyPacketProcessor(
            AccessoryEnchantService accessoryEnchantService,
            ISender sender,
            ILogger logger)
        {
            _accessoryEnchantService = accessoryEnchantService;
            _sender = sender;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var tamerHandle = packet.ReadInt();
            var slot = packet.ReadShort();
            _logger.Information(
                "AccessoryCheck/ItemIdentify request: tamer={TamerId} handle={Handle} slot={Slot}",
                client.TamerId,
                tamerHandle,
                slot);

            var identifiedItem = client.Tamer.Inventory.FindItemBySlot(slot);
            if (identifiedItem == null || identifiedItem.ItemId == 0)
            {
                _logger.Warning("AccessoryCheck invalid item: tamer={TamerId} slot={Slot}", client.TamerId, slot);
                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new ItemIdentifyPacket(slot, new ItemModel()).Serialize(),
                        new SystemMessagePacket("Invalid item.").Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()));
                return;
            }

            var result = _accessoryEnchantService.Identify(identifiedItem);
            if (!result.Applied)
            {
                _logger.Warning(
                    "AccessoryCheck failed: tamer={TamerId} slot={Slot} item={ItemId} reason={Reason}",
                    client.TamerId,
                    slot,
                    identifiedItem.ItemId,
                    result.Message);

                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new ItemIdentifyPacket(slot, identifiedItem).Serialize(),
                        new SystemMessagePacket(result.Message).Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()));
                return;
            }

            await _sender.Send(new UpdateItemAccessoryStatusCommand(identifiedItem));

            var statusString = identifiedItem.AccessoryStatus
                .Where(x => x.Value > 0)
                .Select(x => $"{x.Type} {x.Value}");
            _logger.Verbose(
                "AccessoryCheck success: tamer={TamerId} slot={Slot} item={ItemId} power={Power} renewal={Renewal} statusAmount={StatusAmount} status={Status}",
                client.TamerId,
                slot,
                identifiedItem.ItemId,
                identifiedItem.Power,
                identifiedItem.RerollLeft,
                result.StatusAmount,
                string.Join(',', statusString));

            AccessoryParitySnapshot.LogItemSnapshot(
                _logger,
                "accessory-check-success",
                client.TamerId,
                slot,
                identifiedItem);

            client.Send(
                UtilitiesFunctions.GroupPackets(
                    new ItemIdentifyPacket(slot, identifiedItem).Serialize(),
                    new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()));
        }
    }
}
