using DigitalWorldOnline.Application.GameAssets.Bins;
using DigitalWorldOnline.Commons.Packets.GameServer;
using Serilog;
using System.Collections.Concurrent;

namespace DigitalWorldOnline.Game.Managers
{
    public sealed class DailyCheckService
    {
        private const uint DefaultNextLeftSeconds = 86403;

        private readonly EventTableBinLoader _eventTable;
        private readonly ILogger _logger;
        private readonly ConcurrentDictionary<(long TamerId, uint GroupNo), ClaimState> _claims = new();

        public DailyCheckService(EventTableBinLoader eventTable, ILogger logger)
        {
            _eventTable = eventTable;
            _logger = logger;
        }

        public IReadOnlyList<DailyCheckEventGroupState> GetSnapshot(long tamerId, DateTime now)
        {
            return ActiveGroups(now)
                .Select(group => BuildState(tamerId, group, now))
                .ToList();
        }

        public EventDailyCheckGroup? FindGroup(uint groupNo, DateTime now)
        {
            return ActiveGroups(now).FirstOrDefault(x => x.GroupNo == groupNo);
        }

        public DailyCheckEventGroupState BuildState(long tamerId, EventDailyCheckGroup group, DateTime now)
        {
            var currentDay = CurrentDay(group, now);
            var bits = new byte[((Math.Max(1, currentDay) - 1) / 8) + 1];

            if (_claims.TryGetValue((tamerId, group.GroupNo), out var state)
                && state.Day == currentDay
                && state.ClaimDate == now.Date)
            {
                SetBit(bits, currentDay);
            }

            return new DailyCheckEventGroupState(
                group.GroupNo,
                currentDay,
                SecondsUntilNextReset(group, now),
                bits);
        }

        public bool HasClaimed(long tamerId, uint groupNo, uint currentDay, DateTime now)
        {
            return _claims.TryGetValue((tamerId, groupNo), out var state)
                && state.Day == currentDay
                && state.ClaimDate == now.Date;
        }

        public void RecordClaim(long tamerId, uint groupNo, uint currentDay, DateTime now)
        {
            _claims[(tamerId, groupNo)] = new ClaimState(currentDay, now.Date);
        }

        public uint CurrentDay(EventDailyCheckGroup group, DateTime now)
        {
            var rewardCount = Math.Max(1, group.Rewards.Count);
            var elapsedDays = 0;

            if (group.StartDate != default)
                elapsedDays = Math.Max(0, (now.Date - group.StartDate.Date).Days);

            return (uint)((elapsedDays % rewardCount) + 1);
        }

        public uint SecondsUntilNextReset(EventDailyCheckGroup group, DateTime now)
        {
            var reset = group.ResetTime == default ? TimeSpan.Zero : group.ResetTime;
            var next = now.Date.Add(reset);
            if (next <= now)
                next = next.AddDays(1);

            var seconds = (uint)Math.Max(1, (int)(next - now).TotalSeconds);
            return seconds == 0 ? DefaultNextLeftSeconds : seconds;
        }

        private IEnumerable<EventDailyCheckGroup> ActiveGroups(DateTime now)
        {
            var groups = _eventTable.Data.DailyCheck;
            if (groups.Count == 0)
            {
                _logger.Debug("DailyCheck requested but Event.bin has no §6 groups.");
                return Enumerable.Empty<EventDailyCheckGroup>();
            }

            var active = groups.Where(group =>
                group.Rewards.Count > 0
                && (group.StartDate == default || group.EndDate == default
                    || now.Date >= group.StartDate.Date && now.Date <= group.EndDate.Date))
                .ToList();

            return active.Count > 0 ? active : groups.Where(group => group.Rewards.Count > 0);
        }

        private static void SetBit(byte[] bits, uint oneBasedDay)
        {
            if (oneBasedDay == 0) return;
            var zeroBased = (int)oneBasedDay - 1;
            var byteIndex = zeroBased / 8;
            var bitIndex = zeroBased % 8;
            if (byteIndex >= 0 && byteIndex < bits.Length)
                bits[byteIndex] |= (byte)(1 << bitIndex);
        }

        private sealed record ClaimState(uint Day, DateTime ClaimDate);
    }
}
