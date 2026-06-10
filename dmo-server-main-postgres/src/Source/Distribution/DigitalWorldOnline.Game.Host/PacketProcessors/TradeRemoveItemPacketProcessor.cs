using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.GameHost;
using Serilog;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class TraRemoveItemPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.TradeRemoveItem;

        private readonly MapServer _mapServer;
        private readonly ILogger _logger;


        public TraRemoveItemPacketProcessor(
            MapServer mapServer,
            ILogger logger)
        {
            _mapServer = mapServer;
            _logger = logger;

        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var tradeSlot = packet.ReadByte();

            _logger.Information("TradeRemoveItem request. Character={Client} TargetHandle={TargetHandle} TradeSlot={TradeSlot}",
                client.TamerId,
                client.Tamer.TargetTradeGeneralHandle,
                tradeSlot);

            var targetClient = _mapServer.FindClientByTamerHandle(client.Tamer.TargetTradeGeneralHandle);
            if (targetClient == null)
            {
                _logger.Warning("TradeRemoveItem abort: target not found. Character={Client} TradeSlot={TradeSlot}",
                    client.TamerId,
                    tradeSlot);

                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                return;
            }



            var Item = client.Tamer.TradeInventory.FindItemByTradeSlot(tradeSlot);

            if (Item != null)
            {
                if (client.Tamer.TryGetTradeSlotSource(tradeSlot, out var sourceSlot))
                {
                    _logger.Information("TradeRemoveItem source slot found. Character={Client} TradeSlot={TradeSlot} SourceSlot={SourceSlot}",
                        client.TamerId,
                        tradeSlot,
                        sourceSlot);

                    client.Tamer.RemoveReservedTradeItem(sourceSlot, Item.Amount);
                    client.Tamer.RemoveTradeSlotSource(tradeSlot);
                }

                    client.Tamer.TradeInventory.RemoveOrReduceItem(Item, Item.Amount);
                var remappedSources = new Dictionary<int, int>();
                var nextTradeSlot = 0;
                foreach (var tradeItem in client.Tamer.TradeInventory.EquippedItems.OrderBy(x => x.Slot))
                {
                    if (client.Tamer.TryGetTradeSlotSource(tradeItem.TradeSlot, out var mappedSourceSlot))
                        remappedSources[nextTradeSlot] = mappedSourceSlot;

                    tradeItem.SetTradeSlot(nextTradeSlot);
                    nextTradeSlot++;
                }
                client.Tamer.ReplaceTradeSlotSourceMap(remappedSources);

                _logger.Information("TradeRemoveItem success. Character={Client} SourceTradeSlot={TradeSlot}",
                    client.TamerId,
                    tradeSlot);

                client.Send(new TradeRemoveItemPacket(client.Tamer.GeneralHandler, (byte)tradeSlot));
                targetClient.Send(new TradeRemoveItemPacket(client.Tamer.GeneralHandler, (byte)tradeSlot));
                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                targetClient.Send(new LoadInventoryPacket(targetClient.Tamer.Inventory, InventoryTypeEnum.Inventory));
            }
            else
            {
                _logger.Warning("TradeRemoveItem failed: item not found on trade. Character={Client} TradeSlot={TradeSlot}",
                    client.TamerId,
                    tradeSlot);

                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
            }

            //_logger.Verbose($"Character {client.TamerId} and {targetClient.TamerId} inventory unlock "); ;

        }


    }
}
