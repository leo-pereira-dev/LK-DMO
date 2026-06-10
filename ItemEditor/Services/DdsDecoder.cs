using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ItemEditor.Services;

public static class DdsDecoder
{
    public static BitmapSource DecodeBgra32(byte[] data)
    {
        if (data.Length < 128 ||
            data[0] != 'D' ||
            data[1] != 'D' ||
            data[2] != 'S' ||
            data[3] != ' ')
        {
            throw new InvalidDataException("Invalid DDS file.");
        }

        var height = BitConverter.ToInt32(data, 12);
        var width = BitConverter.ToInt32(data, 16);
        var fourCc = BitConverter.ToUInt32(data, 84);
        var bitCount = BitConverter.ToInt32(data, 88);
        var pixels = fourCc switch
        {
            0x31545844 => DecodeDxt1(data, 128, width, height),
            0x33545844 => DecodeDxt3(data, 128, width, height),
            0x35545844 => DecodeDxt5(data, 128, width, height),
            0 when bitCount == 32 => DecodeBgra(data, 128, width, height),
            _ => throw new NotSupportedException($"DDS format 0x{fourCc:X8} / {bitCount}bpp is not supported.")
        };

        var bitmap = BitmapSource.Create(width, height, 96, 96, PixelFormats.Bgra32, null, pixels, width * 4);
        bitmap.Freeze();
        return bitmap;
    }

    public static BitmapSource Crop(BitmapSource source, int x, int y, int width, int height)
    {
        var cropped = new CroppedBitmap(source, new Int32Rect(x, y, width, height));
        cropped.Freeze();
        return cropped;
    }

    private static byte[] DecodeBgra(byte[] data, int offset, int width, int height)
    {
        var needed = width * height * 4;
        if (data.Length < offset + needed)
            throw new EndOfStreamException("DDS BGRA payload is truncated.");

        var pixels = new byte[needed];
        Array.Copy(data, offset, pixels, 0, pixels.Length);
        return pixels;
    }

    private static byte[] DecodeDxt1(byte[] data, int offset, int width, int height)
    {
        var pixels = new byte[width * height * 4];
        DecodeBlocks(data, offset, width, height, 8, (block, colors, alphas) =>
        {
            ReadColorBlock(block, colors, allowTransparent: true);
            for (var i = 0; i < 16; i++)
                alphas[i] = colors[i * 4 + 3];
        }, pixels);
        return pixels;
    }

    private static byte[] DecodeDxt3(byte[] data, int offset, int width, int height)
    {
        var pixels = new byte[width * height * 4];
        DecodeBlocks(data, offset, width, height, 16, (block, colors, alphas) =>
        {
            for (var i = 0; i < 16; i++)
            {
                var value = (block[i / 2] >> ((i % 2) * 4)) & 0x0F;
                alphas[i] = (byte)(value * 17);
            }
            ReadColorBlock(block.Slice(8), colors, allowTransparent: false);
        }, pixels);
        return pixels;
    }

    private static byte[] DecodeDxt5(byte[] data, int offset, int width, int height)
    {
        var pixels = new byte[width * height * 4];
        DecodeBlocks(data, offset, width, height, 16, (block, colors, alphas) =>
        {
            ReadDxt5Alpha(block, alphas);
            ReadColorBlock(block.Slice(8), colors, allowTransparent: false);
        }, pixels);
        return pixels;
    }

    private static void DecodeBlocks(
        byte[] data,
        int offset,
        int width,
        int height,
        int blockSize,
        Action<ReadOnlySpan<byte>, byte[], byte[]> decodeBlock,
        byte[] pixels)
    {
        var block = new byte[blockSize];
        var colors = new byte[16 * 4];
        var alphas = new byte[16];
        var blockCountX = (width + 3) / 4;
        var blockCountY = (height + 3) / 4;
        var cursor = offset;

        for (var by = 0; by < blockCountY; by++)
        {
            for (var bx = 0; bx < blockCountX; bx++)
            {
                if (cursor + blockSize > data.Length)
                    throw new EndOfStreamException("DDS block payload is truncated.");

                Array.Copy(data, cursor, block, 0, blockSize);
                cursor += blockSize;
                Array.Clear(colors);
                Array.Clear(alphas);
                decodeBlock(block, colors, alphas);

                for (var py = 0; py < 4; py++)
                {
                    var y = by * 4 + py;
                    if (y >= height)
                        continue;

                    for (var px = 0; px < 4; px++)
                    {
                        var x = bx * 4 + px;
                        if (x >= width)
                            continue;

                        var src = (py * 4 + px) * 4;
                        var dst = (y * width + x) * 4;
                        pixels[dst + 0] = colors[src + 0];
                        pixels[dst + 1] = colors[src + 1];
                        pixels[dst + 2] = colors[src + 2];
                        pixels[dst + 3] = alphas[py * 4 + px];
                    }
                }
            }
        }
    }

    private static void ReadColorBlock(ReadOnlySpan<byte> block, byte[] output, bool allowTransparent)
    {
        var color0 = BitConverter.ToUInt16(block[..2]);
        var color1 = BitConverter.ToUInt16(block.Slice(2, 2));
        Span<byte> palette = stackalloc byte[16];
        Write565(color0, palette, 0, 255);
        Write565(color1, palette, 4, 255);

        if (color0 > color1 || !allowTransparent)
        {
            Interpolate(palette, 8, 0, 4, 2, 1, 3, 255);
            Interpolate(palette, 12, 0, 4, 1, 2, 3, 255);
        }
        else
        {
            Interpolate(palette, 8, 0, 4, 1, 1, 2, 255);
            palette[12] = 0;
            palette[13] = 0;
            palette[14] = 0;
            palette[15] = 0;
        }

        var codes = BitConverter.ToUInt32(block.Slice(4, 4));
        for (var i = 0; i < 16; i++)
        {
            var code = (int)((codes >> (2 * i)) & 0x03);
            output[i * 4 + 0] = palette[code * 4 + 0];
            output[i * 4 + 1] = palette[code * 4 + 1];
            output[i * 4 + 2] = palette[code * 4 + 2];
            output[i * 4 + 3] = palette[code * 4 + 3];
        }
    }

    private static void ReadDxt5Alpha(ReadOnlySpan<byte> block, byte[] output)
    {
        Span<byte> alphaPalette = stackalloc byte[8];
        alphaPalette[0] = block[0];
        alphaPalette[1] = block[1];

        if (alphaPalette[0] > alphaPalette[1])
        {
            for (var i = 1; i <= 6; i++)
                alphaPalette[i + 1] = (byte)(((7 - i) * alphaPalette[0] + i * alphaPalette[1]) / 7);
        }
        else
        {
            for (var i = 1; i <= 4; i++)
                alphaPalette[i + 1] = (byte)(((5 - i) * alphaPalette[0] + i * alphaPalette[1]) / 5);
            alphaPalette[6] = 0;
            alphaPalette[7] = 255;
        }

        ulong bits = 0;
        for (var i = 0; i < 6; i++)
            bits |= (ulong)block[2 + i] << (8 * i);

        for (var i = 0; i < 16; i++)
            output[i] = alphaPalette[(int)((bits >> (3 * i)) & 0x07)];
    }

    private static void Write565(ushort value, Span<byte> palette, int offset, byte alpha)
    {
        var r = (byte)(((value >> 11) & 0x1F) * 255 / 31);
        var g = (byte)(((value >> 5) & 0x3F) * 255 / 63);
        var b = (byte)((value & 0x1F) * 255 / 31);
        palette[offset + 0] = b;
        palette[offset + 1] = g;
        palette[offset + 2] = r;
        palette[offset + 3] = alpha;
    }

    private static void Interpolate(Span<byte> palette, int dst, int a, int b, int wa, int wb, int divisor, byte alpha)
    {
        palette[dst + 0] = (byte)((palette[a + 0] * wa + palette[b + 0] * wb) / divisor);
        palette[dst + 1] = (byte)((palette[a + 1] * wa + palette[b + 1] * wb) / divisor);
        palette[dst + 2] = (byte)((palette[a + 2] * wa + palette[b + 2] * wb) / divisor);
        palette[dst + 3] = alpha;
    }
}
