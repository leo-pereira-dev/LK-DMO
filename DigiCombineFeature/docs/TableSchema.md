# Table Schema Notes

Campos com nome original conhecido foram mantidos. Campos `FieldA` a `FieldE`
foram renomeados por comportamento observado nos XMLs.

## ItemList

```text
ItemId     -> itemId
FlagOrType -> rankKey
Value      -> catalogValue
```

`rankKey` e o tier usado para validar os 4 materiais. `catalogValue` ainda
precisa ser confirmado no fluxo de UI; no `DigiCombine` ele parece apontar para
valor de catalogo/evolucao, e no `UnionCombine` para valores na faixa `450xxx`.

## RankList

```text
RankKey -> rankKey
FieldA  -> displayRank
FieldB  -> ceilingFailCount
FieldC  -> ceilingGroupKey
```

Em `DM_DigiCombine-RankList`, os ranks 7 e 8 possuem teto:

- rank 7: `ceilingFailCount=5`, `ceilingGroupKey=101`
- rank 8: `ceilingFailCount=10`, `ceilingGroupKey=102`

Em `DM_UnionCombine-RankList`, o teto vem zerado nos dados atuais.

## ItemGroup / CeilGroup

```text
GroupKey -> groupKey
ItemId   -> rewardItemId
FieldA   -> rewardCount
FieldB   -> chanceBasisPoints
FieldC   -> resultGradeBand
FieldD   -> resultRank
FieldE   -> isTierUpReward
```

`chanceBasisPoints` soma `10000` nos grupos observados. Exemplo do
`UnionCombine` rank 1:

- recompensa rank 1 com `3500`
- recompensa rank 1 com `3500`
- recompensa rank 2 com `3000` e `isTierUpReward=true`

Isto encaixa no comportamento descrito: quatro materiais tentam gerar um item
de tier maior, mas tambem podem retornar resultado do mesmo tier.
