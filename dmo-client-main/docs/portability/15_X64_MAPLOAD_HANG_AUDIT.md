# X64 Map Load Hang Audit

Data: 2026-06-07

## Escopo

Investigar travamento do client x64 na tela de loading apos selecionar/carregar personagem.

O som x64 com Miles 9.4d ja inicializa corretamente. Esta rodada nao altera renderer,
logica de jogo, protocolo de rede, nem comportamento de carregamento. As mudancas sao
instrumentacao e um ajuste de chamada de log.

## Ambiente

- Workspace: `C:\dmo-client-main`
- Runtime testado pelo usuario: `C:\0-NewDmo\ClientDist1_x64`
- MSBuild: `C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe`
- Solucao: `C:\dmo-client-main\DProject.sln`
- Configuracao: `X64_WinDX9_Bridge`
- Plataforma: `x64`
- Saida: `C:\Release\GSP_x64\lk-dmo-x64.exe`
- Copiado para: `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`

## Comando de build

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_mapload_trace.log;Verbosity=normal"
```

## Resultado do build

Build concluido com sucesso.

- Executavel gerado: `C:\Release\GSP_x64\lk-dmo-x64.exe`
- Tamanho: `13,555,200` bytes
- Copia runtime: `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`
- Erros: `0`
- Warnings: `1`

Warning relevante:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot_VersionUp.cpp(870):
warning C4789: buffer 'baseInfo' of size 68 bytes will be overrun; 28 bytes will be written starting at offset 52
```

Classificacao do warning:

- Tipo: `erro potencial 32->64` / `overflow`
- Status: nao corrigido nesta rodada
- Motivo: fica em caminho de versionamento de terrain root; corrigir sem confirmar a versao real do mapa pode mascarar ou quebrar dados antigos.

## Evidencia do travamento anterior

Evento do Windows indicou `AppHangB1`, nao `APPCRASH`, para `lk-dmo-x64.exe`.
Nao houve novo dump WER local em `C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\WER`.

Ultimos marcadores relevantes antes da instrumentacao:

```text
NET recv packet=1308 body_bytes=27 total_len=29
GATE SelectSuccess ipLen=13 port=17608 map=105
MAPLOAD DataLoadStart nextMap=105 prevMap=0
MAPLOAD step<10 nextMap=105 mapList=...
MAPLOAD mapInfo nextMap=105 path=<saida corrompida> cameraMax=0
MAPLOAD step<54 nextMap=105 gameIF=...
MAPLOAD step<56 nextMap=105
MAPLOAD step<58 nextMap=105
MAPLOAD step<60 LoadTerrain begin nextMap=105
```

Conclusao: o travamento ocorre dentro ou apos `CMngCollector::LoadTerrain(105)`.

## Mudancas aplicadas

### Ajuste simples de configuracao/log

Arquivo:

- `C:\dmo-client-main\DProject\_Interface\00.Contents\LoadingContents.cpp`

Correcao:

- `CrashLogger::LogMessage("%s", std::string)` foi trocado por `std::string::c_str()`.

Motivo:

- Em x64, passar `std::string` diretamente para `%s` em funcao variadica imprime bytes do objeto, nao o ponteiro do texto.
- Isso explicava o `path` corrompido no log do mapa 105.

### Instrumentacao minima

Arquivos:

- `C:\dmo-client-main\DProject\MngCollector.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainMng.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot_SaveLoad.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp`

Novos marcadores:

- `MNG LoadTerrain ...`
- `TRMNG CreateRoot ...`
- `TRROOT NiStream.Load ...`
- `TRROOT LoadExtraData ...`
- `TRROOT PathEngine ...`
- `TRROOT ApplyTerrainPath ...`
- `TRROOT Load BM/SM/AM/SkyBox/EMR ...`

Objetivo:

- Identificar se o hang ocorre em cena/evento, `CreateRoot`, `NiStream.Load`, extra data do terrain, PathEngine, texturas ou EMR.

## Checklist DMO

- Packet mismatches: sem evidencia ate agora. O pacote `1308` foi recebido e parseado com `map=105`.
- Race conditions: nao confirmado. O sintoma e `AppHang`; proximo log pode indicar se alguma chamada bloqueia.
- Null checks: possivel em globals de loading, mas nao ha evidencia antes de `LoadTerrain`.
- Game logic: sem evidencia. O fluxo falha antes de NPC/char placement.
- Pipelines: principal suspeito. Falha fica no pipeline de carregamento de terrain/mapa.
- Overflow/layout: risco alto. Terrain usa dados binarios historicos Win32; ja existe warning C4789 em versionamento e ja corrigimos layout Win32 de PathInfo em rodada anterior.

## Arquivos criticos

- `C:\dmo-client-main\DProject\_Interface\00.Contents\LoadingContents.cpp`
- `C:\dmo-client-main\DProject\MngCollector.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainMng.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot_SaveLoad.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp`
- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot_VersionUp.cpp`

## Bloqueadores reais

1. Ainda falta o log da proxima reproducao com o build instrumentado.
2. A validacao anterior usou apenas referencias locais disponiveis neste workspace.
3. `PathEngine` ainda e suspeito porque houve aviso anterior de coordenada de vertice fora do range permitido.
4. `CsGBTerrainRoot_VersionUp.cpp` tem warning C4789 real de overflow em build x64.

## Estimativa

- Para localizar o ponto exato do hang: uma reproducao com o build atual.
- Para corrigir se for PathEngine/ApplyTerrainPath: medio, provavelmente 1 a 3 horas, dependendo se os dados do mapa estao corrompidos ou se e conversao de layout.
- Para corrigir se for `CsGBTerrainRoot_VersionUp.cpp`: medio/alto, precisa validar versao binaria real do mapa e estrutura esperada.
- Para corrigir se for textura/EMR: baixo/medio, tende a ser include/pack/path ou leitura de asset.

## Proxima acao recomendada

Rodar `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`, selecionar o mesmo personagem/mapa 105, aguardar o travamento e preservar o processo se aparecer "nao esta respondendo".

Depois coletar:

- Tail de `C:\0-NewDmo\ClientDist1_x64\logs\crash.log`
- Tail de `C:\0-NewDmo\ClientDist1_x64\logs\network_pgsql.log`
- Se o processo ainda estiver aberto, gerar dump do `lk-dmo-x64.exe` antes de fechar.

## Atualizacao 2026-06-07 05:24

O build instrumentado reproduziu em `map=2` e localizou a falha:

```text
TRROOT PathEngine ApplyTerrainPath end ... errorIndex=-1
TRROOT CreateFromFile begin file=Data\Map\RealWorld\D_terminal\D_terminal\D_terminel_main01.nif
ASSERT: CsGBTerrainLeaf.cpp:1114 expr: fh.s_dwSaveType == CsGBObject::ST_FIRST_OBJECT
ASSERT: CsGBObject.cpp:709 expr: m_pUserDefine != NULL
CRASH: CsGBObject::LoadExtraData+0xd3
```

Conclusao: PathEngine nao foi o ponto de falha nessa reproducao. A causa primaria
aponta para desalinhamento de offset/layout ao ler objetos do terrain leaf em x64.

Correcoes aplicadas e build novo documentados em:

```text
C:\0-NewDmo\dmo-client-main\docs\portability\16_X64_DISK_LAYOUT_PATTERN_SWEEP.md
```
