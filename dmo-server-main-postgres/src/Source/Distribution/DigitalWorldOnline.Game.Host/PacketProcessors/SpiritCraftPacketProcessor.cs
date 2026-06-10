using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Delete;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class SpiritCraftPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.SpiritCraft;

        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly AssetsLoader _assets;
        public SpiritCraftPacketProcessor(
            ILogger logger,
            ISender sender,
            AssetsLoader assets
        )
        {
            _logger = logger;
            _sender = sender;
            _assets = assets;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var slot = packet.ReadByte();
            var validation = packet.ReadString();

            var targetDigimon = client.Tamer.Digimons.FirstOrDefault(x => x.Slot == slot);
            if (targetDigimon == null)
                return;

            var result = client.PartnerDeleteValidation(validation);
            var npcId = ReadOptionalNpcId(packet);
            var digimonId = targetDigimon.Id;
            var targetType = targetDigimon.BaseType;

            var extraEvolutionNpc = ResolveNpc(npcId);

            if (extraEvolutionNpc == null)
                return;

            var extraEvolutionInfo = extraEvolutionNpc.ExtraEvolutionInformation
                .FirstOrDefault(x => (x.IndexId == 2 || x.IndexId == 0) &&
                                     x.ExtraEvolution.Any(extra => extra.Requireds.Any(required => required.ItemId == targetType)))
                ?.ExtraEvolution;

            if (extraEvolutionInfo == null)
                return;

            var extraEvolution = extraEvolutionInfo.FirstOrDefault(x => x.Requireds.Any(x => x.ItemId == targetType));
            
            if (extraEvolution == null)
                return;

            if (result > 0)
            {
                if (targetDigimon.Level < extraEvolution.RequiredLevel)
                {
                    client.Send(new SystemMessagePacket("Digimon level is too low for this exchange."));
                    return;
                }

                if (!TrySelectMaterials(client.Tamer.Inventory, extraEvolution, out var materialsToRemove))
                {
                    client.Send(new SystemMessagePacket("Missing materials for extra evolution."));
                    return;
                }

                if (client.Tamer.Inventory.Bits < extraEvolution.Price)
                {
                    client.Send(new SystemMessagePacket("Insufficient bits for extra evolution."));
                    return;
                }

                var craftedItem = new ItemModel(extraEvolution.DigimonId, 1);
                var craftedItemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == craftedItem.ItemId);
                if (craftedItemInfo == null)
                {
                    client.Send(new SystemMessagePacket($"Unknown item info for {craftedItem.ItemId}."));
                    return;
                }

                craftedItem.SetItemInfo(craftedItemInfo);
                if (!CanAddInventoryItem(client.Tamer.Inventory, craftedItem))
                {
                    client.Send(new SystemMessagePacket("No inventory space available."));
                    return;
                }

                if (!client.Tamer.Inventory.RemoveOrReduceItems(BuildRemovalItems(materialsToRemove)))
                {
                    client.Send(new SystemMessagePacket("Missing materials for extra evolution."));
                    return;
                }

                if (!client.Tamer.Inventory.RemoveBits(extraEvolution.Price))
                {
                    client.Send(new SystemMessagePacket("Insufficient bits for extra evolution."));
                    return;
                }
                
                if (!client.Tamer.Inventory.AddItem((ItemModel)craftedItem.Clone()))
                {
                    client.Send(new SystemMessagePacket("No inventory space available."));
                    return;
                }

                client.Tamer.RemoveDigimon((byte)targetDigimon.Slot);


                client.Send(new SpiritCraftPacket(slot, client.Tamer.Inventory.Bits, extraEvolution.DigimonId));
                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

                await _sender.Send(new DeleteDigimonCommand(digimonId));
                await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));
                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));

                _logger.Verbose($"Character {client.TamerId} deleted partner {digimonId}.");
            }
            else
            {
                client.Send(new PartnerDeletePacket(result));
                _logger.Verbose($"Character {client.TamerId} failed to deleted partner {digimonId} with invalid account information.");
            }
        }

        private ExtraEvolutionNpcAssetModel? ResolveNpc(int npcId)
        {
            return _assets.ExtraEvolutions.FirstOrDefault(x => x.NpcId == npcId)
                   ?? _assets.ExtraEvolutions.FirstOrDefault(x => x.NpcId == 90005);
        }

        private static int ReadOptionalNpcId(GamePacketReader packet)
        {
            return RemainingPayloadBytes(packet) >= sizeof(int) ? packet.ReadInt() : 90005;
        }

        private static int RemainingPayloadBytes(GamePacketReader packet)
        {
            return Math.Max(0, packet.Length - 2 - (int)packet.Packet.Position);
        }

        private static bool TrySelectMaterials(
            ItemListModel inventory,
            ExtraEvolutionAssetModel extraEvolution,
            out List<ExtraEvolutionMaterialAssetModel> selected)
        {
            selected = new List<ExtraEvolutionMaterialAssetModel>();
            var candidates = extraEvolution.Materials
                .Where(x => x.ItemId > 0)
                .Select(x => new ExtraEvolutionMaterialAssetModel
                {
                    Id = x.Id,
                    ItemId = x.ItemId,
                    Amount = NormalizeAmount(x.Amount)
                })
                .ToList();

            if (!candidates.Any())
                return true;

            if (extraEvolution.WayType == 2)
            {
                var candidate = candidates.FirstOrDefault(x => HasInventoryItem(inventory, x.ItemId, x.Amount));
                if (candidate == null)
                    return false;

                selected.Add(candidate);
                return true;
            }

            if (!candidates
                    .GroupBy(x => x.ItemId)
                    .All(group => HasInventoryItem(inventory, group.Key, group.Sum(x => NormalizeAmount(x.Amount)))))
                return false;

            selected.AddRange(candidates);
            return true;
        }

        private static bool HasInventoryItem(ItemListModel inventory, int itemId, int amount)
        {
            return inventory.FindItemsById(itemId).Sum(x => x.Amount) >= NormalizeAmount(amount);
        }

        private static List<ItemModel> BuildRemovalItems(IEnumerable<ExtraEvolutionMaterialAssetModel> materials)
        {
            return materials
                .GroupBy(x => x.ItemId)
                .Select(group => new ItemModel(group.Key, group.Sum(x => NormalizeAmount(x.Amount))))
                .ToList();
        }

        private static bool CanAddInventoryItem(ItemListModel inventory, ItemModel item)
        {
            var overlap = item.ItemInfo?.Overlap > 0 ? item.ItemInfo.Overlap : (short)1;
            var remaining = item.Amount;

            foreach (var target in inventory.FindItemsById(item.ItemId).Where(x => x.ItemInfo?.Overlap > 1))
            {
                remaining -= Math.Max(0, overlap - target.Amount);
                if (remaining <= 0)
                    return true;
            }

            return inventory.TotalEmptySlots * overlap >= remaining;
        }

        private static int NormalizeAmount(int amount) => amount <= 0 ? 1 : amount;
    }
}
