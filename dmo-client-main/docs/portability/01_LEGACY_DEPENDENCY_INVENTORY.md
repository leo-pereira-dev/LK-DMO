# Inventario de Dependencias Legadas

Este documento lista os principais acoplamentos do cliente atual que impactam Windows x64, SDL3, Diligent/bgfx e mobile. Ele nao altera codigo de producao.

## Entrada, Inicializacao e Loop

| Area | Arquivo/funcoes | Dependencias | Risco |
|---|---|---|---|
| Entry point | `DProject/main.cpp`, `_tWinMain` | Win32 `HINSTANCE`, argumentos Windows, globals do cliente | Alto |
| Inicializacao Gamebryo | `CEngine::GBInit`, `CEngine::Init`, `CEngine::Create` | `NiInit`, `NiDX9Renderer`, `NiFile`, `NiPNGReader`, `NiMaterial` | Bloqueador cross-platform |
| Loop principal | `CWin32App::Run` | `PeekMessage`, `TranslateMessage`, `DispatchMessage`, `MSG`, `WM_QUIT` | Alto |
| Janela | `CWin32App::Startup`, `WndProc` | `RegisterClass`, `CreateWindowEx`, `ShowWindow`, `UpdateWindow`, `DefWindowProc` | Alto |
| Resize/modo janela | `CWin32App::SetClientSize`, `CEngine::SwitchFullScreen`, `SwitchWindowMode` | `SetWindowLong`, `SetWindowPos`, DirectDraw | Alto |
| Estado global | `main.cpp`, `Engine.cpp`, muitos subsistemas | `g_pRenderer`, `g_pEngine`, `GAMEAPP_ST`, singletons | Medio/alto |

## Solucao e Build

| Item | Evidencia | Impacto |
|---|---|---|
| Solucao principal | `DProject.sln` | Deve continuar sendo a entrada de build do cliente. |
| Plataforma atual | `DProject/DProject.vcxproj` com `Win32` e `TargetMachine>MachineX86` | Nao ha trilha x64 pronta. |
| Configuracao de producao | `Release_English|Win32` | Saida esperada historica: `DProject_R_ENG.exe`. |
| Bibliotecas DX SDK | `DirectX_June2010/Lib/x86` | Dependencia x86 e DX9/D3DX legado. |
| Bibliotecas Gamebryo | `Gamebryo2.3/GB_Lib/ReleaseLib/*.lib` | Bloqueador se nao houver x64/source. |
| Miles Sound | `LibProj/MilesSound/lib/mss32.lib` | Bloqueador se nao houver lib/dll x64 ou substituicao de audio. |

## Renderer e DirectX 9

| Area | Arquivo/funcoes | Dependencias | Comentario |
|---|---|---|---|
| Criacao do renderer | `DProject/Engine.cpp`, `CEngine::_CreateRender` | `NiDX9Renderer`, `IDirect3D9`, `D3DDISPLAYMODE`, `D3DCAPS9`, `CsGBDX9Renderer::CsGBCreate` | Centro do acoplamento DX9. |
| Frame rendering | `DProject/Engine_Render.cpp`, `StartRender`, `EndRender`, `Clear` | `BeginFrame`, `BeginUsingRenderTargetGroup`, `LPDIRECT3DDEVICE9` | Precisa de interface de renderer antes de trocar backend. |
| Device caps | `CEngine::_CheckDeviceCaps` | `GetDeviceCaps`, Pixel Shader 2.0, texture stages | Pode virar validacao de features no backend novo. |
| Matematica DX | `CEngine::Cal3D_To_2D` | `D3DXVECTOR*`, `D3DXMATRIX`, `D3DXVec3Transform` | Deve migrar para matematica propria ou biblioteca portavel. |
| Fullscreen legado | `SwitchFullScreen`, `SwitchWindowMode` | DirectDraw, `SetDisplayMode` | Inadequado para SDL/mobile. |
| Shader/material | `NiMaterial::SetDefaultWorkingDirectory("Data\\Shaders\\Generated")` | Gamebryo shaders gerados | Pipeline novo precisa estrategia propria. |

## Janela, Sistema Operacional e Win32

| Arquivo | Uso relevante | Risco |
|---|---|---|
| `DProject/App/Win32App.cpp` | Janela, mensagens, work area, conversao string Win32, DX version checker | Alto |
| `DProject/GlobalInput.cpp` | `GetAsyncKeyState`, `MSG`, `WM_KEY*`, `VK_*`, cursor e screenshot | Alto |
| `DProject/CIme.cpp` | IME Win32 | Alto para mobile e Linux/macOS |
| `DProject/_Interface/Game/ItemSeparate.cpp` | Controle edit Win32 e subclassing | Alto para x64/cross-platform |
| `DProject/_Interface/Game/MoneySeparate.cpp` | Controle edit Win32 e subclassing | Alto para x64/cross-platform |
| `DProject/_Interface/Game/WebControl.cpp` | Controle web/Win32 | Alto |
| `LibProj/CsFunc/CsFile.cpp` | Shell/browser/path e cast de ponteiro para `DWORD` | Alto x64 |

## Input

| Area | Arquivo/funcoes | Dependencias | Comentario |
|---|---|---|---|
| Input Gamebryo/DirectInput | `DProject/CInput.cpp`, `CreateInputSystem` | `NiDI8InputSystem`, `NiInputKeyboard`, `NiInputMouse` | Deve ser adaptado para SDL3 ou backend proprio. |
| Teclado global | `DProject/GlobalInput.cpp` | `GetAsyncKeyState`, `VK_*`, `WM_KEY*` | Precisa camada de eventos independente. |
| Mouse/cursor | `GlobalInput.cpp` | `ClientToScreen`, `SetCursorPos`, Win32 cursor | Mobile precisa modelo completamente diferente. |
| Screenshot | `GlobalInput::ScreenShot` | `NiRenderer::LockRenderer`, `SaveScreenShot` | Migracao depende do novo renderer. |

## Audio

| Area | Arquivo/funcoes | Dependencias | Comentario |
|---|---|---|---|
| Inicializacao | `DProject/CSoundMgr.cpp`, `_InitMiles` | `NiMilesAudioSystem`, `SetHWnd`, `Startup("Data\\NP\\Sound")` | Miles e Win32 HWND. |
| Atualizacao | `CSoundMgr::Update` | `NiAudioSystem::Update` | Precisa backend novo se abandonar Miles. |
| Assets audio | `Data\\Music`, `Data\\Sound`, `Data\\StaticSound`, `Data\\NP\\Sound` | Paths Windows e codecs Miles | Validar formatos reais. |
| Linkagem | `DProject/main.cpp`, pragma Miles | `NiMilesAudioSDM::Init` | Historicamente sensivel a linker/IAT. |

## Assets, Packs e File System

| Area | Arquivo/funcoes | Dependencias | Comentario |
|---|---|---|---|
| Pack files | `LibProj/CsFilePack/CsFilePack.cpp`, `CsFilePackSystem.cpp` | `HANDLE`, `_sopen_s`, `_lseeki64`, `_read`, `_write`, structs empacotadas | Portavel apenas com refatoracao cuidadosa. |
| Bridge Gamebryo | `DProject/App/GameApp.cpp`, `CsFilePackFileCreateFunc`, `CsFilePackFileAccessFunc` | `NiFile::SetFileCreateFunc`, `NiMemObject`, `NiFile` | Essencial para manter assets atuais. |
| File tables | `LibProj/CsFileTable/*` | `.bin`, Excel, pack/file modes | Alto risco de layout binario. |
| Modelos/animacao | `DProject/CharMng.cpp`, `LibProj/CsGBChar/*` | `.nif`, `.kfm`, `.kf`, Gamebryo stream | Bloqueador para renderer novo completo. |
| Texturas/UI | `DProject/_Interface/*`, `CharResMng.cpp`, `ResourceMng` | `.tga`, `.dds`, `.png`, atlas e paths Windows | Precisa inventario por pack. |

## UI

| Area | Evidencia | Risco |
|---|---|---|
| UI propria do cliente | `DProject/_Interface/*` | Muitos assets e estados customizados. |
| Win32 nativo misturado | `ItemSeparate.cpp`, `MoneySeparate.cpp`, `WebControl.cpp`, `CIme.cpp` | Alto para x64/cross-platform. |
| Renderizacao UI | Dependente de `NiRenderer`/Gamebryo | Renderer novo precisa camada de sprite/texto. |
| Texto/fontes | `CFont::GlobalInit`, `CreateFont`, libs `freetype213.lib` | Migracao possivel, mas exige validacao visual. |

## Networking

| Area | Arquivo/funcoes | Dependencias | Comentario |
|---|---|---|---|
| Loop de rede | `DProject/network/cNetwork.cpp`, `net::start`, `net::execute` | `nlib`, ASIO, singletons `cCli*` | Menor risco cross-platform que renderer/input. |
| Estado/frame | `CGameApp::OnIdleExtern` chama `net::execute()` | Rede depende do loop principal | Adaptavel a SDL loop. |
| Windows API | `SetWindowTextA`, `Sleep` | Pequenos acoplamentos | Substituiveis. |
| Protocolos | `common_vs2019`, `nlib/packet.cpp` | Layout manual, tipos fixos | Alto risco funcional se alterar. |

## Bibliotecas Externas Identificadas

| Biblioteca | Uso provavel | Situacao |
|---|---|---|
| `NiDX9Renderer.lib` | Renderer Gamebryo DX9 | Bloqueador x64/cross-platform se x86-only. |
| `NiD3D10Renderer.lib` | Renderer Gamebryo D3D10 referenciado | DESCONHECIDO se usado em runtime. |
| `NiInput.lib` | Input Gamebryo/DirectInput | Bloqueador para SDL/mobile. |
| `NiMilesAudio.lib` | Audio Gamebryo/Miles | Bloqueador audio. |
| `NiMain.lib`, `NiSystem.lib`, `NiAnimation.lib`, `NiParticle.lib`, `NiCollision.lib` | Motor Gamebryo | Bloqueador maior. |
| `mss32.lib`/`mss32.dll` | Miles Sound System | Provavelmente x86; confirmar. |
| `zlib.lib`, `libpng.lib`, `freetype213.lib` | Compressao/imagem/fonte | Substituiveis ou recompilaveis. |
| `d3d9.lib`, `d3dx9.lib`, `dinput8.lib`, `dsetup.lib`, `dxguid.lib` | DirectX legado | Windows-only. |
| `ws2_32.lib` | Sockets Windows | ASIO pode abstrair parte, mas link Windows permanece no cliente atual. |

## Classificacao Geral

| Subsistema | Portabilidade Windows x64 | Portabilidade desktop cross-platform | Portabilidade mobile |
|---|---:|---:|---:|
| Networking/protocolo | Media | Media | Media |
| Parsers/pack/file table | Media | Media/baixa | Baixa |
| Janela/input | Media/baixa | Baixa sem SDL | Baixa |
| Audio | Baixa | Baixa | Baixa |
| Renderer/Gamebryo | Baixa | Muito baixa | Muito baixa |
| UI | Baixa | Baixa | Muito baixa |
| Assets/modelos/animacao | Baixa | Muito baixa | Muito baixa |

## Itens DESCONHECIDO

- A arquitetura binaria exata de cada `.lib` nao foi validada neste inventario.
- Nao foi auditado o conteudo completo dos packs ativos.
- Nao foi medida a quantidade total de chamadas Win32/DX9 por subsistema.
- Nao foi validado se existem fontes ou SDKs x64 para Gamebryo/Miles.
