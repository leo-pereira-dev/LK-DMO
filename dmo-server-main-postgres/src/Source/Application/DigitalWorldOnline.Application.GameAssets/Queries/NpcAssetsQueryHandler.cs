using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.DTOs.Assets;
using MediatR;

namespace DigitalWorldOnline.Application.GameAssets.Queries
{
    public class NpcAssetsQueryHandler : IRequestHandler<NpcAssetsQuery, List<NpcAssetDTO>>
    {
        private readonly NpcBinLoader _npcBinLoader;

        public NpcAssetsQueryHandler(NpcBinLoader npcBinLoader)
        {
            _npcBinLoader = npcBinLoader;
        }

        public async Task<List<NpcAssetDTO>> Handle(NpcAssetsQuery request, CancellationToken cancellationToken)
        {
            return await Task.FromResult(_npcBinLoader.Load().Npcs.ToList());
        }
    }
}
