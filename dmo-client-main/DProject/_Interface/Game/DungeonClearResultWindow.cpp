#include "stdafx.h"
#include "DungeonClearResultWindow.h"
#include "../Base/Image.h"
#include "../../CharResMng.h"
#include "../../DataMng.h"
#include "../../network/cCliGame.h"

namespace
{
	static NiColor const DUNGEON_TITLE_COLOR( 0.54f, 0.93f, 1.0f );
	static NiColor const DUNGEON_VALUE_COLOR( 1.0f, 1.0f, 1.0f );
	static NiColor const DUNGEON_DETAIL_TITLE_COLOR( 0.86f, 0.48f, 1.0f );

	static cText* AddDungeonText( cDungeonClearResultWindow* pWindow, TCHAR const* szText, CsPoint const& ptPos, CFont::eFACE_SIZE eSize, NiColor const& color, bool bOutline = true )
	{
		cText::sTEXTINFO textInfo;
		textInfo.Init( &g_pEngine->m_FontSystem, eSize, color );
		textInfo.s_bOutLine = bOutline;
		textInfo.SetText( szText );
		return pWindow->AddText( &textInfo, ptPos );
	}

	static cImage* AddDungeonImage( cDungeonClearResultWindow* pWindow, CsPoint const& ptPos, CsPoint const& ptSize, char const* szPath, CsPoint const& ptToken = CsPoint::ZERO, int nState = 0 )
	{
		cImage* pImage = NiNew cImage;
		if( pImage == NULL )
			return NULL;

		pImage->Init( pWindow->GetRoot(), ptPos, ptSize, szPath, false );
		if( ptToken.x != 0 || ptToken.y != 0 )
		{
			pImage->SetTexToken( ptToken );
			pImage->SetState( nState );
		}
		pWindow->AddChildControl( pImage );
		return pImage;
	}

	static CsPoint Pt( CsPoint const& ptBase, int nX, int nY )
	{
		return CsPoint( ptBase.x + nX, ptBase.y + nY );
	}

	static void SetTextVisible( cText* pText, bool bVisible )
	{
		if( pText )
			pText->SetVisible( bVisible );
	}

	static void SetSpriteVisible( cSprite* pSprite, bool bVisible )
	{
		if( pSprite )
			pSprite->SetVisible( bVisible );
	}
}

cDungeonClearResultWindow::cDungeonClearResultWindow()
: m_pMoveButton( NULL )
, m_pExitButton( NULL )
, m_pReEnterButton( NULL )
, m_pTitleText( NULL )
, m_pRankImage( NULL )
, m_pDungeonNameValue( NULL )
, m_pDifficultyValue( NULL )
, m_pPartyValue( NULL )
, m_pTimeValue( NULL )
, m_pFailValue( NULL )
, m_pExpValue( NULL )
, m_pBitValue( NULL )
, m_pDefaultRewardEmptyText( NULL )
, m_pExtraRewardEmptyText( NULL )
{
	memset( m_pDefaultRewardSlot, 0, sizeof( m_pDefaultRewardSlot ) );
	memset( m_pExtraRewardSlot, 0, sizeof( m_pExtraRewardSlot ) );
	memset( m_pDetailTitle, 0, sizeof( m_pDetailTitle ) );
	memset( m_pDetailTamerName, 0, sizeof( m_pDetailTamerName ) );
	memset( m_pDetailDigimonName, 0, sizeof( m_pDetailDigimonName ) );
	memset( m_pDetailTamerLevel, 0, sizeof( m_pDetailTamerLevel ) );
	memset( m_pDetailDigimonLevel, 0, sizeof( m_pDetailDigimonLevel ) );
	memset( m_pDetailValue, 0, sizeof( m_pDetailValue ) );
	memset( m_pDetailNotApplicable, 0, sizeof( m_pDetailNotApplicable ) );
	memset( m_pDetailDeco, 0, sizeof( m_pDetailDeco ) );
	memset( m_pDetailTamerBg, 0, sizeof( m_pDetailTamerBg ) );
	memset( m_pDetailDigimonBg, 0, sizeof( m_pDetailDigimonBg ) );
	memset( m_pDetailTamerLevelBg, 0, sizeof( m_pDetailTamerLevelBg ) );
	memset( m_pDetailDigimonLevelBg, 0, sizeof( m_pDetailDigimonLevelBg ) );
	memset( m_pDetailStatsIcon, 0, sizeof( m_pDetailStatsIcon ) );
	memset( m_pDetailTamerImage, 0, sizeof( m_pDetailTamerImage ) );
	memset( m_pDetailDigimonImage, 0, sizeof( m_pDetailDigimonImage ) );
}

void cDungeonClearResultWindow::Destroy()
{
	cBaseWindow::Delete();
}

void cDungeonClearResultWindow::DeleteResource()
{
	_ReleaseDetailImages();
	DeleteScript();

	m_pMoveButton = NULL;
	m_pExitButton = NULL;
	m_pReEnterButton = NULL;
	m_pTitleText = NULL;
	m_pRankImage = NULL;
	m_pDungeonNameValue = NULL;
	m_pDifficultyValue = NULL;
	m_pPartyValue = NULL;
	m_pTimeValue = NULL;
	m_pFailValue = NULL;
	m_pExpValue = NULL;
	m_pBitValue = NULL;
	m_pDefaultRewardEmptyText = NULL;
	m_pExtraRewardEmptyText = NULL;
	memset( m_pDefaultRewardSlot, 0, sizeof( m_pDefaultRewardSlot ) );
	memset( m_pExtraRewardSlot, 0, sizeof( m_pExtraRewardSlot ) );
	memset( m_pDetailTitle, 0, sizeof( m_pDetailTitle ) );
	memset( m_pDetailTamerName, 0, sizeof( m_pDetailTamerName ) );
	memset( m_pDetailDigimonName, 0, sizeof( m_pDetailDigimonName ) );
	memset( m_pDetailTamerLevel, 0, sizeof( m_pDetailTamerLevel ) );
	memset( m_pDetailDigimonLevel, 0, sizeof( m_pDetailDigimonLevel ) );
	memset( m_pDetailValue, 0, sizeof( m_pDetailValue ) );
	memset( m_pDetailNotApplicable, 0, sizeof( m_pDetailNotApplicable ) );
	memset( m_pDetailDeco, 0, sizeof( m_pDetailDeco ) );
	memset( m_pDetailTamerBg, 0, sizeof( m_pDetailTamerBg ) );
	memset( m_pDetailDigimonBg, 0, sizeof( m_pDetailDigimonBg ) );
	memset( m_pDetailTamerLevelBg, 0, sizeof( m_pDetailTamerLevelBg ) );
	memset( m_pDetailDigimonLevelBg, 0, sizeof( m_pDetailDigimonLevelBg ) );
	memset( m_pDetailStatsIcon, 0, sizeof( m_pDetailStatsIcon ) );
}

void cDungeonClearResultWindow::Create( int nValue /* = 0 */ )
{
	if( cBaseWindow::Init() == false )
		return;

	int const nWidth = 968;
	int const nHeight = 634;
	int const nPosX = ( g_nScreenWidth / 2 ) - ( nWidth / 2 );
	int const nPosY = ( g_nScreenHeight / 2 ) - ( nHeight / 2 );

	InitScript( "DungeonUI\\Dungeon_window.png", CsPoint( nPosX, nPosY ), CsPoint( nWidth, nHeight ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	m_pMoveButton = AddButton( CsPoint( 0, 0 ), CsPoint( nWidth, 34 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
	if( m_pMoveButton )
		m_pMoveButton->SetAlpha( 0.0f );

	m_pTitleText = AddDungeonText( this, _T( "Resultado do Calabou\x00E7" "o" ), CsPoint( 370, 20 ), CFont::FS_16, DUNGEON_VALUE_COLOR );
	m_pRankImage = AddDungeonImage( this, CsPoint( 70, 70 ), CsPoint( 170, 142 ), "DungeonUI\\Dungeon_Result_Rank.png", CsPoint( 170, 142 ), 0 );

	AddSprite( CsPoint( 325, 68 ), CsPoint( 220, 30 ), "DungeonUI\\Dungeon_stats_bg.png" );
	AddSprite( CsPoint( 325, 132 ), CsPoint( 220, 30 ), "DungeonUI\\Dungeon_stats_bg.png" );
	AddSprite( CsPoint( 325, 196 ), CsPoint( 220, 30 ), "DungeonUI\\Dungeon_stats_bg.png" );
	AddSprite( CsPoint( 625, 68 ), CsPoint( 220, 30 ), "DungeonUI\\Dungeon_stats_bg.png" );
	AddSprite( CsPoint( 625, 132 ), CsPoint( 270, 30 ), "DungeonUI\\Dungeon_stats_bg.png" );

	AddDungeonText( this, _T( "Nome do Calabou\x00E7" "o" ), CsPoint( 356, 77 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddDungeonText( this, _T( "Dificuldade" ), CsPoint( 356, 141 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddDungeonText( this, _T( "Membro do Party" ), CsPoint( 356, 205 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddDungeonText( this, _T( "Tempo Livre" ), CsPoint( 656, 77 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddDungeonText( this, _T( "N\x00FA" "mero de Falha de Membro do Party" ), CsPoint( 656, 141 ), CFont::FS_11, DUNGEON_TITLE_COLOR );

	m_pDungeonNameValue = AddDungeonText( this, _T( "" ), CsPoint( 356, 108 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
	m_pDifficultyValue = AddDungeonText( this, _T( "" ), CsPoint( 356, 172 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
	m_pPartyValue = AddDungeonText( this, _T( "" ), CsPoint( 356, 236 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
	m_pTimeValue = AddDungeonText( this, _T( "" ), CsPoint( 656, 108 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
	m_pFailValue = AddDungeonText( this, _T( "" ), CsPoint( 656, 172 ), CFont::FS_12, DUNGEON_VALUE_COLOR );

	AddSprite( CsPoint( 44, 263 ), CsPoint( 202, 28 ), "DungeonUI\\Dungeon_result_bg.png" );
	AddDungeonText( this, _T( "Resultado Detalhado" ), CsPoint( 58, 272 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	_CreateDetailCards();

	AddSprite( CsPoint( 724, 297 ), CsPoint( 198, 28 ), "DungeonUI\\Dungeon_result_bg.png" );
	AddDungeonText( this, _T( "Ganhou EXP, Bit" ), CsPoint( 738, 306 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddSprite( CsPoint( 733, 340 ), CsPoint( 32, 32 ), "DungeonUI\\Dungeon_icon_XP.png" );
	AddSprite( CsPoint( 738, 382 ), CsPoint( 32, 32 ), "DungeonUI\\Dungeon_icon_bit.png" );
	m_pExpValue = AddDungeonText( this, _T( "0" ), CsPoint( 918, 346 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
	m_pBitValue = AddDungeonText( this, _T( "0" ), CsPoint( 918, 388 ), CFont::FS_12, DUNGEON_VALUE_COLOR );

	AddSprite( CsPoint( 724, 428 ), CsPoint( 198, 28 ), "DungeonUI\\Dungeon_result_bg.png" );
	AddDungeonText( this, _T( "Item obtido" ), CsPoint( 738, 437 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	AddSprite( CsPoint( 724, 535 ), CsPoint( 198, 28 ), "DungeonUI\\Dungeon_result_bg.png" );
	AddDungeonText( this, _T( "Item Adicional Obtido" ), CsPoint( 738, 544 ), CFont::FS_11, DUNGEON_TITLE_COLOR );

	for( int i = 0; i < MAX_DEFAULT_REWARD_VISIBLE; ++i )
		m_pDefaultRewardSlot[i] = AddSprite( _DefaultRewardPos( i ), CsPoint( 44, 44 ), "CommonUI\\ItemSlot.tga" );
	for( int i = 0; i < MAX_EXTRA_REWARD_VISIBLE; ++i )
		m_pExtraRewardSlot[i] = AddSprite( _ExtraRewardPos( i ), CsPoint( 44, 44 ), "CommonUI\\ItemSlot.tga" );

	m_pDefaultRewardEmptyText = AddDungeonText( this, _T( "" ), CsPoint( 738, 475 ), CFont::FS_11, DUNGEON_TITLE_COLOR );
	m_pExtraRewardEmptyText = AddDungeonText( this, _T( "" ), CsPoint( 738, 582 ), CFont::FS_11, DUNGEON_TITLE_COLOR );

	m_pReEnterButton = AddButton( CsPoint( 690, 220 ), CsPoint( 109, 37 ), CsPoint( 0, 37 ), "DungeonUI\\dungeon_result_ui_Btn_restart.png" );
	m_pExitButton = AddButton( CsPoint( 810, 220 ), CsPoint( 109, 37 ), CsPoint( 0, 37 ), "DungeonUI\\dungeon_result_ui_Btn_exit.png" );
	AddDungeonText( this, _T( "Re Enter" ), CsPoint( 732, 231 ), CFont::FS_11, DUNGEON_VALUE_COLOR );
	AddDungeonText( this, _T( "Exit" ), CsPoint( 856, 231 ), CFont::FS_11, DUNGEON_VALUE_COLOR );

	_RefreshTexts();
}

void cDungeonClearResultWindow::Update(float const& fDeltaTime)
{
	UpdateScript(fDeltaTime);
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cDungeonClearResultWindow::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
	{
		if( m_pMoveButton ) m_pMoveButton->Update_ForMouse();
		if( m_pReEnterButton ) m_pReEnterButton->Update_ForMouse();
		if( m_pExitButton ) m_pExitButton->Update_ForMouse();
		return muReturn;
	}

	if( m_pReEnterButton )
	{
		switch( m_pReEnterButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			if( net::game )
				net::game->SendDungeonReEnter();
			Close();
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( m_pExitButton )
	{
		switch( m_pExitButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			if( net::game )
				net::game->SendDungeonClearExit();
			Close();
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( _SetRewardToolTip() )
		return MUT_NONE;

	_UpdateMoveWindow_ForMouse();
	return muReturn;
}

void cDungeonClearResultWindow::Render()
{
	RenderScript();
	_RenderDetailCards();

	for( int i = 0; i < MAX_DEFAULT_REWARD_VISIBLE && i < (int)m_Data.s_vDefaultRewards.size(); ++i )
	{
		sReward const& reward = m_Data.s_vDefaultRewards[i];
		g_pIconMng->RenderItem( reward.s_dwItemId, GetRootClient() + _DefaultRewardPos( i ) + CsPoint( 6, 6 ), reward.s_dwAmount == 0 ? 1 : (int)reward.s_dwAmount );
	}

	for( int i = 0; i < MAX_EXTRA_REWARD_VISIBLE && i < (int)m_Data.s_vExtraRewards.size(); ++i )
	{
		sReward const& reward = m_Data.s_vExtraRewards[i];
		g_pIconMng->RenderItem( reward.s_dwItemId, GetRootClient() + _ExtraRewardPos( i ) + CsPoint( 6, 6 ), reward.s_dwAmount == 0 ? 1 : (int)reward.s_dwAmount );
	}
}

void cDungeonClearResultWindow::ResetDevice()
{
	ResetDeviceScript();

	for( int i = 0; i < MAX_DETAIL_CARD_COUNT; ++i )
	{
		if( m_pDetailTamerImage[i] )
			m_pDetailTamerImage[i]->ResetDevice();
		if( m_pDetailDigimonImage[i] )
			m_pDetailDigimonImage[i]->ResetDevice();
	}
}

void cDungeonClearResultWindow::SetResultData( sResultData const& data )
{
	m_Data = data;
	_LoadDetailImages();
	_RefreshTexts();
}

void cDungeonClearResultWindow::_RefreshTexts()
{
	if( m_pRankImage )
		m_pRankImage->SetState( _RankImageState( m_Data.s_byRank ) );

	if( m_pDungeonNameValue )
		m_pDungeonNameValue->SetText( m_Data.s_wsDungeonName.empty() ? _T( "-" ) : m_Data.s_wsDungeonName.c_str(), 220 );

	if( m_pDifficultyValue )
		m_pDifficultyValue->SetText( _DifficultyText( m_Data.s_wDifficulty ) );

	TCHAR szBuffer[128] = { 0, };
	if( m_pPartyValue )
	{
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%u / %u" ), m_Data.s_wPartyCount, m_Data.s_wPartyMax );
		m_pPartyValue->SetText( szBuffer );
	}

	if( m_pTimeValue )
	{
		_FormatTime( m_Data.s_dwElapsedSeconds, szBuffer, _countof( szBuffer ) );
		m_pTimeValue->SetText( szBuffer );
	}

	if( m_pFailValue )
	{
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%u / %u" ), m_Data.s_wFailCount, m_Data.s_wFailMax );
		m_pFailValue->SetText( szBuffer );
	}

	if( m_pExpValue )
	{
		_FormatNumber( m_Data.s_dwExp, szBuffer, _countof( szBuffer ) );
		m_pExpValue->SetText( szBuffer );
	}

	if( m_pBitValue )
	{
		_FormatNumber( m_Data.s_dwBits, szBuffer, _countof( szBuffer ) );
		m_pBitValue->SetText( szBuffer );
	}

	if( m_pDefaultRewardEmptyText )
		m_pDefaultRewardEmptyText->SetText( m_Data.s_vDefaultRewards.empty() ? _T( "Sem itens." ) : _T( "" ) );
	if( m_pExtraRewardEmptyText )
		m_pExtraRewardEmptyText->SetText( m_Data.s_vExtraRewards.empty() ? _T( "Sem itens." ) : _T( "" ) );

	for( int i = 0; i < MAX_DEFAULT_REWARD_VISIBLE; ++i )
		if( m_pDefaultRewardSlot[i] )
			m_pDefaultRewardSlot[i]->SetVisible( i < (int)m_Data.s_vDefaultRewards.size() );
	for( int i = 0; i < MAX_EXTRA_REWARD_VISIBLE; ++i )
		if( m_pExtraRewardSlot[i] )
			m_pExtraRewardSlot[i]->SetVisible( i < (int)m_Data.s_vExtraRewards.size() );

	_RefreshDetailCards();
}

void cDungeonClearResultWindow::_CreateDetailCards()
{
	for( int i = 0; i < MAX_DETAIL_CARD_COUNT; ++i )
	{
		CsPoint const ptCard = _DetailCardPos( i );
		AddSprite( ptCard, CsPoint( 220, 344 ), "DungeonUI\\Dungeon_stats_box_01.png" );
		m_pDetailDeco[i] = AddSprite( Pt( ptCard, 9, 21 ), CsPoint( 202, 220 ), "DungeonUI\\Dungeon_stats_box_deco.png" );

		m_pDetailTitle[i] = AddDungeonText( this, _DetailTitle( (BYTE)i ), Pt( ptCard, 68, 24 ), CFont::FS_11, DUNGEON_DETAIL_TITLE_COLOR );
		m_pDetailTamerName[i] = AddDungeonText( this, _T( "" ), Pt( ptCard, 88, 62 ), CFont::FS_11, DUNGEON_VALUE_COLOR );
		m_pDetailDigimonName[i] = AddDungeonText( this, _T( "" ), Pt( ptCard, 88, 198 ), CFont::FS_11, DUNGEON_VALUE_COLOR );
		m_pDetailTamerLevel[i] = AddDungeonText( this, _T( "" ), Pt( ptCard, 77, 160 ), CFont::FS_11, DUNGEON_VALUE_COLOR );
		m_pDetailDigimonLevel[i] = AddDungeonText( this, _T( "" ), Pt( ptCard, 83, 270 ), CFont::FS_11, DUNGEON_VALUE_COLOR );
		m_pDetailValue[i] = AddDungeonText( this, _T( "" ), Pt( ptCard, 124, 312 ), CFont::FS_12, DUNGEON_VALUE_COLOR );
		m_pDetailNotApplicable[i] = AddDungeonText( this, _T( "N\x00E3" "o aplic\x00E1" "vel" ), Pt( ptCard, 82, 170 ), CFont::FS_11, DUNGEON_TITLE_COLOR );

		m_pDetailTamerBg[i] = AddSprite( Pt( ptCard, 62, 77 ), CsPoint( 96, 96 ), "DungeonUI\\Dungeon_tamer_bg.png" );
		m_pDetailDigimonBg[i] = AddSprite( Pt( ptCard, 78, 211 ), CsPoint( 65, 65 ), "DungeonUI\\Dungeon_digimon_bg.png" );
		m_pDetailTamerLevelBg[i] = AddSprite( Pt( ptCard, 60, 145 ), CsPoint( 35, 35 ), "TargetWindow\\TamerLevel.tga" );
		m_pDetailDigimonLevelBg[i] = AddSprite( Pt( ptCard, 66, 255 ), CsPoint( 35, 35 ), "TargetWindow\\TamerLevel.tga" );
		m_pDetailStatsIcon[i] = AddDungeonImage( this, Pt( ptCard, 20, 300 ), CsPoint( 26, 23 ), "DungeonUI\\Dungeon_stats_icon.png", CsPoint( 26, 23 ), i );
	}
}

void cDungeonClearResultWindow::_RefreshDetailCards()
{
	TCHAR szBuffer[128] = { 0, };

	for( int i = 0; i < MAX_DETAIL_CARD_COUNT; ++i )
	{
		bool const bExists = i < (int)m_Data.s_vDetailResults.size();
		sDetailResult const* pDetail = bExists ? &m_Data.s_vDetailResults[i] : NULL;
		BYTE const byCategory = pDetail ? pDetail->s_byCategory : (BYTE)i;

		if( m_pDetailTitle[i] )
			m_pDetailTitle[i]->SetText( _DetailTitle( byCategory ), 150 );

		SetTextVisible( m_pDetailNotApplicable[i], !bExists );
		SetTextVisible( m_pDetailTamerName[i], bExists );
		SetTextVisible( m_pDetailDigimonName[i], bExists );
		SetTextVisible( m_pDetailTamerLevel[i], bExists );
		SetTextVisible( m_pDetailDigimonLevel[i], bExists );
		SetTextVisible( m_pDetailValue[i], bExists );
		SetSpriteVisible( m_pDetailDeco[i], bExists );
		SetSpriteVisible( m_pDetailTamerBg[i], bExists );
		SetSpriteVisible( m_pDetailDigimonBg[i], bExists );
		SetSpriteVisible( m_pDetailTamerLevelBg[i], bExists );
		SetSpriteVisible( m_pDetailDigimonLevelBg[i], bExists );
		SetSpriteVisible( m_pDetailStatsIcon[i], bExists );

		if( pDetail == NULL )
			continue;

		if( m_pDetailTamerName[i] )
			m_pDetailTamerName[i]->SetText( pDetail->s_wsTamerName.empty() ? _T( "-" ) : pDetail->s_wsTamerName.c_str(), 120 );
		if( m_pDetailDigimonName[i] )
			m_pDetailDigimonName[i]->SetText( pDetail->s_wsDigimonName.empty() ? _T( "-" ) : pDetail->s_wsDigimonName.c_str(), 120 );
		if( m_pDetailTamerLevel[i] )
		{
			_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%u" ), pDetail->s_wTamerLevel );
			m_pDetailTamerLevel[i]->SetText( szBuffer );
		}
		if( m_pDetailDigimonLevel[i] )
		{
			_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%u" ), pDetail->s_wDigimonLevel );
			m_pDetailDigimonLevel[i]->SetText( szBuffer );
		}
		if( m_pDetailValue[i] )
		{
			_FormatNumber( pDetail->s_dwValue, szBuffer, _countof( szBuffer ) );
			m_pDetailValue[i]->SetText( szBuffer );
		}
		if( m_pDetailStatsIcon[i] )
			m_pDetailStatsIcon[i]->SetState( byCategory % 3 );
	}
}

void cDungeonClearResultWindow::_LoadDetailImages()
{
	_ReleaseDetailImages();

	int const nCount = (int)m_Data.s_vDetailResults.size() < MAX_DETAIL_CARD_COUNT ? (int)m_Data.s_vDetailResults.size() : MAX_DETAIL_CARD_COUNT;
	for( int i = 0; i < nCount; ++i )
	{
		sDetailResult const& detail = m_Data.s_vDetailResults[i];
		if( detail.s_dwTamerModelId )
			m_pDetailTamerImage[i] = g_pCharResMng->CharImageResLoad( detail.s_dwTamerModelId );
		if( detail.s_dwDigimonModelId )
			m_pDetailDigimonImage[i] = g_pCharResMng->CharImageResLoad( detail.s_dwDigimonModelId );
	}
}

void cDungeonClearResultWindow::_ReleaseDetailImages()
{
	for( int i = 0; i < MAX_DETAIL_CARD_COUNT; ++i )
	{
		if( m_pDetailTamerImage[i] )
			g_pCharResMng->CharImageResDelete( &m_pDetailTamerImage[i] );
		if( m_pDetailDigimonImage[i] )
			g_pCharResMng->CharImageResDelete( &m_pDetailDigimonImage[i] );
	}
}

void cDungeonClearResultWindow::_RenderDetailCards()
{
	for( int i = 0; i < MAX_DETAIL_CARD_COUNT && i < (int)m_Data.s_vDetailResults.size(); ++i )
	{
		CsPoint const ptRoot = GetRootClient();
		CsPoint const ptLocalCard = _DetailCardPos( i );
		CsPoint const ptCard( ptRoot.x + ptLocalCard.x, ptRoot.y + ptLocalCard.y );

		if( m_pDetailTamerImage[i] )
			m_pDetailTamerImage[i]->Render_L( Pt( ptCard, 68, 83 ), CsPoint( 84, 84 ) );
		if( m_pDetailDigimonImage[i] )
			m_pDetailDigimonImage[i]->Render_S( Pt( ptCard, 82, 215 ), CsPoint( 57, 57 ) );

		if( m_pDetailTamerLevelBg[i] )
			m_pDetailTamerLevelBg[i]->Render();
		if( m_pDetailDigimonLevelBg[i] )
			m_pDetailDigimonLevelBg[i]->Render();
		if( m_pDetailStatsIcon[i] )
			m_pDetailStatsIcon[i]->Render();
		if( m_pDetailTamerLevel[i] )
			m_pDetailTamerLevel[i]->Render();
		if( m_pDetailDigimonLevel[i] )
			m_pDetailDigimonLevel[i]->Render();
		if( m_pDetailValue[i] )
			m_pDetailValue[i]->Render();
		if( m_pDetailTamerName[i] )
			m_pDetailTamerName[i]->Render();
		if( m_pDetailDigimonName[i] )
			m_pDetailDigimonName[i]->Render();
	}
}

bool cDungeonClearResultWindow::_SetRewardToolTip()
{
	cTooltip* pToolTip = TOOLTIPMNG_STPTR->GetTooltip();
	if( pToolTip == NULL )
		return false;

	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	for( int i = 0; i < MAX_DEFAULT_REWARD_VISIBLE && i < (int)m_Data.s_vDefaultRewards.size(); ++i )
	{
		CsPoint const ptSlot = _DefaultRewardPos( i );
		if( CsRect( ptSlot, CsSIZE( 44, 44 ) ).PtInRect( ptMouse ) )
		{
			pToolTip->SetTooltip( GetRootClient() + ptSlot, CsPoint( 44, 44 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, m_Data.s_vDefaultRewards[i].s_dwItemId, cBaseWindow::WT_DUNGEON_CLEAR_RESULT );
			return true;
		}
	}

	for( int i = 0; i < MAX_EXTRA_REWARD_VISIBLE && i < (int)m_Data.s_vExtraRewards.size(); ++i )
	{
		CsPoint const ptSlot = _ExtraRewardPos( i );
		if( CsRect( ptSlot, CsSIZE( 44, 44 ) ).PtInRect( ptMouse ) )
		{
			pToolTip->SetTooltip( GetRootClient() + ptSlot, CsPoint( 44, 44 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, m_Data.s_vExtraRewards[i].s_dwItemId, cBaseWindow::WT_DUNGEON_CLEAR_RESULT );
			return true;
		}
	}

	return false;
}

CsPoint cDungeonClearResultWindow::_DefaultRewardPos( int nIndex ) const
{
	return CsPoint( 738 + ( nIndex % 4 ) * 46, 470 + ( nIndex / 4 ) * 46 );
}

CsPoint cDungeonClearResultWindow::_ExtraRewardPos( int nIndex ) const
{
	return CsPoint( 738 + ( nIndex % 4 ) * 46, 577 + ( nIndex / 4 ) * 46 );
}

CsPoint cDungeonClearResultWindow::_DetailCardPos( int nIndex ) const
{
	return CsPoint( 44 + nIndex * 220, 286 );
}

int cDungeonClearResultWindow::_RankImageState( BYTE byRank )
{
	switch( byRank )
	{
	case 0: return 6;
	case 1: return 5;
	case 2: return 4;
	case 3: return 3;
	default: return 0;
	}
}

TCHAR const* cDungeonClearResultWindow::_DifficultyText( WORD wDifficulty )
{
	switch( wDifficulty )
	{
	case 1: return _T( "Easy" );
	case 2: return _T( "Normal" );
	case 3: return _T( "Dif\x00ED" "cil" );
	default: return _T( "-" );
	}
}

TCHAR const* cDungeonClearResultWindow::_DetailTitle( BYTE byCategory )
{
	switch( byCategory )
	{
	case 0: return _T( "Tamer Corajoso" );
	case 1: return _T( "Tamer de confian\x00E7" "a" );
	default: return _T( "Tamer Nobre" );
	}
}

void cDungeonClearResultWindow::_FormatTime( DWORD dwSeconds, TCHAR* szBuffer, size_t nBufferCount )
{
	DWORD const dwHours = dwSeconds / 3600;
	DWORD const dwMinutes = ( dwSeconds % 3600 ) / 60;
	DWORD const dwRemainSeconds = dwSeconds % 60;

	if( dwHours > 0 )
		_stprintf_s( szBuffer, nBufferCount, _T( "%uHora(s)%02uMinuto(s)%02uSegundo(s)" ), dwHours, dwMinutes, dwRemainSeconds );
	else
		_stprintf_s( szBuffer, nBufferCount, _T( "%02uMinuto(s)%02uSegundo(s)" ), dwMinutes, dwRemainSeconds );
}

void cDungeonClearResultWindow::_FormatNumber( DWORD dwValue, TCHAR* szBuffer, size_t nBufferCount )
{
	TCHAR szRaw[32] = { 0, };
	_stprintf_s( szRaw, _countof( szRaw ), _T( "%u" ), dwValue );

	size_t const nLen = _tcslen( szRaw );
	size_t nOut = 0;
	for( size_t i = 0; i < nLen && nOut + 1 < nBufferCount; ++i )
	{
		if( i > 0 && ( nLen - i ) % 3 == 0 && nOut + 1 < nBufferCount )
			szBuffer[nOut++] = _T( ',' );
		szBuffer[nOut++] = szRaw[i];
	}
	szBuffer[nOut] = 0;
}
