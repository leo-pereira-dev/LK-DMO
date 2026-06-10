# Tamer bin migration

Migração criada para converter os tamers e skills novos do pacote moderno em
bins compatíveis com o template antigo do LK-DMO.

## Fonte

Fontes locais usadas pela migracao:

```text
C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language
C:\0-NewDmo\TamerBinMigrationFeature\generated\pack03-source
```

XML de strings gerado para esta migração:

```text
C:\0-NewDmo\TamerBinMigrationFeature\source_xml_language\_str
```

## Saida convertida

```text
C:\0-NewDmo\TamerBinMigrationFeature\generated\converted
```

Arquivos gerados:

```text
TamerList.bin
CharCreateTable.bin
DMBase.bin
Skill.bin
```

## Aplicacao

Os bins convertidos foram copiados para:

```text
C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english
C:\0-NewDmo\TamerBinMigrationFeature\generated\pack03-source
```

O pack ativo do cliente tambem foi atualizado:

```text
C:\0-NewDmo\ClientDist\Data\Pack03.hf
C:\0-NewDmo\ClientDist\Data\Pack03.pf
```

## Conversor

Script:

```text
C:\0-NewDmo\TamerBinMigrationFeature\tools\Convert-NewTamersToLegacy.ps1
```

O script:

- le `Tamer.xml`, `Tamer_Str.xml`, `CreateList_Tamer.xml`, `TamerStats.xml`,
  `TamerSkill.xml` e `Skill_Str.xml`;
- preserva dados existentes de `TamerList.bin` e `CharCreateTable.bin` quando o
  tamer ja existia no template antigo;
- adiciona os tamers `80011`, `80012`, `80013`, `80016`, `80017`, `80018`,
  `80019`, `80020` e `80021`;
- converte `TamerStats.xml` para a secao 1 antiga de `DMBase.bin`, mantendo 120
  niveis por tamer;
- troca a secao de `CsTamerSkill::sTSkill_INFO` dentro de `Skill.bin` de 64
  para 116 entradas;
- atualiza nomes e descricoes das skills de tamer dentro de `Skill.bin`.

## Relatorios

```text
C:\0-NewDmo\TamerBinMigrationFeature\generated\reports
```

Relatorios principais:

```text
tamerlist-conversion.csv
charcreate-tamer-conversion.csv
dmbase-tamerstats-conversion.csv
skill-tamerskill-section.csv
skillbin-summary.csv
```

## Backups

Antes de aplicar os bins convertidos, os arquivos anteriores foram copiados para:

```text
C:\0-NewDmo\TamerBinMigrationFeature\generated\backups
```

O patcher do pack tambem gerou backup do `Pack03.hf` com sufixo
`bak_assets_YYYYMMDD_HHMMSS`.

## Observacoes

- `80021` (Digimon Kaiser) nao existia nos enums antigos; foi adicionado no
  cliente e no servidor.
- Os tamers novos entram na criacao de personagem como `Show=1` e `Enable=0`,
  seguindo a fonte global e preservando o comportamento atual dos tamers
  bloqueados.
- `DMBase.bin` continua no limite antigo de 120 niveis por tamer, mesmo que a
  fonte moderna tenha 180 niveis.
