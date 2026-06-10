using System.Text;

namespace ItemEditor.Services;

public static class BinLayout
{
    public const int Utf16Bytes = 2;
    public const int ItemInfoRecordSize = 1596;

    public const int ItemId = 0;
    public const int Name = 4;
    public const int Icon = 132;
    public const int Comment = 136;
    public const int Nif = 1160;
    public const int Class = 1224;
    public const int TypeComment = 1226;
    public const int CodeTag = 1354;
    public const int TypeL = 1356;
    public const int TypeS = 1358;
    public const int TypeValue = 1360;
    public const int Section = 1364;
    public const int SellType = 1368;
    public const int UseMode = 1370;
    public const int CooldownGroup = 1372;
    public const int Overlap = 1374;
    public const int TamerMinLevel = 1376;
    public const int TamerMaxLevel = 1378;
    public const int DigimonMinLevel = 1380;
    public const int DigimonMaxLevel = 1382;
    public const int EventPriceId = 1396;
    public const int EventPriceAmount = 1400;
    public const int DigicorePrice = 1402;
    public const int ScanPrice = 1404;
    public const int SellPrice = 1408;
    public const int SkillCodeType = 1542;
    public const int SkillCode = 1544;
    public const int ApplyValueMax = 1548;
    public const int ApplyValueMin = 1549;
    public const int ApplyElement = 1550;
    public const int SocketCount = 1552;
    public const int SoundId = 1554;
    public const int BoundType = 1556;
    public const int QuestRequire = 1576;
    public const int UseTimeType = 1580;
    public const int UsageTimeMinutes = 1584;

    public static int ReadInt32(byte[] data, int offset) => BitConverter.ToInt32(data, offset);
    public static uint ReadUInt32(byte[] data, int offset) => BitConverter.ToUInt32(data, offset);
    public static ushort ReadUInt16(byte[] data, int offset) => BitConverter.ToUInt16(data, offset);
    public static byte ReadByte(byte[] data, int offset) => data[offset];

    public static void WriteInt32(byte[] data, int offset, int value) => BitConverter.GetBytes(value).CopyTo(data, offset);
    public static void WriteUInt32(byte[] data, int offset, uint value) => BitConverter.GetBytes(value).CopyTo(data, offset);
    public static void WriteUInt16(byte[] data, int offset, ushort value) => BitConverter.GetBytes(value).CopyTo(data, offset);
    public static void WriteByte(byte[] data, int offset, byte value) => data[offset] = value;
    public static void WriteSingle(byte[] data, int offset, float value) => BitConverter.GetBytes(value).CopyTo(data, offset);

    public static string ReadUtf16(byte[] data, int offset, int chars)
    {
        var bytes = data.AsSpan(offset, chars * Utf16Bytes);
        var end = 0;
        for (; end + 1 < bytes.Length; end += 2)
        {
            if (bytes[end] == 0 && bytes[end + 1] == 0)
                break;
        }

        return Encoding.Unicode.GetString(bytes[..end]).TrimEnd('\0');
    }

    public static string ReadAscii(byte[] data, int offset, int bytesLength)
    {
        var bytes = data.AsSpan(offset, bytesLength);
        var end = bytes.IndexOf((byte)0);
        if (end < 0)
            end = bytes.Length;

        return Encoding.ASCII.GetString(bytes[..end]).TrimEnd('\0');
    }

    public static void WriteUtf16(byte[] data, int offset, int chars, string value)
    {
        Array.Clear(data, offset, chars * Utf16Bytes);
        var bytes = Encoding.Unicode.GetBytes(value);
        var max = Math.Min(bytes.Length, (chars - 1) * Utf16Bytes);
        Array.Copy(bytes, 0, data, offset, max);
    }

    public static void WriteAscii(byte[] data, int offset, int bytesLength, string value)
    {
        Array.Clear(data, offset, bytesLength);
        var bytes = Encoding.ASCII.GetBytes(value ?? string.Empty);
        var max = Math.Min(bytes.Length, bytesLength - 1);
        Array.Copy(bytes, 0, data, offset, max);
    }
}
