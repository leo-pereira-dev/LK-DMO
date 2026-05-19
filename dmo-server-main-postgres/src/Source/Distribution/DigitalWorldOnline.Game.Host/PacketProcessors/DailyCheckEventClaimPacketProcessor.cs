using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Game.Managers;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class DailyCheckEventClaimPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DailyCheckEventRequest;

        private readonly DailyCheckService _dailyCheck;
        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public DailyCheckEventClaimPacketProcessor(
            DailyCheckService dailyCheck,
            AssetsLoader assets,
            ISender sender,
            ILogger logger)
        {
            _dailyCheck = dailyCheck;
            _assets = assets;
            _sender = sender;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var groupNo = packet.ReadUInt();
            var now = DateTime.UtcNow;
            var group = _dailyCheck.FindGroup(groupNo, now);

            if (group == null)
            {
                client.Send(new DailyCheckEventClaimResultPacket(
                    DailyCheckEventClaimResultPacket.ResultNotInTime, groupNo, 0, 0));
                return;
            }

            var currentDay = _dailyCheck.CurrentDay(group, now);
            var nextLeft = _dailyCheck.SecondsUntilNextReset(group, now);

            if (_dailyCheck.HasClaimed(client.TamerId, groupNo, currentDay, now))
            {
                client.Send(new DailyCheckEventClaimResultPacket(
                    DailyCheckEventClaimResultPacket.ResultAlreadyClaimed, groupNo, currentDay, nextLeft));
                return;
            }

            var reward = group.Rewards.ElementAtOrDefault((int)currentDay - 1);
            if (reward == null || reward.ItemId == 0 || reward.ItemCount == 0)
            {
                client.Send(new DailyCheckEventClaimResultPacket(
                    DailyCheckEventClaimResultPacket.ResultNotInTime, groupNo, currentDay, nextLeft));
                return;
            }

            var itemInfo = _assets.ItemInfo.FirstOrDefault(x => x.ItemId == reward.ItemId);
            if (itemInfo == null)
            {
                _logger.Warning(
                    "DailyCheck group {GroupNo} day {Day} references unknown item {ItemId}.",
                    groupNo, currentDay, reward.ItemId);
                client.Send(new DailyCheckEventClaimResultPacket(
                    DailyCheckEventClaimResultPacket.ResultNotInTime, groupNo, currentDay, nextLeft));
                return;
            }

            var item = new ItemModel();
            item.SetItemInfo(itemInfo);
            item.ItemId = (int)reward.ItemId;
            item.Amount = (int)reward.ItemCount;
            item.EndDate = DateTime.UtcNow.AddDays(14);
            if (item.IsTemporary)
                item.SetRemainingTime((uint)item.ItemInfo.UsageTimeMinutes);

            if (!client.Tamer.GiftWarehouse.AddGiftItem(item))
            {
                _logger.Warning(
                    "DailyCheck claim from tamer {TamerId} group {GroupNo} day {Day} failed: GiftWarehouse full.",
                    client.TamerId, groupNo, currentDay);
                client.Send(new DailyCheckEventClaimResultPacket(
                    DailyCheckEventClaimResultPacket.ResultNotInTime, groupNo, currentDay, nextLeft));
                return;
            }

            _dailyCheck.RecordClaim(client.TamerId, groupNo, currentDay, now);
            await _sender.Send(new UpdateItemsCommand(client.Tamer.GiftWarehouse));

            client.Send(new DailyCheckEventClaimResultPacket(
                DailyCheckEventClaimResultPacket.ResultSuccess, groupNo, currentDay, nextLeft));
            client.Send(new LoadGiftStoragePacket(client.Tamer.GiftWarehouse));

            _logger.Verbose(
                "DailyCheck claim succeeded: tamer {TamerId} group {GroupNo} day {Day} item {ItemId} x{Count} to GiftWarehouse.",
                client.TamerId, groupNo, currentDay, reward.ItemId, reward.ItemCount);
        }
    }
}
