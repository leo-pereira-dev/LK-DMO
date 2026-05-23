# Protocol audit - 2026-05-22

Scope:
- Client protocol headers under `dmo-client-main/common_vs2019/Protocol`.
- Client send/receive implementations checked for suspicious headers.
- Server packet writers under `DigitalWorldOnline.Commons/Packets`.
- Game Host packet processors under `DigitalWorldOnline.Game.Host/PacketProcessors`.

Validation:
- `dotnet build dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/DigitalWorldOnline.Game.csproj --no-restore /clp:ErrorsOnly`
- Result: success, 0 errors. Existing warnings remain in the project.

Fixed divergences:

1. Login response 3301
- Client: `GS2C_RECV_LOGIN_REQUEST` expects `u4 nResult` + `u1 nSubType`.
- Server fail path was writing five separate bytes.
- Fixed `LoginRequestAnswerPacket` to write `WriteInt(failReason)` then `WriteByte(39)`.

2. Digimon skill cooldown 3246
- Client: `GS2C_SEND_DIGIMON_SKILL_COOL_TIME` reads `u2 digimonIdx`, `u4 evoType`, `n4 count`, then `count * (u4 skill, u4 cooldownTs)`.
- Server wrote `handler` as 4 bytes and wrote `skillIds.Count` even when only some skills had cooldown payload.
- Fixed server writer to use `WriteUShort(handler)` and count only active cooldown entries.

3. Cash shop MultiBuy 3413
- Actual client sender pushes `n1 itemCnt`, `n4 totalPrice`, `u8 orderID`, then product ids.
- Server consumed order id as `u2`, causing product list misalignment.
- Fixed server processor to consume `ReadInt64()`.
- Fixed client protocol header order to match actual sender.

4. HotTime item request 3135
- Client header declared `n1 nTableIdx`, but client code pushes an `int`.
- Server already reads `ReadInt()`.
- Fixed client protocol header to `n4 nTableIdx`.

5. Tamer equipment appearance 1310
- Client receive code pops `u4 nTamerIDX`, but header declared `u2`.
- Server already writes `WriteInt(appearanceHandle)`.
- Fixed client protocol header to `u4 m_nTamerIDX`.

6. Digimon transcendence charge EXP 16039
- Client receive layout after success is:
  `n4 result`, `u1 flag`, `u1 pos`, `u2 digimonCount`, list `u2`, `u2 itemCount`, list item info, `u2 rate`, `u8 chargeExp`, `u8 totalExp`.
- Server response had an extra `WriteShort(0)` before total EXP.
- Fixed writer by removing the extra 2 bytes.
- Server request reader was offset by skipped bytes and wrong item material order.
- Fixed reader for the current client build layout: active item slot, npc id, flag, target slot, material digimon list, material item list.

7. Digimon transcendence final 16040
- Client sends `u4 activeItemSlot`, `u4 npcId`, `u1 targetSlot`, `u8 money`.
- Server processor was registered as `GuildTitleChange` and read the body as a different shape.
- Added `GameServerPacketEnum.DigimonTranscendence = 16040`.
- Fixed processor registration and read order.
- Server response now writes the three 8-byte fields expected by `GS2C_RECV_DigimonTranscendence`.

Second pass fixed divergences:

8. Trade core packets 1501/1502/1503/1504/1505/1506/1508/1509/1531/1532
- Client trade receive structs read `m_TargetTamerUID` as `u2`.
- Server was writing tamer handlers as `int` in request, approval, complete, add item, add money, lock and unlock packets.
- Fixed those handlers to `WriteUShort`.
- Client `RecvTradeAddItem` reads inventory slot as `u2`; server now writes `WriteUShort`.
- Client `RecvTradeCancelItem` is `pTrade::CancelItem` = 1531; server was sending 1519. Fixed packet id to 1531.
- Client `RecvTradeReject`, `RecvTradeCompleteFinal`, `RecvTradeInvenLock` and `RecvTradeUnInvenLock` do not consume the extra fields the server sent. Removed those extra payload bytes.

9. Party EXP and map-change packets 2311/2315
- Client `GS2C_NTF_PARTY_EXP_UPDATE` reads `m_nDigimonUID` as `u2`.
- Client `GS2C_NTF_PARTY_MEMBER_MAP_CHANGE` reads tamer and digimon UID as `u2`.
- Fixed server writers to use `WriteUShort` for those fields.

10. Quest goal update 11001
- Client `RecvQuestChangeInfo` reads quest id `u2`, goal index `u1`, current value `u1`.
- Server wrote current value as `short`.
- Fixed `QuestGoalUpdatePacket` to write one byte for the goal value.

11. Item socket identify 3929
- Client `RecvItemAnalysis` reads `u1 nRate` + `n8 nMoney`.
- Server had the right byte count by writing two `int`s, but truncated money to `int`.
- Fixed writer and callers to use `WriteInt64`.

12. Spirit conversion packets 3239/3240
- Client spirit-to-digimon reads `u4 digimonId`, `n8 money`, then repeated `u1 count` + `u4 itemId` until count zero.
- Client digimon-to-spirit reads `u1 slot`, `n8 money`, consumed item list, then gained item list.
- Fixed hatch/spirit packets to write `n8` money directly, use material amounts as `u1`, and emit the required zero terminators.
- Fixed spirit craft to send remaining bits instead of the price that was just removed.

13. Item timeout 3933
- Client `RecvItemTimeout` reads timeout type as `u1`.
- Server wrote timeout type as `int`.
- Fixed `ItemExpiredPacket` and `UpdateItemDurationPacket` to write one byte.

14. Buff change 4001
- Client `RecvBuffChange` expects `u4 uid`, `u2 beforeBuff`, `u2 buff`, `u2 classLevel`, `u4 endTs`, `u4 skill`.
- Server skipped `beforeBuff`, shifting every field after it.
- Fixed `UpdateBuffPacket` to write the current buff id as `beforeBuff` before the renewed buff id, matching the only current server use: refreshing the same active buff.

15. Partner scale 9942
- Client `RecvChangePartnerScale` expects `u4 uid`, `u2 scale`, `u4 endTick`.
- Server wrote scale as `int` and end tick as `short`.
- Fixed `UpdateSizePacket` to write `u2 scale` + `u4 endTick`.

16. Consigned shop item list 1520
- Client `RecvCommissionShopItemListResult` reads a final `bool/u1 bMyShop`.
- Server did not write it.
- Added final `WriteByte(0)`.

17. Account cash warehouse retrieve 3931
- Client `RecvCropCashItemSuccess` reads result, remaining time, cash warehouse slot `u2`, target tab `u1`, target slot `u1`, item id, count, rate, end time and trade-limit time.
- Server was sending destination slot as the result and wrote the source warehouse slot as an `int`.
- Fixed packet layout to match the client fields and sizes.

18. Guild info 2113
- Current client build includes `GUILD_RENEWAL`, so `RecvGuildInfo` expects GSP, active skill list count and per-member rank/last-time/memo.
- Server did not write the renewal fields.
- Added neutral renewal values to keep the guild info stream aligned.
- Guarded channel serialization so channel is only sent when map id is non-zero, matching the client conditional read.

Third pass fixed divergences and build blockers:

19. Consigned shop item list 1520
- Client reads two item lists using the announced counts, then a final `bool/u1 bMyShop`.
- Server now filters serialized items with the same rule as `ItemListModel.Count` (`ItemId > 0 && Amount > 0`) for both list sections.
- This prevents the server from announcing more items than it writes when a stale/zero-amount row exists.

20. Consigned shop warehouse 1523
- Client `RecvCommissionShopNpcItemList` reads `n4 result`, `n8 money`, `u4 count`, then `count * cItemData`.
- Server was writing only `itemId` + `amount` per item.
- Fixed `LoadConsignedShopWarehousePacket` to write full `cItemData` blocks and to use the same non-empty item filter as the count.

21. Client Release_English|Win32 build blockers
- `CROSSWARS_SYSTEM` code in `DataMng.cpp` used `m_InvenCross`, `m_CrossTatics`, `GetInvenCross()` and `GetCrossTatics()`, but the class header lacked the matching members/getters.
- Added the missing CrossWars members and compatibility getter so the active English client configuration compiles.

Hotfix after in-game test:

22. Mercenary Slot Expansion item 5812 / Type 159
- Client slot expansion is already handled by the normal `pItem::Use` success path: `RecvItemUseSuccess` calls `ServerItemUseSuccess`, which locally increments `_GetOpenSlot()` and adds the battle-tag skill entry.
- Server had no cap check in `IncreaseDigimonSlots`, so repeated use could persist values beyond the active-slot limit.
- Server also sent `UpdateDigimonSlotsPacket` with packet id `1102`; in this client `1102` is `pSkill::ApplyAround`, so the client could parse the slot update as a skill-around packet.
- Fixed server logic to refuse use at `GeneralSizeEnum.MaxActiveDigimonList` and send `ItemConsumeFailPacket(..., MaxLimit)` without consuming the item.
- Removed the invalid `UpdateDigimonSlotsPacket` emission and deleted the packet writer to avoid future reuse of the wrong id.
- Corrected the active-slot cap to the current English layout: `9` total active slots (`1` partner + `8` mercenary slots). The server now caps at `GeneralSizeEnum.MaxActiveDigimonList = 9`, the English client keeps `SizeDigimonBaseSlot = 9`, and client tactics memory tracks `DigimonBaseSlot - 1` slots so slots 7/8 are not outside the internal array.
- Confirmed the server value is total slots: `InitialInfoPacket` writes `character.DigimonSlots`, and the client reads it as `nTacticsOpenSlot - 1` to remove the partner.

Notes for next pass:
- `DigimonSkillLimitOpenResultPacket` looked suspicious in the automated primitive scan because `cEvoUnit` is a packed nested block. Manual check shows the writer intentionally emits 19 bytes for `cEvoUnit`, then `u4 itemPos`, `u4 itemType`.
- `ItemScan` looked suspicious in the broad scan, but manual check showed the server already supports variable request lengths and its response matches `GS2C_RECV_ITEMSCAN`.
- The client has conditional protocol fields under macros such as `SDM_VIP_SYSTEM_20181105` and `ITEM_USE_TIME_PASS`. Current `_TestDefine.h` has `ITEM_USE_TIME_PASS` enabled and VIP disabled. If VIP is enabled later, the transcendence request readers need a second compatibility path.
- `pGame::Sync` (1006) still produces automated false positives because the first fields select a dynamic payload shape. I did not change those without packet-specific manual proof.
- `InitialInfoPacket` still needs a dedicated pass if we want byte-for-byte proof; it has many dynamic array sections and is noisy in the primitive scanner.

Second pass validation:
- `dotnet build dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/DigitalWorldOnline.Game.csproj --no-restore /clp:ErrorsOnly`
- Result: success, 0 errors. Existing warnings remain in the project.

Third pass validation:
- `dotnet build dmo-server-main-postgres/DigitalWorldOnline.sln --no-restore /clp:ErrorsOnly`
- Result: success, 0 errors. Existing warnings remain in the project.
- `"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" C:\0-NewDmo\dmo-client-main\DProject.sln /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal`
- Result: success, 0 errors. Output copied to `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe` and `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`.
- Docker server stack rebuilt and restarted with `docker compose up -d --build account character game routine api admin`.
- Containers confirmed running for Account, Character, Game, Routine, Account.Api and Admin; mapped ports are listening on `17029`, `17050`, `17608`, `15189` and `15000`.

Hotfix validation:
- `dotnet build dmo-server-main-postgres/src/Source/Distribution/DigitalWorldOnline.Game.Host/DigitalWorldOnline.Game.csproj --no-restore /clp:ErrorsOnly`
- Result: success, 0 errors. Existing warnings remain in the project.
- `"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" C:\0-NewDmo\dmo-client-main\DProject.sln /m /p:Configuration=Release_English /p:Platform=Win32 /v:minimal`
- Result: success, 0 errors. Output `C:\Release\GSP\DProject_R_ENG.exe` timestamp `2026-05-22 10:45:47`, length `11139584`.
- Output copied to `D:\OpenDmo\src\Source\Cliente-OficialV2\Release\DProject_R_ENG.exe`; `C:\0-NewDmo\ClientDist\DProject_R_ENG.exe` was locked by a running `DProject_R_ENG.exe` process and still needs a client close before overwriting.
- Docker game service rebuilt and restarted with `docker compose up -d --build game`; `dmo-pg-game` is up on `17608`, and logs show `GameServer started`.
