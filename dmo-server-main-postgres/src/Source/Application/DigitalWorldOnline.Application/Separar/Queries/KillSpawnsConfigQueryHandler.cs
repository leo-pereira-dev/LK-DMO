using DigitalWorldOnline.Commons.DTOs.Config;
using DigitalWorldOnline.Commons.Interfaces;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Queries;

public sealed class KillSpawnsConfigQueryHandler : IRequestHandler<KillSpawnsConfigQuery, IReadOnlyList<KillSpawnConfigDTO>>
{
    private readonly IConfigQueriesRepository _configQueriesRepository;

    public KillSpawnsConfigQueryHandler(IConfigQueriesRepository configQueriesRepository)
    {
        _configQueriesRepository = configQueriesRepository;
    }

    public Task<IReadOnlyList<KillSpawnConfigDTO>> Handle(
        KillSpawnsConfigQuery request,
        CancellationToken cancellationToken)
    {
        return _configQueriesRepository.GetKillSpawnsAsync(cancellationToken);
    }
}
