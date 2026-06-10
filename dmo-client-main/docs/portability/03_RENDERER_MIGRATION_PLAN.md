# Plano de Migracao de Renderer

Escopo: avaliar como sair do renderer legado DX9/Gamebryo para uma arquitetura que possa eventualmente usar bgfx ou Diligent. Este plano nao recomenda troca direta imediata.

## Estado Atual

O renderer atual nao e apenas uma camada grafica isolada. Ele esta misturado com Gamebryo, janela Win32, assets, cena, animacao, UI, cameras, screenshots e efeitos.

Pontos centrais:

- `DProject/Engine.cpp`
  - `CEngine::_CreateRender`
  - `CEngine::_CheckDeviceCaps`
  - `CEngine::ResetDevice`
  - `CEngine::SwitchFullScreen`
  - `CEngine::SwitchWindowMode`
- `DProject/Engine_Render.cpp`
  - `CEngine::StartRender`
  - `CEngine::EndRender`
  - `CEngine::Clear`
  - `CEngine::GetD3DDevice`
  - `CEngine::GetD3D`
- `DProject/Engine.h`
  - `CsGBDX9RendererPtr`
  - `NiDX9Renderer::FrameBufferFormat`
  - `D3DXMATRIX`
  - `D3DVIEWPORT9`

## Decisao Arquitetural

O renderer nao deve ser substituido em um unico passo. A migracao deve criar uma camada fina de fronteira, mantendo DX9/Gamebryo como backend inicial.

Modelo recomendado:

```text
Cliente atual
  -> EngineFacade / RenderFacade
    -> Backend legado Gamebryo DX9
    -> Backend experimental bgfx ou Diligent
```

A fachada deve nascer pequena. Se ela tentar modelar todo Gamebryo desde o inicio, vira uma reescrita disfarçada.

## Interfaces Minimas Propostas

Estas interfaces sao conceituais. Nao implementar em producao antes de uma PoC.

```cpp
struct RenderWindowDesc {
    void* nativeWindowHandle;
    int width;
    int height;
    bool fullscreen;
    bool vsync;
};

struct RenderFrameStats {
    uint32_t drawCalls;
    uint32_t triangles;
};

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;
    virtual bool initialize(const RenderWindowDesc& desc) = 0;
    virtual void shutdown() = 0;
    virtual bool resize(int width, int height) = 0;
    virtual void beginFrame() = 0;
    virtual void clear(float r, float g, float b, float a) = 0;
    virtual void endFrame() = 0;
    virtual RenderFrameStats stats() const = 0;
};
```

Para UI/sprites, uma segunda interface deve ser validada separadamente:

```cpp
class ISpriteRenderer {
public:
    virtual ~ISpriteRenderer() = default;
    virtual void drawTextureRect(uint32_t textureId, float x, float y, float w, float h) = 0;
    virtual void drawText(const char* text, float x, float y, uint32_t color) = 0;
};
```

## Diligent vs bgfx

| Criterio | Diligent | bgfx | Observacao no projeto |
|---|---|---|---|
| PoC simples | Media | Alta | bgfx tende a ser mais rapido para validar janela + textura + mesh. |
| Abstracao de APIs modernas | Alta | Alta | Ambos cobrem multiplas APIs; Diligent e mais explicito. |
| Modelo parecido com DX9 legado | Media | Media/alta | bgfx pode ser mais pragmatico para estados e draws simples. |
| Controle de shaders/pipelines | Alta | Media | Diligent favorece renderer redesenhado. |
| Mobile | Alta | Alta | Nenhum resolve assets/Gamebryo/input/audio sozinho. |
| Integracao incremental | Media | Media/alta | bgfx tende a ser melhor para experimento isolado. |
| Risco de curva inicial | Medio/alto | Medio | A equipe precisa dominar o backend escolhido. |

Recomendacao: **usar bgfx na primeira PoC de renderer**. Manter Diligent como alternativa se a PoC mostrar que a equipe precisa de mais controle explicito sobre pipelines ou se houver exigencia tecnica que bgfx nao cubra bem.

## Estrategias Possiveis

| Opcao | Descricao | Recomendacao |
|---|---|---|
| A | Criar interfaces e manter backend DX9 atual | Sim, como mecanismo de migracao. |
| B | Substituir DX9 por bgfx/Diligent no cliente inteiro | Nao agora. Muito risco. |
| C | Recriar renderer e asset pipeline em paralelo | Apenas depois de PoCs. |
| D | Preservar cliente DX9 e criar PoCs isoladas | Sim, primeira estrategia. |
| E | Portar direto para mobile | Nao agora. |

## Ordem Recomendada

### Fase 1: Medir o acoplamento real

Objetivo: saber quantas chamadas diretas a DX9/Gamebryo precisam de adaptador.

Arquivos principais:

- `DProject/Engine.cpp`
- `DProject/Engine_Render.cpp`
- `DProject/Engine.h`
- `LibProj/CsGamebryo2.3/*`
- `LibProj/CsGBChar/*`
- `DProject/_Interface/*`

Sucesso:

- Inventario de chamadas diretas a `IDirect3D*`, `D3DX*`, `NiDX9Renderer`, `NiRenderer`.

### Fase 2: Backend legado por fachada

Objetivo: criar uma camada minima que chama o renderer atual, sem mudar comportamento.

Sucesso:

- O cliente Win32/DX9 continua rodando igual.
- `StartRender`, `EndRender`, `Clear`, `Resize` passam por funcoes controladas.

### Fase 3: PoC bgfx isolada

Objetivo: abrir janela SDL3, inicializar bgfx, desenhar:

- clear color;
- textura simples;
- quad/sprite com alpha;
- mesh com depth;
- camera basica;
- captura de tela.

Sucesso:

- Desktop Windows x64 roda a PoC.
- Android fica como alvo posterior, nao obrigatorio nesta fase.

### Fase 4: PoC de assets

Objetivo: carregar um asset simples do pack ou de arquivo extraido e desenhar no backend novo.

Itens:

- textura `.dds` ou `.png`;
- quad de UI;
- fonte/texto;
- material basico.

Sucesso:

- Um elemento visual real do cliente renderiza fora de Gamebryo.

### Fase 5: UI/sprite bridge

Objetivo: escolher uma parte pequena da UI para backend novo.

Nao escolher tela complexa. Um bom candidato e:

- cursor;
- sprite estatico;
- overlay/debug;
- tela simples com textura e texto.

Sucesso:

- A UI selecionada renderiza igual ou aceitavelmente parecida no backend experimental.

### Fase 6: Cena 3D minima

Objetivo: carregar ou reconstruir um modelo simples.

Risco:

- `.nif`, `.kfm`, animacao, skinning e efeitos Gamebryo podem virar o maior bloco de trabalho.

Sucesso:

- Um modelo simples aparece com camera, material e textura.

## Migracao de Shaders

O cliente atual usa Gamebryo e diretorio `Data\\Shaders\\Generated`. A estrategia de shader deve ser validada por asset:

| Tipo | Risco | Comentario |
|---|---:|---|
| UI/sprite sem shader complexo | Baixo/medio | Melhor primeiro alvo. |
| Terreno | Alto | Provavel dependencia de material/effects. |
| Personagens | Alto | Skinning, alpha, attachments, efeitos. |
| Particulas | Alto | Gamebryo particle system. |
| Post effects | Medio/alto | Ver `m_pPostEffect`. |

## Riscos de Troca Direta

- Perder compatibilidade visual com assets existentes.
- Quebrar animacao/personagens.
- Quebrar UI por ordem de draw/blend/alpha.
- Quebrar screenshot, picking, camera e coordenadas 3D->2D.
- Quebrar device reset/fullscreen/windowed.
- Criar divergencia dificil de testar com o cliente de producao.

## Decisao Final

O primeiro renderer moderno deve ser **uma PoC fora do cliente principal**. O cliente principal deve ganhar apenas uma fronteira pequena e reversivel. Quando a PoC conseguir renderizar assets reais, a equipe pode decidir entre:

- aprofundar bgfx;
- trocar para Diligent;
- manter DX9 e focar x64;
- iniciar uma reescrita parcial de UI/renderer.
