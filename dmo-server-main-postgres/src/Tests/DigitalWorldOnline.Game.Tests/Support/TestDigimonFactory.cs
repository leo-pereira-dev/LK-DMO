using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;

namespace DigitalWorldOnline.Game.Tests.Support;

internal static class TestDigimonFactory
{
    public static CharacterModel CharacterWithSlots(byte openedSlots)
    {
        var character = new CharacterModel();
        character.AddDigimonSlots(openedSlots);
        return character;
    }

    public static DigimonModel Digimon(long id, byte slot, string name = "Mockmon", int type = 31001, byte level = 120)
    {
        var digimon = DigimonModel.Create(name, type, type, DigimonHatchGradeEnum.Perfect, 12500, slot);
        digimon.SetId(id);
        digimon.SetLevel(level);
        return digimon;
    }

    public static CharacterDigimonArchiveItemModel EmptyArchiveSlot(int slot)
    {
        return new CharacterDigimonArchiveItemModel(slot);
    }
}
