using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.Items;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class SealOpenPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.OpenSeal;

        private readonly AssetsLoader _assets;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        public SealOpenPacketProcessor(
            AssetsLoader assets,
            ISender sender,
            ILogger logger)
        {
            _assets = assets;
            _sender = sender;
            _logger = logger;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            var sealSlot = packet.ReadShort();
            var sealItem = client.Tamer.Inventory.FindItemBySlot(sealSlot);
            var requestOpener = 1;
            var cardsRemain = 0;

            if (sealItem == null || sealItem.Amount <= 0)
            {
                _logger.Warning("OpenSeal rejected: tamerId={TamerId} tamerName={TamerName} slot={Slot} reason=invalid-slot-or-empty",
                    client.TamerId, client.Tamer?.Name, sealSlot);
                client.Send(new SystemMessagePacket($"Invalid seal slot {sealSlot}."));
                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                return;
            }

            var sealId = sealItem.ItemId;
            _logger.Information("OpenSeal request: tamerId={TamerId} tamerName={TamerName} slot={Slot} sealId={SealId} sealAmount={SealAmount}",
                client.TamerId, client.Tamer?.Name, sealSlot, sealId, sealItem.Amount);

            if (sealItem.Amount >= 50)
            {
                requestOpener = (int)Math.Round((decimal)(sealItem.Amount / 50));
                cardsRemain = (short)Math.Round((decimal)(sealItem.Amount % 50));
            }

            var cardsToOpen = (short)(sealItem.Amount - cardsRemain);

            var availableOpenners = _assets.ItemInfo.Where(x => x.Type == 191 && x.Section == 19101);

            var opennersList = new List<ItemModel>();
            foreach (var availableOpenner in availableOpenners)
            {
                var inventoryCloser = client.Tamer.Inventory.FindItemsById(availableOpenner.ItemId);
                if (inventoryCloser != null) opennersList.AddRange(inventoryCloser);
            }

            opennersList = opennersList.OrderBy(x => x.Slot).ToList();

            var needOpeners = requestOpener;

            foreach (var openner in opennersList)
            {
                if (openner.Amount >= needOpeners)
                {
                    openner.ReduceAmount(needOpeners);
                    needOpeners = 0;
                }
                else
                {
                    needOpeners -= openner.Amount;
                    openner.SetAmount();
                }

                if (needOpeners == 0)
                    break;
            }

            if (needOpeners > 0)
            {
                _logger.Warning("OpenSeal rejected: tamerId={TamerId} tamerName={TamerName} sealId={SealId} sealAmount={SealAmount} requiredOpeners={RequiredOpeners} missingOpeners={MissingOpeners}",
                    client.TamerId, client.Tamer?.Name, sealId, sealItem.Amount, requestOpener, needOpeners);
                client.Send(new SystemMessagePacket($"Invalid openners amount, reload your character."));
                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                return;
            }

            var sealInfo = _assets.SealInfo.FirstOrDefault(x => x.SealId == sealId);
            if (sealInfo != null)
            {
                client.Tamer.SealList.AddOrUpdateSeal(sealId, cardsToOpen, sealInfo.SequentialId);
                client.Partner?.SetSealStatus(_assets.SealInfo);

                client.Send(new UpdateStatusPacket(client.Tamer));

                sealItem.SetAmount(cardsRemain);
                client.Tamer.Inventory.CheckEmptyItems();

                var currentSeal = client.Tamer.SealList.FindSeal(sealId);
                _logger.Information("OpenSeal in-memory success: tamerId={TamerId} tamerName={TamerName} sealListId={SealListId} sealId={SealId} sequential={SequentialId} opened={OpenedAmount} newAmount={NewAmount} remainingItemAmount={RemainingAmount} consumedOpeners={ConsumedOpeners}",
                    client.TamerId, client.Tamer?.Name, client.Tamer.SealList.Id, sealId, sealInfo.SequentialId, cardsToOpen, currentSeal?.Amount, cardsRemain, requestOpener);

                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                await _sender.Send(new UpdateCharacterSealsCommand(client.Tamer.SealList));
                _logger.Information("OpenSeal persisted request sent: tamerId={TamerId} tamerName={TamerName} sealListId={SealListId} sealCount={SealCount}",
                    client.TamerId, client.Tamer?.Name, client.Tamer.SealList.Id, client.Tamer.SealList.Seals.Count);
            }
            else
            {
                _logger.Warning("OpenSeal rejected: tamerId={TamerId} tamerName={TamerName} sealId={SealId} reason=missing-seal-asset",
                    client.TamerId, client.Tamer?.Name, sealId);
                client.Send(new SystemMessagePacket($"Invalid seal id {sealId}."));
                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
            }
        }
    }
}
