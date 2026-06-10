using System.Collections.Concurrent;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Config;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Models.Map;
using DigitalWorldOnline.Commons.Models.Summon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using Serilog;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class VerdandiXProgramService
    {
        public const int VerdandiTerminalMapId = 1700;

        public const int XProgramDebuffId = 64000;
        public const int LegacyWrongXProgramDebuffId = 63000;
        public const int XProtectorSkillCode = 2700001;
        private const int XProgramDotDamage = 100;
        private const int XProgramDotIntervalSeconds = 10;

        private static readonly HashSet<int> XProgramDebuffIds = new()
        {
            XProgramDebuffId,
            LegacyWrongXProgramDebuffId
        };

        private static readonly HashSet<int> XProtectorBuffIds = new()
        {
            64001, // X-Protector item
            64014  // X-Protector variant
        };

        private static readonly HashSet<int> XProtectorItemIds = new()
        {
            75454,
            75455
        };

        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ConcurrentDictionary<long, DateTime> _nextDotDamageByPartnerId = new();

        public VerdandiXProgramService(AssetsLoader assets, ILogger logger)
        {
            _assets = assets.Load();
            _logger = logger;
        }

        public void Tick(
            GameClient client,
            MapInstance map,
            Action<long, byte[]> broadcastToSelfAndViewers,
            Action<long, byte[]> broadcastToViewers)
        {
            if (client.Partner == null)
                return;

            if (!IsVerdandi(map.MapId))
            {
                _nextDotDamageByPartnerId.TryRemove(client.Partner.Id, out _);
                RemoveXProgram(client, packet => broadcastToSelfAndViewers(client.TamerId, packet), "left Verdandi");
                return;
            }

            if (!client.Tamer.DebuffTime)
                return;

            client.Tamer.UpdateDebuffTime();

            if (IsMapSafeFromXProgram(map) || IsXDigimon(client) || HasXProtector(client))
            {
                _nextDotDamageByPartnerId.TryRemove(client.Partner.Id, out _);
                RemoveXProgram(client, packet => broadcastToSelfAndViewers(client.TamerId, packet), "protected");
                return;
            }

            if (ApplyXProgram(client, packet => broadcastToSelfAndViewers(client.TamerId, packet)))
                ApplyXProgramDotDamage(client, packet => broadcastToViewers(client.TamerId, packet));
        }

        public void RemoveXProgramAfterOmegamonDefeat(MapInstance map, MobConfigModel mob, Action<long, byte[]> broadcast)
        {
            if (!IsVerdandi(map.MapId) || !IsOmegamon(mob.Type, mob.Name))
                return;

            RemoveXProgramFromMap(map, broadcast, mob.Name);
        }

        public void RemoveXProgramAfterOmegamonDefeat(MapInstance map, SummonMobModel mob, Action<long, byte[]> broadcast)
        {
            if (!IsVerdandi(map.MapId) || !IsOmegamon(mob.Type, mob.Name))
                return;

            RemoveXProgramFromMap(map, broadcast, mob.Name);
        }

        public bool RemoveXProgram(GameClient client, Action<byte[]> broadcast, string reason)
        {
            if (client.Partner == null)
                return false;

            _nextDotDamageByPartnerId.TryRemove(client.Partner.Id, out _);

            var removed = false;
            foreach (var debuffId in XProgramDebuffIds)
            {
                var debuffs = client.Partner.DebuffList.Buffs
                    .Where(x => x.BuffId == debuffId)
                    .ToList();

                foreach (var debuff in debuffs)
                {
                    broadcast(new RemoveBuffPacket(client.Partner.GeneralHandler, debuff.BuffId).Serialize());
                    client.Partner.DebuffList.Buffs.Remove(debuff);
                    removed = true;
                }
            }

            if (removed)
            {
                _logger.Debug(
                    "Verdandi X-Program removed for tamer {TamerId}, partner {PartnerId}. Reason: {Reason}.",
                    client.TamerId,
                    client.Partner.Id,
                    reason);
            }

            return removed;
        }

        public static bool IsXProtectorItem(ItemModel item)
        {
            if (XProtectorItemIds.Contains(item.ItemId))
                return true;

            return item.ItemInfo?.SkillCode == XProtectorSkillCode &&
                   item.ItemInfo.Name?.Contains("X-Protector", StringComparison.OrdinalIgnoreCase) == true;
        }

        private bool ApplyXProgram(GameClient client, Action<byte[]> broadcast)
        {
            if (client.Partner == null)
                return false;

            RemoveLegacyWrongXProgram(client, broadcast);

            if (client.Partner.DebuffList.ActiveBuffs.Any(x => x.BuffId == XProgramDebuffId))
                return true;

            var buffInfo = FindBuffInfo(XProgramDebuffId);
            if (buffInfo == null)
            {
                _logger.Warning("Verdandi X-Program buff info {BuffId} was not found.", XProgramDebuffId);
                return false;
            }

            var newDebuff = DigimonDebuffModel.Create(buffInfo.BuffId, buffInfo.SkillId, 0, 0);
            newDebuff.SetBuffInfo(buffInfo);
            client.Partner.DebuffList.Add(newDebuff);

            broadcast(new AddBuffPacket(client.Partner.GeneralHandler, buffInfo, 0, uint.MaxValue).Serialize());

            _logger.Debug(
                "Verdandi X-Program applied to tamer {TamerId}, partner {PartnerId}, digimon type {DigimonType}.",
                client.TamerId,
                client.Partner.Id,
                client.Partner.CurrentType);

            return true;
        }

        private void ApplyXProgramDotDamage(GameClient client, Action<byte[]> broadcast)
        {
            if (client.Partner == null || client.Partner.CurrentHp <= 0)
                return;

            var now = DateTime.Now;
            var nextDamageTime = _nextDotDamageByPartnerId.GetOrAdd(client.Partner.Id, now);
            if (now < nextDamageTime)
                return;

            _nextDotDamageByPartnerId[client.Partner.Id] = now.AddSeconds(XProgramDotIntervalSeconds);

            var newHp = client.Partner.ReceiveDamage(XProgramDotDamage);
            if (newHp <= 0)
                client.Tamer.Die();

            client.Send(new UpdateCurrentResourcesPacket(
                client.Partner.GeneralHandler,
                (short)client.Partner.CurrentHp,
                (short)client.Partner.CurrentDs,
                0));
            client.Send(new UpdateStatusPacket(client.Tamer));

            // Packet 1007 is only for other clients tracking this object; the v487 client asserts if it targets the owner's local Digimon.
            broadcast(new UpdateCurrentHPRatePacket(client.Partner.GeneralHandler, client.Partner.HpRate).Serialize());

            _logger.Debug(
                "Verdandi X-Program DoT dealt {Damage} damage to tamer {TamerId}, partner {PartnerId}. Current HP: {CurrentHp}.",
                XProgramDotDamage,
                client.TamerId,
                client.Partner.Id,
                client.Partner.CurrentHp);
        }

        private void RemoveLegacyWrongXProgram(GameClient client, Action<byte[]> broadcast)
        {
            var legacyDebuffs = client.Partner.DebuffList.Buffs
                .Where(x => x.BuffId == LegacyWrongXProgramDebuffId)
                .ToList();

            foreach (var legacyDebuff in legacyDebuffs)
            {
                broadcast(new RemoveBuffPacket(client.Partner.GeneralHandler, legacyDebuff.BuffId).Serialize());
                client.Partner.DebuffList.Buffs.Remove(legacyDebuff);
            }
        }

        private void RemoveXProgramFromMap(MapInstance map, Action<long, byte[]> broadcast, string sourceName)
        {
            foreach (var client in map.Clients.Where(x => x.IsConnected && x.Partner != null))
                RemoveXProgram(client, packet => broadcast(client.TamerId, packet), $"Omegamon defeated: {sourceName}");
        }

        private bool IsMapSafeFromXProgram(MapInstance map)
        {
            var configuredOmegamon =
                map.Mobs.Any(IsOmegamon) ||
                map.SummonMobs.Any(IsOmegamon);

            if (!configuredOmegamon)
                return false;

            return !map.Mobs.Any(IsActiveOmegamon) &&
                   !map.SummonMobs.Any(IsActiveOmegamon);
        }

        private static bool IsVerdandi(int mapId)
        {
            return mapId == VerdandiTerminalMapId;
        }

        private bool IsXDigimon(GameClient client)
        {
            var evolutionType = client.Partner.BaseInfo?.EvolutionType ??
                                _assets.DigimonBaseInfo.FirstOrDefault(x => x.Type == client.Partner.CurrentType)?.EvolutionType;

            if (evolutionType == null)
                return false;

            return (EvolutionRankEnum)evolutionType.Value is EvolutionRankEnum.RookieX
                or EvolutionRankEnum.ChampionX
                or EvolutionRankEnum.UltimateX
                or EvolutionRankEnum.MegaX
                or EvolutionRankEnum.BurstModeX
                or EvolutionRankEnum.JogressX;
        }

        private static bool HasXProtector(GameClient client)
        {
            return client.Tamer.BuffList.ActiveBuffs.Any(x => XProtectorBuffIds.Contains(x.BuffId)) ||
                   client.Partner.BuffList.ActiveBuffs.Any(x => XProtectorBuffIds.Contains(x.BuffId));
        }

        private BuffInfoAssetModel? FindBuffInfo(int buffId)
        {
            var buffInfo = _assets.BuffInfo.FirstOrDefault(x => x.BuffId == buffId);
            if (buffInfo?.SkillInfo == null)
                buffInfo?.SetSkillInfo(_assets.SkillCodeInfo.FirstOrDefault(x => x.SkillCode == buffInfo.SkillId));

            return buffInfo;
        }

        private static bool IsActiveOmegamon(MobConfigModel mob)
        {
            return IsOmegamon(mob) && !mob.Dead && !mob.AwaitingKillSpawn;
        }

        private static bool IsActiveOmegamon(SummonMobModel mob)
        {
            return IsOmegamon(mob) && !mob.Dead;
        }

        private static bool IsOmegamon(MobConfigModel mob)
        {
            return IsOmegamon(mob.Type, mob.Name);
        }

        private static bool IsOmegamon(SummonMobModel mob)
        {
            return IsOmegamon(mob.Type, mob.Name);
        }

        private static bool IsOmegamon(int type, string? name)
        {
            return type is 72100 or 71009 or 71057 or 99843 ||
                   name?.Contains("Omegamon", StringComparison.OrdinalIgnoreCase) == true;
        }
    }
}
