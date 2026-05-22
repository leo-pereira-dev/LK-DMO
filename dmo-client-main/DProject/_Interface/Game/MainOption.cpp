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
	const CsPoint MENU_TOOLTIP_CURSOR_OFFSET( 13, 0 );
	const int MENU_TOOLTIP_SCREEN_MARGIN = 4;
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
		m_MainButtons[ i ].s_pTooltip = _T( "" );
	}

	for( int i = 0; i < OPTION_BUTTON_COUNT; ++i )
	{
		m_OptionButtons[ i ].s_pButton = NULL;
		m_OptionButtons[ i ].s_eAction = MA_NONE;
		m_OptionButtons[ i ].s_pTooltip = _T( "" );
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
		TCHAR const*	tooltip;
	};

	const sINIT_BUTTON mainButtons[] =
	{
		{ "MainMenu\\mainmenu_button_map.png", MA_MAP, _T( "Mapa" ) },
		{ "MainMenu\\mainmenu_button_digimon.png", MA_DIGIMON, _T( "Digimon" ) },
		{ "MainMenu\\mainmenu_button_inventory.png", MA_INVENTORY, _T( "Inventario" ) },
		{ "MainMenu\\mainmenu_button_quest.png", MA_QUEST, _T( "Missoes" ) },
		{ "MainMenu\\mainmenu_button_seal.png", MA_SEAL, _T( "Seal Master" ) },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC, _T( "Opcoes graficas" ) },
		{ "MainMenu\\mainmenu_button_cashwarehouse.png", MA_CASH_WAREHOUSE, _T( "Cash Warehouse" ) },
		{ "MainMenu\\mainmenu_button_tamer.png", MA_TAMER, _T( "Tamer" ) },

		{ "MainMenu\\mainmenu_button_partyfind.png", MA_PARTY, _T( "Party" ) },
		{ "MainMenu\\mainmenu_button_guild.png", MA_GUILD, _T( "Guild" ) },
		{ "MainMenu\\mainmenu_button_friend.png", MA_FRIEND, _T( "Amigos" ) },
		{ "MainMenu\\mainmenu_button_mailbox.png", MA_MAIL, _T( "Correio" ) },
		{ "MainMenu\\mainmenu_button_cashshop.png", MA_CASHSHOP, _T( "Cash Shop" ) },
		{ "MainMenu\\mainmenu_button_consignbox.png", MA_REWARD_BOX, _T( "Gift/Reward Storage" ) },
		{ "MainMenu\\mainmenu_button_membership.png", MA_MEMBERSHIP, _T( "Membership / GM Panel" ) },
		{ "MainMenu\\mainmenu_button_book.png", MA_ENCYCLOPEDIA, _T( "Enciclopedia" ) }
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
		m_MainButtons[ i ].s_pTooltip = mainButtons[ i ].tooltip;
	}
	m_nMainButtonCount = MAIN_BUTTON_COUNT;

	const sINIT_BUTTON optionButtons[] =
	{
		{ "MainMenu\\mainmenu_button_interface.png", MA_INTERFACE, _T( "Interface" ) },
		{ "MainMenu\\mainmenu_button_graphic.png", MA_GRAPHIC, _T( "Graficos" ) },
		{ "MainMenu\\mainmenu_button_sound.png", MA_SOUND, _T( "Som" ) },
		{ "MainMenu\\mainmenu_button_keyset.png", MA_SHORTCUT, _T( "Atalhos" ) },
		{ "MainMenu\\mainmenu_button_logout.png", MA_LOGOUT, _T( "Logout: voltar para selecao de servidor" ) },
		{ "MainMenu\\mainmenu_button_gameoff.png", MA_EXIT, _T( "Sair do jogo" ) }
	};

	m_nOptionButtonCount = OPTION_BUTTON_COUNT;
	for( int i = 0; i < m_nOptionButtonCount; ++i )
	{
		CsPoint pos( OPTION_BUTTON_START_X + i * OPTION_BUTTON_GAP_X, OPTION_BUTTON_START_Y );
		m_OptionButtons[ i ].s_pButton = _AddMenuButton( pos, CsPoint( 52, 52 ), OPTION_BUTTON_TOKEN, optionButtons[ i ].path, optionButtons[ i ].action );
		m_OptionButtons[ i ].s_eAction = optionButtons[ i ].action;
		m_OptionButtons[ i ].s_pTooltip = optionButtons[ i ].tooltip;
	}
}

void cMainOption::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();
}

bool cMainOption::_UpdateMenuButton( sMENU_BUTTON& buttonInfo, eMU_TYPE muReturn )
{
	if( buttonInfo.s_pButton == NULL )
		return false;

	switch( buttonInfo.s_pButton->Update_ForMouse() )
	{
	case cButton::ACTION_CLICK:
		_OnMenuAction( buttonInfo.s_eAction );
		return true;
	case cButton::ACTION_DOWN:
		return true;
	case cButton::ACTION_ON:
	case cButton::ACTION_PRESS:
		_ShowMenuTooltip( buttonInfo );
		return false;
	}

	return false;
}

void cMainOption::_ShowMenuTooltip( sMENU_BUTTON const& buttonInfo )
{
	if( buttonInfo.s_pTooltip == NULL || buttonInfo.s_pTooltip[ 0 ] == _T( '\0' ) )
		return;

	CToolTipMng* pTooltipMng = TOOLTIPMNG_STPTR;
	SAFE_POINTER_RET( pTooltipMng );

	cTooltip* pTooltip = pTooltipMng->GetTooltip();
	SAFE_POINTER_RET( pTooltip );

	CsPoint cursorPos = CURSOR_ST.GetPos();
	pTooltip->SetTooltip_Text( cursorPos, CsPoint::ZERO, buttonInfo.s_pTooltip, CFont::FS_10 );

	int const tooltipW = pTooltip->GetMaxSizeX();
	int const tooltipH = pTooltip->GetMaxSizeY();
	CsPoint targetPos( cursorPos.x + MENU_TOOLTIP_CURSOR_OFFSET.x, cursorPos.y - tooltipH - MENU_TOOLTIP_CURSOR_OFFSET.y );
	targetPos.x = CsMax( MENU_TOOLTIP_SCREEN_MARGIN, CsMin( targetPos.x, g_nScreenWidth - tooltipW - MENU_TOOLTIP_SCREEN_MARGIN ) );
	targetPos.y = CsMax( MENU_TOOLTIP_SCREEN_MARGIN, CsMin( targetPos.y, g_nScreenHeight - tooltipH - MENU_TOOLTIP_SCREEN_MARGIN ) );

	CsPoint renderPos = targetPos;
	if( renderPos.x > g_nScreenWidth / 2 )
		renderPos.x += tooltipW;
	if( renderPos.y > g_nScreenHeight / 2 )
		renderPos.y += tooltipH;

	pTooltip->SetTooltip_Text( renderPos, CsPoint::ZERO, buttonInfo.s_pTooltip, CFont::FS_10 );
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
	{
		if( _UpdateMenuButton( m_MainButtons[ i ], muReturn ) )
			return muReturn;
	}

	for( int i = 0; i < m_nOptionButtonCount; ++i )
	{
		if( _UpdateMenuButton( m_OptionButtons[ i ], muReturn ) )
			return muReturn;
	}

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
		{
			bool bSetTab = true;
			int nTabNo = 0;
			ContentsStream kTmp;
			kTmp << bSetTab << nTabNo;
			GAME_EVENT_ST.OnEvent( EVENT_CODE::OPEN_CLOSE_TAMERSTATUS, &kTmp );
		}
		Close( false );
		break;
	case MA_DIGIMON:
		GAME_EVENT_ST.OnEvent( EVENT_CODE::OPEN_CLOSE_DIGIMONSTATUS, NULL );
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
	case MA_PARTY:
		_ToggleWindow( cBaseWindow::WT_PARTY_WINDOW );
		Close( false );
		break;
	case MA_GUILD:
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_COMMUNITY ) == false )
		{
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_COMMUNITY );
			g_pGameIF->GetCommunity()->SetTab( cCommunity::TAB_GUILD );
		}
		else
		{
			bool const bWasGuildTab = g_pGameIF->GetCommunity()->GetTab() == cCommunity::TAB_GUILD;
			g_pGameIF->CloseDynamicIF( cBaseWindow::WT_COMMUNITY );
			if( bWasGuildTab == false )
			{
				g_pGameIF->GetDynamicIF( cBaseWindow::WT_COMMUNITY );
				g_pGameIF->GetCommunity()->SetTab( cCommunity::TAB_GUILD );
			}
		}
		Close( false );
		break;
	case MA_MEMBERSHIP:
		if( _IsGmPanelEnabled() )
		{
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_GM_PANEL );
		}
		else
		{
#ifdef SDM_VIP_SYSTEM_20181105
			net::game->Send_VipMemberShipData();
#endif
			GAME_EVENT_ST.OnEvent( EVENT_CODE::CASHSHOP_TOGGLE, NULL );
		}
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
		if( _IsGmPanelEnabled() )
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

void cMainOption::_ToggleWindow( eWINDOW_TYPE windowType )
{
	if( g_pGameIF->IsActiveWindow( windowType ) )
		g_pGameIF->CloseDynamicIF( windowType );
	else
		g_pGameIF->GetDynamicIF( windowType );
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

bool cMainOption::_IsGmPanelEnabled() const
{
	SAFE_POINTER_RETVAL( g_pCharMng, false );
	CTamerUser* pTamerUser = g_pCharMng->GetTamerUser();
	SAFE_POINTER_RETVAL( pTamerUser, false );
	return ( pTamerUser->GetServerOption() & CTamerUser::SO_GM_PANEL ) != 0;
}

void cMainOption::Render()
{
	RenderScript();
}

void cMainOption::ResetDevice()
{
	ResetDeviceScript();
}
