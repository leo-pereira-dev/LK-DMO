using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Update
{
    public class UpdateCharacterEncyclopediaDeckCommand : IRequest
    {
        public long CharacterId { get; }
        public int DeckId { get; }

        public UpdateCharacterEncyclopediaDeckCommand(long characterId, int deckId)
        {
            CharacterId = characterId;
            DeckId = deckId;
        }
    }
}
