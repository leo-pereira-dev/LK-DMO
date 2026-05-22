using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using Serilog;

namespace DigitalWorldOnline.Application.GameAssets.Xml
{
    public sealed class DUnitCollectionService
    {
        private readonly UnionXmlAssetLoader _loader;
        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;

        public DUnitCollectionService(UnionXmlAssetLoader loader, AssetsLoader assets, ILogger logger)
        {
            _loader = loader;
            _assets = assets;
            _logger = logger;
        }

        public DUnitCollectionBonusResult CalculateDUnitCollectionBonus(CharacterModel character)
        {
            _loader.Load();

            var ownedDigimons = BuildOwnedDigimonMap(character);
            var evaluations = new List<DUnitCollectionEvaluation>();
            var result = new DUnitCollectionBonusResult(evaluations);
            var appliedGroups = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            foreach (var group in _loader.CollectionGroups.Where(x => x.Enabled))
            {
                var groupKey = $"{group.SourceType}:{group.GroupId}";
                var owned = 0;
                var transcendedCount = 0;
                var totalLevel = 0;
                var allTranscended = true;
                var memberProgress = new List<DUnitRequiredDigimonProgress>();

                foreach (var required in group.RequiredDigimons)
                {
                    if (!ownedDigimons.TryGetValue(required.DigimonId, out var digimon))
                    {
                        allTranscended = false;
                        memberProgress.Add(new DUnitRequiredDigimonProgress(
                            required,
                            false,
                            0,
                            false,
                            false,
                            false,
                            false,
                            false));
                        continue;
                    }

                    var evolutionUnlocked = !required.RequiredUnlocked || IsEvolutionUnlocked(digimon, required.DigimonId);
                    var transcended = digimon.HatchGrade == DigimonHatchGradeEnum.Transcend;
                    var meetsLevel = digimon.Level >= required.RequiredLevel;
                    var memberMeetsTranscendence = !(required.RequiredTranscendence || group.Requirements.RequireAllTranscended) || transcended;
                    var ownedLevel = (ushort)Math.Min((int)digimon.Level, ushort.MaxValue);

                    if (!meetsLevel)
                    {
                        allTranscended = false;
                        memberProgress.Add(new DUnitRequiredDigimonProgress(
                            required,
                            true,
                            ownedLevel,
                            evolutionUnlocked,
                            transcended,
                            false,
                            evolutionUnlocked,
                            memberMeetsTranscendence));
                        continue;
                    }

                    if (!evolutionUnlocked)
                    {
                        allTranscended = false;
                        memberProgress.Add(new DUnitRequiredDigimonProgress(
                            required,
                            true,
                            ownedLevel,
                            false,
                            transcended,
                            true,
                            false,
                            memberMeetsTranscendence));
                        continue;
                    }

                    owned++;
                    totalLevel += digimon.Level;

                    if (!memberMeetsTranscendence)
                        allTranscended = false;
                    else if (transcended)
                        transcendedCount++;

                    memberProgress.Add(new DUnitRequiredDigimonProgress(
                        required,
                        true,
                        ownedLevel,
                        true,
                        transcended,
                        true,
                        true,
                        memberMeetsTranscendence));
                }

                var meetsMembers = owned >= group.Requirements.MembersRequired;
                var meetsLevel1 = group.Requirements.TotalLevelRequirement1 <= 0 ||
                                  totalLevel >= group.Requirements.TotalLevelRequirement1;
                var meetsLevel2 = group.Requirements.TotalLevelRequirement2 <= 0 ||
                                  totalLevel >= group.Requirements.TotalLevelRequirement2;
                var meetsTranscendence = owned >= group.Requirements.MembersRequired &&
                                         transcendedCount >= group.Requirements.MembersRequired;
                var effectProgress = group.Effects
                    .Select(effect => new DUnitCollectionEffectProgress(
                        effect,
                        IsConditionMet(effect.RequiredConditionIndex, meetsMembers, meetsLevel1, meetsTranscendence, meetsLevel2)))
                    .ToList();
                var completed = effectProgress.Count > 0 && effectProgress.All(effect => effect.Active);

                evaluations.Add(new DUnitCollectionEvaluation(
                    group,
                    completed,
                    owned,
                    totalLevel,
                    allTranscended,
                    memberProgress,
                    effectProgress));

                if (!effectProgress.Any(effect => effect.Active))
                    continue;

                if (!appliedGroups.Add(groupKey))
                {
                    _logger.Warning(
                        "[DUnit] Duplicate completed group ignored for character={CharacterName}: {GroupKey} '{GroupName}'",
                        character.Name,
                        groupKey,
                        group.GroupName);
                    continue;
                }

                foreach (var effect in effectProgress.Where(effect => effect.Active))
                    result.Add(effect.Effect.EffectType, effect.Effect.EffectValue);

                if (completed)
                {
                    _logger.Information(
                        "[DUnit] Character {CharacterName} completed group {GroupId}: {GroupName}",
                        character.Name,
                        group.GroupId,
                        group.GroupName);
                }
            }

            return result;
        }

        private static bool IsConditionMet(
            int? conditionIndex,
            bool meetsMembers,
            bool meetsLevel1,
            bool meetsTranscendence,
            bool meetsLevel2)
        {
            return conditionIndex switch
            {
                1 => meetsMembers,
                2 => meetsLevel1,
                3 => meetsTranscendence,
                4 => meetsLevel2,
                null => meetsMembers && meetsLevel1 && meetsTranscendence && meetsLevel2,
                0 => meetsMembers && meetsLevel1 && meetsTranscendence && meetsLevel2,
                _ => false
            };
        }

        public DUnitCollectionBonusModel ApplyBonuses(CharacterModel character, DUnitCollectionBonusResult? calculatedResult = null)
        {
            var result = calculatedResult ?? CalculateDUnitCollectionBonus(character);
            var bonus = ToRuntimeBonus(result);

            character.ClearDUnitCollectionBonus();
            character.SetDUnitCollectionBonus(bonus);

            _logger.Information(
                "[DUnit] Applied bonuses: character={CharacterName} groups={CompletedGroups}/{TotalGroups} HP={HP}, DS={DS}, AT={AT}, DE={DE}, HT={HT}, CT={CT}, EV={EV}, BL={BL}, EXP={EXP}, SCD={SCD}",
                character.Name,
                result.CompletedGroups,
                result.Groups.Count,
                bonus.HP,
                bonus.DS,
                bonus.AT,
                bonus.DE,
                bonus.HT,
                bonus.CT,
                bonus.EV,
                bonus.BL,
                bonus.EXP,
                bonus.SCD);

            return bonus;
        }

        private static DUnitCollectionBonusModel ToRuntimeBonus(DUnitCollectionBonusResult result)
        {
            var bonus = new DUnitCollectionBonusModel();
            bonus.Set(
                result.Get(DUnitCollectionEffectType.HP),
                result.Get(DUnitCollectionEffectType.DS),
                result.Get(DUnitCollectionEffectType.AT),
                result.Get(DUnitCollectionEffectType.DE),
                result.Get(DUnitCollectionEffectType.HT),
                result.Get(DUnitCollectionEffectType.CT),
                result.Get(DUnitCollectionEffectType.EV),
                result.Get(DUnitCollectionEffectType.BL),
                result.Get(DUnitCollectionEffectType.EXP),
                result.Get(DUnitCollectionEffectType.SCD),
                result.Get(DUnitCollectionEffectType.Basic),
                result.Get(DUnitCollectionEffectType.Vaccine),
                result.Get(DUnitCollectionEffectType.Data),
                result.Get(DUnitCollectionEffectType.Virus),
                result.Get(DUnitCollectionEffectType.UnknownAttribute),
                result.Get(DUnitCollectionEffectType.Fire),
                result.Get(DUnitCollectionEffectType.Water),
                result.Get(DUnitCollectionEffectType.Ice),
                result.Get(DUnitCollectionEffectType.Wind),
                result.Get(DUnitCollectionEffectType.Thunder),
                result.Get(DUnitCollectionEffectType.Light),
                result.Get(DUnitCollectionEffectType.Dark),
                result.Get(DUnitCollectionEffectType.Land),
                result.Get(DUnitCollectionEffectType.Wood),
                result.Get(DUnitCollectionEffectType.Steel));

            return bonus;
        }

        private static Dictionary<int, DigimonModel> BuildOwnedDigimonMap(CharacterModel character)
        {
            var result = new Dictionary<int, DigimonModel>();

            foreach (var digimon in character.Digimons)
                AddDigimon(result, digimon);

            foreach (var digimon in character.DigimonArchive.DigimonArchives
                         .Select(x => x.Digimon)
                         .Where(x => x != null)
                         .Cast<DigimonModel>())
            {
                AddDigimon(result, digimon);
            }

            return result;
        }

        private static void AddDigimon(IDictionary<int, DigimonModel> result, DigimonModel digimon)
        {
            foreach (var type in digimon.Evolutions.Select(x => x.Type).Append(digimon.BaseType).Append(digimon.CurrentType))
            {
                if (type > 0 && (!result.TryGetValue(type, out var current) || current.Level < digimon.Level))
                    result[type] = digimon;
            }
        }

        private bool IsEvolutionUnlocked(DigimonModel digimon, int type)
        {
            if (digimon.BaseType == type || digimon.CurrentType == type)
                return true;

            var evolution = digimon.Evolutions.FirstOrDefault(x => x.Type == type);
            if (evolution == null)
                return false;

            if (evolution.Unlocked > 0)
                return true;

            return IsStarterNormalEvolutionAvailable(digimon, type);
        }

        private bool IsStarterNormalEvolutionAvailable(DigimonModel digimon, int type)
        {
            if (!IsStarterPartner(digimon.BaseType))
                return false;

            var line = _assets.EvolutionInfo
                .FirstOrDefault(x => x.Type == digimon.BaseType)?
                .Lines.FirstOrDefault(x => x.Type == type);
            if (line == null)
                return false;

            var rank = (EvolutionRankEnum)(_assets.DigimonBaseInfo
                .FirstOrDefault(x => x.Type == type)?.EvolutionType ?? 0);
            if (rank is < EvolutionRankEnum.Rookie or > EvolutionRankEnum.Mega)
                return false;

            return digimon.Level >= line.UnlockLevel;
        }

        private static bool IsStarterPartner(int baseType)
        {
            return baseType is 31001 or 31002 or 31003 or 31004;
        }
    }
}
