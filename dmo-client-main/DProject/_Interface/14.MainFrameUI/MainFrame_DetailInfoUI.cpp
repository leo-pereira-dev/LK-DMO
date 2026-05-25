#include "stdafx.h"
#include "MainFrame_DetailInfoUI.h"
#include "../Game/AchieveContents.h"

namespace
{
	const int DETAILINFO_WIDTH = 628;
	const int DETAILINFO_HEIGHT = 694;
	const int DETAILINFO_SKILL_COUNT = 6;
	const int DETAILINFO_TAMER_PANEL_WIDTH = 406;
	const int DETAILINFO_TAMER_CENTER_X = 211;
	const int DETAILINFO_TAMER_RENDER_X = 18;
	const int DETAILINFO_TAMER_RENDER_Y = 222;
	const int DETAILINFO_TAMER_STAT_X = 422;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_X = 176;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_Y = 126;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_SIZE = 70;

	const TCHAR* const TAMER_STAT_LABELS[13] =
	{
		_T("HP"), _T("DS"), _T("AT"), _T("AS"), _T("CT"), _T("HT"),
		_T("Aumento SCD"), _T("Dano CT"), _T("Dano SD"), _T("Dano Base"),
		_T("Defesa"), _T("BL"), _T("EV")
	};

	enum eTamerDetailInfoStat
	{
		eTamerDetailHP,
		eTamerDetailDS,
		eTamerDetailAT,
		eTamerDetailAS,
		eTamerDetailCT,
		eTamerDetailHT,
		eTamerDetailSCD,
		eTamerDetailCD,
		eTamerDetailSD,
		eTamerDetailBaseDamage,
		eTamerDetailDE,
		eTamerDetailBL,
		eTamerDetailEV,
		eTamerDetailMax,
	};

	const int DETAILINFO_DIGIMON_STAT_COUNT = 9;
	const TCHAR* const DIGIMON_STAT_LABELS[DETAILINFO_DIGIMON_STAT_COUNT] =
	{
		_T("HP"), _T("DS"), _T("AT"), _T("AS"), _T("CT"),
		_T("HT"), _T("DE"), _T("BL"), _T("EV")
	};

	std::wstring FormatIntValue(int nValue)
	{
		std::wstring wsText;
		DmCS::StringFn::From( wsText, nValue );
		return wsText;
	}

	std::wstring FormatPercentValue(int nValue)
	{
		std::wstring wsText;
		DmCS::StringFn::Format( wsText, L"%d%%", nValue );
		return wsText;
	}

	std::wstring FormatBasisPointPercentValue(int nValue)
	{
		std::wstring wsText;
		DmCS::StringFn::Format( wsText, L"%.2f%%", nValue / 100.0f );
		return wsText;
	}

	std::wstring FormatTamerDetailInfoValue(int nIndex, int nValue)
	{
		switch( nIndex )
		{
		case eTamerDetailAS:
		case eTamerDetailCT:
		case eTamerDetailBL:
		case eTamerDetailEV:
			return FormatBasisPointPercentValue( nValue );

		case eTamerDetailSCD:
		case eTamerDetailCD:
		case eTamerDetailBaseDamage:
			return FormatPercentValue( nValue );

		default:
			return FormatIntValue( nValue );
		}
	}
}

cDetailInfoUI::cDetailInfoUI()
: m_pTabButton(NULL),
m_pCloseButton(NULL),
m_pVisibilityButton(NULL),
m_pEquipmentButton(NULL),
m_pSealMasterButton(NULL),
m_pTamerLeftEquipList(NULL),
m_pTamerRightEquipList(NULL),
m_pDigimonSkillGrid(NULL),
m_pTamerRenderTex(NULL),
m_pDigimonRenderTex(NULL),
m_eCurrentTab(eTabTamer),
m_bPendingRefresh(false),
m_pTamerNameText(NULL),
m_pTamerLevelText(NULL),
m_pTamerAchievementTitleText(NULL),
m_pDigimonNameText(NULL),
m_pDigimonLevelText(NULL),
m_pDigimonInfoText(NULL)
{
	for( int i = 0; i < 13; ++i )
		m_pTamerStatValue[i] = NULL;
	for( int i = 0; i < eDigiMax; ++i )
		m_pDigimonStatValue[i] = NULL;
	for( int i = 0; i < 5; ++i )
		m_pEnchantValue[i] = NULL;
}

cDetailInfoUI::~cDetailInfoUI()
{
	if( m_pTamerRenderTex )
		SAFE_NIDELETE( m_pTamerRenderTex );
	if( m_pDigimonRenderTex )
		SAFE_NIDELETE( m_pDigimonRenderTex );

	Destroy();
}

bool cDetailInfoUI::Construct(void)
{
	if( !CONTENTSSYSTEM_PTR )
		return false;

	SetSystem( CONTENTSSYSTEM_PTR->GetContents<SystemType>( SystemType::IsContentsIdentity() ) );
	if( !GetSystem() )
		return false;

	GetSystem()->Register( SystemType::eUpdateStatus, this );
	GetSystem()->Register( SystemType::eUpdateStat, this );
	GetSystem()->Register( SystemType::eUpdateSkill, this );
	GetSystem()->Register( SystemType::eChangeDigimon, this );

	return true;
}

void cDetailInfoUI::Notify(int const& iNotifiedEvt, ContentsStream const& kStream)
{
	switch( iNotifiedEvt )
	{
	case SystemType::eUpdateStatus:
	case SystemType::eUpdateStat:
	case SystemType::eUpdateSkill:
	case SystemType::eChangeDigimon:
		m_bPendingRefresh = true;
		break;
	}
}

cBaseWindow::eWINDOW_TYPE cDetailInfoUI::GetWindowType()
{
	return WT_DETAIL_INFO;
}

void cDetailInfoUI::Destroy()
{
	if( GetSystem() )
		GetSystem()->UnRegisterAll( this );

	cBaseWindow::Delete();
}

void cDetailInfoUI::DeleteResource()
{
	DeleteScript();
	m_mapEquipIcon.clear();
	m_mapSkillIcon.clear();
	m_vTamerControls.clear();
	m_vTamerTexts.clear();
	m_vDigimonControls.clear();
	m_vDigimonTexts.clear();
}

void cDetailInfoUI::Create(int nValue)
{
	if( !cBaseWindow::Init() )
		return;

	int const nPosX = ( g_nScreenWidth / 2 ) - ( DETAILINFO_WIDTH / 2 );
	int const nPosY = ( g_nScreenHeight / 2 ) - ( DETAILINFO_HEIGHT / 2 );
	InitScript( NULL, CsPoint( nPosX, nPosY ), CsPoint( DETAILINFO_WIDTH, DETAILINFO_HEIGHT ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	_AddSolidSprite( CsPoint::ZERO, CsPoint( DETAILINFO_WIDTH, DETAILINFO_HEIGHT ), NiColorA( 0.0f, 0.025f, 0.055f, 0.98f ) );
	AddSprite( CsPoint::ZERO, CsPoint( DETAILINFO_WIDTH, DETAILINFO_HEIGHT ), "DetailInfo\\TamerStatus_Win_03_bg.tga" );
	AddTitle( _T("Detalhes"), CsPoint( 0, 10 ) );

	m_pVisibilityButton = AddButton( CsPoint( 6, 11 ), CsPoint( 52, 22 ), CsPoint( 0, 22 ), "DetailInfo\\Autocrop_ON_Btn.bmp" );

	m_pCloseButton = AddButton( CsPoint( DETAILINFO_WIDTH - 30, 10 ), CsPoint( 30, 28 ), CsPoint( 0, 28 ), "Event\\event_close.tga" );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickCloseButton );

	_MakeTabButtons();
	_MakeTamerPage();
	_MakeDigimonPage();
	_SetTab( eTabTamer );
	_UpdateAll();
}

void cDetailInfoUI::Update(float const& fDeltaTime)
{
	UpdateScript( fDeltaTime );

	if( m_bPendingRefresh )
	{
		m_bPendingRefresh = false;
		_UpdateAll();
	}

	if( eTabTamer == m_eCurrentTab )
	{
		SAFE_POINTER_RET( g_pCharMng );
		CTamerUser* pUser = g_pCharMng->GetTamerUser();
		if( pUser && !pUser->GetViewSight() && pUser->IsLoad() && m_pTamerRenderTex )
		{
			CsNodeObj* pCsNode = pUser->GetCsNode();
			if( pCsNode )
			{
				CsC_AniProp* pAniProp = pUser->GetProp_Animation();
				if( pAniProp && pAniProp->GetActorMng() )
				{
					float fAniTime = pUser->GetAniTime();
					pAniProp->GetActorMng()->Update( fAniTime );
					pCsNode->m_pNiNode->Update( fAniTime );
				}
			}
		}
	}
	else
	{
		SAFE_POINTER_RET( g_pCharMng );
		CDigimonUser* pUser = g_pCharMng->GetDigimonUser( 0 );
		if( pUser && !pUser->GetViewSight() && pUser->IsLoad() && m_pDigimonRenderTex )
		{
			CsNodeObj* pCsNode = pUser->GetCsNode();
			if( pCsNode )
			{
				CsC_AniProp* pAniProp = pUser->GetProp_Animation();
				if( pAniProp && pAniProp->GetActorMng() )
				{
					float fAniTime = pUser->GetAniTime();
					pAniProp->GetActorMng()->Update( fAniTime );
					pCsNode->m_pNiNode->Update( fAniTime );
				}
			}
		}
	}

	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cDetailInfoUI::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( MUT_OUT_WINDOW == muReturn )
		return muReturn;

	if( m_pTabButton && m_pTabButton->Update_ForMouse() != cRadioButton::INVALIDE_RADIO_CLICK )
		return muReturn;

	if( m_pVisibilityButton && m_pVisibilityButton->Update_ForMouse() )
		return muReturn;

	if( m_pCloseButton && m_pCloseButton->Update_ForMouse() )
		return muReturn;

	if( eTabTamer == m_eCurrentTab )
	{
		if( m_pTamerLeftEquipList && m_pTamerLeftEquipList->Update_ForMouse( CURSOR_ST.GetPos() ) )
		{
			if( _SetEquipTooltip( m_pTamerLeftEquipList->GetMouseOverItem() ) )
				return muReturn;
		}

		if( m_pTamerRightEquipList && m_pTamerRightEquipList->Update_ForMouse( CURSOR_ST.GetPos() ) )
		{
			if( _SetEquipTooltip( m_pTamerRightEquipList->GetMouseOverItem() ) )
				return muReturn;
		}

		if( m_pEquipmentButton && m_pEquipmentButton->Update_ForMouse() )
			return muReturn;

		if( m_pSealMasterButton && m_pSealMasterButton->Update_ForMouse() )
			return muReturn;
	}
	else
	{
		if( m_pDigimonSkillGrid && m_pDigimonSkillGrid->Update_ForMouse( CURSOR_ST.GetPos() ) )
		{
			if( _SetSkillTooltip( m_pDigimonSkillGrid->GetMouseOverItem() ) )
				return muReturn;
		}
	}

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( DETAILINFO_WIDTH, 40 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
		_UpdateMoveWindow_ForMouse();

	return muReturn;
}

void cDetailInfoUI::Render()
{
	RenderScript();

	if( eTabTamer == m_eCurrentTab )
	{
		if( m_pTamerRenderTex && g_pCharMng )
		{
			CTamerUser* pTamer = g_pCharMng->GetTamerUser();
			if( pTamer )
			{
				m_pTamerRenderTex->BeginRender();
				m_pTamerRenderTex->RenderObject( GET_SUBCAMERA( CAMERA_02 ), pTamer, true );
				m_pTamerRenderTex->EndRender( GET_SUBCAMERA( CAMERA_02 ), GetTotalPosition() + CsPoint( DETAILINFO_TAMER_RENDER_X, DETAILINFO_TAMER_RENDER_Y ) );

				if( m_pTamerLeftEquipList )
					m_pTamerLeftEquipList->Render();
				if( m_pTamerRightEquipList )
					m_pTamerRightEquipList->Render();
			}
		}

		_RenderTamerPortrait();
	}
	else
	{
		if( m_pDigimonRenderTex && g_pCharMng )
		{
			CDigimonUser* pDigimon = g_pCharMng->GetDigimonUser( 0 );
			if( pDigimon )
			{
				m_pDigimonRenderTex->BeginRender();
				m_pDigimonRenderTex->RenderObject( GET_SUBCAMERA( CAMERA_03 ), pDigimon, true );
				m_pDigimonRenderTex->EndRender( GET_SUBCAMERA( CAMERA_03 ), GetTotalPosition() + CsPoint( 60, 142 ) );
			}
		}
	}
}

void cDetailInfoUI::ResetDevice()
{
	ResetDeviceScript();
	if( m_pTamerRenderTex )
		m_pTamerRenderTex->ResetDevice();
	if( m_pDigimonRenderTex )
		m_pDigimonRenderTex->ResetDevice();
}

bool cDetailInfoUI::OnEscapeKey()
{
	if( g_pGameIF )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_DETAIL_INFO );
	return true;
}

void cDetailInfoUI::_MakeTabButtons()
{
	CsPoint ptPos( ( DETAILINFO_WIDTH / 2 ) - 104, 50 );
	CsPoint ptSize( 103, 24 );
	CsPoint ptTex( 0, 24 );

	m_pTabButton = AddRadioButton( ptPos, ptSize, ptTex, "System\\Btn_Radio1.tga" );
	if( !m_pTabButton )
		return;

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_10, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Tamer") );
	m_pTabButton->SetCheckBoxText( &ti );
	m_pTabButton->SetUserData( new sTabInfo( eTabTamer ) );
	m_pTabButton->AddEvent( cRadioButton::RadioButton_LBUP_EVENT, this, &cDetailInfoUI::_OnClickTabButton );

	cRadioButton* pDigimonTab = m_pTabButton->AddNode( CsPoint( ( DETAILINFO_WIDTH / 2 ) + 2, 50 ), ptSize, ptTex, "System\\Btn_Radio1.tga" );
	if( pDigimonTab )
	{
		ti.SetText( _T("Digimon") );
		pDigimonTab->SetCheckBoxText( &ti );
		pDigimonTab->SetEnable( true );
		pDigimonTab->SetUserData( new sTabInfo( eTabDigimon ) );
	}

	m_pTabButton->SetCheckIndex( 0 );
}

void cDetailInfoUI::_MakeTamerPage()
{
	_AddPageSolidSprite( true, CsPoint( 8, 86 ), CsPoint( DETAILINFO_TAMER_PANEL_WIDTH, 575 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.94f ) );
	_AddPageSprite( true, CsPoint( 8, 86 ), CsPoint( DETAILINFO_TAMER_PANEL_WIDTH, 575 ), "DetailInfo\\TamerStatus_Win_04_bg.tga" );
	_AddPageSprite( true, CsPoint( 18, 96 ), CsPoint( 386, 47 ), "DetailInfo\\Storage_M_field_a.tga" );
	_AddPageSprite( true, CsPoint( DETAILINFO_TAMER_STAT_X + 4, 88 ), CsPoint( 188, 40 ), "TacticsHouse\\Storage_M_field.tga" );
	_AddPageSolidSprite( true, CsPoint( DETAILINFO_TAMER_STAT_X - 1, 201 ), CsPoint( 200, 365 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.96f ) );
	_AddPageSprite( true, CsPoint( DETAILINFO_TAMER_STAT_X - 1, 201 ), CsPoint( 200, 365 ), "DetailInfo\\TamerStatus_win_02_bg.tga" );
	_AddStatTable( true, CsPoint( DETAILINFO_TAMER_STAT_X, 201 ), 13, 118, 79, 28 );

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_12, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("") );
	m_pTamerNameText = _AddPageText( true, &ti, CsPoint( DETAILINFO_TAMER_CENTER_X, 100 ) );

	ti.SetText( _T("") );
	m_pTamerLevelText = _AddPageText( true, &ti, CsPoint( DETAILINFO_TAMER_CENTER_X, 199 ) );

	ti.Init( CFont::FS_11, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("") );
	m_pTamerAchievementTitleText = _AddPageText( true, &ti, CsPoint( DETAILINFO_TAMER_STAT_X + 99, 101 ) );

	ti.SetText( _T("Conquista Equipada") );
	_AddPageText( true, &ti, CsPoint( DETAILINFO_TAMER_STAT_X + 99, 139 ) );

	ti.SetText( _T("Atributos Obtidos") );
	_AddPageText( true, &ti, CsPoint( DETAILINFO_TAMER_STAT_X + 99, 179 ) );

	for( int i = 0; i < 13; ++i )
		_AddTamerStatRow( i, TAMER_STAT_LABELS[i], CsPoint( DETAILINFO_TAMER_STAT_X + 5, 207 + ( i * 28 ) ) );

	m_pEquipmentButton = _AddPageButton( true, CsPoint( 98, 622 ), CsPoint( 106, 30 ), cButton::IMAGE_NOR_9, _T("Equipamento") );
	if( m_pEquipmentButton )
		m_pEquipmentButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickEquipmentButton );

	m_pSealMasterButton = _AddPageButton( true, CsPoint( 212, 622 ), CsPoint( 106, 30 ), cButton::IMAGE_NOR_9, _T("Selo Mestre") );
	if( m_pSealMasterButton )
		m_pSealMasterButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickSealMasterButton );

	_MakeTamerEquipGrid();

	m_pTamerRenderTex = NiNew cRenderTex;
	if( m_pTamerRenderTex && !m_pTamerRenderTex->Init( CsPoint( 386, 363 ), "TamerStatus_New\\TamerStatus_Win_01_bg.tga" ) )
		SAFE_NIDELETE( m_pTamerRenderTex );
}

void cDetailInfoUI::_MakeDigimonPage()
{
	_AddPageSolidSprite( false, CsPoint( 8, 86 ), CsPoint( 250, 575 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.94f ) );
	_AddPageSolidSprite( false, CsPoint( 26, 134 ), CsPoint( 214, 124 ), NiColorA( 0.0f, 0.015f, 0.025f, 0.92f ) );
	_AddPageSprite( false, CsPoint( 26, 134 ), CsPoint( 214, 124 ), "DetailInfo\\Storage_M_field_a.tga" );
	_AddPageSprite( false, CsPoint( 86, 136 ), CsPoint( 92, 92 ), "DetailInfo\\TamerStatus_Digimon_Tamer.tga" );
	_AddPageSolidSprite( false, CsPoint( 258, 160 ), CsPoint( 198, 312 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.96f ) );
	_AddStatTable( false, CsPoint( 280, 162 ), eDigiMax, 62, 114, 28 );
	_AddPageSprite( false, CsPoint( 424, 134 ), CsPoint( 22, 22 ), "DetailInfo\\zoom_in_aa.tga" );
	_AddPageSprite( false, CsPoint( 24, 268 ), CsPoint( 216, 84 ), "DetailInfo\\RemainTimerBG.tga" );
	_AddPageSolidSprite( false, CsPoint( 26, 372 ), CsPoint( 214, 96 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.90f ) );
	_AddPageSolidSprite( false, CsPoint( 24, 520 ), CsPoint( 224, 116 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.90f ) );
	_AddPageSolidSprite( false, CsPoint( 280, 520 ), CsPoint( 160, 116 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.90f ) );

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_12, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("") );
	m_pDigimonNameText = _AddPageText( false, &ti, CsPoint( 134, 108 ) );

	ti.SetText( _T("") );
	m_pDigimonLevelText = _AddPageText( false, &ti, CsPoint( 134, 228 ) );

	ti.Init( CFont::FS_10, NiColor::WHITE );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( _T("") );
	m_pDigimonInfoText = _AddPageText( false, &ti, CsPoint( 38, 296 ) );

	ti.Init( CFont::FS_11, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Informacao Digimon") );
	_AddPageText( false, &ti, CsPoint( 132, 270 ) );

	ti.SetText( _T("Habilidade") );
	_AddPageText( false, &ti, CsPoint( 132, 360 ) );

	ti.SetText( _T("Atributos") );
	_AddPageText( false, &ti, CsPoint( 356, 132 ) );

	ti.SetText( _T("Encanto Digimon") );
	_AddPageText( false, &ti, CsPoint( 126, 502 ) );

	ti.SetText( _T("Avanco de Digimon") );
	_AddPageText( false, &ti, CsPoint( 360, 502 ) );

	for( int i = 0; i < eDigiMax; ++i )
		_AddDigimonStatRow( static_cast<eDigimonAbilityType>( i ), DIGIMON_STAT_LABELS[i], CsPoint( 285, 168 + ( i * 28 ) ) );

	_AddEnchantRow( 0, _T("AT"), CsPoint( 48, 534 ) );
	_AddEnchantRow( 1, _T("BL"), CsPoint( 160, 534 ) );
	_AddEnchantRow( 2, _T("CT"), CsPoint( 48, 578 ) );
	_AddEnchantRow( 3, _T("EV"), CsPoint( 160, 578 ) );
	_AddEnchantRow( 4, _T("HP"), CsPoint( 48, 622 ) );

	m_pDigimonSkillGrid = NiNew cGridListBox;
	if( m_pDigimonSkillGrid )
	{
		m_pDigimonSkillGrid->Init( m_pRoot, CsPoint( 44, 390 ), CsPoint( 170, 80 ), CsPoint( 28, 14 ), CsPoint( 32, 32 ), cGridListBox::LowRightDown, cGridListBox::LeftTop, NULL, false, 3 );
		m_pDigimonSkillGrid->SetAutoSelection( false );
		m_pDigimonSkillGrid->SetBackOverAndSelectedImgRender( false );
		AddChildControl( m_pDigimonSkillGrid );
		m_vDigimonControls.push_back( m_pDigimonSkillGrid );

		for( int i = 0; i < DETAILINFO_SKILL_COUNT; ++i )
		{
			cString* pControl = NiNew cString;
			SAFE_POINTER_CON( pControl );

			sSkillIcon skill;
			skill.pSkillIcon = pControl->AddSkillIcon( CsPoint( 32, 32 ), 0, 0 );
			if( skill.pSkillIcon )
			{
				skill.pSkillIcon->SetAutoPointerDelete( true );
				skill.pSkillIcon->SetEnable( false );
				skill.pSkillIcon->SetVisible( false );
			}

			cText::sTEXTINFO skillTi;
			skillTi.Init( CFont::FS_10, NiColor( 76.f/255.f, 242.f/255.f, 1 ) );
			skillTi.s_eTextAlign = DT_CENTER;
			skillTi.SetText( _T("") );
			skill.pSkillLevelText = pControl->AddText( &skillTi, CsPoint( 16, 35 ) );
			if( skill.pSkillLevelText )
			{
				skill.pSkillLevelText->SetAutoPointerDelete( true );
				skill.pSkillLevelText->SetVisible( false );
			}

			skill.pGridItem = NiNew cGridListBoxItem( i, CsPoint( 32, 32 ) );
			if( skill.pGridItem )
			{
				skill.pGridItem->SetItem( pControl );
				skill.pGridItem->SetUserData( new sSkillInfo( i ) );
				m_pDigimonSkillGrid->AddItem( skill.pGridItem );
			}

			m_mapSkillIcon.insert( std::make_pair( i, skill ) );
		}
	}

	m_pDigimonRenderTex = NiNew cRenderTex;
	if( m_pDigimonRenderTex && !m_pDigimonRenderTex->Init( CsPoint( 150, 120 ), "DetailInfo\\Storage_M_field_a.tga" ) )
		SAFE_NIDELETE( m_pDigimonRenderTex );
}

void cDetailInfoUI::_SetTab(eTabType eTab)
{
	m_eCurrentTab = eTab;
	if( m_pTabButton )
		m_pTabButton->SetCheckIndex( eTab == eTabTamer ? 0 : 1 );

	_SetPageVisible( true, eTab == eTabTamer );
	_SetPageVisible( false, eTab == eTabDigimon );
	_UpdateAll();
}

void cDetailInfoUI::_SetPageVisible(bool bTamerPage, bool bVisible)
{
	std::vector<cWindow*>& vControls = bTamerPage ? m_vTamerControls : m_vDigimonControls;
	std::vector<cText*>& vTexts = bTamerPage ? m_vTamerTexts : m_vDigimonTexts;

	for( size_t i = 0; i < vControls.size(); ++i )
	{
		if( vControls[i] )
			vControls[i]->SetVisible( bVisible );
	}

	for( size_t i = 0; i < vTexts.size(); ++i )
	{
		if( vTexts[i] )
			vTexts[i]->SetVisible( bVisible );
	}
}

void cDetailInfoUI::_UpdateAll()
{
	_UpdateTamerPage();
	_UpdateDigimonPage();
}

void cDetailInfoUI::_MakeTamerEquipGrid()
{
	CsPoint ptSize( 32, 304 );
	CsPoint ptGap( 0, 14 );
	CsPoint ptItemSize( 32, 32 );

	m_pTamerLeftEquipList = NiNew cGridListBox;
	if( m_pTamerLeftEquipList )
	{
		m_pTamerLeftEquipList->Init( m_pRoot, CsPoint( DETAILINFO_TAMER_RENDER_X + 24, DETAILINFO_TAMER_RENDER_Y + 28 ), ptSize, ptGap, ptItemSize, cGridListBox::LowLeftDown, cGridListBox::LeftTop, NULL, false, 1 );
		m_pTamerLeftEquipList->SetMouseOverImg( "Icon\\Mask_Over.tga" );
		m_pTamerLeftEquipList->SetBackOverAndSelectedImgRender( false );
		m_pTamerLeftEquipList->SetAutoSelection( false );
		AddChildControl( m_pTamerLeftEquipList );
		m_vTamerControls.push_back( m_pTamerLeftEquipList );
	}

	m_pTamerRightEquipList = NiNew cGridListBox;
	if( m_pTamerRightEquipList )
	{
		m_pTamerRightEquipList->Init( m_pRoot, CsPoint( DETAILINFO_TAMER_RENDER_X + 330, DETAILINFO_TAMER_RENDER_Y + 28 ), ptSize, ptGap, ptItemSize, cGridListBox::LowLeftDown, cGridListBox::LeftTop, NULL, false, 1 );
		m_pTamerRightEquipList->SetMouseOverImg( "Icon\\Mask_Over.tga" );
		m_pTamerRightEquipList->SetBackOverAndSelectedImgRender( false );
		m_pTamerRightEquipList->SetAutoSelection( false );
		AddChildControl( m_pTamerRightEquipList );
		m_vTamerControls.push_back( m_pTamerRightEquipList );
	}

	if( m_pTamerLeftEquipList )
	{
		int nIndex = 0;
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Head );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Glass );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Coat );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Pants );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Glove );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Shoes );
		_AddTamerEquipGridItem( m_pTamerLeftEquipList, nIndex++, nTamer::Costume );
	}

	if( m_pTamerRightEquipList )
	{
		int nIndex = 0;
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::Earring );
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::Necklace );
#ifdef SDM_TAMER_EQUIP_ADD_BRACELET_20181031
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::Bracelet );
#endif
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::Ring );
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::EquipAura );
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::MaxParts );
#ifdef SDM_TAMER_XGUAGE_20180628
		_AddTamerEquipGridItem( m_pTamerRightEquipList, nIndex++, nTamer::XAI );
#endif
	}
}

void cDetailInfoUI::_AddTamerEquipGridItem(cGridListBox* pGrid, int nGridIndex, int nEquipType)
{
	cString* pControl = NiNew cString;
	SAFE_POINTER_RET( pControl );

	uint nItemType = GetSystem()->GetEquipItemType( nEquipType );
	sEquipIcon equipIcon;
	equipIcon.pItemIcon = pControl->AddIcon( CsPoint( 32, 32 ), ICONITEM::ITEM_ICON, nItemType, 1 );
	if( equipIcon.pItemIcon )
	{
		equipIcon.pItemIcon->SetAutoPointerDelete( true );
		equipIcon.pItemIcon->SetCoolTimeMode( true );
		if( 0 == nItemType )
			equipIcon.pItemIcon->SetVisible( false );
		else
			equipIcon.pItemIcon->SetCoolTimeSeq( GetSystem()->GetFTItemCoolTime( nItemType ) );
	}

	equipIcon.pGridItem = NiNew cGridListBoxItem( nGridIndex, CsPoint( 32, 32 ) );
	if( equipIcon.pGridItem )
	{
		equipIcon.pGridItem->SetItem( pControl );
		equipIcon.pGridItem->SetUserData( new sEquipInfo( nEquipType, nItemType ) );
		equipIcon.pGridItem->SetEnable( nItemType != 0 );
		if( pGrid )
			pGrid->AddItem( equipIcon.pGridItem );
	}

	m_mapEquipIcon.insert( std::make_pair( nEquipType, equipIcon ) );
}

void cDetailInfoUI::_UpdateTamerPage()
{
	_SetText( m_pTamerNameText, GetSystem()->GetTamerName() );

	std::wstring wsLevel;
	DmCS::StringFn::Format( wsLevel, L"Nv%d", GetSystem()->GetTamerStat( SystemType::eLV ) );
	_SetText( m_pTamerLevelText, wsLevel );

	std::wstring wsAchievementTitle;
	if( CONTENTSSYSTEM_PTR && nsCsFileTable::g_pAchieveMng )
	{
		AchieveContents* pAchieveContents = CONTENTSSYSTEM_PTR->GetContents< AchieveContents >( AchieveContents::IsContentsIdentity() );
		if( pAchieveContents )
		{
			CsAchieve* pAchieve = nsCsFileTable::g_pAchieveMng->GetAchieve( pAchieveContents->GetDispTitle() );
			if( pAchieve )
				wsAchievementTitle = pAchieve->GetInfo()->s_szTitle;
		}
	}
	_SetText( m_pTamerAchievementTitleText, wsAchievementTitle );

	for( std::map<int, sEquipIcon>::iterator it = m_mapEquipIcon.begin(); it != m_mapEquipIcon.end(); ++it )
	{
		uint nItemType = GetSystem()->GetEquipItemType( it->first );
		it->second.ChangeItem( nItemType, GetSystem()->GetFTItemCoolTime( nItemType ) );
	}

	for( int i = 0; i < eTamerDetailMax; ++i )
	{
		int const nValue = GetSystem()->GetTamerDetailInfoStat( i );
		_SetText( m_pTamerStatValue[i], FormatTamerDetailInfoValue( i, nValue ) );
	}
}

void cDetailInfoUI::_UpdateDigimonPage()
{
	_SetText( m_pDigimonNameText, GetSystem()->GetDigimonName() );

	CDigimonUser::sUSER_STAT* pStat = GetSystem()->GetDigimonBaseStat();
	if( pStat )
	{
		std::wstring wsLevel;
		DmCS::StringFn::Format( wsLevel, L"Nv%d", pStat->GetLevel() );
		_SetText( m_pDigimonLevelText, wsLevel );

		CsDigimon::sINFO* pFTInfo = GetSystem()->GetDigimonFTInfo();
		if( pFTInfo )
		{
			int nTall = 0;
			float fPercent = 0.f;
			bool bIsBuff = false;
			GetSystem()->GetDigimonSizeInfo( nTall, fPercent, bIsBuff );

			std::wstring wsInfo;
			DmCS::StringFn::Format( wsInfo, L"%s\n%d Nivel\n%d cm (%.2f%%)", pFTInfo->s_szName, pStat->GetLevel(), nTall, fPercent );
			_SetText( m_pDigimonInfoText, wsInfo );
		}
	}

	for( int i = 0; i < eDigiMax; ++i )
		_SetText( m_pDigimonStatValue[i], _GetDigimonAbilityText( static_cast<eDigimonAbilityType>( i ) ) );

	_UpdateSkillGrid();
	_UpdateEnchantPanel();
}

void cDetailInfoUI::_UpdateSkillGrid()
{
	cEvoUnit* pEvo = GetSystem()->GetEvoUnit();
	SAFE_POINTER_RET( pEvo );

	for( std::map<int, sSkillIcon>::iterator it = m_mapSkillIcon.begin(); it != m_mapSkillIcon.end(); ++it )
	{
		CsSkill::sINFO* pFTSkillInfo = GetSystem()->GetDigimonFTSkillInfo( it->first );
		if( !pFTSkillInfo )
		{
			it->second.ResetSkill();
			continue;
		}

		int nMaxLevel = pFTSkillInfo->s_nMaxLevel;
#ifdef SDM_DIGIMONSKILL_LV_EXPEND_20181206
		nMaxLevel = pEvo->m_nSkillMaxLevel[it->first];
#endif
		it->second.ChangeSkill( pFTSkillInfo->s_nIcon, pEvo->m_nSkillLevel[it->first], nMaxLevel );
	}
}

void cDetailInfoUI::_UpdateEnchantPanel()
{
	CDigimonUser::sENCHENT_STAT* pEnchantStat = GetSystem()->GetDigimonEnchantStat();
	if( !pEnchantStat )
		return;

	int nEnchantTypes[5] = { ET_AT, ET_BL, ET_CR, ET_EV, ET_HP };
	for( int i = 0; i < 5; ++i )
	{
		std::wstring wsText;
		DmCS::StringFn::From( wsText, pEnchantStat->GetEnchantLv( nEnchantTypes[i] ) );
		_SetText( m_pEnchantValue[i], wsText );
	}
}

void cDetailInfoUI::_RenderTamerPortrait()
{
	if( !g_pCharMng )
		return;

	CTamerUser* pTamer = g_pCharMng->GetTamerUser();
	if( !pTamer || !pTamer->GetCharImage() )
		return;

	pTamer->GetCharImage()->Render_L(
		GetTotalPosition() + CsPoint( DETAILINFO_TAMER_PORTRAIT_ICON_X, DETAILINFO_TAMER_PORTRAIT_ICON_Y ),
		CsPoint( DETAILINFO_TAMER_PORTRAIT_ICON_SIZE, DETAILINFO_TAMER_PORTRAIT_ICON_SIZE ) );
}

cSprite* cDetailInfoUI::_AddSolidSprite(CsPoint pos, CsPoint size, NiColorA const& color)
{
	cSprite* pSprite = NiNew cSprite;
	if( pSprite )
	{
		pSprite->Init( GetRoot(), pos, size, color, false );
		AddChildControl( pSprite );
	}
	return pSprite;
}

cSprite* cDetailInfoUI::_AddPageSprite(bool bTamerPage, CsPoint pos, CsPoint size, char const* pPath)
{
	cSprite* pSprite = AddSprite( pos, size, pPath );
	if( pSprite )
	{
		if( bTamerPage )
			m_vTamerControls.push_back( pSprite );
		else
			m_vDigimonControls.push_back( pSprite );
	}
	return pSprite;
}

cSprite* cDetailInfoUI::_AddPageSolidSprite(bool bTamerPage, CsPoint pos, CsPoint size, NiColorA const& color)
{
	cSprite* pSprite = _AddSolidSprite( pos, size, color );
	if( pSprite )
	{
		if( bTamerPage )
			m_vTamerControls.push_back( pSprite );
		else
			m_vDigimonControls.push_back( pSprite );
	}
	return pSprite;
}

cText* cDetailInfoUI::_AddPageText(bool bTamerPage, cText::sTEXTINFO* pTextInfo, CsPoint pos)
{
	cText* pText = AddText( pTextInfo, pos );
	if( pText )
	{
		if( bTamerPage )
			m_vTamerTexts.push_back( pText );
		else
			m_vDigimonTexts.push_back( pText );
	}
	return pText;
}

cButton* cDetailInfoUI::_AddPageButton(bool bTamerPage, CsPoint pos, CsPoint size, cButton::eIMAGE_TYPE eImageType, TCHAR const* pText)
{
	cButton* pButton = AddButton( pos, size, eImageType, pText );
	if( pButton )
	{
		if( bTamerPage )
			m_vTamerControls.push_back( pButton );
		else
			m_vDigimonControls.push_back( pButton );
	}
	return pButton;
}

void cDetailInfoUI::_AddStatTable(bool bTamerPage, CsPoint pos, int nRows, int nLabelWidth, int nValueWidth, int nRowHeight)
{
	int const nTableWidth = nLabelWidth + nValueWidth;
	int const nTableHeight = nRows * nRowHeight;
	NiColorA const kLineColor( 0.0f, 0.34f, 0.58f, 0.95f );
	NiColorA const kLabelColor( 0.07f, 0.16f, 0.28f, 0.96f );
	NiColorA const kValueColor( 0.0f, 0.035f, 0.065f, 0.98f );

	for( int i = 0; i < nRows; ++i )
	{
		CsPoint const ptRow( pos.x, pos.y + ( i * nRowHeight ) );
		_AddPageSolidSprite( bTamerPage, ptRow, CsPoint( nLabelWidth, nRowHeight ), kLabelColor );
		_AddPageSolidSprite( bTamerPage, CsPoint( ptRow.x + nLabelWidth, ptRow.y ), CsPoint( nValueWidth, nRowHeight ), kValueColor );
		_AddPageSolidSprite( bTamerPage, CsPoint( pos.x, ptRow.y ), CsPoint( nTableWidth, 1 ), kLineColor );
	}

	_AddPageSolidSprite( bTamerPage, CsPoint( pos.x, pos.y + nTableHeight ), CsPoint( nTableWidth, 1 ), kLineColor );
	_AddPageSolidSprite( bTamerPage, pos, CsPoint( 1, nTableHeight + 1 ), kLineColor );
	_AddPageSolidSprite( bTamerPage, CsPoint( pos.x + nLabelWidth, pos.y ), CsPoint( 1, nTableHeight + 1 ), kLineColor );
	_AddPageSolidSprite( bTamerPage, CsPoint( pos.x + nTableWidth, pos.y ), CsPoint( 1, nTableHeight + 1 ), kLineColor );
}

void cDetailInfoUI::_AddTamerStatRow(int nIndex, TCHAR const* pLabel, CsPoint pos)
{
	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_10, NiColor::WHITE );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( pLabel );
	_AddPageText( true, &ti, pos );

	ti.Init( CFont::FS_10, NiColor( 1, 235.f/255.f, 0 ) );
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T("-") );
	m_pTamerStatValue[nIndex] = _AddPageText( true, &ti, CsPoint( pos.x + 189, pos.y ) );
}

void cDetailInfoUI::_AddDigimonStatRow(eDigimonAbilityType eType, TCHAR const* pLabel, CsPoint pos)
{
	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_10, NiColor( 76.f/255.f, 242.f/255.f, 1 ) );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( pLabel );
	_AddPageText( false, &ti, pos );

	ti.Init( CFont::FS_10, NiColor( 1, 235.f/255.f, 0 ) );
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T("-") );
	m_pDigimonStatValue[eType] = _AddPageText( false, &ti, CsPoint( 452, pos.y ) );
}

void cDetailInfoUI::_AddEnchantRow(int nIndex, TCHAR const* pLabel, CsPoint pos)
{
	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_10, NiColor( 122.f/255.f, 254.f/255.f, 0 ) );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( pLabel );
	_AddPageText( false, &ti, pos );

	ti.Init( CFont::FS_10, NiColor::WHITE );
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T("0") );
	m_pEnchantValue[nIndex] = _AddPageText( false, &ti, CsPoint( pos.x + 60, pos.y ) );
}

std::wstring cDetailInfoUI::_GetDigimonAbilityText(eDigimonAbilityType eType) const
{
	CDigimonUser::sUSER_STAT* pStat = GetSystem()->GetDigimonBaseStat();
	SAFE_POINTER_RETVAL( pStat, L"" );

	std::wstring wsText;
	switch( eType )
	{
	case eDigiHP:
		DmCS::StringFn::From( wsText, pStat->GetMaxHP() );
		break;
	case eDigiDS:
		DmCS::StringFn::From( wsText, pStat->GetMaxDS() );
		break;
	case eDigiAT:
		DmCS::StringFn::From( wsText, pStat->GetAtt() );
		break;
	case eDigiAS:
		DmCS::StringFn::FloatToString( wsText, pStat->GetAttackSpeed(), 4 );
		break;
	case eDigiCT:
		DmCS::StringFn::Format( wsText, L"%d%%", pStat->GetCritical() / 100 );
		break;
	case eDigiHT:
		DmCS::StringFn::From( wsText, pStat->GetHitRate() );
		break;
	case eDigiDE:
		DmCS::StringFn::From( wsText, pStat->GetDef() );
		break;
	case eDigiBL:
		DmCS::StringFn::Format( wsText, L"%d%%", pStat->GetBL() );
		break;
	case eDigiEV:
		DmCS::StringFn::Format( wsText, L"%d%%", pStat->GetEvade() / 100 );
		break;
	default:
		break;
	}

	return wsText;
}

void cDetailInfoUI::_SetText(cText* pText, TCHAR const* pValue)
{
	if( pText )
		pText->SetText( pValue );
}

void cDetailInfoUI::_SetText(cString::sTEXT* pText, TCHAR const* pValue)
{
	if( pText )
		pText->SetText( pValue );
}

void cDetailInfoUI::_SetText(cText* pText, std::wstring const& value)
{
	_SetText( pText, value.c_str() );
}

void cDetailInfoUI::_SetText(cString::sTEXT* pText, std::wstring const& value)
{
	_SetText( pText, value.c_str() );
}

bool cDetailInfoUI::_SetEquipTooltip(cGridListBoxItem const* pOverItem)
{
	SAFE_POINTER_RETVAL( pOverItem, false );

	sEquipInfo* pEquipInfo = dynamic_cast<sEquipInfo*>( pOverItem->GetUserData() );
	SAFE_POINTER_RETVAL( pEquipInfo, false );
	if( 0 == pEquipInfo->nItemType )
		return false;

	cTooltip* pToolTip = TOOLTIPMNG_STPTR->GetTooltip();
	SAFE_POINTER_RETVAL( pToolTip, false );

	pToolTip->SetTooltip(
		pOverItem->GetWorldPos(),
		CsPoint( 32, 32 ),
		TOOLTIP_MAX_SIZE,
		cTooltip::ITEM,
		pEquipInfo->nItemType,
		cBaseWindow::WT_DETAIL_INFO,
		0,
		0,
		GetSystem()->GetEquipItem( pEquipInfo->nPartsType ) );

	return true;
}

bool cDetailInfoUI::_SetSkillTooltip(cGridListBoxItem const* pOverItem)
{
	SAFE_POINTER_RETVAL( pOverItem, false );
	if( !pOverItem->GetEnable() )
		return false;

	sSkillInfo* pSkillInfo = dynamic_cast<sSkillInfo*>( pOverItem->GetUserData() );
	SAFE_POINTER_RETVAL( pSkillInfo, false );

	CDigimonEvolveObj* pEvoObj = GetSystem()->GetEvoCurObject();
	SAFE_POINTER_RETVAL( pEvoObj, false );

	cTooltip* pTooltip = TOOLTIPMNG_STPTR->GetTooltip();
	SAFE_POINTER_RETVAL( pTooltip, false );

	int nTooltipSize = TOOLTIP_MAX_SIZE;
#ifdef VERSION_USA
	nTooltipSize = 360;
#endif

	int nValue = GetSystem()->IsUseSkill( pSkillInfo->nSkillIndex ) ? 0 : 1000;
	pTooltip->SetTooltip( pOverItem->GetWorldPos(), pOverItem->getItemSize(), nTooltipSize, cTooltip::SKILL, pSkillInfo->nSkillIndex, pEvoObj->m_nEvoSlot, cBaseWindow::WT_DETAIL_INFO, nValue );
	return true;
}

void cDetailInfoUI::_OnClickTabButton(void* pSender, void* pData)
{
	SAFE_POINTER_RET( pData );
	cRadioButton* pButton = static_cast<cRadioButton*>( pData );
	sTabInfo* pUserData = dynamic_cast<sTabInfo*>( pButton->GetUserData() );
	SAFE_POINTER_RET( pUserData );

	_SetTab( pUserData->eType );
	cWindow::PlaySound( cWindow::SD_At2 );
	CURSOR_ST.ReleaseIcon();
}

void cDetailInfoUI::_OnClickCloseButton(void* pSender, void* pData)
{
	Close();
}

void cDetailInfoUI::_OnClickEquipmentButton(void* pSender, void* pData)
{
	if( g_pGameIF && !g_pGameIF->IsActiveWindow( cBaseWindow::WT_NEW_TAMERSTATUS ) )
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_NEW_TAMERSTATUS, 0, 3 );
}

void cDetailInfoUI::_OnClickSealMasterButton(void* pSender, void* pData)
{
	if( g_pGameIF )
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_CARDINVENTORY );
}

void cDetailInfoUI::sEquipIcon::ChangeItem(uint nItemType, CsCoolTimeSeq* pCoolTime)
{
	SAFE_POINTER_RET( pGridItem );
	SAFE_POINTER_RET( pItemIcon );

	pItemIcon->ChangeIcon( ICONITEM::ITEM_ICON, nItemType, 1 );
	pItemIcon->SetCoolTimeSeq( pCoolTime );

	sEquipInfo* pEquipInfo = dynamic_cast<sEquipInfo*>( pGridItem->GetUserData() );
	SAFE_POINTER_RET( pEquipInfo );
	pEquipInfo->nItemType = nItemType;

	pGridItem->SetEnable( nItemType != 0 );
	pItemIcon->SetVisible( nItemType != 0 );
}

void cDetailInfoUI::sSkillIcon::ResetSkill()
{
	if( pGridItem )
		pGridItem->SetEnable( false );
	if( pSkillIcon )
	{
		pSkillIcon->SetEnable( false );
		pSkillIcon->SetVisible( false );
	}
	if( pSkillLevelText )
	{
		pSkillLevelText->SetVisible( false );
		pSkillLevelText->SetText( _T("") );
	}
}

void cDetailInfoUI::sSkillIcon::ChangeSkill(int nIcon, int nLevel, int nMaxLevel)
{
	if( pGridItem )
		pGridItem->SetEnable( true );
	if( pSkillIcon )
	{
		pSkillIcon->SetEnable( true );
		pSkillIcon->SetVisible( true );
		pSkillIcon->ChangeSkillIcon( nIcon, nIcon );
	}
	if( pSkillLevelText )
	{
		std::wstring wsLevel;
		if( nLevel >= nMaxLevel )
			wsLevel = L"Max";
		else
			DmCS::StringFn::Format( wsLevel, L"%d/%d", nLevel, nMaxLevel );
		pSkillLevelText->SetText( wsLevel.c_str() );
		pSkillLevelText->SetVisible( true );
	}
}
