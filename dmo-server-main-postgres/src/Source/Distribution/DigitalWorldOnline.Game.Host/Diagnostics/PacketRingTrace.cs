using DigitalWorldOnline.Commons.Entities;
using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
using System.Text;

namespace DigitalWorldOnline.Game.Diagnostics
{
    public static class PacketRingTrace
    {
        private const int MaxEntries = 1000;
        private const int MaxHexBytes = 96;
        private static readonly ConcurrentDictionary<GameClient, ClientPacketRing> Rings = new();
        private static long _sequence;

        public static void RecordSent(GameClient client, byte[] buffer, int start, int count)
        {
            Record(client, "S", buffer, start, count);
        }

        public static void RecordReceived(GameClient client, byte[] buffer)
        {
            Record(client, "R", buffer, 0, buffer.Length);
        }

        public static void DumpAndForget(GameClient client, string reason)
        {
            if (!Rings.TryRemove(client, out var ring))
                return;

            try
            {
                var root = FindRepoRoot() ?? AppContext.BaseDirectory;
                var logDir = Path.Combine(root, "runlogs", "packet-ring");
                Directory.CreateDirectory(logDir);

                var safeReason = string.Concat(reason.Select(ch =>
                    char.IsLetterOrDigit(ch) || ch == '-' || ch == '_' ? ch : '_'));

                var fileName =
                    $"{DateTime.Now:yyyyMMdd-HHmmssfff}_t{client.TamerId}_a{client.AccountId}_c{RuntimeHelpers.GetHashCode(client)}_{safeReason}.log";
                var path = Path.Combine(logDir, fileName);

                var entries = ring.Snapshot();
                var builder = new StringBuilder();
                builder.AppendLine($"Packet ring dump");
                builder.AppendLine($"reason={reason}");
                builder.AppendLine($"created={DateTime.Now:yyyy-MM-dd HH:mm:ss.fff}");
                builder.AppendLine($"clientHash={RuntimeHelpers.GetHashCode(client)}");
                builder.AppendLine($"account={client.AccountId}");
                builder.AppendLine($"tamer={client.TamerId}");
                builder.AppendLine($"map={client.Tamer?.Location?.MapId}");
                builder.AppendLine($"x={client.Tamer?.Location?.X}");
                builder.AppendLine($"y={client.Tamer?.Location?.Y}");
                builder.AppendLine($"gameQuit={client.GameQuit}");
                builder.AppendLine($"entries={entries.Count}");
                builder.AppendLine();

                foreach (var entry in entries)
                    builder.AppendLine(entry);

                File.WriteAllText(path, builder.ToString());
                PortalTrace.Write($"PacketRing dump tamer={client.TamerId} reason={reason} file={path}");
            }
            catch
            {
                // Diagnostics must not affect gameplay.
            }
        }

        private static void Record(GameClient client, string direction, byte[] buffer, int start, int count)
        {
            try
            {
                if (buffer.Length < 4 || start < 0 || start >= buffer.Length)
                    return;

                var safeCount = Math.Min(count, buffer.Length - start);
                if (safeCount < 4)
                    return;

                var wireLength = BitConverter.ToUInt16(buffer, start);
                var opcode = BitConverter.ToUInt16(buffer, start + 2);
                var subtype = opcode == 1006 && safeCount > 4 ? buffer[start + 4].ToString() : "-";
                var effectiveLength = wireLength >= 6 && wireLength <= safeCount ? wireLength : safeCount;
                var hexLength = Math.Min(Math.Min(effectiveLength, MaxHexBytes), buffer.Length - start);
                var hex = Convert.ToHexString(buffer, start, hexLength);

                var entry =
                    $"{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff} " +
                    $"seq={Interlocked.Increment(ref _sequence)} dir={direction} " +
                    $"op={opcode} sub={subtype} wireLen={wireLength} bufLen={safeCount} " +
                    $"tamer={client.TamerId} map={client.Tamer?.Location?.MapId ?? 0} " +
                    $"x={client.Tamer?.Location?.X ?? 0} y={client.Tamer?.Location?.Y ?? 0} " +
                    $"hex={hex}";

                Rings.GetOrAdd(client, _ => new ClientPacketRing()).Add(entry);
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

        private sealed class ClientPacketRing
        {
            private readonly Queue<string> _entries = new();
            private readonly object _sync = new();

            public void Add(string entry)
            {
                lock (_sync)
                {
                    _entries.Enqueue(entry);
                    while (_entries.Count > MaxEntries)
                        _entries.Dequeue();
                }
            }

            public List<string> Snapshot()
            {
                lock (_sync)
                {
                    return _entries.ToList();
                }
            }
        }
    }
}
