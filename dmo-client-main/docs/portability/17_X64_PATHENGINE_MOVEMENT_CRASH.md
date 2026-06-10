# X64 PathEngine Movement Crash

Data: 2026-06-07

## Sintoma

No build `X64_WinDX9_Bridge`, o personagem entrou no mapa, ficou travado sem
andar e o cliente crashou depois de um Alt+Tab.

Pelo crash log, o Alt+Tab parece coincidencia de timing. A stack nao aponta para
device loss, renderer ou reset de D3D; aponta para movimento/pathfinding.

## Evidencia principal

Log:

```text
C:\0-NewDmo\ClientDist1_x64\logs\crash.log
```

Crash 1:

```text
2026-06-07 05:31:26.814
packet=1006 body_bytes=23
last_context=RecvSyncData dispatch syncType=5

lk-dmo-x64.exe!CsC_PathProp::SetPath
lk-dmo-x64.exe!cCliGame::SyncMoveMonster
lk-dmo-x64.exe!cCliGame::SyncWalkObject
lk-dmo-x64.exe!cCliGame::RecvSyncData
lk-dmo-x64.exe!cCliGame::OnExecute
lk-dmo-x64.exe!cClient::ReadAll::<lambda>
```

Crash 2 no mesmo dump:

```text
pathengine.DLL+0x9121b
pathengine.DLL+0x91189
pathengine.DLL+0x8868c
lk-dmo-x64.exe!CsC_PathProp::PreUpdate
lk-dmo-x64.exe!CsC_AvObject::Update
lk-dmo-x64.exe!CMonster::Update
lk-dmo-x64.exe!CCharMng::_UpdateMonster
```

Dump:

```text
C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\DProject_R_ENG_20260607_053127_9500.dmp
```

## Diagnostico

`nlib\client.cpp` executa `OnExecute()` diretamente no callback do ASIO:

```text
nlib\client.cpp:396 -> OnExecute()
```

Com isso, `RecvSyncData()` e `SyncMoveMonster()` rodam no thread de rede. Ao
mesmo tempo, o loop principal roda `CMonster::Update()` -> `CsC_PathProp::PreUpdate()`.

As duas stacks acessam o mesmo `CsPath` e a mesma `pathengine.DLL`:

- thread de rede: cria/substitui path em `SetPath()`;
- thread principal: avanca o path em `PreUpdate()`.

Essa e a explicacao mais forte para o par de crashes no mesmo dump. Em um fluxo
saudavel, `agent`, `context` e `path` nao deveriam estar nulos durante movimento
valido. Null guard aqui nao e a causa raiz nem uma regra de jogo; e apenas
contencao para impedir AV enquanto confirmamos a corrida.

## Checklist DMO

- Packet mismatches: packet `1006` envolvido, mas a stack mostra dispatch valido
  ate `SyncWalkObject`. Sem evidencia atual de parse errado.
- Race conditions: confirmado como suspeita principal; rede altera path enquanto
  main thread atualiza path.
- Null checks: guardas adicionados para impedir AV em estado invalido, mas nulo
  deve ser tratado como sintoma.
- Game logic: sem alteracao de regra de movimento.
- Pipelines: pipeline afetado e PathEngine/movement update.
- Overflow: nao ha overflow novo confirmado neste crash; segue pendente o warning
  antigo em `CsGBTerrainRoot_VersionUp.cpp`.

## Mudancas aplicadas

Arquivos:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\PathEngine.h
C:\dmo-client-main\LibProj\CsGamebryo2.3\PathEngine.cpp
C:\dmo-client-main\LibProj\CsGBChar\PathProperty.cpp
```

Resumo:

- Adicionado `std::recursive_mutex` global do wrapper `CsPathEngine`.
- Inicializado `CsPath::m_nPathSize = 0`.
- `CsPath::DeletePath()` e `DeleteAgent()` agora usam a mesma trava.
- `CsPathEngine::SetPath()`, `UpdatePath()`, `PathTest()`,
  `FindShortestPathInfo()`, `SetShape()`, `SetDynamicShape()`,
  `DelDynamicShape()` e `ResetContext()` foram serializados.
- `CsC_PathProp::SetPath()`, `PreUpdate()` e `_PreUpdate_AbsolutePath()` foram
  serializados no mesmo mutex.
- Estados invalidos (`agent/context/path/mesh` nulos ou posicao invalida) agora
  retornam sem chamar a DLL.
- `SetPath()` registra breadcrumbs `PATH SetPath skipped ...` no crash log se
  cair em estado invalido.

## Build

Comando usado:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_pathengine_mutex.log;Verbosity=normal"
```

Resultado:

```text
451 Aviso(s)
0 Erro(s)
Tempo Decorrido 00:02:07.18
```

Output:

```text
C:\Release\GSP_x64\lk-dmo-x64.exe
```

Runtime atualizado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Timestamp do runtime:

```text
2026-06-07 05:38:41
```

## Warnings relevantes

O warning antigo continua:

```text
CsGBTerrainRoot_VersionUp.cpp(870): warning C4789:
buffer 'baseInfo' of size 68 bytes will be overrun; 28 bytes will be written starting at offset 52
```

Nao foi corrigido nesta rodada porque nao aparece na stack deste crash.

## Classificacao

Erro principal:

```text
erro de codigo / race condition / pipeline de movimento
```

Nao classificado como:

- renderer;
- protocolo de rede confirmado;
- Miles/audio;
- DirectX;
- API antiga.

## Bloqueadores reais

Ainda falta validar em runtime se a serializacao elimina o crash.

Se o crash sumir mas o personagem continuar travado, o proximo bloqueador real
vira dados de path/mapa em x64, especialmente:

```text
TRROOT LoadExtraData path disk32 groups=14 totalPE=0 dataTotal=112
```

`totalPE=0` com `dataTotal=112` precisa ser revalidado contra o layout Win32 do
extra data de terreno.

## Proxima acao recomendada

Rodar:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Entrar no mesmo personagem/mapa e testar:

- andar com o personagem;
- aguardar monsters receberem movimento;
- Alt+Tab;
- observar se aparecem linhas `PATH SetPath skipped` em:

```text
C:\0-NewDmo\ClientDist1_x64\logs\crash.log
```

Se ainda crashar, comparar a nova stack. Se nao crashar mas continuar sem andar,
investigar `PathTest()`/`SetPath()` rejeitando posicoes e revalidar o extra data
de path do terrain root.

## Regressao Observada Apos Mutex

Depois do build com mutex, houve um crash antes do primeiro render visivel do
mapa. O mapa ja tinha passado por `InGame Initialize end`, mas o primeiro frame
ainda nao tinha sido renderizado.

Nova evidencia:

```text
C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\DProject_R_ENG_20260607_054001_37848.dmp
```

Stack:

```text
lk-dmo-x64.exe!CsC_PathProp::SetPath
lk-dmo-x64.exe!cCliGame::SyncMoveMonster
lk-dmo-x64.exe!cCliGame::SyncInMonster
lk-dmo-x64.exe!cCliGame::SyncInObject
lk-dmo-x64.exe!cCliGame::RecvSyncData
lk-dmo-x64.exe!cCliGame::OnExecute
lk-dmo-x64.exe!cClient::ReadAll::<lambda>
```

Contexto:

```text
last_context=SyncInMonster parsed idx=19 type=71506 typeAll=0x1175200010013 dst=44493,11843 hp=255 level=34 condition=0x0 found=1
```

O caso especifico era:

- `SyncInMonster` recebeu um monstro que ainda nao existia no cliente.
- `AddMonster()` criou o objeto.
- No mesmo pacote, como `bNew == false` e `pos != DstPos`, chamou
  `SyncMoveMonster()`.
- `SyncMoveMonster()` chamou `SetPath()` imediatamente no thread de rede.

Isso e diferente do crash posterior observado antes: agora a falha acontece no
bootstrap dos monstros iniciais do mapa.

## Ajuste Complementar

Arquivo:

```text
C:\dmo-client-main\DProject\network\cCliGameSync.cpp
```

Mudanca:

- `SyncInMonster()` agora salva `bMonsterExisted` antes de chamar `AddMonster()`.
- O path inicial para `DstPos` so e calculado se o monstro ja existia antes do
  pacote.
- Monstro recem-criado fica em `pos`; proximos syncs de movimento podem atualizar
  o path depois que o mapa/frame estiver estabilizado.

Build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m:1 `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_skip_initial_monster_path.log;Verbosity=normal"
```

Resultado:

```text
1 Aviso(s)
0 Erro(s)
Tempo Decorrido 00:00:29.83
```

Runtime atualizado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
timestamp: 2026-06-07 05:43:00
```

Nova classificacao:

```text
erro de codigo / race condition / path inicial de monstro criado no mesmo pacote
```

Observacao importante:

Esta correcao nao afirma que `agent/context/path` nulo seja normal. O objetivo e
impedir que um monstro criado dentro de `SyncInMonster()` calcule path no mesmo
pacote e no thread de rede antes do primeiro update/render do mapa.

## Regressao De Movimento Do Player

Apos a contencao do PathEngine, o personagem continuou sem conseguir andar.

Causa provavel introduzida nesta rodada:

- foram adicionados checks `m_iMesh->positionIsValid(p)` em `SetPath()`,
  `FindShortestPathInfo()` e `PathTest()`;
- o codigo legado monta `cPosition` com `p.cell = 0` e entrega a chamada para a
  PathEngine;
- `positionIsValid()` com `cell=0` pode rejeitar pontos validos fora da celula 0
  antes da propria PathEngine fazer o tratamento antigo.

Mudanca aplicada:

- removidos os checks novos de `positionIsValid()` em:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\PathEngine.cpp
```

Mantido:

- mutex;
- guardas de ponteiro/contexto nulo;
- comportamento original de chamar `findShortestPathTo()` e
  `testPointCollision()` com `cell=0`.

Build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m:1 `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_restore_pathengine_cell_behavior.log;Verbosity=normal"
```

Resultado:

```text
19 Aviso(s)
0 Erro(s)
Tempo Decorrido 00:00:28.79
```

Runtime atualizado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
timestamp: 2026-06-07 05:45:50
```

## Rodada 2026-06-07 05:54 - player anima mas nao desloca

Sintoma novo reportado:

```text
player faz animacao de movimento, mas nao sai do lugar; Alt+Tab gerou crash
```

Evidencia local:

- `logs/crash.log` ainda aponta o crash fatal para `SyncMoveMonster()` chamando
  `CsC_PathProp::SetPath()` no thread de rede.
- O log de rede anterior ja mostrou pacotes de clique/movimento saindo do
  cliente, entao input/protocolo de envio nao parece ser o primeiro bloqueador.
- O detalhe "anima mas nao desloca" indica que `SetPath()` ou o estado de
  movimento foi acionado, mas `PreUpdate()` pode estar:
  - pausado por `PAUSE_PATH`;
  - recebendo velocidade/distancia invalida;
  - chamando `UpdatePath()` e recebendo a mesma posicao do agente;
  - apagando path ao chegar com `getLength() == 0`.

Checklist DMO aplicado:

```text
packet mismatches: baixo no momento; clique gera pacote, crash fatal e em monstro remoto.
race conditions: alto; PathEngine ainda e chamada no thread ASIO para objetos remotos.
null checks: guardas ja adicionadas em PathEngine/PathProperty, sem log de guarda recente.
game logic: player pode estar em PAUSE_PATH ou path com distancia/velocidade invalida.
pipelines: update/render do mapa esta ativo, mas movimento local precisa trace em PreUpdate.
overflow: ainda possivel em structs legadas x86, mas este crash atual nao mostrou truncamento direto.
```

Mudancas aplicadas nesta rodada:

1. Em `C:\dmo-client-main\DProject\network\cCliGameSync.cpp`:
   - no build `DMO_X64_WINDX9_BRIDGE`, `SyncMoveMonster()` agora retorna antes de
     chamar `PathTest()` / `SetPath()` no thread de rede;
   - adiciona log limitado `SYNC-MONSTER x64 skip network-thread path`.

2. Em `C:\dmo-client-main\LibProj\CsGBChar\PathProperty.cpp`:
   - adiciona logs limitados apenas para `m_bUserControl == true`;
   - logs novos:
     - `PATH-USER SetPath guard`;
     - `PATH-USER SetPath null`;
     - `PATH-USER SetPath ignored short`;
     - `PATH-USER SetPath ok`;
     - `PATH-USER PreUpdate paused`;
     - `PATH-USER PreUpdate`.

Build:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_path_user_trace.log;Verbosity=Normal"
```

Resultado:

```text
Build OK
DProject.vcxproj -> C:\Release\GSP_x64\lk-dmo-x64.exe
warning C4789 em CsGBTerrainRoot_VersionUp.cpp:870, ja conhecido
```

Runtime atualizado:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
timestamp: 2026-06-07 05:54:26
```

Proxima leitura apos teste:

```powershell
Select-String "C:\0-NewDmo\ClientDist1_x64\logs\crash.log" `
  -Pattern "PATH-USER|SYNC-MONSTER x64|CRASH" `
  -Context 0,2
```

Interpretacao esperada:

- Se aparecer `PATH-USER PreUpdate paused`, o bloqueio e estado/pausa do objeto.
- Se `PATH-USER SetPath ok` tem `speed=0` ou `len` estranho, o problema esta em
  propriedade de movimento/dados carregados.
- Se `PATH-USER PreUpdate` mostra `before` e `after` iguais com `lenAfter`
  reduzindo, a PathEngine x64 esta consumindo path sem mover coordenada.
- Se nao aparecer `PATH-USER SetPath ok`, o clique chega ao envio de pacote mas
  nao cria path local; investigar `CTamerUser::SetTargetPos()` e
  `CalTargetPath()`.
