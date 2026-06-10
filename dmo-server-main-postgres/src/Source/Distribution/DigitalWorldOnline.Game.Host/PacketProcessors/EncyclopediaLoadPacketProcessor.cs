using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Asset;
using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;
using Serilog;
using System.Diagnostics;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class EncyclopediaLoadPacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.EncyclopediaLoad;

        private readonly AssetsLoader _assets;
        private readonly ILogger _logger;

        public EncyclopediaLoadPacketProcessor(AssetsLoader assets, ILogger logger)
        {
            _assets = assets;
            _logger = logger;
        }

        public Task Process(GameClient client, byte[] packetData)
        {
            var stopwatch = Stopwatch.StartNew();
            if (client.Tamer?.State == CharacterStateEnum.Loading || client.Loading)
            {
                _logger.Warning(
                    "[EncyclopediaLoad] skipped during loading tamerId={TamerId} state={State} loading={Loading} requestBytes={RequestBytes}",
                    client.TamerId,
                    client.Tamer?.State,
                    client.Loading,
                    packetData?.Length ?? 0);
                return Task.CompletedTask;
            }

            var activeDigimonCount = client.Tamer.Digimons?.Count ?? 0;
            var archiveSlotCount = client.Tamer.DigimonArchive?.DigimonArchives?.Count ?? 0;
            var archiveLoadedCount = client.Tamer.DigimonArchive?.DigimonArchives?.Count(x => x.Digimon != null) ?? 0;
            var archiveMissingCount = client.Tamer.DigimonArchive?.DigimonArchives?.Count(x => x.DigimonId > 0 && x.Digimon == null) ?? 0;
            _logger.Information(
                "[EncyclopediaLoad] start tamerId={TamerId} activeDigimons={ActiveDigimons} archiveSlots={ArchiveSlots} archiveLoaded={ArchiveLoaded} archiveMissing={ArchiveMissing} requestBytes={RequestBytes}",
                client.TamerId,
                activeDigimonCount,
                archiveSlotCount,
                archiveLoadedCount,
                archiveMissingCount,
                packetData?.Length ?? 0);

            var sourceDigimons = new List<(DigimonModel Digimon, string Source)>();
            foreach (var digimon in client.Tamer.Digimons ?? Enumerable.Empty<DigimonModel>())
                sourceDigimons.Add((digimon, $"active:{digimon.Slot}"));

            if (client.Tamer.DigimonArchive?.DigimonArchives != null)
            {
                foreach (var archiveItem in client.Tamer.DigimonArchive.DigimonArchives)
                {
                    if (archiveItem.Digimon == null)
                        continue;

                    sourceDigimons.Add((archiveItem.Digimon, $"archive:{archiveItem.Slot}"));
                }
            }

            var digimonRecords = new Dictionary<int, (DigimonModel Digimon, ulong SlotOpened, List<string> Sources)>();
            foreach (var sourceDigimon in sourceDigimons)
            {
                var digimon = sourceDigimon.Digimon;
                if (digimon.BaseType <= 0)
                    continue;

                var slotOpened = GetSlotOpenedMask(digimon);
                if (digimonRecords.TryGetValue(digimon.BaseType, out var record))
                {
                    if (digimon.Level > record.Digimon.Level)
                        record.Digimon = digimon;

                    record.SlotOpened |= slotOpened;
                    record.Sources.Add(sourceDigimon.Source);
                    digimonRecords[digimon.BaseType] = record;
                }
                else
                {
                    digimonRecords.Add(digimon.BaseType, (digimon, slotOpened, new List<string> { sourceDigimon.Source }));
                }

                _logger.Information(
                    "[EncyclopediaLoad] source tamerId={TamerId} source={Source} digimonId={DigimonId} base={BaseType} current={CurrentType} level={Level} mask=0x{SlotMask:X} evolutions={Evolutions}",
                    client.TamerId,
                    sourceDigimon.Source,
                    digimon.Id,
                    digimon.BaseType,
                    digimon.CurrentType,
                    digimon.Level,
                    slotOpened,
                    FormatEvolutionState(digimon));
            }

            var recordBuildMs = stopwatch.ElapsedMilliseconds;
            PacketWriter writer = new();
            writer.Type(3234);
            writer.WriteInt(digimonRecords.Count);

            foreach (var record in digimonRecords.Values)
            {
                var digimon = record.Digimon;
                writer.WriteInt(digimon.BaseType);
                writer.WriteShort(digimon.Level);

                writer.WriteUInt64(record.SlotOpened);

                writer.WriteShort(digimon.Digiclone.ATLevel);
                writer.WriteShort(digimon.Digiclone.BLLevel);
                writer.WriteShort(digimon.Digiclone.CTLevel);
                writer.WriteShort(digimon.Digiclone.EVLevel);
                writer.WriteShort(digimon.Digiclone.HPLevel);

                writer.WriteShort(digimon.Size);
                writer.WriteByte(0);

                _logger.Information(
                    "[EncyclopediaLoad] record tamerId={TamerId} base={BaseType} selectedDigimonId={DigimonId} sources={Sources} mergedMask=0x{SlotMask:X}",
                    client.TamerId,
                    digimon.BaseType,
                    digimon.Id,
                    string.Join(",", record.Sources),
                    record.SlotOpened);
            }

            var responseBytes = writer.Serialize();
            client.Send(responseBytes);
            _logger.Information(
                "[EncyclopediaLoad] sent tamerId={TamerId} records={Records} responseBytes={ResponseBytes} recordBuildMs={RecordBuildMs} totalMs={TotalMs}",
                client.TamerId,
                digimonRecords.Count,
                responseBytes.Length,
                recordBuildMs,
                stopwatch.ElapsedMilliseconds);
            return Task.CompletedTask;
        }

        private ulong GetSlotOpenedMask(DigimonModel digimon)
        {
            ulong slotOpened = 0;
            var slotByType = GetEvolutionSlotByType(digimon.BaseType);

            if (slotByType.Count > 0)
            {
                foreach (var evolution in digimon.Evolutions)
                {
                    if (evolution.Unlocked <= 0 && digimon.CurrentType != evolution.Type)
                        continue;

                    if (!slotByType.TryGetValue(evolution.Type, out var slotLevel))
                    {
                        _logger.Warning(
                            "[EncyclopediaLoad] missing slot mapping base={BaseType} digimonId={DigimonId} evolutionType={EvolutionType} unlocked={Unlocked}",
                            digimon.BaseType,
                            digimon.Id,
                            evolution.Type,
                            evolution.Unlocked);
                        continue;
                    }

                    var bitIndex = slotLevel - 1;
                    if (bitIndex < 0 || bitIndex >= 64)
                    {
                        _logger.Warning(
                            "[EncyclopediaLoad] invalid slot mapping base={BaseType} digimonId={DigimonId} evolutionType={EvolutionType} slotLevel={SlotLevel}",
                            digimon.BaseType,
                            digimon.Id,
                            evolution.Type,
                            slotLevel);
                        continue;
                    }

                    slotOpened |= 1UL << bitIndex;
                }

                return slotOpened;
            }

            var maxSlot = Math.Min(digimon.Evolutions.Count, 64);
            for (var slot = 0; slot < maxSlot; slot++)
            {
                if (digimon.Evolutions[slot].Unlocked > 0 || digimon.CurrentType == digimon.Evolutions[slot].Type)
                    slotOpened |= 1UL << slot;
            }

            _logger.Warning(
                "[EncyclopediaLoad] fallback slot mask used base={BaseType} digimonId={DigimonId} evolutions={Evolutions} mask=0x{SlotMask:X}",
                digimon.BaseType,
                digimon.Id,
                FormatEvolutionState(digimon),
                slotOpened);

            return slotOpened;
        }

        private Dictionary<int, byte> GetEvolutionSlotByType(int baseType)
        {
            EvolutionAssetModel? evolutionInfo = _assets.EvolutionInfo.FirstOrDefault(x => x.Type == baseType);
            if (evolutionInfo?.Lines == null)
                return new Dictionary<int, byte>();

            var result = new Dictionary<int, byte>();
            foreach (var line in evolutionInfo.Lines)
            {
                if (line.Type <= 0 || line.SlotLevel <= 0 || line.SlotLevel > 64)
                    continue;

                result.TryAdd(line.Type, line.SlotLevel);
            }

            return result;
        }

        private static string FormatEvolutionState(DigimonModel digimon)
        {
            if (digimon.Evolutions == null || digimon.Evolutions.Count == 0)
                return string.Empty;

            return string.Join("|", digimon.Evolutions.Select((x, i) => $"{i}:{x.Type}:u{x.Unlocked}"));
        }
    }
}
