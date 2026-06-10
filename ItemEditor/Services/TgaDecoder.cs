using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ItemEditor.Services;

public static class TgaDecoder
{
    public static BitmapSource DecodeBgra32(byte[] data)
    {
        if (data.Length < 18)
            throw new InvalidDataException("TGA header is truncated.");

        var idLength = data[0];
        var colorMapType = data[1];
        var imageType = data[2];
        var colorMapLength = BitConverter.ToUInt16(data, 5);
        var colorMapDepth = data[7];
        var width = BitConverter.ToUInt16(data, 12);
        var height = BitConverter.ToUInt16(data, 14);
        var bitsPerPixel = data[16];
        var descriptor = data[17];

        if (width <= 0 || height <= 0)
            throw new InvalidDataException("TGA has invalid dimensions.");

        if (colorMapType != 0 || colorMapLength != 0 || colorMapDepth != 0)
            throw new NotSupportedException("Color-mapped TGA files are not supported.");

        if (bitsPerPixel is not (8 or 24 or 32))
            throw new NotSupportedException($"TGA {bitsPerPixel}bpp is not supported.");

        var cursor = 18 + idLength;
        if (cursor > data.Length)
            throw new EndOfStreamException("TGA image id is truncated.");

        var topOrigin = (descriptor & 0x20) != 0;
        var rightOrigin = (descriptor & 0x10) != 0;
        var bytesPerPixel = bitsPerPixel / 8;
        var pixels = new byte[width * height * 4];

        switch (imageType)
        {
            case 2:
            case 3:
                DecodeRaw(data, ref cursor, width, height, bytesPerPixel, topOrigin, rightOrigin, pixels);
                break;
            case 10:
            case 11:
                DecodeRle(data, ref cursor, width, height, bytesPerPixel, topOrigin, rightOrigin, pixels);
                break;
            default:
                throw new NotSupportedException($"TGA image type {imageType} is not supported.");
        }

        var bitmap = BitmapSource.Create(width, height, 96, 96, PixelFormats.Bgra32, null, pixels, width * 4);
        bitmap.Freeze();
        return bitmap;
    }

    private static void DecodeRaw(byte[] data, ref int cursor, int width, int height, int bytesPerPixel, bool topOrigin, bool rightOrigin, byte[] pixels)
    {
        for (var index = 0; index < width * height; index++)
        {
            if (cursor + bytesPerPixel > data.Length)
                throw new EndOfStreamException("TGA raw payload is truncated.");

            WritePixel(data.AsSpan(cursor, bytesPerPixel), index, width, height, topOrigin, rightOrigin, pixels);
            cursor += bytesPerPixel;
        }
    }

    private static void DecodeRle(byte[] data, ref int cursor, int width, int height, int bytesPerPixel, bool topOrigin, bool rightOrigin, byte[] pixels)
    {
        var index = 0;
        Span<byte> pixel = stackalloc byte[4];
        while (index < width * height)
        {
            if (cursor >= data.Length)
                throw new EndOfStreamException("TGA RLE packet is truncated.");

            var header = data[cursor++];
            var count = (header & 0x7F) + 1;
            if ((header & 0x80) != 0)
            {
                if (cursor + bytesPerPixel > data.Length)
                    throw new EndOfStreamException("TGA RLE pixel is truncated.");

                data.AsSpan(cursor, bytesPerPixel).CopyTo(pixel);
                cursor += bytesPerPixel;
                for (var i = 0; i < count && index < width * height; i++, index++)
                    WritePixel(pixel[..bytesPerPixel], index, width, height, topOrigin, rightOrigin, pixels);
            }
            else
            {
                for (var i = 0; i < count && index < width * height; i++, index++)
                {
                    if (cursor + bytesPerPixel > data.Length)
                        throw new EndOfStreamException("TGA RLE raw packet is truncated.");

                    WritePixel(data.AsSpan(cursor, bytesPerPixel), index, width, height, topOrigin, rightOrigin, pixels);
                    cursor += bytesPerPixel;
                }
            }
        }
    }

    private static void WritePixel(ReadOnlySpan<byte> source, int index, int width, int height, bool topOrigin, bool rightOrigin, byte[] pixels)
    {
        var x = index % width;
        var y = index / width;
        if (!topOrigin)
            y = height - 1 - y;
        if (rightOrigin)
            x = width - 1 - x;

        var dst = ((y * width) + x) * 4;
        if (source.Length == 1)
        {
            pixels[dst + 0] = source[0];
            pixels[dst + 1] = source[0];
            pixels[dst + 2] = source[0];
            pixels[dst + 3] = 255;
            return;
        }

        pixels[dst + 0] = source[0];
        pixels[dst + 1] = source[1];
        pixels[dst + 2] = source[2];
        pixels[dst + 3] = source.Length >= 4 ? source[3] : (byte)255;
    }
}
