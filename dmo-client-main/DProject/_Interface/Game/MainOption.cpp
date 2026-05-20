#include "stdafx.h"
#include "MainOption.h"

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
}

cMainOption::cMainOption()
: m_pCancelButton( NULL )
, m_nMainButtonCount( 0 )
, m_nOptionButtonCount( 0 )
{
	for( int i = 0; i < MAIN_BUTTON_COUNT; ++i )
	{
		m_MainButtons[ i ].s_pButton = NULL;
		m_MainButtons[ i ].s_eAction = MA_NONE;
	}

	for( int i = 0; i < OPTION_BUTTON_COUNT; ++i )
	{
		m_OptionButtons[ i ].s_pButton = NULL;
		m_OptionButtons[ i ].s_eAction = MA_NONE;
	}
}

void cMainOption::Destroy()
{
	cBaseWindow::Delete();
}

void cMainOption::DeleteResource()
{
	DeleteScript();
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

	struct sINIT_BUTTON
	{
		char const*		path;
		eMENU_ACTION	action;
	};

	const sINIT_BUTTON mainButtons[] =
	{
		{ "MainMenu\\mainmenu_button_map.png", MA_MAP },
		{ "MainMenu\\mainmenu_button_digimon.png", MA_DIGIMON },
		{ "MainMenu\\mainmenu_button_inventory.png", MA_INVENTORY },
		{ "MainMenu\\mainmenu_button_quest.png", MA_QUEST },
		{ "MainMenu\\mainmenu_button_seal.png", MA_SEAL },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC },
		{ "MainMenu\\mainmenu_button_cashwarehouse.png", MA_CASH_WAREHOUSE },
		{ "MainMenu\\mainmenu_button_tamer.png", MA_TAMER },

		{ "MainMenu\\mainmenu_button_partyfind.png", MA_NONE },
		{ "MainMenu\\mainmenu_button_guild.png", MA_GUILD },
		{ "MainMenu\\mainmenu_button_friend.png", MA_FRIEND },
		{ "MainMenu\\mainmenu_button_mailbox.png", MA_MAIL },
		{ "MainMenu\\mainmenu_button_cashshop.png", MA_CASHSHOP },
		{ "MainMenu\\mainmenu_button_consignbox.png", MA_REWARD_BOX },
		{ "MainMenu\\mainmenu_button_membership.png", MA_GM_PANEL },
		{ "MainMenu\\mainmenu_button_book.png", MA_ENCYCLOPEDIA }
	};

	for( int i = 0; i < MAIN_BUTTON_COUNT; ++i )
	{
		const int col = i % 8;
		const int row = i / 8;
		CsPoint pos( MAIN_BUTTON_START_X + col * ( MAIN_BUTTON_SIZE.x + MAIN_BUTTON_GAP_X ),
			MAIN_BUTTON_START_Y + row * ( MAIN_BUTTON_SIZE.y + MAIN_BUTTON_GAP_Y ) );

		m_MainButtons[ i ].s_pButton = _AddMenuButton( pos, MAIN_BUTTON_SIZE, MAIN_BUTTON_TOKEN, mainButtons[ i ].path, mainButtons[ i ].action );
		m_MainButtons[ i ].s_eAction = mainButtons[ i ].action;
	}
	m_nMainButtonCount = MAIN_BUTTON_COUNT;

	const sINIT_BUTTON optionButtons[] =
	{
		{ "MainMenu\\mainmenu_button_interface.png", MA_INTERFACE },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC },
		{ "MainMenu\\mainmenu_button_sound.png", MA_SOUND },
		{ "MainMenu\\mainmenu_button_keyset.png", MA_SHORTCUT },
		{ "MainMenu\\mainmenu_button_logout.png", MA_LOGOUT },
		{ "MainMenu\\mainmenu_button_gameoff.png", MA_EXIT }
	};

	m_nOptionButtonCount = OPTION_BUTTON_COUNT;
	for( int i = 0; i < m_nOptionButtonCount; ++i )
	{
		CsPoint pos( OPTION_BUTTON_START_X + i * OPTION_BUTTON_GAP_X, OPTION_BUTTON_START_Y );
		m_OptionButtons[ i ].s_pButton = _AddMenuButton( pos, CsPoint( 52, 52 ), OPTION_BUTTON_TOKEN, optionButtons[ i ].path, optionButtons[ i ].action );
		m_OptionButtons[ i ].s_eAction = optionButtons[ i ].action;
	}
}

void cMainOption::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();
}

void cMainOption::_UpdateMenuButton( sMENU_BUTTON& buttonInfo, eMU_TYPE muReturn )
{
	if( buttonInfo.s_pButton == NULL )
		return;

	switch( buttonInfo.s_pButton->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		_OnMenuAction( buttonInfo.s_eAction );
	case cButton::ACTION_DOWN:
		return;
	}
}

cBaseWindow::eMU_TYPE cMainOption::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();

	if( muReturn == MUT_OUT_WINDOW )
	{
		m_pCancelButton->Update_ForMouse();
		for( int i = 0; i < m_nMainButtonCount; ++i )
			if( m_MainButtons[ i ].s_pButton )
				m_MainButtons[ i ].s_pButton->Update_ForMouse();
		for( int i = 0; i < m_nOptionButtonCount; ++i )
			if( m_OptionButtons[ i ].s_pButton )
				m_OptionButtons[ i ].s_pButton->Update_ForMouse();
		return muReturn;
	}

	switch( m_pCancelButton->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		Close();
	case cButton::ACTION_DOWN:
		return muReturn;
	}

	for( int i = 0; i < m_nMainButtonCount; ++i )
		_UpdateMenuButton( m_MainButtons[ i ], muReturn );

	for( int i = 0; i < m_nOptionButtonCount; ++i )
		_UpdateMenuButton( m_OptionButtons[ i ], muReturn );

	_UpdateMoveWindow_ForMouse();
	return muReturn;
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

	if( nServerRecv == false )
	{
		cPrintMsg::PrintMsg( 10019 );
		net::game->SendEncyclopediaOpen();
	}
	else
	{
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA );
	}
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
}

void cMainOption::ResetDevice()
{
	ResetDeviceScript();
}
