using DigitalWorldOnline.Commons.Interfaces;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Create
{
    public class CreateEvolutionCommandHandler : IRequestHandler<CreateEvolutionCommand, long>
    {
        private readonly ICharacterCommandsRepository _repository;

        public CreateEvolutionCommandHandler(ICharacterCommandsRepository repository)
        {
            _repository = repository;
        }

        public async Task<long> Handle(CreateEvolutionCommand request, CancellationToken cancellationToken)
        {
            return await _repository.AddEvolutionAsync(request.DigimonId, request.Evolution);
        }
    }
}
