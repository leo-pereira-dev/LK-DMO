using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.Map;
using DigitalWorldOnline.Commons.Extensions;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Config;
using DigitalWorldOnline.Commons.Models.Map;
using DigitalWorldOnline.Commons.Models.Summon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.GameServer.Arena;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Commons.ViewModel.Players;
using DigitalWorldOnline.Commons.Writers;
using DigitalWorldOnline.Game.Managers;
using Microsoft.Extensions.Logging;
using System.Diagnostics;

namespace DigitalWorldOnline.GameHost
{
    public sealed partial class DungeonsServer
    {
        private Dictionary<int, List<QuestLootDropRule>>? _questLootDropRulesByMob;

        private readonly record struct QuestLootDropRule(int ItemId);

        private void TryApplyEquipmentSetWhenHit(MapInstance map, long? tamerId)
        {
            if (tamerId == null)
                return;

            var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamerId.Value);
            if (targetClient == null)
                return;

            _equipmentSetBonusService.TryApplyPartnerTrigger(
                targetClient,
                EquipmentSetBonusTrigger.WhenHit,
                packet => map.BroadcastForTamerViewsAndSelf(targetClient.TamerId, packet));
        }

        private void MonsterOperation(MapInstance map)
        {
            if (!map.ConnectedTamers.Any())
                return;

            var stopwatch = new Stopwatch();
            stopwatch.Start();

            map.UpdateMapMobs(_assets.NpcColiseum);

            foreach (var mob in map.Mobs)
            {
                if (!mob.AwaitingKillSpawn && DateTime.Now > mob.ViewCheckTime)
                {
                    if (mob.CurrentAction == MobActionEnum.Destroy)
                        continue;

                    mob.SetViewCheckTime(2);

                    mob.TamersViewing.RemoveAll(x => !map.ConnectedTamers.Select(y => y.Id).Contains(x));

                    var nearTamers = map.NearestTamers(mob.Id);

                    if (!nearTamers.Any() && !mob.TamersViewing.Any())
                        continue;

                    if (!mob.Dead && mob.CurrentAction != MobActionEnum.Destroy)
                    {
                        nearTamers.ForEach(nearTamer =>
                        {
                            if (!mob.TamersViewing.Contains(nearTamer))
                            {
                                mob.TamersViewing.Add(nearTamer);

                                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == nearTamer);

                                targetClient?.Send(new LoadMobsPacket(mob));
                                targetClient?.Send(new LoadBuffsPacket(mob));
                            }
                        });
                    }

                    var farTamers = map.ConnectedTamers.Select(x => x.Id).Except(nearTamers).ToList();

                    farTamers.ForEach(farTamer =>
                    {
                        if (mob.TamersViewing.Contains(farTamer))
                        {
                            var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == farTamer);

                            mob.TamersViewing.Remove(farTamer);
                            targetClient?.Send(new UnloadMobsPacket(mob));
                        }
                    });
                }

                if (!mob.CanAct)
                    continue;

                MobsOperation(map, mob);

                mob.SetNextAction();
            }

            map.UpdateMapMobs(true);

            foreach (var mob in map.SummonMobs)
            {
                if (DateTime.Now > mob.ViewCheckTime)
                {
                    mob.SetViewCheckTime(2);

                    mob.TamersViewing.RemoveAll(x => !map.ConnectedTamers.Select(y => y.Id).Contains(x));

                    var nearTamers = map.NearestTamers(mob.Id);

                    if (!nearTamers.Any() && !mob.TamersViewing.Any())
                        continue;

                    if (!mob.Dead && mob.CurrentAction != MobActionEnum.Destroy)
                    {
                        nearTamers.ForEach(nearTamer =>
                        {
                            if (!mob.TamersViewing.Contains(nearTamer))
                            {
                                mob.TamersViewing.Add(nearTamer);

                                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == nearTamer);

                                targetClient?.Send(new LoadMobsPacket(mob));

                            }
                            else
                            {
                                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == nearTamer);

                                targetClient?.Send(new LoadMobsPacket(mob, true));

                            }
                        });
                    }

                    var farTamers = map.ConnectedTamers.Select(x => x.Id).Except(nearTamers).ToList();

                    farTamers.ForEach(farTamer =>
                    {
                        if (mob.TamersViewing.Contains(farTamer))
                        {
                            var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == farTamer);

                            mob.TamersViewing.Remove(farTamer);
                            targetClient?.Send(new UnloadMobsPacket(mob));
                        }
                    });
                }

                if (!mob.CanAct)
                    continue;

                MobsOperation(map, mob);

                mob.SetNextAction();
            }
            stopwatch.Stop();

            var totalTime = stopwatch.Elapsed.TotalMilliseconds;

            if (totalTime >= 1000)
                Console.WriteLine($"MonstersOperation ({map.Mobs.Count}): {totalTime}.");
        }

        private void MobsOperation(MapInstance map, MobConfigModel mob)
        {

            switch (mob.CurrentAction)
            {
                case MobActionEnum.CrowdControl:
                    {
                        var debuff = mob.DebuffList.ActiveBuffs.Where(buff =>
                                buff.BuffInfo.SkillInfo.Apply.Any(apply =>
                                    apply.Attribute == Commons.Enums.SkillCodeApplyAttributeEnum.CrowdControl
                                )
                            ).ToList();

                        if (debuff.Any())
                        {
                            CheckDebuff(map, mob, debuff);
                            break;
                        }
                    }
                    break;

                case MobActionEnum.Respawn:
                    {
                        mob.Reset();
                        mob.ResetLocation();
                    }
                    break;

                case MobActionEnum.Reward:
                    {
                        _verdandiXProgram.RemoveXProgramAfterOmegamonDefeat(map, mob, BroadcastForTamerViewsAndSelf);
                        ItemsReward(map, mob);
                        QuestKillReward(map, mob);
                        ExperienceReward(map, mob);
                        HandleDungeonStepProgression(map, mob.Id, mob.Type);

                        var dungeonClearHandled = TryHandleDungeonClear(map, mob.Type, mob.RaidDamage);
                        if (!dungeonClearHandled)
                        {
                            SourceKillSpawn(map, mob);
                            TargetKillSpawn(map, mob);
                        }

                        ColiseumStageClear(map, mob);

                        mob.UpdateCurrentAction(MobActionEnum.Destroy);
                    }
                    break;

                case MobActionEnum.Wait:
                    {
                        if (mob.Respawn && DateTime.Now > mob.DieTime.AddSeconds(2))
                        {

                            mob.SetNextWalkTime(UtilitiesFunctions.RandomInt(7, 14));
                            mob.SetAgressiveCheckTime(5);
                            mob.SetRespawn();
                        }
                        else
                        {
                            map.AttackNearbyTamer(mob, mob.TamersViewing, _assets.NpcColiseum);
                        }
                    }
                    break;

                case MobActionEnum.Walk:
                    {
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SyncConditionPacket(mob.GeneralHandler, ConditionEnum.Default).Serialize());
                        mob.Move();
                        map.BroadcastForTargetTamers(mob.TamersViewing, new MobWalkPacket(mob).Serialize());
                    }
                    break;

                case MobActionEnum.GiveUp:
                    {
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SyncConditionPacket(mob.GeneralHandler, ConditionEnum.Immortal).Serialize());
                        mob.ResetLocation();
                        map.BroadcastForTargetTamers(mob.TamersViewing, new MobRunPacket(mob).Serialize());
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SetCombatOffPacket(mob.GeneralHandler).Serialize());

                        foreach (var targetTamer in mob.TargetTamers)
                        {
                            if (targetTamer.TargetMobs.Count <= 1)
                            {
                                targetTamer.StopBattle();
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());
                            }
                        }

                        mob.Reset(true);
                        map.BroadcastForTargetTamers(mob.TamersViewing, new UpdateCurrentHPRatePacket(mob.GeneralHandler, mob.CurrentHpRate).Serialize());
                    }
                    break;

                case MobActionEnum.Attack:
                    {
                        var debuff = mob.DebuffList.ActiveBuffs.Where(buff =>
                                buff.BuffInfo.SkillInfo.Apply.Any(apply =>
                                    apply.Attribute == Commons.Enums.SkillCodeApplyAttributeEnum.CrowdControl
                                )
                            ).ToList();

                        if (debuff.Any())
                        {
                            CheckDebuff(map, mob, debuff);
                            break;
                        }
                        if (!mob.Dead && mob.SkillTime && !mob.CheckSkill && mob.IsPossibleSkill)
                        {
                            mob.UpdateCurrentAction(MobActionEnum.UseAttackSkill);
                            mob.SetNextAction();
                            break;
                        }

                        if (!mob.Dead && ((mob.TargetTamer == null || mob.TargetTamer.Hidden) || DateTime.Now > mob.LastHitTryTime.AddSeconds(15))) //Anti-kite
                        {
                            mob.GiveUp();
                            break;
                        }

                        if (!mob.Dead && !mob.Chasing && mob.TargetAlive)
                        {
                            var diff = UtilitiesFunctions.CalculateDistance(
                                mob.CurrentLocation.X,
                                mob.Target.Location.X,
                                mob.CurrentLocation.Y,
                                mob.Target.Location.Y);

                            var range = Math.Max(mob.ARValue, mob.Target.BaseInfo.ARValue);
                            if (diff <= range)
                            {
                                if (DateTime.Now < mob.LastHitTime.AddMilliseconds(mob.ASValue))
                                    break;

                                var missed = false;

                                if (mob.TargetTamer != null && mob.TargetTamer.GodMode)
                                    missed = true;
                                else if (mob.CanMissHit())
                                    missed = true;

                                if (missed)
                                {
                                    mob.UpdateLastHitTry();
                                    map.BroadcastForTargetTamers(mob.TamersViewing, new MissHitPacket(mob.GeneralHandler, mob.TargetHandler).Serialize());
                                    mob.UpdateLastHit();
                                    break;
                                }

                                var hitTamerId = mob.TargetTamer?.Id;
                                map.AttackTarget(mob, _assets.NpcColiseum);
                                TryApplyEquipmentSetWhenHit(map, hitTamerId);
                            }
                            else
                            {
                                map.ChaseTarget(mob);
                            }
                        }

                        if (mob.Dead)
                        {
                            foreach (var targetTamer in mob.TargetTamers)
                            {

                                targetTamer.StopBattle();
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());

                            }


                        }
                    }
                    break;

                case MobActionEnum.UseAttackSkill:
                    {
                        var debuff = mob.DebuffList.ActiveBuffs.Where(buff =>
                                buff.BuffInfo.SkillInfo.Apply.Any(apply =>
                                    apply.Attribute == Commons.Enums.SkillCodeApplyAttributeEnum.CrowdControl
                                )
                            ).ToList();

                        if (debuff.Any())
                        {
                            CheckDebuff(map, mob, debuff);
                            break;
                        }

                        if (!mob.Dead && ((mob.TargetTamer == null || mob.TargetTamer.Hidden))) //Anti-kite
                        {
                            mob.GiveUp();
                            break;
                        }

                        // ─── Step 6: cast window + per-skill cooldown ────────────
                        if (mob.IsCasting)
                            break;

                        if (mob.CastingComplete && mob.CastingSkillIndex.HasValue)
                        {
                            var castingId = mob.CastingSkillIndex.Value;
                            var castingSkill = _assets.MonsterSkillInfo.FirstOrDefault(s => s.Type == mob.Type && s.SkillId == castingId);
                            mob.FinishCast();
                            if (castingSkill != null && !mob.Dead && !mob.Chasing && mob.TargetAlive)
                            {
                                var hitTamerId = mob.TargetTamer?.Id;
                                map.SkillTarget(mob, castingSkill, _assets.NpcColiseum);
                                TryApplyEquipmentSetWhenHit(map, hitTamerId);
                                mob.MarkSkillCooldown(castingId, castingSkill.Cooldown);
                                mob.UpdateLastSkill();
                                mob.UpdateLastSkillTry();
                                mob.UpdateCheckSkill(false);
                                if (mob.Target != null)
                                {
                                    mob.UpdateCurrentAction(MobActionEnum.Wait);
                                    mob.SetNextAction();
                                }
                            }
                            break;
                        }

                        // Pick a skill off cooldown AND satisfying its s_nUse_Terms gate (Step 7).
                        var offCooldownSkills = _assets.MonsterSkillInfo
                            .Where(x => x.Type == mob.Type
                                     && !mob.IsSkillOnCooldown(x.SkillId))
                            .ToList();
                        var skillList = offCooldownSkills
                            .Where(x => MonsterSkillRotation.TermMatches(x, mob, mob.Target))
                            .ToList();

                        if (!skillList.Any())
                        {
                            mob.UpdateCheckSkill(false);
                            mob.UpdateCurrentAction(MobActionEnum.Wait);
                            mob.UpdateLastSkillTry();
                            mob.SetNextAction();
                            break;
                        }

                        Random random = new Random();
                        var targetSkill = skillList[random.Next(0, skillList.Count)];

                        if (!mob.Dead && !mob.Chasing && mob.TargetAlive)
                        {
                            var diff = UtilitiesFunctions.CalculateDistance(
                               mob.CurrentLocation.X,
                               mob.Target.Location.X,
                               mob.CurrentLocation.Y,
                               mob.Target.Location.Y);

                            if (diff <= 1900)
                            {
                                mob.StartCast(targetSkill.SkillId, targetSkill.CastingTime, moveLocked: false);
                            }
                            else
                            {
                                map.ChaseTarget(mob);
                            }
                        }

                        if (mob.Dead)
                        {
                            foreach (var targetTamer in mob.TargetTamers)
                            {

                                targetTamer.StopBattle();
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());

                            }

                            break;
                        }
                    }
                    break;

            }
        }

        private static void CheckDebuff(MapInstance map, MobConfigModel mob, List<MobDebuffModel> debuffs)
        {


            if (debuffs != null)
            {
                for (int i = 0; i < debuffs.Count; i++)
                {
                    var debuff = debuffs[i];

                    if (!debuff.Expired && mob.CurrentAction != MobActionEnum.CrowdControl)
                    {
                        mob.UpdateCurrentAction(MobActionEnum.CrowdControl);
                    }

                    if (debuff.Expired && mob.CurrentAction == MobActionEnum.CrowdControl)
                    {
                        debuffs.Remove(debuff);

                        if (debuffs.Count == 0)
                        {

                            map.BroadcastForTargetTamers(mob.TamersViewing, new RemoveBuffPacket(mob.GeneralHandler, debuff.BuffId, 1).Serialize());

                            mob.DebuffList.Buffs.Remove(debuff);

                            mob.UpdateCurrentAction(MobActionEnum.Wait);
                            mob.SetNextAction();

                        }
                        else
                        {
                            mob.DebuffList.Buffs.Remove(debuff);
                        }
                    }
                }

            }

        }

        private DungeonListRecord? ResolveDungeonRecord(MapInstance map)
        {
            return map.DungeonRecordId > 0
                ? _dungeonBins.Data.ListByDungeonId.GetValueOrDefault(map.DungeonRecordId)
                : _dungeonBins.Data.ResolveByRuntimeMapId(map.MapId, map.DungeonEntryPortalId);
        }

        private bool IsDungeonRaidBoss(MapInstance map, int mobType, int mobClass)
        {
            if (mobClass == 8)
                return true;

            var dungeonRecord = ResolveDungeonRecord(map);
            return dungeonRecord != null && _dungeonBins.Data.IsStepObjective(dungeonRecord.DungeonId, mobType);
        }

        private void HandleDungeonStepProgression(MapInstance map, long mobId, int mobType)
        {
            var dungeonRecord = ResolveDungeonRecord(map);
            if (dungeonRecord == null)
                return;

            var step = _dungeonBins.Data.GetObjectiveStep(dungeonRecord.DungeonId, mobType);
            if (step == null)
                return;

            if (!map.TryRegisterDungeonObjectiveKill(mobId, mobType))
                return;

            if (!IsDungeonStepComplete(map, step))
                return;

            if (!map.MarkDungeonStepCompleted(step.StepKey))
                return;

            var nextStep = _dungeonBins.Data.GetNextStep(dungeonRecord.DungeonId, step.StepKey);
            if (nextStep != null)
                UnlockDungeonStep(map, nextStep);

            _logger.Debug(
                "Dungeon step completed: dungeonId={DungeonId} map={MapId} step={StepKey} nextStep={NextStep}",
                dungeonRecord.DungeonId,
                map.MapId,
                step.StepKey,
                nextStep?.StepKey);
        }

        private static bool IsDungeonStepComplete(MapInstance map, DungeonStepGroup step)
        {
            foreach (var objective in step.Objectives)
            {
                var requiredAmount = Math.Max(1, objective.Amount);
                if (map.GetDungeonObjectiveKillCount(objective.TargetMonsterType) < requiredAmount)
                    return false;
            }

            return true;
        }

        private static void UnlockDungeonStep(MapInstance map, DungeonStepGroup step)
        {
            var objectiveTypes = step.Objectives
                .Select(x => x.TargetMonsterType)
                .ToHashSet();

            foreach (var mob in map.Mobs.Where(x => objectiveTypes.Contains(x.Type)))
                mob.SetAwaitingKillSpawn(false);

            foreach (var mob in map.SummonMobs.Where(x => objectiveTypes.Contains(x.Type)))
                mob.SetAwaitingKillSpawn(false);
        }

        private bool TryHandleDungeonClear(MapInstance map, int mobType, IReadOnlyDictionary<long, int>? raidDamage)
        {
            var dungeonRecord = ResolveDungeonRecord(map);

            if (dungeonRecord == null)
                return false;

            if (!_dungeonBins.Data.IsFinalObjective(dungeonRecord.DungeonId, mobType))
                return false;

            var finalStep = _dungeonBins.Data.GetObjectiveStep(dungeonRecord.DungeonId, mobType);
            if (finalStep != null && !_dungeonBins.Data.HasCompletedPriorSteps(dungeonRecord.DungeonId, finalStep.StepKey, map.DungeonCompletedStepKeys))
            {
                _logger.Warning(
                    "Dungeon final objective killed before prior steps were completed: dungeonId={DungeonId} map={MapId} mob={MobType} step={StepKey}",
                    dungeonRecord.DungeonId,
                    map.MapId,
                    mobType,
                    finalStep.StepKey);
                return true;
            }

            if (!map.TryMarkDungeonClear())
                return true;

            _dungeonBins.Data.ClearInfo.TryGetValue(dungeonRecord.DungeonId, out var clearInfo);
            _dungeonBins.Data.Rewards.TryGetValue(dungeonRecord.DungeonId, out var rewardInfo);

            var elapsedSeconds = map.DungeonClearElapsedSeconds;
            var failCount = Math.Max(0, map.DungeonFailCount);
            var partyCount = (ushort)Math.Max(1, map.Clients.Count);
            var partyMax = (ushort)Math.Max(partyCount, _dungeonBins.Data.GetMaxPlayers(dungeonRecord.DungeonId));
            var rank = _dungeonBins.Data.CalculateRank(dungeonRecord.DungeonId, elapsedSeconds, failCount);
            var failMax = (ushort)Math.Max(0, clearInfo?.FailLimit ?? 0);
            var defaultRewards = ToDungeonClearPacketRewards(rewardInfo, 0);
            var extraRewards = ToDungeonClearPacketRewards(rewardInfo, 1);
            var detailResults = BuildDungeonClearDetailResults(map, raidDamage);
            var dungeonName = _dungeonBins.Data.GetDisplayName(dungeonRecord);

            foreach (var client in map.Clients.ToList())
            {
                client.Send(new DungeonClearResultPacket(
                    dungeonRecord.DungeonId,
                    dungeonRecord.PortalId,
                    rank,
                    dungeonRecord.Difficulty,
                    elapsedSeconds,
                    partyCount,
                    partyMax,
                    (ushort)Math.Min(ushort.MaxValue, failCount),
                    failMax,
                    0,
                    0,
                    dungeonName,
                    defaultRewards,
                    extraRewards,
                    detailResults));
            }

            _logger.Information(
                "Dungeon clear result sent: dungeonId={DungeonId} name={DungeonName} map={MapId} mob={MobType} rank={Rank} elapsed={Elapsed}s limit={Limit}s fail={Fail}/{FailMax} clients={Clients}",
                dungeonRecord.DungeonId,
                dungeonName,
                map.MapId,
                mobType,
                rank,
                elapsedSeconds,
                clearInfo?.TimeLimitSeconds ?? 0,
                failCount,
                failMax,
                map.Clients.Count);

            return true;
        }

        private static IReadOnlyList<DungeonClearRewardPacketItem> ToDungeonClearPacketRewards(DungeonRewardRecord? rewardInfo, byte groupKey)
        {
            if (rewardInfo == null)
                return Array.Empty<DungeonClearRewardPacketItem>();

            return rewardInfo.Groups
                .Where(x => groupKey == 0 ? x.GroupKey == 0 : x.GroupKey != 0)
                .SelectMany(x => x.Items)
                .Select(x => new DungeonClearRewardPacketItem(x.ItemId, Math.Max(1, x.Amount)))
                .ToList();
        }

        private static IReadOnlyList<DungeonClearDetailPacketItem> BuildDungeonClearDetailResults(
            MapInstance map,
            IReadOnlyDictionary<long, int>? raidDamage)
        {
            var orderedDamage = (raidDamage ?? new Dictionary<long, int>())
                .Where(x => x.Key > 0 && x.Value > 0)
                .GroupBy(x => x.Key)
                .Select(x => new { TamerId = x.Key, Damage = x.Sum(y => (long)y.Value) })
                .OrderByDescending(x => x.Damage)
                .Take(3)
                .ToList();

            var detailResults = new List<DungeonClearDetailPacketItem>();
            byte category = 0;

            foreach (var entry in orderedDamage)
            {
                var client = map.Clients.FirstOrDefault(x => x.TamerId == entry.TamerId);
                var detail = CreateDungeonClearDetail(client, category, entry.Damage);
                if (detail != null)
                    detailResults.Add(detail);

                category++;
            }

            if (!detailResults.Any())
            {
                var firstClient = map.Clients.FirstOrDefault(x => x.Tamer != null && x.Partner != null);
                var detail = CreateDungeonClearDetail(firstClient, 0, 0);
                if (detail != null)
                    detailResults.Add(detail);
            }

            return detailResults;
        }

        private static DungeonClearDetailPacketItem? CreateDungeonClearDetail(GameClient? client, byte category, long value)
        {
            if (client?.Tamer == null || client.Partner == null)
                return null;

            return new DungeonClearDetailPacketItem(
                category,
                client.Tamer.Model.GetHashCode(),
                client.Partner.CurrentType,
                client.Tamer.Level,
                client.Partner.Level,
                (int)Math.Min(int.MaxValue, Math.Max(0, value)),
                client.Tamer.Name ?? string.Empty,
                client.Partner.Name ?? string.Empty);
        }

        private void ColiseumStageClear(MapInstance map, MobConfigModel mob)
        {
            if (map.ColiseumMobs.Contains((int)mob.Id))
            {
                map.ColiseumMobs.Remove((int)mob.Id);

                if (map.ColiseumMobs.Count == 1)
                {
                    var npcInfo = _assets.NpcColiseum.FirstOrDefault(x => x.NpcId == map.ColiseumMobs.First());

                    if (npcInfo != null)
                    {
                        foreach (var player in map.Clients.Where(x => x.Tamer.Partner.Alive))
                        {
                            player.Tamer.Points.IncreaseAmount(npcInfo.MobInfo[player.Tamer.Points.CurrentStage - 1].WinPoints);

                            _sender.Send(new UpdateCharacterArenaPointsCommand(player.Tamer.Points));

                            player?.Send(new DungeonArenaStageClearPacket(mob.Type, mob.TargetTamer.Points.CurrentStage, mob.TargetTamer.Points.Amount, npcInfo.MobInfo[mob.TargetTamer.Points.CurrentStage - 1].WinPoints, map.ColiseumMobs.First()));

                        }

                    }
                }
            }
        }

        private static void TargetKillSpawn(MapInstance map, MobConfigModel mob)
        {
            var targetKillSpawn = map.KillSpawns.FirstOrDefault(x => x.TargetMobs.Any(x => x.TargetMobType == mob.Type));

            if (targetKillSpawn != null)
            {
                mob.SetAwaitingKillSpawn();

                foreach (var targetMob in targetKillSpawn.TargetMobs.Where(x => x.TargetMobType == mob.Type).ToList())
                {
                    if (!map.Mobs.Exists(x => x.Type == targetMob.TargetMobType && !x.AwaitingKillSpawn))
                    {
                        targetKillSpawn.DecreaseTempMobs(targetMob);
                        targetKillSpawn.ResetCurrentSourceMobAmount();

                        map.BroadcastForMap(new KillSpawnEndChatNotifyPacket(targetMob.TargetMobType).Serialize());
                    }
                }
            }
        }

        private static void SourceKillSpawn(MapInstance map, MobConfigModel mob)
        {
            var sourceMobKillSpawn = map.KillSpawns.FirstOrDefault(ks => ks.SourceMobs.Any(sm => sm.SourceMobType == mob.Type));

            if (sourceMobKillSpawn == null)
                return;

            var sourceKillSpawn = sourceMobKillSpawn.SourceMobs.FirstOrDefault(x => x.SourceMobType == mob.Type);

            if (sourceKillSpawn != null && sourceKillSpawn.CurrentSourceMobRequiredAmount > 0)
            {
                sourceKillSpawn.DecreaseCurrentSourceMobAmount();

                if (sourceMobKillSpawn.ShowOnMinimap && sourceKillSpawn.CurrentSourceMobRequiredAmount <= 10)
                {

                    map.BroadcastForMap(new KillSpawnMinimapNotifyPacket(sourceKillSpawn.SourceMobType, sourceKillSpawn.CurrentSourceMobRequiredAmount).Serialize());

                }

                if (sourceMobKillSpawn.Spawn())
                {
                    foreach (var targetMob in sourceMobKillSpawn.TargetMobs)
                    {
                        //TODO: para todos os canais (apenas do mapa)
                        map.BroadcastForMap(new KillSpawnChatNotifyPacket(map.MapId, map.Channel, targetMob.TargetMobType).Serialize());

                        map.Mobs.Where(x => x.Type == targetMob.TargetMobType)?.ToList().ForEach(targetMob =>
                        {
                            targetMob.SetRespawn(true);
                            targetMob.SetAwaitingKillSpawn(false);
                        });
                    }
                }
            }
        }

        private void QuestKillReward(MapInstance map, MobConfigModel mob)
        {
            var partyIdList = new List<int>();

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;

                var giveUpList = new List<short>();

                foreach (var questInProgress in tamer.Progress.InProgressQuestData)
                {
                    var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                    if (questInfo != null)
                    {
                        if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.KillMonster))
                            continue;

                        var goalIndex = -1;
                        foreach (var questGoal in questInfo.QuestGoals)
                        {
                            if (questGoal.GoalId == mob?.Type)
                            {
                                goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                break;
                            }
                        }

                        if (goalIndex != -1)
                        {
                            var currentGoalValue = tamer.Progress.GetQuestGoalProgress(questInProgress.QuestId, goalIndex);
                            if (currentGoalValue < questInfo.QuestGoals[goalIndex].GoalAmount)
                            {
                                currentGoalValue++;
                                tamer.Progress.UpdateQuestInProgress(questInProgress.QuestId, goalIndex, currentGoalValue);

                                targetClient.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, currentGoalValue));
                                var questToUpdate = targetClient.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                                _sender.Send(new UpdateCharacterInProgressCommand(questToUpdate));
                            }
                        }
                    }
                    else
                    {
                        _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                        targetClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                        giveUpList.Add(questInProgress.QuestId);
                    }
                }

                giveUpList.ForEach(giveUp =>
                {
                    tamer.Progress.RemoveQuest(giveUp);
                });

                var party = _partyManager.FindParty(targetClient.TamerId);
                if (party != null && !partyIdList.Contains(party.Id))
                {
                    partyIdList.Add(party.Id);

                    foreach (var partyMemberId in party.Members.Values.Select(x => x.Id))
                    {
                        var partyMemberClient = map.Clients.FirstOrDefault(x => x.TamerId == partyMemberId);
                        if (partyMemberClient == null || partyMemberId == targetClient.TamerId)
                            continue;

                        giveUpList = new List<short>();

                        foreach (var questInProgress in partyMemberClient.Tamer.Progress.InProgressQuestData)
                        {
                            var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                            if (questInfo != null)
                            {
                                if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.KillMonster))
                                    continue;

                                var goalIndex = -1;
                                foreach (var questGoal in questInfo.QuestGoals)
                                {
                                    if (questGoal.GoalId == mob?.Type)
                                    {
                                        goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                        break;
                                    }
                                }

                                if (goalIndex != -1)
                                {
                                    var currentGoalValue = partyMemberClient.Tamer.Progress.GetQuestGoalProgress(questInProgress.QuestId, goalIndex);
                                    if (currentGoalValue < questInfo.QuestGoals[goalIndex].GoalAmount)
                                    {
                                        currentGoalValue++;
                                        partyMemberClient.Tamer.Progress.UpdateQuestInProgress(questInProgress.QuestId, goalIndex, currentGoalValue);

                                        partyMemberClient.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, currentGoalValue));
                                        var questToUpdate = partyMemberClient.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                                        _sender.Send(new UpdateCharacterInProgressCommand(questToUpdate));
                                    }
                                }
                            }
                            else
                            {
                                _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                                partyMemberClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                                giveUpList.Add(questInProgress.QuestId);
                            }
                        }

                        giveUpList.ForEach(giveUp =>
                        {
                            partyMemberClient.Tamer.Progress.RemoveQuest(giveUp);
                        });
                    }
                }
            }

            partyIdList.Clear();
        }

        private void ItemsReward(MapInstance map, MobConfigModel mob)
        {
            var isRaidBoss = IsDungeonRaidBoss(map, mob.Type, mob.Class);

            if (mob.DropReward != null)
                QuestDropReward(map, mob);

            if (isRaidBoss)
                RaidReward(map, mob);
            else if (mob.DropReward != null)
                DropReward(map, mob);
        }

        private void ExperienceReward(MapInstance map, MobConfigModel mob)
        {
            if (mob.ExpReward == null)
                return;

            var partyIdList = new List<int>();

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;
                double expBonusMultiplier = CalculateExperienceMultiplier(tamer.BonusEXP, targetClient.ServerExperience);

                var basePartnerExperience = CalculateExperience(tamer.Partner.Level, mob.Level, mob.ExpReward.DigimonExperience);
                var partnerExpToReceive = (long)(basePartnerExperience * expBonusMultiplier); //TODO: +bonus

                if (basePartnerExperience == 0)
                    partnerExpToReceive = 0;

                if (partnerExpToReceive > 100) partnerExpToReceive += UtilitiesFunctions.RandomInt(-15, 15);
                var tamerExpToReceive = ExperienceRewardCalculator.CalculateTamerExperienceFromKilledDigimon(
                    tamer.Level,
                    mob.Level,
                    mob.ExpReward.DigimonExperience);

                var fatigueExp = _fatigueService.GetMultipliers(targetClient).exp;   // FATIGUE_HOOK
                var tamerResult = ReceiveTamerExp(targetClient.Tamer, tamerExpToReceive, fatigueExp);
                var partnerResult = ReceivePartnerExp(targetClient.Partner, mob, partnerExpToReceive, fatigueExp);   // FATIGUE_HOOK

                targetClient.Send(
                    new ReceiveExpPacket(
                        tamerExpToReceive,
                        0,//TODO: obter os bonus
                        targetClient.Tamer.CurrentExperience,
                        targetClient.Partner.GeneralHandler,
                        partnerExpToReceive,
                        0,//TODO: obter os bonus
                        targetClient.Partner.CurrentExperience,
                        targetClient.Partner.CurrentEvolution.SkillExperience
                    )
                );

                //TODO: importar o DMBase e tratar isso
                SkillExpReward(map, targetClient);

                if (tamerResult.LevelGain > 0 || partnerResult.LevelGain > 0)
                {
                    targetClient.Send(new UpdateStatusPacket(targetClient.Tamer));

                    map.BroadcastForTamerViewsAndSelf(targetClient.TamerId,
                        new UpdateMovementSpeedPacket(targetClient.Tamer).Serialize());
                }

                _sender.Send(new UpdateCharacterExperienceCommand(tamer));
                _sender.Send(new UpdateDigimonExperienceCommand(tamer.Partner));

                PartyExperienceReward(map, mob, partyIdList, targetClient, ref tamerExpToReceive, ref tamerResult, ref partnerExpToReceive, ref partnerResult);
            }

            partyIdList.Clear();
        }

        private static double CalculateExperienceMultiplier(int bonusExperience, int serverExperience)
        {
            var serverMultiplier = serverExperience > 0 ? serverExperience / 100.0 : 1.0;
            var bonusMultiplier = bonusExperience > 0 ? bonusExperience / 100.0 : 0.0;

            return Math.Max(0.0, serverMultiplier + bonusMultiplier);
        }

        public long CalculateExperience(int tamerLevel, int mobLevel, long baseExperience)
        {
            return ExperienceRewardCalculator.CalculateKillExperience(tamerLevel, mobLevel, baseExperience);
        }


        private void SkillExpReward(MapInstance map, GameClient? targetClient)
        {


            var ExpNeed = int.MaxValue;
            var evolutionType = _assets.DigimonBaseInfo.First(x => x.Type == targetClient.Partner.CurrentEvolution.Type).EvolutionType;


            ExpNeed = SkillExperienceTable(evolutionType, targetClient.Partner.CurrentEvolution.SkillMastery);

            if (targetClient.Partner.CurrentEvolution.SkillExperience >= ExpNeed)
            {
                targetClient.Partner.ReceiveSkillPoint();

                var evolutionIndex = targetClient.Partner.Evolutions.IndexOf(targetClient.Partner.CurrentEvolution);

                //TODO: Receive skill point packet
                var packet = new PacketWriter();
                packet.Type(1105);
                packet.WriteInt(targetClient.Partner.GeneralHandler);
                packet.WriteByte((byte)(evolutionIndex + 1));
                packet.WriteByte(targetClient.Partner.CurrentEvolution.SkillPoints);
                packet.WriteByte(targetClient.Partner.CurrentEvolution.SkillMastery);
                packet.WriteInt(targetClient.Partner.CurrentEvolution.SkillExperience);

                map.BroadcastForTamerViewsAndSelf(targetClient.TamerId, packet.Serialize());
            }
        }

        private void PartyExperienceReward(
            MapInstance map,
            MobConfigModel mob,
            List<int> partyIdList,
            GameClient? targetClient,
            ref long tamerExpToReceive,
            ref ReceiveExpResult tamerResult,
            ref long partnerExpToReceive,
            ref ReceiveExpResult partnerResult)
        {
            var party = _partyManager.FindParty(targetClient.TamerId);
            if (party != null && !partyIdList.Contains(party.Id))
            {
                partyIdList.Add(party.Id);

                foreach (var partyMemberId in party.Members.Values.Select(x => x.Id))
                {
                    var partyMemberClient = map.Clients.FirstOrDefault(x => x.TamerId == partyMemberId);
                    if (partyMemberClient == null || partyMemberId == targetClient.TamerId)
                        continue;

                    var fatiguePartyExp = _fatigueService.GetMultipliers(partyMemberClient).exp;   // FATIGUE_HOOK
                    const double partyExperienceShare = 0.80;
                    partnerExpToReceive = (long)((double)(mob.ExpReward.DigimonExperience) * partyExperienceShare); //TODO: +bonus
                    if (partnerExpToReceive > 100) partnerExpToReceive += UtilitiesFunctions.RandomInt(-15, 15);
                    tamerExpToReceive = ExperienceRewardCalculator.CalculateTamerExperienceFromKilledDigimon(
                        partyMemberClient.Tamer.Level,
                        mob.Level,
                        mob.ExpReward.DigimonExperience,
                        partyExperienceShare);
                    tamerResult = ReceiveTamerExp(partyMemberClient.Tamer, tamerExpToReceive, fatiguePartyExp);
                    partnerResult = ReceivePartnerExp(partyMemberClient.Partner, mob, partnerExpToReceive, fatiguePartyExp);   // FATIGUE_HOOK

                    partyMemberClient.Send(
                        new PartyReceiveExpPacket(
                            tamerExpToReceive,
                            0,//TODO: obter os bonus
                            partyMemberClient.Tamer.CurrentExperience,
                            partyMemberClient.Partner.GeneralHandler,
                            partnerExpToReceive,
                            0,//TODO: obter os bonus
                            partyMemberClient.Partner.CurrentExperience,
                            partyMemberClient.Partner.CurrentEvolution.SkillExperience,
                            targetClient.Tamer.Name
                        ));

                    if (tamerResult.LevelGain > 0 || partnerResult.LevelGain > 0)
                    {
                        targetClient.Send(new UpdateStatusPacket(targetClient.Tamer));

                        map.BroadcastForTamerViewsAndSelf(targetClient.TamerId,
                            new UpdateMovementSpeedPacket(targetClient.Tamer).Serialize());
                    }

                    _sender.Send(new UpdateCharacterExperienceCommand(partyMemberClient.Tamer));
                    _sender.Send(new UpdateDigimonExperienceCommand(partyMemberClient.Partner));
                }
            }
        }

        private void PartyExperienceReward(
          MapInstance map,
          SummonMobModel mob,
          List<int> partyIdList,
          GameClient? targetClient,
          ref long tamerExpToReceive,
          ref ReceiveExpResult tamerResult,
          ref long partnerExpToReceive,
          ref ReceiveExpResult partnerResult)
        {
            var party = _partyManager.FindParty(targetClient.TamerId);
            if (party != null && !partyIdList.Contains(party.Id))
            {
                partyIdList.Add(party.Id);

                foreach (var partyMemberId in party.Members.Values.Select(x => x.Id))
                {
                    var partyMemberClient = map.Clients.FirstOrDefault(x => x.TamerId == partyMemberId);
                    if (partyMemberClient == null || partyMemberId == targetClient.TamerId)
                        continue;

                    var fatiguePartyExp = _fatigueService.GetMultipliers(partyMemberClient).exp;   // FATIGUE_HOOK
                    const double partyExperienceShare = 0.80;
                    partnerExpToReceive = (long)((double)(mob.ExpReward.DigimonExperience) * partyExperienceShare); //TODO: +bonus
                    if (partnerExpToReceive > 100) partnerExpToReceive += UtilitiesFunctions.RandomInt(-15, 15);
                    tamerExpToReceive = ExperienceRewardCalculator.CalculateTamerExperienceFromKilledDigimon(
                        partyMemberClient.Tamer.Level,
                        mob.Level,
                        mob.ExpReward.DigimonExperience,
                        partyExperienceShare);
                    tamerResult = ReceiveTamerExp(partyMemberClient.Tamer, tamerExpToReceive, fatiguePartyExp);
                    partnerResult = ReceivePartnerExp(partyMemberClient.Partner, mob, partnerExpToReceive, fatiguePartyExp);   // FATIGUE_HOOK

                    partyMemberClient.Send(
                        new PartyReceiveExpPacket(
                            tamerExpToReceive,
                            0,//TODO: obter os bonus
                            partyMemberClient.Tamer.CurrentExperience,
                            partyMemberClient.Partner.GeneralHandler,
                            partnerExpToReceive,
                            0,//TODO: obter os bonus
                            partyMemberClient.Partner.CurrentExperience,
                            partyMemberClient.Partner.CurrentEvolution.SkillExperience,
                            targetClient.Tamer.Name
                        ));

                    if (tamerResult.LevelGain > 0 || partnerResult.LevelGain > 0)
                    {
                        targetClient.Send(new UpdateStatusPacket(targetClient.Tamer));

                        map.BroadcastForTamerViewsAndSelf(targetClient.TamerId,
                            new UpdateMovementSpeedPacket(targetClient.Tamer).Serialize());
                    }

                    _sender.Send(new UpdateCharacterExperienceCommand(partyMemberClient.Tamer));
                    _sender.Send(new UpdateDigimonExperienceCommand(partyMemberClient.Partner));
                }
            }
        }
        private void DropReward(MapInstance map, MobConfigModel mob)
        {
            var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == mob.TargetTamer?.Id);
            if (targetClient == null)
                return;

            BitDropReward(map, mob, targetClient);

            ItemDropReward(map, mob, targetClient);
        }

        private void BitDropReward(MapInstance map, MobConfigModel mob, GameClient? targetClient)
        {
            // FATIGUE_HOOK
            var fatigueDrop = (double)_fatigueService.GetMultipliers(targetClient).drop;
            if (fatigueDrop <= 0) return;

            var bitsReward = mob.DropReward.BitsDrop;

            if (bitsReward != null && bitsReward.Chance * fatigueDrop >= UtilitiesFunctions.RandomDouble())
            {
                if (targetClient.Tamer.HasAura && targetClient.Tamer.Aura.ItemInfo.Section == 2100)
                {
                    var amount = UtilitiesFunctions.RandomInt(bitsReward.MinAmount, bitsReward.MaxAmount);

                    targetClient.Send(
                        new PickBitsPacket(
                            targetClient.Tamer.GeneralHandler,
                            amount
                        )
                    );

                    targetClient.Tamer.Inventory.AddBits(amount);

                    _sender.Send(new UpdateItemsCommand(targetClient.Tamer.Inventory)).GetAwaiter().GetResult();
                    _logger.Verbose($"Character {targetClient.TamerId} aquired {amount} bits from mob {mob.Id} with magnetic aura {targetClient.Tamer.Aura.ItemId}.");
                }
                else
                {
                    var drop = _dropManager.CreateBitDrop(
                        targetClient.TamerId,
                        targetClient.Tamer.GeneralHandler,
                        bitsReward.MinAmount,
                        bitsReward.MaxAmount,
                        mob.CurrentLocation.MapId,
                        mob.CurrentLocation.X,
                        mob.CurrentLocation.Y
                    );

                    map.AddMapDrop(drop);
                }
            }
        }

        private void ItemDropReward(MapInstance map, MobConfigModel mob, GameClient? targetClient)
        {
            if (!mob.DropReward.Drops.Any())
                return;

            // FATIGUE_HOOK
            var fatigueDrop = (double)_fatigueService.GetMultipliers(targetClient).drop;
            if (fatigueDrop <= 0) return;

            var itemsReward = new List<ItemDropConfigModel>();
            itemsReward.AddRange(mob.DropReward.Drops);
            itemsReward.RemoveAll(x => _assets.QuestItemList.Contains(x.ItemId));

            if (!itemsReward.Any())
                return;

            var dropped = 0;
            var totalDrops = UtilitiesFunctions.RandomInt(
                mob.DropReward.MinAmount,
                mob.DropReward.MaxAmount);

            while (dropped < totalDrops)
            {
                if (!itemsReward.Any())
                {
                    _logger.Warning($"Mob {mob.Id} has incorrect drops configuration.");
                    break;
                }

                var possibleDrops = itemsReward.OrderBy(x => Guid.NewGuid()).ToList();
                foreach (var itemDrop in possibleDrops)
                {
                    if (itemDrop.Chance * fatigueDrop >= UtilitiesFunctions.RandomDouble())   // FATIGUE_HOOK
                    {
                        if (targetClient.Tamer.HasAura && targetClient.Tamer.Aura.ItemInfo.Section == 2100)
                        {
                            var newItem = new ItemModel();
                            newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemDrop.ItemId));

                            if (newItem.ItemInfo == null)
                            {
                                _logger.Warning($"No item info found with ID {itemDrop.ItemId} for tamer {targetClient.Tamer.Id}.");
                                targetClient.Send(new SystemMessagePacket($"No item info found with ID {itemDrop.ItemId}."));
                                continue;
                            }

                            newItem.ItemId = itemDrop.ItemId;
                            newItem.Amount = UtilitiesFunctions.RandomInt(itemDrop.MinAmount, itemDrop.MaxAmount);

                            var itemClone = (ItemModel)newItem.Clone();
                            if (targetClient.Tamer.Inventory.AddItem(newItem))
                            {
                                targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                _sender.Send(new UpdateItemsCommand(targetClient.Tamer.Inventory)).GetAwaiter().GetResult();
                                _logger.Verbose($"Character {targetClient.TamerId} aquired {newItem.ItemId} x{newItem.Amount} from " +
                                    $"mob {mob.Id} with magnetic aura {targetClient.Tamer.Aura.ItemId}.");
                            }
                            else
                            {
                                targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));

                                var drop = _dropManager.CreateItemDrop(
                                    targetClient.Tamer.Id,
                                    targetClient.Tamer.GeneralHandler,
                                    itemDrop.ItemId,
                                    itemDrop.MinAmount,
                                    itemDrop.MaxAmount,
                                    mob.CurrentLocation.MapId,
                                    mob.CurrentLocation.X,
                                    mob.CurrentLocation.Y
                                );

                                map.AddMapDrop(drop);
                            }

                            dropped++;
                        }
                        else
                        {
                            var drop = _dropManager.CreateItemDrop(
                                targetClient.Tamer.Id,
                                targetClient.Tamer.GeneralHandler,
                                itemDrop.ItemId,
                                itemDrop.MinAmount,
                                itemDrop.MaxAmount,
                                mob.CurrentLocation.MapId,
                                mob.CurrentLocation.X,
                                mob.CurrentLocation.Y
                            );

                            dropped++;

                            map.AddMapDrop(drop);
                        }

                        itemsReward.RemoveAll(x => x.Id == itemDrop.Id);
                        break;
                    }
                }
            }
        }

        private Dictionary<int, List<QuestLootDropRule>> QuestLootDropRulesByMob()
        {
            if (_questLootDropRulesByMob != null)
                return _questLootDropRulesByMob;

            var rulesByMob = new Dictionary<int, List<QuestLootDropRule>>();
            var ruleCount = 0;

            foreach (var questDrop in _assets.QuestLootItemDropsByMob)
            {
                if (!rulesByMob.TryGetValue(questDrop.Key, out var rules))
                {
                    rules = new List<QuestLootDropRule>();
                    rulesByMob.Add(questDrop.Key, rules);
                }

                foreach (var itemId in questDrop.Value)
                {
                    if (rules.Any(x => x.ItemId == itemId))
                        continue;

                    rules.Add(new QuestLootDropRule(itemId));
                    ruleCount++;
                }
            }

            _questLootDropRulesByMob = rulesByMob;
            _logger.Information(
                "QuestLootDropIndex attached mobs={MobCount} rules={RuleCount}",
                rulesByMob.Count,
                ruleCount);

            return _questLootDropRulesByMob;
        }

        private List<ItemDropConfigModel> BuildQuestItemRewards(MobConfigModel mob)
        {
            var itemsReward = new List<ItemDropConfigModel>();
            var questDropChance = QuestItemDropChanceByMonsterLevel(mob.Level);

            if (QuestLootDropRulesByMob().TryGetValue(mob.Type, out var runtimeRules))
            {
                foreach (var rule in runtimeRules)
                {
                    var runtimeDrop = new ItemDropConfigModel();
                    runtimeDrop.SetItemId(rule.ItemId);
                    runtimeDrop.SetMinAmount(1);
                    runtimeDrop.SetMaxAmount(1);
                    runtimeDrop.SetChance(questDropChance);
                    itemsReward.Add(runtimeDrop);
                }
            }

            if (mob.DropReward?.Drops != null)
            {
                foreach (var configuredDrop in mob.DropReward.Drops.Where(x => _assets.QuestItemList.Contains(x.ItemId)))
                {
                    if (itemsReward.Any(x => x.ItemId == configuredDrop.ItemId))
                        continue;

                    itemsReward.Add(configuredDrop);
                }
            }

            return itemsReward;
        }

        private List<SummonMobItemDropModel> BuildQuestItemRewards(SummonMobModel mob)
        {
            var itemsReward = new List<SummonMobItemDropModel>();
            var questDropChance = QuestItemDropChanceByMonsterLevel(mob.Level);

            if (QuestLootDropRulesByMob().TryGetValue(mob.Type, out var runtimeRules))
            {
                foreach (var rule in runtimeRules)
                {
                    itemsReward.Add(new SummonMobItemDropModel
                    {
                        ItemId = rule.ItemId,
                        MinAmount = 1,
                        MaxAmount = 1,
                        Chance = questDropChance
                    });
                }
            }

            if (mob.DropReward?.Drops != null)
            {
                foreach (var configuredDrop in mob.DropReward.Drops.Where(x => _assets.QuestItemList.Contains(x.ItemId)))
                {
                    if (itemsReward.Any(x => x.ItemId == configuredDrop.ItemId))
                        continue;

                    itemsReward.Add(configuredDrop);
                }
            }

            return itemsReward;
        }

        private static double QuestItemDropChanceByMonsterLevel(int monsterLevel)
        {
            const double levelOneChance = 70.0;
            const double levelOneHundredFiftyChance = 35.0;
            const double maxRuleLevel = 150.0;

            var clampedLevel = Math.Clamp(monsterLevel, 1, (int)maxRuleLevel);
            var progress = (clampedLevel - 1) / (maxRuleLevel - 1);
            return levelOneChance - ((levelOneChance - levelOneHundredFiftyChance) * progress);
        }

        private void QuestDropReward(MapInstance map, MobConfigModel mob)
        {
            var itemsReward = BuildQuestItemRewards(mob);

            if (!itemsReward.Any())
                return;

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;

                if (!tamer.Progress.InProgressQuestData.Any())
                    continue;

                var updateItemList = false;
                var possibleDrops = itemsReward.Randomize();
                foreach (var itemDrop in possibleDrops)
                {
                    if (itemDrop.Chance >= UtilitiesFunctions.RandomDouble())
                    {
                        foreach (var questInProgress in tamer.Progress.InProgressQuestData)
                        {
                            var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                            if (questInfo != null)
                            {
                                if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.LootItem))
                                    continue;

                                var goalIndex = -1;
                                foreach (var questGoal in questInfo.QuestGoals)
                                {
                                    if (questGoal.GoalId == itemDrop?.ItemId)
                                    {
                                        var inventoryItems = tamer.Inventory.FindItemsById(questGoal.GoalId);
                                        var goalAmount = questGoal.GoalAmount;

                                        foreach (var inventoryItem in inventoryItems)
                                        {
                                            goalAmount -= inventoryItem.Amount;
                                            if (goalAmount <= 0)
                                            {
                                                goalAmount = 0;
                                                break;
                                            }
                                        }

                                        if (goalAmount > 0)
                                        {
                                            goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                            break;
                                        }
                                    }
                                }

                                if (goalIndex != -1)
                                {
                                    var newItem = new ItemModel();
                                    newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemDrop.ItemId));

                                    if (newItem.ItemInfo == null)
                                    {
                                        _logger.Warning($"No item info found with ID {itemDrop.ItemId} for tamer {tamer.Id}.");
                                        targetClient.Send(new SystemMessagePacket($"No item info found with ID {itemDrop.ItemId}."));
                                        continue;
                                    }

                                    newItem.ItemId = itemDrop.ItemId;
                                    newItem.Amount = UtilitiesFunctions.RandomInt(itemDrop.MinAmount, itemDrop.MaxAmount);

                                    var itemClone = (ItemModel)newItem.Clone();
                                    if (tamer.Inventory.AddItem(newItem))
                                    {
                                        updateItemList = true;
                                        targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                    }
                                    else
                                    {
                                        targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                                    }
                                }
                            }
                            else
                            {
                                _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                                targetClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                            }
                        }

                        if (updateItemList) _sender.Send(new UpdateItemsCommand(tamer.Inventory)).GetAwaiter().GetResult();

                        itemsReward.RemoveAll(x => x.Id == itemDrop.Id);
                    }
                }
            }
        }

        private void RaidReward(MapInstance map, MobConfigModel mob)
        {
            var raidResult = mob.RaidDamage
                .Where(x => x.Key > 0)
                .DistinctBy(x => x.Key)
                .OrderByDescending(x => x.Value)
                .ToList();

            var writer = new PacketWriter();
            writer.Type(1604);
            writer.WriteInt(Math.Min(10, raidResult.Count));

            int i = 1;

            var updateItemList = new List<ItemListModel>();

            foreach (var raidTamer in raidResult)
            {
                _logger.Verbose($"Character {raidTamer.Key} rank {i} on raid {mob.Id} - {mob.Name} with damage {raidTamer.Value}.");

                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == raidTamer.Key);

                if (i <= 10)
                {
                    writer.WriteInt(i);
                    writer.WriteString(targetClient?.Tamer?.Name ?? $"Tamer{i}");
                    writer.WriteString(targetClient?.Partner?.Name ?? $"Partner{i}");
                    writer.WriteInt(raidTamer.Value);
                }

                var bitsReward = mob.DropReward?.BitsDrop;
                if (targetClient != null && bitsReward != null && bitsReward.Chance >= UtilitiesFunctions.RandomDouble())
                {
                    var drop = _dropManager.CreateBitDrop(
                        targetClient.Tamer.Id,
                        targetClient.Tamer.GeneralHandler,
                        bitsReward.MinAmount,
                        bitsReward.MaxAmount,
                        mob.CurrentLocation.MapId,
                        mob.CurrentLocation.X,
                        mob.CurrentLocation.Y
                    );

                    map.DropsToAdd.Add(drop);
                }

                var raidRewards = mob.DropReward?.Drops?
                    .Where(x => !_assets.QuestItemList.Contains(x.ItemId))
                    .ToList() ?? new List<ItemDropConfigModel>();

                if (targetClient != null && raidRewards != null && raidRewards.Any())
                {
                    var rewards = raidRewards.Where(x => x.Rank == i);

                    if (rewards == null || !rewards.Any())
                        rewards = raidRewards.Where(x => x.Rank == raidRewards.Max(x => x.Rank));

                    foreach (var reward in rewards)
                    {
                        if (reward.Chance >= UtilitiesFunctions.RandomDouble())
                        {
                            var newItem = new ItemModel();
                            newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == reward.ItemId));

                            if (newItem.ItemInfo == null)
                            {
                                _logger.Warning($"No item info found with ID {reward.ItemId} for tamer {targetClient.TamerId}.");
                                targetClient.Send(new SystemMessagePacket($"No item info found with ID {reward.ItemId}."));
                                break;
                            }

                            newItem.ItemId = reward.ItemId;
                            newItem.Amount = UtilitiesFunctions.RandomInt(reward.MinAmount, reward.MaxAmount);

                            if (newItem.IsTemporary)
                                newItem.SetRemainingTime((uint)newItem.ItemInfo.UsageTimeMinutes);

                            var itemClone = (ItemModel)newItem.Clone();
                            if (targetClient.Tamer.Inventory.AddItem(newItem))
                            {
                                targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                updateItemList.Add(targetClient.Tamer.Inventory);
                            }
                            else
                            {
                                targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                                targetClient.Tamer.GiftWarehouse.AddItem(newItem);
                                updateItemList.Add(targetClient.Tamer.GiftWarehouse);
                            }
                        }
                    }
                }

                i++;
            }

            map.BroadcastForTargetTamers(mob.RaidDamage.Select(x => x.Key).ToList(), writer.Serialize());
            updateItemList.ForEach(itemList => { _sender.Send(new UpdateItemsCommand(itemList)).GetAwaiter().GetResult(); });
        }
        private void RaidReward(MapInstance map, SummonMobModel mob)
        {
            var raidResult = mob.RaidDamage
                .Where(x => x.Key > 0)
                .DistinctBy(x => x.Key)
                .OrderByDescending(x => x.Value)
                .ToList();

            var writer = new PacketWriter();
            writer.Type(1604);
            writer.WriteInt(Math.Min(10, raidResult.Count));

            int i = 1;

            var updateItemList = new List<ItemListModel>();

            foreach (var raidTamer in raidResult)
            {
                _logger.Verbose($"Character {raidTamer.Key} rank {i} on raid {mob.Id} - {mob.Name} with damage {raidTamer.Value}.");

                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == raidTamer.Key);

                if (i <= 10)
                {
                    writer.WriteInt(i);
                    writer.WriteString(targetClient?.Tamer?.Name ?? $"Tamer{i}");
                    writer.WriteString(targetClient?.Partner?.Name ?? $"Partner{i}");
                    writer.WriteInt(raidTamer.Value);
                }

                var bitsReward = mob.DropReward?.BitsDrop;
                if (targetClient != null && bitsReward != null && bitsReward.Chance >= UtilitiesFunctions.RandomDouble())
                {
                    var drop = _dropManager.CreateBitDrop(
                        targetClient.Tamer.Id,
                        targetClient.Tamer.GeneralHandler,
                        bitsReward.MinAmount,
                        bitsReward.MaxAmount,
                        mob.CurrentLocation.MapId,
                        mob.CurrentLocation.X,
                        mob.CurrentLocation.Y
                    );

                    map.DropsToAdd.Add(drop);
                }

                var raidRewards = mob.DropReward?.Drops?
                    .Where(x => !_assets.QuestItemList.Contains(x.ItemId))
                    .ToList() ?? new List<SummonMobItemDropModel>();

                if (targetClient != null && raidRewards != null && raidRewards.Any())
                {
                    foreach (var reward in raidRewards)
                    {
                        if (reward.Chance >= UtilitiesFunctions.RandomDouble())
                        {
                            var newItem = new ItemModel();
                            newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == reward.ItemId));

                            if (newItem.ItemInfo == null)
                            {
                                _logger.Warning($"No item info found with ID {reward.ItemId} for tamer {targetClient.TamerId}.");
                                targetClient.Send(new SystemMessagePacket($"No item info found with ID {reward.ItemId}."));
                                continue; // Continue para a próxima recompensa se não houver informações sobre o item.
                            }

                            newItem.ItemId = reward.ItemId;
                            newItem.Amount = UtilitiesFunctions.RandomInt(reward.MinAmount, reward.MaxAmount);

                            if (newItem.IsTemporary)
                                newItem.SetRemainingTime((uint)newItem.ItemInfo.UsageTimeMinutes);

                            var itemClone = (ItemModel)newItem.Clone();
                            if (targetClient.Tamer.Inventory.AddItem(newItem))
                            {
                                targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                updateItemList.Add(targetClient.Tamer.Inventory);
                            }
                            else
                            {
                                targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                                targetClient.Tamer.GiftWarehouse.AddItem(newItem);
                                updateItemList.Add(targetClient.Tamer.GiftWarehouse);
                            }
                        }
                    }
                }


                i++;
            }

            map.BroadcastForTargetTamers(mob.RaidDamage.Select(x => x.Key).ToList(), writer.Serialize());
            updateItemList.ForEach(itemList => { _sender.Send(new UpdateItemsCommand(itemList)).GetAwaiter().GetResult(); });
        }

        private void MobsOperation(MapInstance map, SummonMobModel mob)
        {

            switch (mob.CurrentAction)
            {
                case MobActionEnum.Respawn:
                    {

                        mob.Reset();
                        mob.ResetLocation();
                    }
                    break;

                case MobActionEnum.Reward:
                    {
                        _verdandiXProgram.RemoveXProgramAfterOmegamonDefeat(map, mob, BroadcastForTamerViewsAndSelf);
                        ItemsReward(map, mob);
                        QuestKillReward(map, mob);
                        ExperienceReward(map, mob);
                        HandleDungeonStepProgression(map, mob.Id, mob.Type);
                        TryHandleDungeonClear(map, mob.Type, mob.RaidDamage);
                    }
                    break;

                case MobActionEnum.Wait:
                    {

                        if (mob.Respawn && DateTime.Now > mob.DieTime.AddSeconds(2))
                        {

                            mob.SetNextWalkTime(UtilitiesFunctions.RandomInt(7, 14));
                            mob.SetAgressiveCheckTime(5);
                            mob.SetRespawn();
                        }
                        else
                        {
                            //map.AttackNearbyTamer(mob, mob.TamersViewing);
                        }
                    }
                    break;

                case MobActionEnum.Walk:
                    {
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SyncConditionPacket(mob.GeneralHandler, ConditionEnum.Default).Serialize());
                        mob.Move();
                        map.BroadcastForTargetTamers(mob.TamersViewing, new MobWalkPacket(mob).Serialize());
                    }
                    break;

                case MobActionEnum.GiveUp:
                    {
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SyncConditionPacket(mob.GeneralHandler, ConditionEnum.Immortal).Serialize());
                        mob.ResetLocation();
                        map.BroadcastForTargetTamers(mob.TamersViewing, new MobRunPacket(mob).Serialize());
                        map.BroadcastForTargetTamers(mob.TamersViewing, new SetCombatOffPacket(mob.GeneralHandler).Serialize());

                        foreach (var targetTamer in mob.TargetTamers)
                        {
                            if (targetTamer.TargetMobs.Count <= 1)
                            {
                                targetTamer.StopBattle(true);
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());
                            }
                        }

                        mob.Reset(true);
                        map.BroadcastForTargetTamers(mob.TamersViewing, new UpdateCurrentHPRatePacket(mob.GeneralHandler, mob.CurrentHpRate).Serialize());
                    }
                    break;

                case MobActionEnum.Attack:
                    {
                        if (!mob.Dead && mob.SkillTime && !mob.CheckSkill && mob.IsPossibleSkill)
                        {
                            mob.UpdateCurrentAction(MobActionEnum.UseAttackSkill);
                            mob.SetNextAction();
                            break;
                        }

                        if (!mob.Dead && ((mob.TargetTamer == null || mob.TargetTamer.Hidden) || DateTime.Now > mob.LastHitTryTime.AddSeconds(15))) //Anti-kite
                        {
                            mob.GiveUp();
                            break;
                        }

                        if (!mob.Dead && !mob.Chasing && mob.TargetAlive)
                        {
                            var diff = UtilitiesFunctions.CalculateDistance(
                                mob.CurrentLocation.X,
                                mob.Target.Location.X,
                                mob.CurrentLocation.Y,
                                mob.Target.Location.Y);

                            var range = Math.Max(mob.ARValue, mob.Target.BaseInfo.ARValue);
                            if (diff <= range)
                            {
                                if (DateTime.Now < mob.LastHitTime.AddMilliseconds(mob.ASValue))
                                    break;

                                var missed = false;

                                if (mob.TargetTamer != null && mob.TargetTamer.GodMode)
                                    missed = true;
                                else if (mob.CanMissHit())
                                    missed = true;

                                if (missed)
                                {
                                    mob.UpdateLastHitTry();
                                    map.BroadcastForTargetTamers(mob.TamersViewing, new MissHitPacket(mob.GeneralHandler, mob.TargetHandler).Serialize());
                                    mob.UpdateLastHit();
                                    break;
                                }

                                var hitTamerId = mob.TargetTamer?.Id;
                                map.AttackTarget(mob);
                                TryApplyEquipmentSetWhenHit(map, hitTamerId);
                            }
                            else
                            {
                                map.ChaseTarget(mob);
                            }
                        }

                        if (mob.Dead)
                        {
                            foreach (var targetTamer in mob.TargetTamers)
                            {

                                targetTamer.StopBattle(true);
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());

                            }
                        }
                    }
                    break;

                case MobActionEnum.UseAttackSkill:
                    {
                        if (!mob.Dead && ((mob.TargetTamer == null || mob.TargetTamer.Hidden))) //Anti-kite
                        {
                            mob.GiveUp();
                            break;
                        }

                        var skillList = _assets.MonsterSkillInfo.Where(x => x.Type == mob.Type).ToList();

                        if (!skillList.Any())
                        {
                            mob.UpdateCheckSkill(false);
                            mob.UpdateCurrentAction(MobActionEnum.Wait);
                            mob.UpdateLastSkillTry();
                            mob.SetNextAction();
                            break;
                        }

                        Random random = new Random();

                        var targetSkill = skillList[random.Next(0, skillList.Count)];

                        if (!mob.Dead && !mob.Chasing && mob.TargetAlive)
                        {
                            var diff = UtilitiesFunctions.CalculateDistance(
                               mob.CurrentLocation.X,
                               mob.Target.Location.X,
                               mob.CurrentLocation.Y,
                               mob.Target.Location.Y);

                            if (diff <= 1900)
                            {
                                if (DateTime.Now < mob.LastSkillTime.AddMilliseconds(mob.Cooldown) && mob.Cooldown > 0)
                                    break;

                                var hitTamerId = mob.TargetTamer?.Id;
                                map.SkillTarget(mob, targetSkill);
                                TryApplyEquipmentSetWhenHit(map, hitTamerId);



                                if (mob.Target != null)
                                {
                                    mob.UpdateCurrentAction(MobActionEnum.Wait);

                                    mob.SetNextAction();
                                }
                            }
                            else
                            {
                                map.ChaseTarget(mob);
                            }
                        }

                        if (mob.Dead)
                        {
                            foreach (var targetTamer in mob.TargetTamers)
                            {

                                targetTamer.StopBattle(true);
                                map.BroadcastForTamerViewsAndSelf(targetTamer.Id, new SetCombatOffPacket(targetTamer.Partner.GeneralHandler).Serialize());

                            }

                            break;
                        }
                    }
                    break;
            }
        }

        private void QuestKillReward(MapInstance map, SummonMobModel mob)
        {
            var partyIdList = new List<int>();

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;

                var giveUpList = new List<short>();

                foreach (var questInProgress in tamer.Progress.InProgressQuestData)
                {
                    var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                    if (questInfo != null)
                    {
                        if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.KillMonster))
                            continue;

                        var goalIndex = -1;
                        foreach (var questGoal in questInfo.QuestGoals)
                        {
                            if (questGoal.GoalId == mob?.Type)
                            {
                                goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                break;
                            }
                        }

                        if (goalIndex != -1)
                        {
                            var currentGoalValue = tamer.Progress.GetQuestGoalProgress(questInProgress.QuestId, goalIndex);
                            if (currentGoalValue < questInfo.QuestGoals[goalIndex].GoalAmount)
                            {
                                currentGoalValue++;
                                tamer.Progress.UpdateQuestInProgress(questInProgress.QuestId, goalIndex, currentGoalValue);

                                targetClient.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, currentGoalValue));
                                var questToUpdate = targetClient.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                                _sender.Send(new UpdateCharacterInProgressCommand(questToUpdate));
                            }
                        }
                    }
                    else
                    {
                        _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                        targetClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                        giveUpList.Add(questInProgress.QuestId);
                    }
                }

                giveUpList.ForEach(giveUp =>
                {
                    tamer.Progress.RemoveQuest(giveUp);
                });

                var party = _partyManager.FindParty(targetClient.TamerId);
                if (party != null && !partyIdList.Contains(party.Id))
                {
                    partyIdList.Add(party.Id);

                    foreach (var partyMemberId in party.Members.Values.Select(x => x.Id))
                    {
                        var partyMemberClient = map.Clients.FirstOrDefault(x => x.TamerId == partyMemberId);
                        if (partyMemberClient == null || partyMemberId == targetClient.TamerId)
                            continue;

                        giveUpList = new List<short>();

                        foreach (var questInProgress in partyMemberClient.Tamer.Progress.InProgressQuestData)
                        {
                            var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                            if (questInfo != null)
                            {
                                if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.KillMonster))
                                    continue;

                                var goalIndex = -1;
                                foreach (var questGoal in questInfo.QuestGoals)
                                {
                                    if (questGoal.GoalId == mob?.Type)
                                    {
                                        goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                        break;
                                    }
                                }

                                if (goalIndex != -1)
                                {
                                    var currentGoalValue = partyMemberClient.Tamer.Progress.GetQuestGoalProgress(questInProgress.QuestId, goalIndex);
                                    if (currentGoalValue < questInfo.QuestGoals[goalIndex].GoalAmount)
                                    {
                                        currentGoalValue++;
                                        partyMemberClient.Tamer.Progress.UpdateQuestInProgress(questInProgress.QuestId, goalIndex, currentGoalValue);

                                        partyMemberClient.Send(new QuestGoalUpdatePacket(questInProgress.QuestId, (byte)goalIndex, currentGoalValue));
                                        var questToUpdate = partyMemberClient.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                                        _sender.Send(new UpdateCharacterInProgressCommand(questToUpdate));
                                    }
                                }
                            }
                            else
                            {
                                _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                                partyMemberClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                                giveUpList.Add(questInProgress.QuestId);
                            }
                        }

                        giveUpList.ForEach(giveUp =>
                        {
                            partyMemberClient.Tamer.Progress.RemoveQuest(giveUp);
                        });
                    }
                }
            }

            partyIdList.Clear();
        }

        private void ItemsReward(MapInstance map, SummonMobModel mob)
        {
            var isRaidBoss = IsDungeonRaidBoss(map, mob.Type, mob.Class);

            if (mob.DropReward != null)
                QuestDropReward(map, mob);

            if (isRaidBoss)
                RaidReward(map, mob);
            else if (mob.DropReward != null)
                DropReward(map, mob);
        }

        private void ExperienceReward(MapInstance map, SummonMobModel mob)
        {
            if (mob.ExpReward == null)
                return;

            var partyIdList = new List<int>();

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;
                double expBonusMultiplier = CalculateExperienceMultiplier(tamer.BonusEXP, targetClient.ServerExperience);

                var basePartnerExperience = CalculateExperience(tamer.Partner.Level, mob.Level, mob.ExpReward.DigimonExperience);
                var partnerExpToReceive = (long)(basePartnerExperience * expBonusMultiplier); //TODO: +bonus

                if (basePartnerExperience == 0)
                    partnerExpToReceive = 0;

                if (partnerExpToReceive > 100) partnerExpToReceive += UtilitiesFunctions.RandomInt(-15, 15);
                var tamerExpToReceive = ExperienceRewardCalculator.CalculateTamerExperienceFromKilledDigimon(
                    tamer.Level,
                    mob.Level,
                    mob.ExpReward.DigimonExperience);

                var fatigueExp = _fatigueService.GetMultipliers(targetClient).exp;   // FATIGUE_HOOK
                var tamerResult = ReceiveTamerExp(targetClient.Tamer, tamerExpToReceive, fatigueExp);
                var partnerResult = ReceivePartnerExp(targetClient.Partner, mob, partnerExpToReceive, fatigueExp);   // FATIGUE_HOOK

                targetClient.Send(
                    new ReceiveExpPacket(
                        tamerExpToReceive,
                        0,//TODO: obter os bonus
                        targetClient.Tamer.CurrentExperience,
                        targetClient.Partner.GeneralHandler,
                        partnerExpToReceive,
                        0,//TODO: obter os bonus
                        targetClient.Partner.CurrentExperience,
                        targetClient.Partner.CurrentEvolution.SkillExperience
                    )
                );

                //TODO: importar o DMBase e tratar isso
                SkillExpReward(map, targetClient);

                if (tamerResult.LevelGain > 0 || partnerResult.LevelGain > 0)
                {
                    targetClient.Send(new UpdateStatusPacket(targetClient.Tamer));

                    map.BroadcastForTamerViewsAndSelf(targetClient.TamerId,
                        new UpdateMovementSpeedPacket(targetClient.Tamer).Serialize());
                }

                _sender.Send(new UpdateCharacterExperienceCommand(tamer));
                _sender.Send(new UpdateDigimonExperienceCommand(tamer.Partner));

                PartyExperienceReward(map, mob, partyIdList, targetClient, ref tamerExpToReceive, ref tamerResult, ref partnerExpToReceive, ref partnerResult);
            }

            partyIdList.Clear();
        }
        private void DropReward(MapInstance map, SummonMobModel mob)
        {
            var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == mob.TargetTamer?.Id);
            if (targetClient == null)
                return;

            BitDropReward(map, mob, targetClient);

            ItemDropReward(map, mob, targetClient);
        }

        private void BitDropReward(MapInstance map, SummonMobModel mob, GameClient? targetClient)
        {
            // FATIGUE_HOOK
            var fatigueDrop = (double)_fatigueService.GetMultipliers(targetClient).drop;
            if (fatigueDrop <= 0) return;

            var bitsReward = mob.DropReward.BitsDrop;

            if (bitsReward != null && bitsReward.Chance * fatigueDrop >= UtilitiesFunctions.RandomDouble())
            {
                if (targetClient.Tamer.HasAura && targetClient.Tamer.Aura.ItemInfo.Section == 2100)
                {
                    var amount = UtilitiesFunctions.RandomInt(bitsReward.MinAmount, bitsReward.MaxAmount);

                    targetClient.Send(
                        new PickBitsPacket(
                            targetClient.Tamer.GeneralHandler,
                            amount
                        )
                    );

                    targetClient.Tamer.Inventory.AddBits(amount);

                    _sender.Send(new UpdateItemsCommand(targetClient.Tamer.Inventory)).GetAwaiter().GetResult();
                    _logger.Verbose($"Character {targetClient.TamerId} aquired {amount} bits from mob {mob.Id} with magnetic aura {targetClient.Tamer.Aura.ItemId}.");
                }
                else
                {
                    var drop = _dropManager.CreateBitDrop(
                        targetClient.TamerId,
                        targetClient.Tamer.GeneralHandler,
                        bitsReward.MinAmount,
                        bitsReward.MaxAmount,
                        mob.CurrentLocation.MapId,
                        mob.CurrentLocation.X,
                        mob.CurrentLocation.Y
                    );

                    map.AddMapDrop(drop);
                }
            }
        }

        private void ItemDropReward(MapInstance map, SummonMobModel mob, GameClient? targetClient)
        {
            if (!mob.DropReward.Drops.Any())
                return;

            // FATIGUE_HOOK
            var fatigueDrop = (double)_fatigueService.GetMultipliers(targetClient).drop;
            if (fatigueDrop <= 0) return;

            var itemsReward = new List<SummonMobItemDropModel>();
            itemsReward.AddRange(mob.DropReward.Drops);
            itemsReward.RemoveAll(x => _assets.QuestItemList.Contains(x.ItemId));

            if (!itemsReward.Any())
                return;

            var dropped = 0;
            var totalDrops = UtilitiesFunctions.RandomInt(
                mob.DropReward.MinAmount,
                mob.DropReward.MaxAmount);

            while (dropped < totalDrops)
            {
                if (!itemsReward.Any())
                {
                    _logger.Warning($"Mob {mob.Id} has incorrect drops configuration.");
                    break;
                }

                var possibleDrops = itemsReward.OrderBy(x => Guid.NewGuid()).ToList();
                foreach (var itemDrop in possibleDrops)
                {
                    if (itemDrop.Chance * fatigueDrop >= UtilitiesFunctions.RandomDouble())   // FATIGUE_HOOK
                    {
                        if (targetClient.Tamer.HasAura && targetClient.Tamer.Aura.ItemInfo.Section == 2100)
                        {
                            var newItem = new ItemModel();
                            newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemDrop.ItemId));

                            if (newItem.ItemInfo == null)
                            {
                                _logger.Warning($"No item info found with ID {itemDrop.ItemId} for tamer {targetClient.Tamer.Id}.");
                                targetClient.Send(new SystemMessagePacket($"No item info found with ID {itemDrop.ItemId}."));
                                continue;
                            }

                            newItem.ItemId = itemDrop.ItemId;
                            newItem.Amount = UtilitiesFunctions.RandomInt(itemDrop.MinAmount, itemDrop.MaxAmount);

                            var itemClone = (ItemModel)newItem.Clone();
                            if (targetClient.Tamer.Inventory.AddItem(newItem))
                            {
                                targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                _sender.Send(new UpdateItemsCommand(targetClient.Tamer.Inventory)).GetAwaiter().GetResult();
                                _logger.Verbose($"Character {targetClient.TamerId} aquired {newItem.ItemId} x{newItem.Amount} from " +
                                    $"mob {mob.Id} with magnetic aura {targetClient.Tamer.Aura.ItemId}.");
                            }
                            else
                            {
                                targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));

                                var drop = _dropManager.CreateItemDrop(
                                    targetClient.Tamer.Id,
                                    targetClient.Tamer.GeneralHandler,
                                    itemDrop.ItemId,
                                    itemDrop.MinAmount,
                                    itemDrop.MaxAmount,
                                    mob.CurrentLocation.MapId,
                                    mob.CurrentLocation.X,
                                    mob.CurrentLocation.Y
                                );

                                map.AddMapDrop(drop);
                            }

                            dropped++;
                        }
                        else
                        {
                            var drop = _dropManager.CreateItemDrop(
                                targetClient.Tamer.Id,
                                targetClient.Tamer.GeneralHandler,
                                itemDrop.ItemId,
                                itemDrop.MinAmount,
                                itemDrop.MaxAmount,
                                mob.CurrentLocation.MapId,
                                mob.CurrentLocation.X,
                                mob.CurrentLocation.Y
                            );

                            dropped++;

                            map.AddMapDrop(drop);
                        }

                        itemsReward.RemoveAll(x => x.Id == itemDrop.Id);
                        break;
                    }
                }
            }
        }

        private void QuestDropReward(MapInstance map, SummonMobModel mob)
        {
            var itemsReward = BuildQuestItemRewards(mob);

            if (!itemsReward.Any())
                return;

            foreach (var tamer in mob.TargetTamers)
            {
                var targetClient = map.Clients.FirstOrDefault(x => x.TamerId == tamer?.Id);
                if (targetClient == null)
                    continue;

                if (!tamer.Progress.InProgressQuestData.Any())
                    continue;

                var updateItemList = false;
                var possibleDrops = itemsReward.Randomize();
                foreach (var itemDrop in possibleDrops)
                {
                    if (itemDrop.Chance >= UtilitiesFunctions.RandomDouble())
                    {
                        foreach (var questInProgress in tamer.Progress.InProgressQuestData)
                        {
                            var questInfo = _assets.Quest.FirstOrDefault(x => x.QuestId == questInProgress.QuestId);
                            if (questInfo != null)
                            {
                                if (!questInfo.QuestGoals.Exists(x => x.GoalType == QuestGoalTypeEnum.LootItem))
                                    continue;

                                var goalIndex = -1;
                                foreach (var questGoal in questInfo.QuestGoals)
                                {
                                    if (questGoal.GoalId == itemDrop?.ItemId)
                                    {
                                        var inventoryItems = tamer.Inventory.FindItemsById(questGoal.GoalId);
                                        var goalAmount = questGoal.GoalAmount;

                                        foreach (var inventoryItem in inventoryItems)
                                        {
                                            goalAmount -= inventoryItem.Amount;
                                            if (goalAmount <= 0)
                                            {
                                                goalAmount = 0;
                                                break;
                                            }
                                        }

                                        if (goalAmount > 0)
                                        {
                                            goalIndex = questInfo.QuestGoals.FindIndex(x => x == questGoal);
                                            break;
                                        }
                                    }
                                }

                                if (goalIndex != -1)
                                {
                                    var newItem = new ItemModel();
                                    newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemDrop.ItemId));

                                    if (newItem.ItemInfo == null)
                                    {
                                        _logger.Warning($"No item info found with ID {itemDrop.ItemId} for tamer {tamer.Id}.");
                                        targetClient.Send(new SystemMessagePacket($"No item info found with ID {itemDrop.ItemId}."));
                                        continue;
                                    }

                                    newItem.ItemId = itemDrop.ItemId;
                                    newItem.Amount = UtilitiesFunctions.RandomInt(itemDrop.MinAmount, itemDrop.MaxAmount);

                                    var itemClone = (ItemModel)newItem.Clone();
                                    if (tamer.Inventory.AddItem(newItem))
                                    {
                                        updateItemList = true;
                                        targetClient.Send(new ReceiveItemPacket(itemClone, InventoryTypeEnum.Inventory));
                                    }
                                    else
                                    {
                                        targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                                    }
                                }
                            }
                            else
                            {
                                _logger.Error($"Unknown quest id {questInProgress.QuestId}.");
                                targetClient.Send(new SystemMessagePacket($"Unknown quest id {questInProgress.QuestId}."));
                            }
                        }

                        if (updateItemList) _sender.Send(new UpdateItemsCommand(tamer.Inventory)).GetAwaiter().GetResult();

                        itemsReward.RemoveAll(x => x.Id == itemDrop.Id);
                    }
                }
            }
        }

        private int SkillExperienceTable(int evolutionType, int SkillMastery)
        {
            List<Tuple<int, int>> RockieExperienceTemp = new List<Tuple<int, int>>
                      {
                          new Tuple<int, int>(0, 1792),
                          new Tuple<int, int>(1, 2835),
                          new Tuple<int, int>(2, 4131),
                          new Tuple<int, int>(3, 5688),
                          new Tuple<int, int>(4, 7516),
                          new Tuple<int, int>(5, 9622),
                          new Tuple<int, int>(6, 12015),
                          new Tuple<int, int>(7, 14701),
                          new Tuple<int, int>(8, 17690),
                          new Tuple<int, int>(9, 20989),
                          new Tuple<int, int>(10, 24607),
                          new Tuple<int, int>(11, 28552),
                          new Tuple<int, int>(12, 32832),
                          new Tuple<int, int>(13, 37454),
                          new Tuple<int, int>(14, 42427)
            };

            var ChampionExperienceTemp = new List<Tuple<int, int>>
                {
                    new Tuple<int, int>(0, 14701),
                    new Tuple<int, int>(1, 17690),
                    new Tuple<int, int>(2, 20989),
                    new Tuple<int, int>(3, 24607),
                    new Tuple<int, int>(4, 28552),
                    new Tuple<int, int>(5, 32832),
                    new Tuple<int, int>(6, 37454),
                    new Tuple<int, int>(7, 42427),
                    new Tuple<int, int>(8, 47760),
                    new Tuple<int, int>(9,  53460),
                    new Tuple<int, int>(10, 66083),
                    new Tuple<int, int>(11, 73180),
                    new Tuple<int, int>(12, 80698),
                    new Tuple<int, int>(13, 88646),
                    new Tuple<int, int>(14, 97030)
                };

            var UltimateExperienceTemp = new List<Tuple<int, int>>
                {
                    new Tuple<int, int>(0, 97030),
                    new Tuple<int, int>(1, 105861),
                    new Tuple<int, int>(2, 115145),
                    new Tuple<int, int>(3, 124891),
                    new Tuple<int, int>(4, 135106),
                    new Tuple<int, int>(5, 218700),
                    new Tuple<int, int>(6, 285417),
                    new Tuple<int, int>(7, 359424),
                    new Tuple<int, int>(8, 441045),
                    new Tuple<int, int>(9,  546210),
                    new Tuple<int, int>(10, 661500),
                    new Tuple<int, int>(11, 787320),
                    new Tuple<int, int>(12, 924075),
                    new Tuple<int, int>(13, 1072170),
                    new Tuple<int, int>(14, 1232010)
                };

            var MegaExperienceTemp = new List<Tuple<int, int>>
                {
                    new Tuple<int, int>(0, 1588544),
                    new Tuple<int, int>(1, 1786050),
                    new Tuple<int, int>(2, 1996920),
                    new Tuple<int, int>(3, 2221560),
                    new Tuple<int, int>(4, 2460375),
                    new Tuple<int, int>(5, 2713770),
                    new Tuple<int, int>(6, 2982150),
                    new Tuple<int, int>(7, 3265920),
                    new Tuple<int, int>(8, 3565485),
                    new Tuple<int, int>(9,  3881250),
                    new Tuple<int, int>(10, 4213620),
                    new Tuple<int, int>(11, 4563000),
                    new Tuple<int, int>(12, 4929795),
                    new Tuple<int, int>(13, 5314410),
                    new Tuple<int, int>(14, 5717250)
                };

            var JogressExperienceTemp = new List<Tuple<int, int>>
                {
                    new Tuple<int, int>(0, 0),
                    new Tuple<int, int>(1, 6138720),
                    new Tuple<int, int>(2, 6579225),
                    new Tuple<int, int>(3, 7039170),
                    new Tuple<int, int>(4, 7518960),
                    new Tuple<int, int>(5, 8019000),
                    new Tuple<int, int>(6, 8539695),
                    new Tuple<int, int>(7, 9081450),
                    new Tuple<int, int>(8, 9644670),
                    new Tuple<int, int>(9,  10229760),
                    new Tuple<int, int>(10, 10837125),
                    new Tuple<int, int>(11, 11467170),
                    new Tuple<int, int>(12, 12120300),
                    new Tuple<int, int>(13, 12796920),
                    new Tuple<int, int>(14, 13497435)
                };

            var BurstModeExperienceTemp = new List<Tuple<int, int>>
               {
                   new Tuple<int, int>(0, 4213619),
                   new Tuple<int, int>(1, 14971770),
                   new Tuple<int, int>(2, 15746400),
                   new Tuple<int, int>(3, 16546545),
                   new Tuple<int, int>(4, 17372610),
                   new Tuple<int, int>(5, 18225000),
                   new Tuple<int, int>(6, 19104120),
                   new Tuple<int, int>(7, 20010374),
                   new Tuple<int, int>(8, 20944170),
                   new Tuple<int, int>(9,  21474836),
                   new Tuple<int, int>(10, 21474836),
                   new Tuple<int, int>(11, 21474836),
                   new Tuple<int, int>(12, 21474836),
                   new Tuple<int, int>(13, 21474836),
                   new Tuple<int, int>(14, 21474836)
               };

            switch ((EvolutionRankEnum)evolutionType)
            {

                case EvolutionRankEnum.RookieX:
                case EvolutionRankEnum.Rookie:
                    return RockieExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.ChampionX:
                case EvolutionRankEnum.Champion:
                    return ChampionExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.UltimateX:
                case EvolutionRankEnum.Ultimate:
                    return UltimateExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.MegaX:
                case EvolutionRankEnum.Mega:
                    return MegaExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.BurstModeX:
                case EvolutionRankEnum.BurstMode:
                    return BurstModeExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.JogressX:
                case EvolutionRankEnum.Jogress:
                    return JogressExperienceTemp.FirstOrDefault(x => x.Item1 == SkillMastery)?.Item2 ?? -1;

                case EvolutionRankEnum.Capsule:
                    break;
                case EvolutionRankEnum.Spirit:
                    break;


                case EvolutionRankEnum.Extra:
                    break;
                default:
                    break;
            }

            return -1;

        }
    }
}
