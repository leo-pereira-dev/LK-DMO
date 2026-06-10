using DigitalWorldOnline.Commons.DTOs.Assets;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed record NpcBin(IReadOnlyList<NpcAssetDTO> Npcs);
