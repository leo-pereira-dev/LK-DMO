# Roadmap de PoCs

Este roadmap propoe passos pequenos, reversiveis e mensuraveis. A regra geral e: provar um subsistema antes de tocar no cliente de producao.

Escala de esforco:

- P: pequeno
- M: medio
- G: grande
- GG: muito grande

## Fase 0: Auditoria documental

| Campo | Valor |
|---|---|
| Objetivo | Mapear dependencias, riscos e ordem de ataque. |
| Arquivos afetados | Apenas `docs/portability/*`. |
| Risco | Baixo. |
| Esforco | P. |
| Sucesso | Relatorios criados e revisados. |
| Rollback | Remover docs. |

Status: realizado nesta entrega.

## Fase 1: Confirmar arquitetura das bibliotecas

| Campo | Valor |
|---|---|
| Objetivo | Saber se libs criticas sao x86-only. |
| Arquivos afetados | Nenhum codigo; scripts/logs opcionais. |
| Risco | Baixo. |
| Esforco | P. |
| Sucesso | Lista `x86/x64/DESCONHECIDO` para cada `.lib/.dll`. |
| Rollback | Nao aplicavel. |

Comandos sugeridos:

```powershell
dumpbin /headers Gamebryo2.3\GB_Lib\ReleaseLib\NiMain.lib
dumpbin /headers LibProj\MilesSound\lib\mss32.lib
```

## Fase 2: Subconjunto x64 sem renderer

| Campo | Valor |
|---|---|
| Objetivo | Compilar bibliotecas nao-renderer em x64. |
| Arquivos afetados | Config experimental/CMake, nao `Release_English`. |
| Risco | Medio. |
| Esforco | M. |
| Sucesso | `nlib`, `common_vs2019`, partes de `CsFunc`, `CsFilePack` compilam x64. |
| Rollback | Remover configuracao experimental. |

Nao incluir:

- `DProject/Engine.cpp`
- Gamebryo renderer
- Miles audio
- UI Win32 complexa

## Fase 3: Correcoes mecanicas x64

| Campo | Valor |
|---|---|
| Objetivo | Corrigir blockers obvios sem alterar protocolo/layout. |
| Arquivos provaveis | `LibProj/CsFunc/assert_cs.h`, `ItemSeparate.cpp`, `MoneySeparate.cpp`, `CsFile.cpp`. |
| Risco | Medio/alto. |
| Esforco | M. |
| Sucesso | Compilacao x64 avanca sem truncamento de ponteiro conhecido. |
| Rollback | Reverter branch experimental. |

Validacoes:

- `static_assert(sizeof(...))` para structs de disco/protocolo.
- Nao trocar tipos de pacote por `size_t`.
- Nao mudar ordem/campo de structs serializadas.

## Fase 4: Abstracao de path/filesystem

| Campo | Valor |
|---|---|
| Objetivo | Reduzir dependencia de paths Windows. |
| Arquivos provaveis | `CsFilePack`, loaders de assets, inicializacao `Data\\...`. |
| Risco | Medio. |
| Esforco | M. |
| Sucesso | Leitura de pack e arquivos funciona com separador normalizado. |
| Rollback | Manter wrappers chamando APIs antigas. |

## Fase 5: Loop SDL3 isolado

| Campo | Valor |
|---|---|
| Objetivo | Criar app minimo SDL3 com loop, janela e eventos. |
| Arquivos afetados | Novo diretorio de PoC. |
| Risco | Baixo/medio. |
| Esforco | P/M. |
| Sucesso | Janela abre, processa teclado/mouse, fecha limpo. |
| Rollback | Remover PoC. |

Nao integrar ainda com `CWin32App`.

## Fase 6: PoC bgfx desktop

| Campo | Valor |
|---|---|
| Objetivo | Validar backend grafico moderno. |
| Arquivos afetados | Novo diretorio de PoC. |
| Risco | Medio. |
| Esforco | M. |
| Sucesso | Clear, textura, quad alpha, mesh simples e camera. |
| Rollback | Remover PoC. |

Resultado esperado:

- Evidencia visual.
- Lista de recursos suportados.
- Comparacao com Diligent se necessario.

## Fase 7: Asset real em renderer novo

| Campo | Valor |
|---|---|
| Objetivo | Renderizar textura real do cliente fora de Gamebryo. |
| Arquivos afetados | PoC + loader simples. |
| Risco | Medio/alto. |
| Esforco | M/G. |
| Sucesso | Uma textura UI ou item renderiza corretamente. |
| Rollback | PoC isolada. |

Boas escolhas:

- textura `.png`/`.tga` simples;
- sprite de UI;
- evitar modelo 3D no primeiro teste.

## Fase 8: Fachada de renderer no cliente legado

| Campo | Valor |
|---|---|
| Objetivo | Passar funcoes basicas do renderer atual por uma interface. |
| Arquivos provaveis | `Engine.cpp`, `Engine_Render.cpp`, `Engine.h`. |
| Risco | Alto. |
| Esforco | G. |
| Sucesso | Cliente DX9 roda identico com backend legado atras da fachada. |
| Rollback | Reverter interface/branch. |

Esta fase so deve ocorrer depois da PoC provar o desenho.

## Fase 9: Audio portavel isolado

| Campo | Valor |
|---|---|
| Objetivo | Testar backend audio sem Miles. |
| Arquivos afetados | Novo PoC; depois `CSoundMgr` em branch. |
| Risco | Medio. |
| Esforco | M. |
| Sucesso | Toca musica/efeito convertido de asset real. |
| Rollback | Manter Miles no cliente legado. |

Candidatos:

- SDL audio;
- miniaudio;
- OpenAL Soft.

## Fase 10: Input SDL3 mapeado

| Campo | Valor |
|---|---|
| Objetivo | Mapear eventos SDL3 para a semantica atual de input. |
| Arquivos provaveis | `GlobalInput.cpp`, `CInput.cpp`, nova camada de input. |
| Risco | Alto. |
| Esforco | G. |
| Sucesso | Login/menu simples aceita teclado/mouse via camada nova. |
| Rollback | Backend Win32/DirectInput permanece. |

## Fase 11: Android smoke test

| Campo | Valor |
|---|---|
| Objetivo | Provar app SDL3 + bgfx/Diligent em Android. |
| Arquivos afetados | PoC mobile. |
| Risco | Medio/alto. |
| Esforco | M/G. |
| Sucesso | Clear + textura real em device/emulador. |
| Rollback | PoC isolada. |

Nao incluir rede, personagem ou UI completa ainda.

## Fase 12: Cena 3D real minima

| Campo | Valor |
|---|---|
| Objetivo | Renderizar um modelo/asset 3D real ou convertido. |
| Arquivos afetados | PoC asset pipeline. |
| Risco | Muito alto. |
| Esforco | GG. |
| Sucesso | Modelo com textura, camera e talvez animacao simples. |
| Rollback | Manter PoC separada. |

Esta fase decide se a migracao pode preservar assets Gamebryo ou se exigira conversao/recriacao.

## Primeira PoC Recomendada

A primeira PoC apos estes documentos deve ser:

**Fase 1 + Fase 2: confirmar arquitetura das libs e compilar um subconjunto x64 sem renderer.**

Motivo: se Gamebryo/Miles forem x86-only e o codigo base nao compilar x64 nem parcialmente, trocar renderer agora nao desbloqueia o projeto. A prova inicial precisa separar o que e problema de toolchain/codigo do que e problema de renderer.

## Proxima PoC de Renderer

Depois da Fase 2, iniciar:

**Fase 5 + Fase 6: SDL3 + bgfx desktop isolado.**

Motivo: valida uma tecnologia moderna sem tocar no cliente de producao. Se a PoC falhar, o cliente DX9 permanece intacto.

## Critérios de Parada

Parar e reavaliar se:

- Gamebryo/Miles nao possuem x64 e nao ha alternativa viavel.
- O asset pipeline exige conversao total antes de qualquer renderizacao real.
- A UI precisa ser redesenhada completamente para mobile antes de haver renderer.
- A manutencao do cliente legado ficar comprometida por alteracoes experimentais.

## Resultado Esperado Em 4 Marcos

1. **Marco A:** inventario de libs x86/x64 completo.
2. **Marco B:** subconjunto nao-renderer compila x64.
3. **Marco C:** PoC SDL3 + bgfx renderiza textura real.
4. **Marco D:** decisao tecnica sobre manter DX9, seguir bgfx, testar Diligent ou iniciar reescrita parcial.
