using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class NpcPurchasePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.NpcItemPurchase;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public NpcPurchasePacketProcessor(
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
            try
            {
                var packet = new GamePacketReader(packetData);
                var npcId = packet.ReadInt();
                var unk = packet.ReadByte();
                var shopSlot = packet.ReadInt();
                var purchaseCount = packet.ReadShort();

                _logger.Information(
                    "NPC purchase request: tamer {TamerId} map {MapId} npc {NpcId} slot {ShopSlot} count {Count} marker {Marker}.",
                    client.TamerId,
                    client.Tamer.Location.MapId,
                    npcId,
                    shopSlot,
                    purchaseCount,
                    unk);

                var npc = _assets.Npcs.FirstOrDefault(x => x.NpcId == npcId && x.MapId == client.Tamer.Location.MapId);
                if (npc == null)
                {
                    client.Send(new SystemMessagePacket($"NPC Shop not found."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Error($"Unknown NPC id {npcId} at map {client.Tamer.Location.MapId}.");
                    return;
                }

                if (purchaseCount <= 0)
                {
                    client.Send(new SystemMessagePacket($"Invalid purchase amount."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Warning($"Invalid NPC purchase amount {purchaseCount} from NPC {npcId} at {client.TamerLocation}.");
                    return;
                }

                if (shopSlot < 0 || shopSlot >= npc.Items.Count)
                {
                    client.Send(new SystemMessagePacket($"NPC Shop item not found."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Error($"Invalid NPC shop slot {shopSlot} for NPC {npcId} at map {client.Tamer.Location.MapId}. Item count: {npc.Items.Count}.");
                    return;
                }

                var npcItem = npc.Items[shopSlot];
                var itemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == npcItem.ItemId);
                if (itemInfo == null)
                {
                    client.Send(new SystemMessagePacket($"NPC Shop item data not found."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Error($"NPC shop item {npcItem.ItemId} from NPC {npcId} has no ItemInfo at {client.TamerLocation}.");
                    return;
                }

                var purchasingItem = new ItemModel();
                purchasingItem.SetItemId(npcItem.ItemId);
                purchasingItem.SetAmount(purchaseCount);
                purchasingItem.SetItemInfo(itemInfo);

                if (client.Tamer.Inventory.FindAvailableSlot(purchasingItem) < 0)
                {
                    client.Send(new SystemMessagePacket($"Not enough inventory space."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Warning($"No inventory slot to buy item {npcItem.ItemId} x{purchaseCount} from NPC {npcId} at {client.TamerLocation}.");
                    return;
                }

                if (purchasingItem.IsTemporary)
                    purchasingItem.SetRemainingTime((uint)purchasingItem.ItemInfo.UsageTimeMinutes);

                if (purchasingItem.ItemInfo!.EventPriceId > 0)
                {
                    var eventItemPrice = purchasingItem.ItemInfo.EventPriceAmount * purchaseCount;
                    if (!client.Tamer.Inventory.RemoveOrReduceItemsByItemId(purchasingItem.ItemInfo.EventPriceId, eventItemPrice))
                    {
                        client.Send(new SystemMessagePacket($"Insufficient required item amount."));
                        client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                        _logger.Warning($"Insufficient required item amount to buy item {npcItem.ItemId} from NPC {npcId} at {client.TamerLocation}.");
                        return;
                    }

                    _logger.Verbose($"Character {client.TamerId} purchased {purchasingItem.ItemId} x{purchasingItem.Amount} for " +
                        $"{purchasingItem.ItemInfo.EventPriceId} x{eventItemPrice} from NPC {npcId} at {client.TamerLocation}.");
                }
                else if (purchasingItem.ItemInfo.DigicorePrice > 0)
                {
                    var digicorePrice = purchasingItem.ItemInfo.DigicorePrice * purchaseCount;
                    if (!client.Tamer.Inventory.RemoveOrReduceItemsBySection(16100, digicorePrice))
                    {
                        client.Send(new SystemMessagePacket($"Insufficient digicore amount."));
                        client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                        _logger.Warning($"Insufficient digicore amount to buy item {npcItem.ItemId} from NPC {npcId} at {client.TamerLocation}.");
                        return;
                    }

                    _logger.Verbose($"Character {client.TamerId} purchased {purchasingItem.ItemId} x{purchasingItem.Amount} for " +
                        $"{digicorePrice} digicore from NPC {npcId} at {client.TamerLocation}.");
                }
                else
                {
                    var bitsPrice = purchasingItem.ItemInfo.ScanPrice * purchaseCount;
                    if (client.Tamer.Inventory.Bits < bitsPrice)
                    {
                        client.Send(new SystemMessagePacket($"Insufficient bits amount."));
                        client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                        _logger.Warning($"Insufficient bits amount to buy item {npcItem.ItemId} from NPC {npcId} at {client.TamerLocation}. Required: {bitsPrice}, current: {client.Tamer.Inventory.Bits}.");
                        return;
                    }

                    if (!client.Tamer.Inventory.RemoveBits(bitsPrice))
                    {
                        client.Send(new SystemMessagePacket($"Insufficient bits amount."));
                        client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                        _logger.Warning($"Insufficient bits amount to buy item {npcItem.ItemId} from NPC {npcId} at {client.TamerLocation}.");
                        return;
                    }

                    _logger.Verbose($"Character {client.TamerId} purchased {purchasingItem.ItemId} x{purchasingItem.Amount} for " +
                        $"{bitsPrice} bits from NPC {npcId} at {client.TamerLocation}.");
                }
                
                if (!client.Tamer.Inventory.AddItem(purchasingItem))
                {
                    if (purchasingItem.ItemInfo.EventPriceId <= 0 && purchasingItem.ItemInfo.DigicorePrice <= 0)
                        client.Tamer.Inventory.AddBits(purchasingItem.ItemInfo.ScanPrice * purchaseCount);

                    client.Send(new SystemMessagePacket($"Not enough inventory space."));
                    client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
                    _logger.Warning($"Failed to add purchased item {npcItem.ItemId} x{purchaseCount} from NPC {npcId} at {client.TamerLocation}.");
                    return;
                }

                client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits, purchasingItem));

                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
            }
            catch (Exception ex)
            {
                _logger.Error(ex, "NPC purchase crashed for tamer {TamerId} at {Location}.", client.TamerId, client.TamerLocation);
                client.Send(new NpcItemPurchasePacket(client.Tamer.Inventory.Bits));
            }
        }
    }
}
