namespace DigitalWorldOnline.Application.GameAssets.Bins;

/// <summary>
/// Parses the legacy client <c>Digimon_Book.bin</c> deck section used by the
/// Encyclopedia deck-effect system. The current converted bin may store the
/// UINT Prob/Time arrays without the C++ struct alignment pad; the loader
/// accepts both layouts and chooses the sane one per deck row.
/// </summary>
public sealed class DigimonBookBinLoader
{
    private const string FileName = "Digimon_Book.bin";
    private const int OptionRecordSize = 4 + 128 + 2 + 1024 + 2;
    private const int ExceptionRecordSize = 4 + 128;
    private const int DeckRecordSize = 1204;
    private const int DeckConditionOffset = 2 + 128 + 1024;
    private const int DeckNoPadProbOffset = DeckConditionOffset + (4 * 3 * 2);
    private const int DeckNoPadTimeOffset = DeckNoPadProbOffset + (3 * 4);
    private const int DeckCppPadProbOffset = DeckNoPadProbOffset + 2;
    private const int DeckCppPadTimeOffset = DeckCppPadProbOffset + (3 * 4);

    private DigimonBook? _data;

    public DigimonBook Data => _data ?? throw new InvalidOperationException(
        $"{nameof(DigimonBookBinLoader)}: bin not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public DigimonBook Load()
    {
        if (_data != null) return _data;

        var path = Path.Combine(BinPath.ResolveDirectory(), FileName);
        using var stream = File.OpenRead(path);
        using var reader = new BinaryReader(stream);
        _data = Parse(reader);
        return _data;
    }

    public static DigimonBook Parse(BinaryReader reader)
    {
        var optionCount = checked((int)reader.ReadUInt32());
        reader.BaseStream.Seek((long)optionCount * OptionRecordSize, SeekOrigin.Current);

        var exceptionCount = checked((int)reader.ReadUInt32());
        reader.BaseStream.Seek((long)exceptionCount * ExceptionRecordSize, SeekOrigin.Current);

        var deckCount = checked((int)reader.ReadUInt32());
        var decks = new Dictionary<int, DigimonBookDeckRecord>(deckCount);

        for (var i = 0; i < deckCount; i++)
        {
            var record = reader.ReadBytes(DeckRecordSize);
            if (record.Length != DeckRecordSize)
                throw new InvalidDataException($"Digimon_Book.bin deck row {i} ended early.");

            var deck = ReadDeckRecord(record);
            decks[deck.GroupId] = deck;
        }

        var compositionGroupCount = checked((int)reader.ReadUInt32());

        return new DigimonBook(optionCount, exceptionCount, compositionGroupCount, decks);
    }

    private static DigimonBookDeckRecord ReadDeckRecord(ReadOnlySpan<byte> record)
    {
        var groupId = BitConverter.ToUInt16(record[0..2]);
        var name = ReadWStringField(record[2..130]);
        var offset = DeckConditionOffset;

        var conditions = ReadUShortArray(record, offset); offset += 3 * 2;
        var attackTypes = ReadUShortArray(record, offset); offset += 3 * 2;
        var options = ReadUShortArray(record, offset); offset += 3 * 2;
        var values = ReadUShortArray(record, offset);

        var noPadProb = ReadUIntArray(record, DeckNoPadProbOffset);
        var noPadTime = ReadUIntArray(record, DeckNoPadTimeOffset);
        var cppPadProb = ReadUIntArray(record, DeckCppPadProbOffset);
        var cppPadTime = ReadUIntArray(record, DeckCppPadTimeOffset);

        var useNoPad = IsTimingLayoutSane(noPadProb, noPadTime) && !IsTimingLayoutSane(cppPadProb, cppPadTime);
        var probabilities = useNoPad ? noPadProb : cppPadProb;
        var times = useNoPad ? noPadTime : cppPadTime;

        var effects = new List<DigimonBookDeckEffect>(3);
        for (var i = 0; i < 3; i++)
        {
            if (conditions[i] == 0 && options[i] == 0)
                continue;

            effects.Add(new DigimonBookDeckEffect(
                conditions[i],
                attackTypes[i],
                options[i],
                values[i],
                checked((int)probabilities[i]),
                checked((int)times[i])));
        }

        return new DigimonBookDeckRecord(groupId, name, effects);
    }

    private static ushort[] ReadUShortArray(ReadOnlySpan<byte> record, int offset)
    {
        return new[]
        {
            BitConverter.ToUInt16(record[offset..(offset + 2)]),
            BitConverter.ToUInt16(record[(offset + 2)..(offset + 4)]),
            BitConverter.ToUInt16(record[(offset + 4)..(offset + 6)])
        };
    }

    private static uint[] ReadUIntArray(ReadOnlySpan<byte> record, int offset)
    {
        return new[]
        {
            BitConverter.ToUInt32(record[offset..(offset + 4)]),
            BitConverter.ToUInt32(record[(offset + 4)..(offset + 8)]),
            BitConverter.ToUInt32(record[(offset + 8)..(offset + 12)])
        };
    }

    private static bool IsTimingLayoutSane(IReadOnlyList<uint> probabilities, IReadOnlyList<uint> times)
    {
        for (var i = 0; i < probabilities.Count; i++)
        {
            if (probabilities[i] > 10000)
                return false;
            if (times[i] > 86400)
                return false;
        }

        return true;
    }

    private static string ReadWStringField(ReadOnlySpan<byte> field)
    {
        var charLen = 0;
        for (var i = 0; i < field.Length; i += 2)
        {
            if (field[i] == 0 && field[i + 1] == 0) break;
            charLen++;
        }

        return System.Text.Encoding.Unicode.GetString(field[..(charLen * 2)]);
    }
}
