# Window reconstruction map

The screenshot maps to the official item upgrade management family.

## Main frame

- `CreateEquipmentUpgradeSystemWindow.original.c`
  - entry `0128c370`
  - registers `EquipmentUpgradeSystem`
- `CreateDataTradeDigimonUpgradeWindow.original.c`
  - entry `0128ac10`
  - registers adjacent `DataTradeDigimonUpgrade`
- `BuildUpgradeManagementWindow.original.c`
  - entry `01278ad0`
  - creates the `EquipSystem\Recharge_bg.tga` frame, the title
    `UPGRADESYSTEM_TITLE`, the close button, and the three tabs.

## Tabs

- Fortalecimento:
  - text key `EQUIPSYSTEM_UPGRADE`
  - panel constructor `CreateStrengthenTabPanel.original.c`
  - layout chain includes `BuildStrengthenTabLayout.original.c` and
    `BuildStrengthenSlotNetwork.original.c`
- Transferencia:
  - text key `UPGRADESYSTEM_TAB_UPGRADE_DATA_TRANSFER`
  - panel constructor `CreateTransferTabPanel.original.c`
  - layout chain includes `BuildTransferTabLayout.original.c` and
    `BuildTransferConnectionLines.original.c`
- Recuperacao:
  - text key `UPGRADESYSTEM_TAB_UPGRADE_DATA_RESTORE`
  - panel constructor `CreateRestoreTabPanel.original.c`
  - layout chain includes `BuildRestoreTabLayout.original.c` and
    `BuildRestoreConnectionLines.original.c`

## Screenshot visual anchors

- outer blue frame: `EquipSystem\Recharge_bg.tga`
- close button: `CommonUI\CommonUI_close.tga`
- tab buttons: `CommonUI\Menu_tap_btn.tga`
- item slots: `EquipSystem\Upgrade_Slot.tga`
- central/slot icon background: `EquipSystem\Upgrade_IC_BG.tga`
- horizontal/vertical links: `EquipSystem\Upgrade_Line_horizontal.tga`,
  `EquipSystem\Upgrade_Line_vertical.tga`
- yellow up/down arrows near the counters:
  - `EquipSystem\ArrowBtn_Up.tga`
  - `EquipSystem\ArrowBtn_Down.tga`
- progress stripes/gauge: `Item_Make\Make_ani_gauge_pixel.tga`
- result/effect sprites:
  - `EquipSystem\Upgrade_Progress_Effect.dds`
  - `EquipSystem\Upgrade_Success_Effect.dds`
  - `EquipSystem\Upgrade_Fail_Effect.dds`
  - `EquipSystem\Upgrade_Destroy_Effect.dds`
