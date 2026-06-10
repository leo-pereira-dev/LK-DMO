using System.Collections.Concurrent;

namespace DProject.Runtime;

public sealed class MainThreadDispatcher
{
    private readonly ConcurrentQueue<Action> _queue = new();
    private readonly RuntimeMetrics? _metrics;
    private int _mainThreadId;

    public MainThreadDispatcher(RuntimeMetrics? metrics = null)
    {
        _metrics = metrics;
    }

    public void BindToCurrentThread()
    {
        _mainThreadId = Environment.CurrentManagedThreadId;
    }

    public bool IsMainThread => Environment.CurrentManagedThreadId == _mainThreadId;
    public int QueueDepth => _queue.Count;

    public void Post(Action action)
    {
        ArgumentNullException.ThrowIfNull(action);
        _queue.Enqueue(action);
        _metrics?.RecordMainThreadQueueDepth(QueueDepth);
    }

    public int Drain(int maxActions)
    {
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();
        int processed = 0;
        while (processed < maxActions && _queue.TryDequeue(out Action? action))
        {
            action();
            processed++;
        }

        stopwatch.Stop();
        _metrics?.RecordMainThreadDrain(stopwatch.Elapsed, processed, QueueDepth);
        return processed;
    }
}
