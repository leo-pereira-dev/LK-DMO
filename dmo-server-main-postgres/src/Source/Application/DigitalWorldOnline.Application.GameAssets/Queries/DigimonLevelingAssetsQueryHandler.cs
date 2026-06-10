using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Constants;
using DigitalWorldOnline.Commons.DTOs.Assets;
using MediatR;

namespace DigitalWorldOnline.Application.GameAssets.Queries
{
    /// <summary>
    /// Backed by <c>DMBase.bin</c> section 2 (480 rows = 4 digimon ranks × 120 levels) instead
    /// of the DB. The bin's <c>s_dwID</c> encodes <c>digimonRank * 1000 + level</c>; we expose
    /// the rank in the DTO's <c>Type</c> field — Game.Host's level-up math joins it with the
    /// per-digimon base info from <c>Digimon_List.bin</c> (which carries the rank) at runtime.
    /// </summary>
    public class DigimonLevelingAssetsQueryHandler : IRequestHandler<DigimonLevelingAssetsQuery, List<DigimonLevelStatusAssetDTO>>
    {
        private readonly DMBaseBinLoader _dmBase;

        public DigimonLevelingAssetsQueryHandler(DMBaseBinLoader dmBase)
        {
            _dmBase = dmBase;
        }

        public Task<List<DigimonLevelStatusAssetDTO>> Handle(DigimonLevelingAssetsQuery request, CancellationToken cancellationToken)
        {
            var rows = _dmBase.Data.DigimonStats;
            var list = new List<DigimonLevelStatusAssetDTO>(rows.Count);
            foreach (var rec in rows.Values)
            {
                int rank = rec.Id / 1000;
                list.Add(new DigimonLevelStatusAssetDTO
                {
                    Type = rank,
                    // ScaleType is queried by ExpManager (`x.ScaleType == digimon.BaseInfo.ScaleType`)
                    // and equals the digimon rank (1..4) in v487 — see DigimonListBinLoader for
                    // why ScaleType ≡ s_nDigimonType, not s_dwCharSize.
                    ScaleType = rank,
                    // StatusId is queried by StatusManager.GetDigimonBaseStatus
                    // (`x.StatusId == level + ScaleType * 1000`) which equals the bin's record key.
                    // Without this, .Single() throws and InitialInformationPacketProcessor hangs
                    // (player loading screen stuck after character select).
                    StatusId = rec.Id,
                    Level = (byte)rec.Level,
                    // Bin's ExpValue is in WIRE units (100× the real threshold) — same
                    // convention as TamerLevelingAssetsQueryHandler. Divide here so the
                    // server's level-up comparison against `digimon.CurrentExperience`
                    // (stored real-units) matches DB-era behavior.
                    ExpValue = rec.Exp / 100,
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

        private static void ExtendSyntheticLevels(List<DigimonLevelStatusAssetDTO> list)
        {
            foreach (var group in list.GroupBy(x => x.ScaleType).ToList())
            {
                var ordered = group.OrderBy(x => x.Level).ToList();
                if (!ordered.Any() || ordered.Last().Level >= LevelConstants.MaxLevel)
                    continue;

                var last = ordered.Last();
                var previous = ordered.Count > 1 ? ordered[^2] : last;

                for (var level = last.Level + 1; level <= LevelConstants.MaxLevel; level++)
                {
                    var offset = level - last.Level;

                    list.Add(new DigimonLevelStatusAssetDTO
                    {
                        Id = last.Id + offset,
                        Type = last.Type,
                        ScaleType = last.ScaleType,
                        StatusId = last.ScaleType * 1000 + level,
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
