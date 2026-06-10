using AutoMapper;
using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Commons.DTOs.Assets;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Assets;
using DigitalWorldOnline.Commons.Models.Summon;
using MediatR;
using System.Numerics;
using System.Reflection.Metadata.Ecma335;

namespace DigitalWorldOnline.Application.GameAssets
{
    public class AssetsLoader
    {
        private readonly ISender _sender;
        private readonly IMapper _mapper;
        private readonly MonsterBinLoader _monster;
        private readonly QuestBinLoader _questBin;
        private readonly ContainerBinLoader _containerBin;
        private readonly ExtraExchangeBinLoader _extraExchangeBin;
        private readonly UnionXmlAssetLoader _xmlUnion;
        private bool? _loading;

        /// <summary>Catalog access for runtime mob factory (SUMMON_MONSTER, CALL_UP, SummonPos).</summary>
        public MonsterBinLoader Monster => _monster;
        public UnionXmlAssetLoader XmlUnion => _xmlUnion;

        public bool Loading => _loading == null || _loading.Value;

        public List<ItemAssetModel> ItemInfo { get; private set; }
        public List<SummonModel> SummonInfo { get; private set; }
        public List<CharacterLevelStatusAssetModel> TamerLevelInfo { get; private set; }
        // Retired: see InitialInformationPacketProcessor / StatusManager — per-model baseline
        // is no longer needed; DMBase.bin §1 (TamerLevelInfo) carries the full per-level stats.
        //public List<CharacterBaseStatusAssetModel> TamerBaseInfo { get; private set; }
        public List<DigimonLevelStatusAssetModel> DigimonLevelInfo { get; private set; }
        public List<DigimonBaseInfoAssetModel> DigimonBaseInfo { get; private set; }
        public List<DigimonSkillAssetModel> DigimonSkillInfo { get; private set; }
        public List<MonsterSkillAssetModel> MonsterSkill { get; private set; }
        public List<SkillCodeAssetModel> SkillCodeInfo { get; private set; }
        public List<SkillInfoAssetModel> SkillInfo { get; private set; }
        public List<MonsterSkillInfoAssetModel> MonsterSkillInfo { get; private set; }
        public List<MonthlyEventAssetModel> MonthlyEvents { get; private set; }
        public List<AchievementAssetModel> AchievementAssets { get; private set; }
        public List<SealDetailAssetModel> SealInfo { get; private set; }
        public List<EvolutionAssetModel> EvolutionInfo { get; private set; }
        public List<BuffInfoAssetModel> BuffInfo { get; private set; }
        public List<ScanDetailAssetModel> ScanDetail { get; private set; }
        public List<ContainerAssetModel> Container { get; private set; }
        public List<StatusApplyAssetModel> StatusApply { get; private set; }
        public List<TitleStatusAssetModel> TitleStatus { get; private set; }
        public List<AccessoryRollAssetModel> AccessoryRoll { get; private set; }
        public List<PortalAssetModel> Portal { get; private set; }
        public List<HatchAssetModel> Hatchs { get; private set; }
        public List<QuestAssetModel> Quest { get; private set; }
        public List<int> QuestItemList { get; private set; }
        public Dictionary<int, List<int>> QuestLootItemDropsByMob { get; private set; }
        public List<short> DailyQuestList { get; private set; }
        public List<MapAssetModel> Maps { get; private set; }
        public List<CloneAssetModel> Clones { get; private set; }
        public List<CloneValueAssetModel> CloneValues { get; private set; }
        public List<TamerSkillAssetModel> TamerSkills { get; private set; }
        public List<TamerSkillAreaCheckAssetModel> TamerSkillAreaChecks { get; private set; }
        public List<NpcAssetModel> Npcs { get; private set; }
        public List<NpcColiseumAssetModel> NpcColiseum { get; private set; }
        public List<ArenaRankingDailyItemRewardsModel> ArenaRankingDailyItemRewards { get; private set; }
        public List<EvolutionArmorAssetModel> EvolutionsArmor { get; private set; }
        public List<ExtraEvolutionNpcAssetModel> ExtraEvolutions { get; private set; }
      
        public AssetsLoader(
            ISender sender,
            IMapper mapper,
            MonsterBinLoader monster,
            QuestBinLoader questBin,
            ContainerBinLoader containerBin,
            ExtraExchangeBinLoader extraExchangeBin,
            UnionXmlAssetLoader xmlUnion)
        {
            _sender = sender;
            _mapper = mapper;
            _monster = monster;
            _questBin = questBin;
            _containerBin = containerBin;
            _extraExchangeBin = extraExchangeBin;
            _xmlUnion = xmlUnion;
        }

        public AssetsLoader Load()
        {
            Task.Run(LoadAssets);

            return this;
        }

        private async Task LoadAssets()
        {
            if (_loading != null)
                return;

            _loading = true;
            _xmlUnion.Load();

            ItemInfo = _mapper.Map<List<ItemAssetModel>>(await _sender.Send(new ItemAssetsQuery()));
            SummonInfo = _mapper.Map<List<SummonModel>>(await _sender.Send(new SummonAssetsQuery()));
            SkillCodeInfo = _mapper.Map<List<SkillCodeAssetModel>>(await _sender.Send(new SkillCodeAssetsQuery()));
            TamerLevelInfo = _mapper.Map<List<CharacterLevelStatusAssetModel>>(await _sender.Send(new TamerLevelingAssetsQuery()));
            //TamerBaseInfo = _mapper.Map<List<CharacterBaseStatusAssetModel>>(await _sender.Send(new TamerBaseStatusAssetsQuery()));
            DigimonLevelInfo = _mapper.Map<List<DigimonLevelStatusAssetModel>>(await _sender.Send(new DigimonLevelingAssetsQuery()));
            DigimonBaseInfo = _mapper.Map<List<DigimonBaseInfoAssetModel>>(await _sender.Send(new AllDigimonBaseInfoQuery()));
            SkillInfo = _mapper.Map<List<SkillInfoAssetModel>>(await _sender.Send(new SkillInfoAssetsQuery()));
            DigimonSkillInfo = _mapper.Map<List<DigimonSkillAssetModel>>(await _sender.Send(new DigimonSkillAssetsQuery()));
            MonsterSkill = _mapper.Map<List<MonsterSkillAssetModel>>(await _sender.Send(new MonsterSkillAssetsQuery()));
            MonsterSkillInfo = _mapper.Map<List<MonsterSkillInfoAssetModel>>(await _sender.Send(new MonsterSkillInfoAssetsQuery()));
            SealInfo = _mapper.Map<List<SealDetailAssetModel>>(await _sender.Send(new SealStatusAssetsQuery()));
            EvolutionInfo = _mapper.Map<List<EvolutionAssetModel>>(await _sender.Send(new DigimonEvolutionAssetsQuery()));
            BuffInfo = _mapper.Map<List<BuffInfoAssetModel>>(await _sender.Send(new BuffInfoAssetsQuery()));
            ScanDetail = _mapper.Map<List<ScanDetailAssetModel>>(await _sender.Send(new ScanDetailAssetQuery()));
            Container = _mapper.Map<List<ContainerAssetModel>>(_containerBin.Load().Containers);
            StatusApply = _mapper.Map<List<StatusApplyAssetModel>>(await _sender.Send(new StatusApplyAssetQuery()));
            TitleStatus = _mapper.Map<List<TitleStatusAssetModel>>(await _sender.Send(new AllTitleStatusAssetsQuery()));
            AccessoryRoll = _mapper.Map<List<AccessoryRollAssetModel>>(await _sender.Send(new AccessoryRollAssetsQuery()));
            Portal = _mapper.Map<List<PortalAssetModel>>(await _sender.Send(new PortalAssetsQuery()));
            Npcs = _mapper.Map<List<NpcAssetModel>>(await _sender.Send(new NpcAssetsQuery()));
            NpcColiseum = _mapper.Map<List<NpcColiseumAssetModel>>(await _sender.Send(new NpcColiseumAssetsQuery()));
            Quest = _questBin.Load().Quests.ToList();
            QuestLootItemDropsByMob = BuildQuestLootItemDropsByMob(Quest);
            Hatchs = _mapper.Map<List<HatchAssetModel>>(await _sender.Send(new HatchAssetsQuery()));
            EnsureTutorialHatchAssets();
            Maps = _mapper.Map<List<MapAssetModel>>(await _sender.Send(new MapAssetsQuery()));
            Clones = _mapper.Map<List<CloneAssetModel>>(await _sender.Send(new CloneAssetsQuery()));
            CloneValues = _mapper.Map<List<CloneValueAssetModel>>(await _sender.Send(new CloneValueAssetsQuery()));
            TamerSkills = _mapper.Map<List<TamerSkillAssetModel>>(await _sender.Send(new TamerSkillAssetsQuery()));
            TamerSkillAreaChecks = _mapper.Map<List<TamerSkillAreaCheckAssetModel>>(await _sender.Send(new TamerSkillAreaChecksQuery()));
            MonthlyEvents = _mapper.Map<List<MonthlyEventAssetModel>>(await _sender.Send(new MonthlyEventAssetsQuery()));
            AchievementAssets = _mapper.Map<List<AchievementAssetModel>>(await _sender.Send(new AchievementAssetsQuery()));
            ArenaRankingDailyItemRewards = _mapper.Map<List<ArenaRankingDailyItemRewardsModel>>(await _sender.Send(new ArenaRankingDailyItemRewardsQuery()));
            EvolutionsArmor = _mapper.Map<List<EvolutionArmorAssetModel>>(await _sender.Send(new EvolutionArmorAssetsQuery()));
            var extraEvolutionsFromBin = _extraExchangeBin.Load();
            ExtraEvolutions = extraEvolutionsFromBin.Any()
                ? extraEvolutionsFromBin.ToList()
                : _mapper.Map<List<ExtraEvolutionNpcAssetModel>>(await _sender.Send(new ExtraEvolutionNpcAssetQuery()));
            ItemInfo.ForEach(item => { item.SetSkillInfo(SkillCodeInfo.FirstOrDefault(x => x.SkillCode == item.SkillCode)); });
            TamerEquipmentUpgradeStageRegistry.ReplaceRules(SkillCodeInfo);
            EquipmentSetBonusRegistry.ReplaceRules(EquipmentSetBonusRegistry.BuildFrom(ItemInfo, SkillCodeInfo, SkillInfo, BuffInfo));
            BuffInfo.ForEach(buff => { buff.SetSkillInfo(SkillCodeInfo.FirstOrDefault(x => x.SkillCode == buff.SkillCode || x.SkillCode == buff.DigimonSkillCode)); });
            DigimonSkillInfo.ForEach(skill => { skill.SetSkillInfo(SkillInfo.FirstOrDefault(x => x.SkillId == skill.SkillId)); });
            MonsterSkill.ForEach(skill => { skill.SetSkillInfo(MonsterSkillInfo.FirstOrDefault(x => x.Type == skill.Type && x.SkillId == skill.SkillId)); });

            // Terms join — resolve each skill's RangeId against Monster.bin §4 so the
            // dispatcher can read the bin's actual AoE radius / shape instead of falling
            // back to hardcoded constants.  Zero RangeId stays at zero (means no Terms).
            if (_monster?.IsLoaded == true)
            {
                var terms = _monster.Data.TermsByIndex;
                foreach (var s in MonsterSkillInfo)
                {
                    if (s.RangeId == 0) continue;
                    if (!terms.TryGetValue(s.RangeId, out var t)) continue;
                    s.RangeUnits = t.Range;
                    s.RangeDirection = t.Direction;
                    s.RangeTargetingType = t.TargetingType;
                    s.RangeRefCode = t.RefCode;
                }
            }
            SealInfo = SealInfo.OrderByDescending(x => x.RequiredAmount).ToList();
            QuestItemList = ItemInfo.Where(x => x.Type == 80 || x.Type == 85).Select(x => x.ItemId).ToList();
            DailyQuestList = Quest.Where(x => x.QuestType == QuestTypeEnum.DailyQuest).Select(x => (short)x.QuestId).ToList();


            _loading = false;
        }

        private void EnsureTutorialHatchAssets()
        {
            var tutorialHatches = new[]
            {
                new { ItemId = 114240, SourceItemId = 113561 },
                new { ItemId = 114241, SourceItemId = 113874 },
                new { ItemId = 114242, SourceItemId = 113810 },
                new { ItemId = 114243, SourceItemId = 113534 },
                new { ItemId = 114244, SourceItemId = 113628 },
                new { ItemId = 114245, SourceItemId = 113687 }
            };

            var nextId = Hatchs.Count == 0 ? 1 : Hatchs.Max(x => x.Id) + 1;
            foreach (var tutorialHatch in tutorialHatches)
            {
                if (Hatchs.Any(x => x.ItemId == tutorialHatch.ItemId))
                    continue;

                var source = Hatchs.FirstOrDefault(x => x.ItemId == tutorialHatch.SourceItemId);
                if (source == null)
                    continue;

                Hatchs.Add(_mapper.Map<HatchAssetModel>(new HatchAssetDTO
                {
                    Id = nextId++,
                    ItemId = tutorialHatch.ItemId,
                    HatchType = source.HatchType,
                    LowClassDataSection = source.LowClassDataSection,
                    MidClassDataSection = source.MidClassDataSection,
                    LowClassDataAmount = source.LowClassDataAmount,
                    MidClassDataAmount = source.MidClassDataAmount,
                    LowClassBreakPoint = source.LowClassBreakPoint,
                    MidClassBreakPoint = source.MidClassBreakPoint
                }));
            }
        }

        private static Dictionary<int, List<int>> BuildQuestLootItemDropsByMob(IEnumerable<QuestAssetModel> quests)
        {
            var result = new Dictionary<int, List<int>>();

            foreach (var quest in quests)
            {
                foreach (var questGoal in quest.QuestGoals)
                {
                    if (questGoal.GoalType != QuestGoalTypeEnum.LootItem)
                        continue;

                    if (questGoal.GoalId <= 0 || questGoal.SubValue <= 0)
                        continue;

                    if (!result.TryGetValue(questGoal.SubValue, out var itemIds))
                    {
                        itemIds = new List<int>();
                        result.Add(questGoal.SubValue, itemIds);
                    }

                    if (!itemIds.Contains(questGoal.GoalId))
                        itemIds.Add(questGoal.GoalId);
                }
            }

            return result;
        }
    }
   
}
