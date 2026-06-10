using DProject.Config;
using DProject.Data.CsFilePack;
using DProject.Data.CsFileTable;
using DProject.GlobalData;
using DProject.Resist;
using DProject.Runtime;

namespace DProject.Data.Loading;

public enum FileTableLoadState
{
    NotStarted,
    Loading,
    Succeeded,
    Failed
}

public sealed record FileTableLoadResult(
    bool Success,
    eFILE_TYPE FileType,
    eLANGUAGE Language,
    string LanguagePath,
    IReadOnlyCollection<string> LoadedManagers,
    IReadOnlyCollection<BinFileSnapshot> LoadedSnapshots,
    string? ErrorMessage);

public sealed class FileTableLoadingService
{
    private readonly AppConfig _config;
    private readonly cResist _resist;
    private readonly GlobalDataStore _globalData;
    private readonly MainThreadDispatcher _mainThread;
    private readonly JobSystem _jobs;
    private int _started;

    public FileTableLoadingService(
        AppConfig config,
        cResist resist,
        GlobalDataStore globalData,
        MainThreadDispatcher mainThread,
        JobSystem jobs)
    {
        _config = config;
        _resist = resist;
        _globalData = globalData;
        _mainThread = mainThread;
        _jobs = jobs;
    }

    public FileTableLoadState State { get; private set; } = FileTableLoadState.NotStarted;
    public FileTableLoadResult? Result { get; private set; }
    public bool IsComplete => State is FileTableLoadState.Succeeded or FileTableLoadState.Failed;

    public void Start()
    {
        if (Interlocked.Exchange(ref _started, 1) != 0)
        {
            return;
        }

        State = FileTableLoadState.Loading;
        if (_jobs.IsStarted)
        {
            _jobs.Enqueue(LoadFileTable);
            return;
        }

        ThreadPool.QueueUserWorkItem(_ => LoadFileTable(CancellationToken.None));
    }

    public void Delete()
    {
        nsCsFileTable.g_FileTableMng.Delete();
        CsFPSystem.Destroy();
        State = FileTableLoadState.NotStarted;
        Result = null;
        Interlocked.Exchange(ref _started, 0);
    }

    private void LoadFileTable(CancellationToken token)
    {
        FileTableLoadResult result;
        try
        {
            token.ThrowIfCancellationRequested();
            nsCsFileTable.ApplyFeatures(_config.FileTable.Features);

            eLANGUAGE language = nsCsFileTable.ParseLanguage(_config.FileTable.Language, _globalData.Language);
            eFILE_TYPE fileType = nsCsFileTable.ParseFileType(_config.FileTable.Source, _config.FilePack.UsePack);
            MapLanguageToResist(language);

            bool packReady = CsFPSystem.Initialize(
                _config.FilePack.UsePack,
                _config.FilePack.PackNames,
                _config.FilePack.WriteMode,
                _config.Paths.PackRoot,
                _config.FilePack.AllowMissingPackFiles);

            if (!packReady)
            {
                result = new FileTableLoadResult(
                    false,
                    fileType,
                    language,
                    string.Empty,
                    [],
                    [],
                    "File pack initialization failed.");
            }
            else
            {
                bool tableReady = nsCsFileTable.g_FileTableMng.Init(
                    fileType,
                    language,
                    _config.Paths.BinRoot,
                    _config.FileTable.AllowMissingTables);

                result = new FileTableLoadResult(
                    tableReady,
                    fileType,
                    language,
                    nsCsFileTable.g_FileTableMng.LanguagePath,
                    nsCsFileTable.g_FileTableMng.GetLoadedManagerNames(),
                    nsCsFileTable.g_FileTableMng.GetLoadedSnapshots(),
                    tableReady ? null : "File table initialization failed.");
            }
        }
        catch (Exception ex)
        {
            result = new FileTableLoadResult(
                false,
                nsCsFileTable.g_eFileType,
                nsCsFileTable.g_eLanguage,
                nsCsFileTable.g_FileTableMng.LanguagePath,
                nsCsFileTable.g_FileTableMng.GetLoadedManagerNames(),
                nsCsFileTable.g_FileTableMng.GetLoadedSnapshots(),
                ex.Message);
        }

        _mainThread.Post(() => ApplyResult(result));
    }

    private void ApplyResult(FileTableLoadResult result)
    {
        Result = result;
        State = result.Success ? FileTableLoadState.Succeeded : FileTableLoadState.Failed;
        _globalData.FileTablesLoaded = result.Success;
        _globalData.FileTableSource = result.FileType.ToString();
        _globalData.FileTableLanguage = result.Language.ToString();
        _globalData.FileTablePath = result.LanguagePath;
    }

    private void MapLanguageToResist(eLANGUAGE language)
    {
        _resist.m_Global.s_eFTLanguage = language switch
        {
            eLANGUAGE.KOREA_TRANSLATION or eLANGUAGE.KOREA_ORIGINAL => cResist.sGLOBAL.eLANGUAGE.KOREA_TRANSLATION,
            eLANGUAGE.THAILAND => cResist.sGLOBAL.eLANGUAGE.THAILAND,
            eLANGUAGE.TAIWAN => cResist.sGLOBAL.eLANGUAGE.TAIWAN,
            eLANGUAGE.HONGKONG => cResist.sGLOBAL.eLANGUAGE.HONGKONG,
            _ => cResist.sGLOBAL.eLANGUAGE.ENGLISH
        };
    }
}
