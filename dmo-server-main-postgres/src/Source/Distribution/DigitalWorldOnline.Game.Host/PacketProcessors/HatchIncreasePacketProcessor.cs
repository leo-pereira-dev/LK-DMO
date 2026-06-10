using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;
using System.Collections.Generic;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchIncreasePacketProcessor : IGamePacketProcessor
    {
        private static readonly HashSet<int> TutorialHatchEggItemIds = new HashSet<int>
        {
            114240,
            114241,
            114242,
            114243,
            114244,
            114245
        };
        private const int MaxHatchLevelForFreeClick = 5;

        public GameServerPacketEnum Type => GameServerPacketEnum.HatchIncrease;

        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly AssetsLoader _assets;
        private readonly ConfigsLoader _configs;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public HatchIncreasePacketProcessor(
            MapServer mapServer,
            DungeonsServer dungeonsServer,
            AssetsLoader assets,
            ConfigsLoader configs,
            ILogger logger,
            ISender sender
        )
        {
            _mapServer = mapServer;
            _dungeonServer = dungeonsServer;
            _assets = assets;
            _configs = configs;
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var npcId = packet.ReadInt();
            var dataTier = packet.ReadByte();
            var consumeBackupDisk = false;

            var targetItem = client.Tamer.Incubator.EggId;
            _logger.Information(
                "Hatch increase request: tamer {TamerId} npc {NpcId} dataTier {DataTier} egg {EggId} hatchLevel {HatchLevel} backupDisk {BackupDiskId}.",
                client.TamerId,
                npcId,
                dataTier,
                targetItem,
                client.Tamer.Incubator.HatchLevel,
                client.Tamer.Incubator.BackupDiskId);

            if (targetItem == 0 && client.Tamer.Incubator.BackupDiskId > 0)
            {
                var misplacedEgg = _assets.Hatchs.FirstOrDefault(x => x.ItemId == client.Tamer.Incubator.BackupDiskId);
                if (misplacedEgg != null)
                {
                    targetItem = client.Tamer.Incubator.BackupDiskId;
                    client.Tamer.Incubator.RemoveBackupDisk();
                    client.Tamer.Incubator.InsertEgg(targetItem);

                    _logger.Warning(
                        "Recovered hatchable item from backup slot: tamer {TamerId} egg {EggId}.",
                        client.TamerId,
                        targetItem);
                }
            }

            var hatchInfo = _assets.Hatchs.FirstOrDefault(x => x.ItemId == targetItem);
            if (hatchInfo == null)
            {
                _logger.Warning(
                    "Unknown hatch info: tamer {TamerId} egg {EggId} hatchLevel {HatchLevel} backupDisk {BackupDiskId} dataTier {DataTier}.",
                    client.TamerId,
                    targetItem,
                    client.Tamer.Incubator.HatchLevel,
                    client.Tamer.Incubator.BackupDiskId,
                    dataTier);
                client.Send(new HatchIncreaseFailedPacket(client.Tamer.GeneralHandler, HatchIncreaseResultEnum.Failled));
                client.Send(new SystemMessagePacket($"Unknown hatch info for egg {targetItem}."));
                return;
            }

            if (client.Tamer.Incubator.HatchLevel == MaxHatchLevelForFreeClick)
            {
                _logger.Information(
                    "Hatch increase request at max level. Allowing free click at level {HatchLevel} for egg {EggId} without consuming data items.",
                    client.Tamer.Incubator.HatchLevel,
                    targetItem);

                SendHatchResult(client, new HatchIncreaseSucceedPacket(
                    client.Tamer.GeneralHandler,
                    client.Tamer.Incubator.HatchLevel
                ).Serialize());

                return;
            }

            if (client.Tamer.Incubator.HatchLevel > MaxHatchLevelForFreeClick)
            {
                _logger.Warning("Invalid hatch level {HatchLevel} for tamer {TamerId} and egg {EggId}.", 
                    client.Tamer.Incubator.HatchLevel,
                    client.TamerId,
                    targetItem);

                client.Send(new HatchIncreaseFailedPacket(client.Tamer.GeneralHandler, HatchIncreaseResultEnum.Failled));
                client.Send(new SystemMessagePacket("Invalid hatch level."));
                return;
            }

            var hatchConfig = _configs.Hatchs.FirstOrDefault(x => x.Type.GetHashCode() == client.Tamer.Incubator.HatchLevel + 1);
            if (hatchConfig == null)
            {
                client.Send(new HatchIncreaseFailedPacket(client.Tamer.GeneralHandler, HatchIncreaseResultEnum.Failled));
                _logger.Error($"Invalid hatch config for level {client.Tamer.Incubator.HatchLevel + 1}.");
                client.Send(new SystemMessagePacket($"Invalid hatch config for level {client.Tamer.Incubator.HatchLevel + 1}."));
                return;
            }

            _logger.Information(
                "Hatch increase resolved: tamer {TamerId} egg {EggId} hatchType {HatchType} currentLevel {HatchLevel} nextLevel {NextLevel} successChance {SuccessChance} breakChance {BreakChance} lowSection {LowSection} lowAmount {LowAmount} midSection {MidSection} midAmount {MidAmount}.",
                client.TamerId,
                targetItem,
                hatchInfo.HatchType,
                client.Tamer.Incubator.HatchLevel,
                client.Tamer.Incubator.HatchLevel + 1,
                hatchConfig.SuccessChance,
                hatchConfig.BreakChance,
                hatchInfo.LowClassDataSection,
                hatchInfo.LowClassDataAmount,
                hatchInfo.MidClassDataSection,
                hatchInfo.MidClassDataAmount);

            if (dataTier == 0)
            {
                var success = client.Tamer.Inventory.RemoveOrReduceItemsBySection(hatchInfo.LowClassDataSection, hatchInfo.LowClassDataAmount);
                if (!success)
                {
                    client.Send(new HatchIncreaseFailedPacket(client.Tamer.GeneralHandler, HatchIncreaseResultEnum.Failled));
                    _logger.Error($"Invalid low class data amount for egg {targetItem} and section {hatchInfo.LowClassDataSection}.");
                    client.Send(new SystemMessagePacket($"Invalid low class data amount for egg {targetItem} and section {hatchInfo.LowClassDataSection}."));
                    return;
                }
            }
            else
            {
                var success = client.Tamer.Inventory.RemoveOrReduceItemsBySection(hatchInfo.MidClassDataSection, hatchInfo.MidClassDataAmount);
                if (!success)
                {
                    client.Send(new HatchIncreaseFailedPacket(client.Tamer.GeneralHandler, HatchIncreaseResultEnum.Failled));
                    _logger.Error($"Invalid mid class data amount for egg {targetItem} and section {hatchInfo.MidClassDataSection}.");
                    client.Send(new SystemMessagePacket($"Invalid mid class data amount for egg {targetItem} and section {hatchInfo.MidClassDataSection}."));
                    return;
                }
            }

            var guaranteedTutorialHatch =
                TutorialHatchEggItemIds.Contains(targetItem) &&
                client.Tamer.Incubator.HatchLevel < MaxHatchLevelForFreeClick;

            if(guaranteedTutorialHatch || hatchConfig.SuccessChance >= UtilitiesFunctions.RandomDouble())
            {
                client.Tamer.Incubator.IncreaseLevel();

                SendHatchResult(client, new HatchIncreaseSucceedPacket(
                    client.Tamer.GeneralHandler,
                    client.Tamer.Incubator.HatchLevel
                ).Serialize());

                _logger.Verbose($"Character {client.TamerId} succeeded to increase egg {targetItem} to level {client.Tamer.Incubator.HatchLevel} " +
                    $"with data section {hatchInfo.LowClassDataSection} x{hatchInfo.LowClassDataAmount}.");
                _logger.Information(
                    "Hatch increase result: tamer {TamerId} egg {EggId} result Success hatchLevel {HatchLevel} dataTier {DataTier}.",
                    client.TamerId,
                    targetItem,
                    client.Tamer.Incubator.HatchLevel,
                    dataTier);
            }
            else
            {
                if (hatchConfig.BreakChance >= UtilitiesFunctions.RandomDouble())
                {
                    if (client.Tamer.Incubator.BackupDiskId > 0)
                    {
                        consumeBackupDisk = true;

                        SendHatchResult(client, new HatchIncreaseFailedPacket(
                            client.Tamer.GeneralHandler,
                            HatchIncreaseResultEnum.Backuped
                        ).Serialize());

                        _logger.Verbose($"Character {client.TamerId} failed to increase egg {targetItem} to level {client.Tamer.Incubator.HatchLevel + 1} " +
                            $"with data section {hatchInfo.MidClassDataSection} x{hatchInfo.MidClassDataAmount} and egg was saved by {client.Tamer.Incubator.BackupDiskId}.");
                        _logger.Information(
                            "Hatch increase result: tamer {TamerId} egg {EggId} result Backuped hatchLevel {HatchLevel} dataTier {DataTier} backupDisk {BackupDiskId}.",
                            client.TamerId,
                            targetItem,
                            client.Tamer.Incubator.HatchLevel,
                            dataTier,
                            client.Tamer.Incubator.BackupDiskId);
                    }
                    else
                    {
                        SendHatchResult(client, new HatchIncreaseFailedPacket(
                            client.Tamer.GeneralHandler,
                            HatchIncreaseResultEnum.Broken
                        ).Serialize());

                        _logger.Verbose($"Character {client.TamerId} failed to increase egg {targetItem} to level {client.Tamer.Incubator.HatchLevel + 1} " +
                            $"with data section {hatchInfo.MidClassDataSection} x{hatchInfo.MidClassDataAmount} and egg has broken.");

                        client.Tamer.Incubator.RemoveEgg();
                        _logger.Information(
                            "Hatch increase result: tamer {TamerId} egg {EggId} result Broken hatchLevelAfter {HatchLevel} dataTier {DataTier}.",
                            client.TamerId,
                            targetItem,
                            client.Tamer.Incubator.HatchLevel,
                            dataTier);
                    }
                }
                else
                {
                    SendHatchResult(client, new HatchIncreaseFailedPacket(
                        client.Tamer.GeneralHandler,
                        HatchIncreaseResultEnum.Failled
                    ).Serialize());

                    _logger.Verbose($"Character {client.TamerId} failed to increase egg {targetItem} to level {client.Tamer.Incubator.HatchLevel + 1} " +
                        $"with data section {hatchInfo.MidClassDataSection} x{hatchInfo.MidClassDataAmount}.");
                    _logger.Information(
                        "Hatch increase result: tamer {TamerId} egg {EggId} result Failed hatchLevel {HatchLevel} dataTier {DataTier}.",
                        client.TamerId,
                        targetItem,
                        client.Tamer.Incubator.HatchLevel,
                        dataTier);
                }
            }

            if (consumeBackupDisk)
                client.Tamer.Incubator.RemoveBackupDisk();

            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));
        }

        private void SendHatchResult(GameClient client, byte[] packet)
        {
            client.Send(packet);
        }
    }
}
