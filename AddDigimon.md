# Manual workflow: adicionar novos Digimon e evoluções

Este guia descreve o caminho seguro para adicionar Digimon/evoluções novas dos
bins atualizados do global/GDMO dentro do template antigo do LK-DMO.

A regra principal: não substituir todos os bins novos direto no cliente e no
servidor. Converter por família de dados, validar dependências e testar cada
rota. O cliente antigo tem estruturas, enums, limits e asserts que não aceitam
automaticamente tudo que existe nos bins modernos.

## 1. Locais importantes

Workspace:

```text
C:\0-NewDmo
```

Cliente fonte:

```text
C:\0-NewDmo\dmo-client-main
```

Servidor ativo PostgreSQL:

```text
C:\0-NewDmo\dmo-server-main-postgres
```

Bins atuais do servidor:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
```

Bins/XML atualizados usados como fonte:

```text
C:\0-NewDmo\Dcommerce\Bin
C:\0-NewDmo\Dcommerce\xml\AllBins\Data
```

Pack03/bin extraido usado durante os testes:

```text
C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english
```

Pack01 descompactado com modelos/assets que faltavam:

```text
D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11
```

Executavel cliente gerado:

```text
C:\Release\GSP\DProject_R_ENG.exe
```

Copias de teste do cliente:

```text
C:\0-NewDmo\ClientDist\DProject_R_ENG.exe
D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe
```

## 2. Antes de mexer

Faça uma copia dos bins atuais que serão tocados. No minimo:

```text
Digimon_List.bin
DigimonEvo.bin
Skill.bin
Tactics.bin
Monster.dat / Monster.bin se entrar na rota
Model.dat se a investigação realmente exigir
MasterCard.bin se for selo/card relacionado
Seal*.bin se for Seal Master
```

Crie uma pasta de mapeamento para a importação especifica:

```text
C:\0-NewDmo\DigimonImportMaps\<NomeOuData>
```

Estrutura sugerida:

```text
DigimonImportMaps\<NomeOuData>\
  README.md
  source\
    new_xml\
    new_bin\
  current\
    server_bin\
    client_pack03_bin\
  converted\
    server_bin\
    client_pack03_bin\
  reports\
    changed_rows.csv
    missing_models.csv
    missing_tactics.csv
    missing_skills.csv
    skipped_new_format_fields.csv
    test_crashes.md
```

O README da importação deve registrar:

- quais Digimon IDs serão adicionados;
- quais linhas evolutivas serão alteradas;
- origem exata dos dados;
- bins finais modificados;
- assets copiados;
- crashes encontrados e resolução.

## 3. Identificar a linha que será adicionada

Comece pelo XML atualizado, pois ele é mais facil de comparar que bin bruto.
Procure por:

```text
Digimon_List
DigimonEvo
SkillCodeDigimon
Tactics
Model
Buff_Data
```

Para cada Digimon novo, anote:

```text
DigimonID / Type
nome
rank/evolution type
base/starter relacionado
slot de evolução esperado
level requerido
item requerido, se houver
skills
model id
icon/card id
tactics explain id
familia/nature/attribute
skill name string id
buff/debuff ids e name string ids
base/return evolution icon esperado
```

Exemplo de crash que indica falta de Tactics:

```text
TacticsMng.cpp line 75
Condition: IsTacticsExplain(dwDigiMonID)
DigiMon ID = 33005
```

Exemplo de crash que indica falta de QuickEvol/evolution slot:

```text
QuickEvol.cpp line 360
ID 77009
index 17
```

Exemplo de crash que indica falta de model/asset:

```text
Model.dat - 77017 ...
```

## 4. Converter, não copiar direto

Os bins novos podem ter formato mais moderno que o cliente antigo. Um exemplo
real: `SkillCodeDigimon.xml` novo pode ter `ApplyCount=5`, mas o velho
`Skill.bin`/`CsSkill::sINFO` trabalha com 3 applies fixos.

Ao converter:

- preservar o layout antigo esperado pelo cliente;
- manter apenas campos que cabem no template antigo;
- registrar tudo que foi descartado em `skipped_new_format_fields.csv`;
- se houver mais de 3 applies, escolher conscientemente os 3 que o cliente e o
  servidor precisam para dano/efeito principal;
- nunca preencher valores aleatorios para "parar crash".

## 5. Bins principais e o que cada um destrava

### Digimon_List.bin

Responsavel por dados base do Digimon:

- ID/type;
- nome via language/table;
- rank;
- atributo/nature/family;
- stats base;
- modelo/icon referencias;
- flags gerais.

Validar no cliente:

- janela Digimon Information abre;
- Digimon aparece com nome correto;
- status nao fica zerado;
- modelo renderiza.

Validar no servidor:

- asset loader carrega a linha;
- parceiro nasce/carrega sem exception;
- status calculado nao quebra.

### DigimonEvo.bin

Responsavel pela arvore de evolução:

- base Digimon;
- evoluções ligadas;
- slots;
- level/item required;
- unlock state;
- special route type.

Sintomas de problema:

- evolução nao aparece;
- slot aparece com cadeado errado;
- `QuickEvol.cpp` crash;
- troca/evolve envia packet mas volta ao estado anterior.

Arquivos server/client a conferir:

```text
dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\DigimonEvoBinLoader.cs
dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\PartnerEvolutionPacketProcessor.cs
dmo-client-main\LibProj\CsFileTable\TacticsMng.cpp
dmo-client-main\DProject\_Interface\Game\QuickEvol.cpp
```

### Tactics.bin

Responsavel pelo texto/explicação/associação que a UI de Digimon usa.

Sintoma classico:

```text
TacticsMng.cpp
Condition: IsTacticsExplain(dwDigiMonID)
```

Cada Digimon importado que aparece na arvore precisa ter entrada compativel em
Tactics. Se o Digimon existe em `DigimonEvo.bin` mas nao em `Tactics.bin`, a UI
pode cair ao abrir a janela.

### Skill.bin / SkillCodeDigimon

Responsavel por:

- skills ativas/passivas;
- dano base;
- consumo HP/DS;
- cooldown;
- target/range;
- apply effects;
- method/formula ids.

Arquivos cliente importantes:

```text
dmo-client-main\DProject\_Interface\Game\ToolTip_Skill.cpp
dmo-client-main\DProject\FmCommon.cpp
dmo-client-main\DProject\Skill.cpp
dmo-client-main\DProject\TamerUser.cpp
```

Arquivos servidor importantes:

```text
dmo-server-main-postgres\src\Source\Application\DigitalWorldOnline.Application.GameAssets\Bins\SkillBinLoader.cs
dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\PartnerAttackPacketProcessor.cs
dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\PacketProcessors\PartnerSkillPacketProcessor.cs
```

Methods já vistos no `Skill.bin` atual e suportados no tooltip:

```text
1, 2, 10, 101, 102, 104, 105, 106, 107, 108, 200,
201, 202, 205, 206, 207, 208, 209, 301, 401, 402,
403, 501
```

Se aparecer outro method, nao remover o assert. Adicionar o method com texto e
semantica correta.

ApplyA moderno já visto como risco:

```text
ApplyA=91 com MethodID=209 em Skill 7700521
```

Se o crash for `Undefined skill tooltip apply type`, mapear o `ApplyA` novo.
Se o crash for `Undefined skill tooltip method`, mapear o `MethodID` novo.

### Buff_Data.bin / Buff_Data.xml

Responsavel pelos efeitos de buff/debuff, icones, duração e descrição.

Quando uma skill tem apply especial mas o tooltip cai tentando desenhar efeito,
conferir:

- se existe `BuffCode`;
- se `Buff_Data` tem a linha;
- se o client consegue resolver o texto e icone;
- se o servidor aplica/remove o buff corretamente.

Regras praticas aprendidas na importacao recente:

- Se o global mostra um buff no player, a conversao tambem precisa criar o buff
  visivel no player com icone e nome.
- Se o global aplica debuff no monstro, a conversao precisa criar o debuff
  visivel no alvo/monstro.
- Nao basta aplicar o efeito numerico no servidor. A UI tambem precisa resolver
  icone, nome e descricao.
- Skill/buff sem string correta aparece como fallback, por exemplo
  `Skill 7112731` ou `Buff 64421`.
- Conferir os bins/string tables do cliente junto com `Skill.bin` e
  `Buff_Data`, especialmente quando o tooltip de cima ainda mostra o ID em vez
  do nome.
- Formatar texto de efeito com espacos legiveis. Exemplo aceitavel:

  ```text
  With 100% chance Critical Damage 20% Increase (30 Second)
  ```

  Evitar texto colado como `chanceCritical`.

Caso real usado como referencia:

- OMM F3 aplica Critical Damage 20% por 30 segundos.
- O efeito deve aparecer no tooltip da skill e tambem como buff visivel no
  player.
- A database pode chamar o campo de `criticaldamage`; confirmar se o efeito
  final e Critical Damage, nao taxa de critico.

### Model.dat e Pack01

Nao substituir `Model.dat` inteiro sem necessidade.

Workflow correto:

1. Identificar o model id faltante no crash.
2. Verificar se o modelo existe na fonte descompactada:

   ```text
   D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11
   ```

3. Copiar apenas os arquivos de modelo/textura/animação necessários para o
   Pack01/cliente ativo.
4. Validar se o `Model.dat` antigo consegue referenciar o ID.
5. Se o ID nao existe no `Model.dat` antigo, converter/adicionar somente a linha
   necessária no formato antigo.

Sinais de model incompleto:

- popup `Model.dat - <id>`;
- client abre e trava logo depois;
- Digimon invisivel;
- textura rosa ou branca;
- skill effect nao renderiza.

### Skill effects / scene effects

Se a evolução aparece e a skill executa, mas nao tem efeito visual:

- verificar `Skill.bin` skill effect/id;
- verificar `Buff_Data` se for efeito de buff;
- verificar arquivos `.nif`, `.kf`, textura e som no Pack01;
- verificar paths extraidos do cliente global;
- testar em Tamer-control e Digimon-control view.

Se a skill gasta DS, aplica buff, mas nao toca animacao/efeito:

- conferir se o icone/skill effect pertence ao Digimon correto, nao a uma forma
  parecida;
- comparar F1/F2/F3/F4 individualmente, pois uma skill pode funcionar e outra
  nao;
- verificar se a skill tem effect/scene/sound em Pack01;
- verificar no servidor se o processor esta enviando o resultado visual correto
  e nao apenas aplicando status.

Caso OMM observado:

- F3 chegou a aplicar buff sem animacao/efeito, depois a animacao foi ajustada
  e o buff precisou ser restaurado.
- F4 tinha problema separado de animacao/efeito.
- Os icones de skill chegaram a apontar para Zwart D, entao sempre validar
  icones contra o global para cada forma nova.

## 6. Ordem segura de importação

1. Escolha um Digimon ou uma linha evolutiva pequena.
2. Copie as linhas fonte dos XML novos para `DigimonImportMaps`.
3. Compare com as linhas antigas equivalentes.
4. Gere relatório de dependências:
   - Digimon IDs;
   - Skill IDs;
   - Buff IDs;
   - Model IDs;
   - Tactics IDs;
   - item IDs;
   - icon/card paths.
5. Converta `Digimon_List.bin`.
6. Converta `Tactics.bin`.
7. Converta `DigimonEvo.bin`.
8. Converta `Skill.bin`.
9. Converta/copie `Buff_Data` se a skill usar buff/debuff novo.
10. Copie assets Pack01 faltantes.
11. Atualize os bins do servidor.
12. Atualize Pack03/cliente.
13. Recompile cliente.
14. Rebuild/restart servidor.
15. Teste login, janela Digimon Information, evolução, ataque básico, skill,
    tooltip e relog.

## 7. Atualizar servidor

Copiar os bins convertidos para:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
```

Depois validar build:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\DigitalWorldOnline.sln" -c Release
```

Ou, se quiser só o host de game:

```powershell
dotnet build "C:\0-NewDmo\dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\DigitalWorldOnline.Game.csproj" -v:minimal
```

Subir/reiniciar:

```powershell
cd "C:\0-NewDmo\dmo-server-main-postgres"
docker compose up -d --build game
```

Logs úteis:

```powershell
docker logs --tail 300 dmo-pg-game
docker logs --tail 300 dmo-pg-game | Select-String "Loaded Digimon_List|Loaded DigimonEvo|Skill|Tactics|error|fail|exception"
```

## 8. Atualizar cliente / Pack03

Atualizar os bins convertidos no Pack03/extraido usado pelo cliente.

Conferir pelo menos:

```text
Digimon_List.bin
DigimonEvo.bin
Tactics.bin
Skill.bin
Buff_Data.bin se usado
```

Se o sistema de pack exigir repack, repackar Pack03 depois de trocar os bins.
Se o teste estiver apontando direto para extraido, garantir que o cliente esteja
realmente lendo a pasta alterada.

Erro comum: arquivo certo no lugar errado. Sintomas:

- mudança nao aparece no client;
- textura fica rosa;
- tooltip/linha antiga continua;
- servidor tem dados novos, mas UI mostra dados antigos.

## 9. Atualizar Pack01/assets

Quando o Digimon/model/skill effect novo precisar de assets, buscar na pasta
fonte:

```text
D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11
```

Copiar mantendo a estrutura relativa original. Nao achatar pastas.

Validar:

- modelo aparece;
- textura nao fica rosa;
- animações basicas funcionam;
- idle/walk/attack carregam;
- skill effect aparece;
- som nao causa erro.

## 10. Recompilar cliente

Sempre usar o alvo English release pela solution:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Se der contenção de PDB:

```powershell
$env:CL="/FS"
& "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" `
  "C:\0-NewDmo\dmo-client-main\DProject.sln" `
  /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal
```

Copiar exe:

```powershell
Copy-Item "C:\Release\GSP\DProject_R_ENG.exe" "C:\0-NewDmo\ClientDist\DProject_R_ENG.exe" -Force
Copy-Item "C:\Release\GSP\DProject_R_ENG.exe" "D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe" -Force
```

Se o copy falhar, fechar `DProject_R_ENG.exe`.

## 11. Teste minimo obrigatorio

Para cada Digimon/evolução importada:

1. Login no personagem.
2. Abrir Digimon Information.
3. Conferir arvore `Evolution & Riding`.
4. Passar mouse sobre cada skill.
5. Evoluir para a forma nova.
6. Ver modelo, nome, rank e stats.
7. Testar ataque básico na visão do Tamer.
8. Testar ataque básico na visão do Digimon.
9. Testar skill na visão do Tamer.
10. Testar skill na visão do Digimon.
11. Relogar com a evolução desbloqueada.
12. Verificar logs do cliente e servidor.

Validacoes extras para formas novas:

- conferir se o icone da base/rookie aparece para voltar a evolucao, quando
  esse for o comportamento esperado no global;
- conferir icones de skill contra o global;
- conferir tooltip de skill, nomes, cooldown, consumo, dano e efeito;
- conferir buff/debuff visivel no player/monstro quando a skill aplicar.

## 12. Como interpretar crashes comuns

### `MonsterMng.cpp line 693`

Normalmente indica Monster/Digimon ID referenciado mas nao carregado no mapa
esperado.

Conferir:

- `Digimon_List.bin`;
- monster table se o ID for mob;
- duplicidade de ID;
- formato convertido;
- se client e servidor estao com o mesmo conjunto de bins.

### `TacticsMng.cpp line 75`

Falta `TacticsExplain` para o Digimon ID.

Conferir:

- `Tactics.bin`;
- `DigimonName_Str`;
- conversão de linha da evolução;
- IDs usados pela UI da arvore.

### `QuickEvol.cpp line 360`

Falta rota/slot compativel para quick evolution.

Conferir:

- `DigimonEvo.bin`;
- slot index;
- max evolutions suportado pelo client;
- rota especial que pode precisar de template manual.

### `ToolTip_Skill.cpp`

Se o assert fala `method`:

- adicionar suporte ao `MethodID`;
- comparar com `SkillCodeDigimon.xml`;
- atualizar `FmCommon.cpp` se o texto comum tambem precisar.

Se o assert fala `apply type`:

- mapear o `ApplyA`;
- conferir enum `SkillCodeApplyAttributeEnum.cs`;
- conferir `pParameter.h`;
- decidir texto correto para tooltip.

Nao comentar o assert sem entender o dado.

### `Model.dat`

Falta model id ou asset do model.

Conferir:

- `Model.dat`;
- Pack01;
- paths de `.nif`, `.kf`, `.dds`, `.tga`;
- source `data11`.

## 13. Checklist de compatibilidade cliente/servidor

Cliente e servidor precisam concordar em:

- Digimon ID;
- evolution slot;
- current type/base type;
- skill id;
- target/range;
- attack timing;
- buff id;
- item requirement;
- max skill level;
- model id apenas no cliente;
- packet shape para ataque/evolução/status.

Se o servidor aceita a evolução mas o cliente trava, o problema costuma estar
em bins/assets/tooltip/model no cliente.

Se o cliente mostra a evolução mas ataque/skill nao acontece, conferir:

- `Skill.cpp`;
- `TamerUser.cpp`;
- `PartnerAttackPacketProcessor.cs`;
- `PartnerSkillPacketProcessor.cs`;
- target/range/casting/cooldown no `Skill.bin`.

Se a skill usa no servidor mas nao aparece efeito visual, conferir Pack01 e
Buff/Skill effect.

## 14. Sobre Seal Master e `!openseals`

Nao usar `3000` fixo para todos os selos.

Estado atual:

- `!openseals` foi corrigido para nao abrir todos os selos em `3000`.
- O comando agrupa por `SealId` e usa o maior `RequiredAmount`/
  `SequentialId` carregado para cada selo.
- A regra precisa continuar alinhada com o max/tier oficial de
  `SealCard`/`SealLeader`.

Checklist de manutencao:

1. Ler max/tier oficial de `SealCard`/`SealLeader`.
2. Aplicar por seal id.
3. Garantir que `!openseals` respeita max individual.
4. Atualizar tooltip para refletir o mesmo max.
5. Testar selo com max menor que 3000, como os casos vistos no global.

Arquivos relevantes:

```text
dmo-client-main\DProject\_Interface\Game\SealMaster.cpp
dmo-client-main\DProject\_Interface\Game\SealMaster.h
dmo-client-main\DProject\_Interface\Game\ExtraInventory.cpp
dmo-server-main-postgres\src\Source\Distribution\DigitalWorldOnline.Game.Host\GameMasterCommandsProcessor.cs
dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Character\CharacterSealModelBehavior.cs
dmo-server-main-postgres\src\Source\Domain\DigitalWorldOnline.Commons\Models\Character\CharacterSealListModelBehavior.cs
```

## 15. O que evitar

- Nao copiar `Model.dat` inteiro sem saber se o formato bate.
- Nao substituir todo Pack03 quando só uma linha nova é necessária.
- Nao remover asserts de tooltip/tactics/model para "passar".
- Nao tratar crash de tooltip como asset faltando sem verificar MethodID/ApplyA.
- Nao misturar importação de muitos Digimon com refactor de servidor no mesmo
  passo.
- Nao confiar só em data de criação do arquivo; comparar conteúdo, formato e
  contagem de registros.

## 16. Registro final por importação

Ao terminar uma importação, registrar no mapping:

```text
Digimon IDs adicionados:
Evolution IDs/slots:
Skill IDs:
Buff IDs:
Model IDs:
Assets Pack01 copiados:
Bins cliente alterados:
Bins servidor alterados:
Arquivos C++ alterados:
Arquivos C# alterados:
Build cliente:
Build servidor:
Testes feitos:
Crashes encontrados:
Pendencias:
```

Esse registro vira o ponto de rollback se a próxima importação quebrar algo.
