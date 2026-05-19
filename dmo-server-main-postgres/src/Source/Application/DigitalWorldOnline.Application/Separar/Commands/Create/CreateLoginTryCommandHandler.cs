using DigitalWorldOnline.Commons.DTOs.Account;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Security;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Commands.Create
{
    public class CreateLoginTryCommandHandler : IRequestHandler<CreateLoginTryCommand, LoginTryDTO>
    {
        private const int MaxUsernameLength = 20;
        private const int MaxIpLength = 30;

        private readonly IAccountCommandsRepository _repository;

        public CreateLoginTryCommandHandler(IAccountCommandsRepository repository)
        {
            _repository = repository;
        }

        public async Task<LoginTryDTO> Handle(CreateLoginTryCommand request, CancellationToken cancellationToken)
        {
            var loginTry = new LoginTryModel(
                Normalize(request.Username, MaxUsernameLength, "<empty>"),
                DateTime.Now,
                Normalize(request.IpAddress, MaxIpLength, string.Empty),
                request.Result);

            return await _repository.AddLoginTryAsync(loginTry);
        }

        private static string Normalize(string? value, int maxLength, string fallback)
        {
            var normalized = string.IsNullOrWhiteSpace(value) ? fallback : value.Trim();

            return normalized.Length <= maxLength
                ? normalized
                : normalized[..maxLength];
        }
    }
}
