namespace DProject.Network;

public sealed record NetworkPacketEnvelope(
    string ConnectionName,
    ushort PacketId,
    cPacket Packet,
    long Sequence,
    DateTimeOffset ReceivedAt,
    bool ChecksumValid,
    ushort RawLength);
