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
using System.Collections.Concurrent;
using System.Diagnostics;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class ItemRerollPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.ItemReroll;

        private const byte RerollFailed = 2;
        private const byte RerollPreserved = 3;
        private const int EnchantLockWaitMilliseconds = 2000;
        private const int MinimumEnchantIntervalMilliseconds = 250;

        private static readonly ConcurrentDictionary<long, SemaphoreSlim> AccessoryEnchantLocks = new();
        private static readonly ConcurrentDictionary<long, long> LastAccessoryEnchantTicks = new();

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
            int tamerHandle;
            short stoneSlot;
            short accessorySlot;
            byte selectedOptionSlot = 0;
            byte lockedOptionMask = 0;

            try
            {
                var packet = new GamePacketReader(packetData);

                tamerHandle = packet.ReadInt();
                stoneSlot = packet.ReadShort();
                accessorySlot = packet.ReadShort();

                var remaining = (packet.Length - 2) - (int)packet.Packet.Position;
                if (remaining > 0)
                    selectedOptionSlot = packet.ReadByte();

                remaining = (packet.Length - 2) - (int)packet.Packet.Position;
                if (remaining > 0)
                    lockedOptionMask = packet.ReadByte();
            }
            catch (Exception ex)
            {
                _logger.Warning(
                    ex,
                    "AccStoneUse received malformed packet: tamer={TamerId} account={AccountId} len={Length}",
                    client.TamerId,
                    client.AccountId,
                    packetData.Length);
                SendRerollFallback(client, RerollFailed, 0, "Invalid accessory enchant request.");
                return;
            }

            var lockKey = client.TamerId != 0 ? client.TamerId : client.AccountId;
            var enchantLock = AccessoryEnchantLocks.GetOrAdd(lockKey, _ => new SemaphoreSlim(1, 1));

            if (!await enchantLock.WaitAsync(TimeSpan.FromMilliseconds(EnchantLockWaitMilliseconds)))
            {
                _logger.Warning(
                    "AccStoneUse rejected because a previous request is still processing: tamer={TamerId} account={AccountId} stoneSlot={StoneSlot} accessorySlot={AccessorySlot}",
                    client.TamerId,
                    client.AccountId,
                    stoneSlot,
                    accessorySlot);
                SendRerollFallback(client, RerollPreserved, accessorySlot, "Accessory enchant is still processing.");
                return;
            }

            try
            {
                if (IsInsideEnchantCooldown(lockKey, out var elapsedMilliseconds))
                {
                    _logger.Warning(
                        "AccStoneUse rejected by cooldown: tamer={TamerId} account={AccountId} stoneSlot={StoneSlot} accessorySlot={AccessorySlot} elapsedMs={ElapsedMs}",
                        client.TamerId,
                        client.AccountId,
                        stoneSlot,
                        accessorySlot,
                        elapsedMilliseconds);
                    SendRerollFallback(client, RerollPreserved, accessorySlot, "Please wait before using another accessory enchant stone.");
                    return;
                }

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
                    SendRerollFallback(client, RerollFailed, accessorySlot, "Invalid accessory enchant stone.");
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

                LastAccessoryEnchantTicks[lockKey] = Stopwatch.GetTimestamp();

                var stoneItemId = stone.ItemId;
                var accessorySnapshot = CaptureAccessoryState(targetAccessory);
                var result = _accessoryEnchantService.Enchant(stone, targetAccessory, selectedOptionSlot, lockedOptionMask);
                if (result.ConsumeStone)
                {
                    stone = client.Tamer.Inventory.FindItemBySlot(stoneSlot);
                    if (!IsSameStoneStack(stone, stoneItemId, result.StoneAmount))
                    {
                        RestoreAccessoryState(targetAccessory, accessorySnapshot);
                        _logger.Warning(
                            "AccStoneUse canceled because the stone stack changed before consume: tamer={TamerId} stoneSlot={StoneSlot} expectedItemId={ExpectedItemId} requiredAmount={RequiredAmount} currentItemId={CurrentItemId} currentAmount={CurrentAmount}",
                            client.TamerId,
                            stoneSlot,
                            stoneItemId,
                            result.StoneAmount,
                            stone?.ItemId ?? 0,
                            stone?.Amount ?? 0);
                        SendRerollFallback(client, RerollFailed, accessorySlot, "Accessory enchant stone changed. Try again.");
                        return;
                    }

                    if (!client.Tamer.Inventory.RemoveOrReduceItem(stone, result.StoneAmount, stoneSlot))
                    {
                        RestoreAccessoryState(targetAccessory, accessorySnapshot);
                        _logger.Warning(
                            "AccStoneUse canceled because stone consume failed: tamer={TamerId} stoneSlot={StoneSlot} stoneItemId={StoneItemId} requiredAmount={RequiredAmount}",
                            client.TamerId,
                            stoneSlot,
                            stoneItemId,
                            result.StoneAmount);
                        SendRerollFallback(client, RerollFailed, accessorySlot, "Accessory enchant stone could not be consumed. Try again.");
                        return;
                    }
                }

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
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "AccStoneUse failed unexpectedly: tamer={TamerId} account={AccountId} accessorySlot={AccessorySlot}",
                    client.TamerId,
                    client.AccountId,
                    accessorySlot);
                SendRerollFallback(client, RerollFailed, accessorySlot, "Accessory enchant failed.");
            }
            finally
            {
                enchantLock.Release();
            }
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

        private static bool IsInsideEnchantCooldown(long lockKey, out long elapsedMilliseconds)
        {
            elapsedMilliseconds = MinimumEnchantIntervalMilliseconds;
            if (!LastAccessoryEnchantTicks.TryGetValue(lockKey, out var lastTicks))
                return false;

            elapsedMilliseconds = ElapsedMilliseconds(lastTicks, Stopwatch.GetTimestamp());
            return elapsedMilliseconds < MinimumEnchantIntervalMilliseconds;
        }

        private static long ElapsedMilliseconds(long startTicks, long endTicks)
        {
            return (endTicks - startTicks) * 1000 / Stopwatch.Frequency;
        }

        private static bool IsSameStoneStack(ItemModel? stone, int expectedItemId, int requiredAmount)
        {
            return stone != null &&
                   stone.ItemId == expectedItemId &&
                   stone.Amount >= requiredAmount;
        }

        private static AccessoryStateSnapshot CaptureAccessoryState(ItemModel item)
        {
            var statuses = item.AccessoryStatus
                .Select(status => new AccessoryStatusSnapshot(
                    status.Id,
                    status.ItemId,
                    status.Slot,
                    status.Type,
                    status.Value))
                .ToList();

            return new AccessoryStateSnapshot(item.Power, item.RerollLeft, statuses);
        }

        private static void RestoreAccessoryState(ItemModel item, AccessoryStateSnapshot snapshot)
        {
            item.SetPower(snapshot.Power);
            item.SetReroll(snapshot.RerollLeft);
            item.AccessoryStatus = snapshot.Statuses
                .Select(status => new ItemAccessoryStatusModel(status.Slot)
                {
                    Id = status.Id,
                    ItemId = status.ItemId,
                    Item = item,
                    Type = status.Type,
                    Value = status.Value
                })
                .ToList();
        }

        private sealed record AccessoryStateSnapshot(
            byte Power,
            byte RerollLeft,
            List<AccessoryStatusSnapshot> Statuses);

        private sealed record AccessoryStatusSnapshot(
            Guid Id,
            Guid ItemId,
            byte Slot,
            AccessoryStatusTypeEnum Type,
            short Value);
    }
}
