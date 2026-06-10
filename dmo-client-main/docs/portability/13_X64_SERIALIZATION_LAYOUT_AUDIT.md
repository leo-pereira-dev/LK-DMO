# X64 Serialization Layout Audit

Data: 2026-06-07

## Contexto

O crash/nonfatal atual nao parece ser um problema aleatorio do PathEngine. O erro:

```text
iPathEngine::newShape() failed - vertex coordinate exceeds allowed range. Returning zero.
```

foi rastreado ate chamadas de `iPathEngine::newShape()` recebendo arrays de path carregados de `NiBinaryExtraData`.

O padrao critico encontrado e:

```cpp
memcpy(&structRuntime, data, sizeof(structRuntime));
offset += sizeof(structRuntime);
```

quando `structRuntime` contem ponteiros. No Win32, esses ponteiros ocupam 4 bytes. No x64, ocupam 8 bytes. Como os mapas/packs foram gravados por binarios Win32, ler o blob com `sizeof(struct)` do build x64 desloca os offsets e corrompe os arrays seguintes.

## Metodo eficiente para esse tipo de bug

1. Procurar `NiBinaryExtraData::GetValue`, `memcpy`, `sizeof(struct)` e structs com ponteiros.
2. Separar "layout de disco Win32" de "layout runtime x64".
3. Para cada blob, ler apenas os campos persistidos do disco em structs fixas com `DWORD` para ponteiros antigos.
4. Zerar ponteiros runtime e realocar arrays com os tamanhos lidos do blob.
5. So depois chamar subsistemas como PathEngine, audio, renderer ou loader.

Isso evita tentativa e erro porque a falha passa a ser auditavel por offset e tamanho.

## Correcao minima aplicada

### Objetos com path info

Arquivo:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBObject.cpp
```

Funcao:

```cpp
CsGBObject::LoadObjectInfo
```

Mudanca:

- Em `DMO_X64_WINDX9_BRIDGE`, o prefixo de `CsGBObject::sINFO` e copiado ate `s_PathInfo`.
- O bloco `s_PathInfo` e lido com layout fixo Win32:

```cpp
struct sDiskPathInfoWin32
{
    UINT  s_uiPEGroupCount;
    UINT  s_uiTotalPECount;
    DWORD s_pptTranse;
    DWORD s_pPECount;
    DWORD s_pData;
};
```

- Os ponteiros runtime sao resetados antes de realocar `s_pptTranse`, `s_pPECount` e `s_pData`.

### Path info do terreno

Arquivo:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot.cpp
```

Funcao:

```cpp
CsGBTerrainRoot::LoadExtraData
```

Mudanca:

- Em `DMO_X64_WINDX9_BRIDGE`, `CsGBTR_PathExtraKey` e lido com layout fixo Win32:

```cpp
struct sDiskTRPathInfoWin32
{
    UINT  s_uiPEGroupCount;
    UINT  s_uiTotalPECount;
    DWORD s_pptTranse;
    DWORD s_pPECount;
    DWORD s_pData;
    DWORD s_pType;
    int   s_nDataTotalCount;
};
```

- Os arrays runtime sao realocados depois do header de disco.

## Build validado

Comando:

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
```

Executavel copiado para:

```text
C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe
```

## Warning relevante

```text
LibProj\CsGamebryo2.3\CsGBTerrainRoot_VersionUp.cpp(870):
warning C4789: buffer 'baseInfo' of size 68 bytes will be overrun;
28 bytes will be written starting at offset 52
```

Classificacao:

```text
erro/gargalo 32->64 + codigo legado
```

Esse ponto fica no fluxo de version-up de dados de terreno e deve ser tratado como risco real, mas nao foi corrigido agora porque nao estava no caminho direto do erro de `newShape()` e pode alterar conversao de mapas antigos.

## Hotspots restantes

Pontos que precisam de auditoria antes de novas correcoes:

```text
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainRoot_VersionUp.cpp
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainStructVersion.h
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBCell.cpp
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBTerrainLeaf.cpp
C:\dmo-client-main\LibProj\CsGamebryo2.3\CsGBSpeedCell.cpp
```

Prioridade maior:

```text
CsGBTerrainRoot_VersionUp.cpp
CsGBTerrainStructVersion.h
```

Motivo: contem structs versionadas antigas com ponteiros e copias por `sizeof`.

## Limite da correcao atual

A correcao aplicada mira o caminho de leitura usado pelo client x64. O caminho de gravacao ainda contem usos como:

```text
CsGBObject::GetSaveSize / SaveExtraData
CsGBTerrainRoot::_SaveExtraData
```

Esses pontos ainda usam `sizeof(sINFO)` e `sizeof(sTR_PATHINFO)` no layout runtime. Isso nao deve bloquear o client de jogo carregando dados Win32 existentes, mas bloqueia usar um map tool/editor x64 para gravar novos blobs ate criarmos tambem escritores com layout de disco fixo.

## Proxima acao recomendada

1. Rodar o client x64 atualizado no mesmo ponto do erro.
2. Se o popup `iPathEngine::newShape()` sumir, continuar auditoria de layout nos outros `NiBinaryExtraData`.
3. Se ele continuar, instrumentar temporariamente `CsPathEngine::SetShape()` para logar `count`, min/max de coordenadas e origem do shape antes de chamar `newShape()`.
4. Corrigir os proximos pontos somente com structs de disco Win32 equivalentes, sem alterar logica de jogo, renderer ou protocolo.
