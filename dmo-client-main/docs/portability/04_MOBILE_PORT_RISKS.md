# Riscos de Portabilidade Mobile

Escopo: avaliar Android/iOS como alvo futuro. Este documento assume que o cliente atual e desktop Win32/DX9/Gamebryo.

## Portabilidade Mobile: bloqueadores e validacoes obrigatorias

Mobile nao e um alvo de curto prazo para o cliente atual. Antes de Android/iOS, o projeto precisa resolver:

- renderer moderno;
- janela/lifecycle por SDL3 ou camada propria;
- input touch;
- audio sem Miles/Win32;
- filesystem e asset packaging;
- performance/memoria;
- UI adaptada a telas e densidades diferentes;
- validacao legal/licenciamento de libs e assets.

## Bloqueadores Android

| Area | Risco | Comentario |
|---|---:|---|
| Renderer | Bloqueador | DX9/Gamebryo nao atende Android. Precisa Vulkan/OpenGL ES/Metal via backend moderno. |
| Gamebryo | Bloqueador | DESCONHECIDO se ha suporte Android/source adaptavel. Provavelmente nao direto. |
| Audio Miles | Bloqueador | `mss32.dll`/Miles e Windows. Precisa backend novo. |
| Janela Win32 | Bloqueador | `CreateWindowEx`, `WndProc`, `PeekMessage` nao existem no Android. |
| Input | Alto | Mouse/teclado/DirectInput precisam virar touch/gamepad/teclado virtual. |
| Assets | Alto | Packs, paths `Data\\...`, codecs e case sensitivity precisam revisao. |
| Threads/lifecycle | Alto | Pausa/resume, perda de contexto, background, memoria. |
| Rede | Medio | ASIO pode ajudar, mas lifecycle e reconexao precisam tratamento. |

## Bloqueadores iOS

| Area | Risco | Comentario |
|---|---:|---|
| Renderer | Bloqueador | iOS exige Metal ou camada que gere Metal. |
| Bibliotecas proprietarias | Bloqueador | Gamebryo/Miles x86/Windows nao atendem iOS. |
| JIT/dinamismo | Medio | Se houver plugins/dll dinamicas, iOS restringe. DESCONHECIDO no cliente atual. |
| Filesystem | Alto | Paths Windows e escrita livre em diretorios do app nao servem diretamente. |
| Input/UI | Alto | UI desktop nao e ergonomica em touch. |
| App lifecycle | Alto | Precisa lidar com background, interrupcoes, memoria e restore. |

## Riscos de Assets

| Item | Risco | Acao |
|---|---:|---|
| Paths com barra invertida | Medio | Normalizar paths (`Data\\...` -> path abstraction). |
| Case sensitivity | Alto em Android/Linux | Auditar nomes reais dos arquivos/packs. |
| DDS/DXT | Alto | Ver suporte por GPU/API; mobile pode exigir ASTC/ETC2 ou transcoding. |
| NIF/KFM/KF | Bloqueador | Sem Gamebryo, precisa loader/conversor/runtime novo. |
| Shaders gerados Gamebryo | Alto | Converter/recriar shaders. |
| Packs `.pf/.hf` | Medio/alto | Precisa leitura portavel e empacotamento adequado ao app. |
| Audio codecs Miles | Alto | Validar formato real; converter para OGG/WAV/AAC conforme plataforma. |

## Riscos de Input e UI

O cliente atual pressupoe:

- teclado;
- mouse;
- cursor;
- janela desktop;
- IME Win32;
- controles Win32 em algumas telas;
- resolucoes desktop;
- densidade de pixels previsivel.

Para mobile, a UI precisa no minimo:

- layout responsivo;
- escala por DPI;
- touch targets;
- teclado virtual;
- gestos;
- HUD adaptado;
- tratamento de safe areas;
- menus que funcionem sem mouse hover.

Isto e praticamente uma frente de produto, nao apenas engenharia.

## Riscos de Performance

| Area | Risco | Comentario |
|---|---:|---|
| Memoria | Alto | Assets de PC podem exceder limites mobile. |
| Fill rate/UI | Medio/alto | UI com muitos sprites/alpha pode pesar. |
| Particulas | Alto | Sistemas Gamebryo podem precisar reducao. |
| Terreno | Alto | Culling/LOD devem ser medidos. |
| Loading | Alto | Packs e carregamento bloqueante podem causar travamentos. |
| Bateria | Medio/alto | Loop desktop sempre ativo nao e adequado. |

## Validacoes Obrigatorias Antes de Mobile

1. Provar renderer moderno em desktop com bgfx ou Diligent.
2. Provar carregamento de textura real do cliente fora de Gamebryo.
3. Provar UI/sprite/texto em backend novo.
4. Provar audio com backend portavel.
5. Provar leitura dos packs em paths portaveis e case-sensitive.
6. Provar login/rede em loop SDL.
7. Provar Android com clear screen + textura.
8. Provar Android com uma tela de UI real.
9. Provar uma cena 3D simples.
10. So entao discutir cliente jogavel em mobile.

## Decisao

Mobile deve ser considerado **alvo estrategico de longo prazo**, nao milestone inicial. O melhor caminho e:

```text
Windows Win32 atual
  -> Windows x64/subsistemas portaveis
  -> SDL3 desktop
  -> renderer PoC bgfx/Diligent
  -> assets reais no renderer novo
  -> Android/iOS smoke tests
  -> UI/mobile gameplay
```

Qualquer tentativa de ir direto ao Android/iOS tende a esconder os mesmos problemas, so que com debug mais dificil.
