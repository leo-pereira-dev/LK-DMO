# 18 - X64 PathEngine Fake Bridge ABI 5.2

Data: 2026-06-07

## Objetivo

Criar uma ponte/fake `PathEngine.dll` x64 com ABI 5.2, sem tratar a DLL x64
atual 5.29 como compativel com o client.

Escopo mantido:

- Renderer nao alterado.
- Logica de jogo nao alterada.
- Protocolo de rede nao alterado.
- Client nao recompilado para esta troca.
- Mudanca reversivel por troca de DLL no runtime.

## Resumo tecnico

O client carrega PathEngine por:

```cpp
LoadLibrary("pathengine")
GetProcAddress(hInstance, (LPCSTR)1)
```

A ponte criada exporta:

```text
ordinal 1: DLLExport_GetIPathEngine
ordinal 2: DLLExport_RefWrapperPointer
ordinal 3: DllExport_GetIPathEngine
```

A interface retornada anuncia:

```text
runtime=5.2
release=5.23.0
```

## Performance esperada

Nao deve haver perda relevante de desempenho x64 nesta opcao porque a ponte e
in-process: o client chama metodos virtuais C++ normais dentro da propria DLL
x64. Nao ha processo x86 separado, IPC, pipe, socket ou marshal pesado.

O risco real nao e performance. O risco real e funcionalidade: esta ponte ainda
nao e uma PathEngine completa. Ela implementa colisao 2D basica, bounds do XML,
obstaculos base simples e path direto/visibilidade simples. Mapas com regras
complexas de navmesh podem precisar de mais implementacao.

## Evidencia da incompatibilidade anterior

DLL x64 anterior:

```text
C:\dmo-client-main\PathEngine.dll
SHA256: FEA139A20C65BCB0B740E61E8ECE60FD92CE0E8CE83F53D5C8ABF9343477C656
runtime=5.7
release=5.29.0
```

Sintoma confirmado no probe e no client:

```text
agent.findShortestPathTo path=NULL
PATH-USER CalTargetPath fail reason=find_path_null validEmr=1 pathTest=1
```

Conclusao: o problema de movimento nao estava em pacote, agente ou rede nesse
ponto. A DLL PathEngine x64 5.29 nao respondia como a ABI/header 5.2 esperado
pelo client.

## Arquivos criados

```text
C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.cpp
C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.def
C:\dmo-client-main\tools\pathengine_fake_x64\bin\PathEngine.dll
```

## Comando de build usado

```powershell
$src='C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.cpp'
$def='C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.def'
$out='C:\dmo-client-main\tools\pathengine_fake_x64\bin'
$vc='C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat'
cmd.exe /c "call `"$vc`" x64 >nul && cl /nologo /EHsc /W3 /MD /LD /I`"C:\dmo-client-main\LibProj\PathEngine`" /Fo`"$out\PathEngineFake.obj`" /Fe`"$out\PathEngine.dll`" `"$src`" /link /DEF:`"$def`""
```

Resultado:

```text
Build OK
C:\dmo-client-main\tools\pathengine_fake_x64\bin\PathEngine.dll
```

## Validacao por probe

Comando:

```powershell
& C:\dmo-client-main\docs\portability\probe_bin\pathengine_probe_x64.exe `
  C:\0-NewDmo\ClientDist1_x64\PathEngine.dll `
  "D:\LK-DMO TOOLS\EXTRATOR\Pack01\Data\Map\RealWorld_R\01_Yokohama\105\Map_Data\105.xml" `
  39676 12280 40707 12280
```

Resultado importante:

```text
header=5.2 runtime=5.2 release=5.23.0
[raw-cell0] mesh.findShortestPath path=... size=2 length=1031
[raw-cell0] agent.findShortestPathTo path=... size=2 length=1031
[positionFor3DPoint] agent.findShortestPathTo path=... size=2 length=1031
[positionNear3DPoint] agent.findShortestPathTo path=... size=2 length=1031
```

## Deploy aplicado

Runtime alterado:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
```

Backup correto da DLL 5.29 original:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.5_29_x64.bak
```

Hashes apos deploy:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
SHA256: BAB4F4EB5AC1D9C7E6185E62F51BAD3C8775D139D8063B140C5AAF8B65F4E1E1

C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.5_29_x64.bak
SHA256: FEA139A20C65BCB0B740E61E8ECE60FD92CE0E8CE83F53D5C8ABF9343477C656
```

## Reversao

Para voltar para a DLL x64 5.29 anterior:

```powershell
Copy-Item `
  "C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.5_29_x64.bak" `
  "C:\0-NewDmo\ClientDist1_x64\PathEngine.dll" `
  -Force
```

## Checklist DMO bug

- Packet mismatches: nao indicado para este sintoma; clique e fluxo de movimento
  chegavam ao calculo de path.
- Race conditions: nao indicado pelo probe; falha reproduz fora do client.
- Null checks: o NULL relevante vinha de `findShortestPathTo`.
- Game logic: sem alteracao.
- Pipelines: carregamento por ordinal 1 validado.
- Overflow: evitado na ponte com calculos de segmento usando `long long`.

## Bloqueadores e riscos reais

- A ponte nao substitui a PathEngine completa.
- A implementacao atual nao interpreta toda semantica do mesh 5.x.
- Off-mesh connections, terrain layers, surface costs e anchors sao stubs.
- Obstaculos dinamicos sao considerados de forma simples.
- Se um mapa depender de navmesh complexo, o movimento pode ficar permissivo ou
  falhar em bordas/obstaculos especificos.

## Proxima acao recomendada

Testar o client em `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe` com a ponte
instalada e verificar:

1. Mapa carrega apos selecao de personagem.
2. Clique para mover cria path e altera posicao.
3. Monstros voltam a andar sem desativacao de movimento.
4. Alt+Tab nao causa crash novo.
5. Logs nao exibem mais `find_path_null` para cliques validos.

Se o movimento voltar, o proximo passo e ampliar a fake bridge por casos reais
de mapa/obstaculo, guiado por logs e probe, em vez de mexer no agente ou nos
pacotes.

## Rollback das tentativas anteriores

Depois que a ponte fake funcionou, foram desfeitas as alteracoes descritas em:

```text
C:\dmo-client-main\docs\portability\17_X64_PATHENGINE_MOVEMENT_CRASH.md
```

Arquivos restaurados a partir da copia limpa:

```text
C:\0-NewDmo\dmo-client-main\LibProj\CsGamebryo2.3\PathEngine.h
C:\0-NewDmo\dmo-client-main\LibProj\CsGamebryo2.3\PathEngine.cpp
C:\0-NewDmo\dmo-client-main\LibProj\CsGBChar\PathProperty.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameSync.cpp
```

Tambem foram removidos os traces temporarios:

```text
MOVE-TAMER
MOVE-DIGIMON
PATH-USER
SYNC-MONSTER x64 skip network-thread path
CsPathEngineMutex
```

Build apos rollback:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_rollback_md17.log;Verbosity=normal"
```

Resultado:

```text
Build OK
C:\Release\GSP_x64\lk-dmo-x64.exe
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Estado mantido:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll = fake ABI 5.2
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.5_29_x64.bak = DLL x64 5.29 original
```

## Ajuste 2026-06-07 - colisao por shape

Sintoma:

```text
Player movia pelo mapa, mas atravessava/entrava em balcões, paredes finas ou
objetos de cena.
```

Causa na fake:

```text
testPointCollision(shape, context, position) e findShortestPathTo() estavam
tratando colisao principalmente como ponto central. O client, porem, adiciona
obstaculos via SetShape()/SetDynamicShape() e espera teste de shape contra
shape/contexto.
```

Mudanca aplicada em:

```text
C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.cpp
```

Resumo:

- Adicionado overlap de poligonos (`PolysOverlap`).
- `testPointCollision()` agora testa o shape do agente contra obstaculos
  estaticos e agentes/obstaculos do contexto.
- `findShortestPathTo()`/line tests agora usam sweep simples do shape, nao so
  a linha do centro.
- `placeAgent()` agora valida o shape no ponto inicial.
- `firstCollisionTo()` e `testCollisionTo()` passaram a considerar shape.

Build da DLL:

```text
Build OK
C:\dmo-client-main\tools\pathengine_fake_x64\bin\PathEngine.dll
SHA256: B9AA96192C2EFA7DBA205848396E6EE23BD4D725C6EF642BCB0EAAE3CAFA9E23
```

Deploy:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
SHA256: B9AA96192C2EFA7DBA205848396E6EE23BD4D725C6EF642BCB0EAAE3CAFA9E23
```

Backups preservados:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.fake_pre_shape_collision.bak
SHA256: BAB4F4EB5AC1D9C7E6185E62F51BAD3C8775D139D8063B140C5AAF8B65F4E1E1

C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.5_29_x64.bak
SHA256: FEA139A20C65BCB0B740E61E8ECE60FD92CE0E8CE83F53D5C8ABF9343477C656
```

Probe:

```text
header=5.2 runtime=5.2 release=5.23.0
agent.findShortestPathTo path=... size=2 length=1031
```

## Ajuste 2026-06-07 - SetShape de objeto de mapa

Sintoma:

```text
Ao carregar D-Terminal, o client mostrava uma caixa para:
.\Data\Map\RealWorld\D_terminal\D_terminel_E_06_Cmain2.nif

Mensagem coreana original:
<object path>
Objeto com linha PathEngine invalida.
```

Origem no codigo:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBObjectResMng.cpp
CsGBObjectResMng::GetObject()

g_pCsPathEngine->SetShape(...) retornava false e acionava MessageBoxA.
```

Classificacao:

```text
Pipeline / compatibilidade da fake PathEngine x64.
Nao aponta para pacote de rede, servidor, renderer ou logica de combate.
```

Causa na fake:

```text
FakeMesh::placeAgent() recusava qualquer shape que sobrepusesse colisao
estatica do mesh.

Mas CsPathEngine::SetShape() usa placeAgent() para registrar shapes de colisao
de objetos do proprio mapa. Esses shapes podem encostar ou sobrepor a malha
estatica por design; a PathEngine original aceita esse cadastro e depois usa o
contexto para as consultas de colisao/path.
```

Mudanca aplicada:

```text
C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.cpp

FakeMesh::placeAgent() agora rejeita apenas uma ancora fora dos limites do
mesh. A validacao de overlap continua nas consultas testPointCollision,
testLineCollision, firstCollision e pathfind.
```

Build/deploy:

```text
C:\dmo-client-main\tools\pathengine_fake_x64\bin\PathEngine.dll
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
SHA256: 3279013752381697F55513409883D844DAE36BCE0EA3A70289FA8E24B9205192
```

Backup preservado:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.pre_placeagent_relax_20260607_073944.bak
```

Probe basico:

```text
header=5.2 runtime=5.2 release=5.23.0
mesh.findShortestPath path=... size=2 length=1031
agent.findShortestPathTo path=... size=2 length=1031
```

Client build depois do ajuste:

```text
Build OK
C:\Release\GSP_x64\lk-dmo-x64.exe
Log: C:\dmo-client-main\docs\portability\dproject_sln_x64_build_after_fractional_path_fix.log
```

Observacao runtime:

```text
Copia do exe limpo para C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe falhou
porque o client ja estava aberto. A DLL nova ja estava copiada antes do
processo lk-dmo-x64 iniciar.
```

## Ajuste 2026-06-07 - movimento fracionario do path

Sintoma:

```text
Player conseguia mover, colisoes estavam melhores, mas monstros ficavam
parados mesmo em area livre. O modelo podia entrar em animacao de movimento,
mas a posicao nao avancava.
```

Causa na fake:

```text
FakePath::Advance() atualizava points[0].x/y como inteiros a cada frame.
Monstros em modo walk usam velocidade reduzida no client
CsC_PathProp::GetMoveSpeed() => speed * 0.25.

Quando speed * deltaTime ficava abaixo de 1 unidade por frame, o avanco era
truncado para a mesma coordenada inteira a cada chamada. A fracao era perdida,
entao o path permanecia ativo e a animacao tocava, mas o agente nao saia do
lugar.
```

Mudanca aplicada em:

```text
C:\dmo-client-main\tools\pathengine_fake_x64\PathEngineFake.cpp
```

Resumo:

- `FakePath` agora mantem `preciseX/preciseY` como acumulador interno em
  `double`.
- `advanceAlongPath()` continua expondo `cPosition` inteiro para o client, mas
  nao perde mais os avancos menores que 1 unidade entre frames.
- Nenhuma alteracao nova foi aplicada em renderer, protocolo de rede ou logica
  de jogo para esse ajuste.

Build da DLL:

```text
Build OK
C:\dmo-client-main\tools\pathengine_fake_x64\bin\PathEngine.dll
SHA256: 02BC4BEBB7A57B0B64AD5D45EE9D1D41C5512F1D9218348FB25DD092A47C9930
```

Deploy:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll
SHA256: 02BC4BEBB7A57B0B64AD5D45EE9D1D41C5512F1D9218348FB25DD092A47C9930
```

Backup preservado antes desse deploy:

```text
C:\0-NewDmo\ClientDist1_x64\PathEngine.dll.fake_shape_collision_pre_fractional.bak
```

Probe:

```text
header=5.2 runtime=5.2 release=5.23.0
agent.findShortestPathTo path=... size=2 length=1031
```
