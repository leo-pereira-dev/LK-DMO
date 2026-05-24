using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class ExtraInventoryCollectAllPacketProcessor : IGamePacketProcessor
    {
        private const int CategorySeal = 0;
        private const int CategoryTicket = 1;
        private const int CategoryEvolution = 2;
        private const int CategoryDigitama = 3;
        private const int CategoryMaterial = 4;

        private static readonly IReadOnlySet<int> SealTypes = new HashSet<int> { 190, 191, 192 };
        private static readonly IReadOnlySet<int> TicketTypes = new HashSet<int> { 100, 178, 185 };
        private static readonly IReadOnlySet<int> EvolutionTypes = new HashSet<int> { 150, 162, 193, 195, 196, 400 };
        private static readonly IReadOnlySet<int> DigitamaTypes = new HashSet<int> { 91, 92, 93, 95, 96, 97, 177, 188 };
        private static readonly IReadOnlySet<int> MaterialTypes = new HashSet<int> { 80, 81, 82, 83, 84, 85, 86, 161, 176, 187 };

        public GameServerPacketEnum Type => GameServerPacketEnum.ExtraInventoryCollectAll;

        private readonly ISender _sender;
        private readonly ItemListBinLoader _itemListBinLoader;
        private readonly ILogger _logger;

        public ExtraInventoryCollectAllPacketProcessor(
            ISender sender,
            ItemListBinLoader itemListBinLoader,
            ILogger logger)
        {
            _sender = sender;
            _itemListBinLoader = itemListBinLoader;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var category = packet.ReadByte();

            var target = ResolveTargetList(client, category, out var inventoryType);
            if (target == null)
            {
                _logger.Warning(
                    "Extra inventory collect-all rejected for tamer {TamerId}: invalid category {Category}.",
                    client.TamerId,
                    category);
                return;
            }

            _logger.Information(
                "Extra inventory collect-all request for tamer {TamerId}: category={Category} inventoryStacks={InventoryStacks} targetStacks={TargetStacks}.",
                client.TamerId,
                category,
                client.Tamer.Inventory.Items.Count(x => x.ItemId > 0 && x.Amount > 0),
                target.Items.Count(x => x.ItemId > 0 && x.Amount > 0));

            var moved = MoveEligibleItems(client.Tamer.Inventory, target, category);
            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateItemsCommand(target));

            client.Send(
                UtilitiesFunctions.GroupPackets(
                    new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize(),
                    new LoadInventoryPacket(target, inventoryType).Serialize()));

            _logger.Information(
                "Extra inventory collect-all finished for tamer {TamerId}: category={Category} movedStacks={MovedStacks}.",
                client.TamerId,
                category,
                moved);

            if (moved == 0)
            {
                _logger.Warning(
                    "Extra inventory collect-all moved no stacks for tamer {TamerId}: category={Category} inventorySample={InventorySample}.",
                    client.TamerId,
                    category,
                    DescribeInventorySample(client.Tamer.Inventory, category));
            }
        }

        private static ItemListModel? ResolveTargetList(
            GameClient client,
            int category,
            out InventoryTypeEnum inventoryType)
        {
            switch (category)
            {
                case CategorySeal:
                    inventoryType = InventoryTypeEnum.ExtraSeal;
                    return client.Tamer.ExtraInventorySeal;

                case CategoryTicket:
                    inventoryType = InventoryTypeEnum.ExtraTicket;
                    return client.Tamer.ExtraInventoryTicket;

                case CategoryEvolution:
                    inventoryType = InventoryTypeEnum.ExtraEvolution;
                    return client.Tamer.ExtraInventoryEvolution;

                case CategoryDigitama:
                    inventoryType = InventoryTypeEnum.ExtraDigitama;
                    return client.Tamer.ExtraInventoryDigitama;

                case CategoryMaterial:
                    inventoryType = InventoryTypeEnum.ExtraMaterial;
                    return client.Tamer.ExtraInventoryMaterial;

                default:
                    inventoryType = InventoryTypeEnum.Inventory;
                    return null;
            }
        }

        private int MoveEligibleItems(ItemListModel source, ItemListModel target, int category)
        {
            var moved = 0;
            var sourceItems = source.Items
                .Where(x => x.ItemId > 0 && x.Amount > 0)
                .OrderBy(x => x.Slot)
                .ToList();

            foreach (var sourceItem in sourceItems)
            {
                var type = ResolveTypeL(sourceItem);
                if (!IsAllowedType(category, type))
                    continue;

                var itemToMove = (ItemModel)sourceItem.Clone();
                if (!target.AddItem(itemToMove))
                    continue;

                if (!source.RemoveOrReduceItemWithSlot((ItemModel)sourceItem.Clone(), sourceItem.Slot))
                    continue;

                moved++;
            }

            source.CheckEmptyItems();
            target.CheckEmptyItems();
            return moved;
        }

        private int ResolveTypeL(ItemModel item)
        {
            if (item.ItemInfo != null)
                return item.ItemInfo.Type;

            return _itemListBinLoader.Data.Items
                .FirstOrDefault(x => x.ItemId == item.ItemId)
                ?.Type ?? 0;
        }

        private string DescribeInventorySample(ItemListModel source, int category)
        {
            var items = source.Items
                .Where(x => x.ItemId > 0 && x.Amount > 0)
                .OrderBy(x => x.Slot)
                .Take(20)
                .Select(x =>
                {
                    var type = ResolveTypeL(x);
                    var allowed = IsAllowedType(category, type) ? "allowed" : "blocked";
                    return $"{x.Slot}:{x.ItemId}x{x.Amount}/type={type}/{allowed}";
                });

            return string.Join("; ", items);
        }

        private static bool IsAllowedType(int category, int type)
        {
            return category switch
            {
                CategorySeal => SealTypes.Contains(type),
                CategoryTicket => TicketTypes.Contains(type),
                CategoryEvolution => EvolutionTypes.Contains(type),
                CategoryDigitama => DigitamaTypes.Contains(type),
                CategoryMaterial => MaterialTypes.Contains(type),
                _ => false
            };
        }
    }
}
