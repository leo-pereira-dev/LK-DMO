# DetailInfo Function Chain Map

This sweep maps the official GDMO `DetailInfo` window shown in the screenshots:

- main `Detalhes` shell with `ON/OFF`, close, and `Tamer` / `Digimon` tabs;
- `Tamer` tab with equipment, equipped achievement, Seal Master button, and obtained attributes;
- `Digimon` tab with Digimon info, skills, enchant data, and breakthrough/advance data;
- popup/detail panels opened by equipment, Seal Master, zoom, and additional stat buttons.

## Primary Entry Points

| Layer | Address | Copied file | Role |
| --- | --- | --- | --- |
| Status UI factory | `0128bab0` | `decompiled/StatusUI/CreateTamerStatusUI.original.c` | Registers/creates `TamerStatusUI`; related to the older/local status screen. |
| Status UI factory | `0128bcc0` | `decompiled/StatusUI/CreateDigimonStatusUI.original.c` | Registers/creates `DigimonStatusUI`; related to Digimon status helper views. |
| Status UI factory | `0128bd60` | `decompiled/StatusUI/CreateDigimonStatusHelpUI.original.c` | Registers/creates `DigimonStatusUI_Help`. |
| Main UI tabs | `011d8320` | `decompiled/DetailInfoMainUI/BuildLocalDetailInfoTabButtons.original.c` | Builds the local player's `Tamer` / `Digimon` radio tabs. |
| Main UI tabs | `011d9440` | `decompiled/DetailInfoMainUI/BuildOtherDetailInfoTabButtons.original.c` | Builds another player's `Tamer` / `Digimon` radio tabs. |
| Tamer tab root | `011daec0` | `decompiled/DetailInfoUI/BuildTamerDetailRoot.original.c` | Builds the Tamer tab visual root. |
| Digimon tab root | `011d2a20` | `decompiled/DetailInfoUI/BuildDigimonDetailRoot.original.c` | Builds the Digimon tab visual root. |

## Main Window Chain

The official layout has two very similar main UI chains:

| Scope | Build tabs | Create tab contents | Tab switch handler | Event binder |
| --- | --- | --- | --- | --- |
| Local player | `011d8320` | `011d86e0` | `011d8e30` | `011d8ff0` |
| Other player | `011d9440` | `011d9800` | `011d9b80` | `011d9ce0` |

Observed behavior:

- both builders create two radio buttons using `System\\Btn_Radio1.tga`;
- tab label strings are `COMMON_TXT_TAMER` and `COMMON_TXT_DIGIMON`;
- `011d86e0` / `011d9800` allocate and attach the two subcontents:
  - `011dab40` constructs the Tamer sub UI;
  - `011d2710` constructs the Digimon sub UI;
- the tab change handlers update the pressed/selected state and show/hide the matching content.

## Digimon Tab Chain

Root: `011d2a20` -> `BuildDigimonDetailRoot`.

| Step | Address | Copied file | Screenshot area |
| --- | --- | --- | --- |
| Construct sub UI | `011d2710` | `ConstructDetailInfoDigimonSubUI.original.c` | Digimon tab object setup. |
| Root/frame | `011d2a20` | `BuildDigimonDetailRoot.original.c` | Main Digimon frame. |
| Portrait/name/level | `011d2b30` | `BuildDigimonInfoPanel.original.c` | Round portrait, name, level. |
| Info box | `011d30b0` | `BuildDigimonInfoBox.original.c` | "Informacao Digimon" box. |
| Data table | `011d3690` | `RenderDigimonDataSection.original.c` | Right-side Digimon attributes. |
| Data stat labels | `011d5940` | `ResolveDigimonDataStatText.original.c` | HP/DS/AT/AS/CT/HT/DE/BL/EV labels. |
| Skill slots | `011d3b00` | `BuildDigimonSkillSection.original.c` | Habilidade slots. |
| Enchant panel | `011d4320` | `BuildDigimonEnchantPanel.original.c` | Encanto Digimon section. |
| Advance panel | `011d4bd0` | `BuildDigimonAdvancePanel.original.c` | Avanco de Digimon section. |
| Value render helper | `011d5350` | `RenderDigimonEnchantOrAdvanceValue.original.c` | Enchant/advance numeric values. |
| Percent render helper | `011d5590` | `RenderDigimonPercentValue.original.c` | Percent-style values. |
| Zoom/help handler | `011d5d40` | `HandleDigimonZoomButton.original.c` | `zoom_in_aa.tga` button. |
| Zoom/help binder | `011d5e00` | `BindDigimonZoomButton.original.c` | Button event binding. |
| Finalize/events | `011d50f0` | `FinalizeDigimonDetailPanel.original.c` | Wires additional stat popup from Digimon tab. |

## Tamer Tab Chain

Root: `011daec0` -> `BuildTamerDetailRoot`.

| Step | Address | Copied file | Screenshot area |
| --- | --- | --- | --- |
| Construct sub UI | `011dab40` | `ConstructDetailInfoTamerSubUI.original.c` | Tamer tab object setup. |
| Root/frame | `011daec0` | `BuildTamerDetailRoot.original.c` | Main Tamer frame. |
| Prepare controls | `011daf90` | `PrepareTamerDetailControls.original.c` | Wires equipment and Seal Master popups. |
| Equipment slots | `011db440` | `BuildTamerEquipmentSection.original.c` | Left/right equipped items. |
| Equipment button binder | `011dd5b0` | `BindTamerEquipmentButton.original.c` | `Equipamento` button. |
| Equipment button handler | `011dd560` | `HandleTamerEquipmentButton.original.c` | Opens equipment popup. |
| Attribute panel | `011db840` | `BuildTamerAttributePanel.original.c` | Right panel shell. |
| Attribute values | `011dcbe0` | `RenderTamerAttributeValue.original.c` | Numeric obtained attribute values. |
| Obtained stat table | `011dbca0` | `BuildTamerObtainedAttributes.original.c` | `Atributos Obtidos` grid. |
| Obtained stat label helper | `011dd050` | `ResolveDetailInfoStatText.original.c` | Label mapping for HP/DS/AT/... |
| Obtained stat value helper | `011dd1d0` | `ResolveDetailInfoStatValue.original.c` | Value formatting. |
| DS split case | `011dd0a6` | `ResolveDetailInfoDsTextCase.original.c` | Split decompiler case for DS. |
| Preview field | `011dc210` | `BuildTamerDigimonPreviewSection.original.c` | Round tamer portrait / preview. |
| Achievement | `011dc7f0` | `BuildEquippedAchievementSection.original.c` | `Conquista Equipada`. |
| Seal Master handler | `011dd770` | `HandleSealMasterButton.original.c` | `Selo Mestre` button. |

## Popup Chains

### Equipment Popup

Root: `011d61d0` -> `BuildEquipmentPanel`.

- `011d62e0` prepares popup data.
- `011d65d0` builds the scroll/detail list.
- `011d6580` and `011d7de0` bind/handle popup close.
- `011d7cf0` renders each equipment value row.

### Additional Stat Popup

Root: `011d12b0` -> `BuildAdditionalStatPanel`.

- `011d0110` refreshes the internal stat list.
- `011d0390` builds each stat row.
- `011d14a0` binds popup events.
- `011d14f0` builds popup tabs.
- `011d1690`, `011d17c0`, and `011d1890` refresh/switch the row contents.

### Seal Master Detail Popup

Root: `011da000` -> `BuildSealMasterPanel`.

- `011da100` builds the close button.
- `011da150` builds stat rows.
- `011da4b0` maps stat labels.
- `011da5e0` formats stat values.
- `011da8e0` binds close behavior.

## Generated Sweep Reports

- `DeepSweep_DetailInfoCandidateFunctions.csv`: 125 nearby/dependent functions scanned from `unpacked_exe_all/functions`.
- `DeepSweep_DetailInfoCallEdges.csv`: 85 intra-family call edges detected from copied/decompiled bodies.
- `DeepSweepCopiedFunctionFiles.csv`: 39 additional files copied during this deeper pass.

Rows marked `adjacent-or-generic` in the candidate report are intentionally not
copied as core DetailInfo code unless they are later proven by a stronger xref.
Several `011dd***` and later functions belong to Union/Digi event UI neighbors,
not this `Detalhes` window, despite living near the same address block.
