using DigitalWorldOnline.Application;
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
    public class ItemRerollPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.ItemReroll;

        private readonly AccessoryEnchantService _accessoryEnchantService;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public ItemRerollPacketProcessor(
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
            var stoneSlot = packet.ReadShort();
            var accessorySlot = packet.ReadShort();

            byte selectedOptionSlot = 0;
            var remaining = (packet.Length - 2) - (int)packet.Packet.Position;
            if (remaining > 0)
                selectedOptionSlot = packet.ReadByte();

            byte lockedOptionMask = 0;
            remaining = (packet.Length - 2) - (int)packet.Packet.Position;
            if (remaining > 0)
                lockedOptionMask = packet.ReadByte();

            _logger.Information(
                "AccStoneUse/ItemReroll request: tamer={TamerId} handle={Handle} stoneSlot={StoneSlot} accessorySlot={AccessorySlot} selectedOptionSlot={SelectedOptionSlot} lockedOptionMask={LockedOptionMask}",
                client.TamerId,
                tamerHandle,
                stoneSlot,
                accessorySlot,
                selectedOptionSlot,
                lockedOptionMask);

            var stone = client.Tamer.Inventory.FindItemBySlot(stoneSlot);
            if (stone == null || stone.ItemId == 0)
            {
                _logger.Warning("AccStoneUse invalid stone: tamer={TamerId} stoneSlot={StoneSlot}", client.TamerId, stoneSlot);
                SendRerollFallback(client, 2, accessorySlot, "Invalid accessory enchant stone.");
                return;
            }

            var targetAccessory = client.Tamer.Inventory.FindItemBySlot(accessorySlot);
            if (targetAccessory == null || targetAccessory.ItemId == 0)
            {
                _logger.Warning("AccStoneUse invalid accessory: tamer={TamerId} accessorySlot={AccessorySlot}", client.TamerId, accessorySlot);
                client.Send(
                    UtilitiesFunctions.GroupPackets(
                        new ItemRerollPacket(2, accessorySlot, new ItemModel()).Serialize(),
                        new SystemMessagePacket("Invalid accessory.").Serialize(),
                        new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()));
                return;
            }

            var result = _accessoryEnchantService.Enchant(stone, targetAccessory, selectedOptionSlot, lockedOptionMask);
            if (result.ConsumeStone)
                client.Tamer.Inventory.RemoveOrReduceItem(stone, result.StoneAmount);

            await _sender.Send(new UpdateItemAccessoryStatusCommand(targetAccessory));
            await _sender.Send(new UpdateItemCommand(stone));

            var statusString = targetAccessory.AccessoryStatus
                .Where(x => x.Value > 0)
                .Select(x => $"{x.Type} {x.Value}");
            _logger.Verbose(
                "AccStoneUse result: tamer={TamerId} stone={StoneItemId} accessory={AccessoryItemId} result={Result} consumed={Consumed} power={Power} renewal={Renewal} status={Status} message={Message}",
                client.TamerId,
                stone.ItemId,
                targetAccessory.ItemId,
                result.Result,
                result.ConsumeStone,
                targetAccessory.Power,
                targetAccessory.RerollLeft,
                string.Join(',', statusString),
                result.Message);

            AccessoryParitySnapshot.LogItemSnapshot(
                _logger,
                "acc-stone-use-result",
                client.TamerId,
                accessorySlot,
                targetAccessory);

            var packets = new List<byte[]>
            {
                new ItemRerollPacket(result.Result, accessorySlot, targetAccessory).Serialize(),
                new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
            };

            if (!result.Applied)
                packets.Add(new SystemMessagePacket(result.Message).Serialize());

            client.Send(UtilitiesFunctions.GroupPackets(packets.ToArray()));
        }

        private static void SendRerollFallback(GameClient client, byte result, short accessorySlot, string message)
        {
            var accessorySnapshot = client.Tamer.Inventory.FindItemBySlot(accessorySlot) ?? new ItemModel();
            client.Send(
                UtilitiesFunctions.GroupPackets(
                    new ItemRerollPacket(result, accessorySlot, accessorySnapshot).Serialize(),
                    new SystemMessagePacket(message).Serialize(),
                    new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()));
        }
    }
}
