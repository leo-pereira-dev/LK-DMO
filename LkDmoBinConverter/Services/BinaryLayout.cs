namespace LkDmoBinConverter.Services;

internal static class BinaryLayout
{
    private const int AccessoryOptionMaxOptionCount = 44;
    private const int LegacyItemInfoSize = 1596;
    private const int LegacyItemInfoBelongingOffset = 1556;

    public static int Align(int value, int alignment) => (value + (alignment - 1)) & ~(alignment - 1);

    public static int ItemInfoSize()
    {
        return LegacyItemInfoSize;
    }

    public static int ItemInfoBelongingOffset()
    {
        return LegacyItemInfoBelongingOffset;
    }

    public static int ExchangeRecordSize() => 4 + ExchangeInfoSize();

    public static int AccessoryOptionRecordSize() => 4 + AccessoryOptionInfoSize();

    public static int AccessoryEnchantRecordSize() => 4 + AccessoryEnchantInfoSize();

    private static int ExchangeInfoSize()
    {
        var size = 0;
        size += 4;
        size += 2;
        size = Align(size, 4);
        size += 4;
        size += 4 * 4;
        size += 2 * 5;
        return Align(size, 4);
    }

    private static int AccessoryOptionInfoSize()
    {
        var size = 4 + 2 + 2;
        for (var i = 0; i < AccessoryOptionMaxOptionCount; i++)
        {
            var option = 2;
            option = Align(option, 4);
            option += 4 + 4;
            size += option;
        }

        return size;
    }

    private static int AccessoryEnchantInfoSize()
    {
        var size = 0;
        size += 4;
        size += 2;
        size += 2;
        return size;
    }
}
