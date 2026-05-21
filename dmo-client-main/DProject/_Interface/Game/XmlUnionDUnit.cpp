#include "stdafx.h"
#include "XmlUnionDUnit.h"
#include <math.h>
#include <wctype.h>

namespace
{
	const char* const XML_UNION_CLOSE_BTN = "Encyclopedia\\newencyclopedia\\main\\exit_btn.png";
	const char* const XML_UNION_MOVE_BTN = "System\\MoveWindow.tga";
	const char* const XML_UNION_LIST_BUTTON = "Encyclopedia\\newencyclopedia\\detail\\button\\top_button.png";
	const char* const XML_UNION_CARD_BG = "Union\\collection_bg_new.tga";
	const char* const XML_UNION_FRAME_N = "Union\\collection_frame_rank1_new.png";
	const char* const XML_UNION_FRAME_S = "Union\\collection_frame_rank2_new.png";
	const char* const XML_UNION_FRAME_U = "Union\\collection_frame_rank3_new.png";
	const char* const XML_UNION_RANK_ICON = "Encyclopedia\\newencyclopedia\\icon\\encyclopedia_512.png";
	const char* const XML_UNION_BG_LEFT = "Union\\side_bg_a.png";
	const char* const XML_UNION_BG_CENTER = "Union\\center_bg.png";
	const char* const XML_UNION_BG_RIGHT = "Union\\side_bg.png";
	const char* const XML_UNION_BG_INFO = "Union\\side_bg_b.png";
	const char* const XML_UNION_LIST_BG = "Union\\list_bg.png";
	const char* const XML_UNION_SEARCH_BAR = "Encyclopedia\\newencyclopedia\\main\\serch bar.png";
	const char* const XML_UNION_SEARCH_BTN = "Encyclopedia\\newencyclopedia\\main\\sarch.png";
	const char* const XML_UNION_FILTER_ICON = "Union\\filter_icon.png";
	const char* const XML_UNION_BOOKMARK_ICON = "Encyclopedia\\newencyclopedia\\detail\\button\\bookmark.png";
	const char* const XML_UNION_SCROLL_UP = "Encyclopedia\\Scroll\\ScrollUpBtn.tga";
	const char* const XML_UNION_SCROLL_DOWN = "Encyclopedia\\Scroll\\ScrollDownBtn.tga";
	const char* const XML_UNION_DIGIMON_HOLD_GAUGE = "Union\\digimon_hold_gauge.png";
	const char* const XML_UNION_TREASURE_ICON = "Union\\treasure_box_icon.png";
	const char* const DUNGEON_RESULT_SIMPLE = "DungeonUI\\dungeon_result_simple_ui.png";
	int const XML_UNION_FILTER_ROW_X = 21;
	int const XML_UNION_FILTER_ROW_Y = 84;
	int const XML_UNION_FILTER_ROW_W = 174;
	int const XML_UNION_FILTER_ROW_H = 30;
	int const XML_UNION_FILTER_ROW_STEP = 34;
	int const XML_UNION_FILTER_SCROLL_X = 197;
	int const XML_UNION_FILTER_SCROLL_Y = 84;
	int const XML_UNION_FILTER_SCROLL_W = 14;
	int const XML_UNION_FILTER_SCROLL_H = ( IF_XML_UNION_FILTER_COUNT * XML_UNION_FILTER_ROW_STEP ) - 2;
	int const XML_UNION_FILTER_SCROLL_ARROW_H = 14;
	int const XML_UNION_FILTER_SCROLL_TRACK_Y = XML_UNION_FILTER_SCROLL_Y + XML_UNION_FILTER_SCROLL_ARROW_H;
	int const XML_UNION_FILTER_SCROLL_TRACK_H = XML_UNION_FILTER_SCROLL_H - ( XML_UNION_FILTER_SCROLL_ARROW_H * 2 );
	int const XML_UNION_CARD_SCROLL_X = 689;
	int const XML_UNION_CARD_SCROLL_Y = 66;
	int const XML_UNION_CARD_SCROLL_W = 14;
	int const XML_UNION_CARD_SCROLL_H = 288;
	int const XML_UNION_CARD_SCROLL_ARROW_H = 14;
	int const XML_UNION_CARD_SCROLL_TRACK_Y = XML_UNION_CARD_SCROLL_Y + XML_UNION_CARD_SCROLL_ARROW_H;
	int const XML_UNION_CARD_SCROLL_TRACK_H = XML_UNION_CARD_SCROLL_H - ( XML_UNION_CARD_SCROLL_ARROW_H * 2 );
	int const XML_UNION_CARD_SCROLL_PAGE = IF_XML_UNION_CARD_COUNT;

	CsPoint const CARD_POS[ IF_XML_UNION_CARD_COUNT ] =
	{
		CsPoint( 226, 91 ), CsPoint( 345, 91 ), CsPoint( 464, 91 ), CsPoint( 583, 91 ),
		CsPoint( 226, 226 ), CsPoint( 345, 226 ), CsPoint( 464, 226 ), CsPoint( 583, 226 ),
	};

	wchar_t const* const STAT_LABEL[ IF_XML_UNION_STAT_COUNT ] =
	{
		L"MaxHP", L"MaxDS", L"AT", L"DE", L"EV", L"HT",
		L"CT", L"EXP", L"BL", L"SCD", L"Other",
	};

	void _CopyWideToTChar( std::wstring const& wsText, TCHAR* pOut, size_t nOutCount )
	{
		if( pOut == NULL || nOutCount == 0 )
			return;

		pOut[ 0 ] = 0;
#ifdef UNICODE
		wcsncpy_s( pOut, nOutCount, wsText.c_str(), _TRUNCATE );
#else
		WideCharToMultiByte( CP_ACP, 0, wsText.c_str(), -1, pOut, (int)nOutCount, NULL, NULL );
#endif
	}

	std::wstring _FormatReward( cUnionXmlTools::sREWARD const& reward )
	{
		if( reward.s_wsRaw.empty() == false )
			return L"- " + reward.s_wsRaw;

		wchar_t szText[ 128 ] = { 0, };
		swprintf_s( szText, _countof( szText ), L"- %s + %d%s", reward.s_wsStat.c_str(), reward.s_nValue, reward.s_bPercent ? L"%" : L"" );
		return szText;
	}

	std::wstring _MakeOptionText( std::wstring wsName )
	{
		size_t nPos = wsName.find( L" Line " );
		if( nPos != std::wstring::npos )
			wsName.erase( nPos );

		while( wsName.empty() == false && wsName[ wsName.size() - 1 ] >= L'0' && wsName[ wsName.size() - 1 ] <= L'9' )
			wsName.erase( wsName.size() - 1 );

		while( wsName.empty() == false && wsName[ wsName.size() - 1 ] == L' ' )
			wsName.erase( wsName.size() - 1 );

		if( wsName == L"Agumon Black" )
			return L"Agumon (Black)";
		if( wsName == L"Agumon Black Blitz" )
			return L"Agumon (Black) [Blitz]";
		if( wsName == L"Gabumon Black" )
			return L"Gabumon (Black)";
		if( wsName == L"Gabumon Cres" )
			return L"Gabumon [CresGaruru]";

		return wsName;
	}

	bool _PushUnique( std::vector< std::wstring >& vec, std::wstring const& wsText )
	{
		if( wsText.empty() )
			return false;
		for( size_t i = 0; i < vec.size(); ++i )
		{
			if( vec[ i ] == wsText )
				return false;
		}
		vec.push_back( wsText );
		return true;
	}

	bool _IsSameOption( std::wstring const& a, std::wstring const& b )
	{
		return a.empty() == false && a == b;
	}

	std::wstring _ToLower( std::wstring wsText )
	{
		for( size_t i = 0; i < wsText.size(); ++i )
			wsText[ i ] = (wchar_t)towlower( wsText[ i ] );
		return wsText;
	}

	bool _ContainsNoCase( std::wstring const& wsText, std::wstring const& wsNeedle )
	{
		if( wsNeedle.empty() )
			return true;
		return _ToLower( wsText ).find( _ToLower( wsNeedle ) ) != std::wstring::npos;
	}

	std::wstring _NormalizeDigimonName( std::wstring wsText )
	{
		std::wstring wsResult;
		for( size_t i = 0; i < wsText.size(); ++i )
		{
			wchar_t ch = (wchar_t)towlower( wsText[ i ] );
			if( ch == L' ' || ch == L'-' || ch == L'_' || ch == L'(' || ch == L')' || ch == L'[' || ch == L']' || ch == L':' || ch == L',' || ch == L'.' || ch == L'\'' || ch == L'+' || ch == L'?' )
				continue;
			wsResult.push_back( ch );
		}

		size_t nPos = 0;
		while( ( nPos = wsResult.find( L"blk", nPos ) ) != std::wstring::npos )
		{
			wsResult.replace( nPos, 3, L"black" );
			nPos += 5;
		}

		nPos = 0;
		while( ( nPos = wsResult.find( L"millenium", nPos ) ) != std::wstring::npos )
		{
			wsResult.replace( nPos, 9, L"millennium" );
			nPos += 10;
		}

		return wsResult;
	}

	std::wstring _GetBaseNameBeforeX( std::wstring const& wsText )
	{
		std::wstring wsNormalized = _NormalizeDigimonName( wsText );
		size_t const nXPos = wsNormalized.find( L"x" );
		if( nXPos != std::wstring::npos && nXPos > 0 )
			wsNormalized.erase( nXPos );
		return wsNormalized;
	}

	bool _PushUniqueDigimon( std::vector< DWORD >& vec, DWORD dwDigimonID )
	{
		if( dwDigimonID == 0 )
			return false;

		for( size_t i = 0; i < vec.size(); ++i )
		{
			if( vec[ i ] == dwDigimonID )
				return false;
		}
		vec.push_back( dwDigimonID );
		return true;
	}

	int _GetUnionRankIconIndex( int nDigimonRank )
	{
		// Runtime render indexes observed in this client:
		// 9:SSS, 10:SSS+ (Korean badge), 11:U+.
		if( nDigimonRank <= 0 )
			return 1;
		if( nDigimonRank >= 10 )
			return 10;
		return nDigimonRank - 1;
	}

	class cXmlUnionHoverEffectSprite : public cSprite
	{
	public:
		cXmlUnionHoverEffectSprite()
			: m_fCurAniTime( 0.0f )
			, m_fSetAniTime( 0.055f )
			, m_nFrame( 0 )
		{
			for( int i = 0; i < HOVER_EFFECT_FRAME_COUNT; ++i )
				m_szFramePath[ i ][ 0 ] = 0;
		}

		void InitHoverEffect( cWindow* pParent, CsPoint pos, CsPoint size )
		{
			for( int i = 0; i < HOVER_EFFECT_FRAME_COUNT; ++i )
				sprintf_s( m_szFramePath[ i ], MAX_PATH, "Encyclopedia\\newencyclopedia\\collection_effect_frame%02d.png", i + 1 );

			cSprite::Init( pParent, pos, size, m_szFramePath[ 0 ], false );
			SetAlpha( 0.8f );
			SetVisible( false );
		}

		virtual void Update( float const& fDeltaTime )
		{
			if( GetVisible() == false )
				return;

			m_fCurAniTime -= fDeltaTime;
			while( m_fCurAniTime <= 0.0f )
			{
				m_fCurAniTime += m_fSetAniTime;
				m_nFrame = ( m_nFrame + 1 ) % HOVER_EFFECT_FRAME_COUNT;
				ChangeTexture( m_szFramePath[ m_nFrame ] );
				SetAlpha( 0.8f );
			}
		}

	private:
		enum { HOVER_EFFECT_FRAME_COUNT = 7 };

		char		m_szFramePath[ HOVER_EFFECT_FRAME_COUNT ][ MAX_PATH ];
		float		m_fCurAniTime;
		float		m_fSetAniTime;
		int			m_nFrame;
	};
}

cXmlUnionDUnit::cXmlUnionDUnit()
	: m_pCloseButton( NULL )
	, m_pMoveButton( NULL )
	, m_pStatusText( NULL )
	, m_pFilterScrollBorder( NULL )
	, m_pFilterScrollBg( NULL )
	, m_pFilterScrollTrack( NULL )
	, m_pFilterScrollThumb( NULL )
	, m_pFilterScrollUp( NULL )
	, m_pFilterScrollDown( NULL )
	, m_pHoverTooltipBg( NULL )
	, m_pHoverTooltipTop( NULL )
	, m_pHoverTooltipBottom( NULL )
	, m_pHoverTooltipLeft( NULL )
	, m_pHoverTooltipRight( NULL )
	, m_pHoverTooltipText( NULL )
	, m_pTabBody( NULL )
	, m_pTabTop( NULL )
	, m_pTabBottom( NULL )
	, m_pTabLeft( NULL )
	, m_pTabText( NULL )
	, m_pCardScrollBorder( NULL )
	, m_pCardScrollBg( NULL )
	, m_pCardScrollTrack( NULL )
	, m_pCardScrollThumb( NULL )
	, m_pCardScrollUp( NULL )
	, m_pCardScrollDown( NULL )
	, m_pSearchEdit( NULL )
	, m_nFilterScroll( 0 )
	, m_nCardScroll( 0 )
	, m_nSelectedFilter( 0 )
	, m_nHoverFilter( -1 )
	, m_nHoverCard( -1 )
{
	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		m_pFilterButtons[ i ] = NULL;
		m_pFilterHoverBg[ i ] = NULL;
		m_pFilterBookmark[ i ] = NULL;
		m_pFilterBorderTop[ i ] = NULL;
		m_pFilterBorderBottom[ i ] = NULL;
		m_pFilterBorderLeft[ i ] = NULL;
		m_pFilterBorderRight[ i ] = NULL;
		m_pFilterText[ i ] = NULL;
	}

	for( int i = 0; i < IF_XML_UNION_TAB_SLANT_COUNT; ++i )
	{
		m_pTabSlant[ i ] = NULL;
		m_pTabSlantTop[ i ] = NULL;
	}

	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		m_strCardIconPath[ i ].clear();
		m_strCardFramePath[ i ].clear();
		m_pCardBg[ i ] = NULL;
		m_pCardFrame[ i ] = NULL;
		m_pCardHoverEffects[ i ] = NULL;
		m_pCardLevelText[ i ] = NULL;
		m_pCardRankIcon[ i ] = NULL;
		for( int nSlice = 0; nSlice < IF_XML_UNION_ICON_SLICE_COUNT; ++nSlice )
			m_pCardIconSlices[ i ][ nSlice ] = NULL;
	}

	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
		m_pEffectText[ i ] = NULL;

	for( int i = 0; i < IF_XML_UNION_STAT_COUNT; ++i )
		m_pStatValueText[ i ] = NULL;
}

cXmlUnionDUnit::~cXmlUnionDUnit()
{
}

bool cXmlUnionDUnit::Construct(void)
{
	return true;
}

void cXmlUnionDUnit::Destroy()
{
	cBaseWindow::Delete();
}

void cXmlUnionDUnit::DeleteResource()
{
	DeleteScript();

	m_pCloseButton = NULL;
	m_pMoveButton = NULL;
	m_pStatusText = NULL;
	m_pSearchEdit = NULL;
	m_pFilterScrollBorder = NULL;
	m_pFilterScrollBg = NULL;
	m_pFilterScrollTrack = NULL;
	m_pFilterScrollThumb = NULL;
	m_pFilterScrollUp = NULL;
	m_pFilterScrollDown = NULL;
	m_pHoverTooltipBg = NULL;
	m_pHoverTooltipTop = NULL;
	m_pHoverTooltipBottom = NULL;
	m_pHoverTooltipLeft = NULL;
	m_pHoverTooltipRight = NULL;
	m_pHoverTooltipText = NULL;
	m_pTabBody = NULL;
	m_pTabTop = NULL;
	m_pTabBottom = NULL;
	m_pTabLeft = NULL;
	m_pTabText = NULL;
	m_pCardScrollBorder = NULL;
	m_pCardScrollBg = NULL;
	m_pCardScrollTrack = NULL;
	m_pCardScrollThumb = NULL;
	m_pCardScrollUp = NULL;
	m_pCardScrollDown = NULL;
	for( int i = 0; i < IF_XML_UNION_TAB_SLANT_COUNT; ++i )
	{
		m_pTabSlant[ i ] = NULL;
		m_pTabSlantTop[ i ] = NULL;
	}
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		m_pCardBg[ i ] = NULL;
		m_pCardFrame[ i ] = NULL;
		m_pCardHoverEffects[ i ] = NULL;
		m_pCardLevelText[ i ] = NULL;
		m_pCardRankIcon[ i ] = NULL;
		for( int nSlice = 0; nSlice < IF_XML_UNION_ICON_SLICE_COUNT; ++nSlice )
			m_pCardIconSlices[ i ][ nSlice ] = NULL;
	}
	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		m_pFilterButtons[ i ] = NULL;
		m_pFilterHoverBg[ i ] = NULL;
		m_pFilterBookmark[ i ] = NULL;
		m_pFilterBorderTop[ i ] = NULL;
		m_pFilterBorderBottom[ i ] = NULL;
		m_pFilterBorderLeft[ i ] = NULL;
		m_pFilterBorderRight[ i ] = NULL;
		m_pFilterText[ i ] = NULL;
	}
	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
		m_pEffectText[ i ] = NULL;
	for( int i = 0; i < IF_XML_UNION_STAT_COUNT; ++i )
		m_pStatValueText[ i ] = NULL;
}

void cXmlUnionDUnit::Create( int nValue )
{
	if( cBaseWindow::Init() == false )
		return;

	_BuildViewData();

	int nPosX = ( g_nScreenWidth / 2 ) - ( IF_XML_UNION_WIDTH / 2 );
	int nPosY = ( g_nScreenHeight / 2 ) - ( IF_XML_UNION_HEIGHT / 2 );
	InitScript( NULL, CsPoint( nPosX, nPosY ), CsPoint( IF_XML_UNION_WIDTH, IF_XML_UNION_HEIGHT ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	_CreateLayout();
	_CreateCardSprites();
	_CreateCardScrollBar();
	_CreateHoverTooltipControls();
}

void cXmlUnionDUnit::Update(float const& fDeltaTime)
{
	UpdateScript( fDeltaTime );
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cXmlUnionDUnit::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
		return muReturn;

	if( m_pCloseButton && m_pCloseButton->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	if( m_pSearchEdit )
	{
		if( m_pSearchEdit->Update_ForMouse( CsPoint::ZERO ) != cEditBox::ACTION_NONE )
			return muReturn;

		std::wstring const wsNow = m_pSearchEdit->GetString();
		if( wsNow != m_wsSearchKeyword )
		{
			m_wsSearchKeyword = wsNow;
			m_nFilterScroll = 0;
			m_nCardScroll = 0;
			_RefreshView();
			return muReturn;
		}
	}

	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		if( m_pFilterButtons[ i ] && m_pFilterButtons[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
		{
			int const nFilteredIndex = m_nFilterScroll + i;
			int const nNewSelected = nFilteredIndex >= 0 && nFilteredIndex < (int)m_vFilteredFilterIndices.size() ? m_vFilteredFilterIndices[ nFilteredIndex ] : -1;
			if( nNewSelected >= 0 && nNewSelected < (int)m_vAllFilters.size() && nNewSelected != m_nSelectedFilter )
			{
				m_nSelectedFilter = nNewSelected;
				m_nCardScroll = 0;
				_RefreshView();
			}
			return muReturn;
		}
	}

	CsPoint const ptRoot = GetRootClient();
	CsPoint const ptCursor = CURSOR_ST.GetPos();
	int nHoverFilter = -1;
	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		int const nFilteredIndex = m_nFilterScroll + i;
		if( nFilteredIndex >= (int)m_vFilteredFilterIndices.size() )
			continue;

		CsPoint const ptRow( ptRoot.x + XML_UNION_FILTER_ROW_X, ptRoot.y + XML_UNION_FILTER_ROW_Y + ( i * XML_UNION_FILTER_ROW_STEP ) );
		if( ptCursor.x >= ptRow.x && ptCursor.x <= ptRow.x + XML_UNION_FILTER_ROW_W &&
			ptCursor.y >= ptRow.y && ptCursor.y <= ptRow.y + XML_UNION_FILTER_ROW_H )
		{
			nHoverFilter = nFilteredIndex;
			break;
		}
	}
	if( nHoverFilter != m_nHoverFilter )
	{
		m_nHoverFilter = nHoverFilter;
		_UpdateFilterRowControls();
	}

	int nHoverCard = -1;
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT && i < (int)m_vCards.size(); ++i )
	{
		int const nCardIndex = m_nCardScroll + i;
		if( nCardIndex < 0 || nCardIndex >= (int)m_vCards.size() )
			continue;

		CsPoint const ptCard = CARD_POS[ i ];
		if( ptCursor.x >= ptRoot.x + ptCard.x && ptCursor.x <= ptRoot.x + ptCard.x + 104 &&
			ptCursor.y >= ptRoot.y + ptCard.y && ptCursor.y <= ptRoot.y + ptCard.y + 120 )
		{
			nHoverCard = nCardIndex;
			break;
		}
	}
	if( nHoverCard != m_nHoverCard )
	{
		m_nHoverCard = nHoverCard;
		_UpdateHoverTooltipControls();
		return muReturn;
	}

	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL &&
		CURSOR_ST.IsInCursor( ptRoot.x + 218, ptRoot.y + 58, ptRoot.x + 704, ptRoot.y + 358 ) )
	{
		int const nMaxScroll = max( 0, ( ( (int)m_vCards.size() - 1 ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE );
		int const nOldScroll = m_nCardScroll;
		if( CURSOR_ST.GetWheel() < 0 && m_nCardScroll < nMaxScroll )
			m_nCardScroll = min( nMaxScroll, m_nCardScroll + XML_UNION_CARD_SCROLL_PAGE );
		else if( CURSOR_ST.GetWheel() > 0 && m_nCardScroll > 0 )
			m_nCardScroll = max( 0, m_nCardScroll - XML_UNION_CARD_SCROLL_PAGE );

		if( nOldScroll != m_nCardScroll )
		{
			m_nHoverCard = -1;
			_UpdateCardControls();
			_UpdateHoverTooltipControls();
		}
		CURSOR_ST.ResetWheel();
		return muReturn;
	}

	CsRect const rcCardScroll( CsPoint( ptRoot.x + XML_UNION_CARD_SCROLL_X - 3, ptRoot.y + XML_UNION_CARD_SCROLL_Y ),
		CsSIZE( XML_UNION_CARD_SCROLL_W + 6, XML_UNION_CARD_SCROLL_H ) );
	if( CURSOR_ST.CheckClickBox( rcCardScroll ) == CURSOR::LBUTTON_UP && (int)m_vCards.size() > IF_XML_UNION_CARD_COUNT )
	{
		int const nMaxScroll = max( 1, ( ( (int)m_vCards.size() - 1 ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE );
		int const nRelativeY = max( 0, min( XML_UNION_CARD_SCROLL_TRACK_H, ptCursor.y - ( ptRoot.y + XML_UNION_CARD_SCROLL_TRACK_Y ) ) );
		int const nRawScroll = ( nRelativeY * nMaxScroll ) / XML_UNION_CARD_SCROLL_TRACK_H;
		int const nNewScroll = min( nMaxScroll, ( ( nRawScroll + ( XML_UNION_CARD_SCROLL_PAGE / 2 ) ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE );
		if( nNewScroll != m_nCardScroll )
		{
			m_nCardScroll = nNewScroll;
			m_nHoverCard = -1;
			_UpdateCardControls();
			_UpdateHoverTooltipControls();
		}
		return muReturn;
	}

	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL &&
		CURSOR_ST.IsInCursor( ptRoot.x + 8, ptRoot.y + 47, ptRoot.x + 215, ptRoot.y + IF_XML_UNION_HEIGHT - 23 ) )
	{
		int const nMaxScroll = max( 0, (int)m_vFilteredFilterIndices.size() - IF_XML_UNION_FILTER_COUNT );
		int const nOldScroll = m_nFilterScroll;
		if( CURSOR_ST.GetWheel() < 0 && m_nFilterScroll < nMaxScroll )
			++m_nFilterScroll;
		else if( CURSOR_ST.GetWheel() > 0 && m_nFilterScroll > 0 )
			--m_nFilterScroll;

		if( nOldScroll != m_nFilterScroll )
		{
			m_nHoverCard = -1;
			_UpdateVisibleFilters();
			_UpdateFilterRowControls();
			_UpdateHoverTooltipControls();
		}
		CURSOR_ST.ResetWheel();
		return muReturn;
	}

	CsRect const rcScroll( CsPoint( ptRoot.x + XML_UNION_FILTER_SCROLL_X - 3, ptRoot.y + XML_UNION_FILTER_SCROLL_Y ),
		CsSIZE( XML_UNION_FILTER_SCROLL_W + 6, XML_UNION_FILTER_SCROLL_H ) );
	if( CURSOR_ST.CheckClickBox( rcScroll ) == CURSOR::LBUTTON_UP && (int)m_vFilteredFilterIndices.size() > IF_XML_UNION_FILTER_COUNT )
	{
		CsPoint const ptCursor = CURSOR_ST.GetPos();
		int const nMaxScroll = max( 1, (int)m_vFilteredFilterIndices.size() - IF_XML_UNION_FILTER_COUNT );
		int const nRelativeY = max( 0, min( XML_UNION_FILTER_SCROLL_TRACK_H, ptCursor.y - ( ptRoot.y + XML_UNION_FILTER_SCROLL_TRACK_Y ) ) );
		int const nNewScroll = ( nRelativeY * nMaxScroll ) / XML_UNION_FILTER_SCROLL_TRACK_H;
		if( nNewScroll != m_nFilterScroll )
		{
			m_nFilterScroll = nNewScroll;
			m_nHoverCard = -1;
			_UpdateVisibleFilters();
			_UpdateFilterRowControls();
			_UpdateHoverTooltipControls();
		}
		return muReturn;
	}

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( IF_XML_UNION_WIDTH, 42 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
		_UpdateMoveWindow_ForMouse();

	return muReturn;
}

void cXmlUnionDUnit::Render()
{
	RenderScript();
	EndRenderScript();
}

void cXmlUnionDUnit::ResetDevice()
{
	ResetDeviceScript();
}

void cXmlUnionDUnit::OnMoveWindow()
{
	cBaseWindow::OnMoveWindow();
}

void cXmlUnionDUnit::PressCloseButton( void* pSender, void* pData )
{
	Close();
}

bool cXmlUnionDUnit::_FileExists( char const* pPath ) const
{
	if( pPath == NULL || pPath[ 0 ] == 0 )
		return false;

	static std::map< std::string, bool > s_mapExistsCache;
	std::string const strPath = pPath;
	std::map< std::string, bool >::const_iterator itCache = s_mapExistsCache.find( strPath );
	if( itCache != s_mapExistsCache.end() )
		return itCache->second;

	bool bExists = false;
	if( CsFPS::CsFPSystem::IsExistOnlyPack( 0, pPath ) || CsFPS::CsFPSystem::IsExist( pPath ) )
		bExists = true;

	if( bExists == false )
	{
		char szDataPath[ MAX_PATH ] = { 0, };
		strcpy_s( szDataPath, MAX_PATH, "Data\\" );
		strcat_s( szDataPath, MAX_PATH, pPath );
		if( CsFPS::CsFPSystem::IsExistOnlyPack( 0, szDataPath ) || CsFPS::CsFPSystem::IsExist( szDataPath ) )
			bExists = true;
	}

	if( bExists == false )
	{
		char szInterfacePath[ MAX_PATH ] = { 0, };
		strcpy_s( szInterfacePath, MAX_PATH, "Data\\Interface\\" );
		strcat_s( szInterfacePath, MAX_PATH, pPath );
		if( CsFPS::CsFPSystem::IsExistOnlyPack( 0, szInterfacePath ) || CsFPS::CsFPSystem::IsExist( szInterfacePath ) )
			bExists = true;
	}

	if( bExists == false )
	{
		char szShortInterfacePath[ MAX_PATH ] = { 0, };
		strcpy_s( szShortInterfacePath, MAX_PATH, "Interface\\" );
		strcat_s( szShortInterfacePath, MAX_PATH, pPath );
		bExists = CsFPS::CsFPSystem::IsExistOnlyPack( 0, szShortInterfacePath ) || CsFPS::CsFPSystem::IsExist( szShortInterfacePath );
	}

	s_mapExistsCache[ strPath ] = bExists;
	return bExists;
}

cSprite* cXmlUnionDUnit::_AddAssetSprite( CsPoint pos, CsPoint size, char const* pPath, NiColorA const& fallbackColor )
{
	if( _FileExists( pPath ) )
		return AddSprite( pos, size, pPath );

	return _AddSolidSprite( pos, size, fallbackColor );
}

cSprite* cXmlUnionDUnit::_AddAssetSpriteRect( CsPoint pos, CsPoint size, CsRect srcRect, char const* pPath, NiColorA const& fallbackColor )
{
	if( _FileExists( pPath ) )
	{
		cSprite* pSprite = NiNew cSprite;
		if( pSprite == NULL )
			return NULL;

		pSprite->Init( GetRoot(), pos, size, srcRect, pPath, false );
		AddChildControl( pSprite );
		return pSprite;
	}

	return _AddSolidSprite( pos, size, fallbackColor );
}

cSprite* cXmlUnionDUnit::_AddAssetImageState( CsPoint pos, CsPoint size, CsPoint texToken, int nState, char const* pPath, NiColorA const& fallbackColor )
{
	if( _FileExists( pPath ) )
	{
		cImage* pImage = NiNew cImage;
		if( pImage == NULL )
			return NULL;

		pImage->Init( GetRoot(), pos, size, pPath, false );
		pImage->SetTexToken( texToken );
		pImage->SetState( nState );
		AddChildControl( pImage );
		return pImage;
	}

	return _AddSolidSprite( pos, size, fallbackColor );
}

cSprite* cXmlUnionDUnit::_AddSolidSprite( CsPoint pos, CsPoint size, NiColorA const& color )
{
	cSprite* pSprite = NiNew cSprite;
	if( pSprite == NULL )
		return NULL;

	pSprite->Init( GetRoot(), pos, size, color, false );
	AddChildControl( pSprite );
	return pSprite;
}

cText* cXmlUnionDUnit::_AddLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign )
{
	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem );
	ti.s_eFontSize = eSize;
	ti.s_Color = color;
	ti.s_eTextAlign = nAlign;
	ti.SetText( pText );
	return AddText( &ti, pos );
}

cText* cXmlUnionDUnit::_AddBoldLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign )
{
	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem );
	ti.s_eFontSize = eSize;
	ti.s_Color = color;
	ti.s_eTextAlign = nAlign;
	ti.SetBoldLevel( cText::sTEXTINFO::BL_2 );
	ti.SetText( pText );
	return AddText( &ti, pos );
}

std::string cXmlUnionDUnit::_ResolveIconPath( cUnionXmlTools::sGROUP const& group ) const
{
	if( group.s_dwIconCandidateID == 0 || group.s_strIconFolder.empty() )
		return std::string();

	static std::map< std::string, std::string > s_mapIconPathCache;
	char szCacheKey[ MAX_PATH ] = { 0, };
	sprintf_s( szCacheKey, _countof( szCacheKey ), "%s:%lu", group.s_strIconFolder.c_str(), group.s_dwIconCandidateID );
	std::map< std::string, std::string >::const_iterator itCachedIcon = s_mapIconPathCache.find( szCacheKey );
	if( itCachedIcon != s_mapIconPathCache.end() )
		return itCachedIcon->second;

	char szId[ 32 ] = { 0, };
	sprintf_s( szId, _countof( szId ), "%07lu", group.s_dwIconCandidateID );

	char const* const kSuffixes[] =
	{
		"_evo.tga",
		"s.tga",
		"l.tga",
		".tga",
	};

	for( int i = 0; i < _countof( kSuffixes ); ++i )
	{
		char szPath[ MAX_PATH ] = { 0, };
		sprintf_s( szPath, _countof( szPath ), "Data\\Digimon\\%s\\%s_%s%s", group.s_strIconFolder.c_str(), szId, group.s_strIconFolder.c_str(), kSuffixes[ i ] );
		if( _FileExists( szPath ) )
		{
			s_mapIconPathCache[ szCacheKey ] = szPath;
			return szPath;
		}
	}

	if( g_pModelDataMng )
	{
		std::string strIcon = g_pModelDataMng->GetEvoModelIconFile( group.s_dwIconCandidateID );
		if( _FileExists( strIcon.c_str() ) )
		{
			s_mapIconPathCache[ szCacheKey ] = strIcon;
			return strIcon;
		}

		strIcon = g_pModelDataMng->GetSmallModelIconFile( group.s_dwIconCandidateID );
		if( _FileExists( strIcon.c_str() ) )
		{
			s_mapIconPathCache[ szCacheKey ] = strIcon;
			return strIcon;
		}
	}

	s_mapIconPathCache[ szCacheKey ] = std::string();
	return std::string();
}

void cXmlUnionDUnit::_BuildViewData()
{
	m_vCards.clear();
	m_vFilters.clear();
	m_vEffects.clear();
	m_vTotals.clear();
	m_vFilteredFilterIndices.clear();

	if( m_vAllGroups.empty() )
	{
		m_XmlData.Load( "Data\\union" );
		m_vAllGroups = m_XmlData.GetGroups();
		m_mapCardCache.clear();
		m_vAllFilters.clear();
		std::vector< cUnionXmlTools::sGROUP > const& allGroups = m_vAllGroups;
		for( size_t i = 0; i < allGroups.size(); ++i )
			m_vAllFilters.push_back( _MakeOptionText( allGroups[ i ].s_wsName ) );
		m_vTotals = m_XmlData.GetTotals();
	}
	else
	{
		m_vTotals = m_XmlData.GetTotals();
	}

	if( m_nSelectedFilter >= (int)m_vAllFilters.size() )
		m_nSelectedFilter = 0;

	for( size_t i = 0; i < m_vAllFilters.size(); ++i )
	{
		if( _ContainsNoCase( m_vAllFilters[ i ], m_wsSearchKeyword ) )
			m_vFilteredFilterIndices.push_back( (int)i );
	}

	if( m_vFilteredFilterIndices.empty() == false )
	{
		bool bSelectedVisible = false;
		for( size_t i = 0; i < m_vFilteredFilterIndices.size(); ++i )
		{
			if( m_vFilteredFilterIndices[ i ] == m_nSelectedFilter )
			{
				bSelectedVisible = true;
				break;
			}
		}
		if( bSelectedVisible == false )
			m_nSelectedFilter = m_vFilteredFilterIndices[ 0 ];
	}

	if( m_nFilterScroll > max( 0, (int)m_vFilteredFilterIndices.size() - IF_XML_UNION_FILTER_COUNT ) )
		m_nFilterScroll = max( 0, (int)m_vFilteredFilterIndices.size() - IF_XML_UNION_FILTER_COUNT );

	std::wstring const wsSelected = m_nSelectedFilter < (int)m_vAllFilters.size() ? m_vAllFilters[ m_nSelectedFilter ] : std::wstring();
	_BuildEvolutionCards( wsSelected, m_nSelectedFilter );
	if( m_nCardScroll > max( 0, ( ( (int)m_vCards.size() - 1 ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE ) )
		m_nCardScroll = max( 0, ( ( (int)m_vCards.size() - 1 ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE );

	if( m_vCards.empty() && m_wsSearchKeyword.empty() )
	{
		for( size_t i = 0; i < m_vAllGroups.size() && m_vCards.size() < IF_XML_UNION_CARD_COUNT; ++i )
		{
			sCARD_VIEW card;
			_FillCardFromGroup( card, m_vAllGroups[ i ] );
			m_vCards.push_back( card );
		}
	}

	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		int const nFilteredIndex = m_nFilterScroll + i;
		if( nFilteredIndex >= 0 && nFilteredIndex < (int)m_vFilteredFilterIndices.size() )
			m_vFilters.push_back( m_vAllFilters[ m_vFilteredFilterIndices[ nFilteredIndex ] ] );
	}

	if( m_nSelectedFilter >= 0 && m_nSelectedFilter < (int)m_vAllGroups.size() )
	{
		cUnionXmlTools::sGROUP const& selectedGroup = m_vAllGroups[ m_nSelectedFilter ];
		for( size_t r = 0; r < selectedGroup.s_vRewards.size() && m_vEffects.size() < IF_XML_UNION_EFFECT_COUNT; ++r )
			m_vEffects.push_back( _FormatReward( selectedGroup.s_vRewards[ r ] ) );
	}
}

void cXmlUnionDUnit::_RefreshView()
{
	m_nHoverCard = -1;
	_BuildViewData();
	_UpdateFilterRowControls();
	_UpdateTabControls();
	_UpdateEffectControls();
	_UpdateCardControls();
	_UpdateHoverTooltipControls();
}

void cXmlUnionDUnit::_FillCardFromGroup( sCARD_VIEW& card, cUnionXmlTools::sGROUP const& group ) const
{
	card.s_dwGroupID = group.s_dwGroupID;
	card.s_dwDigimonID = 0;
	card.s_dwIconCandidateID = group.s_dwIconCandidateID;
	card.s_nLevel = group.s_nLevel;
	card.s_nRank = group.s_nRank;
	card.s_wsName = group.s_wsName;
	card.s_wsOptionName = _MakeOptionText( group.s_wsName );
	card.s_strIconFolder = group.s_strIconFolder;
	card.s_strIconPath = _ResolveIconPath( group );
}

bool cXmlUnionDUnit::_FillCardFromDigimon( sCARD_VIEW& card, DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pGroup ) const
{
	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return false;

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return false;

	CsDigimon::sINFO* pInfo = pDigimon->GetInfo();
	card.s_dwDigimonID = dwDigimonID;
	card.s_dwGroupID = pGroup ? pGroup->s_dwGroupID : 0;
	card.s_dwIconCandidateID = pInfo->s_dwModelID;
	card.s_nLevel = pGroup ? pGroup->s_nLevel : 0;
	card.s_nRank = pInfo->s_nDigimonRank;
	card.s_wsName = pInfo->s_szName;
	card.s_wsOptionName = pGroup ? _MakeOptionText( pGroup->s_wsName ) : card.s_wsName;
	card.s_strIconFolder = pGroup ? pGroup->s_strIconFolder : std::string();
	card.s_strIconPath.clear();

	if( g_pModelDataMng )
	{
		card.s_strIconPath = g_pModelDataMng->GetEvoModelIconFile( pInfo->s_dwModelID );
		if( _FileExists( card.s_strIconPath.c_str() ) == false )
			card.s_strIconPath = g_pModelDataMng->GetSmallModelIconFile( pInfo->s_dwModelID );
	}

	return true;
}

bool cXmlUnionDUnit::_FillCardFromMember( sCARD_VIEW& card, cUnionXmlTools::sMEMBER const& member, cUnionXmlTools::sGROUP const& group ) const
{
	DWORD const dwDigimonID = _FindDigimonByMember( member );
	if( dwDigimonID != 0 && _FillCardFromDigimon( card, dwDigimonID, &group ) )
	{
		if( member.s_strIconFolder.empty() == false )
			card.s_strIconFolder = member.s_strIconFolder;
		if( member.s_dwIconCandidateID != 0 )
			card.s_dwIconCandidateID = member.s_dwIconCandidateID;
		if( member.s_wsName.empty() == false )
			card.s_wsName = member.s_wsName;

		cUnionXmlTools::sGROUP iconGroup = group;
		iconGroup.s_dwIconCandidateID = member.s_dwIconCandidateID != 0 ? member.s_dwIconCandidateID : card.s_dwIconCandidateID;
		iconGroup.s_strIconFolder = card.s_strIconFolder;
		std::string const strMemberIcon = _ResolveIconPath( iconGroup );
		if( strMemberIcon.empty() == false )
			card.s_strIconPath = strMemberIcon;
		return true;
	}

	card.s_dwGroupID = group.s_dwGroupID;
	card.s_dwDigimonID = 0;
	card.s_dwIconCandidateID = member.s_dwIconCandidateID;
	card.s_nLevel = group.s_nLevel;
	card.s_nRank = group.s_nRank;
	card.s_wsName = member.s_wsName;
	card.s_wsOptionName = _MakeOptionText( group.s_wsName );
	card.s_strIconFolder = member.s_strIconFolder.empty() ? group.s_strIconFolder : member.s_strIconFolder;
	card.s_strIconPath.clear();

	if( g_pModelDataMng && card.s_dwIconCandidateID != 0 )
	{
		card.s_strIconPath = g_pModelDataMng->GetEvoModelIconFile( card.s_dwIconCandidateID );
		if( _FileExists( card.s_strIconPath.c_str() ) == false )
			card.s_strIconPath = g_pModelDataMng->GetSmallModelIconFile( card.s_dwIconCandidateID );
	}

	if( card.s_strIconPath.empty() )
	{
		cUnionXmlTools::sGROUP iconGroup = group;
		iconGroup.s_dwIconCandidateID = member.s_dwIconCandidateID;
		iconGroup.s_strIconFolder = card.s_strIconFolder;
		card.s_strIconPath = _ResolveIconPath( iconGroup );
	}

	return card.s_wsName.empty() == false;
}

DWORD cXmlUnionDUnit::_FindDigimonByName( std::wstring const& wsName ) const
{
	if( nsCsFileTable::g_pDigimonMng == NULL || wsName.empty() )
		return 0;

	CsDigimon::MAP* pMap = nsCsFileTable::g_pDigimonMng->GetDigimonMap();
	if( pMap == NULL )
		return 0;

	std::wstring const wsNeedle = _NormalizeDigimonName( wsName );
	if( wsNeedle.empty() )
		return 0;

	static std::map< std::wstring, DWORD > s_mapDigimonNameCache;
	std::map< std::wstring, DWORD >::const_iterator itCache = s_mapDigimonNameCache.find( wsNeedle );
	if( itCache != s_mapDigimonNameCache.end() )
		return itCache->second;

	DWORD dwLooseMatch = 0;
	for( CsDigimon::MAP_CIT it = pMap->begin(); it != pMap->end(); ++it )
	{
		if( it->second == NULL || it->second->GetInfo() == NULL )
			continue;

		std::wstring const wsDigimonName = it->second->GetInfo()->s_szName;
		std::wstring const wsHaystack = _NormalizeDigimonName( wsDigimonName );
		if( wsHaystack.empty() )
			continue;

		if( wsHaystack == wsNeedle )
		{
			s_mapDigimonNameCache[ wsNeedle ] = it->first;
			return it->first;
		}

		if( dwLooseMatch == 0 && wsHaystack.find( wsNeedle ) != std::wstring::npos )
			dwLooseMatch = it->first;
	}

	s_mapDigimonNameCache[ wsNeedle ] = dwLooseMatch;
	return dwLooseMatch;
}

DWORD cXmlUnionDUnit::_FindDigimonByMember( cUnionXmlTools::sMEMBER const& member ) const
{
	if( nsCsFileTable::g_pDigimonMng == NULL )
		return 0;

	CsDigimon::MAP* pMap = nsCsFileTable::g_pDigimonMng->GetDigimonMap();
	if( pMap == NULL )
		return 0;

	if( member.s_dwIconCandidateID != 0 )
	{
		for( CsDigimon::MAP_CIT it = pMap->begin(); it != pMap->end(); ++it )
		{
			if( it->second == NULL || it->second->GetInfo() == NULL )
				continue;

			if( it->second->GetInfo()->s_dwModelID == member.s_dwIconCandidateID )
				return it->first;
		}
	}

	if( member.s_wsName.empty() )
		return 0;

	DWORD dwByName = _FindDigimonByName( member.s_wsName );
	if( dwByName != 0 )
		return dwByName;

	std::wstring wsCleanName = member.s_wsName;
	size_t nPos = wsCleanName.find( L"?" );
	if( nPos != std::wstring::npos )
		wsCleanName.erase( nPos );
	nPos = wsCleanName.find( L"(" );
	if( nPos != std::wstring::npos )
		wsCleanName.erase( nPos );
	while( wsCleanName.empty() == false && wsCleanName[ wsCleanName.size() - 1 ] == L' ' )
		wsCleanName.erase( wsCleanName.size() - 1 );

	return wsCleanName == member.s_wsName ? 0 : _FindDigimonByName( wsCleanName );
}

DWORD cXmlUnionDUnit::_FindEvolutionBaseByName( std::wstring const& wsName ) const
{
	if( nsCsFileTable::g_pEvolMng == NULL || wsName.empty() )
		return 0;

	CDigimonEvolveInfo::MAP* pEvolMap = nsCsFileTable::g_pEvolMng->GetMap();
	if( pEvolMap == NULL )
		return 0;

	std::wstring const wsNeedle = _NormalizeDigimonName( wsName );
	if( wsNeedle.empty() )
		return 0;

	static std::map< std::wstring, DWORD > s_mapEvolutionNameCache;
	std::map< std::wstring, DWORD >::const_iterator itCache = s_mapEvolutionNameCache.find( wsNeedle );
	if( itCache != s_mapEvolutionNameCache.end() )
		return itCache->second;

	DWORD dwLooseMatch = 0;
	for( CDigimonEvolveInfo::MAP_IT it = pEvolMap->begin(); it != pEvolMap->end(); ++it )
	{
		if( it->second == NULL || it->second->GetInfo() == NULL )
			continue;

		DWORD const dwBaseID = it->second->GetInfo()->s_nDigimonID;
		std::vector< std::wstring > vNames;

		if( nsCsFileTable::g_pTacticsMng && nsCsFileTable::g_pTacticsMng->IsTacticsExplain( dwBaseID ) )
		{
			CsTacticsExplain* pTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( dwBaseID );
			if( pTactics && pTactics->GetInfo() )
				vNames.push_back( pTactics->GetInfo()->s_szTacticsName );
		}

		if( nsCsFileTable::g_pDigimonMng )
		{
			CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwBaseID );
			if( pDigimon && pDigimon->GetInfo() )
				vNames.push_back( pDigimon->GetInfo()->s_szName );
		}

		for( size_t nName = 0; nName < vNames.size(); ++nName )
		{
			std::wstring const wsHaystack = _NormalizeDigimonName( vNames[ nName ] );
			if( wsHaystack.empty() )
				continue;

			if( wsHaystack == wsNeedle )
			{
				s_mapEvolutionNameCache[ wsNeedle ] = dwBaseID;
				return dwBaseID;
			}

			if( dwLooseMatch == 0 && wsHaystack.find( wsNeedle ) != std::wstring::npos )
				dwLooseMatch = dwBaseID;
		}
	}

	s_mapEvolutionNameCache[ wsNeedle ] = dwLooseMatch;
	return dwLooseMatch;
}

void cXmlUnionDUnit::_CollectEvolutionBaseCandidates( std::wstring const& wsName, std::vector< DWORD >& outCandidates ) const
{
	outCandidates.clear();

	if( nsCsFileTable::g_pEvolMng == NULL || wsName.empty() )
		return;

	CDigimonEvolveInfo::MAP* pEvolMap = nsCsFileTable::g_pEvolMng->GetMap();
	if( pEvolMap == NULL )
		return;

	std::wstring const wsNeedle = _NormalizeDigimonName( wsName );
	if( wsNeedle.empty() )
		return;

	std::vector< DWORD > vClassic;
	std::vector< DWORD > vExact;
	std::vector< DWORD > vLoose;

	for( CDigimonEvolveInfo::MAP_IT it = pEvolMap->begin(); it != pEvolMap->end(); ++it )
	{
		if( it->second == NULL || it->second->GetInfo() == NULL )
			continue;

		DWORD const dwBaseID = it->second->GetInfo()->s_nDigimonID;
		std::vector< std::wstring > vNames;

		if( nsCsFileTable::g_pDigimonMng )
		{
			CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwBaseID );
			if( pDigimon && pDigimon->GetInfo() )
				vNames.push_back( pDigimon->GetInfo()->s_szName );
		}

		if( nsCsFileTable::g_pTacticsMng && nsCsFileTable::g_pTacticsMng->IsTacticsExplain( dwBaseID ) )
		{
			CsTacticsExplain* pTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( dwBaseID );
			if( pTactics && pTactics->GetInfo() )
				vNames.push_back( pTactics->GetInfo()->s_szTacticsName );
		}

		bool bClassicAgumon = false;
		bool bExact = false;
		bool bLoose = false;
		for( size_t nName = 0; nName < vNames.size(); ++nName )
		{
			std::wstring const wsHaystack = _NormalizeDigimonName( vNames[ nName ] );
			if( wsHaystack.empty() )
				continue;

			if( wsNeedle == L"agumon" && wsHaystack == L"agumonclassic" )
			{
				bClassicAgumon = true;
				continue;
			}

			if( wsHaystack == wsNeedle )
			{
				bExact = true;
				continue;
			}

			bool const bSpecificVariant =
				wsNeedle.find( L"blitz" ) != std::wstring::npos ||
				wsNeedle.find( L"cresgaruru" ) != std::wstring::npos ||
				wsNeedle.find( L"millennium" ) != std::wstring::npos ||
				wsNeedle.find( L"toyagumonss" ) != std::wstring::npos;

			if( bSpecificVariant && ( wsHaystack.find( wsNeedle ) != std::wstring::npos || wsNeedle.find( wsHaystack ) != std::wstring::npos ) )
				bLoose = true;
		}

		if( bClassicAgumon )
			_PushUniqueDigimon( vClassic, dwBaseID );
		else if( bExact )
			_PushUniqueDigimon( vExact, dwBaseID );
		else if( bLoose )
			_PushUniqueDigimon( vLoose, dwBaseID );
	}

	for( size_t i = 0; i < vClassic.size(); ++i )
		_PushUniqueDigimon( outCandidates, vClassic[ i ] );
	for( size_t i = 0; i < vExact.size(); ++i )
		_PushUniqueDigimon( outCandidates, vExact[ i ] );
	for( size_t i = 0; i < vLoose.size(); ++i )
		_PushUniqueDigimon( outCandidates, vLoose[ i ] );
}

int cXmlUnionDUnit::_GetDuplicateGroupIndex( int nSelectedGroupIndex ) const
{
	if( nSelectedGroupIndex < 0 || nSelectedGroupIndex >= (int)m_vAllGroups.size() )
		return 0;

	std::wstring const wsSelected = _MakeOptionText( m_vAllGroups[ nSelectedGroupIndex ].s_wsName );
	int nDuplicateIndex = 0;
	for( int i = 0; i < nSelectedGroupIndex && i < (int)m_vAllGroups.size(); ++i )
	{
		if( _MakeOptionText( m_vAllGroups[ i ].s_wsName ) == wsSelected )
			++nDuplicateIndex;
	}
	return nDuplicateIndex;
}

DWORD cXmlUnionDUnit::_FindEvolutionBaseForGroup( std::wstring const& wsName, cUnionXmlTools::sGROUP const* pSelectedGroup, int nSelectedGroupIndex ) const
{
	if( _IsSelectedGroupXLine( pSelectedGroup ) && pSelectedGroup->s_dwIconCandidateID != 0 && nsCsFileTable::g_pDigimonMng )
	{
		CsDigimon::MAP* pMap = nsCsFileTable::g_pDigimonMng->GetDigimonMap();
		if( pMap )
		{
			for( CsDigimon::MAP_CIT it = pMap->begin(); it != pMap->end(); ++it )
			{
				if( it->second == NULL || it->second->GetInfo() == NULL )
					continue;
				if( it->second->GetInfo()->s_dwModelID == pSelectedGroup->s_dwIconCandidateID )
					return it->first;
			}
		}
	}

	std::vector< DWORD > vCandidates;
	_CollectEvolutionBaseCandidates( wsName, vCandidates );
	if( vCandidates.empty() && pSelectedGroup )
		_CollectEvolutionBaseCandidates( pSelectedGroup->s_wsName, vCandidates );

	if( vCandidates.empty() == false )
	{
		int const nDuplicateIndex = _GetDuplicateGroupIndex( nSelectedGroupIndex );
		size_t const nCandidateIndex = min( (size_t)max( nDuplicateIndex, 0 ), vCandidates.size() - 1 );
		return vCandidates[ nCandidateIndex ];
	}

	DWORD dwBaseDigimonID = _FindEvolutionBaseByName( wsName );
	if( dwBaseDigimonID == 0 )
		dwBaseDigimonID = _FindDigimonByName( wsName );
	if( dwBaseDigimonID == 0 && pSelectedGroup )
		dwBaseDigimonID = _FindEvolutionBaseByName( pSelectedGroup->s_wsName );
	if( dwBaseDigimonID == 0 && pSelectedGroup )
		dwBaseDigimonID = _FindDigimonByName( pSelectedGroup->s_wsName );

	return dwBaseDigimonID;
}

bool cXmlUnionDUnit::_IsXAntibodyEvolution( DWORD dwDigimonID ) const
{
	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return false;

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return false;

#ifdef SDM_TAMER_XGUAGE_20180628
	if( pDigimon->GetInfo()->s_eEvolutionType >= nsCsDigimonTable::ET_X_ROOKIE &&
		pDigimon->GetInfo()->s_eEvolutionType <= nsCsDigimonTable::ET_X_JOGRESS )
	{
		return true;
	}
#endif

	std::wstring const wsName = _ToLower( pDigimon->GetInfo()->s_szName );
	if( wsName.size() >= 2 && wsName.compare( wsName.size() - 2, 2, L" x" ) == 0 )
		return true;
	if( wsName.find( L" x(" ) != std::wstring::npos || wsName.find( L" x[" ) != std::wstring::npos )
		return true;

	return false;
}

bool cXmlUnionDUnit::_IsSelectedGroupXLine( cUnionXmlTools::sGROUP const* pSelectedGroup ) const
{
	if( pSelectedGroup == NULL )
		return false;

	std::wstring const wsType = _ToLower( pSelectedGroup->s_wsType );
	if( wsType != L"line" )
		return false;

	std::wstring const wsName = _ToLower( _MakeOptionText( pSelectedGroup->s_wsName ) );
	if( wsName.size() >= 2 && wsName.compare( wsName.size() - 2, 2, L" x" ) == 0 )
		return true;
	if( wsName.find( L" x " ) != std::wstring::npos || wsName.find( L" x[" ) != std::wstring::npos || wsName.find( L" x(" ) != std::wstring::npos )
		return true;

	std::wstring const wsNormalized = _NormalizeDigimonName( wsName );
	if( wsNormalized.find( L"xanti" ) != std::wstring::npos || wsNormalized.find( L"antix" ) != std::wstring::npos )
		return true;

	return false;
}

bool cXmlUnionDUnit::_IsDigimonExplicitForSelectedGroup( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const
{
	if( dwDigimonID == 0 || pSelectedGroup == NULL )
		return false;

	if( _IsDigimonMatchedToUnionName( dwDigimonID, _MakeOptionText( pSelectedGroup->s_wsName ) ) )
		return true;

	for( size_t i = 0; i < pSelectedGroup->s_vMembers.size(); ++i )
	{
		if( _IsDigimonMatchedToUnionName( dwDigimonID, pSelectedGroup->s_vMembers[ i ].s_wsName ) )
			return true;
	}

	return false;
}

bool cXmlUnionDUnit::_IsLineEvolutionAllowed( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const
{
	(void)pSelectedGroup;

	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return false;

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return false;

	return true;
}

CDigimonEvolveInfo* cXmlUnionDUnit::_FindEvolveInfoForDigimon( DWORD dwDigimonID, USHORT* pOutRootSlot ) const
{
	if( pOutRootSlot )
		*pOutRootSlot = 0;
	if( dwDigimonID == 0 || nsCsFileTable::g_pEvolMng == NULL )
		return NULL;

	if( nsCsFileTable::g_pEvolMng->IsEvolveInfo( dwDigimonID ) )
	{
		CDigimonEvolveInfo* pInfo = nsCsFileTable::g_pEvolMng->GetEvolveInfo( dwDigimonID );
		CDigimonEvolveObj* pObj = pInfo ? pInfo->GetEvolveObjByID( dwDigimonID ) : NULL;
		if( pObj == NULL && pInfo )
			pObj = pInfo->GetEvolveObjByEvoSlot( 1 );
		if( pOutRootSlot && pObj )
			*pOutRootSlot = pObj->m_nEvoSlot;
		return pInfo;
	}

	CDigimonEvolveInfo::MAP* pMap = nsCsFileTable::g_pEvolMng->GetMap();
	if( pMap == NULL )
		return NULL;

	for( CDigimonEvolveInfo::MAP_IT it = pMap->begin(); it != pMap->end(); ++it )
	{
		if( it->second == NULL || it->second->IsEvolveObjByID( dwDigimonID ) == false )
			continue;

		CDigimonEvolveObj* pObj = it->second->GetEvolveObjByID( dwDigimonID );
		if( pOutRootSlot && pObj )
			*pOutRootSlot = pObj->m_nEvoSlot;
		return it->second;
	}

	return NULL;
}

bool cXmlUnionDUnit::_CanReachEvolutionSlot( CDigimonEvolveInfo* pEvolveInfo, USHORT nRootSlot, USHORT nTargetSlot ) const
{
	if( pEvolveInfo == NULL || nRootSlot == 0 || nTargetSlot == 0 )
		return false;
	if( nRootSlot == nTargetSlot )
		return true;

	std::vector< USHORT > vStack;
	std::vector< USHORT > vVisited;
	vStack.push_back( nRootSlot );

	while( vStack.empty() == false )
	{
		USHORT const nSlot = vStack.back();
		vStack.pop_back();

		bool bVisited = false;
		for( size_t i = 0; i < vVisited.size(); ++i )
		{
			if( vVisited[ i ] == nSlot )
			{
				bVisited = true;
				break;
			}
		}
		if( bVisited )
			continue;
		vVisited.push_back( nSlot );

		CDigimonEvolveObj* pObj = pEvolveInfo->GetEvolveObjByEvoSlot( nSlot );
		if( pObj == NULL )
			continue;

		for( int i = 0; i < MAX_EVOLUTION; ++i )
		{
			USHORT const nNextSlot = pObj->m_nEvolutionList[ i ].nSlot;
			if( nNextSlot <= 1 || nNextSlot >= 64 )
				continue;

			if( nNextSlot == nTargetSlot )
				return true;

			vStack.push_back( nNextSlot );
		}
	}

	return false;
}

void cXmlUnionDUnit::_CollectEvolutionBranch( CDigimonEvolveInfo* pEvolveInfo, USHORT nRootSlot, cUnionXmlTools::sGROUP const* pSelectedGroup, std::vector< DWORD >& outDigimonIDs, size_t nCardLimit ) const
{
	if( pEvolveInfo == NULL || nRootSlot == 0 )
		return;

	std::vector< USHORT > vStack;
	std::vector< USHORT > vVisited;
	vStack.push_back( nRootSlot );

	while( vStack.empty() == false && outDigimonIDs.size() < nCardLimit )
	{
		USHORT const nSlot = vStack.front();
		vStack.erase( vStack.begin() );

		bool bVisited = false;
		for( size_t i = 0; i < vVisited.size(); ++i )
		{
			if( vVisited[ i ] == nSlot )
			{
				bVisited = true;
				break;
			}
		}
		if( bVisited )
			continue;
		vVisited.push_back( nSlot );

		CDigimonEvolveObj* pObj = pEvolveInfo->GetEvolveObjByEvoSlot( nSlot );
		if( pObj == NULL )
			continue;

		if( _IsLineEvolutionAllowed( pObj->m_dwID, pSelectedGroup ) )
			_PushUniqueDigimon( outDigimonIDs, pObj->m_dwID );

		for( int i = 0; i < MAX_EVOLUTION; ++i )
		{
			USHORT const nNextSlot = pObj->m_nEvolutionList[ i ].nSlot;
			DWORD const dwNextDigimonID = pObj->m_nEvolutionList[ i ].dwDigimonID;
			if( nNextSlot <= 1 || nNextSlot >= 64 || dwNextDigimonID == 0 )
				continue;
			if( _IsLineEvolutionAllowed( dwNextDigimonID, pSelectedGroup ) == false )
				continue;

			vStack.push_back( nNextSlot );
		}
	}
}

void cXmlUnionDUnit::_BuildLineEvolutionIDs( DWORD dwBaseDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup, int nSelectedGroupIndex, size_t nCardLimit, std::vector< DWORD >& outDigimonIDs ) const
{
	(void)nSelectedGroupIndex;

	if( dwBaseDigimonID == 0 )
		return;

	if( _IsLineEvolutionAllowed( dwBaseDigimonID, pSelectedGroup ) )
		_PushUniqueDigimon( outDigimonIDs, dwBaseDigimonID );

	USHORT nBaseSlot = 0;
	CDigimonEvolveInfo* pEvolveInfo = _FindEvolveInfoForDigimon( dwBaseDigimonID, &nBaseSlot );
	if( pEvolveInfo == NULL )
		return;

	CDigimonEvolveObj* pBaseObj = pEvolveInfo->GetEvolveObjByID( dwBaseDigimonID );
	if( pBaseObj == NULL && nBaseSlot != 0 )
		pBaseObj = pEvolveInfo->GetEvolveObjByEvoSlot( nBaseSlot );
	if( pBaseObj == NULL )
		pBaseObj = pEvolveInfo->GetEvolveObjByEvoSlot( 1 );

	std::vector< USHORT > vDirectSlots;
	if( pBaseObj )
	{
		for( int i = 0; i < MAX_EVOLUTION; ++i )
		{
			USHORT const nSlot = pBaseObj->m_nEvolutionList[ i ].nSlot;
			DWORD const dwDigimonID = pBaseObj->m_nEvolutionList[ i ].dwDigimonID;
			if( nSlot <= 1 || nSlot >= 64 || dwDigimonID == 0 )
				continue;
			if( _IsLineEvolutionAllowed( dwDigimonID, pSelectedGroup ) == false )
				continue;

			bool bExists = false;
			for( size_t n = 0; n < vDirectSlots.size(); ++n )
			{
				if( vDirectSlots[ n ] == nSlot )
				{
					bExists = true;
					break;
				}
			}
			if( bExists == false )
				vDirectSlots.push_back( nSlot );
		}
	}

	std::vector< USHORT > vRootSlots;
	for( size_t i = 0; i < vDirectSlots.size(); ++i )
	{
		bool bIsChildOfAnotherRoot = false;
		for( size_t n = 0; n < vDirectSlots.size(); ++n )
		{
			if( i == n )
				continue;

			if( _CanReachEvolutionSlot( pEvolveInfo, vDirectSlots[ n ], vDirectSlots[ i ] ) )
			{
				bIsChildOfAnotherRoot = true;
				break;
			}
		}

		if( bIsChildOfAnotherRoot == false )
			vRootSlots.push_back( vDirectSlots[ i ] );
	}

	if( vRootSlots.empty() == false )
	{
		for( size_t i = 0; i < vRootSlots.size() && outDigimonIDs.size() < nCardLimit; ++i )
			_CollectEvolutionBranch( pEvolveInfo, vRootSlots[ i ], pSelectedGroup, outDigimonIDs, nCardLimit );
		return;
	}

	for( int nSlot = 1; nSlot < 64 && outDigimonIDs.size() < nCardLimit; ++nSlot )
	{
		CDigimonEvolveObj* pObj = pEvolveInfo->GetEvolveObjByEvoSlot( nSlot );
		if( pObj && _IsLineEvolutionAllowed( pObj->m_dwID, pSelectedGroup ) )
			_PushUniqueDigimon( outDigimonIDs, pObj->m_dwID );
	}
}

void cXmlUnionDUnit::_AppendRelatedXLineEvolutionIDs( cUnionXmlTools::sGROUP const* pSelectedGroup, size_t nCardLimit, std::vector< DWORD >& outDigimonIDs ) const
{
	if( pSelectedGroup == NULL || _IsSelectedGroupXLine( pSelectedGroup ) || outDigimonIDs.size() >= nCardLimit )
		return;

	std::wstring const wsSelectedBase = _GetBaseNameBeforeX( _MakeOptionText( pSelectedGroup->s_wsName ) );
	if( wsSelectedBase.empty() )
		return;

	for( size_t i = 0; i < m_vAllGroups.size() && outDigimonIDs.size() < nCardLimit; ++i )
	{
		cUnionXmlTools::sGROUP const& group = m_vAllGroups[ i ];
		if( &group == pSelectedGroup )
			continue;
		if( _ToLower( group.s_wsType ) != L"line" || _IsSelectedGroupXLine( &group ) == false )
			continue;

		std::wstring const wsXBase = _GetBaseNameBeforeX( _MakeOptionText( group.s_wsName ) );
		if( wsXBase.empty() || wsXBase != wsSelectedBase )
			continue;

		DWORD const dwXBaseDigimonID = _FindEvolutionBaseForGroup( _MakeOptionText( group.s_wsName ), &group, (int)i );
		if( dwXBaseDigimonID == 0 )
			continue;

		size_t const nBeforeAppend = outDigimonIDs.size();
		_BuildLineEvolutionIDs( dwXBaseDigimonID, &group, (int)i, nCardLimit, outDigimonIDs );
		if( outDigimonIDs.size() == nBeforeAppend && _IsLineEvolutionAllowed( dwXBaseDigimonID, &group ) )
			_PushUniqueDigimon( outDigimonIDs, dwXBaseDigimonID );
	}
}

bool cXmlUnionDUnit::_IsDigimonMatchedToUnionName( DWORD dwDigimonID, std::wstring const& wsUnionName ) const
{
	if( dwDigimonID == 0 || wsUnionName.empty() )
		return false;

	std::wstring const wsUnionNeedle = _NormalizeDigimonName( wsUnionName );
	if( wsUnionNeedle.empty() )
		return false;

	std::vector< std::wstring > vCandidateNames;
	if( nsCsFileTable::g_pDigimonMng )
	{
		CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
		if( pDigimon && pDigimon->GetInfo() )
			vCandidateNames.push_back( pDigimon->GetInfo()->s_szName );
	}

	if( nsCsFileTable::g_pTacticsMng && nsCsFileTable::g_pTacticsMng->IsTacticsExplain( dwDigimonID ) )
	{
		CsTacticsExplain* pTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( dwDigimonID );
		if( pTactics && pTactics->GetInfo() )
			vCandidateNames.push_back( pTactics->GetInfo()->s_szTacticsName );
	}

	for( size_t i = 0; i < vCandidateNames.size(); ++i )
	{
		if( _NormalizeDigimonName( vCandidateNames[ i ] ) == wsUnionNeedle )
			return true;
	}

	return false;
}

bool cXmlUnionDUnit::_IsDigimonDeclaredInUnionXml( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const
{
	if( dwDigimonID == 0 || nsCsFileTable::g_pDigimonMng == NULL )
		return false;

	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwDigimonID );
	if( pDigimon == NULL || pDigimon->GetInfo() == NULL )
		return false;

	DWORD const dwModelID = pDigimon->GetInfo()->s_dwModelID;

	if( pSelectedGroup && _IsDigimonMatchedToUnionName( dwDigimonID, _MakeOptionText( pSelectedGroup->s_wsName ) ) )
		return true;

	for( size_t i = 0; i < m_vAllGroups.size(); ++i )
	{
		cUnionXmlTools::sGROUP const& group = m_vAllGroups[ i ];
		if( _IsDigimonMatchedToUnionName( dwDigimonID, _MakeOptionText( group.s_wsName ) ) )
			return true;

		for( size_t nMember = 0; nMember < group.s_vMembers.size(); ++nMember )
		{
			cUnionXmlTools::sMEMBER const& member = group.s_vMembers[ nMember ];
			if( _IsDigimonMatchedToUnionName( dwDigimonID, member.s_wsName ) )
				return true;
			if( member.s_dwIconCandidateID != 0 && member.s_dwIconCandidateID == dwModelID )
				return true;
		}
	}

	return false;
}

void cXmlUnionDUnit::_BuildEvolutionCards( std::wstring const& wsSelected, int nSelectedGroupIndex )
{
	wchar_t szCacheKey[ 256 ] = { 0, };
	if( nSelectedGroupIndex >= 0 && nSelectedGroupIndex < (int)m_vAllGroups.size() )
		swprintf_s( szCacheKey, _countof( szCacheKey ), L"%u:%s", m_vAllGroups[ nSelectedGroupIndex ].s_dwGroupID, wsSelected.c_str() );
	else
		swprintf_s( szCacheKey, _countof( szCacheKey ), L"0:%s", wsSelected.c_str() );

	std::wstring const wsCacheKey = szCacheKey;
	std::map< std::wstring, std::vector< sCARD_VIEW > >::const_iterator itCardCache = m_mapCardCache.find( wsCacheKey );
	if( itCardCache != m_mapCardCache.end() )
	{
		m_vCards = itCardCache->second;
		return;
	}

	cUnionXmlTools::sGROUP const* pSelectedGroup = NULL;
	if( nSelectedGroupIndex >= 0 && nSelectedGroupIndex < (int)m_vAllGroups.size() )
	{
		pSelectedGroup = &m_vAllGroups[ nSelectedGroupIndex ];
	}
	else
	{
		for( size_t i = 0; i < m_vAllGroups.size(); ++i )
		{
			if( _IsSameOption( _MakeOptionText( m_vAllGroups[ i ].s_wsName ), wsSelected ) )
			{
				pSelectedGroup = &m_vAllGroups[ i ];
				break;
			}
		}
	}

	if( pSelectedGroup && pSelectedGroup->s_vMembers.empty() == false )
	{
		size_t const nMemberCardLimit = min( (size_t)IF_XML_UNION_MAX_CARD_DATA, pSelectedGroup->s_vMembers.size() );
		for( size_t i = 0; i < pSelectedGroup->s_vMembers.size() && m_vCards.size() < nMemberCardLimit; ++i )
		{
			sCARD_VIEW card;
			if( _FillCardFromMember( card, pSelectedGroup->s_vMembers[ i ], *pSelectedGroup ) )
				m_vCards.push_back( card );
		}

		m_mapCardCache[ wsCacheKey ] = m_vCards;
		return;
	}

	DWORD dwBaseDigimonID = _FindEvolutionBaseForGroup( wsSelected, pSelectedGroup, nSelectedGroupIndex );
	std::vector< DWORD > vDigimonIDs;
	bool const bLineGroup = pSelectedGroup && _ToLower( pSelectedGroup->s_wsType ) == L"line";
	size_t const nCardLimit = bLineGroup ? (size_t)IF_XML_UNION_MAX_CARD_DATA : ( pSelectedGroup && pSelectedGroup->s_nMemberCount > 0 ? min( (size_t)IF_XML_UNION_MAX_CARD_DATA, max( (size_t)IF_XML_UNION_CARD_COUNT, (size_t)pSelectedGroup->s_nMemberCount ) ) : (size_t)IF_XML_UNION_MAX_CARD_DATA );
	if( bLineGroup )
	{
		_BuildLineEvolutionIDs( dwBaseDigimonID, pSelectedGroup, nSelectedGroupIndex, nCardLimit, vDigimonIDs );
		_AppendRelatedXLineEvolutionIDs( pSelectedGroup, nCardLimit, vDigimonIDs );
	}
	else
	{
		if( _IsDigimonDeclaredInUnionXml( dwBaseDigimonID, pSelectedGroup ) )
			_PushUniqueDigimon( vDigimonIDs, dwBaseDigimonID );

		if( dwBaseDigimonID != 0 && nsCsFileTable::g_pEvolMng && nsCsFileTable::g_pEvolMng->IsEvolveInfo( dwBaseDigimonID ) )
		{
			CDigimonEvolveInfo* pEvolveInfo = nsCsFileTable::g_pEvolMng->GetEvolveInfo( dwBaseDigimonID );
			if( pEvolveInfo )
			{
				for( int nSlot = 1; nSlot < 32 && vDigimonIDs.size() < nCardLimit; ++nSlot )
				{
					CDigimonEvolveObj* pObj = pEvolveInfo->GetEvolveObjByEvoSlot( nSlot );
					if( pObj == NULL )
						continue;
					if( pObj->m_nEnableSlot == 0 )
						continue;

					if( _IsDigimonDeclaredInUnionXml( pObj->m_dwID, pSelectedGroup ) )
						_PushUniqueDigimon( vDigimonIDs, pObj->m_dwID );
				}

				for( CDigimonEvolveObj::MAP_IT it = pEvolveInfo->m_Map.begin(); it != pEvolveInfo->m_Map.end() && vDigimonIDs.size() < nCardLimit; ++it )
				{
					if( it->second && it->second->m_nEnableSlot != 0 && _IsDigimonDeclaredInUnionXml( it->second->m_dwID, pSelectedGroup ) )
						_PushUniqueDigimon( vDigimonIDs, it->second->m_dwID );
				}
			}
		}
	}

	for( size_t i = 0; i < vDigimonIDs.size() && m_vCards.size() < nCardLimit; ++i )
	{
		sCARD_VIEW card;
		if( _FillCardFromDigimon( card, vDigimonIDs[ i ], pSelectedGroup ) )
			m_vCards.push_back( card );
	}

	if( m_vCards.empty() )
	{
		for( size_t i = 0; i < m_vAllGroups.size() && m_vCards.size() < IF_XML_UNION_CARD_COUNT; ++i )
		{
			if( _IsSameOption( _MakeOptionText( m_vAllGroups[ i ].s_wsName ), wsSelected ) == false )
				continue;

			sCARD_VIEW card;
			_FillCardFromGroup( card, m_vAllGroups[ i ] );
			m_vCards.push_back( card );
		}
	}

	m_mapCardCache[ wsCacheKey ] = m_vCards;
}

void cXmlUnionDUnit::_CreateLayout()
{
	_AddSolidSprite( CsPoint( 0, 0 ), CsPoint( IF_XML_UNION_WIDTH, IF_XML_UNION_HEIGHT ), NiColorA( 0.01f, 0.03f, 0.08f, 0.72f ) );
	_AddAssetSprite( CsPoint( 8, 47 ), CsPoint( 207, IF_XML_UNION_HEIGHT - 70 ), XML_UNION_BG_LEFT, NiColorA( 0.02f, 0.10f, 0.16f, 0.88f ) );
	_AddAssetSprite( CsPoint( 13, 55 ), CsPoint( 199, IF_XML_UNION_HEIGHT - 91 ), XML_UNION_LIST_BG, NiColorA( 0.01f, 0.09f, 0.15f, 0.9f ) );
	_AddAssetSprite( CsPoint( 194, 47 ), CsPoint( 497, 300 ), XML_UNION_BG_CENTER, NiColorA( 0.02f, 0.05f, 0.10f, 0.88f ) );
	_AddAssetSprite( CsPoint( 690, 47 ), CsPoint( 188, 273 ), XML_UNION_BG_RIGHT, NiColorA( 0.02f, 0.10f, 0.16f, 0.88f ) );
	_AddAssetSprite( CsPoint( 690, 323 ), CsPoint( 188, 291 ), XML_UNION_BG_RIGHT, NiColorA( 0.02f, 0.10f, 0.16f, 0.88f ) );
	_AddAssetSprite( CsPoint( 209, 350 ), CsPoint( 468, 202 ), XML_UNION_BG_INFO, NiColorA( 0.02f, 0.05f, 0.14f, 0.86f ) );
	_AddAssetSprite( CsPoint( 702, 74 ), CsPoint( 165, 150 ), DUNGEON_RESULT_SIMPLE, NiColorA( 0.01f, 0.05f, 0.10f, 0.75f ) );
	_AddAssetSprite( CsPoint( 752, 117 ), CsPoint( 64, 64 ), XML_UNION_DIGIMON_HOLD_GAUGE, NiColorA( 0.0f, 0.9f, 1.0f, 0.65f ) );

	cSprite* pTreasureIcon = NiNew cSprite;
	if( pTreasureIcon )
	{
		pTreasureIcon->Init( GetRoot(), CsPoint( 828, 282 ), CsPoint( 35, 38 ), CsRect( 0, 0, 56, 61 ), XML_UNION_TREASURE_ICON, false );
		AddChildControl( pTreasureIcon );
	}

	m_pMoveButton = AddButton( CsPoint( 827, 5 ), CsPoint( 22, 22 ), CsPoint( 0, 22 ), XML_UNION_MOVE_BTN );
	m_pCloseButton = AddButton( CsPoint( 852, 3 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), XML_UNION_CLOSE_BTN );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cXmlUnionDUnit::PressCloseButton );

	_AddLabel( _T( "XML D-Unit" ), CsPoint( 443, 14 ), CFont::FS_12, NiColor( 0.92f, 0.95f, 1.0f ), DT_CENTER );
	m_pStatusText = _AddLabel( m_XmlData.IsLoaded() ? _T( "Data\\union" ) : _T( "XML nao carregado" ), CsPoint( 443, 30 ), CFont::FS_8, NiColor( 0.4f, 1.0f, 0.8f ), DT_CENTER );

	cSprite* pLeftFilterIcon = NiNew cSprite;
	if( pLeftFilterIcon )
	{
		pLeftFilterIcon->Init( GetRoot(), CsPoint( 20, 55 ), CsPoint( 18, 20 ), CsRect( 0, 0, 21, 23 ), XML_UNION_FILTER_ICON, false );
		AddChildControl( pLeftFilterIcon );
	}

	_AddAssetSprite( CsPoint( 42, 51 ), CsPoint( 121, 24 ), XML_UNION_SEARCH_BAR, NiColorA( 0.02f, 0.16f, 0.30f, 0.9f ) );
	AddButton( CsPoint( 164, 51 ), CsPoint( 24, 24 ), CsPoint( 0, 32 ), XML_UNION_SEARCH_BTN );

	cText::sTEXTINFO searchInfo;
	searchInfo.Init( &g_pEngine->m_FontSystem );
	searchInfo.s_eFontSize = CFont::FS_10;
	searchInfo.s_Color = NiColor::WHITE;
	searchInfo.s_eTextAlign = DT_LEFT;
	m_pSearchEdit = NiNew cEditBox;
	if( m_pSearchEdit )
	{
		m_pSearchEdit->Init( GetRoot(), CsPoint( 49, 55 ), CsPoint( 106, 18 ), &searchInfo, false );
		m_pSearchEdit->SetLimitNumber( 32 );
		m_pSearchEdit->SetEmptyMsgText( _T( "Inserir a busca." ), NiColor( 0.55f, 0.65f, 0.78f ) );
		m_pSearchEdit->SetEnableSound( true );
		if( m_wsSearchKeyword.empty() == false )
		{
			TCHAR szSearch[ 96 ] = { 0, };
			_CopyWideToTChar( m_wsSearchKeyword, szSearch, _countof( szSearch ) );
			m_pSearchEdit->SetText( szSearch );
		}
		AddChildControl( m_pSearchEdit );
	}

	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		CsPoint const ptRow( XML_UNION_FILTER_ROW_X, XML_UNION_FILTER_ROW_Y + ( i * XML_UNION_FILTER_ROW_STEP ) );
		m_pFilterButtons[ i ] = AddButton( ptRow, CsPoint( XML_UNION_FILTER_ROW_W, XML_UNION_FILTER_ROW_H ), CsPoint( 0, 28 ), XML_UNION_LIST_BUTTON );

		m_pFilterHoverBg[ i ] = _AddSolidSprite( CsPoint( ptRow.x + 1, ptRow.y + 1 ), CsPoint( XML_UNION_FILTER_ROW_W - 2, XML_UNION_FILTER_ROW_H - 2 ), NiColorA( 0.0f, 0.50f, 0.72f, 0.55f ) );
		m_pFilterText[ i ] = _AddLabel( _T( "" ), CsPoint( ptRow.x + 10, ptRow.y + 10 ), CFont::FS_8, NiColor( 0.96f, 0.97f, 1.0f ), DT_LEFT );
		m_pFilterBookmark[ i ] = _AddAssetSpriteRect( CsPoint( ptRow.x + XML_UNION_FILTER_ROW_W - 22, ptRow.y + 6 ), CsPoint( 18, 18 ), CsRect( 0, 0, 21, 21 ), XML_UNION_BOOKMARK_ICON, NiColorA( 0.7f, 0.72f, 0.70f, 0.75f ) );
		m_pFilterBorderTop[ i ] = _AddSolidSprite( CsPoint( ptRow.x, ptRow.y ), CsPoint( XML_UNION_FILTER_ROW_W, 1 ), NiColorA( 0.0f, 0.35f, 0.75f, 0.75f ) );
		m_pFilterBorderBottom[ i ] = _AddSolidSprite( CsPoint( ptRow.x, ptRow.y + XML_UNION_FILTER_ROW_H - 1 ), CsPoint( XML_UNION_FILTER_ROW_W, 1 ), NiColorA( 0.0f, 0.35f, 0.75f, 0.75f ) );
		m_pFilterBorderLeft[ i ] = _AddSolidSprite( CsPoint( ptRow.x, ptRow.y ), CsPoint( 1, XML_UNION_FILTER_ROW_H ), NiColorA( 0.0f, 0.35f, 0.75f, 0.75f ) );
		m_pFilterBorderRight[ i ] = _AddSolidSprite( CsPoint( ptRow.x + XML_UNION_FILTER_ROW_W - 1, ptRow.y ), CsPoint( 1, XML_UNION_FILTER_ROW_H ), NiColorA( 0.0f, 0.35f, 0.75f, 0.75f ) );
	}
	_CreateFilterScrollBar();
	_UpdateFilterRowControls();

	int const nTabX = 226;
	int const nTabY = 60;
	int const nTabH = 28;
	m_pTabBody = _AddSolidSprite( CsPoint( nTabX, nTabY ), CsPoint( 80, nTabH ), NiColorA( 0.02f, 0.18f, 0.12f, 0.94f ) );
	for( int i = 0; i < IF_XML_UNION_TAB_SLANT_COUNT; ++i )
	{
		m_pTabSlant[ i ] = _AddSolidSprite( CsPoint( nTabX + 80 + i, nTabY + i ), CsPoint( 1, max( 1, nTabH - i - 1 ) ), NiColorA( 0.02f, 0.18f, 0.12f, 0.94f ) );
	}
	m_pTabTop = _AddSolidSprite( CsPoint( nTabX, nTabY ), CsPoint( 80, 1 ), NiColorA( 0.25f, 1.0f, 0.25f, 0.95f ) );
	for( int i = 0; i < IF_XML_UNION_TAB_SLANT_COUNT; ++i )
		m_pTabSlantTop[ i ] = _AddSolidSprite( CsPoint( nTabX + 80 + i, nTabY + i ), CsPoint( 1, 1 ), NiColorA( 0.25f, 1.0f, 0.25f, 0.90f ) );
	m_pTabBottom = _AddSolidSprite( CsPoint( nTabX, nTabY + nTabH - 1 ), CsPoint( 98, 1 ), NiColorA( 0.0f, 0.65f, 0.35f, 0.85f ) );
	m_pTabLeft = _AddSolidSprite( CsPoint( nTabX, nTabY ), CsPoint( 1, nTabH ), NiColorA( 0.25f, 1.0f, 0.25f, 0.95f ) );
	m_pTabText = _AddBoldLabel( _T( "" ), CsPoint( nTabX + 7, nTabY + 5 ), CFont::FS_12, NiColor( 0.96f, 1.0f, 0.96f ), DT_LEFT );
	_UpdateTabControls();

	_AddLabel( _T( "0.0%" ), CsPoint( 784, 148 ), CFont::FS_10, NiColor( 0.92f, 0.95f, 1.0f ), DT_CENTER );
	_AddSolidSprite( CsPoint( 714, 258 ), CsPoint( 142, 1 ), NiColorA( 0.0f, 0.95f, 1.0f, 0.95f ) );
	_AddLabel( _T( "Nivel" ), CsPoint( 722, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );
	_AddLabel( _T( "Desconhecido" ), CsPoint( 816, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );

	_AddLabel( _T( "Efeitos das Metas" ), CsPoint( 443, 369 ), CFont::FS_10, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
	{
		TCHAR szText[ 160 ] = { 0, };
		if( i < (int)m_vEffects.size() )
			_CopyWideToTChar( m_vEffects[ i ], szText, _countof( szText ) );
		m_pEffectText[ i ] = _AddLabel( szText, CsPoint( 220, 398 + ( i * 23 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
	}

	_AddLabel( _T( "Total dos XMLs" ), CsPoint( 784, 357 ), CFont::FS_9, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	for( int i = 0; i < IF_XML_UNION_STAT_COUNT && i < (int)m_vTotals.size(); ++i )
	{
		TCHAR szName[ 48 ] = { 0, };
		TCHAR szValue[ 48 ] = { 0, };
		std::wstring wsName = m_vTotals[ i ].s_wsName;
		if( wsName.empty() )
			wsName = STAT_LABEL[ i ];
		_CopyWideToTChar( wsName, szName, _countof( szName ) );
		_stprintf_s( szValue, _countof( szValue ), m_vTotals[ i ].s_bPercent ? _T( "+ %d%%" ) : _T( "+ %d" ), m_vTotals[ i ].s_nValue );

		_AddLabel( szName, CsPoint( 724, 397 + ( i * 24 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
		m_pStatValueText[ i ] = _AddLabel( szValue, CsPoint( 861, 397 + ( i * 24 ) ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_RIGHT );
	}
}

void cXmlUnionDUnit::_CreateCardSprites()
{
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		CsPoint const ptCard = CARD_POS[ i ];
		m_pCardBg[ i ] = _AddAssetSpriteRect( ptCard, CsPoint( 104, 120 ), CsRect( 0, 0, 102, 128 ), XML_UNION_CARD_BG, NiColorA( 0.06f, 0.06f, 0.08f, 0.9f ) );

		if( _FileExists( XML_UNION_CARD_BG ) )
		{
			for( int nSliceY = 0; nSliceY < IF_XML_UNION_ICON_SLICE_COUNT; ++nSliceY )
			{
				cSprite* pIconSlice = NiNew cSprite;
				if( pIconSlice )
				{
					pIconSlice->Init( GetRoot(), CsPoint( ptCard.x + 52, ptCard.y + 43 ), CsPoint( 1, 1 ), CsRect( 0, 0, 1, 1 ), XML_UNION_CARD_BG, false );
					pIconSlice->SetVisible( false );
					AddChildControl( pIconSlice );
					m_pCardIconSlices[ i ][ nSliceY ] = pIconSlice;
				}
			}
		}

		m_pCardFrame[ i ] = _AddAssetSpriteRect( ptCard, CsPoint( 104, 120 ), CsRect( 0, 0, 126, 150 ), XML_UNION_FRAME_N, NiColorA( 0.0f, 0.0f, 0.0f, 0.0f ) );

		cXmlUnionHoverEffectSprite* pHoverEffect = NiNew cXmlUnionHoverEffectSprite;
		if( pHoverEffect )
		{
			pHoverEffect->InitHoverEffect( GetRoot(), ptCard, CsPoint( 104, 120 ) );
			AddChildControl( pHoverEffect );
			m_pCardHoverEffects[ i ] = pHoverEffect;
		}

		m_pCardLevelText[ i ] = _AddLabel( _T( "" ), CsPoint( ptCard.x + 52, ptCard.y + 74 ), CFont::FS_10, NiColor( 0.96f, 0.97f, 1.0f ), DT_CENTER );

		if( _FileExists( XML_UNION_RANK_ICON ) )
		{
			cImage* pRankIcon = NiNew cImage;
			if( pRankIcon )
			{
				pRankIcon->Init( GetRoot(), CsPoint( ptCard.x + 11, ptCard.y + 75 ), CsPoint( 82, 66 ), XML_UNION_RANK_ICON, false );
				pRankIcon->SetTexToken( CsPoint( 102, 102 ) );
				pRankIcon->SetVisible( false );
				AddChildControl( pRankIcon );
				m_pCardRankIcon[ i ] = pRankIcon;
			}
		}
	}
	_UpdateCardControls();
}

void cXmlUnionDUnit::_UpdateTabControls()
{
	std::wstring const wsSelected = m_nSelectedFilter < (int)m_vAllFilters.size() ? m_vAllFilters[ m_nSelectedFilter ] : std::wstring();
	TCHAR szSelected[ 96 ] = { 0, };
	_CopyWideToTChar( wsSelected, szSelected, _countof( szSelected ) );

	int const nTabTextW = min( 220, max( 62, (int)wsSelected.size() * 8 ) );
	int const nTabBodyW = nTabTextW + 18;
	int const nTabX = 226;
	int const nTabY = 60;
	int const nTabH = 28;
	int const nTabRight = nTabX + nTabBodyW + IF_XML_UNION_TAB_SLANT_COUNT;

	if( m_pTabBody ) m_pTabBody->SetSize( CsPoint( nTabBodyW, nTabH ) );
	if( m_pTabTop ) m_pTabTop->SetSize( CsPoint( nTabBodyW, 1 ) );
	if( m_pTabBottom ) m_pTabBottom->SetSize( CsPoint( nTabRight - nTabX, 1 ) );
	if( m_pTabLeft ) m_pTabLeft->SetSize( CsPoint( 1, nTabH ) );
	if( m_pTabText ) m_pTabText->SetText( szSelected );

	for( int i = 0; i < IF_XML_UNION_TAB_SLANT_COUNT; ++i )
	{
		if( m_pTabSlant[ i ] )
		{
			m_pTabSlant[ i ]->SetPos( CsPoint( nTabX + nTabBodyW + i, nTabY + i ) );
			m_pTabSlant[ i ]->SetSize( CsPoint( 1, max( 1, nTabH - i - 1 ) ) );
		}
		if( m_pTabSlantTop[ i ] )
			m_pTabSlantTop[ i ]->SetPos( CsPoint( nTabX + nTabBodyW + i, nTabY + i ) );
	}
}

void cXmlUnionDUnit::_UpdateEffectControls()
{
	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
	{
		if( m_pEffectText[ i ] == NULL )
			continue;

		TCHAR szText[ 160 ] = { 0, };
		if( i < (int)m_vEffects.size() )
			_CopyWideToTChar( m_vEffects[ i ], szText, _countof( szText ) );
		m_pEffectText[ i ]->SetText( szText );
	}
}

void cXmlUnionDUnit::_UpdateCardControls()
{
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		CsPoint const ptCard = CARD_POS[ i ];
		int const nCardIndex = m_nCardScroll + i;
		bool const bHasCard = nCardIndex >= 0 && nCardIndex < (int)m_vCards.size();

		if( m_pCardBg[ i ] ) m_pCardBg[ i ]->SetVisible( true );
		if( m_pCardFrame[ i ] )
		{
			char const* pFrame = XML_UNION_FRAME_N;
			if( bHasCard )
			{
				if( m_vCards[ nCardIndex ].s_nRank >= 9 )
					pFrame = XML_UNION_FRAME_U;
				else if( m_vCards[ nCardIndex ].s_nRank >= 3 )
					pFrame = XML_UNION_FRAME_S;
			}
			if( _FileExists( pFrame ) && m_strCardFramePath[ i ] != pFrame )
			{
				m_pCardFrame[ i ]->ChangeTexture( pFrame, CsRect( 0, 0, 126, 150 ) );
				m_strCardFramePath[ i ] = pFrame;
			}
			m_pCardFrame[ i ]->SetVisible( true );
		}

		bool const bShowIcon = bHasCard && m_vCards[ nCardIndex ].s_strIconPath.empty() == false && _FileExists( m_vCards[ nCardIndex ].s_strIconPath.c_str() );
		std::string const strIconPath = bShowIcon ? m_vCards[ nCardIndex ].s_strIconPath : std::string();
		bool const bIconChanged = m_strCardIconPath[ i ] != strIconPath;
		int const nIconSize = IF_XML_UNION_ICON_SLICE_COUNT;
		int const nIconRadius = nIconSize / 2;
		int const nIconSrcSize = 32;
		CsPoint const ptIcon( ptCard.x + 52 - nIconRadius, ptCard.y + 43 - nIconRadius );

		for( int nSliceY = 0; nSliceY < IF_XML_UNION_ICON_SLICE_COUNT; ++nSliceY )
		{
			cSprite* pIconSlice = m_pCardIconSlices[ i ][ nSliceY ];
			if( pIconSlice == NULL )
				continue;

			if( bShowIcon == false )
			{
				pIconSlice->SetVisible( false );
				continue;
			}

			float const fDy = (float)nSliceY + 0.5f - (float)nIconRadius;
			float fHalfWidth = sqrtf( (float)( nIconRadius * nIconRadius ) - ( fDy * fDy ) );
			int const nHalfWidth = (int)fHalfWidth;
			if( nHalfWidth <= 0 )
			{
				pIconSlice->SetVisible( false );
				continue;
			}

			int const nDestX = ptIcon.x + nIconRadius - nHalfWidth;
			int const nDestW = nHalfWidth * 2;
			int const nSrcLeft = ( ( nIconRadius - nHalfWidth ) * nIconSrcSize ) / nIconSize;
			int const nSrcRight = ( ( nIconRadius + nHalfWidth ) * nIconSrcSize ) / nIconSize;
			int const nSrcTop = ( nSliceY * nIconSrcSize ) / nIconSize;
			int nSrcBottom = ( ( nSliceY + 1 ) * nIconSrcSize ) / nIconSize;
			if( nSrcBottom <= nSrcTop )
				nSrcBottom = nSrcTop + 1;

			if( bIconChanged )
			{
				pIconSlice->SetPosSize( CsPoint( nDestX, ptIcon.y + nSliceY ), CsPoint( nDestW, 1 ) );
				pIconSlice->ChangeTexture( strIconPath.c_str(), CsRect( nSrcLeft, nSrcTop, nSrcRight, nSrcBottom ), false );
			}
			pIconSlice->SetVisible( true );
		}
		m_strCardIconPath[ i ] = strIconPath;

		if( m_pCardLevelText[ i ] )
		{
			m_pCardLevelText[ i ]->SetVisible( bHasCard );
			if( bHasCard )
			{
				TCHAR szLevel[ 32 ] = { 0, };
				_stprintf_s( szLevel, _countof( szLevel ), _T( "Lvl : %d" ), m_vCards[ nCardIndex ].s_nLevel );
				m_pCardLevelText[ i ]->SetText( szLevel );
			}
		}

		if( m_pCardRankIcon[ i ] )
		{
			m_pCardRankIcon[ i ]->SetVisible( bHasCard );
			if( bHasCard )
				m_pCardRankIcon[ i ]->SetState( _GetUnionRankIconIndex( m_vCards[ nCardIndex ].s_nRank ) );
		}
	}

	_UpdateCardHoverEffects();
	_UpdateCardScrollControls();
}

void cXmlUnionDUnit::_CreateHoverTooltipControls()
{
	m_pHoverTooltipBg = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 1 ), NiColorA( 0.0f, 0.02f, 0.05f, 0.92f ) );
	m_pHoverTooltipTop = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pHoverTooltipBottom = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pHoverTooltipLeft = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pHoverTooltipRight = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pHoverTooltipText = _AddLabel( _T( "" ), CsPoint( 0, 0 ), CFont::FS_8, NiColor( 0.96f, 0.97f, 1.0f ), DT_CENTER );
	_UpdateHoverTooltipControls();
}

void cXmlUnionDUnit::_UpdateHoverTooltipControls()
{
	int const nHoverSlot = m_nHoverCard - m_nCardScroll;
	bool const bVisible = m_nHoverCard >= 0 && m_nHoverCard < (int)m_vCards.size() && nHoverSlot >= 0 && nHoverSlot < IF_XML_UNION_CARD_COUNT;
	if( m_pHoverTooltipBg ) m_pHoverTooltipBg->SetVisible( bVisible );
	if( m_pHoverTooltipTop ) m_pHoverTooltipTop->SetVisible( bVisible );
	if( m_pHoverTooltipBottom ) m_pHoverTooltipBottom->SetVisible( bVisible );
	if( m_pHoverTooltipLeft ) m_pHoverTooltipLeft->SetVisible( bVisible );
	if( m_pHoverTooltipRight ) m_pHoverTooltipRight->SetVisible( bVisible );
	if( m_pHoverTooltipText ) m_pHoverTooltipText->SetVisible( bVisible );
	_UpdateCardHoverEffects();
	if( bVisible == false )
		return;

	CsPoint const ptCard = CARD_POS[ nHoverSlot ];
	std::wstring const& wsName = m_vCards[ m_nHoverCard ].s_wsName;
	int const nTooltipW = min( 150, max( 72, (int)wsName.size() * 7 + 14 ) );
	int const nTooltipH = 22;
	int const nTooltipX = ptCard.x + 52 - ( nTooltipW / 2 );
	int const nTooltipY = ptCard.y - 22;

	if( m_pHoverTooltipBg )
	{
		m_pHoverTooltipBg->SetPos( CsPoint( nTooltipX, nTooltipY ) );
		m_pHoverTooltipBg->SetSize( CsPoint( nTooltipW, nTooltipH ) );
	}
	if( m_pHoverTooltipTop )
	{
		m_pHoverTooltipTop->SetPos( CsPoint( nTooltipX, nTooltipY ) );
		m_pHoverTooltipTop->SetSize( CsPoint( nTooltipW, 1 ) );
	}
	if( m_pHoverTooltipBottom )
	{
		m_pHoverTooltipBottom->SetPos( CsPoint( nTooltipX, nTooltipY + nTooltipH - 1 ) );
		m_pHoverTooltipBottom->SetSize( CsPoint( nTooltipW, 1 ) );
	}
	if( m_pHoverTooltipLeft )
	{
		m_pHoverTooltipLeft->SetPos( CsPoint( nTooltipX, nTooltipY ) );
		m_pHoverTooltipLeft->SetSize( CsPoint( 1, nTooltipH ) );
	}
	if( m_pHoverTooltipRight )
	{
		m_pHoverTooltipRight->SetPos( CsPoint( nTooltipX + nTooltipW - 1, nTooltipY ) );
		m_pHoverTooltipRight->SetSize( CsPoint( 1, nTooltipH ) );
	}
	if( m_pHoverTooltipText )
	{
		TCHAR szName[ 64 ] = { 0, };
		_CopyWideToTChar( wsName, szName, _countof( szName ) );
		m_pHoverTooltipText->SetText( szName );
		m_pHoverTooltipText->SetPos( CsPoint( ptCard.x + 52, nTooltipY + 5 ) );
	}
}

void cXmlUnionDUnit::_UpdateCardHoverEffects()
{
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		if( m_pCardHoverEffects[ i ] )
			m_pCardHoverEffects[ i ]->SetVisible( m_nCardScroll + i == m_nHoverCard && m_nHoverCard < (int)m_vCards.size() );
	}
}

void cXmlUnionDUnit::_UpdateVisibleFilters()
{
	m_vFilters.clear();
	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		int const nFilteredIndex = m_nFilterScroll + i;
		if( nFilteredIndex >= 0 && nFilteredIndex < (int)m_vFilteredFilterIndices.size() )
			m_vFilters.push_back( m_vAllFilters[ m_vFilteredFilterIndices[ nFilteredIndex ] ] );
	}
}

void cXmlUnionDUnit::_UpdateFilterRowControls()
{
	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		int const nFilteredIndex = m_nFilterScroll + i;
		int const nAbsoluteIndex = nFilteredIndex >= 0 && nFilteredIndex < (int)m_vFilteredFilterIndices.size() ? m_vFilteredFilterIndices[ nFilteredIndex ] : -1;
		bool const bVisible = i < (int)m_vFilters.size();
		bool const bSelected = nAbsoluteIndex == m_nSelectedFilter;
		bool const bHover = bVisible && nFilteredIndex == m_nHoverFilter;

		if( m_pFilterButtons[ i ] ) m_pFilterButtons[ i ]->SetVisible( bVisible );
		if( m_pFilterHoverBg[ i ] ) m_pFilterHoverBg[ i ]->SetVisible( bHover && bSelected == false );
		if( m_pFilterBookmark[ i ] ) m_pFilterBookmark[ i ]->SetVisible( bVisible );

		TCHAR szText[ 96 ] = { 0, };
		if( bVisible )
			_CopyWideToTChar( m_vFilters[ i ], szText, _countof( szText ) );
		if( m_pFilterText[ i ] )
		{
			m_pFilterText[ i ]->SetText( szText );
			m_pFilterText[ i ]->SetVisible( bVisible );
		}

		NiColorA const borderColor = bSelected ? NiColorA( 1.0f, 0.9f, 0.05f, 0.95f ) : ( bHover ? NiColorA( 0.0f, 0.95f, 1.0f, 0.95f ) : NiColorA( 0.0f, 0.35f, 0.75f, 0.75f ) );
		cSprite* pBorders[ 4 ] = { m_pFilterBorderTop[ i ], m_pFilterBorderBottom[ i ], m_pFilterBorderLeft[ i ], m_pFilterBorderRight[ i ] };
		for( int nBorder = 0; nBorder < 4; ++nBorder )
		{
			if( pBorders[ nBorder ] )
			{
				pBorders[ nBorder ]->SetVisible( bVisible );
				pBorders[ nBorder ]->SetColorA( borderColor );
			}
		}
	}

	bool const bShowScroll = (int)m_vFilteredFilterIndices.size() > IF_XML_UNION_FILTER_COUNT;
	if( m_pFilterScrollBorder ) m_pFilterScrollBorder->SetVisible( bShowScroll );
	if( m_pFilterScrollBg ) m_pFilterScrollBg->SetVisible( bShowScroll );
	if( m_pFilterScrollTrack ) m_pFilterScrollTrack->SetVisible( bShowScroll );
	if( m_pFilterScrollThumb ) m_pFilterScrollThumb->SetVisible( bShowScroll );
	if( m_pFilterScrollUp ) m_pFilterScrollUp->SetVisible( bShowScroll );
	if( m_pFilterScrollDown ) m_pFilterScrollDown->SetVisible( bShowScroll );
	if( bShowScroll == false || m_pFilterScrollThumb == NULL )
		return;

	int const nVisibleCount = (int)m_vFilteredFilterIndices.size();
	int const nMaxScroll = max( 1, nVisibleCount - IF_XML_UNION_FILTER_COUNT );
	int const nThumbH = max( 28, ( XML_UNION_FILTER_SCROLL_TRACK_H * IF_XML_UNION_FILTER_COUNT ) / nVisibleCount );
	int const nTravel = max( 1, XML_UNION_FILTER_SCROLL_TRACK_H - nThumbH );
	int const nThumbY = XML_UNION_FILTER_SCROLL_TRACK_Y + ( nTravel * m_nFilterScroll ) / nMaxScroll;
	m_pFilterScrollThumb->SetPos( CsPoint( XML_UNION_FILTER_SCROLL_X + 1, nThumbY ) );
	m_pFilterScrollThumb->SetSize( CsPoint( XML_UNION_FILTER_SCROLL_W - 2, nThumbH ) );
}

void cXmlUnionDUnit::_CreateFilterScrollBar()
{
	int const nBottomArrowY = XML_UNION_FILTER_SCROLL_Y + XML_UNION_FILTER_SCROLL_H - XML_UNION_FILTER_SCROLL_ARROW_H;

	m_pFilterScrollBorder = _AddSolidSprite( CsPoint( XML_UNION_FILTER_SCROLL_X - 1, XML_UNION_FILTER_SCROLL_Y - 1 ), CsPoint( XML_UNION_FILTER_SCROLL_W + 2, XML_UNION_FILTER_SCROLL_H + 2 ), NiColorA( 0.0f, 0.35f, 0.75f, 0.85f ) );
	m_pFilterScrollBg = _AddSolidSprite( CsPoint( XML_UNION_FILTER_SCROLL_X, XML_UNION_FILTER_SCROLL_Y ), CsPoint( XML_UNION_FILTER_SCROLL_W, XML_UNION_FILTER_SCROLL_H ), NiColorA( 0.0f, 0.10f, 0.22f, 0.9f ) );
	m_pFilterScrollUp = _AddAssetSpriteRect( CsPoint( XML_UNION_FILTER_SCROLL_X, XML_UNION_FILTER_SCROLL_Y ), CsPoint( XML_UNION_FILTER_SCROLL_W, XML_UNION_FILTER_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_UP, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pFilterScrollDown = _AddAssetSpriteRect( CsPoint( XML_UNION_FILTER_SCROLL_X, nBottomArrowY ), CsPoint( XML_UNION_FILTER_SCROLL_W, XML_UNION_FILTER_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_DOWN, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pFilterScrollTrack = _AddSolidSprite( CsPoint( XML_UNION_FILTER_SCROLL_X + 1, XML_UNION_FILTER_SCROLL_TRACK_Y ), CsPoint( XML_UNION_FILTER_SCROLL_W - 2, XML_UNION_FILTER_SCROLL_TRACK_H ), NiColorA( 0.0f, 0.40f, 0.75f, 0.35f ) );
	m_pFilterScrollThumb = _AddSolidSprite( CsPoint( XML_UNION_FILTER_SCROLL_X + 1, XML_UNION_FILTER_SCROLL_TRACK_Y ), CsPoint( XML_UNION_FILTER_SCROLL_W - 2, 28 ), NiColorA( 0.0f, 0.78f, 1.0f, 0.95f ) );
	_UpdateFilterRowControls();
}

void cXmlUnionDUnit::_CreateCardScrollBar()
{
	int const nBottomArrowY = XML_UNION_CARD_SCROLL_Y + XML_UNION_CARD_SCROLL_H - XML_UNION_CARD_SCROLL_ARROW_H;

	m_pCardScrollBorder = _AddSolidSprite( CsPoint( XML_UNION_CARD_SCROLL_X - 1, XML_UNION_CARD_SCROLL_Y - 1 ), CsPoint( XML_UNION_CARD_SCROLL_W + 2, XML_UNION_CARD_SCROLL_H + 2 ), NiColorA( 0.0f, 0.35f, 0.75f, 0.85f ) );
	m_pCardScrollBg = _AddSolidSprite( CsPoint( XML_UNION_CARD_SCROLL_X, XML_UNION_CARD_SCROLL_Y ), CsPoint( XML_UNION_CARD_SCROLL_W, XML_UNION_CARD_SCROLL_H ), NiColorA( 0.0f, 0.10f, 0.22f, 0.9f ) );
	m_pCardScrollUp = _AddAssetSpriteRect( CsPoint( XML_UNION_CARD_SCROLL_X, XML_UNION_CARD_SCROLL_Y ), CsPoint( XML_UNION_CARD_SCROLL_W, XML_UNION_CARD_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_UP, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pCardScrollDown = _AddAssetSpriteRect( CsPoint( XML_UNION_CARD_SCROLL_X, nBottomArrowY ), CsPoint( XML_UNION_CARD_SCROLL_W, XML_UNION_CARD_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_DOWN, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pCardScrollTrack = _AddSolidSprite( CsPoint( XML_UNION_CARD_SCROLL_X + 1, XML_UNION_CARD_SCROLL_TRACK_Y ), CsPoint( XML_UNION_CARD_SCROLL_W - 2, XML_UNION_CARD_SCROLL_TRACK_H ), NiColorA( 0.0f, 0.40f, 0.75f, 0.35f ) );
	m_pCardScrollThumb = _AddSolidSprite( CsPoint( XML_UNION_CARD_SCROLL_X + 1, XML_UNION_CARD_SCROLL_TRACK_Y ), CsPoint( XML_UNION_CARD_SCROLL_W - 2, 28 ), NiColorA( 0.0f, 0.78f, 1.0f, 0.95f ) );
	_UpdateCardScrollControls();
}

void cXmlUnionDUnit::_UpdateCardScrollControls()
{
	bool const bShowScroll = (int)m_vCards.size() > IF_XML_UNION_CARD_COUNT;
	if( m_pCardScrollBorder ) m_pCardScrollBorder->SetVisible( bShowScroll );
	if( m_pCardScrollBg ) m_pCardScrollBg->SetVisible( bShowScroll );
	if( m_pCardScrollTrack ) m_pCardScrollTrack->SetVisible( bShowScroll );
	if( m_pCardScrollThumb ) m_pCardScrollThumb->SetVisible( bShowScroll );
	if( m_pCardScrollUp ) m_pCardScrollUp->SetVisible( bShowScroll );
	if( m_pCardScrollDown ) m_pCardScrollDown->SetVisible( bShowScroll );
	if( bShowScroll == false || m_pCardScrollThumb == NULL )
		return;

	int const nVisibleCount = (int)m_vCards.size();
	int const nMaxScroll = max( 1, ( ( nVisibleCount - 1 ) / XML_UNION_CARD_SCROLL_PAGE ) * XML_UNION_CARD_SCROLL_PAGE );
	int const nThumbH = max( 28, ( XML_UNION_CARD_SCROLL_TRACK_H * IF_XML_UNION_CARD_COUNT ) / nVisibleCount );
	int const nTravel = max( 1, XML_UNION_CARD_SCROLL_TRACK_H - nThumbH );
	int const nThumbY = XML_UNION_CARD_SCROLL_TRACK_Y + ( nTravel * m_nCardScroll ) / nMaxScroll;
	m_pCardScrollThumb->SetPos( CsPoint( XML_UNION_CARD_SCROLL_X + 1, nThumbY ) );
	m_pCardScrollThumb->SetSize( CsPoint( XML_UNION_CARD_SCROLL_W - 2, nThumbH ) );
}
