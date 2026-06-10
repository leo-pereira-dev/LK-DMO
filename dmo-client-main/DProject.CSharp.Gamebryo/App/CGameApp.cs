using DProject.Config;
using DProject.Audio;
using DProject.ContentsSystem;
using DProject.Data.Loading;
using DProject.Diagnostics;
using DProject.Engine;
using DProject.Flow;
using DProject.Game;
using DProject.GlobalData;
using DProject.Network;
using DProject.Option;
using DProject.Resist;
using DProject.Runtime;

namespace DProject.App;

public sealed class CGameApp : CWin32App
{
    private readonly AppConfig _config;
    private readonly string[] _args;
    private readonly cResist _resist;
    private readonly COptionMng _optionMng;
    private readonly GlobalDataStore _globalData = new();
    private readonly FileTableLoadingService _fileTableLoading;
    private readonly ContentsSystem.ContentsSystem _contentsSystem;
    private readonly CFlowMgr _flowMgr;
    private readonly CEngine _engine;
    private readonly NetworkService _networkService;
    private readonly RuntimeMetrics _runtimeMetrics = new();
    private readonly MainThreadDispatcher _mainThread;
    private readonly JobSystem _jobSystem;
    private readonly RenderCommandQueue _renderCommands;
    private readonly GameStateSync _gameStateSync;
    private readonly CSoundMgr _soundMgr;
    private readonly AssetLoadingCoordinator _assetLoading;
    private readonly MapLoadingService _mapLoading;
    private int _lastMainThreadActions;

    public CGameApp(AppConfig config, string[] args)
    {
        _config = config;
        _args = args;
        _mainThread = new MainThreadDispatcher(_runtimeMetrics);
        _jobSystem = new JobSystem(_runtimeMetrics);
        _renderCommands = new RenderCommandQueue(_runtimeMetrics);
        _gameStateSync = new GameStateSync(_globalData, _mainThread);
        _soundMgr = new CSoundMgr(config.Paths);
        _assetLoading = new AssetLoadingCoordinator(_jobSystem, _mainThread, _renderCommands);
        _mapLoading = new MapLoadingService(_globalData, _mainThread, _jobSystem, _assetLoading, _renderCommands);
        _resist = new cResist(config);
        _optionMng = new COptionMng(_resist);
        _engine = new CEngine();
        _networkService = new NetworkService(config, _mainThread, _runtimeMetrics, _gameStateSync, _globalData, _mapLoading);
        _fileTableLoading = new FileTableLoadingService(config, _resist, _globalData, _mainThread, _jobSystem);
        _contentsSystem = new ContentsSystem.ContentsSystem();
        _flowMgr = new CFlowMgr(new CFlowFactory());
    }

    protected override bool OnInitialize()
    {
        CrashLogger.Init();
        CrashLogger.SetContext("CGameApp.OnInitialize");
        _mainThread.BindToCurrentThread();

        _resist.Init();
        _optionMng.LoadMachineOption();
        Startup(
            _config.Window.Title,
            _resist.m_Global.s_nResolutionWidth,
            _resist.m_Global.s_nResolutionHeight,
            _resist.m_Global.s_bFullScreen);

        _globalData.Language = GameLanguage.English;
        _globalData.AccountId = _config.Login.Id;
        _globalData.AccountPassword = _config.Login.Password;
        _globalData.SelectedServerName = _config.Debug.Server;
        _globalData.UseFilePack = _config.FilePack.UsePack;
        _globalData.AssetLoading = _assetLoading;
        _globalData.DataMng.Init();
        _globalData.IconMng.Init();
        _globalData.CharResMng.Init();

        _contentsSystem.BuildContents();
        _contentsSystem.IntraConnection();

        _engine.GBInit();
        if (!_engine.Init())
        {
            return false;
        }

        if (!_engine.Create())
        {
            return false;
        }

        _soundMgr.Init();

        if (_config.Threading.EnableJobThreads)
        {
            _jobSystem.Start(_config.Threading.WorkerCount);
        }

        _networkService.Start();

        var context = new FlowContext(
            _config,
            _resist,
            _optionMng,
            _fileTableLoading,
            _globalData,
            _contentsSystem,
            _engine,
            _networkService,
            _flowMgr,
            _mainThread,
            _jobSystem,
            _gameStateSync,
            _mapLoading,
            _runtimeMetrics,
            _renderCommands);

        _flowMgr.Create(context);
        _flowMgr.StartFlow(CFlow.FLOW_IDS.FLW_LOGO);
        return true;
    }

    protected override void OnIdleExtern()
    {
        _networkService.Execute();
        _lastMainThreadActions = _mainThread.Drain(_config.Threading.MainThreadDispatchLimit);
    }

    protected override void OnIdle()
    {
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();
        _gameStateSync.UpdateServerSync();
        _globalData.DataMng.Update(1.0f / 60.0f);
        _soundMgr.Update(1.0f / 60.0f);
        _globalData.CharResMng.Update(1.0f / 60.0f);
        _flowMgr.OnIdle();
        _globalData.EffectMng.Update(1.0f / 60.0f);
        _globalData.EffectMng.Render(_renderCommands);
        cObjectController.RenderShadow(_globalData.CharMng, _resist, _renderCommands);
        int renderCommands = _renderCommands.ExecuteAll();
        stopwatch.Stop();
        _runtimeMetrics.RecordFrame(stopwatch.Elapsed, _lastMainThreadActions, renderCommands);
        if (_flowMgr.CurrentFlowId == CFlow.FLOW_IDS.FLW_MAINGAME)
        {
            SetProcessState(PROCESS_STATE.PROCESS_STOP);
        }
    }

    protected override bool CanContinueLoop(int ticks)
    {
        return ticks < _config.Porting.MaxLoopTicks;
    }

    protected override void OnTerminate()
    {
        CrashLogger.SetContext("CGameApp.OnTerminate");
        _fileTableLoading.Delete();
        _networkService.Stop();
        _globalData.EffectMng.Reset();
        _globalData.PatAIMng.Delete();
        _globalData.IconMng.Delete();
        _globalData.CharResMng.Destroy();
        _globalData.ResourceMng.End();
        _globalData.DataMng.Delete();
        _soundMgr.Destroy();
        _jobSystem.Dispose();
        RuntimeMetricsSnapshot metrics = _runtimeMetrics.Snapshot();
        CrashLogger.LogStatsMessage(
            "RuntimeMetrics frames={0} lastFrameMs={1:F3} networkQueue={2} jobQueue={3} activeJobs={4} renderQueue={5}",
            metrics.TotalFrames,
            metrics.LastFrameTime.TotalMilliseconds,
            metrics.NetworkQueueDepth,
            metrics.JobQueueDepth,
            metrics.ActiveJobCount,
            metrics.RenderCommandQueueDepth);
        _engine.Delete();
        _engine.ShutDown();
    }
}
