using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.DTOs.Assets;
using MediatR;

namespace DigitalWorldOnline.Application.GameAssets.Queries
{
    /// <summary>
    /// Backed by <c>Buff.bin</c> instead of the DB. Each <c>BuffRecord</c> from the bin maps
    /// 1:1 to a <c>BuffAssetDTO</c>; the only field with no bin source is <c>Name</c> (string,
    /// skipped on the server side per <c>reference_bin_string_framing.md</c>) and the legacy
    /// sequential <c>Id</c> primary key (server consumers don't read it). Records flagged
    /// <c>s_bDelete</c> are usually dropped, except when a consumable buff item references
    /// that skill code. Many v487 EXP/AT/DS booster items point at records flagged deleted
    /// even though the client still expects those item buffs to work.
    /// </summary>
    public class BuffInfoAssetsQueryHandler : IRequestHandler<BuffInfoAssetsQuery, List<BuffAssetDTO>>
    {
        private readonly BuffBinLoader _loader;
        private readonly ItemListBinLoader _itemList;

        public BuffInfoAssetsQueryHandler(BuffBinLoader loader, ItemListBinLoader itemList)
        {
            _loader = loader;
            _itemList = itemList;
        }

        public Task<List<BuffAssetDTO>> Handle(BuffInfoAssetsQuery request, CancellationToken cancellationToken)
        {
            var itemBuffSkillCodes = _itemList.Data.Items
                .Where(item => (item.Type == 63 || item.Type == 64) && item.SkillCode > 0)
                .Select(item => (uint)item.SkillCode)
                .ToHashSet();

            var list = new List<BuffAssetDTO>(_loader.Data.ById.Count);
            foreach (var rec in _loader.Data.ById.Values)
            {
                if (rec.IsDeleted && !itemBuffSkillCodes.Contains(rec.SkillCode))
                    continue;

                list.Add(new BuffAssetDTO
                {
                    BuffId = rec.Id,
                    DigimonSkillCode = (int)rec.DigimonSkillCode,
                    SkillCode = (int)rec.SkillCode,
                    MinLevel = rec.MinLevel,
                    ConditionLevel = rec.ConditionLevel,
                    Class = (short)rec.BuffClass,
                    Type = rec.BuffType,
                    LifeType = rec.LifeType,
                    TimeType = rec.TimeType
                });
            }
            return Task.FromResult(list);
        }
    }
}
