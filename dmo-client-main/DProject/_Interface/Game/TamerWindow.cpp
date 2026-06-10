
#include "stdafx.h"
#include "TamerWindow.h"
#include "../Adapt/AdaptPartySystem.h"

cTamerWindow::cTamerWindow()
	: m_pTargetObject(NULL)
	, m_pWindowBG(NULL)
	, m_pTamerLvBG(NULL)
	, m_pHPBarMin(NULL)
	, m_pHPBarMax(NULL)
	, m_pFPBar(NULL)
	, m_pHostSprite(NULL)
	, m_pNameText(NULL)
	, m_pLevelText(NULL)
	, m_pDSBar(NULL)
	, m_pExpBar(NULL)
	, m_pExpPercentText(NULL)
	, m_pHPText(NULL)
	, m_bStatusCacheReady(false)
	, m_nLastLevel(0)
	, m_nLastHP(0)
	, m_nLastMaxHP(0)
	, m_nLastFP(0)
	, m_nLastDS(0)
	, m_nLastMaxDS(0)
	, m_nLastExpBar(0)
	, m_nLastExpBarMax(0)
	, m_pAdaptParty(NULL)
{
	m_szLastExpPercent[0] = 0;

	m_pAdaptParty = new AdaptPartySystem;
	if (m_pAdaptParty)
		m_pAdaptParty->SetContents(CONTENTSSYSTEM_PTR->GetContents(E_CT_PARTYSYSTEM));
}

void cTamerWindow::ResetCachedValues()
{
	m_bStatusCacheReady = false;

	m_nLastLevel = 0;

	m_nLastHP = 0;
	m_nLastMaxHP = 0;
	m_nLastFP = 0;

	m_nLastDS = 0;
	m_nLastMaxDS = 0;

	m_nLastExpBar = 0;
	m_nLastExpBarMax = 0;

	m_szLastExpPercent[0] = 0;
}

void cTamerWindow::Destroy()
{
	m_pAdaptParty = 0;
	cBaseWindow::Delete();
}

void cTamerWindow::DeleteResource()
{
	DeleteScript();

	// Esses são criados por AddSprite/AddText e normalmente são destruídos pelo DeleteScript().
	// Zera para evitar ponteiro pendurado caso algum Update/Render rode depois.
	m_pTamerLvBG = NULL;
	m_pNameText = NULL;
	m_pLevelText = NULL;
	m_pExpPercentText = NULL;

	SAFE_NIDELETE(m_pWindowBG);
	SAFE_NIDELETE(m_pHPBarMin);
	SAFE_NIDELETE(m_pHPBarMax);
	SAFE_NIDELETE(m_pFPBar);
	SAFE_NIDELETE(m_pHostSprite);
	SAFE_NIDELETE(m_pDSBar);
	SAFE_NIDELETE(m_pExpBar);
	SAFE_NIDELETE(m_pHPText);

	ResetCachedValues();
}

void cTamerWindow::Create( int nValue /* = 0  */)
{
	if( cBaseWindow::Init() == false )
		return;

	assert_cs( m_pTargetObject == NULL );

	InitScript( NULL, CsPoint::ZERO, CsPoint( 236, 96 ), false, IFREGION_X::LEFT, IFREGION_Y::TOP );
	// 레벨 배경
	m_pTamerLvBG = AddSprite( CsPoint( 0, 69 ), CsPoint( 35, 35 ), "TargetWindow\\TamerLevel.tga" );

	// 뒷배경
	assert_cs( m_pWindowBG == NULL );
	m_pWindowBG = NiNew cSprite;
	m_pWindowBG->Init( m_pRoot, CsPoint::ZERO, CsPoint( 236, 96 ), "TargetWindow\\TamerTarget.tga", false );

	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontText, CFont::FS_12 );
	ti.s_eTextAlign = DT_CENTER;
	// 이름
	m_pNameText = AddText( &ti, CsPoint( 162, 20 ) );

	// 레벨
	m_pLevelText = AddText( &ti, CsPoint( 18, 78 ) );

	// hp
	m_pHPBarMin = NiNew cProgressBar;
	m_pHPBarMin->Init( m_pRoot, cProgressBar::BAR_3IMAGE, CsPoint( 101, 49 ), CsPoint( 126, 8 ), CsPoint( 2, 2 ), "TargetWindow\\Hp3Bar1.tga", false );

	m_pHPBarMax = NiNew cProgressBar;
	m_pHPBarMax->Init( m_pRoot, cProgressBar::BAR_3IMAGE, CsPoint( 101, 49 ), CsPoint( 126, 8 ), CsPoint( 2, 2 ), "TargetWindow\\HpBar1.tga", false );

	m_pFPBar = NiNew cProgressBar;
	m_pFPBar->Init( m_pRoot, cProgressBar::BAR_3IMAGE, CsPoint( 101, 49 ), CsPoint( 126, 8 ), CsPoint( 2, 2 ), "TargetWindow\\Hp2Bar1.tga", false );

	// hp text
	ti.Init( CFont::FS_9, NiColor::WHITE );
	ti.s_eTextAlign = DT_CENTER | DT_VCENTER;
	m_pHPText = NiNew cText;
	m_pHPText->Init( m_pRoot, CsPoint( 164, 53 ), &ti, false );

	// Ds
	m_pDSBar = NiNew cProgressBar;
	m_pDSBar->Init( m_pRoot, cProgressBar::BAR_3IMAGE, CsPoint( 101, 64 ), CsPoint( 126, 8 ), CsPoint( 2, 2 ), "TargetWindow\\DsBar1.tga", false );
	m_pDSBar->SetRenderPosText( cProgressBar::TEXT_VALUE );

	// Exp
	m_pExpBar = NiNew cProgressBar;
	m_pExpBar->Init( m_pRoot, cProgressBar::BAR_1IMAGE, CsPoint( 87, 79 ), CsPoint( 146, 5 ), CsPoint( 0, 0 ), "TargetWindow\\Expbar.tga", false );

	m_pHostSprite = NiNew cSprite;
	m_pHostSprite->Init( m_pRoot, CsPoint( 4, 5 ), CsPoint( 22, 25 ), "Party\\Host.tga", false );
	m_pHostSprite->SetVisible(false);

	ti.s_eFontSize = CFont::FS_9;
	ti.s_eTextAlign = DT_RIGHT;
	ti.SetText( _T( "" ) );
	m_pExpPercentText = AddText( &ti, CsPoint( 235, 85 ) );

	// 경험치 툴팁 영역
	m_rcExpRegion.SetRect( CsPoint( 87, 75 ), CsSIZE( 146, 10 ) );	
}

void cTamerWindow::Update(float const& fDeltaTime)
{
	SAFE_POINTER_RET(m_pTargetObject);

	CsC_AvObject::sBASE_STAT* pBaseStat = m_pTargetObject->GetBaseStat();
	SAFE_POINTER_RET(pBaseStat);

	const bool bFullRefresh = !m_bStatusCacheReady;

	// Level
	const int nLevel = pBaseStat->GetLevel();

	if (m_pLevelText)
	{
		if (bFullRefresh || m_nLastLevel != nLevel)
		{
			if (nLevel != CsC_AvObject::INVALIDE_STAT)
				m_pLevelText->SetText(nLevel);
			else
				m_pLevelText->SetText(_T("?"));
		}
	}

	// HP / FP
	int nMaxHP = pBaseStat->GetMaxHP();
	int nHP = pBaseStat->GetHP();
	int nFP = pBaseStat->GetFP();

	if (nMaxHP < 1)
		nMaxHP = 1;

	if (nHP < 0)
		nHP = 0;
	if (nHP > nMaxHP)
		nHP = nMaxHP;

	if (nFP < 0)
		nFP = 0;
	if (nFP > nMaxHP)
		nFP = nMaxHP;

	const bool bHPRangeChanged = (bFullRefresh || m_nLastMaxHP != nMaxHP);
	const bool bHPChanged = (bFullRefresh || m_nLastHP != nHP);
	const bool bFPChanged = (bFullRefresh || m_nLastFP != nFP);

	if (m_pHPBarMin)
	{
		if (bHPRangeChanged)
			m_pHPBarMin->SetRange(CsPoint(0, nMaxHP));

		if (bHPChanged)
			m_pHPBarMin->SetBarPos(nHP);
	}

	if (m_pHPBarMax)
	{
		if (bHPRangeChanged)
			m_pHPBarMax->SetRange(CsPoint(0, nMaxHP));

		if (bHPChanged)
			m_pHPBarMax->SetBarPos(nHP);
	}

	if (m_pFPBar)
	{
		if (bHPRangeChanged)
			m_pFPBar->SetRange(CsPoint(0, nMaxHP));

		if (bFPChanged)
			m_pFPBar->SetBarPos(nFP);
	}

	if (m_pHPText)
	{
		if (bFullRefresh || m_nLastHP != nHP || m_nLastMaxHP != nMaxHP)
		{
			std::wstring wsText;
			DmCS::StringFn::Format(wsText, L"%d / %d", nHP, nMaxHP);
			m_pHPText->SetText(wsText.c_str());
		}
	}

	// DS
	int nMaxDS = pBaseStat->GetMaxDS();
	int nDS = pBaseStat->GetDS();

	if (nMaxDS < 1)
		nMaxDS = 1;

	if (nDS < 0)
		nDS = 0;
	if (nDS > nMaxDS)
		nDS = nMaxDS;

	const bool bDSRangeChanged = (bFullRefresh || m_nLastMaxDS != nMaxDS);
	const bool bDSChanged = (bFullRefresh || m_nLastDS != nDS);

	if (m_pDSBar)
	{
		if (bDSRangeChanged)
			m_pDSBar->SetRange(CsPoint(0, nMaxDS));

		if (bDSChanged)
			m_pDSBar->SetBarPos(nDS);
	}

	// EXP
	// Evita overflow em level alto. Em vez de usar EXP bruto como range do progress,
	// usamos escala fixa 0..10000.
	const int EXP_BAR_MAX = 10000;

	double dMaxEXP = 0.0;
	double dCurEXP = 0.0;

	if (nLevel != CsC_AvObject::INVALIDE_STAT)
		dMaxEXP = static_cast<double>(FMTamer::GetMaxExp(nLevel));

	dCurEXP = static_cast<double>(pBaseStat->GetExp());

	if (dMaxEXP < 1.0)
		dMaxEXP = 1.0;

	if (dCurEXP < 0.0)
		dCurEXP = 0.0;

	double dExpRate = dCurEXP / dMaxEXP;

	if (dExpRate < 0.0)
		dExpRate = 0.0;
	if (dExpRate > 1.0)
		dExpRate = 1.0;

	const int nExpBar = static_cast<int>(dExpRate * EXP_BAR_MAX);

	if (m_pExpBar)
	{
		if (bFullRefresh || m_nLastExpBarMax != EXP_BAR_MAX)
			m_pExpBar->SetRange(CsPoint(0, EXP_BAR_MAX));

		if (bFullRefresh || m_nLastExpBar != nExpBar)
			m_pExpBar->SetBarPos(nExpBar);
	}

	if (m_pExpPercentText)
	{
		TCHAR sz[32] = { 0 };
		_stprintf_s(sz, 32, _T("%.3f%%"), dExpRate * 100.0);

		if (bFullRefresh || _tcscmp(m_szLastExpPercent, sz) != 0)
		{
			m_pExpPercentText->SetText(sz);
			_tcscpy_s(m_szLastExpPercent, 32, sz);
		}
	}

	m_nLastLevel = nLevel;

	m_nLastHP = nHP;
	m_nLastMaxHP = nMaxHP;
	m_nLastFP = nFP;

	m_nLastDS = nDS;
	m_nLastMaxDS = nMaxDS;

	m_nLastExpBar = nExpBar;
	m_nLastExpBarMax = EXP_BAR_MAX;

	m_bStatusCacheReady = true;
}

cBaseWindow::eMU_TYPE
cTamerWindow::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if (muReturn == MUT_OUT_WINDOW)
		return muReturn;

	if (m_pTargetObject == NULL)
		return muReturn;

	CsC_AvObject::sBASE_STAT* pBaseStat = m_pTargetObject->GetBaseStat();
	if (pBaseStat == NULL)
		return muReturn;

	CsPoint ptMouseLocalPos = MousePosToWindowPos(CURSOR_ST.GetPos());
	if (m_rcExpRegion.PtInRect(ptMouseLocalPos) == TRUE)
	{
		TOOLTIPMNG_STPTR->GetTooltip()->SetTooltip(
			m_rcExpRegion.GetPos(),
			m_rcExpRegion.GetSize(),
			TOOLTIP_MAX_SIZE,
			cTooltip::EXP,
			pBaseStat->GetExp(),
			FMTamer::GetMaxExp(pBaseStat->GetLevel())
		);
	}

	return muReturn;
}

void cTamerWindow::Render()
{
	if (m_pWindowBG == NULL)
		return;

	// 뒷 배경
	m_pWindowBG->Render();

	if (m_pTargetObject == NULL)
		return;

	// 얼굴 이미지
	sCHAR_IMAGE* pCharImage = ((CTamerUser*)m_pTargetObject)->GetCharImage();
	if (pCharImage != NULL)
		pCharImage->Render_L(CsPoint(m_pRoot->GetClient().x + 6, m_pRoot->GetClient().y + 6), CsPoint(84, 84));

	RenderScript();

	CsC_AvObject::sBASE_STAT* pBaseStat = m_pTargetObject->GetBaseStat();
	if (pBaseStat == NULL)
		return;

	if (m_pHPBarMax && m_pFPBar && m_pHPBarMin)
	{
		if (pBaseStat->GetFP() < pBaseStat->GetHP())
		{
			m_pHPBarMax->Render();
			m_pFPBar->Render();
		}
		else
		{
			m_pFPBar->Render();
			m_pHPBarMin->Render();
		}
	}

	if (m_pHPText)
		m_pHPText->Render();

	if (m_pDSBar)
		m_pDSBar->Render();

	if (m_pExpBar)
		m_pExpBar->Render();

	if (m_pHostSprite)
		m_pHostSprite->Render();
}

void cTamerWindow::OnRButtonUp( CsPoint pos )
{
	if( IsInWindow( pos ) == false )
		return;	

	if( nsCsGBTerrain::g_nSvrLibType == nLIB::SVR_BATTLE )
		return;

	if( m_pAdaptParty && !m_pAdaptParty->IsJoinParty() )
 		return;

  	cPopUpWindow::ePARTY_TYPE pt = m_pAdaptParty->IsMePartyMaster() ? cPopUpWindow::PARTY_HS_MY : cPopUpWindow::PARTY_NR_MY;
 	g_pGameIF->GetPopup()->SetPopup( CURSOR_ST.GetPos() + CsPoint( 15, 0 ), CsPoint( 130, 0 ), cPopUpWindow::PARTY_MENU, pt );
}

void cTamerWindow::OnLButtonUp( CsPoint pos )
{
	if( IsInWindow( pos ) == false )
		return;

	if( nsCsGBTerrain::g_nSvrLibType != nLIB::SVR_BATTLE )
		g_pCharResMng->SetTargetMark( m_pTargetObject );	
}

void cTamerWindow::SetTargetObject(CsC_AvObject* pTarget)
{
	m_pTargetObject = pTarget;
	ResetCachedValues();

	if (m_pNameText == NULL)
		return;

	if (m_pTargetObject == NULL)
	{
		m_pNameText->SetText(_T(""));
		return;
	}

	CTamerUser* pTamerUser = (CTamerUser*)m_pTargetObject;
	if (pTamerUser == NULL || pTamerUser->GetCharOption() == NULL)
	{
		m_pNameText->SetText(_T(""));
		return;
	}

	m_pNameText->SetText(pTamerUser->GetCharOption()->GetName());
}

void cTamerWindow::ResetDevice()
{
	ResetDeviceScript();

	if (m_pWindowBG)
		m_pWindowBG->ResetDevice();

	if (m_pHPBarMin)
		m_pHPBarMin->ResetDevice();

	if (m_pHPBarMax)
		m_pHPBarMax->ResetDevice();

	if (m_pFPBar)
		m_pFPBar->ResetDevice();

	if (m_pDSBar)
		m_pDSBar->ResetDevice();

	if (m_pExpBar)
		m_pExpBar->ResetDevice();

	if (m_pHostSprite)
		m_pHostSprite->ResetDevice();

	if (m_pHPText)
		m_pHPText->ResetDevice();

	ResetCachedValues();
}

void cTamerWindow::SetPartyMaster(bool bValue)
{
	if( m_pHostSprite )
		m_pHostSprite->SetVisible(bValue);
}

// Vip Mebmer Ship 일때 테이머 배경 이미지 변경
void cTamerWindow::ChangeVipMemberShip( bool bValue )
{
	if( bValue )
	{
		if( m_pWindowBG )
			m_pWindowBG->ChangeTexture( "TargetWindow\\TamerTarget_MM.tga" );
		if( m_pTamerLvBG )
			m_pTamerLvBG->ChangeTexture( "TargetWindow\\TamerLevel_MM.tga" );
	}
	else
	{
		if( m_pWindowBG )
			m_pWindowBG->ChangeTexture( "TargetWindow\\TamerTarget.tga" );
		if( m_pTamerLvBG )
			m_pTamerLvBG->ChangeTexture( "TargetWindow\\TamerLevel.tga" );
	}
}