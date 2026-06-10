using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchFinishPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchFinish;

        private readonly StatusManager _statusManager;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public HatchFinishPacketProcessor(
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

            packet.Skip(4); // Portable incubator inventory slot.
            var digiName = NormalizeHatchName(packet.ReadString(), client.Tamer.Name);

            if (client.Partner == null)
            {
                _logger.Warning("Rejected hatch finish for tamer {TamerId}: invalid partner.", client.TamerId);
                client.Send(new SystemMessagePacket("Unable to hatch without an active partner."));
                return;
            }

            _logger.Information(
                "Hatch finish request: tamer {TamerId} name {DigimonName} incubatorEgg {EggId} hatchLevel {HatchLevel} backupDisk {BackupDiskId} currentSlots [{Slots}].",
                client.TamerId,
                digiName,
                client.Tamer.Incubator.EggId,
                client.Tamer.Incubator.HatchLevel,
                client.Tamer.Incubator.BackupDiskId,
                string.Join(",", client.Tamer.Digimons.Select(x => $"{x.Id}:{x.Slot}:{x.BaseType}:{x.Name}")));

            var hatchInfo = _assets.Hatchs.FirstOrDefault(x => x.ItemId == client.Tamer.Incubator.EggId);
            if (hatchInfo == null)
            {
                _logger.Warning($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}.");
                client.Send(new SystemMessagePacket($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}."));
                return;
            }

            if (client.Tamer.Incubator.HatchLevel < 3)
            {
                _logger.Warning($"Character {client.TamerId} tried to finish hatch with egg {client.Tamer.Incubator.EggId} at level {client.Tamer.Incubator.HatchLevel}.");
                client.Send(new SystemMessagePacket("DigiEgg must be at least level 3 to hatch."));
                return;
            }

            client.Tamer.CompactActiveDigimonSlots();
            await _sender.Send(new UpdateCharacterDigimonsOrderCommand(client.Tamer));

            var activeSlotCount = client.Tamer.GetNextActiveDigimonSlot();

            _logger.Information(
                "Hatch finish slot scan: tamer {TamerId} openedSlots {OpenedSlots} activeSlotCount {ActiveSlotCount}.",
                client.TamerId,
                client.Tamer.DigimonSlots,
                activeSlotCount);

            if (activeSlotCount >= client.Tamer.DigimonSlots)
            {
                _logger.Warning($"Character {client.TamerId} tried to hatch {client.Tamer.Incubator.EggId} without an available digivice slot.");
                client.Send(new SystemMessagePacket("No available digivice slot for the hatched Digimon."));
                return;
            }

            var targetSlot = (byte)activeSlotCount;

            var digimonBaseInfo = _assets.DigimonBaseInfo.FirstOrDefault(x => x.Type == hatchInfo.HatchType);
            var evolutionInfo = _assets.EvolutionInfo.FirstOrDefault(x => x.Type == hatchInfo.HatchType);

            if (digimonBaseInfo == null || evolutionInfo == null || !evolutionInfo.Lines.Any())
            {
                _logger.Warning(
                    "Rejected hatch finish for tamer {TamerId}: egg {EggId} resolves to digimon {BaseType}, but baseInfo={HasBaseInfo}, evolutionInfo={HasEvolutionInfo}, evolutionLines={EvolutionLines}.",
                    client.TamerId,
                    client.Tamer.Incubator.EggId,
                    hatchInfo.HatchType,
                    digimonBaseInfo != null,
                    evolutionInfo != null,
                    evolutionInfo?.Lines.Count ?? 0);
                client.Send(new SystemMessagePacket($"Unknown digimon info for {hatchInfo.HatchType}."));
                return;
            }

            var newDigimon = DigimonModel.Create(
                digiName,
                hatchInfo.HatchType,
                hatchInfo.HatchType,
                (DigimonHatchGradeEnum)client.Tamer.Incubator.HatchLevel,
                client.Tamer.Incubator.GetLevelSize(),
                targetSlot
            );

            newDigimon.NewLocation(
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y
            );

            newDigimon.SetBaseInfo(digimonBaseInfo);

            try
            {
                newDigimon.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        newDigimon.BaseType,
                        newDigimon.Level,
                        newDigimon.Size
                    )
                );
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Rejected hatch finish for tamer {TamerId}: could not resolve base status for digimon {BaseType}, level {Level}, size {Size}.",
                    client.TamerId,
                    newDigimon.BaseType,
                    newDigimon.Level,
                    newDigimon.Size);
                client.Send(new SystemMessagePacket($"Unknown digimon status for {newDigimon.BaseType}."));
                return;
            }

            newDigimon.FullHeal();

            newDigimon.AddEvolutions(evolutionInfo);

            if (newDigimon.BaseInfo == null || newDigimon.BaseStatus == null || !newDigimon.Evolutions.Any())
            {
                _logger.Warning($"Unknown digimon info for {newDigimon.BaseType}.");
                client.Send(new SystemMessagePacket($"Unknown digimon info for {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetTamer(client.Tamer);

            DigitalWorldOnline.Commons.DTOs.Digimon.DigimonDTO? digimonInfo;
            try
            {
                digimonInfo = await _sender.Send(new CreateDigimonCommand(newDigimon));
            }
            catch (Exception ex)
            {
                _logger.Error(
                    ex,
                    "Rejected hatch finish for tamer {TamerId}: failed to persist digimon {BaseType}.",
                    client.TamerId,
                    newDigimon.BaseType);
                client.Send(new SystemMessagePacket($"Could not hatch digimon {newDigimon.BaseType}."));
                return;
            }

            if (digimonInfo == null)
            {
                _logger.Warning($"Could not persist hatched digimon {newDigimon.BaseType} for character {client.TamerId}.");
                client.Send(new SystemMessagePacket($"Could not hatch digimon {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetId(digimonInfo.Id);
            var evolutionSlot = -1;

            foreach (var digimon in newDigimon.Evolutions)
            {
                evolutionSlot++;

                if (evolutionSlot >= digimonInfo.Evolutions.Count)
                {
                    _logger.Warning(
                        "Hatch finish persisted fewer evolutions than expected for tamer {TamerId}, digimon {DigimonId}: expected slot {EvolutionSlot}, persisted count {PersistedCount}.",
                        client.TamerId,
                        newDigimon.Id,
                        evolutionSlot,
                        digimonInfo.Evolutions.Count);
                    continue;
                }

                var evolution = digimonInfo.Evolutions[evolutionSlot];

                if (evolution != null)
                {
                    digimon.SetId(evolution.Id);

                    var skillSlot = -1;

                    foreach (var skill in digimon.Skills)
                    {
                        skillSlot++;

                        if (skillSlot >= evolution.Skills.Count)
                        {
                            _logger.Warning(
                                "Hatch finish persisted fewer evolution skills than expected for tamer {TamerId}, digimon {DigimonId}, evolution {EvolutionId}: expected slot {SkillSlot}, persisted count {PersistedCount}.",
                                client.TamerId,
                                newDigimon.Id,
                                evolution.Id,
                                skillSlot,
                                evolution.Skills.Count);
                            continue;
                        }

                        var dtoSkill = evolution.Skills[skillSlot];

                        skill.SetId(dtoSkill.Id);
                    }
                }
            }

            client.Tamer.Incubator.RemoveEgg();
            client.Tamer.Incubator.RemoveBackupDisk();

            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));

            client.Tamer.AddDigimon(newDigimon);

            // Slot 0 is the active partner on the server, but the client receives
            // only mercenary slots here and writes to slot - 1. A DB slot of 1
            // must therefore be sent as client slot 1, not 2.
            var displaySlot = newDigimon.Slot;

            var hatchFinishPacket = new HatchFinishPacket(newDigimon, (uint)(client.Partner.GeneralHandler + 1000), displaySlot).Serialize();
            _logger.Information(
                "Hatch finish send: tamer {TamerId} digimon {DigimonId} name {DigimonName} base {BaseType} slot {Slot} clientSlot {ClientSlot} clientIndex {ClientIndex} tempHandler {TemporaryHandler} level {Level} size {Size} currentHp {CurrentHp} currentDs {CurrentDs} bytes {Bytes}.",
                client.TamerId,
                newDigimon.Id,
                newDigimon.Name,
                newDigimon.BaseType,
                newDigimon.Slot,
                displaySlot,
                displaySlot - 1,
                client.Partner.GeneralHandler + 1000,
                newDigimon.Level,
                newDigimon.Size,
                newDigimon.CurrentHp,
                newDigimon.CurrentDs,
                hatchFinishPacket.Length);
            client.Send(hatchFinishPacket);

            await UpdateClientActionQuestProgress(client, 7, newDigimon.BaseType, "hatch");

            if (client.Tamer.Incubator.PerfectSize(newDigimon.HatchGrade, newDigimon.Size))
            {
                _mapServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
                _dungeonServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
            }

            _logger.Verbose($"Character {client.TamerId} hatched {newDigimon.Id}({newDigimon.BaseType}) with grade {newDigimon.HatchGrade} and size {newDigimon.Size}.");
        }

        private async Task UpdateClientActionQuestProgress(GameClient client, int actionId, int actionValue, string actionName)
        {
            if (!client.Tamer.Progress.InProgressQuestData.Any())
            {
                _logger.Information(
                    "Quest {ActionName}: no in-progress quests for tamer {TamerId} while processing action {ActionId}/{ActionValue}.",
                    actionName,
                    client.TamerId,
                    actionId,
                    actionValue);
                return;
            }

            foreach (var questInProgress in client.Tamer.Progress.InProgressQuestData)
            {
                var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                if (questInfo == null)
                {
                    _logger.Warning(
                        "Quest {QuestId} is in progress for tamer {TamerId}, but no asset was loaded while processing action {ActionName}.",
                        questInProgress.QuestId,
                        client.TamerId,
                        actionName);
                    continue;
                }

                var goalIndex = questInfo.QuestGoals.FindIndex(x =>
                    x.GoalType == QuestGoalTypeEnum.ClientAction &&
                    x.GoalId == actionId &&
                    (x.CurTypeCount == 0 || x.CurTypeCount == actionValue));

                if (goalIndex < 0)
                {
                    var actionGoals = string.Join(", ", questInfo.QuestGoals
                        .Select((goal, index) => new { goal, index })
                        .Where(x => x.goal.GoalType == QuestGoalTypeEnum.ClientAction)
                        .Select(x => $"idx={x.index}/action={x.goal.GoalId}/cur={x.goal.CurTypeCount}/amount={x.goal.GoalAmount}"));

                    _logger.Information(
                        "Quest {ActionName}: no matching client-action goal for tamer {TamerId}, quest {QuestId}, action {ActionId}/{ActionValue}. Available client-action goals: [{Goals}]",
                        actionName,
                        client.TamerId,
                        questInProgress.QuestId,
                        actionId,
                        actionValue,
                        actionGoals);
                    continue;
                }

                var currentGoalValue = questInProgress.GetGoalValue(goalIndex);
                var targetGoalValue = questInfo.QuestGoals[goalIndex].GoalAmount;
                if (currentGoalValue >= targetGoalValue)
                {
                    _logger.Information(
                        "Quest {ActionName}: goal already complete for tamer {TamerId}, quest {QuestId}, action {ActionId}/{ActionValue}, goal {GoalIndex}, value {Current}/{Target}.",
                        actionName,
                        client.TamerId,
                        questInProgress.QuestId,
                        actionId,
                        actionValue,
                        goalIndex,
                        currentGoalValue,
                        targetGoalValue);
                    return;
                }

                var updatedGoalValue = (byte)Math.Min(byte.MaxValue, Math.Min(targetGoalValue, currentGoalValue + 1));

                questInProgress.UpdateCondition(goalIndex, updatedGoalValue);
                client.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, updatedGoalValue));
                await _sender.Send(new UpdateCharacterInProgressCommand(questInProgress));

                _logger.Information(
                    "Quest {QuestId} {ActionName} goal updated for tamer {TamerId}: action {ActionId}/{ActionValue}, goal {GoalIndex}, value {Current}/{Target}.",
                    questInProgress.QuestId,
                    actionName,
                    client.TamerId,
                    actionId,
                    actionValue,
                    goalIndex,
                    updatedGoalValue,
                    targetGoalValue);
                return;
            }
        }

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
