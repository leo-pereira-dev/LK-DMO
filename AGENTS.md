# AGENTS.md - LK-DMO Workspace Guide

## CodeGraph

For projects that contain `.codegraph/`, use the CodeGraph MCP tools for fast
code navigation before broad filesystem scans. Start with `codegraph_status`,
`codegraph_search`, `codegraph_files`, `codegraph_callers`,
`codegraph_callees`, `codegraph_impact`, or `codegraph_node` when they fit the
question, then use normal file reads to verify exact edits.

In `C:\0-NewDmo`, the CodeGraph index is intentionally scoped to the active
LK-DMO code from `CONTEXT.MD`: the C++ client areas (`DProject`, `LibProj`,
`common_vs2019`, `nlib`) and the C# PostgreSQL server source
(`dmo-server-main-postgres/src/Source`). Local build outputs, client
distribution files, packs, decompiled dumps, vendor SDKs, tools, migrations,
and generated artifacts are excluded to keep searches focused.

## Client Build Target

The default client build for this workspace is English release, not Korean
release.

Always use this target unless the user explicitly requests another locale:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Build through `DProject.sln` only. Do not build `DProject.vcxproj` directly:
that bypasses solution-level configuration/dependency mapping and can trigger
false `Release_English|Win32` configuration or precompiled-header/PDB lock
errors. If parallel compilation hits `C1041`/PDB contention, keep using the
solution and add `/FS` through the environment for that command:

```powershell
$env:CL="/FS"
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Expected output:

```text
C:\Release\GSP\DProject_R_ENG.exe
```

Copy the built executable to:

```text
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```

Do not compile `Release|Win32` for normal testing. That target is Korean and
outputs `C:\Release\Kor\DProject_R.exe`.

If copying is blocked by a running client process, stop the blocking
`DProject_R_ENG.exe` process and retry the copy.
