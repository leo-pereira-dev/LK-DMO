using DProject.Config;
using DProject.Data.Loading;
using DProject.Engine;
using DProject.GlobalData;
using DProject.Network;
using DProject.Option;
using DProject.Resist;
using DProject.Runtime;

namespace DProject.Flow;

public sealed record FlowContext(
    AppConfig Config,
    cResist Resist,
    COptionMng OptionMng,
    FileTableLoadingService FileTableLoading,
    GlobalDataStore GlobalData,
    ContentsSystem.ContentsSystem ContentsSystem,
    CEngine Engine,
    NetworkService Network,
    CFlowMgr FlowMgr,
    MainThreadDispatcher MainThread,
    JobSystem Jobs,
    GameStateSync GameStateSync,
    MapLoadingService MapLoading,
    RuntimeMetrics Metrics,
    RenderCommandQueue RenderCommands);
