using System;
using System.IO;

namespace DigitalWorldOnline.Game.Diagnostics
{
    public static class PortalTrace
    {
        private static readonly object Sync = new();

        public static void Write(string message)
        {
            try
            {
                var root = FindRepoRoot() ?? AppContext.BaseDirectory;
                var logDir = Path.Combine(root, "runlogs");
                Directory.CreateDirectory(logDir);

                var line = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff} {message}{Environment.NewLine}";
                lock (Sync)
                {
                    File.AppendAllText(Path.Combine(logDir, "portal-trace.log"), line);
                }
            }
            catch
            {
                // Diagnostics must not affect gameplay.
            }
        }

        private static string? FindRepoRoot()
        {
            var dir = new DirectoryInfo(AppContext.BaseDirectory);
            while (dir != null)
            {
                if (File.Exists(Path.Combine(dir.FullName, "start_servers.bat")) ||
                    Directory.Exists(Path.Combine(dir.FullName, ".git")))
                    return dir.FullName;

                dir = dir.Parent;
            }

            return null;
        }
    }
}
