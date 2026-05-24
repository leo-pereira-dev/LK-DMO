using DigitalWorldOnline.Commons.Enums.ClientEnums;
using DigitalWorldOnline.Commons.Models.Base;
using DigitalWorldOnline.Commons.Packets.GameServer;
using DigitalWorldOnline.Game.Tests.Support;
using Xunit;
using Xunit.Abstractions;

namespace DigitalWorldOnline.Game.Tests.Transcendence;

public sealed class TranscendenceReceiveExpPacketTests
{
    private const int PacketNumber = 16039;
    private const int SerializedItemSize = 68;

    private readonly ITestOutputHelper _output;

    public TranscendenceReceiveExpPacketTests(ITestOutputHelper output)
    {
        _output = output;
    }

    [Fact]
    public void Success_payload_matches_v487_client_read_order()
    {
        var updatedItem = new ItemModel
        {
            ItemId = 97303,
            Amount = 2,
            Power = 100,
            RerollLeft = 1
        };

        var packet = new DigimonTranscendenceReceiveExpPacket(
            AcademyInputType.High,
            targetSlot: 0,
            digimonCount: 2,
            targetDeleteSlots: new short[] { 0, 3 },
            updatedItems: new[] { new KeyValuePair<short, ItemModel>(26, updatedItem) },
            successRate: 1,
            chargeExp: 140000,
            targetPartnerFinalExp: 140000).Serialize();

        var cursor = new PacketCursor(packet);

        Assert.Equal(packet.Length, cursor.ReadInt16());
        Assert.Equal(PacketNumber, cursor.ReadInt16());
        Assert.Equal(0, cursor.ReadInt32());
        Assert.Equal((byte)AcademyInputType.High, cursor.ReadByte());
        Assert.Equal(0, cursor.ReadByte());
        Assert.Equal(2, cursor.ReadUInt16());
        Assert.Equal(0, cursor.ReadUInt16());
        Assert.Equal(3, cursor.ReadUInt16());
        Assert.Equal(1, cursor.ReadUInt16());
        Assert.Equal(26, cursor.ReadUInt16());
        Assert.Equal(0, cursor.ReadUInt16());

        var itemStart = cursor.Offset;
        cursor.Skip(SerializedItemSize);

        _output.WriteLine(
            $"16039 layout: itemStart={itemStart}, successRateOffset={cursor.Offset}, packetLength={packet.Length}");

        Assert.Equal(1, cursor.ReadUInt16());
        Assert.Equal(140000, cursor.ReadInt64());
        Assert.Equal(140000, cursor.ReadInt64());
        Assert.Equal(packet.Length - sizeof(short), cursor.Offset);
    }

    [Fact]
    public void Failure_payload_contains_only_result_after_packet_header()
    {
        const int resultCode = 20109;

        var packet = new DigimonTranscendenceReceiveExpPacket(resultCode).Serialize();
        var cursor = new PacketCursor(packet);

        Assert.Equal(packet.Length, cursor.ReadInt16());
        Assert.Equal(PacketNumber, cursor.ReadInt16());
        Assert.Equal(resultCode, cursor.ReadInt32());
        Assert.Equal(packet.Length - sizeof(short), cursor.Offset);
    }
}
