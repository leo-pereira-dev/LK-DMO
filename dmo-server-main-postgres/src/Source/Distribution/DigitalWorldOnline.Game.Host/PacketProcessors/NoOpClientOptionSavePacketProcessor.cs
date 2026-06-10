using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using MediatR;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public sealed class NoOpClientOptionSavePacketProcessor : IGamePacketProcessor
    {
        private const int DoTutorialClientOption = 0x00000001;
        private const int TutorialIntroVideoClientOption = 0x00000002;
        private const int TutorialFinalQuestId = 4053;
        private const int TutorialIntroVideoMarkerQuestId = 8998;
        private const int TutorialCompletedMarkerQuestId = 8999;

        private readonly ILogger _logger;
        private readonly ISender _sender;

        public NoOpClientOptionSavePacketProcessor(ILogger logger, ISender sender)
        {
            _logger = logger;
            _sender = sender;
        }

        public GameServerPacketEnum Type => GameServerPacketEnum.ClientOptionSave;

        public async Task Process(GameClient client, byte[] packetData)
        {
            var packet = new GamePacketReader(packetData);
            var clientOption = packet.ReadInt();

            var tutorialStillEnabled = (clientOption & DoTutorialClientOption) != 0;
            var introVideoStillEnabled = (clientOption & TutorialIntroVideoClientOption) != 0;
            _logger.Information(
                "Client option save received for tamer {TamerId}: option={ClientOption}, tutorialEnabled={TutorialEnabled}, tutorialIntroVideoEnabled={TutorialIntroVideoEnabled}.",
                client.TamerId,
                clientOption,
                tutorialStillEnabled,
                introVideoStillEnabled);

            if (client.Tamer?.Progress == null)
                return;

            var progress = client.Tamer.Progress;
            var markedIntroVideoDone = false;
            if (!introVideoStillEnabled)
                markedIntroVideoDone = progress.MarkQuestCompleted(TutorialIntroVideoMarkerQuestId);

            if (tutorialStillEnabled)
            {
                if (markedIntroVideoDone)
                {
                    await _sender.Send(new UpdateCharacterProgressCompleteCommand(progress));
                    _logger.Information(
                        "Tutorial intro video marked as watched for tamer {TamerId}: markerQuest={MarkerQuestId}.",
                        client.TamerId,
                        TutorialIntroVideoMarkerQuestId);
                }

                return;
            }

            if (!progress.IsQuestCompleted(TutorialFinalQuestId))
            {
                if (markedIntroVideoDone)
                    await _sender.Send(new UpdateCharacterProgressCompleteCommand(progress));

                _logger.Information(
                    "Ignoring tutorial option clear for tamer {TamerId}: final quest {FinalQuestId} is not completed yet.",
                    client.TamerId,
                    TutorialFinalQuestId);
                return;
            }

            var markedTutorialDone = progress.MarkQuestCompleted(TutorialCompletedMarkerQuestId);

            if (markedIntroVideoDone || markedTutorialDone)
                await _sender.Send(new UpdateCharacterProgressCompleteCommand(progress));

            if (markedTutorialDone)
            {
                _logger.Information(
                    "Tutorial finalization saved for tamer {TamerId}: markerQuest={MarkerQuestId}.",
                    client.TamerId,
                    TutorialCompletedMarkerQuestId);
            }
        }
    }
}
