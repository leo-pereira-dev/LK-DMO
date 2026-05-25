#include "stdafx.h"
#include "MainFrame_DetailInfoUI.h"
#include "../Game/AchieveContents.h"
#include "../Game/_GameIF.h"

namespace
{
	const int DETAILINFO_WIDTH = 628;
	const int DETAILINFO_HEIGHT = 694;
	const int DETAILINFO_SKILL_COUNT = 9;
	const int DETAILINFO_TAMER_PANEL_WIDTH = 406;
	const int DETAILINFO_TAMER_CENTER_X = 211;
	const int DETAILINFO_TAMER_RENDER_X = 18;
	const int DETAILINFO_TAMER_RENDER_Y = 222;
	const int DETAILINFO_TAMER_STAT_X = 422;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_X = 176;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_Y = 126;
	const int DETAILINFO_TAMER_PORTRAIT_ICON_SIZE = 70;
	const int DETAILINFO_DIGIMON_LEFT_PANEL_X = 8;
	const int DETAILINFO_DIGIMON_LEFT_PANEL_WIDTH = 380;
	const int DETAILINFO_DIGIMON_CENTER_X = DETAILINFO_DIGIMON_LEFT_PANEL_X + ( DETAILINFO_DIGIMON_LEFT_PANEL_WIDTH / 2 );
	const int DETAILINFO_DIGIMON_PORTRAIT_FIELD_X = 24;
	const int DETAILINFO_DIGIMON_PORTRAIT_FIELD_Y = 114;
	const int DETAILINFO_DIGIMON_PORTRAIT_FIELD_WIDTH = 354;
	const int DETAILINFO_DIGIMON_PORTRAIT_FIELD_HEIGHT = 132;
	const int DETAILINFO_DIGIMON_PORTRAIT_FRAME_X = DETAILINFO_DIGIMON_CENTER_X - 42;
	const int DETAILINFO_DIGIMON_PORTRAIT_FRAME_Y = 132;
	const int DETAILINFO_DIGIMON_PORTRAIT_FRAME_SIZE = 84;
	const int DETAILINFO_DIGIMON_PORTRAIT_ICON_X = DETAILINFO_DIGIMON_PORTRAIT_FRAME_X + 5;
	const int DETAILINFO_DIGIMON_PORTRAIT_ICON_Y = DETAILINFO_DIGIMON_PORTRAIT_FRAME_Y + 5;
	const int DETAILINFO_DIGIMON_PORTRAIT_ICON_SIZE = 74;
	const int DETAILINFO_DIGIMON_INFO_FIELD_X = 30;
	const int DETAILINFO_DIGIMON_INFO_FIELD_Y = 264;
	const int DETAILINFO_DIGIMON_INFO_FIELD_WIDTH = 342;
	const int DETAILINFO_DIGIMON_INFO_FIELD_HEIGHT = 96;
	const int DETAILINFO_DIGIMON_SKILL_PANEL_X = 30;
	const int DETAILINFO_DIGIMON_SKILL_PANEL_Y = 374;
	const int DETAILINFO_DIGIMON_SKILL_PANEL_WIDTH = 342;
	const int DETAILINFO_DIGIMON_SKILL_PANEL_HEIGHT = 160;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_SIZE = 46;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_X = 72;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_Y = 382;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_FRAME_OFFSET_Y = -1;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_STEP_X = 104;
	const int DETAILINFO_DIGIMON_SKILL_SLOT_STEP_Y = DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 20;
	const int DETAILINFO_DIGIMON_SKILL_ICON_SIZE = 30;
	const int DETAILINFO_DIGIMON_SKILL_GRID_WIDTH = 260;
	const int DETAILINFO_DIGIMON_SKILL_GRID_HEIGHT = 132;
	const int DETAILINFO_DIGIMON_SKILL_GRID_X = DETAILINFO_DIGIMON_SKILL_SLOT_X + ( ( DETAILINFO_DIGIMON_SKILL_SLOT_SIZE - DETAILINFO_DIGIMON_SKILL_ICON_SIZE ) / 2 );
	const int DETAILINFO_DIGIMON_SKILL_GRID_Y = DETAILINFO_DIGIMON_SKILL_SLOT_Y + ( ( DETAILINFO_DIGIMON_SKILL_SLOT_SIZE - DETAILINFO_DIGIMON_SKILL_ICON_SIZE ) / 2 );
	const int DETAILINFO_DIGIMON_SKILL_GRID_STEP_X = DETAILINFO_DIGIMON_SKILL_SLOT_STEP_X - DETAILINFO_DIGIMON_SKILL_ICON_SIZE;
	const int DETAILINFO_DIGIMON_SKILL_GRID_STEP_Y = DETAILINFO_DIGIMON_SKILL_SLOT_STEP_Y - DETAILINFO_DIGIMON_SKILL_ICON_SIZE;
	const int DETAILINFO_DIGIMON_STAT_LABEL_WIDTH = 76;
	const int DETAILINFO_DIGIMON_STAT_VALUE_WIDTH = 144;
	const int DETAILINFO_DIGIMON_STAT_ROW_HEIGHT = 33;
	const int DETAILINFO_DIGIMON_STAT_TABLE_WIDTH = DETAILINFO_DIGIMON_STAT_LABEL_WIDTH + DETAILINFO_DIGIMON_STAT_VALUE_WIDTH;
	const int DETAILINFO_DIGIMON_STAT_X = DETAILINFO_WIDTH - 15 - DETAILINFO_DIGIMON_STAT_TABLE_WIDTH;
	const int DETAILINFO_DIGIMON_STAT_Y = 160;
	const int DETAILINFO_DIGIMON_STAT_HEADER_X = DETAILINFO_DIGIMON_STAT_X + ( DETAILINFO_DIGIMON_STAT_TABLE_WIDTH / 2 );
	const int DETAILINFO_DIGIMON_ADVANCE_WIDTH = DETAILINFO_DIGIMON_STAT_TABLE_WIDTH;
	const int DETAILINFO_DIGIMON_ADVANCE_X = DETAILINFO_WIDTH - 15 - DETAILINFO_DIGIMON_ADVANCE_WIDTH;
	const int DETAILINFO_DIGIMON_ADVANCE_Y = 530;
	const int DETAILINFO_DIGIMON_ADVANCE_HEADER_X = DETAILINFO_DIGIMON_ADVANCE_X + ( DETAILINFO_DIGIMON_ADVANCE_WIDTH / 2 );
	const int DETAILINFO_DIGIMON_ENCHANT_PANEL_X = 24;
	const int DETAILINFO_DIGIMON_ENCHANT_PANEL_Y = 530;
	const int DETAILINFO_DIGIMON_ENCHANT_PANEL_WIDTH = 352;
	const int DETAILINFO_DIGIMON_ENCHANT_PANEL_HEIGHT = 132;
	const int DETAILINFO_DIGIMON_ENCHANT_TITLE_X = 152;
	const int DETAILINFO_DIGIMON_ENCHANT_TITLE_Y = 512;
	const int DETAILINFO_DIGIMON_ENCHANT_TOTAL_ICON_X = 225;
	const int DETAILINFO_DIGIMON_ENCHANT_TOTAL_ICON_Y = 501;
	const int DETAILINFO_DIGIMON_ENCHANT_TOTAL_TEXT_X = 292;
	const int DETAILINFO_DIGIMON_ENCHANT_BADGE_WIDTH = 82;
	const int DETAILINFO_DIGIMON_ENCHANT_BADGE_HEIGHT = 20;
	const int DETAILINFO_DIGIMON_ENCHANT_LEVEL_ICON_WIDTH = 28;
	const int DETAILINFO_DIGIMON_ENCHANT_LEVEL_ICON_HEIGHT = 20;
	const int DETAILINFO_ADDITIONAL_STAT_COUNT = 15;
	const int DETAILINFO_ADDITIONAL_STAT_WIDTH = 300;
	const int DETAILINFO_ADDITIONAL_STAT_HEIGHT = 489;
	const int DETAILINFO_ADDITIONAL_STAT_X = DETAILINFO_WIDTH - DETAILINFO_ADDITIONAL_STAT_WIDTH - 8;
	const int DETAILINFO_ADDITIONAL_STAT_Y = 38;
	const int DETAILINFO_ADDITIONAL_STAT_TITLE_HEIGHT = 32;
	const int DETAILINFO_ADDITIONAL_STAT_TITLE_Y = DETAILINFO_ADDITIONAL_STAT_Y + 11;
	const int DETAILINFO_ADDITIONAL_STAT_ROW_X = DETAILINFO_ADDITIONAL_STAT_X + 52;
	const int DETAILINFO_ADDITIONAL_STAT_ROW_Y = DETAILINFO_ADDITIONAL_STAT_Y + 72;
	const int DETAILINFO_ADDITIONAL_STAT_ROW_WIDTH = 222;
	const int DETAILINFO_ADDITIONAL_STAT_ROW_HEIGHT = 35;
	const int DETAILINFO_ADDITIONAL_STAT_VISIBLE_COUNT = 11;
	const int DETAILINFO_ADDITIONAL_STAT_ICON_X = DETAILINFO_ADDITIONAL_STAT_X + 18;
	const int DETAILINFO_ADDITIONAL_STAT_ICON_SIZE = 30;
	const int DETAILINFO_ADDITIONAL_STAT_TEXT_X = DETAILINFO_ADDITIONAL_STAT_ROW_X + 11;
	const int DETAILINFO_ADDITIONAL_STAT_VALUE_X = DETAILINFO_ADDITIONAL_STAT_ROW_X + DETAILINFO_ADDITIONAL_STAT_ROW_WIDTH - 12;

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

	enum eXmlUnionCollectionBonusIndex
	{
		eXmlUnionBonusHP,
		eXmlUnionBonusDS,
		eXmlUnionBonusAT,
		eXmlUnionBonusDE,
		eXmlUnionBonusEV,
		eXmlUnionBonusHT,
		eXmlUnionBonusCT,
		eXmlUnionBonusEXP,
		eXmlUnionBonusBL,
		eXmlUnionBonusSCD,
		eXmlUnionBonusBasic,
		eXmlUnionBonusVaccine,
		eXmlUnionBonusData,
		eXmlUnionBonusVirus,
		eXmlUnionBonusUnknown,
		eXmlUnionBonusFire,
		eXmlUnionBonusWater,
		eXmlUnionBonusIce,
		eXmlUnionBonusWind,
		eXmlUnionBonusThunder,
		eXmlUnionBonusLight,
		eXmlUnionBonusDarkness,
		eXmlUnionBonusEarth,
		eXmlUnionBonusWood,
		eXmlUnionBonusSteel,
		eXmlUnionBonusMax,
	};

	const int DETAILINFO_DIGIMON_STAT_COUNT = 9;
	const TCHAR* const DIGIMON_STAT_LABELS[DETAILINFO_DIGIMON_STAT_COUNT] =
	{
		_T("HP"), _T("DS"), _T("AT"), _T("AS"), _T("CT"),
		_T("HT"), _T("DE"), _T("BL"), _T("EV")
	};

	const char* const DIGIMON_ENCHANT_BADGE_PATHS[5] =
	{
		"DetailInfo\\Enchant_AT.tga",
		"DetailInfo\\Enchant_bl.tga",
		"DetailInfo\\Enchant_ct.tga",
		"DetailInfo\\Enchant_ev.tga",
		"DetailInfo\\Enchant_HP.tga"
	};

	const TCHAR* const ADDITIONAL_STAT_LABELS[DETAILINFO_ADDITIONAL_STAT_COUNT] =
	{
		_T("Dano de Gelo"),
		_T("Dano de Agua"),
		_T("Dano de Fogo"),
		_T("Dano de Terra"),
		_T("Dano de Vento"),
		_T("Dano de Madeira"),
		_T("Dano de Luz"),
		_T("Dano de Trevas"),
		_T("Dano de Trovao"),
		_T("Dano de Aco"),
		_T("Dano Base"),
		_T("Dano de Dados"),
		_T("Dano de Vacina"),
		_T("Dano de Virus"),
		_T("Dano desconhecido")
	};

	const int ADDITIONAL_STAT_ICON_INDEX[DETAILINFO_ADDITIONAL_STAT_COUNT] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		nsCsDigimonTable::AT_DATA - nsCsDigimonTable::AT_NONE,
		nsCsDigimonTable::AT_VACCINE - nsCsDigimonTable::AT_NONE,
		nsCsDigimonTable::AT_VIRUS - nsCsDigimonTable::AT_NONE,
		4
	};

	const cIconMng::eDAttribute ADDITIONAL_STAT_ICON_TYPE[DETAILINFO_ADDITIONAL_STAT_COUNT] =
	{
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::Nature,
		cIconMng::DAttribute,
		cIconMng::DAttribute,
		cIconMng::DAttribute,
		cIconMng::DAttribute
	};

	CsPoint GetDigimonEnchantBadgePos(int nIndex)
	{
		int const nBadgeY = DETAILINFO_DIGIMON_ENCHANT_PANEL_Y + 14;
		switch( nIndex )
		{
		case 0: return CsPoint( 52, nBadgeY );
		case 1: return CsPoint( 218, nBadgeY );
		case 2: return CsPoint( 52, nBadgeY + 46 );
		case 3: return CsPoint( 218, nBadgeY + 46 );
		case 4: return CsPoint( 52, nBadgeY + 92 );
		default: return CsPoint::ZERO;
		}
	}

	CsPoint GetDigimonEnchantLevelIconPos(int nIndex)
	{
		CsPoint const ptBadge = GetDigimonEnchantBadgePos( nIndex );
		return CsPoint( ptBadge.x + 66, ptBadge.y );
	}

	CsPoint GetDigimonSkillSlotPos(int nIndex)
	{
		return CsPoint(
			DETAILINFO_DIGIMON_SKILL_SLOT_X + -1 +( ( nIndex % 3 ) * DETAILINFO_DIGIMON_SKILL_SLOT_STEP_X ),
			DETAILINFO_DIGIMON_SKILL_SLOT_Y + DETAILINFO_DIGIMON_SKILL_SLOT_FRAME_OFFSET_Y + ( ( nIndex / 3 ) * DETAILINFO_DIGIMON_SKILL_SLOT_STEP_Y ) );
	}

	CsPoint ToAdditionalStatLocal(CsPoint ptPos)
	{
		return ptPos - CsPoint( DETAILINFO_ADDITIONAL_STAT_X, DETAILINFO_ADDITIONAL_STAT_Y );
	}

	CsPoint ClampAdditionalStatPopupPos(CsPoint ptPos)
	{
		if( ptPos.x < 0 )
			ptPos.x = 0;
		else if( ptPos.x + DETAILINFO_ADDITIONAL_STAT_WIDTH > g_nScreenWidth )
			ptPos.x = g_nScreenWidth - DETAILINFO_ADDITIONAL_STAT_WIDTH;

		if( ptPos.y < 0 )
			ptPos.y = 0;
		else if( ptPos.y + DETAILINFO_ADDITIONAL_STAT_HEIGHT > g_nScreenHeight )
			ptPos.y = g_nScreenHeight - DETAILINFO_ADDITIONAL_STAT_HEIGHT;

		return ptPos;
	}

	int ClampEnchantLevelIconIndex(int nLevel)
	{
		if( nLevel < 0 )
			return 0;
		if( nLevel > 19 )
			return 19;
		return nLevel;
	}

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

	std::wstring FormatWholePercentValue(int nValue)
	{
		std::wstring wsText;
		DmCS::StringFn::Format( wsText, L"%.2f%%", (double)nValue );
		return wsText;
	}

	int GetXmlUnionCollectionBonusValue(int nIndex)
	{
		if( nIndex < 0 || nIndex >= eXmlUnionBonusMax || g_pGameIF == NULL || g_pGameIF->GetXmlUnionDUnit() == NULL )
			return 0;

		return g_pGameIF->GetXmlUnionDUnit()->GetCollectionBonusValue( nIndex );
	}

	std::wstring FormatTamerDetailInfoValue(int nIndex, int nValue)
	{
		switch( nIndex )
		{
		case eTamerDetailAS:
		case eTamerDetailCT:
		case eTamerDetailBL:
		case eTamerDetailEV:
		case eTamerDetailSCD:
		case eTamerDetailCD:
		case eTamerDetailBaseDamage:
			return FormatBasisPointPercentValue( nValue );

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
m_pDigimonZoomButton(NULL),
m_pAdditionalStatCloseButton(NULL),
m_pAdditionalStatScrollBar(NULL),
m_pAdditionalStatPopup(NULL),
m_pTamerLeftEquipList(NULL),
m_pTamerRightEquipList(NULL),
m_pDigimonSkillGrid(NULL),
m_pTamerRenderTex(NULL),
m_pDigimonRenderTex(NULL),
m_eCurrentTab(eTabTamer),
m_bPendingRefresh(false),
m_bAdditionalStatVisible(false),
m_bAdditionalStatDragging(false),
m_bAdditionalStatUserMoved(false),
m_ptAdditionalStatDragOffset(0, 0),
m_pTamerNameText(NULL),
m_pTamerLevelText(NULL),
m_pTamerAchievementTitleText(NULL),
m_pDigimonNameText(NULL),
m_pDigimonLevelText(NULL),
m_pDigimonInfoText(NULL),
m_pDigimonInfoLevelText(NULL),
m_pDigimonInfoSizeText(NULL),
m_pEnchantTotalText(NULL)
{
	for( int i = 0; i < 13; ++i )
		m_pTamerStatValue[i] = NULL;
	for( int i = 0; i < eDigiMax; ++i )
		m_pDigimonStatValue[i] = NULL;
	for( int i = 0; i < 5; ++i )
	{
		m_pEnchantValue[i] = NULL;
		m_nEnchantLevel[i] = 0;
	}
	for( int i = 0; i < eAddStatMax; ++i )
	{
		m_pAdditionalStatRowSprite[i] = NULL;
		m_pAdditionalStatLabel[i] = NULL;
		m_pAdditionalStatValue[i] = NULL;
	}
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
	if( m_pAdditionalStatPopup )
	{
		m_pAdditionalStatPopup->DeleteScript();
		SAFE_NIDELETE( m_pAdditionalStatPopup );
	}

	DeleteScript();
	m_mapEquipIcon.clear();
	m_mapSkillIcon.clear();
	m_vTamerControls.clear();
	m_vTamerTexts.clear();
	m_vDigimonControls.clear();
	m_vDigimonTexts.clear();
	m_vAdditionalStatControls.clear();
	m_vAdditionalStatTexts.clear();
	m_pAdditionalStatCloseButton = NULL;
	m_pAdditionalStatScrollBar = NULL;
	m_bAdditionalStatVisible = false;
	m_bAdditionalStatDragging = false;
	m_bAdditionalStatUserMoved = false;
	m_ptAdditionalStatDragOffset = CsPoint( 0, 0 );
	for( int i = 0; i < eAddStatMax; ++i )
	{
		m_pAdditionalStatRowSprite[i] = NULL;
		m_pAdditionalStatLabel[i] = NULL;
		m_pAdditionalStatValue[i] = NULL;
	}
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
	cText* pTitle = AddTitle( _T("Detalhes"), CsPoint( 0, 14 ), CFont::FS_16 );
	if( pTitle )
		pTitle->GetTextInfo()->SetBoldLevel( cText::sTEXTINFO::BL_2 );

	m_pVisibilityButton = AddButton( CsPoint( 6, 11 ), CsPoint( 52, 22 ), CsPoint( 0, 22 ), "DetailInfo\\Autocrop_ON_Btn.bmp" );

	m_pCloseButton = AddButton( CsPoint( DETAILINFO_WIDTH - 30, 10 ), CsPoint( 30, 28 ), CsPoint( 0, 28 ), "Event\\event_close.tga" );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickCloseButton );

	_MakeTabButtons();
	_MakeTamerPage();
	_MakeDigimonPage();
	_MakeAdditionalStatPopup();
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

	if( m_bAdditionalStatVisible && m_pAdditionalStatPopup )
	{
		_UpdateAdditionalStatPopupDragging();
		m_pAdditionalStatPopup->UpdateScript( fDeltaTime );
	}
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cDetailInfoUI::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( m_bAdditionalStatVisible && _UpdateAdditionalStatPopupForMouse() )
		return MUT_CLICK_FOCUS;

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
		if( m_pDigimonZoomButton && m_pDigimonZoomButton->Update_ForMouse() )
			return muReturn;

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
		_RenderDigimonPortrait();
		_RenderDigimonEnchantIcons();
	}

	if( m_bAdditionalStatVisible && m_pAdditionalStatPopup )
	{
		m_pAdditionalStatPopup->RenderScript();
		_RenderAdditionalStatIcons();
	}
}

void cDetailInfoUI::ResetDevice()
{
	ResetDeviceScript();
	if( m_pTamerRenderTex )
		m_pTamerRenderTex->ResetDevice();
	if( m_pDigimonRenderTex )
		m_pDigimonRenderTex->ResetDevice();
	if( m_pAdditionalStatPopup )
		m_pAdditionalStatPopup->ResetDeviceScript();
}

bool cDetailInfoUI::OnEscapeKey()
{
	if( m_bAdditionalStatVisible )
	{
		_SetAdditionalStatPopupVisible( false );
		return true;
	}

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
	ti.Init( CFont::FS_11, NiColor::WHITE );
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
	_AddPageSolidSprite( false, CsPoint( DETAILINFO_DIGIMON_LEFT_PANEL_X, 86 ), CsPoint( DETAILINFO_DIGIMON_LEFT_PANEL_WIDTH, 575 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.94f ) );
	_AddPageSolidSprite( false, CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FIELD_X, DETAILINFO_DIGIMON_PORTRAIT_FIELD_Y ), CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FIELD_WIDTH, DETAILINFO_DIGIMON_PORTRAIT_FIELD_HEIGHT ), NiColorA( 0.0f, 0.015f, 0.025f, 0.92f ) );
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FIELD_X, DETAILINFO_DIGIMON_PORTRAIT_FIELD_Y ), CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FIELD_WIDTH, DETAILINFO_DIGIMON_PORTRAIT_FIELD_HEIGHT ), "DetailInfo\\Storage_M_field_a.tga" );
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FRAME_X, DETAILINFO_DIGIMON_PORTRAIT_FRAME_Y ), CsPoint( DETAILINFO_DIGIMON_PORTRAIT_FRAME_SIZE, DETAILINFO_DIGIMON_PORTRAIT_FRAME_SIZE ), "DetailInfo\\TamerStatus_Digimon_Tamer.tga" );
	_AddPageSolidSprite( false, CsPoint( DETAILINFO_DIGIMON_STAT_X - 12, 156 ), CsPoint( DETAILINFO_DIGIMON_STAT_TABLE_WIDTH + 12, 328 ), NiColorA( 0.0f, 0.035f, 0.07f, 0.96f ) );
	_AddStatTable( false, CsPoint( DETAILINFO_DIGIMON_STAT_X, DETAILINFO_DIGIMON_STAT_Y ), eDigiMax, DETAILINFO_DIGIMON_STAT_LABEL_WIDTH, DETAILINFO_DIGIMON_STAT_VALUE_WIDTH, DETAILINFO_DIGIMON_STAT_ROW_HEIGHT );
	m_pDigimonZoomButton = AddButton( CsPoint( DETAILINFO_DIGIMON_STAT_X + DETAILINFO_DIGIMON_STAT_TABLE_WIDTH - 31, 132 ), CsPoint( 26, 26 ), CsPoint( 0, 26 ), "DetailInfo\\zoom_in_aa.tga" );
	if( m_pDigimonZoomButton )
	{
		m_pDigimonZoomButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickDigimonZoomButton );
		m_vDigimonControls.push_back( m_pDigimonZoomButton );
	}
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_INFO_FIELD_X, DETAILINFO_DIGIMON_INFO_FIELD_Y ), CsPoint( DETAILINFO_DIGIMON_INFO_FIELD_WIDTH, DETAILINFO_DIGIMON_INFO_FIELD_HEIGHT ), "DetailInfo\\RemainTimerBG.tga" );
	_AddPageSolidSprite( false, CsPoint( DETAILINFO_DIGIMON_SKILL_PANEL_X, DETAILINFO_DIGIMON_SKILL_PANEL_Y ), CsPoint( DETAILINFO_DIGIMON_SKILL_PANEL_WIDTH, DETAILINFO_DIGIMON_SKILL_PANEL_HEIGHT ), NiColorA( 0.0f, 0.035f, 0.07f, 0.90f ) );
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_ENCHANT_PANEL_X, DETAILINFO_DIGIMON_ENCHANT_PANEL_Y ), CsPoint( DETAILINFO_DIGIMON_ENCHANT_PANEL_WIDTH, DETAILINFO_DIGIMON_ENCHANT_PANEL_HEIGHT ), "TacticsHouse\\CheckOnline.tga" );
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_ADVANCE_X, DETAILINFO_DIGIMON_ADVANCE_Y ), CsPoint( DETAILINFO_DIGIMON_ADVANCE_WIDTH, DETAILINFO_DIGIMON_ENCHANT_PANEL_HEIGHT ), "TacticsHouse\\CheckOnline.tga" );

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_14, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("") );
	m_pDigimonNameText = _AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_CENTER_X, 98 ) );

	ti.Init( CFont::FS_13, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("") );
	m_pDigimonLevelText = _AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_CENTER_X, 214 ) );

	ti.Init( CFont::FS_12, NiColor::WHITE );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( _T("") );
	m_pDigimonInfoText = _AddPageText( false, &ti, CsPoint( 57, 291 ) );
	ti.SetText( _T("") );
	m_pDigimonInfoLevelText = _AddPageText( false, &ti, CsPoint( 57, 313 ) );
	ti.SetText( _T("") );
	m_pDigimonInfoSizeText = _AddPageText( false, &ti, CsPoint( 57, 335 ) );

	ti.Init( CFont::FS_13, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Informacao Digimon") );
	_AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_CENTER_X, 252 ) );

	ti.SetText( _T("Habilidade") );
	_AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_CENTER_X, 360 ) );

	ti.SetText( _T("Atributos") );
	_AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_STAT_HEADER_X, 132 ) );

	ti.Init( CFont::FS_13, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Encanto Digimon") );
	_AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_ENCHANT_TITLE_X, DETAILINFO_DIGIMON_ENCHANT_TITLE_Y ) );
	_AddPageSprite( false, CsPoint( DETAILINFO_DIGIMON_ENCHANT_TOTAL_ICON_X, DETAILINFO_DIGIMON_ENCHANT_TOTAL_ICON_Y ), CsPoint( 32, 32 ), "DigimonStatus_New\\Digimon_Status_Enchant_01.tga" );

	ti.Init( CFont::FS_12, NiColor::WHITE );
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T("0/75") );
	m_pEnchantTotalText = _AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_ENCHANT_TOTAL_TEXT_X, DETAILINFO_DIGIMON_ENCHANT_TITLE_Y ) );

	ti.Init( CFont::FS_13, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Avanco de Digimon") );
	_AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_ADVANCE_HEADER_X, DETAILINFO_DIGIMON_ENCHANT_TITLE_Y ) );

	for( int i = 0; i < eDigiMax; ++i )
		_AddDigimonStatRow( static_cast<eDigimonAbilityType>( i ), DIGIMON_STAT_LABELS[i], CsPoint( DETAILINFO_DIGIMON_STAT_X + 7, 168 + ( i * DETAILINFO_DIGIMON_STAT_ROW_HEIGHT ) ) );

	for( int i = 0; i < 5; ++i )
		_AddEnchantRow( i, DIGIMON_ENCHANT_BADGE_PATHS[i], GetDigimonEnchantBadgePos( i ) );

	_AddPageSprite(false, GetDigimonSkillSlotPos(0), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE+2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE+2), "EquipSystem\\Upgrade_Slot.tga");
	_AddPageSprite(false, GetDigimonSkillSlotPos(1), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2), "EquipSystem\\Upgrade_Slot.tga");
	_AddPageSprite(false, GetDigimonSkillSlotPos(2), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2), "EquipSystem\\Upgrade_Slot.tga");
	_AddPageSprite(false, GetDigimonSkillSlotPos(3), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2), "EquipSystem\\Upgrade_Slot.tga");
	_AddPageSprite(false, GetDigimonSkillSlotPos(4), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2), "EquipSystem\\Upgrade_Slot.tga");
	_AddPageSprite(false, GetDigimonSkillSlotPos(5), CsPoint(DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2, DETAILINFO_DIGIMON_SKILL_SLOT_SIZE + 2), "EquipSystem\\Upgrade_Slot.tga");



	for( int i = 0; i < DETAILINFO_SKILL_COUNT; ++i )
	m_pDigimonSkillGrid = NiNew cGridListBox;
	if( m_pDigimonSkillGrid )
	{
		m_pDigimonSkillGrid->Init( m_pRoot, CsPoint( DETAILINFO_DIGIMON_SKILL_GRID_X, DETAILINFO_DIGIMON_SKILL_GRID_Y ), CsPoint( DETAILINFO_DIGIMON_SKILL_GRID_WIDTH, DETAILINFO_DIGIMON_SKILL_GRID_HEIGHT ), CsPoint( DETAILINFO_DIGIMON_SKILL_GRID_STEP_X, DETAILINFO_DIGIMON_SKILL_GRID_STEP_Y ), CsPoint( DETAILINFO_DIGIMON_SKILL_ICON_SIZE, DETAILINFO_DIGIMON_SKILL_ICON_SIZE ), cGridListBox::LowRightDown, cGridListBox::LeftTop, NULL, false, 3 );
		m_pDigimonSkillGrid->SetAutoSelection( false );
		m_pDigimonSkillGrid->SetBackOverAndSelectedImgRender( false );
		AddChildControl( m_pDigimonSkillGrid );
		m_vDigimonControls.push_back( m_pDigimonSkillGrid );

		for( int i = 0; i < DETAILINFO_SKILL_COUNT; ++i )
		{
			cString* pControl = NiNew cString;
			SAFE_POINTER_CON( pControl );

			sSkillIcon skill;
			skill.pSkillIcon = pControl->AddSkillIcon( CsPoint( DETAILINFO_DIGIMON_SKILL_ICON_SIZE, DETAILINFO_DIGIMON_SKILL_ICON_SIZE ), 0, 0 );
			if( skill.pSkillIcon )
			{
				skill.pSkillIcon->SetAutoPointerDelete( true );
				skill.pSkillIcon->SetEnable( false );
				skill.pSkillIcon->SetVisible( false );
			}

			cText::sTEXTINFO skillTi;
			skillTi.Init( CFont::FS_12, NiColor( 76.f/255.f, 242.f/255.f, 1 ) );
			skillTi.s_eTextAlign = DT_CENTER;
			skillTi.SetText( _T("") );
			skill.pSkillLevelText = pControl->AddText( &skillTi, CsPoint( 15, 32 ) );
			if( skill.pSkillLevelText )
			{
				skill.pSkillLevelText->SetAutoPointerDelete( true );
				skill.pSkillLevelText->SetVisible( false );
			}

			skill.pGridItem = NiNew cGridListBoxItem( i, CsPoint( DETAILINFO_DIGIMON_SKILL_ICON_SIZE, DETAILINFO_DIGIMON_SKILL_ICON_SIZE ) );
			if( skill.pGridItem )
			{
				skill.pGridItem->SetItem( pControl );
				skill.pGridItem->setItemSize( CsPoint( DETAILINFO_DIGIMON_SKILL_ICON_SIZE, DETAILINFO_DIGIMON_SKILL_ICON_SIZE ) );
				skill.pGridItem->SetUserData( new sSkillInfo( i ) );
				m_pDigimonSkillGrid->AddItem( skill.pGridItem );
			}

			m_mapSkillIcon.insert( std::make_pair( i, skill ) );
		}
	}

}

void cDetailInfoUI::_MakeAdditionalStatPopup()
{
	m_pAdditionalStatPopup = NiNew cScriptUI;
	if( !m_pAdditionalStatPopup )
		return;

	m_pAdditionalStatPopup->InitScript(
		NULL,
		ClampAdditionalStatPopupPos( GetTotalPosition() + CsPoint( DETAILINFO_ADDITIONAL_STAT_X, DETAILINFO_ADDITIONAL_STAT_Y ) ),
		CsPoint( DETAILINFO_ADDITIONAL_STAT_WIDTH, DETAILINFO_ADDITIONAL_STAT_HEIGHT ),
		true,
		IFREGION_X::LEFT,
		IFREGION_Y::TOP,
		false );

	cSprite* pSolidBackground = NiNew cSprite;
	if( pSolidBackground )
	{
		pSolidBackground->Init( m_pAdditionalStatPopup->GetRoot(), CsPoint::ZERO, CsPoint( DETAILINFO_ADDITIONAL_STAT_WIDTH, DETAILINFO_ADDITIONAL_STAT_HEIGHT ), NiColorA( 0.0f, 0.025f, 0.055f, 0.98f ), false );
		m_pAdditionalStatPopup->AddChildControl( pSolidBackground );
		m_vAdditionalStatControls.push_back( pSolidBackground );
	}

	_AddAdditionalStatSprite(
		CsPoint( DETAILINFO_ADDITIONAL_STAT_X, DETAILINFO_ADDITIONAL_STAT_Y ),
		CsPoint( DETAILINFO_ADDITIONAL_STAT_WIDTH, DETAILINFO_ADDITIONAL_STAT_HEIGHT ),
		"DetailInfo\\skin_bg_a.tga" );

	cText::sTEXTINFO ti;
	ti.Init( CFont::FS_12, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( _T("Atributos Adicionais") );
	cText* pTitle = _AddAdditionalStatText( &ti, CsPoint( DETAILINFO_ADDITIONAL_STAT_X + ( DETAILINFO_ADDITIONAL_STAT_WIDTH / 2 ), DETAILINFO_ADDITIONAL_STAT_TITLE_Y ) );
	if( pTitle )
		pTitle->GetTextInfo()->SetBoldLevel( cText::sTEXTINFO::BL_1 );

	m_pAdditionalStatCloseButton = m_pAdditionalStatPopup->AddButton(
		ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_X + DETAILINFO_ADDITIONAL_STAT_WIDTH - 24, DETAILINFO_ADDITIONAL_STAT_Y + 2 ) ),
		CsPoint( 20, 20 ),
		CsPoint( 0, 28 ),
		"CommonUI\\CommonUI_close.tga" );
	if( m_pAdditionalStatCloseButton )
	{
		m_pAdditionalStatCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cDetailInfoUI::_OnClickAdditionalStatCloseButton );
		m_vAdditionalStatControls.push_back( m_pAdditionalStatCloseButton );
	}

	cRadioButton* pAttributeTab = m_pAdditionalStatPopup->AddRadioButton(
		ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_X + 115, DETAILINFO_ADDITIONAL_STAT_Y + 38 ) ),
		CsPoint( 70, 23 ),
		CsPoint( 0, 24 ),
		"System\\Btn_Radio1.tga" );
	if( pAttributeTab )
	{
		ti.Init( CFont::FS_10, NiColor::WHITE );
		ti.s_eTextAlign = DT_CENTER;
		ti.SetText( _T("Atributo") );
		pAttributeTab->SetCheckBoxText( &ti );
		pAttributeTab->SetCheckIndex( 0 );
		m_vAdditionalStatControls.push_back( pAttributeTab );
	}

	_AddAdditionalStatSprite(
		CsPoint( DETAILINFO_ADDITIONAL_STAT_X + 7, DETAILINFO_ADDITIONAL_STAT_Y + 67 ),
		CsPoint( 286, 416 ),
		"Mail\\Mail_List_Box.tga" );

	for( int i = 0; i < eAddStatMax; ++i )
	{
		int const nRowY = DETAILINFO_ADDITIONAL_STAT_ROW_Y + ( i * DETAILINFO_ADDITIONAL_STAT_ROW_HEIGHT );
		m_pAdditionalStatRowSprite[i] = _AddAdditionalStatSprite(
			CsPoint( DETAILINFO_ADDITIONAL_STAT_ROW_X, nRowY + 2 ),
			CsPoint( DETAILINFO_ADDITIONAL_STAT_ROW_WIDTH, 28 ),
			"DMG_SKINS\\skin_btn_a.tga" );

		ti.Init( CFont::FS_10, NiColor::WHITE );
		ti.s_eTextAlign = DT_LEFT;
		ti.SetText( ADDITIONAL_STAT_LABELS[i] );
		m_pAdditionalStatLabel[i] = _AddAdditionalStatText( &ti, CsPoint( DETAILINFO_ADDITIONAL_STAT_TEXT_X, nRowY + 9 ) );

		ti.Init( CFont::FS_10, NiColor::WHITE );
		ti.s_eTextAlign = DT_RIGHT;
		ti.SetText( _T("0.00%") );
		m_pAdditionalStatValue[i] = _AddAdditionalStatText( &ti, CsPoint( DETAILINFO_ADDITIONAL_STAT_VALUE_X, nRowY + 9 ) );
	}

	m_pAdditionalStatScrollBar = m_pAdditionalStatPopup->AddScrollBar(
		cScrollBar::TYPE_1,
		ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_X + 276, DETAILINFO_ADDITIONAL_STAT_Y + 73 ) ),
		CsPoint( 16, 407 ),
		cScrollBar::GetDefaultBtnSize(),
		CsRect( ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_X + 7, DETAILINFO_ADDITIONAL_STAT_Y + 67 ) ), CsSIZE( 286, 416 ) ),
		DETAILINFO_ADDITIONAL_STAT_VISIBLE_COUNT );
	if( m_pAdditionalStatScrollBar )
	{
		m_pAdditionalStatScrollBar->SetRange( CsPoint( 0, DETAILINFO_ADDITIONAL_STAT_COUNT ) );
		m_vAdditionalStatControls.push_back( m_pAdditionalStatScrollBar );
	}

	_SetAdditionalStatPopupVisible( false );
}

void cDetailInfoUI::_SetTab(eTabType eTab)
{
	m_eCurrentTab = eTab;
	if( m_pTabButton )
		m_pTabButton->SetCheckIndex( eTab == eTabTamer ? 0 : 1 );

	_SetPageVisible( true, eTab == eTabTamer );
	_SetPageVisible( false, eTab == eTabDigimon );
	if( eTab != eTabDigimon )
		_SetAdditionalStatPopupVisible( false );
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

void cDetailInfoUI::_SetAdditionalStatPopupVisible(bool bVisible)
{
	m_bAdditionalStatVisible = bVisible && eTabDigimon == m_eCurrentTab && m_pAdditionalStatPopup;
	m_bAdditionalStatDragging = false;

	if( m_bAdditionalStatVisible && !m_bAdditionalStatUserMoved )
		_SetAdditionalStatPopupDefaultPosition();

	if( m_pAdditionalStatPopup )
		m_pAdditionalStatPopup->SetVisible( m_bAdditionalStatVisible );

	for( size_t i = 0; i < m_vAdditionalStatControls.size(); ++i )
	{
		if( m_vAdditionalStatControls[i] )
			m_vAdditionalStatControls[i]->SetVisible( m_bAdditionalStatVisible );
	}

	for( size_t i = 0; i < m_vAdditionalStatTexts.size(); ++i )
	{
		if( m_vAdditionalStatTexts[i] )
			m_vAdditionalStatTexts[i]->SetVisible( m_bAdditionalStatVisible );
	}

	if( m_bAdditionalStatVisible )
	{
		_UpdateAdditionalStatPopup();
		_UpdateAdditionalStatRows();
	}
}

void cDetailInfoUI::_SetAdditionalStatPopupDefaultPosition()
{
	if( !m_pAdditionalStatPopup )
		return;

	m_pAdditionalStatPopup->SetWindowPosition(
		ClampAdditionalStatPopupPos( GetTotalPosition() + CsPoint( DETAILINFO_ADDITIONAL_STAT_X, DETAILINFO_ADDITIONAL_STAT_Y ) ) );
}

bool cDetailInfoUI::_UpdateAdditionalStatPopupForMouse()
{
	if( !m_pAdditionalStatPopup )
		return false;

	CsPoint const ptPopup = m_pAdditionalStatPopup->GetTotalPosition();
	CsRect const rcPopup( ptPopup, CsSIZE( DETAILINFO_ADDITIONAL_STAT_WIDTH, DETAILINFO_ADDITIONAL_STAT_HEIGHT ) );
	if( CURSOR_ST.CheckClickBox( rcPopup ) == CURSOR::BUTTON_OUTWINDOW && !m_bAdditionalStatDragging )
		return false;

	if( m_pAdditionalStatCloseButton && m_pAdditionalStatCloseButton->Update_ForMouse() )
		return true;

	if( m_pAdditionalStatScrollBar && m_pAdditionalStatScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
	{
		_UpdateAdditionalStatRows();
		return true;
	}

	CsRect const rcTitle( ptPopup, CsSIZE( DETAILINFO_ADDITIONAL_STAT_WIDTH, DETAILINFO_ADDITIONAL_STAT_TITLE_HEIGHT ) );
	if( CURSOR_ST.CheckClickBox( rcTitle ) != CURSOR::BUTTON_OUTWINDOW &&
		CURSOR_ST.GetButtonState() == CURSOR::LBUTTON_DOWN )
	{
		m_bAdditionalStatDragging = true;
		m_bAdditionalStatUserMoved = true;
		m_ptAdditionalStatDragOffset = CURSOR_ST.GetPos() - ptPopup;
		return true;
	}

	return true;
}

void cDetailInfoUI::_UpdateAdditionalStatPopupDragging()
{
	if( !m_bAdditionalStatDragging || !m_pAdditionalStatPopup )
		return;

	if( CURSOR_ST.GetButtonState() != CURSOR::LBUTTON_PRESS &&
		CURSOR_ST.GetButtonState() != CURSOR::LBUTTON_DOWN )
	{
		m_bAdditionalStatDragging = false;
		return;
	}

	CsPoint ptNewPos = CURSOR_ST.GetPos() - m_ptAdditionalStatDragOffset;
	m_pAdditionalStatPopup->SetWindowPosition( ClampAdditionalStatPopupPos( ptNewPos ) );
}

void cDetailInfoUI::_UpdateAdditionalStatRows()
{
	int nFirstRow = 0;
	if( m_pAdditionalStatScrollBar && m_pAdditionalStatScrollBar->IsEnableScroll() )
		nFirstRow = m_pAdditionalStatScrollBar->GetCurPosIndex();

	for( int i = 0; i < eAddStatMax; ++i )
	{
		bool const bRowVisible =
			m_bAdditionalStatVisible &&
			i >= nFirstRow &&
			i < nFirstRow + DETAILINFO_ADDITIONAL_STAT_VISIBLE_COUNT;

		int const nVisibleIndex = i - nFirstRow;
		int const nRowY = DETAILINFO_ADDITIONAL_STAT_ROW_Y + ( nVisibleIndex * DETAILINFO_ADDITIONAL_STAT_ROW_HEIGHT );

		if( m_pAdditionalStatRowSprite[i] )
		{
			m_pAdditionalStatRowSprite[i]->SetVisible( bRowVisible );
			if( bRowVisible )
				m_pAdditionalStatRowSprite[i]->SetPos( ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_ROW_X, nRowY + 2 ) ) );
		}

		if( m_pAdditionalStatLabel[i] )
		{
			m_pAdditionalStatLabel[i]->SetVisible( bRowVisible );
			if( bRowVisible )
				m_pAdditionalStatLabel[i]->SetPos( ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_TEXT_X, nRowY + 9 ) ) );
		}

		if( m_pAdditionalStatValue[i] )
		{
			m_pAdditionalStatValue[i]->SetVisible( bRowVisible );
			if( bRowVisible )
				m_pAdditionalStatValue[i]->SetPos( ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_VALUE_X, nRowY + 9 ) ) );
		}
	}
}

void cDetailInfoUI::_UpdateAll()
{
	_UpdateTamerPage();
	_UpdateDigimonPage();
	_UpdateAdditionalStatPopup();
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

			_SetText( m_pDigimonInfoText, pFTInfo->s_szForm[0] ? pFTInfo->s_szForm : pFTInfo->s_szName );

			std::wstring wsInfoLevel;
			DmCS::StringFn::Format( wsInfoLevel, L"%d Nivel", pStat->GetLevel() );
			_SetText( m_pDigimonInfoLevelText, wsInfoLevel );

			std::wstring wsInfoSize;
			DmCS::StringFn::Format( wsInfoSize, L"%d cm (%.2f%%)", nTall, fPercent );
			_SetText( m_pDigimonInfoSizeText, wsInfoSize );
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
	{
		for( int i = 0; i < 5; ++i )
			m_nEnchantLevel[i] = 0;
		_SetText( m_pEnchantTotalText, _T("0/75") );
		return;
	}

	int nEnchantTypes[5] = { ET_AT, ET_BL, ET_CR, ET_EV, ET_HP };
	for( int i = 0; i < 5; ++i )
		m_nEnchantLevel[i] = pEnchantStat->GetEnchantLv( nEnchantTypes[i] );

	std::wstring wsEnchantTotal;
	DmCS::StringFn::Format( wsEnchantTotal, L"%d/%d", pEnchantStat->GetEnchantLevel(), nLimit::MaxInchantLevel );
	_SetText( m_pEnchantTotalText, wsEnchantTotal );
}

void cDetailInfoUI::_UpdateAdditionalStatPopup()
{
	for( int i = 0; i < eAddStatMax; ++i )
		_SetText( m_pAdditionalStatValue[i], _T("0.00%") );

	int nAdditionalStatValue[eAddStatMax] = { 0, };
	nAdditionalStatValue[eAddStatIce] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusIce );
	nAdditionalStatValue[eAddStatWater] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusWater );
	nAdditionalStatValue[eAddStatFire] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusFire );
	nAdditionalStatValue[eAddStatEarth] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusEarth );
	nAdditionalStatValue[eAddStatWind] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusWind );
	nAdditionalStatValue[eAddStatWood] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusWood );
	nAdditionalStatValue[eAddStatLight] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusLight );
	nAdditionalStatValue[eAddStatDarkness] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusDarkness );
	nAdditionalStatValue[eAddStatThunder] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusThunder );
	nAdditionalStatValue[eAddStatSteel] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusSteel );
	nAdditionalStatValue[eAddStatBaseDamage] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusBasic );
	nAdditionalStatValue[eAddStatData] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusData );
	nAdditionalStatValue[eAddStatVaccine] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusVaccine );
	nAdditionalStatValue[eAddStatVirus] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusVirus );
	nAdditionalStatValue[eAddStatUnknown] = GetXmlUnionCollectionBonusValue( eXmlUnionBonusUnknown );

	for( int i = 0; i < eAddStatMax; ++i )
	{
		if( nAdditionalStatValue[i] > 0 )
			_SetText( m_pAdditionalStatValue[i], FormatWholePercentValue( nAdditionalStatValue[i] ) );
	}

	_UpdateAdditionalStatRows();
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

void cDetailInfoUI::_RenderDigimonPortrait()
{
	if( !g_pCharMng )
		return;

	CDigimonUser* pDigimon = g_pCharMng->GetDigimonUser( 0 );
	if( !pDigimon || !pDigimon->GetCharImage() )
		return;

	pDigimon->GetCharImage()->Render_L(
		GetTotalPosition() + CsPoint( DETAILINFO_DIGIMON_PORTRAIT_ICON_X, DETAILINFO_DIGIMON_PORTRAIT_ICON_Y ),
		CsPoint( DETAILINFO_DIGIMON_PORTRAIT_ICON_SIZE, DETAILINFO_DIGIMON_PORTRAIT_ICON_SIZE ) );
}

void cDetailInfoUI::_RenderDigimonEnchantIcons()
{
	if( !g_pIconMng )
		return;

	CsPoint ptRoot = GetTotalPosition();
	for( int i = 0; i < 5; ++i )
	{
		g_pIconMng->RenderIcon(
			ICONITEM::Enchant_Lv,
			ptRoot + GetDigimonEnchantLevelIconPos( i ),
			CsPoint( DETAILINFO_DIGIMON_ENCHANT_LEVEL_ICON_WIDTH, DETAILINFO_DIGIMON_ENCHANT_LEVEL_ICON_HEIGHT ),
			ClampEnchantLevelIconIndex( m_nEnchantLevel[i] ) );
	}
}

void cDetailInfoUI::_RenderAdditionalStatIcons()
{
	if( !m_bAdditionalStatVisible || eTabDigimon != m_eCurrentTab || !g_pIconMng || !m_pAdditionalStatPopup )
		return;

	CsPoint ptRoot = m_pAdditionalStatPopup->GetTotalPosition();
	int nFirstRow = 0;
	if( m_pAdditionalStatScrollBar && m_pAdditionalStatScrollBar->IsEnableScroll() )
		nFirstRow = m_pAdditionalStatScrollBar->GetCurPosIndex();

	int const nLastRow = CsMin( eAddStatMax, nFirstRow + DETAILINFO_ADDITIONAL_STAT_VISIBLE_COUNT );
	for( int i = nFirstRow; i < nLastRow; ++i )
	{
		int const nVisibleIndex = i - nFirstRow;
		g_pIconMng->RenderDAttribute(
			ADDITIONAL_STAT_ICON_TYPE[i],
			ADDITIONAL_STAT_ICON_INDEX[i],
			ptRoot + ToAdditionalStatLocal( CsPoint( DETAILINFO_ADDITIONAL_STAT_ICON_X, DETAILINFO_ADDITIONAL_STAT_ROW_Y + ( nVisibleIndex * DETAILINFO_ADDITIONAL_STAT_ROW_HEIGHT ) + 1 ) ),
			CsPoint( DETAILINFO_ADDITIONAL_STAT_ICON_SIZE, DETAILINFO_ADDITIONAL_STAT_ICON_SIZE ) );
	}
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

cSprite* cDetailInfoUI::_AddAdditionalStatSprite(CsPoint pos, CsPoint size, char const* pPath)
{
	if( !m_pAdditionalStatPopup )
		return NULL;

	cSprite* pSprite = m_pAdditionalStatPopup->AddSprite( ToAdditionalStatLocal( pos ), size, pPath );
	if( pSprite )
		m_vAdditionalStatControls.push_back( pSprite );
	return pSprite;
}

cText* cDetailInfoUI::_AddAdditionalStatText(cText::sTEXTINFO* pTextInfo, CsPoint pos)
{
	if( !m_pAdditionalStatPopup )
		return NULL;

	cText* pText = m_pAdditionalStatPopup->AddText( pTextInfo, ToAdditionalStatLocal( pos ) );
	if( pText )
		m_vAdditionalStatTexts.push_back( pText );
	return pText;
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
	ti.Init( CFont::FS_12, NiColor( 76.f/255.f, 242.f/255.f, 1 ) );
	ti.s_eTextAlign = DT_LEFT;
	ti.SetText( pLabel );
	_AddPageText( false, &ti, pos );

	ti.Init( CFont::FS_12, NiColor( 1, 235.f/255.f, 0 ) );
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T("-") );
	m_pDigimonStatValue[eType] = _AddPageText( false, &ti, CsPoint( DETAILINFO_DIGIMON_STAT_X + DETAILINFO_DIGIMON_STAT_TABLE_WIDTH - 4, pos.y ) );
}

void cDetailInfoUI::_AddEnchantRow(int nIndex, char const* pBadgePath, CsPoint pos)
{
	_AddPageSprite( false, pos, CsPoint( DETAILINFO_DIGIMON_ENCHANT_BADGE_WIDTH, DETAILINFO_DIGIMON_ENCHANT_BADGE_HEIGHT ), pBadgePath );
	m_pEnchantValue[nIndex] = NULL;
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

void cDetailInfoUI::_OnClickDigimonZoomButton(void* pSender, void* pData)
{
	_SetAdditionalStatPopupVisible( !m_bAdditionalStatVisible );
}

void cDetailInfoUI::_OnClickAdditionalStatCloseButton(void* pSender, void* pData)
{
	_SetAdditionalStatPopupVisible( false );
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
