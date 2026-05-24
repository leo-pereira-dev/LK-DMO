using DigitalWorldOnline.Commons.Models.Character;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Game.Tests.Support;
using Xunit;
using Xunit.Abstractions;

namespace DigitalWorldOnline.Game.Tests.Archive;

public sealed class ArchiveSlotContractTests
{
    private readonly ITestOutputHelper _output;

    public ArchiveSlotContractTests(ITestOutputHelper output)
    {
        _output = output;
    }

    [Theory]
    [InlineData(0, 0)]
    [InlineData(1, 1)]
    [InlineData(2, 2)]
    [InlineData(8, 8)]
    public void ClientDigiviceSlot_matches_active_zero_and_mercenary_slots_for_archive_3201(int clientSlot, int expectedServerSlot)
    {
        var resolved = Archive3201Harness.ResolveDigiviceSlot(clientSlot);

        _output.WriteLine($"3201 slot resolve: client={clientSlot}, server={resolved}");

        Assert.Equal(expectedServerSlot, resolved);
    }

    [Fact]
    public void First_mercenary_slot_does_not_resolve_to_active_partner()
    {
        var character = TestDigimonFactory.CharacterWithSlots(3);
        var active = TestDigimonFactory.Digimon(10, 0, "Active");
        var raremon = TestDigimonFactory.Digimon(11, 1, "Raremon");

        character.AddDigimon(active);
        character.AddDigimon(raremon);

        var resolvedSlot = Archive3201Harness.ResolveDigiviceSlot(1);
        var resolvedPartner = character.Digimons.FirstOrDefault(x => x.Slot == resolvedSlot);

        _output.WriteLine(
            $"3201 first mercenary resolve: client=1, server={resolvedSlot}, partner={resolvedPartner?.Id}:{resolvedPartner?.Name}");

        Assert.Equal(1, resolvedSlot);
        Assert.Same(raremon, resolvedPartner);
        Assert.NotSame(active, resolvedPartner);
    }

    [Fact]
    public void Active_partner_can_be_archived_when_another_active_digimon_can_be_promoted()
    {
        var character = TestDigimonFactory.CharacterWithSlots(3);
        var active = TestDigimonFactory.Digimon(10, 0, "Active");
        var mercenary = TestDigimonFactory.Digimon(11, 1, "Mercenary");
        var archiveSlot = TestDigimonFactory.EmptyArchiveSlot(5);

        character.AddDigimon(active);
        character.AddDigimon(mercenary);

        var shouldBlock = Archive3201Harness.ShouldBlockMoveToEmptyArchive(character, active, archiveSlot);
        Assert.False(shouldBlock);

        Archive3201Harness.MovePartnerToArchive(character, active, archiveSlot);

        _output.WriteLine(
            $"after archive active: archived={archiveSlot.DigimonId}, activeSlot={active.Slot}, promoted={mercenary.Id}:{mercenary.Slot}");

        Assert.Equal(active.Id, archiveSlot.DigimonId);
        Assert.Equal(byte.MaxValue, active.Slot);
        Assert.Single(character.Digimons);
        Assert.Equal(0, mercenary.Slot);
    }

    [Fact]
    public void Last_active_partner_is_blocked_from_empty_archive_slot()
    {
        var character = TestDigimonFactory.CharacterWithSlots(3);
        var active = TestDigimonFactory.Digimon(10, 0, "OnlyActive");
        var archiveSlot = TestDigimonFactory.EmptyArchiveSlot(5);

        character.AddDigimon(active);

        var shouldBlock = Archive3201Harness.ShouldBlockMoveToEmptyArchive(character, active, archiveSlot);

        _output.WriteLine($"last active archive attempt: activeCount={character.Digimons.Count}, shouldBlock={shouldBlock}");

        Assert.True(shouldBlock);
    }

    [Fact]
    public void Restoring_archived_digimon_uses_next_active_slot_after_compaction()
    {
        var character = TestDigimonFactory.CharacterWithSlots(4);
        var first = TestDigimonFactory.Digimon(10, 0, "First");
        var third = TestDigimonFactory.Digimon(12, 2, "Third");
        var archived = TestDigimonFactory.Digimon(20, byte.MaxValue, "Archived");
        var archiveSlot = TestDigimonFactory.EmptyArchiveSlot(2);
        archiveSlot.AddDigimon(archived);

        character.AddDigimon(first);
        character.AddDigimon(third);

        character.CompactActiveDigimonSlots();
        var restoreSlot = character.GetNextActiveDigimonSlot();
        archived.SetSlot(restoreSlot);
        archiveSlot.RemoveDigimon();
        character.AddDigimon(archived);

        _output.WriteLine($"restore archive: first={first.Slot}, third={third.Slot}, restored={archived.Slot}");

        Assert.Equal(0, first.Slot);
        Assert.Equal(1, third.Slot);
        Assert.Equal(2, archived.Slot);
        Assert.Equal(0, archiveSlot.DigimonId);
    }

    private static class Archive3201Harness
    {
        public static int ResolveDigiviceSlot(int clientDigiviceSlot)
        {
            return clientDigiviceSlot;
        }

        public static bool ShouldBlockMoveToEmptyArchive(
            CharacterModel character,
            DigimonModel digivicePartner,
            CharacterDigimonArchiveItemModel archivePartner)
        {
            var activeCount = character.Digimons.Count(x =>
                x.Slot != byte.MaxValue &&
                x.Slot < character.DigimonSlots);

            return digivicePartner.Slot == 0 &&
                   archivePartner.DigimonId == 0 &&
                   activeCount <= 1;
        }

        public static void MovePartnerToArchive(
            CharacterModel character,
            DigimonModel digivicePartner,
            CharacterDigimonArchiveItemModel archivePartner)
        {
            archivePartner.AddDigimon(digivicePartner);
            character.RemoveDigimon(digivicePartner.Slot);
            digivicePartner.SetSlot(byte.MaxValue);
        }
    }
}
