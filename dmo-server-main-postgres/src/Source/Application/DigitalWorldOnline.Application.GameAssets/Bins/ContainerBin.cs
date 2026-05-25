using DigitalWorldOnline.Commons.DTOs.Assets;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed record ContainerBin(
    IReadOnlyList<ContainerAssetDTO> Containers,
    int RandomBoxRows,
    int ItemGroupRows,
    int RewardGroupRows,
    int MissingItemGroups,
    int MissingRewardGroups);

