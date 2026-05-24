using AutoMapper;
using DigitalWorldOnline.Application.Separar.Commands.Delete;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class DigimonTranscendenceExpPacketProcessor : IGamePacketProcessor
    {
        private const long MaxTranscendenceExperience = 140000;
        private const byte MinimumLevel = 100;
        private const short MinimumSize = 12500;
        private const int EnchantDefaultCorrect = 5;
        private const int SuccessChancePercent = 15;

        private const int ResultSuccess = 0;
        private const int ResultFail = 1;
        private const int ResultNotTermsLevel = 20103;
        private const int ResultNotTermsScale = 20104;
        private const int ResultNotTermsDigiclone = 20105;
        private const int ResultNotUseAvailableMaterial = 20106;
        private const int ResultNotUseMaterial = 20107;
        private const int ResultItemCountShortage = 20108;
        private const int ResultAlreadyTranscendence = 20111;
        private const int ResultFullExp = 20112;
        private const int ResultNotUseTranscendenceMaterial = 20116;

        private static readonly IReadOnlyDictionary<DigimonHatchGradeEnum, int> DifferentTypeCorrect = new Dictionary<DigimonHatchGradeEnum, int>
        {
            { DigimonHatchGradeEnum.Default, 1 },
            { DigimonHatchGradeEnum.High, 10 },
            { DigimonHatchGradeEnum.Perfect, 40 }
        };

        private static readonly IReadOnlyDictionary<DigimonHatchGradeEnum, int> SameTypeCorrect = new Dictionary<DigimonHatchGradeEnum, int>
        {
            { DigimonHatchGradeEnum.Default, 3 },
            { DigimonHatchGradeEnum.High, 30 },
            { DigimonHatchGradeEnum.Perfect, 120 }
        };

        private static readonly IReadOnlyDictionary<DigimonHatchGradeEnum, int> RaremonFixedExp = new Dictionary<DigimonHatchGradeEnum, int>
        {
            { DigimonHatchGradeEnum.Default, 3140 },
            { DigimonHatchGradeEnum.High, 23550 },
            { DigimonHatchGradeEnum.Perfect, 62800 }
        };

        public GameServerPacketEnum Type => GameServerPacketEnum.TranscendenceReceiveExpResult;

        private readonly StatusManager _statusManager;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly IMapper _mapper;
        private readonly Random _random = new();

        public DigimonTranscendenceExpPacketProcessor(
            ILogger logger,
            ISender sender,
            IMapper mapper,
            StatusManager statusManager)
        {
            _logger = logger;
            _sender = sender;
            _mapper = mapper;
            _statusManager = statusManager;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            try
            {
                var packet = new GamePacketReader(packetData);

                var activeItemSlot = packet.ReadInt();
                var npcId = packet.ReadInt();
                var inputType = (AcademyInputType)packet.ReadByte();
                var targetSlot = packet.ReadByte();
                var materialCount = packet.ReadUShort();

                var materialSlots = new List<short>();
                for (var i = 0; i < materialCount; i++)
                {
                    materialSlots.Add(packet.ReadShort());
                }

                var itemCount = packet.ReadUShort();
                var itemRequests = new List<TranscendenceItemRequest>();
                for (var i = 0; i < itemCount; i++)
                {
                    itemRequests.Add(new TranscendenceItemRequest(
                        packet.ReadInt(),
                        packet.ReadUShort(),
                        packet.ReadUShort()));
                }

                _logger.Information("[Transcendence] Charge request: tamer {TamerId} targetSlot {TargetSlot} input {InputType} npc {NpcId} activeItemSlot {ActiveItemSlot} materials [{MaterialSlots}] items [{Items}].",
                    client.TamerId,
                    targetSlot,
                    inputType,
                    npcId,
                    activeItemSlot,
                    string.Join(",", materialSlots),
                    string.Join(",", itemRequests.Select(x => $"{x.ItemId}@{x.Slot}x{x.Amount}")));

                var targetPartner = ClientDigimonSlotResolver.Resolve(client, targetSlot, out var resolvedTargetSlot);
                if (targetPartner == null)
                {
                    SendChargeFailure(client, ResultNotUseAvailableMaterial,
                        $"target slot {targetSlot} not found. available slots [{string.Join(",", client.Tamer.Digimons.Select(x => x.Slot))}]");
                    return;
                }

                if (resolvedTargetSlot != targetSlot)
                {
                    _logger.Information("[Transcendence] Charge target slot resolved: tamer {TamerId} clientSlot {ClientSlot} realSlot {RealSlot} target {TargetId}.",
                        client.TamerId, targetSlot, resolvedTargetSlot, targetPartner.Id);
                }

                var validationResult = ValidateTarget(targetPartner, requireFullExp: false);
                if (validationResult != ResultSuccess)
                {
                    SendChargeFailure(client, validationResult, $"target validation failed for digimon {targetPartner.Id}");
                    return;
                }

                if (targetPartner.TranscendenceExperience >= MaxTranscendenceExperience)
                {
                    SendChargeFailure(client, ResultFullExp, $"target {targetPartner.Id} already full exp");
                    return;
                }

                if (!IsValidInputType(inputType) || materialSlots.Count == 0)
                {
                    SendChargeFailure(client, ResultNotUseAvailableMaterial, "invalid input type or empty material list");
                    return;
                }

                if (materialSlots.Distinct().Count() != materialSlots.Count)
                {
                    SendChargeFailure(client, ResultNotUseAvailableMaterial, "duplicated material slots");
                    return;
                }

                long totalGainedExp = 0;
                short successRate = 0;
                var plannedMaterials = new List<TranscendenceMaterialPlan>();
                var plannedExp = targetPartner.TranscendenceExperience;

                foreach (var materialSlot in materialSlots)
                {
                    if (plannedExp >= MaxTranscendenceExperience)
                    {
                        break;
                    }

                    var archiveItem = client.Tamer.DigimonArchive.DigimonArchives.FirstOrDefault(x => x.Slot == materialSlot);
                    if (archiveItem?.DigimonId <= 0)
                    {
                        SendChargeFailure(client, ResultNotUseAvailableMaterial, $"archive slot {materialSlot} is empty");
                        return;
                    }

                    if (archiveItem.Digimon == null)
                    {
                        var material = _mapper.Map<DigimonModel>(
                            await _sender.Send(new GetDigimonByIdQuery(archiveItem.DigimonId)));

                        if (material == null)
                        {
                            SendChargeFailure(client, ResultNotUseAvailableMaterial, $"digimon {archiveItem.DigimonId} was not found");
                            return;
                        }

                        archiveItem.SetDigimonInfo(material);
                    }

                    var materialPartner = archiveItem.Digimon;
                    if (materialPartner == null)
                    {
                        SendChargeFailure(client, ResultNotUseAvailableMaterial, $"archive slot {materialSlot} has no digimon data");
                        return;
                    }

                    if (materialPartner.Id == targetPartner.Id)
                    {
                        _logger.Warning("[Transcendence] Ignoring selected material because it is the target digimon itself. tamer {TamerId} target {TargetId} archiveSlot {ArchiveSlot}.",
                            client.TamerId,
                            targetPartner.Id,
                            materialSlot);
                        continue;
                    }

                    if (!EnsureBaseInfo(materialPartner))
                    {
                        SendChargeFailure(client, ResultNotUseMaterial, $"material {materialPartner.Id} base info missing");
                        return;
                    }

                    var materialValidation = ValidateMaterial(materialPartner);
                    if (materialValidation != ResultSuccess)
                    {
                        SendChargeFailure(client, materialValidation, $"material validation failed for digimon {materialPartner.Id}");
                        return;
                    }

                    var baseExp = CalculateMaterialExp(targetPartner, materialPartner, inputType);
                    if (baseExp <= 0)
                    {
                        SendChargeFailure(client, ResultNotUseMaterial, $"material {materialPartner.Id} generated zero exp");
                        return;
                    }

                    var bonusSuccess = _random.Next(0, 100) < SuccessChancePercent;
                    var requestedGain = baseExp + (bonusSuccess ? baseExp * 2 : 0);
                    var remainingExp = MaxTranscendenceExperience - plannedExp;
                    var gainedExp = Math.Min(requestedGain, remainingExp);

                    if (gainedExp <= 0)
                    {
                        break;
                    }

                    plannedExp += gainedExp;
                    totalGainedExp += gainedExp;

                    plannedMaterials.Add(new TranscendenceMaterialPlan(
                        archiveItem,
                        materialPartner,
                        materialSlot,
                        baseExp,
                        gainedExp,
                        plannedExp,
                        bonusSuccess));

                    _logger.Information("[Transcendence] Material planned: tamer {TamerId} target {TargetId} material {MaterialId} archiveSlot {ArchiveSlot} baseExp {BaseExp} gained {GainedExp} plannedTotalExp {TotalExp} bonus {Bonus}.",
                        client.TamerId,
                        targetPartner.Id,
                        materialPartner.Id,
                        materialSlot,
                        baseExp,
                        gainedExp,
                        plannedExp,
                        bonusSuccess);
                }

                if (plannedMaterials.Count == 0 || totalGainedExp <= 0)
                {
                    SendChargeFailure(client, ResultNotUseAvailableMaterial, "no material was consumed");
                    return;
                }

                var requiredItemAmount = plannedMaterials.Count;
                if (!ValidateItems(client, inputType, itemRequests, requiredItemAmount, out var inventoryItems, out var itemFailureReason))
                {
                    SendChargeFailure(client, ResultItemCountShortage, itemFailureReason);
                    return;
                }

                var deletedMaterialSlots = new List<short>();
                var changedArchiveItems = new HashSet<CharacterDigimonArchiveItemModel>();
                foreach (var plannedMaterial in plannedMaterials)
                {
                    if (plannedMaterial.BonusSuccess)
                    {
                        successRate = 1;
                    }

                    var materialDigimonId = plannedMaterial.MaterialPartner.Id;
                    plannedMaterial.ArchiveItem.RemoveDigimon();
                    changedArchiveItems.Add(plannedMaterial.ArchiveItem);
                    await _sender.Send(new DeleteDigimonCommand(materialDigimonId));

                    deletedMaterialSlots.Add(plannedMaterial.ArchiveSlot);

                    _logger.Information("[Transcendence] Material consumed: tamer {TamerId} target {TargetId} material {MaterialId} archiveSlot {ArchiveSlot} baseExp {BaseExp} gained {GainedExp} totalExp {TotalExp} bonus {Bonus}.",
                        client.TamerId,
                        targetPartner.Id,
                        materialDigimonId,
                        plannedMaterial.ArchiveSlot,
                        plannedMaterial.BaseExp,
                        plannedMaterial.GainedExp,
                        plannedMaterial.TotalExpAfter,
                        plannedMaterial.BonusSuccess);
                }

                foreach (var changedArchiveItem in CompactArchiveSlots(client))
                {
                    changedArchiveItems.Add(changedArchiveItem);
                }

                foreach (var changedArchiveItem in changedArchiveItems)
                {
                    await _sender.Send(new UpdateCharacterDigimonArchiveItemCommand(changedArchiveItem));
                }

                targetPartner.UpdateTranscendenceExp(plannedExp);

                var updatedItems = ConsumeItems(client, inventoryItems);

                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                await _sender.Send(new UpdateDigimonExperienceCommand(targetPartner));

                client.Send(UtilitiesFunctions.GroupPackets(
                    new DigimonTranscendenceReceiveExpPacket(
                        inputType,
                        targetSlot,
                        (short)deletedMaterialSlots.Count,
                        deletedMaterialSlots,
                        updatedItems,
                        successRate,
                        totalGainedExp,
                        targetPartner.TranscendenceExperience).Serialize(),
                    new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive).Serialize()));

                _logger.Information("[Transcendence] Charge finished: tamer {TamerId} target {TargetId} slot {TargetSlot} gained {GainedExp} total {TotalExp}/{MaxExp} deleted [{DeletedSlots}].",
                    client.TamerId,
                    targetPartner.Id,
                    targetSlot,
                    totalGainedExp,
                    targetPartner.TranscendenceExperience,
                    MaxTranscendenceExperience,
                    string.Join(",", deletedMaterialSlots));
            }
            catch (Exception ex)
            {
                _logger.Error(ex, "[Transcendence] Charge packet failed for tamer {TamerId}.", client.TamerId);
                client.Send(new DigimonTranscendenceReceiveExpPacket(ResultNotUseAvailableMaterial));
            }
        }

        private IReadOnlyCollection<CharacterDigimonArchiveItemModel> CompactArchiveSlots(GameClient client)
        {
            var changedItems = new List<CharacterDigimonArchiveItemModel>();
            var archiveItems = client.Tamer.DigimonArchive.DigimonArchives
                .Where(x => x.Slot >= 0 && x.Slot < client.Tamer.DigimonArchive.Slots)
                .OrderBy(x => x.Slot)
                .ToList();

            var filledItems = archiveItems
                .Where(x => x.DigimonId > 0)
                .OrderBy(x => x.Slot)
                .ToList();

            for (var index = 0; index < filledItems.Count && index < archiveItems.Count; index++)
            {
                var source = filledItems[index];
                var destination = archiveItems[index];

                if (source.Id == destination.Id)
                    continue;

                _logger.Information(
                    "[Transcendence] Archive compact: tamer {TamerId} digimon {DigimonId} fromSlot {FromSlot} toSlot {ToSlot}.",
                    client.TamerId,
                    source.DigimonId,
                    source.Slot,
                    destination.Slot);

                destination.AddDigimon(source.DigimonId);
                destination.SetDigimonInfo(source.Digimon);
                source.RemoveDigimon();

                changedItems.Add(destination);
                changedItems.Add(source);
            }

            return changedItems;
        }

        private int ValidateTarget(DigimonModel targetPartner, bool requireFullExp)
        {
            if (targetPartner.HatchGrade == DigimonHatchGradeEnum.Transcend)
            {
                return ResultAlreadyTranscendence;
            }

            if (targetPartner.Level < MinimumLevel)
            {
                return ResultNotTermsLevel;
            }

            if (targetPartner.Size < MinimumSize)
            {
                return ResultNotTermsScale;
            }

            if (targetPartner.Digiclone.CloneLevel < 1)
            {
                return ResultNotTermsDigiclone;
            }

            if (requireFullExp && targetPartner.TranscendenceExperience < MaxTranscendenceExperience)
            {
                return ResultFullExp;
            }

            if (!IsValidHatchGrade(targetPartner.HatchGrade))
            {
                return ResultNotUseMaterial;
            }

            return EnsureBaseInfo(targetPartner) ? ResultSuccess : ResultNotUseMaterial;
        }

        private int ValidateMaterial(DigimonModel materialPartner)
        {
            if (materialPartner.HatchGrade == DigimonHatchGradeEnum.Transcend)
            {
                return ResultNotUseTranscendenceMaterial;
            }

            return IsValidHatchGrade(materialPartner.HatchGrade)
                ? ResultSuccess
                : ResultNotUseMaterial;
        }

        private bool ValidateItems(
            GameClient client,
            AcademyInputType inputType,
            IReadOnlyList<TranscendenceItemRequest> itemRequests,
            int requiredAmount,
            out List<TranscendenceInventoryItem> inventoryItems,
            out string failureReason)
        {
            inventoryItems = new List<TranscendenceInventoryItem>();
            failureReason = string.Empty;

            if (itemRequests.Count == 0 || itemRequests.Sum(x => x.Amount) < requiredAmount)
            {
                failureReason = $"insufficient item amount requested={itemRequests.Sum(x => x.Amount)} required={requiredAmount}";
                return false;
            }

            var remainingAmount = requiredAmount;
            foreach (var request in itemRequests)
            {
                if (remainingAmount <= 0)
                    break;

                if (request.Amount <= 0)
                {
                    failureReason = $"invalid amount for item slot {request.Slot}";
                    return false;
                }

                var amountToUse = Math.Min(request.Amount, remainingAmount);
                var inventoryItem = client.Tamer.Inventory.FindItemBySlot(request.Slot);
                if (inventoryItem == null || inventoryItem.ItemId != request.ItemId || inventoryItem.Amount < amountToUse)
                {
                    failureReason = $"inventory slot {request.Slot} item mismatch packet={request.ItemId}x{amountToUse} inventory={inventoryItem?.ItemId}x{inventoryItem?.Amount}";
                    return false;
                }

                var itemSection = inventoryItem.ItemInfo?.Section ?? 0;
                if (!IsValidChargeItem(inputType, request.ItemId) && !IsValidChargeItem(inputType, itemSection))
                {
                    failureReason = $"invalid charge item {request.ItemId} section {itemSection} for input {inputType}";
                    return false;
                }

                inventoryItems.Add(new TranscendenceInventoryItem(
                    new TranscendenceItemRequest(request.ItemId, request.Slot, amountToUse),
                    inventoryItem));
                remainingAmount -= amountToUse;
            }

            if (remainingAmount > 0)
            {
                failureReason = $"insufficient usable charge items remaining={remainingAmount}";
                return false;
            }

            return true;
        }

        private List<KeyValuePair<short, ItemModel>> ConsumeItems(GameClient client, IReadOnlyList<TranscendenceInventoryItem> inventoryItems)
        {
            var updatedItems = new List<KeyValuePair<short, ItemModel>>();
            foreach (var requestedItem in inventoryItems)
            {
                client.Tamer.Inventory.RemoveOrReduceItem(
                    requestedItem.InventoryItem,
                    requestedItem.Request.Amount,
                    requestedItem.Request.Slot);

                var updatedItem = client.Tamer.Inventory.FindItemBySlot(requestedItem.Request.Slot);
                updatedItems.Add(new KeyValuePair<short, ItemModel>(
                    (short)requestedItem.Request.Slot,
                    updatedItem ?? new ItemModel { Slot = requestedItem.Request.Slot }));
            }

            return updatedItems;
        }

        private long CalculateMaterialExp(DigimonModel targetPartner, DigimonModel materialPartner, AcademyInputType inputType)
        {
            var chargeCorrect = inputType == AcademyInputType.High ? 160 : 100;

            if (materialPartner.IsRaremonType)
            {
                return RaremonFixedExp.TryGetValue(materialPartner.HatchGrade, out var fixedExp)
                    ? (long)Math.Ceiling(fixedExp * (chargeCorrect * 0.01d))
                    : 0;
            }

            var correctTable = targetPartner.SameType(materialPartner.BaseType) ? SameTypeCorrect : DifferentTypeCorrect;
            if (!correctTable.TryGetValue(materialPartner.HatchGrade, out var growthCorrect))
            {
                return 0;
            }

            var cloneLevel = Math.Max(1, (int)materialPartner.Digiclone.CloneLevel);
            var defaultExp = 250 + materialPartner.Level + ((cloneLevel - 1) * EnchantDefaultCorrect);
            return (long)Math.Ceiling((defaultExp * growthCorrect) * (chargeCorrect * 0.01d));
        }

        private bool EnsureBaseInfo(DigimonModel digimon)
        {
            try
            {
                if (digimon.BaseInfo == null || digimon.BaseInfo.Type != digimon.BaseType)
                {
                    digimon.SetBaseInfo(_statusManager.GetDigimonBaseInfo(digimon.BaseType));
                }

                return true;
            }
            catch (Exception ex)
            {
                _logger.Warning(ex, "[Transcendence] Missing base info for digimon {DigimonId} baseType {BaseType}.", digimon.Id, digimon.BaseType);
                return false;
            }
        }

        private static bool IsValidHatchGrade(DigimonHatchGradeEnum hatchGrade)
        {
            return hatchGrade == DigimonHatchGradeEnum.Default ||
                   hatchGrade == DigimonHatchGradeEnum.High ||
                   hatchGrade == DigimonHatchGradeEnum.Perfect;
        }

        private static bool IsValidInputType(AcademyInputType inputType)
        {
            return inputType == AcademyInputType.Low || inputType == AcademyInputType.High;
        }

        private static bool IsValidChargeItem(AcademyInputType inputType, int itemId)
        {
            return inputType switch
            {
                AcademyInputType.Low => itemId >= 9800 && itemId <= 9810,
                AcademyInputType.High => itemId >= 9811 && itemId <= 9821,
                _ => false
            };
        }

        private void SendChargeFailure(GameClient client, int result, string reason)
        {
            _logger.Warning("[Transcendence] Charge rejected for tamer {TamerId}: result {Result} reason {Reason}.",
                client.TamerId, result, reason);
            client.Send(new DigimonTranscendenceReceiveExpPacket(result));
        }

        private sealed class TranscendenceItemRequest
        {
            public int ItemId { get; }
            public int Slot { get; }
            public int Amount { get; }

            public TranscendenceItemRequest(int itemId, int slot, int amount)
            {
                ItemId = itemId;
                Slot = slot;
                Amount = amount;
            }
        }

        private sealed class TranscendenceInventoryItem
        {
            public TranscendenceItemRequest Request { get; }
            public ItemModel InventoryItem { get; }

            public TranscendenceInventoryItem(TranscendenceItemRequest request, ItemModel inventoryItem)
            {
                Request = request;
                InventoryItem = inventoryItem;
            }
        }

        private sealed class TranscendenceMaterialPlan
        {
            public CharacterDigimonArchiveItemModel ArchiveItem { get; }
            public DigimonModel MaterialPartner { get; }
            public short ArchiveSlot { get; }
            public long BaseExp { get; }
            public long GainedExp { get; }
            public long TotalExpAfter { get; }
            public bool BonusSuccess { get; }

            public TranscendenceMaterialPlan(
                CharacterDigimonArchiveItemModel archiveItem,
                DigimonModel materialPartner,
                short archiveSlot,
                long baseExp,
                long gainedExp,
                long totalExpAfter,
                bool bonusSuccess)
            {
                ArchiveItem = archiveItem;
                MaterialPartner = materialPartner;
                ArchiveSlot = archiveSlot;
                BaseExp = baseExp;
                GainedExp = gainedExp;
                TotalExpAfter = totalExpAfter;
                BonusSuccess = bonusSuccess;
            }
        }
    }
}
