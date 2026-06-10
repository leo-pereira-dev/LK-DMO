# DProject C# Port Checklist

Objetivo: portar o cliente C++ para C# mantendo a arquitetura e nomenclatura reconhecivel do `DProject`, com placeholders para Gamebryo ate as libs oficiais portadas ficarem prontas.

## Regra De Port

- Manter a estrutura conceitual do C++: `main` -> `CGameApp` -> `CFlowMgr` -> `CFlow` -> `ContentsSystem`.
- Manter nomes legados quando eles carregam significado no projeto: `CGameApp`, `CFlowMgr`, `CFlowFactory`, `CLoginFlow`, `CEngine`, `NiRenderer`, `NiFile`.
- Centralizar configuracoes em `appsettings.json`.
- Toda chamada Gamebryo deve passar por classes placeholder em `Gamebryo2.3` / namespace `DProject.Gamebryo`.
- Rede deve comecar por contratos e stubs, depois receber pacote/binario real do protocolo.
- O port deve compilar antes de tentar render real.
- Multithread desde o inicio: render/main thread fica dono de Gamebryo/DirectX; rede, loading, descompressao, file tables e preparacao de assets rodam fora do frame loop.

## Prioridade Multithread

Multithread e requisito de arquitetura do port, nao etapa de otimizacao posterior.

- [x] Todo fluxo de tela deve ser comandado pelo main thread.
- [x] Comandos de troca de flow podem ser solicitados por qualquer thread, mas sao executados apenas no main thread.
- [x] Rede deve rodar em thread propria ou modo pump configuravel.
- [x] Loading/file tables/pack/decompress/hash devem rodar em workers.
- [x] Render/Gamebryo devem ser atualizados por fila de comandos consumida no main/render thread.
- [x] Sockets reais devem separar `receive`, `decode`, `dispatch`.
- [x] Packet decode pesado deve poder ir para worker, mantendo ordenacao por conexao.
- [x] Asset loading deve produzir DTOs/dados imutaveis; criacao de objetos renderizaveis acontece no main thread.
- [x] Criar metricas por thread: frame time, network queue, job queue, render command queue.
- [x] Definir pontos de sincronizacao para map transition e combat state.

Regra de seguranca: nenhum worker deve mutar `ContentsSystem`, `CFlowMgr`, UI, `GlobalData` sensivel ou objetos Gamebryo diretamente. Worker publica resultado em filas; main thread aplica.

## Fluxo Basico Inicial

- [x] Criar projeto C# paralelo: `DProject.CSharp`.
- [x] Configurar build C# `Release_English`.
- [x] Criar `appsettings.json` com valores vindos do `dmo.ini`.
- [x] Criar `Program.cs` equivalente ao `_tWinMain`.
- [x] Criar `App/CWin32App.cs` com loop basico.
- [x] Criar `App/CGameApp.cs` com inicializacao principal.
- [x] Criar `Flow/CFlow.cs` com lifecycle original.
- [x] Criar `Flow/CFlowMgr.cs`.
- [x] Criar `Flow/CFlowFactory.cs`.
- [x] Criar `LogoFlow`, `LoginFlow`, `ServerSelectFlow`, `CharacterSelectFlow`, `LoadingFlow`, `InGameFlow`.
- [x] Criar `ContentsSystem`.
- [x] Criar `Network/NetworkService` placeholder.
- [x] Criar `Engine/CEngine` placeholder.
- [x] Criar placeholders Gamebryo: `NiRenderer`, `NiFile`, `NiTexture`, `NiSourceTexture`, `NiMaterial`, `NiDX9Renderer`.
- [x] Criar `Runtime/MainThreadDispatcher`.
- [x] Criar `Runtime/JobSystem`.
- [x] Criar `Runtime/RenderCommandQueue`.
- [x] Rodar `NetworkService` em thread dedicada configuravel.
- [x] Portar `CFlowCmdQueue` para fila thread-safe.
- [x] Portar `CFlowStack`.
- [x] Centralizar endpoints, paths, file pack e file table em `appsettings.json`.
- [x] Criar `Data/Loading/FileTableLoadingService` em worker thread.

## Classes Principais A Mapear

### Entrada E App

- [x] `main.cpp` -> `Program.cs`
- [x] `App/CWin32App.*` -> `App/CWin32App.cs`
- [x] `App/CGameApp.*` -> `App/CGameApp.cs`
- [x] `Resist.*` -> `Resist/cResist.cs` base
- [x] `GlobalData.*` -> `GlobalData/GlobalDataStore.cs` base
- [x] `OptionMng.*` -> `Option/COptionMng.cs` base
- [x] `CrashLogger` -> `Diagnostics/CrashLogger.cs` base

### Flow

- [x] `Flow/CFlow`
- [x] `Flow/CFlowMgr`
- [x] `Flow/CFlowFactory`
- [x] `Flow/CFlowCmdQueue`
- [x] `Flow/CFlowStack`
- [x] `LogoFlow`
- [x] `LoginFlow`
- [x] `ServerSelectFlow`
- [x] `CharacterSelectFlow`
- [x] `LoadingFlow`
- [x] `InGameFlow`
- [x] `SecondPasswordFlow`
- [x] `CharacterCreateFlow`
- [x] `DigimonCreateFlow`
- [x] `DatsCenterFlow`
- [x] `BattleFlow`
- [x] `BattleRegisterFlow`
- [x] `BattleResultFlow`
- [x] `CharacterServerRelocateFlow`

### Contents/UI

- [x] `ContentsSystem`
- [x] `LogoContents`
- [x] `LoginContents`
- [x] `ServerSelectContents`
- [x] `CharacterSelectContents`
- [x] `LoadingContents`
- [x] `InGameContents`
- [x] `SecondPasswordContents`
- [x] `CharacterCreateContents`
- [x] `DigimonCreateContents`
- [x] `MainFrame_Contents`
- [x] `BattleSystemContent`
- [x] `InventoryContents`
- [x] `MapContents`
- [x] `ChatContents`
- [x] `WareHouseContents`
- [x] `MailContents`
- [x] `Party_Contents`
- [x] `TutorialContents`
- [x] `MacroProtectContents`

### Rede

- [x] `NetworkService` placeholder.
- [x] Thread dedicada de rede.
- [x] Dispatcher para devolver callbacks ao main thread.
- [x] `nlib/client.*` -> socket client real configuravel.
- [x] `nlib/packet.*` -> `cPacket` inicial com bounds checking.
- [x] `nlib/client.*` -> pipeline receive/decode/dispatch com ordenacao por conexao.
- [x] `cNetwork.*` -> orquestracao inicial dos sockets em `NetworkService`.
- [x] `cCliAccount*` -> login/account placeholder.
- [x] `cCliGate*` -> gate/character placeholder.
- [x] `cCliGame*` -> game/channel/map/combat placeholder.
- [x] `common_vs2019/nScope`, `pLogin`, `pSvr`, `pGate`, `pTamer`, `pPass2` -> ids iniciais em C#.
- [x] `cCliAccountReceive.*` -> handlers iniciais de login, cluster list, gate info, select cluster e second pass.
- [x] `cCliGateReceive.*` -> handlers iniciais de tamer list, select success/failure e change server.
- [x] `cCliAccountSend.*` / `cCliGateSend.*` -> sends iniciais com packet ids oficiais e payload base.
- [x] `common_vs2019/pGame`, `pGameTwo`, `pSync` -> ids iniciais em C#.
- [x] `cCliGameSend.*` -> `SendAccessCode`, `SendReady`, `SendChannelInfo`, `SendChangeServer`, `SendSelectPortal` base.
- [x] `cCliGameReceive.*` -> handlers iniciais de `InitData`, `ChannelInfo`, `StartMessage`, portal/change server e version error.
- [x] `cCliGameSync.*` -> leitura inicial resumida de `pGame::Sync` preservando dispatch ordenado.
- [x] `pSync::Walk/Move/Rotation/Out/Delete` -> DTOs de movement sync e snapshot de objetos no main thread.
- [x] `pSync::New/In` -> snapshots de `Tamer`, `Digimon`, `Item`, `CommissionShop` e nucleo de `Monster` sem acoplar render Gamebryo.
- [x] `pSync::Condition`, mensagens e `BuffSync` -> DTOs e aplicacao no estado global/main thread.
- [x] Protocolos `common_vs2019/Protocol/*`.
- [x] Validar endian, tipos `u1/u2/u4/u8`, handlers e packet ids.

Nota: o socket real ja esta implementado no C#, mas `Network.UseRealSockets` fica `false` por padrao enquanto os packet ids e handlers oficiais ainda nao forem portados. Isso mantem o smoke test local estavel e deixa a troca para rede real controlada por configuracao.

### Render/Gamebryo

- [x] `CEngine` placeholder.
- [x] Fila de comandos de render para sincronizar trabalho vindo de outros threads.
- [x] `NiRenderer` placeholder.
- [x] `NiFile` placeholder.
- [x] `NiTexture`, `NiSourceTexture`, `NiMaterial` placeholders.
- [x] `NiDX9Renderer` placeholder.
- [x] `NiNode`, `NiObject`, `NiAVObject` placeholders estruturais com nome, ref-count, transform e filhos.
- [x] `NiCamera` placeholder com frustum e camera principal no `CEngine`.
- [x] `NiLight` placeholder com cor/dimmer.
- [x] `NiTexture` placeholder instanciavel com `FormatPrefs` e flags estaticas.
- [x] `NiSourceTexture` placeholder instanciavel com `Create`, preload/mipmap e destroy app data.
- [x] `NiTexturingProperty` placeholder com base texture.
- [x] `NiAnimation`/`NiTimeController` placeholders com start/stop/update.
- [x] `NiMilesAudio` placeholder via `NiMilesAudioSystem`, `NiMilesAudioSDM` e `CSoundMgr`.
- [x] Shader/material pipeline placeholder com `NiShader`, `NiMaterialProperty` e cache `NiShaderMaterialPipeline`.
- [x] Device lost/reset pipeline placeholder em `NiRenderer`/`CEngine`.

### Dados E Assets

- [x] `JobSystem` base para loading/descompressao/file tables.
- [x] `CsFilePackSystem` placeholder.
- [x] `FileTableMng` placeholder.
- [x] BIN loader abstraction/base.
- [x] Safe binary reader com bounds checking.
- [x] BIN section snapshot/count inspector inicial.
- [x] Pack `.pf/.hf` metadata reader para hash normal `0x10`.
- [x] Parser real inicial `TamerList.bin` (`CsTamerMng`).
- [x] `CsTamerMng` base com consultas de tamers, mapa de emotes e busca por comando de emotion.
- [x] Parser real inicial `Skill.bin` (`CsSkillMng`).
- [x] Parser real inicial `Digimon_List.bin` e `Ride.bin` (`CsDigimonMng`).
- [x] Parser real inicial `New_Element.bin` para atributos/naturezas de `CsDigimonMng`.
- [x] Parser real inicial `ItemList.bin` para items/categorias/cooldowns/tipos/ranks (`CsItemMng`).
- [x] Parser real inicial `Monster.bin` para monstros/hit/skills/skill terms (`CsMonsterMng`).
- [x] Parser real inicial `Nature.bin` para natureza/base elements e comparacao de vantagem (`CsNatureMng`).
- [x] Parser real inicial `Npc.bin` para NPCs/tipos/portais/extras/model offsets/card game (`CsNpcMng`).
- [x] Parser real inicial `Cuid.bin` para filtro de ID/chat (`CsCuidMng`).
- [x] Parser real inicial `Buff.bin` para buffs/status e efeitos de tabela (`CsBuffMng`).
- [x] Parser real inicial `WorldMap.bin` para mundos/areas/listas de mapa global (`CsWorldMapMng`).
- [x] `CsMapMng` composto para submanagers de mapa.
- [x] Parser real inicial `MapList.bin` para lista/path/configuracao de mapas (`CsMapListMng`).
- [x] Parser real inicial `MapPortal.bin` para portais por mapa (`CsMapPortalMng`).
- [x] Parser real inicial `MapNpc.bin` para NPCs posicionados por mapa (`CsMapNpcMng`).
- [x] Parser real inicial `MapMonsterList.bin` para spawns de monstros por mapa (`CsMapMonsterMng`).
- [x] Parser real inicial `MapRegion.bin` para regioes/limit evo/buff regions (`CsMapRegionMng`).
- [x] `CsMapRegionMng` base com consultas por mapa.
- [x] Parser real inicial `MapCharLight.bin` para luz direcional/ponto por mapa (`CsMapCharLightMng`).
- [x] `CsMapCharLightMng` base com consultas por mapa.
- [x] Parser real inicial `Weather.bin` para clima/elementos por mapa (`CsWeatherMng`).
- [x] `CsWeatherMng` base com consultas por mapa.
- [x] Parser real inicial `MapObject.bin` para objetos ativos/fatores por mapa (`CsMapObjectMng`).
- [x] `CsMapObjectMng` base com consultas por mapa/objeto/fator.
- [x] Parser real inicial `TableObj.bin`/`TableObj2.bin` para blobs de props por mapa/node (`CsMapTableObjMng`).
- [x] `CsMapTableObjMng` base com consultas por mapa/node.
- [x] Parser real inicial `Quest.bin` para quests/textos/requisitos/recompensas/eventos (`CsQuestMng`).
- [x] Parser real inicial `DMBase.bin` para bases de tamer/digimon/mapa/guild/limites/loja pessoal/evolucao (`CsBaseMng`).
- [x] `CsBaseMng` base com consultas principais e normalizacao legada de JumpBuster.
- [x] Parser real inicial `Talk.bin`/`Tutorial.bin` para falas de digimon, eventos, mensagens, tips/loading tips e tutorial (`CsTalkMng`).
- [x] `CsTalkMng` base com consultas principais por subtabela.
- [x] Parser real inicial `Scene.bin` para event scenes, passos de cena, targets, falas, efeitos e sons (`CsSceneDataMng`).
- [x] `CsSceneDataMng` base com consultas por scene id e target uid.
- [x] Parser real inicial `Data\\MO.bin` para rotas/acoes de move objects (`CsMoveObjectMng`).
- [x] `CsMoveObjectMng` base com consultas legadas e interpolacao inicial de posicao.
- [x] Parser real inicial `AddExp.bin` para efeitos de EXP por skill (`CsAddExpMng`).
- [x] `CsAddExpMng` base com consultas por ID e por skill.
- [x] Parser real inicial `DigimonEvo.bin` para arvores de evolucao, jogress/capsula e battle evolution (`CDigimonEvolution`).
- [x] `CDigimonEvolution` base com consultas por digimon, objeto evoluido e battle evo.
- [x] Parser real inicial `Tactics.bin` para digitama/tactics, explain, enchant, transcend e exp digimon (`CsTacticsMng`).
- [x] `CsTacticsMng` base com consultas de hatch, explain, enchant e transcend.
- [x] Parser real inicial `Achieve.bin` para arvore de tipos, titulos, pontos, icones e buffs (`CsAchieveMng`).
- [x] `CsAchieveMng` base com consultas por quest/title id.
- [x] Parser real inicial `CashShop.bin` para tabelas default/Steam, categorias, grupos, produtos, pacotes e web links (`CsCashShopMng`).
- [x] `CsCashShopMng` base com consultas por tabela/categoria/produto.
- [x] Parser real inicial `Gotcha.bin` para maquinas, grupos de item, rares e mystery coin/item (`CsGotchaMng`).
- [x] `CsGotchaMng` base com consultas por NPC, item, rare e mystery machine.
- [x] BIN loaders reais por tabela.
- [x] Pack `.pf/.hf` completo: hash protegido/decrypt/decompress.
- [x] `DataMng`.
- [x] `ResourceMng` base com cache de texturas, cleanup e API `GetTexture`/`RemoveTexture`.
- [x] `TextureMng`/`GUI.Texture` placeholder com hash estavel, nome de arquivo e ref-count manual.
- [x] `IconMng` base com `cSprite`, `cIcon`, `sIFIcon`, atlas iniciais, dynamic icons e cache via `ResourceMng`.
- [x] `CharResMng` base com fila de delete, target mark, move point, char image cache, NPC marks, monster create scene e figures.
- [x] `MapObjectMng` base com parser e consultas principais.
- [x] `QuestMng` base com parser e consultas principais.
- [x] `ItemMng` base com parser e consultas principais.
- [x] `CsDigimonMng` base com parser e consultas principais.
- [x] `MonsterMng` base com parser e consultas principais.
- [x] `NatureMng` base com parser e consultas principais.
- [x] `SkillMng` base com parser e consultas principais.
- [x] `NpcMng` base com parser e consultas principais.
- [x] `CuidMng` base com filtro de ID/chat.
- [x] `BuffMng` base com parser e consultas principais.
- [x] `WorldMapMng` base com parser, areas por mundo e listas real/digital.

### Jogo

- [x] `Tamer` runtime base (`CTamer`) com stats, condition, equip parts, guild/achievement e link para digimon.
- [x] `TamerUser` runtime base (`CTamerUser`) com estado local, server option e criação pelo `pGame.InitGameData`.
- [x] `Digimon` runtime base (`CDigimon`) com stats, condition, escala, enchant e link para tamer.
- [x] `DigimonUser` runtime base (`CDigimonUser`) com slots locais, estado de follow/attack/skill e ponteiro em `CCharMng`.
- [x] `Monster` runtime base (`CMonster`) com stats, condition, casting e seed effects.
- [x] `CCharMng` runtime registry para `Tamer`, `Digimon`, `Monster`, field item e commission shop.
- [x] Field item/commission shop runtime base (`CFieldItem`, `CEmployment`).
- [x] `Npc` runtime base (`CNpc`) com `NpcBinRecord`/`MapNpcRecord`, model id, display flag, quest view e enable/use placeholders.
- [x] `EffectMng` base com lista de efeitos, render commands placeholder e marbles de evolucao (`CREATE/FALLOW/DIE`).
- [x] `Pat_AI_Mng` base com containers, elementos `Time/Ani/Move`, links ponderados e selecao `GetNext`.
- [x] `ObjectController` base para elegibilidade/ordem de shadow render usando `CCharMng` e `RenderCommandQueue`.
- [x] `Data_ServerSync` base (`cData_ServerSync`) com enum, ref-count, comandos pendentes, `IsChageDigimon`, ride/skill/change flags.
- [ ] Map transition/loading completo de terrain/assets/render real.
  Nota: a estrutura C# do pipeline ja planeja mapa em worker thread, carrega assets via drive/file pack e publica renderables/render commands no main thread. O item fica aberto ate conectar terrain/assets em objetos Gamebryo reais.
- [x] Combat state runtime base com `EnterCombat`/`LeaveCombat` e derivados de `ServerSync` para troca de digimon, skill e ride.
- [x] `GameStateSync` base para aplicar map transition/combat state no main thread.
- [x] `RuntimeMetrics` base para frame time, network queue, job queue e render command queue.
- [x] `cCliGame` inicial aplica character-select/portal/init-data em `GameStateSync`.
- [x] `pGame.InitGameData` cria `CTamerUser` local no `CCharMng` e espelha em `WorldObjects`.
- [x] Map transition carrega NPCs do mapa via `CsMapMng`/`CsNpcMng` para `CCharMng`.
- [x] Movement sync base para `Walk`, `Move`, `Rotation`, `Out` e `Delete`.
- [x] Movement sync `New/In` para snapshots de `Tamer`, `Digimon`, `Item` e `CommissionShop`.
- [x] Movement sync `New/In` com nucleo de `Monster` (`DstPos`, HP, level, condition, casting skill e skill growth).
- [x] Movement sync `Monster` completo para payload de seed/area effect usando `CsMonsterMng` para identificar `ATTACH_SEED`.
- [x] Sync runtime para `Condition`, chat/shout/system messages e buffs de tamer/digimon/monster.

## Appsettings Inicial

Valores importados do ini fornecido:

```ini
[DEBUG]
AccountIP=104.234.41.94
FilePack=1
PgSqlAccountPort=17029
PgSqlCharacterPort=17050
PgSqlGamePort=17608
Server=pgsql

[login]
RememberPassword=1
id=leleos
pw=Essa0521@

[network]
id=admin
ip=104.234.41.94
port=17029
pw=admin
```

No C# ficam em:

- `Debug.AccountIP`
- `Debug.FilePack`
- `Debug.PgSqlAccountPort`
- `Debug.PgSqlCharacterPort`
- `Debug.PgSqlGamePort`
- `Debug.Server`
- `Login.RememberPassword`
- `Login.Id`
- `Login.Password`
- `Network.Id`
- `Network.Ip`
- `Network.Port`
- `Network.Password`
- `Network.NetVersion`
- `Network.UserType`
- `Network.UseRealSockets`
- `Network.DecodePacketsOnWorker`
- `Network.MaxDispatchPerPump`
- `Network.TamerEquipBytes`
- `ServerEndpoints.Account.Host`
- `ServerEndpoints.Account.Port`
- `ServerEndpoints.Character.Host`
- `ServerEndpoints.Character.Port`
- `ServerEndpoints.Game.Host`
- `ServerEndpoints.Game.Port`
- `Paths.DataRoot`
- `Paths.BinRoot`
- `Paths.PackRoot`
- `Paths.UserDataRoot`
- `Paths.LogRoot`
- `Paths.SoundRoot`
- `FilePack.UsePack`
- `FilePack.PackNames`
- `FilePack.WriteMode`
- `FilePack.AllowMissingPackFiles`
- `FileTable.Source`
- `FileTable.Language`
- `FileTable.AllowMissingTables`
- `FileTable.ReloadQuestWhenBinMode`
- `FileTable.Features.*`

## Ordem Recomendada De Port

1. Consolidar bootstrap e flow basico.
2. Portar pacote e sockets com logs fortes.
3. Portar account/gate ate login -> server select -> character select.
4. Portar loading com leitura minima de file tables.
5. Portar entidades de mapa sem render real.
6. Substituir placeholders Gamebryo por libs portadas.
7. Ligar render, camera, resource manager e UI.
8. Smoke test real com servidor.

## Modelo De Threads

- Main thread: `CGameApp`, `CFlowMgr`, UI state, render calls e objetos Gamebryo.
- Network thread: socket IO, decode inicial de pacotes e enqueue de eventos para `MainThreadDispatcher`.
- Job workers: file tables, pack reads, decompression, hash, preparacao de recursos e parse pesado.
- Render command queue: ponte para comandos de render criados por jobs, executados no main/render thread.

Regra importante: objetos Gamebryo oficiais nao devem ser mutados diretamente por worker threads ate provarmos que a lib port trabalha com isso. Worker prepara dados puros; main thread consome e cria/atualiza objetos renderizaveis.
