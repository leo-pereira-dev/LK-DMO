using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class DigimonTranscendenceSuccessPacketProcessor : IGamePacketProcessor
    {
        private const long MaxTranscendenceExperience = 140000;
        private const long TranscendenceCost = 5000000;

        private const int ResultSuccess = 0;
        private const int ResultFail = 1;
        private const int ResultNotUseAvailableMaterial = 20106;
        private const int ResultNotExp = 20109;
        private const int ResultNotTransCost = 20110;
        private const int ResultAlreadyTranscendence = 20111;

        public GameServerPacketEnum Type => GameServerPacketEnum.DigimonTranscendence;

        private readonly ILogger _logger;
        private readonly ISender _sender;

        public DigimonTranscendenceSuccessPacketProcessor(
            ILogger logger,
            ISender sender)
        {
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            try
            {
                var packet = new GamePacketReader(packetData);

                var activeItemSlot = packet.ReadInt();
                var npcId = packet.ReadInt();
                var targetSlot = packet.ReadByte();
                var requestedMoney = packet.ReadInt64();

                var targetPartner = ClientDigimonSlotResolver.Resolve(client, targetSlot, out var resolvedTargetSlot);
                if (targetPartner == null)
                {
                    _logger.Warning("[Transcendence] Finish failed: tamer {TamerId} target slot {TargetSlot} not found. NPC {NpcId} itemSlot {ItemSlot}. available slots [{Slots}].",
                        client.TamerId, targetSlot, npcId, activeItemSlot, string.Join(",", client.Tamer.Digimons.Select(x => x.Slot)));
                    client.Send(new DigimonTranscendenceSuccessPacket(ResultNotUseAvailableMaterial));
                    return;
                }

                if (resolvedTargetSlot != targetSlot)
                {
                    _logger.Information("[Transcendence] Finish target slot resolved: tamer {TamerId} clientSlot {ClientSlot} realSlot {RealSlot} target {TargetId}.",
                        client.TamerId, targetSlot, resolvedTargetSlot, targetPartner.Id);
                }

                if (targetPartner.HatchGrade == DigimonHatchGradeEnum.Transcend)
                {
                    _logger.Warning("[Transcendence] Finish rejected: digimon {DigimonId} already transcendence.", targetPartner.Id);
                    client.Send(new DigimonTranscendenceSuccessPacket(ResultAlreadyTranscendence));
                    return;
                }

                if (targetPartner.TranscendenceExperience < MaxTranscendenceExperience)
                {
                    _logger.Warning("[Transcendence] Finish rejected: digimon {DigimonId} exp {Exp}/{MaxExp}.",
                        targetPartner.Id, targetPartner.TranscendenceExperience, MaxTranscendenceExperience);
                    client.Send(new DigimonTranscendenceSuccessPacket(ResultNotExp));
                    return;
                }

                if (client.Tamer.Inventory.Bits < TranscendenceCost)
                {
                    _logger.Warning("[Transcendence] Finish rejected: tamer {TamerId} bits {Bits} cost {Cost}.",
                        client.TamerId, client.Tamer.Inventory.Bits, TranscendenceCost);
                    client.Send(new DigimonTranscendenceSuccessPacket(ResultNotTransCost));
                    return;
                }

                if (requestedMoney != TranscendenceCost)
                {
                    _logger.Warning("[Transcendence] Finish cost mismatch: tamer {TamerId} requested {RequestedMoney}, server {ServerCost}.",
                        client.TamerId, requestedMoney, TranscendenceCost);
                }

                client.Tamer.Inventory.RemoveBits(TranscendenceCost);
                targetPartner.Transcend();

                client.Send(new DigimonTranscendenceSuccessPacket(
                    ResultSuccess,
                    targetSlot,
                    targetPartner.HatchGrade,
                    TranscendenceCost,
                    client.Tamer.Inventory.Bits,
                    targetPartner.TranscendenceExperience));

                await _sender.Send(new UpdateDigimonGradeCommand(targetPartner.Id, targetPartner.HatchGrade));
                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));

                _logger.Information("[Transcendence] Finished: tamer {TamerId} digimon {DigimonId} slot {TargetSlot} cost {Cost} remainingBits {Bits}.",
                    client.TamerId, targetPartner.Id, targetSlot, TranscendenceCost, client.Tamer.Inventory.Bits);
            }
            catch (Exception ex)
            {
                _logger.Error(ex, "[Transcendence] Finish packet failed for tamer {TamerId}.", client.TamerId);
                client.Send(new DigimonTranscendenceSuccessPacket(ResultNotUseAvailableMaterial));
            }
        }

    }
}
