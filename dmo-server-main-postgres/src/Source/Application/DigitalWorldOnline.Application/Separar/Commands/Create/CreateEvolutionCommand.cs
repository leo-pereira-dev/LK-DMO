using DigitalWorldOnline.Commons.Models.Digimon;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Create
{
    public class CreateEvolutionCommand : IRequest<long>
    {
        public long DigimonId { get; }
        public DigimonEvolutionModel Evolution { get; }

        public CreateEvolutionCommand(long digimonId, DigimonEvolutionModel evolution)
        {
            DigimonId = digimonId;
            Evolution = evolution;
        }
    }
}
