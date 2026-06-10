using DigitalWorldOnline.Commons.DTOs.Assets;
using DigitalWorldOnline.Commons.Models.Digimon;
using Xunit;

namespace DigitalWorldOnline.Game.Tests.Evolution;

public sealed class EvolutionAutoUnlockTests
{
    [Fact]
    public void AddEvolutions_OnlyAutoUnlocksNormalLineThroughUltimate()
    {
        var digimon = new DigimonModel();
        var evolution = new EvolutionAssetDTO
        {
            Type = 31001,
            Lines = new List<EvolutionLineAssetDTO>
            {
                Line(31001, 1),
                Line(31002, 2),
                Line(31003, 3),
                Line(31004, 4),
                Line(31005, 5)
            }
        };

        digimon.AddEvolutions(evolution);

        Assert.Equal(new byte[] { 1, 1, 1, 0, 0 }, digimon.Evolutions.Select(x => x.Unlocked).ToArray());
    }

    private static EvolutionLineAssetDTO Line(int type, byte slotLevel) => new()
    {
        Type = type,
        SlotLevel = slotLevel,
        Stages = new List<EvolutionStageAssetDTO>()
    };
}
