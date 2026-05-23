using AutoMapper;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Managers;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class DigimonArchivePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.DigimonArchive;

        private readonly StatusManager _statusManager;
        private readonly IMapper _mapper;
        private readonly DUnitCollectionService _dUnitCollections;
        private readonly ILogger _logger;
        private readonly ISender _sender;

        public DigimonArchivePacketProcessor(
            StatusManager statusManager,
            IMapper mapper,
            DUnitCollectionService dUnitCollections,
            ILogger logger,
            ISender sender)
        {
            _statusManager = statusManager;
            _mapper = mapper;
            _dUnitCollections = dUnitCollections;
            _logger = logger;
            _sender = sender;
        }

        public async Task Process(GameClient client, byte[] packetData)
        {
            var archiveItems = client.Tamer.DigimonArchive.DigimonArchives
                .Where(x => x.DigimonId > 0)
                .OrderBy(x => x.Slot)
                .ToList();

            _logger.Information(
                "ARCHIVE 3204 request begin tamer={TamerId} slots={Slots} archiveItems={ArchiveItems} filledItems={FilledItems}",
                client.TamerId,
                client.Tamer.DigimonArchive.Slots,
                client.Tamer.DigimonArchive.DigimonArchives.Count,
                archiveItems.Count);

            foreach (var digimonArchive in archiveItems)
            {
                _logger.Information(
                    "ARCHIVE 3204 loading digimon tamer={TamerId} archiveSlot={ArchiveSlot} digimonId={DigimonId}",
                    client.TamerId,
                    digimonArchive.Slot,
                    digimonArchive.DigimonId);

                digimonArchive.SetDigimonInfo(_mapper.Map<DigimonModel>(
                    await _sender.Send(
                        new GetDigimonByIdQuery(digimonArchive.DigimonId))
                    )
                );

                digimonArchive.Digimon?.SetBaseInfo(
                    _statusManager.GetDigimonBaseInfo(
                        digimonArchive.Digimon.BaseType
                    )
                );

                digimonArchive.Digimon?.SetTamer(client.Tamer);

                digimonArchive.Digimon?.SetBaseStatus(
                    _statusManager.GetDigimonBaseStatus(
                        digimonArchive.Digimon.BaseType,
                        digimonArchive.Digimon.Level,
                        digimonArchive.Digimon.Size
                    )
                );

                _logger.Information(
                    "ARCHIVE 3204 loaded digimon tamer={TamerId} archiveSlot={ArchiveSlot} digimonId={DigimonId} name={Name} currentType={CurrentType} baseType={BaseType} level={Level} size={Size} evoCount={EvoCount} transExp={TransExp}",
                    client.TamerId,
                    digimonArchive.Slot,
                    digimonArchive.DigimonId,
                    digimonArchive.Digimon?.Name,
                    digimonArchive.Digimon?.CurrentType,
                    digimonArchive.Digimon?.BaseType,
                    digimonArchive.Digimon?.Level,
                    digimonArchive.Digimon?.Size,
                    digimonArchive.Digimon?.Evolutions.Count,
                    digimonArchive.Digimon?.TranscendenceExperience);
            }

            _logger.Debug($"Character {client.TamerId} loaded digimon archive info.");

            _dUnitCollections.ApplyBonuses(client.Tamer);
            client.Send(new UpdateStatusPacket(client.Tamer));

            var packet = new DigimonArchiveLoadPacket(client.Tamer.DigimonArchive).Serialize();
            _logger.Information(
                "ARCHIVE 3204 send tamer={TamerId} packetBytes={PacketBytes} slots={Slots} filledItems={FilledItems} items={Items}",
                client.TamerId,
                packet.Length,
                client.Tamer.DigimonArchive.Slots,
                archiveItems.Count,
                string.Join(",",
                    archiveItems.Select(x =>
                        $"{x.Slot}:{x.DigimonId}:{x.Digimon?.CurrentType}/{x.Digimon?.BaseType}:evo{x.Digimon?.Evolutions.Count}")));

            client.Send(packet);
        }
    }
}
