

#include "stdafx.h"

#include "cCliGame.h"

#include "common_vs2019/pTrade.h"

#include "common_vs2019/pPass2.h"

//////////////////////////////////////////////////////////////////////////
// Recebeu um pedido de transaÃ§Ã£o (apenas da outra parte)
// Received transaction request (only from the other party)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeRequest(void)	
{
	GS2C_RECV_TRADE_REQUEST recv;
	pop(recv.m_TargetTamerUID);

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_RECEIVE_REQUEST, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ë¶ˆê°€í•œ ìƒíƒœ(ìƒëŒ€ë°©í•œí…Œë§Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeImpossible(void)// ê±°ëž˜ ë¶ˆê°€í•œ ìƒíƒœ
{
	u4 nFlag;
	pop(nFlag);

	GS2C_RECV_TRADE_REQUEST_RESULT recv;
	switch( nFlag )
	{
	case 0:		recv.m_dwResult = 30023;	break;		//ë‹¤ë¥¸ ìš©ë¬´ ì¤‘ìž…ë‹ˆë‹¤. ê±°ëž˜ë¥¼ ì‹ ì²­í•  ìˆ˜ ì—†ìŠµë‹ˆë‹¤
	case 1:		recv.m_dwResult = 11015;	break;		// ë‚´ ê°€ë°©ì— ë¹ˆê³µê°„ ì—†ìŒ
	case 2:		recv.m_dwResult = 30031;	break;		// ìƒëŒ€ë°© ê°€ë°©ì— ë¹ˆê³µê°„ ì—†ìŒ
	default:	recv.m_dwResult	= nFlag;	break;
	}

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_REQUEST_RESULT, &recv );	
}


//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ê±°ë¶€ ë‹¹í•¨(ë‘˜ë‹¤ í•œí…Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeReject(void)
{
	GS2C_RECV_TRADE_REQUEST_RESULT recv;
	recv.m_dwResult = 30021;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_REQUEST_RESULT, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ìš”ì²­ì— ëŒ€í•œ ìŠ¹ë½/ê±°ë¶€ (ë‘˜ë‹¤ í•œí…Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeApproval(void)
{
	GS2C_RECV_TRADE_REQUEST_RESULT recv;
	pop(recv.m_TargetTamerUID);

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_REQUEST_RESULT, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì•„ì´í…œ ë“±ë¡ (ë‘˜ë‹¤ í•œí…Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeAddItem(void)
{
	GS2C_RECV_TRADE_ITEM_REGIST recv;
	pop(recv.m_TargetTamerUID);//ì•„ì´í…œì„ ì¶”ê°€í•œ ìœ ì € ì•„ì´ë””
	pop(recv.m_ItemData);
	pop(recv.m_TradeInvenSlotNum);// ê±°ëž˜ì°½ ìŠ¬ë¡¯ ë²ˆí˜¸
	pop(recv.m_InventorySlotNum);// ì¸ë²¤í† ë¦¬ ìŠ¬ë¡¯ ë²ˆí˜¸
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_ITEM_REGIST, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì•„ì´í…œ ì·¨ì†Œ (ë‘˜ë‹¤ í•œí…Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeCancelItem(void)
{
	GS2C_RECV_TRADE_ITEM_UNREGIST recv;
	pop(recv.m_TargetTamerUID);//ì•„ì´í…œì„ ì œê±°í•œ ìœ ì € ì•„ì´ë””
	pop(recv.m_nItemPosition);
	LOG("RecvTradeCancelItem protocol=%d target=%u slot=%u", pTrade::CancelItem, recv.m_TargetTamerUID, (int)recv.m_nItemPosition);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_ITEM_UNREGIST, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ëˆ ë³€ê²½ (ë‘˜ë‹¤ í•œí…Œ ê°)
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeAddMoney(void)	// ê²œë¨¸ë‹ˆ ì¶”ê°€
{
	GS2C_RECV_TRADE_MONEY_CHANGE recv;
	pop(recv.m_TargetTamerUID);
	pop(recv.m_nMoney);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_MONEY_CHANGE, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì¸ë²¤í† ë¦¬ ìž ê¸ˆ.
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeInvenLock(void)
{
	GS2C_RECV_TRADE_INVEN_LOCK recv;
	pop(recv.m_TargetTamerUID);// ê±°ëž˜ Ready ì·¨ì†Œí•œ í…Œì´ë¨¸ UID
	recv.m_bLock = true;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_INVEN_LOCK_UNLOCK, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì¸ë²¤í† ë¦¬ í•´ì œ
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeUnInvenLock(void)
{
	GS2C_RECV_TRADE_INVEN_LOCK recv;
	pop(recv.m_TargetTamerUID);// ê±°ëž˜ Ready ì·¨ì†Œí•œ í…Œì´ë¨¸ UID
	recv.m_bLock = false;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_INVEN_LOCK_UNLOCK, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ìµœì¢… ê±°ëž˜ í™•ì¸ íŒ¨í‚·
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeComplete(void)
{
	GS2C_RECV_TRADE_STANDBY recv;
	pop(recv.m_TargetTamerUID);
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_STANDBY_RESULT, &recv );	
}

//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì™„ë£Œ - ìµœì¢…
//////////////////////////////////////////////////////////////////////////
void cCliGame::RecvTradeCompleteFinal(void)
{
	GS2C_RECV_TRADE_FINAL_RESULT recv;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_TRADE_FINAL_RESULT, &recv );	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ê±°ëž˜ ìš”ì²­
void cCliGame::SendTradeRequest(u4 nTargetTamerUID)
{
	cCreateName::DeleteInstance();

	newp(pTrade::Request);
		push(nTargetTamerUID);
	endp(pTrade::Request);
	send();
}

// ìŠ¹ì¸
void cCliGame::SendTradeApproval(u4 nTargetTamerUID)
{
	newp(pTrade::Approval);
		push(nTargetTamerUID); // ë‚˜ì—ê²Œ ê±°ëž˜ ìš”ì²­í•œ ì‚¬ìš©ìžì—ê²Œ, ê±°ëž˜ ìŠ¹ì¸
	endp(pTrade::Approval);
	send();
}

// void cCliGame::SendTradeApprovalConfirm()
// {
// 	newp(pTrade::ApprovalConfirm);	
// 	endp(pTrade::ApprovalConfirm);
// 	send();
// }

void cCliGame::SendTradeReject(u4 nTargetTamerUID)
{
	newp(pTrade::Reject);
		push(nTargetTamerUID);
	endp(pTrade::Reject);
	send();
}


void cCliGame::SendTradeImpossible(u4 nTargetTamerUID)
{
	newp(pTrade::Impossible);
		push(nTargetTamerUID); // ë‚˜ì—ê²Œ ê±°ëž˜ ìš”ì²­í•œ ì‚¬ìš©ìžì—ê²Œ, ê±°ëž˜ ë¶ˆê°€ ì•Œë¦¼
	endp(pTrade::Impossible);
	send();
}


void cCliGame::SendTradeAddItem(u2 nInvenPos, u2 nCnt)
{
	newp(pTrade::AddItem);
		push(nInvenPos);
		push(nCnt);
	endp(pTrade::AddItem);
	send();
}

void cCliGame::SendTradeCancelItem(n1 nPos)
{
	newp(pTrade::CancelItem);		
	LOG("SendTradeCancelItem protocol=%d slot=%d", pTrade::CancelItem, (int)nPos);
		push(nPos);
		endp(pTrade::CancelItem);
	send();
}


void cCliGame::SendTradeAddMoney(u4 nMoney)
{
	newp(pTrade::AddMoney);
		push(nMoney);
	endp(pTrade::AddMoney);
	send();
}
//////////////////////////////////////////////////////////////////////////
// ê±°ëž˜ ì°½ ì¸ë²¤íŠ¸ë¡œ ìž ê¸ˆ/í•´ì œ 
//////////////////////////////////////////////////////////////////////////
void cCliGame::SendTradeInvenLock( bool bLock )
{
	if( bLock )  
	{
		newp(pTrade::InvenLock);
		endp(pTrade::InvenLock);
	}
	else
	{
		newp(pTrade::CompleteCancel);
		endp(pTrade::CompleteCancel);
	}

	send();
}

//////////////////////////////////////////////////////////////////////////
// ìµœì¢… ê±°ëž˜ í™•ì¸ íŒ¨í‚·
//////////////////////////////////////////////////////////////////////////
void cCliGame::SendTradeCompleteFinal()
{
	newp(pTrade::Complete);
	endp(pTrade::Complete);
	send();	
}


//////////////////////////////////////////////////////////////////
// í…Œì´ë¨¸ ê°œì¸ ìƒì  ê´€ë ¨ ì²˜ë¦¬

// ê°œì¸ ìƒì  ì˜¤í”ˆ ì¤€ë¹„ ìš”ì²­ & ê°œì¸ ìƒì  íŽ¸ì§‘ ê°€ëŠ¥ìƒíƒœë¡œ ë³€ê²½ ìš”ì²­
void cCliGame::SendTamerShopReady(n4 nRequestType, u4 nSlot)
{
	newp( pTrade::TamerShopReady );
		push( nRequestType );

#ifdef ITEM_USE_TIME_PASS
		if( nRequestType == 1 || nRequestType == 4 )	//2017-04-04-nova 1ì¼ê²½ìš°ì—ë„ ìŠ¬ë¡¯ë²ˆí˜¸ ì „ì†¡
		{
			push( nSlot );
		}
#else
		if( nRequestType == 4 )
		{
			push( nSlot );
		}
#endif
		
	endp( pTrade::TamerShopReady );

	send();
}

void cCliGame::RecvTamerShopReadyResult(void)
{
	GS2C_RECV_STORE_READY_RESULT recv;
	pop( recv.nResult );
	pop( recv.nItemType );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_STORE_READY_RESULT, &recv );
}

void cCliGame::SendTamerShopOpen(wchar const* szTitle, n4 nItemNumber, nTrade::Item *pSellItemList)	// ê°œì¸ ìƒì  ê°œì„¤
{
	std::wstring wtitle(szTitle);

	newp(pTrade::TamerShopOpen);
		push( wtitle );		// ìƒì  íƒ€ì´í‹€ 
		push(nItemNumber);	// íŒë§¤í•  ì•„ì´í…œ ê°œìˆ˜
		push(pSellItemList, sizeof(nTrade::Item)*nItemNumber);	// íŒë§¤í•  ì•„ì´í…œ ì •ë³´
	endp(pTrade::TamerShopOpen);

	send();
}

// ê°œì¸ ìƒì  ì˜¤í”ˆ ê²°ê³¼
void cCliGame::RecvTamerShopOpenResult(void)
{
	GS2C_RECV_STORE_OPEN_CLOSE_RESULT recv;
	pop( recv.nResult );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_OPEN_PERSONSTORE, &recv );
}

void cCliGame::SendTamerShopClose(void)	// ê°œì¸ ìƒì  íì‡„
{
	newp(pTrade::TamerShopClose);
	endp(pTrade::TamerShopClose);
	send();
}

void cCliGame::RecvTamerShopCloseResult(void)
{
	GS2C_RECV_STORE_OPEN_CLOSE_RESULT recv;
	pop( recv.nResult );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_CLOSE_PERSONSTORE, &recv );
}

// nType ê¸°ë³¸ ê°’ì€ 0 ìž…ë‹ˆë‹¤. 0: ê°œì¸ ìƒì  1: ìœ„íƒ ìƒì 
void cCliGame::SendTamerShopItemList(u4 nTargetTamerUID, uint nType) // ìƒì ì—ì„œ íŒë§¤í•˜ëŠ” ì•„ì´í…œ ëª©ë¡ ìš”ì²­
{
	newp(pTrade::TamerShopList);
		push( nType );
		push( nTargetTamerUID );
	endp(pTrade::TamerShopList);
	send();
}

void cCliGame::RecvTamerShopItemList(void)	// í…Œì´ë¨¸ ê°œì¸ ìƒì  ì•„ì´í…œ ëª©ë¡
{
	GS2C_RECV_PERSONSTORE_ITEMLIST recv;
	pop( recv.nResult );

	if( 100 == recv.nResult )
	{
		pop( recv.szTitle );
		pop( recv.nItemCount );		// íŒë§¤ ì•„ì´í…œ ê°œìˆ˜
		for ( int i = 0; i < recv.nItemCount; ++i )
		{
			nsPersonStore::sSellItemInfo SellItem;
			pop( SellItem.ItemData );
			pop( SellItem.nItemPrice );
			recv.lItemInfo.push_back( SellItem );
		}
	}

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_PERSONSTORE_ITEMLIST, &recv );
}


void cCliGame::SendTamerShopBuy(u4 nTargetTamerUID, uint nIndex, cItemData *item, n8 Price)	// ìƒì ìœ¼ë¡œ ë¶€í„° ì•„ì´í…œ êµ¬ë§¤
{
	// ë™ê¸°í™” - ê¸°ë°˜ì€ 1ë¶€í„°
	nIndex += 1;

	newp(pTrade::TamerShopBuy);
		push(nTargetTamerUID);	// ëŒ€ìƒ UID
		push(nIndex);			// ìƒì ì˜ ì•„ì´í…œ ì¸ë±ìŠ¤ 0ê°’ì€ ì‚¬ìš©í•˜ì§€ ì•ŠìŠµë‹ˆë‹¤. 1ë¶€í„°~
		push(*item);			// êµ¬ë§¤ ì•„ì´í…œ ì •ë³´
		push(Price);			// ê°œë³„ êµ¬ë§¤ ê¸ˆì•¡
	endp(pTrade::TamerShopBuy);

	send();
}

// ìƒì ìœ¼ë¡œ ë¶€í„° ì•„ì´í…œì„ êµ¬ë§¤ ê²°ê³¼
void cCliGame::RecvTamerShopBuy(void)
{
	GS2C_RECV_BUY_RESULT_PERSONITEM recv;
	pop( recv.nResult );
	if( 100 == recv.nResult )
	{
		pop( recv.nSlotIndex );
		pop( recv.nCount );
	}

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_BUY_ITEM_RESULT, &recv );

	net::game->SendQueryMoneyInven();
}

// ê°œì¸ ìƒì  - ìžê¸° ìƒì  ì•„ì´í…œ íŒë§¤ ê²°ê³¼
void cCliGame::RecvTamerShopSellResult(void)
{	
	// ìžì‹ ì´ ì˜¬ë¦° ì•„ì´í…œ íŒ”ë ¸ì„ ê²½ìš°
	GS2C_RECV_BUY_RESULT_PERSONITEM recv;
	recv.nResult = 100;		// íŒë§¤ ì„±ê³µìœ¼ë¡œ ê°€ì•¼í•œë‹¤
	pop( recv.nSlotIndex );
	pop( recv.nCount );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_BUY_ITEM_RESULT, &recv );

	net::game->SendQueryMoneyInven();
}

//////////////////////////////////////////////////////////////////
// ìœ„íƒ ìƒì  ê´€ë ¨ ì²˜ë¦¬

// nRequestType 1: ìƒì  ì˜¤í”ˆ ì¤€ë¹„ ìš”ì²­ 2: ìƒì  íŽ¸ì§‘ìƒíƒœë¡œ ë³€ê²½ ìš”ì²­ 3: ìƒì  ì˜¤í”ˆ ì¤€ë¹„ ì·¨ì†Œ
// pos ìœ„ì¹˜, nFloat íšŒì „ ê°’
// nSlot ë¼ì´ì„¼ìŠ¤ ì•„ì´í…œì´ ìžˆëŠ” ìœ„ì¹˜
	
// ìœ„íƒ ìƒì  ì˜¤í”ˆ ìš”ì²­ ( ìƒì  ì´ë¦„ )

void cCliGame::SendCommissionShopOpen(nSync::Pos pos, float nFloat, wchar const * szTitle, n8 nMoney, uint nItemCount, nTrade::Item *item )
{
	std::wstring wtitle(szTitle);

	newp( pTrade::ShopOpen );
		push( pos );
		push( nFloat );
		push( wtitle );				// ìƒì  ì´ë¦„
		push( nMoney );					// ìƒì  êµ¬ë§¤ ê¸ˆì•¡ ë“±ë¡
		push( nItemCount );
		push( item, sizeof( nTrade::Item) * nItemCount );
	endp( pTrade::ShopOpen );			
	send();

	ChannelContents::CHANNELINFO ChannelData;
	GAME_EVENT_ST.OnEvent(EVENT_CODE::GET_CHANNELINFO, &ChannelData );

	GS2C_RECV_SHOP_DATA pRecv;
	pRecv.iCharMapID = nsCsGBTerrain::g_pCurRoot->GetInfo()->s_dwMapID;
	pRecv.iCharChannel = ChannelData.channel_idx;
	pRecv.iCharModelID = -1;
	pRecv.iCharPos = NiPoint2( (float)pos.m_nX, (float)pos.m_nY );
	GAME_EVENT_ST.OnEvent( EVENT_CODE::UNION_SET_SHOPDATA, &pRecv );	
}

void cCliGame::RecvCommissionShopOpenResult(void)
{
	GS2C_RECV_STORE_OPEN_CLOSE_RESULT recv;
	pop( recv.nResult );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_OPEN_COMMISSIONSTORE, &recv );
}

void cCliGame::SendCommissionShopClose(void)															
{
	newp(pTrade::ShopClose);
	endp(pTrade::ShopClose);
	send();
}

void cCliGame::RecvCommissionShopCloseResult()
{
	GS2C_RECV_STORE_OPEN_CLOSE_RESULT recv;
	pop( recv.nResult );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_CLOSE_COMMISSIONSTORE, &recv );
}

// ìœ„íƒ ìƒì  ì•„ì´í…œ êµ¬ë§¤ ìš”ì²­ ( UID, ìƒí’ˆ ì¸ë±ìŠ¤, êµ¬ë§¤ ê°œìˆ˜ )
void cCliGame::SendCommissionShopBuy(u4 nShopUID, uint nIndex, cItemData *pItem, n8 nUnitPrice)
{
	// ë™ê¸°í™” - ê¸°ë°˜ì€ 1ë¶€í„°
	nIndex += 1;

	newp( pTrade::ShopBuy );
		push( nShopUID );
		push( nIndex );
		push( *pItem );
		push( nUnitPrice );
	endp( pTrade::ShopBuy );
	send();
}

void cCliGame::RecvCommissionShopBuyResult(void)
{
	GS2C_RECV_BUY_RESULT_PERSONITEM recv;
	pop( recv.nResult );
	pop( recv.nSlotIndex );
	pop( recv.nCount );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_BUY_ITEM_RESULT, &recv );

	net::game->SendQueryMoneyInven();
}

// ìœ„íƒ ìƒì  ì•„ì´í…œ íŒë§¤ ìš”ì²­ ( UID, ìƒí’ˆ ì¸ë±ìŠ¤, íŒë§¤ ì•„ì´í…œ, íŒë§¤ ê°œìˆ˜, ì•„ì´í…œ íƒ€ìž…, ê°œë³„ ê°€ê²© )	
void cCliGame::SendCommissionShopSell(u4 nShopUID, uint nIndex, cItemData *pItem, n8 nUnitPrice)
{
	// ë™ê¸°í™” - ê¸°ë°˜ì€ 1ë¶€í„°
	nIndex += 1;

	newp( pTrade::ShopSell );
		push( nShopUID );
		push( nIndex );		
		push( *pItem );
		push( nUnitPrice );
	endp( pTrade::ShopSell );
	send();
}

void cCliGame::RecvCommissionShopSellResult(void)
{
	// ìœ„íƒ ìƒì  íŒë§¤ ê²°ê³¼ì‹œ í˜¸ì¶œë˜ì§€ë§Œ ì•„ë¬´ê²ƒë„ ì•ˆ í•¨
	// íŒë§¤ ê²°ê³¼ ê°±ì‹ ë„ ì•ˆ í•¨
	// í˜„ìž¬ëŠ” UI ë‹¤ì‹œ ì¼¤ ë•Œ ê°±ì‹ ë¨
}


void cCliGame::RecvCommissionShopItemListResult(void)
{
	GS2C_RECV_COMMISSIONSTORE_ITEMLIST recv;
	pop( recv.nResult );

	if( 100 == recv.nResult )
	{
		pop( recv.nUsedItemType );
		pop( recv.szShopName );

		if( recv.szShopName.empty() )
			recv.szShopName = L"???";

#ifdef EMPLOYMENT_TAMERNAME
		pop( recv.szTamerName );
#endif

		{	// ì•ˆ ì“´ë‹¤. ë‚ ë¦°ë‹¤.
			u4 nBuyCount = 0;
			pop( nBuyCount );						// êµ¬ë§¤ ê°œìˆ˜

			cItemData ItemData;
			n8 nUnitPrice = 0;
			for( uint i = 0; i < nBuyCount; i++ )
			{		
				pop( ItemData );	// ì•„ì´í…œ ì •ë³´
				pop( nUnitPrice );	// ì•„ì´í…œ ê°œë³„ êµ¬ë§¤ê°€
			}
		}

		pop( recv.nItemCount );
		for( int i = 0; i < recv.nItemCount; ++i )
		{
			nsPersonStore::sSellItemInfo SellItem;
			pop( SellItem.ItemData );
			pop( SellItem.nItemPrice );
			recv.lItemInfo.push_back( SellItem );
		}

		pop( recv.bMyShop );
	}

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_COMMISSIONSTORE_ITEMLIST, &recv );
}

void cCliGame::SendCommissionShopNpcItemList()
{
	newp(pTrade::ShopNPCItemList);
	endp(pTrade::ShopNPCItemList);
	send();	
}


void cCliGame::RecvCommissionShopNpcItemList(void)
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_UNIONSTORE, 0 ) )
	{
		if( g_pGameIF->GetUnionStore()->IsActiveCollectBtn() == false )
			g_pGameIF->GetUnionStore()->SetEnableWindow( true );
	}

	GAME_EVENT_ST.OnEvent( EVENT_CODE::UNION_DELETE_DATA, NULL );

	uint nResult = 0;
	pop( nResult );

	if( 100 == nResult )
	{
		n8			nMoney = 0;
		uint		nCount = 0;
		cItemData	ItemData;

		pop( nMoney );	// ë³´ê´€ì¤‘ì¸ ê¸ˆì•¡
		GS2C_RECV_SET_UNIONMONEY recv1;
		recv1.money = nMoney;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::UNION_MONEY_SET_DATA, &recv1 );	

		pop( nCount );	// ë³´ê´€ì¤‘ì¸ ì•„ì´í…œ ê°œìˆ˜
		GS2C_RECV_ADD_UNIONITEM recv2;

		for( uint i = 0; i < nCount; i++ )
		{
			pop( ItemData );

			recv2.ItemData = ItemData;

			if( ItemData.GetType() == 0 )
				continue;

			GAME_EVENT_ST.OnEvent( EVENT_CODE::UNION_ITEM_ADD_DATA, &recv2 );
		}

		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_UNIONSTORE, 0 ) )
			g_pGameIF->GetUnionStore()->SetEnableCollectBtn( true );
	}
	else
	{
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_UNIONSTORE, 0 ) )
			g_pGameIF->GetUnionStore()->SetEnableCollectBtn( false );
	}	
}

// ìœ„íƒ ìƒì  ì•„ì´í…œ ë³´ê´€í•¨ì—ì„œ ì•„ì´í…œ íšŒìˆ˜ Type 1: ì•„ì´í…œ íšŒìˆ˜ 2: ëˆ íšŒìˆ˜
// ëˆ íšŒìˆ˜ì˜ ê²½ìš° nIndex ê°’ì„ 
void cCliGame::SendCommissionShopItemWithDraw()
{
	newp( pTrade::ShopWithdraw );
	endp( pTrade::ShopWithdraw );
	send();
}

void cCliGame::RecvCommissionShopItemWithDrawResult(void)
{
	int nResult = 0;
	pop( nResult );

	GAME_EVENT_ST.OnEvent( EVENT_CODE::RECV_UNION_SHOP_WITHDRAW, &nResult );	
}


