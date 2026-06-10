using System.Collections.Concurrent;

namespace DProject.Runtime;

public sealed class RenderCommandQueue
{
    private readonly ConcurrentQueue<Action> _commands = new();
    private readonly RuntimeMetrics? _metrics;

    public RenderCommandQueue(RuntimeMetrics? metrics = null)
    {
        _metrics = metrics;
    }

    public int QueueDepth => _commands.Count;

    public void Enqueue(Action command)
    {
        ArgumentNullException.ThrowIfNull(command);
        _commands.Enqueue(command);
        _metrics?.RecordRenderCommandQueueDepth(QueueDepth);
    }

    public int ExecuteAll()
    {
        var stopwatch = System.Diagnostics.Stopwatch.StartNew();
        int executed = 0;
        while (_commands.TryDequeue(out Action? command))
        {
            command();
            executed++;
        }

        stopwatch.Stop();
        _metrics?.RecordRenderCommands(stopwatch.Elapsed, executed, QueueDepth);
        return executed;
    }
}
