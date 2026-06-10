using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Game.Configuration;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;

namespace DigitalWorldOnline.Game.PacketProcessors
{
    public class PartyMemberLeavePacketProcessor : IGamePacketProcessor
    {
        public GameServerPacketEnum Type => GameServerPacketEnum.PartyMemberLeave;

        private const string GamerServerPublic = "GameServer:PublicAddress";
        private readonly PartyManager _partyManager;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly IConfiguration _configuration;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;
        
        public PartyMemberLeavePacketProcessor(
             PartyManager partyManager,
            MapServer mapServer,
            ILogger logger,
            ISender sender,
            IConfiguration configuration,
            DungeonsServer dungeonServer,
            OwnerStorageFlushService ownerStorageFlushService)
        {
            _partyManager = partyManager;
            _mapServer = mapServer;
            _logger = logger;
            _sender = sender;
            _configuration = configuration;
            _dungeonServer = dungeonServer;
            _ownerStorageFlushService = ownerStorageFlushService;
        }
        public async Task Process(GameClient client, byte[] packetData)
        {
            var party = _partyManager.FindParty(client.TamerId);
            if (party != null)
            {

                _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberLeavePacket(party[client.TamerId].Key).Serialize()
                );

                _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                    new PartyMemberLeavePacket(party[client.TamerId].Key).Serialize()
                );

                if (party.Members.Count == 2)
                {
                    var map = UtilitiesFunctions.MapGroup(client.Tamer.Location.MapId);
                    var fallbackMap = -1;

                    if (map < 0)
                    {
                        foreach (var member in party.Members)
                        {
                            var partyClient = _dungeonServer.FindClientByTamerId(member.Value.Id);

                            if (partyClient == null || partyClient.Tamer == null)
                                continue;

                            var memberMap = UtilitiesFunctions.MapGroup(partyClient.Tamer.Location.MapId);
                            if (memberMap > 0)
                            {
                                fallbackMap = memberMap;
                                break;
                            }
                        }
                    }

                    if (fallbackMap > 0)
                    {
                        map = fallbackMap;
                    }

                    if (map < 0)
                    {
                        client.Send(new SystemMessagePacket($"Failed to find a valid map for party jump booster. Party was left normally."));
                        _logger.Warning($"Invalid map group for character {client.TamerId} while leaving party {party.Id}.");
                    }
                    else
                    {
                        var waypoints = await _sender.Send(new MapRegionListAssetsByMapIdQuery(map));

                        if (waypoints == null || !waypoints.Regions.Any())
                        {
                            client.Send(new SystemMessagePacket($"Map information not found for map Id {map}."));
                            _logger.Warning($"Map information not found for map Id {map} on character {client.TamerId} jump booster.");
                        }
                        else
                        {
                            var destination = waypoints.Regions.First();

                            foreach (var member in party.Members)
                            {
                                var dungeonClient = _dungeonServer.FindClientByTamerId(member.Value.Id);

                                if (dungeonClient == null)
                                {
                                    continue;
                                }

                                await _ownerStorageFlushService.FlushForTransitionAsync(dungeonClient);
                                _dungeonServer.RemoveClient(dungeonClient);

                                dungeonClient.Tamer.NewLocation(map, destination.X, destination.Y);
                                await _sender.Send(new UpdateCharacterLocationCommand(dungeonClient.Tamer.Location));

                                dungeonClient.Tamer.Partner.NewLocation(map, destination.X, destination.Y);
                                await _sender.Send(new UpdateDigimonLocationCommand(dungeonClient.Tamer.Partner.Location));

                                dungeonClient.Tamer.UpdateState(CharacterStateEnum.Loading);
                                await _sender.Send(new UpdateCharacterStateCommand(dungeonClient.TamerId, CharacterStateEnum.Loading));

                                _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                                    new PartyMemberWarpGatePacket(party[dungeonClient.TamerId]).Serialize());



                                dungeonClient.SetGameQuit(false);

                                dungeonClient.Send(new MapSwapPacket(
                                    _configuration[GamerServerPublic],
                                    _configuration.GetPublicGameServerPort(),
                                    dungeonClient.Tamer.Location.MapId,
                                    dungeonClient.Tamer.Location.X,
                                    dungeonClient.Tamer.Location.Y));
                            }
                        }
                    }
                }

                if(party.LeaderId == party[client.TamerId].Key)
                {
                    party.RemoveMember(party[client.TamerId].Key);

                    var randomIndex = new Random().Next(party.Members.Count);
                    var sortedPlayer = party.Members.ElementAt(randomIndex).Key;

                    party.ChangeLeader(sortedPlayer);

                    _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                new PartyLeaderChangedPacket(sortedPlayer)
                  .Serialize());

                    _dungeonServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                new PartyLeaderChangedPacket(sortedPlayer)
                  .Serialize());
                }
                else
                {
                    party.RemoveMember(party[client.TamerId].Key);
                }
              

                if (party.Members.Count < 2)
                {

                    _partyManager.RemoveParty(party.Id);
                }

                _logger.Verbose($"Character {client.TamerId} left the party {party.Id}.");
            }
        }
    }
}
