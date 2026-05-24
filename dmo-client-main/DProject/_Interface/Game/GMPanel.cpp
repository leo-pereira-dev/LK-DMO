#include "stdafx.h"
#include "GMPanel.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"

namespace
{
	const TCHAR* const GM_PANEL_PAGE_NAME[] =
	{
		_T( "Make Item" ),
		_T( "Desbug Char" ),
		_T( "Tamer EXP" ),
		_T( "Digimon EXP" ),
		_T( "AllEvo" ),
		_T( "Bits" )
	};

	const TCHAR* const GM_PANEL_PAGE_HELP[] =
	{
		_T( "Send an item to an online player. If target is empty, the item is created for you." ),
		_T( "Reloads your current character on the same map to clear stuck state." ),
		_T( "Levels your tamer. Leave value empty for max level, or type EXP to add." ),
		_T( "Levels your active Digimon. Leave value empty for max level, or type EXP to add." ),
		_T( "Unlocks all evolutions for your active Digimon and reloads the character." ),
		_T( "Adds bits to your current character inventory." )
	};

	static std::wstring GMPanelToLower( std::wstring value )
	{
		for( size_t i = 0; i < value.length(); ++i )
			value[i] = (wchar_t)towlower( value[i] );
		return value;
	}

	static bool GMPanelIsNumber( TCHAR const* szText )
	{
		if( szText == NULL || szText[0] == 0 )
			return false;

		for( int i = 0; szText[i] != 0; ++i )
		{
			if( _istdigit( szText[i] ) == 0 )
				return false;
		}

		return true;
	}
}

cGMPanel::cGMPanel()
: m_pMoveButton(NULL)
, m_pCancelButton(NULL)
, m_pActionBtn(NULL)
, m_pExitBtn(NULL)
, m_pTargetEdit(NULL)
, m_pItemSearchEdit(NULL)
, m_pAmountEdit(NULL)
, m_pValueEdit(NULL)
, m_pPageTitleText(NULL)
, m_pPageHelpText(NULL)
, m_pStatusText(NULL)
, m_pTargetLabel(NULL)
, m_pItemLabel(NULL)
, m_pAmountLabel(NULL)
, m_pValueLabel(NULL)
, m_nItemResultScroll(0)
, m_dwSelectedItemId(0)
, m_bApplyingSelection(false)
, m_ePage(PAGE_MAKE_ITEM)
{
	for( int i = 0; i < PAGE_COUNT; ++i )
		m_pPageBtn[i] = NULL;
	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		m_pItemResultBtn[i] = NULL;
}

void cGMPanel::Destroy()
{
	cBaseWindow::Delete();
}

void cGMPanel::DeleteResource()
{
	DeleteScript();
	m_pMoveButton = NULL;
	m_pCancelButton = NULL;
	m_pActionBtn = NULL;
	m_pExitBtn = NULL;
	m_pTargetEdit = NULL;
	m_pItemSearchEdit = NULL;
	m_pAmountEdit = NULL;
	m_pValueEdit = NULL;
	m_pPageTitleText = NULL;
	m_pPageHelpText = NULL;
	m_pStatusText = NULL;
	m_pTargetLabel = NULL;
	m_pItemLabel = NULL;
	m_pAmountLabel = NULL;
	m_pValueLabel = NULL;
	for( int i = 0; i < PAGE_COUNT; ++i )
		m_pPageBtn[i] = NULL;
	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		m_pItemResultBtn[i] = NULL;
	m_vItems.clear();
	m_vFilteredItems.clear();
	m_nItemResultScroll = 0;
	m_dwSelectedItemId = 0;
	m_bApplyingSelection = false;
	m_ePage = PAGE_MAKE_ITEM;
}

void cGMPanel::Create(int nValue /* = 0 */)
{
	if( cBaseWindow::Init() == false )
		return;

	int const nWidth = 886;
	int const nHeight = 600;
	int const nPosX = ( g_nScreenWidth / 2 ) - ( nWidth / 2 );
	int const nPosY = ( g_nScreenHeight / 2 ) - ( nHeight / 2 );

	InitScript( "Encyclopedia\\newencyclopedia\\main\\main_bg.png", CsPoint( nPosX, nPosY ), CsPoint( nWidth, nHeight ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	m_pMoveButton = AddButton( CsPoint( 827, 5 ), CsPoint( 22, 22 ), CsPoint( 0, 22 ), "System\\MoveWindow.tga" );
	m_pCancelButton = AddButton( CsPoint( 852, 3 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\main\\exit_btn.png" );

	cText::sTEXTINFO titleInfo;
	titleInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_20, FONT_WHITE );
	titleInfo.s_eTextAlign = DT_CENTER;
	titleInfo.s_bOutLine = true;
	titleInfo.SetText( _T( "GM Panel" ) );
	AddText( &titleInfo, CsPoint( 443, 34 ) );
	AddSprite( CsPoint( 341, 73 ), CsPoint( 205, 17 ), "Encyclopedia\\newencyclopedia\\main\\title.png" );

	AddSprite( CsPoint( 32, 82 ), CsPoint( 176, 470 ), "Encyclopedia\\newencyclopedia\\main\\black_sub bg.png" );
	AddSprite( CsPoint( 228, 82 ), CsPoint( 620, 470 ), "Encyclopedia\\newencyclopedia\\main\\black_sub bg.png" );

	cText::sTEXTINFO menuText;
	menuText.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
	menuText.s_eTextAlign = DT_CENTER;
	menuText.s_bOutLine = true;

	for( int i = 0; i < PAGE_COUNT; ++i )
	{
		int const nY = 105 + ( i * 63 );
		m_pPageBtn[i] = AddButton( CsPoint( 52, nY ), CsPoint( 136, 44 ), cButton::IMAGE_NOR_9, GM_PANEL_PAGE_NAME[i] );
		m_pPageBtn[i]->SetTextColor( NiColor( 0.72f, 0.92f, 1.0f ) );
	}

	cText::sTEXTINFO panelTitle;
	panelTitle.Init( &g_pEngine->m_FontSystem, CFont::FS_16, NiColor( 0.35f, 0.92f, 1.0f ) );
	panelTitle.s_bOutLine = true;
	panelTitle.SetText( GM_PANEL_PAGE_NAME[PAGE_MAKE_ITEM] );
	m_pPageTitleText = AddText( &panelTitle, CsPoint( 255, 104 ) );

	cText::sTEXTINFO helpText;
	helpText.Init( &g_pEngine->m_FontText, CFont::FS_12, NiColor( 0.70f, 0.86f, 1.0f ) );
	helpText.s_bOutLine = true;
	helpText.SetText( GM_PANEL_PAGE_HELP[PAGE_MAKE_ITEM] );
	m_pPageHelpText = AddText( &helpText, CsPoint( 255, 132 ) );

	cText::sTEXTINFO label;
	label.Init( &g_pEngine->m_FontText, CFont::FS_12, NiColor( 0.35f, 0.92f, 1.0f ) );
	label.s_bOutLine = true;

	cText::sTEXTINFO editText;
	editText.Init( &g_pEngine->m_FontText, CFont::FS_12, FONT_WHITE );
	editText.s_bOutLine = false;

	label.SetText( _T( "Target player" ) );
	m_pTargetLabel = AddText( &label, CsPoint( 255, 177 ) );
	m_pTargetEdit = NiNew cEditBox;
	m_pTargetEdit->Init( GetRoot(), CsPoint( 410, 172 ), CsPoint( 260, 24 ), &editText, false );
	m_pTargetEdit->SetEmptyMsgText( _T( "player name, blank = self" ), NiColor(0.55f,0.55f,0.55f) );
	m_pTargetEdit->SetFontLength( 20 );
	m_pTargetEdit->EnableUnderline( editText.s_Color );
	m_pTargetEdit->SetEnableSound( true );
	AddChildControl( m_pTargetEdit );

	label.SetText( _T( "Search item" ) );
	m_pItemLabel = AddText( &label, CsPoint( 255, 220 ) );
	m_pItemSearchEdit = NiNew cEditBox;
	m_pItemSearchEdit->Init( GetRoot(), CsPoint( 410, 215 ), CsPoint( 300, 24 ), &editText, false );
	m_pItemSearchEdit->SetEmptyMsgText( _T( "item name or id" ), NiColor(0.55f,0.55f,0.55f) );
	m_pItemSearchEdit->SetFontLength( 60 );
	m_pItemSearchEdit->EnableUnderline( editText.s_Color );
	m_pItemSearchEdit->SetEnableSound( true );
	m_pItemSearchEdit->AddEvent( cEditBox::eEditbox_ChangeText, this, &cGMPanel::_OnItemSearchChanged );
	AddChildControl( m_pItemSearchEdit );

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
	{
		m_pItemResultBtn[i] = AddButton( CsPoint( 410, 248 + ( i * 25 ) ), CsPoint( 300, 24 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
		m_pItemResultBtn[i]->SetTextColor( NiColor(0.70f,0.90f,1.0f) );
		m_pItemResultBtn[i]->SetVisible( false );
	}

	label.SetText( _T( "Amount" ) );
	m_pAmountLabel = AddText( &label, CsPoint( 255, 445 ) );
	m_pAmountEdit = NiNew cEditBox;
	m_pAmountEdit->Init( GetRoot(), CsPoint( 410, 440 ), CsPoint( 120, 24 ), &editText, false );
	m_pAmountEdit->SetText( _T( "1" ) );
	m_pAmountEdit->SetFontLength( 4 );
	m_pAmountEdit->EnableUnderline( editText.s_Color );
	m_pAmountEdit->SetEnableSound( true );
	AddChildControl( m_pAmountEdit );

	label.SetText( _T( "Value" ) );
	m_pValueLabel = AddText( &label, CsPoint( 255, 220 ) );
	m_pValueEdit = NiNew cEditBox;
	m_pValueEdit->Init( GetRoot(), CsPoint( 410, 215 ), CsPoint( 220, 24 ), &editText, false );
	m_pValueEdit->SetEmptyMsgText( _T( "optional value" ), NiColor(0.55f,0.55f,0.55f) );
	m_pValueEdit->SetFontLength( 10 );
	m_pValueEdit->EnableUnderline( editText.s_Color );
	m_pValueEdit->SetEnableSound( true );
	AddChildControl( m_pValueEdit );

	cText::sTEXTINFO statusInfo;
	statusInfo.Init( &g_pEngine->m_FontText, CFont::FS_12, NiColor( 0.62f, 0.92f, 1.0f ) );
	statusInfo.s_bOutLine = true;
	statusInfo.SetText( _T( "Ready." ) );
	m_pStatusText = AddText( &statusInfo, CsPoint( 255, 503 ) );

	m_pActionBtn = AddButton( CsPoint( 574, 438 ), CsPoint( 170, 36 ), cButton::IMAGE_NOR_9, _T( "Execute" ) );
	m_pActionBtn->SetTextColor( FONT_WHITE );
	m_pExitBtn = AddButton( CsPoint( 755, 506 ), CsPoint( 74, 27 ), cButton::IMAGE_NOR_5, UISTRING_TEXT( "MAINOPTION_BTN_EXIT" ).c_str() );

	_LoadItemsFromTable();
	_RefreshItemFilter();
	_SetPage( PAGE_MAKE_ITEM );
}

void cGMPanel::Update(float const& fDeltaTime)
{
	UpdateScript(fDeltaTime);
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cGMPanel::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
	{
		if( m_pCancelButton ) m_pCancelButton->Update_ForMouse();
		if( m_pMoveButton ) m_pMoveButton->Update_ForMouse();
		for( int i = 0; i < PAGE_COUNT; ++i )
			if( m_pPageBtn[i] ) m_pPageBtn[i]->Update_ForMouse();
		if( m_pActionBtn ) m_pActionBtn->Update_ForMouse();
		if( m_pExitBtn ) m_pExitBtn->Update_ForMouse();
		if( m_ePage == PAGE_MAKE_ITEM )
		{
			if( m_pTargetEdit ) m_pTargetEdit->Update_ForMouse( CsPoint::ZERO );
			if( m_pItemSearchEdit ) m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO );
			if( m_pAmountEdit ) m_pAmountEdit->Update_ForMouse( CsPoint::ZERO );
			for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
				if( m_pItemResultBtn[i] && m_pItemResultBtn[i]->GetVisible() ) m_pItemResultBtn[i]->Update_ForMouse();
		}
		else if( m_pValueEdit )
		{
			m_pValueEdit->Update_ForMouse( CsPoint::ZERO );
		}
		return muReturn;
	}

	switch( m_pCancelButton->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		Close();
	case cButton::ACTION_DOWN:
		return muReturn;
	}

	for( int i = 0; i < PAGE_COUNT; ++i )
	{
		switch( m_pPageBtn[i]->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			_SetPage( (eGMPage)i );
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( m_ePage == PAGE_MAKE_ITEM )
	{
		if( m_pTargetEdit && m_pTargetEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
		if( m_pItemSearchEdit && m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
		if( m_pAmountEdit && m_pAmountEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;

		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverItemResults() )
		{
			if( CURSOR_ST.GetWheel() < 0 )
				_ScrollItemResults( 1 );
			else if( CURSOR_ST.GetWheel() > 0 )
				_ScrollItemResults( -1 );

			CURSOR_ST.ResetWheel();
			return muReturn;
		}

		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		{
			if( m_pItemResultBtn[i] && m_pItemResultBtn[i]->GetVisible() )
			{
				switch( m_pItemResultBtn[i]->Update_ForMouse() )
				{
				case cButton::ACTION_CLICK:
					_SelectFilteredItem( i );
				case cButton::ACTION_DOWN:
					return muReturn;
				}
			}
		}
	}
	else
	{
		if( m_pValueEdit && m_pValueEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
	}

	switch( m_pActionBtn->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		_SendCurrentPageCommand();
	case cButton::ACTION_DOWN:
		return muReturn;
	}

	switch( m_pExitBtn->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		Close();
	case cButton::ACTION_DOWN:
		return muReturn;
	}

	_UpdateMoveWindow_ForMouse();
	return muReturn;
}

void cGMPanel::Render()
{
	RenderScript();
}

void cGMPanel::ResetDevice()
{
	ResetDeviceScript();
}

void cGMPanel::_SetStatus( TCHAR const* szText )
{
	if( m_pStatusText )
		m_pStatusText->SetText( szText );
}

void cGMPanel::_SetPage( eGMPage ePage )
{
	m_ePage = ePage;
	_RefreshPage();
}

void cGMPanel::_RefreshPage()
{
	if( m_pPageTitleText )
		m_pPageTitleText->SetText( GM_PANEL_PAGE_NAME[m_ePage] );
	if( m_pPageHelpText )
		m_pPageHelpText->SetText( GM_PANEL_PAGE_HELP[m_ePage] );

	for( int i = 0; i < PAGE_COUNT; ++i )
		if( m_pPageBtn[i] ) m_pPageBtn[i]->SetMouseOnMode( i == (int)m_ePage );

	bool const bMakeItem = ( m_ePage == PAGE_MAKE_ITEM );
	bool const bValuePage = ( m_ePage == PAGE_LEVEL_UP || m_ePage == PAGE_LEVEL_UP_DIGI || m_ePage == PAGE_BITS );

	if( m_pTargetLabel ) m_pTargetLabel->SetVisible( bMakeItem );
	if( m_pTargetEdit ) m_pTargetEdit->SetVisible( bMakeItem );
	if( m_pItemLabel ) m_pItemLabel->SetVisible( bMakeItem );
	if( m_pItemSearchEdit ) m_pItemSearchEdit->SetVisible( bMakeItem );
	if( m_pAmountLabel ) m_pAmountLabel->SetVisible( bMakeItem );
	if( m_pAmountEdit ) m_pAmountEdit->SetVisible( bMakeItem );

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		if( m_pItemResultBtn[i] ) m_pItemResultBtn[i]->SetVisible( bMakeItem && i < (int)m_vFilteredItems.size() );
	if( bMakeItem )
		_RefreshItemResultButtons();

	if( m_pValueLabel ) m_pValueLabel->SetVisible( bValuePage );
	if( m_pValueEdit ) m_pValueEdit->SetVisible( bValuePage );

	if( m_pActionBtn )
	{
		switch( m_ePage )
		{
		case PAGE_MAKE_ITEM:		m_pActionBtn->SetText( _T( "Create Item" ) );	break;
		case PAGE_DESBUG_CHAR:	m_pActionBtn->SetText( _T( "Desbug Char" ) );	break;
		case PAGE_LEVEL_UP:		m_pActionBtn->SetText( _T( "Tamer EXP" ) );		break;
		case PAGE_LEVEL_UP_DIGI:	m_pActionBtn->SetText( _T( "Digimon EXP" ) );	break;
		case PAGE_ALL_EVO:		m_pActionBtn->SetText( _T( "Unlock All Evo" ) );	break;
		case PAGE_BITS:			m_pActionBtn->SetText( _T( "Add Bits" ) );		break;
		}
	}

	if( m_ePage == PAGE_LEVEL_UP || m_ePage == PAGE_LEVEL_UP_DIGI )
	{
		if( m_pValueLabel )
			m_pValueLabel->SetText( _T( "EXP Value" ) );
		if( m_pValueEdit )
			m_pValueEdit->SetEmptyMsgText( _T( "blank = max level" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Blank value sends max level. A value sends EXP add." ) );
	}
	else if( m_ePage == PAGE_BITS )
	{
		if( m_pValueLabel )
			m_pValueLabel->SetText( _T( "Bits" ) );
		if( m_pValueEdit )
			m_pValueEdit->SetEmptyMsgText( _T( "amount" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type the bits amount to add." ) );
	}
	else if( m_ePage == PAGE_DESBUG_CHAR )
	{
		_SetStatus( _T( "This sends !reload for your current character." ) );
	}
	else if( m_ePage == PAGE_ALL_EVO )
	{
		_SetStatus( _T( "This sends !unlockevos for your active Digimon." ) );
	}
	else
	{
		_SetStatus( _T( "Type an item name or ID, select a result, then execute." ) );
	}
}

void cGMPanel::_LoadItemsFromTable()
{
	m_vItems.clear();

	if( nsCsFileTable::g_pItemMng == NULL || nsCsFileTable::g_pItemMng->GetItemMap() == NULL )
	{
		_SetStatus( _T( "Item table is not loaded yet." ) );
		return;
	}

	CsItem::MAP* pMap = nsCsFileTable::g_pItemMng->GetItemMap();
	for( CsItem::MAP_IT it = pMap->begin(); it != pMap->end(); ++it )
	{
		CsItem* pItem = it->second;
		if( pItem == NULL || pItem->GetInfo() == NULL )
			continue;

		CsItem::sINFO* pInfo = pItem->GetInfo();
		if( pInfo->s_dwItemID == 0 || pInfo->s_szName[0] == 0 )
			continue;

		sGMItemEntry entry;
		entry.s_dwItemId = pInfo->s_dwItemID;
		entry.s_wsName = pInfo->s_szName;
		entry.s_wsLowerName = GMPanelToLower( entry.s_wsName );

		TCHAR szDisplay[256] = { 0, };
		_stprintf_s( szDisplay, 256, _T( "%lu - %s" ), (unsigned long)entry.s_dwItemId, entry.s_wsName.c_str() );
		entry.s_wsDisplay = szDisplay;

		m_vItems.push_back( entry );
	}
}

void cGMPanel::_RefreshItemFilter()
{
	m_vFilteredItems.clear();

	TCHAR const* szSearch = m_pItemSearchEdit ? m_pItemSearchEdit->GetTextAll() : _T( "" );
	std::wstring wsSearch = szSearch ? szSearch : _T( "" );
	std::wstring wsLowerSearch = GMPanelToLower( wsSearch );
	m_nItemResultScroll = 0;

	if( wsLowerSearch.empty() )
	{
		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		{
			if( m_pItemResultBtn[i] )
				m_pItemResultBtn[i]->SetVisible( false );
		}
		_SetStatus( _T( "Type an item name or ID, then select a result." ) );
		return;
	}

	for( size_t i = 0; i < m_vItems.size(); ++i )
	{
		TCHAR szId[32] = { 0, };
		_stprintf_s( szId, 32, _T( "%lu" ), (unsigned long)m_vItems[i].s_dwItemId );

		if( m_vItems[i].s_wsLowerName.find( wsLowerSearch ) != std::wstring::npos ||
			std::wstring( szId ).find( wsLowerSearch ) != std::wstring::npos )
		{
			m_vFilteredItems.push_back( (int)i );
		}
	}

	_RefreshItemResultButtons();
}

void cGMPanel::_RefreshItemResultButtons()
{
	_ClampItemResultScroll();

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
	{
		if( m_pItemResultBtn[i] == NULL )
			continue;

		int const nFilteredIndex = m_nItemResultScroll + i;
		if( m_ePage == PAGE_MAKE_ITEM && nFilteredIndex < (int)m_vFilteredItems.size() )
		{
			sGMItemEntry const& entry = m_vItems[m_vFilteredItems[nFilteredIndex]];
			m_pItemResultBtn[i]->SetText( entry.s_wsDisplay.c_str() );
			m_pItemResultBtn[i]->SetVisible( true );
		}
		else
		{
			m_pItemResultBtn[i]->SetText( _T( "" ) );
			m_pItemResultBtn[i]->SetVisible( false );
		}
	}

	if( m_ePage != PAGE_MAKE_ITEM )
		return;

	if( m_vFilteredItems.empty() )
	{
		_SetStatus( _T( "No item found for this search." ) );
		return;
	}

	TCHAR szStatus[128] = { 0, };
	int const nFirst = m_nItemResultScroll + 1;
	int const nLast = min( m_nItemResultScroll + MAX_ITEM_RESULTS, (int)m_vFilteredItems.size() );

	if( (int)m_vFilteredItems.size() > MAX_ITEM_RESULTS )
		_stprintf_s( szStatus, 128, _T( "Showing %d-%d of %d. Use mouse wheel to scroll." ), nFirst, nLast, (int)m_vFilteredItems.size() );
	else
		_stprintf_s( szStatus, 128, _T( "Showing %d result%s." ), (int)m_vFilteredItems.size(), m_vFilteredItems.size() == 1 ? _T( "" ) : _T( "s" ) );

	_SetStatus( szStatus );
}

void cGMPanel::_ClampItemResultScroll()
{
	int const nMaxScroll = max( 0, (int)m_vFilteredItems.size() - MAX_ITEM_RESULTS );
	if( m_nItemResultScroll < 0 )
		m_nItemResultScroll = 0;
	if( m_nItemResultScroll > nMaxScroll )
		m_nItemResultScroll = nMaxScroll;
}

void cGMPanel::_ScrollItemResults( int nDelta )
{
	if( m_vFilteredItems.empty() )
		return;

	int const nOldScroll = m_nItemResultScroll;
	m_nItemResultScroll += nDelta;
	_ClampItemResultScroll();

	if( nOldScroll != m_nItemResultScroll )
		_RefreshItemResultButtons();
}

bool cGMPanel::_IsMouseOverItemResults()
{
	CsPoint const ptRoot = GetRootClient();
	return CURSOR_ST.IsInCursor(
		ptRoot.x + 405,
		ptRoot.y + 244,
		ptRoot.x + 720,
		ptRoot.y + 244 + ( MAX_ITEM_RESULTS * 25 ) );
}

void cGMPanel::_SelectFilteredItem( int nFilteredIndex )
{
	int const nActualFilteredIndex = m_nItemResultScroll + nFilteredIndex;
	if( nActualFilteredIndex < 0 || nActualFilteredIndex >= (int)m_vFilteredItems.size() )
		return;

	sGMItemEntry const& entry = m_vItems[m_vFilteredItems[nActualFilteredIndex]];
	if( m_pItemSearchEdit )
	{
		m_bApplyingSelection = true;
		m_pItemSearchEdit->SetText( entry.s_wsDisplay.c_str() );
		m_bApplyingSelection = false;
	}

	m_dwSelectedItemId = entry.s_dwItemId;
	_SetStatus( _T( "Item selected. Choose amount and execute." ) );
}

DWORD cGMPanel::_ResolveItemIdForSend()
{
	if( m_dwSelectedItemId != 0 )
		return m_dwSelectedItemId;

	TCHAR const* szItem = m_pItemSearchEdit ? m_pItemSearchEdit->GetTextAll() : NULL;
	if( szItem != NULL )
	{
		if( GMPanelIsNumber( szItem ) )
		{
			DWORD dwItemId = (DWORD)_ttoi( szItem );
			if( nsCsFileTable::g_pItemMng && nsCsFileTable::g_pItemMng->GetItem( dwItemId ) )
				return dwItemId;
		}

		if( _istdigit( szItem[0] ) )
		{
			DWORD dwItemId = (DWORD)_ttoi( szItem );
			if( nsCsFileTable::g_pItemMng && nsCsFileTable::g_pItemMng->GetItem( dwItemId ) )
				return dwItemId;
		}
	}

	if( !m_vFilteredItems.empty() )
		return m_vItems[m_vFilteredItems[m_nItemResultScroll]].s_dwItemId;

	return 0;
}

int cGMPanel::_ReadValueEdit( int nDefaultValue, int nMaxValue )
{
	TCHAR const* szValue = m_pValueEdit ? m_pValueEdit->GetTextAll() : NULL;
	if( szValue == NULL || szValue[0] == 0 || GMPanelIsNumber( szValue ) == false )
		return nDefaultValue;

	int nValue = _ttoi( szValue );
	if( nValue < 0 )
		nValue = nDefaultValue;
	if( nMaxValue > 0 && nValue > nMaxValue )
		nValue = nMaxValue;
	return nValue;
}

void cGMPanel::_SendCommand( TCHAR const* szCommand, TCHAR const* szStatus )
{
	if( szCommand == NULL || szCommand[0] == 0 )
		return;

	char szCommandLog[256] = { 0, };
#ifdef _UNICODE
	WideCharToMultiByte( CP_ACP, 0, szCommand, -1, szCommandLog, 256, NULL, NULL );
#else
	_sntprintf_s( szCommandLog, 256, _TRUNCATE, "%s", szCommand );
#endif
	nsCSDEBUG::CrashLogger::LogMessage( "GM_PANEL send page=%d command=%s", (int)m_ePage, szCommandLog );

	net::game->SendChatMsg( (wchar_t*)szCommand );
	_SetStatus( szStatus );
}

void cGMPanel::_SendCurrentPageCommand()
{
	TCHAR szCommand[256] = { 0, };
	TCHAR szStatus[160] = { 0, };

	switch( m_ePage )
	{
	case PAGE_MAKE_ITEM:
		{
			DWORD dwItemId = _ResolveItemIdForSend();
			int nAmount = m_pAmountEdit ? _ttoi( m_pAmountEdit->GetTextAll() ) : 1;
			TCHAR const* szTarget = m_pTargetEdit ? m_pTargetEdit->GetTextAll() : NULL;

			if( dwItemId == 0 )
			{
				_SetStatus( _T( "Type and select a valid item." ) );
				return;
			}

			if( nAmount <= 0 )
				nAmount = 1;
			if( nAmount > 9999 )
				nAmount = 9999;

			if( szTarget && szTarget[0] != 0 )
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!giveitem %s %lu %d" ), szTarget, (unsigned long)dwItemId, nAmount );
			else
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!item %lu %d" ), (unsigned long)dwItemId, nAmount );

			_stprintf_s( szStatus, 160, _T( "Request sent: item %lu x%d." ), (unsigned long)dwItemId, nAmount );
			_SendCommand( szCommand, szStatus );
		}
		break;

	case PAGE_DESBUG_CHAR:
		_SendCommand( _T( "!reload" ), _T( "Request sent: character reload." ) );
		break;

	case PAGE_LEVEL_UP:
		{
			int const nValue = _ReadValueEdit( 0, 999999999 );
			if( nValue > 0 )
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!tamer exp add %d" ), nValue );
			else
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!tamer exp max" ) );
			_SendCommand( szCommand, _T( "Request sent: tamer level up." ) );
		}
		break;

	case PAGE_LEVEL_UP_DIGI:
		{
			int const nValue = _ReadValueEdit( 0, 999999999 );
			if( nValue > 0 )
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!digimon exp add %d" ), nValue );
			else
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!digimon exp max" ) );
			_SendCommand( szCommand, _T( "Request sent: digimon level up." ) );
		}
		break;

	case PAGE_ALL_EVO:
		_SendCommand( _T( "!unlockevos" ), _T( "Request sent: unlock all evolutions." ) );
		break;

	case PAGE_BITS:
		{
			int const nValue = _ReadValueEdit( 0, 999999999 );
			if( nValue <= 0 )
			{
				_SetStatus( _T( "Type a valid bits amount." ) );
				return;
			}

			_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!currency bits %d" ), nValue );
			_stprintf_s( szStatus, 160, _T( "Request sent: %d bits." ), nValue );
			_SendCommand( szCommand, szStatus );
		}
		break;
	}
}

void cGMPanel::_OnItemSearchChanged( void* pkSender, void* pData )
{
	if( m_bApplyingSelection )
		return;

	m_dwSelectedItemId = 0;
	_RefreshItemFilter();
}
