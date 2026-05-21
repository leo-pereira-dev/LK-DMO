using DigitalWorldOnline.Commons.Models.Character;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Update
{
    public class UpdateCharacterXmlUnionProgressCommand : IRequest
    {
        public CharacterXmlUnionProgressModel Progress { get; }

        public UpdateCharacterXmlUnionProgressCommand(CharacterXmlUnionProgressModel progress)
        {
            Progress = progress;
        }
    }
}
