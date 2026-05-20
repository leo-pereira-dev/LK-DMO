# Packet Processor Tutorial

This document explains the current server/client packet flow using one real,
small example from this repo: megaphone chat.

The golden rule is simple: the side that writes bytes and the side that reads
bytes must agree on packet id, field order, field size, string format, and
terminator bytes. Most hard bugs in this project came from one side reading a
different layout than the other side wrote.

## Main Pieces

### Client C++

Relevant files:

- `dmo-client-main/common_vs2019/pGame.h`
- `dmo-client-main/common_vs2019/pSync.h`
- `dmo-client-main/DProject/network/cCliGameSend.cpp`
- `dmo-client-main/DProject/network/cCliGameSync.cpp`
- `dmo-client-main/nlib/packet.cpp`

The client usually builds an outbound packet with this shape:

```cpp
newp(PacketId);
    push(field1);
    push(field2);
endp(PacketId);
send();
```

Inbound packets are read with `pop(...)` in the exact order expected by the
client handler.

### Server C#

Relevant files:

- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/GamePacketReader.cs`
- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/GamePacketProcessor.cs`
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Enums/PacketProcessor/GameServerPacketEnum.cs`
- `dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/*.cs`
- `dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/**/*.cs`

The game server reads the packet header with `GamePacketReader`, maps the
numeric packet id to `GameServerPacketEnum`, then finds an
`IGamePacketProcessor` whose `Type` matches that enum.

In `GamePacketProcessor.cs`:

```csharp
var packet = new GamePacketReader(data);
var processor = _packetProcessors.FirstOrDefault(x => x.Type == packet.Enum);
await processor.Process(client, data);
```

## Packet Header

For game packets, `GamePacketReader` reads:

```csharp
Length = ReadShort();
Type = ReadShort();
```

Then it validates the checksum at the end and seeks back to payload position 4.
That means a processor reads only the payload after the 2-byte length and
2-byte packet type.

## Real Example: Client Sends Megaphone

The client sends a global megaphone from
`dmo-client-main/DProject/network/cCliGameSend.cpp`:

```cpp
void cCliGame::SendMsgAllByItem(wchar *msg, n4 nItemSlotNo)
{
    nsCsFileTable::g_pCuidMng->CheckChat( msg );

    newp(pGame::MsgAllByItem);
        push( msg );
        push(nItemSlotNo);
    endp(pGame::MsgAllByItem);
    send();
}
```

The protocol id comes from `pGame::MsgAllByItem` in
`dmo-client-main/common_vs2019/pGame.h`. On the server this is mapped as:

```csharp
MegaphoneMessage = 1074
```

inside `GameServerPacketEnum.cs`.

So the outbound payload order is:

1. message string
2. inventory slot as integer

## Server Receives Megaphone

The processor lives at:

`dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/MegaphoneMessagePacketProcessor.cs`

It declares which packet id it handles:

```csharp
public GameServerPacketEnum Type => GameServerPacketEnum.MegaphoneMessage;
```

Then it reads the payload in the same order the client wrote it:

```csharp
var packet = new GamePacketReader(packetData);
var message = NormalizeClientText(packet.ReadString());
var slot = packet.ReadInt();
```

After validation, it broadcasts a server-to-client chat packet:

```csharp
_mapServer.BroadcastGlobal(
    new ChatMessagePacket(
        message,
        ChatTypeEnum.Megaphone,
        client.Tamer.Name,
        inventoryItem.ItemId,
        client.Tamer.Level
    ).Serialize()
);
```

It also removes or reduces the consumed item from inventory when the item is not
temporary.

## Server Broadcasts Chat Back To Clients

The packet writer is:

`dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/Chat/ChatMessagePacket.cs`

For megaphone, it writes packet type `1006`:

```csharp
private const int PacketNumber = 1006;
```

Then it writes the sync subtype and payload:

```csharp
Type(PacketNumber);
WriteByte((byte)chatType);
WriteByte(1);

WriteString(senderName);
WriteString(message);
WriteInt(itemId);
WriteByte(senderLevel);
WriteByte(0);
```

For megaphone, `chatType` is `ChatTypeEnum.Megaphone`.

The `WriteByte(1)` is the count of chat entries in this sync block. The final
`WriteByte(0)` ends the sync packet sequence for the client loop.

## Client Receives Broadcast

The client routes packet `1006` as a sync packet. In
`dmo-client-main/DProject/network/cCliGameSync.cpp`, the sync loop reads a sync
type byte and dispatches:

```cpp
case pSync::MsgAllByItem:
    SyncMsgAllByItem();
    break;
```

`SyncMsgAllByItem()` then reads the payload in the order written by
`ChatMessagePacket`:

```cpp
uchar cnt = 0;
pop(cnt);

for (int i = 0; i < cnt; i++)
{
    std::wstring src;
    pop(src);

    std::wstring msg;
    pop(msg);

    n4 nItemType = 0;
    pop(nItemType);

    u1 nTamerLevel = 0;
    pop(nTamerLevel);
}
```

Then it formats the message and emits `EVENT_CHAT_PROCESS`, which eventually
adds the text to the chat UI.

## Common Failure Pattern

If the server writes:

```text
message, slot
```

but the server processor reads:

```text
message, terminator, slot
```

then the read cursor moves one byte too far and the slot becomes wrong. The same
thing happens in the other direction: if the server writes an extra byte or the
client forgets a terminator, later fields are shifted and the client may display
garbled strings, wrong item ids, or repeated chat artifacts.

## Checklist For Adding A Packet

1. Pick or confirm the numeric packet id in the client enum.
2. Add or confirm the matching server enum value.
3. Write the client payload order.
4. Read the server payload in the exact same order.
5. If the server answers, create a packet writer that matches the client read
   order.
6. Find the client receive handler and confirm each `pop(...)` matches the
   server writer.
7. Log packet id, payload length, reader position, and raw bytes while debugging.
8. Remove noisy logs after the packet is stable, or keep them behind targeted
   diagnostics.
