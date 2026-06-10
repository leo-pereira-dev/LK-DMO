namespace DProject.Data.CsFilePack;

public static class CsFPSystem
{
    private static readonly object SyncRoot = new();
    private static readonly List<PackPackageMetadata> Packages = new();
    private static bool _initialized;
    private static bool _usePackage;
    private static bool _writeMode;
    private static bool _allowMissingPackFiles;
    private static string _packRoot = "Data";
    private static readonly List<string> PackNames = new();

    public static bool Initialize(
        bool bUsePack,
        IReadOnlyCollection<string> vecPackName,
        bool bWrite,
        string packRoot,
        bool allowMissingPackFiles)
    {
        lock (SyncRoot)
        {
            _usePackage = bUsePack;
            _writeMode = bWrite;
            _packRoot = string.IsNullOrWhiteSpace(packRoot) ? "Data" : packRoot;
            _allowMissingPackFiles = allowMissingPackFiles;
            PackNames.Clear();
            PackNames.AddRange(vecPackName.Where(name => !string.IsNullOrWhiteSpace(name)));
            Packages.Clear();

            if (_usePackage)
            {
                foreach (string packName in PackNames)
                {
                    PackPackageMetadata package = PackPackageMetadata.Load(
                        packName,
                        ResolvePackBasePath(packName),
                        allowUnsupportedHashFormat: allowMissingPackFiles);
                    Packages.Add(package);

                    if (!_allowMissingPackFiles &&
                        package.Status != PackMetadataStatus.Loaded)
                    {
                        _initialized = false;
                        return false;
                    }
                }
            }

            _initialized = true;
            return true;
        }
    }

    public static bool Initialize(
        bool bUsePackage,
        string cnPkgNamevec,
        bool bWrite,
        string packRoot,
        bool allowMissingPackFiles)
    {
        string[] packNames = cnPkgNamevec
            .Split([';', ',', '|'], StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
        return Initialize(bUsePackage, packNames, bWrite, packRoot, allowMissingPackFiles);
    }

    public static void Destroy()
    {
        lock (SyncRoot)
        {
            _initialized = false;
            _usePackage = false;
            _writeMode = false;
            PackNames.Clear();
            Packages.Clear();
        }
    }

    public static bool IsInitialized => _initialized;

    public static bool IsUsePackage() => _initialized && _usePackage;

    public static bool IsWriteMode() => _initialized && _writeMode;

    public static bool IsExist(string psPath)
    {
        if (string.IsNullOrWhiteSpace(psPath))
        {
            return false;
        }

        return IsExistFromDrive(psPath) || (_usePackage && Packages.Any(pack => pack.Contains(psPath)));
    }

    public static bool IsExist(int nIdx, string p_psPath)
    {
        if ((uint)nIdx >= (uint)Packages.Count)
        {
            return false;
        }

        return Packages[nIdx].Contains(p_psPath);
    }

    public static bool IsExistOnlyPack(int nIdx, string p_psPath)
    {
        if ((uint)nIdx >= (uint)Packages.Count)
        {
            return false;
        }

        return Packages[nIdx].Contains(p_psPath);
    }

    public static bool IsExistOnlyPack(string packName, string pathInPack)
    {
        if (!_usePackage || string.IsNullOrWhiteSpace(pathInPack))
        {
            return false;
        }

        PackPackageMetadata? package = Packages.FirstOrDefault(
            p => p.Name.Equals(packName, StringComparison.OrdinalIgnoreCase));
        return package?.Contains(pathInPack) == true;
    }

    public static bool IsExistFromDrive(string psPath)
    {
        return File.Exists(psPath) || Directory.Exists(psPath);
    }

    public static bool IsExistPackFile(string pPackName)
    {
        lock (SyncRoot)
        {
            return IsExistPackFileUnsafe(pPackName);
        }
    }

    public static bool TryGetHashData(int nIdx, string path, out PackHashInfo hashInfo)
    {
        lock (SyncRoot)
        {
            hashInfo = default!;
            if ((uint)nIdx >= (uint)Packages.Count)
            {
                return false;
            }

            return Packages[nIdx].TryGetHashInfo(path, out hashInfo);
        }
    }

    public static bool TryGetFileData(int nIdx, string path, out byte[] data)
    {
        lock (SyncRoot)
        {
            data = [];
            if ((uint)nIdx >= (uint)Packages.Count)
            {
                return false;
            }

            return Packages[nIdx].TryReadFile(path, out data);
        }
    }

    public static bool TryGetFileData(string path, out byte[] data)
    {
        lock (SyncRoot)
        {
            foreach (PackPackageMetadata package in Packages)
            {
                if (package.TryReadFile(path, out data))
                {
                    return true;
                }
            }

            data = [];
            return false;
        }
    }

    public static int GetFileHandle(int nFilePackIndex, string szPath)
    {
        if (!IsExist(nFilePackIndex, szPath))
        {
            return -1;
        }

        return TryGetHashData(nFilePackIndex, szPath, out PackHashInfo hashInfo)
            ? hashInfo.Offset.GetHashCode()
            : -1;
    }

    public static uint GetHashCode(string value)
    {
        if (string.IsNullOrWhiteSpace(value))
        {
            return 0;
        }

        uint hash = 5381;
        foreach (char ch in value.ToLowerInvariant())
        {
            if (ch is '.' or '\\')
            {
                continue;
            }

            hash = ((hash << 5) + hash) + ch;
        }

        return hash;
    }

    public static string StringToLower(string value) => value.ToLowerInvariant();

    public static string StringToUpper(string value) => value.ToUpperInvariant();

    public static int GetTotalFileCount(int nIdx)
    {
        if ((uint)nIdx >= (uint)Packages.Count)
        {
            return 0;
        }

        return Packages[nIdx].HashMap.Count;
    }

    public static IReadOnlyList<string> GetFileList(int nIdx)
    {
        if ((uint)nIdx >= (uint)Packages.Count)
        {
            return [];
        }

        return Packages[nIdx].GetFileList();
    }

    private static bool IsExistPackFileUnsafe(string packName)
    {
        if (string.IsNullOrWhiteSpace(packName))
        {
            return false;
        }

        string basePath = ResolvePackBasePath(packName);
        string pf = basePath + ".pf";
        string hf = basePath + ".hf";
        return File.Exists(pf) && File.Exists(hf);
    }

    private static string ResolvePackBasePath(string packName)
    {
        if (Path.IsPathRooted(packName) ||
            packName.Contains(Path.DirectorySeparatorChar) ||
            packName.Contains(Path.AltDirectorySeparatorChar))
        {
            return packName;
        }

        return Path.Combine(_packRoot, packName);
    }
}
