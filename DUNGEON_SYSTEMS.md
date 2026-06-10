# Dungeon Systems Guide

Este documento registra o estado atual dos sistemas de dungeon no workspace
`C:\0-NewDmo`. Ele foi escrito para que um agente futuro consiga continuar o
trabalho sem precisar redescobrir, do zero, como a janela de entrada, a janela
de resultado, as instancias, as skills de boss e as recompensas se conectam.

O escopo ativo deste guia e o client C++ em:

```text
C:\0-NewDmo\dmo-client-main
```

e o servidor PostgreSQL/C# em:

```text
C:\0-NewDmo\dmo-server-main-postgres
```

Use o pseudo codigo local, os BINs locais e esses dois projetos como fonte de
verdade. Paths externos antigos devem ser tratados como referencia historica,
nao como codigo ativo.

## Visao Geral

O fluxo moderno de dungeon e:

1. O jogador conversa com um NPC de dungeon.
2. O client abre `cDungeonEntranceWindow`.
3. A janela monta a lista a partir dos BINs locais de dungeon e dos assets do
   pack.
4. Ao clicar em `Entrar`, o client envia `pInsMap::DungeonIN` com o portal.
5. O servidor resolve o portal, cria ou reutiliza uma instancia e registra o
   `DungeonId`, dificuldade, portal de entrada e mapa de origem.
6. A instancia controla steps de boss via `DungeonStepInfo.bin`.
7. Bosses usam skills reais vindas de `Monster.bin`/assets carregados pelo
   servidor, respeitando `UseTerms` e cooldown.
8. Ao matar o objetivo final, o servidor nao deve spawnar portal antigo. Ele
   envia `DungeonClearResultPacket` (`4135`).
9. O client abre `cDungeonClearResultWindow`, mostra rank, tempo, dificuldade,
   ranking detalhado, EXP/Bit e recompensas.
10. `Re Enter` cria uma nova copia da dungeon; `Exit` remove o jogador da
    instancia e manda para o mapa de saida.

Invariante importante: a dungeon precisa ser identificavel pelo mesmo
`DungeonId`/`PortalId`/dificuldade no client, no server e nos BINs. Quando um
desses tres lados diverge, os sintomas costumam ser nomes `()`, dificuldade
errada, reward vazio, entrada falhando ou clear result com dados genericos.

## Arquivos Principais

Client UI de entrada:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonEntranceWindow.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonEntranceWindow.cpp
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\Talk.cpp
```

Client UI de resultado:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearResultWindow.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearResultWindow.cpp
```

Client packets de dungeon:

```text
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameInsMap.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGame.cpp
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGame.h
C:\0-NewDmo\dmo-client-main\common_vs2019\pInsMap.h
```

Server instancias, clear e boss logic:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DungeonsServer\DungeonsServerBaseOperation.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DungeonsServer\DungeonsServerMonsterOperation.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Maps\MapInstance.cs
```

Server packets/handlers de resultado:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\DungeonClearResultPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\DungeonClearExitPacketProcessor.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\DungeonReEnterPacketProcessor.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Enums\PacketProcessor\GameServerPacketEnum.cs
```

Server loader dos BINs de dungeon:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\DungeonBinLoader.cs
```

Boss skills:

```text
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\Managers\MonsterSkillRotation.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Maps\MapInstanceMobBehavior.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\MonsterSkillVisualPacket.cs
C:\0-NewDmo\dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Packets\GameServer\Combat\SkillHitPacket.cs
```

Estruturas recuperadas dos BINs:

```text
C:\0-NewDmo\new bins\PACK03_BIN_STRUCTURES.md
C:\0-NewDmo\new bins\PACK03_COMBINED_VIEW_NOTES.md
```

Pseudo codigo local relevante:

```text
C:\0-NewDmo\new bins\unpacked_exe_all\functions
```

## BINs de Dungeon

O servidor carrega a familia de dungeon em `DungeonBinLoader.Load()`:

```text
DungeonList.bin
DungeonClearInfo.bin
DungeonReward.bin
DungeonStepInfo.bin
DungeonValue.bin
DungeonQualification.bin
```

O client de entrada tambem le:

```text
Data\Bin\Table\DungeonList.bin
Data\Bin\Table\DungeonQualification.bin
Data\Bin\Table\DungeonObtainable.bin
Data\Bin\Table\DungeonReward.bin
Data\Bin\Language\English\Dungeon_Str.bin
```

### DungeonList.bin

Layout validado no pseudo:

```text
u32 count
u32 dungeonId
u16 difficulty
u16 sortGroup
string imagePath
u32 portalId
u16 extra
```

No servidor:

```text
DungeonListRecord(DungeonId, Difficulty, SortGroup, ImagePath, PortalId, Extra)
BaseMapId = DungeonId / 100
```

Dificuldades esperadas:

```text
1 = Easy
2 = Normal
3 = Hard / Dificil
```

Nao use fallback generico para dificuldade. Se uma dungeon precisa de Easy,
Normal e Hard, os tres registros devem existir de forma coerente no BIN e no
servidor. A lista lateral conversa com o servidor pelo portal/dungeon record;
visual certo sem dado certo ainda quebra a entrada.

### DungeonQualification.bin

Usado para min/max players e condicoes de entrada.

Campos relevantes:

```text
u32 dungeonId
u32 fieldA
u32 fieldB
u16 minPlayers
u16 maxPlayers
requirements[]
```

O client usa isso para:

```text
s_wMinPlayers
s_wMaxPlayers
s_vRequirements
```

O servidor usa para:

```text
DungeonBin.GetMaxPlayers(dungeonId)
```

Sintoma tipico de divergencia: texto `Para 01P` quando a dungeon deveria ser
`Para 04P`, ou resultado mostrando party max incorreto.

### DungeonObtainable.bin

Este BIN representa itens obtiveis/monster reward da dungeon. Ele alimenta o
painel pequeno `Recompensa Monstro` da janela de entrada.

Layout:

```text
u32 count
u32 dungeonId
u32 itemCount
u32 itemId[itemCount]
```

No client:

```text
cDungeonEntranceWindow::_LoadDungeonObtainableBin()
s_vMonsterRewards
```

Nao confundir com clear reward.

### DungeonReward.bin

Este BIN representa recompensas de conclusao da dungeon. Ele alimenta a tela do
bau na entrada e a janela de resultado no servidor/client.

Layout:

```text
u32 count
u32 dungeonId
u32 rewardGroupCount
u8  groupKey
u32 rewardCount
u32 itemId
u32 amount
```

No client:

```text
cDungeonEntranceWindow::_LoadDungeonRewardBin()
s_vCompletionRewards
```

No servidor:

```text
DungeonBinLoader.LoadDungeonReward()
DungeonBin.Rewards
DungeonsServerMonsterOperation.ToDungeonClearPacketRewards()
```

Regra atual de grupos:

```text
groupKey == 0  -> recompensas basicas/default
groupKey != 0  -> recompensas extras/adicionais
```

Nao use `DungeonObtainable.bin` para a tela de resultado. Isso troca reward de
entrada/monster reward por recompensa de conclusao e gera item errado.

### DungeonStepInfo.bin

Controla a ordem dos bosses/objetivos. E o BIN que impede o jogador de ignorar
boss intermediario e matar o boss final direto.

Layout:

```text
u32 count
u32 dungeonId
u32 stepGroupCount
u8  stepKey
u32 objectiveCount
u32 objectiveId
u8  fieldA
u32 fieldB
u8  fieldC
u32 targetMonsterType
u32 amount
```

No servidor:

```text
DungeonBin.GetOrderedSteps()
DungeonBin.GetObjectiveStep()
DungeonBin.GetNextStep()
DungeonBin.IsFinalObjective()
DungeonBin.HasCompletedPriorSteps()
```

Fluxo:

1. Ao criar a instancia, `GateDungeonStepMobs()` bloqueia todos os objetivos que
   nao pertencem ao primeiro step.
2. Ao matar um objetivo, `HandleDungeonStepProgression()` registra kill por
   `mobId` e `mobType`.
3. Quando o step completa, `UnlockDungeonStep()` libera os mobs do proximo step.
4. `TryHandleDungeonClear()` so aceita o boss final se todos os steps anteriores
   estiverem em `DungeonCompletedStepKeys`.

### DungeonClearInfo.bin

Controla tempo limite e falhas usadas no resultado.

Layout:

```text
u32 count
u32 dungeonId
u32 clearConditionCount
u8  conditionKey
u32 value
u8  flag
```

Uso atual:

```text
conditionKey == 1 -> TimeLimitSeconds
conditionKey == 2 -> FailLimit
```

### DungeonValue.bin

Usado pelo calculo de rank.

Valores validados no pseudo antigo:

```text
(1, 40)
(4, 60)
(6, 80)
```

No servidor:

```text
DungeonBin.CalculateRank()
```

Regra atual:

```text
failCount >= failLimit -> C
sem time limit          -> S
elapsed <= S limit      -> S se failCount == 0
elapsed <= A limit      -> A
elapsed <= B limit      -> B
restante                -> C
```

O servidor envia o rank logico:

```text
0 = S
1 = A
2 = B
3 = C
4 = F
```

No client, a spritesheet fisica `DungeonUI\Dungeon_Result_Rank.png` usa
token `170x142` e estados em ordem de atlas:

```text
0 = F
1 = E
2 = D
3 = C
4 = B
5 = A
6 = S
```

## Janela de Entrada

Classe:

```text
cDungeonEntranceWindow
```

Arquivo:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonEntranceWindow.cpp
```

Criacao da janela:

```text
cDungeonEntranceWindow::Create()
```

Assets principais:

```text
DungeonUI\Dungeon_Bg.tga
DungeonUI\Dungeon_List_Frame.tga
DungeonUI\Dungeon_Image_Frame.tga
DungeonUI\Dungeon_Target_Participation_Frame.tga
DungeonUI\Dungeon_Reward_Frame.tga
DungeonUI\Dungeon_Line.tga
DungeonUI\Dungeon_List_Btn.tga
DungeonUI\Dungeon_Level_Easy.tga
DungeonUI\Dungeon_Level_Normal.tga
DungeonUI\Dungeon_Level_Hard.tga
DungeonUI\Dungeon_reward_btn.png
DungeonUI\Dungeon_Entrance_Btn.tga
DungeonUI\Dungeon_User_Icon.tga
DungeonUI\Dungeon_Slot.tga
```

### Fonte dos dados

Ordem atual de carregamento:

```text
_LoadPortalDungeonData()
_LoadDungeonListBin()
_LoadDungeonStringBin()
_LoadDungeonQualificationBin()
_LoadDungeonObtainableBin()
_LoadDungeonRewardBin()
```

Depois:

```text
_FilterByTarget()
_RefreshList()
_RefreshDetail()
```

Campos por entrada:

```text
s_dwDungeonId
s_dwPortalId
s_dwSourceMapId
s_dwDestinationMapId
s_dwNpcObjectId
s_wDifficulty
s_wSortGroup
s_wMinPlayers
s_wMaxPlayers
s_strImagePath
s_wsName
s_wsDescription
s_vRequirements
s_vMonsterRewards
s_vCompletionRewards
```

### Lista lateral

Metodo:

```text
cDungeonEntranceWindow::_RefreshList()
```

Cada card e montado com:

```text
m_pListButtonBg
m_pListButtonStroke
m_pListDifficultyBadge
m_pListText
m_pListButton
```

O fundo do card vem de `entry.s_strImagePath`, normalmente algo como:

```text
DungeonUI\DungeonImage\Dungeon_1600.png
```

Se a imagem do BIN nao existir, o client tenta candidatos por destination map e
por `dungeonId / 100`. Se mesmo assim faltar, o card deve esconder o fundo e
logar `DUNGEON_UI list image missing`.

O texto do card e cortado por largura em `DungeonFitListCardText()`, usando
reticencias. Nao deixe nomes quebrarem linha no card: fica visualmente errado e
desalinha a lista.

O badge de dificuldade vem de:

```text
DungeonDifficultyTexture()
```

Mapeamento:

```text
1 -> Dungeon_Level_Easy.tga
2 -> Dungeon_Level_Normal.tga
3 -> Dungeon_Level_Hard.tga
```

### Preview principal

Metodo:

```text
cDungeonEntranceWindow::_RefreshDetail()
```

O preview usa:

```text
m_pPreviewSprite
m_pPreviewDifficultyBadge
m_pTitleText
```

O badge da dificuldade tambem precisa aparecer no preview principal, nao so na
lista lateral.

### Condicoes de participacao

Metodos:

```text
_BuildRequirementLines()
_RefreshDetail()
```

A lista e uma sequencia plana de linhas. Nao existe hierarquia real entre
titulo e descricao; o visual so diferencia titulo pela cor/textura de linha.

Regra visual atual:

```text
s_bSection == true  -> titulo verde + Dungeon_Line.tga
s_bSection == false -> texto branco normal
```

Secoes esperadas:

```text
Condicoes de Participacao
Itens de entrada
Passo a passo
```

Cada titulo e cada descricao entram no mesmo vetor, via `AddLine()` ou
`AddWideLine()`. Nao renderize descricoes como filhos/subtitulos de outro
titulo. O scroll do frame deve rolar a sequencia inteira.

O texto de descricao longa e quebrado por largura em `AddWrappedDescriptionLine`.
Se ficar ilegivel, ajuste a largura ali, nao no render.

### Recompensa Monstro

Painel pequeno a direita da entrada. Usa:

```text
s_vMonsterRewards
DungeonObtainable.bin
m_pRewardSlot
m_pRewardScrollBar
```

Layout atual:

```text
MAX_REWARD_COLUMNS = 4
MAX_REWARD_ROWS = 2
```

A scrollbar deve ficar visualmente presente e so ativar rolagem quando passar
do limite visivel.

### Bau de recompensa de conclusao

Botao:

```text
m_pCompletionOpenIcon
m_pCompletionOpenButton
DungeonUI\Dungeon_reward_btn.png
```

Ao clicar, a janela troca o frame principal para o modo:

```text
_SetCompletionRewardMode(true)
_RefreshCompletionRewardPanel()
```

Esse painel usa:

```text
s_vCompletionRewards
DungeonReward.bin
```

Se estiver vazio, aparece:

```text
Sem recompensas configuradas.
```

Esse texto so deve aparecer quando `DungeonReward.bin` realmente nao tiver
grupo/itens para o dungeonId selecionado. Se o painel da entrada mostra item e o
bau nao, compare `s_vMonsterRewards` contra `s_vCompletionRewards`: eles vem de
BINs diferentes.

### NPC e filtro da lista

`Talk.cpp` abre a janela. `SetTarget()` passa o NPC atual para:

```text
_FilterByTarget()
```

Quando o NPC e `NT_GDMO_PORTAL_TEST_NPC`, a lista filtra por:

```text
NPC id
map id de origem
IDs permitidos no payload do NPC
```

Se a janela abre vazia:

```text
DUNGEON_UI no linked MapPortal dungeon entries
```

e um sinal de divergencia entre NPC, `MapPortal`, `DungeonList.bin` e os IDs
permitidos no tipo do NPC.

## Janela de Resultado

Classe:

```text
cDungeonClearResultWindow
```

Arquivos:

```text
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearResultWindow.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearResultWindow.cpp
```

Criacao:

```text
cDungeonClearResultWindow::Create()
```

Antes da janela grande, o client mostra o aviso `CLEAR` em uma janela separada:

```text
cDungeonClearAnnounceWindow
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearAnnounceWindow.h
C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\DungeonClearAnnounceWindow.cpp
DungeonUI\dungeon_result_simple_ui.png
```

`cCliGame::RecvDungeonClearResult()` recebe o pacote, abre
`WT_DUNGEON_CLEAR_ANNOUNCE`, espera cerca de 1.45s e so entao abre
`WT_DUNGEON_CLEAR_RESULT`.

Assets principais:

```text
DungeonUI\Dungeon_window.png
DungeonUI\Dungeon_Result_Rank.png
DungeonUI\Dungeon_result_bg.png
DungeonUI\Dungeon_stats_bg.png
DungeonUI\Dungeon_stats_box_01.png
DungeonUI\Dungeon_stats_box_deco.png
DungeonUI\Dungeon_tamer_bg.png
DungeonUI\Dungeon_digimon_bg.png
DungeonUI\Dungeon_stats_icon.png
DungeonUI\Dungeon_icon_XP.png
DungeonUI\Dungeon_icon_bit.png
TargetWindow\TamerLevel.tga
DungeonUI\dungeon_result_ui_Btn_restart.png
DungeonUI\dungeon_result_ui_Btn_exit.png
CommonUI\ItemSlot.tga
```

Os botoes `Re Enter` e `Exit` seguem o pseudo codigo em
`010c6020_FUN_010c6020.c`: posicoes `690,220` e `810,220`, tamanho `109x37`,
e token vertical `0,37`. Esse token e o que ativa os estados `normal`,
`mouse_on` e `click` da textura.

### Pseudo codigo de referencia

Funcoes locais uteis:

```text
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c4920_FUN_010c4920.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c4fa0_FUN_010c4fa0.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c6020_FUN_010c6020.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c6930_FUN_010c6930.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c7cb0_FUN_010c7cb0.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c8070_FUN_010c8070.c
C:\0-NewDmo\new bins\unpacked_exe_all\functions\010c8830_FUN_010c8830.c
```

Essas funcoes mostram, respectivamente, a janela raiz/rank, linhas de stats,
botoes, cards de player, EXP/Bit, secoes de item e atualizacao/render do
resultado.

### Dados exibidos

Struct no client:

```text
cDungeonClearResultWindow::sResultData
```

Campos:

```text
s_dwDungeonId
s_dwEntryPortalId
s_byRank
s_wDifficulty
s_dwElapsedSeconds
s_wPartyCount
s_wPartyMax
s_wFailCount
s_wFailMax
s_dwExp
s_dwBits
s_wsDungeonName
s_vDefaultRewards
s_vExtraRewards
s_vDetailResults
```

O rank enviado pelo servidor e logico:

```text
0 -> S
1 -> A
2 -> B
3 -> C
4 -> F
```

Na spritesheet `DungeonUI\Dungeon_Result_Rank.png`, o frame `0` e `F`, o
frame `4` e `B`, o frame `5` e `A` e o frame `6` e `S`; por isso o client
converte o rank antes de chamar `SetState()`. Se essa conversao ficar
desatualizada, uma dungeon finalizada rapido pode aparecer com rank visual
errado mesmo quando o servidor enviou o rank correto.

Cards detalhados:

```text
MAX_DETAIL_CARD_COUNT = 3
0 -> Tamer Corajoso
1 -> Tamer de confianca
2 -> Tamer Nobre
```

No servidor, os cards vem do dano do boss final:

```text
BuildDungeonClearDetailResults(map, raidDamage)
CreateDungeonClearDetail(client, category, damage)
```

Se nao houver dano valido, existe fallback para o primeiro client com valor `0`.
Isso evita janela vazia, mas nao deve mascarar bug de ranking. Se os cards nao
aparecem ou aparecem com `Nao aplicavel`, investigue o `RaidDamage` do boss
final e o vinculo `TamerId -> GameClient`.

### Botoes

No client:

```text
Re Enter -> net::game->SendDungeonReEnter()
Exit     -> net::game->SendDungeonClearExit()
```

No server:

```text
DungeonReEnterPacketProcessor
DungeonClearExitPacketProcessor
```

`Re Enter` limpa a instancia antiga e manda os membros para a primeira regiao do
mapa da dungeon com o mesmo `entryPortalId`.

`Exit` remove os membros da instancia, limpa `LastDungeonEntry` e usa destino:

```text
MapGroup(instance.MapId)
fallback ResurrectionMapId
fallback map 3
```

## Contrato de Pacotes

Namespace client:

```text
pInsMap
```

IDs client:

```text
DungeonIN
DungeonClearResult
DungeonClearExit
DungeonReEnter
```

IDs server:

```text
DungeonClearResult = 4135
DungeonClearExit   = 4136
DungeonReEnter     = 4137
```

### Entrada

Client:

```text
cCliGame::SendInsMap_DungeonIN(uint nPortalNo)
```

Payload:

```text
u32 portalNo
```

Server:

```text
WarpGateDungeonPacketProcessor
client.SetLastDungeonEntry(portalId, sourceMapId)
```

### Resultado

Server:

```text
DungeonClearResultPacket
```

Ordem exata do payload:

```text
i32 dungeonId
i32 entryPortalId
u8  rank
u16 difficulty
i32 elapsedSeconds
u16 partyCount
u16 partyMax
u16 failCount
u16 failMax
i32 earnedExp
i32 earnedBits
str dungeonName
rewardList defaultRewards
rewardList extraRewards
detailList detailResults
```

`dungeonName` deve vir de `Dungeon_Str.bin` pelo `dungeonId` real. Nao use
`MapInstance.Name` para resultado final: ele pode ser apenas `Map 1600`.

`rewardList`:

```text
u16 count
i32 itemId
i32 amount
```

`detailList`:

```text
u16 count
u8  category
i32 tamerModelId
i32 digimonModelId
u16 tamerLevel
u16 digimonLevel
i32 value
str tamerName
str digimonName
```

Client:

```text
cCliGame::RecvDungeonClearResult()
```

O parser le `detailList` somente se `GetReadAvailable() >= sizeof(WORD)`.
Isso mantem compatibilidade com pacote antigo, mas o pacote moderno deve mandar
os detalhes sempre.

## Ciclo da Instancia

Criacao da instancia:

```text
DungeonsServerBaseOperation
MapInstance.StartDungeonRun()
```

Estado guardado em `MapInstance`:

```text
DungeonId
DungeonEntryPortalId
DungeonEntranceMapId
DungeonRecordId
DungeonDifficulty
DungeonStartedAt
DungeonFailCount
DungeonClearAnnounced
DungeonCompletedStepKeys
DungeonObjectiveKillCounts
DungeonObjectiveRewardedMobIds
```

Ao entrar:

1. `WarpGateDungeonPacketProcessor` resolve o portal e salva
   `LastDungeonEntryPortalId`/`LastDungeonEntranceMapId`.
2. `DungeonsServerBaseOperation` cria a instancia para party ou solo tamer.
3. `ResolveByRuntimeMapId(dto.MapId, client.LastDungeonEntryPortalId)` encontra
   o registro de dungeon correto.
4. `StartDungeonRun()` inicia cronometro, falhas, dificuldade e step state.
5. `GateDungeonStepMobs()` bloqueia objetivos dos steps posteriores.
6. `RemoveLegacyDungeonExitPortals()` remove os portais antigos de saida.

Portais antigos removidos:

```text
51991
51992
```

Nao reintroduza esses portais como solucao para saida de dungeon. O fluxo certo
e janela de resultado + botoes `Re Enter`/`Exit`.

## Boss Skills

O servidor escolhe skills em:

```text
DungeonsServerMonsterOperation.cs
case MobActionEnum.UseAttackSkill
```

Fluxo:

1. Se o mob esta em cast, aguarda.
2. Quando o cast completa, busca a skill em `_assets.MonsterSkillInfo` por:

```text
Type == mob.Type
SkillId == castingId
```

3. Executa:

```text
map.SkillTarget(mob, castingSkill, _assets.NpcColiseum)
mob.MarkSkillCooldown(castingId, castingSkill.Cooldown)
```

4. Se nao esta castando, monta lista de skills:

```text
_assets.MonsterSkillInfo
  .Where(Type == mob.Type)
  .Where(!mob.IsSkillOnCooldown(SkillId))
  .Where(MonsterSkillRotation.TermMatches(...))
```

5. Escolhe uma aleatoria da lista valida e chama:

```text
mob.StartCast(targetSkill.SkillId, targetSkill.CastingTime)
```

Nao crie fallback generico de skill para boss. Se o boss nao solta skill, a
correcao boa e garantir que o boss tem rows reais em `Monster.bin`/asset skill,
que `UseTerms` permite o cast e que a animacao do modelo existe no client.

### UseTerms

Arquivo:

```text
MonsterSkillRotation.cs
```

Valores:

```text
0  -> sempre elegivel
1  -> target HP% <= MinValue
2  -> target HP% >= MinValue
3  -> target HP <= MinValue
4  -> target HP >= MinValue
5  -> mob HP% <= MinValue
6  -> mob HP% >= MinValue
7  -> mob HP <= MinValue
8  -> mob HP >= MinValue
9..16 -> padroes equivalentes para DS
```

`UseTerms=0` e valido e deve passar. Se bloquear `0`, varios bosses nunca
castam.

### Visual e dano da skill

Packets:

```text
MonsterSkillVisualPacket -> 1123
SkillHitPacket           -> 1102
```

O client resolve a animacao em:

```text
C:\0-NewDmo\dmo-client-main\DProject\network\cCliGameReceive.cpp
cCliGame::SetSkillInfo2()
```

Se aparecer assert parecido com:

```text
AniID = ..., ModelID = ..., LoadSeqModelID = ...
```

investigue:

1. `SkillId` enviado pelo servidor.
2. Row da skill em `Monster.bin`.
3. `s_dwSequenceID`/animacao do modelo no client.
4. Se o servidor voltou a mandar slot artificial em vez de skill real.

## Ranking de Dano

O ranking de dano do boss final usa:

```text
mob.RaidDamage
```

Esse dicionario e atualizado em modelos de mob/summon quando o tamer causa
dano. A janela de resultado usa os top 3:

```text
BuildDungeonClearDetailResults()
OrderByDescending(Damage)
Take(3)
```

Sintomas:

```text
cards vazios
Nao aplicavel nos tres cards
valor 0 sempre
ranking de dano nao aparece ao matar boss
```

Checklist:

1. O boss final e realmente o `mob` passado para `TryHandleDungeonClear()`?
2. O `raidDamage` passado e `mob.RaidDamage`, nao um dicionario novo/vazio?
3. O dano esta sendo registrado com `TamerId`, nao PartnerId.
4. O `GameClient` ainda esta dentro de `map.Clients` no momento do clear.
5. O boss final nao foi removido/resetado antes de construir o pacote.

## Recompensas

Existem tres familias que nao devem ser misturadas:

```text
DungeonObtainable.bin -> painel Recompensa Monstro na entrada
DungeonReward.bin     -> clear reward/bau/resultado
Drop/Raid rewards     -> drops e rewards de combate normais
```

No clear result:

```text
defaultRewards = groupKey == 0
extraRewards   = groupKey != 0
```

No client:

```text
_DefaultRewardPos(index) -> grade 4 colunas
_ExtraRewardPos(index)   -> grade 4 colunas
```

Se uma dungeon nao mostra reward:

1. Confirme se `DungeonReward.bin` tem row para o `DungeonId` exato.
2. Confirme se o `DungeonId` da instancia e o record de dificuldade certo.
3. Confirme se os itens existem no `ItemList`/icones do client.
4. Confirme se o pack03/arquivos BIN atuais foram deployados para client e
   server.
5. Nao use item de entrada como fallback visual.

## Assets e Texturas Rosa

Textura rosa significa uma destas coisas:

```text
path errado
asset ausente no pack
token/atlas errado
case/path divergente
sprite criada com tamanho/estado invalido
```

Antes de mexer em layout, valide se o asset existe no pack correto. No client a
janela usa `CsFPS::CsFPSystem::IsExist()` e tambem tenta:

```text
data\interface\<path>
```

Referencias confirmadas do pseudo:

```text
DungeonUI\Dungeon_Bg.tga
DungeonUI\Dungeon_window.png
DungeonUI\Dungeon_Result_Rank.png
DungeonUI\dungeon_result_ui_Btn_exit.png
DungeonUI\dungeon_result_ui_Btn_restart.png
DungeonUI\Dungeon_reward_btn.png
DungeonUI\Dungeon_User_Icon.tga
DungeonUI\Dungeon_Line.tga
```

Caso importante ja investigado: a linha verde horizontal no topo da entrada nao
estava dentro de `DungeonUI\Dungeon_Bg.tga`; ela era criada por codigo. Quando
um artefato visual nao aparece no bitmap renderizado do asset, procure sprites
ou linhas criadas no `Create()`/`Render()`.

## Pseudo Codigo: Mapa Rapido

Entrada:

```text
010c8fa0_FUN_010c8fa0.c  -> usa DungeonUI\Dungeon_Bg.tga
010c93b0_FUN_010c93b0.c  -> frame/lista/linha/click list
010ca260_FUN_010ca260.c  -> reward frame/linha
010ca940_FUN_010ca940.c  -> Dungeon_User_Icon.tga
010cbd20_FUN_010cbd20.c  -> image frame, difficulty badge, reward button
010ccc80_FUN_010ccc80.c  -> preload Easy/Normal/Hard textures
```

Resultado:

```text
010c4920_FUN_010c4920.c  -> janela raiz, rank, header Resultado Detalhado
010c4fa0_FUN_010c4fa0.c  -> linhas de stat
010c6020_FUN_010c6020.c  -> botoes Exit/Re Enter
010c6930_FUN_010c6930.c  -> cards de player/tamer/digimon/ranking
010c7cb0_FUN_010c7cb0.c  -> secao Ganhou EXP, Bit
010c8070_FUN_010c8070.c  -> secoes Item obtido/Item Adicional Obtido
010c8830_FUN_010c8830.c  -> update/render do resultado
```

BIN family:

```text
01607560_FUN_01607560.c  -> registra/load da familia Dungeon*.bin
```

## Checklists de Manutencao

### Adicionar ou corrigir uma dungeon

1. Atualize `DungeonList.bin` com `DungeonId`, dificuldade, sort, imagePath e
   portal.
2. Atualize `Dungeon_Str.bin` com nome/descricao.
3. Atualize `DungeonQualification.bin` com min/max players e requisitos.
4. Atualize `DungeonObtainable.bin` se o painel `Recompensa Monstro` precisa
   mostrar itens.
5. Atualize `DungeonReward.bin` se a conclusao/bau/resultado precisa mostrar
   reward.
6. Atualize `DungeonStepInfo.bin` com a cadeia de bosses.
7. Atualize `DungeonClearInfo.bin` com tempo/falhas.
8. Garanta assets `DungeonUI\DungeonImage\...` e badges no pack correto.
9. Confirme que client e server usam os mesmos BINs.

### Corrigir entrada que nao abre dungeon

1. Verifique se `SendInsMap_DungeonIN()` envia o portal correto.
2. Verifique `WarpGateDungeonPacketProcessor` e validacao de portal.
3. Verifique `client.SetLastDungeonEntry(portalId, sourceMapId)`.
4. Verifique `ResolveByRuntimeMapId(mapId, entryPortalId)`.
5. Verifique se `DungeonList.bin` tem o portal/dificuldade correta.
6. Leia logs do server para `Dungeon warp rejected`.

### Corrigir boss que nao solta skill

1. Confirme rows de `_assets.MonsterSkillInfo` para `mob.Type`.
2. Confirme `SkillId`, `Cooldown`, `CastingTime`, `UseTerms`, `MinValue`.
3. Confirme que `MonsterSkillRotation.TermMatches()` retorna true.
4. Confirme que `mob.IsSkillOnCooldown()` nao esta sempre true.
5. Confirme que `map.SkillTarget()` implementa o `SkillType`.
6. Confirme `MonsterSkillVisualPacket` e `SkillHitPacket` no client.
7. Se der assert de animacao, ajuste a skill/model asset, nao invente skill
   fake.

### Corrigir clear que cria portal antigo

1. Verifique `RemoveLegacyDungeonExitPortals()`.
2. Verifique se a finalizacao passa por `TryHandleDungeonClear()`.
3. Verifique se `TryHandleDungeonClear()` retorna true ao reconhecer objetivo
   final.
4. Verifique se `TargetKillSpawn()`/`SourceKillSpawn()` nao esta rodando para o
   portal legado apos o clear.
5. Verifique se o mob final esta em `DungeonStepInfo.bin`.

### Corrigir janela de resultado incompleta

1. Confirme pacote `4135` no server e no client.
2. Confirme a ordem exata dos campos do payload.
3. Confirme `detailResults` com top 3 de `RaidDamage`.
4. Confirme `TamerModelId` e `DigimonModelId` carregaveis por
   `g_pCharResMng->CharImageResLoad()`.
5. Confirme `DungeonReward.bin` para `defaultRewards` e `extraRewards`.
6. Confirme assets da janela em `pack01`/interface.

### Corrigir reward errado

1. Se aparece item de entrada no painel errado, alguem misturou
   `DungeonObtainable.bin` com `DungeonReward.bin`.
2. Se so a dungeon de teste mostra reward, compare o `DungeonId` selecionado
   contra as rows reais de `DungeonReward.bin`.
3. Se o tooltip mostra item inesperado, confirme o `itemId` no BIN e o
   `ItemList`.
4. Se a janela do bau diz `Sem recompensas configuradas`, confirme se
   `s_vCompletionRewards` esta vazio ou se o scroll/index esta escondendo os
   slots.

## Build, Deploy e Verificacao

Build padrao do client:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=X64_WinDX9_Bridge /p:Platform=x64 /v:minimal
```

Saida:

```text
C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe
```

Copiar para runtime:

```powershell
Copy-Item "C:\0-NewDmo\Release\GSP_x64\lk-dmo-x64.exe" "C:\0-NewDmo\ClientDist1_x64\lk-dmo-x64.exe" -Force
```

Build do server:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" -c Release
```

Deploy do server:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1
```

Deploy rapido reaproveitando arquivo:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File C:\0-NewDmo\Publish-DmoServerToVpsDockerComposeWithPassword.ps1 -ReuseLatestArchive
```

Nao use `ssh`/`scp` direto sem chave configurada; o wrapper gerencia senha e
evita travamento em prompt interativo.

## Observacoes de Engenharia

Ao tocar esse sistema, compare sempre quatro camadas:

```text
client C++
server C#
BINs locais
pseudo codigo/assets
```

Os bugs mais caros aqui aparecem quando uma camada e corrigida isoladamente. Um
exemplo classico: deixar a lista visual com Easy/Normal/Hard no client, mas o
server ainda resolver apenas duas dificuldades. Outro exemplo: ajustar o icone
da reward visualmente, mas ler reward do BIN errado.

Antes de concluir qualquer alteracao, verifique:

```text
packet mismatch
race condition de troca de mapa/instancia
null checks em client, tamer e partner
logica de dungeon step/clear
pipeline de skill visual + hit
overflow/conversao de long damage para int
```

Regra pratica: se o pseudo codigo mostra uma tabela ou asset especifico, nao
substitua por fallback generico ate provar que a fonte real esta ausente. Na
maioria dos casos deste sistema, o dado correto existe; o bug costuma estar no
path, no record id, no group key ou na dificuldade.
