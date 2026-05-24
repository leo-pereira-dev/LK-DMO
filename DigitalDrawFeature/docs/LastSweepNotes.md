# Last Sweep Notes

Final sweep results for the Digital Draw screenshot:

- Found official LuckyDraw table string:
  `DM_Luckydraw_Info-LuckyDrawInfo.bin`.
- Found RTTI names:
  - `.?AUsLuckyDrawInfo@Tb@@`
  - `.?AVCLuckyDrawTable@Tb@@`
  - `.?AV?$TableEventHandler@VCLuckyDrawTable@Tb@@@Tb@@`
- Found table loader/reader functions:
  - `016828f0`
  - `01682ab0`
  - `01682430`
  - `01682ad0`
  - `016827f0`
  - `01682880`
- Confirmed the visible UI assets come from the `Random_box` / `DigiSummonUI`
  family.
- Confirmed no `DM_Luckydraw_Info-LuckyDrawInfo.xml` or `.bin` file exists in
  the current `Dcommerce` XML/BIN extraction.
- A final recursive forced filename sweep over `C:\0-NewDmo` found no loose
  extracted `DM_Luckydraw_Info-LuckyDrawInfo.xml`. The only `LuckyDrawInfo`
  filename hits are the copied decompiled functions inside this subproject.
- Compressed archives were listed, but `7z`, `rar`, and `unrar` are not
  available in PATH, so RAR internals were not inspected in this pass.
- Copied 58 decompiled functions total:
  - 52 reused visual UI functions
  - 6 LuckyDraw table functions
- Added the internal `info. de prob.` subwindow chain after the final check:
  `011b6d30`, `011b6e50`, `011b7390`, `011b7530`, and `011b7bb0`.
- Generated xref-based manifest with 50 asset/table/sound references.

Do not invent card images or ticket records for this screen until the missing
LuckyDrawInfo table is recovered from the official data source.
