using AutoMapper;
using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class ItemMovePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.MoveItem;

        private readonly MapServer _mapServer;
        private readonly ISender _sender;
        private readonly ILogger _logger;
        private readonly IMapper _mapper;
        private readonly DungeonsServer _dungeonServer;
        private readonly ItemListBinLoader _itemListBinLoader;

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

        public ItemMovePacketProcessor(
            MapServer mapServer,
            ISender sender,
            ILogger logger,
            DungeonsServer dungeonsServer,
            IMapper mapper,
            ItemListBinLoader itemListBinLoader)
        {
            _mapServer = mapServer;
            _sender = sender;
            _logger = logger;
            _dungeonServer = dungeonsServer;
            _mapper = mapper;
            _itemListBinLoader = itemListBinLoader;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var originSlot = packet.ReadShort();
            var destinationSlot = packet.ReadShort();

            var itemListMovimentation = UtilitiesFunctions.SwitchItemList(originSlot, destinationSlot);
            if (IsExtraInventoryMovement(itemListMovimentation))
            {
                _logger.Information(
                    "ExtraInventory manual move request: tamer={TamerId} origin={OriginSlot} destination={DestinationSlot} movement={Movement}",
                    client.TamerId,
                    originSlot,
                    destinationSlot,
                    itemListMovimentation);
            }

            var success = SwapItems(client, originSlot, destinationSlot, itemListMovimentation);
            if (!success)
            {
                _logger.Warning(
                    "ItemMove failed for tamer {TamerId}: origin={OriginSlot} destination={DestinationSlot} movement={Movement}",
                    client.TamerId, originSlot, destinationSlot, itemListMovimentation);
            }

            if (success)
            {
                switch (itemListMovimentation)
                {
                    case ItemListMovimentationEnum.InventoryToInventory:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                        }
                        break;

                    case ItemListMovimentationEnum.EquipmentToInventory:
                    case ItemListMovimentationEnum.InventoryToEquipment:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.Equipment.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Equipment));
                    
                            
                        }
                        break;

                    case ItemListMovimentationEnum.InventoryToDigivice:
                    case ItemListMovimentationEnum.DigiviceToInventory:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.Digivice.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Digivice));
                        }
                        break;

                    case ItemListMovimentationEnum.InventoryToChipset:
                    case ItemListMovimentationEnum.ChipsetToInventory:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.ChipSets.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.ChipSets));
                        }
                        break;
                    case ItemListMovimentationEnum.InventoryToJogressChipset:
                    case ItemListMovimentationEnum.JogressChipsetToInventory:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.JogressChipSet.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.JogressChipSet));
                        }
                        break;

                    case ItemListMovimentationEnum.WarehouseToInventory:
                    case ItemListMovimentationEnum.InventoryToWarehouse:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.Warehouse.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Warehouse));
                        }
                        break;

                    case ItemListMovimentationEnum.AccountWarehouseToInventory:
                    case ItemListMovimentationEnum.InventoryToAccountWarehouse:
                        {
                            client.Tamer.Inventory.CheckEmptyItems();
                            client.Tamer.AccountWarehouse.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.AccountWarehouse));
                        }
                        break;

                    case ItemListMovimentationEnum.AccountWarehouseToWarehouse:
                    case ItemListMovimentationEnum.WarehouseToAccountWarehouse:
                        {
                            client.Tamer.AccountWarehouse.CheckEmptyItems();
                            client.Tamer.Warehouse.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Warehouse));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.AccountWarehouse));
                        }
                        break;

                    case ItemListMovimentationEnum.AccountWarehouseToAccountWarehouse:
                        {
                            client.Tamer.AccountWarehouse.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.AccountWarehouse));
                        }
                        break;

                    case ItemListMovimentationEnum.InventoryToExtraSeal:
                    case ItemListMovimentationEnum.ExtraSealToInventory:
                    case ItemListMovimentationEnum.ExtraSealToExtraSeal:
                        await UpdateExtraInventoryMoveAsync(client.Tamer.Inventory, client.Tamer.ExtraInventorySeal, itemListMovimentation);
                        break;

                    case ItemListMovimentationEnum.InventoryToExtraTicket:
                    case ItemListMovimentationEnum.ExtraTicketToInventory:
                    case ItemListMovimentationEnum.ExtraTicketToExtraTicket:
                        await UpdateExtraInventoryMoveAsync(client.Tamer.Inventory, client.Tamer.ExtraInventoryTicket, itemListMovimentation);
                        break;

                    case ItemListMovimentationEnum.InventoryToExtraEvolution:
                    case ItemListMovimentationEnum.ExtraEvolutionToInventory:
                    case ItemListMovimentationEnum.ExtraEvolutionToExtraEvolution:
                        await UpdateExtraInventoryMoveAsync(client.Tamer.Inventory, client.Tamer.ExtraInventoryEvolution, itemListMovimentation);
                        break;

                    case ItemListMovimentationEnum.InventoryToExtraDigitama:
                    case ItemListMovimentationEnum.ExtraDigitamaToInventory:
                    case ItemListMovimentationEnum.ExtraDigitamaToExtraDigitama:
                        await UpdateExtraInventoryMoveAsync(client.Tamer.Inventory, client.Tamer.ExtraInventoryDigitama, itemListMovimentation);
                        break;

                    case ItemListMovimentationEnum.InventoryToExtraMaterial:
                    case ItemListMovimentationEnum.ExtraMaterialToInventory:
                    case ItemListMovimentationEnum.ExtraMaterialToExtraMaterial:
                        await UpdateExtraInventoryMoveAsync(client.Tamer.Inventory, client.Tamer.ExtraInventoryMaterial, itemListMovimentation);
                        break;

                    case ItemListMovimentationEnum.WarehouseToWarehouse:
                        {
                            client.Tamer.Warehouse.CheckEmptyItems();
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Warehouse));
                        }
                        break;
                }

                client.Send(
                    UtilitiesFunctions.GroupPackets(BuildMoveResultPackets(client, itemListMovimentation, true, originSlot, destinationSlot))
                );

                if (originSlot == GeneralSizeEnum.XaiSlot.GetHashCode())
                {
                    client.Tamer.Xai.RemoveXai();
                    client.Send(new XaiInfoPacket());
                    client.Send(new TamerXaiResourcesPacket(0, 0));
                    await _sender.Send(new UpdateCharacterXaiCommand(client.Tamer.Xai));
                }

                if (destinationSlot == GeneralSizeEnum.XaiSlot.GetHashCode())
                {
                    var equippedItem = client.Tamer.Equipment.FindItemBySlot(destinationSlot - 1000);
                    if (equippedItem == null || equippedItem.ItemId <= 0)
                    {
                        _logger.Warning("Character {TamerId} failed XAI equip: empty slot item.", client.TamerId);
                        return;
                    }

                    var itemId = equippedItem.ItemId;
                    var xaiDto = await _sender.Send(new XaiInformationQuery(itemId));
                    if (xaiDto == null)
                    {
                        _logger.Warning("Character {TamerId} failed XAI equip: item {ItemId} has no XAI asset row.", client.TamerId, itemId);
                        return;
                    }

                    var XaiInfo = _mapper.Map<XaiAssetModel>(xaiDto);

                    client.Tamer.Xai.EquipXai(XaiInfo.ItemId, XaiInfo.XGauge, XaiInfo.XCrystals);
                    client.Tamer.ClampXaiResourcesToCap();

                    client.Send(new XaiInfoPacket(client.Tamer.Xai));
                    client.Send(new TamerXaiResourcesPacket(client.Tamer.XGauge, client.Tamer.XCrystals));

                    await _sender.Send(new UpdateCharacterXaiCommand(client.Tamer.Xai));
                }

                if (IsDigimonStatAffectingMove(itemListMovimentation))
                {
                    LogDigimonStatSnapshot(client, originSlot, destinationSlot, itemListMovimentation);
                    LogEquipmentPacketSnapshot(client, originSlot, destinationSlot, itemListMovimentation);
                    AccessoryParitySnapshot.LogEquippedSnapshot(
                        _logger,
                        $"equip-move:{itemListMovimentation}",
                        client.TamerId,
                        client.Tamer.Equipment.EquippedItems);
                }

                _logger.Verbose($"Character {client.TamerId} moved an item from {originSlot} to {destinationSlot}.");
            }
            else
            {
                client.Send(
                    UtilitiesFunctions.GroupPackets(BuildMoveResultPackets(client, itemListMovimentation, false, originSlot, destinationSlot))
                );

                _logger.Warning($"Character {client.TamerId} failled to move item from {originSlot} to {destinationSlot}.");
            }
        }

        private static bool IsDigimonStatAffectingMove(ItemListMovimentationEnum movimentation)
        {
            return movimentation is ItemListMovimentationEnum.EquipmentToInventory
                or ItemListMovimentationEnum.InventoryToEquipment
                or ItemListMovimentationEnum.InventoryToDigivice
                or ItemListMovimentationEnum.DigiviceToInventory
                or ItemListMovimentationEnum.InventoryToChipset
                or ItemListMovimentationEnum.ChipsetToInventory
                or ItemListMovimentationEnum.InventoryToJogressChipset
                or ItemListMovimentationEnum.JogressChipsetToInventory;
        }

        private static bool IsExtraInventoryMovement(ItemListMovimentationEnum movimentation)
        {
            return movimentation is ItemListMovimentationEnum.InventoryToExtraSeal
                or ItemListMovimentationEnum.ExtraSealToInventory
                or ItemListMovimentationEnum.ExtraSealToExtraSeal
                or ItemListMovimentationEnum.InventoryToExtraTicket
                or ItemListMovimentationEnum.ExtraTicketToInventory
                or ItemListMovimentationEnum.ExtraTicketToExtraTicket
                or ItemListMovimentationEnum.InventoryToExtraEvolution
                or ItemListMovimentationEnum.ExtraEvolutionToInventory
                or ItemListMovimentationEnum.ExtraEvolutionToExtraEvolution
                or ItemListMovimentationEnum.InventoryToExtraDigitama
                or ItemListMovimentationEnum.ExtraDigitamaToInventory
                or ItemListMovimentationEnum.ExtraDigitamaToExtraDigitama
                or ItemListMovimentationEnum.InventoryToExtraMaterial
                or ItemListMovimentationEnum.ExtraMaterialToInventory
                or ItemListMovimentationEnum.ExtraMaterialToExtraMaterial;
        }

        private async Task UpdateExtraInventoryMoveAsync(
            ItemListModel inventory,
            ItemListModel extraInventory,
            ItemListMovimentationEnum movimentation)
        {
            inventory.CheckEmptyItems();
            extraInventory.CheckEmptyItems();
            await _sender.Send(new UpdateItemsCommand(inventory));
            await _sender.Send(new UpdateItemsCommand(extraInventory));
        }

        private void LogDigimonStatSnapshot(
            GameClient client,
            short originSlot,
            short destinationSlot,
            ItemListMovimentationEnum movimentation)
        {
            var partner = client.Partner;
            if (partner == null)
            {
                _logger.Warning(
                    "ItemMove stat snapshot skipped: tamer={TamerId} movement={Movement} origin={OriginSlot} destination={DestinationSlot} reason=NoPartner",
                    client.TamerId, movimentation, originSlot, destinationSlot);
                return;
            }

            _logger.Information(
                "ItemMove stat snapshot: tamer={TamerId} partnerId={PartnerId} partnerName={PartnerName} movement={Movement} origin={OriginSlot} destination={DestinationSlot} HP={HP} DS={DS} AT={AT} DE={DE} AS={AS} HT={HT} BL={BL} CT={CT} EV={EV} CD={CD} ATT={ATT} SCD={SCD}",
                client.TamerId,
                partner.Id,
                partner.Name,
                movimentation,
                originSlot,
                destinationSlot,
                partner.HP,
                partner.DS,
                partner.AT,
                partner.DE,
                partner.AS,
                partner.HT,
                partner.BL,
                partner.CC,
                partner.EV,
                partner.CD,
                partner.ATT,
                partner.SCD);
        }

        private static byte[][] BuildMoveResultPackets(
            GameClient client,
            ItemListMovimentationEnum movimentation,
            bool success,
            short originSlot,
            short destinationSlot)
        {
            var packets = new List<byte[]>
            {
                success
                    ? new ItemMoveSuccessPacket(originSlot, destinationSlot).Serialize()
                    : new ItemMoveFailPacket(originSlot, destinationSlot).Serialize(),
                new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory).Serialize()
            };

            if (movimentation is ItemListMovimentationEnum.InventoryToWarehouse or
                ItemListMovimentationEnum.WarehouseToWarehouse or
                ItemListMovimentationEnum.WarehouseToInventory or
                ItemListMovimentationEnum.WarehouseToAccountWarehouse or
                ItemListMovimentationEnum.AccountWarehouseToWarehouse)
            {
                packets.Add(new LoadInventoryPacket(client.Tamer.Warehouse, InventoryTypeEnum.Warehouse).Serialize());
            }

            if (movimentation is ItemListMovimentationEnum.InventoryToAccountWarehouse or
                ItemListMovimentationEnum.WarehouseToAccountWarehouse or
                ItemListMovimentationEnum.AccountWarehouseToAccountWarehouse or
                ItemListMovimentationEnum.AccountWarehouseToInventory or
                ItemListMovimentationEnum.AccountWarehouseToWarehouse)
            {
                packets.Add(new LoadInventoryPacket(client.Tamer.AccountWarehouse, InventoryTypeEnum.AccountWarehouse).Serialize());
            }

            AppendExtraInventoryPackets(packets, client, movimentation);

            return packets.ToArray();
        }

        private static void AppendExtraInventoryPackets(
            List<byte[]> packets,
            GameClient client,
            ItemListMovimentationEnum movimentation)
        {
            switch (movimentation)
            {
                case ItemListMovimentationEnum.InventoryToExtraSeal:
                case ItemListMovimentationEnum.ExtraSealToInventory:
                case ItemListMovimentationEnum.ExtraSealToExtraSeal:
                    packets.Add(new LoadInventoryPacket(client.Tamer.ExtraInventorySeal, InventoryTypeEnum.ExtraSeal).Serialize());
                    break;

                case ItemListMovimentationEnum.InventoryToExtraTicket:
                case ItemListMovimentationEnum.ExtraTicketToInventory:
                case ItemListMovimentationEnum.ExtraTicketToExtraTicket:
                    packets.Add(new LoadInventoryPacket(client.Tamer.ExtraInventoryTicket, InventoryTypeEnum.ExtraTicket).Serialize());
                    break;

                case ItemListMovimentationEnum.InventoryToExtraEvolution:
                case ItemListMovimentationEnum.ExtraEvolutionToInventory:
                case ItemListMovimentationEnum.ExtraEvolutionToExtraEvolution:
                    packets.Add(new LoadInventoryPacket(client.Tamer.ExtraInventoryEvolution, InventoryTypeEnum.ExtraEvolution).Serialize());
                    break;

                case ItemListMovimentationEnum.InventoryToExtraDigitama:
                case ItemListMovimentationEnum.ExtraDigitamaToInventory:
                case ItemListMovimentationEnum.ExtraDigitamaToExtraDigitama:
                    packets.Add(new LoadInventoryPacket(client.Tamer.ExtraInventoryDigitama, InventoryTypeEnum.ExtraDigitama).Serialize());
                    break;

                case ItemListMovimentationEnum.InventoryToExtraMaterial:
                case ItemListMovimentationEnum.ExtraMaterialToInventory:
                case ItemListMovimentationEnum.ExtraMaterialToExtraMaterial:
                    packets.Add(new LoadInventoryPacket(client.Tamer.ExtraInventoryMaterial, InventoryTypeEnum.ExtraMaterial).Serialize());
                    break;
            }
        }

        private bool SwapItems(GameClient client, short originSlot, short destinationSlot, ItemListMovimentationEnum itemListMovimentation)
        {
            switch (itemListMovimentation)
            {
                case ItemListMovimentationEnum.InventoryToInventory:
                    return client.Tamer.Inventory.MoveItem(originSlot, destinationSlot);

                case ItemListMovimentationEnum.InventoryToDigivice:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.DigiviceSlot.GetHashCode();
                        if (!client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.Digivice, srcSlot, dstSlot))
                            return false;

                        var equippedItem = client.Tamer.Digivice.FindItemBySlot(dstSlot) ?? new ItemModel();
                        BroadcastAppearanceUpdate(client, 13, equippedItem, 1);
                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.ChipsetToInventory:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.ChipsetMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        if (!client.Tamer.ChipSets.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot))
                            return false;

                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.InventoryToChipset:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.ChipsetMinSlot.GetHashCode();
                        if (!client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.ChipSets, srcSlot, dstSlot))
                            return false;

                        SendStatusAndSpeed(client);
                        return true;
                    }
                case ItemListMovimentationEnum.InventoryToJogressChipset:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        const int dstSlot = 0;
                        if (!client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.JogressChipSet, srcSlot, dstSlot))
                            return false;

                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.JogressChipsetToInventory:
                    {
                        const int srcSlot = 0;
                        var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        if (!client.Tamer.JogressChipSet.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot))
                            return false;

                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.InventoryToEquipment:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        var dstSlot = destinationSlot == GeneralSizeEnum.XaiSlot.GetHashCode() ? 11 : destinationSlot - GeneralSizeEnum.EquipmentMinSlot.GetHashCode();
                        var srcItemBefore = client.Tamer.Inventory.FindItemBySlot(srcSlot);
                        var dstItemBefore = client.Tamer.Equipment.FindItemBySlot(dstSlot);
                        if (!client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.Equipment, srcSlot, dstSlot))
                        {
                            _logger.Warning(
                                "InventoryToEquipment rejected: tamer={TamerId} srcSlot={SrcSlot} dstSlot={DstSlot} invSize={InvSize} equipSize={EquipSize} srcExists={SrcExists} srcItemId={SrcItemId} srcAmount={SrcAmount} dstExists={DstExists} dstItemId={DstItemId} dstAmount={DstAmount}",
                                client.TamerId,
                                srcSlot,
                                dstSlot,
                                client.Tamer.Inventory.Size,
                                client.Tamer.Equipment.Size,
                                srcItemBefore != null,
                                srcItemBefore?.ItemId ?? 0,
                                srcItemBefore?.Amount ?? 0,
                                dstItemBefore != null,
                                dstItemBefore?.ItemId ?? 0,
                                dstItemBefore?.Amount ?? 0);
                            return false;
                        }

                        var equippedItem = client.Tamer.Equipment.FindItemBySlot(dstSlot) ?? new ItemModel();
                        BroadcastAppearanceUpdate(client, (byte)dstSlot, equippedItem, 1);
                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.InventoryToWarehouse:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.WarehouseMinSlot.GetHashCode();
                        return client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.Warehouse, srcSlot, dstSlot);
                    }

                case ItemListMovimentationEnum.InventoryToAccountWarehouse:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.AccountWarehouseMinSlot.GetHashCode();
                        var sourceItem = client.Tamer.Inventory.FindItemBySlot(srcSlot);
                        var destinationItem = client.Tamer.AccountWarehouse.FindItemBySlot(dstSlot);
                        var moved = client.Tamer.Inventory.TryMoveAcrossLists(client.Tamer.AccountWarehouse, srcSlot, dstSlot);
                        if (!moved)
                        {
                            _logger.Warning(
                                "InventoryToAccountWarehouse rejected: tamer={TamerId} srcSlot={SrcSlot} dstSlot={DstSlot} invSize={InvSize} accSize={AccSize} srcExists={SrcExists} srcItemId={SrcItemId} srcAmount={SrcAmount} dstExists={DstExists} dstItemId={DstItemId} dstAmount={DstAmount}",
                                client.TamerId,
                                srcSlot,
                                dstSlot,
                                client.Tamer.Inventory.Size,
                                client.Tamer.AccountWarehouse.Size,
                                sourceItem != null,
                                sourceItem?.ItemId ?? 0,
                                sourceItem?.Amount ?? 0,
                                destinationItem != null,
                                destinationItem?.ItemId ?? 0,
                                destinationItem?.Amount ?? 0);
                        }

                        return moved;
                    }

                case ItemListMovimentationEnum.EquipmentToInventory:
                    {
                        var srcSlot = originSlot == GeneralSizeEnum.XaiSlot.GetHashCode() ? 11 : originSlot - GeneralSizeEnum.EquipmentMinSlot.GetHashCode();
                        var dstSlot = destinationSlot;
                        var destinationItem = client.Tamer.Inventory.FindItemBySlot(dstSlot);
                        var hadDestinationItem = destinationItem?.ItemId > 0;
                        if (!client.Tamer.Equipment.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot))
                            return false;

                        var equippedItem = client.Tamer.Equipment.FindItemBySlot(srcSlot) ?? new ItemModel();
                        BroadcastAppearanceUpdate(
                            client,
                            (byte)srcSlot,
                            equippedItem.ItemId > 0 ? equippedItem : new ItemModel(),
                            hadDestinationItem ? (byte)1 : (byte)0);
                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.DigiviceToInventory:
                    {
                        var srcSlot = 0;
                        var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        if (!client.Tamer.Digivice.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot))
                            return false;

                        var digiviceItem = client.Tamer.Digivice.FindItemBySlot(srcSlot) ?? new ItemModel();
                        BroadcastAppearanceUpdate(
                            client,
                            13,
                            digiviceItem.ItemId > 0 ? digiviceItem : new ItemModel(),
                            0);
                        SendStatusAndSpeed(client);
                        return true;
                    }

                case ItemListMovimentationEnum.WarehouseToWarehouse:
                    {
                        var orgSlot = (short)(originSlot - (short)GeneralSizeEnum.WarehouseMinSlot);
                        var destSlot = (short)(destinationSlot - (short)GeneralSizeEnum.WarehouseMinSlot);

                        return client.Tamer.Warehouse.MoveItem(orgSlot, destSlot);
                    }

                case ItemListMovimentationEnum.AccountWarehouseToAccountWarehouse:
                    {
                        var orgSlot = (short)(originSlot - (short)GeneralSizeEnum.AccountWarehouseMinSlot);
                        var destSlot = (short)(destinationSlot - (short)GeneralSizeEnum.AccountWarehouseMinSlot);

                        return client.Tamer.AccountWarehouse.MoveItem(orgSlot, destSlot);
                    }

                case ItemListMovimentationEnum.WarehouseToInventory:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.WarehouseMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        return client.Tamer.Warehouse.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot);
                    }

                case ItemListMovimentationEnum.WarehouseToAccountWarehouse:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.WarehouseMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.AccountWarehouseMinSlot.GetHashCode();
                        return client.Tamer.Warehouse.TryMoveAcrossLists(client.Tamer.AccountWarehouse, srcSlot, dstSlot);
                    }

                case ItemListMovimentationEnum.AccountWarehouseToInventory:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.AccountWarehouseMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
                        return client.Tamer.AccountWarehouse.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot);
                    }

                case ItemListMovimentationEnum.AccountWarehouseToWarehouse:
                    {
                        var srcSlot = originSlot - GeneralSizeEnum.AccountWarehouseMinSlot.GetHashCode();
                        var dstSlot = destinationSlot - GeneralSizeEnum.WarehouseMinSlot.GetHashCode();
                        return client.Tamer.AccountWarehouse.TryMoveAcrossLists(client.Tamer.Warehouse, srcSlot, dstSlot);
                    }

                case ItemListMovimentationEnum.InventoryToExtraSeal:
                    return MoveInventoryToExtra(client, client.Tamer.ExtraInventorySeal, originSlot, destinationSlot, GeneralSizeEnum.ExtraSealMinSlot, CategorySeal);

                case ItemListMovimentationEnum.ExtraSealToInventory:
                    return MoveExtraToInventory(client, client.Tamer.ExtraInventorySeal, originSlot, destinationSlot, GeneralSizeEnum.ExtraSealMinSlot, "Seal");

                case ItemListMovimentationEnum.ExtraSealToExtraSeal:
                    return MoveWithinExtra(client, client.Tamer.ExtraInventorySeal, originSlot, destinationSlot, GeneralSizeEnum.ExtraSealMinSlot, "Seal");

                case ItemListMovimentationEnum.InventoryToExtraTicket:
                    return MoveInventoryToExtra(client, client.Tamer.ExtraInventoryTicket, originSlot, destinationSlot, GeneralSizeEnum.ExtraTicketMinSlot, CategoryTicket);

                case ItemListMovimentationEnum.ExtraTicketToInventory:
                    return MoveExtraToInventory(client, client.Tamer.ExtraInventoryTicket, originSlot, destinationSlot, GeneralSizeEnum.ExtraTicketMinSlot, "Ticket");

                case ItemListMovimentationEnum.ExtraTicketToExtraTicket:
                    return MoveWithinExtra(client, client.Tamer.ExtraInventoryTicket, originSlot, destinationSlot, GeneralSizeEnum.ExtraTicketMinSlot, "Ticket");

                case ItemListMovimentationEnum.InventoryToExtraEvolution:
                    return MoveInventoryToExtra(client, client.Tamer.ExtraInventoryEvolution, originSlot, destinationSlot, GeneralSizeEnum.ExtraEvolutionMinSlot, CategoryEvolution);

                case ItemListMovimentationEnum.ExtraEvolutionToInventory:
                    return MoveExtraToInventory(client, client.Tamer.ExtraInventoryEvolution, originSlot, destinationSlot, GeneralSizeEnum.ExtraEvolutionMinSlot, "Evolution");

                case ItemListMovimentationEnum.ExtraEvolutionToExtraEvolution:
                    return MoveWithinExtra(client, client.Tamer.ExtraInventoryEvolution, originSlot, destinationSlot, GeneralSizeEnum.ExtraEvolutionMinSlot, "Evolution");

                case ItemListMovimentationEnum.InventoryToExtraDigitama:
                    return MoveInventoryToExtra(client, client.Tamer.ExtraInventoryDigitama, originSlot, destinationSlot, GeneralSizeEnum.ExtraDigitamaMinSlot, CategoryDigitama);

                case ItemListMovimentationEnum.ExtraDigitamaToInventory:
                    return MoveExtraToInventory(client, client.Tamer.ExtraInventoryDigitama, originSlot, destinationSlot, GeneralSizeEnum.ExtraDigitamaMinSlot, "Digitama");

                case ItemListMovimentationEnum.ExtraDigitamaToExtraDigitama:
                    return MoveWithinExtra(client, client.Tamer.ExtraInventoryDigitama, originSlot, destinationSlot, GeneralSizeEnum.ExtraDigitamaMinSlot, "Digitama");

                case ItemListMovimentationEnum.InventoryToExtraMaterial:
                    return MoveInventoryToExtra(client, client.Tamer.ExtraInventoryMaterial, originSlot, destinationSlot, GeneralSizeEnum.ExtraMaterialMinSlot, CategoryMaterial);

                case ItemListMovimentationEnum.ExtraMaterialToInventory:
                    return MoveExtraToInventory(client, client.Tamer.ExtraInventoryMaterial, originSlot, destinationSlot, GeneralSizeEnum.ExtraMaterialMinSlot, "Material");

                case ItemListMovimentationEnum.ExtraMaterialToExtraMaterial:
                    return MoveWithinExtra(client, client.Tamer.ExtraInventoryMaterial, originSlot, destinationSlot, GeneralSizeEnum.ExtraMaterialMinSlot, "Material");
            }

            return false;
        }

        private bool MoveInventoryToExtra(
            GameClient client,
            ItemListModel targetExtraInventory,
            short originSlot,
            short destinationSlot,
            GeneralSizeEnum extraMinSlot,
            int category)
        {
            var srcSlot = originSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
            var dstSlot = destinationSlot - extraMinSlot.GetHashCode();
            var sourceItem = client.Tamer.Inventory.FindItemBySlot(srcSlot);
            var sourceType = sourceItem == null ? 0 : ResolveTypeL(sourceItem);

            if (sourceItem == null || sourceItem.ItemId <= 0 || sourceItem.Amount <= 0)
            {
                _logger.Warning(
                    "ExtraInventory manual move rejected: tamer={TamerId} reason=EmptySource origin={OriginSlot} destination={DestinationSlot} srcSlot={SrcSlot} dstSlot={DstSlot} category={Category}",
                    client.TamerId,
                    originSlot,
                    destinationSlot,
                    srcSlot,
                    dstSlot,
                    category);
                return false;
            }

            if (!IsAllowedExtraInventoryType(category, sourceType))
            {
                _logger.Warning(
                    "ExtraInventory manual move rejected: tamer={TamerId} reason=WrongCategory origin={OriginSlot} destination={DestinationSlot} srcSlot={SrcSlot} dstSlot={DstSlot} itemId={ItemId} type={Type} category={Category}",
                    client.TamerId,
                    originSlot,
                    destinationSlot,
                    srcSlot,
                    dstSlot,
                    sourceItem.ItemId,
                    sourceType,
                    category);
                return false;
            }

            var moved = client.Tamer.Inventory.TryMoveAcrossLists(targetExtraInventory, srcSlot, dstSlot);
            _logger.Information(
                "ExtraInventory manual move inventory-to-extra: tamer={TamerId} moved={Moved} origin={OriginSlot} destination={DestinationSlot} srcSlot={SrcSlot} dstSlot={DstSlot} itemId={ItemId} type={Type} amount={Amount} category={Category}",
                client.TamerId,
                moved,
                originSlot,
                destinationSlot,
                srcSlot,
                dstSlot,
                sourceItem.ItemId,
                sourceType,
                sourceItem.Amount,
                category);
            return moved;
        }

        private bool MoveExtraToInventory(
            GameClient client,
            ItemListModel sourceExtraInventory,
            short originSlot,
            short destinationSlot,
            GeneralSizeEnum extraMinSlot,
            string categoryName)
        {
            var srcSlot = originSlot - extraMinSlot.GetHashCode();
            var dstSlot = destinationSlot - GeneralSizeEnum.InventoryMinSlot.GetHashCode();
            var sourceItem = sourceExtraInventory.FindItemBySlot(srcSlot);
            var moved = sourceExtraInventory.TryMoveAcrossLists(client.Tamer.Inventory, srcSlot, dstSlot);
            _logger.Information(
                "ExtraInventory manual move extra-to-inventory: tamer={TamerId} moved={Moved} category={Category} origin={OriginSlot} destination={DestinationSlot} srcSlot={SrcSlot} dstSlot={DstSlot} itemId={ItemId} amount={Amount}",
                client.TamerId,
                moved,
                categoryName,
                originSlot,
                destinationSlot,
                srcSlot,
                dstSlot,
                sourceItem?.ItemId ?? 0,
                sourceItem?.Amount ?? 0);
            return moved;
        }

        private bool MoveWithinExtra(
            GameClient client,
            ItemListModel extraInventory,
            short originSlot,
            short destinationSlot,
            GeneralSizeEnum extraMinSlot,
            string categoryName)
        {
            var srcSlot = (short)(originSlot - extraMinSlot.GetHashCode());
            var dstSlot = (short)(destinationSlot - extraMinSlot.GetHashCode());
            var sourceItem = extraInventory.FindItemBySlot(srcSlot);
            var moved = extraInventory.MoveItem(srcSlot, dstSlot);
            _logger.Information(
                "ExtraInventory manual move extra-to-extra: tamer={TamerId} moved={Moved} category={Category} origin={OriginSlot} destination={DestinationSlot} srcSlot={SrcSlot} dstSlot={DstSlot} itemId={ItemId} amount={Amount}",
                client.TamerId,
                moved,
                categoryName,
                originSlot,
                destinationSlot,
                srcSlot,
                dstSlot,
                sourceItem?.ItemId ?? 0,
                sourceItem?.Amount ?? 0);
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

        private static bool IsAllowedExtraInventoryType(int category, int type)
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

        private void BroadcastAppearanceUpdate(GameClient client, byte slot, ItemModel item, byte mode)
        {
            if (!IsVisualEquipmentSlot(slot))
            {
                _logger.Information(
                    "Appearance update skipped for non-visual equipment slot: tamer={TamerId} slot={Slot} itemId={ItemId} mode={Mode}",
                    client.TamerId,
                    slot,
                    item.ItemId,
                    mode);
                return;
            }

            var packet = new UpdateTamerAppearancePacket(
                client.Tamer.GeneralHandler,
                slot,
                item,
                mode).Serialize();

            _logger.Information(
                "Appearance update send: tamer={TamerId} handler={Handler} slot={Slot} itemId={ItemId} mode={Mode} packet={Packet} bytes={Bytes}",
                client.TamerId,
                client.Tamer.GeneralHandler,
                slot,
                item.ItemId,
                mode,
                GetPacketNumber(packet),
                packet.Length);

            if (client.DungeonMap)
            {
                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet);
            }
            else
            {
                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet);
            }
        }

        private void SendStatusAndSpeed(GameClient client)
        {
            var statusPacket = new UpdateStatusPacket(client.Tamer).Serialize();
            _logger.Information(
                "Status update send after equipment-affecting move: tamer={TamerId} packet={Packet} bytes={Bytes}",
                client.TamerId,
                GetPacketNumber(statusPacket),
                statusPacket.Length);
            client.Send(statusPacket);

            var packet = new UpdateMovementSpeedPacket(client.Tamer).Serialize();
            _logger.Information(
                "Movement speed update send after equipment-affecting move: tamer={TamerId} packet={Packet} bytes={Bytes}",
                client.TamerId,
                GetPacketNumber(packet),
                packet.Length);
            if (client.DungeonMap)
            {
                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet);
            }
            else
            {
                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, packet);
            }
        }

        private void LogEquipmentPacketSnapshot(
            GameClient client,
            short originSlot,
            short destinationSlot,
            ItemListMovimentationEnum movimentation)
        {
            var packets = BuildMoveResultPackets(client, movimentation, true, originSlot, destinationSlot);
            _logger.Information(
                "ItemMove response packets: tamer={TamerId} movement={Movement} origin={OriginSlot} destination={DestinationSlot} packets={Packets}",
                client.TamerId,
                movimentation,
                originSlot,
                destinationSlot,
                string.Join(", ", packets.Select(packet => $"{GetPacketNumber(packet)}:{packet.Length}b")));

            _logger.Information(
                "Equipment slots after move: tamer={TamerId} slots={Slots}",
                client.TamerId,
                string.Join(", ",
                    client.Tamer.Equipment.Items
                        .OrderBy(item => item.Slot)
                        .Select(item => $"{item.Slot}:{item.ItemId}x{item.Amount}/p{item.Power}/end={(item.EndDate == DateTime.MinValue ? "min" : item.EndDate.ToString("O"))}")));
        }

        private static bool IsVisualEquipmentSlot(byte slot)
        {
            return slot <= 6 || slot == 10 || slot == 11 || slot == 13;
        }

        private static int GetPacketNumber(byte[] packet)
        {
            return packet.Length >= 4 ? BitConverter.ToUInt16(packet, 2) : 0;
        }
    }
}
