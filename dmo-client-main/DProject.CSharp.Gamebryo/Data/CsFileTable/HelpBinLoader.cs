using DProject.Data.Binary;
using DProject.Interface.Base;
using System.Text;

namespace DProject.Data.CsFileTable;

public enum CsHelpRequireType
{
    RT_NONE,
    RT_TLEVEL,
    RT_QUEST_RECV,
    RT_MAP_ARRIVE,
    RT_PARTNERMON_LEVEL
}

public enum CsHelpTextType
{
    TT_NORMAL,
    TT_LINK
}

public enum CsHelpImageType
{
    IT_NORMAL,
    IT_VIDEO
}

public sealed record HelpTitleRecord(
    uint UniqueId,
    string Title,
    CsHelpRequireType RequireType,
    int ReqId1,
    int ReqId2);

public sealed record HelpTextRunRecord(
    CsHelpTextType TextType,
    uint TextTypeValue1,
    uint TextTypeValue2,
    CsRect Region,
    byte Red,
    byte Green,
    byte Blue,
    short TextSize,
    short Bold,
    string Text);

public sealed record HelpStringRecord(
    IReadOnlyList<HelpTextRunRecord> Texts,
    CsPoint Position,
    int LineDeltaHeight);

public sealed record HelpImageRecord(
    CsHelpImageType ImageType,
    CsPoint Position,
    CsPoint Size,
    string ImagePath);

public sealed record HelpContainerRecord(
    int LimitScroll,
    IReadOnlyList<HelpStringRecord> Strings,
    IReadOnlyList<HelpImageRecord> Images);

public sealed record HelpRecord(HelpTitleRecord Title, HelpContainerRecord Body);

public sealed record HelpBinData(
    uint LastTitleUniqueId,
    IReadOnlyList<HelpRecord> Records);

public sealed class HelpBinLoader : IBinLoader
{
    private const int TitleCharCount = 64;
    private const int MaxReasonableCount = 100000;
    private const int MaxReasonableTextLength = 1_000_000;

    public string TableName => "CsHelpMng";

    public BinLoadResult Load(string languagePath, eFILE_TYPE fileType, bool allowMissingTables)
    {
        string binPath = ResolveHelpPath();
        if (!TryReadHelpData(binPath, fileType, out byte[] data))
        {
            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, 0)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, 0);
        }

        if (!TryParse(data, out HelpBinData? parsed, out int bytesConsumed) || parsed is null)
        {
            var failedSnapshot = new BinFileSnapshot(
                TableName,
                binPath,
                data.Length,
                [new BinSectionSnapshot("HelpRecords", 0, 0, null, bytesConsumed, false)]);

            return allowMissingTables
                ? new BinLoadResult(BinLoadStatus.Placeholder, binPath, data.Length, failedSnapshot)
                : new BinLoadResult(BinLoadStatus.Missing, binPath, data.Length, failedSnapshot);
        }

        var snapshot = new BinFileSnapshot(
            TableName,
            binPath,
            data.Length,
            [new BinSectionSnapshot("HelpRecords", sizeof(uint), parsed.Records.Count, null, bytesConsumed, bytesConsumed <= data.Length)]);

        return new BinLoadResult(BinLoadStatus.Loaded, binPath, data.Length, snapshot, parsed);
    }

    private static string ResolveHelpPath()
    {
        return nsCsFileTable.g_eLanguage is eLANGUAGE.KOREA_TRANSLATION or eLANGUAGE.KOREA_ORIGINAL
            ? "Data\\Interface\\Help\\Help.bin"
            : "Data\\Interface\\Help\\HelpEng.bin";
    }

    private static bool TryReadHelpData(string path, eFILE_TYPE fileType, out byte[] data)
    {
        if (BinDataSource.TryRead(path, fileType, out data))
        {
            return true;
        }

        if (!path.EndsWith("Help.bin", StringComparison.OrdinalIgnoreCase) &&
            BinDataSource.TryRead("Data\\Interface\\Help\\Help.bin", fileType, out data))
        {
            return true;
        }

        data = [];
        return false;
    }

    private static bool TryParse(ReadOnlyMemory<byte> data, out HelpBinData? parsed, out int bytesConsumed)
    {
        parsed = null;
        var reader = new SafeBinaryReader(data);
        if (!reader.TryReadUInt32(out uint lastTitleUniqueId) ||
            !TryReadCount(reader, out int count))
        {
            bytesConsumed = reader.Position;
            return false;
        }

        var records = new List<HelpRecord>(count);
        for (int i = 0; i < count; i++)
        {
            if (!TryReadHelp(reader, out HelpRecord? record) || record is null)
            {
                bytesConsumed = reader.Position;
                return false;
            }

            records.Add(record);
        }

        bytesConsumed = reader.Position;
        parsed = new HelpBinData(lastTitleUniqueId, records);
        return true;
    }

    private static bool TryReadHelp(SafeBinaryReader reader, out HelpRecord? record)
    {
        record = null;
        if (!TryReadTitle(reader, out HelpTitleRecord? title) ||
            title is null ||
            !TryReadContainer(reader, out HelpContainerRecord? body) ||
            body is null)
        {
            return false;
        }

        record = new HelpRecord(title, body);
        return true;
    }

    private static bool TryReadTitle(SafeBinaryReader reader, out HelpTitleRecord? title)
    {
        title = null;
        if (!reader.TryReadUInt32(out uint uniqueId) ||
            !reader.TryReadBytes(TitleCharCount * sizeof(char), out ReadOnlyMemory<byte> titleBytes) ||
            !reader.TryReadInt32(out int requireType) ||
            !reader.TryReadInt32(out int reqId1) ||
            !reader.TryReadInt32(out int reqId2))
        {
            return false;
        }

        title = new HelpTitleRecord(
            uniqueId,
            reader.ReadFixedUtf16String(titleBytes.Span),
            Enum.IsDefined(typeof(CsHelpRequireType), requireType) ? (CsHelpRequireType)requireType : CsHelpRequireType.RT_NONE,
            reqId1,
            reqId2);
        return true;
    }

    private static bool TryReadContainer(SafeBinaryReader reader, out HelpContainerRecord? container)
    {
        container = null;
        if (!reader.TryReadInt32(out int limitScroll) ||
            !TryReadCount(reader, out int stringCount))
        {
            return false;
        }

        var strings = new List<HelpStringRecord>(stringCount);
        for (int i = 0; i < stringCount; i++)
        {
            if (!TryReadString(reader, out HelpStringRecord? value) || value is null)
            {
                return false;
            }

            strings.Add(value);
        }

        if (!TryReadCount(reader, out int imageCount))
        {
            return false;
        }

        var images = new List<HelpImageRecord>(imageCount);
        for (int i = 0; i < imageCount; i++)
        {
            if (!TryReadImage(reader, out HelpImageRecord? image) || image is null)
            {
                return false;
            }

            images.Add(image);
        }

        container = new HelpContainerRecord(limitScroll, strings, images);
        return true;
    }

    private static bool TryReadString(SafeBinaryReader reader, out HelpStringRecord? value)
    {
        value = null;
        if (!TryReadCount(reader, out int textCount))
        {
            return false;
        }

        var texts = new List<HelpTextRunRecord>(textCount);
        for (int i = 0; i < textCount; i++)
        {
            if (!TryReadText(reader, out HelpTextRunRecord? text) || text is null)
            {
                return false;
            }

            texts.Add(text);
        }

        if (!TryReadPoint(reader, out CsPoint position) ||
            !reader.TryReadInt32(out int lineDeltaHeight))
        {
            return false;
        }

        value = new HelpStringRecord(texts, position, lineDeltaHeight);
        return true;
    }

    private static bool TryReadText(SafeBinaryReader reader, out HelpTextRunRecord? text)
    {
        text = null;
        if (!reader.TryReadUInt16(out ushort textType) ||
            !reader.TryReadUInt32(out uint textTypeValue1) ||
            !reader.TryReadUInt32(out uint textTypeValue2) ||
            !TryReadRect(reader, out CsRect region) ||
            !reader.TryReadByte(out byte red) ||
            !reader.TryReadByte(out byte green) ||
            !reader.TryReadByte(out byte blue) ||
            !reader.TryReadInt16(out short textSize) ||
            !reader.TryReadInt16(out short bold) ||
            !reader.TryReadInt32(out int length) ||
            length < 0 ||
            length > MaxReasonableTextLength ||
            !reader.TryReadBytes(length * sizeof(char), out ReadOnlyMemory<byte> textBytes))
        {
            return false;
        }

        text = new HelpTextRunRecord(
            Enum.IsDefined(typeof(CsHelpTextType), (int)textType) ? (CsHelpTextType)textType : CsHelpTextType.TT_NORMAL,
            textTypeValue1,
            textTypeValue2,
            region,
            red,
            green,
            blue,
            textSize,
            bold,
            Encoding.Unicode.GetString(textBytes.Span).TrimEnd('\0'));
        return true;
    }

    private static bool TryReadImage(SafeBinaryReader reader, out HelpImageRecord? image)
    {
        image = null;
        if (!reader.TryReadInt32(out int imageType) ||
            !TryReadPoint(reader, out CsPoint position) ||
            !TryReadPoint(reader, out CsPoint size) ||
            !reader.TryReadInt32(out int length) ||
            length < 0 ||
            length > MaxReasonableTextLength ||
            !reader.TryReadBytes(length, out ReadOnlyMemory<byte> imagePathBytes))
        {
            return false;
        }

        image = new HelpImageRecord(
            Enum.IsDefined(typeof(CsHelpImageType), imageType) ? (CsHelpImageType)imageType : CsHelpImageType.IT_NORMAL,
            position,
            size,
            Encoding.Default.GetString(imagePathBytes.Span).TrimEnd('\0'));
        return true;
    }

    private static bool TryReadPoint(SafeBinaryReader reader, out CsPoint point)
    {
        point = default;
        if (!reader.TryReadInt32(out int x) || !reader.TryReadInt32(out int y))
        {
            return false;
        }

        point = new CsPoint(x, y);
        return true;
    }

    private static bool TryReadRect(SafeBinaryReader reader, out CsRect rect)
    {
        rect = default;
        if (!reader.TryReadInt32(out int left) ||
            !reader.TryReadInt32(out int top) ||
            !reader.TryReadInt32(out int right) ||
            !reader.TryReadInt32(out int bottom))
        {
            return false;
        }

        rect = new CsRect(left, top, right, bottom);
        return true;
    }

    private static bool TryReadCount(SafeBinaryReader reader, out int count)
    {
        count = 0;
        return reader.TryReadInt32(out count) && count >= 0 && count <= MaxReasonableCount;
    }
}

public sealed class CsHelpMng : IBinFileTableManager
{
    private readonly HelpBinLoader _loader = new();
    private readonly eFILE_TYPE _fileType;
    private readonly string _languagePath;
    private readonly bool _allowMissingTables;
    private readonly Dictionary<uint, HelpRecord> _data = new();
    private readonly List<HelpRecord> _checkTLevel = [];
    private readonly List<HelpRecord> _checkPartnerDLevel = [];
    private readonly List<HelpRecord> _checkMapArrive = [];

    public CsHelpMng(eFILE_TYPE fileType, string languagePath, bool allowMissingTables)
    {
        _fileType = fileType;
        _languagePath = languagePath;
        _allowMissingTables = allowMissingTables;
    }

    public string Name => "CsHelpMng";
    public bool IsLoaded { get; private set; }
    public BinLoadResult? LoadResult { get; private set; }
    public uint LastTitleUniqueId { get; private set; }
    public IReadOnlyDictionary<uint, HelpRecord> Data => _data;
    public IReadOnlyList<HelpRecord> CheckTLevel => _checkTLevel;
    public IReadOnlyList<HelpRecord> CheckPartnerDLevel => _checkPartnerDLevel;
    public IReadOnlyList<HelpRecord> CheckMapArrive => _checkMapArrive;

    public bool Init()
    {
        LoadResult = _loader.Load(_languagePath, _fileType, _allowMissingTables);
        if (LoadResult.Status == BinLoadStatus.Missing)
        {
            return false;
        }

        Delete();
        if (LoadResult.ParsedTable is HelpBinData data)
        {
            LastTitleUniqueId = data.LastTitleUniqueId;
            foreach (HelpRecord record in data.Records)
            {
                _data[record.Title.UniqueId] = record;
                switch (record.Title.RequireType)
                {
                    case CsHelpRequireType.RT_TLEVEL:
                        _checkTLevel.Add(record);
                        break;
                    case CsHelpRequireType.RT_PARTNERMON_LEVEL:
                        _checkPartnerDLevel.Add(record);
                        break;
                    case CsHelpRequireType.RT_MAP_ARRIVE:
                        _checkMapArrive.Add(record);
                        break;
                }
            }
        }

        IsLoaded = true;
        return true;
    }

    public void Delete()
    {
        _data.Clear();
        _checkTLevel.Clear();
        _checkPartnerDLevel.Clear();
        _checkMapArrive.Clear();
        LastTitleUniqueId = 0;
        IsLoaded = false;
    }

    public bool IsTitleID(int uniqueId) => uniqueId >= 0 && _data.ContainsKey((uint)uniqueId);

    public HelpRecord? GetData(int uniqueId)
    {
        return uniqueId >= 0 && _data.TryGetValue((uint)uniqueId, out HelpRecord? record) ? record : null;
    }

    public IReadOnlyDictionary<uint, HelpRecord> GetData() => _data;
    public IReadOnlyList<HelpRecord> GetCheck_TLevel() => _checkTLevel;
    public IReadOnlyList<HelpRecord> GetCheck_PartnerDLevel() => _checkPartnerDLevel;
    public IReadOnlyList<HelpRecord> GetCheck_MapArrive() => _checkMapArrive;
}
