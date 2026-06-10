using DigitalWorldOnline.Commons.DTOs.Config;
using MediatR;

namespace DigitalWorldOnline.Application.Separar.Queries;

public sealed class KillSpawnsConfigQuery : IRequest<IReadOnlyList<KillSpawnConfigDTO>>
{
}
