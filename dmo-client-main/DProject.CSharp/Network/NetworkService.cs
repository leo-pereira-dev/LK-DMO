using DProject.Config;
using DProject.Data.CsFileTable;
using DProject.Data.Loading;
using DProject.GlobalData;
using DProject.Network.Protocol;
using DProject.Runtime;
using System.Collections.Concurrent;
using System.Diagnostics;

namespace DProject.Network;

public sealed class NetworkService : IDisposable
{
    private readonly AppConfig _config;
    private readonly MainThreadDispatcher _mainThread;
    private readonly RuntimeMetrics? _metrics;
    private readonly GameStateSync? _gameStateSync;
    private readonly GlobalDataStore? _globalData;
    private readonly MapLoadingService? _mapLoading;
    private readonly ConcurrentQueue<Action> _outbound = new();
    private readonly CancellationTokenSource _cts = new();
    private readonly cCliAccount _accountClient = new();
    private readonly cCliGate _gateClient = new();
    private readonly cCliGame _gameClient = new();
    private readonly object _stateLock = new();
    private readonly List<ServerInfo> _servers = new();
    private readonly List<CharacterSummary> _characters = new();
    private readonly Dictionary<byte, byte> _channels = new();
    private Thread? _networkThread;
    private GateConnectionInfo? _gateConnection;
    private GameConnectionInfo? _gameConnection;
    private CharacterSelectResult? _characterSelectResult;
    private GameInitData? _gameInitData;
    private PortalTransitionInfo? _portalTransition;
    private bool _started;
    private int _loginRequested;
    private int _loginAccepted;

    public NetworkService(
        AppConfig config,
        MainThreadDispatcher mainThread,
        RuntimeMetrics? metrics = null,
        GameStateSync? gameStateSync = null,
        GlobalDataStore? globalData = null,
        MapLoadingService? mapLoading = null)
    {
        _config = config;
        _mainThread = mainThread;
        _metrics = metrics;
        _gameStateSync = gameStateSync;
        _globalData = globalData;
        _mapLoading = mapLoading;
        _gameClient.ConfigureFileTables(nsCsFileTable.g_FileTableMng);
        WireClientEvents();
    }

    public void Start()
    {
        if (_started)
        {
            return;
        }

        ProtocolValidationResult protocolValidation = CommonProtocolCatalog.Validate();
        if (!protocolValidation.IsValid)
        {
            string issues = string.Join("; ", protocolValidation.Issues.Select(static issue => issue.Message));
            throw new InvalidOperationException($"Protocol catalog validation failed: {issues}");
        }

        _started = true;
        EndpointOptions accountEndpoint = _config.GetAccountEndpoint();
        EndpointOptions characterEndpoint = _config.GetCharacterEndpoint();
        EndpointOptions gameEndpoint = _config.GetGameEndpoint();
        _accountClient.NetVersion = _config.Network.NetVersion == 0
            ? DmoVersion.UsaNetVersion
            : _config.Network.NetVersion;
        _accountClient.UserType = _config.Network.UserType;
        _gateClient.TamerEquipBytes = _config.Network.TamerEquipBytes <= 0
            ? pLimit.TamerEquipBytesCompat487
            : _config.Network.TamerEquipBytes;
        _gameClient.NetVersion = _accountClient.NetVersion;
        _accountClient.Start(
            accountEndpoint.Host,
            accountEndpoint.Port,
            _config.Network.UseRealSockets,
            _config.Network.DecodePacketsOnWorker);
        _gateClient.Start(
            characterEndpoint.Host,
            characterEndpoint.Port,
            _config.Network.UseRealSockets,
            _config.Network.DecodePacketsOnWorker);
        _gameClient.Start(
            gameEndpoint.Host,
            gameEndpoint.Port,
            _config.Network.UseRealSockets,
            _config.Network.DecodePacketsOnWorker);

        if (_config.Threading.EnableNetworkThread)
        {
            _networkThread = new Thread(NetworkLoop)
            {
                IsBackground = true,
                Name = "DProject.Network"
            };
            _networkThread.Start();
        }
    }

    public void Execute()
    {
        if (_config.Threading.EnableNetworkThread)
        {
            return;
        }

        PumpOutbound();
    }

    public void Stop()
    {
        _started = false;
        _cts.Cancel();
        _networkThread?.Join(TimeSpan.FromSeconds(1));
        _gameClient.Stop();
        _gateClient.Stop();
        _accountClient.Stop();
    }

    public bool Login(string accountId, string password)
    {
        if (!_started || string.IsNullOrWhiteSpace(accountId))
        {
            return false;
        }

        if (Interlocked.CompareExchange(ref _loginRequested, 1, 0) == 0)
        {
            _outbound.Enqueue(() =>
            {
                _accountClient.SendLogin(accountId, password);
                _mainThread.Post(() => { });
            });
            _metrics?.RecordNetworkQueueDepth(_outbound.Count);

            if (!_config.Network.UseRealSockets)
            {
                Interlocked.Exchange(ref _loginAccepted, 1);
            }
        }

        return Volatile.Read(ref _loginAccepted) == 1;
    }

    public IReadOnlyList<string> GetServerList()
    {
        lock (_stateLock)
        {
            return _servers.Count == 0
                ? new[] { _config.Debug.Server }
                : _servers.Select(static server => server.Name).ToArray();
        }
    }

    public IReadOnlyList<string> GetCharacterList()
    {
        lock (_stateLock)
        {
            return _characters.Count == 0
                ? new[] { "PortedTamer" }
                : _characters.Select(static character => character.TamerName).ToArray();
        }
    }

    public GateConnectionInfo? GetGateConnection()
    {
        lock (_stateLock)
        {
            return _gateConnection;
        }
    }

    public CharacterSelectResult? GetCharacterSelectResult()
    {
        lock (_stateLock)
        {
            return _characterSelectResult;
        }
    }

    public GameInitData? GetGameInitData()
    {
        lock (_stateLock)
        {
            return _gameInitData;
        }
    }

    public IReadOnlyDictionary<byte, byte> GetChannelInfo()
    {
        lock (_stateLock)
        {
            return new Dictionary<byte, byte>(_channels);
        }
    }

    public bool SendGameReady(int x, int y)
    {
        if (!_started)
        {
            return false;
        }

        _outbound.Enqueue(() => _gameClient.SendReady(x, y));
        _metrics?.RecordNetworkQueueDepth(_outbound.Count);
        return true;
    }

    public bool SelectServerByName(string serverName)
    {
        if (!_started)
        {
            return false;
        }

        uint serverIndex = 0;
        lock (_stateLock)
        {
            ServerInfo? server = _servers.FirstOrDefault(item =>
                string.Equals(item.Name, serverName, StringComparison.OrdinalIgnoreCase));
            if (server is not null)
            {
                serverIndex = unchecked((uint)server.Index);
            }
        }

        _outbound.Enqueue(() => _accountClient.SendSelectCluster(serverIndex));
        _metrics?.RecordNetworkQueueDepth(_outbound.Count);
        return true;
    }

    public bool SelectCharacter(string characterName)
    {
        if (!_started)
        {
            return false;
        }

        uint slotNo = 0;
        lock (_stateLock)
        {
            CharacterSummary? character = _characters.FirstOrDefault(item =>
                string.Equals(item.TamerName, characterName, StringComparison.OrdinalIgnoreCase));
            if (character is not null)
            {
                slotNo = unchecked((uint)character.SlotNumber);
            }
        }

        _outbound.Enqueue(() => _gateClient.SendSelect(slotNo));
        _metrics?.RecordNetworkQueueDepth(_outbound.Count);
        return true;
    }

    private void NetworkLoop()
    {
        while (!_cts.IsCancellationRequested)
        {
            PumpOutbound();
            Thread.Sleep(Math.Max(1, _config.Threading.NetworkTickMilliseconds));
        }
    }

    private void PumpOutbound()
    {
        var stopwatch = Stopwatch.StartNew();
        int processed = 0;
        while (_outbound.TryDequeue(out Action? action))
        {
            action();
            processed++;
        }

        processed += _accountClient.OnExecute(_config.Network.MaxDispatchPerPump);
        processed += _gateClient.OnExecute(_config.Network.MaxDispatchPerPump);
        processed += _gameClient.OnExecute(_config.Network.MaxDispatchPerPump);
        stopwatch.Stop();
        int queueDepth = _outbound.Count
            + _accountClient.PendingDispatchCount
            + _gateClient.PendingDispatchCount
            + _gameClient.PendingDispatchCount;
        _metrics?.RecordNetworkPump(stopwatch.Elapsed, processed, queueDepth);
    }

    private void WireClientEvents()
    {
        _accountClient.LoginResultReceived += result =>
        {
            if (result.Succeeded)
            {
                Interlocked.Exchange(ref _loginAccepted, 1);
                _outbound.Enqueue(_accountClient.SendReqClusterList);
            }
            else
            {
                Interlocked.Exchange(ref _loginAccepted, 0);
            }
        };

        _accountClient.ClusterListReceived += servers =>
        {
            lock (_stateLock)
            {
                _servers.Clear();
                _servers.AddRange(servers);
            }
        };

        _accountClient.GateInfoReceived += gateInfo =>
        {
            lock (_stateLock)
            {
                _gateConnection = gateInfo;
            }

            _gateClient.ConfigureAccess(gateInfo);
            _outbound.Enqueue(_gateClient.SendAccessCode);
        };

        _accountClient.SelectClusterReceived += _ =>
        {
        };

        _gateClient.CharacterListReceived += characters =>
        {
            lock (_stateLock)
            {
                _characters.Clear();
                _characters.AddRange(characters);
            }
        };

        _gateClient.ChangeServerReceived += () =>
        {
            lock (_stateLock)
            {
                _characterSelectResult = _characterSelectResult is null
                    ? new CharacterSelectResult(true, string.Empty, 0, 0, 0)
                    : _characterSelectResult with { Succeeded = true };
            }
        };

        _gateClient.CharacterSelectReceived += result =>
        {
            GateConnectionInfo? gateConnection;
            lock (_stateLock)
            {
                _characterSelectResult = result;
                gateConnection = _gateConnection;
                if (result.Succeeded && gateConnection is not null)
                {
                    _gameConnection = new GameConnectionInfo(
                        gateConnection.AccountIndex,
                        gateConnection.AccessCode,
                        result.GameHost,
                        result.GamePort,
                        result.MapId);
                }
            }

            if (result.Succeeded && gateConnection is not null)
            {
                var gameConnection = new GameConnectionInfo(
                    gateConnection.AccountIndex,
                    gateConnection.AccessCode,
                    result.GameHost,
                    result.GamePort,
                    result.MapId);
                _gameClient.ConfigureAccess(gameConnection);
                _outbound.Enqueue(_gameClient.SendAccessCode);
            }

            if (result.Succeeded)
            {
                _mainThread.Post(() =>
                {
                    if (_globalData is not null)
                    {
                        _globalData.GameServerHost = result.GameHost;
                        _globalData.GameServerPort = result.GamePort;
                    }
                });
                _gameStateSync?.RequestMapTransition(result.MapId, $"Map {result.MapId}", "cCliGate.RecvTamerSelectSuccess");
            }
        };

        _gameClient.ChannelInfoReceived += channelInfo =>
        {
            lock (_stateLock)
            {
                _channels.Clear();
                foreach (KeyValuePair<byte, byte> channel in channelInfo.Channels)
                {
                    _channels[channel.Key] = channel.Value;
                }
            }
        };

        _gameClient.InitGameDataReceived += initData =>
        {
            uint mapId;
            lock (_stateLock)
            {
                _gameInitData = initData;
                mapId = _gameConnection?.MapId ?? _characterSelectResult?.MapId ?? 0;
            }

            if (_gameStateSync is not null)
            {
                _gameStateSync.ApplyInitGameData(initData);
            }
            else
            {
                _mainThread.Post(() =>
                {
                    if (_globalData is not null)
                    {
                        _globalData.ServerLibType = initData.ServerLibType;
                        _globalData.StartPositionX = initData.StartX;
                        _globalData.StartPositionY = initData.StartY;
                        _globalData.SelectedCharacterName = string.IsNullOrWhiteSpace(initData.TamerName)
                            ? _globalData.SelectedCharacterName
                            : initData.TamerName;
                    }
                });
            }

            CompleteOrLoadMap(mapId, mapId == 0 ? "Game" : $"Map {mapId}", "pGame.InitGameData");
        };

        _gameClient.SyncReceived += ApplyGameSync;

        _gameClient.PortalTransitionReceived += portal =>
        {
            lock (_stateLock)
            {
                _portalTransition = portal;
                if (_gameConnection is not null)
                {
                    _gameConnection = _gameConnection with
                    {
                        Host = portal.Host,
                        Port = portal.Port,
                        MapId = portal.MapId
                    };
                }
            }

            _mainThread.Post(() =>
            {
                if (_globalData is not null)
                {
                    _globalData.GameServerHost = portal.Host;
                    _globalData.GameServerPort = portal.Port;
                    _globalData.StartPositionX = portal.X;
                    _globalData.StartPositionY = portal.Y;
                }
            });
            _gameStateSync?.RequestMapTransition(portal.MapId, $"Map {portal.MapId}", "cCliGame.RecvSelectPortalSuccess");
        };

        _gameClient.LocalPortalReceived += portal =>
        {
            uint mapId;
            lock (_stateLock)
            {
                _portalTransition = _portalTransition is null
                    ? new PortalTransitionInfo(string.Empty, 0, 0, portal.X, portal.Y)
                    : _portalTransition with { X = portal.X, Y = portal.Y };
                mapId = _portalTransition.MapId;
            }

            _mainThread.Post(() =>
            {
                if (_globalData is not null)
                {
                    _globalData.StartPositionX = portal.X;
                    _globalData.StartPositionY = portal.Y;
                }
            });
            CompleteOrLoadMap(mapId, mapId == 0 ? "LocalPortal" : $"Map {mapId}", "cCliGame.RecvLocalPortal");
        };

        _gameClient.ChangeServerReceived += () =>
        {
            uint mapId;
            lock (_stateLock)
            {
                mapId = _gameConnection?.MapId ?? 0;
            }

            if (mapId != 0)
            {
                _gameStateSync?.RequestMapTransition(mapId, $"Map {mapId}", "cCliGame.RecvChangeServer");
            }

            _outbound.Enqueue(_gameClient.SendAccessCode);
        };
    }

    private void CompleteOrLoadMap(uint mapId, string fallbackName, string reason)
    {
        if (_gameStateSync is null)
        {
            return;
        }

        if (mapId != 0 && _mapLoading is not null && nsCsFileTable.g_FileTableMng.IsInitialized)
        {
            _mapLoading.RequestLoad(nsCsFileTable.g_FileTableMng, mapId, fallbackName, reason, result =>
            {
                string mapName = result.Plan?.MapName ?? fallbackName;
                _gameStateSync.CompleteMapTransition(mapId, mapName);
                _gameStateSync.LoadMapNpcs(nsCsFileTable.g_FileTableMng, mapId);
            });
            return;
        }

        _gameStateSync.CompleteMapTransition(mapId, fallbackName);
        _gameStateSync.LoadMapNpcs(nsCsFileTable.g_FileTableMng, mapId);
    }

    private void ApplyGameSync(GameSyncSummary summary)
    {
        if (_gameStateSync is null)
        {
            return;
        }

        var snapshotUids = new HashSet<uint>();
        foreach (SyncWorldObjectSnapshot snapshot in summary.WorldObjects)
        {
            snapshotUids.Add(snapshot.ObjectType.Uid);
            _gameStateSync.UpsertWorldObject(snapshot, SyncCommandName(snapshot.Command));
        }

        foreach (SyncPresence presence in summary.PresenceChanges)
        {
            string source = SyncCommandName(presence.Command);
            if (presence.Command == pSync.New || presence.Command == pSync.In)
            {
                if (snapshotUids.Contains(presence.ObjectType.Uid))
                {
                    continue;
                }

                SyncPosition position = presence.Position ?? new SyncPosition(0, 0);
                _gameStateSync.UpsertWorldObject(
                    presence.ObjectType.Uid,
                    presence.ObjectType.Index,
                    presence.ObjectType.ClassId,
                    presence.ObjectType.TypeId,
                    position.X,
                    position.Y,
                    source);
                continue;
            }

            _gameStateSync.RemoveWorldObject(presence.ObjectType.Uid, source);
        }

        foreach (SyncMovement movement in summary.Movements)
        {
            _gameStateSync.MoveWorldObject(
                movement.ObjectType.Uid,
                movement.ObjectType.Index,
                movement.ObjectType.ClassId,
                movement.Position.X,
                movement.Position.Y,
                SyncCommandName(movement.Command));
        }

        foreach (SyncRotation rotation in summary.Rotations)
        {
            _gameStateSync.RotateWorldObject(
                rotation.ObjectType.Uid,
                rotation.ObjectType.Index,
                rotation.ObjectType.ClassId,
                rotation.Direction,
                SyncCommandName(pSync.Rotation));
        }

        _gameStateSync.ApplyConditionChanges(summary.ConditionChanges, SyncCommandName(pSync.Condition));
        _gameStateSync.ApplyBuffSync(summary.BuffObjects);
        _gameStateSync.AppendSyncChatMessages(summary.ChatMessages);

        int objectCount =
            summary.WorldObjects.Count +
            summary.PresenceChanges.Count +
            summary.Movements.Count +
            summary.Rotations.Count +
            summary.ConditionChanges.Count +
            summary.BuffObjects.Count +
            summary.ChatMessages.Count;
        _gameStateSync.RecordMovementSync(objectCount, summary.RemainingBytes);
    }

    private static string SyncCommandName(byte command)
    {
        return command switch
        {
            pSync.New => "pSync.New",
            pSync.Delete => "pSync.Delete",
            pSync.In => "pSync.In",
            pSync.Out => "pSync.Out",
            pSync.Walk => "pSync.Walk",
            pSync.Move => "pSync.Move",
            pSync.MsgChat => "pSync.MsgChat",
            pSync.MsgWhisper => "pSync.MsgWhisper",
            pSync.MsgAll => "pSync.MsgAll",
            pSync.MsgSystem => "pSync.MsgSystem",
            pSync.MsgShout => "pSync.MsgShout",
            pSync.MsgAllByItem => "pSync.MsgAllByItem",
            pSync.Condition => "pSync.Condition",
            pSync.BuffSync => "pSync.BuffSync",
            pSync.Rotation => "pSync.Rotation",
            pSync.MsgGuildShout => "pSync.MsgGuildShout",
            _ => $"pSync.{command}"
        };
    }

    public void Dispose()
    {
        Stop();
        _gameClient.Dispose();
        _gateClient.Dispose();
        _accountClient.Dispose();
        _cts.Dispose();
    }
}
