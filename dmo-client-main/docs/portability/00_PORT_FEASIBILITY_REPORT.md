# Relatorio de Viabilidade de Portabilidade

Projeto: DMO Client legado C++ 32-bit  
Escopo: avaliacao tecnica para migracao progressiva para C++ moderno, Windows x64 e possivel suporte cross-platform/mobile com SDL3 e Diligent/bgfx.  
Data: 2026-06-07

## Conclusao Executiva

A portabilidade e **parcialmente viavel**, mas nao como uma recompilacao direta do cliente atual. O cliente esta profundamente acoplado a Win32, DirectX 9, Gamebryo 2.3, Miles Sound System, DirectInput e bibliotecas estaticas 32-bit. O caminho seguro e uma migracao em etapas, mantendo o cliente DX9 como referencia funcional enquanto se criam adaptadores e PoCs isoladas.

O primeiro objetivo realista nao deve ser "rodar em mobile" nem "trocar o renderer". O primeiro objetivo deve ser provar uma base Windows x64/minima, com subsistemas nao-renderer compilando fora do executavel principal, e listar exatamente quais bibliotecas proprietarias impedem o link x64.

## Recomendacao Principal

Recomendacao: **manter o cliente C++ legado como fonte de verdade e fazer migracao incremental por adaptadores**.

Nao e recomendavel reescrever o cliente inteiro de uma vez em outro stack. Tambem nao e recomendavel substituir Gamebryo/DX9 logo no primeiro movimento. A chance de regressao em rendering, assets, UI, animacao, input, pacotes e sincronizacao de objetos seria alta demais.

Para uma futura camada grafica:

- **bgfx** parece o melhor candidato para a primeira PoC de renderizacao multiplataforma, por ser pragmatico, maduro em desktop/mobile e mais facil de usar como backend experimental.
- **Diligent Engine** e uma opcao forte se o objetivo for reconstruir um renderer moderno com controle explicito de pipelines, shaders e APIs, mas tende a exigir uma modelagem mais completa desde cedo.
- A decisao definitiva deve vir depois de uma PoC pequena com malha, textura, camera, blend, depth, alpha e uma amostra de UI/sprite.

## O Que E Viavel

| Objetivo | Viabilidade | Comentario |
|---|---:|---|
| Auditoria e inventario tecnico | Alta | Ja e possivel mapear dependencias e blockers com o codigo atual. |
| Isolar subsistemas nao-renderer | Alta | `nlib`, parte de `common_vs2019`, parsers e pacotes podem ser trabalhados antes do renderer. |
| Windows x64 parcial | Media | Bloqueado por libs 32-bit, casts perigosos, `__asm` e Win32 APIs antigas. |
| Windows x64 completo com renderer atual | Baixa | Gamebryo/DX9/Miles parecem depender de libs x86. Precisa confirmar binariamente. |
| SDL3 para janela/input em branch experimental | Media | Viavel se mantiver Win32 como caminho original durante a transicao. |
| Renderer novo com bgfx/Diligent | Media/baixa | Viavel como PoC; alto risco como troca direta. |
| Android/iOS | Baixa no curto prazo | Exige novo renderer, novo audio, novo input, lifecycle, asset pipeline e possivelmente abandono de partes proprietarias. |

## Top 10 Riscos

1. **Gamebryo 2.3 e bibliotecas externas 32-bit**: o projeto referencia `NiDX9Renderer.lib`, `NiInput.lib`, `NiMilesAudio.lib`, `NiMain.lib` e outras libs proprietarias. Se nao houver build x64/source completo, o cliente nao linka em x64.
2. **Renderer DX9 acoplado ao motor**: `DProject/Engine.cpp`, `Engine_Render.cpp` e `Engine.h` usam `NiDX9Renderer`, `IDirect3D9`, `IDirect3DDevice9`, `D3DXMATRIX`, `D3DVIEWPORT9` e DirectDraw.
3. **Win32 no loop principal**: `CWin32App::Startup`, `Run` e `WndProc` dependem de `RegisterClass`, `CreateWindowEx`, `PeekMessage`, `DispatchMessage`, `DefWindowProc`, `SetWindowLong` e mensagens Win32.
4. **Input legado**: `CInput.cpp` usa `NiDI8InputSystem`, `NiInputKeyboard`, `NiInputMouse`; `GlobalInput.cpp` usa `GetAsyncKeyState`, `MSG`, `WM_KEY*`, `VK_*`, `MapVirtualKey`, `SetCursorPos`.
5. **Audio Miles**: `CSoundMgr::_InitMiles` usa `NiMilesAudioSystem` e `mss32.lib/mss32.dll`, com dependencia historicamente sensivel no import library.
6. **Serializacao binaria e pack files**: varios sistemas usam `sizeof(struct)`, `#pragma pack`, `_read/_write`, `HANDLE` e layout binario fixo.
7. **Casts e truncamento de ponteiros**: ha uso de `SetWindowLong(..., (LONG)func_ptr)` e casts para `DWORD`, bloqueando x64 seguro.
8. **Assembly inline MSVC**: `LibProj/CsFunc/assert_cs.h` usa `__asm { int 3 }`, que nao compila em x64 MSVC.
9. **UI fortemente dependente de assets e Win32**: alem da UI propria, alguns fluxos criam controles Win32 nativos e usam IME/WebControl.
10. **Mobile lifecycle/performance**: o cliente pressupoe loop desktop, janela persistente, filesystem local, mouse/teclado e recursos DX9.

## Diligent vs bgfx

| Criterio | Diligent | bgfx | Observacao no projeto |
|---|---|---|---|
| Facilidade para PoC rapida | Media | Alta | bgfx tende a permitir uma PoC menor para mesh/textura/sprite. |
| Controle moderno de pipelines | Alta | Media | Diligent e mais adequado se o renderer for redesenhado com pipelines explicitos. |
| Mobile | Alta | Alta | Ambos suportam mobile, mas os blockers do cliente estao antes do backend grafico. |
| Compatibilidade mental com DX9 legado | Media | Media/alta | bgfx pode ser mais simples para mapear estados basicos e draw calls iniciais. |
| Substituir Gamebryo diretamente | Baixa | Baixa | Nenhum substitui Gamebryo sem recriar asset loading, cena, animacao e efeitos. |
| Melhor primeira escolha | Condicional | Recomendada para PoC | Recomendacao: PoC bgfx, mantendo Diligent em avaliacao. |

## Caminho Recomendado

1. Criar uma trilha de auditoria/PoC que nao altera o cliente de producao.
2. Confirmar arquitetura real das libs `.lib/.dll` com ferramentas como `dumpbin /headers`.
3. Criar uma configuracao experimental x64 ou CMake subset para compilar `nlib`, `common_vs2019`, `CsFunc`, `CsFilePack` e parsers isolados.
4. Corrigir blockers x64 mecanicos apenas em branch experimental: `__asm`, `SetWindowLong`, casts `DWORD`, tipos de ponteiro e structs serializadas.
5. Criar interfaces pequenas para janela, input, audio, filesystem e renderer, mas ligar primeiro ao backend legado.
6. Fazer uma PoC bgfx separada que carrega uma textura/mesh simples e valida blend/depth/camera.
7. Somente depois avaliar substituicao progressiva de renderizacao de sprites/UI, terreno e personagens.

## Nao Fazer Agora

- Nao remover Gamebryo do cliente principal.
- Nao iniciar por Android/iOS.
- Nao trocar o renderer sem uma camada de compatibilidade e testes visuais.
- Nao converter todo o projeto para x64 antes de confirmar as bibliotecas proprietarias.
- Nao tratar o port C# experimental como substituto imediato do cliente C++.

## Fontes Inspecionadas

- `DProject/main.cpp`
- `DProject/App/Win32App.cpp`
- `DProject/App/GameApp.cpp`
- `DProject/Engine.cpp`
- `DProject/Engine_Render.cpp`
- `DProject/Engine.h`
- `DProject/CInput.cpp`
- `DProject/GlobalInput.cpp`
- `DProject/CSoundMgr.cpp`
- `DProject/network/cNetwork.cpp`
- `DProject/DProject.vcxproj`
- `DProject.sln`
- `LibProj/CsFilePack/*`
- `LibProj/CsFileTable/*`
- `LibProj/CsFunc/*`
- `common_vs2019/*`

## Itens DESCONHECIDO

- Arquitetura real de todas as libs proprietarias (`Ni*.lib`, `Cs*.lib`, `mss32.lib`) ainda nao foi validada com `dumpbin`.
- Disponibilidade de source/build x64 para Gamebryo 2.3 e Miles Sound System e **DESCONHECIDO**.
- Formato completo dos assets dentro dos packs ativos e **DESCONHECIDO** nesta auditoria.
- Compatibilidade legal/licenciamento para redistribuir Gamebryo/Miles em novas plataformas e **DESCONHECIDO**.
- Viabilidade de iOS com os assets e codecs atuais e **DESCONHECIDO**.
