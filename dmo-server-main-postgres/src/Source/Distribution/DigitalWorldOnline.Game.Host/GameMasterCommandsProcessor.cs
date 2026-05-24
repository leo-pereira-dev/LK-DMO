using DigitalWorldOnline.Application;
using DigitalWorldOnline.Application.GameAssets;
using DigitalWorldOnline.Application.Admin.Queries;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Application.GameAssets.Queries;
using DigitalWorldOnline.Commons.DTOs.Account;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Account;
using DigitalWorldOnline.Commons.Enums.Character;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Models.Summon;
using DigitalWorldOnline.Commons.Packets.Chat;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Commons.Packets.GameServer.Combat;
using DigitalWorldOnline.Commons.Packets.Items;
using DigitalWorldOnline.Commons.Packets.MapServer;
using DigitalWorldOnline.Commons.Utils;
using DigitalWorldOnline.Commons.Writers;
using DigitalWorldOnline.Game.Managers;
using DigitalWorldOnline.Game.Services;
using DigitalWorldOnline.GameHost;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;
using System.Text.RegularExpressions;

namespace DigitalWorldOnline.Game
{
    public sealed class GameMasterCommandsProcessor : IDisposable
    {
        private const string GameServerAddress = "GameServer:Address";
        private const string GamerServerPublic = "GameServer:PublicAddress";
        private const string GameServerPort = "GameServer:Port";

        private readonly PartyManager _partyManager;
        private readonly StatusManager _statusManager;
        private readonly ExpManager _expManager;
        private readonly AssetsLoader _assets;
        private readonly MapServer _mapServer;
        private readonly DungeonsServer _dungeonServer;
        private readonly PvpServer _pvpServer;
        private readonly MapRegistry _registry;
        private readonly ILogger _logger;
        private readonly ISender _sender;
        private readonly IConfiguration _configuration;
        private readonly OwnerStorageFlushService _ownerStorageFlushService;

        public GameMasterCommandsProcessor(
            PartyManager partyManager,
            StatusManager statusManager,
            ExpManager expManager,
            AssetsLoader assets,
            MapServer mapServer,
            DungeonsServer dungeonsServer,
            PvpServer pvpServer,
            MapRegistry registry,
            ILogger logger,
            ISender sender,
            IConfiguration configuration,
            OwnerStorageFlushService ownerStorageFlushService)
        {
            _partyManager = partyManager;
            _expManager = expManager;
            _statusManager = statusManager;
            _assets = assets;
            _mapServer = mapServer;
            _dungeonServer = dungeonsServer;
            _pvpServer = pvpServer;
            _registry = registry;
            _logger = logger;
            _sender = sender;
            _configuration = configuration;
            _ownerStorageFlushService = ownerStorageFlushService;
        }

        public async Task ExecuteCommand(GameClient client, string message)
        {
            var command = Regex.Replace(message.Trim().ToLower(), @"\s+", " ").Split(' ');

            if (command[0] == "summon" && command.Length > 2)
            {
               
                // Mantém o segundo elemento em sua forma original
                command[2] = message.Split(' ')[2];
            }
            _logger.Information($"Account {client.AccountId} {client.Tamer.Name} used !{message}.");

            switch (command[0])
            {
                //TODO: split by GM level
                //TODO: logs

                case "showfullmap":
                    {
                        if (client.Tamer.TempShowFullMap)
                        {
                            client.Tamer.TempShowFullMap = false;
                            client.Send(new SystemMessagePacket("Unloading full map..."));
                        }
                        else
                        {
                            client.Tamer.TempShowFullMap = true;
                            client.Send(new SystemMessagePacket("Loading full map..."));
                        }
                    }
                    break;

                case "trade":
                    {
                        client.Send(new TradeAcceptPacket(client.Tamer.GeneralHandler));
                        break;
                    }
                case "teste1":
                    {
                        client.Tamer.NewLocation(client.Tamer.Location.X + 200, client.Tamer.Location.Y + 300);
                        client.Send(new TamerWalkPacket(client.Tamer));
                    }
                    break;

                case "teste2c":
                    {
                        var returnQuest = client.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == 4021);
                        if (returnQuest != null)
                        {
                            var returnedItem = new ItemModel();
                            returnedItem.SetItemId(9072);

                            client.Tamer.Inventory.RemoveOrReduceItem(returnedItem, 1);
                            client.Tamer.Inventory.AddBits(100);
                            returnQuest.UpdateCondition(1, 1);

                            client.Send(new QuestGoalUpdatePacket(4021, 1, 1));
                            var questToUpdate = client.Tamer.Progress.InProgressQuestData.FirstOrDefault(x => x.QuestId == 4021);
                            _sender.Send(new UpdateCharacterInProgressCommand(questToUpdate));
                            await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                        }
                    }
                    break;

                case "teste3":
                    {
                        var duration = UtilitiesFunctions.RemainingTimeSeconds(30);

                        var packet = new PacketWriter();
                        packet.Type(4013);
                        packet.WriteInt(client.Tamer.GeneralHandler);
                        packet.WriteInt(client.Partner.GeneralHandler);
                        packet.WriteInt(7500511);
                        packet.WriteShort(465);
                        packet.WriteInt(duration);

                        packet = new PacketWriter();
                        packet.Type(4013);
                        packet.WriteInt(client.Partner.GeneralHandler);
                        packet.WriteInt(7501411);
                        packet.WriteShort(472);
                        packet.WriteInt(duration);

                        client.Send(packet);
                    }
                    break;

                case "teste4":
                    {
                        var duration = UtilitiesFunctions.RemainingTimeSeconds(30);

                        var packet = new PacketWriter();
                        packet.Type(4001); //TamerBuff
                        packet.WriteUInt(client.Tamer.GeneralHandler);
                        packet.WriteUShort(40165);
                        packet.WriteInt(40165);

                        packet.WriteInt(465);

                        packet.WriteInt(duration);
                        packet.WriteUInt(client.Tamer.GeneralHandler);
                        packet.WriteInt(7500511);

                        client.Send(packet);
                    }
                    break;

                case "teste5":
                    {
                        var duration = UtilitiesFunctions.RemainingTimeSeconds(30);

                        var packet = new PacketWriter();
                        packet.Type(4000);//Digimon
                        packet.WriteUInt(client.Partner.GeneralHandler);
                        packet.WriteUShort(40165);
                        packet.WriteShort(465);
                        packet.WriteInt(duration);
                        packet.WriteInt(7500511);

                        client.Send(packet);
                    }
                    break;

                case "teste7":
                    {
                        var packet = new PacketWriter();
                        packet.Type(4126);
                        packet.WriteInt(0);
                        packet.WriteInt(92137);
                        packet.WriteInt(0);
                        client.Send(packet);
                    }
                    break;

                case "joinevent":
                    {
                        //if (_eventQueueManager.JoinQueue(client.Tamer))
                        //{
                        //    client.Send(new SystemMessagePacket("Joinned event queue. You will be automatic transfered to the event arena soon."));
                        //}
                        //else
                        //{
                        client.Send(new SystemMessagePacket("Unable to join the event queue, try again later."));
                        //}
                    }
                    break;

                case "quitevent":
                    {
                        //if (_eventQueueManager.LeaveQueue(client.Tamer))
                        //    client.Send(new SystemMessagePacket("You have left the event queue."));
                    }
                    break;

                case "testarena":
                    {
                        var writer = new PacketWriter();
                        writer.Type(30009);
                        writer.WriteInt(UtilitiesFunctions.RemainingTimeSeconds(10));
                        writer.WriteInt(UtilitiesFunctions.RemainingTimeSeconds(90));
                        writer.WriteInt(1);

                        writer.WriteInt64(1); //tickets
                        writer.WriteByte(1); //team
                        writer.WriteInt(1); //rodadas?

                        client.Send(writer);
                    }
                    break;

                case "maintenance":
                    {
                        if (client.AccessLevel == AccountAccessLevelEnum.Administrator)
                        {
                            var packet = new PacketWriter();
                            packet.Type(1006);
                            packet.WriteByte(10);
                            packet.WriteByte(1);
                            packet.WriteString("Server shutdown for maintenance in 2 minutes");
                            packet.WriteByte(0);

                            _mapServer.BroadcastGlobal(packet.Serialize());

                            var server = await _sender.Send(new GetServerByIdQuery(client.ServerId));
                            if (server.Register != null)
                                await _sender.Send(new UpdateServerCommand(server.Register.Id, server.Register.Name, server.Register.Experience, true));

                            Task.Run(() =>
                            {
                                Thread.Sleep(60000);
                                var packet = new PacketWriter();
                                packet.Type(1006);
                                packet.WriteByte(10);
                                packet.WriteByte(1);
                                packet.WriteString("Server shutdown for maintenance in 60s");
                                packet.WriteByte(0);
                                _mapServer.BroadcastGlobal(packet.Serialize());
                                _dungeonServer.BroadcastGlobal(packet.Serialize());

                                Thread.Sleep(30000);
                                packet = new PacketWriter();
                                packet.Type(1006);
                                packet.WriteByte(10);
                                packet.WriteByte(1);
                                packet.WriteString("Server shutdown for maintenance in 30s");
                                packet.WriteByte(0);
                                _mapServer.BroadcastGlobal(packet.Serialize());
                                _dungeonServer.BroadcastGlobal(packet.Serialize());

                                Thread.Sleep(20000);
                                packet = new PacketWriter();
                                packet.Type(1006);
                                packet.WriteByte(10);
                                packet.WriteByte(1);
                                packet.WriteString("Server shutdown for maintenance in 10s");
                                packet.WriteByte(0);
                                _mapServer.BroadcastGlobal(packet.Serialize());
                                _dungeonServer.BroadcastGlobal(packet.Serialize());

                                Thread.Sleep(5000);
                                for (int i = 5; i >= 0; i--)
                                {
                                    Thread.Sleep(1300);
                                    packet = new PacketWriter();
                                    packet.Type(1006);
                                    packet.WriteByte(10);
                                    packet.WriteByte(1);
                                    packet.WriteString($"Server shutdown for maintenance in {i}s");
                                    packet.WriteByte(0);

                                    _mapServer.BroadcastGlobal(packet.Serialize());
                                    _dungeonServer.BroadcastGlobal(packet.Serialize());
                                }

                                _mapServer.BroadcastGlobal(new DisconnectUserPacket("Server maintenance").Serialize());
                                _dungeonServer.BroadcastGlobal(new DisconnectUserPacket("Server maintenance").Serialize());
                            });
                        }
                        else
                        {
                            client.Send(new SystemMessagePacket("Insufficient permission level for this action."));
                        }
                    }
                    break;

                case "notice":
                    {
                        var notice = string.Join(" ", message.Split(' ').Skip(1));
                        var packet = new PacketWriter();
                        packet.Type(1006);
                        packet.WriteByte(10);
                        packet.WriteByte(1);
                        packet.WriteString($"{notice}");
                        packet.WriteByte(0);

                        _mapServer.BroadcastGlobal(packet.Serialize());
                    }
                    break;

                case "channels":
                    {
                        // Phase E Step 7 — live channel snapshot for the
                        // requesting GM's current map (default-map type only).
                        var mapId = client.Tamer.Location.MapId;
                        var summary = new System.Text.StringBuilder();
                        summary.Append($"Map {mapId} channels: ");
                        bool any = false;
                        foreach (var c in _registry.GetChannelsOf(
                            Commons.Enums.MapTypeEnum.Default, mapId))
                        {
                            if (any) summary.Append(", ");
                            summary.Append($"ch{c.Channel}={c.Clients.Count}");
                            if (c.CloseMap) summary.Append("(closing)");
                            any = true;
                        }
                        if (!any) summary.Append("(none registered for default type)");
                        client.Send(new SystemMessagePacket(summary.ToString()));
                    }
                    break;

                case "pos":
                case "position":
                case "location":
                case "where":
                    {
                        var regex = @"(where$){1}|(location$){1}|(position$){1}|(pos$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        var loc = client.Tamer.Location;
                        client.Send(new SystemMessagePacket($"Map: {loc.MapId} X: {loc.X} Y: {loc.Y}"));
                    }
                    break;

                case "tamer":
                    {
                        if (command.Length == 1)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        switch (command[1])
                        {
                            case "size":
                                {
                                    var regex = @"(tamer\ssize\s\d){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                                        break;
                                    }

                                    if (short.TryParse(command[2], out var value))
                                    {
                                        client.Tamer.SetSize(value);

                                        if (client.DungeonMap)
                                        {
                                            _dungeonServer.BroadcastForTamerViewsAndSelf(client.TamerId, new UpdateSizePacket(client.Tamer.GeneralHandler, client.Tamer.Size).Serialize());
                                        }
                                        else
                                        {
                                            _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new UpdateSizePacket(client.Tamer.GeneralHandler, client.Tamer.Size).Serialize());

                                        }
                                        await _sender.Send(new UpdateCharacterSizeCommand(client.TamerId, value));
                                    }
                                    else
                                    {
                                        client.Send(new SystemMessagePacket($"Invalid value. Max possible amount is {short.MaxValue}."));
                                    }
                                }
                                break;

                            case "exp":
                                {
                                    //TODO: refazer
                                    var regex = @"(tamer\sexp\sadd\s\d){1}|(tamer\sexp\sremove\s\d){1}|(tamer\sexp\smax){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket("Correct usage is \"!tamer exp add value\" or " +
                                            "\"!tamer exp remove value\"" +
                                            "\"!tamer exp max\".")
                                            .Serialize());

                                        break;
                                    }

                                    switch (command[2])
                                    {
                                        case "max":
                                            {
                                                _logger.Information(
                                                    "GM tamer exp max begin tamer={TamerId}:{TamerName} level={Level} partner={PartnerId}:{PartnerName} partnerLevel={PartnerLevel} partnerSlot={PartnerSlot} partnerHandler={PartnerHandler}",
                                                    client.TamerId,
                                                    client.Tamer.Name,
                                                    client.Tamer.Level,
                                                    client.Partner.Id,
                                                    client.Partner.Name,
                                                    client.Partner.Level,
                                                    client.Partner.Slot,
                                                    client.Partner.GeneralHandler);

                                                if (client.Tamer.Level >= 120)
                                                {
                                                    client.Send(new SystemMessagePacket($"Tamer already at max level."));
                                                    _logger.Information("GM tamer exp max ignored tamer={TamerId} reason=max-level level={Level}", client.TamerId, client.Tamer.Level);
                                                    break;
                                                }

                                                var result = _expManager.ReceiveMaxTamerExperience(client.Tamer);

                                                if (result.Success)
                                                {
                                                    client.Send(
                                                        new ReceiveExpPacket(
                                                            0,
                                                            0,
                                                            client.Tamer.CurrentExperience,
                                                            client.Tamer.Partner.GeneralHandler,
                                                            0,
                                                            0,
                                                            client.Tamer.Partner.CurrentExperience,
                                                            0
                                                        )
                                                    );
                                                }
                                                else
                                                {
                                                    client.Send(new SystemMessagePacket($"No proper configuration for tamer {client.Tamer.Model} leveling."));
                                                    return;
                                                }

                                                if (result.LevelGain > 0)
                                                {
                                                    client.Tamer.SetLevelStatus(
                                                        _statusManager.GetTamerLevelStatus(
                                                            client.Tamer.Model,
                                                            client.Tamer.Level
                                                        )
                                                    );

                                                    
                                                    _mapServer.BroadcastForTamerViewsAndSelf(
                                                    client.TamerId,
                                                    new LevelUpPacket(
                                                        client.Tamer.GeneralHandler,
                                                        client.Tamer.Level)
                                                    .Serialize());

                                                    client.Tamer.FullHeal();

                                                    client.Send(new UpdateStatusPacket(client.Tamer));
                                                }

                                                if (result.Success)
                                                    await _sender.Send(new UpdateCharacterExperienceCommand(client.TamerId, client.Tamer.CurrentExperience, client.Tamer.Level));
                                            }
                                            break;

                                        case "add":
                                            {
                                                _logger.Information(
                                                    "GM tamer exp add begin tamer={TamerId}:{TamerName} level={Level} value={ValueText} partner={PartnerId}:{PartnerName} partnerLevel={PartnerLevel} partnerSlot={PartnerSlot} partnerHandler={PartnerHandler}",
                                                    client.TamerId,
                                                    client.Tamer.Name,
                                                    client.Tamer.Level,
                                                    command.Length > 3 ? command[3] : "",
                                                    client.Partner.Id,
                                                    client.Partner.Name,
                                                    client.Partner.Level,
                                                    client.Partner.Slot,
                                                    client.Partner.GeneralHandler);

                                                if (client.Tamer.Level >= 120)
                                                {
                                                    client.Send(new SystemMessagePacket($"Tamer already at max level."));
                                                    _logger.Information("GM tamer exp add ignored tamer={TamerId} reason=max-level level={Level}", client.TamerId, client.Tamer.Level);
                                                    break;
                                                }

                                                var value = Convert.ToInt64(command[3]);

                                                var result = _expManager.ReceiveTamerExperience(value, client.Tamer);

                                                if (result.Success)
                                                {
                                                    client.Send(
                                                        new ReceiveExpPacket(
                                                            value,
                                                            0,
                                                            client.Tamer.CurrentExperience,
                                                            client.Tamer.Partner.GeneralHandler,
                                                            0,
                                                            0,
                                                            client.Tamer.Partner.CurrentExperience,
                                                            0
                                                        )
                                                    );
                                                }
                                                else
                                                {
                                                    client.Send(new SystemMessagePacket($"No proper configuration for tamer {client.Tamer.Model} leveling."));
                                                    return;
                                                }

                                                if (result.LevelGain > 0)
                                                {
                                                    client.Tamer.SetLevelStatus(
                                                        _statusManager.GetTamerLevelStatus(
                                                            client.Tamer.Model,
                                                            client.Tamer.Level
                                                        )
                                                    );

                                                    _mapServer.BroadcastForTamerViewsAndSelf(
                                                    client.TamerId,
                                                    new LevelUpPacket(
                                                        client.Tamer.GeneralHandler,
                                                        client.Tamer.Level).Serialize());

                                                    client.Tamer.FullHeal();

                                                    client.Send(new UpdateStatusPacket(client.Tamer));
                                                }

                                                if (result.Success)
                                                    await _sender.Send(new UpdateCharacterExperienceCommand(client.TamerId, client.Tamer.CurrentExperience, client.Tamer.Level));
                                            }
                                            break;

                                        case "remove":
                                            {
                                                var value = Convert.ToInt64(command[3]);

                                                var tamerInfos = _assets.TamerLevelInfo
                                                    .Where(x => x.Type == client.Tamer.Model)
                                                    .ToList();

                                                if (tamerInfos == null || !tamerInfos.Any() || tamerInfos.Count != 120)
                                                {
                                                    _logger.Warning($"Incomplete level config for tamer {client.Tamer.Model}.");

                                                    client.Send(new SystemMessagePacket
                                                        ($"No proper configuration for tamer {client.Tamer.Model} leveling."));
                                                    break;
                                                }

                                                //TODO: ajeitar
                                                client.Tamer.LooseExp(value);

                                                client.Send(new ReceiveExpPacket(
                                                    value * -1,
                                                    0,
                                                    client.Tamer.CurrentExperience,
                                                    client.Tamer.Partner.GeneralHandler,
                                                    0,
                                                    0,
                                                    client.Tamer.Partner.CurrentExperience,
                                                    0
                                                ));

                                                await _sender.Send(new UpdateCharacterExperienceCommand(client.TamerId, client.Tamer.CurrentExperience, client.Tamer.Level));
                                            }
                                            break;


                                        default:
                                            {
                                                client.Send(new SystemMessagePacket("Correct usage is \"!tamer exp add {value}\" or " +
                                                "\"!tamer exp max\"."));
                                            }
                                            break;
                                    }
                                }
                                break;

                            case "summon":
                                {

                                    var TargetSummon = _mapServer.FindClientByTamerName(command[2]);

                                    if (TargetSummon == null)
                                        return;

                                    TargetSummon.Send(new UpdateSizePacket(client.Tamer.Name, client.Tamer.Location.MapId));

                                }
                                break;
                            default:
                                {
                                    client.Send(new SystemMessagePacket("Under development."));
                                }
                                break;
                        }
                    }
                    break;

                case "digimon":
                    {
                        if (command.Length == 1)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        switch (command[1])
                        {
                            case "transcend":
                                {
                                    var regex = @"(digimon\stranscend){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                                        break;
                                    }

                                    client.Partner.Transcend();
                                    client.Partner.SetSize(14000);

                                    client.Partner.SetBaseStatus(
                                        _statusManager.GetDigimonBaseStatus(
                                            client.Partner.CurrentType,
                                            client.Partner.Level,
                                            client.Partner.Size
                                        )
                                    );

                                    await _sender.Send(new UpdateDigimonSizeCommand(client.Partner.Id, client.Partner.Size));
                                    await _sender.Send(new UpdateDigimonGradeCommand(client.Partner.Id, client.Partner.HatchGrade));
                                }
                                break;

                            case "size":
                                {
                                    var regex = @"(digimon\ssize\s\d){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                                        break;
                                    }

                                    if (short.TryParse(command[2], out var value))
                                    {
                                        client.Partner.SetSize(value);
                                        client.Partner.SetBaseStatus(
                                            _statusManager.GetDigimonBaseStatus(
                                                client.Partner.CurrentType,
                                                client.Partner.Level,
                                                client.Partner.Size
                                            )
                                        );

                                        _mapServer.BroadcastForTamerViewsAndSelf(client.TamerId, new UpdateSizePacket(client.Partner.GeneralHandler, client.Partner.Size).Serialize());
                                        client.Send(new UpdateStatusPacket(client.Tamer));
                                        await _sender.Send(new UpdateDigimonSizeCommand(client.Partner.Id, value));
                                    }
                                    else
                                    {
                                        client.Send(new SystemMessagePacket($"Invalid value. Max possible amount is {short.MaxValue}."));
                                    }
                                }
                                break;

                            case "exp":
                                {
                                    var regex = @"(digimon\sexp\sadd\s\d){1}|(digimon\sexp\sremove\s\d){1}|(digimon\sexp\smax){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket("Correct usage is \"!digimon exp add value\" or " +
                                            "\"!digimon exp remove value\" or " +
                                            "\"!digimon exp max\".")
                                            .Serialize());

                                        break;
                                    }

                                    switch (command[2])
                                    {
                                        case "max":
                                            {
                                                _logger.Information(
                                                    "GM digimon exp max begin tamer={TamerId}:{TamerName} partner={PartnerId}:{PartnerName} base={BaseType} current={CurrentType} level={Level} slot={Slot} handler={Handler}",
                                                    client.TamerId,
                                                    client.Tamer.Name,
                                                    client.Partner.Id,
                                                    client.Partner.Name,
                                                    client.Partner.BaseType,
                                                    client.Partner.CurrentType,
                                                    client.Partner.Level,
                                                    client.Partner.Slot,
                                                    client.Partner.GeneralHandler);

                                                if (client.Partner.Level == 120)
                                                {
                                                    client.Send(new SystemMessagePacket($"Partner already at max level."));
                                                    _logger.Information("GM digimon exp max ignored tamer={TamerId} partner={PartnerId}:{PartnerName} reason=max-level level={Level}", client.TamerId, client.Partner.Id, client.Partner.Name, client.Partner.Level);
                                                    break;
                                                }

                                                var result = _expManager.ReceiveMaxDigimonExperience(client.Partner);

                                                if (result.Success)
                                                {
                                                    client.Send(
                                                        new ReceiveExpPacket(
                                                            0,
                                                            0,
                                                            client.Tamer.CurrentExperience,
                                                            client.Tamer.Partner.GeneralHandler,
                                                            0,
                                                            0,
                                                            client.Tamer.Partner.CurrentExperience,
                                                            0
                                                        )
                                                    );
                                                }
                                                else
                                                {
                                                    client.Send(new SystemMessagePacket($"No proper configuration for digimon {client.Partner.Model} leveling."));
                                                    return;
                                                }

                                                if (result.LevelGain > 0)
                                                {
                                                    client.Partner.SetBaseStatus(
                                                        _statusManager.GetDigimonBaseStatus(
                                                            client.Partner.CurrentType,
                                                            client.Partner.Level,
                                                            client.Partner.Size
                                                        )
                                                    );

                                                    _mapServer.BroadcastForTamerViewsAndSelf(
                                                        client.TamerId,
                                                        new LevelUpPacket(
                                                            client.Tamer.Partner.GeneralHandler,
                                                            client.Tamer.Partner.Level
                                                        ).Serialize()
                                                    );

                                                    client.Partner.FullHeal();

                                                    client.Send(new UpdateStatusPacket(client.Tamer));
                                                }

                                                if (result.Success)
                                                    await _sender.Send(new UpdateDigimonExperienceCommand(client.Partner));
                                            }
                                            break;

                                        case "add":
                                            {
                                                _logger.Information(
                                                    "GM digimon exp add begin tamer={TamerId}:{TamerName} partner={PartnerId}:{PartnerName} base={BaseType} current={CurrentType} level={Level} slot={Slot} handler={Handler} value={ValueText}",
                                                    client.TamerId,
                                                    client.Tamer.Name,
                                                    client.Partner.Id,
                                                    client.Partner.Name,
                                                    client.Partner.BaseType,
                                                    client.Partner.CurrentType,
                                                    client.Partner.Level,
                                                    client.Partner.Slot,
                                                    client.Partner.GeneralHandler,
                                                    command.Length > 3 ? command[3] : "");

                                                if (client.Partner.Level == 120)
                                                {
                                                    client.Send(new SystemMessagePacket($"Partner already at max level."));
                                                    _logger.Information("GM digimon exp add ignored tamer={TamerId} partner={PartnerId}:{PartnerName} reason=max-level level={Level}", client.TamerId, client.Partner.Id, client.Partner.Name, client.Partner.Level);
                                                    break;
                                                }

                                                var value = Convert.ToInt64(command[3]);

                                                var result = _expManager.ReceiveDigimonExperience(value, client.Partner);

                                                if (result.Success)
                                                {
                                                    client.Send(
                                                        new ReceiveExpPacket(
                                                            0,
                                                            0,
                                                            client.Tamer.CurrentExperience,
                                                            client.Tamer.Partner.GeneralHandler,
                                                            value,
                                                            0,
                                                            client.Tamer.Partner.CurrentExperience,
                                                            0
                                                        )
                                                    );
                                                }
                                                else
                                                {
                                                    client.Send(new SystemMessagePacket($"No proper configuration for digimon {client.Partner.Model} leveling."));
                                                    return;
                                                }

                                                if (result.LevelGain > 0)
                                                {
                                                    client.Partner.SetBaseStatus(
                                                        _statusManager.GetDigimonBaseStatus(
                                                            client.Partner.CurrentType,
                                                            client.Partner.Level,
                                                            client.Partner.Size
                                                        )
                                                    );

                                                    _mapServer.BroadcastForTamerViewsAndSelf(
                                                        client.TamerId,
                                                        new LevelUpPacket(
                                                            client.Tamer.Partner.GeneralHandler,
                                                            client.Tamer.Partner.Level
                                                        ).Serialize()
                                                    );

                                                    client.Partner.FullHeal();

                                                    client.Send(new UpdateStatusPacket(client.Tamer));
                                                }

                                                if (result.Success)
                                                    await _sender.Send(new UpdateDigimonExperienceCommand(client.Partner));
                                            }
                                            break;

                                        case "remove":
                                            {
                                                var value = Convert.ToInt64(command[3]);

                                                var digimonInfos = _assets.DigimonLevelInfo
                                                    .Where(x => x.Type == client.Tamer.Partner.BaseType)
                                                    .ToList();

                                                if (digimonInfos == null || !digimonInfos.Any() || digimonInfos.Count != 120)
                                                {
                                                    _logger.Warning($"Incomplete level config for digimon {client.Tamer.Partner.BaseType}.");

                                                    client.Send(new SystemMessagePacket
                                                        ($"No proper configuration for digimon {client.Tamer.Partner.BaseType} leveling."));
                                                    break;
                                                }

                                                //TODO: ajeitar
                                                var partnerInitialLevel = client.Partner.Level;

                                                client.Tamer.LooseExp(value);

                                                client.Send(new ReceiveExpPacket(
                                                    0,
                                                    0,
                                                    client.Tamer.CurrentExperience,
                                                    client.Tamer.Partner.GeneralHandler,
                                                    value * -1,
                                                    0,
                                                    client.Tamer.Partner.CurrentExperience,
                                                    0
                                                ));

                                                if (partnerInitialLevel != client.Partner.Level)
                                                    client.Send(new LevelUpPacket(client.Partner.GeneralHandler, client.Partner.Level));

                                                await _sender.Send(new UpdateDigimonExperienceCommand(client.Partner));
                                            }
                                            break;

                                        default:
                                            {
                                                client.Send(new SystemMessagePacket("Correct usage is \"!digimon exp add value\" or " +
                                                "\"!digimon exp max\"."));
                                            }
                                            break;
                                    }
                                }
                                break;

                            case "clone":
                                {
                                    var regex = @"(digimon\sclone\sfull$){1}";
                                    var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                                    if (!match.Success)
                                    {
                                        client.Send(new SystemMessagePacket("Correct usage is \"!digimon clone full\"."));
                                        break;
                                    }

                                    var cloneTypes = new[]
                                    {
                                        DigicloneTypeEnum.AT,
                                        DigicloneTypeEnum.BL,
                                        DigicloneTypeEnum.CT,
                                        DigicloneTypeEnum.EV,
                                        DigicloneTypeEnum.HP
                                    };

                                    foreach (var cloneType in cloneTypes)
                                    {
                                        client.Partner.Digiclone.ResetAll(cloneType);

                                        for (byte level = 1; level <= 15; level++)
                                        {
                                            var cloneValue = _assets.CloneValues.FirstOrDefault(x =>
                                                x.Type == cloneType &&
                                                level >= x.MinLevel &&
                                                level <= x.MaxLevel);

                                            if (cloneValue == null)
                                            {
                                                client.Send(new SystemMessagePacket($"Missing clone value asset for {cloneType} level {level}."));
                                                break;
                                            }

                                            client.Partner.Digiclone.IncreaseCloneLevel(cloneType, (short)cloneValue.MaxValue);
                                        }
                                    }

                                    client.Partner.SetBaseStatus(
                                        _statusManager.GetDigimonBaseStatus(
                                            client.Partner.CurrentType,
                                            client.Partner.Level,
                                            client.Partner.Size
                                        )
                                    );

                                    client.Send(new UpdateStatusPacket(client.Tamer));
                                    await _sender.Send(new UpdateDigicloneCommand(client.Partner.Digiclone));
                                    client.Send(new SystemMessagePacket("Active Digimon full clone applied."));
                                }
                                break;

                            default:
                                {
                                    client.Send(new SystemMessagePacket("Unknown command. Check the available commands at the admin portal."));
                                }
                                break;
                        }
                    }
                    break;

                case "teleport":
                    {
                        var regex = @"(teleport\s\d\s\d){1}|(teleport\s\d){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        var mapId = Convert.ToInt32(command[1]);
                        var waypoint = command.Length == 3 ? Convert.ToInt32(command[2]) : 0;

                        var waypoints = await _sender.Send(new MapRegionListAssetsByMapIdQuery(mapId));
                        if (waypoints == null || !waypoints.Regions.Any())
                        {
                            client.Send(new SystemMessagePacket($"Map information not found for ID {mapId}"));
                            break;
                        }

                        await _ownerStorageFlushService.FlushForTransitionAsync(client);
                        if (client.PvpMap)
                            _pvpServer.RemoveClient(client);
                        else if (client.DungeonMap)
                            _dungeonServer.RemoveClient(client);
                        else
                            _mapServer.RemoveClient(client);

                        var destination = waypoints.Regions.First();

                        if (waypoint > waypoints.Regions.Count)
                            waypoint = waypoints.Regions.Count;

                        if (waypoint > 0)
                            destination = waypoints.Regions[waypoint - 1] ?? destination;

                        client.Tamer.NewLocation(mapId, destination.X, destination.Y);
                        await _sender.Send(new UpdateCharacterLocationCommand(client.Tamer.Location));

                        client.Tamer.Partner.NewLocation(mapId, destination.X, destination.Y);
                        await _sender.Send(new UpdateDigimonLocationCommand(client.Tamer.Partner.Location));

                        client.Tamer.UpdateState(CharacterStateEnum.Loading);
                        await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

                        client.SetGameQuit(false);

                        client.Send(new MapSwapPacket(
                            _configuration[GamerServerPublic],
                            _configuration[GameServerPort],
                            client.Tamer.Location.MapId,
                            client.Tamer.Location.X,
                            client.Tamer.Location.Y)
                            .Serialize());

                        var party = _partyManager.FindParty(client.TamerId);
                        if (party != null)
                        {
                            party.UpdateMember(party[client.TamerId]);

                            _mapServer.BroadcastForTargetTamers(party.GetMembersIdList(),
                                new PartyMemberWarpGatePacket(party[client.TamerId]).Serialize());
                        }
                    }
                    break;

                case "currency":
                    {
                        var regex = @"(currency\sbits\s\d){1}|(currency\spremium\s\d){1}|(currency\ssilk\s\d){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        switch (command[1])
                        {
                            case "bits":
                                {
                                    var value = long.Parse(command[2]);
                                    client.Tamer.Inventory.AddBits(value);

                                    client.Send(new LoadInventoryPacket(client.Tamer.Inventory,
                                        InventoryTypeEnum.Inventory)
                                        .Serialize());

                                    await _sender.Send(new UpdateItemListBitsCommand(client.Tamer.Inventory.Id, client.Tamer.Inventory.Bits));
                                }
                                break;

                                //case "premium":
                                //    {
                                //        var value = int.Parse(command[2]);
                                //        client.AddPremium(value);
                                //
                                //        await _sender.Send(new UpdatePremiumAndSilkCommand(client.Premium,
                                //            client.Silk, client.AccountId));
                                //    }
                                //    break;
                                //
                                //case "silk":
                                //    {
                                //        var value = int.Parse(command[2]);
                                //        client.AddSilk(value);
                                //
                                //        await _sender.Send(new UpdatePremiumAndSilkCommand(client.Premium,
                                //            client.Silk, client.AccountId));
                                //    }
                                //    break;
                        }
                    }
                    break;

                case "reload":
                    {
                        var regex = @"(reload$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        _logger.Debug($"Updating tamer state...");
                        client.Tamer.UpdateState(CharacterStateEnum.Loading);
                        await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

                        await _ownerStorageFlushService.FlushForTransitionAsync(client);
                        _mapServer.RemoveClient(client);

                        client.SetGameQuit(false);

                        client.Send(new MapSwapPacket(
                            _configuration[GamerServerPublic],
                            _configuration[GameServerPort],
                            client.Tamer.Location.MapId,
                            client.Tamer.Location.X,
                            client.Tamer.Location.Y));
                    }
                    break;

                case "item":
                    {
                        var regex = @"(item\s\d{1,7}\s\d{1,4}$){1}|(item\s\d{1,7}$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        var itemId = int.Parse(command[1]);

                        var newItem = new ItemModel();
                        newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemId));

                        if (newItem.ItemInfo == null)
                        {
                            _logger.Warning($"No item info found with ID {itemId} for tamer {client.TamerId}.");
                            client.Send(new SystemMessagePacket($"No item info found with ID {itemId}."));
                            break;
                        }

                        newItem.ItemId = itemId;
                        newItem.Amount = command.Length == 2 ? 1 : int.Parse(command[2]);

                        if (newItem.IsTemporary)
                            newItem.SetRemainingTime((uint)newItem.ItemInfo.UsageTimeMinutes);

                        if (client.Tamer.Inventory.AddItem(newItem))
                        {
                            client.Send(new ReceiveItemPacket(newItem, InventoryTypeEnum.Inventory));
                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                        }
                        else
                        {
                            client.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                        }
                    }
                    break;

                case "giveitem":
                    {
                        var regex = @"(giveitem\s\S+\s\d{1,7}\s\d{1,4}$){1}|(giveitem\s\S+\s\d{1,7}$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Correct usage is \"!giveitem {{player}} {{itemid}} [amount]\"."));
                            break;
                        }

                        var targetName = command[1];
                        var itemId = int.Parse(command[2]);
                        var amount = command.Length == 3 ? 1 : int.Parse(command[3]);

                        var targetClient =
                            _mapServer.FindClientByTamerName(targetName) ??
                            _dungeonServer.FindClientByTamerName(targetName) ??
                            _pvpServer.FindClientByTamerName(targetName);

                        if (targetClient == null)
                        {
                            _logger.Warning($"GM giveitem failed. Player {targetName} is not online. gm={client.TamerId} item={itemId} amount={amount}");
                            client.Send(new SystemMessagePacket($"Player {targetName} is not online."));
                            break;
                        }

                        var newItem = new ItemModel();
                        newItem.SetItemInfo(_assets.ItemInfo.FirstOrDefault(x => x.ItemId == itemId));

                        if (newItem.ItemInfo == null)
                        {
                            _logger.Warning($"GM giveitem failed. No item info found with ID {itemId}. gm={client.TamerId} target={targetClient.TamerId}");
                            client.Send(new SystemMessagePacket($"No item info found with ID {itemId}."));
                            break;
                        }

                        newItem.ItemId = itemId;
                        newItem.Amount = amount;

                        if (newItem.IsTemporary)
                            newItem.SetRemainingTime((uint)newItem.ItemInfo.UsageTimeMinutes);

                        if (targetClient.Tamer.Inventory.AddItem(newItem))
                        {
                            targetClient.Send(new ReceiveItemPacket(newItem, InventoryTypeEnum.Inventory));
                            await _sender.Send(new UpdateItemsCommand(targetClient.Tamer.Inventory));

                            client.Send(new SystemMessagePacket($"Sent item {itemId} x{amount} to {targetClient.Tamer.Name}."));
                            targetClient.Send(new SystemMessagePacket($"GM sent item {itemId} x{amount}."));
                            _logger.Information($"GM giveitem success. gm={client.TamerId} target={targetClient.TamerId} item={itemId} amount={amount}");
                        }
                        else
                        {
                            client.Send(new SystemMessagePacket($"{targetClient.Tamer.Name}'s inventory is full."));
                            targetClient.Send(new PickItemFailPacket(PickItemFailReasonEnum.InventoryFull));
                            _logger.Information($"GM giveitem failed. Inventory full. gm={client.TamerId} target={targetClient.TamerId} item={itemId} amount={amount}");
                        }
                    }
                    break;

                case "summonmonster":
                    {
                        if (command.Length < 2)
                        {
                            client.Send(new SystemMessagePacket("Correct usage is \"!summonmonster {monsterid} [amount] [easy|normal|medium|hard]\" or \"!summonmonster {monsterid} [amount] custom Level HP DS AT DE HT EV CT AS\"."));
                            break;
                        }

                        var monsterId = int.Parse(command[1]);
                        var amount = command.Length == 2 ? 1 : int.Parse(command[2]);
                        var preset = command.Length >= 4 ? command[3].ToLowerInvariant() : "normal";
                        int[]? customStats = null;
                        var bossMonster = false;

                        if (amount <= 0)
                            amount = 1;
                        if (amount > 50)
                            amount = 50;

                        if (preset == "custom")
                        {
                            if (command.Length < 13)
                            {
                                client.Send(new SystemMessagePacket("Custom usage: !summonmonster {monsterid} {amount} custom Level HP DS AT DE HT EV CT AS"));
                                break;
                            }

                            customStats = new int[9];
                            var validCustom = true;
                            for (var i = 0; i < customStats.Length; i++)
                            {
                                if (!long.TryParse(command[4 + i], out var rawValue) || rawValue <= 0)
                                {
                                    validCustom = false;
                                    break;
                                }

                                customStats[i] = ClampSummonCustomStat(rawValue, i);
                            }

                            if (!validCustom)
                            {
                                client.Send(new SystemMessagePacket("Custom stats must be positive numbers: Level HP DS AT DE HT EV CT AS."));
                                break;
                            }

                            bossMonster = command.Length >= 14 && command[13].Equals("boss", StringComparison.OrdinalIgnoreCase);
                        }
                        else if (preset is not ("easy" or "normal" or "medium" or "hard"))
                        {
                            client.Send(new SystemMessagePacket("Invalid preset. Use easy, normal, medium, hard, or custom."));
                            break;
                        }
                        else
                        {
                            bossMonster = command.Length >= 5 && command[4].Equals("boss", StringComparison.OrdinalIgnoreCase);
                        }

                        SummonMonsterByGm(client, monsterId, amount, preset, customStats, bossMonster);
                    }
                    break;

                case "stattest":
                    {
                        var regex = @"(stattest\sapply$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        var candidateLists = new[]
                        {
                            client.Tamer.Equipment,
                            client.Tamer.Digivice,
                            client.Tamer.ChipSets
                        };

                        var updatedItems = 0;
                        var updatedStatuses = 0;

                        foreach (var list in candidateLists)
                        {
                            foreach (var item in list.Items.Where(x => x != null && x.ItemId > 0))
                            {
                                item.SetItemInfo(item.ItemInfo ?? _assets.ItemInfo.FirstOrDefault(x => x.ItemId == item.ItemId));
                                var accessoryAsset = _assets.AccessoryRoll.FirstOrDefault(x => x.ItemId == item.ItemInfo?.Type)
                                    ?? _assets.AccessoryRoll.FirstOrDefault(x => x.ItemId == item.ItemId);

                                if (accessoryAsset == null)
                                    continue;

                                var touchedItem = false;

                                foreach (var status in item.AccessoryStatus.Where(x => x.Value > 0))
                                {
                                    var maxValue = accessoryAsset.Status
                                        .Where(x => x.Type == (int)status.Type)
                                        .Select(x => x.MaxValue)
                                        .DefaultIfEmpty(status.Value)
                                        .Max();

                                    if (status.Value != maxValue)
                                    {
                                        status.SetValue((short)maxValue);
                                        updatedStatuses++;
                                        touchedItem = true;
                                    }
                                }

                                if (!touchedItem)
                                    continue;

                                item.SetPower(102);
                                await _sender.Send(new UpdateItemAccessoryStatusCommand(item));
                                updatedItems++;
                            }
                        }

                        client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));
                        client.Send(new UpdateStatusPacket(client.Tamer));

                        client.Send(new SystemMessagePacket($"Stat test apply done. Items={updatedItems}, Statuses={updatedStatuses}."));
                        _logger.Information(
                            "GM !stattest apply: tamer={TamerId} account={AccountId} updatedItems={UpdatedItems} updatedStatuses={UpdatedStatuses}",
                            client.TamerId,
                            client.AccountId,
                            updatedItems,
                            updatedStatuses);
                    }
                    break;

                case "hide":
                    {
                        var regex = @"(hide$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        if (client.Tamer.Hidden)
                        {
                            client.Send(new SystemMessagePacket($"You are already in hide mode."));
                        }
                        else
                        {
                            client.Tamer.SetHidden(true);
                            client.Send(new SystemMessagePacket($"View state has been set to hide mode."));
                        }
                    }
                    break;

                case "show":
                    {
                        var regex = @"(show$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        if (client.Tamer.Hidden)
                        {
                            client.Tamer.SetHidden(false);
                            client.Send(new SystemMessagePacket($"View state has been set to show mode."));
                        }
                        else
                        {
                            client.Send(new SystemMessagePacket($"You are already in show mode."));
                        }
                    }
                    break;

                case "inv":
                case "inventory":
                    {
                        var regex = @"(inv\sslots\sadd\s\d{1,3}$){1}|(inventory\sslots\sadd\s\d{1,3}$){1}|(inventory\sslots\sclear$){1}|(inv\sslots\sclear$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        if (command[2] == "add")
                        {
                            if (byte.TryParse(command[3], out byte targetSize))
                            {
                                if (targetSize == byte.MinValue)
                                {
                                    client.Send(new SystemMessagePacket($"Invalid slots amount. Check your command on the Admin Portal."));
                                    break;
                                }

                                var newSize = client.Tamer.Inventory.AddSlots(targetSize);

                                client.Send(new SystemMessagePacket($"Inventory slots updated to {newSize}."));
                                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

                                var newSlots = client.Tamer.Inventory.Items.Where(x => x.ItemList == null).ToList();
                                await _sender.Send(new AddInventorySlotsCommand(newSlots));
                                newSlots.ForEach(newSlot => { newSlot.ItemList = client.Tamer.Inventory.Items.First(x => x.ItemList != null).ItemList; });
                                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            }
                            else
                            {
                                client.Send(new SystemMessagePacket($"Invalid command parameters. Check the available commands on the Admin Portal."));
                                break;
                            }
                        }
                        else if (command[2] == "clear")
                        {
                            client.Tamer.Inventory.Clear();
                            client.Send(new SystemMessagePacket($"Inventory slots cleaned."));
                            client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

                            await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                        }
                    }
                    break;
                case "giftstorage":
                    {
                        var regex = @"(inv\sslots\sadd\s\d{1,3}$){1}|(giftstorage\sslots\sadd\s\d{1,3}$){1}|(giftstorage\sslots\sclear$){1}|(gif\sslots\sclear$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        if (command[2] == "add")
                        {
                            if (byte.TryParse(command[3], out byte targetSize))
                            {
                                if (targetSize == byte.MinValue)
                                {
                                    client.Send(new SystemMessagePacket($"Invalid slots amount. Check your command on the Admin Portal."));
                                    break;
                                }

                                var newSize = client.Tamer.Inventory.AddSlots(targetSize);

                                client.Send(new SystemMessagePacket($"Inventory slots updated to {newSize}."));
                                client.Send(new LoadInventoryPacket(client.Tamer.Inventory, InventoryTypeEnum.Inventory));

                                var newSlots = client.Tamer.Inventory.Items.Where(x => x.ItemList == null).ToList();
                                await _sender.Send(new AddInventorySlotsCommand(newSlots));
                                newSlots.ForEach(newSlot => { newSlot.ItemList = client.Tamer.Inventory.Items.First(x => x.ItemList != null).ItemList; });
                                await _sender.Send(new UpdateItemsCommand(client.Tamer.Inventory));
                            }
                            else
                            {
                                client.Send(new SystemMessagePacket($"Invalid command parameters. Check the available commands on the Admin Portal."));
                                break;
                            }
                        }
                        else if (command[2] == "clear")
                        {
                            client.Tamer.GiftWarehouse.Clear();
                            client.Send(new SystemMessagePacket($" GiftStorage slots cleaned."));
                            client.Send(new LoadGiftStoragePacket(client.Tamer.GiftWarehouse));

                            await _sender.Send(new UpdateItemsCommand(client.Tamer.GiftWarehouse));
                        }
                    }
                    break;
                case "godmode":
                    {
                        var regex = @"(godmode\son$){1}|(godmode\soff$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        if (command[1] == "on")
                        {
                            if (client.Tamer.GodMode)
                            {
                                client.Send(new SystemMessagePacket($"You are already in god mode."));
                            }
                            else
                            {
                                client.Tamer.SetGodMode(true);
                                client.Send(new SystemMessagePacket($"God mode enabled."));
                            }
                        }
                        else
                        {
                            if (!client.Tamer.GodMode)
                            {
                                client.Send(new SystemMessagePacket($"You are already with god mode disabled."));
                            }
                            else
                            {
                                client.Tamer.SetGodMode(false);
                                client.Send(new SystemMessagePacket($"God mode disabled."));
                            }
                        }
                    }
                    break;

                case "unlockevos":
                    {
                        foreach (var evolution in client.Partner.Evolutions)
                        {
                            evolution.Unlock();
                            await _sender.Send(new UpdateEvolutionCommand(evolution));
                        }

                        client.Tamer.UpdateState(CharacterStateEnum.Loading);
                        await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

                        await _ownerStorageFlushService.FlushForTransitionAsync(client);
                        _mapServer.RemoveClient(client);

                        client.SetGameQuit(false);

                        client.Send(new MapSwapPacket(
                            _configuration[GamerServerPublic],
                            _configuration[GameServerPort],
                            client.Tamer.Location.MapId,
                            client.Tamer.Location.X,
                            client.Tamer.Location.Y));
                    }
                    break;

                case "openseals":
                    {
                        var sealInfoList = _assets.SealInfo;
                        var maxSealInfoList = sealInfoList
                            .GroupBy(x => x.SealId)
                            .Select(x => x
                                .OrderByDescending(seal => seal.RequiredAmount)
                                .ThenByDescending(seal => seal.SequentialId)
                                .First())
                            .ToList();

                        foreach (var seal in maxSealInfoList)
                        {
                            client.Tamer.SealList.AddOrSetSeal(seal.SealId, seal.RequiredAmount, seal.SequentialId);
                        }

                        client.Partner?.SetSealStatus(sealInfoList);

                        client.Send(new UpdateStatusPacket(client.Tamer));

                        await _sender.Send(new UpdateCharacterSealsCommand(client.Tamer.SealList));

                        client.Tamer.UpdateState(CharacterStateEnum.Loading);
                        await _sender.Send(new UpdateCharacterStateCommand(client.TamerId, CharacterStateEnum.Loading));

                        await _ownerStorageFlushService.FlushForTransitionAsync(client);
                        _mapServer.RemoveClient(client);

                        client.SetGameQuit(false);

                        client.Send(new MapSwapPacket(
                            _configuration[GamerServerPublic],
                            _configuration[GameServerPort],
                            client.Tamer.Location.MapId,
                            client.Tamer.Location.X,
                            client.Tamer.Location.Y));
                    }
                    break;

                case "admin":
                    {
                        var regex = @"(admin\skick\s\S+$){1}|(admin\sban\s\S+$){1}|(admin\sunban\s\S+$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket("Correct usage is \"!admin kick {tamer}\", \"!admin ban {tamer}\" or \"!admin unban {account|tamer}\"."));
                            break;
                        }

                        var action = command[1];
                        var targetName = command[2];

                        switch (action)
                        {
                            case "kick":
                                {
                                    var targetClient = FindOnlineClient(targetName);
                                    if (targetClient == null)
                                    {
                                        client.Send(new SystemMessagePacket($"Player {targetName} is not online."));
                                        break;
                                    }

                                    if (targetClient.AccessLevel >= client.AccessLevel)
                                    {
                                        client.Send(new SystemMessagePacket("Cannot kick a player with equal or higher access level."));
                                        break;
                                    }

                                    targetClient.Send(new DisconnectUserPacket("Disconnected by administrator."));
                                    targetClient.Disconnect(true);
                                    client.Send(new SystemMessagePacket($"Player {targetClient.Tamer.Name} disconnected."));
                                    _logger.Information("GM admin kick gm={GmTamerId}:{GmName} target={TargetTamerId}:{TargetName}", client.TamerId, client.Tamer.Name, targetClient.TamerId, targetClient.Tamer.Name);
                                }
                                break;

                            case "ban":
                                {
                                    var targetClient = FindOnlineClient(targetName);
                                    AccountDTO? account = null;

                                    if (targetClient != null)
                                        account = await _sender.Send(new AccountByIdQuery(targetClient.AccountId));
                                    else
                                        account = await FindAccountForAdminTarget(targetName);

                                    if (account == null)
                                    {
                                        client.Send(new SystemMessagePacket($"Account/tamer {targetName} not found."));
                                        break;
                                    }

                                    if (targetClient != null && targetClient.AccessLevel >= client.AccessLevel)
                                    {
                                        client.Send(new SystemMessagePacket("Cannot ban a player with equal or higher access level."));
                                        break;
                                    }

                                    if (account.AccessLevel >= client.AccessLevel)
                                    {
                                        client.Send(new SystemMessagePacket("Cannot ban an account with equal or higher access level."));
                                        break;
                                    }

                                    await _sender.Send(new DigitalWorldOnline.Application.Admin.Commands.UpdateAccountCommand(
                                        account.Id,
                                        account.Username,
                                        account.Email,
                                        AccountAccessLevelEnum.Blocked,
                                        account.Premium,
                                        account.Silk));

                                    if (targetClient != null)
                                    {
                                        targetClient.Send(new DisconnectUserPacket("Account blocked by administrator."));
                                        targetClient.Disconnect(true);
                                    }

                                    client.Send(new SystemMessagePacket($"Account {account.Username} blocked."));
                                    _logger.Information("GM admin ban gm={GmTamerId}:{GmName} account={AccountId}:{Username} target={Target}", client.TamerId, client.Tamer.Name, account.Id, account.Username, targetName);
                                }
                                break;

                            case "unban":
                                {
                                    var account = await FindAccountForAdminTarget(targetName);
                                    if (account == null)
                                    {
                                        client.Send(new SystemMessagePacket($"Account/tamer {targetName} not found."));
                                        break;
                                    }

                                    await _sender.Send(new DigitalWorldOnline.Application.Admin.Commands.UpdateAccountCommand(
                                        account.Id,
                                        account.Username,
                                        account.Email,
                                        AccountAccessLevelEnum.Default,
                                        account.Premium,
                                        account.Silk));

                                    client.Send(new SystemMessagePacket($"Account {account.Username} unblocked."));
                                    _logger.Information("GM admin unban gm={GmTamerId}:{GmName} account={AccountId}:{Username} target={Target}", client.TamerId, client.Tamer.Name, account.Id, account.Username, targetName);
                                }
                                break;
                        }
                    }
                    break;

                case "membership":
                    {
                        var regex = @"(membership\sadd\s\d{1,7}$){1}";
                        var match = Regex.Match(message, regex, RegexOptions.IgnoreCase);

                        if (!match.Success)
                        {
                            client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                            break;
                        }

                        switch (command[1])
                        {
                            case "add":
                                {
                                    var value = int.Parse(command[2]);

                                    client.IncreaseMembershipDuration(value);
                                    client.Send(new MembershipPacket(client.MembershipExpirationDate!.Value, client.MembershipUtcSeconds));

                                    await _sender.Send(new UpdateAccountMembershipCommand(client.AccountId, client.MembershipExpirationDate));
                                    await _ownerStorageFlushService.FlushForTransitionAsync(client);
                                    _mapServer.RemoveClient(client);
                                    client.SetGameQuit(false);
                                    client.Send(new MapSwapPacket(
                                        _configuration[GamerServerPublic],
                                        _configuration[GameServerPort],
                                        client.Tamer.Location.MapId,
                                        client.Tamer.Location.X,
                                        client.Tamer.Location.Y));
                                }
                                break;

                            default:
                                client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                                break;
                        }
                    }
                    break;

                default:
                    client.Send(new SystemMessagePacket($"Unknown command. Check the available commands on the Admin Portal."));
                    break;
            }
        }

        private GameClient? FindOnlineClient(string tamerName)
        {
            return
                _mapServer.FindClientByTamerName(tamerName) ??
                _dungeonServer.FindClientByTamerName(tamerName) ??
                _pvpServer.FindClientByTamerName(tamerName);
        }

        private static int ClampSummonStat(long value)
        {
            if (value < 1)
                return 1;
            if (value > 999999999)
                return 999999999;
            return (int)value;
        }

        private static int ClampSummonCustomStat(long value, int index)
        {
            var max = index == 0 ? byte.MaxValue : 999999999;
            if (value < 1)
                return 1;
            if (value > max)
                return max;
            return (int)value;
        }

        private static int ScaleSummonStat(int value, int percent)
        {
            return ClampSummonStat((long)value * percent / 100);
        }

        private static (byte Level, int Percent) ResolveSummonPreset(int baseLevel, string preset)
        {
            baseLevel = Math.Clamp(baseLevel, 1, byte.MaxValue);

            return preset switch
            {
                "easy" => ((byte)Math.Clamp(baseLevel * 60 / 100, 1, byte.MaxValue), 60),
                "medium" => ((byte)Math.Clamp(baseLevel * 150 / 100, 1, byte.MaxValue), 150),
                "hard" => ((byte)Math.Clamp(baseLevel * 250 / 100, 1, byte.MaxValue), 250),
                _ => ((byte)baseLevel, 100)
            };
        }

        private void SummonMonsterByGm(GameClient client, int monsterId, int amount, string preset = "normal", int[]? customStats = null, bool bossMonster = false)
        {
            if (_assets.Monster?.IsLoaded != true || !_assets.Monster.Data.ByType.TryGetValue(monsterId, out var rec))
            {
                client.Send(new SystemMessagePacket($"No monster info found with ID {monsterId}."));
                _logger.Warning("GM summonmonster failed. Monster {MonsterId} not found. gm={GmTamerId}", monsterId, client.TamerId);
                return;
            }

            var map = client.DungeonMap
                ? _dungeonServer.FindMapByTamer(client.TamerId)
                : _mapServer.FindMapByTamer(client.TamerId);

            if (map == null)
            {
                client.Send(new SystemMessagePacket("Unable to find your current map instance."));
                _logger.Warning("GM summonmonster failed. Map not found. gm={GmTamerId} monster={MonsterId}", client.TamerId, monsterId);
                return;
            }

            var rng = Random.Shared;
            var maxMobId = map.Mobs.Any() ? map.Mobs.Max(x => x.Id) : 0;
            var maxSummonId = map.SummonMobs.Any() ? map.SummonMobs.Max(x => x.Id) : 0;
            var firstId = Math.Max(maxMobId, maxSummonId) + 1;
            var presetValues = ResolveSummonPreset(rec.Level, preset);
            var level = customStats == null
                ? presetValues.Level
                : (byte)Math.Clamp(customStats[0], 1, byte.MaxValue);
            var percent = presetValues.Percent;

            for (var i = 0; i < amount; i++)
            {
                var x = client.Tamer.Location.X + (i == 0 ? 0 : rng.Next(-350, 351));
                var y = client.Tamer.Location.Y + (i == 0 ? 0 : rng.Next(-350, 351));

                var summon = new SummonMobModel
                {
                    Id = firstId + i,
                    Type = rec.Type,
                    Model = rec.ModelId,
                    Name = string.Empty,
                    Level = level,
                    ViewRange = rec.Sight,
                    HuntRange = rec.HuntRange,
                    ATValue = customStats == null ? ScaleSummonStat(rec.AttPower, percent) : customStats[3],
                    ASValue = customStats == null ? ScaleSummonStat(rec.AttSpeed, percent) : customStats[8],
                    ARValue = rec.AttRange,
                    DEValue = customStats == null ? ScaleSummonStat(rec.DefPower, percent) : customStats[4],
                    EVValue = customStats == null ? ScaleSummonStat(rec.Evasion, percent) : customStats[6],
                    HPValue = customStats == null ? ScaleSummonStat(rec.Hp, percent) : customStats[1],
                    HTValue = customStats == null ? ScaleSummonStat(rec.HitRate, percent) : customStats[5],
                    MSValue = rec.MoveSpeed,
                    WSValue = rec.WalkSpeed,
                    CTValue = customStats == null ? ScaleSummonStat(rec.CriticalRate, percent) : customStats[7],
                    DSValue = customStats == null ? ScaleSummonStat(rec.Ds, percent) : customStats[2],
                    Duration = 3600
                };

                summon.SetLocation(client.Tamer.Location.MapId, x, y);
                summon.SetDuration();
                summon.SetTargetSummonHandle(client.Tamer.GeneralHandler);
                summon.SetClass(bossMonster ? 8 : rec.Class);
                if (bossMonster)
                    summon.SetDropReward(new SummonMobDropRewardModel());
                map.AddMob(summon);
            }

            client.Send(new SystemMessagePacket($"Summoned monster {monsterId} x{amount} preset={preset} level={level} boss={bossMonster}."));
            _logger.Information("GM summonmonster success. gm={GmTamerId} monster={MonsterId} amount={Amount} preset={Preset} level={Level} boss={Boss} map={MapId}", client.TamerId, monsterId, amount, preset, level, bossMonster, map.MapId);
        }

        private async Task<AccountDTO?> FindAccountForAdminTarget(string accountOrTamerName)
        {
            var account = await _sender.Send(new AccountByUsernameQuery(accountOrTamerName));
            if (account != null)
                return account;

            var character = await _sender.Send(new CharacterByNameQuery(accountOrTamerName));
            if (character == null)
                return null;

            return await _sender.Send(new AccountByIdQuery(character.AccountId));
        }

        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }
    }
}
