# Tamer Keyring, NamePlate, XAI, And Digivice Slot Fix

Data: 2026-06-08

## Resumo

O assert do chaveiro nao era falta de BIN novo. O item `Ghost Keyring [VB]`
tem `TypeL 35`, mas o client ainda nao conhecia esse tipo como equipamento
usavel. Ao clicar com botao direito, `DataMng.cpp` caia no `item use Error :
type = 35`.

O mesmo contrato tambem explica os slots visuais errados:

- Digivice deve ficar na coluna direita acima do XAI.
- XAI deve ficar imediatamente abaixo do Digivice.
- Keyring/chaveiro deve ficar no slot separado a frente do XAI, nao dentro da
  coluna direita.
- Magnetic ID/NamePlate deve ficar no overlay esquerdo inferior, ao lado do
  Costume.

## Contrato Atual

Slots logicos de equipamento do Tamer:

| Slot | Enum client | Uso |
|---:|---|---|
| 0 | `nTamer::Head` | Head |
| 1 | `nTamer::Coat` | Coat |
| 2 | `nTamer::Glove` | Glove |
| 3 | `nTamer::Pants` | Pants |
| 4 | `nTamer::Shoes` | Shoes |
| 5 | `nTamer::Costume` | Costume |
| 6 | `nTamer::Glass` | Glass/Bag |
| 7 | `nTamer::Ring` | Ring |
| 8 | `nTamer::Necklace` | Necklace |
| 9 | `nTamer::Earring` | Earring |
| 10 | `nTamer::EquipAura` | Aura |
| 11 | `nTamer::XAI` | XAI |
| 12 | `nTamer::Bracelet` | Bracelet |
| 13 | `nTamer::NamePlate` | Magnetic ID / NamePlate |
| 14 | `nTamer::Goggles` | Goggles |
| 15 | `nTamer::Keyring` | Keyring |

Partes visuais:

| Part | Uso |
|---:|---|
| 13 | `nsPART::NamePlate` |
| 14 | `nsPART::Goggles` |
| 15 | `nsPART::Keyring` |
| 16 | `nsPART::Digivice` |

Pacote servidor/client:

```text
Equipment size: 16
EquipmentMinSlot: 1000
EquipmentMaxSlot: 1015
XAI special packet slot: 1011
Keyring packet slot: 1015
Digivice item container: separate Digivice list, visual part 16
```

## Alteracoes Aplicadas

Client:

- `common_vs2019/pItem.h`: adicionado `nItem::Keyring = 35`.
- `common_vs2019/pTamer.h`: adicionado `nTamer::Keyring` antes de
  `MaxParts`.
- `LibProj/CsGBChar/04_PartObject.h`: adicionado `nsPART::Keyring = 15`;
  Digivice visual passa para `16`.
- `LibProj/CsGBChar/04_PartObject.cpp`: `TypeL 34/35/36` valida como
  NamePlate/Keyring/Goggles e esses slots logicos nao entram no loader de
  modelo `.nif`. Isto evita o assert em `bSuccess == true` e o cubo branco
  quando Magnetic ID, Keyring ou XAI sao equipados.
- `DProject/DataMng.cpp` e `DProject/Data_TEquip.cpp`: resolvem `TypeL 34`,
  `35` e `36` por enum dedicado, nao por `TypeL - Head`.
- `DProject/network/cCliGameItem.cpp`: aceita Keyring no retorno de item
  ativo.
- `DProject/network/cCliGameShop.cpp` e
  `DProject/_Interface/00.Contents/NewCashshopContents.cpp`: convertem `TypeL`
  moderno para slot logico antes de validar `MaxParts`.
- `DProject/_Interface/14.MainFrameUI/TamerStatusUI_Tamer.cpp` e
  `MainFrame_DetailInfoUI.cpp`: coluna direita agora mostra Digivice e XAI no
  final; Keyring usa um grid separado no slot lateral a frente do XAI.
- `DProject/_Interface/Game/TamerStatus_Equip.cpp`: UI antiga tambem recebeu
  coordenada para NamePlate e Keyring.
- `ToolTip_Item.cpp`, `InventoryContents.cpp` e `MainFrame_Contents.cpp`:
  fluxos de tooltip, inventario e uso direto passam a aceitar Keyring.

Server:

- `GeneralSizeEnum.cs`: `Equipment = 16` e `EquipmentMaxSlot = 1015`.
- `ItemMovePacketProcessor.cs`: `TypeL 35` resolve para slot 15; Digivice
  visual passa para part 16; Keyring nao e tratado como visual mesh.

## Checklist DMO

- Packet mismatches: corrigido. Client e server usam o mesmo tamanho de lista.
- Race conditions: sem indicio nesta falha.
- Null checks: mantidos no fluxo de item info onde ja existiam.
- Game logic: corrigida no resolvedor de slot.
- Pipelines: NamePlate, Goggles e Keyring nao tentam carregar NIF.
- Overflow: arrays baseados em `nLimit::Equip` acompanham `nTamer::MaxParts`.

## Build

Client:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Resultado:

- OK
- saida: `C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe`
- copiado para: `C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe`
- timestamp final: `08/06/2026 18:27:42`
- tamanho final: `13689344`

Server:

```powershell
dotnet build C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln -c Release
```

Resultado:

- OK
- warnings ja conhecidos: `net7.0` EOL e vulnerabilidades NuGet antigas em
  `AutoMapper` / `System.Linq.Dynamic.Core`.

## Observacao De Uso

Depois desta mudanca, o client novo precisa rodar contra o servidor novo. Se
um dos lados estiver antigo, a lista de equipamentos volta a ficar desalinhada
e itens podem aparecer duplicados, travados ou no slot visual errado.
