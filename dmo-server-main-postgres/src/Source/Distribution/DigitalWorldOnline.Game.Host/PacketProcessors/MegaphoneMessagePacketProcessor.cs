using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Chat;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class MegaphoneMessagePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.MegaphoneMessage;

        private readonly MapServer _mapServer;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly DungeonsServer _dungeonServer;
        public MegaphoneMessagePacketProcessor(
            MapServer mapServer,
            ILogger logger,
            ISender sender,
            DungeonsServer dungeonsServer)
        {
            _mapServer = mapServer;
            _logger = logger;
            _sender = sender;
            _dungeonServer = dungeonsServer;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var messageLength = packetData.Length > 4 ? packetData[4] : 0;
            var message = NormalizeClientText(packet.ReadString());
            var slot = packet.ReadInt();

            _logger.Information("Megaphone packet from tamer {TamerId}: packetBytes={PacketBytes} stringLen={StringLen} readerPos={ReaderPos} remainingPayloadBytes={RemainingBytes} slot={Slot} raw={RawPacket}",
                client.TamerId,
                packetData.Length,
                messageLength,
                packet.Packet.Position,
                Math.Max(0, packetData.Length - 2 - packet.Packet.Position),
                slot,
                Convert.ToHexString(packetData));

            var inventoryItem = client.Tamer.Inventory.FindItemBySlot(slot);
            if (inventoryItem == null) 
            {
                client.Send(new SystemMessagePacket($"Unable to find item in slot {slot}."));
                _logger.Warning($"Item not found in slot {slot} for player {client.TamerId} megaphone.");
                return;
            }

            if (string.IsNullOrWhiteSpace(message))
            {
                _logger.Warning("Character {TamerId} tried to send an empty megaphone message from slot {Slot} item {ItemId}.",
                    client.TamerId, slot, inventoryItem.ItemId);
                return;
            }

            if (inventoryItem.ItemInfo.Type is not 165 and not 166 and not 167 and not 168 and not 169)
            {
                _logger.Warning("Character {TamerId} sent megaphone with server asset item {ItemId} type {ItemType} from slot {Slot}; allowing because client opened megaphone UI.",
                    client.TamerId, inventoryItem.ItemId, inventoryItem.ItemInfo.Type, slot);
            }

            _mapServer.BroadcastGlobal(new ChatMessagePacket(message, ChatTypeEnum.Megaphone, client.Tamer.Name, 
                inventoryItem.ItemId, client.Tamer.Level).Serialize());


            _dungeonServer.BroadcastGlobal(new ChatMessagePacket(message, ChatTypeEnum.Megaphone, client.Tamer.Name,
                inventoryItem.ItemId, client.Tamer.Level).Serialize());

            _logger.Information("Character {TamerId} sent megaphone slot {Slot} item {ItemId} type {ItemType} message \"{Message}\".",
                client.TamerId, slot, inventoryItem.ItemId, inventoryItem.ItemInfo.Type, message);

            try
            {
                await _sender.Send(new CreateChatMessageCommand(ChatMessageModel.Create(client.TamerId, message)));
            }
            catch (Exception ex)
            {
                _logger.Warning(ex, "Failed to persist megaphone chat history for tamer {TamerId}; gameplay message was already broadcast.",
                    client.TamerId);
            }

            if (!inventoryItem.ItemInfo.TemporaryItem)
            {
                client.Tamer.Inventory.RemoveOrReduceItem(inventoryItem, 1, slot);
                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
            }
        }

        private static string NormalizeClientText(string value)
        {
            return string.IsNullOrEmpty(value)
                ? string.Empty
                : value.Replace("\0", string.Empty).Trim();
        }
    }
}
