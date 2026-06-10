using DProject.Network.Protocol;

namespace DProject.Network;

public sealed class cCliGate : cClient
{
    private GateConnectionInfo? _gateConnection;

    public cCliGate() : base(nameof(cCliGate))
    {
    }

    public event Action? ChangeServerReceived;
    public event Action<IReadOnlyList<CharacterSummary>>? CharacterListReceived;
    public event Action<CharacterSelectResult>? CharacterSelectReceived;
    public event Action<uint>? CheckDoubleNameReceived;
    public event Action<NetworkPacketEnvelope>? UnknownPacketReceived;

    public uint IsChangeServer { get; private set; }
    public int TamerEquipBytes { get; set; } = pLimit.TamerEquipBytesCompat487;

    public void ConfigureAccess(GateConnectionInfo gateConnection)
    {
        _gateConnection = gateConnection;
    }

    public void SendAccessCode()
    {
        if (_gateConnection is null)
        {
            return;
        }

        var packet = new cPacket();
        uint port = (uint)_gateConnection.Port;
        packet.PushUInt32((port ^ _gateConnection.AccessCode) | _gateConnection.AccountIndex);
        packet.PushUInt32(_gateConnection.AccountIndex);
        packet.PushUInt32(_gateConnection.AccessCode);
        Send(pSvr.AccessCode, packet);
    }

    public void SendChangeServer()
    {
        IsChangeServer++;
        Send(pSvr.Change, new cPacket());
    }

    public void SendSelect(uint slotNo)
    {
        var packet = new cPacket();
        packet.PushUInt32(slotNo);
        Send(pTamer.Select, packet);
    }

    protected override void OnPacket(NetworkPacketEnvelope packet)
    {
        switch (packet.PacketId)
        {
            case ushort.MaxValue:
                _ = packet.Packet.PopInt16();
                return;
            case pSvr.Change:
                ChangeServerReceived?.Invoke();
                return;
            case pTamer.List:
                CharacterListReceived?.Invoke(ReadCharacterList(packet.Packet));
                return;
            case pTamer.SelectSuccess:
                CharacterSelectReceived?.Invoke(ReadSelectSuccess(packet.Packet));
                return;
            case pTamer.SelectFailure:
                CharacterSelectReceived?.Invoke(new CharacterSelectResult(
                    false,
                    string.Empty,
                    0,
                    0,
                    packet.Packet.PopUInt32()));
                return;
            case pTamer.CheckDoubleName:
                CheckDoubleNameReceived?.Invoke(packet.Packet.PopUInt32());
                return;
            case pTamer.CreateSuccess:
            case pTamer.CreateFailure:
            case pTamer.CreateWaitTime:
            case pTamer.Remove:
            case pMaster.BanUser:
            case pMaster.DiscUser:
                UnknownPacketReceived?.Invoke(packet);
                return;
            default:
                UnknownPacketReceived?.Invoke(packet);
                return;
        }
    }

    private IReadOnlyList<CharacterSummary> ReadCharacterList(cPacket packet)
    {
        var characters = new List<CharacterSummary>();
        sbyte slotNo = unchecked((sbyte)packet.PopByte());
        while (slotNo != 99)
        {
            short mapId = packet.PopInt16();
            int tamerObjectTableId = packet.PopInt32();
            byte tamerLevel = packet.PopByte();
            string tamerName = packet.PopString(consumeTrailingNull: true);
            _ = packet.PopBytes(TamerEquipBytes);
            uint digimonObjectTableId = packet.PopUInt32();
            byte digimonLevel = packet.PopByte();
            string digimonName = packet.PopString(consumeTrailingNull: true);
            short digimonScale = packet.PopInt16();
            ushort patLeader = packet.PopUInt16();
            int relocateState = packet.PopInt32();

            characters.Add(new CharacterSummary(
                slotNo,
                mapId,
                tamerObjectTableId,
                tamerLevel,
                tamerName,
                digimonObjectTableId,
                digimonLevel,
                digimonName,
                digimonScale,
                patLeader,
                relocateState));

            slotNo = unchecked((sbyte)packet.PopByte());
        }

        return characters;
    }

    private static CharacterSelectResult ReadSelectSuccess(cPacket packet)
    {
        string host = packet.PopString(consumeTrailingNull: true);
        int port = packet.PopInt32();
        uint mapId = packet.PopUInt32();
        return new CharacterSelectResult(true, host, port, mapId, 0);
    }
}
