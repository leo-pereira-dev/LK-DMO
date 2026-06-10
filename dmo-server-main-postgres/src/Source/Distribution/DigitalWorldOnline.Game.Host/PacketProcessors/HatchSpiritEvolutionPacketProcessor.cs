using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Commons.Writers;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchSpiritEvolutionPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchSpiritEvolution;

        private readonly StatusManager _statusManager;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public HatchSpiritEvolutionPacketProcessor(
            StatusManager statusManager,
            MapServer mapServer,
            AssetsLoader assets,
            ILogger logger,
            ISender sender,
            DungeonsServer dungeonsServer
        )
        {
            _statusManager = statusManager;
            _mapServer = mapServer;
            _assets = assets;
            _logger = logger;
            _sender = sender;
            _dungeonServer = dungeonsServer;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var targetType = packet.ReadInt();
            var digiName = NormalizeHatchName(packet.ReadString(), client.Tamer.Name);
            var npcId = ReadOptionalNpcId(packet);

            var extraEvolutionNpc = ResolveNpc(npcId);

            if (extraEvolutionNpc == null)
                return;

            var extraEvolutionInfo = extraEvolutionNpc.ExtraEvolutionInformation
                .FirstOrDefault(x => (x.IndexId == 1 || x.IndexId == 0) &&
                                     x.ExtraEvolution.Any(extra => extra.DigimonId == targetType))
                ?.ExtraEvolution;

            if (extraEvolutionInfo == null)
                return;

            var extraEvolution = extraEvolutionInfo.FirstOrDefault(x => x.DigimonId == targetType);

            if (extraEvolution == null)
            {
                //_logger.Warning($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}.");
                //client.Send(new SystemMessagePacket($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}."));
                return;
            }        

            if (!TrySelectRequireds(client.Tamer.Inventory, extraEvolution, out var requiredsToPacket) ||
                !TrySelectMaterials(client.Tamer.Inventory, extraEvolution, out var materialToPacket))
            {
                client.Send(new SystemMessagePacket("Missing materials for extra evolution."));
                return;
            }

            if (client.Tamer.Inventory.Bits < extraEvolution.Price)
            {
                client.Send(new SystemMessagePacket("Insufficient bits for extra evolution."));
                return;
            }

            var slot = FindEmptyDigimonSlot(client);
            if (slot < 0)
            {
                client.Send(new SystemMessagePacket("No empty digimon slot available."));
                return;
            }

            var newDigimon = DigimonModel.Create(
                digiName,
                targetType,
                targetType,
                DigimonHatchGradeEnum.Default,
                UtilitiesFunctions.GetLevelSize(3),
                (byte)slot
            );

            newDigimon.NewLocation(
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y
            );

            newDigimon.SetBaseInfo(
                _statusManager.GetDigimonBaseInfo(
                    newDigimon.BaseType
                )
            );

            newDigimon.SetBaseStatus(
                _statusManager.GetDigimonBaseStatus(
                    newDigimon.BaseType,
                    newDigimon.Level,
                    newDigimon.Size
                )
            );
            newDigimon.FullHeal();

            var evolutionInfo = _assets.EvolutionInfo.FirstOrDefault(x => x.Type == newDigimon.BaseType);
            if (evolutionInfo != null)
                newDigimon.AddEvolutions(evolutionInfo);

            if (newDigimon.BaseInfo == null || newDigimon.BaseStatus == null || !newDigimon.Evolutions.Any())
            {
                _logger.Warning($"Unknown digimon info for {newDigimon.BaseType}.");
                client.Send(new SystemMessagePacket($"Unknown digimon info for {newDigimon.BaseType}."));
                return;
            }

            if (!client.Tamer.Inventory.RemoveOrReduceItems(BuildRemovalItems(materialToPacket, requiredsToPacket)))
            {
                client.Send(new SystemMessagePacket("Missing materials for extra evolution."));
                return;
            }

            if (!client.Tamer.Inventory.RemoveBits(extraEvolution.Price))
            {
                client.Send(new SystemMessagePacket("Insufficient bits for extra evolution."));
                return;
            }

            newDigimon.SetTamer(client.Tamer);

            client.Tamer.AddDigimon(newDigimon);


         
            if (client.Tamer.Incubator.PerfectSize(newDigimon.HatchGrade, newDigimon.Size))
            {
                _mapServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
                _dungeonServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
            }


            var digimonInfo = await _sender.Send(new CreateDigimonCommand(newDigimon));

            var displaySlot = client.Tamer.ActiveDigimons.FindIndex(x => x == newDigimon) + 1;
            if (displaySlot <= 0)
                displaySlot = newDigimon.Slot;

            client.Send(new HatchFinishPacket(newDigimon, (uint)(client.Partner.GeneralHandler + 1000), displaySlot));

            client.Send(new HatchSpiritEvolutionPacket(targetType, (int)client.Tamer.Inventory.Bits, materialToPacket, requiredsToPacket));
            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));

            if (digimonInfo != null)
            {
                newDigimon.SetId(digimonInfo.Id);
                var evolutionSlot = -1;

                foreach (var digimon in newDigimon.Evolutions)
                {
                    evolutionSlot++;

                    var evolution = digimonInfo.Evolutions[evolutionSlot];

                    if (evolution != null)
                    {
                        digimon.SetId(evolution.Id);

                        var skillSlot = -1;

                        foreach (var skill in digimon.Skills)
                        {
                            skillSlot++;

                            var dtoSkill = evolution.Skills[skillSlot];

                            skill.SetId(dtoSkill.Id);
                        }
                    }
                }
            }

            _logger.Verbose($"Character {client.TamerId} hatched spirit {newDigimon.Id}({newDigimon.BaseType}) with grade {newDigimon.HatchGrade} and size {newDigimon.Size}.");
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

        private static int FindEmptyDigimonSlot(GameClient client)
        {
            for (var slot = 0; slot < client.Tamer.DigimonSlots; slot++)
            {
                if (client.Tamer.Digimons.FirstOrDefault(x => x.Slot == slot) == null)
                    return slot;
            }

            return -1;
        }

        private static bool TrySelectRequireds(
            ItemListModel inventory,
            ExtraEvolutionAssetModel extraEvolution,
            out List<ExtraEvolutionRequiredAssetModel> selected)
        {
            selected = new List<ExtraEvolutionRequiredAssetModel>();
            var candidates = extraEvolution.Requireds
                .Where(x => x.ItemId > 0)
                .Select(x => new ExtraEvolutionRequiredAssetModel
                {
                    Id = x.Id,
                    ItemId = x.ItemId,
                    Amount = NormalizeAmount(x.Amount)
                })
                .ToList();

            if (!candidates.Any())
                return true;

            if (IsNeedOne(extraEvolution))
            {
                var candidate = candidates.FirstOrDefault(x => HasInventoryItem(inventory, x.ItemId, x.Amount));
                if (candidate == null)
                    return false;

                selected.Add(candidate);
                return true;
            }

            if (!HasInventoryItems(inventory, candidates.Select(x => (x.ItemId, x.Amount))))
                return false;

            selected.AddRange(candidates);
            return true;
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

            if (IsNeedOne(extraEvolution))
            {
                var candidate = candidates.FirstOrDefault(x => HasInventoryItem(inventory, x.ItemId, x.Amount));
                if (candidate == null)
                    return false;

                selected.Add(candidate);
                return true;
            }

            if (!HasInventoryItems(inventory, candidates.Select(x => (x.ItemId, x.Amount))))
                return false;

            selected.AddRange(candidates);
            return true;
        }

        private static bool IsNeedOne(ExtraEvolutionAssetModel extraEvolution) => extraEvolution.WayType == 2;

        private static bool HasInventoryItem(ItemListModel inventory, int itemId, int amount)
        {
            return inventory.FindItemsById(itemId).Sum(x => x.Amount) >= NormalizeAmount(amount);
        }

        private static bool HasInventoryItems(ItemListModel inventory, IEnumerable<(int ItemId, int Amount)> materials)
        {
            return materials
                .GroupBy(x => x.ItemId)
                .All(group => HasInventoryItem(inventory, group.Key, group.Sum(x => NormalizeAmount(x.Amount))));
        }

        private static List<ItemModel> BuildRemovalItems(
            IReadOnlyCollection<ExtraEvolutionMaterialAssetModel> materials,
            IReadOnlyCollection<ExtraEvolutionRequiredAssetModel> requireds)
        {
            var totals = new Dictionary<int, int>();

            foreach (var material in materials)
                AddRemovalTotal(totals, material.ItemId, material.Amount);

            foreach (var required in requireds)
                AddRemovalTotal(totals, required.ItemId, required.Amount);

            return totals.Select(x => new ItemModel(x.Key, x.Value)).ToList();
        }

        private static void AddRemovalTotal(IDictionary<int, int> totals, int itemId, int amount)
        {
            if (itemId <= 0)
                return;

            totals.TryGetValue(itemId, out var current);
            totals[itemId] = current + NormalizeAmount(amount);
        }

        private static int NormalizeAmount(int amount) => amount <= 0 ? 1 : amount;

        private static string NormalizeHatchName(string? value, string fallbackName)
        {
            var name = string.IsNullOrEmpty(value)
                ? string.Empty
                : value.Replace("\0", string.Empty).Trim();

            if (string.IsNullOrWhiteSpace(name))
            {
                name = string.IsNullOrWhiteSpace(fallbackName)
                    ? "Digimon"
                    : fallbackName.Replace("\0", string.Empty).Trim();
            }

            if (string.IsNullOrWhiteSpace(name))
                name = "Digimon";

            const int maxDigimonNameLength = 20;
            if (name.Length > maxDigimonNameLength)
                name = name.Substring(0, maxDigimonNameLength);

            return name;
        }
    }
}
