using System.Xml.Linq;
using DigitalWorldOnline.Application.GameAssets.Bins;
using Serilog;

namespace DigitalWorldOnline.Application.GameAssets.Xml
{
    public sealed class UnionXmlAssetLoader
    {
        private const string LinesFileName = "DM_Union-DigimonLines.xml";
        private const string CollectionsFileName = "DM_Union-DigimonCollections.xml";
        private const string EffectsFileName = "DM_Union-Effect.xml";
        private const string LevelFileName = "DM_Union-Level.xml";

        private static readonly IReadOnlySet<DUnitCollectionEffectType> AutoApplicableEffects =
            new HashSet<DUnitCollectionEffectType>
            {
                DUnitCollectionEffectType.HP,
                DUnitCollectionEffectType.DS,
                DUnitCollectionEffectType.AT,
                DUnitCollectionEffectType.DE,
                DUnitCollectionEffectType.HT,
                DUnitCollectionEffectType.CT,
                DUnitCollectionEffectType.EV,
                DUnitCollectionEffectType.BL,
                DUnitCollectionEffectType.EXP,
                DUnitCollectionEffectType.SCD,
                DUnitCollectionEffectType.Basic,
                DUnitCollectionEffectType.Vaccine,
                DUnitCollectionEffectType.Data,
                DUnitCollectionEffectType.Virus,
                DUnitCollectionEffectType.UnknownAttribute,
                DUnitCollectionEffectType.Fire,
                DUnitCollectionEffectType.Water,
                DUnitCollectionEffectType.Ice,
                DUnitCollectionEffectType.Wind,
                DUnitCollectionEffectType.Thunder,
                DUnitCollectionEffectType.Light,
                DUnitCollectionEffectType.Dark,
                DUnitCollectionEffectType.Land,
                DUnitCollectionEffectType.Wood,
                DUnitCollectionEffectType.Steel
            };

        private readonly object _sync = new();
        private readonly DigimonListBinLoader _digimonList;
        private readonly ILogger _logger;
        private bool _loaded;

        public UnionXmlAssetLoader(DigimonListBinLoader digimonList, ILogger logger)
        {
            _digimonList = digimonList;
            _logger = logger;
        }

        public IReadOnlyList<UnionXmlRewardTier> RewardTiers { get; private set; } =
            new List<UnionXmlRewardTier>
            {
                new(0, "Bronze", "bronze"),
                new(1, "Prata", "silver"),
                new(2, "Ouro", "gold"),
                new(3, "Platinum", "platinum"),
                new(4, "Diamante", "diamond"),
                new(5, "Master", "master"),
                new(6, "Mestre Digimon", "digimon_master")
            };

        public IReadOnlyList<UnionXmlStatusBonus> TotalStatus { get; private set; } =
            Array.Empty<UnionXmlStatusBonus>();

        public IReadOnlyDictionary<byte, int> RequiredExperienceByLevel { get; private set; } =
            new Dictionary<byte, int>();

        public IReadOnlyList<DUnitCollectionGroup> CollectionGroups { get; private set; } =
            Array.Empty<DUnitCollectionGroup>();

        public int InvalidGroupCount { get; private set; }

        public int XmlFileCount { get; private set; }

        public int TotalGroupCount { get; private set; }

        public int MemberCount { get; private set; }

        public int EffectCount { get; private set; }

        public int UnknownEffectCount { get; private set; }

        public int MissingDigimonIdCount { get; private set; }

        public string? SourceDirectory { get; private set; }

        public UnionXmlAssetLoader Load()
        {
            lock (_sync)
            {
                if (_loaded)
                    return this;

                SourceDirectory = ResolveDirectory();
                if (string.IsNullOrWhiteSpace(SourceDirectory))
                {
                    _logger.Warning("[DUnit] XML union directory not found. Expected xml/union, Data/union, or ClientDist/Data/union.");
                    _loaded = true;
                    return this;
                }

                XmlFileCount = Directory.GetFiles(SourceDirectory, "*.xml", SearchOption.TopDirectoryOnly).Length;
                TotalStatus = LoadStatusTotals(SourceDirectory);
                RequiredExperienceByLevel = LoadLevelRequirements(SourceDirectory);
                CollectionGroups = LoadCollectionGroups(
                    SourceDirectory,
                    out var totalGroups,
                    out var invalidGroups,
                    out var memberCount,
                    out var effectCount,
                    out var unknownEffectCount,
                    out var missingDigimonIdCount);
                TotalGroupCount = totalGroups;
                InvalidGroupCount = invalidGroups;
                MemberCount = memberCount;
                EffectCount = effectCount;
                UnknownEffectCount = unknownEffectCount;
                MissingDigimonIdCount = missingDigimonIdCount;

                _logger.Information(
                    "[DUnit] XML summary: files={XmlFiles}, groupsTotal={TotalGroups}, groupsValid={ValidGroups}, groupsInvalid={InvalidGroups}, members={Members}, effects={Effects}, unknownEffects={UnknownEffects}, missingDigimonIds={MissingDigimonIds}, source={SourceDirectory}",
                    XmlFileCount,
                    TotalGroupCount,
                    CollectionGroups.Count,
                    InvalidGroupCount,
                    MemberCount,
                    EffectCount,
                    UnknownEffectCount,
                    MissingDigimonIdCount,
                    SourceDirectory);

                _logger.Information(
                    "[DUnit] Loaded groups: {GroupCount}. Effects: {EffectCount}. Invalid groups: {InvalidGroups}. Source: {SourceDirectory}",
                    CollectionGroups.Count,
                    EffectCount,
                    InvalidGroupCount,
                    SourceDirectory);

                _loaded = true;
                return this;
            }
        }

        public int GetRequiredExperience(byte level)
        {
            Load();

            if (RequiredExperienceByLevel.TryGetValue(level, out var requiredExperience))
                return Math.Max(1, requiredExperience);

            return 10000;
        }

        private static string? ResolveDirectory()
        {
            var dir = new DirectoryInfo(AppContext.BaseDirectory);
            while (dir != null)
            {
                foreach (var relative in new[]
                {
                    Path.Combine("xml", "union"),
                    Path.Combine("Data", "union"),
                    Path.Combine("ClientDist", "Data", "union")
                })
                {
                    var candidate = Path.Combine(dir.FullName, relative);
                    if (Directory.Exists(candidate))
                        return candidate;
                }

                dir = dir.Parent;
            }

            return null;
        }

        private static IReadOnlyList<UnionXmlStatusBonus> LoadStatusTotals(string directory)
        {
            var totals = new Dictionary<string, (int Value, bool Percent)>(StringComparer.OrdinalIgnoreCase);

            foreach (var fileName in new[] { LinesFileName, CollectionsFileName })
            {
                var path = Path.Combine(directory, fileName);
                if (!File.Exists(path))
                    continue;

                var document = XDocument.Load(path);
                foreach (var stat in document.Descendants("TotalStat"))
                {
                    var name = (string?)stat.Attribute("type");
                    if (string.IsNullOrWhiteSpace(name))
                        continue;

                    var value = (int?)stat.Attribute("value") ?? 0;
                    var percent = string.Equals((string?)stat.Attribute("unit"), "percent", StringComparison.OrdinalIgnoreCase);

                    if (totals.TryGetValue(name, out var current))
                        totals[name] = (current.Value + value, current.Percent || percent);
                    else
                        totals[name] = (value, percent);
                }
            }

            return totals
                .Select(x => new UnionXmlStatusBonus(x.Key, x.Value.Value, x.Value.Percent))
                .OrderBy(x => x.Stat)
                .ToList();
        }

        private static IReadOnlyDictionary<byte, int> LoadLevelRequirements(string directory)
        {
            var path = Path.Combine(directory, LevelFileName);
            if (!File.Exists(path))
                return new Dictionary<byte, int>();

            var result = new Dictionary<byte, int>();
            var document = XDocument.Load(path);

            foreach (var level in document.Descendants("Level"))
            {
                var id = (int?)level.Attribute("id") ?? (int?)level.Attribute("level");
                var required = (int?)level.Attribute("requiredExperience") ?? (int?)level.Attribute("exp");

                if (id == null || required == null || id < byte.MinValue || id > byte.MaxValue)
                    continue;

                result[(byte)id.Value] = Math.Max(1, required.Value);
            }

            return result;
        }

        private IReadOnlyList<DUnitCollectionGroup> LoadCollectionGroups(
            string directory,
            out int totalGroups,
            out int invalidGroups,
            out int memberCount,
            out int effectCount,
            out int unknownEffectCount,
            out int missingDigimonIdCount)
        {
            totalGroups = 0;
            invalidGroups = 0;
            memberCount = 0;
            missingDigimonIdCount = 0;

            var digimonList = _digimonList.Load();
            var digimonIds = digimonList.ByType.Keys.ToHashSet();
            var digimonNames = digimonList.ByType.Values
                .GroupBy(x => NormalizeName(x.Name))
                .ToDictionary(x => x.Key, x => x.Select(y => y.Type).Distinct().ToArray());

            var effects = LoadEffects(directory, out unknownEffectCount);
            effectCount = effects.Count;

            var result = new List<DUnitCollectionGroup>();
            var seenGroupIds = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

            LoadGroupsFromFile(
                directory,
                LinesFileName,
                "Line",
                "Line",
                effects,
                digimonIds,
                digimonNames,
                seenGroupIds,
                result,
                ref totalGroups,
                ref memberCount,
                ref missingDigimonIdCount,
                ref invalidGroups);

            LoadGroupsFromFile(
                directory,
                CollectionsFileName,
                "Collection",
                "Collection",
                effects,
                digimonIds,
                digimonNames,
                seenGroupIds,
                result,
                ref totalGroups,
                ref memberCount,
                ref missingDigimonIdCount,
                ref invalidGroups);

            return result
                .OrderBy(x => x.SourceType)
                .ThenBy(x => x.GroupId)
                .ToList();
        }

        private Dictionary<int, DUnitCollectionEffect> LoadEffects(string directory, out int unknownEffectCount)
        {
            unknownEffectCount = 0;
            var path = Path.Combine(directory, EffectsFileName);
            var result = new Dictionary<int, DUnitCollectionEffect>();

            if (!File.Exists(path))
            {
                _logger.Warning("[DUnit] Missing effect file: {Path}", path);
                return result;
            }

            var document = XDocument.Load(path);
            foreach (var node in document.Descendants("Effect"))
            {
                var effectId = (int?)node.Attribute("effectId");
                if (effectId == null)
                {
                    _logger.Warning("[DUnit] Effect entry without effectId in {Path}", path);
                    continue;
                }

                var rawStat = (string?)node.Attribute("stat");
                if (!DUnitCollectionEffectTypeMapper.TryMap(rawStat, out var effectType))
                {
                    unknownEffectCount++;
                    _logger.Warning("[DUnit] Unknown effect type: {EffectType}. effectId={EffectId}", rawStat, effectId);
                    continue;
                }

                var value = (int?)node.Attribute("value");
                if (value == null)
                {
                    _logger.Warning("[DUnit] Effect value is missing or non-numeric. effectId={EffectId} stat={Stat}", effectId, rawStat);
                    continue;
                }

                if (!AutoApplicableEffects.Contains(effectType))
                {
                    _logger.Warning("[DUnit] Effect type is mapped but not auto-applicable yet: {EffectType}. effectId={EffectId}", rawStat, effectId);
                    continue;
                }

                var percent = string.Equals((string?)node.Attribute("unit"), "percent", StringComparison.OrdinalIgnoreCase);
                result[effectId.Value] = new DUnitCollectionEffect(
                    effectType,
                    value.Value,
                    rawStat,
                    null,
                    (string?)node.Attribute("raw") ?? rawStat ?? effectType.ToString(),
                    percent);
            }

            return result;
        }

        private void LoadGroupsFromFile(
            string directory,
            string fileName,
            string sourceType,
            string elementName,
            IReadOnlyDictionary<int, DUnitCollectionEffect> effects,
            IReadOnlySet<int> digimonIds,
            IReadOnlyDictionary<string, int[]> digimonNames,
            ISet<string> seenGroupIds,
            ICollection<DUnitCollectionGroup> result,
            ref int totalGroups,
            ref int memberCount,
            ref int missingDigimonIdCount,
            ref int invalidGroups)
        {
            var path = Path.Combine(directory, fileName);
            if (!File.Exists(path))
            {
                _logger.Warning("[DUnit] Missing collection file: {Path}", path);
                return;
            }

            var document = XDocument.Load(path);

            foreach (var node in document.Descendants(elementName))
            {
                totalGroups++;
                var groupId = (int?)node.Attribute("id");
                var groupName = ((string?)node.Attribute("name"))?.Trim();
                var groupKey = $"{sourceType}:{groupId}";

                if (groupId == null || string.IsNullOrWhiteSpace(groupName))
                {
                    invalidGroups++;
                    _logger.Warning("[DUnit] Invalid group in {File}: missing id or name.", fileName);
                    continue;
                }

                if (!seenGroupIds.Add(groupKey))
                {
                    invalidGroups++;
                    _logger.Warning("[DUnit] Duplicate GroupId: {GroupKey}", groupKey);
                    continue;
                }

                var requiredDigimons = LoadRequiredDigimons(
                    fileName,
                    groupId.Value,
                    groupName,
                    node,
                    digimonIds,
                    digimonNames,
                    ref memberCount,
                    ref missingDigimonIdCount);

                if (requiredDigimons.Count == 0)
                {
                    invalidGroups++;
                    _logger.Warning("[DUnit] Invalid group {GroupKey} '{GroupName}': no usable required digimons.", groupKey, groupName);
                    continue;
                }

                var groupEffects = LoadGroupEffects(fileName, groupKey, node, effects);
                if (groupEffects.Count == 0)
                {
                    invalidGroups++;
                    _logger.Warning("[DUnit] Invalid group {GroupKey} '{GroupName}': no usable effects.", groupKey, groupName);
                    continue;
                }

                if (groupEffects.Count < 4)
                {
                    _logger.Warning(
                        "[DUnit] Group {GroupKey} '{GroupName}' has less than 4 effects: {EffectCount}",
                        groupKey,
                        groupName,
                        groupEffects.Count);
                }

                var levelReq1 = ReadInt(node, "levelReq1");
                var levelReq2 = ReadInt(node, "levelReq2");
                if (levelReq1 < 0 || levelReq1 > 2000 || levelReq2 < 0 || levelReq2 > 2000)
                {
                    invalidGroups++;
                    _logger.Warning(
                        "[DUnit] Invalid group {GroupKey} '{GroupName}': level requirements out of range. levelReq1={LevelReq1}, levelReq2={LevelReq2}",
                        groupKey,
                        groupName,
                        levelReq1,
                        levelReq2);
                    continue;
                }

                var membersRequired = ReadInt(node, "membersRequired");
                if (membersRequired <= 0)
                    membersRequired = requiredDigimons.Count;

                result.Add(new DUnitCollectionGroup(
                    groupId.Value,
                    groupName,
                    sourceType,
                    requiredDigimons,
                    groupEffects,
                    new DUnitCollectionRequirement(
                        membersRequired,
                        levelReq1,
                        levelReq2,
                        string.Equals((string?)node.Attribute("allTranscended"), "TRUE", StringComparison.OrdinalIgnoreCase)),
                    true,
                    sourceType));
            }
        }

        private List<DUnitRequiredDigimon> LoadRequiredDigimons(
            string fileName,
            int groupId,
            string groupName,
            XElement groupNode,
            IReadOnlySet<int> digimonIds,
            IReadOnlyDictionary<string, int[]> digimonNames,
            ref int memberCount,
            ref int missingDigimonIdCount)
        {
            var result = new List<DUnitRequiredDigimon>();

            foreach (var memberNode in groupNode.Element("Members")?.Elements("Member") ?? Enumerable.Empty<XElement>())
            {
                memberCount++;
                var memberName = ((string?)memberNode.Attribute("name"))?.Trim() ?? string.Empty;
                var resolvedId = ResolveDigimonId(memberNode, digimonIds, digimonNames);

                if (resolvedId == null)
                {
                    missingDigimonIdCount++;
                    _logger.Warning(
                        "[DUnit] DigimonId not found. file={File} groupId={GroupId} group='{GroupName}' member='{MemberName}' iconCandidateId={IconCandidateId}",
                        fileName,
                        groupId,
                        groupName,
                        memberName,
                        (string?)memberNode.Attribute("iconCandidateId") ?? string.Empty);
                    continue;
                }

                var slot = ReadInt(memberNode, "slot");
                result.Add(new DUnitRequiredDigimon(
                    resolvedId.Value,
                    memberName,
                    1,
                    false,
                    0,
                    true,
                    (byte)Math.Clamp(slot, 0, byte.MaxValue)));
            }

            return result;
        }

        private int? ResolveDigimonId(
            XElement memberNode,
            IReadOnlySet<int> digimonIds,
            IReadOnlyDictionary<string, int[]> digimonNames)
        {
            var candidate = (int?)memberNode.Attribute("iconCandidateId");
            if (candidate != null && digimonIds.Contains(candidate.Value))
                return candidate.Value;

            var memberName = NormalizeName((string?)memberNode.Attribute("name"));
            if (!string.IsNullOrWhiteSpace(memberName) &&
                digimonNames.TryGetValue(memberName, out var matches) &&
                matches.Length == 1)
            {
                if (candidate != null)
                {
                    _logger.Warning(
                        "[DUnit] iconCandidateId {IconCandidateId} is not a valid Digimon_List.bin type for '{MemberName}', resolved by name to {ResolvedId}.",
                        candidate.Value,
                        (string?)memberNode.Attribute("name") ?? string.Empty,
                        matches[0]);
                }

                return matches[0];
            }

            return null;
        }

        private List<DUnitCollectionEffect> LoadGroupEffects(
            string fileName,
            string groupKey,
            XElement groupNode,
            IReadOnlyDictionary<int, DUnitCollectionEffect> effects)
        {
            var result = new List<DUnitCollectionEffect>();

            foreach (var rewardNode in groupNode.Element("Rewards")?.Elements("Reward") ?? Enumerable.Empty<XElement>())
            {
                var effectId = (int?)rewardNode.Attribute("effectId");
                if (effectId == null || !effects.TryGetValue(effectId.Value, out var effect))
                {
                    _logger.Warning(
                        "[DUnit] Invalid effect reference. file={File} group={GroupKey} effectId={EffectId}",
                        fileName,
                        groupKey,
                        effectId?.ToString() ?? string.Empty);
                    continue;
                }

                result.Add(effect with
                {
                    RequiredConditionIndex = (int?)rewardNode.Attribute("slot"),
                    Description = (string?)rewardNode.Attribute("raw") ?? effect.Description
                });
            }

            return result;
        }

        private static int ReadInt(XElement node, string attributeName)
            => (int?)node.Attribute(attributeName) ?? 0;

        private static string NormalizeName(string? value)
        {
            if (string.IsNullOrWhiteSpace(value))
                return string.Empty;

            return new string(value
                .ToLowerInvariant()
                .Where(char.IsLetterOrDigit)
                .ToArray());
        }
    }
}
