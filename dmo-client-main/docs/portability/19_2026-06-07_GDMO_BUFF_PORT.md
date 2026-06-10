# 2026-06-07 - Port dos Buffs GDMO para o Client Legado

## Resumo

Em 2026-06-07 foi feita a adaptacao dos buffs atuais do GDMO para o template legado do client v487, sem alterar renderer, protocolo de rede ou estrutura de leitura do client. A solucao converte os arquivos modernos `Buff_Data.bin` + `Buff_Str.bin` do GDMO para o formato antigo `Buff.bin` usado por `CsBuffMng`.

O objetivo principal foi corrigir os icones e tooltips de buffs de set de armadura, removendo reaproveitamento incorreto de buffs antigos como `Light of Kari(Va)` e `Hope of T.K.(No)`.

## Problema Corrigido

Antes, os efeitos permanentes/temporarios dos sets estavam sendo aplicados no servidor, mas a barra de buffs do client exibia icones errados porque o servidor enviava `BuffId` antigo reaproveitado.

Exemplo do erro anterior:

- `2702283` era mapeado para `40236`.
- `40236` no `Buff.bin` antigo e `Light of Kari(Va)`.
- O client recebia `40236` e mostrava corretamente o que existia no `Buff.bin`, mas isso era semanticamente errado para o set da Yolei.

Conclusao tecnica: nao era bug visual isolado. Era mismatch entre `SkillCode` de set e `BuffId` de UI.

## Fluxo Correto do Client

O client possui duas pipelines diferentes:

1. Tooltip do item equipado:
   - Usa `ItemList.bin` + `Skill.bin` + strings de skill.
   - Monta as linhas com checkbox do set.
   - Exemplo: `2702283` e `2702284` aparecem como efeitos do set.

2. Icone na barra de buffs:
   - Usa `Buff.bin`.
   - O pacote de buff envia `BuffId`.
   - O client busca `s_nBuffIcon`, `s_szName` e `s_szComment` no `Buff.bin`.

Por isso o bind correto deve ser:

```text
Set equipado -> SkillCode do efeito -> Buff.bin record com esse SkillCode -> BuffId enviado ao client
```

Nao deve ser:

```text
Set equipado -> tabela manual de BuffIds reaproveitados
```

## Fonte dos Dados

Arquivos modernos GDMO usados como fonte:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\notused\Buff_Data.bin
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\notused\Buff_Str.bin
```

Decoder usado:

```text
C:\0-NewDmo\new bins\pack03_unpack_v6.py
C:\0-NewDmo\new bins\unpacked_GDMO.exe
```

O decoder usa a cadeia Pack03 moderna com `callback_control=0x6C`. A validacao confirmou:

```text
Buff_Data.bin -> 1324 rows
Buff_Str.bin  -> 1324 rows
```

## Adaptacao Realizada

Foi criado o conversor:

```text
C:\0-NewDmo\TamerBinMigrationFeature\tools\Port-GdmoBuffsToLegacy.py
```

Ele faz:

- Decodifica `Buff_Data.bin` moderno.
- Decodifica `Buff_Str.bin` moderno.
- Junta dados e strings por `BuffId`.
- Converte cada row moderna para `CsBuff::sINFO` legado.
- Preserva qualquer buff legado que nao exista no GDMO.
- Gera um `Buff.bin` compativel com o client v487.
- Opcionalmente aplica no servidor e nos Pack03 do client.

Resultado gerado:

```text
C:\0-NewDmo\TamerBinMigrationFeature\generated\gdmo-buffs\Buff.bin
C:\0-NewDmo\TamerBinMigrationFeature\generated\gdmo-buffs\gdmo-buff-port-report.csv
```

Contagem final:

```text
legacy_count=784
gdmo_count=1324
merged_count=1325
```

O `merged_count=1325` significa:

- `1324` buffs vieram do GDMO.
- `1` buff legado foi preservado porque nao existia nos dados GDMO.

## Buffs de Set Validados

Os principais efeitos de set agora resolvem assim:

```text
2702279 -> 64403 Power of Courage 4-set       icon 2135
2702280 -> 64404 Power of Courage 6-set       icon 2136
2702281 -> 64405 Light of Hope 4-set          icon 2137
2702282 -> 64406 Light of Hope 6-set          icon 2138
2702283 -> 64407 Heart of Love 4-set          icon 2139
2702284 -> 64408 Heart of Love 6-set          icon 2141
2702826 -> 64533 Four Holy Beasts Power 6-Set icon 2125
```

Para o caso da Yolei:

```text
2702283 -> 64407 Heart of Love 4-set
2702284 -> 64408 Heart of Love 6-set
```

Isso remove o uso incorreto de:

```text
40233 Hope of T.K.(No)
40236 Light of Kari(Va)
```

## Mudanca no Servidor

Arquivo:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Asset\EquipmentSetBonusRegistry.cs
```

Antes, o `runtimeBuffId` era resolvido por switch manual.

Agora, o registry recebe a lista de `BuffInfo` carregada do `Buff.bin` e monta:

```text
SkillCode -> BuffId
```

Assim, quando um set novo aparece no `Skill.bin` e existe um buff correspondente no `Buff.bin`, o bind acontece automaticamente.

Arquivo:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\AssetsLoader.cs
```

O carregamento agora chama:

```csharp
EquipmentSetBonusRegistry.BuildFrom(ItemInfo, SkillCodeInfo, SkillInfo, BuffInfo)
```

## Como Esta Sendo Usado

Ao equipar/remover armadura:

1. O servidor reconta as pecas equipadas.
2. `EquipmentSetBonusRegistry` identifica quais efeitos do set estao ativos.
3. Para cada efeito ativo, ele usa o `SkillCode`.
4. O `SkillCode` e resolvido para `BuffId` pelo `Buff.bin`.
5. O servidor envia o `AddBuffPacket` com o `BuffId` correto.
6. O client renderiza o icone/nome/comentario pelo `Buff.bin` atualizado no Pack03.

Para efeitos temporarios:

- A chance, trigger e duracao continuam vindo do comentario/skill info ja usados pelo sistema atual.
- O buff visual temporario agora tambem usa o `BuffId` real do GDMO quando existir.

Para efeitos passivos:

- O status continua sendo aplicado pelo sistema de set bonus.
- O icone permanente pode aparecer porque agora existe `Buff.bin` real para o `SkillCode`.

## Arquivos Aplicados

Servidor local:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Buff.bin
```

Clients locais:

```text
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.hf
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.pf
C:\0-NewDmo\ClientDist1\Data\Pack03.hf
C:\0-NewDmo\ClientDist1\Data\Pack03.pf
```

VPS:

```text
/home/ubuntu/dmo-server-main-postgres/Bins/data/bin/english/Buff.bin
/home/ubuntu/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Asset/EquipmentSetBonusRegistry.cs
/home/ubuntu/dmo-server-main-postgres/src/Source/Application/DigitalWorldOnline.Application.GameAssets/AssetsLoader.cs
```

## Comandos Usados

Gerar sem aplicar:

```powershell
& "C:\Users\leo_h\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe" `
  "C:\0-NewDmo\TamerBinMigrationFeature\tools\Port-GdmoBuffsToLegacy.py"
```

Aplicar no servidor e Pack03 padrao:

```powershell
& "C:\Users\leo_h\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe" `
  "C:\0-NewDmo\TamerBinMigrationFeature\tools\Port-GdmoBuffsToLegacy.py" `
  --apply
```

Aplicar somente em um Pack03, sem tocar no servidor:

```powershell
& "C:\Users\leo_h\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe" `
  "C:\0-NewDmo\TamerBinMigrationFeature\tools\Port-GdmoBuffsToLegacy.py" `
  --apply `
  --no-server `
  --pack03 "C:\0-NewDmo\ClientDist1_x64\Data\Pack03"
```

Build local validado:

```powershell
dotnet build `
  "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj" `
  --configuration Release
```

Resultado:

```text
0 erros
964 warnings existentes
```

Deploy remoto:

```bash
cd /home/ubuntu/dmo-server-main-postgres
sudo docker compose up -d --build game
```

Status remoto confirmado:

```text
dmo-pg-game Up
GameServer started
```

## Validacao

Servidor local:

```text
count 1325
size 630704
expected 630704
```

VPS:

```text
remote_count 1325
size 630704
expected 630704
```

Pack03 local x64:

```text
C:\0-NewDmo\ClientDist1_x64\Data\Pack03
count 1325
64407 Heart of Love 4-set icon 2139 skill 2702283
64408 Heart of Love 6-set icon 2141 skill 2702284
```

Pack03 local Win32:

```text
C:\0-NewDmo\ClientDist1\Data\Pack03
count 1325
64407 Heart of Love 4-set icon 2139 skill 2702283
64408 Heart of Love 6-set icon 2141 skill 2702284
```

## Backups

Backups locais do servidor:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Buff.bin.bak_gdmo_buffs_20260607_154525
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Buff.bin.bak_gdmo_buffs_20260607_154611
```

Backups locais do Pack03:

```text
C:\0-NewDmo\ClientDist1\Data\Pack03.pf.bak_gdmo_buffs_20260607_154611
C:\0-NewDmo\ClientDist1\Data\Pack03.hf.bak_gdmo_buffs_20260607_154611
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.pf.bak_gdmo_buffs_20260607_154738
C:\0-NewDmo\ClientDist1_x64\Data\Pack03.hf.bak_gdmo_buffs_20260607_154738
```

Backups remotos foram criados em:

```text
/home/ubuntu/deploy-backups/gdmo-buffs-<timestamp>
```

## Checklist de Bug DMO

Packet mismatches:

- Corrigido o mismatch pratico entre `SkillCode` de set e `BuffId` enviado ao client.
- Protocolo nao foi alterado; `BuffId` continua `short/USHORT`.

Race conditions:

- Nao identificado como causa.
- O problema era deterministico por dados errados.

Null checks:

- Nao identificado como causa.
- O `Buff.bin` agora contem os records que o client precisa encontrar.

Game logic:

- O bind de set agora e dinamico por `SkillCode`.
- O sistema de aplicacao de status continua o mesmo.

Pipelines:

- Separadas corretamente:
  - tooltip de item por `Skill.bin`;
  - icone de buff por `Buff.bin`.

Overflow:

- Validado que os `BuffId` GDMO portados cabem em `USHORT`.
- Maior ID encontrado nos dados portados: `64646`.

## Riscos e Observacoes

- O client precisa ser aberto do zero para recarregar o Pack03 atualizado.
- Se um client estiver aberto, o patch do Pack03 pode falhar por arquivo em uso.
- O script nao trunca `BuffId` acima de `65535`; esses seriam ignorados para evitar bug silencioso.
- Os icones dependem dos assets de icone existentes no client. Os IDs de icone agora sao reais do GDMO, mas se alguma textura/icon atlas estiver faltando no pack, o proximo problema sera asset ausente, nao bind errado.

## Proxima Validacao In-Game

1. Abrir o client atualizado.
2. Equipar set com 4 pecas.
3. Confirmar tooltip do item com checkbox preenchido.
4. Confirmar icone permanente correto na barra de buffs.
5. Equipar 6 pecas.
6. Ativar efeito temporario por trigger.
7. Confirmar icone temporario correto e tooltip correto.
8. Repetir em pelo menos:
   - Yolei / Heart of Love;
   - T.K. / Light of Hope;
   - Davis / Power of Courage;
   - Four Holy Beasts.

## Ajuste Extra: Elegibilidade de Peca Permanente

Depois da primeira validacao in-game, o client ainda exibia `Light of Kari(Va)`
na barra de buffs em vez do icone do set. O `Buff.bin` local foi conferido e os
records do set existem:

```text
64403 -> 2702279
64404 -> 2702280
64405 -> 2702281
64406 -> 2702282
64407 -> 2702283
64408 -> 2702284
64533 -> 2702826
```

O ponto corrigido no servidor foi a validacao de peca equipada para bonus de
set. Antes o codigo dependia de `RemainingMinutes() != 0xFFFFFFFF`; isso e bom
para temporarios expirados, mas e fragil para item de roupa/armadura importado
como "No limits". Agora a elegibilidade do set usa `!item.Expired`, que e a
semantica correta do modelo.

Arquivos alterados:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\Services\EquipmentSetBonusService.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Character\CharacterModelBehavior.cs
```

Build local validado:

```powershell
dotnet build C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj --configuration Release
```

Resultado:

```text
0 Erro(s)
880 Aviso(s) existentes
```

Para testar esse ajuste nao precisa recompilar o client. Precisa rebuildar /
reiniciar o container `game` e fechar/abrir o client para recarregar o Pack03.
