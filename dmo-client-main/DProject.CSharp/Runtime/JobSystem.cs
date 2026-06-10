using System.Collections.Concurrent;

namespace DProject.Runtime;

public sealed class JobSystem : IDisposable
{
    private readonly BlockingCollection<Func<CancellationToken, Task>> _jobs = new();
    private readonly List<Thread> _workers = new();
    private readonly CancellationTokenSource _cts = new();
    private readonly RuntimeMetrics? _metrics;
    private int _activeJobCount;
    private bool _started;

    public JobSystem(RuntimeMetrics? metrics = null)
    {
        _metrics = metrics;
    }

    public bool IsStarted => _started;
    public int QueueDepth => _jobs.Count;
    public int ActiveJobCount => Volatile.Read(ref _activeJobCount);

    public void Start(int workerCount)
    {
        if (_started)
        {
            return;
        }

        _started = true;
        int count = Math.Max(1, workerCount);
        for (int i = 0; i < count; i++)
        {
            var thread = new Thread(WorkerLoop)
            {
                IsBackground = true,
                Name = $"DProject.JobWorker.{i}"
            };
            _workers.Add(thread);
            thread.Start();
        }
    }

    public void Enqueue(Func<CancellationToken, Task> job)
    {
        ArgumentNullException.ThrowIfNull(job);
        if (!_jobs.IsAddingCompleted)
        {
            _jobs.Add(job);
            _metrics?.RecordJobQueue(QueueDepth, ActiveJobCount);
        }
    }

    public void Enqueue(Action<CancellationToken> job)
    {
        ArgumentNullException.ThrowIfNull(job);
        Enqueue(token =>
        {
            job(token);
            return Task.CompletedTask;
        });
    }

    private void WorkerLoop()
    {
        try
        {
            foreach (Func<CancellationToken, Task> job in _jobs.GetConsumingEnumerable(_cts.Token))
            {
                Interlocked.Increment(ref _activeJobCount);
                var stopwatch = System.Diagnostics.Stopwatch.StartNew();
                try
                {
                    job(_cts.Token).GetAwaiter().GetResult();
                }
                finally
                {
                    stopwatch.Stop();
                    int activeJobs = Interlocked.Decrement(ref _activeJobCount);
                    _metrics?.RecordJobCompleted(stopwatch.Elapsed, QueueDepth, activeJobs);
                }
            }
        }
        catch (OperationCanceledException)
        {
        }
    }

    public void Dispose()
    {
        _jobs.CompleteAdding();
        _cts.Cancel();

        foreach (Thread worker in _workers)
        {
            if (worker.IsAlive)
            {
                worker.Join(TimeSpan.FromSeconds(1));
            }
        }

        _jobs.Dispose();
        _cts.Dispose();
    }
}
