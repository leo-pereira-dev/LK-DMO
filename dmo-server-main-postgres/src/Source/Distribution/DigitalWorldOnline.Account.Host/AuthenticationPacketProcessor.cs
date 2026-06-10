using AutoMapper;
using DigitalWorldOnline.Application.Separar.Commands.Create;
using DigitalWorldOnline.Application.Separar.Commands.Update;
using DigitalWorldOnline.Application.Separar.Queries;
using DigitalWorldOnline.Commons.Entities;
using DigitalWorldOnline.Commons.Enums.Account;
using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Enums.PacketProcessor;
using DigitalWorldOnline.Commons.Extensions;
using DigitalWorldOnline.Commons.Interfaces;
using DigitalWorldOnline.Commons.Models.Account;
using DigitalWorldOnline.Commons.Models.Servers;
using DigitalWorldOnline.Commons.Packets.AuthenticationServer;
using MediatR;
using Microsoft.Extensions.Configuration;
using Serilog;
using System.Text;

namespace DigitalWorldOnline.Account
{
    public sealed class AuthenticationPacketProcessor : IProcessor, IDisposable
    {
        private readonly IConfiguration _configuration;
        private readonly IMapper _mapper;
        private readonly ISender _sender;
        private readonly ILogger _logger;

        private const string CharacterServerAddress = "CharacterServer:Address";
        private const string CharacterServerPublicPort = "CharacterServer:PublicPort";
        private const string AuthenticationServerHash = "AuthenticationServer:Hash";

        private const int HandshakeDegree = 32321;
        private const int MaxUsernameLength = 20;
        private const int MaxSystemInfoLength = 50;

        public AuthenticationPacketProcessor(IMapper mapper, ILogger logger, ISender sender, IConfiguration configuration)
        {
            _configuration = configuration;
            _mapper = mapper;
            _sender = sender;
            _logger = logger;
        }

        /// <summary>
        /// Process the arrived TCP packet, sent from the game client
        /// </summary>
        /// <param name="client">The game client whos sended the packet</param>
        /// <param name="data">The packet bytes array</param>
        public async Task ProcessPacketAsync(GameClient client, byte[] data)
        {
            var packet = new AuthenticationPacketReader(data);

            switch (packet.Enum)
            {
                case AuthenticationServerPacketEnum.Connection:
                    {
                        DebugLog("Reading packet parameters...");
                        var kind = packet.ReadByte();

                        var handshakeTimestamp = (uint)DateTimeOffset.UtcNow.ToUnixTimeSeconds();
                        var handshake = (short)(client.Handshake ^ HandshakeDegree);

                        client.Send(new ConnectionPacket(handshake, handshakeTimestamp));
                    }
                    break;

                case AuthenticationServerPacketEnum.KeepConnection:
                    break;

                case AuthenticationServerPacketEnum.LoginRequest:
                    {
                        DebugLog("Reading packet parameters...");
                        LoginRequestData loginRequest;
                        try
                        {
                            loginRequest = ExtractLoginRequest(packet);
                        }
                        catch (InvalidDataException ex)
                        {
                            _logger.Warning(
                                ex,
                                "Rejected malformed login request from {Address}. {PacketDetails}",
                                client.ClientAddress,
                                DescribeLoginRequestPacket(packet.ToArray(), packet.Length));
                            client.Send(new LoginRequestAnswerPacket(LoginFailReasonEnum.UserNotFound));
                            break;
                        }

                        var username = loginRequest.Username;
                        var password = loginRequest.Password;
                        var cpu = loginRequest.Cpu;
                        var gpu = loginRequest.Gpu;

                        _logger.Information(
                            "Login request parsed from {Address}: username={Username} userLen={UserLength} passLen={PasswordLength} cpuLen={CpuLength} gpuLen={GpuLength} packetLength={PacketLength}.",
                            client.ClientAddress,
                            username,
                            username.Length,
                            password.Length,
                            cpu.Length,
                            gpu.Length,
                            packet.Length);

                        if (!IsValidUsername(username))
                        {
                            _logger.Warning(
                                "Rejected login request with invalid username length {Length} from {Address}. username={Username}",
                                username.Length,
                                client.ClientAddress,
                                ToLoginTryUsername(username));

                            await _sender.Send(
                                new CreateLoginTryCommand(
                                    ToLoginTryUsername(username),
                                    client.ClientAddress,
                                    LoginTryResultEnum.IncorrectUsername
                                )
                            );

                            client.Send(new LoginRequestAnswerPacket(LoginFailReasonEnum.UserNotFound));
                            break;
                        }

                        DebugLog($"Validating login data for {username}...");
                        var account = await _sender.Send(new AccountByUsernameQuery(username));

                        if (account == null)
                        {
                            _logger.Warning(
                                "Login rejected: username {Username} was not found from {Address}.",
                                username,
                                client.ClientAddress);
                            await _sender.Send(
                                new CreateLoginTryCommand(
                                    ToLoginTryUsername(username),
                                    client.ClientAddress,
                                    LoginTryResultEnum.IncorrectUsername
                                )
                            );

                            client.Send(new LoginRequestAnswerPacket(LoginFailReasonEnum.UserNotFound));
                            break;
                        }

                        client.SetAccountId(account.Id);
                        client.SetAccessLevel(account.AccessLevel);
                        client.SetAccountSecondaryPassword(account.SecondaryPassword);

                        if (account.AccountBlock != null)
                        {
                            var blockInfo = _mapper.Map<AccountBlockModel>(await _sender.Send(new AccountBlockByIdQuery(account.AccountBlock.Id)));

                            DebugLog($"Saving {username} login try for blocked account...");
                            await _sender.Send(new CreateLoginTryCommand(ToLoginTryUsername(username), client.ClientAddress, LoginTryResultEnum.AccountBlocked));

                            client.Send(new LoginRequestBannedAnswerPacket(blockInfo));
                            break;
                        }

                        if (account.Password != password.Encrypt())
                        {
                            _logger.Warning(
                                "Login rejected: incorrect password for username {Username} from {Address}.",
                                username,
                                client.ClientAddress);
                            await _sender.Send(new CreateLoginTryCommand(ToLoginTryUsername(username), client.ClientAddress, LoginTryResultEnum.IncorrectPassword));

                            client.Send(new LoginRequestAnswerPacket(LoginFailReasonEnum.IncorrectPassword));
                            break;
                        }

                        _logger.Information(
                            "Login accepted for username {Username} accountId {AccountId}; secondaryPasswordConfigured={HasSecondaryPassword}.",
                            username,
                            account.Id,
                            !string.IsNullOrEmpty(account.SecondaryPassword));

                        if (string.IsNullOrEmpty(account.SecondaryPassword))
                        {
                            //Obs.: The client itself handles the "Not Today" checkbox
                            //When checked, sending "3" will not show the request screen
                            client.Send(new LoginRequestAnswerPacket(SecondaryPasswordScreenEnum.RequestSetup));
                        }
                        else
                        {
                            client.Send(new LoginRequestAnswerPacket(SecondaryPasswordScreenEnum.RequestInput));
                        }

                        if (bool.Parse(_configuration[AuthenticationServerHash]))
                        {
                            DebugLog("Getting resources hash...");
                            var hashString = await _sender.Send(new ResourcesHashQuery());

                            client.Send(new ResourcesHashPacket(hashString));
                        }

                        if (account.SystemInformation == null)
                        {
                            //TODO: Create equipment change history

                            DebugLog($"Creating system information...");
                            await _sender.Send(
                                new CreateSystemInformationCommand(
                                    account.Id, 
                                    cpu, 
                                    gpu, 
                                    client.ClientAddress
                                )
                            );
                        }
                        else
                        {
                            DebugLog($"Updating system information...");
                            await _sender.Send(
                                new UpdateSystemInformationCommand(
                                    account.SystemInformation.Id, 
                                    account.Id, 
                                    cpu, 
                                    gpu, 
                                    client.ClientAddress
                                )
                            );
                        }
                    }
                    break;

                case AuthenticationServerPacketEnum.SecondaryPasswordRegister:
                    {
                        DebugLog("Reading packet parameters...");
                        var securityPassword = packet.ReadZString();

                        DebugLog($"Updating {client.AccountId} account information...");
                        await _sender.Send(new CreateOrUpdateSecondaryPasswordCommand(client.AccountId, securityPassword));

                        client.Send(new LoginRequestAnswerPacket(SecondaryPasswordScreenEnum.RequestInput));
                    }
                    break;

                case AuthenticationServerPacketEnum.SecondaryPasswordCheck:
                    {
                        DebugLog("Reading packet first part parameters...");
                        var needToCheck = packet.ReadShort() == SecondaryPasswordCheckEnum.Check.GetHashCode();

                        if (needToCheck)
                        {
                            DebugLog("Reading packet second part parameters...");
                            var securitycode = packet.ReadZString();

                            if (client.AccountSecondaryPassword == securitycode)
                            {
                                client.Send(
                                    new SecondaryPasswordCheckResultPacket(SecondaryPasswordCheckEnum.CorrectOrSkipped));
                            }
                            else
                            {
                                client.Send(new SecondaryPasswordCheckResultPacket(SecondaryPasswordCheckEnum.Incorrect));
                            }
                        }
                        else
                        {
                            DebugLog($"Sending answer for skipped secondary password check...");
                            client.Send(new SecondaryPasswordCheckResultPacket(SecondaryPasswordCheckEnum.CorrectOrSkipped).Serialize());
                        }
                    }
                    break;

                case AuthenticationServerPacketEnum.SecondaryPasswordChange:
                    {
                        DebugLog("Getting packet parameters...");
                        var currentSecurityCode = packet.ReadZString();
                        var newSecurityCode = packet.ReadZString();

                        DebugLog($"{currentSecurityCode} {newSecurityCode}");

                        DebugLog($"Checking secondary password...");

                        if (client.AccountSecondaryPassword == currentSecurityCode)
                        {
                            DebugLog($"Saving new secondary password...");
                            await _sender.Send(new CreateOrUpdateSecondaryPasswordCommand(client.AccountId, newSecurityCode));
                            client.SetAccountSecondaryPassword(newSecurityCode);

                            DebugLog($"Sending answer for correct secondary password check...");
                            client.Send(new SecondaryPasswordChangeResultPacket(SecondaryPasswordChangeEnum.Changed).Serialize());
                        }
                        else
                        {
                            DebugLog($"Sending answer for incorrect secondary password change...");
                            client.Send(new SecondaryPasswordChangeResultPacket(SecondaryPasswordChangeEnum.IncorretCurrentPassword).Serialize());
                        }
                    }
                    break;

                case AuthenticationServerPacketEnum.LoadServerList:
                    {
                        //TODO: Disconnect current account

                        DebugLog($"Getting server list...");
                        var servers = _mapper.Map<IEnumerable<ServerObject>>(
                            await _sender.Send(new ServersQuery(client.AccessLevel)));

                        foreach (var server in servers)
                            server.UpdateCharacterCount(await _sender.Send(new CharactersInServerQuery(client.AccountId, server.Id)));

                        DebugLog($"Sending server list...");
                        client.Send(new ServerListPacket(servers).Serialize());
                    }
                    break;

                case AuthenticationServerPacketEnum.ConnectCharacterServer:
                    {
                        DebugLog($"Reading packet parameters...");
                        var serverId = packet.ReadInt();

                        DebugLog($"Updating account {client.AccountId} last selected server for {serverId}...");
                        await _sender.Send(new UpdateLastPlayedServerCommand(client.AccountId, serverId));

                        if (bool.Parse(_configuration[AuthenticationServerHash]))
                        {
                            DebugLog("Getting resources hash...");
                            var hashString = await _sender.Send(new ResourcesHashQuery());

                            client.Send(new ResourcesHashPacket(hashString));
                        }

                        DebugLog($"Getting server list...");
                        var servers = _mapper.Map<IEnumerable<ServerObject>>(
                            await _sender.Send(new ServersQuery(client.AccessLevel)));

                        var targetServer = servers.First(x => x.Id == serverId);

                        DebugLog($"Sending selected server info...");
                        client.Send(new ConnectCharacterServerPacket(
                            client.AccountId,
                            _configuration[CharacterServerAddress],
                            _configuration[CharacterServerPublicPort] ?? targetServer.Port.ToString())
                        );
                    }
                    break;

                default:
                    {
                        _logger.Warning($"Unknown packet. Type: {packet.Type} Length: {packet.Length}.");
                    }
                    break;
            }
        }

        private static LoginRequestData ExtractLoginRequest(AuthenticationPacketReader packet)
        {
            var data = packet.ToArray();
            var end = Math.Min(packet.Length - 2, data.Length);

            foreach (var usernameOffset in new[] { 8, 9 })
            {
                if (TryExtractLoginRequest(data, usernameOffset, end, out var loginRequest))
                    return loginRequest;
            }

            throw new InvalidDataException("Login request payload does not match the expected client layout.");
        }

        private static string DescribeLoginRequestPacket(byte[] data, int packetLength)
        {
            var end = Math.Min(packetLength - 2, data.Length);
            var details = new StringBuilder();
            details.Append($"packetLength={packetLength}, dataLength={data.Length}, payloadEnd={end}");

            foreach (var usernameOffset in new[] { 8, 9 })
            {
                var offset = usernameOffset;
                details.Append($"; offset{usernameOffset}: ");
                details.Append(DescribeSizedAsciiField(data, ref offset, end, MaxUsernameLength, "username"));

                if (offset < end && data[offset] == 0)
                {
                    details.Append(", separator=00");
                    offset++;
                }

                details.Append(", ");
                details.Append(DescribeSizedAsciiField(data, ref offset, end, 64, "password"));
                details.Append($", next={offset}");
            }

            return details.ToString();
        }

        private static string DescribeSizedAsciiField(
            byte[] data,
            ref int offset,
            int end,
            int maxLength,
            string fieldName)
        {
            if (offset >= end)
                return $"{fieldName}=missing(at={offset})";

            var sizeOffset = offset;
            var size = data[offset++];

            if (size == 0)
                return $"{fieldName}=zero(at={sizeOffset})";

            if (size > maxLength)
                return $"{fieldName}=too-long(size={size}, max={maxLength}, at={sizeOffset})";

            if (offset + size > end)
                return $"{fieldName}=overrun(size={size}, at={sizeOffset}, end={end})";

            for (var i = offset; i < offset + size; i++)
            {
                if (data[i] < 32 || data[i] > 126)
                {
                    var value = data[i].ToString("X2");
                    return $"{fieldName}=non-ascii(size={size}, byte=0x{value}, at={i})";
                }
            }

            offset += size;
            return $"{fieldName}=ok(size={size}, at={sizeOffset})";
        }

        private static bool TryExtractLoginRequest(
            byte[] data,
            int usernameOffset,
            int end,
            out LoginRequestData loginRequest)
        {
            loginRequest = new LoginRequestData(string.Empty, string.Empty, string.Empty, string.Empty);
            var offset = usernameOffset;

            if (!TryReadSizedAsciiField(data, ref offset, end, MaxUsernameLength, out var username))
                return false;

            if (offset < end && data[offset] == 0)
                offset++;

            if (!TryReadSizedAsciiField(data, ref offset, end, 64, out var password))
                return false;

            var cpu = TryReadOptionalSizedAsciiField(data, ref offset, end, MaxSystemInfoLength, out var cpuValue)
                ? cpuValue
                : string.Empty;
            var gpu = TryReadOptionalSizedAsciiField(data, ref offset, end, MaxSystemInfoLength, out var gpuValue)
                ? gpuValue
                : string.Empty;

            loginRequest = new LoginRequestData(username, password, cpu, gpu);
            return true;
        }

        private static bool TryReadOptionalSizedAsciiField(
            byte[] data,
            ref int offset,
            int end,
            int maxLength,
            out string value)
        {
            var start = offset;
            if (TryReadSizedAsciiField(data, ref offset, end, maxLength, out value))
                return true;

            offset = start;
            value = string.Empty;
            return false;
        }

        private static bool TryReadSizedAsciiField(
            byte[] data,
            ref int offset,
            int end,
            int maxLength,
            out string value)
        {
            value = string.Empty;
            if (offset >= end)
                return false;

            var size = data[offset++];
            if (size == 0 || size > maxLength || offset + size > end)
                return false;

            for (var i = offset; i < offset + size; i++)
            {
                if (data[i] < 32 || data[i] > 126)
                    return false;
            }

            value = Encoding.ASCII.GetString(data, offset, size).Trim();
            offset += size;
            return !string.IsNullOrWhiteSpace(value);
        }

        private static bool IsValidUsername(string username)
        {
            return !string.IsNullOrWhiteSpace(username) && username.Length <= MaxUsernameLength;
        }

        private static string ToLoginTryUsername(string username)
        {
            var normalized = string.IsNullOrWhiteSpace(username) ? "<empty>" : username;

            return Truncate(normalized, MaxUsernameLength);
        }

        private static string Truncate(string value, int maxLength)
        {
            if (string.IsNullOrEmpty(value) || value.Length <= maxLength)
                return value;

            return value[..maxLength];
        }

        private sealed record LoginRequestData(string Username, string Password, string Cpu, string Gpu);

        /// <summary>
        /// Shortcut for debug logging with client and packet info.
        /// </summary>
        /// <param name="message">The message to log</param>
        private void DebugLog(string message)
        {
            _logger?.Debug($"{message}");
        }

        /// <summary>
        /// Disposes the entire object.
        /// </summary>
        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }
    }
}
