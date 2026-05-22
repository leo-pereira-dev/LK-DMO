using DigitalWorldOnline.Commons.Interfaces;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Update
{
    public class UpdateCharacterXmlUnionProgressCommandHandler : IRequestHandler<UpdateCharacterXmlUnionProgressCommand>
    {
        private readonly ICharacterCommandsRepository _repository;

        public UpdateCharacterXmlUnionProgressCommandHandler(ICharacterCommandsRepository repository)
        {
            _repository = repository;
        }

        public async Task<Unit> Handle(UpdateCharacterXmlUnionProgressCommand request, CancellationToken cancellationToken)
        {
            await _repository.UpdateCharacterXmlUnionProgressAsync(request.Progress);
            return Unit.Value;
        }
    }
}
