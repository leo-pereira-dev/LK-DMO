# DProject GamebryoSharp DMO Needed Checklist

Objetivo: mapear as classes Gamebryo que o DMO Client C# precisa para sair dos placeholders e usar `D:\Gamebryo2.3\GamebryoSharp`.

Fonte do status: `D:\Gamebryo2.3\GamebryoSharp\PORTING_CHECKLIST.md`, lido em 2026-06-04.

## Status Geral

GamebryoSharp completo no SDK inteiro:

- C++ definitions mapped: 1885
- Complete: 288
- Partial: 15
- Pending: 1582

Recorte necessario para o DMO:

- Classes mapeadas neste checklist: 132
- Complete em GamebryoSharp: 89
- Partial em GamebryoSharp: 11
- Pending em GamebryoSharp: 29
- NotFound no checklist GamebryoSharp: 3
- Potencialmente utilizaveis agora com ponte/adaptador: 100
- Completas sem ressalva no recorte DMO: 67.42%
- Completas + parciais no recorte DMO: 75.76%
- Adaptadores/entradas ja integradas na copia `DProject.CSharp.Gamebryo`: 44
- Classes/tipos `Complete` do GamebryoSharp com wrapper nativo direto na copia: 41/89
- Classes/tipos `Complete` com wrapper estrutural aguardando renderer nativo: 1/89
- Classes/tipos `Complete` do recorte DMO ainda sem adaptador na copia: 47/89
- Classes/tipos `Partial`, `Pending` ou `NotFound` ainda em fallback/bloqueio: 43

## Legenda

- `[x]` GamebryoSharp marcado como `Complete`.
- `[ ]` Ainda exige ponte cautelosa, complemento no GamebryoSharp, ou placeholder DMO.
- `Partial`: existe classe C#, mas a propria checklist oficial declara lacunas.
- `Pending`: classe existe no checklist oficial, mas ainda nao foi portada.
- `NotFound`: usada/necessaria no DMO, mas nao aparece como item canonico no checklist atual.

## Bootstrap, Sistema E Memoria

- [x] `NiInitOptions` - Complete - bootstrap/allocator options.
- [x] `NiStaticDataManager` - Complete - ciclo de init/shutdown de static data managers.
- [ ] `NiSystemSDM` - NotFound - usar ponte para `NiStaticDataManager`/init system.
- [x] `NiMemManager` - Complete - memoria gerenciada/allocator.
- [x] `NiAllocator` - Complete - contrato base de allocator.
- [x] `NiStandardAllocator` - Complete - allocator padrao.
- [x] `NiMemObject` - Complete - base de objetos Gamebryo.

## Scene Graph, Objetos E Luzes

- [x] `NiRefObject` - Complete - ref-count/ownership.
- [ ] `NiObject` - Partial - clone/stream ainda depende de `NiStream` completo.
- [ ] `NiObjectNET` - Partial - nomes, extra-data e controllers parcialmente portados.
- [ ] `NiAVObject` - Partial - transform/properties/culling ainda incompleto.
- [ ] `NiNode` - Partial - children/update/culling com lacunas de property/effect state.
- [x] `NiCamera` - Complete - camera/frustum/projecao.
- [x] `NiLight` - Complete - base de luz.
- [x] `NiAmbientLight` - Complete - luz ambiente.
- [x] `NiDirectionalLight` - Complete - luz direcional.
- [x] `NiPointLight` - Complete - luz pontual.
- [x] `NiSpotLight` - Complete - luz spot.

## Renderer, Views E Culling

- [ ] `NiRenderer` - Partial - base existe, mas renderer real ainda nao e DX9 completo.
- [ ] `NiDX9Renderer` - Pending - essencial para equivalencia do cliente original.
- [ ] `NiDX9SystemDesc` - Pending - device/system description DX9.
- [ ] `NiDX9AdapterDesc` - Pending - adapter description DX9.
- [ ] `NiDX9DeviceDesc` - Pending - device description DX9.
- [x] `NiDx9RendererMetrics` - Complete - metricas DX9.
- [x] `NiRenderTargetGroup` - Complete - targets/backbuffers.
- [x] `Ni2DBuffer` - Complete - buffers 2D.
- [x] `NiDepthStencilBuffer` - Complete - depth/stencil.
- [x] `NiRenderStep` - Complete - etapa de render.
- [x] `NiRenderClick` - Complete - click de render.
- [x] `NiDefaultClickRenderStep` - Complete - render step default.
- [x] `NiRenderView` - Complete - view base.
- [x] `Ni3DRenderView` - Complete - view 3D/culling.
- [x] `Ni2DRenderView` - Complete - view 2D/screen.
- [x] `NiCullingProcess` - Complete - culling process.
- [x] `NiVisibleArray` - Complete - lista de objetos visiveis.
- [x] `NiFrustum` - Complete - volume de camera.
- [x] `NiFrustumPlanes` - Complete - planos de frustum.

## Math E Tipos Basicos

- [x] `NiPoint2` - Complete - vetor 2D.
- [x] `NiPoint3` - Complete - vetor 3D.
- [x] `NiColor` - Complete - cor RGB.
- [x] `NiColorA` - Complete - cor RGBA.
- [x] `NiMatrix3` - Complete - matriz 3x3.
- [x] `NiTransform` - Complete - transform completo.
- [x] `NiQuaternion` - Complete - rotacao quaternion.
- [x] `NiBound` - Complete - bounding sphere.
- [x] `NiPlane` - Complete - plano.

## Arquivos, Stream E Paths

- [x] `NiFile` - Complete - leitura/escrita de arquivo.
- [x] `NiMemFile` - Complete - arquivo em memoria.
- [x] `NiMemStream` - Complete - stream em memoria.
- [x] `NiBufferReader` - Complete - reader por buffer.
- [ ] `NiStream` - Partial - ainda falta loader registry/header/load completo para NIF real.
- [x] `NiBinaryStream` - Complete - stream binario/endian.
- [x] `NiSearchPath` - Complete - busca de paths.
- [x] `NiFilename` - Complete - manipulacao de filename.

## Texturas, Materiais E Render Properties

- [x] `NiTexture` - Complete - textura base.
- [ ] `NiSourceTexture` - Partial - filename/preload ok; conversao real de imagem/plataforma pendente.
- [x] `NiDynamicTexture` - Complete - textura dinamica.
- [x] `NiRenderedTexture` - Complete - render-to-texture.
- [x] `NiPixelFormat` - Complete - formatos de pixel.
- [x] `NiPixelData` - Complete - dados de pixel.
- [x] `NiPalette` - Complete - palette.
- [x] `NiTextureCodec` - Complete - codec DXT inicial.
- [x] `NiTexturePalette` - Complete - palette/cache de texturas.
- [x] `NiDefaultTexturePalette` - Complete - palette default.
- [x] `NiTextureTransform` - Complete - transform UV.
- [x] `NiTexturingProperty` - Complete - mapas de textura.
- [ ] `NiMaterial` - Pending - material pipeline high-level ainda nao portado.
- [x] `NiMaterialProperty` - Complete - cor/material property.
- [x] `NiProperty` - Complete - base de propriedade.
- [x] `NiPropertyState` - Complete - conjunto de propriedades.
- [x] `NiAlphaProperty` - Complete - alpha render state.
- [x] `NiZBufferProperty` - Complete - z-buffer render state.
- [x] `NiStencilProperty` - Complete - stencil render state.
- [x] `NiVertexColorProperty` - Complete - vertex color property.
- [x] `NiFogProperty` - Complete - fog property.
- [x] `NiRendererSpecificProperty` - Complete - propriedade especifica do renderer.

## Geometria, Terrain, UI 2D E Screen

- [ ] `NiGeometry` - Partial - bridge existe, mas integracao material/shader/skinning incompleta.
- [x] `NiGeometryData` - Complete - vertices/normais/UV/bounds.
- [x] `NiTriBasedGeom` - Complete - base para geometria triangular.
- [x] `NiTriShape` - Complete - geometria triangular.
- [x] `NiTriShapeData` - Complete - dados de trishape.
- [x] `NiTriStrips` - Complete - strips.
- [x] `NiTriStripsData` - Complete - dados de strips.
- [x] `NiLines` - Complete - linhas.
- [x] `NiLinesData` - Complete - dados de linhas.
- [x] `NiParticles` - Complete - particulas base.
- [x] `NiParticlesData` - Complete - dados de particulas.
- [x] `NiScreenElements` - Complete - elementos 2D.
- [x] `NiScreenElementsData` - Complete - dados de elementos 2D.
- [x] `NiScreenTexture` - Complete - textura de tela.
- [x] `NiScreenTextureRenderClick` - Complete - render click de screen texture.

## Shader E Material Graph

- [ ] `NiShader` - Pending - shader runtime ainda placeholder.
- [ ] `NiShaderFactory` - Pending - factory de shader.
- [ ] `NiShaderLibrary` - Pending - biblioteca de shader.
- [x] `NiShaderDesc` - Complete - descricao de shader.
- [x] `NiShaderDescBase` - Complete - base de descricao.
- [x] `NiShaderRequirementDesc` - Complete - requisitos de shader.
- [x] `NiShaderAttributeDesc` - Complete - atributos de shader.
- [ ] `NiShaderDeclaration` - Partial - enum/seed, declaracao completa pendente.
- [ ] `NiShaderConstantMap` - Pending - mapa de constantes.
- [ ] `NiShaderConstantMapEntry` - Pending - entrada de constante.
- [x] `NiGPUProgram` - Complete - programa GPU base.
- [x] `NiGPUProgramDescriptor` - Complete - descriptor GPU.
- [x] `NiMaterialDescriptor` - Complete - descriptor de material.
- [x] `NiMaterialResource` - Complete - recurso de material.
- [x] `NiMaterialNode` - Complete - node de material.
- [x] `NiMaterialNodeLibrary` - Complete - biblioteca de nodes.

## Animacao, Controllers E Skinning

- [ ] `NiTimeController` - Partial - base existe, interpolator stack pendente.
- [ ] `NiTransformController` - Partial - seed helper.
- [x] `NiTextKey` - Complete - text key.
- [x] `NiTextKeyExtraData` - Complete - text keys extra data.
- [ ] `NiActorManager` - Pending - actor animation manager.
- [ ] `NiControllerManager` - Pending - controller manager.
- [ ] `NiControllerSequence` - Pending - sequencias de animacao.
- [ ] `NiInterpolator` - Pending - interpolators base.
- [ ] `NiKFMTool` - Pending - KFM/tool de animacao.
- [ ] `NiSkinInstance` - Pending - skinning.
- [ ] `NiSkinData` - Pending - dados de skin.
- [ ] `NiSkinPartition` - Pending - particoes de skin.

## Input, Audio E App Framework

- [ ] `NiInputSystem` - Pending - input system.
- [ ] `NiInputKeyboard` - Pending - keyboard input.
- [ ] `NiInputMouse` - Pending - mouse input.
- [ ] `NiAction` - Pending - action mapping.
- [ ] `NiActionMap` - Pending - mapas de acao.
- [ ] `NiAudioSystem` - Pending - audio core.
- [ ] `NiAudioSource` - Pending - fonte de audio.
- [ ] `NiAudioListener` - Pending - listener de audio.
- [ ] `NiMilesAudioSystem` - Pending - necessario para equivalencia Miles do cliente.
- [ ] `NiMilesAudioSDM` - NotFound - existe no DMO original, nao aparece no checklist canonico.
- [ ] `NiMilesAudio` - NotFound - headers/libs DMO existem, mas nao esta portado no checklist.
- [ ] `NiApplication` - Pending - app framework Gamebryo.
- [ ] `NiAppWindow` - Pending - janela app framework.

## Prioridade De Implantacao No DMO

1. Ponte segura: math/core (`NiPoint2`, `NiPoint3`, `NiMatrix3`, `NiQuaternion`, `NiTransform`, `NiPlane`, `NiBound`, `NiFrustum`), cores, arquivo, textura, propriedades, camera e luzes.
2. Scene graph parcial: `NiRefObject`, `NiObject`, `NiObjectNET`, `NiAVObject`, `NiNode`.
3. Render estrutural: `NiRenderer` parcial + `NiRenderTargetGroup`, `NiRenderView`, `Ni3DRenderView`, `NiCullingProcess`.
4. Assets/mapa: `NiStream`, `NiSourceTexture`, `NiGeometry`, `NiTriShape`, `NiScreenElements`.
5. Pendencias bloqueantes para render oficial: `NiDX9Renderer`, `NiShader`, `NiShaderFactory`, `NiShaderLibrary`, `NiShaderConstantMap`, `NiSkinInstance`, `NiAudio/Miles`.

## Status Da Copia DMO Gamebryo

- Copia criada: `C:\dmo-client-main\DProject.CSharp.Gamebryo`
- Projeto da copia: `C:\dmo-client-main\DProject.CSharp.Gamebryo\DProject.CSharp.Gamebryo.csproj`
- Referencia adicionada: `D:\Gamebryo2.3\GamebryoSharp\src\GamebryoSharp.Core\GamebryoSharp.Core.csproj`
- Build da copia com a referencia: passou em `Release_English` com 0 warnings e 0 errors.
- Integracao atual: `DProject.Gamebryo` agora usa adaptadores para classes `Complete` do GamebryoSharp onde isso nao depende de render real.
- Ultima validacao: `dotnet build` e `dotnet run` da copia passaram em `Release_English` com 0 warnings e 0 errors em 2026-06-04.
- Lote atual integrado: paths/search, buffers 2D/depth, visible array, culling process, render views 2D/3D e render step default.

## Integradas Na Copia DMO

- [x] `NiRuntime` -> chama `GamebryoSharp.NiStaticDataManager.Init/Shutdown` com `NiInitOptions`.
- [x] `NiInitOptions` - usado no bootstrap nativo.
- [x] `NiStaticDataManager` - usado no bootstrap/shutdown nativo.
- [x] `NiMemManager` - inicializado via `NiSystemSDM` do GamebryoSharp.
- [x] `NiRefObject` - wrapper DMO encaminha ref-count para `GamebryoSharp.NiRefObject` quando ha objeto nativo.
- [x] `NiPoint2` - conversao e operacoes passam por `GamebryoSharp.NiPoint2`.
- [x] `NiPoint3` - conversao e operacoes passam por `GamebryoSharp.NiPoint3`.
- [x] `NiColor` - conversao para `GamebryoSharp.NiColor`.
- [x] `NiColorA` - conversao para `GamebryoSharp.NiColorA`/`NiColor`.
- [x] `NiMatrix3` - wrapper para matriz 3x3 nativa.
- [x] `NiQuaternion` - wrapper para quaternion nativo.
- [x] `NiTransform` - wrapper para rotate/translate/scale nativo.
- [x] `NiPlane` - wrapper para plano nativo.
- [x] `NiBound` - wrapper para bounding sphere nativo.
- [x] `NiFrustum` - wrapper para frustum nativo.
- [x] `NiFile` - wrapper DMO cria `GamebryoSharp.NiFile` real.
- [x] `NiFilename` - wrapper DMO cria `GamebryoSharp.NiFilename` real para split/make path.
- [x] `NiSearchPath` - wrapper DMO cria `GamebryoSharp.NiSearchPath` real para sequencia de busca.
- [x] `NiTexture` - flags globais e `FormatPrefs` mapeiam para `GamebryoSharp.NiTexture`.
- [x] `NiTexturingProperty` - wrapper DMO cria `GamebryoSharp.NiTexturingProperty` real.
- [x] `NiMaterialProperty` - wrapper DMO cria `GamebryoSharp.NiMaterialProperty` real.
- [x] `NiCamera` - wrapper DMO cria `GamebryoSharp.NiCamera` real e traduz frustum basico.
- [x] `NiLight` - wrapper DMO cria `GamebryoSharp.NiDirectionalLight`/`NiLight` real para cor e dimmer.
- [x] `NiAmbientLight` - wrapper cria `GamebryoSharp.NiAmbientLight` real.
- [x] `NiDirectionalLight` - wrapper cria `GamebryoSharp.NiDirectionalLight` real.
- [x] `NiPointLight` - wrapper cria `GamebryoSharp.NiPointLight` real com atenuacao.
- [x] `NiSpotLight` - wrapper cria `GamebryoSharp.NiSpotLight` real com angulos/expoente.
- [x] `NiProperty` - base local agora guarda `GamebryoSharp.NiProperty` nativo.
- [x] `NiPropertyState` - wrapper cria `GamebryoSharp.NiPropertyState` real e encaminha propriedades.
- [x] `NiAlphaProperty` - wrapper cria `GamebryoSharp.NiAlphaProperty` real.
- [x] `NiZBufferProperty` - wrapper cria `GamebryoSharp.NiZBufferProperty` real.
- [x] `NiStencilProperty` - wrapper cria `GamebryoSharp.NiStencilProperty` real.
- [x] `NiVertexColorProperty` - wrapper cria `GamebryoSharp.NiVertexColorProperty` real.
- [x] `NiFogProperty` - wrapper cria `GamebryoSharp.NiFogProperty` real.
- [x] `NiRendererSpecificProperty` - wrapper cria `GamebryoSharp.NiRendererSpecificProperty` real.
- [x] `Ni2DBuffer` - wrapper cria `GamebryoSharp.Ni2DBuffer` real e preserva dimensoes.
- [x] `NiDepthStencilBuffer` - wrapper cria `GamebryoSharp.NiDepthStencilBuffer` real sem renderer data ate o device existir.
- [x] `NiRenderTargetGroup` - wrapper estrutural DMO para anexar buffers; target nativo ainda depende de `NiRenderer` real.
- [x] `NiVisibleArray` - wrapper cria `GamebryoSharp.NiVisibleArray` real e mantem lista local de visiveis.
- [x] `NiCullingProcess` - wrapper cria `GamebryoSharp.NiCullingProcess` real e faz fallback para visible set local.
- [x] `NiRenderView` - base local aponta para `GamebryoSharp.NiRenderView` via views concretas.
- [x] `Ni2DRenderView` - wrapper cria `GamebryoSharp.Ni2DRenderView` real.
- [x] `Ni3DRenderView` - wrapper cria `GamebryoSharp.Ni3DRenderView` real com camera/culling opcionais.
- [x] `NiDefaultClickRenderStep` - wrapper cria `GamebryoSharp.NiDefaultClickRenderStep` real.

## Mantidas Como Placeholder/Fallback

- [ ] `NiRenderer` - `Partial` no GamebryoSharp; wrapper DMO continua placeholder ate renderer real.
- [ ] `NiDX9Renderer` - `Pending`; bloqueia equivalencia de render oficial.
- [ ] `NiSourceTexture` - `Partial`; wrapper DMO mantem caminho/prefs local sem instanciar o source texture nativo ainda.
- [ ] `NiAVObject` - `Partial`; usado como base local, com nativo apenas quando classe completa como `NiCamera`/`NiLight` exige.
- [ ] `NiNode` - `Partial`; mantido como lista local de filhos.
- [ ] `NiObject`/`NiObjectNET` - `Partial`; usado apenas como base de ponte.
- [ ] `NiShader`, `NiShaderFactory`, `NiShaderLibrary`, `NiShaderConstantMap` - `Pending`; continuam placeholders.
- [ ] `NiAudio/Miles` - `Pending/NotFound`; audio DMO continua placeholder.
