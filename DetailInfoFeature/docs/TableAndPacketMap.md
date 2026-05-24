# Table And Packet Map

## Tabelas copiadas

As tabelas foram copiadas de `Dcommerce/xml/AllBins/Data` e
`Dcommerce/Bin/Table`.

Status base:

- `TamerStats.xml/.bin`
- `DigimonStats.xml/.bin`

Encanto/equipamento:

- `AccEnchant.xml/.bin`
- `EquipEnchant.xml/.bin`
- `EnchantItemInfo.xml/.bin`
- `EnchantStatInfo.xml/.bin`

Selo mestre:

- `SealCard.xml/.bin`
- `SealLeader.xml/.bin`
- `SealDigimonImage.xml/.bin`
- `SealPlateImage.xml/.bin`
- `SealNatureImage.xml/.bin`
- `SealBaseImage.xml/.bin`
- `SealFilter.xml/.bin`

Atributos e recompensas:

- `AttributeReward.xml/.bin`
- `AttributeBase.xml/.bin`
- `AttributeNature.xml/.bin`

Avanco/breakthrough:

- `DM_Breakthrough-Stat.xml/.bin`
- `DM_Breakthrough-SlotInfo.xml/.bin`
- `DM_Breakthrough-Rate.xml/.bin`
- `DM_Breakthrough-Cost.xml/.bin`
- `DM_Breakthrough-EtcValue.xml/.bin`

## Textos copiados

Bins de linguagem em `language/bin`:

- `Achieve_Str.bin`
- `DigimonName_Str.bin`
- `DigimonTypeName_Str.bin`
- `Item_Str.bin`
- `SealMasterName_Str.bin`
- `Tamer_Str.bin`

`UIText.bin` nao existia no caminho English consultado nesta workspace, entao
as chaves de UI foram preservadas pelos dumps de string do executavel.

## Pacotes oficiais encontrados

Os dumps de string/RTTI apontam a janela de outro jogador para:

- `C2GS_REQ_OTHER_TAMER_DETAIL_INFO`
- `GS2C_RES_OTHER_TAMER_DETAIL_INFO`
- `nsOtherTamerDetailInfo::stTamer`
- `nsOtherTamerDetailInfo::stDigimon`

Esses nomes indicam que a janela da print e a versao de detalhes de outro
Tamer/Digimon, nao apenas a tela local de status.
