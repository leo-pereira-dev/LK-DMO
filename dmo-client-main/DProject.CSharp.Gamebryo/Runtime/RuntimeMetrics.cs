namespace DProject.Runtime;

public sealed record RuntimeMetricsSnapshot(
    TimeSpan LastFrameTime,
    TimeSpan LastNetworkPumpTime,
    TimeSpan LastJobTime,
    TimeSpan LastRenderCommandTime,
    TimeSpan LastMainThreadDrainTime,
    int MainThreadQueueDepth,
    int NetworkQueueDepth,
    int JobQueueDepth,
    int ActiveJobCount,
    int RenderCommandQueueDepth,
    int LastMainThreadActions,
    int LastNetworkActions,
    int LastRenderCommands,
    long TotalJobsCompleted,
    long TotalNetworkPumps,
    long TotalFrames);

public sealed class RuntimeMetrics
{
    private long _lastFrameTicks;
    private long _lastNetworkPumpTicks;
    private long _lastJobTicks;
    private long _lastRenderCommandTicks;
    private long _lastMainThreadDrainTicks;
    private long _totalJobsCompleted;
    private long _totalNetworkPumps;
    private long _totalFrames;
    private int _mainThreadQueueDepth;
    private int _networkQueueDepth;
    private int _jobQueueDepth;
    private int _activeJobCount;
    private int _renderCommandQueueDepth;
    private int _lastMainThreadActions;
    private int _lastNetworkActions;
    private int _lastRenderCommands;

    public RuntimeMetricsSnapshot Snapshot()
    {
        return new RuntimeMetricsSnapshot(
            TimeSpan.FromTicks(Interlocked.Read(ref _lastFrameTicks)),
            TimeSpan.FromTicks(Interlocked.Read(ref _lastNetworkPumpTicks)),
            TimeSpan.FromTicks(Interlocked.Read(ref _lastJobTicks)),
            TimeSpan.FromTicks(Interlocked.Read(ref _lastRenderCommandTicks)),
            TimeSpan.FromTicks(Interlocked.Read(ref _lastMainThreadDrainTicks)),
            Volatile.Read(ref _mainThreadQueueDepth),
            Volatile.Read(ref _networkQueueDepth),
            Volatile.Read(ref _jobQueueDepth),
            Volatile.Read(ref _activeJobCount),
            Volatile.Read(ref _renderCommandQueueDepth),
            Volatile.Read(ref _lastMainThreadActions),
            Volatile.Read(ref _lastNetworkActions),
            Volatile.Read(ref _lastRenderCommands),
            Interlocked.Read(ref _totalJobsCompleted),
            Interlocked.Read(ref _totalNetworkPumps),
            Interlocked.Read(ref _totalFrames));
    }

    public void RecordFrame(TimeSpan elapsed, int mainThreadActions, int renderCommands)
    {
        Interlocked.Exchange(ref _lastFrameTicks, elapsed.Ticks);
        Interlocked.Exchange(ref _lastMainThreadActions, mainThreadActions);
        Interlocked.Exchange(ref _lastRenderCommands, renderCommands);
        Interlocked.Increment(ref _totalFrames);
    }

    public void RecordMainThreadQueueDepth(int queueDepth)
    {
        Interlocked.Exchange(ref _mainThreadQueueDepth, queueDepth);
    }

    public void RecordMainThreadDrain(TimeSpan elapsed, int actions, int queueDepth)
    {
        Interlocked.Exchange(ref _lastMainThreadDrainTicks, elapsed.Ticks);
        Interlocked.Exchange(ref _lastMainThreadActions, actions);
        RecordMainThreadQueueDepth(queueDepth);
    }

    public void RecordNetworkQueueDepth(int queueDepth)
    {
        Interlocked.Exchange(ref _networkQueueDepth, queueDepth);
    }

    public void RecordNetworkPump(TimeSpan elapsed, int actions, int queueDepth)
    {
        Interlocked.Exchange(ref _lastNetworkPumpTicks, elapsed.Ticks);
        Interlocked.Exchange(ref _lastNetworkActions, actions);
        RecordNetworkQueueDepth(queueDepth);
        Interlocked.Increment(ref _totalNetworkPumps);
    }

    public void RecordJobQueue(int queueDepth, int activeJobCount)
    {
        Interlocked.Exchange(ref _jobQueueDepth, queueDepth);
        Interlocked.Exchange(ref _activeJobCount, activeJobCount);
    }

    public void RecordJobCompleted(TimeSpan elapsed, int queueDepth, int activeJobCount)
    {
        Interlocked.Exchange(ref _lastJobTicks, elapsed.Ticks);
        RecordJobQueue(queueDepth, activeJobCount);
        Interlocked.Increment(ref _totalJobsCompleted);
    }

    public void RecordRenderCommandQueueDepth(int queueDepth)
    {
        Interlocked.Exchange(ref _renderCommandQueueDepth, queueDepth);
    }

    public void RecordRenderCommands(TimeSpan elapsed, int executed, int queueDepth)
    {
        Interlocked.Exchange(ref _lastRenderCommandTicks, elapsed.Ticks);
        Interlocked.Exchange(ref _lastRenderCommands, executed);
        RecordRenderCommandQueueDepth(queueDepth);
    }
}
