using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Constants;
using DigitalWorldOnline.Commons.DTOs.Assets;
using DigitalWorldOnline.Commons.Enums;
using MediatR;

namespace DigitalWorldOnline.Application.GameAssets.Queries
{
    /// <summary>
    /// Backed by <c>DMBase.bin</c> section 1 (1440 rows = 12 tamer models × 120 levels) instead
    /// of the DB. The bin's <c>s_dwID</c> encodes <c>(model − 80000) × 1000 + level</c>; we
    /// project it back into the DTO's <c>Type</c> + <c>Level</c>.
    /// </summary>
    public class TamerLevelingAssetsQueryHandler : IRequestHandler<TamerLevelingAssetsQuery, List<CharacterLevelStatusAssetDTO>>
    {
        private readonly DMBaseBinLoader _dmBase;

        public TamerLevelingAssetsQueryHandler(DMBaseBinLoader dmBase)
        {
            _dmBase = dmBase;
        }

        public Task<List<CharacterLevelStatusAssetDTO>> Handle(TamerLevelingAssetsQuery request, CancellationToken cancellationToken)
        {
            var rows = _dmBase.Data.TamerStats;
            var fallbackExpByLevel = _dmBase.Data.DigimonStats.Values
                .GroupBy(x => x.Level)
                .ToDictionary(x => x.Key, x => x.OrderBy(y => y.Id).First().Exp);

            var list = new List<CharacterLevelStatusAssetDTO>(rows.Count);
            foreach (var rec in rows.Values)
            {
                int tamerModel = (rec.Id / 1000) + 80000;
                list.Add(new CharacterLevelStatusAssetDTO
                {
                    Type = (CharacterModelEnum)tamerModel,
                    Level = (byte)rec.Level,
                    // Bin's ExpValue is in WIRE units (100× the real threshold). The v487
                    // client multiplies by 0.01 when reading (FmTamer.cpp:169 —
                    // `s_dwExp * 0.01f`), so its on-screen threshold = bin_value / 100. The
                    // server's level-up comparison runs against `tamer.CurrentExperience`
                    // which is stored in real units in the DB (and is what the OLD pre-bin
                    // path compared against the DB's already-divided Asset_CharacterLevelStatus
                    // value). Without /100 here, the server compares e.g. 8010 < 13500 and
                    // never levels up while the client UI shows 5xxx% complete (8010 / 135).
                    ExpValue = ResolveTamerExpValue(rec, fallbackExpByLevel),
                    HPValue = rec.HP,
                    DSValue = rec.DS,
                    MSValue = rec.MoveSpeed,
                    DEValue = rec.Defence,
                    EVValue = rec.Evasion,
                    CTValue = rec.Critical,
                    ATValue = rec.Attack,
                    HTValue = rec.HitRate
                });
            }
            ExtendSyntheticLevels(list);

            return Task.FromResult(list);
        }

        private static long ResolveTamerExpValue(
            DMBaseStatRecord rec,
            IReadOnlyDictionary<ushort, long> fallbackExpByLevel)
        {
            var wireExp = rec.Exp;
            if (wireExp <= 0 && fallbackExpByLevel.TryGetValue(rec.Level, out var fallbackWireExp))
                wireExp = fallbackWireExp;

            return Math.Max(1L, wireExp / 100);
        }

        private static void ExtendSyntheticLevels(List<CharacterLevelStatusAssetDTO> list)
        {
            foreach (var group in list.GroupBy(x => x.Type).ToList())
            {
                var ordered = group.OrderBy(x => x.Level).ToList();
                if (!ordered.Any() || ordered.Last().Level >= LevelConstants.MaxLevel)
                    continue;

                var last = ordered.Last();
                var previous = ordered.Count > 1 ? ordered[^2] : last;

                for (var level = last.Level + 1; level <= LevelConstants.MaxLevel; level++)
                {
                    var offset = level - last.Level;

                    list.Add(new CharacterLevelStatusAssetDTO
                    {
                        Id = last.Id + offset,
                        Type = last.Type,
                        Level = (byte)level,
                        ExpValue = last.ExpValue,
                        HPValue = Extrapolate(last.HPValue, previous.HPValue, offset),
                        DSValue = Extrapolate(last.DSValue, previous.DSValue, offset),
                        MSValue = last.MSValue,
                        DEValue = Extrapolate(last.DEValue, previous.DEValue, offset),
                        EVValue = Extrapolate(last.EVValue, previous.EVValue, offset),
                        CTValue = Extrapolate(last.CTValue, previous.CTValue, offset),
                        ATValue = Extrapolate(last.ATValue, previous.ATValue, offset),
                        HTValue = Extrapolate(last.HTValue, previous.HTValue, offset),
                        ASValue = last.ASValue,
                        ARValue = last.ARValue,
                        BLValue = last.BLValue,
                        WSValue = last.WSValue
                    });
                }
            }
        }

        private static int Extrapolate(int last, int previous, int offset)
        {
            return last + Math.Max(0, last - previous) * offset;
        }
    }
}
