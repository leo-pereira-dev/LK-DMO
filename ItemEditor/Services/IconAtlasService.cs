using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ItemEditor.Services;

public sealed class IconAtlasService
{
    private const int IconCellSize = 32;
    private const int IconMaskType = 500;
    private readonly Dictionary<int, BitmapSource> _atlasCache = [];
    private readonly Dictionary<int, string> _atlasPathCache = [];
    private readonly Dictionary<int, string> _missingAtlasCache = [];
    private readonly List<string> _packCandidates = [];

    private static readonly Dictionary<int, string[]> TextureFiles = new()
    {
        [1] = [@"Icon\Icon01.dds", @"data\interface\Icon\icon01.bmp"],
        [2] = [@"Icon\Icon02.dds", @"data\interface\Icon\icon02.bmp"],
        [3] = [@"Icon\Icon03.dds", @"data\interface\Icon\icon03.bmp"],
        [4] = [@"Icon\Icon04.dds", @"data\interface\Icon\icon04.bmp"],
        [5] = [@"Icon\Icon05.dds", @"data\interface\Icon\icon05.bmp"],
        [6] = [@"Icon\Icon06.dds", @"data\interface\Icon\icon06.bmp"],
        [7] = [@"Icon\Icon07.dds", @"data\interface\Icon\icon07.bmp"],
        [8] = [@"Icon\Icon08.dds", @"data\interface\Icon\icon08.bmp"],
        [9] = [@"Icon\Icon09.dds", @"data\interface\Icon\icon09.bmp"],
        [10] = [@"Icon\Icon10.dds", @"data\interface\Icon\icon10.bmp"],
        [11] = [@"data\interface\Icon\icon11.bmp"],
        [47] = [@"Icon\Icon47.dds", @"data\interface\Icon\icon47.bmp"],
        [48] = [@"Icon\Icon48.dds", @"data\interface\Icon\icon48.bmp"],
        [49] = [@"Icon\Icon49.dds", @"data\interface\Icon\icon49.bmp"],
        [IconMaskType] = [@"Icon\ItemMask.tga", @"data\interface\Icon\ItemMask.tga"]
    };

    public void Configure(string sourcePath)
    {
        _atlasCache.Clear();
        _atlasPathCache.Clear();
        _missingAtlasCache.Clear();
        _packCandidates.Clear();

        TryAddPack(sourcePath);
        TryAddPack(@"C:\0-NewDmo\NãoUsarEsseClient\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\NãoUsarEsseClient\Data\Pack03");
        TryAddPack(@"C:\0-NewDmo\ClientDist\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\ClientDist\Data\Pack03");
        TryAddPack(@"C:\0-NewDmo\ClientDist1\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\ClientDist1\Data\Pack03");
    }

    public (ImageSource? image, string status) ResolveItemIcon(int iconId, int itemClass)
    {
        var textureType = iconId / 1000;
        var iconIndex = iconId % 1000;
        if (!TextureFiles.ContainsKey(textureType))
            return (null, $"Icon {iconId}: atlas {textureType} is not registered in ICONITEM.");

        if (!TryLoadAtlas(textureType, out var atlas, out var status))
            return (null, status);

        var columns = atlas.PixelWidth / IconCellSize;
        var rows = atlas.PixelHeight / IconCellSize;
        if (columns <= 0 || rows <= 0 || iconIndex < 0 || iconIndex >= columns * rows)
            return (null, $"Icon {iconId}: index {iconIndex} is outside {columns}x{rows} atlas.");

        var x = (iconIndex % columns) * IconCellSize;
        var y = (iconIndex / columns) * IconCellSize;
        var icon = DdsDecoder.Crop(atlas, x, y, IconCellSize, IconCellSize);
        var composed = TryComposeMask(icon, itemClass);
        var atlasName = _atlasPathCache.TryGetValue(textureType, out var loadedPath)
            ? Path.GetFileName(loadedPath)
            : Path.GetFileName(TextureFiles[textureType][0]);
        return (composed ?? icon, $"{atlasName} [{iconIndex}]");
    }

    private ImageSource? TryComposeMask(BitmapSource icon, int itemClass)
    {
        if (itemClass <= 0)
            return null;

        if (!TryLoadAtlas(IconMaskType, out var maskAtlas, out _))
            return null;

        var index = Math.Max(0, itemClass - 1);
        var columns = maskAtlas.PixelWidth / IconCellSize;
        if (columns <= 0 || index >= columns * (maskAtlas.PixelHeight / IconCellSize))
            return null;

        var mask = DdsDecoder.Crop(maskAtlas, (index % columns) * IconCellSize, (index / columns) * IconCellSize, IconCellSize, IconCellSize);
        var group = new DrawingGroup();
        group.Children.Add(new ImageDrawing(icon, new System.Windows.Rect(0, 0, IconCellSize, IconCellSize)));
        group.Children.Add(new ImageDrawing(mask, new System.Windows.Rect(0, 0, IconCellSize, IconCellSize)));
        var drawing = new DrawingImage(group);
        drawing.Freeze();
        return drawing;
    }

    private bool TryLoadAtlas(int textureType, out BitmapSource atlas, out string status)
    {
        if (_atlasCache.TryGetValue(textureType, out atlas!))
        {
            status = string.Empty;
            return true;
        }

        if (_missingAtlasCache.TryGetValue(textureType, out var missingStatus))
        {
            status = missingStatus;
            atlas = null!;
            return false;
        }

        if (!TextureFiles.TryGetValue(textureType, out var relativePaths))
        {
            status = $"Texture type {textureType} is unknown.";
            return false;
        }

        foreach (var pack in _packCandidates)
        {
            foreach (var relativePath in relativePaths)
            {
                try
                {
                    var bytes = PackArchive.ReadFile(pack, relativePath);
                    atlas = relativePath.EndsWith(".dds", StringComparison.OrdinalIgnoreCase)
                        ? DdsDecoder.DecodeBgra32(bytes)
                        : DecodeBitmap(bytes);
                    _atlasCache[textureType] = atlas;
                    _atlasPathCache[textureType] = relativePath;
                    status = string.Empty;
                    return true;
                }
                catch
                {
                }
            }
        }

        status = $"{string.Join(" or ", relativePaths.Select(Path.GetFileName))} was not found in configured packs.";
        _missingAtlasCache[textureType] = status;
        atlas = null!;
        return false;
    }

    private static BitmapSource DecodeBitmap(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        var image = BitmapFrame.Create(stream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.OnLoad);
        image.Freeze();
        return image;
    }

    private void TryAddPack(string path)
    {
        if (string.IsNullOrWhiteSpace(path))
            return;

        try
        {
            var pack = PackArchive.ResolveBasePath(path);
            if (!_packCandidates.Contains(pack, StringComparer.OrdinalIgnoreCase))
                _packCandidates.Add(pack);
        }
        catch
        {
        }
    }
}
