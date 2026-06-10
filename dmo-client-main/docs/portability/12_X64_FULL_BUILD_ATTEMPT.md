# Windows x64 Full Build Attempt - X64_WinDX9_Bridge

Data: 2026-06-07  
Workspace: `C:\dmo-client-main`  
Gamebryo source usado: `D:\Gamebryo2.3`  
Resultado: **client compilou e linkou em x64**  
Artefato: `C:\Release\GSP_x64\lk-dmo-x64.exe`

## Escopo

Esta tentativa manteve o client em DX9 e não iniciou migração de renderer.

Não foram alterados:

- renderer/backend;
- protocolo de rede;
- regras de jogo;
- formato de pacotes.

As mudanças aplicadas foram de build, include/lib path, compatibilidade Win64 pontual e shims pequenos para símbolos ausentes.

## Ambiente

- MSBuild: `C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe`
- MSVC libs x64: `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.44.35207\lib\x64`
- ATL/MFC libs x64: `C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.44.35207\atlmfc\lib\x64`
- Windows SDK libs: `C:\Program Files (x86)\Windows Kits\10\Lib\10.0.26100.0\um\x64` e `...\ucrt\x64`
- D3DX NuGet: `C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8`
- FreeType NuGet usado como substituto: `freetype28.lib`
- Miles x64: `C:\dmo-client-main\LibProj\MilesSoundX64\lib\mss64.lib`

## Comando final de build

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  /nologo
```

Log final de sucesso:

```text
C:\dmo-client-main\docs\portability\dproject_sln_x64_windx9_bridge_build_after_d3dx9guid_pch.log
```

Saida final:

```text
DProject.vcxproj -> C:\Release\GSP_x64\lk-dmo-x64.exe
```

## Verificacao do executavel

`dumpbin /headers C:\Release\GSP_x64\lk-dmo-x64.exe`:

```text
8664 machine (x64)
20B magic # (PE32+)
Application can handle large (>2GB) addresses
```

Imports relevantes:

```text
d3dx9_43.dll
d3dx10_43.dll
d3d9.dll
DINPUT8.dll
mss64.dll
MSVCP140.dll
VCRUNTIME140.dll
VCRUNTIME140_1.dll
KERNEL32.dll
USER32.dll
```

Arquivos gerados:

```text
C:\Release\GSP_x64\lk-dmo-x64.exe  13,551,616 bytes
C:\Release\GSP_x64\lk-dmo-x64.pdb 119,664,640 bytes
```

## O que foi recompilado em x64

Libs internas do client em `C:\dmo-client-main\Lib\x64\X64_WinDX9_Bridge`:

- `COMMON_R.lib`
- `CsDM.lib`
- `CsFilePack.lib`
- `CsFileTable.lib`
- `CsFunc.lib`
- `CsGBChar.lib`
- `CsGamebryo2.3.lib`
- `CsThread.lib`
- `lib_json.lib`
- `nlib.lib`

Gamebryo/libs de apoio em `D:\Gamebryo2.3\GB_Lib\x64`:

- `NiSystem.lib`
- `NiMain.lib`
- `NiAnimation.lib`
- `NiParticle.lib`
- `NiCollision.lib`
- `NiPortal.lib`
- `NiVisualTracker.lib`
- `NiInput.lib`
- `NiPNGReader.lib`
- `NiDX9Renderer.lib`
- `NiD3D10Renderer.lib`
- `NiMilesAudio.lib`
- `NiD3DXEffectShaderLib.lib`
- `NiBinaryShaderLib.lib`
- `NiD3D10BinaryShaderLib.lib`
- `NSBShaderLib.lib`
- `NSFParserLib.lib`
- `zlib.lib`
- `libpng.lib`

Log de rebuild das libs criticas depois do patch `NiAtomic*64`:

```text
C:\dmo-client-main\docs\portability\gamebryo_x64_rebuild_after_niatomic64.log
```

## Erros encontrados e classificacao

| Erro | Classificacao | Resultado |
|---|---|---|
| `Qedit.h` ausente | include/lib, API antiga DirectShow | Resolvido com include legado do `DirectX_June2010\Include`; nao foi usado lib x86. |
| `SetWindowLong/GWL_WNDPROC` e `SetClassLong/GCL_HCURSOR` em x64 | 32->64, API antiga | Corrigido para `SetWindowLongPtr/GWLP_WNDPROC` e `SetClassLongPtr/GCLP_HCURSOR`. |
| `__asm int 3` no client/LibProj | dependencia x86 / codigo | Corrigido pontualmente para `__debugbreak()`. |
| `__asm` em `NiD3DUtils.h` e `NiD3D10Utility.cpp` | dependencia x86 | Adicionado fallback Win64 com cast C++ simples. |
| `d3d9.lib`, `atls.lib`, `ucrt.lib`, `msvcprt.lib` nao encontrados | configuracao | Resolvido com paths x64 de Windows SDK, VC runtime e ATL/MFC. |
| `freetype213.lib` nao encontrado em x64 | dependencia | Usado `freetype28.lib` do NuGet como substituto temporario. Requer validacao visual/runtime. |
| `dsetup.lib` sem x64 valido | API antiga / dependencia | Removido do caminho x64 e criado bypass temporario do check `DirectXSetupGetVersion` apenas em `_WIN64`. |
| `NiD3DXEffectShaderLib.lib`, `NiBinaryShaderLib.lib`, `NiD3D10BinaryShaderLib.lib`, `NSBShaderLib.lib`, `NSFParserLib.lib` ausentes | dependencia/configuracao | Configuracoes x64 geradas e libs recompiladas. |
| Colisao de objetos `PopUp.cpp`/`Popup.cpp` | configuracao | Resolvido com nomes de `.obj` distintos para x64. |
| Colisao/residuo `DigimonTalk.obj` | configuracao | Resolvido com `Root_DigimonTalk.obj` e limpeza de objeto intermediario antigo. |
| `NiAtomicIncrement/Decrement(unsigned __int64&)` indefinido | 32->64 / API Gamebryo | Corrigido em `NiSystem.inl` com `InterlockedIncrement64/InterlockedDecrement64` e rebuild das libs afetadas. |
| `IID_ID3DXEffect` e `IID_ID3DXEffectStateManager` indefinidos | include/lib / D3DX NuGet | Criado shim `D3DX9Guid.cpp` definindo somente os dois GUIDs faltantes. |

## Arquivos criticos alterados

Client:

- `C:\dmo-client-main\DProject\DProject.vcxproj`
- `C:\dmo-client-main\DProject\D3DX9Guid.cpp`
- `C:\dmo-client-main\DProject\Engine.cpp`
- `C:\dmo-client-main\DProject\App\DXVersionChecker.cpp`
- `C:\dmo-client-main\DProject\CCursor.cpp`
- `C:\dmo-client-main\DProject\_Interface\Game\MoneySeparate.cpp`
- `C:\dmo-client-main\DProject\_Interface\Game\ItemSeparate.cpp`
- `C:\dmo-client-main\DProject\_Interface\Game\ChatContents.cpp`
- `C:\dmo-client-main\DProject\network\cCliGameSend.cpp`

LibProj:

- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGamebryo2.3.vcxproj`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp`
- `C:\dmo-client-main\LibProj\CsGBChar\CsGBChar.vcxproj`

Gamebryo:

- `D:\Gamebryo2.3\CoreLibs\NiSystem\Win32\NiSystem.inl`
- `D:\Gamebryo2.3\GB_Include\NiSystem.inl`
- `D:\Gamebryo2.3\CoreLibs\NiDX9Renderer\NiD3DUtils.h`
- `D:\Gamebryo2.3\GB_Include\NiD3DUtils.h`
- `D:\Gamebryo2.3\CoreLibs\NiD3D10Renderer\NiD3D10Utility.cpp`
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiMath.inl`
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiProcessor.cpp`
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiSkinInstance_Win32.cpp`
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiPixelData.cpp`
- `D:\Gamebryo2.3\CoreLibs\NiMain\NiPersistentSrcTextureRendererData.cpp`

## Libs e DLLs faltantes para runtime

A compilacao passou, mas a pasta `C:\Release\GSP_x64` contem apenas:

```text
lk-dmo-x64.exe
lk-dmo-x64.pdb
```

Para executar fora do ambiente de build, ainda precisa copiar/garantir:

- `mss64.dll` de `C:\dmo-client-main\LibProj\MilesSoundX64\bin\mss64.dll`;
- `d3dx9_43.dll` x64 de `C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\D3DX9_43.dll`;
- `d3dx10_43.dll` x64 de `C:\dmo-client-main\packages\Microsoft.DXSDK.D3DX.9.29.952.8\build\native\release\bin\x64\d3dx10_43.dll`;
- VC runtime x64 (`MSVCP140.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll`) se a maquina alvo nao tiver redistributable instalado;
- assets normais do client: `Data\`, `dmo.ini`, packs, codecs Miles em `Data\NP\Sound`.

## Gargalos e riscos restantes

- **Miles x64 ainda nao validado em runtime.** A linkagem usa `mss64.lib` e o executavel importa `mss64.dll`, mas ainda precisa abrir o client e passar por `NiMilesAudioSystem::Startup`.
- **FreeType foi substituido.** `freetype213.lib` nao existe x64 no workspace; `freetype28.lib` permitiu linkar, mas pode mudar ABI/comportamento de texto.
- **DirectSetup foi stubado em x64.** `DirectXSetupGetVersion`/`dsetup.lib` nao sao caminho x64 moderno. O check agora assume sucesso no branch `_WIN64`; isso e aceitavel para build bridge, mas deve virar validacao explicita de DLL/runtime.
- **DX10 ainda aparece como dependencia.** Mesmo mirando DX9, Gamebryo puxa `d3dx10_43.dll` por libs/shader libs D3D10. Isso nao muda renderer, mas aumenta dependencias de runtime.
- **Warnings 32->64 continuam altos.** O rebuild critico registrou centenas de C4244/C4267/C4312/C4477. Eles nao impedem a build, mas sao riscos reais de truncamento.
- **Warning C4789 no link final.** `CsGBTerrainRoot_VersionUp.cpp(870)` indica potencial buffer overrun em `baseInfo`. Nao foi corrigido por estar fora do escopo de build minimo.
- **Nao foi feito teste de execucao.** O resultado desta etapa e compilacao/linkagem, nao estabilidade runtime.

## Bloqueadores reais

Para **compilar x64**, nao ha bloqueador restante neste ambiente: a build final concluiu.

Para **rodar x64**, os bloqueadores reais sao:

- preparar uma pasta runtime x64 com `mss64.dll`, D3DX x64 e assets;
- validar inicializacao Miles;
- validar render DX9 com D3DX NuGet x64;
- validar UI/texto por causa da troca de FreeType;
- investigar warnings 32->64 que podem causar crash ou corrupcao em runtime.

## Estimativa

- Compilar novamente neste ambiente: imediato, comando final acima.
- Preparar primeira execucao x64: 1 a 3 horas, principalmente copiando DLLs/runtime e ajustando working directory.
- Estabilizar runtime x64 inicial: 1 a 3 dias, dependendo de crashes de Miles, renderer DX9 e truncamentos 32->64.
- Auditoria seria de warnings 32->64: 3 a 7 dias se for feita com criterio e testes.

## Proxima acao recomendada

1. Criar uma pasta runtime x64 separada, sem misturar com o client Win32.
2. Copiar `lk-dmo-x64.exe`, `mss64.dll`, `d3dx9_43.dll`, `d3dx10_43.dll` e assets existentes.
3. Rodar o client x64 pelo working directory do runtime, capturando crash/logs.
4. Se abrir janela, validar ordem: Direct3D init, Pack/Data load, Miles init, login screen.
5. Depois disso, atacar warnings por prioridade: ponteiro->DWORD/int/long, formato `%d/%ld` com `size_t`/`n8`, e C4789.

## Conclusao

A etapa de compilacao x64 foi bem-sucedida com a configuracao `X64_WinDX9_Bridge|x64`. O projeto agora gera um PE32+ x64 em `C:\Release\GSP_x64\lk-dmo-x64.exe`.

O resultado ainda deve ser tratado como bridge tecnico, nao como port final: ele compila, mas precisa validacao runtime e limpeza dos shims temporarios antes de qualquer uso serio.

## Atualizacao - PathEngine x64 e rebuild pos-pack

Data/hora: 2026-06-07, depois dos asserts de font/effect/event.

### Comando de build usado

O processo antigo `lk-dmo-x64.exe` ficou vivo e `taskkill /F` retornou `Acesso negado`, bloqueando a escrita do PDB principal em `C:\Release\GSP_x64\lk-dmo-x64.pdb`. Para separar erro real de build de arquivo travado, foi feita uma build de verificacao em outro `OutDir`:

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

Log:

```text
C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_eventdata_hash_fix_retry_outdir.log
```

Resultado:

```text
DProject.vcxproj -> C:\Release\GSP_x64_retry\lk-dmo-x64.exe
```

Artefatos principais:

```text
C:\Release\GSP_x64_retry\lk-dmo-x64.exe
C:\Release\GSP_x64_retry\lk-dmo-x64.pdb
C:\Release\GSP_x64_retry\CsGamebryo2.3.lib
C:\Release\GSP_x64_retry\CsGBChar.lib
C:\Release\GSP_x64_retry\CsFilePack.lib
C:\Release\GSP_x64_retry\COMMON_R.lib
```

Conclusao desta build: o client x64 esta compilando e linkando. O `LNK1201` anterior e erro de ambiente/arquivo travado, nao erro de codigo.

### Correcoes simples de compatibilidade de dados aplicadas

Essas correcoes foram mantidas porque preservam o layout Win32 dos arquivos ja empacotados e explicam os asserts vistos em runtime:

- `C:\dmo-client-main\LibProj\CsFilePack\CsFileHash.h`
  - `sINFO::s_nDataSize`, `sINFO::s_nAllocSize`, `sINFO::s_nHash`, `sINFONew::s_nHash`, `sINFONew::s_nDataSize`, `sINFONew::s_nAllocSize`: `size_t` -> `DWORD`.
  - Motivo: os `.hf` atuais foram gerados com layout Win32. Em x64, `size_t` inflava o struct e quebrava a leitura do Pack01.
- `C:\dmo-client-main\LibProj\CsFilePack\FPHeader.cpp`
  - `GetHashCode(LPCSTR/LPCWSTR)` agora acumula em `DWORD` e retorna `size_t`.
  - Motivo: manter o hash persistido em 32 bits igual ao pack existente.
- `C:\dmo-client-main\LibProj\CsGBChar\EffectSound.h`
  - `sES_Info::HashCode`: `size_t` -> `DWORD`.
  - Motivo: corrigir desalinhamento que disparava assert em `EffectSoundMng.cpp:145`.
- `C:\dmo-client-main\LibProj\CsGBChar\EventData.h`
  - `sEventEffect_Info::s_EffectHashCode`: `size_t` -> `DWORD`.
  - Motivo: corrigir desalinhamento que disparava assert em `EventDataMng.cpp:153`.

### Estado dos erros runtime

| Sintoma | Classificacao | Estado |
|---|---|---|
| `FT_New_Face failed data\font\tahoma.ttf` / fonte do Pack01 nao carregava | 32->64 / formato persistido | Corrigido por manter campos de pack/hash em 32 bits. O log passou a mostrar `packOnly=1` e dados de fonte vindos do pack. |
| Assert `EffectSoundMng.cpp:145` | 32->64 / formato persistido | Corrigido em header e rebuildou com sucesso. Precisa confirmacao em runtime depois do PathEngine. |
| Assert `EventDataMng.cpp:153` | 32->64 / formato persistido | Patch aplicado e build de verificacao passou. Precisa confirmacao em runtime depois do PathEngine. |
| `Error: failed calling LoadLibrary() for pathengine` | dependencia | Bloqueador atual de runtime. Falta `PathEngine.dll` x64 compativel. |
| `LINK : fatal error LNK1201` ao escrever `lk-dmo-x64.pdb` na saida principal | ambiente/configuracao local | Causado por processo antigo vivo segurando arquivos. Build em `C:\Release\GSP_x64_retry` passou. |

### Avaliacao do repo `sigmaco/pathengine`

Repo analisado:

```text
https://github.com/sigmaco/pathengine
C:\0-NewDmo\third_party\pathengine-sigmaco
Commit: 8b4ea08
```

Achados:

- O repo contem `launchers_Windows-x64` e binarios x64 de testbed/demo.
- O repo nao contem `PathEngine.dll` standalone.
- Os binarios x64 encontrados sao `TestBed.exe`, `CP3DDemo_Integrated.exe` e DLLs de aplicacoes de teste em `build\x64\Release\testbedApp`.
- `dumpbin /dependents build\x64\Release\dxExe\TestBed.exe` nao mostra dependencia de `PathEngine.dll`; o testbed parece embutir o runtime.
- `dumpbin /exports build\x64\Release\dxExe\TestBed.exe` nao exporta `DLLExport_GetIPathEngine`.
- `BasicApplication.dll` exporta apenas `TestApplicationEntryPoint`.

Incompatibilidade de ABI:

```text
C:\dmo-client-main\LibProj\PathEngine\i_pathengine.h
PATHENGINE_INTERFACE_MAJOR_VERSION = 5
PATHENGINE_INTERFACE_MINOR_VERSION = 2

C:\0-NewDmo\third_party\pathengine-sigmaco\code\externalAPI\i_pathengine.h
PATHENGINE_INTERFACE_MAJOR_VERSION = 18
PATHENGINE_INTERFACE_MINOR_VERSION = 0
```

Assinatura critica diferente:

```text
DMO/5.2: iShape* newShape(tSigned32 numberOfPoints, const tSigned32* coordinateData)
Repo/18.0: iShape* newShape(tSigned32 const* shapeCoordinates_Buffer, tUnsigned32 shapeCoordinates_BufferEntries)
```

Conclusao: esse repo nao e drop-in para o DMO. Usar o header/runtime 18.0 contra o client 5.2 tende a quebrar vtable e chamadas virtuais. Nao foi aplicado nenhum stub ou troca de header.

### DLL x86 original validada

Arquivo:

```text
C:\0-NewDmo\ClientDist1\PathEngine.dll
SHA256: 4230BEE6189A61FD8F6B89098AA2BF28A878A35CA463F238417ADC724DF4142A
```

`dumpbin`:

```text
14C machine (x86)
time date stamp Fri Jan 8 09:15:01 2010
```

Exports:

```text
ordinal 1 DLLExport_GetIPathEngine
ordinal 2 DLLExport_RefWrapperPointer
ordinal 3 DllExport_GetIPathEngine
```

Strings internas relevantes confirmam checagem de interface:

```text
interface_major_version
interface_minor_version
collision_preprocess_major_version
pathfind_preprocess_major_version
DLLExport_GetIPathEngine
```

### Fontes externas verificadas

- `https://github.com/sigmaco/pathengine`: repo base/testbed com x64, mas sem DLL standalone compativel.
- `https://pathengine.com/downloads/`: a pagina oficial separa `PathEngine_SDKBase_06_04.zip` de `PathEngine_Binaries_x64_06_04.zip` e dos pacotes de source.
- `https://pathengine.com/Contents/Overview/SDKContentsandInstallation/page.php`: a documentacao oficial informa que o SDK base inclui testbed e exemplos, mas nao inclui os binarios standalone para linkar o runtime fora do testbed.
- `https://pathengine.com/Contents/ProgrammersGuide/LinkingwiththeSDK/LinkingwiththePathEngineDLL/page.php`: a documentacao oficial confirma que o uso standalone requer a DLL PathEngine completa e entrada por `LoadLibrary/GetProcAddress`.

### Sobre "fake x64" para PathEngine

Nao foi criado fake/stub x64 de `PathEngine.dll`.

Motivo tecnico:

- Um stub que apenas retorna ponteiros falsos passaria do `LoadLibrary`, mas quebraria em chamadas como `newShape`, `loadMeshFromBuffer`, `newContext`, `placeAgent`, `findShortestPathTo`, `testCollisionTo` e `advanceAlongPath`.
- Um stub funcional alteraria pathfinding, colisao e movimento. Isso entra em logica de jogo/runtime, fora da regra de mudancas minimas.
- O client usa dados de terreno e agentes dinamicos; ignorar PathEngine pode permitir abrir a tela seguinte, mas invalida qualquer teste de mapa/movimento.

Acao segura: registrar como bloqueador e exigir DLL/source x64 compativel com SDK/interface 5.2.

### Bloqueadores reais atuais

1. Fechar o processo antigo `lk-dmo-x64.exe` que esta preso e impede escrita no PDB da saida principal.
2. Obter `PathEngine.dll` x64 standalone compativel com o ABI 5.2 usado pelo client.
3. Validar exports da DLL x64 esperada:

```powershell
dumpbin /headers PathEngine.dll
dumpbin /exports PathEngine.dll
```

Checklist esperado:

```text
8664 machine (x64)
ordinal 1 DLLExport_GetIPathEngine
```

4. Validar que o header/runtime tem `PATHENGINE_INTERFACE_MAJOR_VERSION=5` e `MINOR_VERSION=2`, ou confirmar compatibilidade binaria equivalente.

### Proxima acao recomendada

Prioridade 1: conseguir a versao x64 do mesmo SDK PathEngine usado em 2010 pelo client, ou o source package correspondente, e nao a distro 18.0 do repo `sigmaco/pathengine`.

Depois disso:

1. Copiar a DLL x64 real como `C:\0-NewDmo\ClientDist1_x64\PathEngine.dll`.
2. Rodar `C:\Release\GSP_x64_retry\lk-dmo-x64.exe` com working directory `C:\0-NewDmo\ClientDist1_x64`.
3. Confirmar se os asserts de `EffectSoundMng.cpp:145` e `EventDataMng.cpp:153` desapareceram.
4. Se passar da inicializacao de mapas, iniciar a auditoria de warnings 32->64 restantes.

## Atualizacao - PathEngine.dll x64 encontrada no workspace

Arquivo encontrado:

```text
C:\dmo-client-main\PathEngine.dll
SHA256: FEA139A20C65BCB0B740E61E8ECE60FD92CE0E8CE83F53D5C8ABF9343477C656
```

`dumpbin /headers`:

```text
8664 machine (x64)
time date stamp Fri Apr 5 03:49:00 2019
12.00 linker version
```

`dumpbin /exports`:

```text
ordinal 1 DLLExport_GetIPathEngine
ordinal 2 DLLExport_RefWrapperPointer
ordinal 3 DllExport_GetIPathEngine
```

Dependencias:

```text
KERNEL32.dll
MSVCP120.dll
MSVCR120.dll
```

As DLLs `MSVCP120.dll` e `MSVCR120.dll` existem em `C:\Windows\System32`, portanto a dependencia VS2013 x64 esta disponivel nesta maquina.

Teste nativo direto via `LoadLibraryA` + `GetProcAddress((LPCSTR)1)`:

```text
interface=5.7
release=5.29.0
buildstring=2012-01-13 14:53
shapeIsValid=1
newShape=<ponteiro valido>
shapeSize=8
```

Comparacao com o header do client:

```text
Client header: interface 5.2
DLL encontrada: interface 5.7 / release 5.29.0
```

Classificacao: dependencia x64 promissora, mas ainda requer validacao runtime. O major da interface e igual (`5`), e as chamadas iniciais usadas pelo client (`getInterface*`, `shapeIsValid`, `newShape`) funcionaram no tester. Como o minor e maior (`5.7` vs `5.2`), tratar como possivel compatibilidade por extensao, nao como prova absoluta.

Acao feita:

```text
C:\dmo-client-main\PathEngine.dll
-> C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
```

Smoke test:

```powershell
Start-Process `
  -FilePath "C:\Release\GSP_x64_retry\lk-dmo-x64.exe" `
  -WorkingDirectory "C:\0-NewDmo\ClientDist1_x64"
```

Resultado observado:

- O erro `Error: failed calling LoadLibrary() for pathengine` nao reapareceu.
- O client avancou para o proximo gargalo e gerou dump:

```text
C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\DProject_R_ENG_20260607_043652_11252.dmp
```

### Lock de processo segurando packs/logs

O processo antigo continua vivo:

```text
PID: 6572
ProcessName: lk-dmo-x64
StartTime: 2026-06-07 04:15:09
MainWindowHandle: 0
```

Tentativas de encerramento:

```text
Stop-Process -Id 6572 -Force -> Acesso negado
taskkill /PID 6572 /T /F -> Acesso negado
Restart Manager RmShutdown(force) -> 350, processo continuou vivo
```

Restart Manager confirmou que o PID `6572` esta segurando recursos registrados, incluindo packs/log/PDB:

```text
Pid  strAppName      ApplicationType  bRestartable
6572 lk-dmo-x64.exe  RmCritical       False
```

Arquivos registrados no teste de lock:

```text
C:\0-NewDmo\ClientDist1_x64\Data\Pack01.hf
C:\0-NewDmo\ClientDist1_x64\Data\Pack01.pf
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.hf
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.pf
C:\0-NewDmo\ClientDist1_x64\logs\crash.log
C:\Release\GSP_x64\lk-dmo-x64.pdb
```

Classificacao: erro de ambiente/processo preso. Enquanto o PID `6572` existir, testes de pack/runtime podem apresentar falso erro de load e logs podem ficar ilegíveis.

Proxima acao antes de novo teste:

1. Encerrar manualmente o PID `6572` pelo Gerenciador de Tarefas elevado, ou reiniciar a maquina/sessao.
2. Confirmar:

```powershell
Get-Process lk-dmo-x64 -ErrorAction SilentlyContinue
```

3. Rodar novamente:

```powershell
Start-Process `
  -FilePath "C:\Release\GSP_x64_retry\lk-dmo-x64.exe" `
  -WorkingDirectory "C:\0-NewDmo\ClientDist1_x64"
```

## Atualizacao 2026-06-07 04:52 - Crash pos-login em Miles x64

Depois que o erro de `PathEngine.dll` desapareceu e o assert de
`EventDataMng.cpp:153` parou de ocorrer, o crash seguinte foi analisado pelos
logs do client e pelo Event Viewer do Windows.

### Crash log do client

Arquivo:

```text
C:\0-NewDmo\ClientDist1_x64\logs\crash.log
```

Ultimas linhas da execucao de `2026-06-07 04:45:58`:

```text
[2026-06-07 04:46:01.371] GAMEAPP net start end
[2026-06-07 04:46:01.371] GAMEAPP StartFlow login begin
[2026-06-07 04:46:01.371] GAMEAPP StartFlow end
[2026-06-07 04:46:01.371] GAMEAPP OnInitialize end
```

O `crash.log` nao registrou stack/exception depois disso.

### Event Viewer

Evento `Application Error`, `Id=1000`, `2026-06-07 04:46:04`:

```text
Aplicativo: C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
Modulo com falha: C:\0-NewDmo\ClientDist1_x64\mss64.dll
Versao do modulo: 7.0.24.0
Codigo de excecao: 0xc0000005
Offset: 0x0000000000070812
```

Classificacao: erro de dependencia/runtime x64 em Miles.

### LocalDumps habilitado

Foi habilitado dump local via WER por usuario:

```text
HKCU\Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\lk-dmo-x64.exe
DumpFolder = C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\WER
DumpCount  = 10
DumpType   = 2
```

### Validacao da cadeia Miles

`mss64.dll` no runtime:

```text
C:\0-NewDmo\ClientDist1_x64\mss64.dll
SHA256: 97165D7396723F5ECA279EF9A30EFDCFA93C162D06C5ECF3BBC1987C48886F66
PE: x64 / PE32+
Dependencias: USER32.dll, KERNEL32.dll, WINMM.dll
```

O DLL do runtime bate com:

```text
C:\dmo-client-main\LibProj\MilesSoundX64\bin\mss64.dll
```

`mss64.lib` tambem foi validada como x64:

```text
C:\dmo-client-main\LibProj\MilesSoundX64\lib\mss64.lib
machine: x64
```

O link command da configuracao `X64_WinDX9_Bridge|x64` confirma:

```text
/LIBPATH:"C:\DMO-CLIENT-MAIN\LIBPROJ\MILESSOUNDX64\LIB" MSS64.LIB
```

### Gargalo encontrado

Antes do ajuste, o diretorio usado por `CSoundMgr::_InitMiles()`:

```cpp
pkSS->Startup("Data\\NP\\Sound")
```

continha plugins Miles x86:

```text
mssdolby.flt  -> x86 / PE32
mssds3d.flt   -> x86 / PE32
mssdsp.flt    -> x86 / PE32
msseax.flt    -> x86 / PE32
mssmp3.asi    -> x86 / PE32
msssrs.flt    -> x86 / PE32
mssvoice.asi  -> x86 / PE32
```

Isso e incompatvel com um processo x64 carregando `mss64.dll`.

### Ajuste aplicado no runtime

Mudanca reversivel, sem alterar source/build:

```text
Backup dos plugins x86:
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\_x86_backup_20260607_045219
```

Plugins x64 copiados do SDK Miles:

```text
C:\0-NewDmo\dmo-client-main\LibProj\MilesSound\Mss\RAD Game Tools Access until 2018-12-06 for colin@movegames.co.kr\MilesSDKWindows\redist64
C:\0-NewDmo\dmo-client-main\LibProj\MilesSound\Mss\RAD Game Tools Access until 2018-12-06 for colin@movegames.co.kr\MilesSDKWindowsMP3\redist64
```

Estado atual de `C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound`:

```text
mss64dolby.flt  x64 / PE32+
mss64ds3d.flt   x64 / PE32+
mss64dsp.flt    x64 / PE32+
mss64eax.flt    x64 / PE32+
mss64mp3.asi    x64 / PE32+
mss64srs.flt    x64 / PE32+
```

### Proxima validacao

Rodar novamente:

```powershell
Start-Process `
  -FilePath "C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe" `
  -WorkingDirectory "C:\0-NewDmo\ClientDist1_x64"
```

Se ainda houver crash, coletar o dump novo em:

```text
C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\WER
```

## Atualizacao posterior - Miles e PathEngine

### Miles x64

Os providers Miles x64 9.4d copiados para `Data\NP\Sound` foram desativados depois da validacao. Eles carregavam dentro de `mss64.dll` 7.0x, mas causavam crash em providers como `mss64mp3.asi` e `mss64dolby.flt`. Isso indica incompatibilidade entre a DLL Miles 7.0x usada pelo Gamebryo e providers Miles 9.4d.

Estado atual:

```text
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\_mss64_94d_disabled_20260607_0455
```

O build `X64_WinDX9_Bridge` esta com Miles stubado em:

```text
C:\dmo-client-main\DProject\CSoundMgr.cpp
```

Esse stub e temporario e reversivel. O objetivo foi liberar validacao do resto do client x64 sem crash imediato em audio.

### PathEngine

O proximo erro observado foi:

```text
iPathEngine::newShape() failed - vertex coordinate exceeds allowed range. Returning zero.
```

Classificacao:

```text
32->64 / serializacao de layout Win32
```

Foi aplicada correcao minima em:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBObject.cpp
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp
```

Os blobs de path info gravados em Win32 agora sao lidos com headers de disco fixos em x64, evitando que ponteiros de 8 bytes alterem os offsets dos arrays usados pelo PathEngine.

Build validado:

```text
C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_win32_disk_layout.log
C:\Release\GSP_x64\lk-dmo-x64.exe
```

Executavel copiado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Relatorio tecnico especifico:

```text
C:\dmo-client-main\docs\portability\13_X64_SERIALIZATION_LAYOUT_AUDIT.md
```

## Atualizacao posterior - Miles 9.4d habilitado

O usuario adicionou a DLL correta:

```text
C:\dmo-client-main\mss64.dll
```

Validacao:

```text
ProductVersion: 9.4d
SHA256: CD9DBECE13DA468636308D8C424FF9C30B9A40F2772ABE298F6C09AE0FC01500
```

Foi recompilado `NiMilesAudio.lib` para `X64_WinDX9_Bridge|x64`, removendo no caminho x64 as chamadas para `AIL_quick_startup`, `AIL_quick_shutdown` e `AIL_quick_handles`, que nao existem na `mss64.dll` 9.4d.

O stub temporario em `CSoundMgr::_InitMiles()` foi removido. O client voltou a chamar:

```text
NiMilesAudioSystem::Startup("Data\\NP\\Sound")
```

Build validado:

```text
C:\dmo-client-main\docs\portability\nimilesaudio_x64_94d_build.log
C:\dmo-client-main\docs\portability\dproject_sln_x64_build_miles94d_enabled.log
```

Runtime atualizado com core/providers Miles 9.4d x64:

```text
C:\0-NewDmo\ClientDist1_x64\mss64.dll
C:\0-NewDmo\ClientDist1_x64\mss64midi.dll
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64*.flt
C:\0-NewDmo\ClientDist1_x64\Data\NP\Sound\mss64mp3.asi
```

Smoke test:

```text
SMOKE_ALIVE_AFTER_20S
NO_NEW_DUMPS
```

Relatorio detalhado:

```text
C:\dmo-client-main\docs\portability\14_MILES_X64_94D_AUDIO_RESTORE.md
```
