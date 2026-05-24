#include "stdafx.h"
#include "ExtraInventory.h"
#include "ExtraInventoryDebugLog.h"
#include "Separate.h"
#include "SealMasterContents.h"
#include "../../ContentsSystem/ContentsSystemDef.h"
#include <algorithm>

#define IF_EXTRA_INVENTORY_ICON_SIZE	CsPoint( 32, 32 )
#define IF_EXTRA_INVENTORY_NORMAL_GRID_POS CsPoint( 151, 78 )
#define IF_EXTRA_INVENTORY_NORMAL_GRID_STEP CsPoint( 34, 35 )
#define IF_EXTRA_INVENTORY_SEAL_CARD_SIZE CsPoint( 47, 69 )
#define IF_EXTRA_INVENTORY_SEAL_GRID_POS CsPoint( 158, 63 )
#define IF_EXTRA_INVENTORY_SEAL_GRID_STEP CsPoint( 56, 73 )
#define IF_EXTRA_INVENTORY_COMBO_POS CsPoint( 150, 43 )
#define IF_EXTRA_INVENTORY_COMBO_WIDE_SIZE CsPoint( 202, 22 )
#define IF_EXTRA_INVENTORY_COMBO_SMALL_SIZE CsPoint( 116, 22 )
#define IF_EXTRA_INVENTORY_SEARCH_POS CsPoint( 150, 43 )
#define IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_POS CsPoint( 282, 43 )
#define IF_EXTRA_INVENTORY_SEARCH_SIZE CsPoint( 230, 22 )
#define IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_SIZE CsPoint( 138, 22 )
#define IF_EXTRA_INVENTORY_SEARCH_EDIT_OFFSET CsPoint( 6, 3 )
#define IF_EXTRA_INVENTORY_SEARCH_EDIT_SIZE CsPoint( 172, 18 )
#define IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_EDIT_SIZE CsPoint( 86, 18 )
#define IF_EXTRA_INVENTORY_SEARCH_BUTTON_OFFSET CsPoint( 96, 0 )
#define IF_EXTRA_INVENTORY_SEARCH_RESET_OFFSET CsPoint( 207, 0 )
#define IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_RESET_OFFSET CsPoint( 118, 0 )
#define IF_EXTRA_INVENTORY_NORMAL_SCROLL_ROWS 10
#define IF_EXTRA_INVENTORY_SEAL_SCROLL_ROWS 5
#define IF_EXTRA_INVENTORY_SCROLL_POS	CsPoint( 426, 75 )
#define IF_EXTRA_INVENTORY_SIDE_MASK_POS CsPoint( 13, 154 )
#define IF_EXTRA_INVENTORY_SIDE_MASK_SIZE CsPoint( 123, 351 )

namespace
{
	std::wstring ToLowerText( std::wstring text )
	{
		std::transform( text.begin(), text.end(), text.begin(), _totlower );
		return text;
	}

	int GetActiveSealCount( int nItemType )
	{
		if( CONTENTSSYSTEM_PTR == NULL )
			return 0;

		cSealMasterContents* pSealContents = CONTENTSSYSTEM_PTR->GetContents< cSealMasterContents >( cSealMasterContents::IsContentsIdentity() );
		if( pSealContents == NULL )
			return 0;

		cSealMasterContents::SealInfoMap const& sealInfoMap = pSealContents->GetSealInfoMap();
		cSealMasterContents::SealInfoMap::const_iterator it = sealInfoMap.find( nItemType );
		if( it == sealInfoMap.end() )
			return 0;

		return it->second.sSealCount;
	}

	std::string GetSealTierBackImage( int nGrade )
	{
		switch( nGrade )
		{
		case nCardGrade::Normal:		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_normal.dds";
		case nCardGrade::Bronze:		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_bronze.dds";
		case nCardGrade::Silver:		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_silver.dds";
		case nCardGrade::Gold:		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_master.dds";
		case nCardGrade::Platinum:	return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_platinum.dds";
		case nCardGrade::Master:		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_master2.dds";
		default:					break;
		}

		return "Data\\Interface\\SealMaster\\SealBack\\sealmaster_bg_normal.dds";
	}
}

cExtraInventory::cExtraInventory()
: m_eCategory( CAT_SEAL )
, m_nOverviewFilter( 0 )
, m_pSidePanelMask( NULL )
, m_pCloseButton( NULL )
, m_pCollectAllButton( NULL )
, m_pCollectAllText( NULL )
, m_pOverviewComboBox( NULL )
, m_pOverviewCombo( NULL )
, m_pOverviewDropArrow( NULL )
, m_pOverviewComboButton( NULL )
, m_pOverviewText( NULL )
, m_pSearchBack( NULL )
, m_pSearchEdit( NULL )
, m_pSearchRenderText( NULL )
, m_pSearchButton( NULL )
, m_pSearchResetButton( NULL )
, m_pScrollBar( NULL )
, m_pScrollStaticTop( NULL )
, m_pScrollStaticCenter( NULL )
, m_pScrollStaticBottom( NULL )
, m_pSealHighlight( NULL )
, m_pSealEmptyCard( NULL )
, m_pBlock( NULL )
{
	for( int i = 0; i < CAT_MAX; ++i )
	{
		m_pCategoryButtons[ i ] = NULL;
		m_pCategoryTexts[ i ] = NULL;
	}

	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
	{
		m_pSlotFrames[ i ] = NULL;
		m_pSealCardBack[ i ] = NULL;
		m_pSealCardImage[ i ] = NULL;
		m_pSealCardCountBack[ i ] = NULL;
		m_pSealCardCountText[ i ] = NULL;
	}
}

cExtraInventory::~cExtraInventory()
{
}

bool cExtraInventory::Construct(void)
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] Construct this=%p", this );
	return true;
}

void cExtraInventory::Destroy()
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] Destroy this=%p live=%d show=%d", this, IsLive() ? 1 : 0, IsShowWindow() ? 1 : 0 );
	cBaseWindow::Delete();
}

void cExtraInventory::DeleteResource()
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] DeleteResource this=%p block=%p close=%p", this, m_pBlock, m_pCloseButton );
	DeleteScript();
	SAFE_NIDELETE( m_pSidePanelMask );
	SAFE_NIDELETE( m_pScrollStaticTop );
	SAFE_NIDELETE( m_pScrollStaticCenter );
	SAFE_NIDELETE( m_pScrollStaticBottom );
	SAFE_NIDELETE( m_pSealHighlight );
	SAFE_NIDELETE( m_pSealEmptyCard );
	SAFE_NIDELETE( m_pBlock );
	SAFE_NIDELETE( m_pOverviewComboBox );
	SAFE_NIDELETE( m_pSearchRenderText );
	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
	{
		SAFE_NIDELETE( m_pSealCardBack[ i ] );
		SAFE_NIDELETE( m_pSealCardImage[ i ] );
		SAFE_NIDELETE( m_pSealCardCountBack[ i ] );
		SAFE_NIDELETE( m_pSealCardCountText[ i ] );
		m_strSealCardBack[ i ].clear();
		m_strSealCardImage[ i ].clear();
	}
}

void cExtraInventory::Create( int nValue )
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] Create begin this=%p value=%d liveBefore=%d showBefore=%d screen=%dx%d",
		this, nValue, IsLive() ? 1 : 0, IsShowWindow() ? 1 : 0, g_nScreenWidth, g_nScreenHeight );

	if( cBaseWindow::Init() == false )
	{
		ExtraInventoryDebugLog( "[ExtraInventory][Window] Create aborted: cBaseWindow::Init returned false" );
		return;
	}

	SetRootClient( CsPoint( g_nScreenWidth - 470, 80 ) );
	InitScript( "NewInventory\\new_inventory_win.tga", m_ptRootClient, CsPoint( 452, 520 ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );
	AddTitle( _T( "Inventario Exclusivo" ) );

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_10, FONT_WHITE );

	TCHAR const* categoryNames[ CAT_MAX ] =
	{
		_T( "Inventario de Selo" ),
		_T( "Bolsa de Bilhete" ),
		_T( "Itens de Evolucao" ),
		_T( "DigiOvos" ),
		_T( "Materiais" )
	};

	for( int i = 0; i < CAT_MAX; ++i )
	{
		CsPoint ptTab( 17, 45 + i * 20 );
		m_pCategoryButtons[ i ] = AddButton( ptTab, CsPoint( 122, 20 ), CsPoint( 0, 23 ), "NewInventory\\new_inventory_btn_tab.tga" );
		if( m_pCategoryButtons[ i ] )
		{
			m_pCategoryButtons[ i ]->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cExtraInventory::CategoryButtonClick );
			m_pCategoryButtons[ i ]->SetMouseOnMode( i == static_cast<int>( m_eCategory ) );
		}

		ti.Init( CFont::FS_10, FONT_WHITE );
		ti.SetText( categoryNames[ i ] );
		ti.s_eTextAlign = DT_CENTER;
		m_pCategoryTexts[ i ] = AddText( &ti, ptTab + CsPoint( 61, 3 ) );
	}

	m_pSidePanelMask = NiNew cSprite;
	m_pSidePanelMask->Init( NULL, CsPoint::ZERO, IF_EXTRA_INVENTORY_SIDE_MASK_SIZE, NiColorA( 0.0f, 0.025f, 0.055f, 0.96f ), false );

	CreateOverviewComboBox();
	CreateSearchBox();

	CsPoint ptStart( IF_EXTRA_INVENTORY_NORMAL_GRID_POS );
	for( int y = 0; y < IF_EXTRA_INVENTORY_ROW; ++y )
	{
		for( int x = 0; x < IF_EXTRA_INVENTORY_COL; ++x )
		{
			int nSlotIndex = y * IF_EXTRA_INVENTORY_COL + x;
			m_pSlotFrames[ nSlotIndex ] = AddSprite( ptStart, IF_EXTRA_INVENTORY_ICON_SIZE, "Control_G\\Popup\\N2Dlg5.tga" );
			m_IFIcon[ nSlotIndex ].Init( CsRect( ptStart, (CsSIZE)IF_EXTRA_INVENTORY_ICON_SIZE ) );
			ptStart.x += IF_EXTRA_INVENTORY_NORMAL_GRID_STEP.x;
		}
		ptStart.x = IF_EXTRA_INVENTORY_NORMAL_GRID_POS.x;
		ptStart.y += IF_EXTRA_INVENTORY_NORMAL_GRID_STEP.y;
	}

	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
	{
		m_pSealCardBack[ i ] = NiNew cSprite;
		if( m_pSealCardBack[ i ] )
			m_pSealCardBack[ i ]->Init( NULL, CsPoint::ZERO, CsPoint( 45, 52 ), " ", false );

		m_pSealCardImage[ i ] = NiNew cSprite;
		if( m_pSealCardImage[ i ] )
			m_pSealCardImage[ i ]->Init( NULL, CsPoint::ZERO, CsPoint( 43, 47 ), " ", false );

		m_pSealCardCountBack[ i ] = NiNew cSprite;
		if( m_pSealCardCountBack[ i ] )
			m_pSealCardCountBack[ i ]->Init( NULL, CsPoint::ZERO, CsPoint( 45, 8 ), CsRect( 0, 0, 86, 14 ), "SealMaster\\sealmaster_belt_s.dds", false );

		cText::sTEXTINFO cardCountTi;
		cardCountTi.Init( CFont::FS_10, FONT_WHITE );
		cardCountTi.SetText( _T( "" ) );
		cardCountTi.s_eTextAlign = DT_CENTER;
		m_pSealCardCountText[ i ] = NiNew cText;
		if( m_pSealCardCountText[ i ] )
			m_pSealCardCountText[ i ]->Init( NULL, CsPoint::ZERO, &cardCountTi, false );
	}

	m_pBlock = NiNew cSprite;
	m_pBlock->Init( NULL, CsPoint::ZERO, IF_EXTRA_INVENTORY_ICON_SIZE, "NewInventory\\new_inventory_lock.tga", false );
	m_pBlock->SetAlpha( 0.35f );

	m_pSealHighlight = NiNew cSprite;
	m_pSealHighlight->Init( NULL, CsPoint::ZERO, IF_EXTRA_INVENTORY_SEAL_CARD_SIZE, "SealMaster\\sealmaster_highlight.tga", false );

	m_pSealEmptyCard = NiNew cSprite;
	m_pSealEmptyCard->Init( NULL, CsPoint::ZERO, IF_EXTRA_INVENTORY_SEAL_CARD_SIZE, CsRect( 8, 24, 94, 152 ), "SealMaster\\Sealmaster_bg_00.tga", false );

	m_pScrollStaticTop = NiNew cSprite;
	m_pScrollStaticTop->Init( NULL, CsPoint::ZERO, CsPoint( 16, 8 ), "SealMaster\\common_scroll_bar_01.tga", false );
	m_pScrollStaticCenter = NiNew cSprite;
	m_pScrollStaticCenter->Init( NULL, CsPoint::ZERO, CsPoint( 16, 28 ), "SealMaster\\common_scroll_bar_02.tga", false );
	m_pScrollStaticBottom = NiNew cSprite;
	m_pScrollStaticBottom->Init( NULL, CsPoint::ZERO, CsPoint( 16, 8 ), "SealMaster\\common_scroll_bar_03.tga", false );

	m_pScrollBar = AddScrollBar( cScrollBar::TYPE_SEALMASTER, IF_EXTRA_INVENTORY_SCROLL_POS, CsPoint( 16, 371 ), CsPoint( 16, 16 ), CsRect( IF_EXTRA_INVENTORY_SCROLL_POS, CsSIZE( 16, 371 ) ), IF_EXTRA_INVENTORY_NORMAL_SCROLL_ROWS );

	m_pCollectAllButton = AddButton( CsPoint( 234, 466 ), CsPoint( 124, 40 ), CsPoint( 0, 40 ), "CommonUI\\CommonUI_btn_s.tga" );
	if( m_pCollectAllButton )
		m_pCollectAllButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cExtraInventory::CollectAllButtonClick );

	ti.Init( CFont::FS_10, FONT_WHITE );
	ti.SetText( GetCollectAllText( m_eCategory ) );
	ti.s_eTextAlign = DT_CENTER;
	m_pCollectAllText = AddText( &ti, CsPoint( 296, 480 ) );

	m_pCloseButton = AddButton( CsPoint( 425, 8 ), CsPoint( 16, 16 ), CsPoint( 0, 16 ), "System\\Ch_Close.tga" );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cExtraInventory::CloseButtonClick );

	RefreshCategoryVisuals();

	ExtraInventoryDebugLog( "[ExtraInventory][Window] Create end this=%p liveAfter=%d showAfter=%d root=(%d,%d) size=(452,520) close=%p block=%p cat0=%p cat1=%p cat2=%p cat3=%p cat4=%p",
		this,
		IsLive() ? 1 : 0,
		IsShowWindow() ? 1 : 0,
		m_ptRootClient.x,
		m_ptRootClient.y,
		m_pCloseButton,
		m_pBlock,
		m_pCategoryButtons[ 0 ],
		m_pCategoryButtons[ 1 ],
		m_pCategoryButtons[ 2 ],
		m_pCategoryButtons[ 3 ],
		m_pCategoryButtons[ 4 ] );
}

bool cExtraInventory::Close( bool bSound )
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] Close this=%p sound=%d live=%d show=%d", this, bSound ? 1 : 0, IsLive() ? 1 : 0, IsShowWindow() ? 1 : 0 );
	return cBaseWindow::Close( bSound );
}

void cExtraInventory::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();
	UpdateFilterControlsLayout();
	cSeparate::UpdateInstance( GetWindowType(), 0 );
}

cBaseWindow::eMU_TYPE cExtraInventory::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muSeparate = cSeparate::Update_ForMouseInstance( GetWindowType(), 0 );
	if( muSeparate != MUT_OUT_WINDOW )
		return muSeparate;

	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
		return muReturn;

	for( int i = 0; i < CAT_MAX; ++i )
	{
		if( m_pCategoryButtons[ i ] && m_pCategoryButtons[ i ]->Update_ForMouse() != cButton::ACTION_NONE )
			return muReturn;
	}

	if( m_pCloseButton && m_pCloseButton->Update_ForMouse() != cButton::ACTION_NONE )
		return muReturn;

	if( m_pCollectAllButton && m_pCollectAllButton->Update_ForMouse() != cButton::ACTION_NONE )
		return muReturn;

	if( m_pSearchEdit && IsSearchFilterVisible() && m_pSearchEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
		return muReturn;

	if( m_pSearchButton && IsSearchFilterVisible() && m_pSearchButton->Update_ForMouse() != cButton::ACTION_NONE )
		return muReturn;

	if( m_pSearchResetButton && IsSearchFilterVisible() && m_pSearchResetButton->Update_ForMouse() != cButton::ACTION_NONE )
		return muReturn;

	if( m_pOverviewComboBox && IsOverviewFilterVisible() && m_pOverviewComboBox->Update_ForMouse() != cComboBox::CI_INVALIDE )
		return muReturn;

	if( m_pScrollBar )
	{
		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL )
		{
			m_pScrollBar->SetMouseWheelMove( CURSOR_ST.GetWheel() );
			CURSOR_ST.ResetWheel();
		}

		if( m_pScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
			return muReturn;
	}

	if( UpdateIconForMouse() )
		return muReturn;

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( 452, 34 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
	{
		ExtraInventoryDebugLog( "[ExtraInventory][Move] header drag candidate mouse=(%d,%d) root=(%d,%d)",
			CURSOR_ST.GetPos().x, CURSOR_ST.GetPos().y, GetRootClient().x, GetRootClient().y );
		_UpdateMoveWindow_ForMouse();
	}

	return muReturn;
}

void cExtraInventory::Render()
{
	RenderScript();

	std::vector<int> visibleSlots;
	CollectVisibleItems( visibleSlots );
	SAFE_POINTER_RET( g_pDataMng );
	cData_Inven* pCategoryInventory = GetCategoryInventory();
	SAFE_POINTER_RET( pCategoryInventory );

	int nColumnCount = GetVisibleColumnCount();
	int nVisibleRows = GetVisibleRowCount();
	int nVisibleSlotCount = GetVisibleSlotCount();
	int nTotalRows = ( pCategoryInventory->GetInvenSlotCount() + nColumnCount - 1 ) / nColumnCount;
	if( nTotalRows < nVisibleRows )
		nTotalRows = nVisibleRows;

	if( m_pScrollBar )
	{
		m_pScrollBar->SetRenderCount( nVisibleRows );
		m_pScrollBar->SetRange( CsPoint( 0, nTotalRows ) );
	}

	if( m_pSidePanelMask )
		m_pSidePanelMask->Render( GetRootClient() + IF_EXTRA_INVENTORY_SIDE_MASK_POS );
	RenderStaticScrollThumb();

	SAFE_POINTER_RET( g_pIconMng );

	int nScrollOffset = GetScrollOffset();
	int nOpenSlotCount = pCategoryInventory->GetInvenSlotCount();
	for( int i = 0; i < nVisibleSlotCount; ++i )
	{
		CsPoint pos = m_IFIcon[ i ].GetPos() + GetRootClient();
		int nVisibleIndex = i + nScrollOffset;

		if( nVisibleIndex >= nOpenSlotCount )
		{
			if( m_eCategory == CAT_SEAL )
				RenderSealEmptyCard( pos );

			if( m_pBlock )
			{
				CsPoint ptLockSize = IF_EXTRA_INVENTORY_ICON_SIZE;
				CsPoint ptLockPos = pos + CsPoint( ( GetSlotSize().x - ptLockSize.x ) / 2, ( GetSlotSize().y - ptLockSize.y ) / 2 );
				m_pBlock->Render( ptLockPos, ptLockSize );
			}
			RenderSlotHover( i, pos );
			continue;
		}

		if( nVisibleIndex >= static_cast<int>( visibleSlots.size() ) )
		{
			if( m_eCategory == CAT_SEAL )
				RenderSealEmptyCard( pos );
			RenderSlotHover( i, pos );
			continue;
		}

		cItemInfo* pItem = pCategoryInventory->GetData( visibleSlots[ nVisibleIndex ] );
		if( pItem && pItem->IsEnable() )
		{
			if( m_eCategory == CAT_SEAL )
				RenderSealCard( pItem, i, pos );
			else
			{
				CsItem::sINFO* pFTItemInfo = nsCsFileTable::g_pItemMng->GetItem( pItem->GetType() )->GetInfo();
				if( pFTItemInfo && ( pFTItemInfo->s_nType_L == nItem::Money || pFTItemInfo->s_nType_L == nItem::GoldBar ) )
					g_pIconMng->RenderItem( pItem, pos, false );
				else
					g_pIconMng->RenderItem( pItem, pos, true );
			}
		}
		else if( m_eCategory == CAT_SEAL )
			RenderSealEmptyCard( pos );

		RenderSlotHover( i, pos );
	}

	if( IsSearchFilterVisible() )
	{
		RenderSearchText();
		if( m_pSearchButton )
			m_pSearchButton->Render();
		if( m_pSearchResetButton )
			m_pSearchResetButton->Render();
	}

	if( m_pOverviewComboBox && IsOverviewFilterVisible() )
	{
		m_pOverviewComboBox->Render();
	}

	cSeparate::RenderInstance( GetWindowType(), 0 );
}

void cExtraInventory::OnLButtonUp( CsPoint pos )
{
	if( CURSOR_ST.IsIcon() == false )
	{
		CsPoint ptLocal = MousePosToWindowPos( pos );
		std::vector<int> visibleSlots;
		CollectVisibleItems( visibleSlots );
		int nScrollOffset = GetScrollOffset();

		SAFE_POINTER_RET( g_pDataMng );
		cData_Inven* pCategoryInventory = GetCategoryInventory();
		SAFE_POINTER_RET( pCategoryInventory );

		for( int i = 0; i < GetVisibleSlotCount(); ++i )
		{
			int nVisibleIndex = i + nScrollOffset;
			if( nVisibleIndex < 0 || nVisibleIndex >= static_cast<int>( visibleSlots.size() ) )
				continue;

			if( ( m_IFIcon[ i ].s_eState == sIFIcon::MOUSE_LCLICK ) && m_IFIcon[ i ].s_Rect.PtInRect( ptLocal ) )
			{
				int nSlotIndex = visibleSlots[ nVisibleIndex ];
				cItemInfo* pItem = pCategoryInventory->GetData( nSlotIndex );
				if( pItem && pItem->IsEnable() )
				{
					int nServerSlot = GetCategoryServerSlot( nSlotIndex );
					if( g_pDataMng->IsItemLock( nServerSlot ) == true )
					{
						cPrintMsg::PrintMsg( 10004 );
						break;
					}

					ExtraInventoryDebugLog( "[ExtraInventory][Drag] pickup category=%d slot=%d serverSlot=%d item=%d count=%d",
						m_eCategory, nSlotIndex, nServerSlot, pItem->GetType(), pItem->GetCount() );

					int nItemCount = pItem->GetCount();
					if( GLOBALINPUT_ST.IsShift() == false || nItemCount == 1
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
						|| cData_Inven::IsTradeLimitedItem( pItem )
#endif
						)
					{
						CURSOR_ST.SetIcon( CURSOR_ICON::CI_EXTRAINVEN, nServerSlot, nItemCount, &m_IFIcon[ i ] );
						cSeparate::DeleteInstance( GetWindowType(), 0 );
					}
					else
					{
						CsPoint posSeparate = m_IFIcon[ i ].GetPos() + GetRootClient();
						cSeparate::NewInstance( GetWindowType(), 0, posSeparate, 1, nItemCount, &m_IFIcon[ i ], nServerSlot );
					}
					break;
				}
			}
		}
	}

	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		m_IFIcon[ i ].ReleaseState();
}

void cExtraInventory::OnRButtonUp( CsPoint pos )
{
	if( CURSOR_ST.IsIcon() == true )
	{
		for( int i = 0; i < GetVisibleSlotCount(); ++i )
			m_IFIcon[ i ].ReleaseState();
		return;
	}

	CsPoint ptLocal = MousePosToWindowPos( pos );
	std::vector<int> visibleSlots;
	CollectVisibleItems( visibleSlots );
	int nScrollOffset = GetScrollOffset();

	SAFE_POINTER_RET( g_pDataMng );
	cData_Inven* pCategoryInventory = GetCategoryInventory();
	SAFE_POINTER_RET( pCategoryInventory );

	for( int i = 0; i < GetVisibleSlotCount(); ++i )
	{
		int nVisibleIndex = i + nScrollOffset;
		if( nVisibleIndex < 0 || nVisibleIndex >= static_cast<int>( visibleSlots.size() ) )
			continue;

		if( ( m_IFIcon[ i ].s_eState == sIFIcon::MOUSE_RCLICK ) && m_IFIcon[ i ].s_Rect.PtInRect( ptLocal ) )
		{
			int nSlotIndex = visibleSlots[ nVisibleIndex ];
			cItemInfo* pItem = pCategoryInventory->GetData( nSlotIndex );
			if( !pItem || pItem->IsEnable() == false )
				continue;

			int nServerSlot = GetCategoryServerSlot( nSlotIndex );
			if( g_pDataMng->IsItemLock( nServerSlot ) == true )
			{
				cPrintMsg::PrintMsg( 10004 );
				break;
			}

			int nItemCount = pItem->GetCount();
			if( GLOBALINPUT_ST.IsShift() == true && nItemCount > 1
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
				&& cData_Inven::IsTradeLimitedItem( pItem ) == false
#endif
				)
			{
				CsPoint posSeparate = m_IFIcon[ i ].GetPos() + GetRootClient();
				ExtraInventoryDebugLog( "[ExtraInventory][RClick] split category=%d slot=%d serverSlot=%d item=%d count=%d",
					m_eCategory, nSlotIndex, nServerSlot, pItem->GetType(), nItemCount );
				cSeparate::NewInstance( GetWindowType(), 0, posSeparate, 1, nItemCount, &m_IFIcon[ i ], nServerSlot );
			}
			else if( m_eCategory == CAT_SEAL )
			{
				ExtraInventoryDebugLog( "[ExtraInventory][RClick] use seal category=%d slot=%d serverSlot=%d item=%d count=%d",
					m_eCategory, nSlotIndex, nServerSlot, pItem->GetType(), nItemCount );
				g_pDataMng->SendItemUse( nServerSlot );
			}
			else
			{
				ExtraInventoryDebugLog( "[ExtraInventory][RClick] move-to-inven category=%d slot=%d serverSlot=%d item=%d count=%d",
					m_eCategory, nSlotIndex, nServerSlot, pItem->GetType(), nItemCount );
				g_pDataMng->SendItemMoveInven( nServerSlot, 0 );
			}
			break;
		}
	}

	for( int i = 0; i < GetVisibleSlotCount(); ++i )
		m_IFIcon[ i ].ReleaseState();
}

bool cExtraInventory::CursorIconLBtnUp( CURSOR_ICON::eTYPE eIconType, int nIconSlot, int nIconCount )
{
	CsPoint ptLocal = MousePosToWindowPos( CURSOR_ST.GetPos() );
	int nTargetSlot = GetGridSlotFromLocalPoint( ptLocal );
	if( nTargetSlot < 0 )
		return false;

	SAFE_POINTER_RETVAL( g_pDataMng, false );
	cData_Inven* pCategoryInventory = GetCategoryInventory();
	SAFE_POINTER_RETVAL( pCategoryInventory, false );

	int nDestServerSlot = GetCategoryServerSlot( nTargetSlot );
	ExtraInventoryDebugLog( "[ExtraInventory][Drop] begin category=%d cursorType=%d srcServerSlot=%d destSlot=%d destServerSlot=%d count=%d",
		m_eCategory, eIconType, nIconSlot, nTargetSlot, nDestServerSlot, nIconCount );

	if( g_pDataMng->IsItemLock( nIconSlot ) == true || g_pDataMng->IsItemLock( nDestServerSlot ) == true )
	{
		cPrintMsg::PrintMsg( 10004 );
		ExtraInventoryDebugLog( "[ExtraInventory][Drop] rejected: locked srcLocked=%d dstLocked=%d",
			g_pDataMng->IsItemLock( nIconSlot ) ? 1 : 0, g_pDataMng->IsItemLock( nDestServerSlot ) ? 1 : 0 );
		return true;
	}

	switch( eIconType )
	{
	case CURSOR_ICON::CI_INVEN:
		{
			cItemInfo* pSourceItem = g_pDataMng->GetInven()->GetData( TO_ID( nIconSlot ) );
			if( !pSourceItem || !pSourceItem->IsEnable() )
			{
				ExtraInventoryDebugLog( "[ExtraInventory][Drop] rejected: empty inventory source srcServerSlot=%d", nIconSlot );
				return true;
			}

			if( IsAllowedItem( pSourceItem ) == false )
			{
				cPrintMsg::PrintMsg( 11016 );
				ExtraInventoryDebugLog( "[ExtraInventory][Drop] rejected: wrong item category=%d srcServerSlot=%d item=%d count=%d",
					m_eCategory, nIconSlot, pSourceItem->GetType(), pSourceItem->GetCount() );
				return true;
			}

			ExtraInventoryDebugLog( "[ExtraInventory][Drop] send inventory-to-extra src=%d dest=%d item=%d count=%d",
				nIconSlot, nDestServerSlot, pSourceItem->GetType(), nIconCount );
			g_pDataMng->SendItemMove( nIconSlot, nDestServerSlot, nIconCount );
			CURSOR_ST.ReleaseIcon();
			return true;
		}

	case CURSOR_ICON::CI_EXTRAINVEN:
		if( IsCurrentCategoryServerSlot( nIconSlot ) == false )
		{
			cPrintMsg::PrintMsg( 11016 );
			ExtraInventoryDebugLog( "[ExtraInventory][Drop] rejected: source extra category mismatch src=%d currentCategory=%d", nIconSlot, m_eCategory );
			return true;
		}

		ExtraInventoryDebugLog( "[ExtraInventory][Drop] send extra-to-extra src=%d dest=%d count=%d", nIconSlot, nDestServerSlot, nIconCount );
		g_pDataMng->SendItemMove( nIconSlot, nDestServerSlot, nIconCount );
		CURSOR_ST.ReleaseIcon();
		return true;

	default:
		ExtraInventoryDebugLog( "[ExtraInventory][Drop] ignored cursorType=%d src=%d", eIconType, nIconSlot );
		return false;
	}
}

void cExtraInventory::OnMouseOutWindow()
{
	if( CURSOR_ST.GetButtonState() == CURSOR::LBUTTON_PRESS && CURSOR_ST.IsIcon() == false )
	{
		CsPoint ptMouseLocalPos = MousePosToWindowPos( CURSOR_ST.GetPos() );
		std::vector<int> visibleSlots;
		CollectVisibleItems( visibleSlots );
		int nScrollOffset = GetScrollOffset();

		SAFE_POINTER_RET( g_pDataMng );
		cData_Inven* pCategoryInventory = GetCategoryInventory();
		SAFE_POINTER_RET( pCategoryInventory );

		for( int i = 0; i < GetVisibleSlotCount(); ++i )
		{
			int nVisibleIndex = i + nScrollOffset;
			if( nVisibleIndex < 0 || nVisibleIndex >= static_cast<int>( visibleSlots.size() ) )
				continue;

			cItemInfo* pItem = pCategoryInventory->GetData( visibleSlots[ nVisibleIndex ] );
			if( !pItem || !pItem->IsEnable() )
				continue;

			if( m_IFIcon[ i ].s_eState == sIFIcon::MOUSE_LCLICK && m_IFIcon[ i ].s_Rect.PtInRect( ptMouseLocalPos ) == FALSE )
			{
				int nServerSlot = GetCategoryServerSlot( visibleSlots[ nVisibleIndex ] );
				if( g_pDataMng->IsItemLock( nServerSlot ) == true )
				{
					cPrintMsg::PrintMsg( 10004 );
					return;
				}

				ExtraInventoryDebugLog( "[ExtraInventory][Drag] pickup-out category=%d slot=%d serverSlot=%d item=%d count=%d",
					m_eCategory, visibleSlots[ nVisibleIndex ], nServerSlot, pItem->GetType(), pItem->GetCount() );

				int nItemCount = pItem->GetCount();
				if( GLOBALINPUT_ST.IsShift() == false || nItemCount == 1
#ifdef SDM_CASHITEM_TRADE_LIMITED_20170214
					|| cData_Inven::IsTradeLimitedItem( pItem )
#endif
					)
				{
					CURSOR_ST.SetIcon( CURSOR_ICON::CI_EXTRAINVEN, nServerSlot, nItemCount, &m_IFIcon[ i ] );
					cSeparate::DeleteInstance( GetWindowType(), 0 );
				}
				else
				{
					CsPoint posSeparate = m_IFIcon[ i ].GetPos() + GetRootClient();
					cSeparate::NewInstance( GetWindowType(), 0, posSeparate, 1, nItemCount, &m_IFIcon[ i ], nServerSlot );
				}
				break;
			}
		}
	}

	for( int i = 0; i < GetVisibleSlotCount(); ++i )
		m_IFIcon[ i ].ReleaseState();
}

bool cExtraInventory::OnEscapeKey()
{
	SAFE_POINTER_RETVAL( g_pGameIF, false );
	return g_pGameIF->CloseDynamicIF( GetWindowType() );
}

void cExtraInventory::ResetDevice()
{
	ResetDeviceScript();
	if( m_pSidePanelMask )
		m_pSidePanelMask->ResetDevice();
	if( m_pScrollStaticTop )
		m_pScrollStaticTop->ResetDevice();
	if( m_pScrollStaticCenter )
		m_pScrollStaticCenter->ResetDevice();
	if( m_pScrollStaticBottom )
		m_pScrollStaticBottom->ResetDevice();
	if( m_pBlock )
		m_pBlock->ResetDevice();
	if( m_pSealHighlight )
		m_pSealHighlight->ResetDevice();
	if( m_pSealEmptyCard )
		m_pSealEmptyCard->ResetDevice();
	if( m_pSearchRenderText )
		m_pSearchRenderText->ResetDevice();
	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
	{
		if( m_pSealCardBack[ i ] )
			m_pSealCardBack[ i ]->ResetDevice();
		if( m_pSealCardImage[ i ] )
			m_pSealCardImage[ i ]->ResetDevice();
		if( m_pSealCardCountBack[ i ] )
			m_pSealCardCountBack[ i ]->ResetDevice();
	}
}

void cExtraInventory::CloseButtonClick(void* pSender, void* pData)
{
	SAFE_POINTER_RET( g_pGameIF );
	g_pGameIF->CloseDynamicIF( GetWindowType() );
}

void cExtraInventory::CategoryButtonClick(void* pSender, void* pData)
{
	for( int i = 0; i < CAT_MAX; ++i )
	{
		if( pSender == m_pCategoryButtons[ i ] )
		{
			m_eCategory = static_cast< eCategory >( i );
			m_nOverviewFilter = 0;
			m_wsSearchText.clear();
			if( m_pSearchEdit )
				m_pSearchEdit->SetText( _T( "" ) );
			if( m_pScrollBar && m_pScrollBar->IsEnableScroll() )
				m_pScrollBar->SetCurPosIndex( 0 );
			RefreshCategoryVisuals();
			break;
		}
	}
}

void cExtraInventory::CollectAllButtonClick(void* pSender, void* pData)
{
	ExtraInventoryDebugLog( "[ExtraInventory][Window] CollectAll clicked category=%d net=%p", m_eCategory, net::game );
	if( net::game )
		net::game->SendExtraInventoryCollectAll( static_cast<u1>( m_eCategory ) );
	else
		ExtraInventoryDebugLog( "[ExtraInventory][Window] CollectAll blocked because net::game is null" );
}

void cExtraInventory::OverviewComboSelect(void* pSender, void* pData)
{
	if( m_pOverviewComboBox )
		m_nOverviewFilter = m_pOverviewComboBox->GetCurSel_Idx();
	if( m_nOverviewFilter < 0 )
		m_nOverviewFilter = 0;

	if( m_pScrollBar && m_pScrollBar->IsEnableScroll() )
		m_pScrollBar->SetCurPosIndex( 0 );

	ExtraInventoryDebugLog( "[ExtraInventory][Filter] overview changed idx=%d category=%d", m_nOverviewFilter, m_eCategory );
}

void cExtraInventory::SearchTextChanged(void* pSender, void* pData)
{
	m_wsSearchText.clear();
	if( m_pSearchEdit )
		m_wsSearchText = ToLowerText( m_pSearchEdit->GetTextAll() );

	if( m_pSearchResetButton )
		m_pSearchResetButton->SetVisible( IsSearchFilterVisible() );

	if( m_pScrollBar && m_pScrollBar->IsEnableScroll() )
		m_pScrollBar->SetCurPosIndex( 0 );

	ExtraInventoryDebugLog( "[ExtraInventory][Filter] search changed category=%d len=%d", m_eCategory, (int)m_wsSearchText.size() );
}

void cExtraInventory::SearchResetClick(void* pSender, void* pData)
{
	m_wsSearchText.clear();
	if( m_pSearchEdit )
		m_pSearchEdit->SetText( _T( "" ) );
	if( m_pSearchResetButton )
		m_pSearchResetButton->SetVisible( IsSearchFilterVisible() );

	if( m_pScrollBar && m_pScrollBar->IsEnableScroll() )
		m_pScrollBar->SetCurPosIndex( 0 );
}

bool cExtraInventory::UpdateIconForMouse()
{
	bool bCheck = false;
	CURSOR::eTYPE eCursor = CURSOR_ST.GetButtonState();
	CsPoint ptMouseLocalPos = MousePosToWindowPos( CURSOR_ST.GetPos() );
	int nIconIndex = -1;

	switch( eCursor )
	{
	case CURSOR::LBUTTON_DOWN:
		for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		{
			if( i >= GetVisibleSlotCount() )
			{
				m_IFIcon[ i ].ReleaseState();
				continue;
			}

			if( bCheck == false && m_IFIcon[ i ].MouseLBtnClick( ptMouseLocalPos ) == true )
			{
				bCheck = true;
				nIconIndex = i;
			}
			else
			{
				m_IFIcon[ i ].ReleaseState();
			}
		}
		break;
	case CURSOR::RBUTTON_DOWN:
		for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		{
			if( i >= GetVisibleSlotCount() )
			{
				m_IFIcon[ i ].ReleaseState();
				continue;
			}

			if( bCheck == false && m_IFIcon[ i ].MouseRBtnClick( ptMouseLocalPos ) == true )
			{
				bCheck = true;
				nIconIndex = i;
			}
			else
			{
				m_IFIcon[ i ].ReleaseState();
			}
		}
		break;
	case CURSOR::LBUTTON_PRESS:
		OnMouseOutWindow();
		for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		{
			if( i >= GetVisibleSlotCount() )
			{
				m_IFIcon[ i ].ReleaseState();
				continue;
			}

			if( m_IFIcon[ i ].s_eState != sIFIcon::MOUSE_LCLICK )
			{
				if( bCheck == false && m_IFIcon[ i ].MouseOn( ptMouseLocalPos ) == true )
				{
					bCheck = true;
					nIconIndex = i;
				}
				else
				{
					m_IFIcon[ i ].ReleaseState();
				}
			}
		}
		break;
	case CURSOR::RBUTTON_PRESS:
		for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		{
			if( i >= GetVisibleSlotCount() )
			{
				m_IFIcon[ i ].ReleaseState();
				continue;
			}

			if( m_IFIcon[ i ].s_eState != sIFIcon::MOUSE_RCLICK )
			{
				if( bCheck == false && m_IFIcon[ i ].MouseOn( ptMouseLocalPos ) == true )
				{
					bCheck = true;
					nIconIndex = i;
				}
				else
				{
					m_IFIcon[ i ].ReleaseState();
				}
			}
		}
		break;
	case CURSOR::LBUTTON_UP:
	case CURSOR::RBUTTON_UP:
	case CURSOR::BUTTON_NONE:
		for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
		{
			if( i >= GetVisibleSlotCount() )
			{
				m_IFIcon[ i ].ReleaseState();
				continue;
			}

			if( bCheck == false && m_IFIcon[ i ].MouseOn( ptMouseLocalPos ) == true )
			{
				bCheck = true;
				nIconIndex = i;
			}
			else
			{
				m_IFIcon[ i ].ReleaseState();
			}
		}
		break;
	}

	UpdateTooltip( ptMouseLocalPos, nIconIndex );

	return bCheck;
}

void cExtraInventory::UpdateTooltip( CsPoint ptPos, int nIndex )
{
	if( nIndex < 0 )
		return;

	std::vector<int> visibleSlots;
	CollectVisibleItems( visibleSlots );
	nIndex += GetScrollOffset();
	if( nIndex < 0 || nIndex >= static_cast<int>( visibleSlots.size() ) )
		return;

	SAFE_POINTER_RET( g_pDataMng );
	cData_Inven* pCategoryInventory = GetCategoryInventory();
	SAFE_POINTER_RET( pCategoryInventory );

	cItemInfo* pData = pCategoryInventory->GetData( visibleSlots[ nIndex ] );
	if( !pData || !pData->IsEnable() )
		return;

	cTooltip* pTooltip = TOOLTIPMNG_STPTR->GetTooltip();
	SAFE_POINTER_RET( pTooltip );
	CsPoint pos = m_IFIcon[ nIndex - GetScrollOffset() ].GetPos() + GetRootClient();

	if( m_eCategory == CAT_SEAL && nsCsFileTable::g_pMaster_CardMng )
	{
		CsMaster_Card* pCard = nsCsFileTable::g_pMaster_CardMng->GetMasterCard( pData->GetType() );
		if( pCard && pCard->GetInfo() )
		{
			int const nTotalSealCount = GetActiveSealCount( pData->GetType() ) + pData->GetCount();
			int const nGrade = GetSealGrade( pData->GetType(), pData->GetCount() );
			pTooltip->SetTooltip( pos, GetSlotSize(), 350, cTooltip::SEALMASTER, pData->GetType(), GetWindowType(), nGrade, nTotalSealCount, pCard->GetInfo() );
			return;
		}
	}

	pTooltip->SetTooltip( pos, GetSlotSize(), TOOLTIP_MAX_SIZE, cTooltip::ITEM, pData->GetType(), GetWindowType(), cTooltip::OPEN_SLOT, 0, pData );
}

void cExtraInventory::CollectVisibleItems( std::vector<int>& outSlots ) const
{
	outSlots.clear();
	SAFE_POINTER_RET( g_pDataMng );

	cData_Inven* pCategoryInventory = GetCategoryInventory();
	SAFE_POINTER_RET( pCategoryInventory );

	int nSlotCount = pCategoryInventory->GetInvenSlotCount();
	for( int i = 0; i < nSlotCount && i < nLimit::Inven; ++i )
	{
		cItemInfo* pItem = pCategoryInventory->GetData( i );
		if( pItem && pItem->IsEnable() && IsAllowedItem( pItem ) && IsAllowedByOverviewFilter( pItem ) && IsAllowedBySearchText( pItem ) )
			outSlots.push_back( i );
	}
}

cData_Inven* cExtraInventory::GetCategoryInventory() const
{
	SAFE_POINTER_RETVAL( g_pDataMng, NULL );
	return g_pDataMng->GetExtraInven( static_cast<int>( m_eCategory ) );
}

bool cExtraInventory::HasCategoryInventoryData() const
{
	cData_Inven* pInven = GetCategoryInventory();
	if( !pInven )
		return false;

	int nSlotCount = pInven->GetInvenSlotCount();
	for( int i = 0; i < nSlotCount && i < nLimit::Inven; ++i )
	{
		cItemInfo* pItem = pInven->GetData( i );
		if( pItem && pItem->IsEnable() )
			return true;
	}

	return false;
}

int cExtraInventory::GetCategoryServerSlot( int nSlotIndex ) const
{
	return TO_EXTRAINVEN_SID( static_cast<int>( m_eCategory ), nSlotIndex );
}

int cExtraInventory::GetGridSlotFromLocalPoint( CsPoint ptLocal )
{
	int nScrollOffset = GetScrollOffset();
	int nSlotCount = IF_EXTRA_INVENTORY_COUNT;
	cData_Inven* pInven = GetCategoryInventory();
	if( pInven )
		nSlotCount = pInven->GetInvenSlotCount();

	for( int i = 0; i < GetVisibleSlotCount(); ++i )
	{
		if( m_IFIcon[ i ].s_Rect.PtInRect( ptLocal ) == FALSE )
			continue;

		int nSlotIndex = i + nScrollOffset;
		if( nSlotIndex >= 0 && nSlotIndex < nSlotCount )
			return nSlotIndex;
		return -1;
	}

	return -1;
}

bool cExtraInventory::IsExtraInventoryServerSlot( int nServerSlot ) const
{
	return IS_EXTRAINVEN_CONSTANT( TO_CONSTANT( nServerSlot ) ) ? true : false;
}

bool cExtraInventory::IsCurrentCategoryServerSlot( int nServerSlot ) const
{
	if( IsExtraInventoryServerSlot( nServerSlot ) == false )
		return false;

	return TO_EXTRAINVEN_CATEGORY( TO_CONSTANT( nServerSlot ) ) == static_cast<int>( m_eCategory );
}

bool cExtraInventory::IsAllowedItem( cItemInfo const* pItem ) const
{
	if( !pItem || !pItem->IsEnable() )
		return false;

	CsItem* pFTItem = nsCsFileTable::g_pItemMng->GetItem( pItem->GetType() );
	if( !pFTItem )
		return false;

	CsItem::sINFO* pFTInfo = pFTItem->GetInfo();
	if( !pFTInfo )
		return false;

	return IsAllowedItemType( pFTInfo->s_nType_L, pFTInfo->s_nType_S );
}

bool cExtraInventory::IsAllowedItemType( int nTypeL, int nTypeS ) const
{
	switch( m_eCategory )
	{
	case CAT_SEAL:
		return nTypeL == nItem::CardMaster || nTypeL == nItem::UnSealCardMaster || nTypeL == nItem::ReSealCardMaster;
	case CAT_TICKET:
		return nTypeL == nItem::GotchaCoin || nTypeL == nItem::Capsule_Coin || nTypeL == nItem::Cash_CrossInvenSlot;
	case CAT_EVOLUTION:
		return nTypeL == nItem::Cash_Evoluter ||
#if COMMON_LIB_FIXED
			nTypeL == nItem::Cash_Evoluter2 ||
#endif
			nTypeL == nItem::Excelator || nTypeL == nItem::EvoCapsule || nTypeL == nItem::Spirit ||
			nTypeL == nItem::SpiritEXP || nTypeL == nItem::AntiXEvoluter;
	case CAT_DIGITAMA:
		return nTypeL == nItem::Digitama1 || nTypeL == nItem::Digitama2 || nTypeL == nItem::Digitama3 ||
			nTypeL == nItem::Digitama4 || nTypeL == nItem::Digitama5 || nTypeL == nItem::Digitama6 ||
			nTypeL == nItem::RClick_Hatch || nTypeL == nItem::Incubater;
	case CAT_MATERIAL:
		return nTypeL == nItem::Matrial || nTypeL == nItem::Matrial2 || nTypeL == nItem::Revision ||
			nTypeL == nItem::AttributeItem || nTypeL == nItem::Cash_DigiCore || nTypeL == nItem::BuffEvent ||
			nTypeL == nItem::Quest || nTypeL == nItem::UseQuest || nTypeL == nItem::EventItem || nTypeL == nItem::EventItem2;
	default:
		return false;
	}
}

bool cExtraInventory::IsAllowedByOverviewFilter( cItemInfo const* pItem ) const
{
	if( m_nOverviewFilter <= 0 )
		return true;

	if( !pItem || !pItem->IsEnable() )
		return false;

	if( m_eCategory == CAT_SEAL )
	{
		if( !nsCsFileTable::g_pMaster_CardMng )
			return false;

		CsMaster_Card* pCard = nsCsFileTable::g_pMaster_CardMng->GetMasterCard( pItem->GetType() );
		if( !pCard || !pCard->GetInfo() )
			return false;

		int const nEffectType = pCard->GetInfo()->s_stGradeInfo[ CsMaster_Card::FT_CARD_NORMAL ].s_nEff1;
		return EffectTypeToOverviewIndex( nEffectType ) == m_nOverviewFilter;
	}

	if( m_eCategory == CAT_EVOLUTION )
		return GetEvolutionRankFilterIndex( pItem ) == m_nOverviewFilter;

	return true;
}

bool cExtraInventory::IsAllowedBySearchText( cItemInfo const* pItem ) const
{
	if( m_wsSearchText.empty() || IsSearchFilterVisible() == false )
		return true;

	if( !pItem || !pItem->IsEnable() || !nsCsFileTable::g_pItemMng )
		return false;

	CsItem* pFTItem = nsCsFileTable::g_pItemMng->GetItem( pItem->GetType() );
	if( !pFTItem || !pFTItem->GetInfo() )
		return false;

	std::wstring wsItemName = ToLowerText( pFTItem->GetInfo()->s_szName );
	return wsItemName.find( m_wsSearchText ) != std::wstring::npos;
}

TCHAR const* cExtraInventory::GetCategoryName( eCategory eType ) const
{
	switch( eType )
	{
	case CAT_SEAL:		return _T( "Inventario de Selo" );
	case CAT_TICKET:	return _T( "Bolsa de Bilhete" );
	case CAT_EVOLUTION:	return _T( "Itens de Evolucao" );
	case CAT_DIGITAMA:	return _T( "DigiOvos" );
	case CAT_MATERIAL:	return _T( "Materiais" );
	default:			return _T( "" );
	}
}

TCHAR const* cExtraInventory::GetCollectAllText( eCategory eType ) const
{
	switch( eType )
	{
	case CAT_SEAL:
		return _T( "Trazer todos os Selos" );
	case CAT_TICKET:
		return _T( "Obter todos ingressos" );
	case CAT_EVOLUTION:
	case CAT_DIGITAMA:
	case CAT_MATERIAL:
		return _T( "Trazer" );
	default:			return _T( "" );
	}
}

void cExtraInventory::CreateOverviewComboBox()
{
	m_pOverviewComboBox = NiNew cComboBox;
	SAFE_POINTER_RET( m_pOverviewComboBox );

	m_pOverviewComboBox->Init( GetRoot(), cComboBox::CT_NORMAL, IF_EXTRA_INVENTORY_COMBO_POS, IF_EXTRA_INVENTORY_COMBO_WIDE_SIZE, CFont::FS_10, DT_CENTER, false, false, false );
	m_pOverviewComboBox->ChangeComboBoxImg( "SealMaster\\btn_combobox.tga", CsPoint( 0, 22 ) );
	m_pOverviewComboBox->SetComboListWindow( "SealMaster\\dropdown_list_0" );
	m_pOverviewComboBox->AddEvent( cComboBox::COMBO_SELECTITEM, this, &cExtraInventory::OverviewComboSelect );

	RefreshOverviewComboItems();
}

void cExtraInventory::CreateSearchBox()
{
	m_pSearchBack = AddSprite( IF_EXTRA_INVENTORY_SEARCH_POS, IF_EXTRA_INVENTORY_SEARCH_SIZE, "TacticsHouse\\Storage_search.tga" );

	m_pSearchEdit = NiNew cEditBox;
	if( m_pSearchEdit )
	{
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontText, CFont::FS_11, NiColor::WHITE );
		ti.SetText( _T( "" ) );
		ti.s_bOutLine = false;

		m_pSearchEdit->Init( GetRoot(), IF_EXTRA_INVENTORY_SEARCH_POS + IF_EXTRA_INVENTORY_SEARCH_EDIT_OFFSET, IF_EXTRA_INVENTORY_SEARCH_EDIT_SIZE, &ti, false );
		m_pSearchEdit->SetEmptyMsgText( _T( "Inserir a busca." ), NiColor( 0.45f, 0.60f, 0.72f ) );
		m_pSearchEdit->SetFontLength( 40 );
		m_pSearchEdit->EnableUnderline( ti.s_Color );
		m_pSearchEdit->SetEnableSound( true );
		m_pSearchEdit->AddEvent( cEditBox::eEditbox_ChangeText, this, &cExtraInventory::SearchTextChanged );
		AddChildControl( m_pSearchEdit );
	}

	m_pSearchRenderText = NiNew cText;
	if( m_pSearchRenderText )
	{
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontText, CFont::FS_11, NiColor::WHITE );
		ti.SetText( _T( "" ) );
		ti.s_bOutLine = false;
		ti.s_eTextAlign = DT_LEFT;
		m_pSearchRenderText->Init( NULL, CsPoint::ZERO, &ti, false );
	}

	m_pSearchButton = AddButton( IF_EXTRA_INVENTORY_SEARCH_POS + IF_EXTRA_INVENTORY_SEARCH_BUTTON_OFFSET, CsPoint( 22, 22 ), CsPoint( 0, 32 ), "Encyclopedia\\NewEncyclopedia\\main\\sarch.png" );
	if( m_pSearchButton )
		m_pSearchButton->SetVisible( false );

	m_pSearchResetButton = AddButton( IF_EXTRA_INVENTORY_SEARCH_POS + IF_EXTRA_INVENTORY_SEARCH_RESET_OFFSET, CsPoint( 22, 22 ), CsPoint( 0, 32 ), "Encyclopedia\\NewEncyclopedia\\main\\re.png" );
	if( m_pSearchResetButton )
	{
		m_pSearchResetButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cExtraInventory::SearchResetClick );
		m_pSearchResetButton->SetVisible( false );
	}
}

void cExtraInventory::RefreshOverviewComboItems()
{
	SAFE_POINTER_RET( m_pOverviewComboBox );

	m_pOverviewComboBox->SetVisibleRenderPopup( false );
	m_pOverviewComboBox->RemoveAllItem();

	if( m_eCategory == CAT_EVOLUTION )
	{
		m_pOverviewComboBox->SetSize( IF_EXTRA_INVENTORY_COMBO_SMALL_SIZE );
		m_pOverviewComboBox->SetComboListWindow( "SealMaster\\dropdown_list_0" );

		m_pOverviewComboBox->AddItem( _T( "Visao geral" ), 0, FONT_GREEN );
		m_pOverviewComboBox->AddItem( _T( "N" ), 1, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "A" ), 2, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "A+" ), 3, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "S" ), 4, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "S+" ), 5, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "SS" ), 6, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "SS+" ), 7, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "SSS" ), 8, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "SSS+" ), 9, FONT_WHITE );
		m_pOverviewComboBox->AddItem( _T( "U" ), 10, FONT_WHITE );
		m_pOverviewComboBox->SetCurSel( 0, false );
		return;
	}

	m_pOverviewComboBox->SetSize( IF_EXTRA_INVENTORY_COMBO_WIDE_SIZE );
	m_pOverviewComboBox->SetComboListWindow( "SealMaster\\dropdown_list_0" );
	m_pOverviewComboBox->AddItem( _T( "Visao geral" ), 0, FONT_GREEN );
	m_pOverviewComboBox->AddItem( _T( "HP [Ponto de Acerto Maximo]" ), 1, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "DS [DigiSoul Maximo]" ), 2, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "AT [Dano de Ataque]" ), 3, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "AS [Velocidade de Ataque]" ), 4, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "CT [Taxa de Acerto Critico]" ), 5, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "HT [Taxa de Acerto]" ), 6, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "DE [Defesa]" ), 7, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "BL [Taxa de Bloqueio]" ), 8, FONT_WHITE );
	m_pOverviewComboBox->AddItem( _T( "EV [Taxa de Evasao]" ), 9, FONT_WHITE );
	m_pOverviewComboBox->SetCurSel( 0, false );
}

void cExtraInventory::RefreshCategoryVisuals()
{
	RefreshOverviewComboItems();

	for( int i = 0; i < CAT_MAX; ++i )
	{
		if( m_pCategoryButtons[ i ] )
			m_pCategoryButtons[ i ]->SetMouseOnMode( i == static_cast<int>( m_eCategory ) );
	}

	if( m_pCollectAllText )
		m_pCollectAllText->SetText( GetCollectAllText( m_eCategory ) );

	UpdateFilterControlsLayout();
	UpdateSlotLayout();
}

void cExtraInventory::UpdateFilterControlsLayout()
{
	bool const bShowOverview = IsOverviewFilterVisible();
	bool const bShowSearch = IsSearchFilterVisible();

	if( m_pOverviewComboBox )
	{
		m_pOverviewComboBox->SetVisible( bShowOverview );
		if( bShowOverview )
			m_pOverviewComboBox->SetPos( IF_EXTRA_INVENTORY_COMBO_POS );
	}

	bool const bEvolutionSearch = ( m_eCategory == CAT_EVOLUTION );
	CsPoint ptSearch = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_POS : IF_EXTRA_INVENTORY_SEARCH_POS;
	CsPoint ptSearchSize = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_SIZE : IF_EXTRA_INVENTORY_SEARCH_SIZE;
	CsPoint ptEditSize = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_EDIT_SIZE : IF_EXTRA_INVENTORY_SEARCH_EDIT_SIZE;
	CsPoint ptSearchButtonOffset = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_BUTTON_OFFSET : CsPoint( 184, 0 );
	CsPoint ptResetOffset = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_RESET_OFFSET : IF_EXTRA_INVENTORY_SEARCH_RESET_OFFSET;
	if( m_pSearchBack )
	{
		m_pSearchBack->SetVisible( bShowSearch );
		m_pSearchBack->SetPosSize( ptSearch, ptSearchSize );
	}
	if( m_pSearchEdit )
	{
		m_pSearchEdit->SetVisible( bShowSearch );
		m_pSearchEdit->SetPos( ptSearch + IF_EXTRA_INVENTORY_SEARCH_EDIT_OFFSET );
		m_pSearchEdit->SetSize( ptEditSize );
	}
	if( m_pSearchButton )
	{
		m_pSearchButton->SetVisible( bShowSearch );
		m_pSearchButton->SetPos( ptSearch + ptSearchButtonOffset );
	}
	if( m_pSearchResetButton )
	{
		m_pSearchResetButton->SetVisible( bShowSearch );
		m_pSearchResetButton->SetPos( ptSearch + ptResetOffset );
	}
}

void cExtraInventory::RenderSearchText()
{
	if( !m_pSearchRenderText || !IsSearchFilterVisible() )
		return;

	TCHAR const* pText = _T( "" );
	if( m_pSearchEdit )
		pText = m_pSearchEdit->GetTextAll();

	bool const bEmpty = ( pText == NULL || pText[ 0 ] == _T( '\0' ) );
	m_pSearchRenderText->SetColor( bEmpty ? NiColor( 0.45f, 0.60f, 0.72f ) : NiColor::WHITE );
	m_pSearchRenderText->SetText( bEmpty ? _T( "Inserir a busca." ) : pText );

	bool const bEvolutionSearch = ( m_eCategory == CAT_EVOLUTION );
	CsPoint ptSearch = bEvolutionSearch ? IF_EXTRA_INVENTORY_SEARCH_WITH_COMBO_POS : IF_EXTRA_INVENTORY_SEARCH_POS;
	CsPoint ptText = GetRootClient() + ptSearch + IF_EXTRA_INVENTORY_SEARCH_EDIT_OFFSET;
	m_pSearchRenderText->Render( ptText, DT_LEFT );
}

int cExtraInventory::GetScrollOffset() const
{
	if( !m_pScrollBar )
		return 0;

	return m_pScrollBar->GetCurPosIndex() * GetVisibleColumnCount();
}

int cExtraInventory::GetVisibleColumnCount() const
{
	return IF_EXTRA_INVENTORY_COL;
}

int cExtraInventory::GetVisibleRowCount() const
{
	return ( m_eCategory == CAT_SEAL ) ? IF_EXTRA_INVENTORY_SEAL_SCROLL_ROWS : IF_EXTRA_INVENTORY_NORMAL_SCROLL_ROWS;
}

int cExtraInventory::GetVisibleSlotCount() const
{
	return GetVisibleColumnCount() * GetVisibleRowCount();
}

CsPoint cExtraInventory::GetSlotSize() const
{
	return ( m_eCategory == CAT_SEAL ) ? IF_EXTRA_INVENTORY_SEAL_CARD_SIZE : IF_EXTRA_INVENTORY_ICON_SIZE;
}

CsPoint cExtraInventory::GetSlotStep() const
{
	return ( m_eCategory == CAT_SEAL ) ? IF_EXTRA_INVENTORY_SEAL_GRID_STEP : IF_EXTRA_INVENTORY_NORMAL_GRID_STEP;
}

CsPoint cExtraInventory::GetGridStartPos() const
{
	return ( m_eCategory == CAT_SEAL ) ? IF_EXTRA_INVENTORY_SEAL_GRID_POS : IF_EXTRA_INVENTORY_NORMAL_GRID_POS;
}

bool cExtraInventory::IsOverviewFilterVisible() const
{
	return m_eCategory == CAT_SEAL || m_eCategory == CAT_EVOLUTION;
}

bool cExtraInventory::IsSearchFilterVisible() const
{
	return m_eCategory == CAT_TICKET || m_eCategory == CAT_EVOLUTION || m_eCategory == CAT_DIGITAMA || m_eCategory == CAT_MATERIAL;
}

int cExtraInventory::EffectTypeToOverviewIndex( int nEffectType ) const
{
	switch( nEffectType )
	{
	case 0:
		return 0;
	case 1:
	case 2:
		return 1;
	case 3:
	case 4:
		return 2;
	case 5:
	case 6:
		return 3;
	case 7:
	case 8:
		return 4;
	case 9:
	case 10:
		return 5;
	case 11:
	case 12:
		return 6;
	case 13:
	case 14:
		return 7;
	case 15:
	case 16:
		return 8;
	case 17:
	case 18:
		return 9;
	default:
		return 0;
	}
}

int cExtraInventory::GetEvolutionRankFilterIndex( cItemInfo const* pItem ) const
{
	if( !pItem || !pItem->IsEnable() || !nsCsFileTable::g_pItemMng )
		return 0;

	CsItemRank::MAP const* pRanks = nsCsFileTable::g_pItemMng->GetItemRank();
	if( !pRanks )
		return 0;

	CsItemRank::MAP::const_iterator it = pRanks->find( pItem->GetType() );
	if( it == pRanks->end() || !it->second || !it->second->GetRank() )
		return 0;

	int const nDropType = it->second->GetRank()->s_nDropType;
	if( nDropType >= 1 && nDropType <= 10 )
		return nDropType;
	if( nDropType >= 0 && nDropType <= 9 )
		return nDropType + 1;
	return 0;
}

void cExtraInventory::UpdateSlotLayout()
{
	CsPoint ptStart = GetGridStartPos();
	CsPoint ptStep = GetSlotStep();
	CsPoint ptSize = GetSlotSize();
	int const nVisibleSlotCount = GetVisibleSlotCount();

	for( int i = 0; i < IF_EXTRA_INVENTORY_COUNT; ++i )
	{
		if( i < nVisibleSlotCount )
		{
			int const x = i % GetVisibleColumnCount();
			int const y = i / GetVisibleColumnCount();
			CsPoint const ptSlot = ptStart + CsPoint( x * ptStep.x, y * ptStep.y );
			m_IFIcon[ i ].Init( CsRect( ptSlot, (CsSIZE)ptSize ) );

			if( m_pSlotFrames[ i ] )
			{
				m_pSlotFrames[ i ]->SetVisible( m_eCategory != CAT_SEAL );
				m_pSlotFrames[ i ]->SetPosSize( ptSlot, ptSize );
				if( m_eCategory != CAT_SEAL )
					m_pSlotFrames[ i ]->ChangeTexture( "Control_G\\Popup\\N2Dlg5.tga" );
			}
		}
		else
		{
			m_IFIcon[ i ].Init( CsRect( CsPoint( -1000, -1000 ), CsSIZE( 1, 1 ) ) );
			if( m_pSlotFrames[ i ] )
				m_pSlotFrames[ i ]->SetVisible( false );
		}
	}

	if( m_pScrollBar )
		m_pScrollBar->SetRenderCount( GetVisibleRowCount() );
}

int cExtraInventory::GetSealGrade( int nItemType, int nCount ) const
{
	SAFE_POINTER_RETVAL( nsCsFileTable::g_pMaster_CardMng, CsMaster_Card::FT_CARD_NORMAL );
	CsMaster_Card* pCard = nsCsFileTable::g_pMaster_CardMng->GetMasterCard( nItemType );
	if( !pCard || !pCard->GetInfo() )
		return CsMaster_Card::FT_CARD_NORMAL;

	CsMaster_Card::sINFO* pInfo = pCard->GetInfo();
	int const nTotalCount = GetActiveSealCount( nItemType ) + nCount;
	if( nTotalCount <= 0 )
		return CsMaster_Card::FT_CARD_NORMAL;

	if( nTotalCount < pInfo->s_stGradeInfo[ CsMaster_Card::FT_CARD_BRONZE ].s_nMax )
		return CsMaster_Card::FT_CARD_NORMAL;
	if( nTotalCount < pInfo->s_stGradeInfo[ CsMaster_Card::FT_CARD_SHILVER ].s_nMax )
		return CsMaster_Card::FT_CARD_BRONZE;
	if( nTotalCount < pInfo->s_stGradeInfo[ CsMaster_Card::FT_CARD_GOLD ].s_nMax )
		return CsMaster_Card::FT_CARD_SHILVER;
	if( nTotalCount < pInfo->s_stGradeInfo[ CsMaster_Card::FT_CARD_PLATINUM ].s_nMax )
		return CsMaster_Card::FT_CARD_GOLD;
	if( nTotalCount < pInfo->s_stGradeInfo[ CsMaster_Card::FT_CARD_MASTER ].s_nMax )
		return CsMaster_Card::FT_CARD_PLATINUM;
	return CsMaster_Card::FT_CARD_MASTER;
}

void cExtraInventory::RenderSealEmptyCard( CsPoint pos )
{
	if( m_pSealEmptyCard )
	{
		m_pSealEmptyCard->SetColor( NiColor::WHITE );
		m_pSealEmptyCard->Render( pos, IF_EXTRA_INVENTORY_SEAL_CARD_SIZE );
	}
}

void cExtraInventory::RenderSealCard( cItemInfo* pItem, int nIndex, CsPoint pos )
{
	if( !pItem || nIndex < 0 || nIndex >= IF_EXTRA_INVENTORY_COUNT )
		return;

	SAFE_POINTER_RET( nsCsFileTable::g_pMaster_CardMng );
	CsMaster_Card* pCard = nsCsFileTable::g_pMaster_CardMng->GetMasterCard( pItem->GetType() );
	if( !pCard || !pCard->GetInfo() )
	{
		if( g_pIconMng )
			g_pIconMng->RenderItem( pItem, pos + CsPoint( 6, 13 ), true );
		return;
	}

	CsMaster_Card::sINFO* pInfo = pCard->GetInfo();
	int const nGrade = GetSealGrade( pItem->GetType(), pItem->GetCount() );
	std::string const strBack = GetSealTierBackImage( nGrade );
	std::string const strImage = nsCsFileTable::g_pMaster_CardMng->GetSealDigimonImgPath( pInfo->s_nDigimonID );

	if( m_pSealCardBack[ nIndex ] && !strBack.empty() )
	{
		if( m_strSealCardBack[ nIndex ] != strBack )
		{
			m_pSealCardBack[ nIndex ]->ChangeTexture( strBack.c_str(), false );
			m_strSealCardBack[ nIndex ] = strBack;
		}
		m_pSealCardBack[ nIndex ]->SetColor( NiColor::WHITE );
		m_pSealCardBack[ nIndex ]->Render( pos, CsPoint( 47, 59 ) );
	}

	if( m_pSealCardImage[ nIndex ] && !strImage.empty() )
	{
		if( m_strSealCardImage[ nIndex ] != strImage )
		{
			m_pSealCardImage[ nIndex ]->ChangeTexture( strImage.c_str(), false );
			m_strSealCardImage[ nIndex ] = strImage;
		}
		m_pSealCardImage[ nIndex ]->SetColor( NiColor::WHITE );
		m_pSealCardImage[ nIndex ]->Render( pos + CsPoint( 1, 4 ), CsPoint( 45, 52 ) );
	}

	if( m_pSealCardCountBack[ nIndex ] )
		m_pSealCardCountBack[ nIndex ]->Render( pos + CsPoint( 0, 58 ), CsPoint( 47, 10 ) );

	if( m_pSealCardCountText[ nIndex ] )
	{
		std::wstring wsCount = DmCS::StringFn::getNumberFormatW( pItem->GetCount() );
		m_pSealCardCountText[ nIndex ]->SetText( wsCount.c_str() );
		m_pSealCardCountText[ nIndex ]->Render( pos + CsPoint( 23, 58 ) );
	}
}

void cExtraInventory::RenderSlotHover( int nIndex, CsPoint pos )
{
	if( nIndex < 0 || nIndex >= IF_EXTRA_INVENTORY_COUNT )
		return;

	if( m_IFIcon[ nIndex ].s_eState == sIFIcon::NONE )
		return;

	if( m_eCategory == CAT_SEAL && m_IFIcon[ nIndex ].s_eState == sIFIcon::MOUSE_ON && m_pSealHighlight )
	{
		m_pSealHighlight->Render( pos, GetSlotSize() );
		m_IFIcon[ nIndex ].ReleaseState();
		return;
	}

	m_IFIcon[ nIndex ].RenderMask( pos, GetSlotSize() );
}

void cExtraInventory::RenderStaticScrollThumb()
{
	if( !m_pScrollBar || m_pScrollBar->IsEnableScroll() )
		return;

	CsPoint ptRoot = GetRootClient();
	CsPoint ptThumb = ptRoot + IF_EXTRA_INVENTORY_SCROLL_POS + CsPoint( 0, 17 );

	if( m_pScrollStaticTop )
		m_pScrollStaticTop->Render( ptThumb );

	ptThumb.y += 8;
	if( m_pScrollStaticCenter )
		m_pScrollStaticCenter->Render( ptThumb );

	ptThumb.y += 28;
	if( m_pScrollStaticBottom )
		m_pScrollStaticBottom->Render( ptThumb );
}
