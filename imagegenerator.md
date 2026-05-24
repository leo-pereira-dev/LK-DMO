# Image Generator via ComfyUI MCP

Este projeto esta preparado para usar o ComfyUI local via MCP no Codex para gerar imagens, mockups de UI, assets de jogo, botoes, frames, backgrounds e variacoes visuais.

## Status atual

- ComfyUI instalado em: `C:\AI\ComfyUI_windows_portable`
- ComfyUI roda localmente em: `http://127.0.0.1:8188`
- URL de status: `http://127.0.0.1:8188/system_stats`
- MCP ativo no Codex: `comfyui_easy`
- Config do Codex: `C:\Users\leo_h\.codex\config.toml`
- Outputs: `C:\AI\ComfyUI_Outputs`
- Workflows API: `C:\AI\ComfyUI_Workflows\workflows-api`
- Workflows UI: `C:\AI\ComfyUI_Workflows\workflows-ui`
- Modelos/checkpoints: `C:\AI\Models\checkpoints`

Observacao: no momento nao ha checkpoint/modelo instalado. O MCP consegue consultar status, listar ferramentas e listar modelos, mas geracao real precisa de pelo menos um checkpoint em `C:\AI\Models\checkpoints`.

## Como iniciar

Antes de pedir imagens ao Codex, inicie o ComfyUI:

```bat
C:\AI\start_comfyui_codex_cpu.bat
```

Depois confirme:

```text
http://127.0.0.1:8188/system_stats
```

Se responder, o ComfyUI esta ativo.

## Como usar no Codex

Exemplos de pedidos:

```text
Use o MCP comfyui_easy e verifique o status do ComfyUI local.
```

```text
Use o MCP comfyui_easy e liste os modelos/checkpoints disponiveis no ComfyUI.
```

```text
Use o MCP comfyui_easy para listar workflows salvos em C:\AI\ComfyUI_Workflows\workflows-api.
```

```text
Use o MCP comfyui_easy para listar os nos disponiveis no ComfyUI.
```

## Exemplos de geracao

Use estes exemplos depois que houver um checkpoint instalado.

### Painel MMORPG futurista

```text
Use o MCP comfyui_easy para gerar uma imagem com ComfyUI.

Prompt:
high quality futuristic MMORPG blue neon game UI panel, dark navy interface, cyan glowing borders, beveled sci-fi buttons, clean empty frame, no text

Config:
1024x768, 25 steps, CFG 6, seed aleatoria

Salvar em:
C:\AI\ComfyUI_Outputs
```

### Botoes de jogo

```text
Use o MCP comfyui_easy para gerar 4 variacoes de botao MMORPG futurista azul/ciano, sem texto, bordas neon, acabamento metalico escuro, fundo transparente se possivel.
```

### Frame para janela de UI

```text
Use o MCP comfyui_easy para gerar um frame vazio para janela de MMORPG sci-fi, bordas ciano neon, cantos chanfrados, fundo interno transparente se possivel, sem texto.
```

### Mockup de inventario

```text
Use o MCP comfyui_easy para gerar um mockup de inventario MMORPG estilo sci-fi/Digimon, painel escuro, slots vazios, bordas ciano, detalhes futuristas, sem textos legiveis.
```

### Background de tela

```text
Use o MCP comfyui_easy para gerar um background futurista para tela de login MMORPG, tons azul/ciano, energia digital, atmosfera sci-fi, sem personagens, sem texto.
```

## Estilo recomendado

Para UI de jogo/MMORPG, usar termos como:

```text
high quality, futuristic MMORPG game UI, dark navy interface, cyan neon glow, beveled sci-fi panels, metallic edges, clean empty frame, no text, no logo, game asset, transparent background if possible
```

Para evitar texto ruim nas imagens:

```text
no text, no letters, no numbers, no logo, no watermark, no readable typography
```

## Ferramentas MCP detectadas

O MCP `comfyui_easy` foi testado com 31 ferramentas:

```text
get_system_stats, get_queue_status, get_history, cancel_current, clear_queue,
list_nodes, get_node_info, list_models, list_model_folders, list_embeddings,
list_extensions, refresh_nodes, search_nodes, list_workflows, load_workflow,
save_workflow, create_workflow, generate_workflow_name, add_node, remove_node,
update_node_input, get_workflow_template, list_templates, validate_workflow,
convert_workflow_to_ui, run_workflow, execute_workflow, generate_image,
submit_workflow, get_prompt_status, get_result_image
```

## Pendencias

1. Reiniciar o Codex depois de alterar `C:\Users\leo_h\.codex\config.toml`.
2. Instalar pelo menos um checkpoint em `C:\AI\Models\checkpoints`.
3. Reiniciar o ComfyUI apos adicionar modelos.
4. Nao baixar checkpoints grandes sem confirmacao previa.

## Documentacao completa

Mais detalhes da instalacao local estao em:

```text
C:\AI\COMFYUI_CODEX_SETUP.md
```
