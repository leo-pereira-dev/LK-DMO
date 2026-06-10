using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ItemEditor.Services;

public sealed class MapPreviewService
{
    private readonly List<string> _packCandidates = [];
    private readonly List<string> _directRoots = [];
    private readonly Dictionary<string, ImageSource> _cache = new(StringComparer.OrdinalIgnoreCase);
    private readonly Dictionary<string, IReadOnlyList<string>> _packIndexCache = new(StringComparer.OrdinalIgnoreCase);

    public void Configure(string sourcePath)
    {
        _packCandidates.Clear();
        _directRoots.Clear();
        _cache.Clear();
        _packIndexCache.Clear();

        TryAddPack(sourcePath);
        TryAddPack(Path.Combine("C:\\0-NewDmo", "N\u00e3oUsarEsseClient", "Data", "Pack01"));
        TryAddPack(Path.Combine("C:\\0-NewDmo", "N\u00e3oUsarEsseClient", "Data", "Pack03"));
        TryAddPack(@"C:\0-NewDmo\NãoUsarEsseClient\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\NãoUsarEsseClient\Data\Pack03");
        TryAddPack(@"C:\0-NewDmo\NaoUsarEsseClient\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\NaoUsarEsseClient\Data\Pack03");
        TryAddPack(@"C:\0-NewDmo\ClientDist\Data\Pack01");
        TryAddPack(@"C:\0-NewDmo\ClientDist\Data\Pack03");

        AddDirectRoot(sourcePath);
        var sourceDirectory = File.Exists(sourcePath) ? Path.GetDirectoryName(sourcePath) : sourcePath;
        AddDirectRoot(sourceDirectory);
        AddDirectRoot(sourceDirectory == null ? null : Path.GetDirectoryName(sourceDirectory));
        AddDirectRoot(Path.Combine("C:\\0-NewDmo", "N\u00e3oUsarEsseClient", "Data"));
        AddDirectRoot(@"C:\0-NewDmo\NãoUsarEsseClient\Data");
        AddDirectRoot(@"C:\0-NewDmo\NaoUsarEsseClient\Data");
        AddDirectRoot(@"C:\0-NewDmo\ClientDist\Data");
    }

    public (ImageSource? Image, string Status) ResolveMapImage(IEnumerable<string> relativeCandidates)
    {
        foreach (var relative in ExpandCandidates(relativeCandidates))
        {
            if (_cache.TryGetValue(relative, out var cached))
                return (cached, $"Rendered {relative}");

            foreach (var root in _directRoots)
            {
                var directPath = Path.Combine(root, relative);
                if (!File.Exists(directPath))
                    continue;

                try
                {
                    var image = Decode(File.ReadAllBytes(directPath), relative);
                    _cache[relative] = image;
                    return (image, $"Rendered {relative}");
                }
                catch
                {
                }
            }

            foreach (var pack in _packCandidates)
            {
                try
                {
                    var image = Decode(PackArchive.ReadFile(pack, relative), relative);
                    _cache[relative] = image;
                    return (image, $"Rendered {relative}");
                }
                catch
                {
                }
            }
        }

        return (null, "No map texture found in packs.");
    }

    private IReadOnlyList<string> ExpandCandidates(IEnumerable<string> relativeCandidates)
    {
        var exact = relativeCandidates
            .Where(x => !string.IsNullOrWhiteSpace(x))
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .ToList();

        var expanded = new List<string>(exact);
        var fileNames = exact
            .Select(Path.GetFileName)
            .Where(x => !string.IsNullOrWhiteSpace(x))
            .Select(x => x!)
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .ToList();

        foreach (var indexedPath in IndexedPackFiles())
        {
            var indexedName = Path.GetFileName(indexedPath);
            if (fileNames.Any(x => x.Equals(indexedName, StringComparison.OrdinalIgnoreCase)) &&
                !expanded.Contains(indexedPath, StringComparer.OrdinalIgnoreCase))
            {
                expanded.Add(indexedPath);
            }
        }

        var textureTokens = exact
            .Select(Path.GetFileNameWithoutExtension)
            .Select(NormalizeTextureToken)
            .Where(IsSpecificTextureToken)
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .ToList();

        foreach (var indexedPath in IndexedPackFiles())
        {
            var indexedToken = NormalizeTextureToken(Path.GetFileNameWithoutExtension(indexedPath));
            if (!IsSpecificTextureToken(indexedToken))
                continue;

            if (textureTokens.Any(token => TextureTokenMatches(token, indexedToken)) &&
                !expanded.Contains(indexedPath, StringComparer.OrdinalIgnoreCase))
            {
                expanded.Add(indexedPath);
            }
        }

        return expanded;
    }

    private IEnumerable<string> IndexedPackFiles()
    {
        foreach (var pack in _packCandidates)
        {
            if (!_packIndexCache.TryGetValue(pack, out var files))
            {
                try
                {
                    files = PackArchive.ListFiles(pack)
                        .Where(IsSupportedMapTexture)
                        .ToList();
                }
                catch
                {
                    files = [];
                }

                _packIndexCache[pack] = files;
            }

            foreach (var file in files)
                yield return file;
        }
    }

    private static bool IsSupportedMapTexture(string path)
    {
        var extension = Path.GetExtension(path);
        if (!extension.Equals(".tga", StringComparison.OrdinalIgnoreCase) &&
            !extension.Equals(".dds", StringComparison.OrdinalIgnoreCase) &&
            !extension.Equals(".bmp", StringComparison.OrdinalIgnoreCase) &&
            !extension.Equals(".png", StringComparison.OrdinalIgnoreCase))
        {
            return false;
        }

        var fileName = Path.GetFileNameWithoutExtension(path);
        return fileName.EndsWith("_Zone", StringComparison.OrdinalIgnoreCase) ||
               fileName.EndsWith("_Mini", StringComparison.OrdinalIgnoreCase) ||
               fileName.StartsWith("Area", StringComparison.OrdinalIgnoreCase) ||
               fileName.Equals("_EmptyZone", StringComparison.OrdinalIgnoreCase) ||
               fileName.Equals("DATS_IN_Zone", StringComparison.OrdinalIgnoreCase);
    }

    private static string NormalizeTextureToken(string? value)
    {
        var token = (value ?? string.Empty).Trim();
        foreach (var suffix in new[] { "_Zone", "_Mini" })
        {
            if (token.EndsWith(suffix, StringComparison.OrdinalIgnoreCase))
            {
                token = token[..^suffix.Length];
                break;
            }
        }

        return new string(token
            .Where(char.IsLetterOrDigit)
            .Select(char.ToLowerInvariant)
            .ToArray());
    }

    private static bool IsSpecificTextureToken(string token)
    {
        if (token.Length < 3)
            return false;

        if (token.All(char.IsDigit))
            return false;

        if (token.StartsWith("area", StringComparison.OrdinalIgnoreCase))
            return false;

        if (token.Equals("emptyzone", StringComparison.OrdinalIgnoreCase))
            return false;

        return true;
    }

    private static bool TextureTokenMatches(string candidate, string indexed)
        => indexed.Equals(candidate, StringComparison.OrdinalIgnoreCase) ||
           (candidate.Length >= 5 && indexed.Contains(candidate, StringComparison.OrdinalIgnoreCase)) ||
           (indexed.Length >= 5 && candidate.Contains(indexed, StringComparison.OrdinalIgnoreCase));

    private static ImageSource Decode(byte[] bytes, string relativePath)
    {
        var extension = Path.GetExtension(relativePath);
        if (extension.Equals(".dds", StringComparison.OrdinalIgnoreCase))
            return DdsDecoder.DecodeBgra32(bytes);

        if (extension.Equals(".tga", StringComparison.OrdinalIgnoreCase))
            return TgaDecoder.DecodeBgra32(bytes);

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

    private void AddDirectRoot(string? path)
    {
        if (string.IsNullOrWhiteSpace(path) || !Directory.Exists(path))
            return;

        if (!_directRoots.Contains(path, StringComparer.OrdinalIgnoreCase))
            _directRoots.Add(path);
    }
}
