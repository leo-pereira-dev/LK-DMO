# Asset Inventory

Status do inventario visual da janela oficial de fusao.

## Fonte

- Paths oficiais: `unpacked_exe_all\unpacked_GDMO.exe_strings.csv`.
- Funcoes de UI copiadas: `decompiled\DigiCombineUI` e `decompiled\UnionCombineUI`.
- Pack de origem informado/mapeado: `ClientDist\Data\Pack01.hf` e `ClientDist\Data\Pack01.pf`.
- Manifesto amplo/candidato: `assets\manifest\DigiCombineAssets.csv`.
- Manifesto fiel aos `.c` copiados:
  `assets\manifest\DecompiledUiAssetReferences.csv`.

## Cobertura Atual

- XMLs oficiais: copiados em `xml\DigiCombine` e `xml\UnionCombine`.
- BINs oficiais: copiados em `bin\DigiCombine` e `bin\UnionCombine`.
- Funcoes de tabela: copiadas em `decompiled\DigiCombine` e `decompiled\UnionCombine`.
- Funcoes de UI: copiadas em `decompiled\DigiCombineUI` e `decompiled\UnionCombineUI`.
- Assets da janela confirmados por funcao decompilada: listados em
  `assets\manifest\DecompiledUiAssetReferences.csv` e marcados como `Pack01`.

## Ultima Varredura

Foi feita uma varredura cruzando todos os simbolos `s_*_<address>` nos arquivos
`.c` de `decompiled\DigiCombineUI` e `decompiled\UnionCombineUI` contra
`unpacked_exe_all\unpacked_GDMO.exe_strings.csv`.

Resultado:

- `49` paths unicos de asset aparecem diretamente nas funcoes decompiladas de UI.
- `DecompiledUiAssetReferences.csv` e a lista fiel para portar a janela.
- `DigiCombineAssets.csv` continua util como inventario amplo de strings
  relacionadas, mas contem candidatos globais que nao necessariamente aparecem
  nas funcoes copiadas da janela.

## Observacao

Os assets ainda nao foram extraidos fisicamente do `Pack01`; o ponto importante
para o port e que os paths confirmados por decompilado ja estao registrados para
carregar pelo asset loader do cliente. Extraia/copiar fisicamente apenas se for
necessario inspecionar a imagem ou criar overrides soltos para teste.
