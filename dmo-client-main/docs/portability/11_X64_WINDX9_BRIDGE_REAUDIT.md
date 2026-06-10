# Reauditoria Windows x64 - X64_WinDX9_Bridge

Data: 2026-06-07  
Workspace principal: `C:\dmo-client-main`  
Source Gamebryo candidata: `D:\Gamebryo2.3`  
Escopo: avaliar viabilidade de build Windows x64 mantendo DX9/Gamebryo 2.3, sem migrar renderer, protocolo ou logica de jogo.

## Resposta curta

Da para portar para x64, mas **nao da para compilar x64 completo imediatamente do jeito que a solucao esta hoje**.

A conclusao anterior "Gamebryo bloqueia x64" precisa ser refinada: os binarios Gamebryo disponiveis continuam x86, mas `D:\Gamebryo2.3` contem source e projetos `.vcxproj` modernos suficientes para tentar uma recompilacao x64. O bloqueio real nao e mais "nao existe source"; o bloqueio real e "nao existe configuracao x64 pronta nem libs x64 confiaveis para toda a cadeia".

O caminho correto e criar uma configuracao separada `X64_WinDX9_Bridge|x64`, recompilar libs internas e Gamebryo em x64, substituir DirectX June 2010 x86 por Windows SDK x64 + NuGet oficial de D3DX, e decidir Miles como dependencia confiavel ou stub temporario.

## O que nao foi alterado

- Renderer nao foi migrado.
- Gamebryo nao foi trocado.
- Protocolo de rede nao foi alterado.
- Logica de jogo nao foi alterada.
- Nenhuma lib x86 foi forcada dentro de build x64.
- Nenhuma dependencia aleatoria da internet foi aceita como solucao limpa.
- Nenhuma correcao massiva de codigo foi aplicada nesta reauditoria.

## Ambiente usado

| Item | Valor |
|---|---|
| Shell | PowerShell |
| MSBuild x64 | `C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe` |
| Toolset observado | `v143` |
| Dumpbin encontrado | `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.29.30133\bin\HostX64\x64\dumpbin.exe` |
| Solucao do client | `C:\dmo-client-main\DProject.sln` |
| Configuracao existente testada antes | `Release_English|x64` |
| Logs existentes | `docs\portability\msbuild_release_english_x64.log`, `docs\portability\msbuild_release_english_x64_after_config_fix.log` |

Comando de build usado na auditoria anterior:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=Release_English `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=docs\portability\msbuild_release_english_x64_after_config_fix.log;Verbosity=Normal"
```

Probe da source Gamebryo candidata:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "D:\Gamebryo2.3\Build\Win32\VC80\Core01_CoreLibs.sln" `
  /m `
  /p:Configuration=Release `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_core01_release_x64_probe.log;Verbosity=Normal"
```

Resultado do probe Gamebryo: `MSB4126`, porque `Release|x64` nao existe nessa solucao.

## 1. Sistema de build atual

O sistema atual e Visual Studio/MSBuild com `DProject.sln` e varios `.vcxproj`.

Problema central: `Release_English|x64` existe na solucao, mas e um x64 falso/misto. Quase todos os projetos continuam mapeados para `Win32`; apenas `nlib` usa uma configuracao x64 real.

| Projeto | Tem x64 real hoje? | Mapeamento em `Release_English|x64` | Problema |
|---|---:|---|---|
| `DProject\DProject.vcxproj` | Nao | `Release_English|Win32` | Main exe ainda `Win32`, `MachineX86`. |
| `common_vs2019\common.vcxproj` | Nao | `Release_DLL|Win32` | Lib/protocolo ainda Win32. |
| `LibProj\CsFunc\CsFunc.vcxproj` | Nao | `Release_MT|Win32` | Win32 e `/MT`, conflita com DProject `/MD`. |
| `LibProj\CsThread\CsThread.vcxproj` | Nao | `Release_MT|Win32` | Win32 e `/MT`. |
| `LibProj\CsFilePack\CsFilePack.vcxproj` | Nao | `Release_MT|Win32` | Win32 e `/MT`. |
| `LibProj\CsDM\CsDM.vcxproj` | Nao | `Release_MT|Win32` | Win32 e `/MT`. |
| `LibProj\CsFileTable\CsFileTable.vcxproj` | Nao | `Release_English|Win32` | Win32. |
| `LibProj\CsGamebryo2.3\CsGamebryo2.3.vcxproj` | Nao | `Release_English|Win32` | Win32, usa Gamebryo/DX x86. |
| `LibProj\CsGBChar\CsGBChar.vcxproj` | Nao | `Release_English|Win32` | Win32, usa Gamebryo/DX x86. |
| `LibProj\JsonLib\lib_json.vcxproj` | Nao | `Debug|Win32` | Mistura Debug com Release. |
| `nlib\nlib.vcxproj` | Sim | `Release|x64` | Unico projeto x64 real no build atual. |

Erros observados no build atual:

| Erro | Classe | Evidencia |
|---|---|---|
| `LNK2038 RuntimeLibrary MT_StaticRelease vs MD_DynamicRelease` | erro de configuracao | `CsDM_mt.lib`, `CsFilePack_mt.lib`, `CsFunc_mt.lib`, `CsThread_mt.lib` contra objetos `/MD`. |
| `LNK2038 _ITERATOR_DEBUG_LEVEL 2 vs 0` | erro de configuracao | `json_vc71_libmtd.lib` vindo de `Debug|Win32`. |
| `fatal error C1905: Front end and back end not compatible` | erro de 32->64/configuracao | Mistura de objetos/libs Win32 com parte x64. |
| `fatal error LNK1257: code generation failed` | consequencia | Falha final do link/LTCG apos mistura incoerente. |

## 2. Matriz x64 de dependencias

| Componente | Estado x64 local | Acao necessaria |
|---|---|---|
| `DProject` | Nao tem config x64 real. | Criar `X64_WinDX9_Bridge|x64`, `MachineX64`, saida separada. |
| `common_vs2019` | Nao tem config x64 real. | Criar config x64 mantendo protocolo/packing intocado. |
| `nlib` | Tem `Release|x64`. | Integrar em config nova e validar runtime library. |
| `CsFunc`, `CsThread`, `CsDM`, `CsFilePack` | Win32 somente; build atual usa `/MT`. | Criar x64 `/MD`, saida separada, sem misturar `*_mt.lib`. |
| `CsFileTable`, `CsGamebryo2.3`, `CsGBChar` | Win32 somente. | Criar x64 apos Gamebryo x64 existir. |
| `JsonLib` | Win32 somente; mapeado errado para Debug. | Criar `Release|x64` ou `X64_WinDX9_Bridge|x64` `/MD`. |
| Gamebryo `Ni*.lib` em `C:\dmo-client-main\Gamebryo2.3\GB_Lib\ReleaseLib` | x86. | Nao usar em x64; recompilar da source. |
| Gamebryo source em `D:\Gamebryo2.3` | Source/projetos existem, mas configs x64 nao. | Criar configs x64 e reconstruir cadeia. |
| `zlib.lib`, `libpng.lib`, `freetype213.lib` prebuilt | x86. | Rebuild x64; zlib/libpng tem source local, freetype precisa source/vcpkg/pacote confiavel. |
| DirectX core (`d3d9`, `dinput8`, `dxguid`, `ddraw`) | Windows SDK x64 existe. | Usar libs oficiais do Windows SDK x64. |
| D3DX9 | NuGet oficial existe localmente. | Usar `Microsoft.DXSDK.D3DX.9.29.952.8`. |
| DirectSetup / `dsetup.lib` | Nao encontrado no Windows SDK x64. | Remover/condicionar chamada ou usar alternativa oficial; nao bloquear renderer. |
| Miles `mss32.lib`/`mss32.dll` | x86. | Nao serve para processo x64. |
| Miles `D:\Gamebryo2.3\X64\mss64.dll` | PE x64, nao assinado, origem nao confiavel. | Validar licenca/origem ou usar stub de audio para x64 bridge. |

## 3. Gamebryo 2.3 source x64

Arvores avaliadas:

| Arvore | Resultado |
|---|---|
| `C:\dmo-client-main\Gamebryo2.3` | Headers/libs/third-party; nao e source completa do engine. `GB_Lib\ReleaseLib` e x86. |
| `C:\dmo-client-main\Gamebryo_2.3` | Source antiga com `.vcproj`/VS2005, libs x86, pouca utilidade imediata para x64. |
| `D:\Gamebryo2.3` | Melhor candidata: source completa, `.vcxproj` v143, CodeGraph indexado, mas sem configs x64 prontas. |

Projetos relevantes localizados em `D:\Gamebryo2.3`:

- `CoreLibs\NiSystem\Win32\VC80\NiSystem.vcxproj`
- `CoreLibs\NiMain\Win32\VC80\NiMain.vcxproj`
- `CoreLibs\NiAnimation\Win32\VC80\NiAnimation.vcxproj`
- `CoreLibs\NiDX9Renderer\Win32\VC80\NiDX9Renderer.vcxproj`
- `CoreLibs\NiD3D10Renderer\Win32\VC80\NiD3D10Renderer.vcxproj`
- `CoreLibs\NiMilesAudio\Win32\VC80\NiMilesAudio.vcxproj`
- `AppFrameworks\UtilityLibs\NiInput\Win32\VC80\NiInput.vcxproj`

Conclusao revisada:

- Gamebryo **nao e mais um bloqueio absoluto**, porque ha source utilizavel em `D:\Gamebryo2.3`.
- Gamebryo **ainda e o maior bloqueador pratico**, porque todas as libs disponiveis continuam x86 e as solucoes/projetos nao tem `x64` pronto.
- O primeiro trabalho real e criar configs x64 preservando Win32 e reconstruir `NiSystem`, `NiMain`, `NiAnimation`, `NiDX9Renderer`, `NiInput`, `NiMilesAudio` ou um substituto/stub para audio.

Plano minimo para Gamebryo:

1. Copiar configs `Release|Win32` para uma config nova `Release_X64|x64` ou `X64_WinDX9_Bridge|x64`.
2. Preservar configs Win32 intactas.
3. Trocar `TargetMachine` para `MachineX64`.
4. Usar `/MD` em Release x64 para combinar com o client.
5. Redirecionar output para `D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib` ou outro diretorio x64 separado.
6. Trocar paths `DirectX_June2010\Lib\x86` por Windows SDK x64 + D3DX NuGet.
7. Rebuildar terceiro-party x64 antes de linkar engine.

## 4. DirectX/D3DX oficial para DX9 x64

O plano DX9 x64 deve usar:

| Biblioteca | Fonte recomendada | Observacao |
|---|---|---|
| `d3d9.lib` | Windows SDK x64 | Encontrado em `C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64`. |
| `dinput8.lib` | Windows SDK x64 | Usar SDK x64, nao `DirectX_June2010\Lib\x86`. |
| `dxguid.lib` | Windows SDK x64 | Usar SDK x64. |
| `ddraw.lib` | Windows SDK x64 | Usar SDK x64 se algum modulo legado exigir. |
| `d3dx9.lib` | NuGet `Microsoft.DXSDK.D3DX.9.29.952.8` | Pacote oficial para D3DX legado. |
| `D3DX9_43.dll` | NuGet `Microsoft.DXSDK.D3DX.9.29.952.8` | Copiar para runtime x64. |
| `D3DCompiler_43.dll` | NuGet `Microsoft.DXSDK.D3DX.9.29.952.8` | Copiar para runtime x64 se exigido pelo pacote. |
| `dsetup.lib` | Problema | Nao encontrado no Windows SDK x64; chamadas DirectSetup devem ser removidas/condicionadas na config x64. |

Pacote D3DX ja localizado:

```text
C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\include\d3dx9.h
C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\lib\x64\d3dx9.lib
C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\D3DX9_43.dll
C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\D3DCompiler_43.dll
```

Projetos que provavelmente precisam trocar paths:

- `DProject\DProject.vcxproj`
- `LibProj\CsGamebryo2.3\CsGamebryo2.3.vcxproj`
- `LibProj\CsGBChar\CsGBChar.vcxproj`
- `D:\Gamebryo2.3\CoreLibs\NiDX9Renderer\Win32\VC80\NiDX9Renderer.vcxproj`
- outros projetos Gamebryo que referenciem `DirectX_June2010`.

Pontos de codigo DirectSetup:

- `DProject\Engine.cpp`: `#pragma comment(lib, "dsetup.lib")`
- `DProject\Engine.cpp`: `DirectXSetupGetVersion`
- `DProject\App\DXVersionChecker.cpp`: `DirectXSetupGetVersion`
- `DProject\App\Win32App.cpp`: chamada de instalador DXSETUP.

Classificacao: erro de API antiga/include-lib. Para x64 bridge, a opcao minima e condicionar esse checker/installer em `_WIN64` e assumir runtime via Windows SDK/NuGet, sem mudar renderer.

## 5. Miles x64

Arquivo informado pelo usuario:

```text
D:\Gamebryo2.3\X64\mss64.dll
```

Validacao tecnica local:

```text
Machine: 8664 machine (x64)
Dependents: USER32.dll, KERNEL32.dll, WINMM.dll
Exports: 340 functions / 340 names
Authenticode: NotSigned
```

Tambem foram gerados artefatos de laboratorio em fase anterior:

```text
C:\dmo-client-main\LibProj\MilesSoundX64\bin\mss64.dll
C:\dmo-client-main\LibProj\MilesSoundX64\lib\mss64.def
C:\dmo-client-main\LibProj\MilesSoundX64\lib\mss64.lib
```

Risco: `D:\Gamebryo2.3\X64\readme.txt` apontou origem `www.opendll.com`. Isso nao e aceitavel como dependencia limpa de build/producao sem confirmacao de licenca/origem. O fato de ser PE x64 nao resolve o risco juridico nem a confiabilidade binaria.

Decisao recomendada:

| Opcao | Uso | Status |
|---|---|---|
| Usar `mss64.dll` local | Laboratorio/prova tecnica apenas | Possivel, mas arriscado. |
| Conseguir Miles x64 oficial/RAD compativel | Build limpo | Melhor solucao. |
| Criar `IAudioSystem` + `MilesAudioSystem` + `NullAudioSystem` | Bridge x64 sem audio temporariamente | Recomendado se Miles confiavel nao existir. |

Stub minimo sugerido:

- `MilesAudioSystem`: caminho atual Win32/Miles normal.
- `NullAudioSystem`: inicializa sem audio e retorna sucesso controlado.
- Define de build: `DMO_X64_NULL_AUDIO` ou equivalente.

Essa mudanca e comportamento visivel (cliente sem audio), entao deve ser marcada como bridge temporario, nao como correcao silenciosa.

## 6. Terceiro-party x64

| Lib | Estado atual | Plano |
|---|---|---|
| `zlib` | Prebuilt em Gamebryo e x86; source local existe. | Rebuild x64 da source em `D:\Gamebryo2.3\ThirdPartyCode\ZLib`. |
| `libpng` | Prebuilt x86; source local existe. | Rebuild x64 da source em `D:\Gamebryo2.3\ThirdPartyCode\LibPNG`. |
| `freetype213` | Prebuilt x86; source nao confirmada localmente no mesmo nivel. | Localizar source original ou usar vcpkg/source confiavel, mantendo ABI esperada. |
| Miles | x86 oficial atual; `mss64.dll` local nao assinado. | Oficializar x64 ou stub. |

Nada disso deve usar libs x86 no link x64.

## 7. Riscos de codigo 32->64 encontrados

Esses pontos devem ser corrigidos somente depois que a configuracao x64 real existir, ou em pequenos lotes revisaveis.

| Padrao | Arquivo/local | Classe | Acao minima |
|---|---|---|---|
| `__asm { int 3 }` | `LibProj\CsFunc\assert_cs.h` | erro de 32->64 | Usar `__debugbreak()` sob `_WIN64`. |
| `__asm int 3` | `LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp` | erro de 32->64 | `__debugbreak()` sob `_WIN64`. |
| `__asm int 3` | `DProject\network\cCliGameSend.cpp` | erro de 32->64 | `__debugbreak()` sob `_WIN64`. |
| Inline asm fence | `nlib\asio\detail\win_fenced_block.hpp` | dependencia x86/API antiga | Validar guards do Asio; se ativo, substituir por intrinsecos/fences. |
| `(DWORD)ponteiro` em `SendMessage` | `LibProj\CsFunc\CsFile.cpp` | erro de 32->64 | Usar `LPARAM`/`reinterpret_cast<LPARAM>`. |
| `reinterpret_cast<DWORD&>(pArg->pArg)` | `LibProj\CsThread\Utility\CFJobQueueEx.cpp` | erro de 32->64 | Revisar semantica; usar `uintptr_t`/`DWORD_PTR` se for id/ponteiro. |
| `SetWindowLong(..., (LONG)proc)` | `DProject\_Interface\Game\ItemSeparate.cpp` | erro de 32->64/API antiga | Usar `SetWindowLongPtr`/`GWLP_WNDPROC`/`LONG_PTR`. |
| `SetWindowLong(..., (LONG)proc)` | `DProject\_Interface\Game\MoneySeparate.cpp` | erro de 32->64/API antiga | Mesmo ajuste. |
| `SetWindowLong(..., (LONG)proc)` | `DProject\_Interface\Game\ChatContents.cpp` | erro de 32->64/API antiga | Mesmo ajuste. |
| `GetWindowLong`/`SetWindowLong` estilo | `DProject\App\Win32App.cpp`, `DProject\Engine.cpp`, `DProject\Splash.cpp` | API antiga | Usar `GetWindowLongPtr`/`SetWindowLongPtr` em x64. |
| `SetClassLong` com cursor/handle | `DProject\CCursor.cpp` | erro de 32->64/API antiga | Usar `SetClassLongPtr`. |
| `#pragma pack` em structs de protocolo/bin | `common_vs2019`, `LibProj`, `DProject` | risco protocolo/serializacao | Auditar sem alterar layout; nao mexer em protocolo em massa. |
| Uso amplo de `WIN32` | varios projetos | risco de config | Nao remover automaticamente; diferenciar plataforma Windows de arquitetura. |

Observacao critica: qualquer struct serializada em rede/binario deve preservar layout. Corrigir x64 nao pode mudar pacote, `sizeof` de payload nem packing sem comparacao com server/bin/dump.

## 8. Configuracao nova recomendada

Nome:

```text
X64_WinDX9_Bridge|x64
```

Regras:

- Nao alterar `Release_English|Win32`.
- Nao converter projeto inteiro.
- Criar config separada em cada `.vcxproj` necessario.
- Usar saidas separadas:

```text
C:\dmo-client-main\Lib\x64\X64_WinDX9_Bridge\
C:\dmo-client-main\Bin\x64\X64_WinDX9_Bridge\
D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib\
```

Config padrao recomendada para Release x64:

| Campo | Valor |
|---|---|
| Platform | `x64` |
| Configuration | `X64_WinDX9_Bridge` |
| RuntimeLibrary | `MultiThreadedDLL` (`/MD`) |
| TargetMachine | `MachineX64` |
| WholeProgramOptimization/LTCG | Desativar no primeiro bring-up se atrapalhar diagnostico; reativar depois. |
| DirectX include | D3DX NuGet include + Windows SDK default. |
| DirectX lib | Windows SDK x64 + D3DX NuGet x64. |
| Gamebryo lib | Somente diretorio x64 recompilado. |
| Miles | Oficial x64 ou stub controlado. |

Defines possiveis:

```text
DMO_X64_WINDX9_BRIDGE
DMO_X64_NULL_AUDIO
```

Nao usar define para mascarar erro de arquitetura. `WIN32` pode precisar continuar existindo em alguns projetos como "Windows platform", mas `_WIN64` deve guiar decisoes de ponteiro/API.

## 9. Ordem de build recomendada

1. Rebuild third-party x64: `zlib`, `libpng`, `freetype`.
2. Rebuild `nlib` x64 e confirmar runtime.
3. Rebuild `JsonLib` x64 Release.
4. Rebuild libs internas basicas x64: `CsFunc`, `CsThread`, `CsDM`, `CsFilePack`.
5. Rebuild `common_vs2019` x64 sem alterar protocolo.
6. Rebuild Gamebryo core x64: `NiSystem`, `NiMain`, `NiAnimation`.
7. Rebuild Gamebryo renderer/input x64: `NiDX9Renderer`, `NiInput`.
8. Rebuild Gamebryo audio x64: `NiMilesAudio` com Miles confiavel ou variant/stub.
9. Rebuild libs dependentes do engine: `CsFileTable`, `CsGamebryo2.3`, `CsGBChar`.
10. Linkar `DProject` x64.
11. Copiar runtime x64: `D3DX9_43.dll`, `D3DCompiler_43.dll`, Miles oficial ou stub, assets normais.
12. Rodar client x64 ainda com DX9/Gamebryo.

## 10. Erros e bloqueadores reais

| Bloqueador | Classe | Severidade | Comentario |
|---|---|---:|---|
| `Release_English|x64` fake/misto | erro de configuracao | Alta | Nao usar como base direta; criar config nova. |
| Gamebryo libs atuais x86 | erro de dependencia | Alta | Rebuild de source e obrigatorio. |
| Gamebryo source sem config x64 | erro de configuracao | Alta | Trabalho principal da fase seguinte. |
| DirectX June 2010 `Lib\x86` | erro de include/lib | Alta | Trocar por Windows SDK x64 + NuGet D3DX. |
| `dsetup.lib`/DirectSetup | erro de API antiga/include-lib | Media | Remover/condicionar no x64. |
| Miles x64 nao confiavel | erro de dependencia | Alta | Oficializar ou stubar. |
| `__asm` | erro de 32->64 | Media | Pequenos fixes localizados. |
| Ponteiro para `DWORD`/`LONG` | erro de 32->64 | Alta | Corrigir antes do runtime real. |
| `SetWindowLong`/`SetClassLong` | erro de API antiga | Media | Corrigir para Ptr APIs. |
| `#pragma pack`/serializacao | risco protocolo/binario | Alta | Auditar sem alterar layout. |
| Freetype x64 nao encontrado | erro de dependencia | Media | Buscar source/pacote confiavel. |

## 11. Primeira etapa executada

Status: concluida em 2026-06-07.

Foi criada a configuracao isolada:

```text
X64_WinDX9_Bridge|x64
```

Projetos adicionados/buildados nessa etapa:

- `LibProj\JsonLib\lib_json.vcxproj`
- `LibProj\CsFunc\CsFunc.vcxproj`
- `LibProj\CsThread\CsThread.vcxproj`
- `LibProj\CsDM\CsDM.vcxproj`
- `LibProj\CsFilePack\CsFilePack.vcxproj`
- `common_vs2019\common.vcxproj`
- `nlib\nlib.vcxproj`

Arquivos alterados:

- `DProject.sln`: adicionada a solution config `X64_WinDX9_Bridge|x64`. Apenas os 7 projetos acima possuem `Build.0`; `DProject`, `CsGamebryo2.3`, `CsGBChar` e `CsFileTable` ficaram fora do build.
- `LibProj\JsonLib\lib_json.vcxproj`: config x64 isolada com `/MD` e output separado.
- `LibProj\CsFunc\CsFunc.vcxproj`: config x64 isolada com `/MD` e output separado.
- `LibProj\CsThread\CsThread.vcxproj`: config x64 isolada com `/MD` e output separado.
- `LibProj\CsDM\CsDM.vcxproj`: config x64 isolada com `/MD`, output separado e PCH desabilitado em `SHA256_Fn.cpp`.
- `LibProj\CsFilePack\CsFilePack.vcxproj`: config x64 isolada com `/MD` e output separado.
- `common_vs2019\common.vcxproj`: config x64 isolada com `/MD` e output separado.
- `nlib\nlib.vcxproj`: config `X64_WinDX9_Bridge|x64` baseada em `Release|x64`, com `/MD`, include raiz e define da bridge.
- `LibProj\CsThread\CsThreadBase.h`: `RaiseException` passou a usar `ULONG_PTR`, assinatura correta para x64.

Comando usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /t:Rebuild `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\msbuild_x64_windx9_bridge_internal_rebuild.log;Verbosity=Normal"
```

Resultado:

```text
Build concluido com sucesso.
Log: C:\dmo-client-main\docs\portability\msbuild_x64_windx9_bridge_internal_rebuild.log
Output: C:\dmo-client-main\Lib\x64\X64_WinDX9_Bridge\
```

Libs geradas e validadas com `dumpbin /headers`:

| Lib | Machine |
|---|---|
| `COMMON_R.LIB` | `8664 machine (x64)` |
| `CsDM.lib` | `8664 machine (x64)` |
| `CsFilePack.lib` | `8664 machine (x64)` |
| `CsFunc.lib` | `8664 machine (x64)` |
| `CsThread.lib` | `8664 machine (x64)` |
| `lib_json.lib` | `8664 machine (x64)` |
| `nlib.lib` | `8664 machine (x64)` |

Erros encontrados durante a primeira tentativa:

| Erro | Classe | Arquivo | Status |
|---|---|---|---|
| `C1010 unexpected end of file while looking for precompiled header` | erro de configuracao | `LibProj\CsDM\SHA256_Fn.cpp` / `CsDM.vcxproj` | Corrigido clonando a exclusao de PCH para `X64_WinDX9_Bridge|x64`. |
| `C2664 RaiseException ... DWORD * to const ULONG_PTR *` | erro de 32->64/API | `LibProj\CsThread\CsThreadBase.h` | Corrigido usando `sizeof(ULONG_PTR)` e `reinterpret_cast<const ULONG_PTR*>`. |

Avisos ainda presentes no rebuild limpo:

| Aviso | Classe | Arquivo |
|---|---|---|
| `C4309 truncation of constant value` | risco de codigo legado | `LibProj\JsonLib\Include\value.h:528` |
| `C4267 size_t -> unsigned int` | risco 32->64 | `LibProj\JsonLib\json_writer.cpp:119` |
| `C5037 default argument em definicao out-of-line de template` | codigo legado | `LibProj\CsFunc\MemPool.h:121` |
| `C4267 size_t -> uInt/uLong` | risco 32->64 | `LibProj\CsDM\DmCompressZip.cpp:15,24,45` |
| `C4267 size_t -> int` | risco 32->64 | `nlib\base.cpp:72,81` |
| `C4267/C4244` | risco 32->64/codigo legado | `common_vs2019\Captcha\cCaptchaGenerator.cpp`, `common_vs2019\pLength.cpp`, `common_vs2019\pSkill.cpp` |
| `C4302 truncation from TCHAR* to DWORD` | erro 32->64 latente | `LibProj\CsFunc\CsFile.cpp:243` |
| `C4267 size_t -> unsigned int` | risco 32->64 | `LibProj\CsDM\SHA256_Fn.cpp:161,197` |

Observacao: `LibProj\CsFunc\CsFile.cpp:243` e o proximo ajuste obvio de 32->64 (`SendMessage` com ponteiro convertido para `DWORD`). Nao foi corrigido nesta etapa porque o objetivo era fechar a config/build interno primeiro e registrar o risco.

Proximo bloqueio real depois desta etapa, antes da segunda etapa abaixo:

1. Adicionar `X64_WinDX9_Bridge|x64` para `CsFileTable`.
2. Comecar Gamebryo x64 pela source `D:\Gamebryo2.3`, iniciando por `NiSystem` e `NiMain`.
3. Rebuildar/validar third-party x64 (`zlib`, `libpng`, `freetype`).
4. So depois incluir `CsGamebryo2.3`, `CsGBChar` e `DProject`.

## 12. Segunda etapa executada

Status: concluida em 2026-06-07.

Objetivo desta etapa: avancar a bridge sem tocar no renderer, no protocolo de rede ou na logica de jogo. Foram aplicadas somente configuracoes x64 isoladas e pequenos fallbacks necessarios para compilar `NiSystem`/`NiMain` em x64.

### 12.1 `CsFileTable` x64

Foi adicionada a configuracao:

```text
X64_WinDX9_Bridge|x64
```

Arquivos alterados:

- `DProject.sln`: `CsFileTable` passou a participar do build da solution config `X64_WinDX9_Bridge|x64`.
- `LibProj\CsFileTable\CsFileTable.vcxproj`: config x64 isolada baseada em `Release_English|Win32`, com `/MD`, `VERSION_USA`, output separado e define `DMO_X64_WINDX9_BRIDGE`.

Comando usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /t:Rebuild `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\msbuild_x64_windx9_bridge_internal_csfiletable_rebuild.log;Verbosity=Normal"
```

Resultado:

```text
Build concluido com sucesso.
Output: C:\dmo-client-main\Lib\x64\X64_WinDX9_Bridge\CsFileTable.lib
Validacao: 8664 machine (x64)
```

Libs internas x64 validadas ate aqui:

| Lib | Machine |
|---|---|
| `COMMON_R.LIB` | `8664 machine (x64)` |
| `CsDM.lib` | `8664 machine (x64)` |
| `CsFilePack.lib` | `8664 machine (x64)` |
| `CsFileTable.lib` | `8664 machine (x64)` |
| `CsFunc.lib` | `8664 machine (x64)` |
| `CsThread.lib` | `8664 machine (x64)` |
| `lib_json.lib` | `8664 machine (x64)` |
| `nlib.lib` | `8664 machine (x64)` |

Avisos relevantes ainda nao corrigidos:

| Aviso | Classe | Arquivo |
|---|---|---|
| `C4302 truncation from TCHAR* to DWORD` | erro 32->64 latente | `LibProj\CsFunc\CsFile.cpp:243` |
| `C4018 signed/unsigned mismatch` | codigo legado | `LibProj\CsFileTable\MapObjectMng.cpp:119` |
| `C4267 size_t -> int` via `std::pair<int,int>` | risco 32->64 | `LibProj\CsFileTable\CUIDMng.cpp:439` |
| `C4477 sprintf %d vs size_t` | risco 32->64 | `LibProj\CsFileTable\Excel\BasicExcel.cpp:4615` |
| `C4267 size_t -> int` | risco 32->64 | `LibProj\CsFileTable\Event.cpp:1169,1183`, `TimeChargeMng.cpp:449` |

Esses avisos nao bloquearam a compilacao e nao foram corrigidos agora para evitar mudanca massiva.

### 12.2 Gamebryo `NiSystem` x64

Foi adicionada a configuracao:

```text
X64_WinDX9_Bridge|x64
```

Arquivo alterado:

- `D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\VC80\NiSystem.vcxproj`: config x64 isolada baseada em `Release|Win32`, `/MD`, output separado em `D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib`.

Comando usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\VC80\NiSystem.vcxproj" `
  /t:Rebuild `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_nisystem_x64_windx9_bridge_rebuild.log;Verbosity=Normal"
```

Resultado:

```text
Build concluido com sucesso.
Output: D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib\NiSystem.lib
Validacao: 8664 machine (x64)
```

Avisos criticos ainda nao corrigidos:

| Aviso | Classe | Arquivo |
|---|---|---|
| `C4244 intptr_t -> long` | erro 32->64 latente | `D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\NiSystemTimelock.cpp:61` |
| `C4244 DWORD_PTR -> unsigned int` | erro 32->64 latente | `D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\NiThread_Win32.cpp:39` |
| `C4267 size_t -> DWORD` | risco 32->64 | `D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\NiPath_Win32.cpp:101` |
| varios `C4267 size_t -> int/unsigned int` | risco 32->64 | `NiFilename.cpp`, `NiFile_Win32.cpp`, `NiPath.cpp`, `NiMemTracker.cpp` |

### 12.3 Gamebryo `NiMain` x64

Foi adicionada a configuracao:

```text
X64_WinDX9_Bridge|x64
```

Arquivos alterados:

- `D:\Gamebryo2.3\CoreLibs\NiMain\Win32\VC80\NiMain.vcxproj`: config x64 isolada baseada em `Release|Win32`, `/MD`, output separado em `D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib`.
- `D:\Gamebryo2.3\CoreLibs\NiMain\Win32\NiMath.inl`: fallback `_WIN64` para `sin/cos` no lugar do `__asm fsincos`.
- `D:\Gamebryo2.3\CoreLibs\NiMain\Win32\NiProcessor.cpp`: fallback `_WIN64` para as rotinas basicas C++ ja existentes e desvio do `CPUID` em inline asm.
- `D:\Gamebryo2.3\CoreLibs\NiMain\Win32\NiSkinInstance_Win32.cpp`: ponteiros calculados com `char*` e fallback C++ `_WIN64` para o trecho de blend que usava inline asm.
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiPixelData.cpp`: casts explicitos para resolver overload ambiguo de `NiGetViewerString`.
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiPersistentSrcTextureRendererData.cpp`: casts explicitos para resolver overload ambiguo de `NiGetViewerString`.

Observacao: o build de `NiMain` tambem copiou `NiMath.inl` atualizado para `D:\Gamebryo2.3\GB_Include\NiMath.inl`.

Comando final usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "D:\Gamebryo2.3\CoreLibs\NiMain\Win32\VC80\NiMain.vcxproj" `
  /t:Rebuild `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_nimain_x64_windx9_bridge_rebuild_after_asm_fallbacks.log;Verbosity=Normal"
```

Resultado:

```text
Build concluido com sucesso.
Output: D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib\NiMain.lib
Validacao: 8664 machine (x64)
```

Erros encontrados durante a tentativa e classificacao:

| Erro | Classe | Arquivo | Status |
|---|---|---|---|
| `C4235 '__asm' keyword not supported on this architecture` em `fsincos` | erro de 32->64 | `NiMath.inl:103-106` | Corrigido com fallback `_WIN64` usando `sin/cos`. |
| `C2668 NiGetViewerString ambiguous call` | erro de codigo/API C++ em x64 | `NiPixelData.cpp:395,397` | Corrigido com cast explicito para `unsigned int`. |
| `C2668 NiGetViewerString ambiguous call` | erro de codigo/API C++ em x64 | `NiPersistentSrcTextureRendererData.cpp:522,524` | Corrigido com cast explicito para `unsigned int`. |
| `C4235 '__asm' keyword not supported on this architecture` em transforms/CPUID | erro de 32->64 | `Win32\NiProcessor.cpp` | Corrigido com fallback C++ `_WIN64`, preservando Win32. |
| `C4235 '__asm' keyword not supported on this architecture` em skin blend | erro de 32->64 | `Win32\NiSkinInstance_Win32.cpp` | Corrigido com fallback C++ `_WIN64`, preservando Win32. |
| casts de ponteiro para `unsigned int` | erro de 32->64 | `Win32\NiSkinInstance_Win32.cpp` | Corrigido localmente com aritmetica via `char*`. |

Avisos criticos ainda nao corrigidos:

| Aviso | Classe | Arquivo |
|---|---|---|
| `C4311/C4302/C4312` ponteiro `NiPoint3*` para `unsigned int` e volta | erro 32->64 latente | `D:\Gamebryo2.3\CoreLibs\NiMain\Win32\NiPoint3_Win32.cpp:92` |
| `C4244 ptrdiff_t -> unsigned int` | risco 32->64 | `D:\Gamebryo2.3\CoreLibs\NiMain\NiPersistentSrcTextureRendererData.cpp:149` |
| varios `C4267/C4244` de `size_t`, `__int64` e `ptrdiff_t` | risco 32->64/codigo legado | `NiMain` em geral |

### 12.4 Gamebryo `NiAnimation` x64

Status: concluido em 2026-06-07.

Foi adicionada a configuracao:

```text
X64_WinDX9_Bridge|x64
```

Arquivo alterado:

- `D:\Gamebryo2.3\CoreLibs\NiAnimation\Win32\VC80\NiAnimation.vcxproj`: config x64 isolada baseada em `Release|Win32`, `/MD`, PCH preservado, output separado em `D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib`.

Comando usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "D:\Gamebryo2.3\CoreLibs\NiAnimation\Win32\VC80\NiAnimation.vcxproj" `
  /t:Rebuild `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_nianimation_x64_windx9_bridge_rebuild.log;Verbosity=Normal"
```

Resultado:

```text
Build concluido com sucesso.
Output: D:\Gamebryo2.3\GB_Lib\x64\ReleaseLib\NiAnimation.lib
Validacao: 8664 machine (x64)
```

Erros encontrados:

| Erro | Classe | Status |
|---|---|---|
| Nenhum erro de compilacao/link | n/a | `NiAnimation.lib` x64 gerado. |

Avisos relevantes ainda nao corrigidos:

| Aviso | Classe | Arquivo |
|---|---|---|
| `C4267 size_t -> unsigned int` | risco 32->64 | `NiStringPalette.inl:50,79`, `NiTextKeyExtraData.cpp:135`, `NiKFMTool.cpp:1039,1146,1563,1591,1997,2007` |
| `C4267 size_t -> unsigned int` | risco 32->64 | `NiControllerSequence.cpp:208,212,2284` |
| `C4244 __int64 -> NiBSplineData::Handle` | risco 32->64 | `NiBSplineData.cpp:69,119` |
| `C4267 size_t -> unsigned int` | risco 32->64 | `NiBoneLODController.cpp:383,393,400,407,415,617` |
| `C4267 size_t -> int` | risco 32->64 | `NiFloatsExtraDataController.cpp:81`, `NiFloatsExtraDataPoint3Controller.cpp:89` |

Nenhum desses avisos foi corrigido agora porque nao bloqueou a compilacao e pode alterar comportamento/runtime se tratado sem teste dirigido.

### 12.5 Bloqueadores reais agora

O client completo ainda nao compila x64. O que esta pronto agora e:

- libs internas basicas x64;
- `CsFileTable.lib` x64;
- `NiSystem.lib` x64;
- `NiMain.lib` x64.
- `NiAnimation.lib` x64.

Bloqueadores seguintes:

| Bloqueador | Classe | Comentario |
|---|---|---|
| Gamebryo restante sem x64 | erro de dependencia/configuracao | `NiCollision`, `NiParticle`, `NiPortal`, `NiDX9Renderer`, `NiInput`, `NiMilesAudio` ainda precisam config/build x64. |
| Renderer DX9 ainda nao tocado | include/lib | A regra atual e nao mexer no renderer; `NiDX9Renderer` fica para etapa propria. |
| Third-party Gamebryo x64 | erro de dependencia | `zlib`, `libpng`, `freetype` precisam ser mapeadas antes do renderer/texture readers completos. |
| Miles x64 | erro de dependencia | Ainda precisa Miles oficial confiavel ou stub de audio x64. |
| Ponteiros truncados remanescentes | erro 32->64 | `CsFile.cpp:243`, `NiPoint3_Win32.cpp:92`, `NiSystemTimelock.cpp:61`, `NiThread_Win32.cpp:39`. |

Proxima acao recomendada:

1. Compilar o proximo bloco Gamebryo nao-renderer: `NiCollision`.
2. Em seguida mapear `NiParticle`/`NiPortal` se `CsGamebryo2.3` exigir essas libs.
3. So depois preparar etapa separada para `NiDX9Renderer` com Windows SDK x64 + NuGet D3DX oficial.

## 13. Perguntas objetivas

### 1. Da para compilar em x64?

Sim, como port dirigido. Nao, como build imediato do `Release_English|x64` atual.

### 2. Existe Gamebryo x64 pronto?

Nao foi encontrado. Todas as libs prebuilt avaliadas sao x86.

### 3. Existe Gamebryo source suficiente para tentar?

Sim. `D:\Gamebryo2.3` e a melhor candidata. Ela tem source e `.vcxproj` modernos, mas sem x64 pronto.

### 4. Podemos usar DX9 em x64?

Sim. DX9 nao impede x64. O ponto e usar Windows SDK x64 para libs core e o NuGet oficial `Microsoft.DXSDK.D3DX` para D3DX9.

### 5. Podemos usar libs x86 em exe x64?

Nao. Um processo x64 nao carrega DLL x86 in-process e o linker nao aceita `.lib/.obj` x86 em target x64. Ponte x64->x86 so via processo separado/IPC/COM/RPC, nao como lib normal.

### 6. Miles x64 local resolve?

Resolve parte tecnica de arquitetura para laboratorio, mas nao resolve confiabilidade/licenca. `mss64.dll` e x64 e exporta AIL, porem nao e assinado e a origem indicada e suspeita. Para build limpo, precisa Miles oficial ou stub.

### 7. Qual e o menor conjunto de mudancas?

- Criar `X64_WinDX9_Bridge|x64`.
- Rebuildar libs internas x64 `/MD`.
- Rebuildar Gamebryo x64 de `D:\Gamebryo2.3`.
- Usar Windows SDK x64 + D3DX NuGet.
- Stubar/validar Miles.
- Corrigir apenas `__asm`, pointer casts e WinAPI Ptr necessarios para compilar.

### 8. O que nao fazer agora?

- Nao criar `IRenderBackend`.
- Nao migrar para SDL3/Diligent/bgfx.
- Nao alterar protocolo.
- Nao converter todos os projetos sem necessidade.
- Nao aceitar libs x86.
- Nao usar DLL aleatoria como dependencia final.

### 9. Estimativa para compilar x64

| Nivel | Estimativa | Resultado esperado |
|---|---:|---|
| Config nova + libs internas simples | 1-2 dias | Build parcial x64 sem engine final. |
| Third-party + Gamebryo core x64 | 3-7 dias | `Ni*.lib` x64 iniciais. |
| DProject linkando x64 com DX9 | 1-2 semanas | Exe x64 linkado, possivelmente com audio stub. |
| Rodar client x64 estavel | 2-4 semanas | Depende de pointer casts, API antiga, Miles, runtime data. |

### 10. Qual a proxima acao recomendada?

Atualizacao apos a etapa atual: o subconjunto interno, `CsFileTable`, `NiSystem`, `NiMain` e `NiAnimation` ja compilam em x64.

Proxima acao recomendada:

1. Criar `X64_WinDX9_Bridge|x64` para `D:\Gamebryo2.3\CoreLibs\NiCollision`.
2. Compilar `NiCollision` contra `NiSystem.lib`, `NiMain.lib` e `NiAnimation.lib` x64 se exigido.
3. Registrar os erros reais antes de tocar em `NiDX9Renderer`.
4. Depois mapear `NiParticle`/`NiPortal` ou demais libs core exigidas por `CsGamebryo2.3`.
5. Deixar `NiDX9Renderer`, DirectX x64 e Miles x64 para etapas proprias.

## Plano de rollback

- Como a config nova sera separada, rollback deve ser remover blocos `X64_WinDX9_Bridge|x64` dos `.sln/.vcxproj`.
- Nao substituir `GB_Lib\ReleaseLib` atual.
- Nao substituir runtime Win32.
- Apagar apenas saidas x64:

```text
C:\dmo-client-main\Lib\x64\
C:\dmo-client-main\Bin\x64\
D:\Gamebryo2.3\GB_Lib\x64\
```

- Manter logs e documentos para rastreio.

## Referencias oficiais usadas

- Microsoft: [Configure Visual Studio C++ projects to target x64 platforms](https://learn.microsoft.com/en-us/cpp/build/how-to-configure-visual-cpp-projects-to-target-64-bit-platforms?view=msvc-170)
- Microsoft: [LNK1112 module machine type conflicts with target machine type](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk1112?view=msvc-170)
- Microsoft: [Process Interoperability](https://learn.microsoft.com/en-us/windows/win32/winprog64/process-interoperability)
- Microsoft: [MASM for x64 / inline assembler not supported for x64 targets](https://learn.microsoft.com/en-us/cpp/assembler/masm/masm-for-x64-ml64-exe?view=msvc-170)
- Microsoft: [Where is the DirectX SDK?](https://learn.microsoft.com/en-us/windows/win32/directx-sdk--august-2009-)
- NuGet: [Microsoft.DXSDK.D3DX 9.29.952.8](https://www.nuget.org/packages/Microsoft.DXSDK.D3DX)
- RAD Game Tools: [Miles Sound System](https://www.radgametools.com/miles.htm)
