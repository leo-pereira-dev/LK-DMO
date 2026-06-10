using DProject.Network.Protocol;

namespace DProject.Network;

public sealed class cCliAccount : cClient
{
    public cCliAccount() : base(nameof(cCliAccount))
    {
    }

    public event Action<AccountLoginResult>? LoginResultReceived;
    public event Action<uint>? LoginWaitTimeReceived;
    public event Action<IReadOnlyList<ServerInfo>>? ClusterListReceived;
    public event Action<GateConnectionInfo>? GateInfoReceived;
    public event Action<int>? SelectClusterReceived;
    public event Action<SecondPasswordResult>? SecondPasswordReceived;
    public event Action<NetworkPacketEnvelope>? UnknownPacketReceived;

    public uint IsChangeServer { get; private set; }
    public uint NetVersion { get; set; } = DmoVersion.UsaNetVersion;
    public string UserType { get; set; } = string.Empty;

    public bool SendLogin(string userId, string userPassword)
    {
        if (!IsConnected || string.IsNullOrWhiteSpace(userId))
        {
            return false;
        }

        var packet = new cPacket();
        packet.PushUInt32(NetVersion);
        packet.PushString(UserType, skipEmpty: true);
        packet.PushString(userId);
        packet.PushByte(0);
        packet.PushString(userPassword);
        Send(pLogin.Request, packet);
        return true;
    }

    public void SendReqClusterList()
    {
        Send(pSvr.ClusterList, new cPacket());
    }

    public bool SendSelectCluster(uint serverIndex)
    {
        IsChangeServer = serverIndex;
        var packet = new cPacket();
        packet.PushUInt32(serverIndex);
        Send(pSvr.SelectCluster, packet);
        return IsConnected;
    }

    public bool SendRegister2ndPass(ReadOnlySpan<byte> passwordHash)
    {
        var packet = new cPacket();
        packet.PushBytes(passwordHash);
        Send(pPass2.Register, packet);
        return IsConnected;
    }

    public void SendSecondPassCertified(ReadOnlySpan<byte> passwordHash)
    {
        var packet = new cPacket();
        packet.PushUInt16(nPass2.Certified);
        packet.PushBytes(passwordHash);
        Send(pPass2.Check, packet);
    }

    public void SendSkipSecondPass()
    {
        var packet = new cPacket();
        packet.PushUInt16(nPass2.Skiped);
        packet.PushBytes(new byte[33]);
        Send(pPass2.Check, packet);
    }

    protected override void OnPacket(NetworkPacketEnvelope packet)
    {
        switch (packet.PacketId)
        {
            case ushort.MaxValue:
                _ = packet.Packet.PopInt16();
                return;
            case pLogin.WaitTime:
                LoginWaitTimeReceived?.Invoke(packet.Packet.PopUInt32());
                return;
            case pLogin.Request:
                LoginResultReceived?.Invoke(new AccountLoginResult(packet.Packet.PopUInt32(), packet.Packet.PopByte()));
                return;
            case pGate.SvrInfo:
                GateInfoReceived?.Invoke(ReadGateInfo(packet.Packet));
                return;
            case pSvr.ClusterList:
                ClusterListReceived?.Invoke(ReadClusterList(packet.Packet));
                return;
            case pSvr.SelectCluster:
                SelectClusterReceived?.Invoke(packet.Packet.PopInt32());
                return;
            case pPass2.Check:
            case pPass2.Register:
            case pPass2.ChangePass:
                SecondPasswordReceived?.Invoke(new SecondPasswordResult(packet.PacketId, packet.Packet.PopUInt32()));
                return;
            case pLogin.AccountBan:
            case pLogin.Birthday:
            case pNet.VersionError:
            case pShutdown.WeekCheck:
            case pShutdown.WeekCheckError:
            case pCertify.IntegrityHash:
                UnknownPacketReceived?.Invoke(packet);
                return;
            default:
                UnknownPacketReceived?.Invoke(packet);
                return;
        }
    }

    private static IReadOnlyList<ServerInfo> ReadClusterList(cPacket packet)
    {
        byte count = packet.PopByte();
        var servers = new List<ServerInfo>(count);
        for (int i = 0; i < count; i++)
        {
            int index = packet.PopInt32();
            string name = packet.PopString(consumeTrailingNull: true);
            byte disable = packet.PopByte();
            byte clusterState = packet.PopByte();
            byte tamerNumber = packet.PopByte();
            bool isNewServer = packet.PopByte() != 0;

            var server = new ServerInfo(
                index,
                name,
                disable,
                clusterState,
                tamerNumber,
                isNewServer,
                pLimit.MaxTamerSlot,
                pLimit.OpenedTamerSlot);

            if (server.IsNewServer)
            {
                servers.Insert(0, server);
            }
            else
            {
                servers.Add(server);
            }
        }

        return servers;
    }

    private static GateConnectionInfo ReadGateInfo(cPacket packet)
    {
        uint accountIndex = packet.PopUInt32();
        uint accessCode = packet.PopUInt32();
        string host = packet.PopString(consumeTrailingNull: true);
        int port = packet.PopInt32();
        return new GateConnectionInfo(accountIndex, accessCode, host, port);
    }
}
