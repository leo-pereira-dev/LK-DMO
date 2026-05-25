#include "stdafx.h"
#include "MainOption.h"
#include "EncyclopediaContents.h"
#include "../../ContentsSystem/ContentsSystemDef.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"

namespace
{
	const CsPoint MENU_WINDOW_SIZE( 768, 300 );
	const CsPoint MAIN_BUTTON_SIZE( 62, 62 );
	const CsPoint MAIN_BUTTON_TOKEN( 0, 74 );
	const CsPoint OPTION_BUTTON_TOKEN( 0, 52 );
	const int MAIN_BUTTON_GAP_X = 18;
	const int MAIN_BUTTON_GAP_Y = 18;
	const int MAIN_BUTTON_START_X = 70;
	const int MAIN_BUTTON_START_Y = 62;
	const int OPTION_BUTTON_START_X = 200;
	const int OPTION_BUTTON_START_Y = 230;
	const int OPTION_BUTTON_GAP_X = 62;

	cSprite* _CreateMainMenuSolidSprite( NiColorA const& color )
	{
		cSprite* pSprite = NiNew cSprite;
		SAFE_POINTER_RETVAL( pSprite, NULL );
		pSprite->Init( NULL, CsPoint::ZERO, CsPoint( 1, 1 ), color, false );
		return pSprite;
	}
}

cMainOption::cMainOption()
: m_pCancelButton( NULL )
, m_nMainButtonCount( 0 )
, m_nOptionButtonCount( 0 )
, m_pTooltipBg( NULL )
, m_pTooltipTop( NULL )
, m_pTooltipBottom( NULL )
, m_pTooltipLeft( NULL )
, m_pTooltipRight( NULL )
, m_pTooltipText( NULL )
, m_bTooltipVisible( false )
, m_ptTooltipPos( CsPoint::ZERO )
, m_ptTooltipSize( CsPoint::ZERO )
{
	for( int i = 0; i < MAIN_BUTTON_COUNT; ++i )
	{
		m_MainButtons[ i ].s_pButton = NULL;
		m_MainButtons[ i ].s_eAction = MA_NONE;
		m_MainButtons[ i ].s_pszTooltip = NULL;
	}

	for( int i = 0; i < OPTION_BUTTON_COUNT; ++i )
	{
		m_OptionButtons[ i ].s_pButton = NULL;
		m_OptionButtons[ i ].s_eAction = MA_NONE;
		m_OptionButtons[ i ].s_pszTooltip = NULL;
	}
}

void cMainOption::Destroy()
{
	cBaseWindow::Delete();
}

void cMainOption::DeleteResource()
{
	DeleteScript();
	NISAFE_DELETE( m_pTooltipBg );
	NISAFE_DELETE( m_pTooltipTop );
	NISAFE_DELETE( m_pTooltipBottom );
	NISAFE_DELETE( m_pTooltipLeft );
	NISAFE_DELETE( m_pTooltipRight );
	NISAFE_DELETE( m_pTooltipText );
	m_bTooltipVisible = false;
	m_ptTooltipPos = CsPoint::ZERO;
	m_ptTooltipSize = CsPoint::ZERO;
	m_pCancelButton = NULL;
	for( int i = 0; i < MAIN_BUTTON_COUNT; ++i )
	{
		m_MainButtons[ i ].s_pButton = NULL;
		m_MainButtons[ i ].s_pszTooltip = NULL;
	}
	for( int i = 0; i < OPTION_BUTTON_COUNT; ++i )
	{
		m_OptionButtons[ i ].s_pButton = NULL;
		m_OptionButtons[ i ].s_pszTooltip = NULL;
	}
	m_nMainButtonCount = 0;
	m_nOptionButtonCount = 0;
}

cButton* cMainOption::_AddMenuButton( CsPoint pos, CsPoint size, CsPoint texToken, char const* path, eMENU_ACTION action )
{
	cButton* pButton = AddButton( pos, size, texToken, path );
	return pButton;
}

void cMainOption::Create(int nValue /* = 0  */)
{
	if( cBaseWindow::Init() == false )
		return;

	InitScript( NULL, CsPoint( g_nScreenWidth / 2 - MENU_WINDOW_SIZE.x / 2, g_nScreenHeight / 2 - MENU_WINDOW_SIZE.y / 2 ),
		MENU_WINDOW_SIZE, true, IFREGION_X::CENTER, IFREGION_Y::CENTER, false );

	AddPopup( CsPoint::ZERO, MENU_WINDOW_SIZE, CsPoint( 96, 96 ), "MainMenu\\mainmenu_background_popup\\mainmenu_background_0", "png" );
	AddSprite( CsPoint( 0, 0 ), CsPoint( MENU_WINDOW_SIZE.x, 34 ), "MainMenu\\top.png" );
	AddTitle( _T( "Menu" ), CsPoint::ZERO, CFont::FS_12 );

	m_pCancelButton = AddButton( CsPoint( MENU_WINDOW_SIZE.x - 30, 7 ), CsPoint( 16, 16 ), CsPoint( 0, 16 ), "System\\Ch_Close.tga" );

	NISAFE_DELETE( m_pTooltipBg );
	NISAFE_DELETE( m_pTooltipTop );
	NISAFE_DELETE( m_pTooltipBottom );
	NISAFE_DELETE( m_pTooltipLeft );
	NISAFE_DELETE( m_pTooltipRight );
	NISAFE_DELETE( m_pTooltipText );
	m_pTooltipBg = _CreateMainMenuSolidSprite( NiColorA( 0.0f, 0.02f, 0.05f, 0.92f ) );
	m_pTooltipTop = _CreateMainMenuSolidSprite( NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pTooltipBottom = _CreateMainMenuSolidSprite( NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pTooltipLeft = _CreateMainMenuSolidSprite( NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pTooltipRight = _CreateMainMenuSolidSprite( NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pTooltipText = NiNew cText;
	if( m_pTooltipText )
	{
		cText::sTEXTINFO tooltipTextInfo;
		tooltipTextInfo.Init( &g_pEngine->m_FontSystem );
		tooltipTextInfo.s_eFontSize = CFont::FS_8;
		tooltipTextInfo.s_eTextAlign = DT_CENTER;
		tooltipTextInfo.s_Color = NiColor( 0.96f, 0.97f, 1.0f );
		tooltipTextInfo.SetText( _T( "" ) );
		m_pTooltipText->Init( NULL, CsPoint::ZERO, &tooltipTextInfo, false );
	}
	m_bTooltipVisible = false;
	m_ptTooltipPos = CsPoint::ZERO;
	m_ptTooltipSize = CsPoint::ZERO;

	struct sINIT_BUTTON
	{
		char const*		path;
		eMENU_ACTION	action;
		TCHAR const*	tooltip;
	};

	const sINIT_BUTTON mainButtons[] =
	{
		{ "MainMenu\\mainmenu_button_map.png", MA_MAP, _T( "Map" ) },
		{ "MainMenu\\mainmenu_button_digimon.png", MA_DIGIMON, _T( "Digimon" ) },
		{ "MainMenu\\mainmenu_button_inventory.png", MA_INVENTORY, _T( "Inventory" ) },
		{ "MainMenu\\mainmenu_button_quest.png", MA_QUEST, _T( "Quest" ) },
		{ "MainMenu\\mainmenu_button_seal.png", MA_SEAL, _T( "Seal" ) },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC, _T( "Graphic" ) },
		{ "MainMenu\\mainmenu_button_cashwarehouse.png", MA_CASH_WAREHOUSE, _T( "Cash Warehouse" ) },
		{ "MainMenu\\mainmenu_button_tamer.png", MA_TAMER, _T( "Tamer" ) },

		{ "MainMenu\\mainmenu_button_partyfind.png", MA_NONE, _T( "Party Find" ) },
		{ "MainMenu\\mainmenu_button_guild.png", MA_GUILD, _T( "Guild" ) },
		{ "MainMenu\\mainmenu_button_friend.png", MA_FRIEND, _T( "Friend" ) },
		{ "MainMenu\\mainmenu_button_mailbox.png", MA_MAIL, _T( "Mailbox" ) },
		{ "MainMenu\\mainmenu_button_cashshop.png", MA_CASHSHOP, _T( "Cash Shop" ) },
		{ "MainMenu\\mainmenu_button_consignbox.png", MA_REWARD_BOX, _T( "Reward Box" ) },
		{ "MainMenu\\mainmenu_button_membership.png", MA_GM_PANEL, _T( "GM Panel" ) },
		{ "MainMenu\\mainmenu_button_book.png", MA_ENCYCLOPEDIA, _T( "Encyclopedia" ) }
	};

	for( int i = 0; i < MAIN_BUTTON_COUNT; ++i )
	{
		const int col = i % 8;
		const int row = i / 8;
		CsPoint pos( MAIN_BUTTON_START_X + col * ( MAIN_BUTTON_SIZE.x + MAIN_BUTTON_GAP_X ),
			MAIN_BUTTON_START_Y + row * ( MAIN_BUTTON_SIZE.y + MAIN_BUTTON_GAP_Y ) );

		CsPoint const texToken = mainButtons[ i ].action == MA_GRAPHIC ? OPTION_BUTTON_TOKEN : MAIN_BUTTON_TOKEN;
		m_MainButtons[ i ].s_pButton = _AddMenuButton( pos, MAIN_BUTTON_SIZE, texToken, mainButtons[ i ].path, mainButtons[ i ].action );
		m_MainButtons[ i ].s_eAction = mainButtons[ i ].action;
		m_MainButtons[ i ].s_pszTooltip = mainButtons[ i ].tooltip;
	}
	m_nMainButtonCount = MAIN_BUTTON_COUNT;

	const sINIT_BUTTON optionButtons[] =
	{
		{ "MainMenu\\mainmenu_button_interface.png", MA_INTERFACE, _T( "Interface" ) },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC, _T( "Graphic" ) },
		{ "MainMenu\\mainmenu_button_sound.png", MA_SOUND, _T( "Sound" ) },
		{ "MainMenu\\mainmenu_button_keyset.png", MA_SHORTCUT, _T( "Shortcut" ) },
		{ "MainMenu\\mainmenu_button_logout.png", MA_LOGOUT, _T( "Logout" ) },
		{ "MainMenu\\mainmenu_button_gameoff.png", MA_EXIT, _T( "Exit" ) }
	};

	m_nOptionButtonCount = OPTION_BUTTON_COUNT;
	for( int i = 0; i < m_nOptionButtonCount; ++i )
	{
		CsPoint pos( OPTION_BUTTON_START_X + i * OPTION_BUTTON_GAP_X, OPTION_BUTTON_START_Y );
		m_OptionButtons[ i ].s_pButton = _AddMenuButton( pos, CsPoint( 52, 52 ), OPTION_BUTTON_TOKEN, optionButtons[ i ].path, optionButtons[ i ].action );
		m_OptionButtons[ i ].s_eAction = optionButtons[ i ].action;
		m_OptionButtons[ i ].s_pszTooltip = optionButtons[ i ].tooltip;
	}
}

void cMainOption::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();
}

bool cMainOption::_UpdateMenuButton( sMENU_BUTTON& buttonInfo )
{
	if( buttonInfo.s_pButton == NULL )
		return false;

	switch( buttonInfo.s_pButton->Update_ForMouse() )
	{
	case cButton::ACTION_ON:
		_SetMenuTooltip( buttonInfo.s_pszTooltip );
		return false;
	case cButton::ACTION_PRESS:
		_SetMenuTooltip( buttonInfo.s_pszTooltip );
		return true;
	case cButton::ACTION_CLICK:
		_OnMenuAction( buttonInfo.s_eAction );
		return true;
	case cButton::ACTION_DOWN:
		_SetMenuTooltip( buttonInfo.s_pszTooltip );
		return true;
	}

	return false;
}

cBaseWindow::eMU_TYPE cMainOption::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	m_bTooltipVisible = false;

	if( muReturn == MUT_OUT_WINDOW )
	{
		if( m_pCancelButton )
			m_pCancelButton->Update_ForMouse();
		for( int i = 0; i < m_nMainButtonCount; ++i )
			if( m_MainButtons[ i ].s_pButton )
				m_MainButtons[ i ].s_pButton->Update_ForMouse();
		for( int i = 0; i < m_nOptionButtonCount; ++i )
			if( m_OptionButtons[ i ].s_pButton )
				m_OptionButtons[ i ].s_pButton->Update_ForMouse();
		return muReturn;
	}

	if( m_pCancelButton )
	{
		switch( m_pCancelButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			Close();
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	for( int i = 0; i < m_nMainButtonCount; ++i )
	{
		if( _UpdateMenuButton( m_MainButtons[ i ] ) )
			return muReturn;
	}

	for( int i = 0; i < m_nOptionButtonCount; ++i )
	{
		if( _UpdateMenuButton( m_OptionButtons[ i ] ) )
			return muReturn;
	}

	_UpdateMoveWindow_ForMouse();
	return muReturn;
}

void cMainOption::_SetMenuTooltip( TCHAR const* pszTooltip )
{
	if( pszTooltip == NULL || pszTooltip[ 0 ] == 0 )
		return;

	int const nTextLen = (int)_tcslen( pszTooltip );
	int const nTooltipW = min( 180, max( 64, nTextLen * 7 + 18 ) );
	int const nTooltipH = 22;
	CsPoint const ptCursor = CURSOR_ST.GetPos();
	int nX = ptCursor.x + 12;
	int nY = ptCursor.y - nTooltipH - 8;

	if( nX + nTooltipW > g_nScreenWidth )
		nX = ptCursor.x - nTooltipW - 8;
	if( nX < 0 )
		nX = 0;
	if( nY < 0 )
		nY = ptCursor.y + 18;
	if( nY + nTooltipH > g_nScreenHeight )
		nY = g_nScreenHeight - nTooltipH;
	if( nY < 0 )
		nY = 0;

	if( m_pTooltipText )
		m_pTooltipText->SetText( pszTooltip );
	m_ptTooltipPos = CsPoint( nX, nY );
	m_ptTooltipSize = CsPoint( nTooltipW, nTooltipH );
	m_bTooltipVisible = true;
}

void cMainOption::_RenderMenuTooltip()
{
	if( m_bTooltipVisible == false )
		return;
	if( m_ptTooltipSize.x <= 0 || m_ptTooltipSize.y <= 0 )
		return;

	if( m_pTooltipBg )
		m_pTooltipBg->Render( m_ptTooltipPos, m_ptTooltipSize );
	if( m_pTooltipTop )
		m_pTooltipTop->Render( m_ptTooltipPos, CsPoint( m_ptTooltipSize.x, 1 ) );
	if( m_pTooltipBottom )
		m_pTooltipBottom->Render( CsPoint( m_ptTooltipPos.x, m_ptTooltipPos.y + m_ptTooltipSize.y - 1 ), CsPoint( m_ptTooltipSize.x, 1 ) );
	if( m_pTooltipLeft )
		m_pTooltipLeft->Render( m_ptTooltipPos, CsPoint( 1, m_ptTooltipSize.y ) );
	if( m_pTooltipRight )
		m_pTooltipRight->Render( CsPoint( m_ptTooltipPos.x + m_ptTooltipSize.x - 1, m_ptTooltipPos.y ), CsPoint( 1, m_ptTooltipSize.y ) );
	if( m_pTooltipText )
		m_pTooltipText->Render( CsPoint( m_ptTooltipPos.x + ( m_ptTooltipSize.x / 2 ), m_ptTooltipPos.y + 5 ), DT_CENTER );
}

void cMainOption::_OnMenuAction( eMENU_ACTION action )
{
	switch( action )
	{
	case MA_MAP:
		_ToggleZoneMap();
		Close( false );
		break;
	case MA_SMALLMAP:
		_ToggleSmallMap();
		Close( false );
		break;
	case MA_TAMER:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_NEW_TAMERSTATUS );
		Close( false );
		break;
	case MA_DIGIMON:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_NEW_DIGIMONSTATUS );
		Close( false );
		break;
	case MA_INVENTORY:
		_ToggleInventory();
		Close( false );
		break;
	case MA_QUEST:
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_QUEST_LIST ) )
			g_pGameIF->CloseDynamicIF( cBaseWindow::WT_QUEST_LIST );
		else
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_QUEST_LIST );
		Close( false );
		break;
	case MA_SEAL:
		{
			int mode = 3;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::SEALMASTER_UI_OPEN_CLOSE, &mode );
			Close( false );
		}
		break;
	case MA_ENCYCLOPEDIA:
		_OpenEncyclopedia();
		Close( false );
		break;
	case MA_EVOLUTION_HELP:
		Close( false );
		break;
	case MA_CASH_WAREHOUSE:
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CASH_TOGGLE, NULL );
		Close( false );
		break;
	case MA_CASHSHOP:
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CASHSHOP_TOGGLE, NULL );
		Close( false );
		break;
	case MA_GUILD:
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_COMMUNITY ) )
			g_pGameIF->CloseDynamicIF( cBaseWindow::WT_COMMUNITY );
		else
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_COMMUNITY );
		Close( false );
		break;
	case MA_FRIEND:
		{
			int opentype = 1;
			GAME_EVENT_STPTR->OnEvent( EVENT_CODE::TOGGLE_FRIEND_SHUTOUT_UI, &opentype );
			Close( false );
		}
		break;
	case MA_MAIL:
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_MAIL ) )
			g_pGameIF->CloseDynamicIF( cBaseWindow::WT_MAIL );
		else
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_MAIL );
		Close( false );
		break;
	case MA_REWARD_BOX:
		GAME_EVENT_ST.OnEvent( EVENT_CODE::REWARD_ITEM_TOGGLE, NULL );
		Close( false );
		break;
	case MA_INTERFACE:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_OPTION_INTERFACE );
		Close( false );
		break;
	case MA_GRAPHIC:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_OPTION_GRAPHIC );
		Close( false );
		break;
	case MA_SOUND:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_OPTION_SOUND );
		Close( false );
		break;
	case MA_SHORTCUT:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_OPTION_MACRO );
		Close( false );
		break;
	case MA_GM_PANEL:
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_GM_PANEL );
		Close( false );
		break;
	case MA_LOGOUT:
		_RequestExit( cPrintMsg::LOG_OUT );
		break;
	case MA_EXIT:
		_RequestExit( cPrintMsg::GAME_EXIT );
		break;
	default:
		break;
	}
}

void cMainOption::_ToggleInventory()
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_INVENTORY, 0 ) )
	{
		GAME_EVENT_ST.OnEvent( EVENT_CODE::CLOSE_INVENTORY, NULL );
		if( g_pDataMng->GetPCbang()->GetPcBang() )
			g_pGameIF->CloseDynamicIF( cBaseWindow::WT_PCBANGINVEN );
	}
	else
	{
		bool bSound = true;
		GAME_EVENT_ST.OnEvent( EVENT_CODE::OPEN_INVENTORY, &bSound );
	}
}

void cMainOption::_ToggleZoneMap()
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_SMALLMAP ) )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_SMALLMAP );

	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ZONEMAP ) )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_ZONEMAP );
	else
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_ZONEMAP );
}

void cMainOption::_ToggleSmallMap()
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ZONEMAP ) )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_ZONEMAP );

	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_SMALLMAP ) )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_SMALLMAP );
	else
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_SMALLMAP );
}

void cMainOption::_OpenEncyclopedia()
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ENCYCLOPEDIA ) )
	{
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA );
		return;
	}

	bool nServerRecv = false;
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::GET_ENCYCLOPEDIA_BISRECV, &nServerRecv );
	EncyclopediaContents* pEncyclopediaContents = (EncyclopediaContents*)CONTENTSSYSTEM_PTR->GetContents( E_CT_ENCYCLOPEDIA_CONTENTS );

	g_pGameIF->GetDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA );
	pEncyclopediaContents = (EncyclopediaContents*)CONTENTSSYSTEM_PTR->GetContents( E_CT_ENCYCLOPEDIA_CONTENTS );
	if( pEncyclopediaContents )
	{
		pEncyclopediaContents->EnsureStaticDataLoaded();
		if( pEncyclopediaContents->ShouldRequestServerData( 1000 ) )
		{
			if( net::game )
			{
				pEncyclopediaContents->MarkServerDataRequesting();
				nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] main-option send DigimonBookInfo eventRecv=%d",
					nServerRecv ? 1 : 0 );
				net::game->SendEncyclopediaOpen();
			}
			else
				nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] main-option send failed: net::game is null" );
		}
		else
		{
			nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] main-option skip send eventRecv=%d recv=%d requesting=%d",
				nServerRecv ? 1 : 0,
				pEncyclopediaContents->IsServerDataReceived() ? 1 : 0,
				pEncyclopediaContents->IsServerDataRequesting() ? 1 : 0 );
		}
	}
	else
		nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] main-option skip send: EncyclopediaContents is null eventRecv=%d",
			nServerRecv ? 1 : 0 );
}

void cMainOption::_RequestExit( int msgType )
{
	if( g_pCharMng->GetTamerUser()->IsBattle() == true )
	{
		cPrintMsg::PrintMsg( 16002 );
		return;
	}

	g_pCharMng->GetTamerUser()->DeletePath();
	for( int i = 0; i < MAX_DIGIMONUSER_COUNT; ++i )
	{
		if( g_pCharMng->GetDigimonUser( i ) )
			g_pCharMng->GetDigimonUser( i )->DeletePath();
	}

	cMessageBox::InsertList( UISTRING_TEXT( "MAINOPTION_MSGBOX_ALERT" ).c_str(), _T( "" ), msgType, CsTalk_Message::MT_CANCEL );

	unsigned int nNextExpRate = 0;
	GAME_EVENT_ST.OnEvent( EVENT_CODE::GET_EXP_EVENT_NEXT_EXPRATE, &nNextExpRate );
	if( nNextExpRate > 0 )
	{
		cMessageBox* pExitWindow = cMessageBox::GetMessageBox( msgType );
		if( pExitWindow )
			pExitWindow->SetValue2( nNextExpRate );
	}

	Close( false );
}

void cMainOption::Render()
{
	RenderScript();
	_RenderMenuTooltip();
}

void cMainOption::ResetDevice()
{
	ResetDeviceScript();
}
