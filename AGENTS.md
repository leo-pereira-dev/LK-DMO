# AGENTS.md - LK-DMO Workspace Guide

## Scope

Work directly inside this workspace only:

```text
C:\0-NewDmo
```

Do not use paths outside this project as active references. If an old note
mentions a missing folder, deleted checkout, retired helper, or external legacy
path, treat it as obsolete.

## CodeGraph

For projects that contain `.codegraph/`, use the CodeGraph MCP tools for fast
code navigation before broad filesystem scans. Start with `codegraph_status`,
`codegraph_search`, `codegraph_files`, `codegraph_callers`,
`codegraph_callees`, `codegraph_impact`, or `codegraph_node` when they fit the
question, then use normal file reads to verify exact edits.

When using CodeGraph in this workspace, always pass the project path explicitly:

```text
C:\0-NewDmo
```

The current index is scoped to active code in this checkout:

```text
C:\0-NewDmo\dmo-client-main
C:\0-NewDmo\dmo-server-main-postgres\src\Source
```

Local build outputs, client distribution files, packs, decompiled dumps, vendor
SDKs, migrations, and generated artifacts are excluded to keep searches focused.

## Active Project Areas

Client source:

```text
C:\0-NewDmo\dmo-client-main
C:\0-NewDmo\dmo-client-main\DProject.sln
```

PostgreSQL server source:

```text
C:\0-NewDmo\dmo-server-main-postgres
C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln
```

Local runtime/client data folders:

```text
C:\0-NewDmo\ClientDist
C:\0-NewDmo\ClientDist1
C:\0-NewDmo\ClientDist1_x64
```

Local BIN and pack research/tools:

```text
C:\0-NewDmo\LkDmoBinConverter
C:\0-NewDmo\ItemEditor
C:\0-NewDmo\new bins
C:\0-NewDmo\TamerBinMigrationFeature
C:\0-NewDmo\SealMasterFeature
```

## Client Build Target

The default client build for this workspace is the English x64 bridge target,
not Korean release and not the legacy Win32 target. Always use this target
unless the user explicitly requests another locale or platform:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Build through `DProject.sln` only. Do not build `DProject.vcxproj` directly.
Do not use `Release_English|x64` as the default x64 build: that solution
configuration maps several projects back to Win32. The real x64 client target is
`X64_WinDX9_Bridge|x64`. The build emits:

```text
C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe
```

For runtime testing, copy the built executable to:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

## Bug Investigation Checklist

For DMO bugs, check these before proposing or implementing a fix:

- Packet mismatches
- Race conditions
- Null checks
- Game logic
- Pipelines
- Overflow

For client/server contract bugs, compare the local client source, local server
source, active local BIN data, and available local logs before deciding root
cause.

## VPS Deploy Automation

For VPS deploys from this machine, do not use plain `ssh`/`scp` automation
unless an SSH key has been configured first. The local OpenSSH client waits for
an interactive password prompt and can look like the deploy is hanging.

Use the password-based deploy wrapper instead:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
```

To validate credentials only:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -CheckConnection
```

To redeploy using the latest existing archive, which is faster for repeat test
deploys:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -ReuseLatestArchive
```

The wrapper reads the VPS password from this local-only file:

```text
C:\0-NewDmo\.codex-tmp\dmo-vps-password.txt
```

This file must stay untracked and must never be printed, committed, or copied
into docs. If this file is written by PowerShell, ensure it is saved without a
UTF-8 BOM; the wrapper also reads with BOM stripping because a BOM caused SSH
authentication failures on 2026-06-07.

The wrapper deploys only app services by default:

```text
account character game routine api admin
```

It intentionally does not deploy or recreate `postgres`, because production data
already lives on the VPS database.

Latest successful test deploy:

```text
Date: 2026-06-08
Command: powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
Archive: .codex-tmp\dmo-server-main-postgres-20260608-155153.tar.gz
Archive size: 91.7 MB
Total time: 84.1 seconds
Remote backup: /home/ubuntu/deploy-backups/dmo-server-main-postgres-20260608-185210
Result: account, character, game, routine, api, and admin containers rebuilt/recreated and Up
Postgres: not deployed/recreated; container created 2026-05-25, still healthy, restart count 0
Post-deploy logs: not rechecked for this deploy; deploy output showed all target containers Started/Up
```
