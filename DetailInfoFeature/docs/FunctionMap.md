# Function Map

| Address | Original | Clean name | Copied file |
| --- | --- | --- | --- |
| `0128bab0` | `FUN_0128bab0` | `CreateTamerStatusUI` | `decompiled/StatusUI/CreateTamerStatusUI.original.c` |
| `0128bcc0` | `FUN_0128bcc0` | `CreateDigimonStatusUI` | `decompiled/StatusUI/CreateDigimonStatusUI.original.c` |
| `0128bd60` | `FUN_0128bd60` | `CreateDigimonStatusHelpUI` | `decompiled/StatusUI/CreateDigimonStatusHelpUI.original.c` |
| `011d12b0` | `FUN_011d12b0` | `BuildAdditionalStatPanel` | `decompiled/DetailInfoUI/BuildAdditionalStatPanel.original.c` |
| `011d2a20` | `FUN_011d2a20` | `BuildDigimonDetailRoot` | `decompiled/DetailInfoUI/BuildDigimonDetailRoot.original.c` |
| `011d2b30` | `FUN_011d2b30` | `BuildDigimonInfoPanel` | `decompiled/DetailInfoUI/BuildDigimonInfoPanel.original.c` |
| `011d30b0` | `FUN_011d30b0` | `BuildDigimonInfoBox` | `decompiled/DetailInfoUI/BuildDigimonInfoBox.original.c` |
| `011d3690` | `FUN_011d3690` | `RenderDigimonDataSection` | `decompiled/DetailInfoUI/RenderDigimonDataSection.original.c` |
| `011d3b00` | `FUN_011d3b00` | `BuildDigimonSkillSection` | `decompiled/DetailInfoUI/BuildDigimonSkillSection.original.c` |
| `011d4320` | `FUN_011d4320` | `BuildDigimonEnchantPanel` | `decompiled/DetailInfoUI/BuildDigimonEnchantPanel.original.c` |
| `011d4bd0` | `FUN_011d4bd0` | `BuildDigimonAdvancePanel` | `decompiled/DetailInfoUI/BuildDigimonAdvancePanel.original.c` |
| `011d50f0` | `FUN_011d50f0` | `FinalizeDigimonDetailPanel` | `decompiled/DetailInfoUI/FinalizeDigimonDetailPanel.original.c` |
| `011d61d0` | `FUN_011d61d0` | `BuildEquipmentPanel` | `decompiled/DetailInfoUI/BuildEquipmentPanel.original.c` |
| `011d65d0` | `FUN_011d65d0` | `BuildEquipmentDetailList` | `decompiled/DetailInfoUI/BuildEquipmentDetailList.original.c` |
| `011d82a0` | `FUN_011d82a0` | `BuildVisibilityToggleButton` | `decompiled/DetailInfoUI/BuildVisibilityToggleButton.original.c` |
| `011da000` | `FUN_011da000` | `BuildSealMasterPanel` | `decompiled/DetailInfoUI/BuildSealMasterPanel.original.c` |
| `011da100` | `FUN_011da100` | `BuildSealMasterCloseButton` | `decompiled/DetailInfoUI/BuildSealMasterCloseButton.original.c` |
| `011da150` | `FUN_011da150` | `BuildSealMasterStatRows` | `decompiled/DetailInfoUI/BuildSealMasterStatRows.original.c` |
| `011da4b0` | `FUN_011da4b0` | `ResolveSealMasterStatText` | `decompiled/DetailInfoUI/ResolveSealMasterStatText.original.c` |
| `011da506` | `switchD_011da4c0::caseD_1` | `ResolveSealMasterDsTextCase` | `decompiled/DetailInfoUI/ResolveSealMasterDsTextCase.original.c` |
| `011da5e0` | `FUN_011da5e0` | `ResolveSealMasterStatValue` | `decompiled/DetailInfoUI/ResolveSealMasterStatValue.original.c` |
| `011da8e0` | `FUN_011da8e0` | `BindSealMasterCloseButton` | `decompiled/DetailInfoUI/BindSealMasterCloseButton.original.c` |
| `011daec0` | `FUN_011daec0` | `BuildTamerDetailRoot` | `decompiled/DetailInfoUI/BuildTamerDetailRoot.original.c` |
| `011daf90` | `FUN_011daf90` | `PrepareTamerDetailControls` | `decompiled/DetailInfoUI/PrepareTamerDetailControls.original.c` |
| `011db440` | `FUN_011db440` | `BuildTamerEquipmentSection` | `decompiled/DetailInfoUI/BuildTamerEquipmentSection.original.c` |
| `011db840` | `FUN_011db840` | `BuildTamerAttributePanel` | `decompiled/DetailInfoUI/BuildTamerAttributePanel.original.c` |
| `011dbca0` | `FUN_011dbca0` | `BuildTamerObtainedAttributes` | `decompiled/DetailInfoUI/BuildTamerObtainedAttributes.original.c` |
| `011dc210` | `FUN_011dc210` | `BuildTamerDigimonPreviewSection` | `decompiled/DetailInfoUI/BuildTamerDigimonPreviewSection.original.c` |
| `011dc7f0` | `FUN_011dc7f0` | `BuildEquippedAchievementSection` | `decompiled/DetailInfoUI/BuildEquippedAchievementSection.original.c` |
| `011dd050` | `FUN_011dd050` | `ResolveDetailInfoStatText` | `decompiled/DetailInfoUI/ResolveDetailInfoStatText.original.c` |
| `011ed0a0` | `FUN_011ed0a0` | `BuildSharedDetailRewardPanel` | `decompiled/Shared/BuildSharedDetailRewardPanel.original.c` |

`docs/CopiedFunctionFiles.csv` contem o mapa inicial em formato CSV gerado.
`docs/DeepSweepCopiedFunctionFiles.csv` contem as funcoes adicionadas pela
varredura profunda.

## Deep Sweep Additions

Uma segunda varredura focada na cadeia de chamadas `011d****` adicionou 39
funcoes oficiais que nao estavam no mapa inicial, mas sao chamadas direta ou
indiretamente pela janela das prints.

| Address | Clean name | Copied file |
| --- | --- | --- |
| `011d0110` | `RefreshAdditionalStatList` | `decompiled/DetailInfoAdditionalStat/RefreshAdditionalStatList.original.c` |
| `011d0390` | `BuildAdditionalStatListRow` | `decompiled/DetailInfoAdditionalStat/BuildAdditionalStatListRow.original.c` |
| `011d14a0` | `BindAdditionalStatPopupEvents` | `decompiled/DetailInfoAdditionalStat/BindAdditionalStatPopupEvents.original.c` |
| `011d14f0` | `BuildAdditionalStatTabButtons` | `decompiled/DetailInfoAdditionalStat/BuildAdditionalStatTabButtons.original.c` |
| `011d1690` | `HandleAdditionalStatTabChange` | `decompiled/DetailInfoAdditionalStat/HandleAdditionalStatTabChange.original.c` |
| `011d17c0` | `RefreshAdditionalStatPopup` | `decompiled/DetailInfoAdditionalStat/RefreshAdditionalStatPopup.original.c` |
| `011d1890` | `BuildAdditionalStatAttributeRows` | `decompiled/DetailInfoAdditionalStat/BuildAdditionalStatAttributeRows.original.c` |
| `011d2710` | `ConstructDetailInfoDigimonSubUI` | `decompiled/DetailInfoUI/ConstructDetailInfoDigimonSubUI.original.c` |
| `011d28b0` | `RenderDetailInfoDigimonSubUI` | `decompiled/DetailInfoUI/RenderDetailInfoDigimonSubUI.original.c` |
| `011d5350` | `RenderDigimonEnchantOrAdvanceValue` | `decompiled/DetailInfoUI/RenderDigimonEnchantOrAdvanceValue.original.c` |
| `011d5590` | `RenderDigimonPercentValue` | `decompiled/DetailInfoUI/RenderDigimonPercentValue.original.c` |
| `011d5940` | `ResolveDigimonDataStatText` | `decompiled/DetailInfoUI/ResolveDigimonDataStatText.original.c` |
| `011d5d40` | `HandleDigimonZoomButton` | `decompiled/DetailInfoUI/HandleDigimonZoomButton.original.c` |
| `011d5e00` | `BindDigimonZoomButton` | `decompiled/DetailInfoUI/BindDigimonZoomButton.original.c` |
| `011d62e0` | `PrepareEquipmentPopupData` | `decompiled/DetailInfoUI/PrepareEquipmentPopupData.original.c` |
| `011d6580` | `BindEquipmentPopupCloseButton` | `decompiled/DetailInfoUI/BindEquipmentPopupCloseButton.original.c` |
| `011d7cf0` | `RenderEquipmentDetailValue` | `decompiled/DetailInfoUI/RenderEquipmentDetailValue.original.c` |
| `011d7de0` | `BindEquipmentPopupCloseButtonEvent` | `decompiled/DetailInfoUI/BindEquipmentPopupCloseButtonEvent.original.c` |
| `011d8250` | `SetVisibilityToggleState` | `decompiled/DetailInfoMainUI/SetVisibilityToggleState.original.c` |
| `011d8320` | `BuildLocalDetailInfoTabButtons` | `decompiled/DetailInfoMainUI/BuildLocalDetailInfoTabButtons.original.c` |
| `011d86e0` | `CreateLocalDetailInfoTabContents` | `decompiled/DetailInfoMainUI/CreateLocalDetailInfoTabContents.original.c` |
| `011d8c20` | `RefreshLocalDetailInfoMainUI` | `decompiled/DetailInfoMainUI/RefreshLocalDetailInfoMainUI.original.c` |
| `011d8e30` | `HandleLocalDetailInfoTabChange` | `decompiled/DetailInfoMainUI/HandleLocalDetailInfoTabChange.original.c` |
| `011d8ff0` | `BindLocalDetailInfoButtonEvent` | `decompiled/DetailInfoMainUI/BindLocalDetailInfoButtonEvent.original.c` |
| `011d93f0` | `SetOtherDetailInfoVisibilityToggleState` | `decompiled/DetailInfoMainUI/SetOtherDetailInfoVisibilityToggleState.original.c` |
| `011d9440` | `BuildOtherDetailInfoTabButtons` | `decompiled/DetailInfoMainUI/BuildOtherDetailInfoTabButtons.original.c` |
| `011d9800` | `CreateOtherDetailInfoTabContents` | `decompiled/DetailInfoMainUI/CreateOtherDetailInfoTabContents.original.c` |
| `011d9b80` | `HandleOtherDetailInfoTabChange` | `decompiled/DetailInfoMainUI/HandleOtherDetailInfoTabChange.original.c` |
| `011d9ce0` | `BindOtherDetailInfoButtonEvent` | `decompiled/DetailInfoMainUI/BindOtherDetailInfoButtonEvent.original.c` |
| `011dab40` | `ConstructDetailInfoTamerSubUI` | `decompiled/DetailInfoUI/ConstructDetailInfoTamerSubUI.original.c` |
| `011dac40` | `DestroyDetailInfoTamerSubUI` | `decompiled/DetailInfoUI/DestroyDetailInfoTamerSubUI.original.c` |
| `011dad40` | `RenderDetailInfoTamerSubUI` | `decompiled/DetailInfoUI/RenderDetailInfoTamerSubUI.original.c` |
| `011dcbe0` | `RenderTamerAttributeValue` | `decompiled/DetailInfoUI/RenderTamerAttributeValue.original.c` |
| `011dcf20` | `ResolveTamerObtainedAttributeValue` | `decompiled/DetailInfoUI/ResolveTamerObtainedAttributeValue.original.c` |
| `011dd0a6` | `ResolveDetailInfoDsTextCase` | `decompiled/DetailInfoUI/ResolveDetailInfoDsTextCase.original.c` |
| `011dd1d0` | `ResolveDetailInfoStatValue` | `decompiled/DetailInfoUI/ResolveDetailInfoStatValue.original.c` |
| `011dd560` | `HandleTamerEquipmentButton` | `decompiled/DetailInfoUI/HandleTamerEquipmentButton.original.c` |
| `011dd5b0` | `BindTamerEquipmentButton` | `decompiled/DetailInfoUI/BindTamerEquipmentButton.original.c` |
| `011dd770` | `HandleSealMasterButton` | `decompiled/DetailInfoUI/HandleSealMasterButton.original.c` |

Veja `docs/FunctionChainMap.md` para a cadeia de execucao organizada por janela,
aba e popup.
