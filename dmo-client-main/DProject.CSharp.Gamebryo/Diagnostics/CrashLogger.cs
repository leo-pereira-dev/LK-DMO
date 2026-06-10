using System.Text;

namespace DProject.Diagnostics;

public static class CrashLogger
{
    private static readonly object SyncRoot = new();
    private static string _logPath = Path.Combine("logs", "crash.log");
    private static string _statsLogPath = Path.Combine("logs", "stats.log");
    private static string _context = string.Empty;
    private static string _lastPacket = string.Empty;
    private static bool _initialized;

    public static void Init(string? logPath = null)
    {
        lock (SyncRoot)
        {
            if (_initialized)
            {
                return;
            }

            if (!string.IsNullOrWhiteSpace(logPath))
            {
                _logPath = logPath;
            }

            AppDomain.CurrentDomain.UnhandledException += (_, e) =>
            {
                LogFatal("UnhandledException", e.ExceptionObject);
            };

            TaskScheduler.UnobservedTaskException += (_, e) =>
            {
                LogFatal("UnobservedTaskException", e.Exception);
                e.SetObserved();
            };

            _initialized = true;
        }
    }

    public static void LogAssert(string file, int line, string expr, string? details = null)
    {
        var builder = new StringBuilder();
        builder.Append("ASSERT ");
        builder.Append(file);
        builder.Append(':');
        builder.Append(line);
        builder.Append(" expr=");
        builder.Append(expr);
        if (!string.IsNullOrWhiteSpace(details))
        {
            builder.Append(" details=");
            builder.Append(details);
        }

        WriteLine(_logPath, builder.ToString());
    }

    public static void LogMessage(string message, params object?[] args)
    {
        WriteLine(_logPath, Format(message, args));
    }

    public static void LogStatsMessage(string message, params object?[] args)
    {
        WriteLine(_statsLogPath, Format(message, args));
    }

    public static void SetContext(string message, params object?[] args)
    {
        lock (SyncRoot)
        {
            _context = Format(message, args);
        }
    }

    public static void SetLastPacket(string phase, int packet, int bodyBytes, int totalLen)
    {
        lock (SyncRoot)
        {
            _lastPacket = $"{phase} packet={packet} bodyBytes={bodyBytes} totalLen={totalLen}";
        }
    }

    private static void LogFatal(string title, object? exceptionObject)
    {
        var builder = new StringBuilder();
        builder.AppendLine(title);
        builder.AppendLine($"Context: {_context}");
        builder.AppendLine($"LastPacket: {_lastPacket}");
        builder.AppendLine(exceptionObject?.ToString() ?? "<null exception>");
        WriteLine(_logPath, builder.ToString().TrimEnd());
    }

    private static string Format(string message, params object?[] args)
    {
        if (args.Length == 0)
        {
            return message;
        }

        return string.Format(message, args);
    }

    private static void WriteLine(string path, string message)
    {
        lock (SyncRoot)
        {
            string? directory = Path.GetDirectoryName(path);
            if (!string.IsNullOrWhiteSpace(directory))
            {
                Directory.CreateDirectory(directory);
            }

            string line = $"[{DateTimeOffset.Now:O}] {message}{Environment.NewLine}";
            File.AppendAllText(path, line, Encoding.UTF8);
        }
    }
}
