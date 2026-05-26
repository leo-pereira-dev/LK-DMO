#include "stdafx.h"
#include "../ExtraInventoryDebugLog.h"
#include "cNetwork.h"
#include "cCliGame.h"
#include "../Global.h"
#include "../Engine.h"
#include "common_vs2019/pGame.h"
#include "common_vs2019/pTamer.h"
#include "common_vs2019/pDigimon.h"
#include "common_vs2019/pSkill.h"
#include "common_vs2019/pLimit.h"
#include "common_vs2019/pQuest.h"
#include "common_vs2019/pItem.h"
#include "common_vs2019/pInven.h"
#include "common_vs2019/pDigivice.h"
#include "common_vs2019/cItemData.h"
#include "common_vs2019/cEvoUnit.h"
#include "common_vs2019/cMapRegion.h"
#include "common_vs2019/pScanner.h"
#include "common_vs2019/pPass2.h"
#include "common_vs2019/cBattleInfo.h"
#include "../Flow/Flow.h"
#include "../Flow/FlowMgr.h"
#include "../../LibProj/CsFunc/CrashLogger.h"
#include "../_Interface/Game/_GameIF.h"
#include "common_vs2019/pEvent.h"
#include "../nProtect/Client_nProtect.h"
#ifndef ENCY_PACKET_PERF_LOG
#define ENCY_PACKET_PERF_LOG 1
#endif
#define ENCY_PACKET_PERF( ... ) do { if( ENCY_PACKET_PERF_LOG ) nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPACKET] " __VA_ARGS__ ); } while( 0 )
//bool g_bTempRevPortal = false; // ì•ˆì“°ëŠ” ë³€ìˆ˜ì´ë‹¤.
#ifdef SDM_DEF_XIGNCODE3_20181107
#include "../xigncode3/Client_Xigncode3.h"
#endif
void cCliGame::RecvXmlUnionProgress()
{
	u1 nLevel = 0;
	n4 nCurrentExperience = 0;
	n4 nRequiredExperience = 0;
	u2 nProgressPercentBasisPoints = 0;
	u2 nClaimedRewardMask = 0;
	pop( nLevel );
	pop( nCurrentExperience );
	pop( nRequiredExperience );
	pop( nProgressPercentBasisPoints );
	pop( nClaimedRewardMask );
	if( g_pGameIF && g_pGameIF->GetXmlUnionDUnit() )
	{
		g_pGameIF->GetXmlUnionDUnit()->RecvServerProgress(
			nLevel,
			nCurrentExperience,
			nRequiredExperience,
			nProgressPercentBasisPoints,
			nClaimedRewardMask );
	}
}
void cCliGame::RecvXmlUnionRewardClaimResult()
{
	u1 nResult = 0;
	u1 nRewardIndex = 0;
	u2 nClaimedRewardMask = 0;
	u1 nLevel = 0;
	u2 nProgressPercentBasisPoints = 0;
	pop( nResult );
	pop( nRewardIndex );
	pop( nClaimedRewardMask );
	pop( nLevel );
	pop( nProgressPercentBasisPoints );
	if( g_pGameIF && g_pGameIF->GetXmlUnionDUnit() )
	{
		g_pGameIF->GetXmlUnionDUnit()->RecvRewardClaimResult(
			nResult,
			nRewardIndex,
			nClaimedRewardMask,
			nLevel,
			nProgressPercentBasisPoints );
	}
}
void cCliGame::RecvXmlUnionCollectionInfo()
{
	if( g_pGameIF && g_pGameIF->GetXmlUnionDUnit() )
		g_pGameIF->GetXmlUnionDUnit()->RecvCollectionInfo();
}
// Receive Channel info
void cCliGame::RecvChannelInfo(void)
{
	GS2C_RECV_CHANNEL_LISTINFO recv;
	//DBG("\nChannel: %d\n", recv.channel);
	//DBG("Channel_Result: %d\n", recv.m_nResult);
	do 
	{
		u1 cChannelIDX;
		pop(cChannelIDX);
		//DBG("Channel_ID: %d\n", recv.channel[cChannelIDX]);
		if( cChannelIDX == 0xFF )
			break;
		pop(recv.channel[cChannelIDX]);
	} while ( 1 );
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_CHANNEL_LIST_INFO, &recv );
}
// Reset skill points
void cCliGame::RecvInitDigimonSkillPoint(void)
{
//	cCliGame::RecvInitGameData(void)
	cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
	cData_PostLoad::sDATA* pDigimonData = pPostLoad->GetDigimonData();
	pop( pDigimonData->s_nMaxEvoUnit);
	pop( &pDigimonData->s_EvoUnit[ 1 ], sizeof(cEvoUnit)*pDigimonData->s_nMaxEvoUnit );	
	g_pCharMng->GetDigimonUser( 0 )->SetAttributeEvoUnit( pDigimonData->s_nMaxEvoUnit, pDigimonData->s_EvoUnit );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_SKILL_DIGIMONSTATUS, NULL );
}
// Inicialize Game
void cCliGame::RecvInitGameData(void)
{
	DBG("\n--------- Init Game Data ---------\n\n");
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData begin" );
	g_pDataMng->GetTactics()->Init();
	cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
	cData_PostLoad::sDATA* pTamerData = pPostLoad->GetTamerData();
	cData_PostLoad::sDATA* pDigimonData = pPostLoad->GetDigimonData();
	cData_Inven* pInven = g_pDataMng->GetInven();
	cData_TEquip* pTEquip = g_pDataMng->GetTEquip();
	cData_Digivice* pDigivice = g_pDataMng->GetDigivice();
	//cData_Warehouse* pWarehouse = g_pDataMng->GetWarehouse();
	nLIB::eType LibType = nLIB::SVR_GAME;
	pop(LibType);
	nsCsGBTerrain::g_nSvrLibType = LibType;
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData libType=%d", (int)LibType );
	nSync::Pos pos;
	pop(pos);
	pPostLoad->SetPos(pos);
#ifdef SDM_TAMER_SERVERRELOCATE_20170911
	u4 nServerRelocateCompleteTime = 0;
	pop(nServerRelocateCompleteTime);
	pPostLoad->SetServerRelocateCompletedTime(nServerRelocateCompleteTime);
#endif
	//int TamerHandle = 0;
	//pop(TamerHandle);
	//pTamerData->s_Type.m_nUID = TamerHandle;
	sizeof(pTamerData->s_Type);
	//int TamerModel = 0;
	pop(pTamerData->s_Type);
	/*pTamerData->s_Type.m_nType = TamerModel;
	pTamerData->s_Type.m_nClass = nClass::Tamer;*/
	char szName[Language::pLength::name + 1] = "";
	pop(szName);
	_tcscpy_s(pTamerData->s_szName, Language::pLength::name + 1, LanConvertT(szName));
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tamer name=%s pos=%d,%d",
		szName, pos.m_nX, pos.m_nY );
	memset(szName, 0, sizeof(szName));
#ifndef UI_INVENTORY_RENEWAL
	n8 nMoney;
	pop(nMoney);
	pInven->SetMoney(nMoney, false);
	u2 nInvenSlotCount;
	u2 nWarehouseSlotCount;
	#ifdef NEW_SHARESTASH
		u2 nShareSlotSlotCount;
	#endif
	pop(nInvenSlotCount);		
	pop(nWarehouseSlotCount);
	#ifdef NEW_SHARESTASH
		pop(nShareSlotSlotCount);
	//	nShareSlotSlotCount = 14;
	#endif
	pInven->SetInvenSlotCount(nInvenSlotCount);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData slots inven=%u warehouse=%u",
		(unsigned)nInvenSlotCount, (unsigned)nWarehouseSlotCount );
	//	pWarehouse->SetSlotCount( nWarehouseSlotCount );	
	GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_NORMAL_SLOTCOUNT, &nWarehouseSlotCount);
	#ifdef NEW_SHARESTASH
		//	pWarehouse->SetShareSlotCount( nShareSlotSlotCount );
		GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_SHARE_SLOTCOUNT, &nShareSlotSlotCount);
	#endif
#endif
	pop(pTamerData->s_nExp);
	pTamerData->s_nExp = pTamerData->s_nExp / 100;
	pop(pTamerData->s_nLevel);
	pop(pTamerData->s_Attribute[MHP]);
	pop(pTamerData->s_Attribute[MDS]);
	pop(pTamerData->s_Attribute[HP]);
	pop(pTamerData->s_Attribute[DS]);
	pop(pTamerData->s_Attribute[FP]);
	pop(pTamerData->s_Attribute[AP]);
	pop(pTamerData->s_Attribute[DP]);
	pop(pTamerData->s_Attribute[MS]);
	cItemInfo* pItemInfo = NULL;
 
	// Tamer Equipament
	for (int i = 0; i < nLimit::Equip; ++i)
	{
		pItemInfo = pTEquip->GetData(i);
		pop(*pItemInfo);
		if (pItemInfo->IsEnable())	// Verifica se Ã© diferente de 0 (0 nao esta usando)
		{
			DBG("Item no slot %d em uso\n", i);
			if (nsCsFileTable::g_pItemMng->IsItem(pItemInfo->GetType()) == false)
			{
				CsMessageBox(MB_OK, _ONLY_ENG("You have one item that does not exist on Tamer equipament.\nEquipIndex = %d, ID = %d"), i, pItemInfo->GetType());
				//PostQuitMessage(0);//SetNextGameStep( GAME_EXIT );
			}
		}
	}
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tamer equip parsed equipCount=%d", nLimit::Equip );
	// Tamer Chipset
	for (int i = 0; i < nLimit::Chipset; ++i)
	{
		pItemInfo = pDigivice->GetChipset(i);
		pop(*pItemInfo);
		if (pItemInfo->IsEnable())	// Verifica se Ã© diferente de 0 (0 nao esta usando)
		{
			if (nsCsFileTable::g_pItemMng->IsItem(pItemInfo->GetType()) == false)
			{
				CsMessageBox(MB_OK, _ONLY_ENG("You have one item that does not exist on Tamer Chipset.\nChipsetIndex = %d, ID = %d"), i, pItemInfo->GetType());
				//PostQuitMessage(0);//SetNextGameStep( GAME_EXIT );
			}
		}
	}
	assert(nLimit::Digivice == 1);
	cItemData digiviceItem;
	pop(digiviceItem);
	pTEquip->SetDigiviceItem(&digiviceItem);
	for (int i = 0; i < nLimit::SkillSlot; ++i)
		pop(pDigivice->GetTamerSkill(i), sizeof(cItemData));
#ifdef INVEN_WRITE_FILE
	FILE* fp;
	fp = fopen("ì¸ë²¤í† ë¦¬ì •ë³´.txt", "wt");
	for (int i = 0; i < nLimit::Inven; ++i)
	{
		pItemInfo = pInven->GetData(i);
		pop(*pItemInfo);
		char msg[256];
		_stprintf_s(msg, 256, _T("\nIndex = %d, ID = %d, count = %d"), i, pItemInfo->GetType(), pItemInfo->GetCount());
		fwrite(msg, sizeof(char), _tcslen(msg), fp);
	}
	fclose(fp);
#else
#ifndef UI_INVENTORY_RENEWAL
	for (int i = 0; i < nLimit::Inven; ++i)
	{
		pItemInfo = pInven->GetData(i);
		pop(*pItemInfo);
		if (pItemInfo->IsEnable())	// Verifica se Ã© diferente de 0 (0 nao esta usando)
		{
			if (nsCsFileTable::g_pItemMng->IsItem(pItemInfo->GetType()) == false)
			{
				CsMessageBox(MB_OK, _ONLY_ENG("You have one item that does not exist on inventory.\nInvenIndex = %d, ID = %d \n"), i, pItemInfo->GetType());
				//PostQuitMessage(0);//SetNextGameStep( GAME_EXIT );
			}
		}
	}
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData inventory parsed max=%d", nLimit::Inven );
#endif
#endif
#ifndef UI_INVENTORY_RENEWAL
	int nMaxWareHouse = nsCsFileTable::g_pBaseMng->GetLimit()->s_nMaxWareHouse;
	map<int, cItemInfo> mapNormalWareHouse;
	for (int i = 0; i < nMaxWareHouse; ++i)
	{
		//		pItemInfo = pWarehouse->GetData( i );
		cItemInfo pwItemInfo;
		pop(pwItemInfo);
		mapNormalWareHouse.insert(make_pair(i, pwItemInfo));
		if (pwItemInfo.IsEnable())
		{
			if (nsCsFileTable::g_pItemMng->IsItem(pwItemInfo.m_nType) == false)
			{
				CsMessageBox(MB_OK, _ONLY_KOR("ì°½ê³ ì— ì¡´ìž¬í•˜ì§€ ì•ŠëŠ” ì•„ì´í…œì„ ê°€ì§€ê³  ìžˆìŠµë‹ˆë‹¤.\nWareHouseIndex = %d, ID = %d"), i, pItemInfo->GetType());
				PostQuitMessage(0);//SetNextGameStep( GAME_EXIT );
			}
		}
	}
	GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_SET_NORMAL, &mapNormalWareHouse);
	#ifdef NEW_SHARESTASH
		int nMaxShareStash = nsCsFileTable::g_pBaseMng->GetLimit()->s_nMaxShareStash;
		map<int, cItemInfo> mapShareWareHouse;
		for (int i = 0; i < nMaxShareStash; ++i)
		{
			//		pItemInfo = pWarehouse->GetShareData( i );
			cItemInfo pwItemInfo;
			pop(pwItemInfo);
			mapShareWareHouse.insert(make_pair(i, pwItemInfo));
			if (pwItemInfo.IsEnable())
			{
				if (nsCsFileTable::g_pItemMng->IsItem(pwItemInfo.m_nType) == false)
				{
					CsMessageBox(MB_OK, _ONLY_KOR("ê³µìœ  ì°½ê³ ì— ì¡´ìž¬í•˜ì§€ ì•ŠëŠ” ì•„ì´í…œì„ ê°€ì§€ê³  ìžˆìŠµë‹ˆë‹¤.\nShareStashIndex = %d, ID = %d"), i, pItemInfo->GetType());
					PostQuitMessage(0);//SetNextGameStep( GAME_EXIT );
				}
			}
		}
		GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_SET_SHARE, &mapShareWareHouse);
	#endif
#endif //UI_INVENTORY_RENEWAL
	// todo:: quest
	pop(*g_pDataMng->GetQuest()->GetInfo());
	DBG("Quests em progresso: %d\n", g_pDataMng->GetQuest()->GetInfo()->GetExecuteCount());
	DBG("Quests completadas: %d\n", g_pDataMng->GetQuest()->GetInfo()->GetQuestCompletionInfo());
	for (int i = 0; i < g_pDataMng->GetQuest()->GetInfo()->GetExecuteCount(); i++) {
		DBG("Quest ativada com index: %d\n", g_pDataMng->GetQuest()->GetInfo()->GetExeInfo(i).m_nIDX);
		for (int j = 0; j < 5; j++) {
			DBG("Condicao %d da quest: ", g_pDataMng->GetQuest()->GetInfo()->GetExeInfo(i).m_nCondition[j]);
		}
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_QUEST_INFO_INIT );
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData quests parsed active=%d",
		g_pDataMng->GetQuest()->GetInfo()->GetExecuteCount() );
	// --------------- Digimon hatching information ---------------
	
	GS2C_RECV_MAKE_DIGITAMA recv;
	pop(recv.m_nEggType);
	pop(recv.m_nEggLevel);
	pop(recv.m_nEggTradeLimitTime);
	
	pop(recv.m_nBackupDisk);
	pop(recv.m_nBackupdiskTradeLimitTime);
	GAME_EVENT_ST.OnEvent(EVENT_CODE::MAKETACTICS_DIGITAMA_DATA, &recv);
	
	// -----------------------------------------------------------------------------------
	u2 nBuffCount = 0;
	cData_PostLoad::sPostBuff pInfo;
	pop(nBuffCount);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tamer buff count=%u", (unsigned)nBuffCount );
	if (nBuffCount > 1000)
	{
		nBuffCount = 0;
	}
	if (nBuffCount != 0)
	{
		for (int i = 0; i < nBuffCount; i++)
		{
			pop(pInfo.s_nBuffCode);			// ë²„í”„ ì½”ë“œ
			pop(pInfo.s_nBuffClassLevel);	// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(pInfo.s_nBuffEndTS);		// ë²„í”„ ë§Œë£Œ ì‹œê°„	
			pop(pInfo.s_dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tamer buff[%d] code=%u class=%u end=%u skill=%u",
				i,
				(unsigned)pInfo.s_nBuffCode,
				(unsigned)pInfo.s_nBuffClassLevel,
				(unsigned)pInfo.s_nBuffEndTS,
				(unsigned)pInfo.s_dwSkillCode );
			g_pDataMng->GetPostLoad()->GetTBuffList()->push_back(pInfo);
		}
	}
#ifdef BATTLE_MATCH
	u4 nPoint = 0;
	u4 nWin = 0;
	u4 nLose = 0;
	u4 nDraw = 0;
	u4 nTotal = 0;
	pop(nPoint);
	pop(nWin);
	pop(nLose);
	pop(nDraw);
	pop(nTotal);
#endif
	n1 nTacticsOpenSlot = 2;
	pop(nTacticsOpenSlot);
	g_pDataMng->GetTactics()->_SetOpenSlot(nTacticsOpenSlot - 1);		//	-1 to remove partner
	if (nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE)
	{
		u4 nIDX;		// ê²½ê¸° IDX
		pop(pDigimonData->s_nBattleTeam);
		pop(nIDX);
		//g_pDataMng->GetBattle()->SetBattleRoomIDX( nIDX );
	}
//	uint MonHandle = 0;
//	pop(MonHandle);
//	pDigimonData->s_Type.m_nUID = MonHandle;
//	int MonModel = 0;
	pop(pDigimonData->s_Type);
	//pDigimonData->s_Type.m_nType = MonModel;
	//pDigimonData->s_Type.m_nClass = nClass::Digimon;
	pop(szName);
	_tcscpy_s(pDigimonData->s_szName, Language::pLength::name + 1, LanConvertT(szName));
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner name=%s type=%d",
		szName, pDigimonData->s_Type.m_nType );
	u1 nPartnerHatchLevel;
	pop(nPartnerHatchLevel);
	pDigimonData->s_HatchLevel = nPartnerHatchLevel;
	u2 nScale;
	pop(nScale);
	pDigimonData->s_fScale = nScale * 0.0001f;
	pop(pDigimonData->s_nExp);
#ifdef COMPAT_487
	u8 ExpPt2 = 0;
	pop(ExpPt2);
	pDigimonData->s_nTranscendenceExp = ExpPt2;
#endif
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
	pop(pDigimonData->s_nTranscendenceExp);
#endif
	pDigimonData->s_nExp = pDigimonData->s_nExp / 100;
	pop(pDigimonData->s_nLevel);
	DBG("Level: %d\n", pDigimonData->s_nLevel);
	//	pop( pDigimonData->s_Attribute[] );
	pop(pDigimonData->s_Attribute, sizeof(pDigimonData->s_Attribute));
	pop(pDigimonData->s_dwBaseDigimonID);
	
	DBG("nBaseEvoUnitIDX : %d\n", pDigimonData->s_dwBaseDigimonID);
	// ì„œë²„ë¡œë¶€í„° í˜„ìž¬ ë””ì§€ëª¬ ì§„í™” ì •ë³´ë¥¼ ì–»ëŠ”ë‹¤.
	pop(pDigimonData->s_nMaxEvoUnit);
	pop(&pDigimonData->s_EvoUnit[1], sizeof(cEvoUnit) * pDigimonData->s_nMaxEvoUnit);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner evo max=%d base=%u level=%u transExp=%I64u",
		(int)pDigimonData->s_nMaxEvoUnit,
		(unsigned)pDigimonData->s_dwBaseDigimonID,
		(unsigned)pDigimonData->s_nLevel,
		(unsigned __int64)pDigimonData->s_nTranscendenceExp );
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: DE ë°©ì–´ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  1
	pop(pDigimonData->s_nEnchantLevel);
	DBG("Enchant Level -> %d\n", pDigimonData->s_nEnchantLevel);
	pop(pDigimonData->s_ExtendAttribute, sizeof(pDigimonData->s_ExtendAttribute));
	pop(pDigimonData->s_ExtendAttributeLV, sizeof(pDigimonData->s_ExtendAttributeLV) );
	for (int i = 0; i < sizeof(pDigimonData->s_ExtendAttribute) / 2; i++) {
		DBG("Extended attribute val %d \n", pDigimonData->s_ExtendAttribute[i]);
		DBG("Extended attribut LV val %d \n", pDigimonData->s_ExtendAttributeLV[i]);
	}
	pop(nBuffCount);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner buff count=%u", (unsigned)nBuffCount );
	if (nBuffCount > 1000)
	{
		assert_csm(false, L"ë²„í”„ ê°œìˆ˜ê°€ 1000ê°œë„˜ìŒ");
		nBuffCount = 0;
	}
	
	if (nBuffCount != 0)
	{
		for (int i = 0; i < nBuffCount; i++)
		{
			pop(pInfo.s_nBuffCode);			// ë²„í”„ ì½”ë“œ
#ifndef COMPAT_487
			pop(pInfo.s_nBuffClassLevel);	// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
#else
			pInfo.s_nBuffClassLevel = 0;
#endif
			pop(pInfo.s_nBuffEndTS);		// ë²„í”„ ë§Œë£Œ ì‹œê°„	
			pop(pInfo.s_dwSkillCode);
					// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner buff[%d] code=%u class=%u end=%u skill=%u",
				i,
				(unsigned)pInfo.s_nBuffCode,
				(unsigned)pInfo.s_nBuffClassLevel,
				(unsigned)pInfo.s_nBuffEndTS,
				(unsigned)pInfo.s_dwSkillCode );
			g_pDataMng->GetPostLoad()->GetDBuffList()->push_back(pInfo);
		}
	}
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner buffs parsed" );
#ifdef BATTLE_MATCH
	u1 nBattleGrade;
	u4 nBattlePoint;
	u4 nBattleScore[eBattleMatchScore::eEnd] = { 0, };
	pop(nBattleGrade);
	pop(nBattlePoint);
	pop(nBattleScore[eBattleMatchScore::MatchWin]);
	pop(nBattleScore[eBattleMatchScore::MatchLose]);
	pop(nBattleScore[eBattleMatchScore::MatchDraw]);
	pop(nBattleScore[eBattleMatchScore::MatchTotal]);
#endif
	// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
	for (int i = 0; i < NewAttribute::MaxDigitalType; i++)
	{
		n2 AttributeExp;
		pop(AttributeExp);
		pDigimonData->s_AttributeExp[i] = AttributeExp;
	}
	// ë””ì§€ëª¬ ìžì—°ì†ì„± ê²½í—˜ì¹˜
	for (int i = 0; i < NewAttribute::MaxNatualType; i++)
	{
		n2 NatureExp;
		pop(NatureExp);
		pDigimonData->s_NatureExp[i] = NatureExp;
	}
	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ì˜ ìºì‰¬ ìŠ¤í‚¬ ì •ë³´ë¥¼ ë°›ìž
	// Data ì´ˆê¸°í™”
	for (int i = 0; i < nLimit::EvoUnit; ++i)
	{
		pDigimonData->s_DCashSkill[i].s_nDigimonEvoStatus = 0;
		for (int j = 0; j < nLimit::MAX_ItemSkillDigimon; ++j)
		{
			pDigimonData->s_DCashSkill[i].s_nDigimonCashSkillCode[j] = 0;
			pDigimonData->s_DCashSkill[i].s_nSkillCoolTime[j] = 0;
		}
	}
	u1 nDSkillCnt;
	pop(pDigimonData->s_nUID);
	pop(nDSkillCnt);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner memory uid=%u count=%u max=%u",
		(unsigned)pDigimonData->s_nUID,
		(unsigned)nDSkillCnt,
		(unsigned)(nLimit::EvoUnit * nLimit::MAX_ItemSkillDigimon) );
	if( nDSkillCnt > nLimit::EvoUnit )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData invalid partner memory skill count=%u limit=%u; aborting packet",
			(unsigned)nDSkillCnt,
			(unsigned)nLimit::EvoUnit );
		return;
	}
	for (int i = 0; i < nDSkillCnt; ++i)
	{
		pop(pDigimonData->s_DCashSkill[i].s_nDigimonEvoStatus);
		pop(pDigimonData->s_DCashSkill[i].s_nDigimonCashSkillCode, sizeof(u4) * nLimit::MAX_ItemSkillDigimon);
		pop(pDigimonData->s_DCashSkill[i].s_nSkillCoolTime, sizeof(u4) * nLimit::MAX_ItemSkillDigimon);
		nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner memory[%d] evoStatus=%u skill0=%u skill1=%u cd0=%u cd1=%u",
			i,
			(unsigned)pDigimonData->s_DCashSkill[i].s_nDigimonEvoStatus,
			(unsigned)pDigimonData->s_DCashSkill[i].s_nDigimonCashSkillCode[0],
			(unsigned)pDigimonData->s_DCashSkill[i].s_nDigimonCashSkillCode[1],
			(unsigned)pDigimonData->s_DCashSkill[i].s_nSkillCoolTime[0],
			(unsigned)pDigimonData->s_DCashSkill[i].s_nSkillCoolTime[1] );
	}
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData partner memory parsed" );
	u1 slot;
	pop(slot);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tactics first slot=%u", (unsigned)slot );
	cData_Tactics* pTacticsData = g_pDataMng->GetTactics();
	while (slot != 99)
	{
		if (slot == 0)
		{
			nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData invalid tactics slot=0; stopping tactics parse to avoid loop" );
			break;
		}
		nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tactics slot begin=%u", (unsigned)slot );
		cData_PostLoad::sDATA* pTactics = pTacticsData->GetTactics(slot - 1);
//#ifdef COMPAT_487
//	
//		uint MonHandle = 0;
//		pop(MonHandle);
//#endif
//		pop(MonModel);
//		
//		pTactics->s_Type.m_nType = MonModel;
//		pTactics->s_Type.m_nClass = nClass::Digimon;
		pop(pTactics->s_Type);
		pop(szName);
		_tcscpy_s(pTactics->s_szName, Language::pLength::name + 1, LanConvertT(szName));
		u1 nTacticsHatchLevel;
		pop(nTacticsHatchLevel);
		pTactics->s_HatchLevel = nTacticsHatchLevel;
#ifndef _GIVE
		TCHAR msg[222];
		swprintf_s(msg, _T("%s : %d"), pTactics->s_szName, nTacticsHatchLevel);
		ContentsStream kStream;
		wstring str = msg;
		bool bParcing = true;	bool bCheckSameString = false;
		kStream << str << bParcing << bCheckSameString;
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::EVENT_SYSTEM_PROCESS, &kStream);
#endif //_GIVE
		pop(nScale);
		pTactics->s_fScale = nScale * 0.0001f;
		pop(pTactics->s_nExp);
#ifdef COMPAT_487
		ExpPt2 = 0;
		pop(ExpPt2);
		pTactics->s_nTranscendenceExp = ExpPt2;
#endif
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
		pop(pTactics->s_nTranscendenceExp);
#endif
		pTactics->s_nExp = pTactics->s_nExp / 100;
		pop(pTactics->s_nLevel);
		pop(pTactics->s_Attribute, sizeof(pTactics->s_Attribute));
		pop(pTactics->s_dwBaseDigimonID);
		DBG("nBaseEvoUnitIDX : %d\n", pTactics->s_dwBaseDigimonID);
		pop(pTactics->s_nMaxEvoUnit);
		pop(&pTactics->s_EvoUnit[1], sizeof(cEvoUnit) * pTactics->s_nMaxEvoUnit);
		// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
		// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: DE ë°©ì–´ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  1
		pop(pTactics->s_nEnchantLevel);
		pop(pTactics->s_ExtendAttribute, sizeof(pTactics->s_ExtendAttribute));
		pop(pTactics->s_ExtendAttributeLV, sizeof(pTactics->s_ExtendAttributeLV));
#ifdef BATTLE_MATCH
		u1 nBattleGrade;
		u4 nBattlePoint;
		u4 nBattleScore[eBattleMatchScore::eEnd] = { 0, };
		pop(nBattleGrade);
		pop(nBattlePoint);
		pop(nBattleScore[eBattleMatchScore::MatchWin]);
		pop(nBattleScore[eBattleMatchScore::MatchLose]);
		pop(nBattleScore[eBattleMatchScore::MatchDraw]);
		pop(nBattleScore[eBattleMatchScore::MatchTotal]);
#endif
		// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
		for (int i = 0; i < NewAttribute::MaxDigitalType; i++)
		{
			n2 AttributeExp;
			pop(AttributeExp);
			pTactics->s_AttributeExp[i] = AttributeExp;
		}
		// ê°€ì§€ê³  ìžˆëŠ” ìš©ë³‘ë“¤ ìžì—°ì†ì„± ê²½í—˜ì¹˜
		for (int i = 0; i < NewAttribute::MaxNatualType; i++)
		{
			n2 NatureExp;
			pop(NatureExp);
			pTactics->s_NatureExp[i] = NatureExp;
		}
		// ìš©ë³‘ ë””ì§€ëª¬ì˜ ìºì‰¬ ìŠ¤í‚¬ ì •ë³´ë¥¼ ë°›ìž
		// Data ì´ˆê¸°í™”
		for (int i = 0; i < nLimit::EvoUnit; ++i)
		{
			pTactics->s_DCashSkill[i].s_nDigimonEvoStatus = 0;
			for (int j = 0; j < nLimit::MAX_ItemSkillDigimon; ++j)
			{
				pTactics->s_DCashSkill[i].s_nDigimonCashSkillCode[j] = 0;
				pTactics->s_DCashSkill[i].s_nSkillCoolTime[j] = 0;
			}
		}
		u1 nDSkillCnt;
		pop(pTactics->s_nUID);
		pop(nDSkillCnt);
		nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tactics slot=%u memory uid=%u count=%u",
			(unsigned)slot,
			(unsigned)pTactics->s_nUID,
			(unsigned)nDSkillCnt );
		if( nDSkillCnt > nLimit::EvoUnit )
		{
			nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData invalid tactics memory skill count slot=%u count=%u limit=%u; aborting packet",
				(unsigned)slot,
				(unsigned)nDSkillCnt,
				(unsigned)nLimit::EvoUnit );
			return;
		}
		for (int i = 0; i < nDSkillCnt; ++i)
		{
			pop(pTactics->s_DCashSkill[i].s_nDigimonEvoStatus);
			pop(pTactics->s_DCashSkill[i].s_nDigimonCashSkillCode, sizeof(u4) * nLimit::MAX_ItemSkillDigimon);
			pop(pTactics->s_DCashSkill[i].s_nSkillCoolTime, sizeof(u4) * nLimit::MAX_ItemSkillDigimon);
		}
		pop(slot);
		nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tactics next slot=%u", (unsigned)slot );
	}
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData tactics parsed" );
#ifdef SDM_DIGIMON_PARTSSYSTEM_20200115
	u4 nPartsEffectType = 0;			// ë””ì§€ëª¬ íŒŒì¸  ì´íŽ™íŠ¸ íƒ€ìž…
	pop(nPartsEffectType);
	pPostLoad->AddPartsInfo(pDigimonData->s_Type.m_nType, nPartsEffectType);
#endif
#ifdef CROSSWARS_SYSTEM
	cData_CrossTactics* pCrossTactics = g_pDataMng->GetCrossTatics();
	u1 slot2;
	pop(slot2);
	while (slot2 != 99) // í¬ë¡œìŠ¤ ìš©ë³‘ ë°ì´íƒ€
	{
		cData_CrossTactics::sCross* pTactics = pCrossTactics->GetTactics(slot2);
		pop(pTactics->s_nEnable);
		pop(pTactics->s_Type);
		pop(szName);
		_tcscpy_s(pTactics->s_szName, Language::pLength::name + 1, LanConvertT(szName));
		u1 nTacticsHatchLevel;
		pop(nTacticsHatchLevel);
		pTactics->s_HatchLevel = nTacticsHatchLevel;
#ifndef _GIVE
		TCHAR msg[222];
		swprintf_s(msg, _T("%s : %d"), pTactics->s_szName, nTacticsHatchLevel);
		ContentsStream kStream;
		wstring str = msg;
		bool bParcing = true;	bool bCheckSameString = false;
		kStream << str << bParcing << bCheckSameString;
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::EVENT_SYSTEM_PROCESS, &kStream);
#endif //_GIVE
		pop(nScale);
		pTactics->s_fScale = nScale * 0.0001f;
		pop(pTactics->s_nExp);
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
		pop(pTactics->s_nTranscendenceExp);
#endif
		pTactics->s_nExp = pTactics->s_nExp / 100;
		pop(pTactics->s_nLevel);
		pop(pTactics->s_Attribute, sizeof(pTactics->s_Attribute));
		pop(pTactics->s_dwBaseDigimonID);
		DBG("nBaseEvoUnitIDX : %d", pTactics->s_dwBaseDigimonID);
		pop(pTactics->s_nMaxEvoUnit);
		pop(&pTactics->s_EvoUnit[1], sizeof(cEvoUnit) * pTactics->s_nMaxEvoUnit);
		// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
		// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: DE ë°©ì–´ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  1
		pop(pTactics->s_nEnchantLevel);
		pop(pTactics->s_ExtendAttribute, sizeof(pTactics->s_ExtendAttribute));
		pop(pTactics->s_ExtendAttributeLV, sizeof(pTactics->s_ExtendAttributeLV));
#ifdef BATTLE_MATCH
		u1 nBattleGrade;
		u4 nBattlePoint;
		u4 nBattleScore[eBattleMatchScore::eEnd] = { 0, };
		pop(nBattleGrade);
		pop(nBattlePoint);
		pop(nBattleScore[eBattleMatchScore::MatchWin]);
		pop(nBattleScore[eBattleMatchScore::MatchLose]);
		pop(nBattleScore[eBattleMatchScore::MatchDraw]);
		pop(nBattleScore[eBattleMatchScore::MatchTotal]);
#endif
		// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
		for (int i = 0; i < NewAttribute::MaxDigitalType; i++)
		{
			n2 AttributeExp;
			pop(AttributeExp);
			pTactics->s_AttributeExp[i] = AttributeExp;
		}
		// ê°€ì§€ê³  ìžˆëŠ” ìš©ë³‘ë“¤ ìžì—°ì†ì„± ê²½í—˜ì¹˜
		for (int i = 0; i < NewAttribute::MaxNatualType; i++)
		{
			n2 NatureExp;
			pop(NatureExp);
			pTactics->s_NatureExp[i] = NatureExp;
		}
		pop(slot2);
	}
#endif
#ifdef COMPAT_487
	int IntBeforeChannel = 0;
	pop(IntBeforeChannel);
	//pPostLoad->AddPartsInfo(pDigimonData->s_Type.m_nType, IntBeforeChannel);
#endif
	GS2C_RECV_CURRENT_CHANNEL CurrentChannel;
	pop(CurrentChannel.channel_idx);
	GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_CURRENT_CHANNELINDEX, &CurrentChannel.channel_idx);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData channel=%d", (int)CurrentChannel.channel_idx );
	// -----------------------------------------------------------------------------------
	g_pDataMng->GetMapRegion()->ToBackup();
	pop(*g_pDataMng->GetMapRegion()->GetMapRegion());
	sizeof(*g_pDataMng->GetMapRegion()->GetMapRegion());
	if (g_pDataMng->GetMapRegion()->IsFirstLoad() == true)
	{
		g_pDataMng->GetMapRegion()->ToBackup();
		g_pDataMng->GetMapRegion()->FirstLoadComp();
	}
	// -----------------------------------------------------------------------------------
	GS2C_RECV_EXTEND_ARCHIVE kRecvArchiveData;
	int  iOpenedSlotCnt;
	pop(iOpenedSlotCnt);
	kRecvArchiveData.SetExtendArchiveCnt(iOpenedSlotCnt);
	//cData_TacticsHouse* pDataTH = g_pDataMng->GetTacticsHouse();
	//pDataTH->SetUseSlotCount( nDigimonCareSlotSize );
	//DBG( "%s (%d,%d)", nBase::w2m( pTamerData->s_szName), pPostLoad->GetPos().m_nX, pPostLoad->GetPos().m_nY );
	GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_ARCHIVE_OPENCNT, &kRecvArchiveData);
	// -----------------------------------------------------------------------------------
	
	//////////////////////////////////////////////////////////////////////////
	// íŒŒí‹° ì •ë³´ ë°›ëŠ” ê³³
	//////////////////////////////////////////////////////////////////////////
	GS2C_RECV_PARTY_INFO recvPartyInfo;
	pop(recvPartyInfo.m_nPartyIDX);
	u4 nPartyIDX = recvPartyInfo.m_nPartyIDX;
	u4 nCropType;
	pop(nCropType);
	recvPartyInfo.m_lootingInfo.m_nCropType = nCropType;
	pop(recvPartyInfo.m_lootingInfo.m_nRareRate);
	pop(recvPartyInfo.m_lootingInfo.m_nDispRareGrade);
	u1 nMasterSlotNo;
	pop(nMasterSlotNo);
	recvPartyInfo.m_nMasterSlotNo = nMasterSlotNo;
#ifdef COMPAT_487
	u2 shortBeforeSlot = 0;
	pop(shortBeforeSlot);
#endif
	u1 nSlotNo;
	pop(nSlotNo);
	while (nSlotNo < 99) //88 ^
	{
		TPartyMember addPartyMember;
		addPartyMember.m_nSlotNo = nSlotNo;
		pop(addPartyMember.m_TamerInfo.m_nUID);		// party tamer's uid,	0ì´ë©´ ë‹¤ë¥¸ ë§µì— ì¡´ìž¬
		pop(addPartyMember.m_DigimonInfo.m_nUID);	// party digimon's uid,	0ì´ë©´ ë‹¤ë¥¸ ë§µì— ì¡´ìž¬
		pop(addPartyMember.m_TamerInfo.m_nType);	//
		pop(addPartyMember.m_TamerInfo.m_nLevel);
		pop(addPartyMember.m_TamerInfo.m_Name);
		if (0 == addPartyMember.m_TamerInfo.m_Name.compare(pTamerData->s_szName))
			recvPartyInfo.m_nMySlotNo = nSlotNo;
		pop(addPartyMember.m_DigimonInfo.m_nType);
		pop(addPartyMember.m_DigimonInfo.m_nLevel);
		pop(addPartyMember.m_DigimonInfo.m_Name);
		pop(addPartyMember.m_nMapNo);
		pop(addPartyMember.m_nChannelNo);
		recvPartyInfo.m_listMemberInfo.push_back(addPartyMember);
		pop(nSlotNo);
	}
	GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_PARTY_INFO_INIT, &recvPartyInfo);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData party idx=%u members=%u",
		(unsigned)nPartyIDX, (unsigned)recvPartyInfo.m_listMemberInfo.size() );
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	u2 nAchievement;
	pop(nAchievement);	// í…Œì´ë¨¸ ì—…ì  ì²˜ë¦¬
	GS2C_RECV_DISPTITLE recvTitle;
	recvTitle.dispTitle = nAchievement;
	GAME_EVENT_ST.OnEvent(EVENT_CODE::ACHIEVE_SET_DISPTITLE, &recvTitle);
	u4 nCoolDownEndTimeTS[nLimit::CoolDown];	// 0~31 ==> 1~32 ë²ˆìœ¼ë¡œ ì‚¬ìš©, nCoolDownEndTick[0] ==> DM_Item_List-Time_Group ì‹œê°„ê·¸ë£¹(Time_Group) 1ë²ˆì„ ì˜ë¯¸
	pop(nCoolDownEndTimeTS);
	for (int i = 0; i < nLimit::CoolDown; ++i)
	{
		if (nsCsFileTable::g_pItemMng->IsCoolTime(i + 1) == false)
			continue;
		CsCoolTimeSeq* pSeq = nsCsFileTable::g_pItemMng->GetCoolTime(i + 1)->GetSeq();
		if (nCoolDownEndTimeTS[i] < _TIME_TS + 1)
		{
			pSeq->Reset();
			continue;
		}
		double fOffset = pSeq->GetEndTime() - (nCoolDownEndTimeTS[i] - _TIME_TS) - 1;
		if (fOffset < 0)
			fOffset = 0;
		pSeq->Start(fOffset);
	}
	u4 nOption;
	pop(nOption);	// nTamer::Option ì°¸ì¡°
	pPostLoad->SetSyncOption(nOption);
	n4 nWorkDayHistory;
	pop(nWorkDayHistory);
	u4 nTodayAttendanceTimeTS;
	pop(nTodayAttendanceTimeTS);
#ifndef REWARD_SYSTEM_UI
	g_pDataMng->GetAttendance()->SetWorkDayHistory(nWorkDayHistory);
	g_pDataMng->GetAttendance()->SetTodayAttendanceTimeTS(nTodayAttendanceTimeTS);
#endif
	RecvBossGenInfo();
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData after boss info" );
	bool bIsPCbang = false;
	uint nCostumeNo = 0;
	pop(bIsPCbang);	// PCë°© ì—¬ë¶€
	g_pDataMng->GetPCbang()->SetPcBang(bIsPCbang);
	g_pDataMng->GetPCbang()->SetCostume(0);
	if (bIsPCbang == true)
	{
		pop(nCostumeNo);	// ì½”ìŠ¤íŠ¬ì„ ìž…ê³  ìžˆì„ ê²½ìš° í•´ë‹¹ ì•„ì´í…œ ë²ˆí˜¸
		g_pDataMng->GetPCbang()->SetCostume(nCostumeNo);
	}
	if (nPartyIDX > 0)
	{
		uint nPCBangExpBuffRate = 0;
		uint nPCBangMemberCount = 0;
		pop(nPCBangMemberCount);	// pcë°© ë©¤ë²„ ì¸ì›ìˆ˜
		pop(nPCBangExpBuffRate);	// PCë°© ì¸ì›ì— ë”°ë¥¸ íŒŒí‹° ê²½í—˜ì¹˜ ë¹„ìœ¨ 
		//g_pDataMng->GetPCbang()->SetPartyExp( ( nPCBangMemberCount * 1000 ) + nPCBangExpBuffRate );
	}
	uint nShopData[5] = { 0, };
	pop(nShopData[0]);		// ë§µ ID	
	if (nShopData[0] != 0)		// ë§µ IDê°€ 0ì´ ì•„ë‹ˆë©´
	{
		pop(nShopData[1]);	// ì±„ë„
		pop(nShopData[2]);	// Xì¢Œí‘œ
		pop(nShopData[3]);	// Yì¢Œí‘œ
		pop(nShopData[4]);	// ì‚¬ìš©ëœ ì•„ì´í…œ íƒ€ìž…
		GS2C_RECV_SHOP_DATA pRecv;
		pRecv.iCharMapID = nShopData[0];
		pRecv.iCharChannel = nShopData[1];
		pRecv.iCharModelID = nShopData[4];
		pRecv.iCharPos = NiPoint2((float)nShopData[2], (float)nShopData[3]);
		GAME_EVENT_ST.OnEvent(EVENT_CODE::UNION_SET_SHOPDATA, &pRecv);
	}
	else
	{
		GAME_EVENT_ST.OnEvent(EVENT_CODE::UNION_RELEASE_DATA, NULL);
	}
	u4 nClientOption = 0;
	pop(nClientOption);
	pPostLoad->SetServerOption(nClientOption);
	u4 nRank = 0;
	pop(nRank);
	GS2C_RECV_RANKACHIEVE recvRank;
	recvRank.rankAchieve = nRank;
	GAME_EVENT_ST.OnEvent(EVENT_CODE::ACHIEVE_SET_RANKACHIEVE, &recvRank);
#ifdef PLAY_PANELTY
	// í”Œë ˆì´ íƒ€ìž„ ë³€ê²½
	// íŒ¨ë„í‹° íƒ€ìž… nPlayTime::NO_PANELTY = 0 (íŒ¨ë„í‹° ì—†ìŒ) nPlayTime::LEVEL1_PANELTY = 1 (ê²½í—˜ì¹˜ 30 ê°ì†Œ) nPlayTime::LEVEL2_PANELTY = 2 (ê²½í—˜ì¹˜x)
	u2 nPaneltyType = 0;		// íŒ¨ë„í‹° íƒ€ìž…
	u4 CashTime = 0;			// ìœ ë£Œ í”Œë ˆì´ ì‹œê°„
	u4 NormalTime = 0;			// ì¼ë°˜ í”Œë ˆì´ ì‹œê°„
	u4 nBasePenaltyTime = 0;	// ê¸°ë³¸ íŒ¨ë„í‹° ì ìš© ì‹œê°„
	u4 nBaseFullTime = 0;
	pop(nBaseFullTime);
	pop(nBasePenaltyTime);
	pop(nPaneltyType);
	pop(CashTime);
	pop(NormalTime); // ê¸ˆì¼ í”Œë ˆì´ ê°€ëŠ¥í•œ ë‚¨ì€ ì‹œê°„(ë‹¨ìœ„-ì´ˆ)
	g_pDataMng->PlayTime_Set_PaneltyType(nPaneltyType);
	g_pDataMng->PlayTime_Set_BaseFullTime(nBaseFullTime);
	g_pDataMng->PlayTime_Set_BasePaneltyTime(nBasePenaltyTime);
	g_pDataMng->PlayTime_Set_CashTime(CashTime);
	g_pDataMng->PlayTime_Set_NormalTime(NormalTime);
#endif	
#ifdef BATTLE_MATCH
	u4 nBattleCoin = 0;
	u4 nBattleTamerScore[eBattleMatchScore::eEnd] = { 0, };
	pop(nBattleCoin);
	pop(nBattleTamerScore[eBattleMatchScore::MatchWin]);
	pop(nBattleTamerScore[eBattleMatchScore::MatchLose]);
	pop(nBattleTamerScore[eBattleMatchScore::MatchDraw]);
	pop(nBattleTamerScore[eBattleMatchScore::MatchTotal]);
#endif	
#ifdef MINIGAME
	u1	nIsgame;		//ë¯¸ë‹ˆê²Œìž„ í–ˆëƒ trueë©´ í–ˆê³  falseë©´ ì•„ì§ ì•ˆí•œê±°
	u2		nSuccessCnt;	//ì„±ê³µ íšŸìˆ˜
	pop(nIsgame);
	bool nResult = false;
	if (nIsgame == 1)
		nResult = true;
	else
		nResult = false;
	pop(nSuccessCnt);
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_INIT kRecv;
	kRecv.nResult = nResult;
	kRecv.nSuccessCnt = nSuccessCnt;
	GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_MAKETACTICS_MINIGAME_INIT, &kRecv);
#endif
	u1 nSkill_Count;		// í…Œì´ë¨¸ ìŠ¤í‚¬ ê°œìˆ˜(ê¸°ë³¸ ìŠ¤í‚¬ ì œì™¸)
	pop(nSkill_Count);
	assert_cs(nSkill_Count <= 5);		// nSkill_count ëŠ” 5ë¥¼ ë„˜ì„ ìˆ˜ ì—†ë‹¤.
	for (int i = 0; i < nSkill_Count; ++i)
	{
		pop(pTamerData->s_nTamerSkillCode[i]);		// ì‚¬ìš©ëœ í…Œì´ë¨¸ ìŠ¤í‚¬ ì½”ë“œ
		pop(pTamerData->s_nTamerSkillCoolTime[i]);	// ì‚¬ìš©ëœ í…Œì´ë¨¸ ìŠ¤í‚¬ì˜ ë‚¨ì€ ì¿¨íƒ€ìž„
	}
	u1 nCashCount;					// í…Œì´ë¨¸ ìºì‰¬ ìŠ¤í‚¬ ê°œìˆ˜
	pop(nCashCount);
	assert_cs(nCashCount < 3);	// í˜„ìž¬ ìºì‰¬ ìŠ¤í‚¬ ê°œìˆ˜ëŠ” 2ë¥¼ ë„˜ì„ ìˆ˜ ì—†ë‹¤
	for (int i = 0; i < nCashCount; ++i)
	{
		pop(pTamerData->s_nTamerCashSkill_Index[i]);		// ìºì‰¬ ìŠ¤í‚¬ ì¸ë±ìŠ¤ (Tamer_Skill.xls ì˜ ì¸ë±ìŠ¤ ê°’)
		pop(pTamerData->s_nTamerCashSkill_LifeTime[i]);	// ê¸°ê°„ ë§Œë£Œê¹Œì§€ ì‹œê°„
		pop(pTamerData->s_nTamerCashSkill_CoolTime[i]);	// ì¿¨ íƒ€ìž„
	}
#ifdef CHAT_BAN
	bool bIsChatBan = true;	//	í˜„ìž¬ ì±„íŒ… ê¸ˆì§€ ì¤‘ì¸ì§€ í™•ì¸
	pop(bIsChatBan);
	if (bIsChatBan)//ì±„íŒ…ê¸ˆì§€ ì¤‘ì´ë¼ë©´
	{
		u4 nBanSecond = 0;			//	ì±„íŒ… ê¸ˆì§€ ë‚¨ì€ ì‹œê°„
		pop(nBanSecond);
		std::wstring banMsg = UISTRING_TEXT("CHAT_LIMIT_MESSAGE");
		std::wstring wsHour;
		DmCS::StringFn::From(wsHour, nBanSecond / 3600);
		DmCS::StringFn::Replace(banMsg, L"#Hour#", wsHour);
		std::wstring wsMin;
		DmCS::StringFn::From(wsMin, (nBanSecond / 60) % 60);
		DmCS::StringFn::Replace(banMsg, L"#Min#", wsMin);
		std::wstring wsSec;
		DmCS::StringFn::From(wsSec, nBanSecond % 60);
		DmCS::StringFn::Replace(banMsg, L"#Sec#", wsSec);
		CsMessageBox(MB_OK, banMsg.c_str());
		// ì±„íŒ… ê¸ˆì§€ ë‚¨ì€ì‹œê°„ ì €ìž¥		
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::SET_BANTIME;
		CProtocol.m_value = nBanSecond;
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol);
	}
#endif
#ifdef MASTERS_MATCHING
	u1 nMyTeam = 0;//0 = íŒ€ì—†ìŒ, 1 = AíŒ€, 2 = B
	pop(nMyTeam);
	g_pGameIF->m_sMyInfo.s_eMyTeam = (eTEAM)nMyTeam;//ë³¸ì¸ì˜ ë§ˆìŠ¤í„°ì¦ˆë§¤ì¹­ íŒ€
#endif
#ifndef UI_INVENTORY_RENEWAL
	// ì¸ë²¤í† ë¦¬ ê°¯ìˆ˜ì— ë§žê²Œ ì˜¤í”ˆ
	g_pDataMng->GetInven()->LoadApply();
#endif
	if (true == g_pGameIF->IsActiveWindow(cBaseWindow::WT_CARDINVENTORY))
		g_pGameIF->CloseDynamicIF(cBaseWindow::WT_CARDINVENTORY);
	// Modified so that seal master can be used when moving map / channel regardless of whether seal master is locked or not
	//if (g_pGameIF->IsActiveWindow(cBaseWindow::WT_MAIN_BAR))
		//g_pGameIF->GetMainBar()->SetButtonEnable(cMainBar::MAIN_BT_SEAL, true);
	u4 nDeck = 0;
	pop(nDeck);
	GS2C_RECV_ENCYCLOPEDIA_USEDECK ency_recv;
	if (nDeck != 0)
	{
		ency_recv.iDeckIdx = nDeck;
		ency_recv.bUse = true;
	}
	else
	{
		ency_recv.iDeckIdx = 0;
		ency_recv.bUse = false;
	}
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::ENCYCLOPEDIA_USE_DECK, &ency_recv);
#ifdef MEGAPHONE_BAN
	u1 nMegaphone = 0;
	pop(nMegaphone);	// 1ì´ë©´ ì°¨ë‹¨ ê°€ëŠ¥. 0ì´ë©´ ì°¨ë‹¨ ë¶ˆê°€
	pTamerData->s_bMegaPhoneBan = (nMegaphone == 0) ? false : true;
#endif
	// í˜„ìž¬ëŠ” ë˜ì „ì¼ ê²½ìš°ì—ë§Œ ì˜¤ë¸Œì íŠ¸ ID ë°›ìŒ		// 20150914 GameServerì¼ë•Œë„ ì˜¤ë¸Œì íŠ¸ ID ë°›ê²Œ ë³€ê²½
	RecvActorObjectFactor_Init();
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData after actor object factor" );
	GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_PLAYER_DATA_LOAD_COMPLETE, NULL);
	nsCSDEBUG::CrashLogger::LogMessage( "RECV InitGameData end load-complete fired" );
}
 
void cCliGame::RecvSetMoney(void)
{
	n8 nMoney;
	pop(nMoney);
	cData_Inven* pInven = g_pDataMng->GetInven();
	pInven->SetMoney( nMoney, false );
}
void cCliGame::RecvUserMessage(void)
{
	GS2C_NTF_USER_MESSAGE recv;
	pop(recv.m_szSenderName);
	pop(recv.m_nSendTimeTS);
	pop(recv.m_szMessage);
	recv.m_nMailType = mail::eAccount;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::NTF_USER_MESSAGE, &recv );
// 	cData_Mail::sMAILINFO m_pInfo;
// 	m_pInfo.s_nSendTimeTS = nSendTimeTS;
// 	_tcscpy_s( m_pInfo.s_szSenderName, szSenderName.c_str() );
// 	_tcscpy_s( m_pInfo.s_szMessage, szMessage.c_str() );
// 	g_pDataMng->GetMail()->SetMailInfo( &m_pInfo, cData_Mail::MAIL_ACCOUNT );
// 
// 	g_pGameIF->GetAlime()->SetAlime( 310, cAlime::MAIL, 0 );
// 	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAIL_BUTTON ) == false )
// 	{
// 		g_pGameIF->GetDynamicIF( cBaseWindow::WT_MAIL_BUTTON );
// 	}
}
void cCliGame::RecvTamerMessage(void)
{
	GS2C_NTF_USER_MESSAGE recv;
	pop(recv.m_szSenderName);
	pop(recv.m_nSendTimeTS);
	pop(recv.m_szMessage);
	recv.m_nMailType = mail::eTamer;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::NTF_USER_MESSAGE, &recv );
// 	std::wstring szSenderName;
// 	std::wstring szMessage;
// 	uint nSendTimeTS;
// 	pop(szSenderName);
// 	pop(nSendTimeTS);
// 	pop(szMessage);
// 
// 	if( szMessage[0] == '`' && szMessage[1] == '1' )
// 	{
// 		if( net::game )
// 			net::game->SendTamerMessageConfirm();
// 		g_pGameIF->GetAlime()->SetAlime( 310, cAlime::EVENT, 0 );
// 		return;
// 	}
// 
// 	cData_Mail::sMAILINFO m_pInfo;
// 	m_pInfo.s_nSendTimeTS = nSendTimeTS;
// 	_tcscpy_s( m_pInfo.s_szSenderName, szSenderName.c_str() );
// 	_tcscpy_s( m_pInfo.s_szMessage, szMessage.c_str() );
// 	g_pDataMng->GetMail()->SetMailInfo( &m_pInfo, cData_Mail::MAIL_TAMER );
// 
// 	g_pGameIF->GetAlime()->SetAlime( 310, cAlime::MAIL, 0 );
// 	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAIL_BUTTON ) == false )
// 		g_pGameIF->GetDynamicIF( cBaseWindow::WT_MAIL_BUTTON );
}
void cCliGame::RecvBossGenInfo(void)
{
	if( g_pGameIF )
	{
		cBossScene* pBossScene = g_pGameIF->GetBossScene();
		if(pBossScene)
			pBossScene->PreResetMap();
	}
	n4 nBossMonsterType;	// 
	n1 nBossMonsterCount;	// í•´ë‹¹ ë§µì— ìƒì„±ë˜ì–´ ìžˆëŠ” ë³´ìŠ¤ ëª¬ìŠ¤í„° ìˆ˜
	n4 nSeedMonsterType;	//
	n1 nSeedMonsterCount;	// ë³´ìŠ¤ ìƒì„±ì„ ìœ„í•´ ìž¡ì•„ì•¼í•  ëª¬ìŠ¤í„°ì˜ ë‚¨ì€ ìˆ˜
	pop(nBossMonsterType);
	while(nBossMonsterType)
	{
		pop(nBossMonsterCount);	// 
		if(nBossMonsterCount)	// ë³´ìŠ¤ ëª¬ìŠ¤í„°ê°€ ìžˆë‹¤ë©´ 
		{
			//í˜„ìž¬ ìƒì„±ë˜ì–´ ìžˆëŠ” ë³´ìŠ¤ ëª¬ìŠ¤í„°ì— ëŒ€í•œ ì •ë³´ ì²˜ë¦¬ í•„ìš”
			nBossMonsterType;	// ìƒì„±ë˜ì–´ ìžˆëŠ” ë³´ìŠ¤ëª¹ íƒ€ìž…
			nBossMonsterCount;	// ìƒì„±ë˜ì–´ ìžˆëŠ” ë³´ìŠ¤ëª¹ ê°œì²´ìˆ˜
			g_pGameIF->GetBossScene()->SetBossInfo( nBossMonsterType, nBossMonsterCount, 0, 0 );
		}
		else	// ë³´ìŠ¤ ëª¬ìŠ¤í„°ê°€ ìƒì„±ë˜ì–´ ìžˆì§€ ì•„ë‹ˆí•˜ë‹¤ë©´
		{
			pop(nSeedMonsterType);
			if(nSeedMonsterType)
			{
				pop(nSeedMonsterCount);
				//ìž¡ì•„ì•¼í•  ëª¬ìŠ¤í„°ì˜ ìˆ˜ ì²˜ë¦¬
				nBossMonsterType;	// ìƒì„±ë  ë³´ìŠ¤ ëª¬ìŠ¤í„° íƒ€ìž…
				nSeedMonsterType;	// ìž¡ì•„ì•¼í•  ì‹œë“œ ëª¬ìŠ¤í„° íƒ€ìž…
				nSeedMonsterCount;	// ì•žìœ¼ë¡œ ìž¡ì•„ì•¼í•  ì‹œë“œ ëª¬ìŠ¤í„° ê°œì²´ìˆ˜
				g_pGameIF->GetBossScene()->SetBossInfo( nBossMonsterType, 0, nSeedMonsterType, nSeedMonsterCount );
			}
			else
			{
				// ì •ë³´ ì¶œë ¥ ì—†ìŒ
				// ìž¡ì€ seed monsterì˜ ìˆ˜ê°€ ë¯¸ë¯¸í•œ ê²½ìš°
			}
		}
	
		pop(nBossMonsterType);
	}
}
void cCliGame::RecvTamerAchievement(void)
{
	u4 nTamerIDX = 0;
	u2 nAchievement = 0;
	pop(nTamerIDX);
	pop(nAchievement);
	
	CTamer* pTamer = (CTamer*)g_pCharMng->GetTamer( nTamerIDX );
	if( pTamer == NULL )
		return;
	assert_cs( pTamer->IsKindOf( RTTI_TAMER ) );
	pTamer->GetCharOption()->SetAchieve( nAchievement );
	if( pTamer->GetLeafRTTI() == RTTI_TAMER_USER )
	{
		GS2C_RECV_DISPTITLE recv;
		recv.dispTitle = nAchievement;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_DISPTITLE, &recv );
	}
}
void cCliGame::RecvTamerOption(void)
{
	u4 nTamerUID = 0;
	u4 nOption = 0;
	pop(nTamerUID);
	pop(nOption);
	CsC_AvObject* pTamer = g_pMngCollector->GetObject( nTamerUID );
	if( pTamer )
	{
		assert( pTamer->IsKindOf( RTTI_TAMER ) );
		( (CTamer*)pTamer )->ApplySyncOption( nOption );
	}
}
void cCliGame::RecvItemAnalysis(void)
{
	u1 nRate;	// ì•„ì´í…œ ëŠ¥ë ¥ì¹˜ ì ìš© ë¹„ìœ¨
	n8 nMoney;	// ë¶„ì„í›„ ë‚¨ì€ ê¸ˆì•¡
	pop(nRate);
	pop(nMoney);
	SAFE_POINTER_RET( g_pDataMng );
	cData_ServerSync* pSrvSync = g_pDataMng->GetServerSync();
	SAFE_POINTER_RET( pSrvSync );
	pSrvSync->RecvServer( cData_ServerSync::ELEMENT_ITEM_CHECK, 0, &nRate, &nMoney );
}
void cCliGame::RecvItemSellSuccess(void)
{
	ST_RECV_ItemSellSuccessInfo SellInfo;
	pop(SellInfo.m_CurrentMoney);
	u1 ChangedInvenCount(0);
	pop(ChangedInvenCount);
	for( int i = 0; i < ChangedInvenCount; ++i )
	{
		ST_RECV_ItemSellSuccessInfo::sInvenInfo InvenInfo;
		pop(InvenInfo.m_nInvenSlotNumber);
		pop(InvenInfo.m_nItemTypeID);
		pop(InvenInfo.m_nInvenItemCount);
		SellInfo.m_InvenInfoList.push_back( InvenInfo );
	}
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_STORE_SELL_SUCCESS, &SellInfo );
}
void cCliGame::RecvItemBuySuccess(void)
{
	ST_RECV_ItemBuySuccessInfo BuyInfo;
	pop(BuyInfo.m_CurrentMoney);
	u1 ChangedInvenCount(0);
	pop(ChangedInvenCount);
	for( int i = 0; i < ChangedInvenCount; ++i )
	{
		ST_RECV_ItemBuySuccessInfo::sInvenInfo InvenInfo;
		pop(InvenInfo.m_nInvenSlotNumber);
		pop(InvenInfo.m_nItemTypeID);
		pop(InvenInfo.m_nInvenItemCount);
		pop(InvenInfo.m_nRate);
		pop(InvenInfo.m_nLimited);
		BuyInfo.m_InvenInfoList.push_back( InvenInfo );
	}
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_STORE_BUY_SUCCESS, &BuyInfo );
}
void cCliGame::RecvItemMake(void)
{
	GS2C_RECV_ITEMPRODUCTION recv;
	pop( recv.m_dwResult );			// ë§Œë“¤ì–´ì§„ ì•„ì´í…œ í…Œì´ë¸” IDX	
	pop( recv.m_sDwItemIdx );			// ë§Œë“¤ì–´ì§„ ì•„ì´í…œ í…Œì´ë¸” IDX	
	pop( recv.m_nItemCount );			// ì‹¤ì œ ì•„ì´í…œ ê°œìˆ˜
	pop( recv.m_nTradeLimitTime );		// ì•„ì´í…œ ê±°ëž˜ ì œí•œ ì‹œê°„
	pop( recv.m_MoneyResult );			// ì‹¤ì œ ì‚¬ìš©í›„ ë³´ìœ í•  ê¸ˆì•¡
	pop( recv.m_TotalMakedCount );		// ì œìž‘ ì´ ê°¯ìˆ˜.
	pop( recv.m_GetCount );				// ì‹¤ì œ ì„±ê³µí•œ ê°œìˆ˜
	n4 nDeleteCount = 0;		// ì‚­ì œë˜ëŠ” ìž¬ë£Œ ì•„ì´í…œ ê°œìˆ˜
	pop( nDeleteCount );
	for( int n = 0; n < nDeleteCount; ++n )
	{
		sItemInfo sDelItemInfo;
		pop(sDelItemInfo.sDwItemIdx);	// ì§€ì›Œì§ˆ ì•„ì´í…œ í…Œì´ë¸” IDX
		pop(sDelItemInfo.nItemCount);	// ì‹¤ì œ ì‚­ì œëœ ê°œìˆ˜
		recv.m_DelItemInfo.push_back( sDelItemInfo );
	}
	pop( recv.m_nIsItemProtect );		// ë³´í˜¸ìš© ì•„ì´í…œ ì½”ë“œ ë²ˆí˜¸
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ITEM_PRODUCTION_RESULT, &recv );
}
void cCliGame::RecvEmoticon(void)
{
	u4 nUID = 0;
	int nEmoticonType = 0;
	pop(nUID);	// ì´ëª¨í‹°ì½˜ ì†¡ì‹  í…Œì´ë¨¸
	pop(nEmoticonType);
#ifdef TRICK_ORTREAT
	int nEffectNum;
	pop(nEffectNum);
#endif
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
	if( pObject == NULL )
		return;
	if( nsCsFileTable::g_pTamerMng->IsEmotion( nEmoticonType ) == false )
	{
		g_CriticalLog.Log( _T( "ì¡´ìž¬ í•˜ì§€ ì•ŠëŠ” ì´ëª¨ì…˜ íƒ€ìž…ì´ ë“¤ì–´ì™”ë‹¤. ì´ëª¨ì…˜ íƒ€ìž… = %d" ), nEmoticonType );
		return;
	}
	assert( pObject->IsKindOf( RTTI_TAMER ) );
	DWORD nAni = nsCsFileTable::g_pTamerMng->GetEmotion( nEmoticonType )->GetInfo()->s_nAniID;
#ifdef TRICK_ORTREAT
	if( 1 == nEffectNum)
	{
		pObject->GetProp_Effect()->AddEffect( EFFECT_BOOM, pObject->GetCenter(), 1.0f);
	}
#endif
	pObject->SetAnimation( nAni );
}
void cCliGame::RecvDigitamaSellResult(void)
{
	GS2C_RECV_DIGITAMA_TO_MONEY_SUCCESS recv;
	pop(recv.nTotalPrice);
	pop(recv.nTamerMoney);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_SELL_SUCCESS, &recv );		
}
void cCliGame::RecvDiedAlready(void)
{
	u2 nProtocol;
	pop(nProtocol);
	g_pDataMng->DiedAlready();
	g_pGameIF->RecvDiedAlready();
#ifndef _GIVE
	LOG("ì´ë¯¸ ì£½ì–´ì„œ ì²˜ë¦¬ ë¶ˆê°€ëŠ¥ìž…ë‹ˆë‹¤. (protocol:%d)", nProtocol);
#endif //_GIVE
}
void cCliGame::RecvVersionError(void)
{
	uint nSvrVersion;
	char msg[255];
	pop(nSvrVersion);
	pop(msg);
	/*char *r = strstr(msg, "\\n");
	while(r)
	{
		r[0] = '\n';
		for(int i=1; i<255; i++)
		{
			if(!r[i])
			{
				break;
			}
			r[i] = r[i+1];
		}
		r = strstr(msg, "\\n");
	}*/
// 	ReceiveVersionError();
	//assert( false );
#ifdef VERSION_USA
// 	cPrintMsg::PrintMsg( cPrintMsg::SERVER_VERSION_ERROR );
	TCHAR szBody[ 128 ];
	M2W( szBody, msg, 128 );
	cMessageBox::InsertList( UISTRING_TEXT( "MESSAGEBOX_ERROR_MESSAGE" ).c_str(), szBody, cPrintMsg::USE_MANUAL_MESSAGE, CsTalk_Message::MT_OK );
#else
	cPrintMsg::PrintMsg( cPrintMsg::SERVER_READYING, (void*)nBase::m2w(msg).c_str() );
#endif
}
void cCliGame::RecvInsMap_DungeonFail(void)
{
	GS2C_RECV_DUNGEON_FAIL recv;
	pop(recv.nResult);
	cPrintMsg::PrintMsg( recv.nResult );
	// ë©”ì‹œì§€
	//cPrintMsg::PrintMsg( 30097);
	if( net::game )
		net::game->SetInsMap_DungeonRestoreFlag();
}
void cCliGame::RecvLocalPortalSuccess(void) // ì¡´ë‚´ í¬íƒˆì´ìš©ì‹œ
{
	g_pResist->SetMovePortal(false);	//2017-04-12-nova
	u4 uTamerUID = 0;
	u4 uDigimonUID = 0;
	pop(uTamerUID);
	pop(uDigimonUID);
	nSync::Pos pos;
	pop( pos );
	bool bSendBlock = cClient::IsSendBlock();
	if(bSendBlock)
		cClient::SetSendBlock(false);
	m_bPortalRequesting = false;
	m_bPortalProcessing = false;
	g_pResist->SetEnablePortal( true );
	//CHAT_TEXT_DEGUG(_T("RcvLocalSucs-í¬íƒˆ ìƒíƒœ %d"), (int)g_pResist->IsEnablePortal());
	//net::receive_active = false;
	// ì¸í„°íŽ˜ì´ìŠ¤ ì´ˆê¸°í™”
	//assert_cs( g_pGameIF );
	//g_pGameIF->PreResetMap();
	net::bChangeChannel = false;
	if(net::IsMoveMapByCommand() == false )
	{
		net::prev_map_no = net::next_map_no;
		net::nIsMoveMapByCommand = true;
		net::bPortalMove = true;
	}
	else
	{
				
		net::bPortalMove = false;
	}
	
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAP_NO, &net::next_map_no );
	CsC_AvObject* pTamer = g_pMngCollector->GetObject( uTamerUID );
	CsC_AvObject* pDigimon = g_pMngCollector->GetObject( uDigimonUID );
	if(NULL == pTamer)
		return;
	if(NULL == pDigimon)
		return;
	//CTamerUser* pTamer = g_pCharMng->GetTamerUser();
	//CDigimonUser* pDigimon = g_pCharMng->GetDigimonUser(0);		
	pTamer->SetPauseAll(true);	
	pDigimon->SetPauseAll(true);	
	pTamer->SetPos(NiPoint2( (float)pos.m_nX, (float)pos.m_nY));
	pTamer->GetProp_Path()->ResetAgent();
	pDigimon->SetPos(NiPoint2( (float)pos.m_nX, (float)pos.m_nY));
	pDigimon->GetProp_Path()->ResetAgent();
	pTamer->SetPauseAll(false);		
	pTamer->SetAnimation( ANI::IDLE_NORMAL );	
	pDigimon->SetPauseAll(false);		
	pDigimon->SetAnimation( ANI::IDLE_NORMAL );	
	pTamer->GetProp_Effect()->AddEffect_FT( EFFECT_CLOSED_SCENE );				
	pDigimon->GetProp_Effect()->AddEffect_FT( EFFECT_CLOSED_SCENE );
	// í¬íƒˆíƒ€ëŠ”ê²Œ ìžì‹ ì´ë©´. íŒ¨ì´ë“œ ì”¬.
	if(pTamer->GetIDX() == g_pCharMng->GetTamerUser()->GetIDX())
	{
// 		SAFE_DELETE( g_pFade );
// 		g_pFade = NiNew CFade( FADE_IN, 0.8f );	
	}
}
void cCliGame::RecvSelectPortalFailure(void)
{
	GS2C_RECV_SELECT_PORTAL_FAILURE recv;
	pop(recv.nResult);
	// ì±„ë„ ë³€ê²½ì‹œ ì‹¤íŒ¨ ê°€ ì•„ë‹ˆë¼ë©´
	if( net::bChangeChannel == false )
	{
		// ìž…ìž¥ì‹œê°„ ì•„ë‹Œë° ë“¤ì–´ì™”ë‹¤
		cPrintMsg::PrintMsg(recv.nResult);
// 		ST_CHAT_PROTOCOL	CProtocol;
// 		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
// 		CProtocol.m_wStr = _T( "portal failure" );
// 		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		
	}
	// ì±„ë„ ë³€ê²½ ì‹¤íŒ¨ ë¼ë©´
	else
	{
		assert_cs( net::bChangeChannel == true );		
		cPrintMsg::PrintMsg( 10016 );
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CHANGE_CHANNEL_FAIL, null );
		assert_cs( g_pGameIF );
		g_pGameIF->GetPopup()->PreResetMap();
	}
	g_pResist->SetMovePortal(false);	//2017-04-12-nova
	g_pResist->SetEnablePortal( true ); // í¬íƒˆ ì‚¬ìš© ë‹¤ì‹œ í™œì„±í™”
	net::bChangeChannel = false;
	m_bPortalRequesting = false;
	m_bPortalProcessing = false;
	SetSendBlock(false);
}
void cCliGame::RecvSelectPortalSuccess(void)//ì¸ë§µí¬íƒˆ, ì¼ë°˜í¬íƒˆì´ìš©ì‹œ
{
	net::receive_active = false;
	m_bPortalRequesting = true;
	// ì¸í„°íŽ˜ì´ìŠ¤ ì´ˆê¸°í™”
	assert_cs( g_pGameIF );
	//g_pGameIF->PreResetMap();
	net::bChangeChannel = false;
	if(net::IsMoveMapByCommand() == false )
	{
		net::prev_map_no = net::next_map_no;
		net::nIsMoveMapByCommand = true;
		net::bPortalMove = true;
	}
	else
	{
		// ì œê±° ì´íŽ™íŠ¸
		//if( g_pLoading == NULL )
		//{
			g_pCharMng->GetTamerUser()->GetProp_Effect()->AddEffect_FT( EFFECT_CLOSED_SCENE );
			for( int i=0; i<MAX_DIGIMONUSER_COUNT; ++i )
			{
				CDigimonUser* pDigimon = g_pCharMng->GetDigimonUser( i );
				if( pDigimon )
					pDigimon->GetProp_Effect()->AddEffect_FT( EFFECT_CLOSED_SCENE );
			}
		//}		
		net::bPortalMove = false;
	}
	pop( net::ip );
	pop( net::port );
	pop( net::next_map_no );
	nSync::Pos pos;
	pop( pos );
	nsCSDEBUG::CrashLogger::LogMessage(
		"MAPLOAD RecvSelectPortalSuccess ip=%s port=%u nextMap=%u pos=%d,%d prevMap=%u portalMove=%d moveByCommand=%d",
		net::ip,
		(unsigned)net::port,
		(unsigned)net::next_map_no,
		pos.m_nX,
		pos.m_nY,
		(unsigned)net::prev_map_no,
		(int)net::bPortalMove,
		(int)net::IsMoveMapByCommand());
	g_pDataMng->GetPostLoad()->SetPos( pos );
	m_bPortalProcessing = true;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAP_NO, &net::next_map_no );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::LOADING_START );
}
void cCliGame::RecvChangeServer(void)
{
	net::cmd = Cmd::ConnectGameServer;
//	net::start(net::type::game, net::ip, net::port);
}
void cCliGame::RecvSocketInSuccess(void)
{
// 	assert_cs( g_pGameIF );
// 	assert_cs( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ELEMENTITEM_ATTACH ) );
// 	g_pGameIF->GetEI_Attach()->RecvServerAttach();
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ELEMENT_ITEM_ATTACH, NULL );
}
void cCliGame::RecvSocketOutSuccess(void)
{
// 	assert_cs( g_pGameIF );
// 	assert_cs( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ELEMENTITEM_ATTACH ) );
// 	g_pGameIF->GetEI_Attach()->RecvServerDetach();
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ELEMENT_ITEM_DETACH, NULL );
}
void cCliGame::RecvSocketClearSuccess(void)
{
// 	assert_cs( g_pGameIF );
// 	assert_cs( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ELEMENTITEM_ATTACH ) );
// 	g_pGameIF->GetEI_Attach()->RecvServerDelete();
}
void cCliGame::RecvCropMoney(void)	// 87ë²ˆ ì•„ì´í…œ ìŠµë“ìœ¼ë¡œ, ì¦‰ì‹œ ìž…ê¸ˆëœ ê²½ìš°
{
	u4 nCropTamerUID = 0;
	u4 nMoney = 0;
	u4 nGuildAddMoney = 0;
	u4 nBuffAddMoney = 0;
	pop( nCropTamerUID );	// ì•„ì´í…œ ì‹¤ ìŠµë“ìž
	pop( nMoney );	// í•´ë‹¹ ë¨¸ë‹ˆ(bit) ì¶”ê°€ í•„ìš” , ê¸°ë³¸ íšë“í•˜ëŠ” ëˆ
	pop( nGuildAddMoney );// ê¸¸ë“œ ë²„í”„ë¡œ ì¸í•´ ì¶”ê°€ëœ ê¸ˆì•¡
	pop( nBuffAddMoney );// ì¼ë°˜ ë²„í”„ë¡œ ì¸í•´ ì¶”ê°€ëœ ê¸ˆì•¡
	// ë‹¤ë¥¸ ì‚¬ëžŒì´ ë¨¼ì € ìŠµë“í•œ ê²½ìš°ì—”
	// pItem::CropItem ê³¼ ê°™ì´ pItem::CropFailure - pItem::CropF_Late ë¡œ ì²˜ë¦¬ëœë‹¤.	
	if( g_pCharMng->GetTamerUser() )
		g_pCharMng->GetTamerUser()->MoneyCrop( nMoney, nCropTamerUID, nGuildAddMoney, nBuffAddMoney );	
#ifndef _GIVE
	TCHAR msg[333] = {0,};
	u4 nTotal = nMoney + nGuildAddMoney + nBuffAddMoney;
	swprintf(msg, _T( "Crop Total : %d Bit, Org : %d Bit, Guild : %d Bit, Buff : %d Bit " ), nTotal, nMoney, nGuildAddMoney, nBuffAddMoney);
	CHAT(msg);
#endif //_GIVE
}
void cCliGame::RecvCropFailure(void)
{
	uint nErrCode;
	pop(nErrCode);
	switch(nErrCode)
	{
	case pItem::CropF_Late	:
		//cPrintMsg::PrintMsg( 30012 );
		break;
	case pItem::CropF_Far	:
		cPrintMsg::PrintMsg( 30013 );
		cDigimonTalk::Print( 30013 );
		break;
	case pItem::CropF_Right	:
		cPrintMsg::PrintMsg( 18001 );
		cDigimonTalk::Print( 18001 );
		break;
	case pItem::CropF_Full	:
		cPrintMsg::PrintMsg( cPrintMsg::GAME_INVEN_LIMIT_COUNT );
		break;
	default:
		xassert(false, "í™•ì¸ í•„ìš”");
	}
	g_pCharMng->GetTamerUser()->ItemCropFailed();
}
void cCliGame::RecvAttack( bool bDie )
{
	u4 nHitterUID = 0;
	u4 nTargetUID = 0;
	n4 nDamage = 0;
	u4 nBattleOption = 0;
	pop(nHitterUID);	// ë•Œë¦°ë„˜
	pop(nTargetUID);	// ë§žëŠ”ë„˜
	pop(nDamage);
	pop(nBattleOption);
	n4 hp = 0;
	n4 Mhp = 0;
	if( bDie == false )
	{
		pop( hp );
		pop( Mhp );
	}
	AttackProcesses(nHitterUID, nTargetUID, nDamage, nBattleOption, hp, Mhp, bDie);
}
void cCliGame::RecvAttackMiss(void)
{
	u4 nHitterUID = 0;
	u4 nTargetUID = 0;
	pop(nHitterUID);
	pop(nTargetUID);
	sRECV_HITTER_INFO hitter;
	hitter.s_nUID = nHitterUID;	
	hitter.s_HitType = CsC_AttackProp::HT_Miss;
	sRECV_TARGET_INFO target;
	target.s_nUID = nTargetUID;
	target.s_DamageType = CsC_AttackProp::DT_Miss;
	target.s_nNumEffectType = NUMTYPE::ET_NORMAL;
	SetAttackInfo( &hitter, &target, false );
}
void cCliGame::RecvTargetInfo(void)
{
	u4 nUID = 0;
	u1 nHPRate = 0;
	u4 nRightUID = 0;		// ì†Œìœ ê¶Œìž
	u4 nExpireTimeTS = 0;	// 0ì´ë©´ ì†Œë©¸ê³¼ ë¬´ê´€, ìˆ˜ì¹˜ê°€ ìžˆë‹¤ë©´.. ì†Œë©¸ë  ì‹œê°„ìž„
	u4 nCreatedTimeTS = 0;	// ìƒì„±ëœ ì‹œê°„
	pop(nUID);
	pop(nHPRate);
	pop(nRightUID);
	pop(nExpireTimeTS);
	pop(nCreatedTimeTS);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
	if( pObject == NULL )
		return;
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON_USER:
	case RTTI_TAMER_USER:
	case RTTI_TUTORIAL_DIGIMON:
	case RTTI_TUTORIAL_TAMER:
		return;
	}
	pObject->GetProp_Attack()->ClearDamageValue();
	pObject->GetBaseStat()->SetHP( nHPRate );
	if( pObject->GetLeafRTTI() == RTTI_MONSTER )
	{
		CMonster* pMonster = (CMonster*)pObject;
		
		pMonster->SetOwnerID( nRightUID );
		if( nExpireTimeTS )
		{
			pMonster->SetExpireTimeTS( nExpireTimeTS );			
			pMonster->SetTotalExpireTime( nExpireTimeTS - nCreatedTimeTS );
		}
	}
}
void cCliGame::RecvSkillDie(void)
{
	u4 nHitterUID;
	u4 nTargetUID; // ì£½ëŠ”ë„˜
	u4 nSkillArrIDX; // nHitter's skill array idx
	n4 nDamage;
	pop(nHitterUID);
	pop(nTargetUID);
	pop(nSkillArrIDX);
	pop(nDamage);
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( nTargetUID );
	if( pTarget == NULL )
		return;
	pTarget->SetAniPause( false );
	sRECV_HITTER_INFO hi;
	hi.s_nUID = nHitterUID;
	hi.s_nSkillIndex = nSkillArrIDX;
	CsMonsterSkill::sINFO* pSkillInfo = nsCsFileTable::g_pMonsterMng->GetMonsterSkill( nSkillArrIDX );
	if( pSkillInfo != NULL )
	{
		switch( pSkillInfo->s_nSkillType )
		{
		case CsMonsterSkill::ATTACH_SEED:	// ì”¨ì•—ê¹”ê¸°		( ëŒ€ìƒ ë””ë²„í”„, ì‹œê°„ ê²½ê³¼ í›„ ëŒ€ìƒ ìœ„ì¹˜ ë°”ë‹¥ ìƒì„± )
		case CsMonsterSkill::BERSERK:		// ê´‘í­í™”		( ê³µëžµ ì‹œê°„ ì´ˆê³¼ )
			hi.s_HitType = CsC_AttackProp::HT_Skill_NoAni;
			break;
		default:
			hi.s_HitType = CsC_AttackProp::HT_Skill;
			break;
		}
	}
	else
		hi.s_HitType = CsC_AttackProp::HT_Skill;
	sRECV_TARGET_INFO ti;
	ti.s_nUID = nTargetUID;
	ti.s_DamageType = CsC_AttackProp::DT_Normal;
	ti.s_nNumEffectType = NUMTYPE::ET_NORMAL;
	ti.s_nValue[ 0 ] = -nDamage;
	ti.s_nValue[ 1 ] = -pTarget->GetBaseStat()->GetBackupHP();
	SetSkillInfo( &hi, &ti, true );
	ST_CHAT_PROTOCOL	CProtocol;
	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
	//CProtocol.m_wStr = GetVAString( _T("RecvSkillDie SetDieInfo UID : %d"), pTarget->GetUniqID() );
	DmCS::StringFn::Format(CProtocol.m_wStr,  _T("RecvSkillDie SetDieInfo UID : %d"), pTarget->GetUniqID() );
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );	
	SetDieInfo( pTarget );
}
void cCliGame::RecvDotDie()
{
	// ì„œë²„ì—ì„œ ì•ˆ ë³´ë‚¸ë‹¤ê³  í•¨
// 	u4 HitterUID = 0;
// 	u4 TargetUID = 0;
// 	n2 nDamage = 0;	
// 	u2 nBuffCode = 0;
// 
// 	pop( HitterUID );
// 	pop( TargetUID );
// 	pop( nDamage );
// 	pop( nBuffCode );
// 
// 	CsC_AvObject* pTarget = g_pMngCollector->GetObject( TargetUID );
// 	if( pTarget == NULL )
// 		return;	
// 
// 	sRECV_TARGET_INFO ti;
// 	ti.s_nUID = TargetUID;
// 	ti.s_DamageType = CsC_AttackProp::DT_Damage_Dot;
// 	ti.s_nNumEffectType = NUMTYPE::ET_NORMAL;
// 	ti.s_nValue[ 0 ] = nDamage;
// 	ti.s_nValue[ 1 ] = -pTarget->GetBaseStat()->GetBackupHP();
// 
// 	sRECV_HITTER_INFO hi;
// 	hi.s_nUID = HitterUID;
// 	hi.s_nSkillIndex = nBuffCode;
// 	hi.s_HitType = CsC_AttackProp::HT_Normal;
// 
// 	SetDotInfo( &hi, &ti, nBuffCode, true );
// 
// 	ST_CHAT_PROTOCOL	CProtocol;
// 	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
// 	//CProtocol.m_wStr = GetVAString( _T("RecvDotDie SetDieInfo UID : %d"), pTarget->GetUniqID() );
// 	DmCS::StringFn::Format(CProtocol.m_wStr, _T("RecvDotDie SetDieInfo UID : %d"), pTarget->GetUniqID());
// 	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
// 	SetDieInfo( pTarget );
}
void cCliGame::RecvBattleOn(void)
{
	u4 uid = 0;
	pop(uid);
//	net::chat("ì „íˆ¬ ìƒíƒœ ON %d", uid);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( uid );
	if( pObject == NULL )
		return;
	pObject->SetBattle();
}
void cCliGame::RecvBattleOff(void)
{
	u4 uid = 0;
	pop(uid);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( uid );
	if( pObject == NULL )
		return;
	pObject->GetProp_Attack()->InsertEvent( CsC_AttackProp::EVENT_END_BATTLE, 0, 0, 0, 0 );	
	// GSPì˜ ê¼‰ìš° battle ì‹œê°„ì„ ìˆ˜ì‹ 
#ifdef PLAY_PANELTY
	if( pObject->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		u4 CashTime = 0;
		u4 NormalTime = 0;
		pop(NormalTime);
		pop(CashTime);
		g_pDataMng->PlayTime_Set_CashTime( CashTime );
		g_pDataMng->PlayTime_Set_NormalTime( NormalTime );
	}	
#endif
	if( pObject->GetLeafRTTI() == RTTI_MONSTER )
	{
		// [11/15/2016 hyun] ëª¬ìŠ¤í„° ì œì–´ê¶Œìž ì´ˆê¸°í™”
		CMonster* pMonster = dynamic_cast<CMonster*>(pObject);
		SAFE_POINTER_RET(pMonster);
		pMonster->SetOwnerID(0);
	}
}
void cCliGame::RecvGetExp(void)
{
	n8 nTamerGainExp;	// ì´ë²ˆì— íšë“í•œ exp
	n8 nTamerBonusExp;	//
	n8 nTamerExp; // ìµœì¢… exp
	u4 nDigimonUID;
	n8 nDigimonGainExp;
	n8 nDigimonBonusExp;
	n8 nDigimonExp; // ìµœì¢… exp
	u4 nSkillExp;
	pop(nTamerGainExp);
	pop(nTamerBonusExp);
	pop(nTamerExp);		// 
	pop(nDigimonUID);	// ì†Œìœ  ë””ì§€ëª¬ì´ ì—¬ëŸ¬ ë§ˆë¦¬ì¼ ê²½ìš° ì²˜ë¦¬ë¥¼ ìœ„í•¨
	pop(nDigimonGainExp);
	pop(nDigimonBonusExp);
	pop(nDigimonExp);	// 
	pop(nSkillExp);
	nTamerGainExp = nTamerGainExp / 100;
	nTamerBonusExp = nTamerBonusExp / 100;
	nTamerExp = nTamerExp / 100;
	nDigimonGainExp = nDigimonGainExp / 100;
	nDigimonBonusExp = nDigimonBonusExp / 100;
	nDigimonExp = nDigimonExp / 100;
#ifdef PLAY_PANELTY
	if( ( nTamerGainExp == 0 )&&( nDigimonGainExp == 0 )&&( g_pDataMng->PlayTime_Get_PaneltyType() == nPlayTime::MAX_PENALTY ) )
	{
		cPrintMsg::PrintMsg( 30419 );
	}
	else if( nDigimonBonusExp == 0 )
	{
		n8 TGain = nTamerGainExp + nTamerBonusExp;
		n8 DGain = nDigimonGainExp + nDigimonBonusExp;
		n4 TGainMsg = (n4)TGain;
		n4 DGainMsg = (n4)DGain;
		cPrintMsg::PrintMsg( 30007, &TGainMsg, &DGainMsg );
	}
	else
	{
		n8 TGain = nTamerGainExp + nTamerBonusExp;
		n8 DGain = nDigimonGainExp + nDigimonBonusExp;
		n8 TBonusGain = nTamerBonusExp;
		n8 DBonusGain = nDigimonBonusExp;
		n4 TGainMsg = (n4)TGain;
		n4 DGainMsg = (n4)DGain;
		n4 TBonusGainMsg = (n4)TBonusGain;
		n4 DBonusGainMsg = (n4)DBonusGain;
		cPrintMsg::PrintMsg( 30008, &TGainMsg, &TBonusGainMsg, &DGainMsg, &DBonusGainMsg );
	}
#else
	if( nDigimonBonusExp == 0 )
	{
		n8 TGain = nTamerGainExp + nTamerBonusExp;
		n8 DGain = nDigimonGainExp + nDigimonBonusExp;
		n4 TGainMsg = (n4)TGain;
		n4 DGainMsg = (n4)DGain;
		cPrintMsg::PrintMsg( 30007, &TGainMsg, &DGainMsg );
	}
	else
	{
		n8 TGain = nTamerGainExp + nTamerBonusExp;
		n8 DGain = nDigimonGainExp + nDigimonBonusExp;
		n8 TBonusGain = nTamerBonusExp;
		n8 DBonusGain = nDigimonBonusExp;
		n4 TGainMsg = (n4)TGain;
		n4 DGainMsg = (n4)DGain;
		n4 TBonusGainMsg = (n4)TBonusGain;
		n4 DBonusGainMsg = (n4)DBonusGain;
		cPrintMsg::PrintMsg( 30008, &TGainMsg, &TBonusGainMsg, &DGainMsg, &DBonusGainMsg );
	}
#endif	
	g_pCharMng->GetTamerUser()->GetBaseStat()->SetExp( nTamerExp );
	uint nDigimonIDX = GetIDX( nDigimonUID );
	CDigimon* pDigimon = g_pCharMng->GetDigimon( nDigimonIDX );
	if( pDigimon )
	{
		pDigimon->GetBaseStat()->SetExp( nDigimonExp );
	}
	
// begin ìŠ¤í‚¬ ê²½í—˜ì¹˜+
	if(nSkillExp > 0)
	{
		assert_cs( g_pCharMng->GetDigimon( GetIDX( nDigimonUID ) )->GetLeafRTTI() == RTTI_DIGIMON_USER );
		if( pDigimon )
		{
			CDigimonUser* pDigimonUser = (CDigimonUser*)pDigimon;
			cEvoUnit* pEvoUnit = pDigimonUser->GetAttributeEvoUnit( pDigimonUser->GetFTEvolCurObj()->m_nEvoSlot );
			pEvoUnit->m_nSkillExp = CsFloat2Int( nSkillExp );
		}		
	}
// end ìŠ¤í‚¬ ê²½í—˜ì¹˜+
}
void cCliGame::RecvLevelUp(void)
{
	u4 nUID = 0;
	u1 cLevel = 0;
	pop(nUID);
	pop(cLevel);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
	if( pObject == NULL )
		return;
	assert_cs( ( pObject->IsKindOf( RTTI_DIGIMON ) )||( pObject->IsKindOf( RTTI_TAMER ) ) );
	pObject->GetProp_Attack()->InsertEvent( CsC_AttackProp::EVENT_LEVEL_UP, (int)cLevel, 0, 0, 0 );
	if( pObject->IsKindOf( RTTI_DIGIMON_USER ) )//ë””ì§€ëª¬ì´ ë ˆë²¨ì—… í•œê±°ë©´
	{
		GS2C_RECV_ENCYCLOPEDIA_TACTICSHOUSE recv;
		recv.nDigimonID = ((CDigimon*)pObject)->GetBaseDigimonFTID();
		recv.nLevel = (u1)cLevel;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::ENCYCLOPEDIA_LV_CHECK, &recv );
	}
	else if( pObject->IsKindOf( RTTI_TAMER_USER ) )//í…Œì´ë¨¸ê°€ ë ˆë²¨ì—… í•œê±°ë©´
	{
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
	}
}
void cCliGame::RecvHealUp(void)
{
	u4 nUID = 0;
	u2 nHP = 0; // ìµœì¢… HP
	u2 nDS = 0; // ìµœì¢… DS
	u2 nFP = 0; // ìµœì¢… FP(fatigue point)
	pop(nUID);
	pop(nHP);
	pop(nDS);
	pop(nFP);	// í…Œì´ë¨¸ì¸ ê²½ìš°ë§Œ ì˜ë¯¸ê°€ ìžˆìŒ, ë””ì§€ëª¬ì¸ ê²½ìš° 0
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
	if( pObject == NULL )
		return;
	if( pObject->GetLeafRTTI() != RTTI_DIGIMON_USER &&
		pObject->GetLeafRTTI() != RTTI_TAMER_USER )
		return;
	pObject->GetBaseStat()->SetHP( nHP );
	pObject->GetBaseStat()->SetDS( nDS );
	pObject->GetBaseStat()->SetFP( nFP );
#ifndef _GIVE
	if( pObject->GetLeafRTTI() == RTTI_TAMER_USER )
	{
		TCHAR sz[ 260 ];
		_stprintf_s( sz, 260, _T( "í˜„ìž¬ í”¼ë¡œë„ %d" ), nFP );		
		ContentsStream kStream;
		wstring str = sz;
		bool bParcing = true;	bool bCheckSameString = false;
		kStream << str << bParcing << bCheckSameString;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_SYSTEM_PROCESS, &kStream );
	}	
#endif
}
// ìžì‹ ì˜ ì •ë³´ëŠ” ìžì‹ ì—ê²ŒëŠ” ì˜¤ì§€ ì•„ë‹ˆí•¨
void cCliGame::RecvHpRate(void)
{
	u4 nUID = 0;
	u1 nHpRate = 0;
	pop(nUID);
	pop(nHpRate);	
	SetHpRate(nUID, nHpRate);
}
void cCliGame::SetHpRate(u4 nUID, u1 nHpRate)
{
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
	if( pObject == NULL )
		return;
	assert_cs( pObject->GetLeafRTTI() != RTTI_TAMER_USER );
	assert_cs( pObject->GetLeafRTTI() != RTTI_DIGIMON_USER );
	pObject->GetBaseStat()->SetHP( nHpRate );
}
void cCliGame::RecvMsgAllByItemFailure(void)	// ì™¸ì¹˜ê¸° ì‚¬ìš© ì‹¤íŒ¨
{
	int nErrCode;
	pop(nErrCode);
	switch(nErrCode)
	{
	case 0 :	// ì•„ì´í…œ ë¶€ì¡±
		return;
	case 1:		// ì•„ì´í…œ ë§Œë£Œ
		return;
	default :
		return;
	}
}
void cCliGame::RecvMsgShoutFailure(void)
{
	int nErrCode;
	pop(nErrCode);
	switch(nErrCode)
	{
	case 1 :
		{
			cPrintMsg::PrintMsg( 20027 );			
			ST_CHAT_PROTOCOL	CProtocol;
			CProtocol.m_Type = NS_CHAT::RESET_SHOUTTIME;
			GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		}		
		break;
	default :
		LOG("unknown shout failure code(%d)", nErrCode);
	}
}
void cCliGame::RecvCreateMonsterFailure(void)
{
// 	CHAT_TEXT_DEGUG( _T( "cCliGame::RecvCreateMonsterFailure...maybe type error" ) );
}
void cCliGame::RecvSetMoveSpeed(void)
{
	u2 nMoveSpeedT = 0;
	u2 nMoveSpeedD = 0;
	u4 nTamerUID = 0;
	pop(nTamerUID);
	u4 nDigimonUID0 = 0;
	pop(nDigimonUID0);
#ifdef KEYBOARD_MOVE
	if( g_pResist->m_Global.s_bMoveDigimon && nsCsGBTerrain::g_nSvrLibType != nLIB::SVR_BATTLE )
	{
		pop(nMoveSpeedD);
		pop(nMoveSpeedT);
	}
	else
	{
		pop(nMoveSpeedT);
		pop(nMoveSpeedD);
	}
#else
	pop(nMoveSpeedT);
	pop(nMoveSpeedD);
#endif
	u4 nTamerCondition = 0;
	pop(nTamerCondition);
	u4 nDigimonCondition = 0;
	pop(nDigimonCondition);
	CsC_AvObject* pTamer = g_pMngCollector->GetObject( nTamerUID );
	if( pTamer )
	{
		assert_cs( pTamer->IsKindOf( RTTI_TAMER ) );
		pTamer->GetBaseStat()->SetMoveSpeed( (float)nMoveSpeedT );
		( (CTamer*)pTamer )->SetCondition( nTamerCondition );
	}
	CsC_AvObject* pDigimon = g_pMngCollector->GetObject( nDigimonUID0 );
	if( pDigimon )
	{
		assert_cs( pDigimon->IsKindOf( RTTI_DIGIMON ) );
		pDigimon->GetBaseStat()->SetMoveSpeed( (float)nMoveSpeedD );
		( (CDigimon*)pDigimon )->SetCondition( nDigimonCondition );
	}
	ST_CHAT_PROTOCOL	CProtocol;
	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
	//CProtocol.m_wStr = GetVAString( _T( "MoveSpeed %d %d => %d %d" ), nTamerUID, nDigimonUID0, nMoveSpeedT, nMoveSpeedD );
	DmCS::StringFn::Format(CProtocol.m_wStr, _T( "MoveSpeed %d %d => %d %d" ), nTamerUID, nDigimonUID0, nMoveSpeedT, nMoveSpeedD);
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
}
void cCliGame::RecvEvoOpen(void)
{
	// ì‚¬ìš© ì•ˆë¨
	assert_cs( false );
	u2 nUID;
	u1 nEvoSlotNo;
	pop(nUID);	// digimon's uid
	pop(nEvoSlotNo);
}
void cCliGame::RecvEvoFailure(void)
{
	if( g_pDataMng )
	{
		int nSuccess = 0;
		g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CHANGE_KFM, 0, &nSuccess );
	}
}
// ìžì‹  í¬í•¨ ëª¨ë“  ìœ ì €ë“¤ì˜ ì§„í™”ë¥¼ ë°›ëŠ”ë‹¤
void cCliGame::RecvEvolution(void)
{
	u4 nUID = 0;
	pop(nUID);	// digimon's uid
	u4 nTamerUID = 0;
	pop(nTamerUID);
	u4 nEvoType = 0;
	pop(nEvoType);
	u1 nEvoSlotNo = 0;
	pop(nEvoSlotNo);
	u1 nHPRate = 0;
	pop(nHPRate);
#ifdef SDM_DIGIMON_PARTSSYSTEM_20200115
	u4 nPartsType = 0;
	pop( nPartsType );	// ë””ì§€ëª¬ íŒŒì¸  ì•„ì´í…œ ì •ë³´
#endif
	cType t( nUID );
	CDigimon* pDigimon = g_pCharMng->GetDigimon( t.m_nIDX );	// ìžì‹ ì´ë©´ DigimonUser
	if( pDigimon != NULL )
	{
		if( nEvoSlotNo == nEvo::BurstMode )
		{
			pDigimon->Scene_Evol( nEvoType, true );
			// ì§„í™” ì¤‘ì¼ë•Œë§Œ í…Œì´ë¨¸ ì• ë‹ˆë©”ì´ì…˜ ë³´ì—¬ì£¼ìž
			t.m_nUID = nTamerUID;
			CTamer* pTamer = g_pCharMng->GetTamer( t.m_nIDX );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, nEvoType );
			}
		}
		else if( nEvoSlotNo < nLimit::EvoStep - 1 )
		{
			pDigimon->Scene_Evol( nEvoType, false );
			// ì§„í™” ì¤‘ì¼ë•Œë§Œ í…Œì´ë¨¸ ì• ë‹ˆë©”ì´ì…˜ ë³´ì—¬ì£¼ìž
			t.m_nUID = nTamerUID;
			CTamer* pTamer = g_pCharMng->GetTamer( t.m_nIDX );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, nEvoType );
			}
		}
		else
		{
			pDigimon->Scene_Degenerate( nEvoType );			
		}
		if( pDigimon->GetLeafRTTI() == RTTI_DIGIMON )
		{
			pDigimon->GetBaseStat()->SetHP( nHPRate );
		}
		// ìœ„ì— Scene_Evol()í•¨ìˆ˜ë‹¤ìŒì— ë°˜ë“œì‹œ í˜¸ì¶œí•´ì•¼í•œë‹¤. Scene_Evol()í•¨ìˆ˜ì•ˆì— êµ¬í˜„í•˜ê±°ë‚˜..
		if( pDigimon->GetLeafRTTI() == RTTI_DIGIMON_USER )
		{
			GS2C_RECV_CHECKTYPE recv;
			recv.nType = AchieveContents::CA_Partner_Evo_JointProgress;
			recv.nValue1 = nEvoType;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_CHECKTYPE, &recv );
		}
#ifdef SDM_DIGIMON_PARTSSYSTEM_20200115
		// ì´ì „ ì´íŽ™íŠ¸ íŒŒì¸  ì •ë³´ ì œê±°
		pDigimon->ClearAllPostDParts();
		// ì´íŽ™íŠ¸ íŒŒì¸  ì •ë³´ ì¶”ê°€
		pDigimon->SetPostDParts( nPartsType );
#endif
	}	
#ifdef _DEBUG
	ST_CHAT_PROTOCOL	CProtocol;
	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
	//CProtocol.m_wStr = GetVAString( _T( "EVO nUID(%d) nEvoSlotNo(%d)" ), nUID, nEvoSlotNo );
	DmCS::StringFn::Format(CProtocol.m_wStr, _T( "EVO nUID(%d) nEvoSlotNo(%d)" ), nUID, nEvoSlotNo);
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
#endif //_DEBUG
}
void cCliGame::RecvChangeStatValue(void)
{
	// ì•ˆì“°ìž„
	assert_cs( false );
	// ìžê¸° ìžì‹  ( í…Œì´ë¨¸ + ë””ì§€ëª¬ )
// 	u2 nUID;
// 	u1 nStat;
// 	n4 nValue;
// 
// 	pop(nUID);
// 	pop(nStat);
// 	pop(nValue);
// 
// 	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );
// 	if( pObject == NULL )
// 		return;
	/*switch( nStat )
	{
	case HP:	pObject->GetBaseStat()->SetHP( nValue );		break;
	case DS:	pObject->GetBaseStat()->SetDS( nValue );		break;
	case DP:	pAttObject->m_nDefence = nValue;			break;
	case AP:	pAttObject->m_nAttPower = nValue;			break;
	case EV:	pAttObject->m_nEvade = nValue;				break;
	case CA:	pAttObject->m_nCriticalRate = nValue;		break;
	case MS:	pAttObject->m_fSpeed = (float)nValue;		break;
	case AS:	pAttObject->m_fAttSpeed = nValue*0.001f;	break;
	case AR:	pAttObject->m_fAttRange = (float)nValue;	break;
	case MHP:	if(nValue < pAttObject->m_nHP)
				{
					pAttObject->m_nHP = nValue;
				}
				pAttObject->m_nMaxHP = nValue;				break;
	case MDS:	if(nValue < pAttObject->m_nDS)
				{
					pAttObject->m_nDS = nValue;
				}
				pAttObject->m_nMaxDS = nValue;				break;
	default :	assert_cs( false );							break;
	}*/
}
//===============================================================================================
//
// ì–´íƒì •ë³´ ì„¤ì •
//
//===============================================================================================
void cCliGame::SetAttackInfo( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo, bool bLastAttack )
{
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter && ( pHitter->IsLoad() == false ) )
		pHitter = NULL;
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	ë‘˜ë‹¤ ì•ˆë³´ì¼ë•Œ
	//==============================================================================
	if( ( pHitter == NULL )||( pTarget == NULL ) )
		return;	
	assert_cs( pHitter->GetProp_Animation()->GetSequenceMng() );
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );
	
	// ì •ë³´êµ¬í•¨ - ì›ê±°ë¦¬ê·¼ê±°ë¦¬ì¸ì§€, ì• ë‹ˆë©”ì´ì…˜ ë¨¸í•´ì•¼í•˜ëŠ”ì§€, ì• ë‹ˆë©”ì´ì…˜ í”Œë ˆì´ ê°€ì†ë„
	pHitter->DeletePath();
	pHitterInfo->CalInfo( pHitter );
	
	// ì¼ë°˜ ê³µê²© ì´ë²¤íŠ¸êµ¬í•¨
	CsC_AniProp* pHitterAniProp = pHitter->GetProp_Animation();
	int nEventCount = 1;
	CsVectorPB< int >* pEventArray; 
	pHitterAniProp->GetNAttEventIndex( pHitterInfo->s_AniID, &pEventArray, nEventCount );
// 	assert_csm1( pEventArray->Size() > 0, _T( "ê³µê²©ì´ë²¤íŠ¸ê°€ ì•ˆë“¤ì–´ê°€ ìžˆì–´ìš” ID = %d" ), pHitterInfo->s_AniID );
	if( pEventArray->Size() <= 0)
	{
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString( _T("ê³µê²©ì´ë²¤íŠ¸ê°€ ì•ˆë“¤ì–´ê°€ ìžˆì–´ìš” ID = %d"), pHitterInfo->s_AniID );
		DmCS::StringFn::Format(CProtocol.m_wStr, _T("ê³µê²©ì´ë²¤íŠ¸ê°€ ì•ˆë“¤ì–´ê°€ ìžˆì–´ìš” ID = %d"), pHitterInfo->s_AniID);
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
	}
	//==============================================================================
	// ë–„ë¦¬ëŠ”ë„˜ì˜ ì• ë‹ˆë©”ì´ì…˜ ì…‹íŒ…ê°’ì„ êµ¬í•˜ìž
	//==============================================================================	
	CsVectorPB< float > vpEventTime( 10 );
	for( int i=0; i<nEventCount; ++i )
	{
		vpEventTime.PushBack( pHitter->GetProp_Animation()->CalEventTime( pHitterInfo->s_AniID, pEventArray->GetData( i ), pHitterInfo->s_fAniAccelRate ) );
	}
	// ë ˆì¸ì§€ íƒ€ìž…ì´ë¼ë©´
	if( pHitterInfo->s_bRangeAttack )
	{
		float fDist = ( pHitter->GetPos2D() - pTarget->GetPos2D() ).Length();
		float fHitTimeVelocity = CsC_AttackProp::GetNormalRangeVelocity();
		float fHitTimeAccelVelocity = CsC_AttackProp::GetNormalRangeAccelVelocity();
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			fHitTimeVelocity *= BATTLE_SPEED_RANGE;
			fHitTimeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		// ì¶©ëŒ ì‹œê°„
		float fRangeHitTime = CsC_AttackProp::CalRangeHitTime( fHitTimeVelocity, fHitTimeAccelVelocity, fDist );
		for( int i=0; i<nEventCount; ++i )
		{
			vpEventTime[ i ] += fRangeHitTime;
			if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_Miss )
				vpEventTime[ i ] = CsMax( 0.0f, vpEventTime [ i ] - 0.3f );
		}			
	}
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sDAMAGE_INFO* pDamageInfo = NULL;
	// ë°ë¯¸ì§€ ê³„ì‚°
	float fMin = 45.0f;
	float fMax = 50.0f;	
	// ë°ë¯¸ì§€ ë‚ ë¼ê°€ëŠ” ë°©í–¥ ê³„ì‚°
	NiPoint3 v = pTarget->GetPos() - pHitter->GetPos();
	v.z = 0.0f;
	v.Unitize();
	float fRot = NiACos( NiPoint3( 0.0f, -1.0f, 0.0f ).Dot( v ) );
	if( NiPoint3( 0.0f, -1.0f, 0.0f ).Cross( v ).z > 0 )
		fRot = -fRot;
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pHitter->GetLeafRTTI() == RTTI_MONSTER || pHitter->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::MONSTER_ATTACK : NUMTYPE::DIGIMON_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_NONE_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;	
	DInfo.s_eMarbleType = CsC_AttackProp::MB_NONE;
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		DInfo.s_eMarbleType |= CsC_AttackProp::MB_EVOL;		
	}
	
	int nTotalValue[ 5 ];
	int nTotalDemage = 0;
	memcpy( nTotalValue, pTargetInfo->s_nValue, sizeof( int )*5 );
	for( int i=nEventCount-1; i>-1; --i )
	{
		if( i == 0 )
		{
			memcpy( DInfo.s_nValue, nTotalValue, sizeof( int )*5 );
		}
		else
		{
			// ëžœë¤ ê°’ìœ¼ë¡œ ë°ë¯¸ì§€ íƒ€ê²©íšŸìˆ˜ë¡œ ë‚˜ëˆ„ì–´ ì¤€ë‹¤. - ê²°ê³¼ ê°’ì€ ê°™ê²Œ ë§Œë“¤ìž
			float fCalDamageRate = 0.0f;
			int nRandRate = static_cast<int>(fMax - fMin);
			if( 0 == nRandRate )
				fCalDamageRate = fMin * 0.01f;
			else
				fCalDamageRate = ( rand()%nRandRate + fMin )*0.01f;
			// ë§ˆì§€ë§‰ê°’ì´ ì¡°ê¸ˆ í¬ê²Œ ë§Œë“¤ìž
			if( i == nEventCount-1 )
			{
				fMin = 50.0f/( nEventCount - 1);
				fMax = 70.0f/( nEventCount - 1);
			}
			for( int t=0; t<5; ++t )
			{
				DInfo.s_nValue[ t ] = (int)( pTargetInfo->s_nValue[ t ]*fCalDamageRate );
				if( DInfo.s_nValue[ t ] < 0 )
				{
					if( DInfo.s_nValue[ t ] < nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				else
				{
					if( DInfo.s_nValue[ t ] > nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				nTotalValue[ t ] -= DInfo.s_nValue[ t ];
			}			
		}	
		//ë°ë¯¸ì§€ ê³„ì‚°
		nTotalDemage += DInfo.s_nValue[ 0 ];
		// ëžœë¤ ì ìš©
		switch( DInfo.s_nNumEffect )
		{
		case NUMTYPE::ET_NORMAL:
			DInfo.s_fDamageRot = fRot + CsD2R( rand()%70 - 35 );
			break;
		case NUMTYPE::ET_CRITICAL:
			DInfo.s_fDamageRot = 0.0f;
			break;
		default:
			assert_cs( false );
		}
		DInfo.s_fHitEventTime = vpEventTime[ i ];
		DInfo.s_dwResistTime = GetTickCount() + (DWORD)( DInfo.s_fHitEventTime*2000.0f );		// í‹±ì´ë¯€ë¡œ 1000 ì„ ë” ê³±í•˜ìž
		DInfo.s_pChildDamageInfo = pDamageInfo;
		CsC_AttackProp* pAtProp = pTarget->GetProp_Attack();
		if( pAtProp )
		{
// 			if( bLastAttack )
// 				pAtProp->DeleteDemageEvent();
			pDamageInfo = pAtProp->InsertDamage( &DInfo );
		}
		DInfo.s_nNumEffect = NUMTYPE::ET_NORMAL;
	}
	// íƒ€ê²Ÿ ì°ì–´ ë†“ì€ê²Œ ì—†ì„ë•Œ
	if( pTarget->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		if( g_pCharResMng->GetTargetMark() == NULL )
			g_pCharResMng->SetTargetMark( pHitter );
	}	
	// ë°ë¯¸ì§€ ì „íˆ¬ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime=vpEventTime[ nEventCount - 1 ];
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem << bLastAttack;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
	//==============================================================================
	// ë•Œë¦°ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sHIT_INFO TInfo;
	TInfo.s_eHitType = pHitterInfo->s_HitType;
	TInfo.s_dwAniID = pHitterInfo->s_AniID;	
	TInfo.s_fAniAccelRate = pHitterInfo->s_fAniAccelRate;
	TInfo.s_pPartnerDamageInfo = pDamageInfo;
	TInfo.s_pHitTarget = pTarget;
	TInfo.s_fRangeVelocity = CsC_AttackProp::GetNormalRangeVelocity();
	TInfo.s_fRangeAccelVelocity = CsC_AttackProp::GetNormalRangeAccelVelocity();
	TInfo.s_bRangeAttack = pHitterInfo->s_bRangeAttack;
	// ë°ë¯¸ì§€ ë°˜ì‚¬ ë²„í”„ ìŠ¤í‚¬ ì¶”ê°€_14.05.14		chu8820
	TInfo.s_nReflectType = pHitterInfo->s_nReflectType;
	CsMapMonster* tmpMon = nsCsMapTable::g_pMapMonsterMng->GetGroup(nsCsGBTerrain::g_pCurRoot->GetInfo()->s_dwMapID)->GetMonster_ByMonsterID(	pHitter->GetFTID());
	if(tmpMon != NULL && tmpMon->GetInfo()->s_nMoveType == 4)//ì˜¤ë¸Œì íŠ¸í˜• ì²´í¬. ì˜¤ë¸Œì íŠ¸í˜•ì€ ì•ˆëŒê³  ì•ˆë”°ë¼ê°€ì•¼ë˜
		TInfo.s_bRotation = false;//ëŒì•„ë³¼êº¼ëƒ
	else
		TInfo.s_bRotation = true;
	TInfo.s_vDamagePos = pTarget->GetPos();
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		TInfo.s_fRangeVelocity *= BATTLE_SPEED_RANGE;
		TInfo.s_fRangeAccelVelocity *= BATTLE_SPEED_RANGE;
	}
	switch( g_pResist->m_Global.s_eAttackEffectViewMode )
	{
	case cResist::sGLOBAL::AEM_ALL_OFF:			// ëžœë”ë§ ìžì²´ë¥¼ ì•ˆí•˜ê² ë‹¤
		TInfo.s_nAttackEffectIndex = -1;
		break;
	case cResist::sGLOBAL::AEM_ONLY_NORATT_ON:	// ì¼ë°˜ íƒ€ê²©ì´íŽ™íŠ¸ë§Œ ëžœë”ë§
		TInfo.s_nAttackEffectIndex = 0;
		break;
	case cResist::sGLOBAL::AEM_ALL_ON:
		TInfo.s_nAttackEffectIndex = pHitter->IsStrongNature2( pTarget ) ? pHitter->GetNatureType2() : 0;
		break;
	default:
		assert_cs( false );
	}	
	CsC_AttackProp* pAtProp = pHitter->GetProp_Attack();
	if( pAtProp )
	{
		if( bLastAttack )
			pAtProp->DeleteHitEvent();
		pAtProp->InsertHit( &TInfo );
	}
}
//===============================================================================================
//
// ìŠ¤í‚¬ì •ë³´ ì„¤ì • ê´‘ì—­ ìŠ¤í‚¬í• ë•Œ
//
//===============================================================================================
void cCliGame::SetDamage( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo )
{
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter && ( pHitter->IsLoad() == false ) )
		pHitter = NULL;	
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	íƒ€ê²Ÿì€ ë°˜ë“œì‹œ ì¡´ìž¬í•´ì•¼í•¨
	//==============================================================================
	if( pTarget == NULL )
		return;
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );	
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================	
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pTarget->GetLeafRTTI() == RTTI_MONSTER || pTarget->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::DIGIMON_ATTACK : NUMTYPE::MONSTER_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;			
	DInfo.s_eMarbleType = CsC_AttackProp::MB_NONE;			
	memcpy( DInfo.s_nValue, pTargetInfo->s_nValue, sizeof( int )*5 );	
	// ëžœë¤ ì ìš©
	DInfo.s_fDamageRot = 0.0f;
	DInfo.s_fHitEventTime = 0.0f;
	DInfo.s_pChildDamageInfo = NULL;
	pTarget->GetProp_Attack()->InsertDamage( &DInfo );		
	//í† íƒˆë°ë¯¸ì§€ ê³„ì‚°	
	int nTotalDemage = 0;
	nTotalDemage += DInfo.s_nValue[ 0 ];
	// ë°ë¯¸ì§€ ì „íˆ¬ ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime=0.0f;
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(pHitterInfo->s_nUID, pTargetInfo->s_nUID, nTotalDemage);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
#endif
}
void cCliGame::SetSkillInfo2( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo, bool bLastAttack, u4 nTargetUID, int nSkillSpeed, int nSkillAceel  )
{
	if( bLastAttack )
	{
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		CProtocol.m_wStr = _T("SetSkillInfo2 bLastAttack");
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
	}
	// ë•Œë¦°ë†ˆê³¼ íƒ€ê²Ÿì„ ì–»ëŠ”ë‹¤.
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter )
	{	
		// ë•Œë¦¬ëŠ”ë†ˆ ë¡œë“œ ë˜ì´ì•Šì•˜ìœ¼ë©´?
		if( pHitter->IsLoad() == false )
		{
			// ë•Œë¦¬ëŠ” ë†ˆì´ ë‚´ë””ì§€ëª¬ì´ë©´.
			if( pHitter->GetLeafRTTI() == RTTI_DIGIMON_USER )
			{
				((CDigimonUser*)pHitter)->RecvSkill( false, pHitterInfo->s_nSkillIndex );
			}
			pHitter = NULL;
		}		
	}
	// íƒ€ê²Ÿë„ ë¡œë“œë˜ì§€ ì•Šì•˜ìœ¼ë©´...
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	ë‘˜ì¤‘ì— í•˜ë‚˜ë¼ë„ ì•ˆë³´ì´ë©´ íŒ¨ìŠ¤ ??ì•ˆë³´ì´ë©´??
	//==============================================================================
	if( ( pHitter == NULL )||( pTarget == NULL ) )
		return;
	assert_cs( pHitter->GetProp_Animation()->GetSequenceMng() );
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );
	//==============================================================================
	// ë–„ë¦¬ëŠ”ë„˜ì˜ ì• ë‹ˆë©”ì´ì…˜ ì…‹íŒ…ê°’ì„ êµ¬í•˜ìž
	//==============================================================================	
	
	CsMonsterSkill::sINFO* pSkillInfo =  nsCsFileTable::g_pMonsterMng->GetMonsterSkill( pHitterInfo->s_nSkillIndex );
	if( pSkillInfo == NULL )
	{
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString( _T( "SetSkillInfo2 :: ì¡´ìž¬í•˜ì§€ ì•ŠëŠ” ëª¬ìŠ¤í„°ìŠ¤í‚¬ - SkillIdx : %d" ), pHitterInfo->s_nSkillIndex );
		DmCS::StringFn::Format(CProtocol.m_wStr, _T( "SetSkillInfo2 :: ì¡´ìž¬í•˜ì§€ ì•ŠëŠ” ëª¬ìŠ¤í„°ìŠ¤í‚¬ - SkillIdx : %d" ), pHitterInfo->s_nSkillIndex);
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	// ë°ë¯¸ì§€ ê³„ì‚° ì‹œ ìŠ¤í‚¬ ì‚¬ìš© ì• ë‹ˆ ì“°ì§€ ì•ŠëŠ” ìŠ¤í‚¬ë“¤ ì˜ˆì™¸ ì²˜ë¦¬
	bool _bHitterAni;
	switch( pSkillInfo->s_nSkillType )
	{
	case CsMonsterSkill::ATTACH_SEED:	// ì”¨ì•—ê¹”ê¸°		( ëŒ€ìƒ ë””ë²„í”„, ì‹œê°„ ê²½ê³¼ í›„ ëŒ€ìƒ ìœ„ì¹˜ ë°”ë‹¥ ìƒì„± )
	case CsMonsterSkill::BERSERK:		// ê´‘í­í™”		( ê³µëžµ ì‹œê°„ ì´ˆê³¼ )
		_bHitterAni = false;
		break;
	default:	_bHitterAni = true;		break;
	}
	DWORD dwAniID = pSkillInfo->s_dwSequenceID;	// ìŠ¤í‚¬ì¸ë±ìŠ¤ ê³„ì‚°
	int nEventCount = 1;
	CsVectorPB< int >* pEventArray;
	bool bHitterAni = true;
	bHitterAni = _bHitterAni;
	// ìŠ¤í‚¬ ì •ë³´ë¥¼ ì–»ê³  ì´ê²ƒì´ ë°€ë¦¬ëƒ ëž˜ì¸ì§€ëƒ? ê²€ì‚¬.
	pHitterInfo->s_bRangeAttack = pHitter->IsHaveRangeEvent( dwAniID );
	if( bHitterAni )
	{
		// ë•Œë¦¬ëŠ” ë„˜ì˜ ë•Œë¦¬ëŠ” ì• ë‹ˆë©”ì´ì…˜ ì •ë³´ ì–»ëŠ”ë‹¤.dwAniID ë¡œ ì•„ì›ƒ íŒŒë¼ë¯¸í„° 2ê°œ ì–»ëŠ”ë‹¤.
		if( pHitter->GetProp_Animation()->GetNAttEventIndex( dwAniID, &pEventArray, nEventCount ) == false )
		{
			g_CriticalLog.Log( _T( "SetSKillInfo : No Have Ani_1 ( model = %d, ani = %d, SkillIndex = %d )" ), pHitter->GetModelID(), dwAniID, pHitterInfo->s_nSkillIndex );
			assert_csm2( false, L"ModelID = %d, LoadSeqModelID = %d", pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
			pHitter->RecvSkill( false, pHitterInfo->s_nSkillIndex );
			return;
		}
		assert_csm3( nEventCount > 0, L"AniID = %d, ModelID = %d, LoadSeqModelID = %d", dwAniID, pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
	}
	CsVectorPB< float > vpEventTime( 10 );
	float fAccelRate = 1.0f;
	// ë°°í‹€ ëª¨ë“œì‹œ ëž˜ì´íŒ…ê°’ì„¤ì •
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		fAccelRate = BATTLE_SPEED_SKILL;
	// ì´ë°´íŠ¸ ì¹´ìš´íŠ¸ ë§Œí¼ë§Œ 
	if( bHitterAni )
	{
		for( int i=0; i<nEventCount; ++i )
		{
			vpEventTime.PushBack( pHitter->GetProp_Animation()->CalEventTime( dwAniID, pEventArray->GetData( i ), fAccelRate ) );
		}
	}
	// ë ˆì¸ì§€ íƒ€ìž…ì´ë¼ë©´
	if( pHitterInfo->s_bRangeAttack == true )
	{
		float fDist = ( pHitter->GetPos2D() - pTarget->GetPos2D() ).Length();
		float fHitTimeVelocity = (float)nSkillSpeed;
		float fHitTimeAccelVelocity = (float)nSkillAceel;
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			fHitTimeVelocity *= BATTLE_SPEED_RANGE;
			fHitTimeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		float fRangeHitTime = CsC_AttackProp::CalRangeHitTime( fHitTimeVelocity, fHitTimeAccelVelocity, fDist );
		for( int i=0; i<nEventCount; ++i )
		{
			vpEventTime[ i ] += fRangeHitTime;
		}
	}
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sDAMAGE_INFO* pDamageInfo = NULL;
	// ë°ë¯¸ì§€ ê³„ì‚°
	float fMin = 90.0f/(float)nEventCount;
	float fMax = 110.0f/(float)nEventCount;	
	// ë°ë¯¸ì§€ ë‚ ë¼ê°€ëŠ” ë°©í–¥ ê³„ì‚°
	NiPoint3 v = pTarget->GetPos() - pHitter->GetPos();	// íƒ€ê²Ÿê³¼ ì–´íƒì»¤ì™€ì˜ ë°©í–¥ ê³„ì‚°
	v.z = 0.0f;
	v.Unitize();
	float fRot = NiACos( NiPoint3( 0.0f, -1.0f, 0.0f ).Dot( v ) );
	if( NiPoint3( 0.0f, -1.0f, 0.0f ).Cross( v ).z > 0 )
		fRot = -fRot;
	// ë§žëŠ”ë„˜ì˜ ì–´íƒ í”„ë¡œí¼í‹° ë“±ë¡
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pHitter->GetLeafRTTI() == RTTI_MONSTER || pHitter->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::MONSTER_ATTACK : NUMTYPE::DIGIMON_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_NONE_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;
	DInfo.s_vHitterPos = pHitter->GetPos2D();	
	// ë„‰ë°±ì´ë¼ë©´ ê°€ìž¥ ì´ˆê¸°ì— í•œë²ˆë§Œ ì…‹íŒ…í•´ì£¼ê² ë‹¤
	if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
	{
		DInfo.s_eDamageType = CsC_AttackProp::DT_Normal;
	}	
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		DInfo.s_eMarbleType |= CsC_AttackProp::MB_EVOL;		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// ìŠ¤í‚¬ íƒ€ê²© íšŸìˆ˜ë§Œí¼ ëŒë ¤ì„œ í•˜ë‚˜ì”© í”„ë¡œí¼í‹°ì— ë“±ë¡ ì˜ˆ)ì§¤ì‹¹ë•Œë¦¬ê¸°== íšŸìˆ˜ 2ë²ˆ
	int nTotalValue[ 5 ];
	int nTotalDemage = 0;
	memcpy( nTotalValue, pTargetInfo->s_nValue, sizeof( int )*5 );
	for( int i=nEventCount-1; i>-1; --i )
	{	
		if( i == 0 )
		{
			memcpy( DInfo.s_nValue, nTotalValue, sizeof( int )*5 );
			if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
			{
				DInfo.s_eDamageType = CsC_AttackProp::DT_KnockBack;
			}
		}
		else
		{
			// ëžœë¤ ê°’ìœ¼ë¡œ ë°ë¯¸ì§€ íƒ€ê²©íšŸìˆ˜ë¡œ ë‚˜ëˆ„ì–´ ì¤€ë‹¤. - ê²°ê³¼ ê°’ì€ ê°™ê²Œ ë§Œë“¤ìž
			float fCalDamageRate = 0.0f;
			int nRandRate = static_cast<int>(fMax - fMin);
			if( 0 == nRandRate )
				fCalDamageRate = fMin * 0.01f;
			else
				fCalDamageRate = ( rand()%nRandRate + fMin )*0.01f;
			for( int t=0; t<5; ++t )
			{
				DInfo.s_nValue[ t ] = (int)( pTargetInfo->s_nValue[ t ]*fCalDamageRate );
				if( DInfo.s_nValue[ t ] < 0 )
				{
					if( DInfo.s_nValue[ t ] < nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				else
				{
					if( DInfo.s_nValue[ t ] > nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				nTotalValue[ t ] -= DInfo.s_nValue[ t ];
			}			
		}	
		//í† íƒˆë°ë¯¸ì§€ ê³„ì‚°
		nTotalDemage += DInfo.s_nValue[ 0 ];
		// ëžœë¤ ì ìš©
		DInfo.s_fDamageRot = fRot + CsD2R( rand()%70 - 35 );
		DInfo.s_fHitEventTime = vpEventTime[ i ];
		DInfo.s_dwResistTime = GetTickCount();// + (DWORD)( DInfo.s_fHitEventTime*2000.0f );		// í‹±ì´ë¯€ë¡œ 1000 ì„ ë” ê³±í•˜ìž
		DInfo.s_pChildDamageInfo = pDamageInfo;
		// í”„ë¡œí¼í‹° ë“±ë¡
		pDamageInfo = pTarget->GetProp_Attack()->InsertDamage( &DInfo );		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	if( pTarget->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		if( g_pCharResMng->GetTargetMark() == NULL )
			g_pCharResMng->SetTargetMark( pHitter );
	}
	// ë°ë¯¸ì§€ ì „íˆ¬ ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime=vpEventTime[ nEventCount - 1 ];
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem << bLastAttack;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
	if(pTargetInfo->s_nUID == nTargetUID/* && ( bHitterAni || bLastAttack )*/ )
	{
		//==============================================================================
		// ë•Œë¦°ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
		//==============================================================================
		CsC_AttackProp::sHIT_INFO info;
		switch( pSkillInfo->s_nSkillType )
		{
		case CsMonsterSkill::ATTACH_SEED:	// ì”¨ì•—ê¹”ê¸°		( ëŒ€ìƒ ë””ë²„í”„, ì‹œê°„ ê²½ê³¼ í›„ ëŒ€ìƒ ìœ„ì¹˜ ë°”ë‹¥ ìƒì„± )
		case CsMonsterSkill::BERSERK:		// ê´‘í­í™”		( ê³µëžµ ì‹œê°„ ì´ˆê³¼ )
			info.s_eHitType = CsC_AttackProp::HT_Skill_NoAni;
			break;
		default:
			info.s_eHitType = CsC_AttackProp::HT_Skill;
			break;
		}
		info.s_dwAniID = dwAniID;	
		info.s_fAniAccelRate = fAccelRate;
		info.s_pPartnerDamageInfo = pDamageInfo;
		info.s_pHitTarget = pTarget;
		info.s_fRangeVelocity = (float)nSkillSpeed;
		info.s_fRangeAccelVelocity = (float)nSkillAceel;
		info.s_bRangeAttack = pHitterInfo->s_bRangeAttack;
		info.s_bRotation = true;
		info.s_nAttackEffectIndex = 0;
		switch( g_pResist->m_Global.s_eAttackEffectViewMode )
		{
		case cResist::sGLOBAL::AEM_ALL_OFF:			// ëžœë”ë§ ìžì²´ë¥¼ ì•ˆí•˜ê² ë‹¤
			info.s_nAttackEffectIndex = -1;
			break;
		case cResist::sGLOBAL::AEM_ONLY_NORATT_ON:	// ì¼ë°˜ íƒ€ê²©ì´íŽ™íŠ¸ë§Œ ëžœë”ë§
			info.s_nAttackEffectIndex = 0;
			break;
		case cResist::sGLOBAL::AEM_ALL_ON:
			info.s_nAttackEffectIndex = pHitter->IsStrongNature2( pTarget ) ? pHitter->GetNatureType2() : 0;
			break;
		}
		info.s_vDamagePos = pTarget->GetPos();
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			info.s_fRangeVelocity *= BATTLE_SPEED_RANGE;
			info.s_fRangeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		// í”„ë¡œí¼í‹° ë“±ë¡
		pHitter->GetProp_Attack()->InsertHit( &info );
		///////////////////////////////////////////////////////////////////////////////////////////////
		//pHitter->RecvSkill( bHitterAni, pHitterInfo->s_nSkillIndex );
		if( bHitterAni )
			pHitter->RecvSkill( true, pHitterInfo->s_nSkillIndex );	
	}
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(pHitterInfo->s_nUID, pTargetInfo->s_nUID, nTotalDemage);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
#endif
}
void cCliGame::RecvRaidRankingList()
{
	// ìˆœìœ„ ê°œìˆ˜ Count
	// ìˆœìœ„
	// í…Œì´ë¨¸
	// ë””ì§€ëª¬
	// ë°ë¯¸ì§€
	/*
	ì¹´ìš´íŠ¸ u4
	ìˆœìœ„   u4
	í…Œì´ë¨¸ì´ë¦„ char[22]
	ë””ì§€ëª¬ì´ë¦„ char[22]
	ë°ë¯¸ì§€ u4
	*/
	u4 Count;
	u4 Rank;
	char szTamer[ Language::pLength::id + 1 ];
	char szDigimon[ Language::pLength::id + 1 ];
	u4 Damage;
	TCHAR	tszTamer[ Language::pLength::id + 1 ];
	TCHAR	tszDigimon[ Language::pLength::id + 1 ];
	if(g_pGameIF->IsActiveWindow( cBaseWindow::WT_RAIDRANK ) == false)
	{
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_RAIDRANK );
	}
	g_pGameIF->GetRaidRank()->ResetRankList();
	pop( Count );
	for( u4 i=0; i < Count; i++)
	{
		pop( Rank );
		pop( szTamer );
		pop( szDigimon );
		pop( Damage );
		
		_tcscpy_s( tszTamer, Language::pLength::id + 1, LanConvertT( szTamer ) );
		_tcscpy_s( tszDigimon, Language::pLength::id + 1, LanConvertT( szDigimon ) );
		if(1==Rank)
			g_pGameIF->GetRaidRank()->SetRanker(i,Rank, tszTamer , tszDigimon, Damage, NiColor(CsB2F(180), CsB2F(175), CsB2F(94)));
		else if( _tcscmp( g_pCharMng->GetTamerUser()->GetName(), tszTamer ) == 0)
			g_pGameIF->GetRaidRank()->SetRanker(i,Rank, tszTamer , tszDigimon, Damage, NiColor(CsB2F(252), CsB2F(255), CsB2F(25)));
		else
			g_pGameIF->GetRaidRank()->SetRanker(i,Rank, tszTamer , tszDigimon, Damage);
	
	}	 
}
//===============================================================================================
//
// ìŠ¤í‚¬ì •ë³´ ì„¤ì •
//
//===============================================================================================
void cCliGame::SetSkillInfo( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo, bool bLastAttack )
{
	// ë•Œë¦°ë†ˆê³¼ íƒ€ê²Ÿì„ ì–»ëŠ”ë‹¤.
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter )
	{	
		// ë•Œë¦¬ëŠ”ë†ˆ ë¡œë“œ ë˜ì´ì•Šì•˜ìœ¼ë©´?
		if( pHitter->IsLoad() == false )
		{
			// ë•Œë¦¬ëŠ” ë†ˆì´ ë‚´ë””ì§€ëª¬ì´ë©´.
			if( pHitter->GetLeafRTTI() == RTTI_DIGIMON_USER )
			{
				((CDigimonUser*)pHitter)->RecvSkill( false, pHitterInfo->s_nSkillIndex );
			}
			pHitter = NULL;
		}		
	}
	// íƒ€ê²Ÿë„ ë¡œë“œë˜ì§€ ì•Šì•˜ìœ¼ë©´...
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	ë‘˜ì¤‘ì— í•˜ë‚˜ë¼ë„ ì•ˆë³´ì´ë©´ íŒ¨ìŠ¤ ??ì•ˆë³´ì´ë©´??
	//==============================================================================
	if( ( pHitter == NULL )||( pTarget == NULL ) )
		return;
	assert_cs( pHitter->GetProp_Animation()->GetSequenceMng() );
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );
	// ë•Œë¦°ë†ˆì— ëŒ€í•œ ìŠ¤í‚¬ì •ë³´ë¥¼ ì–»ëŠ”ë‹¤.
	CsSkill::sINFO* pFTSkillInfo = NULL;
	CsTamerSkill* pTamerSkill = NULL;
	switch( pHitter->GetLeafRTTI() )
	{
	case RTTI_TUTORIAL_DIGIMON:
	case RTTI_DIGIMON:
	case RTTI_DIGIMON_USER:
		{
			if( ( (CDigimon*)pHitter )->GetSkillMng()->IsSkill( pHitterInfo->s_nSkillIndex ) == false )
			{
				g_CriticalLog.Log( _T( "SetSKillInfo_1 : ìŠ¤í‚¬í…Œì´ë¸”ì— ì¡´ìž¬í•˜ì§€ë„ ì•ŠëŠ” ìŠ¤í‚¬ ( model = %d, SkillIndex = %d )" ), pHitter->GetModelID(), pHitterInfo->s_nSkillIndex );
				return;
			}
			pFTSkillInfo = ( (CDigimon*)pHitter )->GetSkillMng()->GetFTSkill( pHitterInfo->s_nSkillIndex )->GetInfo();
		}
		break;
	case RTTI_TUTORIAL_TAMER:
	case RTTI_TAMER:
	case RTTI_TAMER_USER:
		{
			pTamerSkill = nsCsFileTable::g_pSkillMng->GetTamer_Skill( pHitterInfo->s_nSkillIndex, -1 );
			assert_csm( nsCsFileTable::g_pSkillMng->IsTamer_Skill( pTamerSkill->Get_TSkill_Info()->s_dwSkillCode ) == true, _T( "ì˜¬ë°”ë¥¸ í…Œì´ë¨¸ ìŠ¤í‚¬ì½”ë“œê°€ ì•„ë‹™ë‹ˆë‹¤.\n ë„ì§€ ë§ê³  ê¹€ìš°ë´‰ì”¨ë¥¼ ì°¾ì•„ì£¼ì„¸ìš”!!" ) );
			pFTSkillInfo = nsCsFileTable::g_pSkillMng->GetSkill( pTamerSkill->Get_TSkill_Info()->s_dwSkillCode )->GetInfo();
		}
		break;
	default:
		assert_cs( false );
	}
	
	// ìŠ¤í‚¬ ì •ë³´ë¥¼ ì–»ê³  ì´ê²ƒì´ ë°€ë¦¬ëƒ ëž˜ì¸ì§€ëƒ? ê²€ì‚¬.
	pHitterInfo->s_bRangeAttack = ( pFTSkillInfo->s_nAttType == 2 );
	//==============================================================================
	// ë–„ë¦¬ëŠ”ë„˜ì˜ ì• ë‹ˆë©”ì´ì…˜ ì…‹íŒ…ê°’ì„ êµ¬í•˜ìž
	//==============================================================================
	DWORD dwAniID = 0 ;
	if( pHitter->GetLeafRTTI() == RTTI_TAMER || pHitter->GetLeafRTTI() == RTTI_TAMER_USER )	// ë•Œë¦¬ëŠ”ë„˜ì´ í…Œì´ë¨¸ë¼ë©´
		dwAniID = pTamerSkill->Get_TSkill_Info()->s_dwTamer_SeqID;
	else
		dwAniID = ANI::ATTACK_SKILL1 + pHitterInfo->s_nSkillIndex;	// ìŠ¤í‚¬ì¸ë±ìŠ¤ ê³„ì‚°
	int nEventCount = 1;
	CsVectorPB< int >* pEventArray;
	// ë•Œë¦¬ëŠ” ë„˜ì˜ ë•Œë¦¬ëŠ” ì• ë‹ˆë©”ì´ì…˜ ì •ë³´ ì–»ëŠ”ë‹¤.dwAniID ë¡œ ì•„ì›ƒ íŒŒë¼ë¯¸í„° 2ê°œ ì–»ëŠ”ë‹¤.
	if( pHitter->GetProp_Animation()->GetNAttEventIndex( dwAniID, &pEventArray, nEventCount ) == false )
	{
		g_CriticalLog.Log( _T( "SetSKillInfo : No Have Ani_2 ( model = %d, ani = %d, SkillIndex = %d )" ), pHitter->GetModelID(), dwAniID, pHitterInfo->s_nSkillIndex );
		assert_csm2( false, L"ModelID = %d, LoadSeqModelID = %d", pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
		pHitter->RecvSkill( false, pHitterInfo->s_nSkillIndex );
		return;
	}
	assert_csm3( nEventCount > 0, L"AniID = %d, ModelID = %d, LoadSeqModelID = %d", dwAniID, pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
	CsVectorPB< float > vpEventTime( 10 );
	float fAccelRate = 1.0f;
	// ë°°í‹€ ëª¨ë“œì‹œ ëž˜ì´íŒ…ê°’ì„¤ì •
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		fAccelRate = BATTLE_SPEED_SKILL;
	// ì´ë°´íŠ¸ ì¹´ìš´íŠ¸ ë§Œí¼ë§Œ 
	for( int i=0; i<nEventCount; ++i )
	{
		vpEventTime.PushBack( pHitter->GetProp_Animation()->CalEventTime( dwAniID, pEventArray->GetData( i ), fAccelRate ) );
	}
	// ë ˆì¸ì§€ íƒ€ìž…ì´ë¼ë©´
	if( pHitterInfo->s_bRangeAttack == true )
	{
		float fDist = ( pHitter->GetPos2D() - pTarget->GetPos2D() ).Length();
		float fHitTimeVelocity = pFTSkillInfo->s_fSkill_Velocity;
		float fHitTimeAccelVelocity = pFTSkillInfo->s_fSkill_Accel;
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			fHitTimeVelocity *= BATTLE_SPEED_RANGE;
			fHitTimeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		float fRangeHitTime = CsC_AttackProp::CalRangeHitTime( fHitTimeVelocity, fHitTimeAccelVelocity, fDist );
		for( int i=0; i<nEventCount; ++i )
		{
			vpEventTime[ i ] += fRangeHitTime;
		}
	}
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sDAMAGE_INFO* pDamageInfo = NULL;
	// ë°ë¯¸ì§€ ê³„ì‚°
	float fMin = 90.0f/(float)nEventCount;
	float fMax = 110.0f/(float)nEventCount;	
	// ë°ë¯¸ì§€ ë‚ ë¼ê°€ëŠ” ë°©í–¥ ê³„ì‚°
	NiPoint3 v = pTarget->GetPos() - pHitter->GetPos();	// íƒ€ê²Ÿê³¼ ì–´íƒì»¤ì™€ì˜ ë°©í–¥ ê³„ì‚°
	v.z = 0.0f;
	v.Unitize();
	float fRot = NiACos( NiPoint3( 0.0f, -1.0f, 0.0f ).Dot( v ) );
	if( NiPoint3( 0.0f, -1.0f, 0.0f ).Cross( v ).z > 0 )
		fRot = -fRot;
	// ë§žëŠ”ë„˜ì˜ ì–´íƒ í”„ë¡œí¼í‹° ë“±ë¡
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pHitter->GetLeafRTTI() == RTTI_MONSTER || pHitter->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::MONSTER_ATTACK : NUMTYPE::DIGIMON_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_NONE_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;
	DInfo.s_vHitterPos = pHitter->GetPos2D();	
	// ë„‰ë°±ì´ë¼ë©´ ê°€ìž¥ ì´ˆê¸°ì— í•œë²ˆë§Œ ì…‹íŒ…í•´ì£¼ê² ë‹¤
	if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
	{
		DInfo.s_eDamageType = CsC_AttackProp::DT_Normal;
	}	
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		DInfo.s_eMarbleType |= CsC_AttackProp::MB_EVOL;		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// ìŠ¤í‚¬ íƒ€ê²© íšŸìˆ˜ë§Œí¼ ëŒë ¤ì„œ í•˜ë‚˜ì”© í”„ë¡œí¼í‹°ì— ë“±ë¡ ì˜ˆ)ì§¤ì‹¹ë•Œë¦¬ê¸°== íšŸìˆ˜ 2ë²ˆ
	int nTotalValue[ 5 ];
	int nTotalDemage = 0;
	memcpy( nTotalValue, pTargetInfo->s_nValue, sizeof( int )*5 );
	for( int i=nEventCount-1; i>-1; --i )
	{	
		if( i == 0 )
		{
			memcpy( DInfo.s_nValue, nTotalValue, sizeof( int )*5 );
			if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
			{
				DInfo.s_eDamageType = CsC_AttackProp::DT_KnockBack;
			}
		}
		else
		{
			// ëžœë¤ ê°’ìœ¼ë¡œ ë°ë¯¸ì§€ íƒ€ê²©íšŸìˆ˜ë¡œ ë‚˜ëˆ„ì–´ ì¤€ë‹¤. - ê²°ê³¼ ê°’ì€ ê°™ê²Œ ë§Œë“¤ìž		
			float fCalDamageRate = 1.0f; 
			(1.0f > (fMax - fMin ) ) ?  fCalDamageRate = fMin *0.01f : fCalDamageRate =  ( rand()%(int)( fMax - fMin ) + fMin )*0.01f;		
			for( int t=0; t<5; ++t )
			{
				DInfo.s_nValue[ t ] = (int)( pTargetInfo->s_nValue[ t ]*fCalDamageRate );
				if( DInfo.s_nValue[ t ] < 0 )
				{
					if( DInfo.s_nValue[ t ] < nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				else
				{
					if( DInfo.s_nValue[ t ] > nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				nTotalValue[ t ] -= DInfo.s_nValue[ t ];
			}			
		}	
		//í† íƒˆë°ë¯¸ì§€ ê³„ì‚°
		nTotalDemage += DInfo.s_nValue[ 0 ];
		// ëžœë¤ ì ìš©
		DInfo.s_fDamageRot = fRot + CsD2R( rand()%70 - 35 );
		DInfo.s_fHitEventTime = vpEventTime[ i ];
		DInfo.s_dwResistTime = GetTickCount() + (DWORD)( DInfo.s_fHitEventTime*2000.0f );		// í‹±ì´ë¯€ë¡œ 1000 ì„ ë” ê³±í•˜ìž
		DInfo.s_pChildDamageInfo = pDamageInfo;
		
		// í”„ë¡œí¼í‹° ë“±ë¡
		pDamageInfo = pTarget->GetProp_Attack()->InsertDamage( &DInfo );		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	if( pTarget->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		if( g_pCharResMng->GetTargetMark() == NULL )
			g_pCharResMng->SetTargetMark( pHitter );
	}
	// ë°ë¯¸ì§€ ì „íˆ¬ ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime=vpEventTime[ nEventCount - 1 ];
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem << bLastAttack;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
	//==============================================================================
	// ë•Œë¦°ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sHIT_INFO info;
	info.s_eHitType = pHitterInfo->s_HitType;
	info.s_dwAniID = dwAniID;	
	info.s_fAniAccelRate = fAccelRate;
	info.s_pPartnerDamageInfo = pDamageInfo;
	info.s_pHitTarget = pTarget;
	info.s_fRangeVelocity = pFTSkillInfo->s_fSkill_Velocity;
	info.s_fRangeAccelVelocity = pFTSkillInfo->s_fSkill_Accel;
	info.s_bRangeAttack = pHitterInfo->s_bRangeAttack;
	info.s_bRotation = true;
	info.s_nAttackEffectIndex = 0;
	switch( g_pResist->m_Global.s_eAttackEffectViewMode )
	{
	case cResist::sGLOBAL::AEM_ALL_OFF:			// ëžœë”ë§ ìžì²´ë¥¼ ì•ˆí•˜ê² ë‹¤
		info.s_nAttackEffectIndex = -1;
		break;
	case cResist::sGLOBAL::AEM_ONLY_NORATT_ON:	// ì¼ë°˜ íƒ€ê²©ì´íŽ™íŠ¸ë§Œ ëžœë”ë§
		info.s_nAttackEffectIndex = 0;
		break;
	case cResist::sGLOBAL::AEM_ALL_ON:
		info.s_nAttackEffectIndex = pHitter->IsStrongNature2( pTarget ) ? pHitter->GetNatureType2() : 0;
		break;
	}
	info.s_vDamagePos = pTarget->GetPos();
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		info.s_fRangeVelocity *= BATTLE_SPEED_RANGE;
		info.s_fRangeAccelVelocity *= BATTLE_SPEED_RANGE;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// í”„ë¡œí¼í‹° ë“±ë¡
	pHitter->GetProp_Attack()->InsertHit( &info );
	///////////////////////////////////////////////////////////////////////////////////////////////
	pHitter->RecvSkill( true, pHitterInfo->s_nSkillIndex );	
	
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(pHitterInfo->s_nUID, pTargetInfo->s_nUID, nTotalDemage);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
// 	if( g_pDataMng->GetParty()->IsJoinParty() == true )
// 	{
// 		if( g_pGameIF->GetMainDigimonWindow()->IsBattle() || pHitterInfo->s_nUID == g_pCharMng->GetDigimonUser(0)->GetUniqID() )
// 		{
// 			cData_DamageMeter* pDamageMeter = g_pDataMng->GetDamageMeter();
// 			if( pDamageMeter == NULL )
// 				return;
// 			pDamageMeter->_SetDamageInfo( pHitterInfo->s_nUID, pTargetInfo->s_nUID, abs(nTotalDemage), _TIME_TS );
// 		}
// 	}
#endif
}
void cCliGame::SetSkillInfo( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo, CsC_AvObject* pTargetMark, bool bLastAttack, bool &bMaxCount, bool bRecvComp )
{
	// ë•Œë¦°ë†ˆê³¼ íƒ€ê²Ÿì„ ì–»ëŠ”ë‹¤.
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter )
	{	
		// ë•Œë¦¬ëŠ”ë†ˆ ë¡œë“œ ë˜ì´ì•Šì•˜ìœ¼ë©´?
		if( pHitter->IsLoad() == false )
		{
			// ë•Œë¦¬ëŠ” ë†ˆì´ ë‚´ë””ì§€ëª¬ì´ë©´.
			if( pHitter->GetLeafRTTI() == RTTI_DIGIMON_USER )
			{
				((CDigimonUser*)pHitter)->RecvSkill( false, pHitterInfo->s_nSkillIndex );
			}
			pHitter = NULL;
		}		
	}
	// íƒ€ê²Ÿë„ ë¡œë“œë˜ì§€ ì•Šì•˜ìœ¼ë©´...
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	ë‘˜ì¤‘ì— í•˜ë‚˜ë¼ë„ ì•ˆë³´ì´ë©´ íŒ¨ìŠ¤ ??ì•ˆë³´ì´ë©´??
	//==============================================================================
	if( ( pHitter == NULL )||( pTarget == NULL ) )
		return;
	assert_cs( pHitter->GetProp_Animation()->GetSequenceMng() );
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );
	// ë•Œë¦°ë†ˆì— ëŒ€í•œ ìŠ¤í‚¬ì •ë³´ë¥¼ ì–»ëŠ”ë‹¤.
	CsSkill::sINFO* pFTSkillInfo = NULL;
	switch( pHitter->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
	case RTTI_DIGIMON_USER:
		if( ( (CDigimon*)pHitter )->GetSkillMng()->IsSkill( pHitterInfo->s_nSkillIndex ) == false )
		{
			g_CriticalLog.Log( _T( "SetSKillInfo_3 : ìŠ¤í‚¬í…Œì´ë¸”ì— ì¡´ìž¬í•˜ì§€ë„ ì•ŠëŠ” ìŠ¤í‚¬ ( model = %d, SkillIndex = %d )" ), pHitter->GetModelID(), pHitterInfo->s_nSkillIndex );
			return;
		}
		if( pHitterInfo->s_nSkillIndex == -1 )
			pFTSkillInfo = nsCsFileTable::g_pSkillMng->GetSkill( pHitterInfo->s_nSkillCode )->GetInfo();
		else
			pFTSkillInfo = ( (CDigimon*)pHitter )->GetSkillMng()->GetFTSkill( pHitterInfo->s_nSkillIndex )->GetInfo();
		break;
	default:
		assert_cs( false );
	}
	// ìŠ¤í‚¬ ì •ë³´ë¥¼ ì–»ê³  ì´ê²ƒì´ ë°€ë¦¬ëƒ ëž˜ì¸ì§€ëƒ? ê²€ì‚¬.
	pHitterInfo->s_bRangeAttack = ( pFTSkillInfo->s_nAttType == 2 );
	//==============================================================================
	// ë–„ë¦¬ëŠ”ë„˜ì˜ ì• ë‹ˆë©”ì´ì…˜ ì…‹íŒ…ê°’ì„ êµ¬í•˜ìž
	//==============================================================================
	DWORD dwAniID;
	if( pHitterInfo->s_AniID != 0 )
	{
		dwAniID = pHitterInfo->s_AniID;
		
		if( pHitter->GetLeafRTTI() == RTTI_DIGIMON_USER )
		{
			cSkill::sINFO* pSkill_Info = g_pCharMng->GetDigimonUser(0)->GetSkillMng()->GetSkill( pHitterInfo->s_nSkillIndex );	// ì‚¬ìš©í•œ ë©”ëª¨ë¦¬ ìŠ¤í‚¬ ì •ë³´ ì–»ì–´ì˜´
			// ì¿¨íƒ€ìž„ ì‹œìž‘
			pSkill_Info->s_CoolTime.Start();
			pSkill_Info->s_CoolTime.SetEndTime( pSkill_Info->s_pFTSkill->GetInfo()->s_fCooldownTime * 0.001f + 0.15f );
		}
	}
	else
		dwAniID= ANI::ATTACK_SKILL1 + pHitterInfo->s_nSkillIndex;	// ìŠ¤í‚¬ì¸ë±ìŠ¤ ê³„ì‚°
	int nEventCount = 1;
	CsVectorPB< int >* pEventArray;
	// ë•Œë¦¬ëŠ” ë„˜ì˜ ë•Œë¦¬ëŠ” ì• ë‹ˆë©”ì´ì…˜ ì •ë³´ ì–»ëŠ”ë‹¤.dwAniID ë¡œ ì•„ì›ƒ íŒŒë¼ë¯¸í„° 2ê°œ ì–»ëŠ”ë‹¤.
	if( pHitter->GetProp_Animation()->GetNAttEventIndex( dwAniID, &pEventArray, nEventCount ) == false )
	{
		g_CriticalLog.Log( _T( "SetSKillInfo : No Have Ani_3 ( model = %d, ani = %d, SkillIndex = %d )" ), pHitter->GetModelID(), dwAniID, pHitterInfo->s_nSkillIndex );
		assert_csm2( false, L"ModelID = %d, LoadSeqModelID = %d", pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
		pHitter->RecvSkill( false, pHitterInfo->s_nSkillIndex );
		return;
	}
	assert_csm3( nEventCount > 0, L"AniID = %d, ModelID = %d, LoadSeqModelID = %d", dwAniID, pHitter->GetModelID(), pHitter->GetProp_Animation()->GetSequenceMng()->GetLoadSequence()->GetModelID() );
	CsVectorPB< float > vpEventTime( 10 );
	float fAccelRate = 1.0f;
	// ë°°í‹€ ëª¨ë“œì‹œ ëž˜ì´íŒ…ê°’ì„¤ì •
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		fAccelRate = BATTLE_SPEED_SKILL;
	// ì´ë°´íŠ¸ ì¹´ìš´íŠ¸ ë§Œí¼ë§Œ 
	for( int i=0; i<nEventCount; ++i )
	{
		vpEventTime.PushBack( pHitter->GetProp_Animation()->CalEventTime( dwAniID, pEventArray->GetData( i ), fAccelRate ) );
	}
	// ë ˆì¸ì§€ íƒ€ìž…ì´ë¼ë©´
	if( pHitterInfo->s_bRangeAttack == true )
	{
		float fDist = ( pHitter->GetPos2D() - pTarget->GetPos2D() ).Length();
		float fHitTimeVelocity = pFTSkillInfo->s_fSkill_Velocity;
		float fHitTimeAccelVelocity = pFTSkillInfo->s_fSkill_Accel;
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			fHitTimeVelocity *= BATTLE_SPEED_RANGE;
			fHitTimeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		float fRangeHitTime = CsC_AttackProp::CalRangeHitTime( fHitTimeVelocity, fHitTimeAccelVelocity, fDist );
		for( int i=0; i<nEventCount; ++i )
		{
			vpEventTime[ i ] += fRangeHitTime;
		}
	}
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================
	CsC_AttackProp::sDAMAGE_INFO* pDamageInfo = NULL;
	// ë°ë¯¸ì§€ ê³„ì‚°
	float fMin = 90.0f/(float)nEventCount;
	float fMax = 110.0f/(float)nEventCount;	
	// ë°ë¯¸ì§€ ë‚ ë¼ê°€ëŠ” ë°©í–¥ ê³„ì‚°
	NiPoint3 v = pTarget->GetPos() - pHitter->GetPos();	// íƒ€ê²Ÿê³¼ ì–´íƒì»¤ì™€ì˜ ë°©í–¥ ê³„ì‚°
	v.z = 0.0f;
	v.Unitize();
	float fRot = NiACos( NiPoint3( 0.0f, -1.0f, 0.0f ).Dot( v ) );
	if( NiPoint3( 0.0f, -1.0f, 0.0f ).Cross( v ).z > 0 )
		fRot = -fRot;
	// ë§žëŠ”ë„˜ì˜ ì–´íƒ í”„ë¡œí¼í‹° ë“±ë¡
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pHitter->GetLeafRTTI() == RTTI_MONSTER || pHitter->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::MONSTER_ATTACK : NUMTYPE::DIGIMON_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_NONE_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;
	DInfo.s_vHitterPos = pHitter->GetPos2D();	
	// ë„‰ë°±ì´ë¼ë©´ ê°€ìž¥ ì´ˆê¸°ì— í•œë²ˆë§Œ ì…‹íŒ…í•´ì£¼ê² ë‹¤
	if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
	{
		DInfo.s_eDamageType = CsC_AttackProp::DT_Normal;
	}	
	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
	{
		DInfo.s_eMarbleType |= CsC_AttackProp::MB_EVOL;		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// ìŠ¤í‚¬ íƒ€ê²© íšŸìˆ˜ë§Œí¼ ëŒë ¤ì„œ í•˜ë‚˜ì”© í”„ë¡œí¼í‹°ì— ë“±ë¡ ì˜ˆ)ì§¤ì‹¹ë•Œë¦¬ê¸°== íšŸìˆ˜ 2ë²ˆ
	int nTotalValue[ 5 ];
	int nTotalDemage = 0;
	memcpy( nTotalValue, pTargetInfo->s_nValue, sizeof( int )*5 );
	for( int i=nEventCount-1; i>-1; --i )
	{	
		if( i == 0 )
		{
			memcpy( DInfo.s_nValue, nTotalValue, sizeof( int )*5 );
			if( pTargetInfo->s_DamageType == CsC_AttackProp::DT_KnockBack )
			{
				DInfo.s_eDamageType = CsC_AttackProp::DT_KnockBack;
			}
		}
		else
		{
			// ëžœë¤ ê°’ìœ¼ë¡œ ë°ë¯¸ì§€ íƒ€ê²©íšŸìˆ˜ë¡œ ë‚˜ëˆ„ì–´ ì¤€ë‹¤. - ê²°ê³¼ ê°’ì€ ê°™ê²Œ ë§Œë“¤ìž		
			float fCalDamageRate = 1.0f; 
			(1.0f > (fMax - fMin ) ) ?  fCalDamageRate = fMin *0.01f : fCalDamageRate =  ( rand()%(int)( fMax - fMin ) + fMin )*0.01f;		
			for( int t=0; t<5; ++t )
			{
				DInfo.s_nValue[ t ] = (int)( pTargetInfo->s_nValue[ t ]*fCalDamageRate );
				if( DInfo.s_nValue[ t ] < 0 )
				{
					if( DInfo.s_nValue[ t ] < nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				else
				{
					if( DInfo.s_nValue[ t ] > nTotalValue[ t ] )
						DInfo.s_nValue[ t ] = nTotalValue[ t ];
				}
				nTotalValue[ t ] -= DInfo.s_nValue[ t ];
			}			
		}	
		//í† íƒˆë°ë¯¸ì§€ ê³„ì‚°
		nTotalDemage += DInfo.s_nValue[ 0 ];
		// ëžœë¤ ì ìš©
		DInfo.s_fDamageRot = fRot + CsD2R( rand()%70 - 35 );
		DInfo.s_fHitEventTime = vpEventTime[ i ];
		DInfo.s_dwResistTime = GetTickCount();// + (DWORD)( DInfo.s_fHitEventTime*2000.0f );		// í‹±ì´ë¯€ë¡œ 1000 ì„ ë” ê³±í•˜ìž
		DInfo.s_pChildDamageInfo = pDamageInfo;
		// í”„ë¡œí¼í‹° ë“±ë¡
		pDamageInfo = pTarget->GetProp_Attack()->InsertDamage( &DInfo );		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	if( pTarget->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		if( g_pCharResMng->GetTargetMark() == NULL )
			g_pCharResMng->SetTargetMark( pHitter );
	}
	// ë°ë¯¸ì§€ ì „íˆ¬ ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime=vpEventTime[ nEventCount - 1 ];
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem << bLastAttack;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
	//==============================================================================
	// ë•Œë¦°ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	// hitterëŠ” í•œëª…ì´ê¸° ë•Œë¬¸ì— hitterê°€ ì„ íƒí•œ íƒ€ê²Ÿê³¼ damageë¥¼ ë°›ëŠ” ëŒ€ìƒì´ ë™ì¼í• ë•Œë§Œ ì²˜ë¦¬í•´ì•¼ í•œë‹¤.
	//==============================================================================
	
	if( pTargetMark == NULL )
		return;
	if( pTargetMark->GetUniqID() == pTarget->GetUniqID() && bMaxCount == false || bRecvComp == true )
	{
		CsC_AttackProp::sHIT_INFO info;
		//info.s_eHitType = CsC_AttackProp::HT_Skill;
		info.s_dwAniID = dwAniID;	
		info.s_fAniAccelRate = fAccelRate;
		info.s_pPartnerDamageInfo = pDamageInfo;
		if( bRecvComp == true )	// íƒ€ê²Ÿ ì¹´ìš´íŠ¸ ë§ˆì§€ë§‰ê¹Œì§€ ì‹¤ì œ ìŠ¤í‚¬ì„ ì‚¬ìš©í•œ ëŒ€ìƒì„ ì°¾ì„ ìˆ˜ ì—†ì„ë•Œ(ìŠ¤í‚¬ì„ ì‚¬ìš©í•œ ëŒ€ìƒì´ ë²”ìœ„ì—ì„œ ë²—ì–´ë‚¬ì„ë•Œ)
		{
			info.s_eHitType = CsC_AttackProp::HT_Miss;
			info.s_pHitTarget = pTargetMark;	// ë””ì§€ëª¬ ë°©í–¥ì€ ìŠ¤í‚¬ì„ ì‚¬ìš©í•œ ëŒ€ìƒì„ ë°”ë¼ë³¸ ìƒíƒœë¡œ ìŠ¤í‚¬ ì• ë‹ˆë¥¼ êµ¬í˜„í•˜ê¸° ìœ„í•¨
		}
		else
		{
			info.s_eHitType = CsC_AttackProp::HT_Skill;
			info.s_pHitTarget = pTarget;
		}
		info.s_fRangeVelocity = pFTSkillInfo->s_fSkill_Velocity;
		info.s_fRangeAccelVelocity = pFTSkillInfo->s_fSkill_Accel;
		info.s_bRangeAttack = pHitterInfo->s_bRangeAttack;
		info.s_bRotation = true;
		info.s_nAttackEffectIndex = 0;
		switch( g_pResist->m_Global.s_eAttackEffectViewMode )
		{
		case cResist::sGLOBAL::AEM_ALL_OFF:			// ëžœë”ë§ ìžì²´ë¥¼ ì•ˆí•˜ê² ë‹¤
			info.s_nAttackEffectIndex = -1;
			break;
		case cResist::sGLOBAL::AEM_ONLY_NORATT_ON:	// ì¼ë°˜ íƒ€ê²©ì´íŽ™íŠ¸ë§Œ ëžœë”ë§
			info.s_nAttackEffectIndex = 0;
			break;
		case cResist::sGLOBAL::AEM_ALL_ON:
			info.s_nAttackEffectIndex = pHitter->IsStrongNature2( pTarget ) ? pHitter->GetNatureType2() : 0;
			break;
		}
// 		if( pTargetMark == NULL )
// 			info.s_vDamagePos = pTarget->GetPos();
// 		else
			info.s_vDamagePos = pTargetMark->GetPos();
		if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		{
			info.s_fRangeVelocity *= BATTLE_SPEED_RANGE;
			info.s_fRangeAccelVelocity *= BATTLE_SPEED_RANGE;
		}
		bMaxCount = true;
		// ë©”ëª¨ë¦¬ ìŠ¤í‚¬ì´ë¼ë©´ ê´‘ì—­ ì´íŽ™íŠ¸ë¥¼ í•œë²ˆ ë¶™ì—¬ì¤€ë‹¤.
		if( pFTSkillInfo->s_nMemorySkill )
			pHitter->GetProp_Effect()->AddEffect_FT( "system\\Buff\\Memory_Buff_Attack_Skill_2.nif"/*, 1.0f, nsEFFECT::POS_CHARPOS*/ );
		///////////////////////////////////////////////////////////////////////////////////////////////
		// í”„ë¡œí¼í‹° ë“±ë¡
		pHitter->GetProp_Attack()->InsertHit( &info );
		///////////////////////////////////////////////////////////////////////////////////////////////
		/*if( bRecvComplete == false )*/
			pHitter->RecvSkill( true, pHitterInfo->s_nSkillIndex );	
	}
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(pHitterInfo->s_nUID, pTargetInfo->s_nUID, nTotalDemage);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
#endif
}
//===============================================================================================
//
// ë„íŠ¸ ìŠ¤í‚¬ì •ë³´ ì„¤ì •
//
//===============================================================================================
void cCliGame::SetDotInfo( sRECV_HITTER_INFO* pHitterInfo, sRECV_TARGET_INFO* pTargetInfo, int nBuffCode, bool bLastAttack )
{
	CsC_AvObject* pHitter = g_pMngCollector->GetObject( pHitterInfo->s_nUID );
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( pTargetInfo->s_nUID );
	if( pHitter && ( pHitter->IsLoad() == false ) )
		pHitter = NULL;	
	if( pTarget && ( pTarget->IsLoad() == false ) )
		pTarget = NULL;
	//==============================================================================
	//	íƒ€ê²Ÿì€ ë°˜ë“œì‹œ ì¡´ìž¬í•´ì•¼í•¨
	//==============================================================================
	if( pTarget == NULL )
		return;
	assert_cs( pTarget->GetProp_Animation()->GetSequenceMng() );	
	//==============================================================================
	// ë§žëŠ”ë„˜ì˜ ì •ë³´ ë§Œë“¤ìž
	//==============================================================================	
	CsC_AttackProp::sDAMAGE_INFO DInfo;
	DInfo.s_pHitter = pHitter;
	DInfo.s_eDamageType = pTargetInfo->s_DamageType;
	DInfo.s_nNumType = ( pTarget->GetLeafRTTI() == RTTI_MONSTER || pTarget->GetLeafRTTI() == RTTI_TUTORIAL_MONSTER ) ? NUMTYPE::DIGIMON_ATTACK : NUMTYPE::MONSTER_ATTACK;
	DInfo.s_eActive = CsC_AttackProp::AT_ACTIVE;
	DInfo.s_nNumEffect = pTargetInfo->s_nNumEffectType;			
	DInfo.s_eMarbleType = CsC_AttackProp::MB_NONE;			
	memcpy( DInfo.s_nValue, pTargetInfo->s_nValue, sizeof( int )*5 );	
	
	// ëžœë¤ ì ìš©
	DInfo.s_fDamageRot = CsD2R( rand()%700 - 350 );
	DInfo.s_fHitEventTime = 0.0f;
	DInfo.s_dwResistTime = nBuffCode;	// ë„íŠ¸ë¥˜ëŠ” ì‹œê°„ì—†ì´ ë°”ë¡œ ë‚˜ì˜¤ë¯€ë¡œ ì´ê°’ì„ ë²„í”„ ë²ˆí˜¸ë¡œì“´ë‹¤!!
	DInfo.s_pChildDamageInfo = NULL;
	pTarget->GetProp_Attack()->InsertDamage( &DInfo );		
	//í† íƒˆë°ë¯¸ì§€ ê³„ì‚°	
	int nTotalDemage = 0;
	nTotalDemage += DInfo.s_nValue[ 0 ];
	// ë°ë¯¸ì§€ ì „íˆ¬ ë¡œê·¸
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_BATTLELOG_WINDOW, 0 ) )
	{
		ContentsStream kStream;
		float nRemainTime = 0.0f;
		int nTotalDem = abs(nTotalDemage);
		kStream << pHitterInfo << pTargetInfo << nRemainTime << nTotalDem << bLastAttack;
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_BATTLE_PROCESS, &kStream );
	}
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(pHitterInfo->s_nUID, pTargetInfo->s_nUID, nTotalDemage);	
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
// 	if( g_pDataMng->GetParty()->IsJoinParty() == true )
// 	{
// 		if( g_pGameIF->GetMainDigimonWindow()->IsBattle() ||  pHitterInfo->s_nUID == g_pCharMng->GetDigimonUser(0)->GetUniqID() )
// 		{
// 			cData_DamageMeter* pDamageMeter = g_pDataMng->GetDamageMeter();
// 			if( pDamageMeter == NULL )
// 				return;
// 			pDamageMeter->_SetDamageInfo( pHitterInfo->s_nUID, pTargetInfo->s_nUID, abs(nTotalDemage), _TIME_TS );
// 		}
// 	}
#endif
}
//===============================================================================================
//
// ì£½ì´ëŠ” ì •ë³´ ì„¤ì •
//
//===============================================================================================
void cCliGame::SetDieInfo( CsC_AvObject* pTarget )
{
// 	CHAT_TEXT_DEGUG( _T("SetDieInfo UID : %d"), pTarget->GetUniqID() );
	pTarget->GetProp_Attack()->InsertDamageEnd();
	pTarget->SetServerDie( true );	
	BHPRT( "Recv SetDieInfo ID : %d, UIDX : %d",  pTarget->GetIDX(), pTarget->GetUniqID() );
}
void cCliGame::RecvStatChange()
{
	u4 nUID = 0;
	pop(nUID);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( nUID );	
	if( pObject == NULL )
		return;
	u2 nChangeCount = 0;
	pop(nChangeCount);
	CsC_AvObject::sBASE_STAT * pBaseStat = pObject->GetBaseStat();
	if( pBaseStat )
	{
		for(int i = 0; i < nChangeCount; i++)
		{
			u2 nType = 0;
			u4 nValue = 0;
			pop(nType);		// ë³€ê²½ëœ íŒŒë¼ë¯¸í„° íƒ€ìž… Skill_Code ì˜ í•­ëª© A ê°’
			pop(nValue);	// ë³€ê²½ëœ ê°’
			switch( nType )
			{
			case APPLY_MS:		pBaseStat->SetMoveSpeed( (float)nValue );	break;
			case APPLY_HP:		pBaseStat->SetHP( nValue );					break;	
			case APPLY_DS:		pBaseStat->SetDS( nValue );					break;
			case APPLY_MAXHP:	pBaseStat->SetMaxHP( nValue );				break;	
			case APPLY_MAXDS:	pBaseStat->SetMaxDS( nValue );				break;		
			case APPLY_AP:		pBaseStat->SetAtt( nValue );				break;		
			case APPLY_DP:		pBaseStat->SetDef( nValue );				break;
			case APPLY_AS:		pBaseStat->SetAttackSpeed( nValue );		break;
			case APPLY_CA:		pBaseStat->SetCritical( nValue );			break;
			case APPLY_EV:		pBaseStat->SetEvade( nValue );				break;						
			case APPLY_HT:		pBaseStat->SetHitRate( nValue );			break;
			case APPLY_AR:				
			case APPLY_FP:				
			case APPLY_FS:				
			case APPLY_EXP:				
			case APPLY_POWERAPPLYRATE:						
			case APPLY_BL:
			case APPLY_DA:				
			case APPLY_ER:				
			case APPLY_AllParam:			
			case APPLY_SER:				
			case APPLY_SDR:				
			case APPLY_SRR:				
			case APPLY_SCD:				
			case APPLY_SCR:				
			case APPLY_HRR:				
			case APPLY_DRR:				
			case APPLY_MDA:				
			case APPLY_HR:				
				break;
			default:
				assert_csm( false, L"ì •ì˜ ë˜ì§€ ì•Šì€ ìŠ¤íƒ¯ê°’ìž„ pParameterì°¸ì¡°!!" )
					break;
			}
		}
		if( RTTI_TAMER_USER == pObject->GetLeafRTTI() )
			GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
		if( RTTI_DIGIMON_USER == pObject->GetLeafRTTI() )
			GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_STAT_DIGIMONSTATUS, NULL );
	}		
}
void cCliGame::RecvAllStat(void)
{
#define ALLSTAT_LOG( fmt, ... ) do { \
    nsCSDEBUG::CrashLogger::LogMessage( fmt, __VA_ARGS__ ); \
    nsCSDEBUG::CrashLogger::LogStatsMessage( fmt, __VA_ARGS__ ); \
} while(0)
	SAFE_POINTER_RET( g_pCharMng );
	CDigimonUser* pDUser = g_pCharMng->GetDigimonUser( 0 );
	SAFE_POINTER_RET( pDUser );
	CTamerUser* pTUser = g_pCharMng->GetTamerUser();
	SAFE_POINTER_RET( pTUser );
	// ìžê¸° ìžì‹  ( í…Œì´ë¨¸ + ë””ì§€ëª¬ )
	n4 nT_MaxHP = 0;
	n4 nT_MaxDS = 0;
	n4 nT_AP = 0;
	n4 nT_DP = 0;
	n4 nT_HP = 0;
	n4 nT_DS = 0;
	u2 nT_MS = 0;
	pop(nT_MaxHP);
	pop(nT_MaxDS);
	pop(nT_HP);
	pop(nT_DS);
	pop(nT_AP);
	pop(nT_DP);
	pop(nT_MS);
	CTamerUser::sUSER_STAT* pTBase = dynamic_cast<CTamerUser::sUSER_STAT*>(pTUser->GetBaseStat());
	if( pTBase && pTUser->GetLeafRTTI() == RTTI_TAMER_USER )
	{
		pTBase->SetMaxHP( nT_MaxHP );
		pTBase->SetMaxDS( nT_MaxDS );
		pTBase->SetHP( nT_HP );
		pTBase->SetDS( nT_DS );
		pTBase->SetAtt( nT_AP );
		pTBase->SetDef( nT_DP );	
	}
	n4 nD_MaxHP = 0;
	n4 nD_MaxDS = 0;
	n4 nD_HP = 0;
	n4 nD_DS = 0;
	u2 nD_FS = 0;	// friend fs
	n4 nD_AP = 0;
	n4 nD_DP = 0;
	u2 nD_CA = 0;
	u2 nD_AS = 0;
	float fD_AS = 0.0f;
	u2 nD_EV = 0;
	n4 nD_HT = 0;
	u2 nD_AR = 0;
	u2 nD_BL = 0;
	pop(nD_MaxHP);
	pop(nD_MaxDS);
	pop(nD_HP);
	pop(nD_DS);
	pop(nD_FS);	// ì¹œë°€ë„
	size_t const nPartnerStatBytesAfterAttack =
		( 7 + 1 + nExtendState::MaxExtendStat + nExtendState::MaxExtendStat ) * sizeof( u2 );
	size_t const nDetailInfoExpectedBytes = CTamerUser::sUSER_STAT::DETAILINFO_STAT_COUNT * sizeof( n4 );
	size_t const nPartnerStatAvailableBytes = GetReadAvailable();
	size_t const nPacketTailBytes = sizeof( u2 );
	size_t const nKnownLegacyAttackBytes = sizeof( u2 ) + nPartnerStatBytesAfterAttack;
	size_t const nKnownExtendedAttackBytes = sizeof( n4 ) + nPartnerStatBytesAfterAttack;
	size_t const nKnownLegacyAttackWithIntDefenseBytes = nKnownLegacyAttackBytes + sizeof( u2 );
	size_t const nKnownExtendedAttackWithIntDefenseBytes = nKnownExtendedAttackBytes + sizeof( u2 );
	size_t const nKnownLegacyAttackWithIntHitRateBytes = nKnownLegacyAttackBytes + sizeof( u2 );
	size_t const nKnownExtendedAttackWithIntHitRateBytes = nKnownExtendedAttackBytes + sizeof( u2 );
	size_t const nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes = nKnownLegacyAttackBytes + (sizeof( u2 ) * 2);
	size_t const nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes = nKnownExtendedAttackBytes + (sizeof( u2 ) * 2);
	size_t const nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes = nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes + sizeof( u2 );
	bool const bLegacyAttackLayout =
		nPartnerStatAvailableBytes == nKnownLegacyAttackBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes;

	bool const bExtendedAttackLayout =
		nPartnerStatAvailableBytes == nKnownExtendedAttackBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes + nPacketTailBytes;

	bool const bAttackAsInt32Layout = bExtendedAttackLayout && !bLegacyAttackLayout;
	bool const bFloatAttackSpeedLayout =
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes + nPacketTailBytes;
	bool const bIntDefenseLayout =
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes + nPacketTailBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes;
	bool const bIntHitRateLayout =
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes ||
		nPartnerStatAvailableBytes == nKnownExtendedAttackWithIntDefenseFloatAttackSpeedAndIntHitRateBytes + nDetailInfoExpectedBytes + nPacketTailBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes ||
		nPartnerStatAvailableBytes == nKnownLegacyAttackWithIntDefenseAndIntHitRateBytes + nDetailInfoExpectedBytes;
	ALLSTAT_LOG(
		"ALLSTAT layout bytes=%u tamerBaseBytes=%u detailTailExpected=%u packetTailBytes=%u layout legacy=%d extended=%d intDefense=%d floatAS=%d intHitRate=%d",
		static_cast<unsigned>( nPartnerStatAvailableBytes ),
		static_cast<unsigned>( nPartnerStatBytesAfterAttack ),
		static_cast<unsigned>( nDetailInfoExpectedBytes ),
		static_cast<unsigned>( nPacketTailBytes ),
		bLegacyAttackLayout ? 1 : 0,
		bExtendedAttackLayout ? 1 : 0,
		bIntDefenseLayout ? 1 : 0,
		bFloatAttackSpeedLayout ? 1 : 0,
		bIntHitRateLayout ? 1 : 0 );
	if( bAttackAsInt32Layout )
	{
		pop(nD_AP);
		if( bAttackAsInt32Layout )
		{
			ALLSTAT_LOG(
				"ALLSTAT partner AT detected as int32 bytes=%u AT=%d",
				static_cast<unsigned>( nPartnerStatAvailableBytes ),
				static_cast<int>( nD_AP ) );
		}
	}
	else
	{
		u2 nD_AP_Legacy = 0;
		pop(nD_AP_Legacy);
		nD_AP = nD_AP_Legacy;
		ALLSTAT_LOG(
			"ALLSTAT legacy partner AT layout detected bytes=%u AT=%u; server is still sending partner AT as u2",
			static_cast<unsigned>( nPartnerStatAvailableBytes ),
			static_cast<unsigned>( nD_AP_Legacy ) );
	}
	if( bIntDefenseLayout )
		pop( nD_DP );
	else
	{
		u2 nD_DP_Legacy = 0;
		pop( nD_DP_Legacy );
		nD_DP = nD_DP_Legacy;
	}
	pop(nD_CA);
	if( bFloatAttackSpeedLayout )
	{
		pop( fD_AS );
		nD_AS = static_cast<u2>( ( fD_AS * 1000.0f ) + 0.5f );
	}
	else
	{
		pop(nD_AS);
		fD_AS = static_cast<float>( nD_AS ) * 0.001f;
	}
	pop(nD_EV);
	if( bIntHitRateLayout )
		pop(nD_HT);
	else
	{
		u2 nD_HT_Legacy = 0;
		pop( nD_HT_Legacy );
		nD_HT = nD_HT_Legacy;
		ALLSTAT_LOG(
			"ALLSTAT legacy partner HT layout detected bytes=%u HT=%u; server is still sending partner HT as u2",
			static_cast<unsigned>( nPartnerStatAvailableBytes ),
			static_cast<unsigned>( nD_HT_Legacy ) );
	}
	pop(nD_AR);
	pop(nD_BL);
	ALLSTAT_LOG(
		"ALLSTAT parsed core TAMER uid=%u HP=%u DS=%u AP=%d DP=%d MAXHP=%u MAXDS=%u MS=%u; DIGIMON uid=%u HP=%u DS=%u MAXHP=%u MAXDS=%u FS=%u AP=%d DP=%d HT=%u AR=%u BL=%u CC=%u ASms=%u ASsec=%.3f EV=%u",
		static_cast<unsigned>( pTUser->GetUniqID() ),
		static_cast<unsigned>( nT_HP ),
		static_cast<unsigned>( nT_DS ),
		static_cast<int>( nT_AP ),
		static_cast<int>( nT_DP ),
		static_cast<unsigned>( nT_MaxHP ),
		static_cast<unsigned>( nT_MaxDS ),
		static_cast<unsigned>( nT_MS ),
		static_cast<unsigned>( pDUser->GetUniqID() ),
		static_cast<unsigned>( nD_HP ),
		static_cast<unsigned>( nD_DS ),
		static_cast<unsigned>( nD_MaxHP ),
		static_cast<unsigned>( nD_MaxDS ),
		static_cast<unsigned>( nD_FS ),
		static_cast<int>( nD_AP ),
		static_cast<int>( nD_DP ),
		static_cast<unsigned>( nD_HT ),
		static_cast<unsigned>( nD_AR ),
		static_cast<unsigned>( nD_BL ),
		static_cast<unsigned>( nD_CA ),
		static_cast<unsigned>( nD_AS ),
		fD_AS,
		static_cast<unsigned>( nD_EV ) );
	u2 nInchantLevel = 0;
	pop(nInchantLevel);
	u2 nExtentionParameter[ nExtendState::MaxExtendStat ] = {0,};
	pop(nExtentionParameter);
	u2 nExtentionParameterLV[ nExtendState::MaxExtendStat ] = {0,};
	pop( nExtentionParameterLV[ nExtendState::ET_AT ] );
	pop( nExtentionParameterLV[ nExtendState::ET_BL ] );
	pop( nExtentionParameterLV[ nExtendState::ET_CR ] );
	pop( nExtentionParameterLV[ nExtendState::ET_AS ] );
	pop( nExtentionParameterLV[ nExtendState::ET_EV ] );
	pop( nExtentionParameterLV[ nExtendState::ET_HT ] );	
	pop( nExtentionParameterLV[ nExtendState::ET_HP ] );
	n4 nT_DetailInfoStat[ CTamerUser::sUSER_STAT::DETAILINFO_STAT_COUNT ] = {0,};
	size_t const nDetailInfoAvailableBytes = GetReadAvailable();
	bool const bHasDetailInfoTail = nDetailInfoAvailableBytes >= nDetailInfoExpectedBytes;
	if( bHasDetailInfoTail )
	{
		for( int i = 0; i < CTamerUser::sUSER_STAT::DETAILINFO_STAT_COUNT; ++i )
			pop( nT_DetailInfoStat[ i ] );
		ALLSTAT_LOG(
			"ALLSTAT DetailInfo recv bytes=%u remaining=%u HP=%d DS=%d AT=%d AS=%d CT=%d HT=%d SCD=%d CD=%d SD=%d BASE=%d DE=%d BL=%d EV=%d",
			static_cast<unsigned>( nDetailInfoAvailableBytes ),
			static_cast<unsigned>( GetReadAvailable() ),
			nT_DetailInfoStat[ 0 ],
			nT_DetailInfoStat[ 1 ],
			nT_DetailInfoStat[ 2 ],
			nT_DetailInfoStat[ 3 ],
			nT_DetailInfoStat[ 4 ],
			nT_DetailInfoStat[ 5 ],
			nT_DetailInfoStat[ 6 ],
			nT_DetailInfoStat[ 7 ],
			nT_DetailInfoStat[ 8 ],
			nT_DetailInfoStat[ 9 ],
			nT_DetailInfoStat[ 10 ],
			nT_DetailInfoStat[ 11 ],
			nT_DetailInfoStat[ 12 ] );
	}
	else
	{
		ALLSTAT_LOG(
			"ALLSTAT DetailInfo missing tail bytes=%u expected=%u; client will keep DetailInfo as zero until server sends the extended packet",
			static_cast<unsigned>( nDetailInfoAvailableBytes ),
			static_cast<unsigned>( nDetailInfoExpectedBytes ) );
	}
	if( pTBase && pTUser->GetLeafRTTI() == RTTI_TAMER_USER )
	{
		for( int i = 0; i < CTamerUser::sUSER_STAT::DETAILINFO_STAT_COUNT; ++i )
			pTBase->SetDetailInfoStat( i, nT_DetailInfoStat[ i ] );
	}
	CDigimonUser::sUSER_STAT* pDBase = dynamic_cast<CDigimonUser::sUSER_STAT*>(pDUser->GetBaseStat());
	if( pDBase && pDUser->GetLeafRTTI() == RTTI_DIGIMON_USER )
	{
		pDBase->SetMaxHP( nD_MaxHP );
		pDBase->SetMaxDS( nD_MaxDS );
		pDBase->SetHP( nD_HP );
		pDBase->SetDS( nD_DS );
		pDBase->SetFriendShip( nD_FS );
		pDBase->SetAtt( nD_AP );
		pDBase->SetDef( nD_DP );
		pDBase->SetCritical( nD_CA );
		pDBase->SetAttackSpeed( nD_AS );
		pDBase->SetEvade( nD_EV );
		pDBase->SetHitRate( nD_HT );
		pDBase->SetAttRange( nD_AR );
		pDBase->SetBL( nD_BL );
		CDigimon::sENCHENT_STAT* pEStat = ( (CDigimon*)g_pCharMng->GetDigimonUser( 0 ) )->GetEnchantStat();
		pEStat->SetEnchantLevel( nInchantLevel );
		for( int i = nExtendState::ET_AT; i < nExtendState::MaxExtendStat; ++i )
		{
			pEStat->SetEnchantInfo( i, nExtentionParameterLV[ i ], nExtentionParameter[ i ] );
		}
		ALLSTAT_LOG(
			"ALLSTAT applied DIGIMON uid=%u AT=%d DP=%d HT=%d DS=%d HP=%d DECOMP=%d detailTailAvailable=%u",
			static_cast<unsigned>( pDUser->GetUniqID() ),
			static_cast<int>( nD_AP ),
			static_cast<int>( nD_DP ),
			static_cast<unsigned>( nD_HT ),
			static_cast<unsigned>( nD_DS ),
			static_cast<unsigned>( nD_HP ),
			static_cast<int>( nT_DetailInfoStat[ 2 ] ),
			static_cast<unsigned>( nDetailInfoAvailableBytes ) );
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_STAT_DIGIMONSTATUS, NULL );
	}
#undef ALLSTAT_LOG
}
// ê²Œìž„ ì‹œìž‘ì‹œ ì²˜ìŒ ë©”ì„¸ì§€(ì¸ì‚¬ë§, ê³µì§€ë“±)
void cCliGame::RecvStartMessage(void)
{
	u1 nMsgCnt = 0;
	char msg[Language::pLength::str + 1] = { 0, };
	pop(nMsgCnt);
	for(uint i=0; i<nMsgCnt; i++)
	{
		memset(msg, 0, sizeof(msg));
		pop(msg);
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = nBase::m2w(msg);
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
	}
}
void cCliGame::RecvItemMoveFailure(void)
{
	u2 nSrcPos;
	u2 nDstPos;
	pop(nSrcPos);
	pop(nDstPos);
	if( IS_EXTRAINVEN_CONSTANT( TO_CONSTANT( nSrcPos ) ) || IS_EXTRAINVEN_CONSTANT( TO_CONSTANT( nDstPos ) ) )
		ExtraInventoryDebugLog( "[ExtraInventory][Network] RecvItemMoveFailure src=%d dst=%d srcConst=%d dstConst=%d",
			nSrcPos, nDstPos, TO_CONSTANT( nSrcPos ), TO_CONSTANT( nDstPos ) );
	g_pDataMng->ServerItemMoveFailed( nSrcPos, nDstPos );
}
void cCliGame::RecvChangeItemLimitedAttribute(void)
{
	u2 nItemPos;
	u1 nLimited;
	pop( nItemPos );
	pop( nLimited );
	
	g_pDataMng->SetChangeItemLimited(nItemPos, nLimited);
}
void cCliGame::RecvItemMoveSuccess(void)
{
	ExtraInventoryDebugLog( "[ExtraInventory][Network] RecvItemMoveSuccess dispatch" );
	RecvItemMove();
}
void cCliGame::RecvItemUseSuccess(void)
{
	u4 nTargetUID = 0;
	n2 nInvenPos = 0;
	pop(nTargetUID);
	pop(nInvenPos);
	g_pDataMng->ServerItemUseSuccess( nTargetUID, nInvenPos );
}
#ifdef CROSSWARS_SYSTEM
void cCliGame::RecvCrossItemUseSuccess(void)
{
	u4 nTargetUID = 0;
	n2 nInvenPos = 0;
	pop(nTargetUID);
	pop(nInvenPos);
	g_pDataMng->ServerCrossItemUseSuccess( nTargetUID, TO_ID(nInvenPos) );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
}
void cCliGame::RecvCrossItemUseFailure(void)
{
	u2 nInvenPos;
	u4 nItemType;
	u4 nErrCode;
	pop(nInvenPos);
	pop(nItemType);
	pop(nErrCode);
	nsCSDEBUG::CrashLogger::LogMessage( "CROSS_ITEM_USE failure slot=%u type=%u err=%u", nInvenPos, nItemType, nErrCode );
	switch(nErrCode)
	{
	case nItem::USE_FAIL_NOT_USE_AREA :	// ì‚¬ìš© ë¶ˆê°€ëŠ¥ ìœ„ì¹˜ìž…ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 30336 );
		break;
	case nItem::USE_FAIL_BATTLE :	// ì „íˆ¬ ìƒíƒœë¡œ ì¸í•´ ì‚¬ìš© ë¶ˆê°€ëŠ¥
		cPrintMsg::PrintMsg( 16002 );
		break;
	case nItem::USE_FAIL_COOLDOWN :	// ìž¬ì‚¬ìš© ì‹œê°„ì´ ì•„ì§ ë‚¨ì•˜ìŠµë‹ˆë‹¤.
		// ì¿¨íƒ€ìž„ ì¤‘ìž…ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 13002 );
		break;
	case nItem::USE_FAIL_NOT_USE_TERMS :
		cPrintMsg::PrintMsg( 13002 );
		break;
	default :	// error
		break;
	}
	g_pDataMng->ItemUnlock( nInvenPos );
}
void cCliGame::RecvCrossInvenSlotSize(void)
{
	u2 nSlotSize;
	pop(nSlotSize);
	g_pDataMng->GetInvenCross()->SetInvenSlotCount( nSlotSize );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
	//==========================================================================================================
	// ì—…ì  ì²´í¬
	//==========================================================================================================
	
}
#endif
void cCliGame::RecvItemUseFailure(void)
{
	u2 nInvenPos;
	u4 nItemType;
	u4 nErrCode;
	pop(nInvenPos);
	pop(nItemType);
	pop(nErrCode);
	nsCSDEBUG::CrashLogger::LogMessage( "ITEM_USE failure slot=%u type=%u err=%u", nInvenPos, nItemType, nErrCode );
	switch(nErrCode)
	{
	case nItem::USE_FAIL_EXPIRE :	// ì‹œê°„ ì œí•œ ì¢…ë£Œë¡œ ì•„ì´í…œ ì‚¬ìš© ë¶ˆê°€ëŠ¥
		cPrintMsg::PrintMsg( 20029, nsCsFileTable::g_pItemMng->GetItem( nItemType )->GetInfo()->s_szName );
		break;
	case nItem::USE_FAIL_BATTLE :	// ì „íˆ¬ ìƒíƒœë¡œ ì¸í•´ ì‚¬ìš© ë¶ˆê°€ëŠ¥
		cPrintMsg::PrintMsg( 16002 );
		break;
	case nItem::USE_FAIL_COOLDOWN :	// ìž¬ì‚¬ìš© ì‹œê°„ì´ ì•„ì§ ë‚¨ì•˜ìŠµë‹ˆë‹¤.
		// ì¿¨íƒ€ìž„ ì¤‘ìž…ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 13002 );
		break;
	case nItem::USE_FAIL_NOT_USE_AREA :	// ì‚¬ìš© ë¶ˆê°€ëŠ¥ ìœ„ì¹˜ìž…ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 30336 );
		break;
	case nItem::USE_FAIL_NON_TARGET :	// ëŒ€ìƒì´ ì„ íƒë˜ì§€ ì•„ë‹ˆí•˜ì˜€ìŠµë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 16001 );
		break;
	case nItem::USE_FAIL_NOT_MONSTER :	// ëŒ€ìƒì´ ëª¬ìŠ¤í„°ê°€ ì•„ë‹™ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 17004 );
		break;
	case nItem::USE_FAIL_NOT_RANGE :	// ëŒ€ìƒì´ ë„ˆë¬´ ë©€ë¦¬ìžˆìŠµë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 17005 );
		break;
	case nItem::USE_FAIL_NOT_USE_TERMS :
		cPrintMsg::PrintMsg( 11009 );
		break;
	case nItem::USE_FAIL_LIMIT :
		cPrintMsg::PrintMsg( 30505 );	// í•œë²ˆë§Œ(ë””ì§€ëª¬) ì‚¬ìš© ê°€ëŠ¥í•œ ì•„ì´í…œìž…ë‹ˆë‹¤.
		break;
	case nItem::USE_FAIL_NOT_USE_TARGET : // ì‚¬ìš©í•  ìˆ˜ ì—†ëŠ” ëŒ€ìƒ ìž…ë‹ˆë‹¤.
		break;
#ifdef ADVENTURE_TAMER_CHANGE
	case nItem::USE_FAIL_NOT_INVEN :		//í…Œì´ë¨¸ ì²´ì¸ì§€ ì•„ì´í…œ ì‚¬ìš©ì‹œ ì¸ë²¤ì´ ëª¨ìžëž„ë•Œ
		cPrintMsg::PrintMsg( 11015 );
		break;
#endif
	case nItem::USE_FAIL_NOT_SAME:		// ë™ì¼í•œ ì•„ì´í…œ ì ìš© ë¶ˆê°€
		cPrintMsg::PrintMsg( 30585 );
		break;
	case nItem::USE_FAIL_NOT_USE_SCAN :
		{
			// ê°•ì œ ìº”ìŠ¬
			UINT nScanWT = cBaseWindow::WT_DIGITAMA_SCAN;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_CLOSE, &nScanWT );
			cPrintMsg::PrintMsg( 11002 );	//ì•„ì´í…œ ìŠ¤ìº”ì„ í•˜ì˜€ëŠ”ë° Item_Rankì— ìŠ¤ìº” ê²°ê³¼ ì•„ì´í…œ ê°’ì´ ì—†ì–´ì„œ ì•„ë¬´ì¼ë„ ì¼ì–´ë‚˜ì§€ ì•ŠëŠ” ê²½ìš°
		}
		break;
	case nItem::USE_FAIL_SPRIT_EXP :
		cPrintMsg::PrintMsg( 30705 );	//ìŠ¤í”¼ë¦¿ ë¶€ìŠ¤í„°(ê²½í—˜ì¹˜ íšë“ ì•„ì´í…œìž„)ë¥¼ ì‚¬ìš© ì‹œ í•˜ì´ë¸Œë¦¬ë“œì²´ ë””ì§€ëª¬(íƒ€ìž… 10ë²ˆ)ì„ ì†Œí™˜í•œ ìƒíƒœê°€ ì•„ë‹ ê²½ìš° ì•„ì´í…œ ì‚¬ìš© ì‹¤íŒ¨ ë©”ì‹œì§€
		break;
	case nItem::USE_FAIL_MAX_LIMIT :
		cPrintMsg::PrintMsg( 30338 );
		break;
	case nItem::USE_FAIL_NO_SKILL :		// ìŠ¤í‚¬ì´ ì—†ë‹¤
		cPrintMsg::PrintMsg( 11010 );	// ì•„ì´í…œ ê¸°ëŠ¥ì´ ì—°ê²°ë˜ì–´ ìžˆì§€ ì•ŠìŠµë‹ˆë‹¤.
		break;
	case nItem::USE_FAIL_OTHERS :		// ì‹¤íŒ¨
		cPrintMsg::PrintMsg( 11009 );	// ì•„ì´í…œì„ ì‚¬ìš©í•  ìˆ˜ ì—†ìŠµë‹ˆë‹¤.
		break;
	default :	// error
		break;
	}
	g_pDataMng->ServerItemUseFailed( nInvenPos );	
	
}
void cCliGame::RecvChangeTamer(void)
{
#ifdef ADVENTURE_TAMER_CHANGE
	cPrintMsg::PrintMsg( 30181 );
	g_pCharMng->DelTamer();
	cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
#ifdef CROSSWARS_SYSTEM
	cData_PostLoad::sTAMERDATA* pTamerData = pPostLoad->GetTamerData();
#else
	cData_PostLoad::sDATA* pTamerData = pPostLoad->GetTamerData();
#endif
	pop( pTamerData->s_Type );
	nSync::Pos pos;
	pop( pos );
	pPostLoad->SetPos( pos );
#ifdef SDM_TAMER_SERVERRELOCATE_20170911
	u4 nServerRelocateCompleteTime = 0;
	pop( nServerRelocateCompleteTime );
	pPostLoad->SetServerRelocateCompletedTime( nServerRelocateCompleteTime );
#endif
	TCHAR szName[ Language::pLength::name + 1 ];
	pop( szName );
	_tcscpy_s( pTamerData->s_szName, Language::pLength::name + 1, szName  );
	pop( pTamerData->s_nExp );
	pTamerData->s_nExp = pTamerData->s_nExp / 100;
	pop( pTamerData->s_nLevel );
	pop( pTamerData->s_Attribute[MHP] );
	pop( pTamerData->s_Attribute[MDS] );
	pop( pTamerData->s_Attribute[HP] );
	pop( pTamerData->s_Attribute[DS] );
	pop( pTamerData->s_Attribute[FP] );
	pop( pTamerData->s_Attribute[AP] );
	pop( pTamerData->s_Attribute[DP] );
	pop( pTamerData->s_Attribute[MS] );	
#ifdef CROSSWARS_SYSTEM
	pop( pTamerData->s_MCR );
	pop( pTamerData->s_CR );
#endif
	u4 nClientOption = 0;
	pop( nClientOption );
	u1 nCashCount;					// í…Œì´ë¨¸ ìºì‰¬ ìŠ¤í‚¬ ê°œìˆ˜
	pop( nCashCount );
	assert_cs( nCashCount < 3 );	// í˜„ìž¬ ìºì‰¬ ìŠ¤í‚¬ ê°œìˆ˜ëŠ” 2ë¥¼ ë„˜ì„ ìˆ˜ ì—†ë‹¤
	for( int i=0; i<nCashCount; ++i )
	{
		pop( pTamerData->s_nTamerCashSkill_Index[i] );		// ìºì‰¬ ìŠ¤í‚¬ ì¸ë±ìŠ¤ (Tamer_Skill.xls ì˜ ì¸ë±ìŠ¤ ê°’)
		pop( pTamerData->s_nTamerCashSkill_LifeTime[i] );	// ê¸°ê°„ ë§Œë£Œê¹Œì§€ ì‹œê°„
		pop( pTamerData->s_nTamerCashSkill_CoolTime[i] );	// ì¿¨ íƒ€ìž„
	}
	g_pDataMng->GetPostLoad()->PostLoadT();
	u2 cnt;
	pop(cnt);
	nSync::Pos SyncPos;
	cType type;
	type = pTamerData->s_Type;
	while(cnt)
	{
		pop(SyncPos);		//ìœ„ì¹˜ë¥¼ ë°›ê³ 
		pop(type);		//íƒ€ìž…ì„ ë°›ì•„ì„œ
		switch(type.m_nClass)			//ì–´ë–¤ ë…€ì„ì¸ì§€ êµ¬ë¶„í•˜ìž
		{
			case nClass::Tamer			: SyncInTamer(SyncPos, type);			break;
			case nClass::Digimon		: SyncInDigimon(SyncPos, type);			break;
		}
		-- cnt;
	}
#else
	cPrintMsg::PrintMsg( 30181 );
#endif
	// ì¶”ê°€ í€µìŠ¬ë¡¯ì— ìŠ¤í‚¬ ë“¤ì–´ê°€ìžˆìœ¼ë©´ ë¹¼ì¤˜ì•¼í•¨
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_QUICKSLOT, cQuickSlot::Q_ADDSLOT ) )
	{
		g_pGameIF->GetQuickSlot( cQuickSlot::Q_ADDSLOT )->_DeleteSkill_AddSlot( RTTI_TAMER_USER );
	}
}
#ifdef ADVENTURE_TAMER_CHANGE
void cCliGame::RecvChangeTamerAround( void )		//ì£¼ë³€ì— ìžˆëŠ” ë…€ì„ë“¤ìš©
{
	u4 nUID = 0;
	nSync::Pos SyncPos;
	cType type;
	for( int i = 0; i < 2; ++i )
	{
		pop(nUID);
		type.m_nUID = nUID;
		switch(GetClass(nUID))
		{
		case nClass::Digimon		:	g_pCharMng->DeleteDigimon( type.m_nIDX );								break;
		case nClass::Tamer			:	g_pCharMng->DeleteTamer( type.m_nIDX );								break;
		default :						xassert1(false, "unknown uid(%d)", nUID);
		}
	}
	for( int j= 0; j < 2; ++j )
	{
		pop(SyncPos);		//ìœ„ì¹˜ë¥¼ ë°›ê³ 
		pop(type);		//íƒ€ìž…ì„ ë°›ì•„ì„œ
		switch(type.m_nClass)			//ì–´ë–¤ ë…€ì„ì¸ì§€ êµ¬ë¶„í•˜ìž
		{
		case nClass::Tamer			: SyncInTamer(SyncPos, type);			break;
		case nClass::Digimon		: SyncInDigimon(SyncPos, type);			break;
		}
	}
}
#endif
void cCliGame::RecvCapsuleToItem(void)
{
	u4 nItemType;
	u1 nItemRate;
	u2 nOverlapItemCount;
	u4 nRemainTradeLimitTime = 0;
	pop(nItemType);
	while(nItemType)
	{
		pop(nItemRate);
		pop(nOverlapItemCount);
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
		pop(nRemainTradeLimitTime);		
#endif
		assert_csm( nOverlapItemCount == 1, _T( "í•œë²ˆì— í•˜ë‚˜ì˜ ìº¡ìŠë§Œ ì–»ì–´ì•¼ í•œë‹¤." ) );
		cItemData data;
		data.m_nType = nItemType;
		data.m_nRate = nItemRate;	
		data.m_nCount = nOverlapItemCount;
		data.m_nLevel = 1;
		data.m_nRemainTradeLimitTime = nRemainTradeLimitTime;
		g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CASH_CAPSULE_COIN_SERVER, 0, &data );
		pop(nItemType);
		assert_csm( nItemType == 0, _T( "ì½”ì¸ í•˜ë‚˜ì— ìº¡ìŠì€ í•˜ë‚˜ì—¬ì•¼í•œë‹¤." ) );
	}
#ifdef TAMER_IDLEANIMATION
	// í…Œì´ë¨¸ê°€ ìžë¦¬ ë¹„ì›€ ìƒíƒœì´ë©´ ì¼ì–´ë‚˜ì•¼ í•œë‹¤.
	switch( g_pCharMng->GetTamerUser()->GetProp_Animation()->GetAnimationID() )
	{
	case ANI::IDLE_SIT:
	case ANI::IDLE_SIT_DOWN:
		g_pCharMng->GetTamerUser()->SetPause( CsC_AvObject::PAUSE_ANIMATION, false );
		g_pCharMng->GetTamerUser()->SetAnimation( ANI::IDLE_SIT_UP );
		break;
	}
#endif
}
#ifdef UI_ITEMSCAN_RENEWAL
void cCliGame::RecvDigitamaScanResult(void)
{
	GS2C_RECV_ITEMSCAN recv;
	pop( recv.nResult );
	pop( recv.n8ScanMoney );	// ìŠ¤ìº” ì´ ë¹„ìš©
	pop( recv.n8NowMoney );		// ìŠ¤ìº” í›„ ëˆ
	pop( recv.uBeforeScanPos );
	pop( recv.uBeforeScanType );
	pop( recv.u2BeforeScanCount );	// ì‹¤íŒ¨ ì‹œ 0
	// íšë“í•œ ì•„ì´í…œ ì¢…ë¥˜ ê°œìˆ˜ - ì‹¤íŒ¨ ì‹œ 0
	pop( recv.uItemCount );
	for( int i = 0; i < recv.uItemCount; ++i )
	{
		nsItemScan::sScanInfo sInfo;
		pop( sInfo.uInvenIdx );
		pop( sInfo.itemData );
		recv.lScanList.push_back( sInfo );
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_SCAN_ADD_DATA, &recv );
}
#else
void cCliGame::RecvDigitamaScanResult(void)
{
	GS2C_RECV_DIGITAMA_SCANITEM recv;
	while( 1 )
	{
		u4 nItemType = 0;
		pop( nItemType );
		if( 0 == nItemType )
			break;
		u1 nItemRate = 0;
		pop( nItemRate );
		u2 nItemCount = 0;
		pop( nItemCount );
		u4 nRemainTradeLimitTime = 0;
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
		pop( nRemainTradeLimitTime );
#endif
		cItemInfo data;
		data.Set( nItemType, nItemCount, nItemRate, 1 );
		data.m_nRemainTradeLimitTime = nRemainTradeLimitTime;
		recv.m_ItemList.push_back( data );
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_SCAN_ADD_DATA, &recv );
}
#endif
void cCliGame::RecvHatchUpSuccess(void)
{
	u4 nTamerUID = 0;
	pop(nTamerUID);
	// ìš©ë³‘ ë¶€í™”ìš© ë°ì´íƒ€ ìž…ë ¥ ê²°ê³¼ - ì„±ê³µ
	u1 nHatchLevel = 0;
	pop(nHatchLevel); // ë°ì´íƒ€ ìž…ë ¥ ì„±ê³µí›„ ë ˆë²¨
	assert( nHatchLevel <= 5 );
	#pragma todo("ìš©ë³‘ ë¶€í™” ë‹¨ìˆ˜ MAX ìˆ˜ì¹˜ ì¡°ì ˆ")	
	CsC_AvObject* pTamer = g_pMngCollector->GetObject( nTamerUID );
	if( pTamer == NULL )
		return;
	switch( nHatchLevel )
	{
	case 3:
	case 4:
	case 5:
		if( pTamer->GetLeafRTTI() != RTTI_TAMER_USER )
		{
			pTamer->SetAnimation( ANI::NPC_YES );
			pTamer->GetProp_Effect()->AddEffect( "System\\Tactics_success.nif", 1.0f, nsEFFECT::POS_CHARPOS );		
		}		
		break;
	}	
	if( pTamer->GetLeafRTTI() != RTTI_TAMER_USER )
		return;
	// ì‚¬ìš´ë“œëŠ” ë‚˜ë§Œ
	switch( nHatchLevel )
	{
	case 3:
	case 4:
	case 5:	
		pTamer->PlaySound( "system\\Tactics_yes.wav" );
		break;
	}
	GS2C_RECV_MAKE_DIGITAMA_SCLV recv;
	recv.m_nSuccessLevel = nHatchLevel;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::MAKETACTICS_DIGITAMA_SC_LV, &recv );
	if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAKE_TACTICS ) )
	{	
		g_pGameIF->GetMakeTactics()->RecvResult( cMakeTactics::SUCCESS );
		g_pGameIF->GetMakeTactics()->RecvServer();
	}
	else
	{
		recv.m_nSuccessLevel = -1;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::MAKETACTICS_DIGITAMA_SC_LV, &recv );
	}
}
void cCliGame::RecvHatchUpFailure(void)
{
	u4 nTamerUID = 0;
	pop(nTamerUID);
	u1 nType = 0;
	pop(nType);
	CsC_AvObject* pTamer = g_pMngCollector->GetObject( nTamerUID );
	if( pTamer == NULL )
		return;
	if( nType == 0 )
	{
		if( pTamer->GetLeafRTTI() != RTTI_TAMER_USER )
		{
			pTamer->SetAnimation( ANI::NPC_NO );
			pTamer->GetProp_Effect()->AddEffect( "System\\Tactics_fail.nif", 1.0f, nsEFFECT::POS_CHARPOS );			
		}		
	}
	if( pTamer->GetLeafRTTI() != RTTI_TAMER_USER )
		return;
	switch(nType)
	{
	case 0 :
		if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAKE_TACTICS ) )
		{
			g_pGameIF->GetMakeTactics()->RecvResult( cMakeTactics::DEL_DIGITAMA );
			g_pGameIF->GetMakeTactics()->RecvServer();
		}
		else
		{
			cPrintMsg::PrintMsg( 20015 );
			GS2C_RECV_MAKE_DIGITAMA recv;
			recv.m_nEggType = 0;
			recv.m_nEggTradeLimitTime = 0;
			recv.m_nEggLevel = 0;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::MAKETACTICS_DIGITAMA_DATA, &recv );
		}
		break;
	case 1 :
		if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAKE_TACTICS ) )
		{
			g_pGameIF->GetMakeTactics()->RecvResult( cMakeTactics::DEL_DATA );
			g_pGameIF->GetMakeTactics()->RecvServer();
		}
		else
		{
			cPrintMsg::PrintMsg( 20016 );
		}
		break;
		case 2 :
		if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAKE_TACTICS ) )
		{
		g_pGameIF->GetMakeTactics()->RecvResult( cMakeTactics::DEL_BACKUP );
		g_pGameIF->GetMakeTactics()->RecvServer();
		}
		else
		{
			cPrintMsg::PrintMsg( 30503 );
#ifdef MINIGAME
			GS2C_RECV_MAKE_DIGITAMA_MINIGAME_INIT kRecv;
			kRecv.nResult = false;
			kRecv.nSuccessCnt = 0;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_MINIGAME_INIT, &kRecv );
#endif
			GS2C_RECV_MAKE_DIGITAMA_LV recv;
			recv.m_nEggLevel = 0;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::MAKETACTICS_DIGITAMA_LV, &recv );
		}
		break;
	}
}
void cCliGame::RecvHatchBackupDisk(void)
{
}
void cCliGame::RecvHatchBackupDiskCancel(void)
{
}
void cCliGame::RecvHatchIn(void)	// ë¶€í™” ë””ì§€íƒ€ë§ˆ ë“±ë¡ ê²°ê³¼
{
	// ì‹¤íŒ¨ì‹œ ì—°ê²° ì¢…ë£Œ,
	// ì„±ê³µì‹œ ê·¸ëƒ¥ ì§„í–‰, ê²°ê³¼ê°€ ì—†ì–´ë„ ë¬´ê´€í• ë“¯...
}
#ifdef CROSSWARS_SYSTEM
void cCliGame::RecvCrossWarsHatch(void)	// ë¶€í™” í™•ì¸ ê²°ê³¼
{
	u1 nArrIDX;
	pop(nArrIDX);
	cData_CrossTactics::sCross Tactics;	
	pop( Tactics.s_nEnable );
	pop( Tactics.s_Type );
	std::wstring digimonName;
	pop( digimonName );
	wmemcpy_s( Tactics.s_szName, sizeof(Tactics.s_szName), digimonName.c_str(), digimonName.length() );
	u2 nScale;
	pop( nScale );
	Tactics.s_fScale = nScale*0.0001f;
	pop( Tactics.s_nExp);
	Tactics.s_nExp = Tactics.s_nExp / 100;
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
	pop( Tactics.s_nTranscendenceExp );
#endif
	pop( Tactics.s_nLevel );
	pop( Tactics.s_Attribute, sizeof( Tactics.s_Attribute ) );
	pop( Tactics.s_dwBaseDigimonID );
	DBG("nBaseEvoUnitIDX : %d", Tactics.s_dwBaseDigimonID );
	pop( Tactics.s_nMaxEvoUnit);
	pop( &Tactics.s_EvoUnit[ 1 ], sizeof(cEvoUnit)*Tactics.s_nMaxEvoUnit );
	memset(&Tactics.s_AttributeExp, NULL, sizeof(n2)*NewAttribute::MaxDigitalType );
	memset(&Tactics.s_NatureExp, NULL, sizeof(n2)*NewAttribute::MaxNatualType );
	u2 nInchantLevel = 0;
	u2 nExtentionParameter[6] = {0,};
	pop( Tactics.s_nEnchantLevel );
	pop( Tactics.s_ExtendAttribute, sizeof( Tactics.s_ExtendAttribute ) );
	pop( Tactics.s_ExtendAttributeLV, sizeof( Tactics.s_ExtendAttributeLV ) );
	g_pDataMng->GetCrossTactics()->AddTactics( &Tactics, nArrIDX );
	g_pTacticsAni->Set_bAniProcess(true);	// ë¶€í™” ì• ë‹ˆ ì‹œìž‘ìœ„ì¹˜ ì„¤ì •
	g_pTacticsAni->Start( Tactics.s_Type.m_nType, g_pCharMng->GetTamerUser() );	// ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”	
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
	//==========================================================================================================
	// ì—…ì  ì²´í¬
	//==========================================================================================================
	//g_pDataMng->GetAchieve()->CheckType( cData_Achieve::CA_GetTactics, Tactics.s_Type.m_nType );
}
void cCliGame::RecvCrossWarsEnergize(void)
{
	// í¬ë¡œìŠ¤ìš©ë³‘ì´ ì´ë¯¸ ìžˆëŠ”ê²½ìš°. ë¡œë”ì— í™œì„±í™”
	u1 nLoaderIdx;
	u1 nEnable;
	pop(nLoaderIdx);
	pop(nEnable);
	g_pDataMng->GetCrossTactics()->GetTactics(nLoaderIdx)->s_nEnable = nEnable;
}
void cCliGame::RecvCrossNotReg(void)
{
	u4 nTargetUID = 0;
	n2 nInvenPos = 0;
	pop(nTargetUID);
	pop(nInvenPos);
	//g_pDataMng->ServerCrossItemUseSuccess( nTargetUID, TO_ID(nInvenPos) );
	CsPoint pos( -1, -1 );
	pos = CURSOR_ST.GetPos();
	cCreateName::DeleteInstance( cBaseWindow::WT_CHANGE_T_NAME, 0 );
	cCreateName::DeleteInstance( cBaseWindow::WT_CHANGE_D_NAME, 0 );
	cCreateName::DeleteInstance( cBaseWindow::WT_MAKE_TACTICS, 0 );
	cCreateName::DeleteInstance( cBaseWindow::WT_MAKE_CROSSTACTICS, 0 );
	cCreateName* pCName = cCreateName::NewInstance( cBaseWindow::WT_MAKE_CROSSTACTICS, 0, pos );			
	if( pCName )
		pCName->SetValue1(  nInvenPos  );
	//g_pDataMng->ItemUnlock(  nInvenPos  );	
}
#endif
void cCliGame::RecvHatchOut(void)	// ë¶€í™” í™•ì¸ ê²°ê³¼
{
	u4 nArrIDX;
	pop(nArrIDX);
	cData_PostLoad::sDATA Tactics;	
	pop( Tactics.s_Type );
	std::wstring szTempName;
	pop( szTempName );
	_tcscpy_s( Tactics.s_szName, szTempName.c_str() );
	u2 nScale;
	pop( nScale );
	Tactics.s_fScale = nScale*0.0001f;
	pop( Tactics.s_nExp);
	Tactics.s_nExp = Tactics.s_nExp / 100;
	pop( Tactics.s_nTranscendenceExp );
	pop( Tactics.s_nLevel );
	pop( Tactics.s_Attribute, sizeof( Tactics.s_Attribute ) );
	
	pop( Tactics.s_HatchLevel);
	pop( Tactics.s_dwBaseDigimonID );
	DBG("nBaseEvoUnitIDX : %d", Tactics.s_dwBaseDigimonID );
	pop( Tactics.s_nMaxEvoUnit);
	pop( &Tactics.s_EvoUnit[ 1 ], sizeof(cEvoUnit)*Tactics.s_nMaxEvoUnit );
	memset(&Tactics.s_AttributeExp, NULL, sizeof(n2)*NewAttribute::MaxDigitalType );
	memset(&Tactics.s_NatureExp, NULL, sizeof(n2)*NewAttribute::MaxNatualType );
	u2 nInchantLevel = 0;
	u2 nExtentionParameter[6] = {0,};
	pop( Tactics.s_nEnchantLevel );
	pop( Tactics.s_ExtendAttribute, sizeof( Tactics.s_ExtendAttribute ) );
	pop( Tactics.s_ExtendAttributeLV, sizeof( Tactics.s_ExtendAttributeLV ) );
	g_pDataMng->GetTactics()->AddTactics( &Tactics, nArrIDX - 1 );
	//==========================================================================================================
	// ì—…ì  ì²´í¬
	//==========================================================================================================
	GS2C_RECV_CHECKTYPE recvAchieve;
	recvAchieve.nType = AchieveContents::CA_GetTactics;
	recvAchieve.nValue1 = Tactics.s_Type.m_nType;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_CHECKTYPE, &recvAchieve );
	GS2C_RECV_ENCYCLOPEDIA_EVOLUTION recv;
	recv.nDigimonID = Tactics.s_dwBaseDigimonID;
	recv.nScale = nScale;
	recv.nType = 0;
	for( int i = 1 ; i < nLimit::EvoUnit ; i++ )
	{
		recv.nEvoUnit[i] = Tactics.s_EvoUnit[i];
	}
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::ENCYCLOPEDIA_EVOL_UPDATE, &recv );	
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_HATCHOUT_COMPLETE, NULL );
}
void cCliGame::RecvBattleTagSuccess(void)	// ì „íˆ¬ì¤‘ ìš©ë³‘ ë³€ê²½ ì„±ê³µ
{
	// ì•„ì´í…œ ê°ì†Œ
	int nSlot = g_pDataMng->GetInven()->GetFirstSlot_Item_TypeLS( nItem::BattleTag, 0 );	
	assert_cs( nSlot != cData_Inven::INVALIDE_INVEN_INDEX );
	if( nSlot != cData_Inven::INVALIDE_INVEN_INDEX )
	{
		g_pDataMng->GetInven()->GetData( nSlot )->DecreaseCount( 1 );
	}	
	DWORD dwMask = nsEFFECT::POS_CHARPOS | nsEFFECT::DIR_CAMERA_AXIS_Z;
	g_pCharMng->GetTamerUser()->GetProp_Effect()->AddEffect( "system\\BattleTag.nif", 1.0f, dwMask );
	switch( g_pCharMng->GetTamerUser()->GetModelID() )
	{
	case CODE_MASARU:
		g_pCharMng->GetTamerUser()->PlaySound( "Tamer\\masaru\\masaru_hit03.wav" );
		break;
	case CODE_TOHMA:	
		g_pCharMng->GetTamerUser()->PlaySound( "Tamer\\tohma\\yoshino_hit03.wav" );
		break;
	case CODE_YOSHINO:
		g_pCharMng->GetTamerUser()->PlaySound( "Tamer\\yoshino\\tohma_hit03.wav" );
		break;
	case CODE_IKUTO:
		g_pCharMng->GetTamerUser()->PlaySound( "Tamer\\ikuto\\ikuto_hit03.wav" );
		break;
	}
	g_pCharMng->GetDigimonUser( 0 )->GetProp_Effect()->AddEffect( "system\\BattleTag_Effect.nif", 1.0f, dwMask );			
}
void cCliGame::RecvBattleTagFailure(void)	// ì „íˆ¬ì¤‘ ìš©ë³‘ ë³€ê²½ ì‹¤íŒ¨
{
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CHANGE_PARTNER, 0, NULL );
#ifdef CROSSWARS_SYSTEM
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSCHANGE_PARTNER, 0, NULL );
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSRECOVER_PARTNER, 0, NULL );
#endif
}
#ifdef CROSSWARS_SYSTEM
void cCliGame::RecvCrossChangePartner(void) // í¬ë¡œìŠ¤ì›Œì¦ˆì™€ íŒŒíŠ¸ë„ˆ ë³€ê²½
{
	n4 nPrevType;
	cData_ServerSync::sCHANGE_PARTNER sync;
	pop(sync.nUID);
	if(sync.nUID == 0)
	{
		// ìžì‹ ì—ê²Œë§Œ ì „ì†¡ë¨
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = UISTRING_TEXT( "CHAT_NOT_POSSIBLE_CHANGE_PARTNER" );
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	pop(nPrevType);
	pop(sync.nNextArrIDX); // ë‹¤ìŒìœ¼ë¡œ ì„ íƒëœ ìš©ë³‘ì˜ ArrIDX
	pop(sync.nNextType);
	pop(sync.nNextLevel);
#ifdef THAI_WORDTRANSFORM	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ ë³€ê²½ ì‹œ ì„œë²„ì—ì„œ char íƒ€ìž…ìœ¼ë¡œ ì´ë¦„ì„ ë³´ë‚´ì¤€ê²ƒì„ tchar ë¡œ ë³€í™˜í•˜ì—¬ ì €ìž¥. lks007	12.08.10.
	CHAR _tempName[Language::pLength::name + 1];
	pop(_tempName);
	M2W(sync.szNextDigimonName, _tempName, Language::pLength::name + 1);
#else
	{
		std::wstring szNextDigimonName;
		pop(szNextDigimonName);
		_tcsncpy_s(sync.szNextDigimonName, Language::pLength::name, szNextDigimonName.c_str(), _TRUNCATE);
	}
#endif
	pop(sync.nNextScale);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( sync.nUID );
	if( pObject == NULL )
		return;
	CDigimon* pDigimon = (CDigimon*)pObject;
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: BL ë¸”ë¡ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  7: HP ì²´ë ¥
	u2 nEnchantTotalLevel;
	pop( nEnchantTotalLevel );
	pDigimon->GetEnchantStat()->SetEnchantLevel( nEnchantTotalLevel );
	u2 nEnchantLevel[ nExtendState::MaxExtendStat ] = {0, };
	pop( nEnchantLevel[ nExtendState::ET_AT ] );
	pop( nEnchantLevel[ nExtendState::ET_BL ] );
	pop( nEnchantLevel[ nExtendState::ET_CR ] );
	pop( nEnchantLevel[ nExtendState::ET_AS ] );
	pop( nEnchantLevel[ nExtendState::ET_EV ] );
	pop( nEnchantLevel[ nExtendState::ET_HT ] );
	pop( nEnchantLevel[ nExtendState::ET_HP ] );
	pDigimon->GetBuff()->GetBuff()->clear();
	pDigimon->GetBuff()->GetDeBuff()->clear();
	u2 nBuffCount = 0;
	pop( nBuffCount );					// ë²„í”„
	if( nBuffCount != 0 )
	{
		u2 BuffCode = 0;
		u2 BuffClass = 0;
		u4 nEndTS = 0;
		u4 dwSkillCode = 0;
		for(int i = 0; i < nBuffCount; i++)
		{ 
			pop(BuffCode);			// ë²„í”„ ì½”ë“œ
			pop(BuffClass);			// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(nEndTS);			// ë²„í”„ ë§Œë£Œ ì‹œê°„
			pop(dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			pDigimon->GetBuff()->SetBuff( BuffCode, 0, nEndTS, BuffClass, dwSkillCode );
		}
	}
	// ìºìŠ¤íŒ… íƒ€ìž…ë³„ë¡œ êµ¬í˜„
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
		{			
			pDigimon->Scene_CrossChangePartner( sync.nNextType, sync.nNextArrIDX, sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );
			CTamer* pTamer = g_pCharMng->GetTamer( pDigimon->GetTamerLink() );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, 0 );
			}
		}
		break;
	case RTTI_DIGIMON_USER:
		{	
			// ë³¸ì¸ ê²ƒë§Œ ë°›ëŠ”ë‹¤
			cData_CrossTactics::sCross* pNextData = g_pDataMng->GetCrossTatics()->GetTactics( sync.nNextArrIDX );			
			pop( pNextData->s_Attribute );					// ìŠ¤í…Ÿ
			//pop(pNextData->s_nEnable);			
			
			g_pCrossEvent->EventStart( pNextData->s_dwBaseDigimonID );
			g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSCHANGE_PARTNER, 0, &sync );
		}
		break;
	default:
		assert_cs( false );
	}	
}
void cCliGame::RecvCrossChangePartner2(void)
{
	n4 nPrevType;
	cData_ServerSync::sCHANGE_PARTNER sync;
	pop(sync.nUID);
	if(sync.nUID == 0)
	{
		// ìžì‹ ì—ê²Œë§Œ ì „ì†¡ë¨
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = UISTRING_TEXT( "CHAT_NOT_POSSIBLE_CHANGE_PARTNER" );
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	pop(nPrevType);
	pop(sync.nNextArrIDX); // ë‹¤ìŒìœ¼ë¡œ ì„ íƒëœ ìš©ë³‘ì˜ ArrIDX
	pop(sync.nNextType);
	pop(sync.nNextLevel);
#ifdef THAI_WORDTRANSFORM	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ ë³€ê²½ ì‹œ ì„œë²„ì—ì„œ char íƒ€ìž…ìœ¼ë¡œ ì´ë¦„ì„ ë³´ë‚´ì¤€ê²ƒì„ tchar ë¡œ ë³€í™˜í•˜ì—¬ ì €ìž¥. lks007	12.08.10.
	CHAR _tempName[Language::pLength::name + 1];
	pop(_tempName);
	M2W(sync.szNextDigimonName, _tempName, Language::pLength::name + 1);
#else
	{
		std::wstring szNextDigimonName;
		pop(szNextDigimonName);
		_tcsncpy_s(sync.szNextDigimonName, Language::pLength::name, szNextDigimonName.c_str(), _TRUNCATE);
	}
#endif
	pop(sync.nNextScale);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( sync.nUID );
	if( pObject == NULL )
		return;
	CDigimon* pDigimon = (CDigimon*)pObject;
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: BL ë¸”ë¡ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  7: HP ì²´ë ¥
	u2 nEnchantTotalLevel;
	pop( nEnchantTotalLevel );
	pDigimon->GetEnchantStat()->SetEnchantLevel( nEnchantTotalLevel );
	u2 nEnchantLevel[ nExtendState::MaxExtendStat ] = {0, };
	pop( nEnchantLevel[ nExtendState::ET_AT ] );
	pop( nEnchantLevel[ nExtendState::ET_BL ] );
	pop( nEnchantLevel[ nExtendState::ET_CR ] );
	pop( nEnchantLevel[ nExtendState::ET_AS ] );
	pop( nEnchantLevel[ nExtendState::ET_EV ] );
	pop( nEnchantLevel[ nExtendState::ET_HT ] );
	pop( nEnchantLevel[ nExtendState::ET_HP ] );
	pDigimon->GetBuff()->GetBuff()->clear();
	pDigimon->GetBuff()->GetDeBuff()->clear();
	u2 nBuffCount = 0;
	pop( nBuffCount );					// ë²„í”„
	if( nBuffCount != 0 )
	{
		u2 BuffCode = 0;
		u2 BuffClass = 0;
		u4 nEndTS = 0;
		u4 dwSkillCode = 0;
		for(int i = 0; i < nBuffCount; i++)
		{ 
			pop(BuffCode);			// ë²„í”„ ì½”ë“œ
			pop(BuffClass);			// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(nEndTS);			// ë²„í”„ ë§Œë£Œ ì‹œê°„
			pop(dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			pDigimon->GetBuff()->SetBuff( BuffCode, 0, nEndTS, BuffClass, dwSkillCode );
		}
	}
	// ìºìŠ¤íŒ… íƒ€ìž…ë³„ë¡œ êµ¬í˜„
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
		{			
			pDigimon->Scene_CrossChangePartner2( sync.nNextType, sync.nNextArrIDX , sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );
			CTamer* pTamer = g_pCharMng->GetTamer( pDigimon->GetTamerLink() );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, 0 );
			}
		}
		break;
	case RTTI_DIGIMON_USER:
		{
			cData_PostLoad::sDATA* pNextData = g_pDataMng->GetCrossTactics()->GetTempTactics();	
			pNextData->s_fScale = sync.nNextScale*0.0001f;//
			pNextData->s_nLevel = sync.nNextLevel;
			pNextData->s_dwBaseDigimonID = sync.nNextType;
			_tcscpy_s( pNextData->s_szName, Language::pLength::name + 1, sync.szNextDigimonName );
			pop( pNextData->s_Attribute );
			pop( pNextData->s_Type );
			pop( pNextData->s_nExp );
			pNextData->s_nExp = pNextData->s_nExp / 100;
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
			pop( pNextData->s_nTranscendenceExp );
#endif
			pop( pNextData->s_nMaxEvoUnit );
			pop( &pNextData->s_EvoUnit[ 1 ], sizeof(cEvoUnit)*pNextData->s_nMaxEvoUnit );
			pop( pNextData->s_ExtendAttribute, sizeof(pNextData->s_ExtendAttribute) );
		
			// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxDigitalType; i++)
			{		
				n2 AttributeExp;
				pop(AttributeExp);
				pNextData->s_AttributeExp[i] = AttributeExp;
			}
			// ë””ì§€ëª¬ ìžì—°ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxNatualType; i++)
			{		
				n2 NatureExp;
				pop(NatureExp);		
				pNextData->s_NatureExp[i] = NatureExp;
			}
			g_pCrossEvent->EventStart( pNextData->s_dwBaseDigimonID );
			g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSCHANGE_PARTNER, 0, &sync );	// ì¡°ê·¸ëž˜ìŠ¤ í”Œëž˜ê·¸ê°€ ì•„ë‹ˆë©´ 
		}
		break;
	default:
		assert_cs( false );
	}
}
void cCliGame::RecvCrossWarsCrownHeal(void)
{
	n4 nHeal;
	pop(nHeal);
	CTamerUser::sUSER_STAT* pBaseStat = (CTamerUser::sUSER_STAT*)g_pCharMng->GetTamerUser()->GetBaseStat();	
	pBaseStat->SetCR( pBaseStat->GetCR() + (nHeal));
	// ë§¥ìŠ¤ì¹˜ë³´ë‹¤ ë„˜ìœ¼ë©´ ìµœëŒ€ì¹˜ ê³ ì •
	if(pBaseStat->GetMaxCR() < pBaseStat->GetCR() )
		pBaseStat->SetCR( pBaseStat->GetMaxCR());
	// í˜„ìž¬ì¹˜ê°€ 0ë³´ë‹¤ ìž‘ìœ¼ë©´ 0ê³ ì •
	else if( 0 > pBaseStat->GetCR() )
		pBaseStat->SetCR( 0);
}
void cCliGame::RecvCrossWarsLoader(void)
{
	n4 nMcr;
	n4 nCr;
	pop(nMcr);
	pop(nCr);	
	CTamerUser::sUSER_STAT* pBaseStat = (CTamerUser::sUSER_STAT*)g_pCharMng->GetTamerUser()->GetBaseStat();
	pBaseStat->SetMaxCR(nMcr);
	pBaseStat->SetCR(nCr);
	CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
	if( pTamerUser == NULL )
		return;			
	pTamerUser->PlaySound(SOUND_ENABLE_CROSSLOADER);
	cData_Inven_Cross* pInvenCross = g_pDataMng->GetInvenCross();
	pInvenCross->SetCrossLoader(1); // í™œì„±í™”
	g_pDataMng->GetPostLoad()->GetTamerData()->s_nCrossLoader = 1;
	if(false == g_pGameIF->IsActiveWindow(cBaseWindow::WT_BGSPRITE, cBGSprite::POINTTER_CROSSBUTTON))
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_BGSPRITE, cBGSprite::POINTTER_CROSSBUTTON, cBGSprite::CROSSBUTTON );	
	// í¬ë¡œìŠ¤ UI ë¦¬ë¡œë“œ.
	GAME_EVENT_ST.OnEvent( EVENT_CODE::OPEN_CROSSLOADER, NULL );
}
void cCliGame::RecvCrossWarsRelease(void)
{
	n4 nPrevType;
	cData_ServerSync::sCHANGE_PARTNER sync;
	pop(sync.nUID);
	if(sync.nUID == 0)
	{
		// ìžì‹ ì—ê²Œë§Œ ì „ì†¡ë¨
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = UISTRING_TEXT( "CHAT_NOT_POSSIBLE_CHANGE_PARTNER" );
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	pop(nPrevType);
	pop(sync.nNextArrIDX); // ë‹¤ìŒìœ¼ë¡œ ì„ íƒëœ ìš©ë³‘ì˜ ArrIDX
	pop(sync.nNextType);
	pop(sync.nNextLevel);
#ifdef THAI_WORDTRANSFORM	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ ë³€ê²½ ì‹œ ì„œë²„ì—ì„œ char íƒ€ìž…ìœ¼ë¡œ ì´ë¦„ì„ ë³´ë‚´ì¤€ê²ƒì„ tchar ë¡œ ë³€í™˜í•˜ì—¬ ì €ìž¥. lks007	12.08.10.
	CHAR _tempName[Language::pLength::name + 1];
	pop(_tempName);
	M2W(sync.szNextDigimonName, _tempName, Language::pLength::name + 1);
#else
	{
		std::wstring szNextDigimonName;
		pop(szNextDigimonName);
		_tcsncpy_s(sync.szNextDigimonName, Language::pLength::name, szNextDigimonName.c_str(), _TRUNCATE);
	}
#endif
	pop(sync.nNextScale);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( sync.nUID );
	if( pObject == NULL )
		return;
	CDigimon* pDigimon = (CDigimon*)pObject;
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: BL ë¸”ë¡ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  7: ì²´ë ¥
	u2 nEnchantTotalLevel;
	pop( nEnchantTotalLevel );
	pDigimon->GetEnchantStat()->SetEnchantLevel( nEnchantTotalLevel );
	u2 nEnchantLevel[ nExtendState::MaxExtendStat ] = {0, };
	pop( nEnchantLevel[ nExtendState::ET_AT ] );
	pop( nEnchantLevel[ nExtendState::ET_BL ] );
	pop( nEnchantLevel[ nExtendState::ET_CR ] );
	pop( nEnchantLevel[ nExtendState::ET_AS ] );
	pop( nEnchantLevel[ nExtendState::ET_EV ] );
	pop( nEnchantLevel[ nExtendState::ET_HT ] );
	pop( nEnchantLevel[ nExtendState::ET_HP ] );
	pDigimon->GetBuff()->GetBuff()->clear();
	pDigimon->GetBuff()->GetDeBuff()->clear();
	u2 nBuffCount = 0;
	pop( nBuffCount );					// ë²„í”„
	if( nBuffCount != 0 )
	{
		u2 BuffCode = 0;
		u2 BuffClass = 0;
		u4 nEndTS = 0;
		u4 dwSkillCode = 0;
		for(int i = 0; i < nBuffCount; i++)
		{ 
			pop(BuffCode);			// ë²„í”„ ì½”ë“œ
			pop(BuffClass);			// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(nEndTS);			// ë²„í”„ ë§Œë£Œ ì‹œê°„
			pop(dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			pDigimon->GetBuff()->SetBuff( BuffCode, 0, nEndTS, BuffClass, dwSkillCode );
		}
	}
	// ìºìŠ¤íŒ… íƒ€ìž…ë³„ë¡œ êµ¬í˜„
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
		{			
			pDigimon->Scene_ChangePartner( sync.nNextType, sync.nNextArrIDX , sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );
			CTamer* pTamer = g_pCharMng->GetTamer( pDigimon->GetTamerLink() );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, 0 );
			}
		}
		break;
	case RTTI_DIGIMON_USER:
		{	
			//ìž„ì‹œ ìš©ë³‘ ë°ì´íƒ€ë¥¼ ë§Œë“¤ì–´ì„œ ë„£ê³  cData_Tactics::ChangeCrossTactics ì—ì„œ ìš©ë³‘ ë°ì´í„° êµì²´í•˜ìž 		
		
			cData_PostLoad::sDATA* pNextData = g_pDataMng->GetTactics()->GetTempTactics();			
			pNextData->s_fScale = sync.nNextScale*0.0001f;//
			pNextData->s_nLevel = sync.nNextLevel;//
			pNextData->s_dwBaseDigimonID = sync.nNextType;//
			_tcscpy_s( pNextData->s_szName, Language::pLength::name + 1, sync.szNextDigimonName );//
			pop( pNextData->s_Attribute );//
			pop( pNextData->s_Type );//
			pop( pNextData->s_nExp );//
			pNextData->s_nExp = pNextData->s_nExp / 100;
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
			pop( pNextData->s_nTranscendenceExp );
#endif
			pop( pNextData->s_nMaxEvoUnit );//
			pop( &pNextData->s_EvoUnit[ 1 ], sizeof(cEvoUnit)*pNextData->s_nMaxEvoUnit );//
			pop( pNextData->s_ExtendAttribute, sizeof(pNextData->s_ExtendAttribute) );//
		
			// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxDigitalType; i++)
			{		
				n2 AttributeExp;
				pop(AttributeExp);//
				pNextData->s_AttributeExp[i] = AttributeExp;
			}
			// ë””ì§€ëª¬ ìžì—°ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxNatualType; i++)
			{		
				n2 NatureExp;
				pop(NatureExp);	//	
				pNextData->s_NatureExp[i] = NatureExp;
			}
			g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSRECOVER_PARTNER, 0, &sync );	// ì¡°ê·¸ëž˜ìŠ¤ í”Œëž˜ê·¸ê°€ ì•„ë‹ˆë©´ 
		}
		break;
	default:
		assert_cs( false );
	}	
}
void cCliGame::RecvCrossWarsCRRelease(void)
{
	n4 nPrevType;
	cData_ServerSync::sCHANGE_PARTNER sync;
	pop(sync.nUID);
	if(sync.nUID == 0)
	{
		// ìžì‹ ì—ê²Œë§Œ ì „ì†¡ë¨
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = UISTRING_TEXT( "CHAT_NOT_POSSIBLE_CHANGE_PARTNER" );
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	pop(nPrevType);
	pop(sync.nNextArrIDX); // ë‹¤ìŒìœ¼ë¡œ ì„ íƒëœ ìš©ë³‘ì˜ ArrIDX
	pop(sync.nNextType);
	pop(sync.nNextLevel);
#ifdef THAI_WORDTRANSFORM	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ ë³€ê²½ ì‹œ ì„œë²„ì—ì„œ char íƒ€ìž…ìœ¼ë¡œ ì´ë¦„ì„ ë³´ë‚´ì¤€ê²ƒì„ tchar ë¡œ ë³€í™˜í•˜ì—¬ ì €ìž¥. lks007	12.08.10.
	CHAR _tempName[Language::pLength::name + 1];
	pop(_tempName);
	M2W(sync.szNextDigimonName, _tempName, Language::pLength::name + 1);
#else
	{
		std::wstring szNextDigimonName;
		pop(szNextDigimonName);
		_tcsncpy_s(sync.szNextDigimonName, Language::pLength::name, szNextDigimonName.c_str(), _TRUNCATE);
	}
#endif
	pop(sync.nNextScale);
	CsC_AvObject* pObject = g_pMngCollector->GetObject( sync.nUID );
	if( pObject == NULL )
		return;
	CDigimon* pDigimon = (CDigimon*)pObject;
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: BL ë¸”ë¡ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  7: HP ì²´ë ¥
	u2 nEnchantTotalLevel;
	pop( nEnchantTotalLevel );
	pDigimon->GetEnchantStat()->SetEnchantLevel( nEnchantTotalLevel );
	u2 nEnchantLevel[ nExtendState::MaxExtendStat ] = {0, };
	pop( nEnchantLevel[ nExtendState::ET_AT ] );
	pop( nEnchantLevel[ nExtendState::ET_BL ] );
	pop( nEnchantLevel[ nExtendState::ET_CR ] );
	pop( nEnchantLevel[ nExtendState::ET_AS ] );
	pop( nEnchantLevel[ nExtendState::ET_EV ] );
	pop( nEnchantLevel[ nExtendState::ET_HT ] );
	pop( nEnchantLevel[ nExtendState::ET_HP ] );
	pDigimon->GetBuff()->GetBuff()->clear();
	pDigimon->GetBuff()->GetDeBuff()->clear();
	u2 nBuffCount = 0;
	pop( nBuffCount );					// ë²„í”„
	if( nBuffCount != 0 )
	{
		u2 BuffCode = 0;
		u2 BuffClass = 0;
		u4 nEndTS = 0;
		u4 dwSkillCode = 0;
		for(int i = 0; i < nBuffCount; i++)
		{ 
			pop(BuffCode);			// ë²„í”„ ì½”ë“œ
			pop(BuffClass);			// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(nEndTS);			// ë²„í”„ ë§Œë£Œ ì‹œê°„
			pop(dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			pDigimon->GetBuff()->SetBuff( BuffCode, 0, nEndTS, BuffClass, dwSkillCode );
		}
	}
	// ìºìŠ¤íŒ… íƒ€ìž…ë³„ë¡œ êµ¬í˜„
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
		{			
			pDigimon->Scene_ChangePartner( sync.nNextType, sync.nNextArrIDX , sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );
			CTamer* pTamer = g_pCharMng->GetTamer( pDigimon->GetTamerLink() );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, 0 );
			}
		}
		break;
	case RTTI_DIGIMON_USER:
		{	
			//ìž„ì‹œ ìš©ë³‘ ë°ì´íƒ€ë¥¼ ë§Œë“¤ì–´ì„œ ë„£ê³  cData_Tactics::ChangeCrossTactics ì—ì„œ ìš©ë³‘ ë°ì´í„° êµì²´í•˜ìž 		
			cData_PostLoad::sDATA* pNextData = g_pDataMng->GetTactics()->GetTempTactics();			
			pNextData->s_fScale = sync.nNextScale*0.0001f;//
			pNextData->s_nLevel = sync.nNextLevel;//
			pNextData->s_dwBaseDigimonID = sync.nNextType;//
			_tcscpy_s( pNextData->s_szName, Language::pLength::name + 1, sync.szNextDigimonName );//
			pop( pNextData->s_Attribute );//
			pop( pNextData->s_Type );//
			pop( pNextData->s_nExp );//
			pNextData->s_nExp = pNextData->s_nExp / 100;
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
			pop( pNextData->s_nTranscendenceExp );
#endif
			pop( pNextData->s_nMaxEvoUnit );//
			pop( &pNextData->s_EvoUnit[ 1 ], sizeof(cEvoUnit)*pNextData->s_nMaxEvoUnit );//
			pop( pNextData->s_ExtendAttribute, sizeof(pNextData->s_ExtendAttribute) );//
			// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxDigitalType; i++)
			{		
				n2 AttributeExp;
				pop(AttributeExp);//
				pNextData->s_AttributeExp[i] = AttributeExp;
			}
			// ë””ì§€ëª¬ ìžì—°ì†ì„± ê²½í—˜ì¹˜
			for( int i=0; i < NewAttribute::MaxNatualType; i++)
			{		
				n2 NatureExp;
				pop(NatureExp);	//	
				pNextData->s_NatureExp[i] = NatureExp;
			}
			//g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CROSSRECOVER_PARTNER, 0, &sync );			
			// êµì²´ ì„±ê³µ		
			{
				CsC_AvObject* pDigimon = g_pMngCollector->GetObject( sync.nUID );
				if( pDigimon == NULL )
				{
					assert_cs( false );
					return;
				}
				assert_cs( pDigimon->GetLeafRTTI() == RTTI_DIGIMON_USER );
				( (CDigimon*)pDigimon )->GetBuff()->ReleaseBuff( nsBuff::BK_CANDY_SIZE_CHANGE );
				( (CDigimon*)pDigimon )->Scene_CrossRecoverPartner( sync.nNextType, sync.nNextArrIDX, sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );				
				CTamer* pTamer = g_pCharMng->GetTamerUser();
				assert_cs( pTamer );
				pTamer->Scene_Evol( pDigimon, 0 );
				g_pDataMng->GetServerSync()->SendServer( cData_ServerSync::CROSSRECOVER_PARTNER_ING, 0 );
				// ì¸í„°íŽ˜ì´ìŠ¤ ë‹«ì•„ì£¼ìž
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_TALK );
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_QUEST_REV );
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_QUEST_COMP );								
			}
		}
		break;
	default:
		assert_cs( false );
	}	
}
void cCliGame::RecvCrossWarsRecover(void)
{
	n1 nType;
	pop(nType);
	if(0 == nType)
	{
		RecvCrossWarsRelease(); // í•´ì œ ìš”ì²­ì‹œ
		//RecvCrossWarsCRRelease();
	}
	else if(1 == nType)
	{
		RecvCrossWarsCRRelease(); // CR ë–¨ì–´ì ¸ì„œ
	}
}
#endif
void cCliGame::RecvChangePartner(void) // íŒŒíŠ¸ë„ˆ ë³€ê²½
{
	n4 nPrevType = 0;
	cData_ServerSync::sCHANGE_PARTNER sync;
	pop(sync.nUID);
	if(sync.nUID == 0)
	{
		// ìžì‹ ì—ê²Œë§Œ ì „ì†¡ë¨
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::NORMAL_TEXT;
		CProtocol.m_wStr = UISTRING_TEXT( "CHAT_NOT_POSSIBLE_CHANGE_PARTNER" );
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	pop(nPrevType);
	pop(sync.nNextArrIDX); // ë‹¤ìŒìœ¼ë¡œ ì„ íƒëœ ìš©ë³‘ì˜ ArrIDX
	pop(sync.nNextType);
	pop(sync.nNextLevel);
#ifdef THAI_WORDTRANSFORM	// íŒŒíŠ¸ë„ˆ ë””ì§€ëª¬ ë³€ê²½ ì‹œ ì„œë²„ì—ì„œ char íƒ€ìž…ìœ¼ë¡œ ì´ë¦„ì„ ë³´ë‚´ì¤€ê²ƒì„ tchar ë¡œ ë³€í™˜í•˜ì—¬ ì €ìž¥. lks007	12.08.10.
	CHAR _tempName[Language::pLength::name + 1];
	pop(_tempName);
	M2W(sync.szNextDigimonName, _tempName, Language::pLength::name + 1);
#else
	{
		std::wstring szNextDigimonName;
		pop(szNextDigimonName);
		_tcsncpy_s(sync.szNextDigimonName, Language::pLength::name, szNextDigimonName.c_str(), _TRUNCATE);
	}
#endif
	pop(sync.nNextScale);
	u4 nEndTick = 0;		// ScaleEvent ë‚¨ì€ì‹œê°„
	pop(nEndTick);			// 0ì´ ì•„ë‹ˆë©´ ScaleEvent ë²„í”„ íˆ´íŒ ì„¸íŒ…
	nsCSDEBUG::CrashLogger::LogMessage( "PARTNER_SWITCH recv uid=%u prevType=%d arr=%u nextType=%d level=%d scale=%u endTick=%u nameLen=%d",
		(unsigned)sync.nUID,
		nPrevType,
		(unsigned)sync.nNextArrIDX,
		sync.nNextType,
		(int)sync.nNextLevel,
		(unsigned)sync.nNextScale,
		(unsigned)nEndTick,
		(int)_tcslen(sync.szNextDigimonName) );
	CsC_AvObject* pObject = g_pMngCollector->GetObject( sync.nUID );
	if( pObject == NULL )
		return;
	CDigimon* pDigimon = (CDigimon*)pObject;
	// ë””ì§€ëª¬ í™•ìž¥ ëŠ¥ë ¥ì¹˜ ìˆ˜ì‹ 
	// ìˆ˜ì‹  ìˆœì„œ 1: AP(AT) ê³µê²© 2: BL ë¸”ë¡ 3: CR í¬ë¦¬í‹°ì»¬ í™•ë¥  4: AS ê³µê²© ìŠ¤í”¼ë“œ 5: EV íšŒí”¼ 6: HT ê³µê²© ì„±ê³µë¥  7: HP ì²´ë ¥
	u2 nEnchantTotalLevel;
	pop( nEnchantTotalLevel );
	pDigimon->GetEnchantStat()->SetEnchantLevel( nEnchantTotalLevel );
	u2 nEnchantLevel[ nExtendState::MaxExtendStat ] = {0, };
	pop( nEnchantLevel[ nExtendState::ET_AT ] );
	pop( nEnchantLevel[ nExtendState::ET_BL ] );
	pop( nEnchantLevel[ nExtendState::ET_CR ] );
	pop( nEnchantLevel[ nExtendState::ET_AS ] );
	pop( nEnchantLevel[ nExtendState::ET_EV ] );
	pop( nEnchantLevel[ nExtendState::ET_HT ] );
	pop( nEnchantLevel[ nExtendState::ET_HP ] );
	pDigimon->GetBuff()->ClearBuffLoopEffect();
	pDigimon->GetBuff()->GetBuff()->clear();
	pDigimon->GetBuff()->GetDeBuff()->clear();
	u2 nBuffCount = 0;
	pop( nBuffCount );					// ë²„í”„
	if( nBuffCount != 0 )
	{
		u2 BuffCode = 0;
		u2 BuffClass = 0;
		u4 nEndTS = 0;
		u4 dwSkillCode = 0;
		for(int i = 0; i < nBuffCount; i++)
		{ 
			pop(BuffCode);			// ë²„í”„ ì½”ë“œ
			pop(BuffClass);			// ë²„í”„ í´ëž˜ìŠ¤ ë ˆë²¨
			pop(nEndTS);			// ë²„í”„ ë§Œë£Œ ì‹œê°„
			pop(dwSkillCode);		// ë²„í”„ì½”ë“œê°€ ê°™ì€ ê²ƒì´ ìžˆì–´ êµ¬ë¶„ì„ ìœ„í•´ ìŠ¤í‚¬ì½”ë“œ ì €ìž¥
			pDigimon->GetBuff()->SetBuff( BuffCode, sync.nUID, nEndTS, BuffClass, dwSkillCode );
		}
	}
	// ìºìŠ¤íŒ… íƒ€ìž…ë³„ë¡œ êµ¬í˜„
	switch( pObject->GetLeafRTTI() )
	{
	case RTTI_DIGIMON:
		{			
			pDigimon->Scene_ChangePartner( sync.nNextType, sync.nNextArrIDX - 1, sync.szNextDigimonName, sync.nNextScale*0.0001f, sync.nNextLevel );
			CTamer* pTamer = g_pCharMng->GetTamer( pDigimon->GetTamerLink() );
			if( pTamer )
			{
				pTamer->Scene_Evol( pDigimon, 0 );
			}
		}
		break;
	case RTTI_DIGIMON_USER:
		{	
			// ë³¸ì¸ ê²ƒë§Œ ë°›ëŠ”ë‹¤
			cData_PostLoad::sDATA* pNextData = g_pDataMng->GetTactics()->GetTactics( sync.nNextArrIDX - 1 );
			pNextData->s_fScale = sync.nNextScale*0.0001f;//
			pNextData->s_nLevel = sync.nNextLevel;//
			pNextData->s_dwBaseDigimonID = sync.nNextType;//
			_tcscpy_s( pNextData->s_szName, Language::pLength::name + 1, sync.szNextDigimonName );//
			pop( pNextData->s_Attribute );					// ìŠ¤í…Ÿ
			// íŒŒíŠ¸ë„ˆ ì±„ì¸ì§€ ì‹œìž‘
			CDigimonUser* pDUser = g_pCharMng->GetDigimonUser( 0 );
			if(0 != pDUser->GetJointProgressFlag())
				g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CHANGE_PARTNER_WITHOUTSCENE, 0, &sync );
			else
				g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CHANGE_PARTNER, 0, &sync );	// ì¡°ê·¸ëž˜ìŠ¤ í”Œëž˜ê·¸ê°€ ì•„ë‹ˆë©´ 
			
			if(nEndTick != 0)// ì´ë²¤íŠ¸ ì‹¤í–‰
			{
				uint nTime = _TIME_TS + nEndTick / 1000;
				pDigimon->GetBuff()->SetBuff( nsBuff::BK_CANDY_SIZE_CHANGE, 0, nTime, 0 );
			}
		}
		break;
	default:
		assert_cs( false );
	}
}
void cCliGame::RecvWarehouseSlotSize(void)
{
	u2 nSlotSize;
	pop(nSlotSize);
	//ì¼ë°˜ì°½ê³ 
	//	g_pDataMng->GetWarehouse()->SetSlotCount( nSlotSize );
	GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_NORMAL_SLOTCOUNT, &nSlotSize); 
	//==========================================================================================================
	// ì—…ì  ì²´í¬
	//==========================================================================================================
	GS2C_RECV_CHECKTYPE recv;
	recv.nType = AchieveContents::CA_WareHouse_AllOpen;
	recv.nValue1 = nSlotSize;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_CHECKTYPE, &recv );
}
// ì¸ë²¤í† ë¦¬ í¬ê¸° ë³€ê²½ì‹œ
void cCliGame::RecvInvenSlotSize(void)
{
	u2 nInvenSlotSize;
	pop(nInvenSlotSize);
	SAFE_POINTER_RET( g_pDataMng );
	cData_Inven* pInven = g_pDataMng->GetInven();
	SAFE_POINTER_RET( pInven );
	pInven->SetInvenSlotCount( nInvenSlotSize );
	pInven->LoadApply();
	// ì¸ë²¤í† ë¦¬ ì¶”ê°€ê°€ë°© ì²« í™•ìž¥ì‹œ~ ê°€ë°© ì¸í„°íŽ˜ì´ìŠ¤ ì˜¤í”ˆì‹œì¼œì£¼ìž
	bool bSound = true;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::OPEN_INVENTORY, &bSound );
	//==========================================================================================================
	// ì—…ì  ì²´í¬
	//==========================================================================================================
	GS2C_RECV_CHECKTYPE recv;
	recv.nType = AchieveContents::CA_InvenSlot_AllOpen;
	recv.nValue1 = nInvenSlotSize;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_CHECKTYPE, &recv );
}
// void cCliGame::RecvMsgAllCropEventItem(void)
// {
// 	std::wstring szTamerName;
// 	u4 nItemType;
// 	u4 nItemNo;	// í•´ë‹¹ ì•„ì´í…œì˜ dm_item_list-item-ItemNo
// 
// 	u2 nProtocol;
// 	u4 nSrcItemNo;	// í•´ë‹¹ ì•„ì´í…œì— ì˜í•´ nItemNoê°€ ìƒì‚°ë¨
// 
// 	pop(szTamerName);
// 	pop(nItemType);	// maybe nItem::EventItem - ì´ë²¤íŠ¸ ì•„ì´í…œ(ë¬¸í™” ìƒí’ˆê¶Œ, mp3 ë“±ë“±)
// 	pop(nItemNo);	//
// 
// 	pop(nProtocol);
// 	pop(nSrcItemNo);
// 
// 	cPublicity::eITEM_HOW it;
// 	switch(nProtocol)
// 	{
// 	case pItem::Crop			:
// 		it = cPublicity::CROP;
// 		break;
// 	case pItem::CapsuleToItem	:
// 	case pItem::DigitamaToItem	:
// 		it = cPublicity::DIGITAMA_SCAN;
// 		break;
// 	default:
// 		assert_cs( false );
// 		return;
// 	}
// 
// 	( (cPublicity*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_PUBLICITY ) )->SetText_Item( it, szTamerName.c_str(), nSrcItemNo, nItemNo );	
// }
// void cCliGame::RecvMsgAllHatchOut(void)
// {
// 	std::wstring szTamerName;
// 	std::wstring szNewPartnerName;
// 	n4 nNewPartnerType;
// 	n4 nNewPartnerScale;
// 	n4 nEggLevel; // ë¶€í™” ë‹¨ê³„
// 
// 	pop(szTamerName);
// 	pop(szNewPartnerName);
// 	pop(nNewPartnerType);
// 	pop(nNewPartnerScale);
// 
// 	pop(nEggLevel);
// #ifdef VERSION_USA
// 	if(nEggLevel >=4)
// 		( (cPublicity*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_PUBLICITY ) )->SetText_Tactics( szTamerName.c_str(), szNewPartnerName.c_str(), nNewPartnerType, nNewPartnerScale*0.0001f, nEggLevel );
// #else
// 	( (cPublicity*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_PUBLICITY ) )->SetText_Tactics( szTamerName.c_str(), szNewPartnerName.c_str(), nNewPartnerType, nNewPartnerScale*0.0001f, nEggLevel );
// #endif
// }
void cCliGame::RecvDigimonCareSlotList(void)
{
	const char* kTrace = "ARCHIVE RECV 3204";
	nsCSDEBUG::CrashLogger::LogMessage("%s begin", kTrace);
	CDigimonArchiveContents::ArchiveInitInfo kRecvData;
	int nType = 0;
	pop(nType);
	kRecvData.SetType(nType);
	nsCSDEBUG::CrashLogger::LogMessage("%s header type=%d", kTrace, nType);
		
	if( nType == 0 )
	{
		int nOpenSlotSize = 0;
		pop(nOpenSlotSize);	// ì—´ë ¤ìžˆëŠ” ìš©ë³‘ ë³´ê´€ì†Œ ìŠ¬ë¡¯ ê°¯ìˆ˜
		kRecvData.SetOpenedArchiveSlotCount(nOpenSlotSize);
		u4 nIncuSlots[3] = {0,};
		u4 nSlot = 0;	// ì¸íë² ì´í„° ì•ˆì— ë“¤ì–´ìžˆëŠ” ë””ì§€ëª¬ì˜ ë””ë³´ ìŠ¬ë¡¯ ë²ˆí˜¸ ( 1000~ )
		for( int i = 0 ; i < 3 ; i++ )
		{
			pop( nSlot );
			nIncuSlots[i] = nSlot;
			kRecvData.AddIncuInfo(i, nSlot);
		}
		nsCSDEBUG::CrashLogger::LogMessage("%s storage header opened=%d incu=%u,%u,%u",
			kTrace,
			nOpenSlotSize,
			nIncuSlots[0],
			nIncuSlots[1],
			nIncuSlots[2]);
	}	
	u2 nScale = 0;
	int nSlotNo =0;
	int nInchantLevel = 0;
	u2 nExtentionParameter[6] = {0,};
	int nTHouseCnt = nsCsFileTable::g_pBaseMng->GetLimit()->s_nMaxTacticsHouse;
	kRecvData.SetMaxArchiveCnt(nTHouseCnt);
	nsCSDEBUG::CrashLogger::LogMessage("%s clientMax=%d", kTrace, nTHouseCnt);
	bool isEndPacket = false;
	int nLoopCount = 0;
	while(nSlotNo < nTHouseCnt)
	{
		pop(nSlotNo);		// ìš©ë³‘ì´ ë“¤ì–´ê°€ ìžˆëŠ” ìŠ¬ë¡¯ì˜ ë²ˆí˜¸
		if(nSlotNo == 1888){//ë§ˆì§€ë§‰ íŒ¨í‚· ìž…ë‹ˆë‹¤.
			g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::TACTICSHOUSE_LIST, 0 );
			isEndPacket = true;
			break;
		}
		else if (nSlotNo == 1999)//ì¤‘ê°„ íŒ¨í‚· ìž…ë‹ˆë‹¤.
		{
			break;
		}
		cData_PostLoad::sDATA* pTactics = new cData_PostLoad::sDATA();// pDataTH->GetTactics( nSlotNo );
		pTactics->s_SlotIdx = nSlotNo;
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d parse begin", kTrace, nSlotNo);
		u2 nLevel = 0;
		u2 nScale = 0;
		pop( pTactics->s_Type );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d typeAll=%I64u",
			kTrace,
			nSlotNo,
			(unsigned __int64)pTactics->s_Type.GetTypeAll());
		std::wstring name;
		pop( name );
		_tcscpy_s( pTactics->s_szName, name.c_str() );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d nameLen=%d", kTrace, nSlotNo, (int)name.size());
		pop( nScale );
		pTactics->s_fScale = nScale*0.0001f;
		pop( pTactics->s_nExp);
		pop( pTactics->s_nTranscendenceExp );
		pTactics->s_nExp = pTactics->s_nExp / 100;
		pop( pTactics->s_nLevel );
		pop( pTactics->s_Attribute, sizeof( pTactics->s_Attribute ) );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d stats scaleRaw=%u scale=%.4f exp=%I64u transExp=%I64u level=%d",
			kTrace,
			nSlotNo,
			nScale,
			pTactics->s_fScale,
			(unsigned __int64)pTactics->s_nExp,
			(unsigned __int64)pTactics->s_nTranscendenceExp,
			pTactics->s_nLevel);
		pop(pTactics->s_HatchLevel);
		pop( pTactics->s_dwBaseDigimonID );
		DBG("nBaseEvoUnitIDX : %d", pTactics->s_dwBaseDigimonID );
		pop( pTactics->s_nMaxEvoUnit);
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d hatch=%u base=%u maxEvo=%d evoUnitSize=%d",
			kTrace,
			nSlotNo,
			pTactics->s_HatchLevel,
			pTactics->s_dwBaseDigimonID,
			(int)pTactics->s_nMaxEvoUnit,
			(int)sizeof(cEvoUnit));
		if( pTactics->s_nMaxEvoUnit < 0 || pTactics->s_nMaxEvoUnit >= nLimit::EvoUnit )
		{
			nsCSDEBUG::CrashLogger::LogMessage( "%s invalid evo max=%d slot=%d type=%d; stopping parse",
				kTrace,
				(int)pTactics->s_nMaxEvoUnit,
				nSlotNo,
				pTactics->s_Type.m_nType );
			delete pTactics;
			isEndPacket = true;
			break;
		}
		pop( &pTactics->s_EvoUnit[ 1 ], sizeof(cEvoUnit)*pTactics->s_nMaxEvoUnit );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d evo parsed bytes=%d",
			kTrace,
			nSlotNo,
			(int)(sizeof(cEvoUnit)*pTactics->s_nMaxEvoUnit));
		pop( pTactics->s_nEnchantLevel );
		pop( pTactics->s_ExtendAttribute, sizeof( pTactics->s_ExtendAttribute ) );
		pop( pTactics->s_ExtendAttributeLV, sizeof( pTactics->s_ExtendAttributeLV ) );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d clone enchant=%d",
			kTrace,
			nSlotNo,
			(int)pTactics->s_nEnchantLevel);
		// ê¸°ë³¸ ì†ì„± ê²½í—˜ì¹˜
		for( int i=0; i < NewAttribute::MaxDigitalType; i++)
		{		
			n2 AttributeExp = 0;
			pop(AttributeExp);
			pTactics->s_AttributeExp[i] = AttributeExp;
		}
		// ë””ì§€ëª¬ ìžì—°ì†ì„± ê²½í—˜ì¹˜
		for( int i=0; i < NewAttribute::MaxNatualType; i++)
		{		
			n2 NatureExp = 0;
			pop(NatureExp);		
			pTactics->s_NatureExp[i] = NatureExp;
		}
		// ìš©ë³‘ ë””ì§€ëª¬ì˜ ìºì‰¬ ìŠ¤í‚¬ ì •ë³´ë¥¼ ë°›ìž
		u1 nDSkillCnt = 0;
		pop( pTactics->s_nUID );
		pop( nDSkillCnt );
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d uid=%u dSkillCnt=%u",
			kTrace,
			nSlotNo,
			pTactics->s_nUID,
			nDSkillCnt);
		if( nDSkillCnt > nLimit::EvoUnit )
		{
			nsCSDEBUG::CrashLogger::LogMessage( "ARCHIVE RECV 3204 invalid memory skill count=%u slot=%d limit=%d type=%d; aborting packet",
				(unsigned)nDSkillCnt,
				nSlotNo,
				nLimit::EvoUnit,
				pTactics->s_Type.m_nType );
			delete pTactics;
			isEndPacket = true;
			break;
		}
		for( int i=0; i< nDSkillCnt; ++i )
		{
			pop( pTactics->s_DCashSkill[i].s_nDigimonEvoStatus );
			pop( pTactics->s_DCashSkill[i].s_nDigimonCashSkillCode, sizeof(u4) * nLimit::MAX_ItemSkillDigimon );
			pop( pTactics->s_DCashSkill[i].s_nSkillCoolTime, sizeof(u4) * nLimit::MAX_ItemSkillDigimon );
		}
		kRecvData.AddData(nSlotNo, pTactics);
		nsCSDEBUG::CrashLogger::LogMessage("%s slot=%d parse end recvCount=%d",
			kTrace,
			nSlotNo,
			(int)kRecvData.GetRecvInfo()->size());
		++nLoopCount;
	}
	nsCSDEBUG::CrashLogger::LogMessage("%s before RECV_ARCHIEVE_DIGIMONS recvCount=%d isEnd=%d max=%d",
		kTrace,
		(int)kRecvData.GetRecvInfo()->size(),
		isEndPacket ? 1 : 0,
		nTHouseCnt);
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_ARCHIEVE_DIGIMONS, &kRecvData);
	nsCSDEBUG::CrashLogger::LogMessage("%s after RECV_ARCHIEVE_DIGIMONS", kTrace);
	if(isEndPacket == true)
	{
		nsCSDEBUG::CrashLogger::LogMessage("%s before RECV_END_ARCHIEVE_DIGIMONS", kTrace);
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_END_ARCHIEVE_DIGIMONS, &kRecvData);
		nsCSDEBUG::CrashLogger::LogMessage("%s after RECV_END_ARCHIEVE_DIGIMONS", kTrace);
	}
	nsCSDEBUG::CrashLogger::LogMessage("%s end", kTrace);
}
void cCliGame::RecvDigimonCareSlotSize(void) 
{
	int nOpenSlotSize;
	pop(nOpenSlotSize);	// ì—´ë ¤ìžˆëŠ” ìš©ë³‘ ë³´ê´€ì†Œ ìŠ¬ë¡¯ ê°¯ìˆ˜
	GS2C_RECV_EXTEND_ARCHIVE kRecvData;
	kRecvData.SetExtendArchiveCnt(nOpenSlotSize);
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_ON_EXTENT_ARCHIVE, &kRecvData);
	cWindow::PlaySound( cWindow::SD_Qu2 );
}
void cCliGame::RecvChangeTamerName(void)
{
	cData_QuickSlot::CheckItemCount_AllUser();
	cMessageBox::DelMsg( 14010, false );
	n4 nResCode;
	pop(nResCode);
	if(nResCode == 100)
	{
		cPrintMsg::PrintMsg(30500);
		g_IME.ReleaseText();
		bool bSuccess = false;
		g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CASH_CHANGE_TNAME, 0, &bSuccess );
		return;
	}
	if(nResCode == 1)
	{
		cPrintMsg::PrintMsg( 10022 );
		g_IME.ReleaseText();
		bool bSuccess = false;
		g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CASH_CHANGE_TNAME, 0, &bSuccess );
		return;
	}
	if(nResCode == 3)
	{
		GS2C_RECV_NAME_CHANGE_NEARBY recvChangeName;
		pop(recvChangeName.s_szPreName);
		pop(recvChangeName.s_szNextName);
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_CHANGE_NAME_NEARBY, &recvChangeName);
		return;
	}
	n4 nInvenSlotNo;
	pop(nInvenSlotNo);
	std::wstring szName;
	pop(szName);
	bool bSuccess = true;
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::CASH_CHANGE_TNAME, 0, &bSuccess );
	CTamerUser* pUser = g_pCharMng->GetTamerUser();	
	// ì¼€ë¦­í„° ì´ë¦„
	pUser->GetCharOption()->SetName( szName.c_str() );
#ifdef MASTERS_MATCHING // í…Œì´ë¨¸ ì´ë¦„ ë³€ê²½ ì‹œ ì•„ì´ì½˜ ì…‹íŒ…
	DWORD dwTeam = pUser->GetCharOption()->GetMatchTeam();
	pUser->GetCharOption()->SetMatchIcon( dwTeam );
#endif
	// íƒ€ê²Ÿì°½
	g_pGameIF->GetTamerWindow()->SetTargetObject( pUser );
	// ìŠ¤í…Œì´í„°ìŠ¤ì°½
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_TAMERSTATUS, NULL );
	pUser->SetAnimation( ANI::IDLE_CHAR );
	pUser->GetProp_Effect()->AddEffect( "system\\QuestEnd.nif", 1.0f, 0 );
	cWindow::PlaySound( cWindow::SD_Qu1 );
	// ì´ë¦„ ì§“ëŠ” ìœˆë„ìš° ë‹«ì•„ì¤€ë‹¤.
	cCreateName::DeleteInstance();
}
void cCliGame::RecvScannerError(void)
{
	u4 nErrCode;
	pop(nErrCode);
	switch(nErrCode)
	{
	case 1 :	// ìŠ¤ìºë„ˆ ì¸ì‹ë˜ì§€ ì•Šì€ ìƒíƒœì—ì„œ ì¹´ë“œ ë¦¬ë”©
		assert_cs( false );
		break;
	}
}
void cCliGame::RecvScannerEquip(void)
{
	u4 nUID = 0;
	pop(nUID);	// nUID í…Œì´ë¨¸ê°€ ìž¥ì°©í•¨
	CsC_AvObject* pObj = g_pMngCollector->GetObject( nUID );
	if( pObj == NULL )
		return;
	if( pObj->GetLeafRTTI() != RTTI_TAMER )
	{
		assert_csm( false, _T( "í…Œì´ë¨¸ë§Œ ìŠ¤ìºë„ˆ ìž¥ì°© ê°€ëŠ¥" ) );
		return;
	}
	( (CTamer*)pObj )->Scene_Barcode_Connect();
}
void cCliGame::RecvScannerOn(void)
{
	int nTamerCondition;
	int nIsRegistered;
	pop(nTamerCondition);
	pop(nIsRegistered);		// ì´ë¯¸ ë“±ë¡ë˜ì–´ ìžˆëŠ”ì§€
	g_pCharMng->GetTamerUser()->SetCondition( nTamerCondition );
	UpdateScannerState(nTamerCondition);
}
void cCliGame::RecvScannerOff(void)
{
	int nTamerCondition = 0;
	u4 nUID = 0;	// nUID í…Œì´ë¨¸ ID
	pop(nTamerCondition);
	pop(nUID);	
	CsC_AvObject* pObj = g_pMngCollector->GetObject( nUID );
	if( pObj == NULL )
		return;
	if( pObj->GetLeafRTTI() == RTTI_TAMER_USER )	// ìžê¸° ìžì‹  ì¼ ë•Œ,
		g_pCharMng->GetTamerUser()->SetCondition( nTamerCondition );
	else if( pObj->GetLeafRTTI() == RTTI_TAMER )			// ë‹¤ë¥¸ ìœ ì € ì¼ ë•Œ,
		( (CTamer*)pObj )->SetCondition(nTamerCondition);
}
#define d_nDummyCardType 30001
void cCliGame::RecvCardScan(void)
{
	u4 nTamerIDX = 0;
	int nCardType = 0;
	pop(nTamerIDX);
	pop(nCardType);
	CTamer* pTamer = g_pCharMng->GetTamer( nTamerIDX );
	if( pTamer == NULL )
		return;
	if(nCardType == 0)
	{
		pTamer->Scene_Barcode_CardScratch_OverCount();
		if( pTamer->GetLeafRTTI() == RTTI_TAMER_USER )
			cPrintMsg::PrintMsg( 20045 );
		return;
	}
	// ë”ë¯¸ ì¹´ë“œ ì‚¬ìš© íšŸìˆ˜ ì´ˆê³¼
	if(nCardType == -1)
	{
		pTamer->Scene_Barcode_CardScratch_OverCount();
		if( pTamer->GetLeafRTTI() == RTTI_TAMER_USER )
			cPrintMsg::PrintMsg( 20046 );
		return;
	}
	int nResCode;
	pop(nResCode);	// 0ì´ë©´ ì•„ì´í…œ ìƒì„± ì‹¤íŒ¨, 1ì´ë©´ ì•„ì´í…œ ìƒì„± ì„±ê³µ
	if(nCardType == d_nDummyCardType)
	{
		// ë”ë¯¸ ì¹´ë“œ ì‚¬ìš© ì„±ê³µ
		int nDummyCardCount;
		pop(nDummyCardCount);	// ë‚¨ì•„ìžˆëŠ” íšŸìˆ˜
		if( pTamer->GetLeafRTTI() == RTTI_TAMER_USER )
			cPrintMsg::PrintMsg( 20047, &nDummyCardCount );
	}
	if( nResCode == 0 )
	{
		pTamer->Scene_Barcode_CardScratch_Faile();
	}
	else
	{
		pTamer->Scene_Barcode_CardScratch_Success();
		//if(nCardType == d_nDummyCardType)
		//{
			// ë”ë¯¸ ì¹´ë“œ ì‚¬ìš© ì„±ê³µ
		//}
		//else
		//{
		//	// ì¼ë°˜ ì¹´ë“œ ì‚¬ìš© ì„±ê³µ
		//}
	}	
}
void cCliGame::UpdateScannerState(n4 nScannerState)
{
#ifdef USE_BARCODE_REDER
	if(nScannerState & nSync::Scanner0)
	{
		// ìŠ¤ìºë„ˆ ìž¥ì°©ì¤‘
	}
	else
	{
		// ìŠ¤ìºë„ˆ ìž¥ì°©í•˜ê³  ìžˆì§€ ì•„ë‹ˆí•¨
		return;
	}
	if(nScannerState & nSync::Scanner1)
	{
		// ìžì‹  ì†Œìœ ì˜ ìŠ¤ìºë„ˆ
	}
	else
	{
		// ë¯¸ë“±ë¡ ë˜ëŠ” íƒ€ì¸ì˜ ìŠ¤ìºë„ˆ
	}
	if(nScannerState & nSync::Scanner2)
	{
		// í•œì •íŒ ìŠ¤ìºë„ˆ
	}
	else
	{
		// ì¼ë°˜íŒ ìŠ¤ìºë„ˆ
	}
	if( nsBARCODE::g_bFirstEquipBarcode == true )
	{
		g_pCharMng->GetTamerUser()->Scene_Barcode_Connect();
		nsBARCODE::g_bFirstEquipBarcode = false;
		bool bUseUpdate = true;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::USE_GAMESCANNER, &bUseUpdate );
	}
#endif
}
void cCliGame::RecvHandshake487(void) {
	short HS;
	pop(HS);
	pop(HS);
	pop(HS);
	pop(HS);
	pop(HS);
	pop(HS);
}
void cCliGame::RecvScannerRegister(void)
{
#ifdef USE_BARCODE_REDER
	u4 nResCode1;
	n4 nTamerCondition;
	pop(nResCode1);
	pop(nTamerCondition);
	if(nResCode1 == 0)
	{
		// ë“±ë¡ ì„±ê³µ í•˜ì˜€ìŠµë‹ˆë‹¤.
		g_pCharMng->GetTamerUser()->SetCondition( nTamerCondition );
		UpdateScannerState(nTamerCondition);
		cPrintMsg::PrintMsg( 20034 );
		bool bUseUpdate = true;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::USE_GAMESCANNER, &bUseUpdate );
	}
	else if(nResCode1 == -1)
	{
		// ë‹¤ë¥¸ ì‚¬ìš©ìžê°€ ì‚¬ìš©ì¤‘ìž…ë‹ˆë‹¤.
		cPrintMsg::PrintMsg( 20037 );
	}
	else
	{
		// ë“±ë¡ ì‹¤íŒ¨ - ì•„ì§ í•´ì œ ì‹œê°„ì´ ë‚¨ì•„ìžˆìŠµë‹ˆë‹¤.
		// nResCode ë“±ë¡ ê°€ëŠ¥ì‹œê¹Œì§€ ë‚¨ì€ ì‹œê°„
		cPrintMsg::PrintMsg( 20043, &nResCode1 );
	}
#endif
}
void cCliGame::RecvScannerRelease(void)
{
	n4 nResCode;
	u4 nTamerCondition;
	pop(nResCode);
	pop(nTamerCondition);
	if(nResCode)
	{
		// í•´ì œ ì„±ê³µ
		cPrintMsg::PrintMsg( 20041 );
		g_pCharMng->GetTamerUser()->SetCondition( nTamerCondition );
		bool bUseUpdate = true;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::USE_GAMESCANNER, &bUseUpdate );
	}
	else
	{
		assert_cs( false );
	}
}
// void cCliGame::RecvAdvertBurstMode(void)
// {
// 	std::wstring szTamerName;
// 	std::wstring szDigimonName;
// 	int nBurstModeDigimonType;
// 	pop(szTamerName);
// 	pop(szDigimonName);
// 	pop(nBurstModeDigimonType);
// 
// 	TCHAR sz[ IF_PUBLICITY_LEN ] = {0, };
// 
// 	switch( nBurstModeDigimonType )
// 	{
// 	case 71037:
// 	case 71019:
// 		{
// 			_stprintf_s( sz, IF_PUBLICITY_LEN, _LAN( "< %s > í…Œì´ë¨¸ ë‹˜ê»˜ì„œ < %s > ë””ì§€ëª¬ ì„ < %s >ë¡œ ì§„í™”ì‹œì¼°ìŠµë‹ˆë‹¤." ), szTamerName.c_str(), szDigimonName.c_str(),
// 				nsCsFileTable::g_pDigimonMng->GetDigimon( nBurstModeDigimonType )->GetInfo()->s_szName );
// 			( (cPublicity*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_PUBLICITY ) )->SetText( sz );
// 		}
// 		break;	
// 	default:
// 		{
// 			_stprintf_s( sz, IF_PUBLICITY_LEN, _LAN( "< %s > í…Œì´ë¨¸ ë‹˜ê»˜ì„œ < %s > ë””ì§€ëª¬ì„ ê°ì„±ëª¨ë“œë¡œ ì§„í™”ì‹œì¼°ìŠµë‹ˆë‹¤." ), szTamerName.c_str(), szDigimonName.c_str() );
// 			( (cPublicity*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_PUBLICITY ) )->SetText( sz );
// 		}
// 		break;
// 	}
// }
void cCliGame::RecvDailyEventInfo(void)
{
	GS2C_RECV_DAILY_EVENT_DATA recv;
	pop(recv.m_nEventNo);
	pop(recv.m_nRemainedTimeTS);
	pop(recv.m_nTotalTime);
	pop(recv.m_nWeek);	
	if(recv.m_nWeek > 6)
		recv.m_nWeek = 0xFF;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_DAILY_EVENT_DATA, &recv );
}
//2017-02-16-nova	
void cCliGame::RecvMonthlyEventInfo(void)
{
	GS2C_RECV_MONTHLY_EVENT_DATA recv;
	pop(recv.m_nEventNo);	//ì´ë²¤íŠ¸ ë²ˆí˜¸
	pop(recv.m_nAttendCnt);	//ì¶œì„ì¼ìˆ˜
	pop(recv.m_bNotify);	//ìµœì´ˆì•Œë¦¼
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MONTHLY_EVENT_DATA, &recv );
}
void cCliGame::RecvAdvertBossDie(void)
{
	n4 nBossMonsterType;
	n4 nRemainedCount;
	pop(nBossMonsterType);
	pop(nRemainedCount);
	if( nRemainedCount == 0 )
	{		
		g_pGameIF->GetBossScene()->SetBossDie( nBossMonsterType );
	}
}
void cCliGame::RecvAdvertBossCount(void)
{
	u4 nRemainedCount;		// ì•žìœ¼ë¡œ ìž¡ì•„ì•¼í•  Seed Monsterì˜ ê°œì²´ìˆ˜
	u4 nBossMonsterType;	// seed monsterë¥¼ ëª¨ë‘ ìž¡ì€ ê²½ìš°, íƒœì–´ë‚˜ëŠ” ë³´ìŠ¤ ëª¬ìŠ¤í„°
	u4 nSeedMonsterType;	// ìž¡ì•„ì•¼í•  seed monster
	pop(nRemainedCount);
	pop(nBossMonsterType);
	pop(nSeedMonsterType);
	g_pGameIF->GetBossScene()->SetWarning( nBossMonsterType, nSeedMonsterType, nRemainedCount );
}
void cCliGame::RecvAdvertBossCreate(void)
{
	u4 nMapIDX;
	u4 nChannelIDX;
	u4 nBossMonsterType;	// ìƒì„±ëœ ë³´ìŠ¤ ëª¬ìŠ¤í„°ì˜ íƒ€ìž…
	u4 nCreatedCount;		// ìƒì„±ëœ ë³´ìŠ¤ ëª¬ìŠ¤í„°ì˜ ê°œì²´ìˆ˜
	pop(nMapIDX);
	pop(nChannelIDX);
	pop(nBossMonsterType);
	pop(nCreatedCount);
	CsMonster::sINFO* pMon = nsCsFileTable::g_pMonsterMng->GetMonster( nBossMonsterType )->GetInfo();
	assert_cs( pMon != NULL );	
	std::wstring wMapDiscript = nsCsMapTable::g_pMapListMng->GetList( nMapIDX )->GetInfo()->s_szMapDiscript;
#ifdef VERSION_USA
	cPrintMsg::PrintMsg( 30171, const_cast<TCHAR*>(wMapDiscript.c_str()),pMon->s_szName,&nChannelIDX);
#else
	cPrintMsg::PrintMsg( 30171, const_cast<TCHAR*>(wMapDiscript.c_str()),&nChannelIDX, pMon->s_szName );	
#endif
	ChannelContents::CHANNELINFO ChannelData;
	GAME_EVENT_ST.OnEvent(EVENT_CODE::GET_CHANNELINFO, &ChannelData );
	if( ChannelData.channel_idx != nChannelIDX )
		return;
	if( g_pTerrainMng->GetBackupMapID() != nMapIDX )
		return;
	g_pGameIF->GetBossScene()->SetEmergency( nBossMonsterType, nCreatedCount );
}
void cCliGame::RecvHatchOptionItemAddResult()
{
	u1 nType;
	pop(nType);
	if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAKE_TACTICS ) )
	{
		g_pGameIF->GetMakeTactics()->RecvResult( cMakeTactics::DEL_BACKUP );
		g_pGameIF->GetMakeTactics()->RecvServer();
		int nLevel = g_pDataMng->GetTactics()->GetTacticsCount();
		GS2C_RECV_MAKE_DIGITAMA_LV recv;
		recv.m_nEggLevel = nLevel;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::MAKETACTICS_DIGITAMA_LV, &recv );
		cPrintMsg::PrintMsg( 30503 );
#ifdef MINIGAME
		GS2C_RECV_MAKE_DIGITAMA_MINIGAME_INIT kRecv;
		kRecv.nResult = false;
		kRecv.nSuccessCnt = 0;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_MINIGAME_INIT, &kRecv );
#endif
	}
}
void cCliGame::RecvPortableItemFail(void)
{
	// ì˜¤ì§ ì‚¬ìš©ê¸°ê°„ ë§Œë£Œ ì¼ë•Œë§Œ ë°›ê² ë‹¤.
	n4 nSubType = 0;
	pop( nSubType );
	switch( nSubType )
	{
	case nItem::Portable_Hatch:// íœ´ëŒ€ìš© ë¶€í™”ê¸°
		{
			if( g_pGameIF && g_pGameIF->IsActiveWindow(cBaseWindow::WT_MAKE_TACTICS) )
			{
				cBaseWindow* pWin = g_pGameIF->GetDynamicIF(cBaseWindow::WT_MAKE_TACTICS);
				if( pWin )
				{
					cMakeTactics* pMakeTactics = static_cast<cMakeTactics*>(pWin);
					pMakeTactics->RecvServer();
				}
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_MAKE_TACTICS, 0, true );		
			}
		}break;
	case nItem::Portable_Scanner :	// íœ´ëŒ€ìš© ìŠ¤ìº”ë„ˆ ì´ìš© ì•„ì´í…œ
		{
			UINT nScanWT = cBaseWindow::WT_DIGITAMA_SCAN;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_CLOSE, &nScanWT );
		}break;
	case nItem::Portable_WareHouse:// íœ´ëŒ€ìš© ì°½ê³  ì´ìš© ì•„ì´í…œ
		{
			if( g_pGameIF )
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_WAREHOUSE, 0, true );
		}break;
	case nItem::Portable_DigimonCareHouse: // íœ´ëŒ€ìš© ë””ì§€ëª¬ ë³´ê´€ì†Œ ì´ìš© ì•„ì´í…œ
		{
			if( g_pGameIF )
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_TACTICSHOUSE, 0, true );
		}break;
	case nItem::Portable_DigitamaCollect:	// íœ´ëŒ€ìš© ì•„ì´í…œ íšŒìˆ˜ ì´ìš© ì•„ì´í…œ
		{
			UINT nSellWT = cBaseWindow::WT_DIGITAMA_SELL;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::DIGITAMA_CLOSE, &nSellWT );
		}break;
	case nItem::Portable_Analysis:// íœ´ëŒ€ìš© ì†ì„± ë¶„ì„ê¸°
	case nItem::Portable_Socket:// íœ´ëŒ€ìš© ìž¥ë¹„ í•©ì„±ê¸°
#ifdef LJW_EQUIPSYSTEM_190724
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CLOSE_EQUIPSYSTEM, NULL );
#else
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CLOSE_ELEMENT_ITEM, NULL );
#endif
		break;
	case nItem::Portable_JumpGate:// ì í”„ë¶€ìŠ¤í„° ì•„ì´í…œ ê¸°ê°„ ë§Œë£Œ
		{
			if( g_pGameIF )
				g_pGameIF->CloseDynamicIF( cBaseWindow::WT_JUMPBUSTER, 0, true );
		}break;
	default:
		assert_cs( false );
		break;
	}
	// ì‚¬ìš©ê¸°ê°„ ì¢…ë£Œ
	cPrintMsg::PrintMsg( 30362 );
}
void cCliGame::RecvDigimonMoveResult()
{
	//int Src = 0;//int Dst = 0;//u4 Price = 0;
	GS2C_RECV_ARCHIVE_CHANGE RecvData;
	pop( RecvData.Src );
	pop( RecvData.Dst );
	pop( RecvData.Price );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_CHANGE_DIGIMONS_ARCHIVE, &RecvData );
}
void cCliGame::RecvDigimonMoveResultInArchive(void)
{
	GS2C_RECV_Archive_Internal_Move RecvData;
	pop(RecvData.nSlot1);
	pop(RecvData.nSlot2);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MOVE_DIGIMONS_IN_ARCHIVE, &RecvData );
}
void cCliGame::RecvPlayTimePaneltyChange()
{
#ifdef PLAY_PANELTY
	u2 nPaneltyType = 0;		// íŒ¨ë„í‹° íƒ€ìž…
	u4 CashTime = 0;			// ìœ ë£Œ í”Œë ˆì´ ì‹œê°„
	u4 NormalTime = 0;			// ì¼ë°˜ í”Œë ˆì´ ì‹œê°„
	u4 oldCashTime = g_pDataMng->PlayTime_Get_CashTime();
	pop(nPaneltyType);
	pop(CashTime);	
	pop(NormalTime); // ê¸ˆì¼ í”Œë ˆì´ ê°€ëŠ¥í•œ ë‚¨ì€ ì‹œê°„(ë‹¨ìœ„-ì´ˆ)
	g_pDataMng->PlayTime_Set_PaneltyType( nPaneltyType );
	g_pDataMng->PlayTime_Set_CashTime( CashTime );
	g_pDataMng->PlayTime_Set_NormalTime( NormalTime );
	switch( nPaneltyType )
	{
	case nPlayTime::LEVEL1_PANELTY:
		{
			int nRTime = g_pDataMng->PlayTime_Get_BaseFullTime() - g_pDataMng->PlayTime_Get_NormalTime();
			int nH = nRTime / 3600;
			nRTime = nRTime % 3600;
			int nM = nRTime / 60;			
			TCHAR sz[ 64 ];
			_stprintf_s( sz, 64, L"%.2d : %.2d", nH, nM );
			cPrintMsg::PrintMsg( 30415, sz );
		}
		break;
	case nPlayTime::LEVEL2_PANELTY:
		{
			g_pDataMng->PlayTime_GameResetTime_PrintMsg();			
		}
		break;
	case nPlayTime::USE_CASH:
		{
			int nAddCashTime = g_pDataMng->PlayTime_Get_CashTime() - oldCashTime;
			int nH = nAddCashTime / 3600;
			nAddCashTime = nAddCashTime % 3600;
			int nM = nAddCashTime / 60;
			TCHAR sz[ 64 ];
			_stprintf_s( sz, 64, L"%.2d : %.2d", nH, nM );
			cPrintMsg::PrintMsg( 30418, sz );
		}
		break;
	}
	g_pGameIF->GetPlayTime()->CalServerDay();
#endif
}
void cCliGame::RecvCropGiftInvenFail()
{
	u1 nResult = 0;
	pop( nResult );
	switch( nResult )
	{
	case nItem::GiftF_ITEMTYPE					:	// ì•„ì´í…œ íƒ€ìž… ì—ëŸ¬
	case nItem::GiftF_INVEN_NOT_EMPTY			:	// ì¸ë²¤ ê³µê°„ ë¶€ì¡±
	case nItem::GiftF_CROP_FAIL					:	// ì•„ì´í…œ íšë“ ì²˜ë¦¬ ì‹¤íŒ¨ ( ë‚´ë¶€ ì‹¤íŒ¨ )
	case nItem::GiftF_COUNT_OVER				:	// ì•„ì´í…œì˜ ì¤‘ì²©ê°œìˆ˜ ì—ëŸ¬ë¡œ íšë“í•  ìˆ˜ì—†ëŠ” ê²½ìš°
		break;
	default : break;
	}
	if(cClient::IsSendBlock())
		cClient::SetSendBlock(false);
}
void cCliGame::RecvGiftBoxUseSuccess()
{
	u4 nTargetUID = 0;
	u2 nInvenPos = 0;
	u4 nUsedItemType = 0;
	pop( nUsedItemType );
	pop(nTargetUID);
	pop(nInvenPos);
	g_pDataMng->ServerItemUseSuccess( nTargetUID, nInvenPos );
	u4 nCount = 0;
	pop(nCount);
	for( u4 i=0; i<nCount; ++i)
	{
		cItemData data;
		u4 nType = 0;
		pop(nType);
		data.m_nType  = nType;
		u4 nItemCount = 0;
		pop(nItemCount);
		data.m_nCount = nItemCount;
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
		u4 nRemainTradeLimitTime = 0;
		pop(nRemainTradeLimitTime);
		data.m_nRemainTradeLimitTime = nRemainTradeLimitTime;
#endif
		// rate ê°’ì´ ì¡´ìž¬í•˜ëŠ” ì•„ì´í…œì„ ë°”ë¡œ ìœ„íƒìƒì ì— ì˜¬ë¦´ ê²½ìš°
		// ì„œë²„ì™€ ë™ê¸°í™”ê°€ ë§žì§€ ì•Šì•„ì„œ ëŠê¸°ëŠ” ë¬¸ì œ ìžˆìŒ
		// ì„œë²„ì—ì„œ ì•„ì´í…œì„ ë°›ì•„ì˜¤ë„ë¡ ìˆ˜ì •í•´ì•¼ í•¨
		data.m_nRate  = 0;	// ì•„ì´í…œ ëŠ¥ë ¥ì¹˜ ì ìš© ë¹„ìœ¨ - 0 == í´ë¼ì—ì„œ ìž„ì˜ë¡œ ì„¸íŒ…í•œ ê°’
		data.m_nLevel = 1;
		nsInventory::sGiftBox giftBox;
		giftBox.nUsedItemType = nUsedItemType;
		giftBox.itemData = data;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::SET_GIFTBOX_WINDOW, &giftBox );
	}
}
void cCliGame::RecvCardEventStart()	// ì¹´ë“œ ì´ë²¤íŠ¸ ì‹œìž‘ ì‹œ ì„œë²„ì—ì„œ ë°ì´í„°ë¥¼ ë°›ìŒ
{
	u8 nMoney = 0;
	u4 nItemID = 0;
	int nItemCount = 0;
	pop(nMoney);
	pop(nItemID);
	pop(nItemCount);
	if( g_pDataMng )
	{
		cData_Inven* pInven = g_pDataMng->GetInven();
		if( pInven )
			pInven->SetMoney( nMoney, false );	// ëˆ ì„¤ì •
	}
	std::pair<DWORD,int> result;
	result.first = nItemID;
	result.second = nItemCount;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_CARD_EVENT_RESULT, &result );
}
void cCliGame::RecvCardEventEnd()
{
	int nResult = 0;
	pop(nResult);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_CARD_EVENT_END, &nResult );
}
void cCliGame::RecvNatureExp()
{
	//namespace NewAttribute
	u1 uMainType;	// ë©”ì¸ íƒ€ìž…, 0~1,	ì‹¤íŒ¨ì‹œ 255
	u1 uSubType;	// ì„œë¸Œ íƒ€ìž…, ë²„íŠ¼ í´ë¦­í›„ ì‹¤íŒ¨ì‹œ 255 
	n2 nExp;	// 0
	/*
	Da,	// 2 Data
	Va,	// 3 Vaccine
	Vi,	// 4 Virus
	*/
	cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
	cData_PostLoad::sDATA* pDigimonData = pPostLoad->GetDigimonData();
	pop(uMainType);	
	pop(uSubType);
	pop(nExp);
	// ê¸°ë³¸ì†ì„±
	if(NewAttribute::Digital == uMainType)
	{
		pDigimonData->s_AttributeExp[uSubType] += nExp;
		g_pCharMng->GetDigimonUser( 0 )->SetAttributeExp( pDigimonData->s_AttributeExp );
		
		if(-10000 == nExp)
		{
			CsElementItem::sINFO* pItem = nsCsFileTable::g_pItemMng->GetElementItem1(uSubType)->GetInfo();
			CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
			if( pTamerUser == NULL )
				return;
			
			cItemData item;
			item.m_nType = pItem->s_dwItemID;
			item.m_nCount = 1;
			item.m_nRate = 0;
			item.m_nLevel = 0;
			item.m_nLimited = 0;	
			pTamerUser->RaidItemCropSuccess( &item, 30519 );
		}
		else if(10000 == pDigimonData->s_AttributeExp[uSubType])
		{
			CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
			if( pTamerUser == NULL )
				return;			
			pTamerUser->PlaySound(SOUND_ELEMENTUP);
			cPrintMsg::PrintMsg( 30521 + uSubType );
		}
		
	}
	// ìžì—°ì†ì„±ì´ë©´
	else if(NewAttribute::Natual == uMainType)
	{
		pDigimonData->s_NatureExp[uSubType] += nExp;
		g_pCharMng->GetDigimonUser( 0 )->SetNatureExp( pDigimonData->s_NatureExp );
		if(-10000 == nExp)
		{
			CsElementItem::sINFO* pItem = nsCsFileTable::g_pItemMng->GetElementItem2(uSubType)->GetInfo();
			CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
			if( pTamerUser == NULL )
				return;			
			cItemData item;
			item.m_nType = pItem->s_dwItemID;
			item.m_nCount = 1;
			item.m_nRate = 0;
			item.m_nLevel = 0;
			item.m_nLimited = 0;	
			pTamerUser->RaidItemCropSuccess( &item, 30519 );
		}
		else if(10000 == pDigimonData->s_NatureExp[uSubType])
		{
			CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
			if( pTamerUser == NULL )
				return;			
			pTamerUser->PlaySound(SOUND_ELEMENTUP);
			cPrintMsg::PrintMsg( 30524 + uSubType );
		}
	}
	else if(255 == uMainType)
	{
		if( g_pDataMng->GetInven()->GetEmptySlotCount() == 0 )
		{
			cPrintMsg::PrintMsg( 11015 );
			
		}
	}
}
// ì˜¤ë¥¸ìª½ ë²„íŠ¼ìœ¼ë¡œ ì¼ë¶€ ì•Œ(ì´ë²¤íŠ¸) ë¶€í™”ë˜ëŠ” ê¸°ëŠ¥
void cCliGame::RecvRClickDigimonHatch(void)
{
	u1 nSuccess;
	pop(nSuccess);	// ë¶€í™” ì„±ê³µ ì‹¤íŒ¨ ì—¬ë¶€ (1 : ì„±ê³µ, 0: ì‹¤íŒ¨)
	
	if( nSuccess != 0 )
	{
		int nInvenPos;
		pop(nInvenPos);	// ì‚¬ìš© Item Inven Position
		u4 nArrIDX;
		pop(nArrIDX);
		cData_PostLoad::sDATA Tactics;	
		pop( Tactics.s_Type );
		pop( Tactics.s_szName );
		u2 nScale;
		pop( nScale );
		Tactics.s_fScale = nScale*0.0001f;
		pop( Tactics.s_nExp);
		Tactics.s_nExp = Tactics.s_nExp / 100;// ì„œë²„ì—ì„œ ê²½í—˜ì¹˜ê°€ ë°±ë¶„ë¥ ë¡œ ì˜¤ê¸° ë•Œë¬¸ìž„
#ifdef SDM_DIGIMON_TRANSCENDENCE_CONTENTS_20190507
		pop( Tactics.s_nTranscendenceExp );
#endif
		pop( Tactics.s_nLevel );
		pop( Tactics.s_Attribute, sizeof( Tactics.s_Attribute ) );
		
		pop( Tactics.s_HatchLevel );
		
		pop( Tactics.s_dwBaseDigimonID );
		DBG("nBaseEvoUnitIDX : %d", Tactics.s_dwBaseDigimonID );
		pop( Tactics.s_nMaxEvoUnit);
		pop( &Tactics.s_EvoUnit[ 1 ], sizeof(cEvoUnit)*Tactics.s_nMaxEvoUnit );
		memset(&Tactics.s_AttributeExp, NULL, sizeof(n2)*NewAttribute::MaxDigitalType );
		memset(&Tactics.s_NatureExp, NULL, sizeof(n2)*NewAttribute::MaxNatualType );
		u2 nInchantLevel = 0;
		u2 nExtentionParameter[6] = {0,};
		pop( Tactics.s_nEnchantLevel );
		pop( Tactics.s_ExtendAttribute, sizeof( Tactics.s_ExtendAttribute ) );
		pop( Tactics.s_ExtendAttributeLV, sizeof( Tactics.s_ExtendAttributeLV ) );
		g_pDataMng->GetTactics()->AddTactics( &Tactics, nArrIDX - 1 );
		//=============================================================================
		//	ì¸ë²¤ ë‚´ ì•„ì´í…œ ê°ì†Œ ë° ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
		//=============================================================================
		g_pTacticsAni->Set_bAniProcess(true);	// ë¶€í™” ì• ë‹ˆ ì‹œìž‘ìœ„ì¹˜ ì„¤ì •
		g_pTacticsAni->Start( g_pDataMng->GetInven()->GetData( nInvenPos )->GetType(), g_pCharMng->GetTamerUser() );	// ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
		g_pDataMng->GetInven()->GetData( nInvenPos )->DecreaseCount( 1 );	// ì‚¬ìš©í•œ Item ì¸ë²¤ ë‚´ ê°ì†Œ		
		//==========================================================================================================
		// ì—…ì  ì²´í¬
		//==========================================================================================================
		GS2C_RECV_CHECKTYPE recvAchieve;
		recvAchieve.nType = AchieveContents::CA_GetTactics;
		recvAchieve.nValue1 = Tactics.s_Type.m_nType;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::ACHIEVE_SET_CHECKTYPE, &recvAchieve );
		GS2C_RECV_ENCYCLOPEDIA_EVOLUTION recv;
		recv.nDigimonID = Tactics.s_dwBaseDigimonID;
		recv.nScale = nScale;
		recv.nType = 0;
		for( int i = 1 ; i < nLimit::EvoUnit ; i++ )
		{
			recv.nEvoUnit[i] = Tactics.s_EvoUnit[i];
		}
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::ENCYCLOPEDIA_EVOL_UPDATE, &recv );		
		GS2C_RECV_EXTEND_ACCOMPANY_SLOT kRecvdata;
		kRecvdata.OpenedCnt = 0;
		GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_ON_EXTENT_ACCOMPANY_SLOT, &kRecvdata);
	}
	else
	{
		cPrintMsg::PrintMsg( 30535 );	// ë””ì§€ë°”ì´ìŠ¤ì— ë””ì§€ëª¬ì´ ê½‰ ì°¨ì„œ ë¶€í™œí•  ìˆ˜ ì—†ìŠµë‹ˆë‹¤. (ë©”ì„¸ì§€ ë°•ìŠ¤)
	}
	
}
#ifdef MINIGAME
void cCliGame::RecvMinigameStart()
{
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_START kRecv;
	pop(kRecv.nStage);
	pop(kRecv.nBarTime);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_DIGITAMA_START_MINIGAME, &kRecv);
}
void cCliGame::RecvMinigameClickBtn()
{
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_CLICKRESULT kRecv;
	pop(kRecv.nResult);
	pop(kRecv.nBarIndex);
	pop(kRecv.nBarTime);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_DIGITAMA_CLICK_RESULT, &kRecv);
}
void cCliGame::RecvMinigameTimeOut()
{
	//ì‹œê°„ ì§€ë‚œê±°ë‹ˆê¹Œ ì‹¤íŒ¨ë¡œ ì²˜ë¦¬í•´ì¤˜ì•¼í•¨
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_TIMEOUT kRecv;
	pop(kRecv.nBarTime);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_DIGITAMA_TIME_OUT, &kRecv);
}
void cCliGame::RecvMinigameLimit()
{
	//ë¯¸ë‹ˆê²Œìž„ ì‹¤í–‰ í•˜ë©´ ì•ˆë˜ëŠ” ìƒí™©ì—ì„œ ì‹¤í–‰í–ˆì„ ë•Œ ì˜ˆì™¸ ì²˜ë¦¬
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_ERRORMESSAGE kRecv;
	pop(kRecv.nErrorCode);
	pop(kRecv.nSuccessCnt);	//ì´ì „ ì‹œë„ì— ì„±ê³µí•œ íšŸìˆ˜
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_DIGITAMA_ERROR_MESSAGE, &kRecv);
}
void cCliGame::RecvMinigameEnd()
{
	GS2C_RECV_MAKE_DIGITAMA_MINIGAME_END kRecv;
	pop(kRecv.nSuccess);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MAKETACTICS_DIGITAMA_END_MINIGAME, &kRecv);
}
#endif
void cCliGame::RecvGotchaOpen()
{
#if 0 // our common lib
	GS2C_RECV_OPEN_GOTCHAMACHINE recv;
	pop( recv.nRareItemCount );
	for( int i = 0; i < recv.nRareItemCount; ++i )
	{
		u4 nItemID = 0;
		pop( nItemID );
		u4 nItemCount = 0;
		pop( nItemCount );
		cItemData itemData;
		itemData.Set( nItemID, nItemCount );
		recv.lRareItems.push_back( itemData );
	}
	pop( recv.nNormalItemCount );
	pop( recv.nResetCount );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_OPEN_GOTCHAMACHINE, &recv );
#endif
}
void cCliGame::RecvGotchaGet()
{
#if 0 // our common lib
	GS2C_RECV_GET_ITEM_GOTCHAMACHINE recv;
	pop( recv.nKind );
	pop( recv.nItemID );
	pop( recv.nItemCount );
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
	pop( recv.nRemainTradeLimitTime );
#endif
	// refresh list
	pop( recv.nRareItemCount );
	for( int i = 0; i < recv.nRareItemCount; ++i )
	{
		u4 nItemID = 0;
		pop( nItemID );
		u4 nItemCount = 0;
		pop( nItemCount );
		cItemData itemData;
		itemData.Set( nItemID, nItemCount );
		recv.lRareItems.push_back( itemData );
	}
	pop( recv.nNormalItemCount );
	pop( recv.nResetCount );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_GET_ITEM_GOTCHAMACHINE, &recv );
#endif
}
void cCliGame::RecvGotchaError()
{
	u4 nErrCode;
	pop(nErrCode);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ERROR_GOTCHAMACHINE, &nErrCode );
}
void cCliGame::RecvGotchaRefresh()
{
}
void cCliGame::RecvMysteryGet()
{
#if 0 // our common lib
	GS2C_RECV_GET_ITEM_MYSTERYMACHINE recv;
	pop( recv.nItemID );
	pop( recv.nItemCount );
	pop( recv.nItemGroupID );
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
	pop( recv.nRemainTradeLimitTime );
#endif
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_GET_ITEM_MYSTERYMACHINE, &recv );
#endif
}
#ifdef ADVENTURE_TAMER_CHANGE
void cCliGame::RecvMoveClear()
{
	u2 nEquipCount;
	n2 nDestID, nSrcID;
	pop( nEquipCount );
	for( u2 i = 0; i < nEquipCount; ++i )
	{
		pop( nDestID );
		pop( nSrcID );
		g_pDataMng->GetTEquip()->ToInven( TO_ID( nDestID ), TO_ID( nSrcID ) );
	}
}
#endif
//#ifdef FIREWORK
void cCliGame::RecvFireWork()
{
	u4	uIDX = 0;
	u4	uItemID = 0;
	pop( uIDX );
	pop( uItemID );
	
	uint nUIDX = GetIDX( uIDX );
	CTamer* pTamer = NULL;
		pTamer = (CTamer*)g_pCharMng->GetTamer( nUIDX );
	if( pTamer )
	{
		pTamer->SetEvent( uItemID );
	}
}
//#endif
void cCliGame::RecvGlobalMsg()
{
	n2	 nCase = 0;
	pop( nCase );
	assert_cs( nCase >= 1 );
	cGlobalMessage* pMsg = g_pGameIF->GetGlobalMsg();
	switch( nCase )
	{
	case 1:	// ë ˆë²¨ì—… ì•„ì´í…œ ì‚¬ìš© ì‹œ ì„œë²„ì˜ ëª¨ë“  ìœ ì €ì—ê²Œ ë©”ì„¸ì§€ ë³´ì—¬ì¤Œ
		{
			std::wstring wsTamerName;
			pop( wsTamerName/*, sizeof( char) * Language::pLength::name*/ );
			u4 nItemNo = 0;
			pop( nItemNo );
			u4 nLevel = 0;
			pop( nLevel );
			std::wstring wsItemName;
			CsItem* pFTItem = nsCsFileTable::g_pItemMng->GetItem( nItemNo );
			if( pFTItem )
			{
				wsItemName = pFTItem->GetName();
				if( wsItemName.empty() )
					wsItemName = L"Unknown Item";
			}
			std::wstring wsLevel;
			DmCS::StringFn::Format( wsLevel, L"%d", nLevel );
			pMsg->SetMessage( nCase, wsTamerName.c_str(), wsItemName.c_str(), wsLevel.c_str() );
		}
		break;
	case pMonster::RandomMonsterCreate:	// ëžœë¤ë¦¬ì   ëª¬ìŠ¤í„° ìƒì„± ë©”ì„¸ì§€
		{
			char szMonName[ MONSTER_NAME_LEN ] = {0, };
			u4 MonIDX = 0;
			pop( MonIDX );
			u4 MapID = 0;
			pop( MapID );
			std::wstring wsMonsterName;
			CsMonster* pMonster = nsCsFileTable::g_pMonsterMng->GetMonster( MonIDX );
			if( pMonster )
			{
				CsMonster::sINFO* pMonsterInfo = pMonster->GetInfo();
				if( pMonsterInfo )
				{
					wsMonsterName = pMonsterInfo->s_szName;
					if( wsMonsterName.empty() )
						wsMonsterName = L"Unknown Monster";
				}
				pMsg->SetMessage( cGlobalMessage::RANDOMREGENBEGIN, wsMonsterName.c_str() );
			}
			//g_pAchieveInfo->GetRandomRegen()->SetRandomRegenStart( pMonster->GetInfo()->s_szName );
		}
		break;
	case pMonster::RandomMonsterEnd:	// ëžœë¤ë¦¬ì   ëª¬ìŠ¤í„° íŒŒê´´ ë©”ì„¸ì§€
		{
			u4 MonIDX = 0;
			pop( MonIDX );
			u4 MapID = 0;
			pop( MapID );
			std::wstring wsMonsterName;
			CsMonster* pMonster = nsCsFileTable::g_pMonsterMng->GetMonster( MonIDX );
			if( pMonster )
			{
				CsMonster::sINFO* pMonsterInfo = pMonster->GetInfo();
				if( pMonsterInfo )
				{
					wsMonsterName = pMonsterInfo->s_szName;
					if( wsMonsterName.empty() )
						wsMonsterName = L"Unknown Monster";
				}
				pMsg->SetMessage( cGlobalMessage::RANDOMREGENEND, wsMonsterName.c_str() );
			}
		}
		break;
	case pMonster::MsgRaidMonsterDie:
		{
			u4 nMonsterIdx = 0;
			pop( nMonsterIdx );
			std::wstring wsTamerName;
			pop(wsTamerName);
			u4 nDigimon = 0;
			pop(nDigimon);
			u4 nItemIDX = 0;
			pop(nItemIDX);
			std::wstring wsMonsterName;
			CsMonster* pFTMonster = nsCsFileTable::g_pMonsterMng->GetMonster( nMonsterIdx );
			if( pFTMonster )
			{
				CsMonster::sINFO* pMonsterInfo = pFTMonster->GetInfo();
				if( pMonsterInfo )
					wsMonsterName = pMonsterInfo->s_szName;
				if( wsMonsterName.empty() )
					wsMonsterName = L"Unknown Monster";
			}
// 			CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( nDigimon );
// 			CsItem* pFTItem = nsCsFileTable::g_pItemMng->GetItem( nItemIDX );
// 
// 			W2M( szDigimonName, pDigimon->GetInfo()->s_szName, 64 );
// 			W2M( szItemName, pFTItem->GetInfo()->s_szName, 64 );
			std::wstring wsDigimonName;
			CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( nDigimon );
			if( pDigimon )
			{
				CsDigimon::sINFO* pDigimonInfo = pDigimon->GetInfo();
				if( pDigimonInfo )
					wsDigimonName = pDigimonInfo->s_szName;
				if( wsDigimonName.empty() )
					wsDigimonName = L"Unknown Digimon";
			}
			// í•˜ë“œ ì½”ë”©ìž„ ë‚˜ì¤‘ì— ì„œë²„ì™€ í˜‘ì˜ í›„ ìˆ˜ì • í•„ìš”í•¨.
			std::wstring wsItemName;
			if( 0 != nItemIDX )
			{
				CsItem* pFTItem = nsCsFileTable::g_pItemMng->GetItem( nItemIDX );
				if( pFTItem )
				{
					wsItemName = pFTItem->GetName();
					if( wsItemName.empty() )
						wsItemName = L"Unknown Item";
				}
			}
			else
			{
				wsItemName = UISTRING_TEXT( "GLOBAL_MESSAGE_VERDANDI_PEACE" );
			}
			pMsg->SetMessage( cGlobalMessage::FILEBLESS, wsMonsterName.c_str(), wsTamerName.c_str(), wsDigimonName.c_str(), wsItemName.c_str() );
		}
		break;
	default:
		assert_cs( false );
	}
}
// ì´ë¯¸ ë°›ì•˜ë˜ ë¦¬ìŠ¤íŠ¸ë¥¼ ë°›ëŠ”ë‹¤.
void cCliGame::RecvRequestRewardList()
{
	u4 count = 0;
	u1 idx = 0;
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::REWARD_REFRESH, 0 );	
	pop(count);
	//2017-03-15-nova
	GS2C_RECV_REWARD_DATA recv;	
	for( u4 i=0; i<count; ++i )
	{
		pop( idx );
		recv.AddData(idx);
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::REWARD_RECEIVE_REWARD, &recv );
}
// ë²„íŠ¼ ëˆ„ë¥¸í›„ ì•„ì´í…œ ë°›ê¸° ì²˜ë¦¬
void cCliGame::RecvRequestRewards()
{
	u1 count = 0;
	u1 idx = 0;
	CsReward::MAP*	pMapReward = nsCsFileTable::g_pRewardMng->GetReward( g_pResist->m_AccountGlobal.s_nLastConnetServer);
	// í…Œì´ë¸” ë¸”ëŸ¬ì˜¤ê¸° ì‹¤íŒ¨, UI ë‹«ì•„ì¤€ë‹¤.
	if(NULL == pMapReward)
	{		
		if(true == g_pGameIF->IsActiveWindow(cBaseWindow::WT_REWARD))
		{
			g_pGameIF->CloseDynamicIF(cBaseWindow::WT_REWARD);
		}
	}
	
	pop(count);
	
	for(int i =0; i < count; i++)
	{
		pop(idx);
		if( pMapReward )
		{
			CsReward * preward = (*pMapReward)[idx];
			if(NULL != preward)
			{
				int nItemIndex = preward->GetInfo()->s_nItemCode1;
				int nItemCount = preward->GetInfo()->s_nItemCount1;
				// [4/14/2016 hyun] ìˆ˜ì •. ì•„ì´í…œ íšë“ì‹œ ì»¬ëŸ¬ê°’ìœ¼ë¡œ ì¶œë ¥ì„ ìœ„í•´
				// ë³´ìƒ ë°›ì•˜ë‹¤ë©´ ë©”ì„¸ì§€
				CsItem::sINFO* pItem = nsCsFileTable::g_pItemMng->GetItem( nItemIndex )->GetInfo();
				if(pItem)
				{
					if(nItemCount == 1)
						cPrintMsg::PrintMsg( cPrintMsg::GAME_ITEM_CROP, pItem->s_szName, &g_pDataMng->GetItemColor(pItem) );
					else if(nItemCount > 1)
						cPrintMsg::PrintMsg( cPrintMsg::GAME_ITEM_MANYCROP, pItem->s_szName, &nItemCount, &g_pDataMng->GetItemColor(pItem));
				}
				// @@ ì—¬ê¸°ê¹Œì§€
				g_pDataMng->GetInven()->ItemCrop( nItemIndex, nItemCount );
				// ë¦¬ìŠ¤íŠ¸ì— ìžˆëŠ” ì•„ì´í…œ ì§€ì›Œì¤€ë‹¤.
//				g_pDataMng->GetCash()->DeleteRewardItem( idx );
				GS2C_RECV_REWARD_SEND_TYPE recv;
				recv.iType = 4;
				recv.iIndex = idx;
				GAME_EVENT_ST.OnEvent( EVENT_CODE::REWARD_SEND_ACCPET_DATA, &recv );
			}
		}
	}
	g_pDataMng->GetServerSync()->RecvServer( cData_ServerSync::GET_REWARD, 0 );	
	
}
void cCliGame::RecvRequestRewardFail()
{
	u1 type = 0;
	pop( type );
}
void cCliGame::RecvMastersMatchOpen()
{
	//ì—´ì–´ë³´ìž
	g_pGameIF->GetDynamicIF( cBaseWindow::WT_MASTERS_MATCHING );
	//íƒ€ê²Ÿ ê°€ì ¸ì™€ì„œ
	CsC_AvObject* pTarget = g_pGameIF->GetMastersMatchTarget();
	//íƒ€ê²Ÿ ì…‹íŒ…
	g_pGameIF->GetMastersMatching()->SetTarget( pTarget );
	//ë‹¤ìŒ ë¦¬ì…‹ ì‹œê°„
	u4 nResetTime = 0;
	char	szResetTime[ 32 ];
	pop( szResetTime ); // 00:00:00
	
 	strcpy( g_pGameIF->m_sMyInfo.s_szNextResetTime, szResetTime );
	pop( nResetTime ); // í‹±ì‹œê°„
	g_pGameIF->m_sMyInfo.s_nNextResetTime = nResetTime;
	//ê° íŒ€ ê¸°ë¶€ ê°¯ìˆ˜
	u4 nTeamCnt = 2345 ;			
	for(int i = 0 ; i < B_TEAM ; i ++)
	{
		pop( nTeamCnt );
		g_pGameIF->m_sRankInfo.s_nCnt[ i ] = nTeamCnt;
		g_pGameIF->GetMastersMatching()->_SetTeamCnt( i+1, nTeamCnt );
	}
//////////////////////////////ë³¸ì¸ ì •ë³´/////////////////////////////////////
	u4 nMyCnt = 52;
	u2 nMyRank = 62;
	u1 nMyTeam = 1;
	pop( nMyCnt );
	pop( nMyRank );
	pop( nMyTeam );
	g_pGameIF->m_sMyInfo.s_nMyCnt = nMyCnt;
	g_pGameIF->m_sMyInfo.s_nMyRank = nMyRank;
	//íŒ€ ê²°ì • ì•ˆë˜ìžˆìœ¼ë©´ ì•„ì´ì½˜ ë¤ìœ¼ë¡œ ì…‹íŒ…
	if( g_pCharMng->GetTamerUser()->GetMatchTeam() == NONE )
		g_pCharMng->GetTamerUser()->GetCharOption()->SetMatchIcon( nMyTeam );
	g_pGameIF->m_sMyInfo.s_eMyTeam = (eTEAM)nMyTeam;
	
///////////////////////////////////////////////////////////////////////////////
	
////////////////////////////////ëž­ì»¤ ì •ë³´/////////////////////////////////////
	u2 nRank = 111;							//ëž­ì»¤ ëž­í‚¹
	//char szTamer[ Language::pLength::id + 5 ];		//ëž­ì»¤ ì´ë¦„
	u4 nCount = 112;						//ëž­ì»¤ ê¸°ë¶€ê°¯ìˆ˜
	//TCHAR	tszTamer[ Language::pLength::id + 5 ];
// 	_tcscpy_s( tszTamer, Language::pLength::id + 5, _T("í…Œì´ë¨¸10ë“±ê¹Œì§€ìˆœìœ„") );
	cStringList* pRankList_A = NiNew cStringList;	//AíŒ€ ë¦¬ìŠ¤íŠ¸
	cStringList* pRankList_B = NiNew cStringList;	//BíŒ€ ë¦¬ìŠ¤íŠ¸
	// ë¦¬ìŠ¤íŠ¸ ìˆœì„œ :  AíŒ€ ( ëž­í‚¹ - ì´ë¦„ - ê¸°ë¶€ê°¯ìˆ˜) * 10 
	//					   BíŒ€ ( ëž­í‚¹ - ì´ë¦„ - ê¸°ë¶€ê°¯ìˆ˜) * 10 
	cString* pString;		
	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem );
	// AíŒ€
	for( u4 i = 0 ; i < 10 ; i++ )//10ë“±
	{
		pop( nRank );
		if( nRank == 0 ) // 0ì´ë©´ í•´ë‹¹ ë“±ìˆ˜ë¶€í„° 10ë“±ê¹Œì§€ëŠ” ë¯¸ë“±ë¡ìƒíƒœ ì…‹íŒ…
		{
			for( int j = i ; j < 10 ; j++ )
			{
				pString = NiNew cString;
				//ëž­í‚¹
				ti.s_eTextAlign = DT_CENTER;
				ti.SetText( j+1 );
				ti.AddText(L".");
				pString->AddText( &ti );
				pString->TailAddSizeX( 20 - pString->GetMaxSize().x );
				//ì´ë¦„
				ti.s_eTextAlign = DT_LEFT;
				ti.SetText( UISTRING_TEXT( "COMMON_TXT_NOT_REGIST" ).c_str() );
				pString->AddText( &ti );
				pString->TailAddSizeX( 170 - pString->GetMaxSize().x );
				//ê¸°ë¶€ê°¯ìˆ˜
				ti.s_eTextAlign = DT_RIGHT;
				ti.SetText( _T("0") );
				pString->AddText( &ti );
				pRankList_A->AddTail( pString );
			}
			break;
		}
		std::wstring szTamer;	//ëž­ì»¤ ì´ë¦„
		pop( szTamer );
		pop( nCount );	
		pString = NiNew cString;
		//ëž­í‚¹
		ti.SetText( nRank );
		ti.AddText(L".");
		ti.s_eTextAlign = DT_CENTER;
		pString->AddText( &ti );
		pString->TailAddSizeX( 20 - pString->GetMaxSize().x );
		//ì´ë¦„ + ì‚­ì œëœ ì•„ì´ë”” ê²€ì‚¬
		int nOrg = szTamer.size();
		std::wstring wsToken = L"_";
		std::wstring wsTempstr = DmCS::StringFn::tokenize( szTamer, wsToken);
		if ( nOrg != wsTempstr.size() )
		{
			//'_' ì•žë¶€ë¶„ì´ë‹ˆê¹Œ ì•„ì´ë”” ì–»ìœ¼ë ¤ë©´ í•œë²ˆë” ìž˜ë¼ì¤˜ì•¼í•¨
			wsTempstr = DmCS::StringFn::tokenize( szTamer, wsToken);
		}
		ti.SetText( wsTempstr.c_str() );
		ti.s_eTextAlign = DT_LEFT;
		pString->AddText( &ti );
		pString->TailAddSizeX( 170 - pString->GetMaxSize().x );
		//ê¸°ë¶€ê°¯ìˆ˜
		ti.s_eTextAlign = DT_RIGHT;
		ti.SetText( nCount );
		pString->AddText( &ti );
		//AíŒ€ ë¦¬ìŠ¤íŠ¸ Add
		pRankList_A->AddTail( pString );
	}
	g_pGameIF->m_sRankInfo.s_pRankList_A = pRankList_A;
	// BíŒ€
	for( int i = 0 ; i < 10 ; i++ )
	{
		pop( nRank );
		if( nRank == 0 ) // 0ì´ë©´ í•´ë‹¹ ë“±ìˆ˜ë¶€í„° 10ë“±ê¹Œì§€ëŠ” ë¯¸ë“±ë¡ìƒíƒœ ì…‹íŒ…
		{
			for( int j = i ; j < 10 ; j++ )
			{
				pString = NiNew cString;
				//ëž­í‚¹
				ti.s_eTextAlign = DT_CENTER;
				ti.SetText( j+1 );
				ti.AddText(L".");
				pString->AddText( &ti );
				pString->TailAddSizeX( 20 - pString->GetMaxSize().x );
				//ì´ë¦„
				ti.s_eTextAlign = DT_LEFT;
				ti.SetText( UISTRING_TEXT( "COMMON_TXT_NOT_REGIST" ).c_str() );
				pString->AddText( &ti );
				pString->TailAddSizeX( 170 - pString->GetMaxSize().x );
				//ê¸°ë¶€ê°¯ìˆ˜
				ti.s_eTextAlign = DT_RIGHT;
				ti.SetText( _T("0") );
				pString->AddText( &ti );
				pRankList_B->AddTail( pString );
			}
			break;
		}
		std::wstring szTamer;	//ëž­ì»¤ ì´ë¦„
		pop( szTamer );
		pop( nCount );		
		pString = NiNew cString;
		//ëž­í‚¹
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( nRank );
		ti.AddText(L".");
		pString->AddText( &ti );
		pString->TailAddSizeX( 20 - pString->GetMaxSize().x );
		
		//ì´ë¦„ + ì‚­ì œëœ ì•„ì´ë”” ê²€ì‚¬
		std::wstring wsToken = L"_";
		int nOrg = szTamer.size();
		std::wstring wsTempstr = DmCS::StringFn::tokenize( szTamer, wsToken);
		if (nOrg != wsTempstr.size())
		{
			//'_' ì•žë¶€ë¶„ì´ë‹ˆê¹Œ ì•„ì´ë”” ì–»ìœ¼ë ¤ë©´ í•œë²ˆë” ìž˜ë¼ì¤˜ì•¼í•¨
			wsTempstr = DmCS::StringFn::tokenize( szTamer, wsToken);
		}
		ti.s_eTextAlign = DT_LEFT;
		ti.SetText( wsTempstr.c_str() );
		pString->AddText( &ti );
		pString->TailAddSizeX( 170 - pString->GetMaxSize().x );
		//ê¸°ë¶€ê°¯ìˆ˜
		ti.s_eTextAlign = DT_RIGHT;
		ti.SetText( nCount );
		pString->AddText( &ti );
		//BíŒ€ ë¦¬ìŠ¤íŠ¸ Add
		pRankList_B->AddTail( pString );
	}	
	g_pGameIF->m_sRankInfo.s_pRankList_B = pRankList_B;
	//ë¦¬ìŠ¤íŠ¸ ì—…ë°ì´íŠ¸
	g_pGameIF->GetMastersMatching()->RefreshList();
///////////////////////////////////////////////////////////////////////////////
	//ì˜¤í”ˆ ì¤‘ ë©”ì„¸ì§€ ì‚­ì œ
	cMessageBox::DelMsg( 10019, false );
}
void cCliGame::RecvMastersMatchInsert()
{
	//ì „ì²´ ë½í’€ê³ 
	g_pDataMng->ReleaseItemLock();
	u4 nSlot = g_pGameIF->GetMastersMatching()->m_nItemSlot;			// ê¸°ë¶€í•  ì•„ì´í…œ ìžˆëŠ” ì¸ë²¤ ë²ˆí˜¸
	u4 nItemCnt = g_pGameIF->GetMastersMatching()->m_nItemCnt;		// ê¸°ë¶€ ê°¯ìˆ˜
	//ì—…ì  í™•ì¸
	g_pGameIF->GetMastersMatching()->_CheckAchieve( nItemCnt );
	//ìžì‹ ì´ ê¸°ë¶€í•œ ì´ ê°¯ìˆ˜(í™”ë©´ì— ë³´ì—¬ì§€ëŠ”) ++
	g_pGameIF->m_sMyInfo.s_nMyCnt += nItemCnt;
	//ê¸°ë¶€ì•„ì´í…œ ê°ì†Œ
	g_pDataMng->GetInven()->GetData( nSlot )->DecreaseCount( nItemCnt );
	//ì—ë””íŠ¸ë°•ìŠ¤ 0ìœ¼ë¡œ
	g_pGameIF->GetMastersMatching()->_SetCount( 0, true );
	//ë¦¬ìŠ¤íŠ¸ ì—…ë°ì´íŠ¸
	g_pGameIF->GetMastersMatching()->RefreshList();
	//ì˜¤í”ˆ ì¤‘ ë©”ì„¸ì§€ ì§€ì›Œ
	cMessageBox::DelMsg( 10019, false );
	//ì•„ì´í…œ ê¸°ë¶€ ì™„ë£Œ ë©”ì„¸ì§€
	cPrintMsg::PrintMsg( 30622, &nItemCnt );
}
#if !COMMON_LIB_FIXED
struct GS2C_RECV_ENCHANT_ITEM_IDENTIFY
{
	u2 nInvenPos;
	u1 nRate;
	u1 nLevel;
	u2 nAccOption[nLimit::MAX_ACCESSORY_OPTION];
	u2 nAccValues[nLimit::MAX_ACCESSORY_OPTION];
};
struct GS2C_RECV_ENCHANT_ITEM_ENCHANT
{
	u1 nResult;
	u2 nInvenPos;
	u1 nRate;
	u1 nLevel;
	u2 nAccOption[nLimit::MAX_ACCESSORY_OPTION];
	u2 nAccValues[nLimit::MAX_ACCESSORY_OPTION];
};
#endif
void cCliGame::RecvEnchantItemIdentify()
{
	GS2C_RECV_ENCHANT_ITEM_IDENTIFY recv;
	pop( recv.nInvenPos );		// ì•…ì„¸ ì¸ë²¤ ìœ„ì¹˜
	pop( recv.nRate );			// ë””ì§€í„°ë¦¬ ìˆ˜ì¹˜( 95~200% )
	pop( recv.nLevel );			// ë””ì§€ì–´ë¸” íŒŒì›Œ ë¦¬ë‰´ì–¼( íšŸìˆ˜ )
	pop( recv.nAccOption );		// ì˜µì…˜ ì¢…ë¥˜
	pop( recv.nAccValues );		// ì˜µì…˜ ëŠ¥ë ¥ì¹˜
	DBG("RecvEnchantItemIdentify: slot=%u rate=%u level=%u opt0=%u val0=%u",
		recv.nInvenPos, recv.nRate, recv.nLevel, recv.nAccOption[0], recv.nAccValues[0]);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ENCHANT_OPTION_IDENTIFY_RESULT, &recv );
}
void cCliGame::RecvEnchantItemEnchant()
{
	GS2C_RECV_ENCHANT_ITEM_ENCHANT recv;
	// ì¸ì²¸ ì„±ê³µ ì—¬ë¶€
	// 1 : ì„±ê³µ, 2 : ì‹¤íŒ¨, 3 : ë³´ì¡´
	pop( recv.nResult );
	pop( recv.nInvenPos );
	pop( recv.nRate );
	pop( recv.nLevel );
	pop( recv.nAccOption );
	pop( recv.nAccValues );
	DBG("RecvEnchantItemEnchant: result=%u slot=%u rate=%u level=%u opt0=%u val0=%u",
		recv.nResult, recv.nInvenPos, recv.nRate, recv.nLevel, recv.nAccOption[0], recv.nAccValues[0]);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ENCHANT_OPTION_ENCHANT_RESULT, &recv );
}
void cCliGame::RecvIncubaterList()		//	ì¸íë² ì´í„° ì˜¤í”ˆ
{
//	g_pGameIF->GetDynamicIF( cBaseWindow::WT_DIGIMON_INCUBATOR );
	u4 nSlot = 0;				// ì¸íë² ì´í„°ì— ë“¤ì–´ìžˆëŠ” ë””ì§€ëª¬ì˜ ë””ë³´ ìŠ¬ë¡¯ ë²ˆí˜¸ ( 1000 ~ )
	u4 nItemID = 0;				//	ì¸íë² ì´í„°ì— ë°–í˜€ìžˆëŠ” ì•„ì´í…œë²ˆí˜¸
	u4 nRemainTime = 0;		//	ì•„ì´í…œ ë§Œë£Œ ì‹œê°„
	
	const int MaxIncubatorCnt = 3;
	GS2C_RECV_INCUBATOR_INFO kIncuRecvInfo;
	for( int i = 0 ; i < 3 ; i++ )
	{
		pop( nSlot );
		pop( nItemID );
		pop( nRemainTime );
		kIncuRecvInfo.AddIncuInfo(i, nSlot, nItemID, nRemainTime);
	}
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_INCUBATOR_INFO, &kIncuRecvInfo);
	//ë°ì´í„° ìˆ˜ì‹ ì¤‘ ë©”ì„¸ì§€
	cMessageBox::DelMsg( 10019, false );
}
void cCliGame::RecvIncubatorFacilitation()		//	ì´‰ì§„
{
	u1 nSlot = 0;			//ì¸íë² ì´í„° ìŠ¬ë¡¯
	u4 nRemainTime = 0;	//ë§Œë£Œì‹œê°„
	pop(nSlot);
	pop(nRemainTime);
	GS2C_RECV_START_INCUBATING_INFO kRecvData;
	kRecvData.Incu_slotIdx = nSlot;
	kRecvData.RemainTime = nRemainTime;
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_START_INCUBATING_INFO, &kRecvData);
	// ë“±ë¡í•œ ë””ì§€ëª¬, ì•„ì´í…œì„ Serv ë°ì´í„°ë¡œ ì…‹íŒ…
	//g_pGameIF->GetTacticsHouse()->OnRecvFacilitation( nSlot, nRemainTime );
}
void cCliGame::RecvIncubatorExtraction()			//	ì¶”ì¶œ ( êº¼ë‚´ê¸° )
{
	// ì—†ì–´ì§
	u1 nSlot = 0;		// ì¸íë² ì´í„° ìŠ¬ë¡¯ë²ˆí˜¸
	pop( nSlot );
	
	
	GS2C_RECV_STOP_INCUBATING_INFO kRecvData;
	kRecvData.Incu_slotIdx = nSlot;
	// ë“±ë¡ë˜ìžˆëŠ” ë””ì§€ëª¬, ì•„ì´í…œ ì œê±°
	GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_STOP_INCUBATING_INFO, &kRecvData);
	//g_pGameIF->GetIncubator()->_RecvExtraction( nSlot );
}
void cCliGame::RecvIncubaterSlotChange()
{
	n4 nIncuSlot = 0;				// ì¸íë² ì´í„°ì— ë“¤ì–´ìžˆëŠ” ë””ì§€ëª¬ì˜ ë””ë³´ ìŠ¬ë¡¯ ë²ˆí˜¸ ( 1000 ~ )
	n4 nHouseSlot = 1000;				//	ë””ë³´ ìŠ¬ë¡¯(1000~)
 	pop( nIncuSlot );
	if( nIncuSlot == -1 )//ì‹¤íŒ¨
	{
		cPrintMsg::PrintMsg( 11014 );	// í•´ë‹¹ ìŠ¬ë¡¯ì— ë“±ë¡í•  ìˆ˜ ì—†ìŠµë‹ˆë‹¤	
		//ë°ì´í„° ìˆ˜ì‹ ì¤‘ ë©”ì„¸ì§€
		cMessageBox::DelMsg( 10019, false );
		return;
	}
   pop( nHouseSlot );
   GS2C_RECV_INCUBATOR_CHANGE kRecvData;
   kRecvData.Incu_slotIdx = nIncuSlot;
   kRecvData.Archive_slotIdx = nHouseSlot;
  GAME_EVENT_STPTR->OnEvent(EVENT_CODE::RECV_CHANGE_INCUBATOR, &kRecvData);
 //  g_pGameIF->GetIncubator()->_RecvDigimonChange( nIncuSlot, nHouseSlot );
}
void cCliGame::RecvIncubatorError()			//	ì˜ˆì™¸ìƒí™© ì²˜ë¦¬
{
	u4 nErrorCode;		// ì—ëŸ¬ì½”ë“œ
	pop( nErrorCode );
	enum eERR_CODE{
		ERR_FACILITATION	= 1,
		ERR_2					= 2,
		ERR_3					= 3,
	};
	switch( nErrorCode )
	{
	case ERR_FACILITATION:	//í•´ë‹¹ ì¸íë² ì´í„° ìŠ¬ë¡¯ì— ë””ì§€ëª¬ ìžˆì„ ë•Œ ë””ì§€ëª¬ ë„£ê¸° ì‹œë„
		{
			//ë©”ì„¸ì§€ ë„ìš°ê³ 
			//ì¸íë² ì´í„°ì°½ ë„ê¸°
		}
		break;
	case ERR_2:	// 
		break;
	}
}
void cCliGame::RecvSetMasterCardLeader()
{
	u4 nTamerID = 0;
	GS2C_RECV_SEALMASTER_LEADER  Leader;
	pop(nTamerID);
	pop(Leader.m_nLeader);	
	CTamer* pTamer = (CTamer*)g_pMngCollector->GetObject( nTamerID );	
	if( pTamer != NULL )
	{
		CsMaster_CardLeader::sINFO*	sInfo = nsCsFileTable::g_pMaster_CardMng->GetMasterCardLeader(Leader.m_nLeader)->GetInfo();
		
		CsMaster_Card::MAP_IT	it = nsCsFileTable::g_pMaster_CardMng->GetMasterCardMap()->begin();
		CsMaster_Card::MAP_IT	it_end = nsCsFileTable::g_pMaster_CardMng->GetMasterCardMap()->end();
		float fscale = 0.1f;
		for( ;it != it_end; ++it)
		{
			if(it->second->GetInfo()->s_nDigimonID == sInfo->s_nDigimonID)
			{
				fscale = it->second->GetInfo()->s_nScale * 0.01f;
			}
		}
		pTamer->DeletePat();
		pTamer->AddPat(sInfo->s_nDigimonID, fscale);
		// ë‚´ ìžì‹ ì˜ ìºë¦­í„°ì¼ ë•Œë§Œ ë°ì´í„°ë¥¼ ìˆ˜ì •í•˜ê³  ë©”ì‹œì§€ë¥¼ ë³´ì—¬ì¤ë‹ˆë‹¤.
		if( g_pCharMng->GetTamerUser()->GetUniqID() == nTamerID)
		{
			GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_SEALMASTER_LEADER, &Leader );
			cPrintMsg::PrintMsg( 30654 );	// ë¦¬ë”ê°€ ì„¤ì •ë˜ì—ˆìŠµë‹ˆë‹¤.
		}
	}
}
void cCliGame::RecvUnSetMasterCardLeader()
{
	u4 nTamerID = 0;
	u2 nCardID = 0;
	pop(nTamerID);
	pop(nCardID);
	CTamer* pTamer = (CTamer*)g_pMngCollector->GetObject( nTamerID );	
	if( pTamer != NULL )
	{		
		pTamer->DeletePat();
		// ë‚´ ìžì‹ ì˜ ìºë¦­í„°ì¼ ë•Œë§Œ ë°ì´í„°ë¥¼ ìˆ˜ì •í•©ë‹ˆë‹¤.
		if( g_pCharMng->GetTamerUser()->GetUniqID() == nTamerID)
		{
			GS2C_RECV_SEALMASTER_LEADER  Leader;	// ë¦¬ë” ì—†ìŒ -1ë¡œ ì´ˆê¸°í™” ë˜ì–´ ìžˆìŒ
			GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_SEALMASTER_LEADER, &Leader );
		}
	}
}
void cCliGame::RecvCardInfoAll(void)
{
	
	GS2C_RECV_SEALMASTER_LEADER  Leader;
	pop(Leader.m_nLeader);
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_SEALMASTER_LEADER, &Leader );
	u2 nCardSlotCount = 0;	// ì¹´ë“œ ì¢…ë¥˜ ê°œìˆ˜
	pop( nCardSlotCount );
	GS2C_RECV_SEALMASTER_SEALINFO  RecvSealInfo;
	for ( n4 i = 0; i < nCardSlotCount; i++ )
	{
		RecvCardInfoAll::SealInfo sealInfo;
		pop( sealInfo.m_nCardCode );
		pop( sealInfo.m_nSealID );
		pop( sealInfo.m_nCardCount );
		RecvSealInfo.m_SealInfoList.push_back(sealInfo);
	}
	u2 nCardBookmarkCount = 0;	// ì¦ê²¨ì°¾ê¸° ì •ë³´ ê°œìˆ˜
	pop( nCardBookmarkCount );
	for ( n4 i = 0; i < nCardBookmarkCount; i++ )
	{
		RecvCardInfoAll::FavoriteInfo sealFavorite;
		pop( sealFavorite.m_nCardCode );
		pop( sealFavorite.m_nBookmark );
		RecvSealInfo.m_FavoriteInfoList.push_back(sealFavorite);
	}
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_SEALMASTER_SEALINFO, &RecvSealInfo );
}
void cCliGame::RecvCardBookmark(void)
{
	GS2C_RECV_SEALMASTER_FAVORITE sealFavorite;
	pop( sealFavorite.m_nCardCode );
	pop( sealFavorite.m_nBookmark );
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::RECV_SEALMASTER_FAVORITE, &sealFavorite);
}
	
void cCliGame::RecvTacticsOpen()
{
// 	u2 nTargetUID = g_pCharMng->GetTamerUser()->GetUniqID();
// 	n2 nInvenPos;
// 
// 	pop(nTargetUID);
// 	pop(nInvenPos);
// 
// 	g_pDataMng->ServerItemUseSuccess( nTargetUID, nInvenPos );
}
void cCliGame::RecvEncyclopediaOpen()
{
	DWORD const dwBegin = GetTickCount();
	// ë„ê° ì •ë³´( ë°ì´í„°ê°€ ìžˆëŠ” ë†ˆë“¤ë§Œ)
	// ë³´ìœ  ê³„ì—´ì²´ ìˆ˜
	GS2C_RECV_ENCYCLOPEDIA_OPEN recv;
	n4 nCnt = 0;
	pop( nCnt );	// ë°ì´í„°ê°€ ì¡´ìž¬í•˜ëŠ” ( í•œë²ˆì´ë¼ë„ ì˜¤í”ˆ í•œ ) ë””ì§€ëª¬ ê³„ì—´ì²´ ê°¯ìˆ˜
	nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPACKET] RecvEncyclopediaOpen begin count=%d", (int)nCnt );
	for( int i = 0 ; i < nCnt ; i++ )
	{
		sEncyclopediaOpendedData sData;		
		pop( sData.nDigimonID );	// 1. ê³„ì—´ì²´ ë””ì§€ëª¬ ë²ˆí˜¸	// ê³„ì—´ì²´ ë²ˆí˜¸
		pop( sData.nLevel );			// 2. ê³„ì—´ì²´ ë ˆë²¨
#ifdef SDM_DIGIMONBOOK_EXTEND_20181126
		pop( sData.nSlotOpened );	// 3. ê³„ì—´ì²´ ë‚´ ì§„í™” ìŠ¬ë¡¯ ì˜¤í”ˆ ì •ë³´ shift ì—°ì‚° í•„ìš”
#else
		for( int i = 1 ; i < 9 ; i++ )	//8ë²ˆ pop, ì¸ë±ìŠ¤ëŠ” 1ë¶€í„° ì‚¬ìš©
			pop( sData.nIsOpen[i] );	// 3. ë””ì§€ëª¬ ì˜¤í”ˆ ì—¬ë¶€( 8ë²ˆ pop, ì§„í™”ìŠ¬ë¡¯ 1~8ê¹Œì§€ ) 0 : Open, 2: Close 
#endif	
		pop( sData.nEnchant_AT );		// ê³„ì—´ì²´ - ê°•í™”::AT
		pop( sData.nEnchant_BL );		// ê³„ì—´ì²´ - ê°•í™”::BL
		pop( sData.nEnchant_CT );		// ê³„ì—´ì²´ - ê°•í™”::CT
		pop( sData.nEnchant_EV );		// ê³„ì—´ì²´ - ê°•í™”::EV
		pop( sData.nEnchant_HP );		// ê³„ì—´ì²´ - ê°•í™”::HP
		//ê³„ì—´ì²´ ì •ë³´	
		pop( sData.nSize );			// 8. ê³„ì—´ì²´ ì‚¬ì´ì¦ˆ
		pop( sData.bIsReward );		// 9. ë³´ìƒì—¬ë¶€, 0: ì•„ì§ ìˆ˜ë ¹ ì•ˆí•¨, 1: ì•„ì´í…œ íšë“í–ˆìŒ
		if( i < 12 )
		{
			nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPACKET] RecvEncyclopediaOpen record idx=%d base=%u level=%u mask=0x%08lX%08lX size=%d reward=%d enchAT=%u enchBL=%u enchCT=%u enchEV=%u enchHP=%u",
				i,
				(unsigned)sData.nDigimonID,
				(unsigned)sData.nLevel,
				(unsigned long)( ( sData.nSlotOpened >> 32 ) & 0xFFFFFFFFULL ),
				(unsigned long)( sData.nSlotOpened & 0xFFFFFFFFULL ),
				(int)sData.nSize,
				(int)sData.bIsReward,
				(unsigned)sData.nEnchant_AT,
				(unsigned)sData.nEnchant_BL,
				(unsigned)sData.nEnchant_CT,
				(unsigned)sData.nEnchant_EV,
				(unsigned)sData.nEnchant_HP );
		}
		recv.m_listOpendedData.push_back( sData );
	}
	DWORD const dwParsedMs = GetTickCount() - dwBegin;
	nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPACKET] RecvEncyclopediaOpen dispatch count=%d parseMs=%u",
		(int)nCnt,
		(unsigned)dwParsedMs );
	DWORD const dwOpenDataBegin = GetTickCount();
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ENCYCLOPEDIA_OPEN_DATA, &recv );
	DWORD const dwOpenDataMs = GetTickCount() - dwOpenDataBegin;
	DWORD const dwReadyBegin = GetTickCount();
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ENCYCLOPEDIA_RECV_SERVER, NULL );
	DWORD const dwReadyMs = GetTickCount() - dwReadyBegin;
	nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPACKET] RecvEncyclopediaOpen end count=%d parseMs=%u openDataEventMs=%u readyEventMs=%u totalMs=%u",
		(int)nCnt,
		(unsigned)dwParsedMs,
		(unsigned)dwOpenDataMs,
		(unsigned)dwReadyMs,
		(unsigned)( GetTickCount() - dwBegin ) );
	ENCY_PACKET_PERF( "RecvEncyclopediaOpen end count=%d parseMs=%u openDataEventMs=%u readyEventMs=%u totalMs=%u",
		(int)nCnt,
		(unsigned)dwParsedMs,
		(unsigned)dwOpenDataMs,
		(unsigned)dwReadyMs,
		(unsigned)( GetTickCount() - dwBegin ) );
	//ë°ì´í„° ìˆ˜ì‹ ì¤‘ ë©”ì„¸ì§€
	cMessageBox::DelMsg( 10019, false );
}
void cCliGame::RecvEncyclopediaGetItem()
{
	//íšë“ ì•„ì´í…œ ìƒì„±
	u4 nItemID(5001);		// íšë“ ì•„ì´í…œ
	u2 nItemCnt(2);	// íšë“ ì•„ì´í…œ ê°¯ìˆ˜
	pop( nItemID );
	pop( nItemCnt );
	g_pDataMng->GetInven()->ItemCrop( nItemID, nItemCnt );
	//ê³„ì—´ì²´ ë³´ìƒ ì•„ì´í…œ ìˆ˜ë ¹ ì—¬ë¶€ ì²´í¬
	int nDigimonID = g_pGameIF->GetEncyclopedia()->_GetSelectDigimonID();		// ê³„ì—´ì²´ ë””ì§€ëª¬ ë²ˆí˜¸
	GS2C_RECV_ENCYCLOPEDIA_BISREWARD recv;
	recv.nDigimonID = nDigimonID;
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::ENCYCLOPEDIA_BISREWARD, &recv );
	//ì•„ì´í…œ íšë“ í‘œì‹œ
	CsItem::sINFO* pFTItem = nsCsFileTable::g_pItemMng->GetItem( nItemID )->GetInfo();
	// [4/14/2016 hyun] ìˆ˜ì •. ì•„ì´í…œ ì»¬ëŸ¬ê°’ ì¶”ê°€
	if(pFTItem)
	{
		int nItemCount = nItemCnt;
		if(nItemCount == 1)
			cPrintMsg::PrintMsg( cPrintMsg::GAME_ITEM_CROP, pFTItem->s_szName, &g_pDataMng->GetItemColor(pFTItem));	// ì‹œìŠ¤í…œì°½
		else if(nItemCount > 1)
			cPrintMsg::PrintMsg( cPrintMsg::GAME_ITEM_MANYCROP, pFTItem->s_szName, &nItemCount, &g_pDataMng->GetItemColor(pFTItem));	// ì‹œìŠ¤í…œì°½
		cPrintMsg::PrintMsg( 30671, pFTItem->s_szName );	// í™”ë©´ì¤‘ì•™
	}
	// @@ ì—¬ê¸°ê¹Œì§€
	//ë°ì´í„° ìˆ˜ì‹ ì¤‘ ë©”ì„¸ì§€
	cMessageBox::DelMsg( 10019, false );
}
void cCliGame::RecvEncyclopediaBuff()
{
	//ì¼ì • ì‹œê°„ë™ì•ˆ ì ìš©ë˜ëŠ” ë°œë™í˜• ë²„í”„ ì •ë³´
	//ë± íš¨ê³¼ì˜ ì¸ë±ìŠ¤(0,1,2) : í•œ ë±ì˜ íš¨ê³¼ëŠ” ìµœëŒ€ 3ê°œ	
	//ì¿¨íƒ€ìž„ ì´ˆê¸°í™”ì¼ ê²½ìš° 100ìœ¼ë¡œ ì˜´
	u2 nOptIdx = 2;
	u4 nEndTime = 0;//= _TIME_TS + 10;	//ë°œë™ëœ íš¨ê³¼ ì¢…ë£Œ í‹±ì‹œê°„
	pop( nOptIdx );
	if( nOptIdx == 100 )		//ì¿¨íƒ€ìž„ ì´ˆê¸°í™”
	{
		g_pCharMng->GetDigimonUser(0)->ResetSkillCool();
		return;
	}
	if( nOptIdx >= ENCY_MAX_OPTION )
	{
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString(_T("Over Max OptIdx - OptIdx:%d"), nOptIdx);
		DmCS::StringFn::Format(CProtocol.m_wStr, _T("Over Max OptIdx - OptIdx:%d"), nOptIdx);
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	bool bGroupInfoNULL = false;
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::GET_ENCYCLOPEDIA_NULL_CHECK, &bGroupInfoNULL );
	if(bGroupInfoNULL)
	{
		return;
	}
	pop( nEndTime );
	GS2C_RECV_ENCYCLOPEDIA_OPTIDX recv;
	recv.nOptIdx = nOptIdx;
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::ENCYCLOPEDIA_GROUPINFOCON, &recv );
	
	if( nEndTime < _TIME_TS )
		return;
	//ë²„í”„ ì…‹íŒ…
	g_pCharMng->GetTamerUser()->GetBuff()->_SetEncyBuff( nOptIdx, nEndTime );
}
void cCliGame::RecvEncyclopediaGetBuffStat()
{
	n4 nMaxHP;		// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ HP
	u2 nAttSpeed;	// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ ê³µê²©ì†ë„
	pop( nMaxHP );
	pop( nAttSpeed );
	/*
	u2 nMaxDS;		// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ DS
	u2 nAtt;		// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ ê³µê²©ë ¥
	u2 nDef;		// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ ë°©ì–´ë ¥
	u2 nCritical;	// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ í¬ë¦¬í‹°ì»¬ í™•ë¥ 
	u2 nEv;			// ë²„í”„ë¡œ ì¸í•´ ë³€ê²½ëœ íšŒí”¼
	pop( nMaxDS );
	pop( nAtt );
	pop( nDef );
	pop( nCritical );
	pop( nEv );
	*/
	if( g_pCharMng->GetDigimonUser( 0 ) )
	{
		CDigimonUser::sUSER_STAT* pDigimonStat = (CDigimonUser::sUSER_STAT*)g_pCharMng->GetDigimonUser( 0 )->GetBaseStat();
		pDigimonStat->SetMaxHP( nMaxHP );
		pDigimonStat->SetAttackSpeed( nAttSpeed );
		/*
		pDigimonStat->SetMaxDS( nMaxDS );
		pDigimonStat->SetAtt( nAtt );
		pDigimonStat->SetDef( nDef );
		pDigimonStat->SetCritical( nCritical );
		pDigimonStat->SetEvade( nEv );
		*/
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_STAT_DIGIMONSTATUS, NULL );
	}	
}
#ifdef SELECT_SERVER_ERROR
void cCliGame::RecvSelectServer_Er()
{
	LOG( "RecvAccessCode_Fail_LOG_í¬íƒˆ" );
	Sleep( 2000 );
	SendAccessCode();
}
#endif
void cCliGame::RecvCapsuleEvol_Result()
{
	u2 nResult = 0;
	bool bSuccess = false;
	pop( nResult );
	switch( nResult )
	{
	case 1:	// ì„±ê³µ
		{
			cPrintMsg::PrintMsg( 30661 );	// ìº¡ìŠì§„í™” ìŠ¬ë¡¯ì´ í™•ìž¥ë˜ì—ˆìŠµë‹ˆë‹¤.
			bSuccess = true;
		}
		break;
	case 2:	// ì‹¤íŒ¨
		{
			cPrintMsg::PrintMsg( 30660 );	// ìº¡ìŠì§„í™” ìŠ¬ë¡¯ í™•ìž¥ì— ì‹¤íŒ¨í•˜ì˜€ìŠµë‹ˆë‹¤.
			bSuccess = false;
		}
		break;
	default:
		break;
	}
	//ì„±ê³µì—¬ë¶€ì— ë”°ë¥¸ ë™ìž‘
	g_pGameIF->GetCapsuleEvol()->_RecvResult(bSuccess);
}
void cCliGame::RecvSpirit_SpritToDigimon()
{
	u4	nDigimonID;		// ë¶€í™”ëœ ë””ì§€ëª¬ íƒ€ìž…
	n8	nMoney;			// ë‚¨ì€ ëˆ
	u1	nUseItemCnt;	// ì‚¬ìš©í•œ ì•„ì´í…œ ê°œìˆ˜
	u4	nItemID;		// ì‚¬ìš©í•œ ì•„ì´í…œ ì¢…ë¥˜
	pop( nDigimonID );
	pop( nMoney );
	g_pDataMng->GetInven()->SetMoney( nMoney, false );
	pop( nUseItemCnt );
	while( nUseItemCnt )
	{
		pop( nItemID );
		g_pDataMng->GetInven()->DecreaseItem( nItemID, nUseItemCnt, false, false );
		pop( nUseItemCnt );
	}
	//=============================================================================
	//	ì¸ë²¤ ë‚´ ì•„ì´í…œ ê°ì†Œ ë° ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
	//=============================================================================
	g_pTacticsAni->Set_bAniProcess(true);	// ë¶€í™” ì• ë‹ˆ ì‹œìž‘ìœ„ì¹˜ ì„¤ì •
	g_pTacticsAni->Start( nDigimonID, g_pCharMng->GetTamerUser() );	// ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_ITEM_TO_DIGIMON );
}
void cCliGame::RecvSpirit_DigimonToSpirit()
{
	u1	nDelSlot;		// ì‚­ì œ ë””ì§€ëª¬ ìŠ¬ë¡¯
	n8	nMoney;			// ë‚¨ì€ ëˆ
	u1	nUseItemCnt;	// ì‚¬ìš©í•œ ì•„ì´í…œ ê°œìˆ˜
	u4	nItemID;		// ì‚¬ìš©í•œ ì•„ì´í…œ ì¢…ë¥˜
	pop( nDelSlot );
	pop( nMoney );
	//cPrintMsg::PrintMsg( 20100, g_pDataMng->GetTactics()->GetTactics( nDelSlot-1 )->s_szName );
	g_pDataMng->GetTactics()->DeleteTactics( nDelSlot - 1 );
	g_pDataMng->GetInven()->SetMoney( nMoney, false );
	// ì†Œëª¨ëœ ì•„ì´í…œ 
	pop( nUseItemCnt );
	while( nUseItemCnt )
	{
		pop( nItemID );
		g_pDataMng->GetInven()->DecreaseItem( nItemID, nUseItemCnt, false, false );
		pop( nUseItemCnt );
	}
	// ì¦ê°€ëœ ì•„ì´í…œ
	pop( nUseItemCnt );
	while( nUseItemCnt )
	{
		pop( nItemID );
		g_pDataMng->GetInven()->ItemCrop( nItemID, nUseItemCnt );
		// [4/14/2016 hyun] ìˆ˜ì •. ì•„ì´í…œ ì»¬ëŸ¬ê°’ ì¶”ê°€
		CsItem::sINFO* pItem = nsCsFileTable::g_pItemMng->GetItem( nItemID )->GetInfo();
		if(pItem)
			cPrintMsg::PrintMsg( cPrintMsg::GAME_ITEM_CROP, pItem->s_szName, &g_pDataMng->GetItemColor(pItem) );
		// @@ ì—¬ê¸°ê¹Œì§€
		pop( nUseItemCnt );
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_DIGIMON_TO_ITEM );
}
void cCliGame::RecvSpirit_AncientSpiritEvolution()
{
	u4	nDigimonID;		// ë¶€í™”ëœ ë””ì§€ëª¬ íƒ€ìž…
	u1	nDelSlot_1;		// ì‚­ì œ ë””ì§€ëª¬ ìŠ¬ë¡¯ 1
	u1	nDelSlot_2;		// ì‚­ì œ ë””ì§€ëª¬ ìŠ¬ë¡¯ 2
	n8	nMoney;			// ë‚¨ì€ ëˆ
	u1	nUseItemCnt;	// ì‚¬ìš©í•œ ì•„ì´í…œ ê°œìˆ˜
	u4	nItemID;		// ì‚¬ìš©í•œ ì•„ì´í…œ ì¢…ë¥˜
	pop( nDigimonID );
	pop( nDelSlot_1 );
	pop( nDelSlot_2 );
	pop( nMoney );
	if( nDelSlot_1 > nDelSlot_2 )
	{
		g_pDataMng->GetTactics()->DeleteTactics( nDelSlot_1 - 1 );
		g_pDataMng->GetTactics()->DeleteTactics( nDelSlot_2 - 1 );
	}
	else
	{
		g_pDataMng->GetTactics()->DeleteTactics( nDelSlot_2 - 1 );
		g_pDataMng->GetTactics()->DeleteTactics( nDelSlot_1 - 1 );
	}
	g_pDataMng->GetInven()->SetMoney( nMoney, false );
	pop( nUseItemCnt );
	while( nUseItemCnt )
	{
		pop( nItemID );
		g_pDataMng->GetInven()->DecreaseItem( nItemID, nUseItemCnt, false, false );
		pop( nUseItemCnt );
	}
	//=============================================================================
	//	ì¸ë²¤ ë‚´ ì•„ì´í…œ ê°ì†Œ ë° ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
	//=============================================================================
	g_pTacticsAni->Set_bAniProcess(true);	// ë¶€í™” ì• ë‹ˆ ì‹œìž‘ìœ„ì¹˜ ì„¤ì •
	g_pTacticsAni->Start( nDigimonID, g_pCharMng->GetTamerUser() );	// ë¶€í™” ì• ë‹ˆë©”ì´ì…˜ í™œì„±í™”
}
void cCliGame::RecvActorObjectFactor_Init()
{
	// ë¬¸ ë‹¤ ì—´ë ¤ìžˆëŠ”ê²ƒ ì²˜ëŸ¼ ì´ˆê¸°í™”
	g_pDataMng->GetMapObject()->Init( true );
	// í•´ë‹¹ë˜ëŠ” Factor( ëª¬ìŠ¤í„°ë¼ë©´ ì‚´ì•„ìžˆëŠ” ëª¬ìŠ¤í„° ) ë“¤ë§Œ ë¬¸ ë‹«ê¸°
	u4 nCnt = 2;
	pop( nCnt );
	ST_CHAT_PROTOCOL	CProtocol;
	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
	//CProtocol.m_wStr = GetVAString( _T("ActorObject_Init - Count : %d"), nCnt );
	DmCS::StringFn::Format(CProtocol.m_wStr ,  _T("ActorObject_Init - Count : %d"), nCnt);
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
	for( u4 i = 0 ; i < nCnt ; i++ )
	{
		u4 nFactID = 0;
		pop( nFactID );		// 20150914 nFactID = MonsterID -> ObjectIDë¡œ ë³€ê²½
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString( _T("ActorObject_Init - MonsterID : %d"), nFactID );
		DmCS::StringFn::Format(CProtocol.m_wStr, _T("ActorObject_Init - MonsterID : %d"), nFactID);
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		
		g_pDataMng->GetMapObject()->OpenAndCloseObject( nFactID, false );
	}
	//g_pDataMng->GetMapObject()->CheckActorFlag();
}
void cCliGame::RecvActorObjectFactor()
{
	// ë¬¸ê³¼ ê´€ë ¨ëœ ëª¬ìŠ¤í„° ì£½ì„ ì‹œ // 20150914 nFactID = MonsterID -> ObjectIDë¡œ ë³€ê²½
	u4 nFactID = 0;
	bool bOpend = false;
	pop( nFactID );	
	pop( bOpend );
	ST_CHAT_PROTOCOL	CProtocol;
	CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
	//CProtocol.m_wStr = GetVAString(_T("ActorObject - MonsterID : %d"), nFactID);
	DmCS::StringFn::Format(CProtocol.m_wStr, _T("ActorObject - MonsterID : %d, Opend : %d"), nFactID, bOpend);
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
	// ëª¬ìŠ¤í„° ì•„ì´ë””ë¡œ ì˜¤ë¸Œì íŠ¸ í”Œëž˜ê·¸ ì—´ë¦¼ ì„¤ì •
	g_pDataMng->GetMapObject()->OpenAndCloseObject( nFactID, bOpend );
	//g_pDataMng->GetMapObject()->SetActorFlag( nFactID, true );
	//g_pDataMng->GetMapObject()->CheckActorFlag();
}
void cCliGame::RecvPinokimonInfoReq()
{
	u4 nResetTime;	// ë‹¤ìŒ ê°±ì‹ ì‹œê°„
	u4 nAllVote;	// ê° ì¹´ë“œì˜ ì „ì²´ íˆ¬í‘œìˆ˜
	u4 nMyVote;		// ê° ì¹´ë“œì— ìžì‹ ì´ íˆ¬í‘œí•œ ìˆ˜
	pop( nResetTime );
	g_pGameIF->m_sEventInfo.s_nNextResetTime = nResetTime;
	for( int i = 0 ; i < MAX_CARD_COUNT ; i++ )
	{
		pop( nAllVote );
		g_pGameIF->m_sEventInfo.s_nAllCnt[i] = nAllVote;
	}
	for( int i = 0 ; i < MAX_CARD_COUNT ; i++ )
	{
		pop( nMyVote );
		g_pGameIF->m_sEventInfo.s_nMyCnt[i] = nMyVote;
	}
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_EVENT_PINOKIMON) == false )
	{
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString( _T( "í”¼ë…¸í‚¤ëª¬ ì´ë²¤íŠ¸ ìœˆë„ìš° ìƒì„± ì‹¤íŒ¨") );
		DmCS::StringFn::Format(CProtocol.m_wStr, _T( "í”¼ë…¸í‚¤ëª¬ ì´ë²¤íŠ¸ ìœˆë„ìš° ìƒì„± ì‹¤íŒ¨"));
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		return;
	}
	g_pGameIF->GetEvent_Pinokimon()->RefreshList();
}
void cCliGame::RecvPinokimonVote()
{
	u2 nInvenSlotCnt;	// ì¸ë²¤ ìŠ¬ë¡¯ ì¹´ìš´íŠ¸
	pop( nInvenSlotCnt );
	u2 nIdx;			// ì¸ë²¤ ìŠ¬ë¡¯ ì¸ë±ìŠ¤
	u4 nItemCnt;		// ì•„ì´í…œ ê°ì†Œ ëŸ‰
	for( int i = 0 ; i < nInvenSlotCnt ; i++ )
	{
		pop( nIdx );
		pop( nItemCnt );
		// ì¸ë²¤ì—ì„œ ì•„ì´í…œ ì‚­ì œ
		g_pDataMng->GetInven()->GetData( nIdx )->DecreaseCount( nItemCnt );
	}
	// ì„œë²„ì‘ë‹µ ëŒ€ê¸° í•´ì œ
	cMessageBox::DelMsg( 10019, false );
}
void cCliGame::RecvRemoveVoteItem()
{
	// íˆ¬í‘œ ì°½ ë„ì›Œë†“ì€ ìƒíƒœë©´ ë‹«ì•„ì£¼ìž
	if( g_pSeparate )
	{
		if( g_pSeparate->GetTargetWindowType() == cBaseWindow::WT_EVENT_PINOKIMON )
		{
			g_pSeparate->OnCancel();
		}
	}
	u4 nNpcID;
	pop( nNpcID );
	// ì¸ë²¤ & ì°½ê³  & ì§€ê¸‰ì°½ê³  ì•„ì´í…œ ì‚­ì œ
	u2 nCnt;		// ìŠ¬ë¡¯ ì¹´ìš´íŠ¸
	u2 nIdx;		// ìŠ¬ë¡¯ ì¸ë±ìŠ¤
	u4 nDecrease;	// ì§€ì›Œì•¼ í•  ì•„ì´í…œ ê°¯ìˆ˜
	// 1. ì¸ë²¤
	pop( nCnt );
	for( int i = 0 ; i < nCnt ; i++ )
	{
		pop( nIdx );
		pop( nDecrease );
		if( g_pDataMng->GetInven() && nIdx < nLimit::Inven )
		{
			g_pDataMng->GetInven()->GetData( nIdx )->DecreaseCount( nDecrease );
		}
	}
	// 2. ì°½ê³ 
	pop( nCnt );
	list<WareHouseContents::sWAREHOUSE> listNormalWareHouse;
	WareHouseContents::sWAREHOUSE stTemp;
	for( int i = 0 ; i < nCnt ; i++ )
	{
		pop( nIdx );
		pop( nDecrease );
		if( nIdx < nsCsFileTable::g_pBaseMng->GetLimit()->s_nMaxWareHouse )
		{
			stTemp.Reset();
			stTemp.nIdx = nIdx;
			stTemp.nDecreaseCount = nDecrease;
			
			listNormalWareHouse.push_back(stTemp);
 
//			g_pDataMng->GetWarehouse()->GetData( nIdx )->DecreaseCount( nDecrease );
		}
	}
	GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_NORMAL_DATA_DECREASE, &listNormalWareHouse );
#ifdef NEW_SHARESTASH	// ê³„ì • ê³µìœ  ì°½ê³ ëŠ” êµ­ë‚´ì—ë§Œ ì ìš©ëœ ì»¨í…ì¸ ìž„.
	// 3. ê³µìœ ì°½ê³ 
	list<WareHouseContents::sWAREHOUSE> listSharelWareHouse;
	pop( nCnt );
	for( int i = 0 ; i < nCnt ; i++ )
	{
		pop( nIdx );
		pop( nDecrease );
		if( nIdx < nsCsFileTable::g_pBaseMng->GetLimit()->s_nMaxShareStash )
		{
			stTemp.Reset();
			stTemp.nIdx = nIdx;
			stTemp.nDecreaseCount = nDecrease;
			listSharelWareHouse.push_back(stTemp);
			/*
			int iData[2] = {0, 0};
			iData[0] = nIdx;
			iData[1] = nDecrease;
			GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_NORMAL_DATA_DECREASE, iData);*/
//			g_pDataMng->GetWarehouse()->GetShareData( nIdx )->DecreaseCount( nDecrease );
		}
	}
	GAME_EVENT_ST.OnEvent(EVENT_CODE::WAREHOUSE_SHARE_DATA_DECREASE, &listSharelWareHouse );
#endif
	// 4. ì§€ê¸‰ì°½ê³  ( í´ë¼ì²˜ë¦¬ )
	CsNpc* pNpc = nsCsFileTable::g_pNpcMng->GetNpc( nNpcID );
	if( pNpc == NULL )
		return;
	sNPC_TYPE_SPECIAL_EVENT* pNpcType = (sNPC_TYPE_SPECIAL_EVENT*)pNpc->GetType();
	for( int i = 0 ; i < pNpcType->GetItemCount() ; i++ )
	{
		if( pNpcType->GetItem( i ) == NULL )
			continue;
 		// íˆ¬í‘œ ì•„ì´í…œ
		int nVoteItemID = pNpcType->GetItem( i )->GetInfo()->s_dwItemID;
		// ì•„ì´í…œ ì‚­ì œ ì•Œë¦¼ ë©”ì„¸ì§€
		cPrintMsg::PrintMsg( 20029, nsCsFileTable::g_pItemMng->GetItem( nVoteItemID )->GetInfo()->s_szName );
		cMessageBox::GetFirstMessageBox()->SetValue1( 1 );					// íƒ€ìž… : ì•„ì´í…œ ì œê±°
		cMessageBox::GetFirstMessageBox()->SetValue2( nItem::TabGiftShop );	// ì‚­ì œ ìœ„ì¹˜
		cMessageBox::GetFirstMessageBox()->SetValue3( nVoteItemID );		// ì‚­ì œ ì•„ì´í…œ
	}
	// ì¸ë²¤ ë½ í•´ì œ
	for( int j = 0 ; j < pNpcType->GetItemCount() ; j++ )
	{
		int nItemID = pNpcType->GetItemID( j );
		g_pDataMng->GetInven()->ItemUnlock_ItemID( nItemID );
	}
}
void cCliGame::RecvDigimonDataChange()
{
	n4 nDataChangeType = nDigimon::eDataChangeType_None;	
	n4 nResult = 0;
	u1 nLDigimonArrIDX = 0;
	u1 nRDigimonArrIDX = 0;
	pop( nDataChangeType );
	pop( nLDigimonArrIDX );
	pop( nRDigimonArrIDX );
	pop( nResult );
	if ( nResult == MESSAGE_COMPLETE )		// êµí™˜ ì„±ê³µ
	{
		switch ( nDataChangeType )
		{
		case nDigimon::eDataChangeType_Size:
			{
				u1 nLHatchLevel = 0;
				u2 nLNewScale = 0;				
				pop( nLHatchLevel );
				pop( nLNewScale );
				u1 nRHatchLevel = 0;
				u2 nRNewScale = 0;
				pop( nRHatchLevel );
				pop( nRNewScale );
				// ë¶€í™” ë‹¨ìˆ˜ ë ˆë²¨ ê°’ë„ ì–»ì–´ ì˜¤ì§€ë§Œ í´ë¼ì—ì„œëŠ” ë¯¸ì‚¬ìš©.
				ContentsStream kStream;
				kStream<<nLDigimonArrIDX<<nRDigimonArrIDX<<nLNewScale<<nRNewScale<<nLHatchLevel<<nRHatchLevel;
				GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_TRADE_DATA_SCALE, &kStream );
			}
			break;
		case nDigimon::eDataChangeType_Inchant:
			{
				u2 nLEnchantLevel = 0;
				u2 nLExtendAttribute[MaxExtendStat] = {0,};
				u2 nLExtendAttributeLV[MaxExtendStat] = {0,};
				pop( nLEnchantLevel );
				pop( nLExtendAttribute, sizeof(nLExtendAttribute) );
				pop( nLExtendAttributeLV, sizeof(nLExtendAttributeLV) );
				u2 nREnchantLevel = 0;
				u2 nRExtendAttribute[MaxExtendStat] = {0,};
				u2 nRExtendAttributeLV[MaxExtendStat] = {0,};
				pop( nREnchantLevel );
				pop( nRExtendAttribute, sizeof(nRExtendAttribute) );
				pop( nRExtendAttributeLV, sizeof(nRExtendAttributeLV) );
				ContentsStream kStream;
				kStream<<nLDigimonArrIDX<<nRDigimonArrIDX<<nLEnchantLevel<<nLExtendAttribute<<nLExtendAttributeLV
					<<nREnchantLevel<<nRExtendAttribute<<nRExtendAttributeLV;
				GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_TRADE_DATA_UPGRADE, &kStream );
			}
			break;
		case nDigimon::eDataChangeType_EvoSlot:
			{
				u1 nLMaxEvoUnit = 0;
				cEvoUnit kLEvoUnit[nLimit::EvoUnit];
				pop( nLMaxEvoUnit );
				pop( &kLEvoUnit[1], sizeof(cEvoUnit)*nLMaxEvoUnit );
				u1 nRMaxEvoUnit = 0;
				cEvoUnit kREvoUnit[nLimit::EvoUnit];
				pop( nRMaxEvoUnit );
				pop( &kREvoUnit[1], sizeof(cEvoUnit)*nRMaxEvoUnit );
				ContentsStream kStream;
				kStream<<nLDigimonArrIDX<<nRDigimonArrIDX<<nLMaxEvoUnit<<kLEvoUnit<<nRMaxEvoUnit<<kREvoUnit;
				GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_TRADE_DATA_EVOLUTION, &kStream );
			}
			break;
		}
	}
	else
	{
		GAME_EVENT_ST.OnEvent(EVENT_CODE::RECV_TRADE_DATA_FAILURE, &nResult);
	}
}
void cCliGame::RecvErrorMessage()
{
	n4 nResult = -1;
	pop( nResult );
	switch ( nResult )
	{
	case nErrorMessage::eErrorMessage_Make_CountOver:		// ì œìž‘ ìš”ì²­ ê°œìˆ˜ ì˜¤ë¥˜
		break;
	case nErrorMessage::eErrorMessage_Make_ItemCountOver:	// ì œìž‘ ì•„ì´í…œ ìš”ì²­ ê°œìˆ˜ ì˜¤ë¥˜
		break;
	case nErrorMessage::eErrorMessage_Make_NoneResource:	// ì œìž‘ í…Œì´ë¸” ë¦¬ì†ŒìŠ¤ ì—†ìŒ
		break;
	case nErrorMessage::eErrorMessage_Item_NoneResource:	// ì•„ì´í…œ í…Œì´ë¸” ë¦¬ì†ŒìŠ¤ ì—†ìŒ
		break;
	case nErrorMessage::eErrorMessage_Make_NotEnoughInvenSlot:	// ì œìž‘ ì¸ë²¤ ê³µê°„ ë¶€ì¡±
		cPrintMsg::PrintMsg( 11015 );
		break;
	case nErrorMessage::eErrorMessage_Make_NotEnoughMoney:		// ì œìž‘ ê¸ˆì•¡ ë¶€ì¡±
		break;
	case nErrorMessage::eErrorMessage_Make_NotEnoughMaterialItem:	// ì œìž‘ ìž¬ë£Œ ë¶€ì¡±
		break;
	case nErrorMessage::eErrorMessage_Repurchase_NotEnoughInvenSlot:	// ìž¬êµ¬ë§¤ ì¸ë²¤ ë¶€ì¡±
		break;
	}
}
void cCliGame::AttackProcesses(	u4 nHitterUID, u4 nTargetUID, n4 nDamage, u4 nBattleOption, n4 hp, n4 Mhp, bool bDie)
{
	CsC_AvObject* pTarget = g_pMngCollector->GetObject( nTargetUID );
	if( pTarget == NULL )
		return;
	sRECV_HITTER_INFO hitter;
	hitter.s_nUID = nHitterUID;	
	hitter.s_HitType = nBattleOption & pBattle::BO_CRITICAL ? CsC_AttackProp::HT_Critical : CsC_AttackProp::HT_Normal;
	// ë°ë¯¸ì§€ ë°˜ì‚¬ ë²„í”„ ìŠ¤í‚¬ ì¶”ê°€_14.05.14		chu8820
	//íƒ€ê²Ÿì—ê²Œ ë°ë¯¸ì§€ë°˜ì‚¬ ë²„í”„ê°€ ìžˆëŠ”ì§€ í™•ì¸
	std::list< cBuffData::sBUFF_DATA >* pBuffList = NULL;
	if( pTarget->GetLeafRTTI() == RTTI_DIGIMON )
		pBuffList = ((CDigimon*)pTarget)->GetBuff()->GetBuff();
	else if( pTarget->GetLeafRTTI() == RTTI_DIGIMON_USER )
		pBuffList = g_pCharMng->GetDigimonUser(0)->GetBuff()->GetBuff();
	if( pBuffList != NULL )
	{
		std::list< cBuffData::sBUFF_DATA >::iterator it = pBuffList->begin();
		std::list< cBuffData::sBUFF_DATA >::iterator itEnd = pBuffList->end();
		for(;it != itEnd;it++)
		{
			u4 nSkillCode = (*it).s_pBuffTable->GetInfo()->s_dwSkillCode;
			if( nSkillCode == 0 )
				continue;
			//ìŠ¤í‚¬ì½”ë“œë¡œ ìŠ¤í‚¬ ì¢…ë¥˜ í™•ì¸
			CsSkill* pSkill = nsCsFileTable::g_pSkillMng->GetSkill( nSkillCode );
			if( pSkill == NULL )
			{
				ST_CHAT_PROTOCOL	CProtocol;
				CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
				//CProtocol.m_wStr = GetVAString( _T("ìŠ¤í‚¬ ëª»ì°¾ìŒ( SkillCode - %d )"), nSkillCode );
				DmCS::StringFn::Format(CProtocol.m_wStr, _T("ìŠ¤í‚¬ ëª»ì°¾ìŒ( SkillCode - %d )"), nSkillCode); 
				GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
				continue;
			}
			for( int i = 0 ; i < SKILL_APPLY_MAX_COUNT ; i++ )
			{
				//ìŠ¤í‚¬ íš¨ê³¼ë¥¼ ë³´ê³  ë°˜ì‚¬ ìŠ¤í‚¬ì¸ì§€ í™•ì¸
				USHORT nApplyType = pSkill->GetInfo()->s_Apply[ i ].s_nA;
				switch( nApplyType )
				{
				case APPLY_DR:	hitter.s_nReflectType = (*it).s_pBuffTable->GetInfo()->s_nBuffClass;	break;	// ë°ë¯¸ì§€ ë°˜ì‚¬ ë²„í”„
				case APPLY_AB:	// ë°ë¹„ì§€ í¡ìˆ˜ ë²„í”„
					if( nDamage == 0 ) 
					{ 
						hitter.s_nSkillIndex = (*it).s_pBuffTable->GetInfo()->s_dwID;//pSkill->GetInfo()->s_Apply[ i ].s_nBuffCode;
						hitter.s_HitType = CsC_AttackProp::HT_Absorb; 
					}	
					break;
				}
			}
		}
	}
	sRECV_TARGET_INFO target;
	target.s_nUID = nTargetUID;
	target.s_DamageType = nBattleOption & pBattle::BO_BLOCK ? CsC_AttackProp::DT_Block : CsC_AttackProp::DT_Normal;
	target.s_nNumEffectType = nBattleOption & pBattle::BO_CRITICAL ? NUMTYPE::ET_CRITICAL : NUMTYPE::ET_NORMAL;
	target.s_nValue[ 0 ] = -nDamage;
	if( bDie == false )
	{		
		switch( pTarget->GetLeafRTTI() )
		{
		case RTTI_TAMER:
		case RTTI_TAMER_USER:
			assert_cs( false );
			break;
		case RTTI_TUTORIAL_MONSTER:
		case RTTI_DIGIMON:
		case RTTI_MONSTER:
			{
				__int64 changeHp = (__int64)hp * 255;
				int nHpRate = changeHp / Mhp;
				if( nHpRate < 1 )
				{
					nHpRate = 1;
				}
				target.s_nValue[ 1 ] = nHpRate - pTarget->GetBaseStat()->GetBackupHP();
				pTarget->GetBaseStat()->SetBackupHP( nHpRate );
			}
			break;
		case RTTI_DIGIMON_USER:
			{
				pTarget->GetBaseStat()->SetHP( hp );
				pTarget->GetBaseStat()->SetBackupHP( hp );
			}
			break;
		default:
			assert_cs( false );
		}
		SetAttackInfo( &hitter, &target, false );
	}
	else
	{
		target.s_nValue[ 1 ] = -pTarget->GetBaseStat()->GetHP();
		pTarget->SetAniPause( false );
		SetAttackInfo( &hitter, &target, true );
		ST_CHAT_PROTOCOL	CProtocol;
		CProtocol.m_Type = NS_CHAT::DEBUG_TEXT;
		//CProtocol.m_wStr = GetVAString( _T("RecvAttack SetDieInfo UID : %d"), pTarget->GetUniqID() );
		DmCS::StringFn::Format(CProtocol.m_wStr, _T("RecvAttack SetDieInfo UID : %d"), pTarget->GetUniqID());
		GAME_EVENT_STPTR->OnEvent( EVENT_CODE::EVENT_CHAT_PROCESS, &CProtocol );
		SetDieInfo( pTarget );
	}
#ifdef DAMAGE_METER
	sDamageMeterInfo damageInfo(nHitterUID, nTargetUID, nDamage);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::ADD_DAMAGEMETER_DATA, &damageInfo );
#endif
}
//////////////////////////////////////////////////////////////////////////
// ë§¤íŠ¸ë¡œ í”„ë¡œí…íŠ¸ ê´€ë ¨ í•¨ìˆ˜
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ë‚´ê°€ ë‹¤ë¥¸ í…Œì´ë¨¸ì—ê²Œ ë§¤í¬ë¡œ í”„ë¡œí…íŠ¸ ì•„ì´í…œ ì‚¬ìš© ìš”ì²­ íŒ¨í‚·
//////////////////////////////////////////////////////////////////////////
void cCliGame::SendMacroQuestion(std::wstring const& szTamerName)
{
	if( szTamerName.empty() )
		return;
	C2GS_SEND_MACRO_QUESTION packet;
	packet.m_TargetName = szTamerName;
	newp(packet.GetPacketID());
	push(packet.m_TargetName);
	endp(packet.GetPacketID());
	send();
}
// ë§¤í¬ë¡œ í”„ë¡œí…íŠ¸ ê²°ê³¼ê°’ ì„œë²„ì— ì „ì†¡
#ifdef SDM_MACRO_PROTECT_RENEW_20181217
void cCliGame::SendMacroAnswer( std::wstring const& wsAnswer)
{
	C2GS_SEND_MACRO_ANSWER packet;
	DmCS::StringFn::ToMB( wsAnswer, packet.m_szAnswer);
	newp( packet.GetPacketID() );
	push(packet.m_szAnswer);
	endp( packet.GetPacketID() );
	send();
}
#else
void cCliGame::SendMacroAnswer( n4 const& nRandNo, n4 const& nAnswer )
{
	C2GS_SEND_MACRO_ANSWER packet;
	packet.m_nRandNo = nRandNo;
	packet.m_nAnswer = nAnswer;
	newp( packet.GetPacketID() );
	push(packet.m_nRandNo);
	push(packet.m_nAnswer);
	endp( packet.GetPacketID() );
	send();
}
#endif
// ëŒ€ìƒì´ ë§¤í¬ë¡œì¸ì§€ ì•„ë‹Œì§€ ê²°ê³¼ê°’ ë°›ìŒ.
void cCliGame::RecvMacroResult(void)
{
	GS2C_RECV_MACRO_PLAYER_RESULT recv;
	pop(recv.m_nResult);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MACRO_PLAYER_RESULT, &recv );
}
// ë‚˜í•œí…Œ ë§¤í¬ë¡œ í”„ë¡œí…íŠ¸ ì§ˆë¬¸ì´ ì™”ë‹¤.
void cCliGame::RecvMacroQuestion(void)
{
	GS2C_NTF_MACRO_QUESTION_REQUEST recv;
#ifdef SDM_MACRO_PROTECT_RENEW_20181217
	pop(recv.m_nTimeMax);
	pop(recv.m_nTimeLeft);	
	pop(recv.m_nChanceLeft);
	pop(recv.m_nSize);
	if( recv.m_nSize > 0 )
	{
		recv.m_pData = new char[recv.m_nSize];
		pop( recv.m_pData, recv.m_nSize );
	}
	
	#ifdef SDM_MACRO_PROTECT_QUESTION_20190524
		pop( recv.m_nQuestionType );
		pop( recv.m_nQuestion );
	#endif
#else
	pop(recv.m_nRandNum);
	pop(recv.m_nHint);
#endif
	GAME_EVENT_ST.OnEvent( EVENT_CODE::NTF_MACROPROTECT_QUESTION_REQUEST, &recv );
}
void cCliGame::RecvMacroSuccess(void)
{
	C2GS_RECV_MACRO_QUESTION_RESULT recv;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MACROPROTECT_QUESTION_RESULT, &recv );
}
// ë‚´ê°€ ë§¤í¬ë¡œ í”„ë¡œí…íŠ¸ ìš”ì²­í•œ ê²ƒì— ëŒ€í•œ ê²°ê³¼ê°’
void cCliGame::RecvMacroFailure(void)
{
	C2GS_RECV_MACRO_QUESTION_RESULT recv;	
	pop(recv.m_nResult);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_MACROPROTECT_QUESTION_RESULT, &recv );
 }
//////////////////////////////////////////////////////////////////////////
//	ë””ì§€ëª¬ ì•„ë ˆë‚˜ ê´€ë ¨ í•¨ìˆ˜
//////////////////////////////////////////////////////////////////////////
// ì•„ë ˆë‚˜ ëž­í‚¹ ë¦¬ìŠ¤íŠ¸ íŒ¨í‚·
void cCliGame::Recv_ArenaRequestRank(void)
{
	GS2C_RECV_ARENA_REQUEST_RANK recv;
	pop(recv.nType);				//ìš”ì²­í•œ ëž­í‚¹íƒ€ìž…(eRankType)
	pop(recv.nResult);				//ê²°ê³¼
	pop(recv.nRankStatus);			//ëž­í‚¹ ìƒí™©(eRankStatus)	
	pop(recv.nPoint);				//ìžì‹ ì˜ í¬ì¸íŠ¸ - ë„£ì€ì •ë³´ê°€ì—†ìœ¼ë©´ 0
	pop(recv.nRank);				//ìžì‹ ì˜ ì ˆëŒ€ ë“±ìˆ˜
	pop(recv.nModifier);			//ìˆœìœ„ ë³€ë™ìˆ˜ì¹˜
	pop(recv.nRankType);			//ìžì‹ ì˜ ëž­í¬ìœ„ì¹˜ ë¶„ë¥˜(0 ì ˆëŒ€ê°’ , 1ìƒëŒ€ê°’(%))
	pop(recv.nRankPercent);			//ìžì‹ ì˜ ëž­í¬ìœ„ì¹˜
	pop(recv.nRankRenewSec);		//ë‹¤ìŒ ëž­í‚¹ ê°±ì‹  ì‹œê°„
	pop(recv.nRankEndSec);			//í˜„ ëž­í‚¹ ë§ˆê° ì‹œê°„
	u1 nRankSize = 0;
	pop(nRankSize);			//Top100 ëž­í‚¹ ê°¯ìˆ˜(ìµœëŒ€ 100)
	for( int n = 0 ; n < nRankSize; ++n )
	{
		nArenaRank::st_TopRankInfo addRankData;
		pop( addRankData.nRank );//ë“±ìˆ˜
		pop( addRankData.szName );//ì´ë¦„
		pop( addRankData.szGuild );//ê¸¸ë“œ
		pop( addRankData.nPoint );//í¬ì¸íŠ¸
		pop( addRankData.nIsNew );//ì‹ ê·œì§„ìž…ì¸ê°€
		pop( addRankData.nModifier );//ë“±ìˆ˜ ë³€ë™ ìˆ˜ì¹˜
		recv.lRankList.push_back( addRankData );
	}
	BHPRT( "Recv - GS2C_RECV_ARENA_REQUEST_RANK : Size(%d)", nRankSize );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INFINITEWAR_NOTICEBOARD_RANKER_LIST, &recv );
}
// ì•„ë ˆë‚˜ ì´ì „ ëž­í‚¹ ë¦¬ìŠ¤íŠ¸ íŒ¨í‚·
void cCliGame::Recv_ArenaRequestOldRank(void)
{
	GS2C_RECV_ARENA_REQUEST_OLD_RANK recv;
	pop(recv.nType);				//ìš”ì²­í•œ ëž­í‚¹íƒ€ìž…(eRankType)
	pop(recv.nResult);				//ê²°ê³¼
	pop(recv.nRank);				//ìžì‹ ì˜ ì ˆëŒ€ ë“±ìˆ˜ , ìžì‹ ì´ ì°¸ì—¬ì•ˆí•´ì„œ ì •ë³´ê°€ì—†ìœ¼ë©´ 0
	u1 nRankSize = 0;
	pop(nRankSize);			//Top100 ëž­í‚¹ ê°¯ìˆ˜(ìµœëŒ€ 100)
	for( int n = 0 ; n < nRankSize; ++n )
	{
		nArenaRank::st_TopRankOldInfo addRankData;
		pop( addRankData.nRank );//ë“±ìˆ˜
		pop( addRankData.szName );//ì´ë¦„
		pop( addRankData.szGuild );//ê¸¸ë“œ
		pop( addRankData.nPoint );//í¬ì¸íŠ¸
		recv.lRankList.push_back( addRankData );
	}
	BHPRT( "Recv - GS2C_RECV_ARENA_REQUEST_OLD_RANK : Size(%d)", nRankSize );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INFINITEWAR_NOTICEBOARD_PRE_RANKER_LIST, &recv );
}
// ì•„ë ˆë‚˜ ëª…ì˜ˆì˜ ì „ë‹¹ ë¦¬ìŠ¤íŠ¸ íŒ¨í‚·
void cCliGame::Recv_ArenaRequestFame(void)
{
	GS2C_RECV_ARENA_REQUEST_FAME recv;
	pop(recv.nSeason);				//ìš”ì²­í•œ ì‹œì¦Œ ë²ˆí˜¸ , 0ì„ë„£ì—ˆì„ê²½ìš° ìµœì‹ ì‹œì¦Œ
	pop(recv.nResult);				//ê²°ê³¼
	u1 nRankSize = 0;
	pop(nRankSize);			//Top50 ëž­í‚¹ ê°¯ìˆ˜(ìµœëŒ€ 50)
	for( int n = 0 ; n < nRankSize; ++n )
	{
		nArenaRank::st_TopRankFameInfo addRankData;
		pop( addRankData.nRank );//ë“±ìˆ˜
		pop( addRankData.szName );//ì´ë¦„
		pop( addRankData.szGuild );//ê¸¸ë“œ
		pop( addRankData.nPoint );//í¬ì¸íŠ¸
		recv.lRankList.push_back( addRankData );
	}
	BHPRT( "Recv - GS2C_RECV_ARENA_REQUEST_FAME : Size(%d)", nRankSize );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INFINITEWAR_NOTICEBOARD_FAME_LIST, &recv );
}
void cCliGame::Recv_ArenaRequestFameSeasonList()
{
	u2 nSeasonSize = 0;
	pop(nSeasonSize);//
	GS2C_RECV_ARENA_REQUEST_FAME_LIST recv;
	for( int n = 0 ; n < nSeasonSize; ++n )
	{
		u2 nSeasonIdx = 0;
		pop( nSeasonIdx );
		recv.lSeasonList.push_back( nSeasonIdx );
	}
	BHPRT( "Recv - GS2C_RECV_ARENA_REQUEST_FAME_LIST : Size(%d)", nSeasonSize );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INFINITEWAR_SEASON_FAME_LIST, &recv );
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void cCliGame::Recv_XGuageUpdate()
{
	GS2C_RECV_XGuage_Update recv;
	pop( recv.nXGuage );
	pop( recv.nCrystal );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::Recv_Ntf_XGuage_Update, &recv );
}
void cCliGame::Recv_XGuageMaxUpdate()
{
	GS2C_RECV_XGuage_Max_Update recv;
	pop( recv.nXGuageMax );
	pop( recv.nCrystalMax );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::Recv_Ntf_XGuage_Max_Update, &recv );
}
void cCliGame::Recv_XGToCrystal()
{
	GS2C_RECV_XGuage_to_Crystal recv;
	pop( recv.nResult );// ì„±ê³µ ì‹¤íŒ¨ë§Œ ë°›ê³  ì‹¤ì œ ê²Œì´ì§€ ë³€ê²½ì€ Recv_XGuageUpdatedì—ì„œ í•¨.
	GAME_EVENT_ST.OnEvent( EVENT_CODE::Recv_XGToCrystal_Result, &recv );
}
void cCliGame::Recv_CrystalToXG()
{
	GS2C_RECV_Crystal_to_XGuage recv;
	pop( recv.nResult );// ì„±ê³µ ì‹¤íŒ¨ë§Œ ë°›ê³  ì‹¤ì œ ê²Œì´ì§€ ë³€ê²½ì€ Recv_XGuageUpdatedì—ì„œ í•¨.
	GAME_EVENT_ST.OnEvent( EVENT_CODE::Recv_CrystalToXG_Result, &recv );
}
void cCliGame::Send_XGToCrystal()
{
	C2GS_SEND_XGuage_to_Crystal packet;
	newp( packet.GetProtocol() );
	endp( packet.GetProtocol() );
	send();
}
void cCliGame::Send_CrystalToXG()
{
	C2GS_SEND_Crystal_to_XGuage packet;
	newp( packet.GetProtocol() );
	endp( packet.GetProtocol() );
	send();
}
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvVipMemberShipData()
{
#ifdef SDM_VIP_SYSTEM_20181105
	GS2C_RECV_VIP_SYSTEM_DATA recv;
	pop( recv.bIsVip );
	pop( recv.unVipEndTime );	
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_VIP_SYSTEM_DATA, &recv );
#endif
}
//////////////////////////////////////////////////////////////////////////
void cCliGame::NTF_NeonDisplay()
{
	GS2C_NOTIFY_Neon_Display recv;
	pop( recv.nNeonType );
	pop( recv.strTamerName );
	pop( recv.nType1 );
	pop( recv.nType2 );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::NTF_NEON_DISPLAY, &recv );
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void cCliGame::Recv_NProtectCheck()
{
#ifdef DEF_CORE_NPROTECT
	BYTE bPacket[4096] = {0,};
	DWORD dwPacketSize = 0;
	UINT unwUniqueIdx = 0;
	pop( unwUniqueIdx );
	pop( bPacket );
	pop( dwPacketSize );
	CSAuth3( unwUniqueIdx, (PBYTE)bPacket, dwPacketSize, 0 );
#endif
}
void cCliGame::Recv_XignCodeCheck(void)
{
#ifdef SDM_DEF_XIGNCODE3_20181107
	GS2C_RECV_XignCodePacket recv;
	pop( recv.szPacketData, XIGNCODE_PACKET_SIZE );
	pop( recv.nSize );
	XignCode_ProbeEx( recv.szPacketData, recv.nSize, NULL );
#endif
}
void cCliGame::RecvItemSort()
{
	GS2C_RECV_ITEMSORT recv;
	pop( recv.m_u1SortFlag );
	pop( recv.m_nResult );
	pop( recv.m_u2ItemCount );
	// countê°€ 0ì´ë©´ ì •ë ¬í•  ì•„ì´í…œì´ ì—†ì–´ì„œ ì •ë ¬ ì‹¤íŒ¨
	for( int i = 0; i < recv.m_u2ItemCount; ++i )
	{
		cItemData itemData;
		pop(itemData);
		recv.m_lItems.push_back( itemData );
	}
	if( nItemSort::eINVEN == recv.m_u1SortFlag )
		GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INVEN_SORT_END, &recv );
	else if( nItemSort::eWAREHOUSE == recv.m_u1SortFlag || nItemSort::eShareStash == recv.m_u1SortFlag )
		GAME_EVENT_ST.OnEvent( EVENT_CODE::WAREHOUSE_RECV_ITEMSORT_END, &recv );
}
void cCliGame::RecvInvenResult()
{
	GS2C_RECV_INVENTORY recv;
	pop( recv.m_nResult );
	pop( recv.m_n8Money );
	pop( recv.m_u1InventoryFlag );
	pop( recv.m_u2InventorySlot );
	for( int i = 0; i < recv.m_u2InventorySlot; ++i )
	{
		cItemData itemData;
		pop( itemData );
		recv.m_lItems.push_back( itemData );
	}
	if( nInventoryInfo::eINVEN == recv.m_u1InventoryFlag )
		GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_INVEN_REQ_RESULT, &recv );
	else if( nInventoryInfo::eWAREHOUSE == recv.m_u1InventoryFlag || nInventoryInfo::eSHARESTASH == recv.m_u1InventoryFlag )
		GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_WAREHOUSE_REQ_RESULT, &recv );
	else if( recv.m_u1InventoryFlag >= nInventoryInfo::eEXTRA_SEAL && recv.m_u1InventoryFlag <= nInventoryInfo::eEXTRA_MATERIAL )
	{
		ExtraInventoryDebugLog( "[ExtraInventory][Network] RecvInvenResult extra flag=%d slotCount=%d itemRows=%d money=%lld result=%d",
			recv.m_u1InventoryFlag, recv.m_u2InventorySlot, (int)recv.m_lItems.size(), recv.m_n8Money, recv.m_nResult );
		SAFE_POINTER_RET( g_pDataMng );
		cData_Inven* pExtraInven = g_pDataMng->GetExtraInven( recv.m_u1InventoryFlag - nInventoryInfo::eEXTRA_SEAL );
		SAFE_POINTER_RET( pExtraInven );
		pExtraInven->SetMoney( recv.m_n8Money, false );
		pExtraInven->SetInvenSlotCount( recv.m_u2InventorySlot );
		pExtraInven->ResetAllItem();
		std::list<cItemData>::iterator it = recv.m_lItems.begin();
		std::list<cItemData>::iterator itEnd = recv.m_lItems.end();
		for( int nIndex = 0; it != itEnd; ++it, ++nIndex )
		{
			cItemInfo* pItemInfo = pExtraInven->GetData( nIndex );
			if( pItemInfo )
				*pItemInfo = *it;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// ì´ˆì›” ê´€ë ¨ ì½”ë“œ 
void cCliGame::RecvDigimonTranscendenceChargeExp()
{
	GS2C_RECV_DigimonTranscendenceChargeEXP recv;
	pop( recv.m_nResult );
	if( nsDigimonTranscendenceResult::Success == recv.m_nResult )
	{
		pop( recv.m_u1Flag );	// ì„ íƒ (ì¼ë°˜ ì¶©ì „, ê³ ê¸‰ ì¶©ì „)
		pop( recv.m_u1DigimonTranscendencePos );	// ê²½í—˜ì¹˜ íšë“í•œ ë””ì§€ëª¬ ìŠ¬ë¡¯ ë²ˆí˜¸
		u2   u2DigimonCount;
		pop( u2DigimonCount );
		for( int n = 0; n < u2DigimonCount; ++n )
		{
			u2 delDigimonArchiveSlotIdx = 0;
			pop( delDigimonArchiveSlotIdx );
			recv.m_listDigimonPos.push_back( delDigimonArchiveSlotIdx );
		}
		u2   u2DelItemCount;
		pop( u2DelItemCount );
		for( int n = 0; n < u2DelItemCount; ++n )
		{
			nsItemDel::sItemInfo delItem;
			pop( delItem );
			recv.m_lItemList.push_back( delItem );
		}
		pop( recv.m_u2SuccessRate );// ì„±ê³µ ë°°ìœ¨
		pop( recv.m_u8ChargeEXP );// íšë“ ê²½í—˜ì¹˜
		pop( recv.m_u8TotalEXP );// ì´ ê²½í—˜ì¹˜
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_DIGIMON_TRANSCENDENCE_CHARGE_EXP, &recv );
}
void cCliGame::RecvDigimonTranscendence()
{
	GS2C_RECV_DigimonTranscendence recv;
	pop( recv.m_nResult );
	if( nsDigimonTranscendenceResult::Success == recv.m_nResult )
	{
		pop( recv.m_u1DigimonTranscendencePos );
		pop( recv.m_u1HatchLevel );
		pop(recv.m_n8DigimonTranscendenceMoney); // ì´ˆì›” ìš”ì²­í•œ ë¹„ìš©
		pop(recv.m_n8Money);					    // í˜„ìž¬ ì†Œìœ  ëˆ
		pop(recv.m_u8Exp);						// í˜„ìž¬ ê²½í—˜ì¹˜
	}
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_DIGIMON_TRANSCENDENCE_HATCH_UP, &recv );
}
//////////////////////////////////////////////////////////////////////////
// ë””ì§€ëª¬ ì´íŒ©íŠ¸ ë³€ê²½
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvChangeDigimonEffect()
{
#if COMMON_LIB_FIXED
	GS2C_RECV_DIGIMON_EFFECT_CHANGE recv;
	pop(recv.nResult);	//ì„±ê³µì•„ë‹ì‹œ ì•„ëž˜ëŠ” ëª¨ë‘ 0
	pop(recv.nItemPos);	//ì•„ì´í…œ ìœ„ì¹˜
	pop(recv.nItemType);	//ì•„ì´í…œ íƒ€ìž…
	pop(recv.nRemainItemCount);			//ì‚¬ìš©í›„ ì•„ì´í…œ ê°¯ìˆ˜ , ë‹¤ì“°ë©´ 0
	pop(recv.nEffectedDigimonEvoSlot);	//ì•„ì´í…œì´ ì ìš©ëœ ì§„í™” ë‹¨ê³„
	pop(recv.nEffectType);				//ì ìš©ëœ ì´íŽ™íŠ¸ ì•„ì´í…œ IDX , ì´íŽ™íŠ¸ì œê±°ì‹œ 0ì¼ìˆ˜ìžˆìŒ
	if ( nsDIGIMON_EFFECT_CHANGE_RES::SUCCESS != recv.nResult)
	{
		cPrintMsg::PrintMsg(recv.nResult);
		return;
	}
	
	CDigimonUser* pDUser = g_pCharMng->GetDigimonUser(0);
	SAFE_POINTER_RET(pDUser);
	SAFE_POINTER_RET(g_pDataMng);
	// ì•„ì´í…œ ì‚¬ìš© ì ìš©
	g_pDataMng->ServerItemUseSuccess(pDUser->GetUniqID(), recv.nItemPos);
	CDigimonEvolveObj* pCurEvolObj = pDUser->GetFTEvolCurObj();
	SAFE_POINTER_RET( pCurEvolObj );
	if ( pCurEvolObj->m_nEvoSlot == recv.nEffectedDigimonEvoSlot )
	{
		if (0 != recv.nEffectType)
		{
			nsDPART::sDPART_INFO partInfo;
			partInfo.s_nFileTableID = recv.nEffectType;
			if (nsCsFileTable::g_pItemMng->GetDPartsInfo(recv.nEffectType, pDUser->GetFTID(), partInfo.s_nPartIndex, partInfo.s_File))
				pDUser->ChangeParts(partInfo);
		}
		else
			pDUser->DetachParts(nsDPART::Skin);
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UPDATE_STAT_DIGIMONSTATUS, NULL );
	}
#endif
}
// ë‹¤ë¥¸ ìœ ì €ê°€ ì•„ì´í…œ ì‚¬ìš© ì‹œ ë‚ ì•„ì˜´
void cCliGame::NTF_DigimonEffectChange()
{
#if COMMON_LIB_FIXED
	GS2C_NTF_DIGIMON_EFFECT_SYNC recv;
	pop(recv.nUID);			//í•´ë‹¹ ë””ì§€ëª¬ UID
	pop(recv.nEffectType);	//ìƒˆë¡œ ì ìš©ëœ ì´íŽ™íŠ¸ ì•„ì´í…œ , ì´íŽ™íŠ¸ì œê±°ì‹œ 0ì¼ìˆ˜ìžˆìŒ
	SAFE_POINTER_RET(g_pCharMng);
	CDigimon* pDigimon = g_pCharMng->GetDigimonFromUIDX(recv.nUID);
	SAFE_POINTER_RET(pDigimon);
	if (0 == recv.nEffectType)
	{
		pDigimon->DetachParts(nsDPART::Skin);
		return;
	}
	// ë””ì§€ëª¬ íŒŒì¸  ì¶”ê°€
	nsDPART::sDPART_INFO partsInfo;
	partsInfo.s_nFileTableID = recv.nEffectType;
	if (nsCsFileTable::g_pItemMng->GetDPartsInfo(recv.nEffectType, pDigimon->GetFTID(), partsInfo.s_nPartIndex, partsInfo.s_File))
		pDigimon->ChangeParts(partsInfo);
	// ë‚´ ë””ì§€ëª¬ì¸ ê²½ìš°
	if (g_pCharMng->IsDigimonUserFromUIDX(recv.nUID))
		GAME_EVENT_ST.OnEvent(EVENT_CODE::UPDATE_STAT_DIGIMONSTATUS, NULL);
#endif
}
