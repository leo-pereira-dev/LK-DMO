using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Utils;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class AccessoryEnchantService
    {
        private const byte Success = 1;
        private const byte Failed = 2;
        private const byte Preserved = 3;
        private const byte MaxAccessorySlots = 8;

        private static readonly HashSet<int> LegacyRenewalStones = new()
        {
            45003, 46996, 46998, 47003, 47007, 47008, 47106
        };

        private static readonly HashSet<int> LegacyDigiPowerStones = new()
        {
            45000, 47000, 46997, 46999, 10026, 47004, 10259, 47009, 47107
        };

        private static readonly HashSet<int> LegacyAccOptionStones = new()
        {
            10052, 45001, 47001, 47005
        };

        private static readonly HashSet<int> LegacyOptionValueStones = new()
        {
            10053, 45002, 47002, 47006
        };

        private readonly AssetsLoader _assets;
        private readonly ItemListBinLoader _itemListBinLoader;

        public AccessoryEnchantService(AssetsLoader assets, ItemListBinLoader itemListBinLoader)
        {
            _assets = assets;
            _itemListBinLoader = itemListBinLoader;
        }

        public AccessoryIdentifyResult Identify(ItemModel targetAccessory)
        {
            EnsureAccessoryState(targetAccessory);

            var optionInfo = ResolveAccessoryOptionInfo(targetAccessory);
            if (optionInfo == null)
                return AccessoryIdentifyResult.Fail("Invalid accessory option data.");

            RollAccessoryOptions(targetAccessory, optionInfo);
            targetAccessory.SetPower(RollInitialPower(targetAccessory));
            targetAccessory.SetReroll((byte)Math.Clamp(optionInfo.MaxReroll, byte.MinValue, byte.MaxValue));

            return AccessoryIdentifyResult.Success(optionInfo.MaxStatusCount);
        }

        public AccessoryEnchantResult Enchant(ItemModel stone, ItemModel targetAccessory, byte selectedOptionSlot, byte lockedOptionMask = 0)
        {
            EnsureAccessoryState(targetAccessory);

            var stoneInfo = ResolveStoneInfo(stone);
            if (stoneInfo.Type == AccessoryEnchantStoneType.Unknown)
                return AccessoryEnchantResult.Fail("Invalid accessory enchant stone.");

            var optionInfo = ResolveAccessoryOptionInfo(targetAccessory);
            if (optionInfo == null)
                return AccessoryEnchantResult.Fail("Invalid accessory option data.");

            return stoneInfo.Type switch
            {
                AccessoryEnchantStoneType.DigiPower => ApplyDigiPower(stoneInfo, targetAccessory),
                AccessoryEnchantStoneType.Renewal => ApplyRenewal(stoneInfo, targetAccessory, optionInfo),
                AccessoryEnchantStoneType.AccOption => ApplyAccOption(stone, targetAccessory, optionInfo, lockedOptionMask),
                AccessoryEnchantStoneType.OptionValue => ApplyOptionValue(targetAccessory, optionInfo, selectedOptionSlot),
                _ => AccessoryEnchantResult.Fail("Unsupported accessory enchant stone.")
            };
        }

        private AccessoryEnchantResult ApplyDigiPower(AccessoryEnchantStoneInfo stoneInfo, ItemModel targetAccessory)
        {
            if (targetAccessory.Power >= 200)
                return AccessoryEnchantResult.Preserve("Accessory power is already at maximum.");

            var oldPower = targetAccessory.Power;
            var delta = RollStoneValue(stoneInfo, fallbackValue: 1);
            if (delta == 0)
                delta = 1;

            var newPower = Math.Clamp(oldPower + delta, 0, 200);
            targetAccessory.SetPower((byte)newPower);

            var result = newPower > oldPower ? Success : newPower < oldPower ? Failed : Preserved;
            return new AccessoryEnchantResult(result, true, $"DigiPower changed from {oldPower} to {newPower}.");
        }

        private AccessoryEnchantResult ApplyRenewal(
            AccessoryEnchantStoneInfo stoneInfo,
            ItemModel targetAccessory,
            AccessoryOptionInfo optionInfo)
        {
            var addAmount = Math.Max(1, RollStoneValue(stoneInfo, fallbackValue: 1));
            var maxReroll = Math.Max(0, optionInfo.MaxReroll);

            if (targetAccessory.RerollLeft + addAmount > maxReroll)
                return AccessoryEnchantResult.Preserve("Accessory renewal count is already at maximum.");

            targetAccessory.SetReroll((byte)Math.Clamp(targetAccessory.RerollLeft + addAmount, byte.MinValue, byte.MaxValue));
            return AccessoryEnchantResult.SuccessResult("Accessory renewal count increased.");
        }

        private AccessoryEnchantResult ApplyAccOption(
            ItemModel stone,
            ItemModel targetAccessory,
            AccessoryOptionInfo optionInfo,
            byte lockedOptionMask)
        {
            var lockedStatuses = GetLockedStatuses(targetAccessory, lockedOptionMask);
            if (lockedStatuses.Count > 2)
                return AccessoryEnchantResult.Fail("Only two accessory options can be locked.");

            var stoneAmount = lockedStatuses.Count == 0 ? 1 : lockedStatuses.Count * 2;
            if (stone.Amount < stoneAmount)
                return AccessoryEnchantResult.Fail("Not enough accessory enchant stones.");

            RollAccessoryOptions(targetAccessory, optionInfo, lockedStatuses);

            return AccessoryEnchantResult.SuccessResult("Accessory options rerolled.", stoneAmount);
        }

        private AccessoryEnchantResult ApplyOptionValue(
            ItemModel targetAccessory,
            AccessoryOptionInfo optionInfo,
            byte selectedOptionSlot)
        {
            if (selectedOptionSlot >= MaxAccessorySlots || selectedOptionSlot >= targetAccessory.AccessoryStatus.Count)
                return AccessoryEnchantResult.Fail("Invalid accessory option slot.");

            targetAccessory.AccessoryStatus = targetAccessory.AccessoryStatus.OrderBy(x => x.Slot).ToList();
            var selectedStatus = targetAccessory.AccessoryStatus[selectedOptionSlot];
            if (selectedStatus.Value <= 0 || selectedStatus.Type == default)
                return AccessoryEnchantResult.Fail("Selected accessory option is empty.");

            var baseType = selectedStatus.Type.NormalizeAccessoryStatus();
            var range = optionInfo.Ranges.FirstOrDefault(x => x.Type == baseType);
            if (range == null)
                return AccessoryEnchantResult.Fail("Selected accessory option is not supported by this item.");

            selectedStatus.SetValue(UtilitiesFunctions.RandomShort(range.MinValue, range.MaxValue));

            return AccessoryEnchantResult.SuccessResult("Accessory option value rerolled.");
        }

        private void RollAccessoryOptions(
            ItemModel targetAccessory,
            AccessoryOptionInfo optionInfo,
            IReadOnlyCollection<LockedAccessoryStatus>? lockedStatuses = null)
        {
            targetAccessory.AccessoryStatus = targetAccessory.AccessoryStatus.OrderBy(x => x.Slot).ToList();
            var lockedBySlot = lockedStatuses?.ToDictionary(x => x.Slot) ?? new Dictionary<byte, LockedAccessoryStatus>();

            foreach (var status in targetAccessory.AccessoryStatus)
            {
                status.SetType(default);
                status.SetValue(0);
            }

            var statusAmount = Math.Max(0, Math.Min(targetAccessory.AccessoryStatus.Count, optionInfo.MaxStatusCount));
            foreach (var lockedStatus in lockedBySlot.Values.Where(x => x.Slot < statusAmount))
            {
                targetAccessory.AccessoryStatus[lockedStatus.Slot].SetType(lockedStatus.Type);
                targetAccessory.AccessoryStatus[lockedStatus.Slot].SetValue(lockedStatus.Value);
            }

            for (var i = 0; i < statusAmount; i++)
            {
                if (lockedBySlot.ContainsKey((byte)i))
                    continue;

                var possibleStatus = optionInfo.Options
                    .Where(x => AccessoryStatusAmount(targetAccessory, x.Type) < x.MaxAmount)
                    .ToList();

                if (!possibleStatus.Any())
                    break;

                var selectedIndex = UtilitiesFunctions.RandomInt(0, possibleStatus.Count - 1);
                var newStatus = possibleStatus[selectedIndex];

                targetAccessory.AccessoryStatus[i].SetType(newStatus.Type);
                targetAccessory.AccessoryStatus[i].SetValue(UtilitiesFunctions.RandomShort(newStatus.MinValue, newStatus.MaxValue));
            }
        }

        private static List<LockedAccessoryStatus> GetLockedStatuses(ItemModel targetAccessory, byte lockedOptionMask)
        {
            if (lockedOptionMask == 0)
                return new List<LockedAccessoryStatus>();

            targetAccessory.AccessoryStatus = targetAccessory.AccessoryStatus.OrderBy(x => x.Slot).ToList();
            var lockedStatuses = new List<LockedAccessoryStatus>();
            for (byte slot = 0; slot < MaxAccessorySlots && slot < targetAccessory.AccessoryStatus.Count; slot++)
            {
                if ((lockedOptionMask & (1 << slot)) == 0)
                    continue;

                var status = targetAccessory.AccessoryStatus[slot];
                if (status.Value <= 0 || status.Type == default)
                    continue;

                lockedStatuses.Add(new LockedAccessoryStatus(slot, status.Type, status.Value));
            }

            return lockedStatuses;
        }

        private byte RollInitialPower(ItemModel targetAccessory)
        {
            var rankRange = _itemListBinLoader.Data.Rank.FirstOrDefault(x => x.ItemId == targetAccessory.ItemId);
            if (rankRange != null)
            {
                var min = (byte)Math.Clamp(rankRange.Min, byte.MinValue, byte.MaxValue);
                var max = (byte)Math.Clamp(rankRange.Max, min, byte.MaxValue);
                return UtilitiesFunctions.RandomByte(min, max);
            }

            return UtilitiesFunctions.RandomByte(95, 102);
        }

        private AccessoryEnchantStoneInfo ResolveStoneInfo(ItemModel stone)
        {
            var itemInfo = ResolveItemInfo(stone);
            var skillCode = itemInfo?.SkillCode ?? 0;

            if (skillCode > 0)
            {
                var record = _itemListBinLoader.Data.AccessoryEnchants.FirstOrDefault(x => x.Index == skillCode);
                if (record != null && Enum.IsDefined(typeof(AccessoryEnchantStoneType), (int)record.ItemType))
                {
                    return new AccessoryEnchantStoneInfo(
                        (AccessoryEnchantStoneType)record.ItemType,
                        record.MinValue,
                        record.MaxValue,
                        true);
                }
            }

            return ResolveLegacyStoneInfo(stone.ItemId);
        }

        private static AccessoryEnchantStoneInfo ResolveLegacyStoneInfo(int itemId)
        {
            if (LegacyDigiPowerStones.Contains(itemId))
                return new AccessoryEnchantStoneInfo(AccessoryEnchantStoneType.DigiPower, 1, ResolveLegacyPowerDelta(itemId), false);

            if (LegacyRenewalStones.Contains(itemId))
                return new AccessoryEnchantStoneInfo(AccessoryEnchantStoneType.Renewal, ResolveLegacyRenewalDelta(itemId), ResolveLegacyRenewalDelta(itemId), false);

            if (LegacyAccOptionStones.Contains(itemId))
                return new AccessoryEnchantStoneInfo(AccessoryEnchantStoneType.AccOption, 0, 0, false);

            if (LegacyOptionValueStones.Contains(itemId))
                return new AccessoryEnchantStoneInfo(AccessoryEnchantStoneType.OptionValue, 0, 0, false);

            return new AccessoryEnchantStoneInfo(AccessoryEnchantStoneType.Unknown, 0, 0, false);
        }

        private static short ResolveLegacyPowerDelta(int itemId)
        {
            if (itemId is 10259 or 47009 or 47107)
                return 10;

            if (itemId is 10026 or 47004)
                return 3;

            if (itemId is 46997 or 46999)
                return 2;

            return 1;
        }

        private static short ResolveLegacyRenewalDelta(int itemId)
        {
            return itemId is 47008 or 47106 ? (short)5 : (short)1;
        }

        private static int RollStoneValue(AccessoryEnchantStoneInfo stoneInfo, int fallbackValue)
        {
            var min = Math.Min(stoneInfo.MinValue, stoneInfo.MaxValue);
            var max = Math.Max(stoneInfo.MinValue, stoneInfo.MaxValue);

            if (min == 0 && max == 0)
                return fallbackValue;

            return UtilitiesFunctions.RandomShort(min, max);
        }

        private AccessoryOptionInfo? ResolveAccessoryOptionInfo(ItemModel targetAccessory)
        {
            var itemInfo = ResolveItemInfo(targetAccessory);
            if (itemInfo == null)
                return null;

            var optionKey = ResolveAccessoryOptionKey(itemInfo);
            var record = _itemListBinLoader.Data.AccessoryOptions.FirstOrDefault(x => x.ItemType == optionKey);
            if (record == null)
                return null;

            var validOptions = record.Options
                .Where(x => Enum.IsDefined(typeof(AccessoryStatusTypeEnum), (int)x.OptionType))
                .Select(x => new AccessoryOptionEntry(
                    (AccessoryStatusTypeEnum)x.OptionType,
                    (short)Math.Clamp((long)x.MinValue, short.MinValue, short.MaxValue),
                    (short)Math.Clamp((long)x.MaxValue, short.MinValue, short.MaxValue),
                    0))
                .ToList();

            if (!validOptions.Any())
                return null;

            var maxAmountByType = validOptions
                .GroupBy(x => x.Type.NormalizeAccessoryStatus())
                .ToDictionary(x => x.Key, x => x.Count());

            var weightedOptions = validOptions
                .Select(x => new AccessoryOptionEntry(
                    x.Type,
                    x.MinValue,
                    x.MaxValue,
                    maxAmountByType[x.Type.NormalizeAccessoryStatus()]))
                .ToList();

            var ranges = validOptions
                .GroupBy(x => x.Type.NormalizeAccessoryStatus())
                .Select(group =>
                {
                    var min = group.Min(x => x.MinValue);
                    var max = group.Max(x => x.MaxValue);
                    return new AccessoryOptionRange(group.Key, min, max, group.Count());
                })
                .ToList();

            var maxStatusCount = Math.Max(0, Math.Min((int)record.MinValue, weightedOptions.Count));
            var maxReroll = Math.Max(0, (int)record.MaxValue);
            return new AccessoryOptionInfo(maxStatusCount, maxReroll, weightedOptions, ranges);
        }

        private uint ResolveAccessoryOptionKey(ItemAssetModel itemInfo)
        {
            if (itemInfo.SkillCode > 0 && _itemListBinLoader.Data.AccessoryOptions.Any(x => x.ItemType == (uint)itemInfo.SkillCode))
                return (uint)itemInfo.SkillCode;

            return (uint)itemInfo.Type;
        }

        private ItemAssetModel? ResolveItemInfo(ItemModel item)
        {
            if (item.ItemInfo != null)
                return item.ItemInfo;

            var itemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == item.ItemId);
            if (itemInfo != null)
                item.SetItemInfo(itemInfo);

            return itemInfo;
        }

        private static void EnsureAccessoryState(ItemModel item)
        {
            if (item.AccessoryStatus == null || item.AccessoryStatus.Count != MaxAccessorySlots)
            {
                item.AccessoryStatus = Enumerable.Range(0, MaxAccessorySlots)
                    .Select(slot => new ItemAccessoryStatusModel((byte)slot))
                    .ToList();
            }
        }

        private static int AccessoryStatusAmount(ItemModel targetAccessory, AccessoryStatusTypeEnum type)
        {
            var baseType = type.NormalizeAccessoryStatus();
            return targetAccessory.AccessoryStatus.Count(x => x.Type.NormalizeAccessoryStatus() == baseType);
        }

        private enum AccessoryEnchantStoneType
        {
            Unknown = 0,
            DigiPower = 1,
            Renewal = 2,
            AccOption = 3,
            OptionValue = 4
        }

        private sealed record AccessoryEnchantStoneInfo(
            AccessoryEnchantStoneType Type,
            short MinValue,
            short MaxValue,
            bool FromOfficialTable);

        private sealed record AccessoryOptionInfo(
            int MaxStatusCount,
            int MaxReroll,
            List<AccessoryOptionEntry> Options,
            List<AccessoryOptionRange> Ranges);

        private sealed record AccessoryOptionEntry(AccessoryStatusTypeEnum Type, short MinValue, short MaxValue, int MaxAmount);

        private sealed record AccessoryOptionRange(AccessoryStatusTypeEnum Type, short MinValue, short MaxValue, int MaxAmount);

        private sealed record LockedAccessoryStatus(byte Slot, AccessoryStatusTypeEnum Type, short Value);
    }

    public sealed record AccessoryIdentifyResult(bool Applied, string Message, int StatusAmount)
    {
        public static AccessoryIdentifyResult Success(int statusAmount) => new(true, "Accessory identified.", statusAmount);

        public static AccessoryIdentifyResult Fail(string message) => new(false, message, 0);
    }

    public sealed record AccessoryEnchantResult(byte Result, bool ConsumeStone, string Message, int StoneAmount = 1)
    {
        public bool Applied => Result == 1;

        public static AccessoryEnchantResult SuccessResult(string message, int stoneAmount = 1) => new(1, true, message, stoneAmount);

        public static AccessoryEnchantResult Fail(string message) => new(2, false, message, 0);

        public static AccessoryEnchantResult Preserve(string message) => new(3, false, message, 0);
    }
}
