using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Combat;
using DigitalWorldOnline.Commons.Models;
using DigitalWorldOnline.Commons.Models.Config;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Models.Summon;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.GameHost;
using Microsoft.Extensions.Configuration;
using Serilog;
using System;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class PartnerAttackPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartnerAttack;

        private readonly MapServer _mapServer;
        private readonly PvpServer _pvpServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly ILogger _logger;
        private readonly DamageFormulaConfig _damageFormulaConfig;

        public PartnerAttackPacketProcessor(
            MapServer mapServer,
            PvpServer pvpServer,
            ILogger logger,
            DungeonsServer dungeonsServer,
            IConfiguration configuration)
        {
            _mapServer = mapServer;
            _pvpServer = pvpServer;
            _dungeonServer = dungeonsServer;
            _logger = logger;
            _damageFormulaConfig = LoadDamageFormulaConfig(configuration);
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var attackerHandler = packet.ReadInt();
            var targetHandler = packet.ReadInt();

            // The client sends handlers with CType type bits encoded
            // (type << 19 | class << 14 | idx), but the server's internal
            // handlers only use the lower 19 bits. Strip the type bits so
            // GetMobByHandler / GetEnemyByHandler lookups succeed.
            // Keep the originals for response packets (client needs the full handler).
            const int CTypeClassIdxMask = 0x7FFFF;
            var lookupAttacker = attackerHandler & CTypeClassIdxMask;
            var lookupTarget = targetHandler & CTypeClassIdxMask;

            _logger.Information("Attack packet: handler={Handler}(0x{Handler:X}) mask={MaskedHandler}(0x{MaskedHandler:X}) target={Target}(0x{Target:X}) mask={MaskedTarget}(0x{MaskedTarget:X}) tamer={TamerId} map={MapId} currentType={CurrentType}",
                attackerHandler, lookupAttacker, targetHandler, lookupTarget, client.TamerId, client.Tamer.Location.MapId, client.Partner?.CurrentType);

            // Reply packets must use the same class+index handler announced in InitialInfoPacket.
            // Newer client builds may send CType.m_nTypeAll here (for example 0x84001 for
            // partner 0x4001); echoing that value makes the client ignore hit animations.
            attackerHandler = lookupAttacker;
            targetHandler = lookupTarget;

            if (client.PvpMap)
            {
                var targetPartner = _pvpServer.GetEnemyByHandler(client.Tamer.Location.MapId, lookupTarget);

                if (targetPartner == null || client.Partner == null)
                    return Task.CompletedTask;

                if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                    client.Partner.StartAutoAttack();

                if (targetPartner.Alive)
                {
                    if (client.Partner.IsAttacking)
                    {
                        if (client.Tamer.TargetMob?.GeneralHandler != targetPartner.GeneralHandler)
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} switched target to partner {targetPartner.Id} - {targetPartner.Name}.");
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTarget(targetPartner);
                            client.Partner.StartAutoAttack();
                        }
                    }
                    else
                    {
                        if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                        {
                            client.Partner.StartAutoAttack();
                            return Task.CompletedTask;
                        }

                        client.Partner.SetEndAttacking();

                        if (!client.Tamer.InBattle)
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged partner {targetPartner.Id} - {targetPartner.Name}.");
                            client.Tamer.SetHidden(false);

                            _pvpServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattle(targetPartner);
                        }
                        else
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} switched to partner {targetPartner.Id} - {targetPartner.Name}.");
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTarget(targetPartner);
                        }

                        if (!targetPartner.Character.InBattle)
                        {
                            _pvpServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                        }

                        targetPartner.Character.StartBattle(client.Partner);

                        client.Tamer.Partner.StartAutoAttack();

                        var missed = false;

                        if (client.Partner.Level <= targetPartner.Level)
                        {
                            missed = client.Tamer.CanMissHit();
                        }

                        if (missed)
                        {
                            _logger.Verbose($"Partner {client.Tamer.Partner.Id} missed hit on {client.Tamer.TargetPartner.Id} - {client.Tamer.TargetPartner.Name}.");
                            _pvpServer.BroadcastForTamerViewsAndSelf(client.TamerId, new MissHitPacket(attackerHandler, targetHandler).Serialize());
                        }
                        else
                        {
                            #region Hit Damage
                            var critBonusMultiplier = 0.00;
                            var blocked = false;
                            var finalDmg = CalculateFinalDamage(client, targetPartner, out critBonusMultiplier, out blocked);

                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }
                            #endregion

                            #region Take Damage
                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetPartner.CurrentHp) finalDmg = targetPartner.CurrentHp;

                            var newHp = targetPartner.ReceiveDamage(finalDmg);

                            var hitType = blocked ? 2 : critBonusMultiplier > 0 ? 1 : 0;

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} to partner {targetPartner?.Id} - {targetPartner?.Name}.");

                                _pvpServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new HitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        targetPartner.HP,
                                        newHp,
                                        hitType).Serialize());
                            }
                            else
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} killed partner {targetPartner?.Id} - {targetPartner?.Name} with {finalDmg} damage.");

                                _pvpServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnHitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        hitType).Serialize());

                                targetPartner.Character.Die();

                                if (!_pvpServer.EnemiesAttacking(client.Tamer.Location.MapId, client.Partner.Id))
                                {
                                    client.Tamer.StopBattle();

                                    _pvpServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new SetCombatOffPacket(attackerHandler).Serialize());
                                }
                            }
                            #endregion
                        }

                        client.Tamer.Partner.UpdateLastHitTime();
                    }
                }
                else
                {
                    if (!_pvpServer.EnemiesAttacking(client.Tamer.Location.MapId, client.Partner.Id))
                    {
                        client.Tamer.StopBattle();

                        _pvpServer.BroadcastForTamerViewsAndSelf(
                            client.TamerId,
                            new SetCombatOffPacket(attackerHandler).Serialize());
                    }
                }
            }
            else if (client.DungeonMap)
            {
                if (_dungeonServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget, true, client.TamerId) != null) //Summon
                {
                    var targetMob = _dungeonServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget, true, client.TamerId);

                    if (targetMob == null || client.Partner == null)
                        return Task.CompletedTask;

                    if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                        client.Partner.StartAutoAttack();

                    if (targetMob.Alive)
                    {
                        if (client.Partner.IsAttacking)
                        {
                            if (client.Tamer.TargetMob?.GeneralHandler != targetMob.GeneralHandler)
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} switched target to {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);
                                client.Tamer.UpdateTarget(targetMob);
                                client.Partner.StartAutoAttack();
                            }
                        }
                        else
                        {
                            if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                            {
                                client.Partner.StartAutoAttack();
                                return Task.CompletedTask;
                            }

                            client.Partner.SetEndAttacking();

                            if (!client.Tamer.InBattle)
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);

                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                                client.Tamer.StartBattle(targetMob);
                            }
                            else
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} switched to {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);
                                client.Tamer.UpdateTarget(targetMob);
                            }

                            if (!targetMob.InBattle)
                            {
                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            client.Tamer.Partner.StartAutoAttack();

                            var missed = false;

                            if (!client.Tamer.GodMode)
                            {
                                missed = client.Tamer.CanMissHit(true);
                            }

                            if (missed)
                            {
                                _logger.Verbose($"Partner {client.Tamer.Partner.Id} missed hit on {client.Tamer.TargetSummonMob.Id} - {client.Tamer.TargetSummonMob.Name}.");
                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new MissHitPacket(attackerHandler, targetHandler).Serialize());
                            }
                            else
                            {
                                #region Hit Damage
                                var critBonusMultiplier = 0.00;
                                var blocked = false;
                                var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateFinalDamage(client, targetMob, out critBonusMultiplier, out blocked);

                                if (finalDmg != 0 && !client.Tamer.GodMode)
                                {
                                    finalDmg = DebuffReductionDamage(client, finalDmg);
                                }
                                #endregion

                                #region Take Damage
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                                var hitType = blocked ? 2 : critBonusMultiplier > 0 ? 1 : 0;

                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} to mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}).");

                                    _dungeonServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new HitPacket(
                                            attackerHandler,
                                            targetHandler,
                                            finalDmg,
                                            targetMob.HPValue,
                                            newHp,
                                            hitType).Serialize());
                                }
                                else
                                {
                                    client.Partner.SetEndAttacking(client.Partner.AS * -2);

                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}) with {finalDmg} damage.");

                                    _dungeonServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new KillOnHitPacket(
                                            attackerHandler,
                                            targetHandler,
                                            finalDmg,
                                            hitType).Serialize());

                                    targetMob?.Die();

                                    if (!_dungeonServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                                    {
                                        client.Tamer.StopBattle(true);

                                        _dungeonServer.BroadcastForTamerViewsAndSelf(
                                            client.TamerId,
                                            new SetCombatOffPacket(attackerHandler).Serialize());
                                    }
                                }
                                #endregion
                            }

                            client.Tamer.Partner.UpdateLastHitTime();
                        }
                    }
                    else
                    {
                        if (!_dungeonServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId, true))
                        {
                            client.Tamer.StopBattle(true);

                            _dungeonServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new SetCombatOffPacket(attackerHandler).Serialize());
                        }
                    }
                }
                else
                {
                    var targetMob = _dungeonServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget, client.TamerId);

                    if (targetMob == null || client.Partner == null)
                        return Task.CompletedTask;

                    if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                        client.Partner.StartAutoAttack();

                    if (targetMob.Alive)
                    {
                        if (client.Partner.IsAttacking)
                        {
                            if (client.Tamer.TargetMob?.GeneralHandler != targetMob.GeneralHandler)
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} switched target to {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);
                                client.Tamer.UpdateTarget(targetMob);
                                client.Partner.StartAutoAttack();
                            }
                        }
                        else
                        {
                            if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                            {
                                client.Partner.StartAutoAttack();
                                return Task.CompletedTask;
                            }

                            client.Partner.SetEndAttacking();

                            if (!client.Tamer.InBattle)
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);

                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                                client.Tamer.StartBattle(targetMob);
                            }
                            else
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} switched to {targetMob.Id} - {targetMob.Name}.");
                                client.Tamer.SetHidden(false);
                                client.Tamer.UpdateTarget(targetMob);
                            }

                            if (!targetMob.InBattle)
                            {
                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            client.Tamer.Partner.StartAutoAttack();

                            var missed = false;

                            if (!client.Tamer.GodMode)
                            {
                                missed = client.Tamer.CanMissHit();
                            }

                            if (missed)
                            {
                                _logger.Verbose($"Partner {client.Tamer.Partner.Id} missed hit on {client.Tamer.TargetMob.Id} - {client.Tamer.TargetMob.Name}.");
                                _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new MissHitPacket(attackerHandler, targetHandler).Serialize());
                            }
                            else
                            {
                                #region Hit Damage
                                var critBonusMultiplier = 0.00;
                                var blocked = false;
                                var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateFinalDamage(client, targetMob, out critBonusMultiplier, out blocked);

                                if (finalDmg != 0 && !client.Tamer.GodMode)
                                {
                                    finalDmg = DebuffReductionDamage(client, finalDmg);
                                }
                                #endregion

                                #region Take Damage
                                if (finalDmg <= 0) finalDmg = 1;
                                if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                                var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                                var hitType = blocked ? 2 : critBonusMultiplier > 0 ? 1 : 0;

                                if (newHp > 0)
                                {
                                    _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} to mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}).");

                                    _dungeonServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new HitPacket(
                                            attackerHandler,
                                            targetHandler,
                                            finalDmg,
                                            targetMob.HPValue,
                                            newHp,
                                            hitType).Serialize());
                                }
                                else
                                {
                                    client.Partner.SetEndAttacking(client.Partner.AS * -2);

                                    _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}) with {finalDmg} damage.");

                                    _dungeonServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new KillOnHitPacket(
                                            attackerHandler,
                                            targetHandler,
                                            finalDmg,
                                            hitType).Serialize());

                                    targetMob?.Die();

                                    if (!_dungeonServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                                    {
                                        client.Tamer.StopBattle();

                                        _dungeonServer.BroadcastForTamerViewsAndSelf(
                                            client.TamerId,
                                            new SetCombatOffPacket(attackerHandler).Serialize());
                                    }
                                }
                                #endregion
                            }

                            client.Tamer.Partner.UpdateLastHitTime();
                        }
                    }
                    else
                    {
                        if (!_dungeonServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                        {
                            client.Tamer.StopBattle();

                            _mapServer.BroadcastForTamerViewsAndSelf(
                                client.TamerId,
                                new SetCombatOffPacket(attackerHandler).Serialize());
                        }
                    }
                }

            }
            else if (_mapServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget, true) != null) //Summon
            {
                var targetMob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget, true);

                if (targetMob == null || client.Partner == null)
                    return Task.CompletedTask;

                if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                    client.Partner.StartAutoAttack();

                if (targetMob.Alive)
                {
                    if (client.Partner.IsAttacking)
                    {
                        if (client.Tamer.TargetSummonMob?.GeneralHandler != targetMob.GeneralHandler)
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} switched target to {targetMob.Id} - {targetMob.Name}.");
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTarget(targetMob);
                            client.Partner.StartAutoAttack();
                        }
                    }
                    else
                    {
                        if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                        {
                            client.Partner.StartAutoAttack();
                            return Task.CompletedTask;
                        }

                        client.Partner.SetEndAttacking();

                        if (!client.Tamer.InBattle)
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name}.");
                            client.Tamer.SetHidden(false);

                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                            client.Tamer.StartBattle(targetMob);
                        }
                        else
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} switched to {targetMob.Id} - {targetMob.Name}.");
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTarget(targetMob);
                        }

                        if (!targetMob.InBattle)
                        {
                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                            targetMob.StartBattle(client.Tamer);
                        }
                        else
                        {
                            targetMob.AddTarget(client.Tamer);
                        }

                        client.Tamer.Partner.StartAutoAttack();

                        var missed = false;

                        if (!client.Tamer.GodMode)
                        {
                            missed = client.Tamer.CanMissHit(true);
                        }

                        if (missed)
                        {
                            _logger.Verbose($"Partner {client.Tamer.Partner.Id} missed hit on {client.Tamer.TargetSummonMob.Id} - {client.Tamer.TargetSummonMob.Name}.");
                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new MissHitPacket(attackerHandler, targetHandler).Serialize());
                        }
                        else
                        {
                            #region Hit Damage
                            var critBonusMultiplier = 0.00;
                            var blocked = false;
                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateFinalDamage(client, targetMob, out critBonusMultiplier, out blocked);

                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }
                            #endregion

                            #region Take Damage
                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            var hitType = blocked ? 2 : critBonusMultiplier > 0 ? 1 : 0;

                            if (newHp > 0)
                            {
                                _logger.Verbose($"Partner {client.Partner.Id} inflicted {finalDmg} to mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}).");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new HitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        targetMob.HPValue,
                                        newHp,
                                        hitType).Serialize());
                            }
                            else
                            {
                                client.Partner.SetEndAttacking(client.Partner.AS * -2);

                                _logger.Verbose($"Partner {client.Partner.Id} killed mob {targetMob?.Id} - {targetMob?.Name}({targetMob?.Type}) with {finalDmg} damage.");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnHitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        hitType).Serialize());

                                targetMob?.Die();

                                if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                                {
                                    client.Tamer.StopBattle(true);

                                    _mapServer.BroadcastForTamerViewsAndSelf(
                                        client.TamerId,
                                        new SetCombatOffPacket(attackerHandler).Serialize());
                                }
                            }
                            #endregion
                        }

                        client.Tamer.Partner.UpdateLastHitTime();
                    }
                }
                else
                {
                    if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId, true))
                    {
                        client.Tamer.StopBattle(true);

                        _mapServer.BroadcastForTamerViewsAndSelf(
                            client.TamerId,
                            new SetCombatOffPacket(attackerHandler).Serialize());
                    }
                }
            }
            else
            {
                var targetMob = _mapServer.GetMobByHandler(client.Tamer.Location.MapId, lookupTarget);

                if (targetMob == null || client.Partner == null)
                {
                    _logger.Information("Attack ignored: targetMob={HasTargetMob} partner={HasPartner} targetHandler={TargetHandler} maskedTarget={MaskedTarget} tamer={TamerId} map={MapId}",
                        targetMob != null, client.Partner != null, targetHandler, lookupTarget, client.TamerId, client.Tamer.Location.MapId);
                    return Task.CompletedTask;
                }

                if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                    client.Partner.StartAutoAttack();

                if (targetMob.Alive)
                {
                    if (client.Partner.IsAttacking)
                    {
                        if (client.Tamer.TargetMob?.GeneralHandler != targetMob.GeneralHandler)
                        {
                            _logger.Verbose($"Character {client.Tamer.Id} switched target to {targetMob.Id} - {targetMob.Name}.");
                            client.Tamer.SetHidden(false);
                            client.Tamer.UpdateTarget(targetMob);
                            client.Partner.StartAutoAttack();
                        }
                    }
                    else
                    {
                        if (DateTime.Now < client.Partner.LastHitTime.AddMilliseconds(client.Partner.AS))
                        {
                            client.Partner.StartAutoAttack();
                            return Task.CompletedTask;
                        }

                            client.Partner.SetEndAttacking();

                            if (!client.Tamer.InBattle)
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} engaged {targetMob.Id} - {targetMob.Name} (atkHandler={attackerHandler}/0x{attackerHandler:X} targetHandler={targetHandler}/0x{targetHandler:X}).");
                                client.Tamer.SetHidden(false);

                                _logger.Verbose($">>> SetCombatON attacker: handler={attackerHandler}/0x{attackerHandler:X} tamer={client.TamerId}");
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(attackerHandler).Serialize());
                                client.Tamer.StartBattle(targetMob);
                            }
                            else
                            {
                                _logger.Verbose($"Character {client.Tamer.Id} switched to {targetMob.Id} - {targetMob.Name} (targetHandler={targetHandler}/0x{targetHandler:X}).");
                                client.Tamer.SetHidden(false);
                                client.Tamer.UpdateTarget(targetMob);
                            }

                            if (!targetMob.InBattle)
                            {
                                _logger.Verbose($">>> SetCombatON target: handler={targetHandler}/0x{targetHandler:X} tamer={client.TamerId}");
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new SetCombatOnPacket(targetHandler).Serialize());
                                targetMob.StartBattle(client.Tamer);
                            }
                            else
                            {
                                targetMob.AddTarget(client.Tamer);
                            }

                            client.Tamer.Partner.StartAutoAttack();

                            var missed = false;

                            if (!client.Tamer.GodMode)
                            {
                                missed = client.Tamer.CanMissHit();
                            }

                            if (missed)
                            {
                                _logger.Verbose($">>> SEND MissHit: atk={attackerHandler}/0x{attackerHandler:X} target={targetHandler}/0x{targetHandler:X} tamer={client.TamerId} mob={client.Tamer.TargetMob?.Name}");
                                _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new MissHitPacket(attackerHandler, targetHandler).Serialize());
                            }
                        else
                        {
                            #region Hit Damage
                            var critBonusMultiplier = 0.00;
                            var blocked = false;
                            var finalDmg = client.Tamer.GodMode ? targetMob.CurrentHP : CalculateFinalDamage(client, targetMob, out critBonusMultiplier, out blocked);

                            if (finalDmg != 0 && !client.Tamer.GodMode)
                            {
                                finalDmg = DebuffReductionDamage(client, finalDmg);
                            }
                            #endregion

                            #region Take Damage
                            if (finalDmg <= 0) finalDmg = 1;
                            if (finalDmg > targetMob.CurrentHP) finalDmg = targetMob.CurrentHP;

                            var newHp = targetMob.ReceiveDamage(finalDmg, client.TamerId);

                            var hitType = blocked ? 2 : critBonusMultiplier > 0 ? 1 : 0;

                            if (newHp > 0)
                            {
                                _logger.Verbose($">>> SEND HitPacket: atk={attackerHandler}/0x{attackerHandler:X} target={targetHandler}/0x{targetHandler:X} dmg={finalDmg} hp={newHp}/{targetMob.HPValue} type={hitType} tamer={client.TamerId} mob={targetMob?.Id}-{targetMob?.Name}");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new HitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        targetMob.HPValue,
                                        newHp,
                                        hitType).Serialize());
                            }
                            else
                            {
                                client.Partner.SetEndAttacking(client.Partner.AS * -2);

                                _logger.Verbose($">>> SEND KillOnHit: atk={attackerHandler}/0x{attackerHandler:X} target={targetHandler}/0x{targetHandler:X} dmg={finalDmg} type={hitType} tamer={client.TamerId} mob={targetMob?.Id}-{targetMob?.Name}");

                                _mapServer.BroadcastForTamerViewsAndSelf(
                                    client.TamerId,
                                    new KillOnHitPacket(
                                        attackerHandler,
                                        targetHandler,
                                        finalDmg,
                                        hitType).Serialize());

                                targetMob?.Die();

                                if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                                {
                                    client.Tamer.StopBattle();

                                    _ = SendBattleOffTask(client, attackerHandler, client.DungeonMap);
                                }
                            }
                            #endregion

                            client.Tamer.Partner.UpdateLastHitTime();
                        }
                    }
                }
                else
                {
                    if (!_mapServer.MobsAttacking(client.Tamer.Location.MapId, client.TamerId))
                    {
                        client.Tamer.StopBattle();

                        _ = SendBattleOffTask(client, attackerHandler, client.DungeonMap);
                    }
                }
            }

            return Task.CompletedTask;
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
                    }
                }
            }

            return finalDmg;
        }

        //public static int GetCurrentDamage(GameClient client, MobConfigModel monster)
        //{
        //    var digimon = client.Partner;
        //    var ResultAT = digimon.AT;
        //
        //    var ResultDamage = 0;
        //    var ResultCriticalDamage = 0;
        //    var ResultCriticalDamageDoubleAdvantage = 0;
        //    double criticalDamageValue = client.Tamer. digimon.Stats.CD / 100;
        //    var ResultDamageDoubleAdvantage = 0;
        //    double MultiplierAttribute = 0;
        //
        //    var Vantage = AttributeVantage(digimon, monster);
        //    var elementVantage = ElementVantage(digimon, monster);
        //    EvolutionLine evolveLine = DigimonEvoDB.GetLine(digimon.Species, digimon.CurrentForm);
        //    EvolvedForm CurrentForm = digimon.Forms[evolveLine.iLevel - 1];
        //
        //    ResultDamage = ResultAT + (int)Math.Floor((double)0);
        //    double addedCriticalDamage = ResultDamage * 0.8;
        //    addedCriticalDamage *= (1 + ((criticalDamageValue + 0) / 100.0));
        //    ResultCriticalDamage = ResultDamage + (int)Math.Floor(addedCriticalDamage);
        //
        //    if (CurrentForm.AttributeExp >= 1000 && Vantage)
        //    {
        //
        //        MultiplierAttribute = (2 + ((digimon.Stats.ATTR) / 200.0));
        //
        //        ResultDamageDoubleAdvantage = (int)Math.Floor(0 + (MultiplierAttribute * ResultAT));
        //        ResultCriticalDamageDoubleAdvantage = ResultDamageDoubleAdvantage + (int)Math.Floor(addedCriticalDamage);
        //    }
        //    else if (CurrentForm.NatureExp >= 1000 && elementVantage)
        //    {
        //        MultiplierAttribute = 2;
        //        ResultDamageDoubleAdvantage = (int)Math.Floor(0 + (MultiplierAttribute * ResultAT));
        //        ResultCriticalDamageDoubleAdvantage = ResultDamageDoubleAdvantage + (int)Math.Floor(addedCriticalDamage);
        //    }
        //
        //    double FinalValue = ResultCriticalDamage + ResultCriticalDamageDoubleAdvantage;
        //
        //    Random random = new Random();
        //
        //    // Gere um valor aleatório entre 0% e 5% a mais do valor original
        //    double percentagemBonus = random.NextDouble() * 0.05;
        //
        //    // Calcule o valor final com o bônus
        //    return (int)(FinalValue * (1.0 + percentagemBonus));
        //
        //}
        //
        //public static bool AttributeVantage(Digimon digimon, MonsterEntity monster)
        //{
        //    //TODO: Figured out through trying, might be wrong
        //    var EnemyStatus = DigimonListDB.GetDigimon((int)monster.Model);
        //    var AttackerStatus = DigimonListDB.GetDigimon(digimon.Species);
        //    bool AttributeVantage = AttackerStatus.Advantage.GetHashCode() == EnemyStatus.AttributeType && AttackerStatus.Advantage == DigimonListDB.eATTRIBUTE_TYPE.AT_NONE;
        //
        //    return AttributeVantage;
        //}
        //public static bool ElementVantage(Digimon digimon, MonsterEntity monster)
        //{
        //    //TODO: Figured out through trying, might be wrong
        //    var EnemyStatus = DigimonListDB.GetDigimon((int)monster.Model);
        //    var AttackerStatus = DigimonListDB.GetDigimon(digimon.Species);
        //
        //    bool ElementVantage = AttackerStatus.NatureAdvantage.GetHashCode() == EnemyStatus.NatureType && AttackerStatus.NatureAdvantage != DigimonListDB.eNATURE_TYPE.NONE;
        //
        //    return ElementVantage;
        //}

        private int CalculateFinalDamage(GameClient client, MobConfigModel? targetMob, out double critBonusMultiplier, out bool blocked)
        {
            critBonusMultiplier = 0.00;
            blocked = false;

            if (targetMob == null || client?.Tamer?.Partner?.BaseInfo == null)
                return 0;

            if (_damageFormulaConfig.Enable)
            {
                blocked = targetMob.BLValue >= UtilitiesFunctions.RandomDouble();
                var isCritical = client.Tamer.Partner.CC / 100.0 >= UtilitiesFunctions.RandomDouble() && client.Partner.CD > 0;
                critBonusMultiplier = isCritical ? 1.0 : 0.0;

                var input = CreateDamageFormulaInput(
                    client,
                    targetMob.Attribute,
                    targetMob.Element,
                    targetMob.GeneralHandler,
                    isCritical,
                    false,
                    0,
                    0,
                    0,
                    blocked ? 50 : 0);

                var result = DamageFormula.CalculateDamage(input, _damageFormulaConfig);
                LogDamageFormula(input, result);
                return result.FinalDamage;
            }

            var baseDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                client.Tamer.Partner.AT,
                client.Tamer.Partner.BaseInfo.Attribute,
                targetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                targetMob.Element);

            var random = new Random();
            // Gere um valor aleatório entre 0% e 5% a mais do valor original
            double percentagemBonus = random.NextDouble() * 0.05;

            // Calcule o valor final com o bônus
            baseDamage = (int)(baseDamage * (1.0 + percentagemBonus));

            if (baseDamage < 0) baseDamage = 0;

            critBonusMultiplier = 0.00;
            double critChance = client.Tamer.Partner.CC / 100;


            blocked = targetMob.BLValue >= UtilitiesFunctions.RandomDouble();

            //var levelBonusMultiplier = client.Tamer.Partner.Level > targetMob.Level ?
            //    (0.01f * (client.Tamer.Partner.Level - targetMob.Level)) : 0; //TODO: externalizar no portal

            //var attributeMultiplier = 0.00;
            //if (client.Tamer.Partner.BaseInfo.Attribute.HasAttributeAdvantage(targetMob.Attribute))
            //{
            //    var vlrAtual = client.Tamer.Partner.GetAttributeExperience();
            //    var bonusMax = 50.0; //TODO: externalizar?
            //    var expMax = 10000; //TODO: externalizar?

            //    attributeMultiplier = (bonusMax * vlrAtual) / expMax;
            //}
            //else if (targetMob.Attribute.HasAttributeAdvantage(client.Tamer.Partner.BaseInfo.Attribute))
            //{
            //    attributeMultiplier = -0.25;
            //}

            //var elementMultiplier = 0.00;
            //if (client.Tamer.Partner.BaseInfo.Element.HasElementAdvantage(targetMob.Element))
            //{
            //    var vlrAtual = client.Tamer.Partner.GetElementExperience();
            //    var bonusMax = 0.50; //TODO: externalizar?
            //    var expMax = 10000; //TODO: externalizar?

            //    elementMultiplier = (bonusMax * vlrAtual) / expMax;
            //}
            //else if (targetMob.Element.HasElementAdvantage(client.Tamer.Partner.BaseInfo.Element))
            //{
            //    elementMultiplier = -0.25;
            //}

            baseDamage /= blocked ? 2 : 1;

            if (critChance >= UtilitiesFunctions.RandomDouble() && client.Partner.CD > 0)
            {
                blocked = false;
                return ApplyFinalDamageBonus(GetCurrentDamage(client, targetMob), client.Tamer.Partner.FinalDamageBasisPoints);
            }

            return ApplyFinalDamageBonus(baseDamage, client.Tamer.Partner.FinalDamageBasisPoints);

            //return (int)Math.Floor(baseDamage +
            //    (baseDamage * critBonusMultiplier) +
            //    (baseDamage * levelBonusMultiplier) +
            //    (baseDamage * attributeMultiplier) +
            //    (baseDamage * elementMultiplier));
        }
        private int CalculateFinalDamage(GameClient client, SummonMobModel targetMob, out double critBonusMultiplier, out bool blocked)
        {
            critBonusMultiplier = 0.00;
            blocked = false;

            if (targetMob == null || client?.Tamer?.Partner?.BaseInfo == null)
                return 0;

            if (_damageFormulaConfig.Enable)
            {
                blocked = targetMob.BLValue >= UtilitiesFunctions.RandomDouble();
                var isCritical = client.Tamer.Partner.CC / 100.0 >= UtilitiesFunctions.RandomDouble() && client.Partner.CD > 0;
                critBonusMultiplier = isCritical ? 1.0 : 0.0;

                var input = CreateDamageFormulaInput(
                    client,
                    targetMob.Attribute,
                    targetMob.Element,
                    targetMob.GeneralHandler,
                    isCritical,
                    false,
                    0,
                    0,
                    0,
                    blocked ? 50 : 0);

                var result = DamageFormula.CalculateDamage(input, _damageFormulaConfig);
                LogDamageFormula(input, result);
                return result.FinalDamage;
            }

            var baseDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                client.Tamer.Partner.AT,
                client.Tamer.Partner.BaseInfo.Attribute,
                targetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                targetMob.Element);

            var random = new Random();
            // Gere um valor aleatório entre 0% e 5% a mais do valor original
            double percentagemBonus = random.NextDouble() * 0.05;

            // Calcule o valor final com o bônus
            baseDamage = (int)(baseDamage * (1.0 + percentagemBonus));

            if (baseDamage < 0) baseDamage = 0;

            critBonusMultiplier = 0.00;
            double critChance = client.Tamer.Partner.CC / 100;


            blocked = targetMob.BLValue >= UtilitiesFunctions.RandomDouble();

            //var levelBonusMultiplier = client.Tamer.Partner.Level > targetMob.Level ?
            //    (0.01f * (client.Tamer.Partner.Level - targetMob.Level)) : 0; //TODO: externalizar no portal

            //var attributeMultiplier = 0.00;
            //if (client.Tamer.Partner.BaseInfo.Attribute.HasAttributeAdvantage(targetMob.Attribute))
            //{
            //    var vlrAtual = client.Tamer.Partner.GetAttributeExperience();
            //    var bonusMax = 50.0; //TODO: externalizar?
            //    var expMax = 10000; //TODO: externalizar?

            //    attributeMultiplier = (bonusMax * vlrAtual) / expMax;
            //}
            //else if (targetMob.Attribute.HasAttributeAdvantage(client.Tamer.Partner.BaseInfo.Attribute))
            //{
            //    attributeMultiplier = -0.25;
            //}

            //var elementMultiplier = 0.00;
            //if (client.Tamer.Partner.BaseInfo.Element.HasElementAdvantage(targetMob.Element))
            //{
            //    var vlrAtual = client.Tamer.Partner.GetElementExperience();
            //    var bonusMax = 0.50; //TODO: externalizar?
            //    var expMax = 10000; //TODO: externalizar?

            //    elementMultiplier = (bonusMax * vlrAtual) / expMax;
            //}
            //else if (targetMob.Element.HasElementAdvantage(client.Tamer.Partner.BaseInfo.Element))
            //{
            //    elementMultiplier = -0.25;
            //}

            baseDamage /= blocked ? 2 : 1;

            if (critChance >= UtilitiesFunctions.RandomDouble() && client.Partner.CD > 0)
            {
                blocked = false;
                return ApplyFinalDamageBonus(GetCurrentDamage(client, targetMob), client.Tamer.Partner.FinalDamageBasisPoints);
            }

            return ApplyFinalDamageBonus(baseDamage, client.Tamer.Partner.FinalDamageBasisPoints);

            //return (int)Math.Floor(baseDamage +
            //    (baseDamage * critBonusMultiplier) +
            //    (baseDamage * levelBonusMultiplier) +
            //    (baseDamage * attributeMultiplier) +
            //    (baseDamage * elementMultiplier));
        }

        private int CalculateFinalDamage(GameClient client, DigimonModel? targetPartner, out double critBonusMultiplier, out bool blocked)
        {
            critBonusMultiplier = 0.00;
            blocked = false;

            if (targetPartner == null || client?.Tamer?.Partner?.BaseInfo == null)
                return 0;

            if (_damageFormulaConfig.Enable)
            {
                blocked = targetPartner.BL >= UtilitiesFunctions.RandomDouble();
                var isCritical = client.Tamer.Partner.CC / 100.0 >= UtilitiesFunctions.RandomDouble() && client.Tamer.Partner.CD > 0;
                critBonusMultiplier = isCritical ? 1.0 : 0.0;

                var input = CreateDamageFormulaInput(
                    client,
                    targetPartner.BaseInfo.Attribute,
                    targetPartner.BaseInfo.Element,
                    targetPartner.GeneralHandler,
                    isCritical,
                    false,
                    Math.Max(0, client.Tamer.Partner.AT - targetPartner.DE + UtilitiesFunctions.RandomInt(1, 15)),
                    0,
                    0,
                    blocked ? 50 : 0);

                var result = DamageFormula.CalculateDamage(input, _damageFormulaConfig);
                LogDamageFormula(input, result);
                return result.FinalDamage;
            }

            var baseDamage = client.Tamer.Partner.AT - targetPartner.DE + UtilitiesFunctions.RandomInt(1, 15);
            if (baseDamage < 0) baseDamage = 0;

            critBonusMultiplier = 0.00;
            double critChance = client.Tamer.Partner.CC / 100;
            if (critChance >= UtilitiesFunctions.RandomDouble())
                critBonusMultiplier = client.Tamer.Partner.CD;

            blocked = targetPartner.BL >= UtilitiesFunctions.RandomDouble();
            var levelBonusMultiplier = client.Tamer.Partner.Level > targetPartner.Level ?
                (0.01f * (client.Tamer.Partner.Level - targetPartner.Level)) : 0; //TODO: externalizar no portal

            baseDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                baseDamage,
                client.Tamer.Partner.BaseInfo.Attribute,
                targetPartner.BaseInfo.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                targetPartner.BaseInfo.Element);

            baseDamage /= blocked ? 2 : 1;

            var finalDamage = (int)Math.Floor(baseDamage +
                (baseDamage * critBonusMultiplier) +
                (baseDamage * levelBonusMultiplier));

            return ApplyFinalDamageBonus(finalDamage, client.Tamer.Partner.FinalDamageBasisPoints);
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
            bool isCritical,
            bool isSkill,
            int attackOverride,
            int skillBaseDamage,
            int skillDamageFlat,
            double targetReductionPercent)
        {
            var partner = client.Tamer.Partner;
            var attackerAttribute = partner.BaseInfo.Attribute;
            var attackerElement = partner.BaseInfo.Element;
            var elementPercent = Math.Max(0, attackerElement.GetElementDelta(targetElement));

            return new DamageFormulaInput
            {
                Attack = attackOverride > 0 ? attackOverride : partner.AT,
                ExtraAttack = 0,
                SkillBaseDamage = skillBaseDamage,
                SkillDamageFlat = skillDamageFlat,
                AttributePercent = partner.ATT,
                ElementPercent = elementPercent,
                SkillDamagePercent = partner.SkillDamagePercent,
                CriticalDamageExtraPercent = partner.CD,
                FinalDamagePercent = partner.FinalDamageBasisPoints / 100.0,
                TargetReductionPercent = targetReductionPercent,
                HasAttributeAdvantage = attackerAttribute.HasAttributeAdvantage(targetAttribute),
                HasElementAdvantage = elementPercent > 0,
                IsCritical = isCritical,
                IsSkill = isSkill,
                AttackerIndex = partner.GeneralHandler,
                TargetIndex = targetHandler,
                SkillId = 0
            };
        }

        private void LogDamageFormula(DamageFormulaInput input, DamageFormulaResult result)
        {
            if (_damageFormulaConfig.EnableDamageFormulaLog)
                _logger.Information(DamageFormula.CreateLogMessage(input, result));
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

        public static int GetCurrentDamage(GameClient client, MobConfigModel? targetMob)
        {
            var ResultAT = client.Tamer.Partner.AT;

            var ResultDamage = 0;
            var ResultCriticalDamage = 0;
            var ResultCriticalDamageDoubleAdvantage = 0;
            double criticalDamageValue = client.Tamer.Partner.CD;
            var ResultDamageDoubleAdvantage = 0;
            double MultiplierAttribute = 0;

            ResultDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                ResultAT,
                client.Tamer.Partner.BaseInfo.Attribute,
                client.Tamer.TargetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                client.Tamer.TargetMob.Element);
            double addedCriticalDamage = ResultDamage * 0.8;
            addedCriticalDamage *= (1 + ((criticalDamageValue + 0) / 100.0));
            ResultCriticalDamage = ResultDamage + (int)Math.Floor(addedCriticalDamage);

            double FinalValue = ResultCriticalDamage + ResultDamageDoubleAdvantage;

            Random random = new Random();

            // Gere um valor aleatório entre 0% e 5% a mais do valor original
            double percentagemBonus = random.NextDouble() * 0.05;

            // Calcule o valor final com o bônus
            return (int)(FinalValue * (1.0 + percentagemBonus));

        }
        public static int GetCurrentDamage(GameClient client, SummonMobModel? targetMob)
        {
            var ResultAT = client.Tamer.Partner.AT;

            var ResultDamage = 0;
            var ResultCriticalDamage = 0;
            var ResultCriticalDamageDoubleAdvantage = 0;
            double criticalDamageValue = client.Tamer.Partner.CD;
            var ResultDamageDoubleAdvantage = 0;
            double MultiplierAttribute = 0;

            ResultDamage = UtilitiesFunctions.ApplyNatureMatrixDamage(
                ResultAT,
                client.Tamer.Partner.BaseInfo.Attribute,
                client.Tamer.TargetMob.Attribute,
                client.Tamer.Partner.BaseInfo.Element,
                client.Tamer.TargetMob.Element);
            double addedCriticalDamage = ResultDamage * 0.8;
            addedCriticalDamage *= (1 + ((criticalDamageValue + 0) / 100.0));
            ResultCriticalDamage = ResultDamage + (int)Math.Floor(addedCriticalDamage);

            double FinalValue = ResultCriticalDamage + ResultDamageDoubleAdvantage;

            Random random = new Random();

            // Gere um valor aleatório entre 0% e 5% a mais do valor original
            double percentagemBonus = random.NextDouble() * 0.05;

            // Calcule o valor final com o bônus
            return (int)(FinalValue * (1.0 + percentagemBonus));

        }

        public async Task SendBattleOffTask(GameClient client, int attackerHandler, bool dungeon = false)
        {
            _ = SendBattleOffAfterDelayAsync(client, attackerHandler, dungeon);
            await Task.CompletedTask;
        }

        private async Task SendBattleOffAfterDelayAsync(GameClient client, int attackerHandler, bool dungeon)
        {
            await Task.Delay(4000);

            if (dungeon)
                _dungeonServer.BroadcastForTamerViewsAndSelf(
                    client.TamerId,
                    new SetCombatOffPacket(attackerHandler).Serialize());
            else
                _mapServer.BroadcastForTamerViewsAndSelf(
                    client.TamerId,
                    new SetCombatOffPacket(attackerHandler).Serialize());
        }
    }
}
