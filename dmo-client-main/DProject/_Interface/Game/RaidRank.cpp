

#include "stdafx.h"

#define IF_RANK_RANK_POS			CsPoint( 24, 41 )
#define IF_RANK_TAMER_POS		CsPoint( 95, 41 )
#define IF_RANK_DIGIMON_POS	CsPoint( 204, 41 )
#define IF_RANK_DAMAGE_POS		CsPoint( 324, 41 )

cRaidRank::cRaidRank()
	: m_pBtnClose( NULL )
	, m_bPendingRankerUpdate( false )
	, m_bControlsReady( false )
{
	InitializeCriticalSection( &m_csRanker );

	for( int i = 0; i < RAID_RANK_FIELD_COUNT; ++i )
		m_pHeaderText[ i ] = NULL;

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
	{
		m_RankerLine[ i ].Reset();
		m_PendingRankerLine[ i ].Reset();

		for( int j = 0; j < RAID_RANK_FIELD_COUNT; ++j )
			m_pRankerText[ i ][ j ] = NULL;
	}
}

cRaidRank::~cRaidRank()
{
	DeleteCriticalSection( &m_csRanker );
}

void cRaidRank::sRANKER_LINE::Reset()
{
	s_bVisible = false;
	s_nRank = 0;
	s_szTamer[ 0 ] = 0;
	s_szDigimon[ 0 ] = 0;
	s_nDamage = 0;
	s_Color = NiColor::WHITE;
}

void cRaidRank::Destroy()
{	
	cBaseWindow::Delete();		
}

void cRaidRank::DeleteResource()
{	
	m_bControlsReady = false;
	DeleteScript();

	m_pBtnClose = NULL;

	for( int i = 0; i < RAID_RANK_FIELD_COUNT; ++i )
		m_pHeaderText[ i ] = NULL;

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
	{
		m_RankerLine[ i ].Reset();
		m_PendingRankerLine[ i ].Reset();

		for( int j = 0; j < RAID_RANK_FIELD_COUNT; ++j )
			m_pRankerText[ i ][ j ] = NULL;
	}
}
void cRaidRank::ResetRankList()
{
	EnterCriticalSection( &m_csRanker );

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
		m_PendingRankerLine[ i ].Reset();

	m_bPendingRankerUpdate = true;

	LeaveCriticalSection( &m_csRanker );
}

void cRaidRank::_CreateStaticText()
{
	cText::sTEXTINFO title;
	title.Init( &g_pEngine->m_FontText, CFont::FS_12 );
	title.SetText( UISTRING_TEXT( "RAIDRANK_DAMAGE_RANKING" ).c_str() );
	title.s_eTextAlign = DT_CENTER;

	CsPoint titlePos;
	titlePos.x = m_ptRootSize.x / 2 + CsPoint::ZERO.x;
	titlePos.y = 7 + CsPoint::ZERO.y;

	m_pWindowTitle = AddText( &title, titlePos );

	cText::sTEXTINFO ti;
	ti.Init();
	ti.s_eFontSize = CFont::FS_10;
	ti.s_bOutLine = false;
	ti.s_eTextAlign = DT_CENTER;
	ti.SetText( UISTRING_TEXT( "RAIDRANK_RANK" ).c_str() );
	m_pHeaderText[ 0 ] = AddText( &ti, IF_RANK_RANK_POS );	

	ti.SetText( UISTRING_TEXT( "COMMON_TXT_TAMER" ).c_str() );
	m_pHeaderText[ 1 ] = AddText( &ti, IF_RANK_TAMER_POS );	


	ti.SetText( UISTRING_TEXT( "COMMON_TXT_DIGIMON" ).c_str() );
	m_pHeaderText[ 2 ] = AddText( &ti, IF_RANK_DIGIMON_POS );	


	ti.SetText( UISTRING_TEXT( "RAIDRANK_DAMAGE" ).c_str() );
	m_pHeaderText[ 3 ] = AddText( &ti, IF_RANK_DAMAGE_POS );	

	ti.SetText( _T( " " ) );

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
	{
		int oy = 58 + (17 * i);

		m_pRankerText[ i ][ 0 ] = AddText( &ti, CsPoint( 24, oy ) );
		m_pRankerText[ i ][ 1 ] = AddText( &ti, CsPoint( 95, oy ) );
		m_pRankerText[ i ][ 2 ] = AddText( &ti, CsPoint( 204, oy ) );
		m_pRankerText[ i ][ 3 ] = AddText( &ti, CsPoint( 324, oy ) );

		for( int j = 0; j < RAID_RANK_FIELD_COUNT; ++j )
		{
			if( m_pRankerText[ i ][ j ] )
				m_pRankerText[ i ][ j ]->SetVisible( false );
		}
	}
}

bool cRaidRank::_HasControlsReady()
{
	if( m_pBtnClose == NULL )
		return false;

	for( int i = 0; i < RAID_RANK_FIELD_COUNT; ++i )
	{
		if( m_pHeaderText[ i ] == NULL )
			return false;
	}

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
	{
		for( int j = 0; j < RAID_RANK_FIELD_COUNT; ++j )
		{
			if( m_pRankerText[ i ][ j ] == NULL )
				return false;
		}
	}

	return true;
}

void cRaidRank::_ApplyPendingRankList()
{
	sRANKER_LINE line[ MAX_RAID_RANKER_COUNT ];

	if( m_bControlsReady == false || _HasControlsReady() == false )
		return;

	EnterCriticalSection( &m_csRanker );

	if( m_bPendingRankerUpdate == false )
	{
		LeaveCriticalSection( &m_csRanker );
		return;
	}

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
		line[ i ] = m_PendingRankerLine[ i ];

	m_bPendingRankerUpdate = false;

	LeaveCriticalSection( &m_csRanker );

	for( int i = 0; i < MAX_RAID_RANKER_COUNT; ++i )
	{
		m_RankerLine[ i ] = line[ i ];

		for( int j = 0; j < RAID_RANK_FIELD_COUNT; ++j )
		{
			if( m_pRankerText[ i ][ j ] )
				m_pRankerText[ i ][ j ]->SetVisible( line[ i ].s_bVisible );
		}

		if( line[ i ].s_bVisible == false )
			continue;

		if( m_pRankerText[ i ][ 0 ] == NULL ||
			m_pRankerText[ i ][ 1 ] == NULL ||
			m_pRankerText[ i ][ 2 ] == NULL ||
			m_pRankerText[ i ][ 3 ] == NULL )
		{
			continue;
		}

		TCHAR sz[ 128 ];

		_stprintf_s( sz, 128, _T( "%d" ), line[ i ].s_nRank );
		m_pRankerText[ i ][ 0 ]->SetText( sz );
		m_pRankerText[ i ][ 0 ]->SetColor( line[ i ].s_Color );

		m_pRankerText[ i ][ 1 ]->SetText( line[ i ].s_szTamer );
		m_pRankerText[ i ][ 1 ]->SetColor( line[ i ].s_Color );

		m_pRankerText[ i ][ 2 ]->SetText( line[ i ].s_szDigimon );
		m_pRankerText[ i ][ 2 ]->SetColor( line[ i ].s_Color );

		_stprintf_s( sz, 128, _T( "%d" ), line[ i ].s_nDamage );
		m_pRankerText[ i ][ 3 ]->SetText( sz );
		m_pRankerText[ i ][ 3 ]->SetColor( line[ i ].s_Color );
	}
}

void cRaidRank::Create(int nValue /* = 0  */)
{	
	cBaseWindow::Init();
	m_bControlsReady = false;

	SetRootClient( CsPoint( (g_nScreenWidth/2)-(398/2), g_nScreenHeight/3 ) );
	InitScript( "Ranking\\Ranking_Raid.bmp", m_ptRootClient , CsPoint( 398, 258 ), true, IFREGION_X::CENTER, IFREGION_Y::CENTER, false );

	m_pBtnClose = AddButton( CsPoint( 370, 6 ), CsPoint( 16, 16 ), CsPoint( 0, 16 ), "System\\Ch_Close.tga" );

	_CreateStaticText();
	m_bControlsReady = _HasControlsReady();
	ResetRankList();
}


void cRaidRank::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();
	_ApplyPendingRankList();
}

cBaseWindow::eMU_TYPE
cRaidRank::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
		return muReturn;

	

	if( m_bControlsReady == false || m_pBtnClose == NULL )
		return muReturn;

	switch( m_pBtnClose->Update_ForMouse() )
	{
	case cButton::ACTION_NONE:
		break;
	case cButton::ACTION_CLICK:
		m_pRankingString.Delete();
		Close();
		return muReturn;
	default:
		return muReturn;
	}
	
	// 위에 바 드래그해야만 이동 시키도록 하겠다.
	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( 398, 28 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
		_UpdateMoveWindow_ForMouse();

	return muReturn;
}

void cRaidRank::OnMouseOutWindow()
{
	cBaseWindow::OnMouseOutWindow();	
}

void cRaidRank::Render()
{
	RenderScript();	

	cBaseWindow::EndRenderScript();
}

void cRaidRank::ResetDevice()
{
	ResetDeviceScript();
	m_pRankingString.ResetDevice();
}

bool cRaidRank::OnEscapeKey()
{
	m_pRankingString.Delete();
	return Close();
}

void cRaidRank::SetRanker(int nIndex, int nRank, TCHAR* szTamer, TCHAR* szDigimon, int nDamage, NiColor color)
{
	if( nIndex < 0 || nIndex >= MAX_RAID_RANKER_COUNT )
		return;

	EnterCriticalSection( &m_csRanker );

	m_PendingRankerLine[ nIndex ].s_bVisible = true;
	m_PendingRankerLine[ nIndex ].s_nRank = nRank;
	_tcsncpy_s( m_PendingRankerLine[ nIndex ].s_szTamer, RAID_RANK_NAME_LEN + 1, szTamer ? szTamer : _T( "" ), _TRUNCATE );
	_tcsncpy_s( m_PendingRankerLine[ nIndex ].s_szDigimon, RAID_RANK_NAME_LEN + 1, szDigimon ? szDigimon : _T( "" ), _TRUNCATE );
	m_PendingRankerLine[ nIndex ].s_nDamage = nDamage;
	m_PendingRankerLine[ nIndex ].s_Color = color;
	m_bPendingRankerUpdate = true;

	LeaveCriticalSection( &m_csRanker );
}
