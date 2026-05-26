# Handoff: Bug de Status/Tooltip com AT Negativo (-16000)

## Referencias dos arquivos envolvidos (antes de tudo)

### Server
- [UpdateStatusPacket.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/GameServer/UpdateStatusPacket.cs)
- [DigimonModelBehavior.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Digimon/DigimonModelBehavior.cs)

### Cliente
- [cCliGameReceive.cpp](/C:/0-NewDmo/dmo-client-main/DProject/network/cCliGameReceive.cpp)
- [ToolTip_Stat.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/ToolTip_Stat.cpp)

### UI (contexto de exibicao de status)
- [TamerStatus_Equip.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/TamerStatus_Equip.cpp)
- [TamerStatusUI_Tamer.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/14.MainFrameUI/TamerStatusUI_Tamer.cpp)
- [MainFrame_DigimonStatusUI.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/14.MainFrameUI/MainFrame_DigimonStatusUI.cpp)

## Resumo da correcao ja aplicada

1) UpdateStatusPacket (Server)
- AT do parceiro/digimon foi enviado como int32 (WriteInt) para evitar truncamento.
- Mantida a escrita de DetailInfo com int32.
- Acrescentado log de layout de AT e breakdown para rastreamento.

2) cCliGameReceive.cpp (Cliente)
- RecvAllStat trata AT do parceiro com parser adaptado:
  - layout novo: int32
  - fallback legado: u2 (com aviso)
- Trata o bloco de DetailInfo de 13 inteiros quando houver bytes suficientes.

3) ToolTip_Stat.cpp (Cliente)
- Tooltip de status do tamer e do digimon foram revisados para reduzir divergencias de detalhe no parser/layout.
- Ajuste inicial aplicado de exibicao no fluxo do Digimon para impedir valor final negativo.

4) DigimonModelBehavior.cs (Server)
- Ajustes de normalizacao em AT e detalhamento para evitar negativos na origem dos dados de status.

## Hipotese de causa raiz

O valor -16000 sem equipamento tende a vir de desalinhamento entre:
- estrutura do pacote enviado pelo servidor
- ordem/espera do parser de cliente
- montagem de subtotais/bonus no tooltip

Ou seja, nao parece mais um "bonus base faltando item", e sim uma divergencia de layout ou indice de status na pipeline de status.

## Continuar corrigindo especificamente status do Digimon (nao do Tamer)

1) Validar pipeline completa do Digimon
- Conferir no vivo:
  - valor AT enviado no pacote para status do Digimon
  - valor AT lido no cliente (parser extended)
  - valor final exibido no tooltip do Digimon
- Ponto de atalho: se um dos 3 passos variar, origem esta no parsing/ordem e nao no valor base.

2) Verificar ordem/quantidade dos campos de DetailInfo
- Confirmar se 13 valores de detalhe ainda correspondem aos mesmos indices usados no tooltip atual.
- Se layout estiver diferente em qualquer fluxo (login, update parcial, revive, partner swap), padronizar com contrato unico.

3) Sanidade para exibicao
- Antes de renderizar:
  - clamp de base/total do Digimon para nao ficar abaixo de 0
  - se discrepancia entre cliente e server persistir, log de alerta + exibicao com fonte de ajuste ja aplicado.

4) Ajuste de prioridade
- Se ainda houver divergencia, priorizar:
  - unificar parsing em RecvAllStat
  - revisar chamadas de SetTooltip em `MainFrame_DigimonStatusUI.cpp` e `ToolTip_Stat.cpp`
- Deixar o parser de status como fonte unica do valor bruto; tooltip deve apenas decompor/formatar.

## Checklist de validacao do proximo agente
- Rodar teste de login sem equipamento e trocar de parceiro/digimon.
- Conferir logs: se parser caiu em layout novo ou legacy.
- Garantir que nD_AP no cliente nunca vira negativo sem motivo de gameplay.
- Comparar AT inicial/server (status pacote) x tooltip final sem equipamentos no Digimon.

### Ajuste continuo aplicado para Digimon (nao do Tamer)

- Foi aplicado ajuste em [ToolTip_Stat.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/ToolTip_Stat.cpp):
  - Removido o bloco de ajuste `server applied` especÃ­fico do calculo do tooltip do Digimon.
  - Incluido clamp de exibiÃ§Ã£o para evitar valor final negativo: `nTotalResult < 0 => 0`.
- Isso reduz o sintoma visÃ­vel de -16000 no tooltip enquanto a validaÃ§Ã£o de contrato de pacote ainda Ã© finalizada.
## Atualização com o log mais recente (26/05/2026 13:34:59)

### Evidências novas do cliente (log bruto)
- Linha de layout recebida:
  - `ALLSTAT layout bytes=98 tamerBaseBytes=44 detailTailExpected=52 layout legacy=1 extended=0 intDefense=0 intHitRate=0`
- Detecção atual:
  - `ALLSTAT legacy partner AT layout detected bytes=98 AT=11924; server is still sending partner AT as u2`
  - `ALLSTAT legacy partner HT layout detected bytes=98 HT=80; server is still sending partner HT as u2`
- Core parse:
  - `ALLSTAT parsed core TAMER uid=32769 HP=1580 DS=320 AP=4391125 DP=1323237926 MAXHP=1580 MAXDS=1570 MS=0; DIGIMON uid=16385 HP=20674 DS=1400111119 MAXHP=20674 MAXDS=20191 FS=0 AP=11924 DP=32767 HT=80 AR=276 BL=76 CC=780 AS=30681 EV=19570`
- Detail tail:
  - `ALLSTAT DetailInfo recv bytes=52 HP=1300889600 DS=1376059392 AT=14417920 AS=1179648000 CT=1250426880 HT=0 SCD=0 CD=0 SD=0 BASE=778567680 DE=671744000 BL=832307200 EV=447610880`
- Aplicado:
  - `ALLSTAT applied DIGIMON uid=16385 AT=11924 DP=32767 HT=80 DS=1400111119 HP=20674 DECOMP=14417920 detailTailAvailable=52`

### Diagnóstico atual
- Continua inconsistente com pacote de servidor para o mesmo personagem (esperado ~2200 para AT em cenários de análise anterior), reforçando problema de layout de parse no cliente ao entrar no fluxo de digimon.
- AS e EV no `DetailInfo recv` continuam com magnitudes fora da faixa real esperada, o que confirma deslocamento de bytes no parsing atual.
- A ação de continuidade permanece: ajustar parse/padrão de Detalhe do Digimon em `RecvAllStat` e validar no mesmo frame com log servidor+cliente.

### Server + Cliente (refs antes do próximo ajuste)
- [UpdateStatusPacket.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/GameServer/UpdateStatusPacket.cs)
- [DigimonModelBehavior.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Models/Digimon/DigimonModelBehavior.cs)
- [cCliGameReceive.cpp](/C:/0-NewDmo/dmo-client-main/DProject/network/cCliGameReceive.cpp)
- [ToolTip_Stat.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/ToolTip_Stat.cpp)
- [CrashLogger.h](/C:/0-NewDmo/dmo-client-main/LibProj/CsFunc/CrashLogger.h)
- [CrashLogger.cpp](/C:/0-NewDmo/dmo-client-main/LibProj/CsFunc/CrashLogger.cpp)
- [Program.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/Program.cs)
- [TamerStatus_Equip.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/TamerStatus_Equip.cpp)
- [TamerStatusUI_Tamer.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/14.MainFrameUI/TamerStatusUI_Tamer.cpp)
- [MainFrame_DigimonStatusUI.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/14.MainFrameUI/MainFrame_DigimonStatusUI.cpp)

### Estado de VPS / Docker Compose (pronto para continuidade)
- VPS: `ubuntu@104.234.41.94`
- Diretório remoto: `/home/ubuntu/dmo-server-main-postgres`
- Env: `.env.production`
- Comandos operacionais:
  - `cd /home/ubuntu/dmo-server-main-postgres`
  - `sudo docker compose --env-file .env.production ps`
  - `sudo docker compose --env-file .env.production logs --tail=200 account character game`
  - `sudo docker compose --env-file .env.production up -d --build account character game`
  - `sudo docker compose --env-file .env.production restart account character game`

### Continuidade recomendada
- Confirmar no próximo ciclo se o valor correto de status esperado para esse Digimon é `AT=2200` (ou outro valor de base para essa tamer/digimon) e comparar com `server` + `ALLSTAT parsed core`.
- Se houver divergência persistente, priorizar parse de Digimon (com dual preview u2 vs int32 por offset) antes de qualquer ajuste de tooltip para evitar mascarar raiz.

### Deploy para VPS
- Nesta etapa, os logs foram alinhados e documentados; não houve nova alteração de código aguardando deploy adicional para a VPS.

## Estado atual - resolvido em 2026-05-26

- O bug original de AT negativo/desalinhado nao esta mais ativo no fluxo principal de status.
- O servidor envia status de parceiro com campos grandes em `int32` e AS como `float`.
- O cliente foi ajustado para ler o layout correto em `RecvAllStat`.
- A divergencia que sobrou no ultimo teste era do tooltip, nao do calculo real de status.
- `ToolTip_Stat.cpp` agora usa o valor autoritativo recebido do servidor como reconciliacao: se o subtotal local nao alcancar o status real, ele mostra a diferenca antes do `ALL`.
- Com isso, o tooltip deixa de parecer que equipamento/roupa nao aplicou quando, na verdade, o painel principal ja recebeu o valor correto.

## Equipamentos, roupas e totais acima de short

- No servidor, `AccessoryStatus`, `ChipsetStatus` e `DigiviceAccessoryStatus` em `CharacterModelBehavior.cs` agora retornam `int` e nao fazem clamp em `short.MaxValue`.
- Esse ajuste permite que somas de equipamentos, acessorios, chipsets e digivice ultrapassem `32767` sem truncar o total aplicado.
- Backup do deploy na VPS: `/home/ubuntu/dmo-server-main-postgres/deploy-backups/20260526-150605-equipment-status-int-totals`.
- VPS validada apos rebuild do `game` com log `GameServer started.`.
