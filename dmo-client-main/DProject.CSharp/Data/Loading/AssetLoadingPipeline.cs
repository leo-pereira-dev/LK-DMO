using DProject.Data.CsFilePack;
using DProject.Runtime;

namespace DProject.Data.Loading;

public enum AssetKind
{
    Unknown,
    Texture,
    Model,
    Animation,
    Sound,
    Map,
    Effect,
    Shader
}

public enum AssetLoadStatus
{
    Queued,
    Loaded,
    Missing,
    Failed
}

public sealed record AssetVector3(float X, float Y, float Z)
{
    public static readonly AssetVector3 Zero = new(0.0f, 0.0f, 0.0f);
    public static readonly AssetVector3 One = new(1.0f, 1.0f, 1.0f);
}

public sealed record AssetLoadRequest(
    ulong RequestId,
    AssetKind Kind,
    string Key,
    string Path,
    bool AllowMissing = true,
    IReadOnlyDictionary<string, string>? Metadata = null);

public sealed record RenderableAssetDto(
    ulong RequestId,
    AssetKind Kind,
    string Key,
    string SourcePath,
    int ByteCount,
    AssetVector3 Position,
    AssetVector3 Scale,
    IReadOnlyDictionary<string, string> Metadata);

public sealed record AssetLoadResult(
    AssetLoadRequest Request,
    AssetLoadStatus Status,
    RenderableAssetDto? Renderable,
    string? ErrorMessage,
    DateTimeOffset CompletedAt);

public sealed class AssetLoadingCoordinator
{
    private readonly JobSystem _jobs;
    private readonly MainThreadDispatcher _mainThread;
    private readonly RenderCommandQueue _renderCommands;
    private readonly Dictionary<string, RenderableAssetDto> _renderables = new(StringComparer.OrdinalIgnoreCase);
    private long _nextRequestId;
    private int _pendingCount;

    public AssetLoadingCoordinator(JobSystem jobs, MainThreadDispatcher mainThread, RenderCommandQueue renderCommands)
    {
        _jobs = jobs;
        _mainThread = mainThread;
        _renderCommands = renderCommands;
    }

    public int PendingCount => Volatile.Read(ref _pendingCount);
    public AssetLoadResult? LastResult { get; private set; }
    public IReadOnlyDictionary<string, RenderableAssetDto> Renderables => _renderables;

    public AssetLoadRequest CreateRequest(
        AssetKind kind,
        string key,
        string path,
        bool allowMissing = true,
        IReadOnlyDictionary<string, string>? metadata = null)
    {
        ulong requestId = unchecked((ulong)Interlocked.Increment(ref _nextRequestId));
        return new AssetLoadRequest(requestId, kind, key, path, allowMissing, metadata);
    }

    public void Enqueue(AssetLoadRequest request, Action<AssetLoadResult>? afterApply = null)
    {
        ArgumentNullException.ThrowIfNull(request);
        Interlocked.Increment(ref _pendingCount);

        if (_jobs.IsStarted)
        {
            _jobs.Enqueue(token => LoadOnWorker(request, afterApply, token));
            return;
        }

        ThreadPool.QueueUserWorkItem(_ => LoadOnWorker(request, afterApply, CancellationToken.None));
    }

    private void LoadOnWorker(AssetLoadRequest request, Action<AssetLoadResult>? afterApply, CancellationToken token)
    {
        AssetLoadResult result;
        try
        {
            token.ThrowIfCancellationRequested();
            Dictionary<string, string> metadata = request.Metadata is null
                ? new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase)
                : new Dictionary<string, string>(request.Metadata, StringComparer.OrdinalIgnoreCase);

            if (File.Exists(request.Path))
            {
                var fileInfo = new FileInfo(request.Path);
                metadata["Source"] = "Drive";
                metadata["LastWriteUtc"] = fileInfo.LastWriteTimeUtc.ToString("O");
                result = BuildResult(request, AssetLoadStatus.Loaded, checked((int)Math.Min(fileInfo.Length, int.MaxValue)), metadata, null);
            }
            else if (CsFPSystem.TryGetFileData(request.Path, out byte[] packData))
            {
                metadata["Source"] = "FilePack";
                result = BuildResult(request, AssetLoadStatus.Loaded, packData.Length, metadata, null);
            }
            else
            {
                result = request.AllowMissing
                    ? BuildResult(request, AssetLoadStatus.Missing, 0, metadata, null)
                    : BuildResult(request, AssetLoadStatus.Failed, 0, metadata, $"Asset not found: {request.Path}");
            }
        }
        catch (Exception ex)
        {
            result = new AssetLoadResult(request, AssetLoadStatus.Failed, null, ex.Message, DateTimeOffset.UtcNow);
        }

        _mainThread.Post(() => ApplyOnMainThread(result, afterApply));
    }

    private static AssetLoadResult BuildResult(
        AssetLoadRequest request,
        AssetLoadStatus status,
        int byteCount,
        IReadOnlyDictionary<string, string> metadata,
        string? errorMessage)
    {
        RenderableAssetDto? renderable = status is AssetLoadStatus.Loaded or AssetLoadStatus.Missing
            ? new RenderableAssetDto(
                request.RequestId,
                request.Kind,
                request.Key,
                request.Path,
                byteCount,
                AssetVector3.Zero,
                AssetVector3.One,
                metadata)
            : null;

        return new AssetLoadResult(request, status, renderable, errorMessage, DateTimeOffset.UtcNow);
    }

    private void ApplyOnMainThread(AssetLoadResult result, Action<AssetLoadResult>? afterApply)
    {
        if (result.Renderable is not null)
        {
            _renderables[result.Renderable.Key] = result.Renderable;
            _renderCommands.Enqueue(() => { });
        }

        LastResult = result;
        Interlocked.Decrement(ref _pendingCount);
        afterApply?.Invoke(result);
    }
}
