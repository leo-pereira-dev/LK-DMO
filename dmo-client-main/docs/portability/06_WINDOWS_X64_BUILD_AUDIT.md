# Auditoria de Build Windows x64

Escopo: Fase 1 - avaliar e preparar compilacao Windows x64 com mudancas minimas e reversiveis.  
Data: 2026-06-07  
Workspace: `C:\dmo-client-main`

## Atualizacao de Estado - 2026-06-07

Relatorio tecnico completo da tentativa x64:

```text
C:\dmo-client-main\docs\portability\12_X64_FULL_BUILD_ATTEMPT.md
```

Status atual:

- A configuracao `X64_WinDX9_Bridge|x64` foi criada e o client compila/linka em x64.
- Build de verificacao concluida:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /p:OutDir=C:\Release\GSP_x64_retry\ `
  /v:minimal `
  /nologo
```

Resultado:

```text
DProject.vcxproj -> C:\Release\GSP_x64_retry\lk-dmo-x64.exe
```

Log:

```text
C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_eventdata_hash_fix_retry_outdir.log
```

Observacao: a saida principal `C:\Release\GSP_x64` pode falhar com `LNK1201` enquanto um processo antigo `lk-dmo-x64.exe` estiver vivo segurando o PDB. A build em `C:\Release\GSP_x64_retry` provou que esse caso e bloqueio de arquivo/processo, nao erro de compilacao.

## Bloqueador Atual de Runtime

O client x64 nao consegue carregar `pathengine` porque falta `PathEngine.dll` x64 compativel.

DLL original encontrada:

```text
C:\0-NewDmo\ClientDist1\PathEngine.dll
SHA256: 4230BEE6189A61FD8F6B89098AA2BF28A878A35CA463F238417ADC724DF4142A
Arquitetura: x86
Exports: ordinal 1 DLLExport_GetIPathEngine
```

O repo informado pelo usuario foi clonado e avaliado:

```text
https://github.com/sigmaco/pathengine
C:\0-NewDmo\third_party\pathengine-sigmaco
Commit: 8b4ea08
```

Conclusao sobre o repo:

- contem testbed/demo x64;
- nao contem `PathEngine.dll` standalone;
- nao exporta `DLLExport_GetIPathEngine` nos binarios x64 de testbed;
- usa `PATHENGINE_INTERFACE_MAJOR_VERSION=18`, `MINOR_VERSION=0`;
- o client usa `PATHENGINE_INTERFACE_MAJOR_VERSION=5`, `MINOR_VERSION=2`.

Classificacao:

| Item | Classificacao | Estado |
|---|---|---|
| `PathEngine.dll` x64 ausente | erro de dependencia | Bloqueador real para runtime x64. |
| Repo `sigmaco/pathengine` interface 18.0 vs client 5.2 | erro de dependencia / ABI | Nao seguro como drop-in. |
| Stub/fake `PathEngine.dll` | risco de logica de jogo | Nao aplicado. Pode passar do `LoadLibrary`, mas invalida pathfinding/colisao/movimento. |
| `LNK1201` na saida principal | erro de ambiente/configuracao local | Processo antigo preso segurando PDB. Build alternativa passou. |

Proxima acao recomendada:

1. Fechar o processo antigo `lk-dmo-x64.exe` pelo Gerenciador de Tarefas ou reiniciar a sessao para liberar `C:\Release\GSP_x64\lk-dmo-x64.pdb`.
2. Obter uma `PathEngine.dll` x64 standalone do mesmo SDK/ABI 5.2 da DLL x86 de 2010, ou obter o source package correspondente.
3. Validar com:

```powershell
dumpbin /headers PathEngine.dll
dumpbin /exports PathEngine.dll
```

Checklist esperado:

```text
8664 machine (x64)
ordinal 1 DLLExport_GetIPathEngine
PATHENGINE_INTERFACE_MAJOR_VERSION 5 / MINOR_VERSION 2, ou compatibilidade binaria comprovada
```

Estimativa:

- Rebuild x64 do client: imediato, ja validado.
- Desbloquear runtime com PathEngine correto: depende de obter DLL/source x64 compativel; sem isso, nao ha port runtime fiel.
- Criar stub fake para smoke test: 0.5 a 1 dia, mas nao recomendado para validar jogo porque altera pathfinding/colisao.

## Atualizacao - PathEngine x64 encontrada e lock de packs

Foi encontrada uma `PathEngine.dll` x64 no root do workspace:

```text
C:\dmo-client-main\PathEngine.dll
SHA256: FEA139A20C65BCB0B740E61E8ECE60FD92CE0E8CE83F53D5C8ABF9343477C656
```

Validacao:

```text
Arquitetura: x64 / 8664 machine
Exports:
  ordinal 1 DLLExport_GetIPathEngine
  ordinal 2 DLLExport_RefWrapperPointer
  ordinal 3 DllExport_GetIPathEngine
Dependencias: MSVCP120.dll, MSVCR120.dll
Tester nativo: interface=5.7, release=5.29.0, newShape funcionou
```

Ela foi copiada para:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
```

Status: o erro `LoadLibrary() for pathengine` nao reapareceu no smoke test. A DLL e uma candidata real para runtime, embora reporte interface `5.7` enquanto o header do client e `5.2`; por isso ainda precisa validacao em mapa.

Bloqueador atual confirmado pelo Restart Manager:

```text
PID 6572 - lk-dmo-x64.exe
ApplicationType: RmCritical
bRestartable: False
```

Esse processo esta segurando recursos registrados, incluindo:

```text
C:\0-NewDmo\ClientDist1_x64\Data\Pack01.hf
C:\0-NewDmo\ClientDist1_x64\Data\Pack01.pf
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.hf
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.pf
C:\0-NewDmo\ClientDist1_x64\logs\crash.log
C:\Release\GSP_x64\lk-dmo-x64.pdb
```

Tentativas de matar pela sessao atual falharam com `Acesso negado`:

```text
Stop-Process -Id 6572 -Force
taskkill /PID 6572 /T /F
Restart Manager RmShutdown(force)
```

Proxima acao recomendada: encerrar manualmente o PID `6572` pelo Gerenciador de Tarefas elevado ou reiniciar a sessao/maquina. Depois rodar novamente `C:\Release\GSP_x64_retry\lk-dmo-x64.exe` com working directory `C:\0-NewDmo\ClientDist1_x64`.

## Resumo Executivo

O cliente **ainda nao possui uma configuracao Windows x64 real**. A solucao `DProject.sln` declara plataformas `x64`, mas `Release_English|x64` mapeia quase todos os projetos para configuracoes `Win32`; apenas `nlib` aponta para `Release|x64`.

Foi feita uma unica correcao simples de configuracao: adicionar o include de `zlib.h` ao bloco `Release_MT|Win32` de `LibProj/CsFilePack/CsFilePack.vcxproj`, pois as outras configuracoes do mesmo projeto ja tinham esse caminho. Depois disso, o build avancou ate o link do `DProject` e falhou por mistura de runtimes, debug/release e arquitetura.

Nao foi gerada uma configuracao x64 completa porque isso exigiria alterar quase todos os `.vcxproj`, trocar diretorios de libs, separar outputs, resolver libs proprietarias x86 e possivelmente tocar em renderer/Gamebryo. Isso nao e seguro dentro desta fase.

## Ambiente Usado

| Item | Valor |
|---|---|
| Shell | PowerShell |
| MSBuild | `C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe` |
| Versao MSBuild observada | `18.6.3+84d3e95b4` |
| Toolset dos projetos | `v143` |
| Dumpbin usado | `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\dumpbin.exe` |
| Solucao | `DProject.sln` |
| Configuracao testada | `Release_English|x64` |

## Comandos Usados

Primeiro build, antes da correcao de include:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "DProject.sln" `
  /m `
  /p:Configuration=Release_English `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=docs\portability\msbuild_release_english_x64.log;Verbosity=Normal"
```

Segundo build, depois da correcao de include em `CsFilePack.vcxproj`:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "DProject.sln" `
  /m `
  /p:Configuration=Release_English `
  /p:Platform=x64 `
  /v:minimal `
  /flp:"LogFile=docs\portability\msbuild_release_english_x64_after_config_fix.log;Verbosity=Normal"
```

Verificacao de arquitetura de libs:

```powershell
dumpbin /headers <lib>
```

## Sistema de Build Atual

| Projeto | Tem configuracao x64 real? | Observacao |
|---|---:|---|
| `DProject\DProject.vcxproj` | Nao | Apenas `Win32`; `Release_English|x64` da solucao mapeia para `Release_English|Win32`. |
| `common_vs2019\common.vcxproj` | Nao | Apenas `Win32`; mapeado para `Release_DLL|Win32` no x64 da solucao. |
| `LibProj\CsFunc\CsFunc.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Release_MT|Win32`. |
| `LibProj\CsThread\CsThread.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Release_MT|Win32`. |
| `LibProj\CsFilePack\CsFilePack.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Release_MT|Win32`. |
| `LibProj\CsDM\CsDM.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Release_MT|Win32`. |
| `LibProj\CsFileTable\CsFileTable.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Release_English|Win32`. |
| `LibProj\CsGamebryo2.3\CsGamebryo2.3.vcxproj` | Nao | Apenas `Win32`; depende de Gamebryo/DX libs x86. |
| `LibProj\CsGBChar\CsGBChar.vcxproj` | Nao | Apenas `Win32`; depende de Gamebryo/DX libs x86. |
| `LibProj\JsonLib\lib_json.vcxproj` | Nao | Apenas `Win32`; x64 da solucao mapeia para `Debug|Win32`, causando mismatch debug/release. |
| `nlib\nlib.vcxproj` | Sim | Tem `Debug|x64` e `Release|x64`; compilou como x64 no teste. |

## Mapeamento Problematico de `Release_English|x64`

Trechos relevantes de `DProject.sln`:

| Projeto | `Release_English|x64` mapeia para |
|---|---|
| `CsFunc` | `Release_MT|Win32` |
| `CsGamebryo2.3` | `Release_English|Win32` |
| `CsFileTable` | `Release_English|Win32` |
| `CsGBChar` | `Release_English|Win32` |
| `DProject` | `Release_English|Win32` |
| `CsThread` | `Release_MT|Win32` |
| `CsFilePack` | `Release_MT|Win32` |
| `CsDM` | `Release_MT|Win32` |
| `common` | `Release_DLL|Win32` |
| `nlib` | `Release|x64` |
| `lib_json` | `Debug|Win32` |

Conclusao: a plataforma `x64` na solucao e nominal. Ela mistura um projeto x64 (`nlib`) com muitos projetos Win32 e ainda mistura release/debug e runtime static/dynamic.

## Mudanca Aplicada

Arquivo alterado:

- `LibProj/CsFilePack/CsFilePack.vcxproj`

Mudanca:

```xml
<AdditionalIncludeDirectories>.\;..\;..\..\..\;..\..\Gamebryo2.3\ThirdPartyCode\ZLib\zlib123</AdditionalIncludeDirectories>
```

Motivo:

- O build falhava em `LibProj\CsDM\Zipwrapper\compress.hpp(11,10)` com `zlib.h` ausente.
- `zlib.h` existe em `Gamebryo2.3\ThirdPartyCode\ZLib\zlib123\zlib.h`.
- `Debug|Win32` e `Release|Win32` do mesmo `CsFilePack.vcxproj` ja tinham caminho equivalente.
- A correcao foi localizada apenas em `Release_MT|Win32`.

Nao foram feitas mudancas em renderer, logica de jogo ou protocolo de rede.

## Resultado do Primeiro Build

Log:

- `docs\portability\msbuild_release_english_x64.log`

Erro principal:

| Erro | Classificacao | Arquivo | Comentario |
|---|---|---|---|
| `error C1083: Cannot open include file: 'zlib.h'` | erro de include/lib / erro de configuracao | `LibProj\CsDM\Zipwrapper\compress.hpp(11,10)` via `CsFilePack.vcxproj` | Corrigido adicionando include path no bloco `Release_MT|Win32` de `CsFilePack.vcxproj`. |

## Resultado do Segundo Build

Log:

- `docs\portability\msbuild_release_english_x64_after_config_fix.log`

O build avancou e gerou/compilou:

| Projeto | Resultado observado |
|---|---|
| `nlib` | Gerou `C:\dmo-client-main\x64\Release\nlib.lib` |
| `CsFilePack` | Gerou `C:\dmo-client-main\Lib\common\CsFilePack_mt.lib` |
| `CsFunc` | Gerou `C:\dmo-client-main\Lib\common\CsFunc_mt.lib` |
| `CsThread` | Gerou `C:\dmo-client-main\Lib\common\CsThread_mt.lib` |
| `CsDM` | Gerou `C:\dmo-client-main\Lib\common\CsDM_mt.lib` |
| `CsFileTable` | Gerou `C:\dmo-client-main\Lib\Release_English\CsFileTable.lib` |
| `CsGamebryo2.3` | Gerou `C:\dmo-client-main\Lib\Release_English\CsGB2.3.lib` |
| `CsGBChar` | Gerou `C:\dmo-client-main\Lib\Release_English\CsGBChar.lib` |
| `DProject` | Falhou no link |

Erros de link encontrados:

| Erro | Classificacao | Exemplo | Causa provavel |
|---|---|---|---|
| `LNK2038: mismatch detected for 'RuntimeLibrary'` | erro de configuracao | `CsDM_mt.lib(...) value 'MT_StaticRelease' doesn't match value 'MD_DynamicRelease' in DirectShow.obj` | A solucao mapeia libs auxiliares para `Release_MT|Win32` (`/MT`), mas `DProject Release_English|Win32` usa `MultiThreadedDLL` (`/MD`). |
| `LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL'` | erro de configuracao | `json_vc71_libmtd.lib(...) value '2' doesn't match value '0' in DirectShow.obj` | `lib_json` foi mapeado para `Debug|Win32` no `Release_English|x64` da solucao. |
| `LNK2038: RuntimeLibrary MDd_DynamicDebug vs MD_DynamicRelease` | erro de configuracao | `json_vc71_libmtd.lib` | Mescla debug/release. |
| `fatal error C1905: Front end and back end not compatible (must target same processor)` | erro de 32->64 / erro de configuracao | `LINK` em `DProject.vcxproj` | Mistura de objetos/libs gerados para processadores diferentes; `nlib` foi gerado x64 enquanto o executavel e varios projetos sao Win32. |
| `fatal error LNK1257: code generation failed` | consequencia | `DProject.vcxproj` | Consequencia do `C1905` e dos mismatches anteriores. |

## Libs x86 Confirmadas com `dumpbin`

Todas as libs abaixo retornaram `14C machine (x86)`:

| Lib | Status |
|---|---|
| `DirectX_June2010\Lib\x86\d3d9.lib` | x86 |
| `DirectX_June2010\Lib\x86\d3dx9.lib` | x86 |
| `DirectX_June2010\Lib\x86\dinput8.lib` | x86 |
| `DirectX_June2010\Lib\x86\ddraw.lib` | x86 |
| `DirectX_June2010\Lib\x86\dsetup.lib` | x86 |
| `DirectX_June2010\Lib\x86\dxguid.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiMain.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiDX9Renderer.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiD3D10Renderer.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiInput.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiMilesAudio.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ReleaseLib\NiSystem.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ThirdPartyLib\zlib.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ThirdPartyLib\libpng.lib` | x86 |
| `Gamebryo2.3\GB_Lib\ThirdPartyLib\freetype213.lib` | x86 |
| `LibProj\MilesSound\lib\mss32.lib` | x86 |

## Libs Faltantes Para x64

| Lib/area | Situacao |
|---|---|
| DirectX June 2010 x64 | Nao ha pasta `DirectX_June2010\Lib\x64`; existe apenas `Lib\x86`. |
| Gamebryo ReleaseLib x64 | Nao encontrada. `ReleaseLib` atual e x86. |
| Miles Sound x64 | Nao encontrada. `mss32.lib` e x86. |
| ThirdParty prebuilt x64 (`zlib`, `libpng`, `freetype213`) | Nao encontrada em `Gamebryo2.3\GB_Lib\ThirdPartyLib`; libs atuais sao x86. |
| Libs internas x64 | So `nlib` tem configuracao x64 real. As demais precisam ser criadas. |

## Atualizacao: Dependencias x64 Localizadas

Depois da auditoria inicial, foram avaliadas as fontes/dependencias adicionais indicadas pelo usuario.

### DirectX D3DX via NuGet

Foi baixado e extraido o pacote oficial `Microsoft.DXSDK.D3DX.9.29.952.8`:

```powershell
Invoke-WebRequest `
  "https://www.nuget.org/api/v2/package/Microsoft.DXSDK.D3DX/9.29.952.8" `
  -OutFile "packages\Microsoft.DXSDK.D3DX.9.29.952.8.nupkg"
```

Arquivos relevantes encontrados:

| Arquivo | Status |
|---|---|
| `packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\include\d3dx9.h` | disponivel |
| `packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\lib\x64\d3dx9.lib` | disponivel x64 |
| `packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\D3DX9_43.dll` | disponivel x64 |
| `packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\D3DCompiler_43.dll` | disponivel x64 |

Observacao importante: esse pacote resolve D3DX, mas **nao resolve `dsetup.lib`/DirectSetup**. O cliente ainda referencia:

| Arquivo | Referencia |
|---|---|
| `DProject\Engine.cpp` | `#pragma comment(lib, "dsetup.lib")` e `DirectXSetupGetVersion` |
| `DProject\App\DXVersionChecker.cpp` | `DirectXSetupGetVersion` |
| `DProject\App\Win32App.cpp` | chama `DXSETUP.exe` em caminhos Jun2010 |

Logo, trocar `DirectX_June2010\Lib\x86` por esse NuGet ajuda no `d3dx9.lib`, mas ainda falta uma decisao para `dsetup.lib` em x64: lib oficial x64 do DXSDK legado, remocao condicional da checagem, ou stub controlado. Isso e classificado como **erro de dependencia / API antiga**, nao como simples config.

### Miles x64

O arquivo indicado pelo usuario foi validado:

| Arquivo | Resultado |
|---|---|
| `D:\Gamebryo2.3\X64\mss64.dll` | `8664 machine (x64)` |

Foi gerada uma import library x64:

```powershell
lib.exe /def:LibProj\MilesSoundX64\lib\mss64.def /machine:x64 /out:LibProj\MilesSoundX64\lib\mss64.lib
```

Arquivos criados:

| Arquivo | Status |
|---|---|
| `LibProj\MilesSoundX64\bin\mss64.dll` | copia da DLL indicada pelo usuario |
| `LibProj\MilesSoundX64\lib\mss64.def` | DEF gerado a partir de 340 exports |
| `LibProj\MilesSoundX64\lib\mss64.lib` | import lib validada como x64 |

Risco: `D:\Gamebryo2.3\X64\readme.txt` informa origem `www.opendll.com`. Isso deve ser tratado como risco de confianca/licenciamento antes de distribuir ou usar em producao.

### `D:\Gamebryo2.3`

Essa arvore e mais completa que a copia local antiga e possui `.vcxproj` ja convertidos para MSBuild moderno com `PlatformToolset v143`.

Resultados:

| Item | Resultado |
|---|---|
| CodeGraph | 4036 arquivos indexados |
| Projetos `.vcxproj` em `CoreLibs`, `AppFrameworks`, `ToolLibs`, `Build` | 44 |
| Projetos com config x64/Win64 | 0 |
| `.lib` verificados com `dumpbin` | 40 |
| `.lib` x64 encontrados | 0 |
| `GB_Lib\ReleaseLib\Ni*.lib` | todos x86 |

Probe de build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "D:\Gamebryo2.3\Build\Win32\VC80\Core01_CoreLibs.sln" `
  /p:Configuration=Release `
  /p:Platform=x64 `
  /v:minimal `
  /m `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_core01_release_x64_probe.log;Verbosity=Normal"
```

Resultado:

| Erro | Classificacao | Comentario |
|---|---|---|
| `MSB4126: A configuracao da solucao especificada "Release|x64" e invalida` | erro de configuracao | A solucao `Core01_CoreLibs.sln` so declara Win32. |

Conclusao: boa candidata para **portar/recompilar Gamebryo**, mas nao entrega x64 pronto.

### `C:\dmo-client-main\Gamebryo_2.3`

Essa arvore tambem tem fontes completas, mas e mais antiga para build moderno:

| Item | Resultado |
|---|---|
| `.sln` | 27 |
| `.vcproj` | 97 |
| `.vcxproj` | 0 |
| `.lib` verificados com `dumpbin` | 184 |
| `.dll` verificados com `dumpbin` | 103 |
| `.lib` x64 encontrados | 0 |
| `.dll` x64 encontrados | 0 |
| Projetos principais do engine com x64/Win64 | 0 de 75 avaliados |

Observacao: existem configs x64 apenas dentro de projetos auxiliares do `ThirdPartyCode\ZLib\zlib123\contrib\vstudio\vc8`, mas nao nos projetos principais do Gamebryo.

Probe de build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" `
  "C:\dmo-client-main\Gamebryo_2.3\CompileSDK.sln" `
  /p:Configuration=Release `
  /p:Platform=x64 `
  /v:minimal `
  /m `
  /flp:"LogFile=C:\dmo-client-main\docs\portability\gamebryo_underscore_compile_sdk_release_x64_probe.log;Verbosity=Normal"
```

Resultado:

| Erro | Classificacao | Comentario |
|---|---|---|
| `MSB4025: Nao foi possivel carregar o arquivo de projeto. Elemento raiz inexistente.` | erro de configuracao / projeto antigo | A solucao e Visual Studio 2005 (`Format Version 9.00`) com `.vcproj`; precisa conversao antes de build moderno. |

Dependencia Miles nesta arvore:

| Arquivo | Status |
|---|---|
| `Gamebryo_2.3\ThirdPartyCode\MilesWin\lib\mss32.lib` | x86 |
| `Gamebryo_2.3\ThirdPartyCode\MilesWin\redist\mss32.dll` | x86 |
| `mss64.lib` / `mss64.dll` | nao encontrados nessa arvore |

Conclusao: esta arvore e util como referencia/source, mas e pior para tentativa imediata x64 do que `D:\Gamebryo2.3`, porque ainda esta em `.vcproj` VC80.

## Casts Perigosos e API Antiga Encontrados

| Severidade | Arquivo | Linha(s) | Classificacao | Comentario |
|---|---|---:|---|---|
| Bloqueador x64 em Debug | `LibProj\CsFunc\assert_cs.h` | 31-37 | erro de 32->64 / codigo | Macros usam `__asm { int 3 }`; MSVC x64 nao aceita inline asm. Deve virar `__debugbreak()` em trilha x64. |
| Alto | `DProject\_Interface\Game\ItemSeparate.cpp` | 1096-1099 | erro de API antiga / 32->64 | `SetWindowLong(..., (LONG)EditSubProc...)` trunca ponteiro em x64. Usar `SetWindowLongPtr`/`LONG_PTR`. |
| Alto | `DProject\_Interface\Game\MoneySeparate.cpp` | 194-196 | erro de API antiga / 32->64 | Mesmo problema de subclassing Win32 com `LONG`. |
| Medio/alto | `DProject\App\Win32App.cpp` | 437-438 | API antiga | `SetWindowLong` para estilos; nao trunca ponteiro neste caso, mas em x64 o padrao e `SetWindowLongPtr`. |
| Alto | `LibProj\CsFunc\CsFile.cpp` | 243 | erro de 32->64 | `SendMessage(..., (DWORD)g_szBrowerInitPath)` trunca ponteiro. Usar `LPARAM`/`reinterpret_cast<LPARAM>`. |
| Alto | `LibProj\CsThread\Utility\CFJobQueueEx.cpp` | 114 | erro de 32->64 / codigo | `reinterpret_cast<DWORD&>(pArg->pArg)` perde bits se `pArg` for ponteiro em x64. |
| Medio | `LibProj\CsThread\CsThreadBase.h` | 383 | API antiga / validacao necessaria | `RaiseException(..., (DWORD*)&info)` deve ser validado com estrutura x64; normalmente essa tecnica usa `ULONG_PTR` no campo apropriado. |

## Arquivos Criticos

| Arquivo | Motivo |
|---|---|
| `DProject.sln` | Declara plataforma x64 nominal, mas mapeia projetos para Win32/debug/MT misturados. |
| `DProject\DProject.vcxproj` | Nao tem configuracao x64; depende de libs x86 e usa `TargetMachine>MachineX86`. |
| `common_vs2019\common.vcxproj` | Nao tem configuracao x64 real. |
| `nlib\nlib.vcxproj` | Unico projeto com x64 real; compilou em x64. |
| `LibProj\CsFilePack\CsFilePack.vcxproj` | Corrigido include de zlib em `Release_MT|Win32`; ainda nao tem x64. |
| `LibProj\JsonLib\lib_json.vcxproj` | Mapeamento `Release_English|x64` usa `Debug|Win32`, gerando `_ITERATOR_DEBUG_LEVEL` e runtime mismatch. |
| `LibProj\CsFunc\assert_cs.h` | Inline asm bloqueia x64 em debug. |
| `LibProj\CsFunc\CsFile.cpp` | Cast de ponteiro para `DWORD`. |
| `DProject\_Interface\Game\ItemSeparate.cpp` | Subclassing com `SetWindowLong`/`LONG`. |
| `DProject\_Interface\Game\MoneySeparate.cpp` | Subclassing com `SetWindowLong`/`LONG`. |
| `DProject\Engine.cpp`, `DProject\Engine.h`, `DProject\Engine_Render.cpp` | Renderer/DX/Gamebryo x86; nao alterados por regra. |

## Bloqueadores Reais

1. **Nao existe configuracao x64 real para o cliente**: 10 dos 11 `.vcxproj` do cliente estao apenas em `Win32`.
2. **`Release_English|x64` da solucao e inconsistente**: mistura Win32, x64, Debug, Release, `/MT` e `/MD`.
3. **Gamebryo prebuilt continua x86**: `D:\Gamebryo2.3` e `C:\dmo-client-main\Gamebryo_2.3` nao trouxeram `Ni*.lib` x64.
4. **Miles x64 agora tem import lib gerada, mas precisa validacao de confianca e compatibilidade**: a DLL fornecida e x64, porem o readme aponta origem `opendll.com`.
5. **DirectX D3DX x64 foi localizado via NuGet, mas `dsetup.lib` ainda bloqueia**: o pacote resolve `d3dx9.lib` x64, nao DirectSetup.
6. **Third-party prebuilt continuam x86**: `zlib`, `libpng`, `freetype213` das arvores Gamebryo atuais sao x86.
7. **Renderer e Gamebryo bloqueiam link x64 completo**: mesmo com configuracao x64, `Ni*.lib` atuais sao x86.
8. **Inline asm e casts de ponteiro precisam ajustes cirurgicos** antes de uma compilacao x64 verdadeira.
9. **Output dirs e nomes de libs estao desalinhados**: warnings `MSB8012` em `common` e `lib_json` indicam configuracao fragil.

## Pesquisa Externa Sobre x64 e "Fake x64"

Fontes consultadas:

| Tema | Fonte | Conclusao aplicada |
|---|---|---|
| Misturar x86 e x64 no link | Microsoft Learn: [LNK1112](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk1112?view=msvc-170) | Objetos compilados para plataformas diferentes geram erro de link. Nao ha chave segura para linkar `.lib` x86 em exe x64. |
| Criar config x64 C++ | Microsoft Learn: [Configure C++ projects to target x64](https://learn.microsoft.com/en-us/cpp/build/how-to-configure-visual-cpp-projects-to-target-64-bit-platforms?view=msvc-170) | O caminho correto e criar plataforma x64 real, copiando Win32 e ajustando `/MACHINE:X64`, nao apenas renomear a solucao. |
| DLL x86 em processo x64 | Microsoft Learn: [Running 32-bit Applications](https://learn.microsoft.com/en-us/windows/win32/winprog64/running-32-bit-applications) e [Process Interoperability](https://learn.microsoft.com/en-us/windows/win32/winprog64/process-interoperability) | Um processo x64 nao carrega DLL 32-bit. O workaround e out-of-process COM/RPC/IPC, nao link in-process. |
| Inline asm x64 | Microsoft Learn: [Inline Assembler](https://learn.microsoft.com/en-us/cpp/assembler/inline/inline-assembler?view=msvc-170) | MSVC nao suporta inline `__asm` em x64; usar `__debugbreak()` ou MASM/externo. |
| DirectX D3DX legado | NuGet: [Microsoft.DXSDK.D3DX](https://www.nuget.org/packages/Microsoft.DXSDK.D3DX) e Microsoft DirectX Blog: [DirectX Landing Page](https://devblogs.microsoft.com/directx/landing-page/) | O pacote contem D3DX9/10/11 x86/x64 para manter codigo legado, mas nao substitui todo o DirectX SDK/DirectSetup. |
| DirectX SDK legado | Microsoft Download Center: [DirectX SDK June 2010](https://www.microsoft.com/en-us/download/details.aspx?id=6812) e Microsoft Learn: [Where is the DirectX SDK?](https://learn.microsoft.com/en-us/windows/win32/directx-sdk--august-2009-) | O SDK legado possui fluxo com `DXSDK_DIR` e libs x86/x64; para `dsetup.lib`, a fonte adequada e o SDK oficial legado, nao o NuGet D3DX. |
| Large Address Aware | Microsoft Learn: [`/LARGEADDRESSAWARE`](https://learn.microsoft.com/en-us/cpp/build/reference/largeaddressaware-handle-large-addresses?view=msvc-170) e [64-bit programming for game developers](https://learn.microsoft.com/en-us/windows/win32/dxtecharts/sixty-four-bit-programming-for-game-developers) | Alternativa util de curto prazo para Win32: manter exe x86 e habilitar enderecos grandes. Nao transforma em x64, mas pode aliviar limite de memoria. |
| Gamebryo 2.3 publico | Beyond3D: [Gamebryo 2.3 revealed](https://www.beyond3d.com/content/news/278), GameDeveloper: [Emergent Adds DX10, PhysX To Gamebryo](https://www.gamedeveloper.com/game-platforms/product-emergent-adds-dx10-physx-to-gamebryo) | Gamebryo 2.3 tinha foco DX10/PhysX/PC, mas nao encontrei evidencia publica confiavel de pacote `Ni*.lib` x64 pronto para 2.3. |
| Gamebryo versoes futuras | GamingBolt: [Gamebryo interview](https://gamingbolt.com/gamebryo-interview-developing-a-next-generation-engine-with-high-quality-graphics-and-stability) | Ha mencao publica de suporte 64-bit em release futura do Gamebryo, o que reforca que 2.3 nao deve ser assumido como x64 pronto. |
| Miles | RAD Game Tools: [Miles Sound System](https://www.radgametools.com/miles.htm) | A fonte oficial do middleware e RAD. DLL baixada de sites terceiros deve ser tratada como risco, mesmo quando `dumpbin` mostra x64. |

Conclusao sobre "fake x64":

- **Fake x64 de solucao**: ja existe em parte no `DProject.sln`, mas e so um nome de plataforma; quando mapeia para Win32, o executavel continua x86.
- **Fake x64 de link**: nao e viavel. O linker e o loader do Windows impedem misturar x86/x64 in-process.
- **Bridge x64 -> x86**: tecnicamente possivel somente como processo separado via IPC/COM/RPC. Para Gamebryo/renderer/audio isso vira mudanca de arquitetura, nao uma correcao simples de build.
- **Alternativa realista de curto prazo**: manter Win32 e avaliar `/LARGEADDRESSAWARE` para aumentar espaco de endereco no Windows x64. Isso nao cumpre "client x64", mas pode ser um paliativo reversivel.

## Resposta Objetiva: Da Para Compilar x64?

**Nao como build imediato do client completo.** O estado atual ainda nao permite compilar `DProject` Windows x64 de ponta a ponta.

**Sim como projeto de portabilidade**, desde que a cadeia abaixo seja feita antes do link final:

1. Criar uma configuracao isolada `X64_WinDX9_Bridge|x64` ou `Release_English_X64|x64`.
2. Gerar configs x64 reais para as libs internas, com outputs separados em `Lib\x64\...`.
3. Recompilar Gamebryo x64 a partir de `D:\Gamebryo2.3` preferencialmente, pois ela ja possui `.vcxproj` e `v143`.
4. Recompilar ou substituir third-party x64 (`zlib`, `libpng`, `freetype213`).
5. Usar `Microsoft.DXSDK.D3DX.9.29.952.8` para `d3dx9.lib`/`D3DX9_43.dll` x64.
6. Resolver `dsetup.lib`/`DirectXSetupGetVersion` por lib x64 oficial ou condicional controlado.
7. Validar Miles x64 com `mss64.dll` confiavel e `mss64.lib`.
8. Corrigir `__asm`, `DWORD(pointer)`, `LONG(pointer)`, `SetWindowLong` e casts similares.

Portanto, a resposta tecnica e: **da para portar, mas ainda nao da para compilar x64 completo sem trabalho estrutural de build/dependencias**.

## Por Que Nao Gerei Configuracao x64 Completa

Gerar uma configuracao x64 segura exigiria:

- adicionar `x64` em quase todos os `.vcxproj`;
- criar output dirs separados para nao misturar libs x86/x64;
- trocar `TargetMachine` para `MachineX64`;
- resolver runtime `/MD` vs `/MT`;
- substituir ou recompilar libs x86 de Gamebryo/Miles/third-party;
- decidir o que fazer com renderer/DX9;
- ajustar casts e `__asm`.

Isso ultrapassa "mudancas minimas e reversiveis". A unica mudanca aplicada foi uma correcao de include ja espelhada em outras configuracoes do mesmo projeto.

## Estimativa Para Compilar x64

| Alvo | Estimativa | Condicao |
|---|---:|---|
| `nlib` x64 | Ja compila | Configuracao existente. |
| Subconjunto x64 sem renderer/Gamebryo/Miles | 2-5 dias | Criar configs x64 para `common`, `CsFunc`, `CsThread`, `CsDM`, `CsFilePack`, `lib_json`; corrigir `__asm`/casts simples. |
| Solucao nominal `Release_English|x64` coerente, ainda Win32 fake | 0.5-1 dia | Corrigir mapeamentos do `.sln`; utilidade limitada para x64 real. |
| Cliente completo Windows x64 com renderer atual | Bloqueado | Requer libs x64 de Gamebryo/Miles/DirectX/third-party ou substituicao dos subsistemas. |
| Cliente completo Windows x64 se todas as libs x64 existirem | 2-6 semanas | Ainda restam casts, `__asm`, output dirs, runtime, link e testes de runtime. |

## Proxima Acao Recomendada

Nao continuar tentando "consertar" `Release_English|x64` da solucao atual como se fosse x64 real. Ela e um mapeamento misto e pode mascarar erros.

Proxima acao recomendada:

1. Criar uma configuracao experimental separada, por exemplo `X64_Audit|x64`, sem tocar `Release_English|Win32`.
2. Incluir somente projetos sem renderer no primeiro corte:
   - `nlib`
   - `common_vs2019`
   - `LibProj\JsonLib`
   - `LibProj\CsFunc`
   - `LibProj\CsThread`
   - `LibProj\CsDM`
   - `LibProj\CsFilePack`
3. Separar outputs em `Lib\x64\...` para nao misturar `.lib` x86 e x64.
4. Corrigir primeiro:
   - `__asm { int 3 }` -> `__debugbreak()` em x64;
   - casts de ponteiro para `DWORD/LONG`;
   - mapeamento `/MD` vs `/MT`;
   - includes de third-party.
5. So depois avaliar `CsFileTable`.
6. Deixar `DProject`, `CsGamebryo2.3`, `CsGBChar`, renderer, Miles e Gamebryo fora do primeiro build x64 real ate existirem libs x64 ou uma estrategia de substituicao.

## Estado Final Desta Fase

| Item | Estado |
|---|---|
| Sistema de build identificado | Concluido |
| Projetos Win32/x86 identificados | Concluido |
| Casts perigosos principais identificados | Concluido |
| `__asm` identificado | Concluido |
| Libs x86 confirmadas com `dumpbin` | Concluido |
| Build tentado | Concluido |
| Erros classificados | Concluido |
| Mudancas massivas evitadas | Concluido |
| Renderer/logica/protocolo alterados | Nao |

## Atualizacao 2026-06-07 - Build x64 completa

Depois desta auditoria inicial, foi executada uma tentativa completa da configuracao `X64_WinDX9_Bridge|x64`, incluindo libs internas, Gamebryo source x64 e `DProject.sln`.

Resultado atualizado: **o client compila e linka em x64** neste ambiente.

Artefato gerado:

```text
C:\Release\GSP_x64\lk-dmo-x64.exe
```

Relatorio tecnico completo da tentativa:

```text
C:\dmo-client-main\docs\portability\12_X64_FULL_BUILD_ATTEMPT.md
```

## Atualizacao 2026-06-07 - Runtime Miles x64

O crash atual pos-login nao foi registrado pelo `crash.log` do client, mas o
Event Viewer mostrou falha em:

```text
C:\0-NewDmo\ClientDist1_x64\mss64.dll
Exception: 0xc0000005
Offset: 0x0000000000070812
```

Foi habilitado WER LocalDumps para `lk-dmo-x64.exe`:

```text
C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\WER
```

Tambem foi encontrado e corrigido um erro de runtime: `Data\NP\Sound` continha
plugins Miles x86 (`mss*.asi/.flt`) sendo usados por processo x64. Eles foram
movidos para backup e substituidos pelos plugins `mss64*` do SDK Miles x64.

Detalhes completos:

```text
C:\dmo-client-main\docs\portability\12_X64_FULL_BUILD_ATTEMPT.md
```

## Atualizacao 2026-06-07 - Estado Real Apos Validacao Runtime

A substituicao direta dos providers Miles por arquivos `mss64*` do SDK 9.4d nao ficou valida para o runtime atual. O processo x64 carregava `mss64.dll` 7.0x, mas crashava dentro de providers 9.4d como `mss64mp3.asi` e `mss64dolby.flt`.

Estado atual do audio:

```text
Miles x64 esta stubado no build X64_WinDX9_Bridge
C:\dmo-client-main\DProject\CSoundMgr.cpp
```

Classificacao:

```text
erro de dependencia / incompatibilidade de versao Miles
```

Isso e reversivel e foi feito para permitir validar os proximos gargalos x64 sem bloquear no audio.

O proximo erro observado no runtime foi:

```text
iPathEngine::newShape() failed - vertex coordinate exceeds allowed range. Returning zero.
```

Classificacao:

```text
erro 32->64 / serializacao Win32 lida com layout x64
```

Correcao minima aplicada:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBObject.cpp
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp
```

Esses arquivos agora leem os headers de path info persistidos em disco como layout Win32 fixo, em vez de avancar offsets usando structs runtime x64 com ponteiros de 8 bytes.

Build validado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_win32_disk_layout.log;Verbosity=normal"
```

Resultado:

```text
Build OK
C:\Release\GSP_x64\lk-dmo-x64.exe
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Relatorio tecnico do metodo sistematico:

```text
C:\dmo-client-main\docs\portability\13_X64_SERIALIZATION_LAYOUT_AUDIT.md
```

## Atualizacao 2026-06-07 - Audio Miles 9.4d restaurado

Foi encontrada a DLL correta:

```text
C:\dmo-client-main\mss64.dll
```

Ela e Miles 9.4d e bate com os providers x64 do SDK local.

Mudancas aplicadas:

```text
D:\Gamebryo2.3\CoreLibs\NiAudio\Miles\NiMilesAudioSystem.cpp
C:\dmo-client-main\DProject\CSoundMgr.cpp
C:\dmo-client-main\DProject\DProject.vcxproj
```

Resultado:

```text
Audio x64 habilitado novamente
Build OK
Smoke test sem crash imediato
```

Relatorio:

```text
C:\dmo-client-main\docs\portability\14_MILES_X64_94D_AUDIO_RESTORE.md
```
