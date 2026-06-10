namespace DProject.App;

public abstract class CWin32App : IDisposable
{
    public enum PROCESS_STATE
    {
        PROCESS_RUN,
        PROCESS_STOP
    }

    private PROCESS_STATE _processState = PROCESS_STATE.PROCESS_RUN;
    private bool _disposed;

    protected CWin32App()
    {
    }

    public string WindowTitle { get; private set; } = "DProject";
    public int Width { get; private set; }
    public int Height { get; private set; }
    public bool FullMode { get; private set; }

    public virtual bool Startup(string windowTitle, int width, int height, bool fullMode)
    {
        WindowTitle = windowTitle;
        Width = width;
        Height = height;
        FullMode = fullMode;
        return true;
    }

    public int Run()
    {
        if (!OnInitialize())
        {
            OnTerminate();
            return 1;
        }

        int ticks = 0;
        while (_processState == PROCESS_STATE.PROCESS_RUN && CanContinueLoop(ticks))
        {
            OnIdleExtern();
            OnIdle();
            ticks++;
        }

        OnTerminate();
        return 0;
    }

    public void SetProcessState(PROCESS_STATE state)
    {
        _processState = state;
    }

    protected virtual bool CanContinueLoop(int ticks)
    {
        return true;
    }

    protected abstract bool OnInitialize();
    protected abstract void OnIdle();
    protected virtual void OnIdleExtern() { }
    protected virtual void OnTerminate() { }

    public void Dispose()
    {
        if (_disposed)
        {
            return;
        }

        _disposed = true;
        GC.SuppressFinalize(this);
    }
}

