#include "stdafx.h"
#include "GMPanel.h"

cGMPanel::cGMPanel()
: m_pCancelButton(NULL)
, m_pGiveItemBtn(NULL)
, m_pExitBtn(NULL)
, m_pTargetEdit(NULL)
, m_pItemSearchEdit(NULL)
, m_pAmountEdit(NULL)
, m_pStatusText(NULL)
, m_dwSelectedItemId(0)
, m_bApplyingSelection(false)
{
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
	m_pCancelButton = NULL;
	m_pGiveItemBtn = NULL;
	m_pExitBtn = NULL;
	m_pTargetEdit = NULL;
	m_pItemSearchEdit = NULL;
	m_pAmountEdit = NULL;
	m_pStatusText = NULL;
	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		m_pItemResultBtn[i] = NULL;
	m_vItems.clear();
	m_vFilteredItems.clear();
	m_dwSelectedItemId = 0;
	m_bApplyingSelection = false;
}

void cGMPanel::Create(int nValue /* = 0 */)
{
	if( cBaseWindow::Init() == false )
		return;

	InitScript( "MainOption\\Option_Win.tga", CsPoint( g_nScreenWidth/2 - 430/2, g_nScreenHeight/2 - 430/2 ), CsPoint( 430, 430 ), true, IFREGION_X::CENTER, IFREGION_Y::CENTER, false );
	AddTitle( _T( "Make Item" ) );

	m_pCancelButton = AddButton( CsPoint( 407, 7 ), CsPoint( 16, 16 ), CsPoint( 0, 16 ), "System\\Ch_Close.tga" );

	cText::sTEXTINFO label;
	label.Init( &g_pEngine->m_FontText, CFont::FS_12, FONT_WHITE );
	label.s_bOutLine = true;

	cText::sTEXTINFO editText;
	editText.Init( &g_pEngine->m_FontText, CFont::FS_12, FONT_WHITE );
	editText.s_bOutLine = false;

	label.SetText( _T( "Target player" ) );
	AddText( &label, CsPoint( 44, 55 ) );
	m_pTargetEdit = NiNew cEditBox;
	m_pTargetEdit->Init( GetRoot(), CsPoint( 170, 50 ), CsPoint( 205, 22 ), &editText, false );
	m_pTargetEdit->SetEmptyMsgText( _T( "player name" ), NiColor(0.55f,0.55f,0.55f) );
	m_pTargetEdit->SetFontLength( 20 );
	m_pTargetEdit->EnableUnderline( editText.s_Color );
	m_pTargetEdit->SetEnableSound( true );
	AddChildControl( m_pTargetEdit );

	label.SetText( _T( "Item" ) );
	AddText( &label, CsPoint( 44, 95 ) );
	m_pItemSearchEdit = NiNew cEditBox;
	m_pItemSearchEdit->Init( GetRoot(), CsPoint( 170, 90 ), CsPoint( 205, 22 ), &editText, false );
	m_pItemSearchEdit->SetEmptyMsgText( _T( "item name or id" ), NiColor(0.55f,0.55f,0.55f) );
	m_pItemSearchEdit->SetFontLength( 60 );
	m_pItemSearchEdit->EnableUnderline( editText.s_Color );
	m_pItemSearchEdit->SetEnableSound( true );
	m_pItemSearchEdit->AddEvent( cEditBox::eEditbox_ChangeText, this, &cGMPanel::_OnItemSearchChanged );
	AddChildControl( m_pItemSearchEdit );

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
	{
		m_pItemResultBtn[i] = AddButton( CsPoint( 170, 118 + ( i * 22 ) ), CsPoint( 205, 21 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
		m_pItemResultBtn[i]->SetTextColor( NiColor(0.70f,0.90f,1.0f) );
		m_pItemResultBtn[i]->SetVisible( false );
	}

	label.SetText( _T( "Amount" ) );
	AddText( &label, CsPoint( 44, 260 ) );
	m_pAmountEdit = NiNew cEditBox;
	m_pAmountEdit->Init( GetRoot(), CsPoint( 170, 255 ), CsPoint( 205, 22 ), &editText, false );
	m_pAmountEdit->SetText( _T( "1" ) );
	m_pAmountEdit->SetFontLength( 4 );
	m_pAmountEdit->EnableUnderline( editText.s_Color );
	m_pAmountEdit->SetEnableSound( true );
	AddChildControl( m_pAmountEdit );

	label.SetText( _T( "Type an item name or ID, then select a result." ) );
	m_pStatusText = AddText( &label, CsPoint( 44, 295 ) );

	m_pGiveItemBtn = AddButton( CsPoint( 127, 330 ), CsPoint( 174, 27 ), cButton::IMAGE_NOR_9, _T( "Give Item" ) );
	m_pExitBtn = AddButton( CsPoint( 127, 385 ), CsPoint( 174, 27 ), cButton::IMAGE_NOR_5, UISTRING_TEXT( "MAINOPTION_BTN_EXIT" ).c_str() );

	_LoadItemsFromTable();
	_RefreshItemFilter();
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
		m_pCancelButton->Update_ForMouse();
		m_pGiveItemBtn->Update_ForMouse();
		m_pExitBtn->Update_ForMouse();
		if( m_pTargetEdit ) m_pTargetEdit->Update_ForMouse( CsPoint::ZERO );
		if( m_pItemSearchEdit ) m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO );
		if( m_pAmountEdit ) m_pAmountEdit->Update_ForMouse( CsPoint::ZERO );
		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		{
			if( m_pItemResultBtn[i] && m_pItemResultBtn[i]->GetVisible() )
				m_pItemResultBtn[i]->Update_ForMouse();
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

	if( m_pTargetEdit && m_pTargetEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
		return muReturn;
	if( m_pItemSearchEdit && m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
		return muReturn;
	if( m_pAmountEdit && m_pAmountEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
		return muReturn;

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

	switch( m_pGiveItemBtn->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		_SendGiveItem();
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
	if( m_pStatusText && m_pStatusText->GetTextInfo() )
		m_pStatusText->GetTextInfo()->SetText( szText );
}

namespace
{
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

	if( wsLowerSearch.empty() )
	{
		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		{
			if( m_pItemResultBtn[i] )
				m_pItemResultBtn[i]->SetVisible( false );
		}
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
			if( m_vFilteredItems.size() >= MAX_ITEM_RESULTS )
				break;
		}
	}

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
	{
		if( m_pItemResultBtn[i] == NULL )
			continue;

		if( i < (int)m_vFilteredItems.size() )
		{
			sGMItemEntry const& entry = m_vItems[m_vFilteredItems[i]];
			m_pItemResultBtn[i]->SetText( entry.s_wsDisplay.c_str() );
			m_pItemResultBtn[i]->SetVisible( true );
		}
		else
		{
			m_pItemResultBtn[i]->SetText( _T( "" ) );
			m_pItemResultBtn[i]->SetVisible( false );
		}
	}
}

void cGMPanel::_SelectFilteredItem( int nFilteredIndex )
{
	if( nFilteredIndex < 0 || nFilteredIndex >= (int)m_vFilteredItems.size() )
		return;

	sGMItemEntry const& entry = m_vItems[m_vFilteredItems[nFilteredIndex]];
	if( m_pItemSearchEdit )
	{
		m_bApplyingSelection = true;
		m_pItemSearchEdit->SetText( entry.s_wsDisplay.c_str() );
		m_bApplyingSelection = false;
	}

	m_dwSelectedItemId = entry.s_dwItemId;
	_SetStatus( _T( "Item selected. Choose amount and send." ) );
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
		return m_vItems[m_vFilteredItems[0]].s_dwItemId;

	return 0;
}

void cGMPanel::_OnItemSearchChanged( void* pkSender, void* pData )
{
	if( m_bApplyingSelection )
		return;

	m_dwSelectedItemId = 0;
	_RefreshItemFilter();
}

void cGMPanel::_SendGiveItem()
{
	if( m_pTargetEdit == NULL || m_pItemSearchEdit == NULL || m_pAmountEdit == NULL )
		return;

	TCHAR const* szTarget = m_pTargetEdit->GetTextAll();
	TCHAR const* szAmount = m_pAmountEdit->GetTextAll();

	if( szTarget == NULL || _tcslen( szTarget ) == 0 )
	{
		_SetStatus( _T( "Type the target player name." ) );
		return;
	}

	DWORD dwItemId = _ResolveItemIdForSend();
	int nAmount = _ttoi( szAmount );

	if( dwItemId == 0 )
	{
		_SetStatus( _T( "Type and select a valid item." ) );
		return;
	}

	if( nAmount <= 0 )
		nAmount = 1;
	if( nAmount > 9999 )
		nAmount = 9999;

	wchar_t szCommand[256] = { 0, };
	_snwprintf_s( szCommand, 256, _TRUNCATE, L"!giveitem %s %lu %d", szTarget, (unsigned long)dwItemId, nAmount );
	net::game->SendChatMsg( szCommand );

	TCHAR szStatus[128] = { 0, };
	_stprintf_s( szStatus, 128, _T( "Request sent: item %lu x%d." ), (unsigned long)dwItemId, nAmount );
	_SetStatus( szStatus );
}
