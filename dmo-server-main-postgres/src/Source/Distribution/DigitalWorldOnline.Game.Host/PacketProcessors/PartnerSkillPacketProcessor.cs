using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Combat;
using DigitalWorldOnline.Commons.Models.Config;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Models.Summon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;
using static DigitalWorldOnline.Commons.Packets.GameServer.AddBuffPacket;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public partial class PartnerSkillPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartnerSkill;

        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly DamageFormulaConfig _damageFormulaConfig;

        public PartnerSkillPacketProcessor(

            AssetsLoader assets,
            MapServer mapServer,
            ILogger logger,
            ISender sender,
            DungeonsServer dungeonServer,
            IConfiguration configuration)
        {
            _assets = assets;
            _mapServer = mapServer;
            _logger = logger;
            _sender = sender;
            _dungeonServer = dungeonServer;
            _damageFormulaConfig = LoadDamageFormulaConfig(configuration);
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var skillSlot = packet.ReadByte();
            var attackerHandler = packet.ReadInt();
            var targetHandler = packet.ReadInt();

            // Client sends `cType.m_nTypeAll` lower 32 bits as targetUID — that's the
            // packed (type << 19) | (class << 14) | idx encoding from v487's pType.h.  The
            // server's MobConfigModel.GeneralHandler is (HandlerRange + mapHandler), which
            // sits in the lower 19 bits (class + idx, no type).  So we strip the type
            // portion before any handler comparison: mask to 0x7FFFF.  Without this, the
            // lookup against `.Mobs.FirstOrDefault(x => x.GeneralHandler == handler)`
            // never matches because the client send carries type bits that the server
            // representation doesn't.  Same applies for the attackerHandler.
            const int CTypeClassIdxMask = 0x7FFFF;
            attackerHandler &= CTypeClassIdxMask;
            targetHandler &= CTypeClassIdxMask;

            if (client.Partner == null)
            {
                _logger.Information("Skill ignored: tamer={TamerId} has no partner. slot={Slot} attacker={Attacker} target={Target}",
                    client.TamerId, skillSlot, attackerHandler, targetHandler);
                return Task.CompletedTask;
            }

            var skill = _assets.DigimonSkillInfo.FirstOrDefault(x => x.Type == client.Partner.CurrentType && x.Slot == skillSlot);
            if (skill == null)
            {
                _logger.Information("Skill not found: currentType={CurrentType} slot={Slot} baseType={BaseType} tamer={TamerId}",
                    client.Partner.CurrentType, skillSlot, client.Partner.BaseType, client.TamerId);
                return Task.CompletedTask;
            }
            if (skill.SkillInfo == null)
            {
                _logger.Information("Skill found but SkillInfo null: currentType={CurrentType} slot={Slot} skillId={SkillId} tamer={TamerId}",
                    client.Partner.CurrentType, skillSlot, skill.SkillId, client.TamerId);
                return Task.CompletedTask;
            }

            _logger.Information("Skill processing: currentType={CurrentType} slot={Slot} skillId={SkillId} target={TargetKind} area={Area} aoeMax={AoeMax} attacker={Attacker} targetHandler={TargetHandler} tamer={TamerId} map={MapId}",
                client.Partner.CurrentType, skillSlot, skill.SkillId,
                skill.SkillInfo.Target, skill.SkillInfo.AreaOfEffect, skill.SkillInfo.AoEMaxDamage,
                attackerHandler, targetHandler, client.TamerId, client.Tamer.Location.MapId);

            // Passive skills (CsSkill::s_nAttType == 4) are always-on buffs — they fire from
            // buff-state machinery, never from a player-triggered cast packet.  Reject so a
            // crafted client can't trigger damage/cooldown logic on a non-active skill.
            if (skill.SkillInfo.IsPassive)
                return Task.CompletedTask;

            var targetSummonMobs = new List<SummonMobModel>();
            SkillTypeEnum skillType;
            if (client.DungeonMap)
            {
                if (_dungeonServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler, true, client.TamerId) != null)
                {

                    if (IsImplosionSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.Implosion;

                        var targets = _dungeonServer.GetMobsNearbyPartner(client.Partner.Location, skill.SkillInfo.AreaOfEffect, true, client.TamerId);

                        targetSummonMobs.AddRange(targets);
                    }
                    else if (IsTargetAreaSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.TargetArea;

                        var targets = _dungeonServer.GetMobsNearbyTargetMob(client.Tamer.Location.MapId, targetHandler, skill.SkillInfo.Range / 10, true, client.TamerId);

                        targetSummonMobs.AddRange(targets);
                    }
                    else
                    {
                        skillType = SkillTypeEnum.Single;

                        var mob = _dungeonServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler, true, client.TamerId);

                        if (mob == null)
                            return Task.CompletedTask;

                        targetSummonMobs.Add(mob);
                    }

                    if (targetSummonMobs.Any())
                    {
                        if (skillType == SkillTypeEnum.Single && !targetSummonMobs.First().Alive)
                            return Task.CompletedTask;

                        client.Partner.ReceiveDamage(skill.SkillInfo.HPUsage);
                        client.Partner.UseDs(skill.SkillInfo.DSUsage);

                        var castingTime = (int)Math.Round((float)0);
                        if (castingTime <= 0) castingTime = 2000;

                        client.Partner.SetEndCasting(0);

                        targetSummonMobs.ForEach(targetMob =>
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name} with skill {skill.SkillId}.");
                        });

                        if (!client.Tamer.InBattle)
                        {
                            client.Tamer.SetHidden(false);
                            _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattleWithSkill(targetSummonMobs, skillType);
                        }
                        else
                        {
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTargetWithSkill(targetSummonMobs, skillType);
                        }

                        if (skillType != SkillTypeEnum.Single)
                        {
                            var finalDmg = 0;

                            if (!client.Tamer.GodMode)
                            {
                                //TODO: regra de 3 para redução de dano conforme distancia do ponto de origem
                                if (skill.SkillInfo.AoEMinDamage > 0 && skill.SkillInfo.AoEMaxDamage > 0)
                                    finalDmg = UtilitiesFunctions.RandomInt(skill.SkillInfo.AoEMinDamage, skill.SkillInfo.AoEMaxDamage);
                                else
                                    finalDmg = client.Partner.AT + UtilitiesFunctions.RandomInt(2300, 3500);
                            }
                            else
                                finalDmg = int.MaxValue;

                            targetSummonMobs.ForEach(targetMob =>
                            {
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                if (!targetMob.InBattle)
                                {
                                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                    targetMob.StartBattle(client.Tamer);
                                }
                                else
                                {
                                    targetMob.AddTarget(client.Tamer);
                                }

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);
                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");
                                }
                                else
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");
                                    targetMob?.Die();
                                }
                            });

                            // pGame::Skill (1015) is a "pre-cast lock-on" packet from a
                            // post-v487 client build. v487's RecvSkill is stubbed
                            // assert(false). The cast animation + damage display are both
                            // driven by the next packet (AreaSkillPacket=1116 or
                            // SkillHitPacket=1102) which carry the hitter UID + skill index;
                            // v487's _RecvDigimonSkill_ApplyAround / _RecvSkillApplyAround_
                            // ExistHitter calls SetSkillInfo() to render cast + damage atomically.

                            _dungeonServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new AreaSkillPacket(
                                    attackerHandler,
                                    client.Partner.HpRate,
                                    targetSummonMobs,
                                    skillSlot,
                                    finalDmg
                                ).Serialize()
                            );
                            ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, true);
                        }
                        else
                        {


                            var targetMob = targetSummonMobs.First();

                            if (!targetMob.InBattle)
                            {
                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateDamageOrHeal(client, targetMob, skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);

                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");

                                // pGame::Skill (1015) skipped for v487 — see comment above.

                                _dungeonServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new SkillHitPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg,
                                        targetMob.CurrentHpRate
                                        ).Serialize());
                                ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, true);
                            }
                            else
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");

                                _dungeonServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnSkillPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg
                                        ).Serialize());

                                ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, true);
                                targetMob?.Die();
                            }
                        }

                        if (!_dungeonServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId, true))
                        {
                            client.Tamer.StopBattle(true);

                            SendBattleOffTask(client, attackerHandler, true);
                        }

                        var evolution = client.Tamer.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Tamer.Partner.CurrentType);

                        if (evolution != null && skill.SkillInfo.Cooldown / 1000 > 59)
                        {
                            evolution.Skills[skillSlot].SetCooldown(skill.SkillInfo.Cooldown / 1000);
                            _sender.Send(new UpdateEvolutionCommand(evolution));
                        }
                    }
                }
                else
                {

                    var targetMobs = new List<MobConfigModel>();



                    if (IsImplosionSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.Implosion;

                        var targets = _mapServer.GetMobsNearbyPartner(client.Partner.Location, skill.SkillInfo.AreaOfEffect);

                        targetMobs.AddRange(targets);
                    }
                    else if (IsTargetAreaSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.TargetArea;

                        var targets = _mapServer.GetMobsNearbyTargetMob(client.Tamer.Location.MapId, targetHandler, skill.SkillInfo.Range / 10);

                        targetMobs.AddRange(targets);
                    }
                    else
                    {
                        skillType = SkillTypeEnum.Single;

                        var mob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler);
                        if (mob == null)
                        {
                            _logger.Information("Skill target mob not found: currentType={CurrentType} slot={Slot} skillId={SkillId} targetHandler={TargetHandler} tamer={TamerId} map={MapId}",
                                client.Partner.CurrentType, skillSlot, skill.SkillId, targetHandler, client.TamerId, client.Tamer.Location.MapId);
                            return Task.CompletedTask;
                        }

                        targetMobs.Add(mob);
                    }

                    if (targetMobs.Any())
                    {
                        if (skillType == SkillTypeEnum.Single && !targetMobs.First().Alive)
                            return Task.CompletedTask;

                        client.Partner.ReceiveDamage(skill.SkillInfo.HPUsage);
                        client.Partner.UseDs(skill.SkillInfo.DSUsage);

                        var castingTime = (int)Math.Round((float)0);
                        if (castingTime <= 0) castingTime = 2000;

                        client.Partner.SetEndCasting(0);

                        targetMobs.ForEach(targetMob =>
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name} with skill {skill.SkillId}.");
                        });

                        if (!client.Tamer.InBattle)
                        {
                            client.Tamer.SetHidden(false);
                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattleWithSkill(targetMobs, skillType);
                        }
                        else
                        {
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTargetWithSkill(targetMobs, skillType);
                        }

                        if (skillType != SkillTypeEnum.Single)
                        {
                            var finalDmg = 0;

                            if (!client.Tamer.GodMode)
                            {
                                //TODO: regra de 3 para redução de dano conforme distancia do ponto de origem
                                finalDmg = CalculateDamageOrHeal(client, targetMobs.First(), skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);

                                if (finalDmg != 0)
                                {
                                    finalDmg = DebuffReductionDamage(client, finalDmg);
                                }
                            }
                            else
                                finalDmg = int.MaxValue;

                            targetMobs.ForEach(targetMob =>
                            {
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                if (!targetMob.InBattle)
                                {
                                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                    targetMob.StartBattle(client.Tamer);
                                }
                                else
                                {
                                    targetMob.AddTarget(client.Tamer);
                                }

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);
                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");
                                }
                                else
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");
                                    targetMob?.Die();
                                }
                            });

                            // pGame::Skill (1015) is a "pre-cast lock-on" packet from a
                            // post-v487 client build. v487's RecvSkill is stubbed
                            // assert(false). The cast animation + damage display are both
                            // driven by the next packet (AreaSkillPacket=1116 or
                            // SkillHitPacket=1102) which carry the hitter UID + skill index;
                            // v487's _RecvDigimonSkill_ApplyAround / _RecvSkillApplyAround_
                            // ExistHitter calls SetSkillInfo() to render cast + damage atomically.

                            _mapServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new AreaSkillPacket(
                                    attackerHandler,
                                    client.Partner.HpRate,
                                    targetMobs,
                                    skillSlot,
                                    finalDmg
                                ).Serialize()
                            );
                            ApplyDigimonSkillEffects(client, skill, targetMobs, skillSlot, false);
                        }
                        else
                        {
                            var targetMob = targetMobs.First();

                            if (!targetMob.InBattle)
                            {
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateDamageOrHeal(client, targetMob, skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);
                            
                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }

                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");

                                // pGame::Skill (1015) skipped for v487 — see comment above.

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new SkillHitPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg,
                                        targetMob.CurrentHpRate
                                        ).Serialize());
                                ApplyDigimonSkillEffects(client, skill, new[] { targetMob }, skillSlot, false);

                                
                            }
                            else
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnSkillPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg
                                        ).Serialize());

                                ApplyDigimonSkillEffects(client, skill, new[] { targetMob }, skillSlot, false);
                                targetMob?.Die();
                            }
                        }

                        if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                        {
                            client.Tamer.StopBattle(true);

                            SendBattleOffTask(client, attackerHandler);
                        }

                        var evolution = client.Tamer.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Tamer.Partner.CurrentType);

                        if (evolution != null && skill.SkillInfo.Cooldown / 1000 > 59)
                        {
                            evolution.Skills[skillSlot].SetCooldown(skill.SkillInfo.Cooldown / 1000);
                            _sender.Send(new UpdateEvolutionCommand(evolution));
                        }
                    }
                }
            }
            else
            {
                if (_mapServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler, true) != null)
                {

                    if (IsImplosionSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.Implosion;

                        var targets = _mapServer.GetMobsNearbyPartner(client.Partner.Location, skill.SkillInfo.AreaOfEffect, true);

                        targetSummonMobs.AddRange(targets);
                    }
                    else if (IsTargetAreaSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.TargetArea;

                        var targets = _mapServer.GetMobsNearbyTargetMob(client.Tamer.Location.MapId, targetHandler, skill.SkillInfo.Range / 10, true);

                        targetSummonMobs.AddRange(targets);
                    }
                    else
                    {
                        skillType = SkillTypeEnum.Single;

                        var mob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler, true);
                        if (mob == null)
                            return Task.CompletedTask;

                        targetSummonMobs.Add(mob);
                    }

                    if (targetSummonMobs.Any())
                    {
                        if (skillType == SkillTypeEnum.Single && !targetSummonMobs.First().Alive)
                            return Task.CompletedTask;

                        client.Partner.ReceiveDamage(skill.SkillInfo.HPUsage);
                        client.Partner.UseDs(skill.SkillInfo.DSUsage);

                        var castingTime = (int)Math.Round((float)0);
                        if (castingTime <= 0) castingTime = 2000;

                        client.Partner.SetEndCasting(0);

                        targetSummonMobs.ForEach(targetMob =>
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name} with skill {skill.SkillId}.");
                        });

                        if (!client.Tamer.InBattle)
                        {
                            client.Tamer.SetHidden(false);
                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattleWithSkill(targetSummonMobs, skillType);
                        }
                        else
                        {
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTargetWithSkill(targetSummonMobs, skillType);
                        }

                        if (skillType != SkillTypeEnum.Single)
                        {
                            var finalDmg = 0;

                            if (!client.Tamer.GodMode)
                            {
                                //TODO: regra de 3 para redução de dano conforme distancia do ponto de origem

                                finalDmg = CalculateDamageOrHeal(client, targetSummonMobs.First(), skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);

                                if (finalDmg != 0 && !client.Tamer.GodMode)
                                {
                                    finalDmg = DebuffReductionDamage(client, finalDmg);
                                }
                            }
                            else
                                finalDmg = int.MaxValue;

                            targetSummonMobs.ForEach(targetMob =>
                            {
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                if (!targetMob.InBattle)
                                {
                                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                    targetMob.StartBattle(client.Tamer);
                                }
                                else
                                {
                                    targetMob.AddTarget(client.Tamer);
                                }

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);
                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");
                                }
                                else
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");
                                    targetMob?.Die();
                                }
                            });

                            // pGame::Skill (1015) skipped for v487 — see comment above.

                            _mapServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new AreaSkillPacket(
                                    attackerHandler,
                                    client.Partner.HpRate,
                                    targetSummonMobs,
                                    skillSlot,
                                    finalDmg
                                ).Serialize()
                            );
                            ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, false);
                        }
                        else
                        {


                            var targetMob = targetSummonMobs.First();

                            if (!targetMob.InBattle)
                            {
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateDamageOrHeal(client, targetMob, skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);

                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }

                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");

                                // pGame::Skill (1015) skipped for v487 — see comment above.

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new SkillHitPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg,
                                        targetMob.CurrentHpRate
                                        ).Serialize());
                                ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, false);

                            }
                            else
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnSkillPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg
                                        ).Serialize());

                                ApplyDigimonSkillEffects(client, skill, Enumerable.Empty<MobConfigModel>(), skillSlot, false);
                                targetMob?.Die();
                            }
                        }

                        if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId, true))
                        {
                            client.Tamer.StopBattle(true);

                            SendBattleOffTask(client, attackerHandler);
                        }

                        var evolution = client.Tamer.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Tamer.Partner.CurrentType);

                        if (evolution != null && skill.SkillInfo.Cooldown / 1000 > 59)
                        {
                            evolution.Skills[skillSlot].SetCooldown(skill.SkillInfo.Cooldown / 1000);
                            _sender.Send(new UpdateEvolutionCommand(evolution));
                        }
                    }
                }
                else
                {

                    var targetMobs = new List<MobConfigModel>();



                    if (IsImplosionSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.Implosion;

                        var targets = _mapServer.GetMobsNearbyPartner(client.Partner.Location, skill.SkillInfo.AreaOfEffect);

                        targetMobs.AddRange(targets);
                    }
                    else if (IsTargetAreaSkill(skill.SkillInfo))
                    {
                        skillType = SkillTypeEnum.TargetArea;

                        var targets = _mapServer.GetMobsNearbyTargetMob(client.Tamer.Location.MapId, targetHandler, skill.SkillInfo.Range / 10);

                        targetMobs.AddRange(targets);

                        // Fallback to single-target lookup when AoE-near-target finds no mobs.
                        // AoEMinDamage/MaxDamage in the bin is the regular damage roll range
                        // for ANY skill, not an AoE-only flag — single-target skills like
                        // Baby Flame (Target=51, AoEMaxDamage=1700) wrongly route here, then
                        // GetMobsNearbyTargetMob can return empty because the handler may not
                        // be in .Mobs for the relevant collection.  True AoE skills have
                        // AreaOfEffect > 0 and take the first branch.
                        if (!targetMobs.Any())
                        {
                            var mob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler);
                            if (mob != null)
                            {
                                skillType = SkillTypeEnum.Single;
                                targetMobs.Add(mob);
                            }
                        }
                    }
                    else
                    {
                        skillType = SkillTypeEnum.Single;

                        var mob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, targetHandler);
                        if (mob == null)
                        {
                            _logger.Information("Skill target mob not found: currentType={CurrentType} slot={Slot} skillId={SkillId} targetHandler={TargetHandler} tamer={TamerId} map={MapId}",
                                client.Partner.CurrentType, skillSlot, skill.SkillId, targetHandler, client.TamerId, client.Tamer.Location.MapId);
                            return Task.CompletedTask;
                        }

                        targetMobs.Add(mob);
                    }

                    if (targetMobs.Any())
                    {
                        if (skillType == SkillTypeEnum.Single && !targetMobs.First().Alive)
                            return Task.CompletedTask;

                        client.Partner.ReceiveDamage(skill.SkillInfo.HPUsage);
                        client.Partner.UseDs(skill.SkillInfo.DSUsage);

                        var castingTime = (int)Math.Round((float)0);
                        if (castingTime <= 0) castingTime = 2000;

                        client.Partner.SetEndCasting(0);

                        targetMobs.ForEach(targetMob =>
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name} with skill {skill.SkillId}.");
                        });

                        if (!client.Tamer.InBattle)
                        {
                            client.Tamer.SetHidden(false);
                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattleWithSkill(targetMobs, skillType);
                        }
                        else
                        {
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTargetWithSkill(targetMobs, skillType);
                        }

                        if (skillType != SkillTypeEnum.Single)
                        {
                            var finalDmg = 0;

                            if (!client.Tamer.GodMode)
                            {
                                //TODO: regra de 3 para redução de dano conforme distancia do ponto de origem
                                if (skill.SkillInfo.AoEMinDamage > 0 && skill.SkillInfo.AoEMaxDamage > 0)
                                    finalDmg = UtilitiesFunctions.RandomInt(skill.SkillInfo.AoEMinDamage, skill.SkillInfo.AoEMaxDamage);
                                else
                                    finalDmg = client.Partner.AT + UtilitiesFunctions.RandomInt(2300, 3500);
                            }
                            else
                                finalDmg = int.MaxValue;

                            targetMobs.ForEach(targetMob =>
                            {
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                if (!targetMob.InBattle)
                                {
                                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                    targetMob.StartBattle(client.Tamer);
                                }
                                else
                                {
                                    targetMob.AddTarget(client.Tamer);
                                }

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);
                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");
                                }
                                else
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");
                                    targetMob?.Die();
                                }
                            });

                            // pGame::Skill (1015) skipped for v487 — see comment above.

                            _mapServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new AreaSkillPacket(
                                    attackerHandler,
                                    client.Partner.HpRate,
                                    targetMobs,
                                    skillSlot,
                                    finalDmg
                                ).Serialize()
                            );
                            ApplyDigimonSkillEffects(client, skill, targetMobs, skillSlot, false);
                        }
                        else
                        {
                            var targetMob = targetMobs.First();

                            if (!targetMob.InBattle)
                            {
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateDamageOrHeal(client, targetMob, skill, _assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == skill.SkillId), skillSlot);

                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }

                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} damage with skill {skill.SkillId} in mob {targetMob?.Id} - {targetMob?.Name}.");

                                // pGame::Skill (1015) skipped for v487 — see comment above.

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new SkillHitPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg,
                                        targetMob.CurrentHpRate
                                        ).Serialize());
                                ApplyDigimonSkillEffects(client, skill, new[] { targetMob }, skillSlot, false);

                            }
                            else
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name} with {finalDmg} skill {skill.Id} damage.");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnSkillPacket(
                                        attackerHandler,
                                        targetMob.GeneralHandler,
                                        skillSlot,
                                        finalDmg
                                        ).Serialize());

                                ApplyDigimonSkillEffects(client, skill, new[] { targetMob }, skillSlot, false);
                                targetMob?.Die();
                            }
                        }

                        if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                        {
                            client.Tamer.StopBattle();

                            SendBattleOffTask(client, attackerHandler);
                        }

                        var evolution = client.Tamer.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Tamer.Partner.CurrentType);

                        if (evolution != null && skill.SkillInfo.Cooldown / 1000 > 59)
                        {
                            evolution.Skills[skillSlot].SetCooldown(skill.SkillInfo.Cooldown / 1000);
                            _sender.Send(new UpdateEvolutionCommand(evolution));
                        }
                    }
                }
            }

            return Task.CompletedTask;
        }

        public async Task SendBattleOffTask(GameClient client, int attackerHandler)
        {
            await Task.Run(async () =>
            {
                Thread.Sleep(4000);

                _mapServer.BroadcastForTamerViewsAndSelf(
                        client.TamerId,
                        new SetCombatOffPacket(attackerHandler).Serialize()
                    );
            });
        }
        public async Task SendBattleOffTask(GameClient client, int attackerHandler, bool dungeon)
        {
            await Task.Run(async () =>
            {
                Thread.Sleep(4000);

                _dungeonServer.BroadcastForTamerViewsAndSelf(
                        client.TamerId,
                        new SetCombatOffPacket(attackerHandler).Serialize()
                    );
            });
        }

        private void ApplyDigimonSkillEffects(
            GameClient client,
            DigimonSkillAssetModel skill,
            IEnumerable<MobConfigModel> targetMobs,
            byte skillSlot,
            bool dungeonBroadcast)
        {
            if (client?.Partner == null || skill?.SkillInfo == null)
                return;

            var buffInfos = _assets.BuffInfo
                .Where(buff => buff.SkillId == skill.SkillId && buff.SkillInfo?.Apply != null)
                .ToList();

            if (!buffInfos.Any())
                return;

            var skillLevel = GetCurrentSkillLevel(client, skillSlot);
            var typeN = (short)Math.Clamp(skillLevel - 1, 0, short.MaxValue);

            foreach (var buffInfo in buffInfos)
            {
                var skillCodeInfo = buffInfo.SkillInfo;
                if (skillCodeInfo?.Apply == null)
                    continue;

                var effectApply = GetSecondaryEffectApply(skillCodeInfo);
                if (effectApply == null || !RollSkillEffect(effectApply))
                    continue;

                var duration = GetSkillEffectDuration(skill.SkillInfo, effectApply, skillLevel);
                if (duration <= 0)
                    continue;

                if (buffInfo.Type == 1)
                {
                    ApplyPartnerSkillBuff(client, buffInfo, typeN, duration, dungeonBroadcast);
                }
                else if (buffInfo.Type == 2)
                {
                    foreach (var targetMob in targetMobs.Where(mob => mob != null && mob.Alive))
                        ApplyMobSkillDebuff(client, targetMob, buffInfo, typeN, duration, dungeonBroadcast);
                }
            }
        }

        private void ApplyPartnerSkillBuff(
            GameClient client,
            BuffInfoAssetModel buffInfo,
            short typeN,
            int duration,
            bool dungeonBroadcast)
        {
            var existing = client.Partner.BuffList.ActiveBuffs
                .FirstOrDefault(buff => buff.BuffId == buffInfo.BuffId || buff.SkillId == buffInfo.SkillId);

            if (existing != null)
            {
                client.Partner.BuffList.Buffs.Remove(existing);
                var removePacket = new RemoveBuffPacket(client.Partner.GeneralHandler, existing.BuffId).Serialize();
                if (dungeonBroadcast)
                    _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, removePacket);
                else
                    _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, removePacket);
            }

            var newBuff = DigimonBuffModel.Create(buffInfo.BuffId, buffInfo.SkillId, typeN, duration);
            newBuff.SetBuffInfo(buffInfo);
            client.Partner.BuffList.Add(newBuff);

            var addPacket = new AddBuffPacket(client.Partner.GeneralHandler, buffInfo.BuffId, buffInfo.SkillId, typeN, duration).Serialize();
            if (dungeonBroadcast)
                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, addPacket);
            else
                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, addPacket);

            client.Send(new UpdateStatusPacket(client.Tamer).Serialize());
            _sender.Send(new UpdateDigimonBuffListCommand(client.Partner.BuffList));
        }

        private void ApplyMobSkillDebuff(
            GameClient client,
            MobConfigModel targetMob,
            BuffInfoAssetModel buffInfo,
            short typeN,
            int duration,
            bool dungeonBroadcast)
        {
            var existing = targetMob.DebuffList.ActiveBuffs.FirstOrDefault(buff => buff.BuffId == buffInfo.BuffId);
            if (existing != null)
            {
                existing.IncreaseEndDate(duration);
            }
            else
            {
                var newDebuff = MobDebuffModel.Create(buffInfo.BuffId, buffInfo.SkillId, typeN, duration);
                newDebuff.SetBuffInfo(buffInfo);
                targetMob.DebuffList.Buffs.Add(newDebuff);
            }

            if (buffInfo.SkillInfo?.Apply.Any(apply => apply.Attribute == SkillCodeApplyAttributeEnum.CrowdControl) == true &&
                targetMob.CurrentAction != Commons.Enums.Map.MobActionEnum.CrowdControl)
            {
                targetMob.UpdateCurrentAction(Commons.Enums.Map.MobActionEnum.CrowdControl);
            }

            var addPacket = new AddStunDebuffPacket(targetMob.GeneralHandler, buffInfo.BuffId, buffInfo.SkillId, duration).Serialize();
            if (dungeonBroadcast)
                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, addPacket);
            else
                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, addPacket);
        }

        private static SkillCodeApplyAssetModel? GetSecondaryEffectApply(SkillCodeAssetModel skillInfo)
        {
            return skillInfo.Apply.FirstOrDefault(apply =>
                apply.Type != SkillCodeApplyTypeEnum.None &&
                apply.Type != SkillCodeApplyTypeEnum.Unknown1 &&
                apply.Type != SkillCodeApplyTypeEnum.Unknown2 &&
                apply.Type != SkillCodeApplyTypeEnum.Unknown10 &&
                apply.Attribute != SkillCodeApplyAttributeEnum.Unknown);
        }

        private static bool RollSkillEffect(SkillCodeApplyAssetModel apply)
        {
            if (apply.Chance <= 0)
                return false;

            if (apply.Chance >= 10000)
                return true;

            return UtilitiesFunctions.RandomInt(1, 10000) <= apply.Chance;
        }

        private static int GetSkillEffectDuration(
            SkillInfoAssetModel skillInfo,
            SkillCodeApplyAssetModel apply,
            int skillLevel)
        {
            var duration = skillInfo.EffectDuration;
            if (duration <= 0)
                duration = apply.AdditionalValue;

            if (apply.Type == SkillCodeApplyTypeEnum.Unknown208)
                duration += Math.Max(0, skillLevel - 1) * apply.IncreaseValue;

            return Math.Max(0, duration);
        }

        private static int GetCurrentSkillLevel(GameClient client, byte skillSlot)
        {
            var evolution = client.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Partner.CurrentType);
            if (evolution?.Skills != null && skillSlot < evolution.Skills.Count)
                return Math.Max(1, (int)evolution.Skills[skillSlot].CurrentLevel);

            return 1;
        }

        private static int DebuffReductionDamage(GameClient client, int finalDmg)
        {
            if (client.Tamer.Partner.DebuffList.ActiveDebuffReductionDamage())
            {
                var debuffInfo = client.Tamer.Partner.DebuffList.ActiveBuffs
                .Where(buff => buff.BuffInfo.SkillInfo.Apply
                    .Any(apply => apply.Attribute == Commons.Enums.SkillCodeApplyAttributeEnum.AttackPowerDown))

                .ToList();

                var totalValue = 0;
                var SomaValue = 0;

                foreach (var debuff in debuffInfo)
                {
                    foreach (var apply in debuff.BuffInfo.SkillInfo.Apply)
                    {

                        switch (apply.Type)
                        {
                            case SkillCodeApplyTypeEnum.Default:
                                totalValue += apply.Value;
                                break;

                            case SkillCodeApplyTypeEnum.AlsoPercent:
                            case SkillCodeApplyTypeEnum.Percent:
                                {

                                    SomaValue += apply.Value + (debuff.TypeN) * apply.IncreaseValue;

                                    double fatorReducao = SomaValue / 100;

                                    // Calculando o novo finalDmg após a redução
                                    finalDmg -= (int)(finalDmg * fatorReducao);

                                }
                                break;

                            case SkillCodeApplyTypeEnum.Unknown200:
                                {

                                    SomaValue += apply.AdditionalValue;

                                    double fatorReducao = SomaValue / 100.0;

                                    // Calculando o novo finalDmg após a redução
                                    finalDmg -= (int)(finalDmg * fatorReducao);

                                }
                                break;

                        }
                        break;
                    
                    }
                }
            }

            return finalDmg;
        }
        private int CalculateDamageOrHeal(GameClient client, MobConfigModel? targetMob, DigimonSkillAssetModel? targetSkill, SkillCodeAssetModel? skill, byte skillSlot)
        {
            if (targetMob == null || client?.Tamer?.Partner == null)
                return 0;

            double SkillValueVal = 0;
            double SkillValueInc = 0;
            int skillLevel = 0;

            if (skill?.Apply != null)
            {
                var sv = skill.Apply.FirstOrDefault(x => x.Type > 0);
                if (sv != null)
                {
                    SkillValueVal = sv.Value;
                    SkillValueInc = sv.IncreaseValue;
                }
            }

            var evolution = client.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Partner.CurrentType);
            if (evolution?.Skills != null && skillSlot < evolution.Skills.Count)
                skillLevel = evolution.Skills[skillSlot].CurrentLevel;

            double f1BaseDamage = SkillValueVal + (skillLevel * SkillValueInc);
            double SkillFactor = 0;

            var Percentual = (decimal)client.Partner.SCD / 100;
            SkillFactor = (double)Percentual;

            double factorFromPF = 144.0 / (client.Tamer?.Partner?.Digiclone?.ATValue ?? 1);
            double cloneFactor = Math.Round(1.0 + (0.43 / factorFromPF), 2);
            f1BaseDamage = Math.Floor(f1BaseDamage * cloneFactor);

            double addedf1Damage = Math.Floor(f1BaseDamage * SkillFactor / 100.0);

            if (_damageFormulaConfig.Enable && client.Tamer.Partner.BaseInfo != null && skill != null)
            {
                var input = CreateDamageFormulaInput(
                    client,
                    targetMob.Attribute,
                    targetMob.Element,
                    targetMob.GeneralHandler,
                    (int)skill.SkillCode,
                    (int)Math.Floor(f1BaseDamage));

                var result = DamageFormula.CalculateDamage(input, _damageFormulaConfig);
                LogDamageFormula(input, result);
                return result.FinalDamage;
            }

            var Damage = (int)Math.Floor(f1BaseDamage + addedf1Damage + client.Tamer.Partner.AT + client.Tamer.Partner.SKD);
            Damage = ApplySkillDamagePercentBonus(
                Damage,
                client.Tamer.Partner.SkillDamagePercentValueAgainst(targetMob.Attribute, targetMob.Element));

            if (client.Tamer.Partner.BaseInfo == null)
            {
                var finalDamage = (int)(Damage * (1.0 + new Random().NextDouble() * 0.05));
                return ApplyFinalDamageBonus(finalDamage, client.Tamer.Partner.FinalDamageBasisPoints);
            }

            var matrixAdjustedDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                Damage,
                client.Tamer.Partner.BaseInfo.Attribute,
                targetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                targetMob.Element);

            Random random = new Random();
            double percentagemBonus = random.NextDouble() * 0.05;
            return ApplyFinalDamageBonus((int)(matrixAdjustedDamage * (1.0 + percentagemBonus)), client.Tamer.Partner.FinalDamageBasisPoints);
        }
        private int CalculateDamageOrHeal(GameClient client, SummonMobModel? targetMob, DigimonSkillAssetModel? targetSkill, SkillCodeAssetModel? skill, byte skillSlot)
        {
            if (targetMob == null || client?.Tamer?.Partner == null)
                return 0;

            double SkillValueVal = 0;
            double SkillValueInc = 0;
            int skillLevel = 0;

            if (skill?.Apply != null)
            {
                var sv = skill.Apply.FirstOrDefault(x => x.Type > 0);
                if (sv != null)
                {
                    SkillValueVal = sv.Value;
                    SkillValueInc = sv.IncreaseValue;
                }
            }

            var evolution = client.Partner.Evolutions.FirstOrDefault(x => x.Type == client.Partner.CurrentType);
            if (evolution?.Skills != null && skillSlot < evolution.Skills.Count)
                skillLevel = evolution.Skills[skillSlot].CurrentLevel;

            double f1BaseDamage = SkillValueVal + (skillLevel * SkillValueInc);
            double SkillFactor = 0;

            var Percentual = (decimal)client.Partner.SCD / 100;
            SkillFactor = (double)Percentual;

            double factorFromPF = 144.0 / (client.Tamer?.Partner?.Digiclone?.ATValue ?? 1);
            double cloneFactor = Math.Round(1.0 + (0.43 / factorFromPF), 2);
            f1BaseDamage = Math.Floor(f1BaseDamage * cloneFactor);

            double addedf1Damage = Math.Floor(f1BaseDamage * SkillFactor / 100.0);

            if (_damageFormulaConfig.Enable && client.Tamer.Partner.BaseInfo != null && skill != null)
            {
                var input = CreateDamageFormulaInput(
                    client,
                    targetMob.Attribute,
                    targetMob.Element,
                    targetMob.GeneralHandler,
                    (int)skill.SkillCode,
                    (int)Math.Floor(f1BaseDamage));

                var result = DamageFormula.CalculateDamage(input, _damageFormulaConfig);
                LogDamageFormula(input, result);
                return result.FinalDamage;
            }

            var Damage = (int)Math.Floor(f1BaseDamage + addedf1Damage + client.Tamer.Partner.AT + client.Tamer.Partner.SKD);
            Damage = ApplySkillDamagePercentBonus(
                Damage,
                client.Tamer.Partner.SkillDamagePercentValueAgainst(targetMob.Attribute, targetMob.Element));

            if (client.Tamer.Partner.BaseInfo == null)
            {
                var finalDamage = (int)(Damage * (1.0 + new Random().NextDouble() * 0.05));
                return ApplyFinalDamageBonus(finalDamage, client.Tamer.Partner.FinalDamageBasisPoints);
            }

            var matrixAdjustedDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                Damage,
                client.Tamer.Partner.BaseInfo.Attribute,
                targetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                targetMob.Element);

            Random random = new Random();
            double percentagemBonus = random.NextDouble() * 0.05;
            return ApplyFinalDamageBonus((int)(matrixAdjustedDamage * (1.0 + percentagemBonus)), client.Tamer.Partner.FinalDamageBasisPoints);
        }

        private static int ApplyFinalDamageBonus(int baseDamage, int basisPoints)
        {
            if (baseDamage <= 0 || basisPoints == 0)
                return baseDamage;

            long scaled = (long)baseDamage * (10000L + basisPoints);
            long adjusted = scaled / 10000L;

            if (adjusted > int.MaxValue) return int.MaxValue;
            if (adjusted < int.MinValue) return int.MinValue;
            return (int)adjusted;
        }

        private static DamageFormulaConfig LoadDamageFormulaConfig(IConfiguration configuration)
        {
            return new DamageFormulaConfig
            {
                Enable = configuration.GetValue("DamageFormula:Enable", true),
                EnableDamageFormulaLog = configuration.GetValue("DamageFormula:EnableLog", false),
                CritBaseRate = configuration.GetValue("DamageFormula:CritBaseRate", 1.0),
                ApplyAttributeToCriticalExtra = configuration.GetValue("DamageFormula:ApplyAttributeToCriticalExtra", false),
                ApplyAttackToSkill = configuration.GetValue("DamageFormula:ApplyAttackToSkill", true),
                ApplyAttributeToSkillFlat = configuration.GetValue("DamageFormula:ApplyAttributeToSkillFlat", false),
                ApplyFinalDamageToSkill = configuration.GetValue("DamageFormula:ApplyFinalDamageToSkill", true),
                ApplyElementDamage = configuration.GetValue("DamageFormula:ApplyElementDamage", true)
            };
        }

        private DamageFormulaInput CreateDamageFormulaInput(
            GameClient client,
            DigimonAttributeEnum targetAttribute,
            DigimonElementEnum targetElement,
            int targetHandler,
            int skillId,
            int skillBaseDamage)
        {
            var partner = client.Tamer.Partner;
            var attackerAttribute = partner.BaseInfo.Attribute;
            var attackerElement = partner.BaseInfo.Element;
            var elementPercent = Math.Max(0, attackerElement.GetElementDelta(targetElement));
            var hasAttributeAdvantage = attackerAttribute.HasAttributeAdvantage(targetAttribute);
            var hasElementAdvantage = elementPercent > 0;

            return new DamageFormulaInput
            {
                Attack = partner.AT,
                ExtraAttack = 0,
                SkillBaseDamage = skillBaseDamage,
                SkillDamageFlat = partner.SKD,
                AttributePercent = partner.ATT,
                ElementPercent = elementPercent,
                SkillDamagePercent = partner.SkillDamagePercentValueForAdvantage(
                    hasAttributeAdvantage,
                    hasElementAdvantage),
                CriticalDamageExtraPercent = 0,
                FinalDamagePercent = partner.FinalDamageBasisPoints / 100.0,
                TargetReductionPercent = 0,
                HasAttributeAdvantage = hasAttributeAdvantage,
                HasElementAdvantage = hasElementAdvantage,
                IsCritical = false,
                IsSkill = true,
                AttackerIndex = partner.GeneralHandler,
                TargetIndex = targetHandler,
                SkillId = skillId
            };
        }

        private void LogDamageFormula(DamageFormulaInput input, DamageFormulaResult result)
        {
            if (_damageFormulaConfig.EnableDamageFormulaLog)
                _logger.Information(DamageFormula.CreateLogMessage(input, result));
        }

        private static int ApplySkillDamagePercentBonus(int baseDamage, double percentPoints)
        {
            if (baseDamage <= 0 || percentPoints == 0)
                return baseDamage;

            var adjusted = Math.Floor(baseDamage * (100.0 + percentPoints) / 100.0);

            if (adjusted > int.MaxValue) return int.MaxValue;
            if (adjusted < int.MinValue) return int.MinValue;
            return (int)adjusted;
        }

        private static bool IsImplosionSkill(SkillInfoAssetModel skillInfo)
        {
            const byte DigimonAroundTarget = 17;
            const byte Field = 61;
            const byte AroundTamer = 62;
            const byte AroundDigimon = 63;
            const byte Rush = 80;

            return skillInfo.AreaOfEffect > 0 &&
                (skillInfo.Target == DigimonAroundTarget ||
                 skillInfo.Target == Field ||
                 skillInfo.Target == AroundTamer ||
                 skillInfo.Target == AroundDigimon ||
                 skillInfo.Target == Rush);
        }

        private static bool IsTargetAreaSkill(SkillInfoAssetModel skillInfo)
        {
            const byte AoEOnTargetMonster = 18;
            const byte PartyMonsters = 52;

            return skillInfo.AreaOfEffect > 0 &&
                (skillInfo.Target == AoEOnTargetMonster ||
                 skillInfo.Target == PartyMonsters);
        }
    }
}
