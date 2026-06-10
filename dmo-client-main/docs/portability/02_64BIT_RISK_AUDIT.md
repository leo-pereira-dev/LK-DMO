# Auditoria de Riscos x86 para x64

Escopo: riscos especificos para migrar o cliente C++ atual de Win32/x86 para Windows x64. Esta auditoria nao assume troca de renderer.

## Resumo

O cliente nao esta pronto para x64. Existem tres classes de bloqueio:

1. Bibliotecas proprietarias provavelmente 32-bit.
2. Codigo que nao compila em x64 (`__asm` MSVC).
3. Codigo que ate poderia compilar, mas quebra por truncamento de ponteiros, layout binario ou APIs Win32 antigas.

## Bloqueadores Para Windows x64

| Severidade | Item | Evidencia | Acao minima |
|---|---|---|---|
| BLOQUEADOR | Projeto so tem plataforma Win32 | `DProject/DProject.vcxproj`, `TargetMachine>MachineX86` | Criar configuracao experimental x64 ou CMake subset. |
| BLOQUEADOR | Gamebryo/Miles/DX SDK x86 | `Gamebryo2.3/GB_Lib/ReleaseLib`, `DirectX_June2010/Lib/x86`, `LibProj/MilesSound/lib/mss32.lib` | Validar com `dumpbin`; obter libs x64 ou substituir subsistemas. |
| BLOQUEADOR | Assembly inline MSVC | `LibProj/CsFunc/assert_cs.h` usa `__asm { int 3 }` | Trocar por `__debugbreak()` em branch experimental. |
| BLOQUEADOR | Renderer DX9 Gamebryo | `CEngine::_CreateRender`, `CsGBDX9Renderer::CsGBCreate` | So linka x64 se houver stack Gamebryo x64. |
| ALTO | Subclassing Win32 com `SetWindowLong` e cast `LONG` | `ItemSeparate.cpp`, `MoneySeparate.cpp` | Usar `SetWindowLongPtr`/`LONG_PTR` quando x64. |
| ALTO | Ponteiro convertido para `DWORD` | `LibProj/CsFunc/CsFile.cpp` | Usar `uintptr_t`, `DWORD_PTR` ou mensagem segura. |
| ALTO | Serializacao com `sizeof(struct)` | `LibProj/CsFileTable/BaseMng.cpp`, `LibProj/CsFilePack/CsFilePack.cpp` | Congelar tipos de disco e validar `static_assert(sizeof(...))`. |

## Riscos de Ponteiro e Inteiro

| Severidade | Padrao | Evidencia | Risco |
|---|---|---|---|
| ALTO | `(LONG)func_ptr` em Win32 | `SetWindowLong(..., (LONG)EditSubProc_...)` | Trunca ponteiro x64. |
| ALTO | `(DWORD)pointer` | `CsFile.cpp` em envio de mensagem/browser | Trunca ponteiro x64. |
| MEDIO | Cast de estruturas DX/Gamebryo | `CEngine::Cal3D_To_2D`: `NiPoint3*` para `D3DXVECTOR3*` | Depende de layout/alinhamento. |
| MEDIO | Handles/ids em tipos fixos | Uso historico de Win32 types | Requer busca direcionada ao criar x64. |
| MEDIO | `int`, `u4`, `u8` em protocolo | `common_vs2019`, `nlib` | Protocolos devem manter tamanho exato; nao trocar por `size_t`. |

## Riscos de Layout Binario

| Severidade | Arquivo/area | Evidencia | Comentario |
|---|---|---|---|
| ALTO | `LibProj/CsFilePack` | `#pragma pack(push, 4)`, `FPHeader`, `sCHUNK`, `_read/_write(sizeof(...))` | Layout de pack deve ser identico. |
| ALTO | `LibProj/CsFileTable` | Leitura/escrita de estruturas e tabelas com `sizeof` | Risco de incompatibilidade com `.bin`. |
| ALTO | `common_vs2019/cEvoUnit.h` | `#pragma pack(1)` | Deve preservar layout. |
| ALTO | `common_vs2019/cItemData.h` | `#pragma pack(4)` | Deve preservar layout. |
| ALTO | `common_vs2019/cDigimonCard.h` | `#pragma pack(4)` | Deve preservar layout. |
| MEDIO | `DProject/Data_PostLoad.h` | `#pragma pack(1)` | Validar tamanho antes/depois. |

## APIs Windows Que Precisam de Atualizacao

| Severidade | API | Onde aparece | Acao |
|---|---|---|---|
| ALTO | `SetWindowLong` | Janela principal e controles UI | Trocar por `SetWindowLongPtr` onde carregar ponteiros. |
| MEDIO | `GetWindowLong` | UI/subclassing | Trocar por `GetWindowLongPtr`. |
| MEDIO | `CreateWindowEx`, `RegisterClass`, `WNDCLASS` | `Win32App.cpp` | Pode permanecer em Windows x64; nao e cross-platform. |
| MEDIO | `DirectDrawCreate`, `SetDisplayMode` | `Engine.cpp` | Remover/substituir em renderer moderno. |
| MEDIO | `Sleep` | `GameApp.cpp`, `cNetwork.cpp` | Substituir por `std::this_thread::sleep_for` em codigo portavel. |
| BAIXO | `WideCharToMultiByte`, `MultiByteToWideChar` | `Win32App.cpp` | OK em Windows; abstrair para cross-platform. |

## Bibliotecas e Toolchain

| Item | Status | Risco |
|---|---|---|
| MSVC x64 | DESCONHECIDO para o projeto completo | Falta configuracao e validacao. |
| DirectX June 2010 x64 | Parcialmente possivel para DX libs, mas projeto aponta `Lib/x86` | Medio. |
| Gamebryo x64 | DESCONHECIDO | Bloqueador ate confirmar. |
| Miles x64 | DESCONHECIDO | Bloqueador audio ate confirmar/substituir. |
| zlib/libpng/freetype | Provavelmente recompilaveis | Baixo/medio. |
| `nlib`/ASIO | Potencialmente portavel | Medio, depende de includes e defines. |

## Plano x64 Seguro

1. Rodar `dumpbin /headers` nas libs criticas para confirmar `machine (x86)` vs `machine (x64)`.
2. Criar branch/configuracao experimental, sem alterar `Release_English|Win32`.
3. Corrigir apenas blockers mecanicos: `__asm`, `SetWindowLongPtr`, casts de ponteiro, includes de Windows.
4. Adicionar `static_assert(sizeof(...))` para estruturas lidas de disco/pacote/protocolo.
5. Compilar primeiro bibliotecas de baixo risco: `nlib`, `common_vs2019`, `CsFunc`, `CsFilePack`.
6. So tentar link do executavel depois de confirmar bibliotecas proprietarias x64.

## Decisao

Windows x64 e um alvo valido para uma primeira trilha tecnica, mas deve ser tratado como **PoC de compilacao e inventario de blockers**, nao como promessa de runtime completo.

O primeiro sucesso mensuravel deve ser: "subconjunto nao-renderer compila em x64 e preserva layouts binarios essenciais". O cliente completo so passa a ser alvo quando Gamebryo/Miles/DirectX estiverem resolvidos.
