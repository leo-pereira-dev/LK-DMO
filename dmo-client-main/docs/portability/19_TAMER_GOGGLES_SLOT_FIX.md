# Tamer Goggles Slot Fix

Data: 2026-06-07

## Resumo

O problema nao estava no item da Yolei em si. O `ItemList.bin` separa `Glass/Bag`
e `Goggles`, mas o client e o server estavam colapsando os dois no slot 6.

Contrato corrigido:

- `nItem::Glass = 27` -> `nTamer::Glass = 6`
- `nItem::Goggles = 36` -> `nTamer::Goggles = 14`
- `nTamer::MaxParts = 15`
- `nsPART::Digivice = 15`
- `nsPART::MAX_TOTAL_COUNT = 16`
- server `GeneralSizeEnum.Equipment = 15`
- server `GeneralSizeEnum.EquipmentMaxSlot = 1014`

## Confirmacao no BIN

BINs comparados:

- client: `C:\0-NewDmo\ClientDist1_x64\Data\Pack03.pf/.hf`, entrada `data\bin\english\ItemList.bin`
- server: `C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\ItemList.bin`

Itens Yolei confirmados nos dois lados:

| ItemId | Nome | TypeL | Slot corrigido |
|---:|---|---:|---|
| 47383 | Yolei Inoue's Hat | 21 | Head |
| 47384 | Yolei Inoue's Bag | 27 | Glass/Bag |
| 47385 | Yolei Inoue's Top | 22 | Coat |
| 47386 | Yolei Inoue's Pants | 24 | Pants |
| 47387 | Yolei Inoue's Gloves | 23 | Glove |
| 47388 | Yolei Inoue's Shoes | 25 | Shoes |

Goggles reais confirmados:

| Exemplo | TypeL | Slot corrigido | Observacao |
|---:|---:|---|---|
| 15101 | 36 | Goggles | NIF vazio |
| 15118 | 36 | Goggles | NIF vazio |
| 76649 | 36 | Goggles | NIF vazio |
| 76650 | 36 | Goggles | NIF vazio |
| 76664 | 36 | Goggles | NIF vazio |

Conclusao: `Goggles` e slot estatistico/equipavel, nao part mesh. O render nao deve
tentar carregar `.nif` para esse slot.

## Alteracoes aplicadas

Client:

- `common_vs2019/pTamer.h`: adicionado `Goggles` antes de `MaxParts`.
- `LibProj/CsGBChar/04_PartObject.h`: adicionado `nsPART::Goggles = 14`, Digivice passa a 15.
- `DProject/DataMng.cpp`: `TypeL 36` equipa em `nTamer::Goggles`.
- `DProject/Data_TEquip.cpp`: assert/slot resolver agora reconhece `nTamer::Goggles`.
- `LibProj/CsGBChar/04_PartObject.cpp`: `TypeL 36` valida como `nsPART::Goggles` e nao tenta carregar NIF.
- `DProject/_Interface/Game/TamerStatus_Equip.cpp`: topo direito passa a ser `Goggles`; `Glass/Bag` fica no primeiro slot da coluna esquerda.
- `DProject/_Interface/14.MainFrameUI/TamerStatusUI_Tamer.cpp`: top grid `Head + Goggles`; coluna esquerda inclui `Glass`.
- `DProject/_Interface/14.MainFrameUI/MainFrame_DetailInfoUI.cpp`: mesmo contrato visual do Tamer Status.
- `DProject/_Interface/Game/CashShop_Char.cpp`: inicializa coordenada do slot `Goggles`.
- `DProject/_Interface/15.CashShop/NewCashshopEquipViewer.cpp`: adiciona `Goggles` ao mapa da preview.
- `DProject/_Interface/Game/ToolTip_Stat.cpp`: soma stat de `nTamer::Goggles`.

Server:

- `GeneralSizeEnum.cs`: equipamento de 14 para 15 slots, max slot de 1013 para 1014.
- `ItemMovePacketProcessor.cs`: `GogglesItemType = 36` resolve para slot 14; Digivice visual passa para 15.
- `global.json`: SDK corrigido de `7.0.0` para `7.0.100`, porque `dotnet publish` do Docker nao aceita `7.0.0` como feature band valida.

## Checklist DMO

- Packet mismatches: corrigido. Client e server agora usam 15 `Equipment` slots.
- Race conditions: nao ha indicio nesta falha.
- Null checks: nao ha indicio nesta falha.
- Game logic: corrigido no resolver de slot e no item move.
- Pipelines: render corrigido para nao carregar NIF vazio em `Goggles`.
- Overflow: nao ha indicio direto; arrays baseados em `nLimit::Equip`/`MaxParts` foram expandidos pelo contrato.

## Comandos de build

Client:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\dproject_x64_goggles_slot_build.log;Verbosity=normal"
```

Resultado:

- OK
- saida: `C:\Release\GSP_x64\lk-dmo-x64.exe`
- copiado para:
  - `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`
  - `C:\0-NewDmo\ClientDist1\lk-dmo-x64.exe`
- SHA256 final: `C30448FD0DAE05BA269830359C7552426EADB1A8BEE944E5EA756135A5AC1231`

Server:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" `
  /restore /m /p:Configuration=Release /v:minimal `
  "/flp:LogFile=C:\dmo-client-main\docs\portability\server_goggles_slot_build.log;Verbosity=normal"
```

Resultado:

- OK
- `dotnet build DigitalWorldOnline.sln -c Release --no-restore -v minimal` tambem passou depois da correcao do `global.json`.
- warnings principais ja existentes: `net7.0` EOL, vulnerabilidades em `AutoMapper 12.0.0` e `System.Linq.Dynamic.Core 1.3.2`, varios nullable warnings.

## Deploy em producao

Ambiente:

- VPS: `ubuntu@104.234.41.94`
- pasta remota do server: `/home/ubuntu/dmo-server-main-postgres`
- compose env: `.env.production`

Backup remoto criado antes do upload:

```text
/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260607_120657
```

Arquivos enviados:

- `global.json`
- `src/Source/Domain/DigitalWorldOnline.Commons/Enums/ClientEnums/GeneralSizeEnum.cs`
- `src/Source/Distribution/DigitalWorldOnline.Game.Host/PacketProcessors/ItemMovePacketProcessor.cs`

Hashes remotos apos upload:

| Arquivo | SHA256 |
|---|---|
| `global.json` | `982a3d125a0e7cd3abe94b5ee881f51168791ef97f51982e7053ae8282641685` |
| `GeneralSizeEnum.cs` | `51df0f37b1ffb8dee3f04bd197d37163323b9f97e5454ee998e7cf9234d3a070` |
| `ItemMovePacketProcessor.cs` | `e89dd608450844ebba7b213f3927a97c9a021d5cadac4cfd2346ed105fd5116a` |

Comando usado no deploy:

```bash
cd /home/ubuntu/dmo-server-main-postgres
sudo -n docker compose --env-file .env.production up -d --build character game
```

Resultado:

- comando finalizou com exit code `0`;
- containers `character` e `game` foram recriados e ficaram `Up`;
- `account`, `admin`, `api`, `routine` e `postgres` nao foram recriados, pois a mudanca de contrato de equipamento afeta o fluxo de personagem/jogo;
- logs pos-deploy:
  - `CharacterServer started.`
  - `GameServer started.`
  - permanecem warnings antigos de dados `[DUnit]`, sem erro novo de startup.

## Bloqueadores / riscos

- O client novo deve rodar contra server novo. Se usar server antigo, o pacote de equipamento fica desalinhado porque o client espera 15 slots.
- Validacao feita por BIN ativo, source e build locais.
- A tela de cash shop preview tem coluna apertada com todos os opcionais ativos; o contrato esta mapeado, mas o layout fino pode precisar de redesenho se ficar visualmente apertado.

## Proxima acao recomendada

1. Subir/reiniciar o server com os binarios novos.
2. Testar login com `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe` ou `C:\0-NewDmo\ClientDist1\lk-dmo-x64.exe`.
3. Equipar Yolei set completo e validar:
   - Yolei Bag no slot `Glass/Bag`.
   - Adventure/Tamer Goggles no topo direito `Goggles`.
   - stats de goggles aplicando no tooltip/status.
   - outro player vendo o visual/estado sem pacote quebrado.
4. Se aparecer desalinhamento visual depois do deploy, coletar screenshot + log imediatamente apos abrir `Tamer Information`, porque o contrato server/client ja esta alinhado e o proximo suspeito passa a ser layout/UI local.
