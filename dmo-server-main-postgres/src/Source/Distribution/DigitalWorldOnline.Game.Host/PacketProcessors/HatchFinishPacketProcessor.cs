using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.GameHost;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class HatchFinishPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.HatchFinish;

        private readonly StatusManager _statusManager;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public HatchFinishPacketProcessor(
            StatusManager statusManager,
            MapServer mapServer,
            AssetsLoader assets,
            ILogger logger,
            ISender sender,
            DungeonsServer dungeonsServer
        )
        {
            _statusManager = statusManager;
            _mapServer = mapServer;
            _assets = assets;
            _logger = logger;
            _sender = sender;
            _dungeonServer = dungeonsServer;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);

            packet.Skip(4); // Portable incubator inventory slot.
            var digiName = NormalizeHatchName(packet.ReadString(), client.Tamer.Name);

            var hatchInfo = _assets.Hatchs.FirstOrDefault(x => x.ItemId == client.Tamer.Incubator.EggId);
            if (hatchInfo == null)
            {
                _logger.Warning($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}.");
                client.Send(new SystemMessagePacket($"Unknown hatch info for egg {client.Tamer.Incubator.EggId}."));
                return;
            }

            if (client.Tamer.Incubator.HatchLevel < 3)
            {
                _logger.Warning($"Character {client.TamerId} tried to finish hatch with egg {client.Tamer.Incubator.EggId} at level {client.Tamer.Incubator.HatchLevel}.");
                client.Send(new SystemMessagePacket("DigiEgg must be at least level 3 to hatch."));
                return;
            }

            byte targetSlot = 0;
            for (byte slot = 1; slot < client.Tamer.DigimonSlots; slot++)
            {
                if (client.Tamer.Digimons.All(x => x.Slot != slot))
                {
                    targetSlot = slot;
                    break;
                }
            }

            if (targetSlot == 0)
            {
                _logger.Warning($"Character {client.TamerId} tried to hatch {client.Tamer.Incubator.EggId} without an available digivice slot.");
                client.Send(new SystemMessagePacket("No available digivice slot for the hatched Digimon."));
                return;
            }

            var newDigimon = DigimonModel.Create(
                digiName,
                hatchInfo.HatchType,
                hatchInfo.HatchType,
                (DigimonHatchGradeEnum)client.Tamer.Incubator.HatchLevel,
                client.Tamer.Incubator.GetLevelSize(),
                targetSlot
            );

            newDigimon.NewLocation(
                client.Tamer.Location.MapId,
                client.Tamer.Location.X,
                client.Tamer.Location.Y
            );

            newDigimon.SetBaseInfo(
                _statusManager.GetDigimonBaseInfo(
                    newDigimon.BaseType
                )
            );

            newDigimon.SetBaseStatus(
                _statusManager.GetDigimonBaseStatus(
                    newDigimon.BaseType,
                    newDigimon.Level,
                    newDigimon.Size
                )
            );

            newDigimon.AddEvolutions(
                _assets.EvolutionInfo.First(x => x.Type == newDigimon.BaseType)
            );

            if (newDigimon.BaseInfo == null || newDigimon.BaseStatus == null || !newDigimon.Evolutions.Any())
            {
                _logger.Warning($"Unknown digimon info for {newDigimon.BaseType}.");
                client.Send(new SystemMessagePacket($"Unknown digimon info for {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetTamer(client.Tamer);

            var digimonInfo = await _sender.Send(new CreateDigimonCommand(newDigimon));
            if (digimonInfo == null)
            {
                _logger.Warning($"Could not persist hatched digimon {newDigimon.BaseType} for character {client.TamerId}.");
                client.Send(new SystemMessagePacket($"Could not hatch digimon {newDigimon.BaseType}."));
                return;
            }

            newDigimon.SetId(digimonInfo.Id);
            var evolutionSlot = -1;

            foreach (var digimon in newDigimon.Evolutions)
            {
                evolutionSlot++;

                var evolution = digimonInfo.Evolutions[evolutionSlot];

                if (evolution != null)
                {
                    digimon.SetId(evolution.Id);

                    var skillSlot = -1;

                    foreach (var skill in digimon.Skills)
                    {
                        skillSlot++;

                        var dtoSkill = evolution.Skills[skillSlot];

                        skill.SetId(dtoSkill.Id);
                    }
                }
            }

            client.Tamer.Incubator.RemoveEgg();
            client.Tamer.Incubator.RemoveBackupDisk();

            await _sender.Send(new UpdateIncubatorCommand(client.Tamer.Incubator));

            client.Tamer.AddDigimon(newDigimon);

            client.Send(new HatchFinishPacket(newDigimon, (uint)(client.Partner.GeneralHandler + 1000), newDigimon.Slot));

            if (client.Tamer.Incubator.PerfectSize(newDigimon.HatchGrade, newDigimon.Size))
            {
                _mapServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
                _dungeonServer.BroadcastGlobal(new NeonMessagePacket(NeonMessageTypeEnum.Scale, client.Tamer.Name, newDigimon.BaseType, newDigimon.Size).Serialize());
            }

            _logger.Verbose($"Character {client.TamerId} hatched {newDigimon.Id}({newDigimon.BaseType}) with grade {newDigimon.HatchGrade} and size {newDigimon.Size}.");
        }

        private static string NormalizeHatchName(string? value, string fallbackName)
        {
            var name = string.IsNullOrEmpty(value)
                ? string.Empty
                : value.Replace("\0", string.Empty).Trim();

            if (string.IsNullOrWhiteSpace(name))
            {
                name = string.IsNullOrWhiteSpace(fallbackName)
                    ? "Digimon"
                    : fallbackName.Replace("\0", string.Empty).Trim();
            }

            if (string.IsNullOrWhiteSpace(name))
                name = "Digimon";

            const int maxDigimonNameLength = 20;
            if (name.Length > maxDigimonNameLength)
                name = name.Substring(0, maxDigimonNameLength);

            return name;
        }
    }
}
