namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed class DigimonBook
{
    public int OptionCount { get; }
    public int ExceptionCount { get; }
    public int CompositionGroupCount { get; }
    public IReadOnlyDictionary<int, DigimonBookDeckRecord> DecksByGroupId { get; }

    public DigimonBook(
        int optionCount,
        int exceptionCount,
        int compositionGroupCount,
        IReadOnlyDictionary<int, DigimonBookDeckRecord> decksByGroupId)
    {
        OptionCount = optionCount;
        ExceptionCount = exceptionCount;
        CompositionGroupCount = compositionGroupCount;
        DecksByGroupId = decksByGroupId;
    }
}

public sealed record DigimonBookDeckRecord(
    int GroupId,
    string Name,
    IReadOnlyList<DigimonBookDeckEffect> Effects);

public sealed record DigimonBookDeckEffect(
    int Condition,
    int AttackType,
    int Option,
    int Value,
    int Probability,
    int Time);
