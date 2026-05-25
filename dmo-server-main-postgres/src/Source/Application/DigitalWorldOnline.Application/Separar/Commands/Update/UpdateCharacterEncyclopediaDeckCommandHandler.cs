using DigitalWorldOnline.Commons.Interfaces;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Update
{
    public class UpdateCharacterEncyclopediaDeckCommandHandler : IRequestHandler<UpdateCharacterEncyclopediaDeckCommand>
    {
        private readonly ICharacterCommandsRepository _repository;

        public UpdateCharacterEncyclopediaDeckCommandHandler(ICharacterCommandsRepository repository)
        {
            _repository = repository;
        }

        public async Task<Unit> Handle(UpdateCharacterEncyclopediaDeckCommand request, CancellationToken cancellationToken)
        {
            await _repository.UpdateCharacterEncyclopediaDeckAsync(request.CharacterId, request.DeckId);

            return Unit.Value;
        }
    }
}
