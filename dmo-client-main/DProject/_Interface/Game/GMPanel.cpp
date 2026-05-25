#include "stdafx.h"
#include "GMPanel.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"

namespace
{
	const TCHAR* const GM_PANEL_PAGE_NAME[] =
	{
		_T( "Make Item" ),
		_T( "Summon Monster" ),
		_T( "Desbug Char" ),
		_T( "Tamer EXP" ),
		_T( "Digimon EXP" ),
		_T( "AllEvo" ),
		_T( "Full Clone" ),
		_T( "Bits" ),
		_T( "God On" ),
		_T( "God Off" ),
		_T( "Kick" ),
		_T( "Ban" ),
		_T( "Unban" ),
		_T( "Membership" )
	};

	const TCHAR* const GM_PANEL_PAGE_HELP[] =
	{
		_T( "Send an item to an online player. If target is empty, the item is created for you." ),
		_T( "Summons monsters near your current position using Monster.bin table IDs." ),
		_T( "Reloads your current character on the same map to clear stuck state." ),
		_T( "Levels your tamer. Leave value empty for max level, or type EXP to add." ),
		_T( "Levels your active Digimon. Leave value empty for max level, or type EXP to add." ),
		_T( "Unlocks all evolutions for your active Digimon and reloads the character." ),
		_T( "Sets AT/BL/CT/EV/HP clone to perfect 15 on your active Digimon." ),
		_T( "Adds bits to your current character inventory." ),
		_T( "Enables god mode for your current character." ),
		_T( "Disables god mode for your current character." ),
		_T( "Disconnects an online player by tamer name." ),
		_T( "Blocks an account by online tamer name or offline tamer name." ),
		_T( "Unblocks an account by username or tamer name." ),
		_T( "Adds membership days to your current account." )
	};

	const TCHAR* const GM_PANEL_SUMMON_PRESET_NAME[] =
	{
		_T( "Easy" ),
		_T( "Normal" ),
		_T( "Medium" ),
		_T( "Hard" ),
		_T( "Custom" )
	};

	const TCHAR* const GM_PANEL_SUMMON_PRESET_ARG[] =
	{
		_T( "easy" ),
		_T( "normal" ),
		_T( "medium" ),
		_T( "hard" ),
		_T( "custom" )
	};

	const TCHAR* const GM_PANEL_CUSTOM_STAT_NAME[] =
	{
		_T( "Level" ),
		_T( "HP" ),
		_T( "DS" ),
		_T( "AT" ),
		_T( "DE" ),
		_T( "HT" ),
		_T( "EV" ),
		_T( "CT" ),
		_T( "AS" )
	};

	static std::wstring GMPanelToLower( std::wstring value )
	{
		for( size_t i = 0; i < value.length(); ++i )
			value[i] = (wchar_t)towlower( value[i] );
		return value;
	}

	static std::wstring GMPanelNormalizeSearch( std::wstring value )
	{
		std::wstring result;
		result.reserve( value.length() );
		for( size_t i = 0; i < value.length(); ++i )
		{
			TCHAR const ch = (TCHAR)towlower( value[i] );
			if( _istalnum( ch ) != 0 )
				result.push_back( ch );
		}
		return result;
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

	static int GMPanelScaleStat( int nValue, int nPercent )
	{
		__int64 nScaled = ( (__int64)nValue * nPercent ) / 100;
		if( nScaled < 1 )
			nScaled = 1;
		if( nScaled > 999999999 )
			nScaled = 999999999;
		return (int)nScaled;
	}

	static int GMPanelPresetPercent( int nPreset )
	{
		switch( nPreset )
		{
		case 0:		return 60;
		case 2:		return 150;
		case 3:		return 250;
		case 4:		return 100;
		case 1:
		default:								return 100;
		}
	}

	static int GMPanelStatMax( int nIndex )
	{
		return nIndex == 0 ? 255 : 999999999;
	}
}

cGMPanel::cGMPanel()
: m_pMoveButton(NULL)
, m_pCancelButton(NULL)
, m_pPageScrollBar(NULL)
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
, m_pSummonCustomLabel(NULL)
, m_pSummonBossCheck(NULL)
, m_nItemResultScroll(0)
, m_nMonsterResultScroll(0)
, m_nPageScroll(0)
, m_dwSelectedItemId(0)
, m_dwSelectedMonsterId(0)
, m_eSummonPreset(SUMMON_PRESET_NORMAL)
, m_bApplyingSelection(false)
, m_ePage(PAGE_MAKE_ITEM)
{
	for( int i = 0; i < PAGE_COUNT; ++i )
		m_pPageBtn[i] = NULL;
	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		m_pItemResultBtn[i] = NULL;
	for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
		m_pSummonPresetBtn[i] = NULL;
	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		m_pSummonCustomStatLabel[i] = NULL;
		m_pSummonCustomStatEdit[i] = NULL;
	}
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
	m_pPageScrollBar = NULL;
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
	m_pSummonCustomLabel = NULL;
	m_pSummonBossCheck = NULL;
	for( int i = 0; i < PAGE_COUNT; ++i )
		m_pPageBtn[i] = NULL;
	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		m_pItemResultBtn[i] = NULL;
	for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
		m_pSummonPresetBtn[i] = NULL;
	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		m_pSummonCustomStatLabel[i] = NULL;
		m_pSummonCustomStatEdit[i] = NULL;
	}
	m_vItems.clear();
	m_vFilteredItems.clear();
	m_vMonsters.clear();
	m_vFilteredMonsters.clear();
	m_nItemResultScroll = 0;
	m_nMonsterResultScroll = 0;
	m_nPageScroll = 0;
	m_dwSelectedItemId = 0;
	m_dwSelectedMonsterId = 0;
	m_bApplyingSelection = false;
	m_ePage = PAGE_MAKE_ITEM;
}

void cGMPanel::Create(int nValue /* = 0 */)
{
	if( cBaseWindow::Init() == false )
		return;

	int const nWidth = 886;
	int const nHeight = 645;
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

	AddSprite( CsPoint( 32, 82 ), CsPoint( 176, 515 ), "Encyclopedia\\newencyclopedia\\main\\black_sub bg.png" );
	AddSprite( CsPoint( 228, 82 ), CsPoint( 620, 515 ), "Encyclopedia\\newencyclopedia\\main\\black_sub bg.png" );

	cText::sTEXTINFO menuText;
	menuText.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
	menuText.s_eTextAlign = DT_CENTER;
	menuText.s_bOutLine = true;

	for( int i = 0; i < PAGE_COUNT; ++i )
	{
		int const nY = 105 + ( ( i < MAX_MENU_VISIBLE ? i : 0 ) * 63 );
		m_pPageBtn[i] = AddButton( CsPoint( 52, nY ), CsPoint( 136, 44 ), cButton::IMAGE_NOR_9, GM_PANEL_PAGE_NAME[i] );
		m_pPageBtn[i]->SetTextColor( NiColor( 0.72f, 0.92f, 1.0f ) );
		m_pPageBtn[i]->SetVisible( i < MAX_MENU_VISIBLE );
	}

	m_pPageScrollBar = AddScrollBar( cScrollBar::TYPE_1, CsPoint( 190, 105 ), CsPoint( 16, 359 ), cScrollBar::GetDefaultBtnSize(), CsRect( CsPoint( 45, 100 ), CsPoint( 207, 515 ) ), MAX_MENU_VISIBLE );
	if( m_pPageScrollBar )
	{
		m_pPageScrollBar->SetEnableRenderFromEnableScroll( true );
		m_pPageScrollBar->SetRange( CsPoint( 0, PAGE_COUNT ) );
		m_pPageScrollBar->SetCurPosIndex( 0 );
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

	for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
	{
		m_pSummonPresetBtn[i] = AddButton( CsPoint( 255 + ( i * 70 ), 470 ), CsPoint( 64, 24 ), cButton::IMAGE_NOR_9, GM_PANEL_SUMMON_PRESET_NAME[i] );
		m_pSummonPresetBtn[i]->SetTextColor( NiColor(0.70f,0.90f,1.0f) );
		m_pSummonPresetBtn[i]->SetVisible( false );
	}

	cText::sTEXTINFO bossCheckText;
	bossCheckText.Init( &g_pEngine->m_FontText, CFont::FS_12, NiColor(0.70f,0.90f,1.0f) );
	bossCheckText.s_bOutLine = true;
	bossCheckText.SetText( _T( "Boss ranking" ) );
	m_pSummonBossCheck = AddCheckBox( CsPoint( 620, 470 ), CsPoint( 120, 24 ), cCheckBox::IMAGE_RADIO_5, &bossCheckText, cWindow::SD_Wi1 );
	if( m_pSummonBossCheck )
	{
		m_pSummonBossCheck->SetVisible( false );
		m_pSummonBossCheck->SetCheck( false );
	}

	label.SetText( _T( "Stats" ) );
	m_pSummonCustomLabel = AddText( &label, CsPoint( 255, 332 ) );
	m_pSummonCustomLabel->SetVisible( false );

	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		int const nCol = i % 3;
		int const nRow = i / 3;
		int const nX = 315 + ( nCol * 135 );
		int const nY = 332 + ( nRow * 38 );

		label.SetText( GM_PANEL_CUSTOM_STAT_NAME[i] );
		m_pSummonCustomStatLabel[i] = AddText( &label, CsPoint( nX, nY ) );
		m_pSummonCustomStatLabel[i]->SetVisible( false );

		m_pSummonCustomStatEdit[i] = NiNew cEditBox;
		m_pSummonCustomStatEdit[i]->Init( GetRoot(), CsPoint( nX + 48, nY - 5 ), CsPoint( 70, 24 ), &editText, false );
		m_pSummonCustomStatEdit[i]->SetEmptyMsgText( GM_PANEL_CUSTOM_STAT_NAME[i], NiColor(0.55f,0.55f,0.55f) );
		m_pSummonCustomStatEdit[i]->SetFontLength( 10 );
		m_pSummonCustomStatEdit[i]->EnableUnderline( editText.s_Color );
		m_pSummonCustomStatEdit[i]->SetEnableSound( true );
		m_pSummonCustomStatEdit[i]->SetVisible( false );
		AddChildControl( m_pSummonCustomStatEdit[i] );
	}

	label.SetText( _T( "Amount" ) );
	m_pAmountLabel = AddText( &label, CsPoint( 255, 512 ) );
	m_pAmountEdit = NiNew cEditBox;
	m_pAmountEdit->Init( GetRoot(), CsPoint( 410, 507 ), CsPoint( 120, 24 ), &editText, false );
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
	m_pStatusText = AddText( &statusInfo, CsPoint( 255, 558 ) );

	m_pActionBtn = AddButton( CsPoint( 574, 505 ), CsPoint( 170, 36 ), cButton::IMAGE_NOR_9, _T( "Execute" ) );
	m_pActionBtn->SetTextColor( FONT_WHITE );
	m_pExitBtn = AddButton( CsPoint( 755, 561 ), CsPoint( 74, 27 ), cButton::IMAGE_NOR_5, UISTRING_TEXT( "MAINOPTION_BTN_EXIT" ).c_str() );

	_LoadItemsFromTable();
	_LoadMonstersFromTable();
	_RefreshItemFilter();
	_RefreshMonsterFilter();
	_RefreshMenuButtons();
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
		if( m_pPageScrollBar ) m_pPageScrollBar->Update_ForMouse();
		for( int i = 0; i < PAGE_COUNT; ++i )
			if( m_pPageBtn[i] && m_pPageBtn[i]->GetVisible() ) m_pPageBtn[i]->Update_ForMouse();
		if( m_pActionBtn ) m_pActionBtn->Update_ForMouse();
		if( m_pExitBtn ) m_pExitBtn->Update_ForMouse();
		if( m_ePage == PAGE_MAKE_ITEM || m_ePage == PAGE_SUMMON_MONSTER )
		{
			if( m_ePage == PAGE_MAKE_ITEM && m_pTargetEdit ) m_pTargetEdit->Update_ForMouse( CsPoint::ZERO );
		if( m_pItemSearchEdit ) m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO );
		if( m_pAmountEdit ) m_pAmountEdit->Update_ForMouse( CsPoint::ZERO );
		if( m_ePage == PAGE_SUMMON_MONSTER && m_eSummonPreset == SUMMON_PRESET_CUSTOM )
		{
			for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
				if( m_pSummonCustomStatEdit[i] && m_pSummonCustomStatEdit[i]->GetVisible() )
					m_pSummonCustomStatEdit[i]->Update_ForMouse( CsPoint::ZERO );
		}
		if( m_ePage == PAGE_SUMMON_MONSTER && m_pSummonBossCheck && m_pSummonBossCheck->GetVisible() )
			m_pSummonBossCheck->Update_ForMouse();
		if( m_ePage == PAGE_SUMMON_MONSTER )
		{
			for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
				if( m_pSummonPresetBtn[i] && m_pSummonPresetBtn[i]->GetVisible() ) m_pSummonPresetBtn[i]->Update_ForMouse();
		}
		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
			if( m_pItemResultBtn[i] && m_pItemResultBtn[i]->GetVisible() ) m_pItemResultBtn[i]->Update_ForMouse();
	}
		else if( m_ePage == PAGE_KICK || m_ePage == PAGE_BAN || m_ePage == PAGE_UNBAN )
		{
			if( m_pTargetEdit ) m_pTargetEdit->Update_ForMouse( CsPoint::ZERO );
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

	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverMenu() )
	{
		if( CURSOR_ST.GetWheel() < 0 )
			_ScrollPages( 1 );
		else if( CURSOR_ST.GetWheel() > 0 )
			_ScrollPages( -1 );

		CURSOR_ST.ResetWheel();
		return muReturn;
	}

	if( m_pPageScrollBar )
	{
		int const nOldScroll = m_nPageScroll;
		if( m_pPageScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
		{
			m_nPageScroll = m_pPageScrollBar->GetCurPosIndex();
			_ClampPageScroll();
			if( nOldScroll != m_nPageScroll )
				_RefreshMenuButtons();
			return muReturn;
		}
	}

	for( int i = 0; i < PAGE_COUNT; ++i )
	{
		if( m_pPageBtn[i] == NULL || m_pPageBtn[i]->GetVisible() == false )
			continue;

		switch( m_pPageBtn[i]->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			if( i < MAX_MENU_VISIBLE )
			{
				int const nPageIndex = m_nPageScroll + i;
				if( nPageIndex >= 0 && nPageIndex < PAGE_COUNT )
					_SetPage( (eGMPage)nPageIndex );
			}
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( m_ePage == PAGE_MAKE_ITEM || m_ePage == PAGE_SUMMON_MONSTER )
	{
		if( m_ePage == PAGE_MAKE_ITEM && m_pTargetEdit && m_pTargetEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
		if( m_pItemSearchEdit && m_pItemSearchEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
		if( m_pAmountEdit && m_pAmountEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;
		if( m_ePage == PAGE_SUMMON_MONSTER )
		{
			for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
			{
				if( m_pSummonPresetBtn[i] == NULL || m_pSummonPresetBtn[i]->GetVisible() == false )
					continue;

				switch( m_pSummonPresetBtn[i]->Update_ForMouse() )
				{
				case cButton::ACTION_CLICK:
					m_eSummonPreset = (eSummonPreset)i;
					_RefreshSummonPresetButtons();
				case cButton::ACTION_DOWN:
					return muReturn;
				}
			}

			if( m_eSummonPreset == SUMMON_PRESET_CUSTOM )
			{
				for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
				{
					if( m_pSummonCustomStatEdit[i] && m_pSummonCustomStatEdit[i]->GetVisible() &&
						m_pSummonCustomStatEdit[i]->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
						return muReturn;
				}
			}

			if( m_pSummonBossCheck && m_pSummonBossCheck->GetVisible() )
			{
				switch( m_pSummonBossCheck->Update_ForMouse() )
				{
				case cCheckBox::ACTION_CLICK:
					_UpdateSummonPresetStatus();
				case cCheckBox::ACTION_DOWN:
					return muReturn;
				}
			}
		}

		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverItemResults() )
		{
			if( CURSOR_ST.GetWheel() < 0 )
			{
				if( m_ePage == PAGE_MAKE_ITEM )
					_ScrollItemResults( 1 );
				else
					_ScrollMonsterResults( 1 );
			}
			else if( CURSOR_ST.GetWheel() > 0 )
			{
				if( m_ePage == PAGE_MAKE_ITEM )
					_ScrollItemResults( -1 );
				else
					_ScrollMonsterResults( -1 );
			}

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
					if( m_ePage == PAGE_MAKE_ITEM )
						_SelectFilteredItem( i );
					else
						_SelectFilteredMonster( i );
				case cButton::ACTION_DOWN:
					return muReturn;
				}
			}
		}
	}
	else
	{
		if( ( m_ePage == PAGE_KICK || m_ePage == PAGE_BAN || m_ePage == PAGE_UNBAN ) &&
			m_pTargetEdit && m_pTargetEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
			return muReturn;

		if( m_pValueEdit && m_pValueEdit->GetVisible() && m_pValueEdit->Update_ForMouse( CsPoint::ZERO ) == cEditBox::ACTION_CLICK )
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

	_RefreshMenuButtons();

	bool const bMakeItem = ( m_ePage == PAGE_MAKE_ITEM );
	bool const bSummonMonster = ( m_ePage == PAGE_SUMMON_MONSTER );
	bool const bSearchPage = ( bMakeItem || bSummonMonster );
	bool const bTargetPage = ( m_ePage == PAGE_KICK || m_ePage == PAGE_BAN || m_ePage == PAGE_UNBAN );
	bool const bValuePage = ( m_ePage == PAGE_LEVEL_UP || m_ePage == PAGE_LEVEL_UP_DIGI || m_ePage == PAGE_BITS || m_ePage == PAGE_MEMBERSHIP );

	if( m_pTargetLabel ) m_pTargetLabel->SetVisible( bMakeItem || bTargetPage );
	if( m_pTargetEdit ) m_pTargetEdit->SetVisible( bMakeItem || bTargetPage );
	if( m_pItemLabel ) m_pItemLabel->SetVisible( bSearchPage );
	if( m_pItemSearchEdit ) m_pItemSearchEdit->SetVisible( bSearchPage );
	if( m_pAmountLabel ) m_pAmountLabel->SetVisible( bSearchPage );
	if( m_pAmountEdit ) m_pAmountEdit->SetVisible( bSearchPage );
	for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
		if( m_pSummonPresetBtn[i] ) m_pSummonPresetBtn[i]->SetVisible( bSummonMonster );
	if( m_pSummonBossCheck ) m_pSummonBossCheck->SetVisible( bSummonMonster );
	if( m_pSummonCustomLabel ) m_pSummonCustomLabel->SetVisible( bSummonMonster );
	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		if( m_pSummonCustomStatLabel[i] ) m_pSummonCustomStatLabel[i]->SetVisible( bSummonMonster );
		if( m_pSummonCustomStatEdit[i] ) m_pSummonCustomStatEdit[i]->SetVisible( bSummonMonster );
	}
	if( bSummonMonster )
		_RefreshSummonPresetButtons();

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		if( m_pItemResultBtn[i] ) m_pItemResultBtn[i]->SetVisible( false );
	if( bMakeItem )
		_RefreshItemResultButtons();
	else if( bSummonMonster )
		_RefreshMonsterResultButtons();

	if( m_pValueLabel ) m_pValueLabel->SetVisible( bValuePage );
	if( m_pValueEdit ) m_pValueEdit->SetVisible( bValuePage );

	if( m_pActionBtn )
	{
		switch( m_ePage )
		{
		case PAGE_MAKE_ITEM:		m_pActionBtn->SetText( _T( "Create Item" ) );	break;
		case PAGE_SUMMON_MONSTER:	m_pActionBtn->SetText( _T( "Summon" ) );	break;
		case PAGE_DESBUG_CHAR:	m_pActionBtn->SetText( _T( "Desbug Char" ) );	break;
		case PAGE_LEVEL_UP:		m_pActionBtn->SetText( _T( "Tamer EXP" ) );		break;
		case PAGE_LEVEL_UP_DIGI:	m_pActionBtn->SetText( _T( "Digimon EXP" ) );	break;
		case PAGE_ALL_EVO:		m_pActionBtn->SetText( _T( "Unlock All Evo" ) );	break;
		case PAGE_FULL_CLONE:	m_pActionBtn->SetText( _T( "Full Clone" ) );	break;
		case PAGE_BITS:			m_pActionBtn->SetText( _T( "Add Bits" ) );		break;
		case PAGE_GOD_ON:		m_pActionBtn->SetText( _T( "God On" ) );		break;
		case PAGE_GOD_OFF:		m_pActionBtn->SetText( _T( "God Off" ) );		break;
		case PAGE_KICK:			m_pActionBtn->SetText( _T( "Kick Player" ) );	break;
		case PAGE_BAN:			m_pActionBtn->SetText( _T( "Ban Player" ) );	break;
		case PAGE_UNBAN:		m_pActionBtn->SetText( _T( "Unban Account" ) );	break;
		case PAGE_MEMBERSHIP:	m_pActionBtn->SetText( _T( "Add Days" ) );		break;
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
	else if( m_ePage == PAGE_SUMMON_MONSTER )
	{
		if( m_pItemLabel )
			m_pItemLabel->SetText( _T( "Search monster" ) );
		if( m_pItemSearchEdit )
			m_pItemSearchEdit->SetEmptyMsgText( _T( "monster name or id" ), NiColor(0.55f,0.55f,0.55f) );
		if( m_pAmountLabel )
			m_pAmountLabel->SetText( _T( "Amount" ) );
		if( m_eSummonPreset == SUMMON_PRESET_CUSTOM )
			_SetStatus( _T( "Custom order: Level HP DS AT DE HT EV CT AS." ) );
		else
			_SetStatus( _T( "Select a monster and preset, then summon near your position." ) );
		_RefreshMonsterFilter();
	}
	else if( m_ePage == PAGE_BITS )
	{
		if( m_pValueLabel )
			m_pValueLabel->SetText( _T( "Bits" ) );
		if( m_pValueEdit )
			m_pValueEdit->SetEmptyMsgText( _T( "amount" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type the bits amount to add." ) );
	}
	else if( m_ePage == PAGE_MEMBERSHIP )
	{
		if( m_pValueLabel )
			m_pValueLabel->SetText( _T( "Days" ) );
		if( m_pValueEdit )
			m_pValueEdit->SetEmptyMsgText( _T( "days" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type the membership days to add." ) );
	}
	else if( m_ePage == PAGE_DESBUG_CHAR )
	{
		_SetStatus( _T( "This sends !reload for your current character." ) );
	}
	else if( m_ePage == PAGE_ALL_EVO )
	{
		_SetStatus( _T( "This sends !unlockevos for your active Digimon." ) );
	}
	else if( m_ePage == PAGE_FULL_CLONE )
	{
		_SetStatus( _T( "This sends !digimon clone full for your active Digimon." ) );
	}
	else if( m_ePage == PAGE_GOD_ON || m_ePage == PAGE_GOD_OFF )
	{
		_SetStatus( _T( "This toggles god mode on your current character." ) );
	}
	else if( m_ePage == PAGE_KICK )
	{
		if( m_pTargetLabel )
			m_pTargetLabel->SetText( _T( "Target player" ) );
		if( m_pTargetEdit )
			m_pTargetEdit->SetEmptyMsgText( _T( "online tamer name" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type an online tamer name to disconnect." ) );
	}
	else if( m_ePage == PAGE_BAN )
	{
		if( m_pTargetLabel )
			m_pTargetLabel->SetText( _T( "Target player" ) );
		if( m_pTargetEdit )
			m_pTargetEdit->SetEmptyMsgText( _T( "tamer name" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type a tamer name to block its account." ) );
	}
	else if( m_ePage == PAGE_UNBAN )
	{
		if( m_pTargetLabel )
			m_pTargetLabel->SetText( _T( "Account/Tamer" ) );
		if( m_pTargetEdit )
			m_pTargetEdit->SetEmptyMsgText( _T( "username or tamer name" ), NiColor(0.55f,0.55f,0.55f) );
		_SetStatus( _T( "Type an account username or tamer name to unblock." ) );
	}
	else
	{
		if( m_pTargetLabel )
			m_pTargetLabel->SetText( _T( "Target player" ) );
		if( m_pTargetEdit )
			m_pTargetEdit->SetEmptyMsgText( _T( "player name, blank = self" ), NiColor(0.55f,0.55f,0.55f) );
		if( m_pItemLabel )
			m_pItemLabel->SetText( _T( "Search item" ) );
		if( m_pItemSearchEdit )
			m_pItemSearchEdit->SetEmptyMsgText( _T( "item name or id" ), NiColor(0.55f,0.55f,0.55f) );
		if( m_pAmountLabel )
			m_pAmountLabel->SetText( _T( "Amount" ) );
		_SetStatus( _T( "Type an item name or ID, select a result, then execute." ) );
	}
}

void cGMPanel::_RefreshMenuButtons()
{
	_ClampPageScroll();

	if( m_pPageScrollBar )
	{
		m_pPageScrollBar->SetRenderCount( MAX_MENU_VISIBLE );
		m_pPageScrollBar->SetRange( CsPoint( 0, PAGE_COUNT ) );
		m_pPageScrollBar->SetCurPosIndex( m_nPageScroll );
	}

	for( int i = 0; i < PAGE_COUNT; ++i )
	{
		if( m_pPageBtn[i] == NULL )
			continue;

		bool const bVisible = ( i < MAX_MENU_VISIBLE && ( m_nPageScroll + i ) < PAGE_COUNT );
		m_pPageBtn[i]->SetVisible( bVisible );
		if( bVisible )
		{
			int const nPageIndex = m_nPageScroll + i;
			CsPoint ptButton( 52, 105 + ( i * 63 ) );
			m_pPageBtn[i]->SetPos( ptButton );
			m_pPageBtn[i]->SetText( GM_PANEL_PAGE_NAME[nPageIndex] );
			m_pPageBtn[i]->SetTextColor( NiColor( 0.72f, 0.92f, 1.0f ) );
			m_pPageBtn[i]->SetMouseOnMode( nPageIndex == (int)m_ePage );
		}
		else
		{
			m_pPageBtn[i]->SetMouseOnMode( false );
		}
	}
}

void cGMPanel::_ClampPageScroll()
{
	int const nMaxScroll = max( 0, PAGE_COUNT - MAX_MENU_VISIBLE );
	if( m_nPageScroll < 0 )
		m_nPageScroll = 0;
	if( m_nPageScroll > nMaxScroll )
		m_nPageScroll = nMaxScroll;
}

void cGMPanel::_ScrollPages( int nDelta )
{
	int const nOldScroll = m_nPageScroll;
	m_nPageScroll += nDelta;
	_ClampPageScroll();

	if( nOldScroll != m_nPageScroll )
		_RefreshMenuButtons();
}

bool cGMPanel::_IsMouseOverMenu()
{
	CsPoint const ptRoot = GetRootClient();
	return CURSOR_ST.IsInCursor(
		ptRoot.x + 45,
		ptRoot.y + 100,
		ptRoot.x + 208,
		ptRoot.y + 470 );
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
		entry.s_wsSearchName = GMPanelNormalizeSearch( entry.s_wsName );

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
	std::wstring wsNormalizedSearch = GMPanelNormalizeSearch( wsSearch );
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
			( !wsNormalizedSearch.empty() && m_vItems[i].s_wsSearchName.find( wsNormalizedSearch ) != std::wstring::npos ) ||
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

void cGMPanel::_LoadMonstersFromTable()
{
	m_vMonsters.clear();

	if( nsCsFileTable::g_pMonsterMng == NULL || nsCsFileTable::g_pMonsterMng->GetMonsterMap() == NULL )
	{
		_SetStatus( _T( "Monster table is not loaded yet." ) );
		return;
	}

	CsMonster::MAP* pMap = nsCsFileTable::g_pMonsterMng->GetMonsterMap();
	for( CsMonster::MAP_IT it = pMap->begin(); it != pMap->end(); ++it )
	{
		CsMonster* pMonster = it->second;
		if( pMonster == NULL || pMonster->GetInfo() == NULL )
			continue;

		CsMonster::sINFO* pInfo = pMonster->GetInfo();
		if( pInfo->s_dwMonsterID == 0 || pInfo->s_szName[0] == 0 )
			continue;

		sGMMonsterEntry entry;
		entry.s_dwMonsterId = pInfo->s_dwMonsterID;
		entry.s_wsName = pInfo->s_szName;
		entry.s_wsLowerName = GMPanelToLower( entry.s_wsName );
		entry.s_wsSearchName = GMPanelNormalizeSearch( entry.s_wsName );
		entry.s_nLevel = pInfo->s_nLevel;
		entry.s_nHP = pInfo->s_nHP;
		entry.s_nDS = pInfo->s_nDS;
		entry.s_nAT = pInfo->s_nAttPower;
		entry.s_nDE = pInfo->s_nDefPower;
		entry.s_nHT = pInfo->s_nHitRate;
		entry.s_nEV = pInfo->s_nEvasion;
		entry.s_nCT = pInfo->s_nCritcalRate;
		entry.s_nAS = pInfo->s_nAttSpeed;

		TCHAR szDisplay[256] = { 0, };
		_stprintf_s( szDisplay, 256, _T( "%lu - %s" ), (unsigned long)entry.s_dwMonsterId, entry.s_wsName.c_str() );
		entry.s_wsDisplay = szDisplay;

		m_vMonsters.push_back( entry );
	}
}

void cGMPanel::_RefreshMonsterFilter()
{
	m_vFilteredMonsters.clear();

	TCHAR const* szSearch = m_pItemSearchEdit ? m_pItemSearchEdit->GetTextAll() : _T( "" );
	std::wstring wsSearch = szSearch ? szSearch : _T( "" );
	std::wstring wsLowerSearch = GMPanelToLower( wsSearch );
	std::wstring wsNormalizedSearch = GMPanelNormalizeSearch( wsSearch );
	m_nMonsterResultScroll = 0;

	if( wsLowerSearch.empty() )
	{
		for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
		{
			if( m_pItemResultBtn[i] )
				m_pItemResultBtn[i]->SetVisible( false );
		}
		if( m_ePage == PAGE_SUMMON_MONSTER )
			_SetStatus( _T( "Type a monster name or ID, then select a result." ) );
		return;
	}

	for( size_t i = 0; i < m_vMonsters.size(); ++i )
	{
		TCHAR szId[32] = { 0, };
		_stprintf_s( szId, 32, _T( "%lu" ), (unsigned long)m_vMonsters[i].s_dwMonsterId );

		if( m_vMonsters[i].s_wsLowerName.find( wsLowerSearch ) != std::wstring::npos ||
			( !wsNormalizedSearch.empty() && m_vMonsters[i].s_wsSearchName.find( wsNormalizedSearch ) != std::wstring::npos ) ||
			std::wstring( szId ).find( wsLowerSearch ) != std::wstring::npos )
		{
			m_vFilteredMonsters.push_back( (int)i );
		}
	}

	_RefreshMonsterResultButtons();
}

void cGMPanel::_RefreshMonsterResultButtons()
{
	_ClampMonsterResultScroll();

	for( int i = 0; i < MAX_ITEM_RESULTS; ++i )
	{
		if( m_pItemResultBtn[i] == NULL )
			continue;

		int const nFilteredIndex = m_nMonsterResultScroll + i;
		if( m_ePage == PAGE_SUMMON_MONSTER && i < 3 && nFilteredIndex < (int)m_vFilteredMonsters.size() )
		{
			sGMMonsterEntry const& entry = m_vMonsters[m_vFilteredMonsters[nFilteredIndex]];
			m_pItemResultBtn[i]->SetText( entry.s_wsDisplay.c_str() );
			m_pItemResultBtn[i]->SetVisible( true );
		}
		else
		{
			m_pItemResultBtn[i]->SetText( _T( "" ) );
			m_pItemResultBtn[i]->SetVisible( false );
		}
	}

	if( m_ePage != PAGE_SUMMON_MONSTER )
		return;

	if( m_vFilteredMonsters.empty() )
	{
		_SetStatus( _T( "No monster found for this search." ) );
		return;
	}

	_UpdateSummonPresetStatus();
}

void cGMPanel::_ClampMonsterResultScroll()
{
	int const nMaxScroll = max( 0, (int)m_vFilteredMonsters.size() - 3 );
	if( m_nMonsterResultScroll < 0 )
		m_nMonsterResultScroll = 0;
	if( m_nMonsterResultScroll > nMaxScroll )
		m_nMonsterResultScroll = nMaxScroll;
}

void cGMPanel::_ScrollMonsterResults( int nDelta )
{
	if( m_vFilteredMonsters.empty() )
		return;

	int const nOldScroll = m_nMonsterResultScroll;
	m_nMonsterResultScroll += nDelta;
	_ClampMonsterResultScroll();

	if( nOldScroll != m_nMonsterResultScroll )
		_RefreshMonsterResultButtons();
}

void cGMPanel::_SelectFilteredMonster( int nFilteredIndex )
{
	int const nActualFilteredIndex = m_nMonsterResultScroll + nFilteredIndex;
	if( nActualFilteredIndex < 0 || nActualFilteredIndex >= (int)m_vFilteredMonsters.size() )
		return;

	sGMMonsterEntry const& entry = m_vMonsters[m_vFilteredMonsters[nActualFilteredIndex]];
	if( m_pItemSearchEdit )
	{
		m_bApplyingSelection = true;
		m_pItemSearchEdit->SetText( entry.s_wsDisplay.c_str() );
		m_bApplyingSelection = false;
	}

	m_dwSelectedMonsterId = entry.s_dwMonsterId;
	_UpdateSummonPresetStatus();
}

DWORD cGMPanel::_ResolveMonsterIdForSend()
{
	if( m_dwSelectedMonsterId != 0 )
		return m_dwSelectedMonsterId;

	TCHAR const* szMonster = m_pItemSearchEdit ? m_pItemSearchEdit->GetTextAll() : NULL;
	if( szMonster != NULL )
	{
		if( GMPanelIsNumber( szMonster ) )
		{
			DWORD dwMonsterId = (DWORD)_ttoi( szMonster );
			if( nsCsFileTable::g_pMonsterMng && nsCsFileTable::g_pMonsterMng->GetMonster( dwMonsterId ) )
				return dwMonsterId;
		}

		if( _istdigit( szMonster[0] ) )
		{
			DWORD dwMonsterId = (DWORD)_ttoi( szMonster );
			if( nsCsFileTable::g_pMonsterMng && nsCsFileTable::g_pMonsterMng->GetMonster( dwMonsterId ) )
				return dwMonsterId;
		}
	}

	if( !m_vFilteredMonsters.empty() )
		return m_vMonsters[m_vFilteredMonsters[m_nMonsterResultScroll]].s_dwMonsterId;

	return 0;
}

void cGMPanel::_RefreshSummonPresetButtons()
{
	for( int i = 0; i < SUMMON_PRESET_COUNT; ++i )
	{
		if( m_pSummonPresetBtn[i] == NULL )
			continue;

		m_pSummonPresetBtn[i]->SetMouseOnMode( i == (int)m_eSummonPreset );
		m_pSummonPresetBtn[i]->SetTextColor( i == (int)m_eSummonPreset ? NiColor( 0.0f, 1.0f, 0.25f ) : NiColor(0.70f,0.90f,1.0f) );
	}

	bool const bSummon = ( m_ePage == PAGE_SUMMON_MONSTER );
	if( m_pSummonCustomLabel ) m_pSummonCustomLabel->SetVisible( bSummon );
	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		if( m_pSummonCustomStatLabel[i] ) m_pSummonCustomStatLabel[i]->SetVisible( bSummon );
		if( m_pSummonCustomStatEdit[i] ) m_pSummonCustomStatEdit[i]->SetVisible( bSummon );
	}
	_UpdateSummonPresetStatus();
}

void cGMPanel::_UpdateSummonPresetStatus()
{
	if( m_ePage != PAGE_SUMMON_MONSTER )
		return;

	DWORD const dwMonsterId = _ResolveMonsterIdForSend();
	if( dwMonsterId == 0 )
	{
		for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
			if( m_pSummonCustomStatEdit[i] ) m_pSummonCustomStatEdit[i]->SetText( _T( "" ) );
		_SetStatus( _T( "Type a monster name or ID, then select a result." ) );
		return;
	}

	sGMMonsterEntry const* pEntry = NULL;
	for( size_t i = 0; i < m_vMonsters.size(); ++i )
	{
		if( m_vMonsters[i].s_dwMonsterId == dwMonsterId )
		{
			pEntry = &m_vMonsters[i];
			break;
		}
	}

	if( pEntry == NULL )
	{
		_SetStatus( _T( "Monster selected. Choose preset and summon." ) );
		return;
	}

	if( m_eSummonPreset == SUMMON_PRESET_CUSTOM )
	{
		_FillSummonStatFields( *pEntry, 100 );
		_SetStatus( _T( "Custom order: Level HP DS AT DE HT EV CT AS." ) );
		return;
	}

	int const nPercent = GMPanelPresetPercent( (int)m_eSummonPreset );
	int const nLevel = min( 255, max( 1, GMPanelScaleStat( pEntry->s_nLevel, nPercent ) ) );
	_FillSummonStatFields( *pEntry, nPercent );
	TCHAR szStatus[256] = { 0, };
	_stprintf_s(
		szStatus,
		256,
		_T( "%s%s: Lv %d HP %d DS %d AT %d DE %d HT %d EV %d CT %d AS %d" ),
		GM_PANEL_SUMMON_PRESET_NAME[(int)m_eSummonPreset],
		( m_pSummonBossCheck && m_pSummonBossCheck->IsCheck() ) ? _T( " Boss" ) : _T( "" ),
		nLevel,
		GMPanelScaleStat( pEntry->s_nHP, nPercent ),
		GMPanelScaleStat( pEntry->s_nDS, nPercent ),
		GMPanelScaleStat( pEntry->s_nAT, nPercent ),
		GMPanelScaleStat( pEntry->s_nDE, nPercent ),
		GMPanelScaleStat( pEntry->s_nHT, nPercent ),
		GMPanelScaleStat( pEntry->s_nEV, nPercent ),
		GMPanelScaleStat( pEntry->s_nCT, nPercent ),
		GMPanelScaleStat( pEntry->s_nAS, nPercent ) );
	_SetStatus( szStatus );
}

void cGMPanel::_FillSummonStatFields( sGMMonsterEntry const& entry, int nPercent )
{
	int nValues[SUMMON_CUSTOM_STAT_COUNT] =
	{
		min( 255, max( 1, GMPanelScaleStat( entry.s_nLevel, nPercent ) ) ),
		GMPanelScaleStat( entry.s_nHP, nPercent ),
		GMPanelScaleStat( entry.s_nDS, nPercent ),
		GMPanelScaleStat( entry.s_nAT, nPercent ),
		GMPanelScaleStat( entry.s_nDE, nPercent ),
		GMPanelScaleStat( entry.s_nHT, nPercent ),
		GMPanelScaleStat( entry.s_nEV, nPercent ),
		GMPanelScaleStat( entry.s_nCT, nPercent ),
		GMPanelScaleStat( entry.s_nAS, nPercent )
	};

	for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
	{
		if( m_pSummonCustomStatEdit[i] == NULL )
			continue;

		TCHAR szValue[32] = { 0, };
		_stprintf_s( szValue, 32, _T( "%d" ), min( GMPanelStatMax( i ), max( 1, nValues[i] ) ) );
		m_pSummonCustomStatEdit[i]->SetText( szValue );
	}
}

int cGMPanel::_ReadSummonCustomStat( int nIndex, int nDefaultValue )
{
	if( nIndex < 0 || nIndex >= SUMMON_CUSTOM_STAT_COUNT )
		return nDefaultValue;

	TCHAR const* szValue = m_pSummonCustomStatEdit[nIndex] ? m_pSummonCustomStatEdit[nIndex]->GetTextAll() : NULL;
	if( szValue == NULL || szValue[0] == 0 || GMPanelIsNumber( szValue ) == false )
		return nDefaultValue;

	__int64 nValue = _ttoi64( szValue );
	if( nValue < 1 )
		nValue = 1;
	if( nValue > GMPanelStatMax( nIndex ) )
		nValue = GMPanelStatMax( nIndex );

	if( m_pSummonCustomStatEdit[nIndex] )
	{
		TCHAR szClamped[32] = { 0, };
		_stprintf_s( szClamped, 32, _T( "%d" ), (int)nValue );
		m_pSummonCustomStatEdit[nIndex]->SetText( szClamped );
	}

	return (int)nValue;
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

	case PAGE_SUMMON_MONSTER:
		{
			DWORD dwMonsterId = _ResolveMonsterIdForSend();
			int nAmount = m_pAmountEdit ? _ttoi( m_pAmountEdit->GetTextAll() ) : 1;

			if( dwMonsterId == 0 )
			{
				_SetStatus( _T( "Type and select a valid monster." ) );
				return;
			}

			if( nAmount <= 0 )
				nAmount = 1;
			if( nAmount > 50 )
				nAmount = 50;

			if( m_eSummonPreset == SUMMON_PRESET_CUSTOM )
			{
				TCHAR szCustom[256] = { 0, };
				for( int i = 0; i < SUMMON_CUSTOM_STAT_COUNT; ++i )
				{
					TCHAR const* szValue = m_pSummonCustomStatEdit[i] ? m_pSummonCustomStatEdit[i]->GetTextAll() : NULL;
					if( szValue == NULL || szValue[0] == 0 || GMPanelIsNumber( szValue ) == false )
					{
						TCHAR szMsg[128] = { 0, };
						_stprintf_s( szMsg, 128, _T( "Fill a valid value for %s." ), GM_PANEL_CUSTOM_STAT_NAME[i] );
						_SetStatus( szMsg );
						return;
					}

					int const nValue = _ReadSummonCustomStat( i, 1 );
					TCHAR szClampedValue[32] = { 0, };
					_stprintf_s( szClampedValue, 32, _T( "%d" ), nValue );

					if( i == 0 )
						_sntprintf_s( szCustom, 256, _TRUNCATE, _T( "%s" ), szClampedValue );
					else
					{
						_tcsncat_s( szCustom, 256, _T( " " ), _TRUNCATE );
						_tcsncat_s( szCustom, 256, szClampedValue, _TRUNCATE );
					}
				}
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!summonmonster %lu %d custom %s %s" ), (unsigned long)dwMonsterId, nAmount, szCustom, ( m_pSummonBossCheck && m_pSummonBossCheck->IsCheck() ) ? _T( "boss" ) : _T( "normal" ) );
			}
			else
			{
				_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!summonmonster %lu %d %s %s" ), (unsigned long)dwMonsterId, nAmount, GM_PANEL_SUMMON_PRESET_ARG[(int)m_eSummonPreset], ( m_pSummonBossCheck && m_pSummonBossCheck->IsCheck() ) ? _T( "boss" ) : _T( "normal" ) );
			}

			_stprintf_s( szStatus, 160, _T( "Request sent: monster %lu x%d %s." ), (unsigned long)dwMonsterId, nAmount, GM_PANEL_SUMMON_PRESET_NAME[(int)m_eSummonPreset] );
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

	case PAGE_FULL_CLONE:
		_SendCommand( _T( "!digimon clone full" ), _T( "Request sent: full clone." ) );
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

	case PAGE_GOD_ON:
		_SendCommand( _T( "!godmode on" ), _T( "Request sent: god mode on." ) );
		break;

	case PAGE_GOD_OFF:
		_SendCommand( _T( "!godmode off" ), _T( "Request sent: god mode off." ) );
		break;

	case PAGE_KICK:
		{
			TCHAR const* szTarget = m_pTargetEdit ? m_pTargetEdit->GetTextAll() : NULL;
			if( szTarget == NULL || szTarget[0] == 0 )
			{
				_SetStatus( _T( "Type a target tamer name." ) );
				return;
			}

			_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!admin kick %s" ), szTarget );
			_SendCommand( szCommand, _T( "Request sent: kick player." ) );
		}
		break;

	case PAGE_BAN:
		{
			TCHAR const* szTarget = m_pTargetEdit ? m_pTargetEdit->GetTextAll() : NULL;
			if( szTarget == NULL || szTarget[0] == 0 )
			{
				_SetStatus( _T( "Type a target tamer name." ) );
				return;
			}

			_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!admin ban %s" ), szTarget );
			_SendCommand( szCommand, _T( "Request sent: ban player." ) );
		}
		break;

	case PAGE_UNBAN:
		{
			TCHAR const* szTarget = m_pTargetEdit ? m_pTargetEdit->GetTextAll() : NULL;
			if( szTarget == NULL || szTarget[0] == 0 )
			{
				_SetStatus( _T( "Type an account username or tamer name." ) );
				return;
			}

			_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!admin unban %s" ), szTarget );
			_SendCommand( szCommand, _T( "Request sent: unban account." ) );
		}
		break;

	case PAGE_MEMBERSHIP:
		{
			int const nValue = _ReadValueEdit( 0, 3650 );
			if( nValue <= 0 )
			{
				_SetStatus( _T( "Type a valid day amount." ) );
				return;
			}

			_sntprintf_s( szCommand, 256, _TRUNCATE, _T( "!membership add %d" ), nValue );
			_stprintf_s( szStatus, 160, _T( "Request sent: %d membership days." ), nValue );
			_SendCommand( szCommand, szStatus );
		}
		break;
	}
}

void cGMPanel::_OnItemSearchChanged( void* pkSender, void* pData )
{
	if( m_bApplyingSelection )
		return;

	if( m_ePage == PAGE_SUMMON_MONSTER )
	{
		m_dwSelectedMonsterId = 0;
		_RefreshMonsterFilter();
	}
	else
	{
		m_dwSelectedItemId = 0;
		_RefreshItemFilter();
	}
}
