# Texture Pack Pitfalls

Notas vindas dos bugs encontrados no port do `Inventario Exclusivo`.

## Regra de path

- As strings decompiladas normalmente aparecem relativas a `Data\Interface`, por exemplo `Control_G\Popup\N2Dlg5.tga`.
- `cSprite::Init` monta o path final com o working folder da UI: `Data\Interface\<asset>`.
- `cSprite::ChangeTexture` tambem usa o working folder por padrao. So use `bUseWorkingFolder=false` quando o path ja vier completo ou for um recurso fora de `Data\Interface`.
- Se trocar textura com `ChangeTexture("Control_G\\Popup\\N2Dlg5.tga", false)`, o cliente procura `Control_G\Popup\N2Dlg5.tga` fora do pack de interface e a sprite pode ficar rosa/magenta.

## Pack01

- As texturas de interface mapeadas ficam em `ClientDist\Data\Pack01.pf` com hash em `ClientDist\Data\Pack01.hf`.
- O `Pack01.pf` e muito grande; para validar existencia, prefira checar hash no `.hf` ou usar ferramenta propria de pack, em vez de carregar o `.pf` inteiro.
- O hash do pack e case-insensitive e ignora `.` e `\`, entao diferencas como `Sealmaster_highlight.tga` e `sealmaster_highlight.tga` podem resolver para o mesmo hash. Mesmo assim, mantenha o path exatamente como veio do decompilado no manifesto.

## Checklist antes de portar

- Confirme se o asset esta no manifesto oficial de xrefs/decompilado.
- Confirme se o asset existe no hash do Pack01 com prefixo `Data\Interface`.
- Ao criar sprite nova, use path relativo oficial e deixe o working folder ativo.
- Ao trocar textura em sprite existente, nao desligue o working folder sem motivo.
- Se aparecer rosa/magenta, primeiro verifique o path final resolvido, depois o hash do pack.

## Slot vazio de selo

- O slot vazio correto da aba de selo deve reaproveitar o mesmo atlas do Seal Master:
  `SealMaster\Sealmaster_bg_00.tga`.
- Nao renderize o atlas inteiro no tamanho do slot pequeno. O Seal Master usa blocos
  `102x173`; no Inventario Exclusivo precisa recortar somente a area interna do card.
- O recorte aplicado no port atual e `CsRect( 8, 24, 94, 152 )`, renderizado em
  `47x69`. Isso evita o bug visual onde a textura aparece pequena e repetida.
