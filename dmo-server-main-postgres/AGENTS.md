# AGENTS.md - DMO Server Work Guide

## Scope

Work from this server repository only:

```text
C:\0-NewDmo\dmo-server-main-postgres
```

Do not use historical external paths or removed workspace files as active
references. Keep investigations and edits inside `C:\0-NewDmo` unless the user
explicitly provides a current local path inside this workspace.

## Local Source

Main solution:

```text
C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln
```

Runtime hosts live under:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution
```

Domain/application/infra source lives under:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Infra
```

Local static BIN data:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
```

## Bug Investigation Rules

For every DMO bug, explicitly check:

- Packet mismatches
- Race conditions
- Null checks
- Game logic
- Pipelines
- Overflow

When the issue involves client/server contracts, packets, loading, map
transitions, UI rendering, movement, mobs, or crashes, compare local server
packet processors, server packet writers, client source, active local BIN data,
and local logs before deciding root cause.

## Current Incident Notes

Recent work focused on players not seeing each other, map/combat freezes, crash
reports, bad stats, and account login disconnects.

High-priority files from that work:

- `src/Source/Distribution/DigitalWorldOnline.Game.Host/MapServers/MapServerTamerOperation.cs`
- `src/Source/Domain/DigitalWorldOnline.Commons/Models/Maps/MapInstanceTamerBehavior.cs`
- `src/Source/Domain/DigitalWorldOnline.Commons/Packets/GameServer/UpdateStatusPacket.cs`
- `src/Source/Domain/DigitalWorldOnline.Commons/Packets/CharacterServer/CharacterListPacket.cs`
- `src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/InitialInformationPacketProcessor.cs`

Known findings:

- `ShowTamer`/visibility was hardened to avoid missing `TamersView` entries and
  duplicate view data.
- `LoadTamerPacket` and `LoadBuffsPacket` were serialized before send and
  traced with `PKT-TRACE ShowTamer...` logs.
- Partner AT in `UpdateStatusPacket` must be written as `int32` to match the
  current client `RecvAllStat` reader. Writing it as `short` caused huge AT
  values such as `761680497`.
- Character list code must not call `character.Partner` for tamers with zero
  digimons; use `character.Digimons.Any()` first.
- For BIN sync issues, compare server BINs against local client/runtime BIN or
  pack sources under `C:\0-NewDmo`.

## Process Handling

If build, copy, deploy, or validation is blocked because local DMO
servers/processes are running and locking files, stop the blocking local
processes and continue instead of asking the user to do it.

## VPS Test Deploy

Use the root deploy wrapper for VPS test deploys:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
```

For repeat deploys using the latest already-created archive:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -ReuseLatestArchive
```

Check credentials only:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -CheckConnection
```

The wrapper reads the local-only password file:

```text
C:\0-NewDmo\.codex-tmp\dmo-vps-password.txt
```

Keep this file untracked and do not print or commit it. The wrapper strips a
UTF-8 BOM from the password because a BOM in this file caused SSH authentication
failure during setup.

The deploy wrapper targets only app services:

```text
account character game routine api admin
```

Do not deploy or recreate `postgres`; the production database already exists on
the VPS.

Latest successful test deploy:

```text
Date: 2026-06-08
Command: powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
Archive: .codex-tmp\dmo-server-main-postgres-20260608-145315.tar.gz
Archive size: 91.7 MB
Total time: 84.4 seconds
Remote backup: /home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-175334
Result: app containers rebuilt/recreated and Up
Postgres: unchanged; container created 2026-05-25, still healthy, restart count 0
Post-deploy logs: no recent ERR/Exception/DI failure/duplicate-key/ItemMove-rejected matches in checked app logs; only known DUnit XML warnings
```
