# X64 Disk Layout Pattern Sweep

Data: 2026-06-07

## Escopo

Varredura por padroes de serializacao Win32 que quebram em x64: leitura de dados
binarios antigos com `sizeof(struct)` quando a struct possui ponteiro, padding ou
layout dependente da arquitetura.

Esta rodada foi limitada a correcoes de layout de disco no bridge x64. Nao altera
renderer, logica de jogo, nem protocolo de rede.

## Evidencia que motivou a correcao

Com o build instrumentado, o mapa carregou ate `CreateFromFile` e falhou ao ler
objetos do terrain leaf:

```text
TRROOT PathEngine ApplyTerrainPath end ... errorIndex=-1
TRROOT CreateFromFile begin file=Data\Map\RealWorld\D_terminal\D_terminal\D_terminel_main01.nif
ASSERT: CsGBTerrainLeaf.cpp:1114 expr: fh.s_dwSaveType == CsGBObject::ST_FIRST_OBJECT
ASSERT: CsGBObject.cpp:709 expr: m_pUserDefine != NULL
CRASH: CsGBObject::LoadExtraData+0xd3
```

Classificacao:

- Tipo: `erro de 32->64`
- Causa: layout/offset de dados binarios de mapa lido com tamanho x64.
- Nao e Miles, pacote `1308`, renderer, nem PathEngine principal.

## Padroes pesquisados

Comandos base:

```powershell
rg -n "memcpy\s*\(\s*&[^,]+,\s*&?pData\s*\[\s*[^\]]+\s*\]\s*,\s*sizeof\s*\(|memcpy\s*\(\s*&[^,]+,\s*pData\s*,\s*sizeof\s*\(|_read\s*\([^,]+,\s*&[^,]+,\s*sizeof\s*\(" `
  "C:\dmo-client-main\LibProj" "C:\dmo-client-main\DProject" `
  -g "*.cpp" -g "*.h" -g "!**/x64/**" -g "!**/Release*/**" -g "!**/*.tlog/**"

rg -n "\*\s+[A-Za-z_][A-Za-z0-9_]*\s*;|[A-Za-z_][A-Za-z0-9_:<>]*\s*\*\s*[A-Za-z_][A-Za-z0-9_]*\s*;|std::map|std::list|std::vector|string" `
  "C:\dmo-client-main\LibProj\CsGamebryo2.3" -g "*.h"
```

## Correcoes aplicadas

### `CsGBObject::sINFO`

Arquivo:

- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBObject.cpp`

Problema:

- `LoadObjectInfo` ja tinha tratamento especial para `s_PathInfo`, mas usava
  `offsetof(CsGBObject::sINFO, s_PathInfo)` como prefixo.
- Em x64 esse `offsetof` inclui padding de alinhamento antes do membro com ponteiros.
- Resultado: o loader consumia bytes a mais do payload Win32 e desalinhava o proximo objeto.

Correcao:

- Criado `sDiskObjectInfoPrefixWin32` com layout de disco fixo.
- Leitura do prefixo passou a ser campo-a-campo no `DMO_X64_WINDX9_BRIDGE`.
- `static_assert(sizeof(sDiskObjectInfoPrefixWin32) == 340)`.

### `CsGBTerrainLeaf::sNiTriINFO`

Arquivo:

- `C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainLeaf.cpp`

Problema:

- `_LoadExtraData` lia `sNiTriINFO` com `sizeof(sNiTriINFO)`.
- `sNiTriINFO` possui `float* s_pfHeight` no final.
- Em x64, o ponteiro e o padding aumentam o tamanho da struct, fazendo a leitura pular bytes do height field.

Correcao:

- Criado `sDiskNiTriInfoWin32` com layout de disco fixo.
- Leitura do leaf tri info passou a ser campo-a-campo no `DMO_X64_WINDX9_BRIDGE`.
- `static_assert(sizeof(sDiskNiTriInfoWin32) == 1336)`.

## Padroes avaliados e nao alterados

Arquivos/estruturas revisados:

- `CsGBCell::sINFO`: contem `NiTransform`, sem ponteiro proprio no payload salvo.
- `CsGBSpeedCell::sOBJECT_INFO`: path fixo e floats, sem ponteiro salvo.
- `CsGBSkyBox::sINFO`: path fixo e float, sem ponteiro salvo.
- `CsGBLightMng/CsGBLight_Dir/CsGBLight_Point`: payload salvo e POD; ponteiros existem nos objetos runtime, nao no struct salvo.

Status:

- Sem alteracao nesta rodada.

## Build

Comando:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m `
  /p:Configuration=X64_WinDX9_Bridge `
  /p:Platform=x64 `
  /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_sln_x64_build_disk_layout_sweep.log;Verbosity=normal"
```

Resultado:

- Build: sucesso
- Erros: `0`
- Warnings: `1`
- Saida: `C:\Release\GSP_x64\lk-dmo-x64.exe`
- Runtime atualizado: `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`

Warning remanescente:

```text
CsGBTerrainRoot_VersionUp.cpp(870): warning C4789:
buffer 'baseInfo' of size 68 bytes will be overrun; 28 bytes will be written starting at offset 52
```

Classificacao:

- Tipo: `overflow` / `32->64` / `codigo de conversao de versao antiga`
- Status: nao corrigido nesta rodada.
- Motivo: exige validar se o mapa ativo entra nessa rota de `_VersionUp`; corrigir sem contexto pode quebrar conversao de mapas antigos.

## Checklist DMO

- Packet mismatches: sem evidencia; `1308` parseou e entregou `map=2`.
- Race conditions: sem evidencia.
- Null checks: crash final bateu em `m_pUserDefine != NULL`, mas a causa primaria foi offset corrompido antes da criacao do objeto.
- Game logic: sem evidencia.
- Pipelines: terrain loading, especificamente `CsGBTerrainRoot::CreateFromFile`.
- Overflow/layout: confirmado.

## Proxima acao recomendada

Rodar novamente:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

Selecionar o mesmo personagem que carregou `map=2`.

Se ainda travar/crashar, coletar o final de:

- `C:\0-NewDmo\ClientDist1_x64\logs\crash.log`
- `C:\0-NewDmo\ClientDist1_x64\logs\network_pgsql.log`
- dump em `C:\0-NewDmo\ClientDist1_x64\Log\CrashDumps\`

Se passar desse ponto, o proximo gargalo provavel fica em table objects, speed cell,
light/skybox extras ou objetos de mapa carregados depois do terrain root.

## Smoke test observado

Apos copiar o build corrigido para `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`,
foi observado um processo ativo:

```text
lk-dmo-x64.exe - janela: Digimon Masters(105) + release [Mon May 25 21:27:05 2026]
```

O `network_pgsql.log` mostrou pacotes de gameplay apos o loading:

```text
packet=1006
packet=3129
packet=1713
packet=1070
packet=1023
packet=16032
```

Nao apareceu dump novo depois de `DProject_R_ENG_20260607_052405_11372.dmp`, que
era o crash anterior em `CsGBObject::LoadExtraData`.

Conclusao provisoria: o crash de leitura de objetos do terrain leaf foi superado
com o layout de disco Win32 para `sNiTriINFO` e `CsGBObject::sINFO`.
