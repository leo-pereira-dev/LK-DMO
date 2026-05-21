#include "stdafx.h"
#include "UnionDUnit.h"
#include <math.h>

namespace
{
	const char* const UNION_BG_LEFT			= "Union\\side_bg_a.png";
	const char* const UNION_BG_CENTER		= "Union\\center_bg.png";
	const char* const UNION_BG_RIGHT		= "Union\\side_bg.png";
	const char* const UNION_BG_INFO			= "Union\\side_bg_b.png";
	const char* const UNION_CARD_BG			= "Union\\collection_bg_new.tga";
	const char* const UNION_CARD_FRAME_N		= "Union\\collection_frame_rank1_new.png";
	const char* const UNION_CARD_FRAME_S		= "Union\\collection_frame_rank2_new.png";
	const char* const UNION_CARD_FRAME_U		= "Union\\collection_frame_rank3_new.png";
	const char* const UNION_GAUGE			= "Union\\union_rating_gauge.png";
	const char* const UNION_DIGIMON_HOLD_GAUGE = "Union\\digimon_hold_gauge.png";
	const char* const UNION_DIGIVICE_BORDER	= "Union\\digivice_border.png";
	const char* const UNION_FILTER_ICON		= "Union\\filter_icon.png";
	const char* const UNION_LIST_BG			= "Union\\list_bg.png";
	const char* const UNION_TREASURE_ICON	= "Union\\treasure_box_icon.png";
	const char* const DUNGEON_RESULT_SIMPLE	= "DungeonUI\\dungeon_result_simple_ui.png";

	const char* const ENCY_MAIN_BG			= "Encyclopedia\\newencyclopedia\\main\\main_bg.png";
	const char* const ENCY_CLOSE_BTN		= "Encyclopedia\\newencyclopedia\\main\\exit_btn.png";
	const char* const ENCY_SEARCH_BAR		= "Encyclopedia\\newencyclopedia\\main\\serch bar.png";
	const char* const ENCY_SEARCH_BTN		= "Encyclopedia\\newencyclopedia\\main\\sarch.png";
	const char* const ENCY_LIST_BUTTON		= "Encyclopedia\\newencyclopedia\\detail\\button\\top_button.png";
	const char* const ENCY_BOOKMARK_ICON		= "Encyclopedia\\newencyclopedia\\detail\\button\\bookmark.png";

	CsPoint const CARD_POS[ IF_UNION_DUNIT_CARD_COUNT ] =
	{
		CsPoint( 210, 58 ), CsPoint( 329, 58 ), CsPoint( 448, 58 ), CsPoint( 567, 58 ),
		CsPoint( 210, 193 ), CsPoint( 329, 193 ), CsPoint( 448, 193 ), CsPoint( 567, 193 ),
	};

	TCHAR const* const STAT_LABEL[ IF_UNION_DUNIT_STAT_COUNT ] =
	{
		_T( "MaxHP" ), _T( "MaxDS" ), _T( "DE" ), _T( "EV" ), _T( "HT" ), _T( "EXP" ),
		_T( "BL" ), _T( "" ), _T( "" ), _T( "" ), _T( "" ),
	};

	int const DEFAULT_STAT_VALUES[ IF_UNION_DUNIT_STAT_COUNT ] =
	{
		600, 390, 140, 100, 30, 70, 120, 0, 0, 0, 0,
	};

	void _CopyStatDefaults( int* pOut )
	{
		for( int i = 0; i < IF_UNION_DUNIT_STAT_COUNT; ++i )
			pOut[ i ] = DEFAULT_STAT_VALUES[ i ];
	}

	std::wstring _FormatNumber( int nValue )
	{
		TCHAR szValue[ 32 ] = { 0, };
		_stprintf_s( szValue, _countof( szValue ), _T( "%d" ), nValue );
		return szValue;
	}

	std::wstring _NormalizeName( std::wstring wsText )
	{
		std::wstring wsResult;
		for( size_t i = 0; i < wsText.size(); ++i )
		{
			wchar_t ch = (wchar_t)towlower( wsText[ i ] );
			if( ch == L' ' || ch == L'-' || ch == L'_' || ch == L'(' || ch == L')' || ch == L'[' || ch == L']' )
				continue;
			wsResult.push_back( ch );
		}
		return wsResult;
	}
}

cUnionDUnit::cUnionDUnit()
	: m_pCloseButton( NULL )
	, m_pMoveButton( NULL )
	, m_pSearchButton( NULL )
	, m_pSearchEdit( NULL )
	, m_pMissingAssetText( NULL )
{
	for( int i = 0; i < IF_UNION_DUNIT_FILTER_COUNT; ++i )
		m_pFilterButtons[ i ] = NULL;

	for( int i = 0; i < IF_UNION_DUNIT_CARD_COUNT; ++i )
	{
		m_pCardNameText[ i ] = NULL;
		m_pCardLevelText[ i ] = NULL;
		m_pCardRankText[ i ] = NULL;
	}

	for( int i = 0; i < IF_UNION_DUNIT_EFFECT_COUNT; ++i )
		m_pEffectText[ i ] = NULL;

	for( int i = 0; i < IF_UNION_DUNIT_STAT_COUNT; ++i )
		m_pStatValueText[ i ] = NULL;

	_CopyStatDefaults( m_nStatValues );
}

cUnionDUnit::~cUnionDUnit()
{
}

bool cUnionDUnit::Construct(void)
{
	return true;
}

void cUnionDUnit::Destroy()
{
	cBaseWindow::Delete();
}

void cUnionDUnit::DeleteResource()
{
	_ResetCardIcons();
	if( m_pSearchEdit )
		m_pSearchEdit->ReleaseFocus();

	DeleteScript();

	m_pCloseButton = NULL;
	m_pMoveButton = NULL;
	m_pSearchButton = NULL;
	m_pSearchEdit = NULL;
	m_pMissingAssetText = NULL;
}

void cUnionDUnit::Create( int nValue )
{
	if( cBaseWindow::Init() == false )
		return;

	_BuildViewData();

	int nPosX = ( g_nScreenWidth / 2 ) - ( IF_UNION_DUNIT_WIDTH / 2 );
	int nPosY = ( g_nScreenHeight / 2 ) - ( IF_UNION_DUNIT_HEIGHT / 2 );
	InitScript( const_cast< char* >( ENCY_MAIN_BG ), CsPoint( nPosX, nPosY ), CsPoint( IF_UNION_DUNIT_WIDTH, IF_UNION_DUNIT_HEIGHT ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	_CreateLayout();
	_CreateCardSprites();
}

void cUnionDUnit::Update(float const& fDeltaTime)
{
	UpdateScript( fDeltaTime );
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cUnionDUnit::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
		return muReturn;

	if( m_pCloseButton && m_pCloseButton->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	if( m_pSearchButton && m_pSearchButton->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	if( m_pSearchEdit )
		m_pSearchEdit->Update_ForMouse( CsPoint::ZERO );

	for( int i = 0; i < IF_UNION_DUNIT_FILTER_COUNT; ++i )
	{
		if( m_pFilterButtons[ i ] && m_pFilterButtons[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
			return muReturn;
	}

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( IF_UNION_DUNIT_WIDTH, 42 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
		_UpdateMoveWindow_ForMouse();

	return muReturn;
}

void cUnionDUnit::Render()
{
	RenderScript();
	EndRenderScript();
}

void cUnionDUnit::ResetDevice()
{
	ResetDeviceScript();
	for( size_t i = 0; i < m_vCards.size(); ++i )
	{
		if( m_vCards[ i ].s_pIcon )
			m_vCards[ i ].s_pIcon->ResetDevice();
	}
}

void cUnionDUnit::OnMoveWindow()
{
	cBaseWindow::OnMoveWindow();
}

void cUnionDUnit::PressCloseButton( void* pSender, void* pData )
{
	Close();
}

void cUnionDUnit::_VerifyAssets()
{
	m_mapAssets.clear();
	m_vMissingAssets.clear();

	char const* const kAssets[] =
	{
		UNION_BG_LEFT,
		UNION_BG_CENTER,
		UNION_BG_RIGHT,
		UNION_BG_INFO,
		UNION_CARD_BG,
		UNION_CARD_FRAME_N,
		UNION_CARD_FRAME_S,
		UNION_CARD_FRAME_U,
		UNION_GAUGE,
		UNION_DIGIMON_HOLD_GAUGE,
		UNION_DIGIVICE_BORDER,
		UNION_FILTER_ICON,
		UNION_LIST_BG,
		UNION_TREASURE_ICON,
		DUNGEON_RESULT_SIMPLE,
		ENCY_CLOSE_BTN,
		ENCY_SEARCH_BAR,
		ENCY_SEARCH_BTN,
		ENCY_LIST_BUTTON,
		ENCY_BOOKMARK_ICON,
		"System\\MoveWindow.tga",
	};

	for( int i = 0; i < _countof( kAssets ); ++i )
	{
		bool const bExists = _FileExists( kAssets[ i ] );
		m_mapAssets[ kAssets[ i ] ] = bExists;
		if( bExists == false )
			m_vMissingAssets.push_back( kAssets[ i ] );
	}
}

bool cUnionDUnit::_FileExists( char const* pPath ) const
{
	if( pPath == NULL || pPath[ 0 ] == 0 )
		return false;

	if( CsFPS::CsFPSystem::IsExist( pPath ) )
		return true;

	char szDataPath[ MAX_PATH ] = { 0, };
	strcpy_s( szDataPath, MAX_PATH, "Data\\" );
	strcat_s( szDataPath, MAX_PATH, pPath );
	if( CsFPS::CsFPSystem::IsExist( szDataPath ) )
		return true;

	char szInterfacePath[ MAX_PATH ] = { 0, };
	strcpy_s( szInterfacePath, MAX_PATH, "Data\\Interface\\" );
	strcat_s( szInterfacePath, MAX_PATH, pPath );
	if( CsFPS::CsFPSystem::IsExist( szInterfacePath ) )
		return true;

	char szShortInterfacePath[ MAX_PATH ] = { 0, };
	strcpy_s( szShortInterfacePath, MAX_PATH, "Interface\\" );
	strcat_s( szShortInterfacePath, MAX_PATH, pPath );
	return CsFPS::CsFPSystem::IsExist( szShortInterfacePath );
}

bool cUnionDUnit::_HasAsset( char const* pPath ) const
{
	std::map< std::string, bool >::const_iterator it = m_mapAssets.find( pPath );
	if( it != m_mapAssets.end() )
		return it->second;

	return _FileExists( pPath );
}

cSprite* cUnionDUnit::_AddAssetSprite( CsPoint pos, CsPoint size, char const* pPath, NiColor const& fallbackColor )
{
	if( _HasAsset( pPath ) )
		return AddSprite( pos, size, pPath );

	return _AddSolidSprite( pos, size, NiColorA( fallbackColor.r, fallbackColor.g, fallbackColor.b, 0.68f ) );
}

cSprite* cUnionDUnit::_AddSolidSprite( CsPoint pos, CsPoint size, NiColorA const& color )
{
	cSprite* pSprite = NiNew cSprite;
	if( pSprite == NULL )
		return NULL;

	pSprite->Init( GetRoot(), pos, size, color, false );
	AddChildControl( pSprite );
	return pSprite;
}

cText* cUnionDUnit::_AddLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign )
{
	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem );
	ti.s_eFontSize = eSize;
	ti.s_Color = color;
	ti.s_eTextAlign = nAlign;
	ti.SetText( pText );
	return AddText( &ti, pos );
}

std::wstring cUnionDUnit::_MakeGroupName( DWORD dwGroupID, int nIndex ) const
{
	if( nsCsFileTable::g_pUnionTableMng )
	{
		std::wstring wsName = nsCsFileTable::g_pUnionTableMng->GetGroupName( dwGroupID );
		if( wsName.empty() == false )
			return wsName;
	}

	TCHAR szText[ 64 ] = { 0, };
	_stprintf_s( szText, _countof( szText ), _T( "Union %02d" ), nIndex + 1 );
	return szText;
}

std::wstring cUnionDUnit::_MakeDigimonName( DWORD dwDigimonID ) const
{
	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return std::wstring();

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return std::wstring();

	return pDigimon->GetInfo()->s_szName;
}

std::wstring cUnionDUnit::_MakeEffectLine( int nIndex ) const
{
	if( nIndex >= 0 && nIndex < (int)m_vEffects.size() )
		return m_vEffects[ nIndex ];

	return std::wstring();
}

DWORD cUnionDUnit::_FindDigimonByName( std::wstring const& wsName ) const
{
	if( nsCsFileTable::g_pDigimonMng == NULL || wsName.empty() )
		return 0;

	CsDigimon::MAP* pMap = nsCsFileTable::g_pDigimonMng->GetDigimonMap();
	if( pMap == NULL )
		return 0;

	std::wstring const wsNeedle = _NormalizeName( wsName );
	if( wsNeedle.empty() )
		return 0;

	DWORD dwLooseMatch = 0;
	for( CsDigimon::MAP_CIT it = pMap->begin(); it != pMap->end(); ++it )
	{
		if( it->second == NULL || it->second->GetInfo() == NULL )
			continue;

		std::wstring wsDigimonName = it->second->GetInfo()->s_szName;
		std::wstring wsHaystack = _NormalizeName( wsDigimonName );
		if( wsHaystack.empty() )
			continue;

		if( wsHaystack == wsNeedle )
			return it->first;

		if( dwLooseMatch == 0 && ( wsHaystack.find( wsNeedle ) != std::wstring::npos || wsNeedle.find( wsHaystack ) != std::wstring::npos ) )
			dwLooseMatch = it->first;
	}

	return dwLooseMatch;
}

bool cUnionDUnit::_SetCardDigimon( sCARD_VIEW& card, DWORD dwDigimonID ) const
{
	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return false;

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return false;

	CsDigimon::sINFO* pInfo = pDigimon->GetInfo();
	card.s_dwDigimonID = dwDigimonID;
	card.s_wsName = pInfo->s_szName;
	card.s_nEvoType = (int)pInfo->s_eEvolutionType;
	card.s_nGrowType = (int)pInfo->s_nDigimonType;
	card.s_strIconPath.clear();

	if( g_pModelDataMng )
	{
		card.s_strIconPath = g_pModelDataMng->GetEvoModelIconFile( pInfo->s_dwModelID );
		if( _FileExists( card.s_strIconPath.c_str() ) == false )
			card.s_strIconPath = g_pModelDataMng->GetSmallModelIconFile( pInfo->s_dwModelID );
	}

	return true;
}

void cUnionDUnit::_BuildViewData()
{
	_ResetCardIcons();
	m_vCards.clear();
	m_vFilters.clear();
	m_vEffects.clear();
	_CopyStatDefaults( m_nStatValues );

	CsUnionTableMng* pUnionMng = nsCsFileTable::g_pUnionTableMng;
	if( pUnionMng )
	{
		std::vector< CsUnionTableMng::sFILTER > const& vFilters = pUnionMng->GetFilters();
		for( size_t i = 0; i < vFilters.size() && m_vFilters.size() < IF_UNION_DUNIT_FILTER_COUNT; ++i )
		{
			std::wstring wsFilter = vFilters[ i ].s_wsName.empty() ? pUnionMng->GetFilterText( vFilters[ i ].s_dwFilterID ) : vFilters[ i ].s_wsName;
			if( wsFilter.empty() == false )
				m_vFilters.push_back( wsFilter );
		}

		std::vector< CsUnionTableMng::sGROUP > const& vGroups = pUnionMng->GetGroups();
		for( size_t i = 0; i < vGroups.size() && m_vCards.size() < IF_UNION_DUNIT_CARD_COUNT; ++i )
		{
			sCARD_VIEW card;
			card.s_dwGroupID = vGroups[ i ].s_dwGroupID;
			card.s_nRank = (int)( i % 4 );
			card.s_wsName = _MakeGroupName( card.s_dwGroupID, (int)i );

			for( size_t nSet = 0; nSet < vGroups[ i ].s_vDigimonSets.size() && card.s_dwDigimonID == 0; ++nSet )
			{
				if( vGroups[ i ].s_vDigimonSets[ nSet ].empty() == false )
				{
					CsUnionTableMng::sPAIR const& pair = vGroups[ i ].s_vDigimonSets[ nSet ][ 0 ];
					if( _SetCardDigimon( card, pair.s_dwFirst ) == false )
						_SetCardDigimon( card, pair.s_dwSecond );
				}
			}

			if( card.s_dwDigimonID == 0 )
				_SetCardDigimon( card, _FindDigimonByName( card.s_wsName ) );

			m_vCards.push_back( card );
		}

		std::vector< CsUnionTableMng::sEFFECT > const& vEffects = pUnionMng->GetEffects();
		for( size_t i = 0; i < vEffects.size() && m_vEffects.size() < IF_UNION_DUNIT_EFFECT_COUNT; ++i )
		{
			for( size_t e = 0; e < vEffects[ i ].s_vEntries.size() && m_vEffects.size() < IF_UNION_DUNIT_EFFECT_COUNT; ++e )
			{
				CsUnionTableMng::sEFFECT::sENTRY const& entry = vEffects[ i ].s_vEntries[ e ];
				TCHAR szLine[ 160 ] = { 0, };
				_stprintf_s( szLine, _countof( szLine ), _T( "- Meta %lu (%lu) : + %lu" ), entry.s_dwConditionType, entry.s_dwConditionValue, entry.s_dwEffectValue );
				m_vEffects.push_back( szLine );

				int nStatIndex = (int)m_vEffects.size() - 1;
				if( nStatIndex >= 0 && nStatIndex < IF_UNION_DUNIT_STAT_COUNT )
					m_nStatValues[ nStatIndex ] = (int)entry.s_dwEffectValue;
			}
		}
	}

	TCHAR const* const kFallbackFilters[ IF_UNION_DUNIT_FILTER_COUNT ] =
	{
		_T( "Agumon" ),
		_T( "Agumon" ),
		_T( "Agumon (Black)" ),
		_T( "Agumon (Black)" ),
		_T( "Agumon (Black) [Blitz]" ),
		_T( "Agumon (Black) [Blitz]" ),
		_T( "Agumon (Black) [Milli]" ),
		_T( "ToyAgumon" ),
		_T( "ToyAgumon" ),
		_T( "Gabumon" ),
		_T( "Gabumon" ),
		_T( "Gabumon [CresGaruru]" ),
		_T( "Gabumon [CresGaruru]" ),
		_T( "Gabumon (Black)" ),
	};

	for( int i = (int)m_vFilters.size(); i < IF_UNION_DUNIT_FILTER_COUNT; ++i )
		m_vFilters.push_back( kFallbackFilters[ i ] );

	for( int i = (int)m_vCards.size(); i < IF_UNION_DUNIT_CARD_COUNT; ++i )
	{
		sCARD_VIEW card;
		card.s_nRank = i % 4;
		card.s_wsName = kFallbackFilters[ i % IF_UNION_DUNIT_FILTER_COUNT ];
		_SetCardDigimon( card, _FindDigimonByName( card.s_wsName ) );
		m_vCards.push_back( card );
	}

	TCHAR const* const kFallbackEffects[] =
	{
		_T( "- Trancender Linea Evolutiva : CT + 20" ),
		_T( "- Mata 1500 mobs (0/1500) : CD + 10" ),
		_T( "- Completa 500 Quest (0/500) : DS + 10" ),
		_T( "- Hach 150 Digimons (0/150) : SCD + 10" ),
	};

	for( int i = (int)m_vEffects.size(); i < IF_UNION_DUNIT_EFFECT_COUNT && i < _countof( kFallbackEffects ); ++i )
		m_vEffects.push_back( kFallbackEffects[ i ] );
}

void cUnionDUnit::_CreateLayout()
{
	AddSprite( CsPoint( 8, 47 ), CsPoint( 185, 582 ), UNION_BG_LEFT );
	AddSprite( CsPoint( 13, 55 ), CsPoint( 178, 552 ), UNION_LIST_BG );
	AddSprite( CsPoint( 194, 47 ), CsPoint( 497, 294 ), UNION_BG_CENTER );
	AddSprite( CsPoint( 690, 47 ), CsPoint( 188, 273 ), UNION_BG_RIGHT );
	AddSprite( CsPoint( 690, 323 ), CsPoint( 188, 291 ), UNION_BG_RIGHT );
	AddSprite( CsPoint( 209, 343 ), CsPoint( 468, 202 ), UNION_BG_INFO );

	AddSprite( CsPoint( 702, 74 ), CsPoint( 165, 150 ), DUNGEON_RESULT_SIMPLE );
	AddSprite( CsPoint( 752, 117 ), CsPoint( 64, 64 ), UNION_DIGIMON_HOLD_GAUGE );

	cSprite* pTreasureIcon = NiNew cSprite;
	if( pTreasureIcon )
	{
		pTreasureIcon->Init( GetRoot(), CsPoint( 828, 282 ), CsPoint( 35, 38 ), CsRect( 0, 0, 56, 61 ), UNION_TREASURE_ICON, false );
		AddChildControl( pTreasureIcon );
	}

	m_pMoveButton = AddButton( CsPoint( 827, 5 ), CsPoint( 22, 22 ), CsPoint( 0, 22 ), "System\\MoveWindow.tga" );
	m_pCloseButton = AddButton( CsPoint( 852, 3 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), ENCY_CLOSE_BTN );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cUnionDUnit::PressCloseButton );

	cText::sTEXTINFO titleInfo;
	titleInfo.Init( &g_pEngine->m_FontSystem );
	titleInfo.s_eFontSize = CFont::FS_12;
	titleInfo.s_Color = NiColor( 0.92f, 0.95f, 1.0f );
	titleInfo.s_eTextAlign = DT_CENTER;
	titleInfo.SetText( _T( "D-Unit" ) );
	AddText( &titleInfo, CsPoint( 443, 14 ) );
	AddText( &titleInfo, CsPoint( 444, 14 ) );

	cSprite* pLeftFilterIcon = NiNew cSprite;
	if( pLeftFilterIcon )
	{
		pLeftFilterIcon->Init( GetRoot(), CsPoint( 20, 55 ), CsPoint( 18, 20 ), CsRect( 0, 0, 21, 23 ), UNION_FILTER_ICON, false );
		AddChildControl( pLeftFilterIcon );
	}

	AddSprite( CsPoint( 42, 51 ), CsPoint( 121, 24 ), ENCY_SEARCH_BAR );

	m_pSearchButton = AddButton( CsPoint( 164, 51 ), CsPoint( 24, 24 ), CsPoint( 0, 32 ), ENCY_SEARCH_BTN );

	cText::sTEXTINFO searchInfo;
	searchInfo.Init( &g_pEngine->m_FontSystem );
	searchInfo.s_eFontSize = CFont::FS_10;
	searchInfo.s_Color = NiColor::WHITE;
	searchInfo.s_eTextAlign = DT_LEFT;
	m_pSearchEdit = NiNew cEditBox;
	m_pSearchEdit->Init( GetRoot(), CsPoint( 49, 55 ), CsPoint( 106, 18 ), &searchInfo, false );
	m_pSearchEdit->SetLimitNumber( 32 );
	m_pSearchEdit->SetEmptyMsgText( _T( "Inserir a busca." ), NiColor( 0.55f, 0.65f, 0.78f ) );
	m_pSearchEdit->SetEnableSound( true );
	AddChildControl( m_pSearchEdit );

	int const nRowX = 24;
	int const nRowY = 84;
	int const nRowW = 160;
	int const nRowH = 26;
	int const nRowStep = 28;
	for( int i = 0; i < IF_UNION_DUNIT_FILTER_COUNT; ++i )
	{
		TCHAR const* pFilter = i < (int)m_vFilters.size() ? m_vFilters[ i ].c_str() : _T( "" );
		CsPoint const ptRow( nRowX, nRowY + ( i * nRowStep ) );
		m_pFilterButtons[ i ] = AddButton( ptRow, CsPoint( nRowW, nRowH ), CsPoint( 0, 28 ), ENCY_LIST_BUTTON );
		if( i == 0 )
		{
			_AddSolidSprite( CsPoint( ptRow.x, ptRow.y ), CsPoint( nRowW, 1 ), NiColorA( 1.0f, 0.9f, 0.05f, 0.95f ) );
			_AddSolidSprite( CsPoint( ptRow.x, ptRow.y + nRowH - 1 ), CsPoint( nRowW, 1 ), NiColorA( 1.0f, 0.9f, 0.05f, 0.95f ) );
			_AddSolidSprite( CsPoint( ptRow.x, ptRow.y ), CsPoint( 1, nRowH ), NiColorA( 1.0f, 0.9f, 0.05f, 0.95f ) );
			_AddSolidSprite( CsPoint( ptRow.x + nRowW - 1, ptRow.y ), CsPoint( 1, nRowH ), NiColorA( 1.0f, 0.9f, 0.05f, 0.95f ) );
		}

		_AddLabel( pFilter, CsPoint( ptRow.x + 10, ptRow.y + 8 ), CFont::FS_8, NiColor( 0.96f, 0.97f, 1.0f ), DT_LEFT );

		cSprite* pStar = NiNew cSprite;
		if( pStar )
		{
			pStar->Init( GetRoot(), CsPoint( ptRow.x + nRowW - 25, ptRow.y + 3 ), CsPoint( 20, 20 ), CsRect( 0, 84, 28, 112 ), ENCY_BOOKMARK_ICON, false, NiColor::WHITE );
			AddChildControl( pStar );
		}
	}

	_AddLabel( _T( "0.0%" ), CsPoint( 784, 148 ), CFont::FS_10, NiColor( 0.92f, 0.95f, 1.0f ), DT_CENTER );
	_AddSolidSprite( CsPoint( 714, 258 ), CsPoint( 142, 1 ), NiColorA( 0.0f, 0.95f, 1.0f, 0.95f ) );
	_AddLabel( _T( "Nivel" ), CsPoint( 722, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );
	_AddLabel( _T( "Desconhecido" ), CsPoint( 816, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );

	_AddLabel( _T( "Efeitos das Metas" ), CsPoint( 443, 362 ), CFont::FS_10, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	for( int i = 0; i < IF_UNION_DUNIT_EFFECT_COUNT; ++i )
	{
		TCHAR const* pLine = i < (int)m_vEffects.size() ? m_vEffects[ i ].c_str() : _T( "" );
		m_pEffectText[ i ] = _AddLabel( pLine, CsPoint( 211, 391 + ( i * 23 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
	}

	_AddLabel( _T( "Efeito Total de Coleta" ), CsPoint( 784, 357 ), CFont::FS_9, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	for( int i = 0; i < 7; ++i )
	{
		TCHAR szValue[ 32 ] = { 0, };
		if( i == 5 )
			_stprintf_s( szValue, _countof( szValue ), _T( "+ %d%%" ), m_nStatValues[ i ] );
		else
			_stprintf_s( szValue, _countof( szValue ), _T( "+ %d" ), m_nStatValues[ i ] );

		_AddLabel( STAT_LABEL[ i ], CsPoint( 724, 397 + ( i * 24 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
		m_pStatValueText[ i ] = _AddLabel( szValue, CsPoint( 861, 397 + ( i * 24 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_RIGHT );
	}

	m_pMissingAssetText = NULL;
}

void cUnionDUnit::_CreateCardSprites()
{
	for( int i = 0; i < IF_UNION_DUNIT_CARD_COUNT; ++i )
	{
		CsPoint const ptCard = CARD_POS[ i ];
		cSprite* pCardBg = NiNew cSprite;
		if( pCardBg )
		{
			pCardBg->Init( GetRoot(), ptCard, CsPoint( 104, 120 ), CsRect( 0, 0, 102, 128 ), UNION_CARD_BG, false );
			AddChildControl( pCardBg );
		}

		if( i < (int)m_vCards.size() && m_vCards[ i ].s_strIconPath.empty() == false && _FileExists( m_vCards[ i ].s_strIconPath.c_str() ) )
		{
			int const nIconSize = 36;
			int const nIconRadius = nIconSize / 2;
			int const nIconSrcSize = 32;
			CsPoint const ptIcon( ptCard.x + 52 - nIconRadius, ptCard.y + 44 - nIconRadius );

			for( int nSliceY = 0; nSliceY < nIconSize; ++nSliceY )
			{
				float const fDy = (float)nSliceY + 0.5f - (float)nIconRadius;
				float fHalfWidth = sqrtf( (float)( nIconRadius * nIconRadius ) - ( fDy * fDy ) );
				int const nHalfWidth = (int)fHalfWidth;
				if( nHalfWidth <= 0 )
					continue;

				int const nDestX = ptIcon.x + nIconRadius - nHalfWidth;
				int const nDestW = nHalfWidth * 2;
				int const nSrcLeft = ( ( nIconRadius - nHalfWidth ) * nIconSrcSize ) / nIconSize;
				int const nSrcRight = ( ( nIconRadius + nHalfWidth ) * nIconSrcSize ) / nIconSize;
				int const nSrcTop = ( nSliceY * nIconSrcSize ) / nIconSize;
				int nSrcBottom = ( ( nSliceY + 1 ) * nIconSrcSize ) / nIconSize;
				if( nSrcBottom <= nSrcTop )
					nSrcBottom = nSrcTop + 1;

				cSprite* pIconSlice = NiNew cSprite;
				if( pIconSlice )
				{
					pIconSlice->Init( GetRoot(), CsPoint( nDestX, ptIcon.y + nSliceY ), CsPoint( nDestW, 1 ), CsRect( nSrcLeft, nSrcTop, nSrcRight, nSrcBottom ), m_vCards[ i ].s_strIconPath.c_str(), false, NiColor::WHITE, false );
					AddChildControl( pIconSlice );
				}
			}
		}

		char const* pFrame = UNION_CARD_FRAME_N;
		if( i < (int)m_vCards.size() )
		{
			if( m_vCards[ i ].s_nRank >= 3 )
				pFrame = UNION_CARD_FRAME_U;
			else if( m_vCards[ i ].s_nRank >= 1 )
				pFrame = UNION_CARD_FRAME_S;
		}
		cSprite* pFrameSprite = NiNew cSprite;
		if( pFrameSprite )
		{
			pFrameSprite->Init( GetRoot(), ptCard, CsPoint( 104, 120 ), CsRect( 0, 0, 126, 150 ), pFrame, false );
			AddChildControl( pFrameSprite );
		}

		TCHAR szLevel[ 32 ] = { 0, };
		int nLevel = i < (int)m_vCards.size() ? m_vCards[ i ].s_nLevel : 120;
		_stprintf_s( szLevel, _countof( szLevel ), _T( "Lvl : %d" ), nLevel );

		CsPoint const ptName( ptCard.x + 52, ptCard.y + 21 );
		CsPoint const ptLevel( ptCard.x + 52, ptCard.y + 74 );
		CsPoint const ptRank( ptCard.x + 52, ptCard.y + 112 );
		m_pCardNameText[ i ] = _AddLabel( _T( "" ), ptName, CFont::FS_8, NiColor( 0.96f, 0.97f, 1.0f ), DT_CENTER );
		m_pCardLevelText[ i ] = _AddLabel( szLevel, ptLevel, CFont::FS_10, NiColor( 0.96f, 0.97f, 1.0f ), DT_CENTER );

		TCHAR const* pRank = _T( "N" );
		if( i < (int)m_vCards.size() )
		{
			switch( m_vCards[ i ].s_nRank )
			{
			case 1: pRank = _T( "S" ); break;
			case 2: pRank = _T( "S+" ); break;
			case 3: pRank = _T( "U" ); break;
			default: pRank = _T( "N" ); break;
			}
		}
		m_pCardRankText[ i ] = _AddLabel( pRank, ptRank, CFont::FS_8, NiColor( 0.95f, 0.95f, 1.0f ), DT_CENTER );
	}
}

void cUnionDUnit::_ResetCardIcons()
{
	for( size_t i = 0; i < m_vCards.size(); ++i )
		NISAFE_DELETE( m_vCards[ i ].s_pIcon );
}

void cUnionDUnit::_RenderCardIcons()
{
	CsPoint const ptRoot = GetRootClient();
	for( int i = 0; i < IF_UNION_DUNIT_CARD_COUNT && i < (int)m_vCards.size(); ++i )
	{
		CsPoint const ptIcon( ptRoot.x + CARD_POS[ i ].x + 28, ptRoot.y + CARD_POS[ i ].y + 28 );
		if( m_vCards[ i ].s_pIcon )
		{
			m_vCards[ i ].s_pIcon->Render( ptIcon, CsPoint( 48, 48 ) );
			continue;
		}

		if( g_pIconMng )
		{
			CsPoint const ptInnerIcon( ptIcon.x + 8, ptIcon.y + 8 );
			if( m_vCards[ i ].s_nGrowType > 0 )
				g_pIconMng->RenderIcon( ICONITEM::DGrowType, ptInnerIcon, CsPoint( 32, 32 ), m_vCards[ i ].s_nGrowType - 1 );
			else
				g_pIconMng->RenderDEvol( (nsCsDigimonTable::eEVOLUTION_TYPE)m_vCards[ i ].s_nEvoType, ptInnerIcon, CsPoint( 32, 32 ) );
		}
	}
}
