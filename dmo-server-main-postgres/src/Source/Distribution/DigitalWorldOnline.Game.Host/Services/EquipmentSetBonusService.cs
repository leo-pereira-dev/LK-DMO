using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.Services
{
    public sealed class EquipmentSetBonusService
    {
        private const short ClientBuffVisualClearCount = 1;

        private readonly ISender _sender;
        private readonly ILogger _logger;

        public EquipmentSetBonusService(ISender sender, ILogger logger)
        {
            _sender = sender;
            _logger = logger;
        }

        public void TryApplyPartnerTrigger(
            GameClient client,
            EquipmentSetBonusTrigger trigger,
            Action<byte[]> broadcast)
        {
            if (client?.Tamer == null || client.Partner == null)
                return;

            foreach (var rule in EquipmentSetBonusRegistry.Rules)
            {
                var equippedPieces = CountEquippedPieces(client, rule);
                if (equippedPieces == 0)
                    continue;

                foreach (var effect in rule.Effects)
                {
                    if (effect.IsPassive ||
                        effect.Trigger != trigger ||
                        effect.BuffInfo == null ||
                        effect.DurationSeconds <= 0 ||
                        equippedPieces < effect.RequiredCount)
                        continue;

                    if (!Roll(effect.ChancePercent))
                        continue;

                    ApplyPartnerBuff(client, effect, broadcast);
                }
            }
        }

        public bool SyncPartnerPassiveBuffs(
            GameClient client,
            Action<byte[]>? broadcast = null,
            bool sendStatus = false)
        {
            if (client?.Tamer == null || client.Partner == null)
                return false;

            var partner = client.Partner;
            var expectedEffects = new List<EquipmentSetBonusEffect>();
            foreach (var rule in EquipmentSetBonusRegistry.Rules)
            {
                var equippedPieces = CountEquippedPieces(client, rule);
                if (equippedPieces == 0)
                    continue;

                expectedEffects.AddRange(rule.Effects.Where(effect =>
                    effect.IsPassive &&
                    effect.BuffId > 0 &&
                    effect.BuffInfo != null &&
                    equippedPieces >= effect.RequiredCount));
            }

            var expectedBySkill = expectedEffects
                .GroupBy(effect => effect.SkillCode)
                .ToDictionary(group => group.Key, group => group.First());

            var knownPassiveSkillCodes = EquipmentSetBonusRegistry.Rules
                .SelectMany(rule => rule.Effects)
                .Where(effect => effect.IsPassive && effect.BuffId > 0)
                .Select(effect => (int)effect.SkillCode)
                .ToHashSet();

            var changed = false;
            var currentSetBuffs = partner.BuffList.Buffs
                .Where(buff => knownPassiveSkillCodes.Contains(buff.SkillId))
                .ToList();

            foreach (var buff in currentSetBuffs)
            {
                if (expectedBySkill.ContainsKey(buff.SkillId))
                    continue;

                partner.BuffList.Buffs.RemoveAll(current => current.SkillId == buff.SkillId);
                broadcast?.Invoke(new RemoveBuffPacket(partner.GeneralHandler, buff.BuffId, ClientBuffVisualClearCount).Serialize());
                changed = true;
            }

            foreach (var effect in expectedBySkill.Values)
            {
                var existing = partner.BuffList.Buffs.FirstOrDefault(buff => buff.SkillId == effect.SkillCode);
                if (existing != null)
                {
                    if (existing.BuffId != effect.BuffId)
                    {
                        var previousBuffIds = partner.BuffList.Buffs
                            .Where(buff => buff.SkillId == effect.SkillCode)
                            .Select(buff => buff.BuffId)
                            .Distinct()
                            .ToList();

                        partner.BuffList.Buffs.RemoveAll(buff => buff.SkillId == effect.SkillCode);

                        foreach (var previousBuffId in previousBuffIds)
                            broadcast?.Invoke(new RemoveBuffPacket(partner.GeneralHandler, previousBuffId, ClientBuffVisualClearCount).Serialize());

                        var correctedBuff = DigimonBuffModel.Create(effect.BuffId, (int)effect.SkillCode);
                        correctedBuff.SetBuffInfo(effect.BuffInfo);
                        partner.BuffList.Add(correctedBuff);

                        broadcast?.Invoke(new AddBuffPacket(partner.GeneralHandler, effect.BuffInfo!, 0, uint.MaxValue).Serialize());
                        changed = true;
                        continue;
                    }

                    existing.SetBuffInfo(effect.BuffInfo);
                    continue;
                }

                var passiveBuff = DigimonBuffModel.Create(effect.BuffId, (int)effect.SkillCode);
                passiveBuff.SetBuffInfo(effect.BuffInfo);
                partner.BuffList.Add(passiveBuff);

                broadcast?.Invoke(new AddBuffPacket(partner.GeneralHandler, effect.BuffInfo!, 0, uint.MaxValue).Serialize());
                changed = true;
            }

            if (!changed)
                return false;

            if (sendStatus)
                client.Send(new UpdateStatusPacket(client.Tamer).Serialize());

            _ = _sender.Send(new UpdateDigimonBuffListCommand(partner.BuffList));

            _logger.Debug(
                "Equipment set passive visuals synced. tamer={TamerId} partner={PartnerId} active={ActiveBuffs}",
                client.TamerId,
                partner.Id,
                string.Join(",", expectedBySkill.Values.Select(effect => $"{effect.SkillCode}:{effect.BuffId}")));

            return true;
        }

        private void ApplyPartnerBuff(
            GameClient client,
            EquipmentSetBonusEffect effect,
            Action<byte[]> broadcast)
        {
            var partner = client.Partner;
            var buffInfo = effect.BuffInfo;
            if (partner == null || buffInfo == null)
                return;

            var buffIdsToClear = partner.BuffList.ActiveBuffs
                .Where(buff => buff.BuffId == effect.BuffId || buff.SkillId == effect.SkillCode)
                .Select(buff => buff.BuffId)
                .Append(effect.BuffId)
                .Distinct()
                .ToList();

            partner.BuffList.Buffs.RemoveAll(buff => buff.BuffId == effect.BuffId || buff.SkillId == effect.SkillCode);

            foreach (var buffId in buffIdsToClear)
                broadcast(new RemoveBuffPacket(partner.GeneralHandler, buffId, ClientBuffVisualClearCount).Serialize());

            var newBuff = DigimonBuffModel.Create(effect.BuffId, (int)effect.SkillCode, 0, effect.DurationSeconds);
            newBuff.SetBuffInfo(buffInfo);
            partner.BuffList.Add(newBuff);

            var clientEndTimestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds() + effect.DurationSeconds;
            broadcast(new AddBuffPacket(partner.GeneralHandler, effect.BuffId, (int)effect.SkillCode, 0, (int)clientEndTimestamp).Serialize());
            client.Send(new UpdateStatusPacket(client.Tamer).Serialize());

            _ = _sender.Send(new UpdateDigimonBuffListCommand(partner.BuffList));

            _logger.Debug(
                "Equipment set buff applied. tamer={TamerId} partner={PartnerId} trigger={Trigger} skill={SkillCode} buff={BuffId} duration={Duration}s chance={Chance}%",
                client.TamerId,
                partner.Id,
                effect.Trigger,
                effect.SkillCode,
                effect.BuffId,
                effect.DurationSeconds,
                effect.ChancePercent);
        }

        private static int CountEquippedPieces(GameClient client, EquipmentSetBonusRule rule)
        {
            var tamer = client.Tamer;
            var partner = client.Partner;
            if (tamer == null || partner == null || tamer.Equipment == null)
                return 0;

            return tamer.Equipment.EquippedItems.Count(item =>
                item.ItemInfo != null &&
                !item.Expired &&
                tamer.Level >= item.ItemInfo.TamerMinLevel &&
                partner.Level >= item.ItemInfo.DigimonMinLevel &&
                rule.ContainsItem(item.ItemId));
        }

        private static bool Roll(int chancePercent)
        {
            if (chancePercent >= 100)
                return true;

            if (chancePercent <= 0)
                return false;

            return Random.Shared.Next(1, 101) <= chancePercent;
        }
    }
}
