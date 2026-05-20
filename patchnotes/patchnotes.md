# Patch Notes

## Current LK-DMO Workspace Update

### Server

- The workspace is now focused on the PostgreSQL server tree:
  `dmo-server-main-postgres/`.
- The old local MySQL server tree was retired from the active workspace so new
  server fixes target the PostgreSQL stack only.
- PostgreSQL Docker/service notes are maintained in `CONTEXT.MD`.

### Client Chat And Mojibake Fixes

- Fixed length-prefixed `char*` packet reads in `dmo-client-main/nlib/packet.cpp`
  so copied strings are always null-terminated.
- Cleared the local system-message buffer in `SyncMsgSystem()` before reading
  packet text.
- Blocked release-build debug chat from entering the visible chat UI.
- Removed several client debug chat emissions from monster, buff, and sync paths.
- Added chat diagnostics:
  - `Log/ChatRenderTrace_client.txt`
  - `Log/ChatVisibleTrace_client.txt`
  - `Log/TextRenderTrace_client.txt`
- Added a final low-level guard in `cString::sTEXT::Render()` and
  `RenderLimit()` for short mojibake strings. This catches patterns such as
  Cyrillic/Latin-extended mojibake, the Hangul artifact `U+CD2B`, and repeated
  comma-zero suffixes like `, 0`.
- The repeated malformed cyan/green chat text was confirmed stopped after the
  low-level render guard.

### Chat Korean/System Text Handling

- Added a small translation pass for known Korean system/debug chat phrases
  before they enter the chat UI.
- The source uses Unicode escape literals for those known phrases so the mixed
  legacy source encodings do not corrupt them at compile time.

### Packet/Protocol Notes

- Added `tutorial/packetprocessor.md` with a real client/server packet flow
  using megaphone chat as the example.
- The most important packet maintenance rule is unchanged: client write/read
  order and server read/write order must match byte-for-byte.
