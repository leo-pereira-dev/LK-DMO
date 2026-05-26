# Handoff: Correção de AS de Digimon (layout int32 DE/HT)

## Referências de arquivos (server + cliente)
- [UpdateStatusPacket.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/GameServer/UpdateStatusPacket.cs)
- [cCliGameReceive.cpp](/C:/0-NewDmo/dmo-client-main/DProject/network/cCliGameReceive.cpp)
- [ToolTip_Stat.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/Game/ToolTip_Stat.cpp)
- [MainFrame_DigimonStatusUI.cpp](/C:/0-NewDmo/dmo-client-main/DProject/_Interface/14.MainFrameUI/MainFrame_DigimonStatusUI.cpp)

## O que foi identificado
- No `UpdateStatusPacket` de produção (log do VPS), o envio está assim: `partner AS=2200` e `partner EV=14930`.
- Na mesma máquina, o cliente pode exibir `AS` com valor absurdo (ex.: 14930 em vez do valor esperado), o que indica provável desalinhamento de parsing no layout do pacote no cliente.
- O ponto mais provável era em `RecvAllStat` (`cCliGameReceive.cpp`), onde a detecção do layout não considerava o caso **AT/DE/HT = int32** simultaneamente com/sem bloco detail.

## Correção aplicada neste handoff
- Em [cCliGameReceive.cpp](C:/0-NewDmo/dmo-client-main/DProject/network/cCliGameReceive.cpp), ajuste de heurística em `RecvAllStat`:
  - O parser agora reconhece a combinação de ataque, defesa e hit rate em `int32` simultaneamente (com e sem `DetailInfo`), além dos cenários legado.
  - Isso corrige o deslocamento de bytes que fazia os campos seguintes, inclusive AS/EV, serem lidos trocados.
- Mantidos os logs detalhados em `ALLSTAT layout ...` e `ALLSTAT parsed ...` para rastrear `AT/DS/HP/DE/AS/EV/HT` em tempo real.

## Logs recomendados para validar
- **VPS/Game**: procurar
  - `UpdateStatusPacket:Main` (server enviou valores de AS/EV/HT esperados)
  - `ALLSTAT layout` (cliente detectou layout)
  - `ALLSTAT parsed core` (cliente aplicou AT/DE/AS/EV/HT)
- Conferir se no cliente o campo `AS` passou a bater com `UpdateStatusPacket:Main` para o mesmo `partner`/UID.

## Formato de logs úteis para validação (copiar/colar em grep)

### Server (VPS)
- `[UpdateStatusPacket:Main] tamer=8 partner=32 partnerName=Noah ... partnerAT=... partnerATLayout=int32`
- `partnerAS=...`
- `partnerEV=...`
- `partnerHT=...`
- `bytesBeforeSerialize=...`

### Cliente (log local)
- `ALLSTAT layout bytes=%u tamerBaseBytes=%u detailTailExpected=%u layout legacy=%d extended=%d intDefense=%d intHitRate=%d`
- `ALLSTAT parsed core TAMER uid=%u HP=%u DS=%u AP=%d DP=%d MAXHP=%u MAXDS=%u MS=%u; DIGIMON uid=%u HP=%u DS=%u MAXHP=%u MAXDS=%u FS=%u AP=%d DP=%d HT=%u AR=%u BL=%u CC=%u AS=%u EV=%u`
- `ALLSTAT DetailInfo recv bytes=%u HP=%d DS=%d AT=%d AS=%d CT=%d HT=%d SCD=%d CD=%d SD=%d BASE=%d DE=%d BL=%d EV=%d`
- `ALLSTAT applied DIGIMON uid=%u AT=%d DP=%d HT=%d DS=%d HP=%d DECOMP=%d detailTailAvailable=%u`

### Checklist de conferência por frame
1. Compare `tamer.uid`/`partner.uid` entre server e cliente.
2. Compare `partnerAS` do server com `AS=` do `ALLSTAT parsed core`.
3. Compare `partnerEV` do server com `EV=` do `ALLSTAT parsed core`.
4. Compare flags do layout do cliente (`extended/intDefense/intHitRate`) com `partnerATLayout=int32` do server.

## Próximo passo (se ainda divergente no cliente)
- Se o valor do Digimon continuar errado após esta correção, verificar `LoadTamerPacket`/outro parser de status para um possível segundo parse path do status de `partner`.
- Registrar no log um snapshot com: `nPartnerStatAvailableBytes`, flags de layout e valores crus de `partner AT/DE/AS/EV/HT` no mesmo frame.

## Comandos de coleta (pronto para copiar)

### VPS (produção)
```bash
cd /home/ubuntu/dmo-server-main-postgres
sudo docker compose --env-file .env.production logs game --tail=400 | rg "UpdateStatusPacket:Main|UpdateStatusPacket:DetailInfo|partnerAS=|partnerEV=|partnerHT=|partnerAT=|partnerDS=|partnerHP="
```

### Cliente (processo local)
```bash
rg "ALLSTAT layout|ALLSTAT parsed core|ALLSTAT DetailInfo recv|ALLSTAT applied DIGIMON|AS=|EV=|AT=|DE=|HT=|DS=|HP=" "Client-*.log"
```

### O que conferir por linha (exemplos)
- Server: `... partnerAT=1200 partnerAS=2200 partnerDE=500 partnerEV=14930 partnerHT=19625 partnerDS=800 partnerHP=1234 ...`
- Cliente: `... DIGIMON uid=... AT=1200 DE=500 AS=2200 EV=14930 HT=19625 DS=800 HP=1234 ...`

### Conferência obrigatória de campos por frame
1. `partnerAT == AT`  (se for Digimon parceiro)
2. `partnerDE == DE`
3. `partnerAS == AS`
4. `partnerEV == EV`
5. `partnerHT == HT`
6. `partnerDS == DS`
7. `partnerHP == HP`

## Atualizacao da etapa: log dedicado de stats (stats.log)

- Logs dedicados em cliente:
  - Implementado em [CrashLogger](/C:/0-NewDmo/dmo-client-main/LibProj/CsFunc/CrashLogger.cpp): `nsCSDEBUG::CrashLogger::LogStatsMessage(...)`.
  - `RecvAllStat` chama `ALLSTAT_LOG(...)` para gravar `ALLSTAT` tanto no `crash.log` quanto no `logs\\stats.log`.
  - Arquivos de saÃ­da esperados:
    - `ClientDist\logs\stats.log` (build/distribuiÃ§Ã£o do cliente)
    - `dmo-client-main\DProject\Release_English\logs\stats.log` (build local da soluÃ§Ã£o)
- Comando para inspeÃ§Ã£o direta do arquivo dedicado:
  - `rg "ALLSTAT" C:/0-NewDmo/ClientDist/logs/stats.log`
  - `rg "ALLSTAT" C:/0-NewDmo/dmo-client-main/DProject/Release_English/logs/stats.log`
- Arquivos no handoff jÃ¡ cobrindo status server+cliente:
  - [UpdateStatusPacket.cs](/C:/0-NewDmo/dmo-server-main-postgres/src/Source/Domain/DigitalWorldOnline.Commons/Packets/GameServer/UpdateStatusPacket.cs)
  - [cCliGameReceive.cpp](/C:/0-NewDmo/dmo-client-main/DProject/network/cCliGameReceive.cpp)
  - [CrashLogger.h](/C:/0-NewDmo/dmo-client-main/LibProj/CsFunc/CrashLogger.h)
  - [CrashLogger.cpp](/C:/0-NewDmo/dmo-client-main/LibProj/CsFunc/CrashLogger.cpp)

## Deploy na VPS (docker compose)

- Ambiente produÃ§Ã£o (prioridade operacional):
  - VPS: `ubuntu@104.234.41.94`
  - Diretorio remoto: `/home/ubuntu/dmo-server-main-postgres`
  - Env file: `.env.production`
- Comandos Ãºteis de operaÃ§Ã£o:
  - `cd /home/ubuntu/dmo-server-main-postgres`
  - `sudo docker compose --env-file .env.production ps`
  - `sudo docker compose --env-file .env.production logs --tail=200 account character game`
  - `sudo docker compose --env-file .env.production up -d --build account character game`
  - `sudo docker compose --env-file .env.production restart account character game`

## Estado desta correÃ§Ã£o
- Alteracao implementada APENAS no cliente para logging de `ALLSTAT` em `logs\\stats.log`.
- `CrashLogger` e `RecvAllStat` foram ajustados para gerar o canal `stats.log`.
- Ainda nao foi feito upload/build no servidor VPS nesta etapa.
- Proximo passo recomendado: compilar cliente e validar localmente; somente entao empacotar/transferir o exe e/ou pacotes conforme seu processo de release.

## Estado atual - resolvido em 2026-05-26

- A parte principal de status foi validada pelo usuario como correta.
- O contrato atual do `UpdateStatusPacket` para parceiro/Digimon e:
  - HP/DS/current HP/current DS em `int32`.
  - AT em `int32`.
  - DE em `int32`.
  - AS em `float` no pacote, normalizado pelo cliente para exibicao.
  - HT em `int32`.
  - `DetailInfo` continua com 13 valores `int32`.
- O cliente em `RecvAllStat` reconhece o layout estendido com `AT int32`, `DE int32`, `AS float` e `HT int32`, mantendo fallback para layouts antigos quando necessario.
- Os logs `ALLSTAT layout`, `ALLSTAT parsed core`, `ALLSTAT DetailInfo recv` e `ALLSTAT applied DIGIMON` continuam uteis, mas nao representam mais um bloqueio aberto para esta correcao.
- O executavel foi recompilado em `Release_English|Win32` e copiado para `ClientDist`, `ClientDist1` e `D:\OpenDmo\src\Source\Cliente-OficialV2\Release`.

## Observacao importante sobre tooltip

- A aplicacao de status pelo servidor estava correta; a incoerencia restante era visual no tooltip.
- O tooltip do Digimon agora compara o subtotal decomposto pelo cliente com o valor autoritativo recebido do servidor e adiciona a diferenca antes do `ALL` quando alguma fonte nova nao e decomposta localmente.
- Arquivo do ajuste visual: `C:\0-NewDmo\dmo-client-main\DProject\_Interface\Game\ToolTip_Stat.cpp`.
