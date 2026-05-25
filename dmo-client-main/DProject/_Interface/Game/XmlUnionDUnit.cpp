#include "stdafx.h"
#include "XmlUnionDUnit.h"
#include "../../network/cNetwork.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
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
	const char* const XML_UNION_FRAME_COMPLETED = "Encyclopedia\\newencyclopedia\\rate_frame\\collection_frame_rank3.png";
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
	int const XML_UNION_FILTER_BOOKMARK_SIZE = 21;
	int const XML_UNION_FILTER_BOOKMARK_RIGHT_PAD = 17;
	int const XML_UNION_FILTER_BOOKMARK_Y_OFFSET = -1;
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
	int const XML_UNION_RIGHT_PANEL_SHIFT = 10;
	int const XML_UNION_TOTAL_STAT_ROW_X = 724 + XML_UNION_RIGHT_PANEL_SHIFT;
	int const XML_UNION_TOTAL_STAT_VALUE_X = 853 + XML_UNION_RIGHT_PANEL_SHIFT;
	int const XML_UNION_TOTAL_STAT_ROW_Y = 397;
	int const XML_UNION_TOTAL_STAT_ROW_STEP = 24;
	int const XML_UNION_TOTAL_STAT_SCROLL_X = 866 + XML_UNION_RIGHT_PANEL_SHIFT;
	int const XML_UNION_TOTAL_STAT_SCROLL_Y = 389;
	int const XML_UNION_TOTAL_STAT_SCROLL_W = 14;
	int const XML_UNION_TOTAL_STAT_SCROLL_H = 210;
	int const XML_UNION_TOTAL_STAT_SCROLL_ARROW_H = 14;
	int const XML_UNION_TOTAL_STAT_SCROLL_TRACK_Y = XML_UNION_TOTAL_STAT_SCROLL_Y + XML_UNION_TOTAL_STAT_SCROLL_ARROW_H;
	int const XML_UNION_TOTAL_STAT_SCROLL_TRACK_H = XML_UNION_TOTAL_STAT_SCROLL_H - ( XML_UNION_TOTAL_STAT_SCROLL_ARROW_H * 2 );
	int const XML_UNION_TREASURE_X = 828;
	int const XML_UNION_TREASURE_Y = 282;
	int const XML_UNION_TREASURE_W = 35;
	int const XML_UNION_TREASURE_H = 38;
	int const XML_UNION_TREASURE_HOVER_GROW = 6;
	int const XML_UNION_REWARD_MODAL_X = 128;
	int const XML_UNION_REWARD_MODAL_Y = 36;
	int const XML_UNION_REWARD_MODAL_W = 640;
	int const XML_UNION_REWARD_MODAL_H = 610;
	int const XML_UNION_REWARD_SCROLL_W = 14;
	int const XML_UNION_REWARD_SCROLL_H = XML_UNION_REWARD_MODAL_H - 46;
	int const XML_UNION_REWARD_SCROLL_ARROW_H = 14;
	int const XML_UNION_REWARD_SCROLL_TRACK_H = XML_UNION_REWARD_SCROLL_H - ( XML_UNION_REWARD_SCROLL_ARROW_H * 2 );
	int const XML_UNION_REWARD_VISIBLE_ROWS = 2;

	struct sREWARD_MODAL_OPTION
	{
		TCHAR const*	s_pName;
		NiColorA	s_GlowColor;
	};

	sREWARD_MODAL_OPTION const REWARD_MODAL_OPTIONS[ IF_XML_UNION_REWARD_COUNT ] =
	{
		{ _T( "Bronze" ), NiColorA( 0.83f, 0.50f, 0.32f, 0.88f ) },
		{ _T( "Prata" ), NiColorA( 0.92f, 0.92f, 0.86f, 0.88f ) },
		{ _T( "Ouro" ), NiColorA( 0.92f, 1.00f, 0.12f, 0.88f ) },
		{ _T( "Platinum" ), NiColorA( 1.00f, 0.10f, 1.00f, 0.88f ) },
		{ _T( "Diamante" ), NiColorA( 0.04f, 1.00f, 1.00f, 0.88f ) },
		{ _T( "Master" ), NiColorA( 1.00f, 0.04f, 0.42f, 0.88f ) },
		{ _T( "Mestre Digimon" ), NiColorA( 0.20f, 1.00f, 0.10f, 0.88f ) },
	};

	CsPoint const CARD_POS[ IF_XML_UNION_CARD_COUNT ] =
	{
		CsPoint( 226, 91 ), CsPoint( 345, 91 ), CsPoint( 464, 91 ), CsPoint( 583, 91 ),
		CsPoint( 226, 226 ), CsPoint( 345, 226 ), CsPoint( 464, 226 ), CsPoint( 583, 226 ),
	};

	wchar_t const* const STAT_LABEL[ IF_XML_UNION_STAT_COUNT ] =
	{
		L"MaxHP", L"MaxDS", L"AT", L"DE", L"EV", L"HT",
		L"CT", L"EXP", L"BL", L"SCD", L"Basic Attribute",
		L"Vaccine", L"Data", L"Virus", L"Unknown",
		L"Fire", L"Water", L"Ice", L"Wind", L"Thunder",
		L"Light", L"Dark", L"Land", L"Wood", L"Steel",
	};

	bool _IsPercentStatIndex( int nIndex )
	{
		return nIndex == 7 || nIndex >= 9;
	}

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

	std::wstring _GetDisplayStatName( std::wstring wsName )
	{
		if( wsName == L"HP" )
			return L"MaxHP";
		if( wsName == L"DS" )
			return L"MaxDS";
		return wsName;
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

	CsRect _GetUnionRankIconRect( int nDigimonRank )
	{
		// encyclopedia_512.png is a 102x102 atlas. Use explicit source rects
		// instead of cImage state math so SSS+ always crops the Korean badge.
		int nTile = 1; // N
		if( nDigimonRank > 0 )
		{
			nTile = nDigimonRank - 1;
			if( nDigimonRank >= 8 )
				nTile = 9; // SSS+
		}

		int const nTileSize = 102;
		int const nCol = nTile % 5;
		int const nRow = nTile / 5;
		return CsRect( nCol * nTileSize, nRow * nTileSize, ( nCol + 1 ) * nTileSize, ( nRow + 1 ) * nTileSize );
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
	, m_pProgressPercentText( NULL )
	, m_pProgressLevelValueText( NULL )
	, m_pEffectCompletedText( NULL )
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
	, m_pTotalStatScrollBorder( NULL )
	, m_pTotalStatScrollBg( NULL )
	, m_pTotalStatScrollTrack( NULL )
	, m_pTotalStatScrollThumb( NULL )
	, m_pTotalStatScrollUp( NULL )
	, m_pTotalStatScrollDown( NULL )
	, m_pRewardTreasureIcon( NULL )
	, m_pRewardModalBg( NULL )
	, m_pRewardModalTitleBg( NULL )
	, m_pRewardModalBorderTop( NULL )
	, m_pRewardModalBorderBottom( NULL )
	, m_pRewardModalBorderLeft( NULL )
	, m_pRewardModalBorderRight( NULL )
	, m_pRewardModalContentLine( NULL )
	, m_pRewardModalTitleText( NULL )
	, m_pRewardScrollBorder( NULL )
	, m_pRewardScrollBg( NULL )
	, m_pRewardScrollTrack( NULL )
	, m_pRewardScrollThumb( NULL )
	, m_pRewardScrollUp( NULL )
	, m_pRewardScrollDown( NULL )
	, m_pRewardModalCloseButton( NULL )
	, m_bRewardModalVisible( false )
	, m_bRewardTreasureIconHover( false )
	, m_bRewardModalDragging( false )
	, m_ptRewardModalPos( XML_UNION_REWARD_MODAL_X, XML_UNION_REWARD_MODAL_Y )
	, m_ptRewardModalDragOffset( 0, 0 )
	, m_nRewardModalScrollRow( 0 )
	, m_nRewardReceiveHoverIndex( -1 )
	, m_bServerProgressLoaded( false )
	, m_nServerXmlUnionLevel( 0 )
	, m_nServerCurrentExperience( 0 )
	, m_nServerRequiredExperience( 0 )
	, m_nServerProgressPercentBasisPoints( 0 )
	, m_nServerClaimedRewardMask( 0 )
	, m_nTotalStatScroll( 0 )
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
	{
		m_pEffectText[ i ] = NULL;
		m_pEffectCheckBg[ i ] = NULL;
		m_pEffectCheckTop[ i ] = NULL;
		m_pEffectCheckBottom[ i ] = NULL;
		m_pEffectCheckLeft[ i ] = NULL;
		m_pEffectCheckRight[ i ] = NULL;
		m_pEffectCheckMark[ i ] = NULL;
		m_pEffectStateButton[ i ] = NULL;
		m_pEffectStateTop[ i ] = NULL;
		m_pEffectStateBottom[ i ] = NULL;
		m_pEffectStateLeft[ i ] = NULL;
		m_pEffectStateRight[ i ] = NULL;
		m_pEffectStateText[ i ] = NULL;
	}

	for( int i = 0; i < IF_XML_UNION_STAT_VISIBLE_COUNT; ++i )
	{
		m_pStatNameText[ i ] = NULL;
		m_pStatValueText[ i ] = NULL;
	}

	for( int i = 0; i < IF_XML_UNION_STAT_COUNT; ++i )
		m_nServerBonusValue[ i ] = 0;

	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		m_pRewardOptionGlow[ i ] = NULL;
		m_pRewardOptionBody[ i ] = NULL;
		m_pRewardOptionGauge[ i ] = NULL;
		m_pRewardReceiveHoverBg[ i ] = NULL;
		m_pRewardReceiveBorderTop[ i ] = NULL;
		m_pRewardReceiveBorderBottom[ i ] = NULL;
		m_pRewardReceiveBorderLeft[ i ] = NULL;
		m_pRewardReceiveBorderRight[ i ] = NULL;
		m_pRewardOptionNameText[ i ] = NULL;
		m_pRewardReceiveText[ i ] = NULL;
		m_pRewardReceiveButton[ i ] = NULL;
	}
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
	m_pProgressPercentText = NULL;
	m_pProgressLevelValueText = NULL;
	m_pEffectCompletedText = NULL;
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
	m_pTotalStatScrollBorder = NULL;
	m_pTotalStatScrollBg = NULL;
	m_pTotalStatScrollTrack = NULL;
	m_pTotalStatScrollThumb = NULL;
	m_pTotalStatScrollUp = NULL;
	m_pTotalStatScrollDown = NULL;
	m_pRewardTreasureIcon = NULL;
	m_pRewardModalBg = NULL;
	m_pRewardModalTitleBg = NULL;
	m_pRewardModalBorderTop = NULL;
	m_pRewardModalBorderBottom = NULL;
	m_pRewardModalBorderLeft = NULL;
	m_pRewardModalBorderRight = NULL;
	m_pRewardModalContentLine = NULL;
	m_pRewardModalTitleText = NULL;
	m_pRewardScrollBorder = NULL;
	m_pRewardScrollBg = NULL;
	m_pRewardScrollTrack = NULL;
	m_pRewardScrollThumb = NULL;
	m_pRewardScrollUp = NULL;
	m_pRewardScrollDown = NULL;
	m_pRewardModalCloseButton = NULL;
	m_bRewardModalVisible = false;
	m_bRewardTreasureIconHover = false;
	m_bRewardModalDragging = false;
	m_ptRewardModalPos = CsPoint( XML_UNION_REWARD_MODAL_X, XML_UNION_REWARD_MODAL_Y );
	m_ptRewardModalDragOffset = CsPoint( 0, 0 );
	m_nRewardModalScrollRow = 0;
	m_nRewardReceiveHoverIndex = -1;
	m_bServerProgressLoaded = false;
	m_nServerXmlUnionLevel = 0;
	m_nServerCurrentExperience = 0;
	m_nServerRequiredExperience = 0;
	m_nServerProgressPercentBasisPoints = 0;
	m_nServerClaimedRewardMask = 0;
	m_nTotalStatScroll = 0;
	m_mapServerGroups.clear();
	m_vRewardModalSprites.clear();
	m_vRewardModalTexts.clear();
	m_vRewardModalButtons.clear();
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
	{
		m_pEffectText[ i ] = NULL;
		m_pEffectCheckBg[ i ] = NULL;
		m_pEffectCheckTop[ i ] = NULL;
		m_pEffectCheckBottom[ i ] = NULL;
		m_pEffectCheckLeft[ i ] = NULL;
		m_pEffectCheckRight[ i ] = NULL;
		m_pEffectCheckMark[ i ] = NULL;
		m_pEffectStateButton[ i ] = NULL;
		m_pEffectStateTop[ i ] = NULL;
		m_pEffectStateBottom[ i ] = NULL;
		m_pEffectStateLeft[ i ] = NULL;
		m_pEffectStateRight[ i ] = NULL;
		m_pEffectStateText[ i ] = NULL;
	}
	for( int i = 0; i < IF_XML_UNION_STAT_VISIBLE_COUNT; ++i )
	{
		m_pStatNameText[ i ] = NULL;
		m_pStatValueText[ i ] = NULL;
	}
	for( int i = 0; i < IF_XML_UNION_STAT_COUNT; ++i )
		m_nServerBonusValue[ i ] = 0;
	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		m_pRewardOptionGlow[ i ] = NULL;
		m_pRewardOptionBody[ i ] = NULL;
		m_pRewardOptionGauge[ i ] = NULL;
		m_pRewardReceiveHoverBg[ i ] = NULL;
		m_pRewardReceiveBorderTop[ i ] = NULL;
		m_pRewardReceiveBorderBottom[ i ] = NULL;
		m_pRewardReceiveBorderLeft[ i ] = NULL;
		m_pRewardReceiveBorderRight[ i ] = NULL;
		m_pRewardOptionNameText[ i ] = NULL;
		m_pRewardReceiveText[ i ] = NULL;
		m_pRewardReceiveButton[ i ] = NULL;
	}
}

void cXmlUnionDUnit::Create( int nValue )
{
	if( cBaseWindow::Init() == false )
		return;

	_LoadFavoriteCache();
	_BuildViewData();

	int nPosX = ( g_nScreenWidth / 2 ) - ( IF_XML_UNION_WIDTH / 2 );
	int nPosY = ( g_nScreenHeight / 2 ) - ( IF_XML_UNION_HEIGHT / 2 );
	InitScript( NULL, CsPoint( nPosX, nPosY ), CsPoint( IF_XML_UNION_WIDTH, IF_XML_UNION_HEIGHT ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	_CreateLayout();
	_CreateCardSprites();
	_CreateCardScrollBar();
	_CreateHoverTooltipControls();
	_CreateRewardModalControls();

	if( net::game )
		net::game->SendXmlUnionProgressRequest();
}

void cXmlUnionDUnit::Update(float const& fDeltaTime)
{
	UpdateScript( fDeltaTime );
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cXmlUnionDUnit::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW && m_bRewardModalVisible == false )
		return muReturn;

	if( m_pCloseButton && m_pCloseButton->Update_ForMouse() == cButton::ACTION_CLICK )
		return muReturn;

	CsPoint const ptRoot = GetRootClient();
	CsPoint const ptCursor = CURSOR_ST.GetPos();
	CsRect const rcTreasure( CsPoint( ptRoot.x + XML_UNION_TREASURE_X + XML_UNION_RIGHT_PANEL_SHIFT - 4, ptRoot.y + XML_UNION_TREASURE_Y - 4 ), CsSIZE( XML_UNION_TREASURE_W + 8, XML_UNION_TREASURE_H + 8 ) );
	bool const bTreasureHover = m_bRewardModalVisible == false &&
		ptCursor.x >= rcTreasure.left && ptCursor.x <= rcTreasure.right &&
		ptCursor.y >= rcTreasure.top && ptCursor.y <= rcTreasure.bottom;
	if( m_pRewardTreasureIcon && bTreasureHover != m_bRewardTreasureIconHover )
	{
		m_bRewardTreasureIconHover = bTreasureHover;
		int const nGrow = bTreasureHover ? XML_UNION_TREASURE_HOVER_GROW : 0;
		m_pRewardTreasureIcon->SetPosSize(
			CsPoint( XML_UNION_TREASURE_X + XML_UNION_RIGHT_PANEL_SHIFT - ( nGrow / 2 ), XML_UNION_TREASURE_Y - ( nGrow / 2 ) ),
			CsPoint( XML_UNION_TREASURE_W + nGrow, XML_UNION_TREASURE_H + nGrow ) );
		m_pRewardTreasureIcon->SetColorA( bTreasureHover ? NiColorA( 0.85f, 1.0f, 1.0f, 1.0f ) : NiColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}

	if( m_bRewardModalVisible )
	{
		int const nModalX = m_ptRewardModalPos.x;
		int const nModalY = m_ptRewardModalPos.y;
		int const nRewardScrollX = nModalX + XML_UNION_REWARD_MODAL_W - 17;
		int const nRewardScrollY = nModalY + 38;
		int const nRewardScrollTrackY = nRewardScrollY + XML_UNION_REWARD_SCROLL_ARROW_H;

		if( m_pRewardModalCloseButton && m_pRewardModalCloseButton->Update_ForMouse() == cButton::ACTION_CLICK )
		{
			_SetRewardModalVisible( false );
			return muReturn;
		}

		CsRect const rcRewardTitle( CsPoint( ptRoot.x + nModalX, ptRoot.y + nModalY ), CsSIZE( XML_UNION_REWARD_MODAL_W - 42, 34 ) );
		CURSOR::eTYPE const eTitleMouse = CURSOR_ST.CheckClickBox( rcRewardTitle );
		if( m_bRewardModalDragging == false && eTitleMouse == CURSOR::LBUTTON_DOWN )
		{
			m_bRewardModalDragging = true;
			m_ptRewardModalDragOffset = CsPoint( ptCursor.x - ( ptRoot.x + nModalX ), ptCursor.y - ( ptRoot.y + nModalY ) );
		}
		if( m_bRewardModalDragging )
		{
			CURSOR::eTYPE const eButtonState = CURSOR_ST.GetButtonState();
			if( eButtonState == CURSOR::LBUTTON_DOWN || eButtonState == CURSOR::LBUTTON_PRESS )
			{
				int nNewX = ptCursor.x - ptRoot.x - m_ptRewardModalDragOffset.x;
				int nNewY = ptCursor.y - ptRoot.y - m_ptRewardModalDragOffset.y;
				nNewX = max( -ptRoot.x, min( g_nScreenWidth - ptRoot.x - XML_UNION_REWARD_MODAL_W, nNewX ) );
				nNewY = max( -ptRoot.y, min( g_nScreenHeight - ptRoot.y - XML_UNION_REWARD_MODAL_H, nNewY ) );
				if( nNewX != m_ptRewardModalPos.x || nNewY != m_ptRewardModalPos.y )
				{
					m_ptRewardModalPos = CsPoint( nNewX, nNewY );
					_UpdateRewardModalFrameControls();
					_UpdateRewardModalOptionControls();
					_UpdateRewardModalScrollControls();
				}
			}
			else
			{
				m_bRewardModalDragging = false;
			}
			return muReturn;
		}

		int nRewardHover = -1;
		for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
		{
			int const nOriginalRow = i / 3;
			int const nDisplayRow = nOriginalRow - m_nRewardModalScrollRow;
			if( nDisplayRow < 0 || nDisplayRow >= XML_UNION_REWARD_VISIBLE_ROWS )
				continue;

			int const nCol = i % 3;
			int const nCenterX = nModalX + 98 + ( nCol * 200 );
			int const nIconY = nModalY + 62 + ( nDisplayRow * 245 );
			CsRect const rcReceive( CsPoint( ptRoot.x + nCenterX - 52, ptRoot.y + nIconY + 178 ), CsSIZE( 104, 36 ) );
			if( CURSOR_ST.CheckClickBox( rcReceive ) != CURSOR::BUTTON_OUTWINDOW )
			{
				nRewardHover = i;
				break;
			}
		}
		_UpdateRewardReceiveButtonHover( nRewardHover );

		CsRect const rcRewardScrollUp( CsPoint( ptRoot.x + nRewardScrollX, ptRoot.y + nRewardScrollY ), CsSIZE( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ) );
		CsRect const rcRewardScrollDown( CsPoint( ptRoot.x + nRewardScrollX, ptRoot.y + nRewardScrollY + XML_UNION_REWARD_SCROLL_H - XML_UNION_REWARD_SCROLL_ARROW_H ), CsSIZE( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ) );
		if( CURSOR_ST.CheckClickBox( rcRewardScrollUp ) == CURSOR::LBUTTON_UP )
		{
			if( m_nRewardModalScrollRow > 0 )
			{
				--m_nRewardModalScrollRow;
				_UpdateRewardModalOptionControls();
				_UpdateRewardModalScrollControls();
			}
			return muReturn;
		}
		if( CURSOR_ST.CheckClickBox( rcRewardScrollDown ) == CURSOR::LBUTTON_UP )
		{
			if( m_nRewardModalScrollRow < 1 )
			{
				++m_nRewardModalScrollRow;
				_UpdateRewardModalOptionControls();
				_UpdateRewardModalScrollControls();
			}
			return muReturn;
		}

		CsRect const rcRewardTrack( CsPoint( ptRoot.x + nRewardScrollX, ptRoot.y + nRewardScrollTrackY ), CsSIZE( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_TRACK_H ) );
		if( CURSOR_ST.CheckClickBox( rcRewardTrack ) == CURSOR::LBUTTON_DOWN )
		{
			int const nNewScroll = ptCursor.y > rcRewardTrack.top + ( XML_UNION_REWARD_SCROLL_TRACK_H / 2 ) ? 1 : 0;
			if( nNewScroll != m_nRewardModalScrollRow )
			{
				m_nRewardModalScrollRow = nNewScroll;
				_UpdateRewardModalOptionControls();
				_UpdateRewardModalScrollControls();
			}
			return muReturn;
		}

		CsRect const rcRewardWindow( CsPoint( ptRoot.x + nModalX, ptRoot.y + nModalY ), CsSIZE( XML_UNION_REWARD_MODAL_W, XML_UNION_REWARD_MODAL_H ) );
		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && CURSOR_ST.CheckClickBox( rcRewardWindow ) != CURSOR::BUTTON_OUTWINDOW )
		{
			int const nOldScroll = m_nRewardModalScrollRow;
			if( CURSOR_ST.GetWheel() < 0 && m_nRewardModalScrollRow < 1 )
				++m_nRewardModalScrollRow;
			else if( CURSOR_ST.GetWheel() > 0 && m_nRewardModalScrollRow > 0 )
				--m_nRewardModalScrollRow;

			if( nOldScroll != m_nRewardModalScrollRow )
			{
				_UpdateRewardModalOptionControls();
				_UpdateRewardModalScrollControls();
			}
			CURSOR_ST.ResetWheel();
			return muReturn;
		}

		for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
		{
			int const nOriginalRow = i / 3;
			int const nDisplayRow = nOriginalRow - m_nRewardModalScrollRow;
			if( nDisplayRow < 0 || nDisplayRow >= XML_UNION_REWARD_VISIBLE_ROWS )
				continue;

			if( m_pRewardReceiveButton[ i ] && m_pRewardReceiveButton[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
			{
				if( net::game && _IsRewardClaimed( i ) == false )
					net::game->SendXmlUnionRewardClaim( (u1)i );
				return muReturn;
			}
		}

		return muReturn;
	}

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

	if( CURSOR_ST.CheckClickBox( rcTreasure ) == CURSOR::LBUTTON_UP )
	{
		_SetRewardModalVisible( true );
		return muReturn;
	}

	int const nTotalStatMaxScroll = max( 0, IF_XML_UNION_STAT_COUNT - IF_XML_UNION_STAT_VISIBLE_COUNT );
	CsRect const rcTotalStatScroll( CsPoint( ptRoot.x + XML_UNION_TOTAL_STAT_SCROLL_X - 3, ptRoot.y + XML_UNION_TOTAL_STAT_SCROLL_Y ),
		CsSIZE( XML_UNION_TOTAL_STAT_SCROLL_W + 6, XML_UNION_TOTAL_STAT_SCROLL_H ) );
	CsRect const rcTotalStatArea( CsPoint( ptRoot.x + 710 + XML_UNION_RIGHT_PANEL_SHIFT, ptRoot.y + 382 ), CsSIZE( 168, 220 ) );
	if( nTotalStatMaxScroll > 0 )
	{
		CsRect const rcTotalStatScrollUp( CsPoint( ptRoot.x + XML_UNION_TOTAL_STAT_SCROLL_X, ptRoot.y + XML_UNION_TOTAL_STAT_SCROLL_Y ), CsSIZE( XML_UNION_TOTAL_STAT_SCROLL_W, XML_UNION_TOTAL_STAT_SCROLL_ARROW_H ) );
		CsRect const rcTotalStatScrollDown( CsPoint( ptRoot.x + XML_UNION_TOTAL_STAT_SCROLL_X, ptRoot.y + XML_UNION_TOTAL_STAT_SCROLL_Y + XML_UNION_TOTAL_STAT_SCROLL_H - XML_UNION_TOTAL_STAT_SCROLL_ARROW_H ), CsSIZE( XML_UNION_TOTAL_STAT_SCROLL_W, XML_UNION_TOTAL_STAT_SCROLL_ARROW_H ) );
		if( CURSOR_ST.CheckClickBox( rcTotalStatScrollUp ) == CURSOR::LBUTTON_UP )
		{
			if( m_nTotalStatScroll > 0 )
			{
				--m_nTotalStatScroll;
				_UpdateTotalStatControls();
			}
			return muReturn;
		}
		if( CURSOR_ST.CheckClickBox( rcTotalStatScrollDown ) == CURSOR::LBUTTON_UP )
		{
			if( m_nTotalStatScroll < nTotalStatMaxScroll )
			{
				++m_nTotalStatScroll;
				_UpdateTotalStatControls();
			}
			return muReturn;
		}
		if( CURSOR_ST.CheckClickBox( rcTotalStatScroll ) == CURSOR::LBUTTON_UP )
		{
			int const nRelativeY = max( 0, min( XML_UNION_TOTAL_STAT_SCROLL_TRACK_H, ptCursor.y - ( ptRoot.y + XML_UNION_TOTAL_STAT_SCROLL_TRACK_Y ) ) );
			int const nNewScroll = ( nRelativeY * nTotalStatMaxScroll ) / XML_UNION_TOTAL_STAT_SCROLL_TRACK_H;
			if( nNewScroll != m_nTotalStatScroll )
			{
				m_nTotalStatScroll = nNewScroll;
				_UpdateTotalStatControls();
			}
			return muReturn;
		}
		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && CURSOR_ST.CheckClickBox( rcTotalStatArea ) != CURSOR::BUTTON_OUTWINDOW )
		{
			int const nOldScroll = m_nTotalStatScroll;
			if( CURSOR_ST.GetWheel() < 0 && m_nTotalStatScroll < nTotalStatMaxScroll )
				++m_nTotalStatScroll;
			else if( CURSOR_ST.GetWheel() > 0 && m_nTotalStatScroll > 0 )
				--m_nTotalStatScroll;

			if( nOldScroll != m_nTotalStatScroll )
				_UpdateTotalStatControls();
			CURSOR_ST.ResetWheel();
			return muReturn;
		}
	}

	for( int i = 0; i < IF_XML_UNION_FILTER_COUNT; ++i )
	{
		int const nFilteredIndex = m_nFilterScroll + i;
		int const nFavoriteIndex = nFilteredIndex >= 0 && nFilteredIndex < (int)m_vFilteredFilterIndices.size() ? m_vFilteredFilterIndices[ nFilteredIndex ] : -1;
		if( nFavoriteIndex >= 0 )
		{
			CsPoint const ptStar( ptRoot.x + XML_UNION_FILTER_ROW_X + XML_UNION_FILTER_ROW_W - XML_UNION_FILTER_BOOKMARK_SIZE - XML_UNION_FILTER_BOOKMARK_RIGHT_PAD,
				ptRoot.y + XML_UNION_FILTER_ROW_Y + ( i * XML_UNION_FILTER_ROW_STEP ) + ( ( XML_UNION_FILTER_ROW_H - XML_UNION_FILTER_BOOKMARK_SIZE ) / 2 ) + XML_UNION_FILTER_BOOKMARK_Y_OFFSET );
			CsRect const rcStar( ptStar, CsSIZE( XML_UNION_FILTER_BOOKMARK_SIZE + 4, XML_UNION_FILTER_BOOKMARK_SIZE + 4 ) );
			if( CURSOR_ST.CheckClickBox( rcStar ) == CURSOR::LBUTTON_UP )
			{
				_ToggleFilterFavorite( nFavoriteIndex );
				return muReturn;
			}
		}

		if( m_pFilterButtons[ i ] && m_pFilterButtons[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
		{
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
	if( m_bRewardModalVisible )
	{
		std::vector< bool > vSpriteVisible;
		std::vector< bool > vTextVisible;
		std::vector< bool > vButtonVisible;
		_CaptureRewardModalVisibility( vSpriteVisible, vTextVisible, vButtonVisible );
		_SetRewardModalControlsVisible( false );

		RenderScript();
		EndRenderScript();

		_RestoreRewardModalVisibility( vSpriteVisible, vTextVisible, vButtonVisible );
		_RenderRewardModalControls();
		return;
	}

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

void cXmlUnionDUnit::RecvServerProgress( u1 nLevel, n4 nCurrentExperience, n4 nRequiredExperience, u2 nProgressPercentBasisPoints, u2 nClaimedRewardMask )
{
	m_bServerProgressLoaded = true;
	m_nServerXmlUnionLevel = nLevel;
	m_nServerCurrentExperience = nCurrentExperience;
	m_nServerRequiredExperience = nRequiredExperience;
	m_nServerProgressPercentBasisPoints = nProgressPercentBasisPoints;
	m_nServerClaimedRewardMask = nClaimedRewardMask;
	_UpdateServerProgressControls();
	_UpdateRewardModalOptionControls();
}

void cXmlUnionDUnit::RecvRewardClaimResult( u1 nResult, u1 nRewardIndex, u2 nClaimedRewardMask, u1 nLevel, u2 nProgressPercentBasisPoints )
{
	m_bServerProgressLoaded = true;
	m_nServerXmlUnionLevel = nLevel;
	m_nServerProgressPercentBasisPoints = nProgressPercentBasisPoints;
	m_nServerClaimedRewardMask = nClaimedRewardMask;

	if( nRewardIndex < IF_XML_UNION_REWARD_COUNT && m_pRewardReceiveText[ nRewardIndex ] )
	{
		if( nResult == 0 || nResult == 1 )
			m_pRewardReceiveText[ nRewardIndex ]->SetText( _T( "Recebido" ) );
		else
			m_pRewardReceiveText[ nRewardIndex ]->SetText( _T( "Indisponivel" ) );
	}

	_UpdateServerProgressControls();
	_UpdateRewardModalOptionControls();
}

void cXmlUnionDUnit::RecvCollectionInfo()
{
	if( net::game == NULL )
		return;

	m_mapServerGroups.clear();

	u2 nGroupCount = 0;
	net::game->pop( nGroupCount );
	for( u2 nGroup = 0; nGroup < nGroupCount; ++nGroup )
	{
		u4 nGroupID = 0;
		u1 nCompleted = 0;
		u1 nOwnedCount = 0;
		u2 nTotalLevel = 0;
		u1 nAllTranscended = 0;
		u1 nMemberCount = 0;

		net::game->pop( nGroupID );
		net::game->pop( nCompleted );
		net::game->pop( nOwnedCount );
		net::game->pop( nTotalLevel );
		net::game->pop( nAllTranscended );
		net::game->pop( nMemberCount );

		sSERVER_GROUP_STATE groupState;
		groupState.s_bCompleted = nCompleted != 0;
		groupState.s_nOwnedCount = nOwnedCount;
		groupState.s_nTotalLevel = nTotalLevel;
		groupState.s_bAllTranscended = nAllTranscended != 0;

		for( u1 nMember = 0; nMember < nMemberCount; ++nMember )
		{
			u4 nDigimonID = 0;
			u1 nSlot = 0;
			u1 nOwned = 0;
			u2 nLevel = 0;
			u1 nEvolutionUnlocked = 0;
			u1 nTranscended = 0;
			u1 nMeetsLevel = 0;
			u1 nMeetsEvolution = 0;
			u1 nMeetsTranscendence = 0;

			net::game->pop( nDigimonID );
			net::game->pop( nSlot );
			net::game->pop( nOwned );
			net::game->pop( nLevel );
			net::game->pop( nEvolutionUnlocked );
			net::game->pop( nTranscended );
			net::game->pop( nMeetsLevel );
			net::game->pop( nMeetsEvolution );
			net::game->pop( nMeetsTranscendence );

			sSERVER_MEMBER_STATE memberState;
			memberState.s_dwDigimonID = nDigimonID;
			memberState.s_nSlot = nSlot;
			memberState.s_bOwned = nOwned != 0;
			memberState.s_nLevel = nLevel;
			memberState.s_bEvolutionUnlocked = nEvolutionUnlocked != 0;
			memberState.s_bTranscended = nTranscended != 0;
			memberState.s_bMeetsLevel = nMeetsLevel != 0;
			memberState.s_bMeetsEvolution = nMeetsEvolution != 0;
			memberState.s_bMeetsTranscendence = nMeetsTranscendence != 0;
			groupState.s_vMembers.push_back( memberState );
		}

		u1 nEffectCount = 0;
		net::game->pop( nEffectCount );
		for( u1 nEffect = 0; nEffect < nEffectCount; ++nEffect )
		{
			u1 nConditionIndex = 0;
			u1 nActive = 0;
			u1 nEffectType = 0;
			n4 nValue = 0;

			net::game->pop( nConditionIndex );
			net::game->pop( nActive );
			net::game->pop( nEffectType );
			net::game->pop( nValue );

			sSERVER_EFFECT_STATE effectState;
			effectState.s_nConditionIndex = nConditionIndex;
			effectState.s_bActive = nActive != 0;
			effectState.s_nEffectType = nEffectType;
			effectState.s_nValue = nValue;
			groupState.s_vEffects.push_back( effectState );
		}

		m_mapServerGroups[ nGroupID ] = groupState;
	}

	for( int i = 0; i < IF_XML_UNION_STAT_COUNT; ++i )
	{
		n4 nValue = 0;
		net::game->pop( nValue );
		m_nServerBonusValue[ i ] = nValue;
	}

	m_mapCardCache.clear();
	_RefreshView();
	_UpdateServerProgressControls();
}

int cXmlUnionDUnit::GetCollectionBonusValue( int nIndex ) const
{
	if( nIndex < 0 || nIndex >= IF_XML_UNION_STAT_COUNT )
		return 0;

	return m_nServerBonusValue[ nIndex ];
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
	m_vEffectActive.clear();
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

	std::set< int > setInsertedFilters;
	for( size_t nFav = 0; nFav < m_vFavoriteGroupOrder.size(); ++nFav )
	{
		DWORD const dwFavoriteGroupID = m_vFavoriteGroupOrder[ nFav ];
		for( size_t i = 0; i < m_vAllGroups.size() && i < m_vAllFilters.size(); ++i )
		{
			if( m_vAllGroups[ i ].s_dwGroupID == dwFavoriteGroupID && _ContainsNoCase( m_vAllFilters[ i ], m_wsSearchKeyword ) )
			{
				m_vFilteredFilterIndices.push_back( (int)i );
				setInsertedFilters.insert( (int)i );
				break;
			}
		}
	}

	for( size_t i = 0; i < m_vAllFilters.size(); ++i )
	{
		if( setInsertedFilters.find( (int)i ) == setInsertedFilters.end() && _ContainsNoCase( m_vAllFilters[ i ], m_wsSearchKeyword ) )
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
		{
			m_vEffects.push_back( _FormatRewardGoal( selectedGroup, selectedGroup.s_vRewards[ r ], (int)r ) );
			m_vEffectActive.push_back( _IsRewardMetaActive( selectedGroup, selectedGroup.s_vRewards[ r ], (int)r ) );
		}
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
	_UpdateTotalStatControls();
	_UpdateHoverTooltipControls();
}

void cXmlUnionDUnit::_FillCardFromGroup( sCARD_VIEW& card, cUnionXmlTools::sGROUP const& group ) const
{
	card.s_dwGroupID = group.s_dwGroupID;
	card.s_dwDigimonID = 0;
	card.s_dwIconCandidateID = group.s_dwIconCandidateID;
	card.s_nLevel = 0;
	card.s_nRank = group.s_nRank;
	card.s_nSlot = 0;
	card.s_wsName = group.s_wsName;
	card.s_wsOptionName = _MakeOptionText( group.s_wsName );
	card.s_strIconFolder = group.s_strIconFolder;
	card.s_strIconPath = _ResolveIconPath( group );
	_ApplyServerStateToCard( card );
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
	card.s_nLevel = 0;
	card.s_nRank = pInfo->s_nDigimonRank;
	card.s_nSlot = 0;
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

	_ApplyServerStateToCard( card );
	return true;
}

bool cXmlUnionDUnit::_FillCardFromMember( sCARD_VIEW& card, cUnionXmlTools::sMEMBER const& member, cUnionXmlTools::sGROUP const& group ) const
{
	DWORD const dwDigimonID = _FindDigimonByMember( member );
	if( dwDigimonID != 0 && _FillCardFromDigimon( card, dwDigimonID, &group ) )
	{
		card.s_nSlot = member.s_nSlot;
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
		_ApplyServerStateToCard( card );
		return true;
	}

	card.s_dwGroupID = group.s_dwGroupID;
	card.s_dwDigimonID = 0;
	card.s_dwIconCandidateID = member.s_dwIconCandidateID;
	card.s_nLevel = 0;
	card.s_nRank = group.s_nRank;
	card.s_nSlot = member.s_nSlot;
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

	_ApplyServerStateToCard( card );
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

	std::wstring const wsNeedle = _NormalizeDigimonName( member.s_wsName );
	if( member.s_dwIconCandidateID != 0 && wsNeedle.empty() == false )
	{
		for( CsDigimon::MAP_CIT it = pMap->begin(); it != pMap->end(); ++it )
		{
			if( it->second == NULL || it->second->GetInfo() == NULL )
				continue;

			CsDigimon::sINFO* pInfo = it->second->GetInfo();
			if( pInfo->s_dwModelID == member.s_dwIconCandidateID && _NormalizeDigimonName( pInfo->s_szName ) == wsNeedle )
				return it->first;
		}
	}

	if( member.s_wsName.empty() )
	{
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
		return 0;
	}

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
	_AddAssetSprite( CsPoint( 690 + XML_UNION_RIGHT_PANEL_SHIFT, 47 ), CsPoint( 188, 273 ), XML_UNION_BG_RIGHT, NiColorA( 0.02f, 0.10f, 0.16f, 0.88f ) );
	_AddAssetSprite( CsPoint( 690 + XML_UNION_RIGHT_PANEL_SHIFT, 323 ), CsPoint( 188, 291 ), XML_UNION_BG_RIGHT, NiColorA( 0.02f, 0.10f, 0.16f, 0.88f ) );
	_AddAssetSprite( CsPoint( 209, 350 ), CsPoint( 468, 202 ), XML_UNION_BG_INFO, NiColorA( 0.02f, 0.05f, 0.14f, 0.86f ) );
	_AddAssetSprite( CsPoint( 702 + XML_UNION_RIGHT_PANEL_SHIFT, 74 ), CsPoint( 165, 150 ), DUNGEON_RESULT_SIMPLE, NiColorA( 0.01f, 0.05f, 0.10f, 0.75f ) );
	_AddAssetSprite( CsPoint( 752 + XML_UNION_RIGHT_PANEL_SHIFT, 117 ), CsPoint( 64, 64 ), XML_UNION_DIGIMON_HOLD_GAUGE, NiColorA( 0.0f, 0.9f, 1.0f, 0.65f ) );

	m_pRewardTreasureIcon = NiNew cSprite;
	if( m_pRewardTreasureIcon )
	{
		m_pRewardTreasureIcon->Init( GetRoot(), CsPoint( XML_UNION_TREASURE_X + XML_UNION_RIGHT_PANEL_SHIFT, XML_UNION_TREASURE_Y ), CsPoint( XML_UNION_TREASURE_W, XML_UNION_TREASURE_H ), CsRect( 0, 0, 56, 61 ), XML_UNION_TREASURE_ICON, false );
		AddChildControl( m_pRewardTreasureIcon );
	}

	m_pMoveButton = AddButton( CsPoint( 827 + XML_UNION_RIGHT_PANEL_SHIFT, 5 ), CsPoint( 22, 22 ), CsPoint( 0, 22 ), XML_UNION_MOVE_BTN );
	m_pCloseButton = AddButton( CsPoint( 852 + XML_UNION_RIGHT_PANEL_SHIFT, 3 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), XML_UNION_CLOSE_BTN );
	if( m_pCloseButton )
		m_pCloseButton->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cXmlUnionDUnit::PressCloseButton );

	_AddLabel( _T( "D-Unit" ), CsPoint( IF_XML_UNION_WIDTH / 2, 14 ), CFont::FS_12, NiColor( 0.92f, 0.95f, 1.0f ), DT_CENTER );
	m_pStatusText = _AddLabel( m_XmlData.IsLoaded() ? _T( "Data\\union" ) : _T( "XML nao carregado" ), CsPoint( IF_XML_UNION_WIDTH / 2, 30 ), CFont::FS_8, NiColor( 0.4f, 1.0f, 0.8f ), DT_CENTER );

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
		m_pFilterBookmark[ i ] = _AddAssetSpriteRect( CsPoint( ptRow.x + XML_UNION_FILTER_ROW_W - XML_UNION_FILTER_BOOKMARK_SIZE - XML_UNION_FILTER_BOOKMARK_RIGHT_PAD, ptRow.y + ( ( XML_UNION_FILTER_ROW_H - XML_UNION_FILTER_BOOKMARK_SIZE ) / 2 ) + XML_UNION_FILTER_BOOKMARK_Y_OFFSET ), CsPoint( XML_UNION_FILTER_BOOKMARK_SIZE, XML_UNION_FILTER_BOOKMARK_SIZE ), CsRect( 0, 0, 28, 28 ), XML_UNION_BOOKMARK_ICON, NiColorA( 0.25f, 0.27f, 0.30f, 0.55f ) );
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

	m_pProgressPercentText = _AddLabel( _T( "0.0%" ), CsPoint( 784 + XML_UNION_RIGHT_PANEL_SHIFT, 142 ), CFont::FS_10, NiColor( 0.92f, 0.95f, 1.0f ), DT_CENTER );
	_AddSolidSprite( CsPoint( 714 + XML_UNION_RIGHT_PANEL_SHIFT, 258 ), CsPoint( 142, 1 ), NiColorA( 0.0f, 0.95f, 1.0f, 0.95f ) );
	_AddLabel( _T( "Nivel" ), CsPoint( 722 + XML_UNION_RIGHT_PANEL_SHIFT, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );
	m_pProgressLevelValueText = _AddLabel( _T( "Desconhecido" ), CsPoint( 816 + XML_UNION_RIGHT_PANEL_SHIFT, 241 ), CFont::FS_12, NiColor( 0.3f, 1.0f, 1.0f ), DT_CENTER );
	_UpdateServerProgressControls();

	_AddLabel( _T( "Efeitos das Metas" ), CsPoint( 443, 369 ), CFont::FS_10, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	m_pEffectCompletedText = _AddLabel( _T( "0 / 0" ), CsPoint( 484, 369 ), CFont::FS_9, NiColor( 0.92f, 0.95f, 1.0f ), DT_LEFT );
	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
	{
		int const nRowY = 398 + ( i * 23 );
		TCHAR szText[ 160 ] = { 0, };
		if( i < (int)m_vEffects.size() )
			_CopyWideToTChar( m_vEffects[ i ], szText, _countof( szText ) );
		m_pEffectCheckBg[ i ] = _AddSolidSprite( CsPoint( 220, nRowY + 2 ), CsPoint( 14, 14 ), NiColorA( 0.0f, 0.13f, 0.32f, 0.94f ) );
		m_pEffectCheckTop[ i ] = _AddSolidSprite( CsPoint( 220, nRowY + 2 ), CsPoint( 14, 1 ), NiColorA( 0.0f, 0.85f, 1.0f, 0.95f ) );
		m_pEffectCheckBottom[ i ] = _AddSolidSprite( CsPoint( 220, nRowY + 15 ), CsPoint( 14, 1 ), NiColorA( 0.0f, 0.45f, 0.90f, 0.95f ) );
		m_pEffectCheckLeft[ i ] = _AddSolidSprite( CsPoint( 220, nRowY + 2 ), CsPoint( 1, 14 ), NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		m_pEffectCheckRight[ i ] = _AddSolidSprite( CsPoint( 233, nRowY + 2 ), CsPoint( 1, 14 ), NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		m_pEffectCheckMark[ i ] = _AddLabel( _T( "V" ), CsPoint( 227, nRowY ), CFont::FS_10, NiColor( 0.25f, 1.0f, 0.20f ), DT_CENTER );
		m_pEffectStateButton[ i ] = _AddSolidSprite( CsPoint( 240, nRowY ), CsPoint( 32, 16 ), NiColorA( 0.0f, 0.34f, 0.72f, 0.98f ) );
		m_pEffectStateTop[ i ] = _AddSolidSprite( CsPoint( 240, nRowY ), CsPoint( 32, 1 ), NiColorA( 0.0f, 0.85f, 1.0f, 0.95f ) );
		m_pEffectStateBottom[ i ] = _AddSolidSprite( CsPoint( 240, nRowY + 15 ), CsPoint( 32, 1 ), NiColorA( 0.0f, 0.42f, 0.85f, 0.95f ) );
		m_pEffectStateLeft[ i ] = _AddSolidSprite( CsPoint( 240, nRowY ), CsPoint( 1, 16 ), NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		m_pEffectStateRight[ i ] = _AddSolidSprite( CsPoint( 271, nRowY ), CsPoint( 1, 16 ), NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		m_pEffectStateText[ i ] = _AddLabel( _T( "PLAY" ), CsPoint( 256, nRowY + 2 ), CFont::FS_8, NiColor( 0.72f, 1.0f, 1.0f ), DT_CENTER );
		m_pEffectText[ i ] = _AddLabel( szText, CsPoint( 280, nRowY ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
	}
	_UpdateEffectControls();

	_AddLabel( _T( "Efeito Total de Coleta" ), CsPoint( 784 + XML_UNION_RIGHT_PANEL_SHIFT, 357 ), CFont::FS_9, NiColor( 0.0f, 1.0f, 0.25f ), DT_CENTER );
	for( int i = 0; i < IF_XML_UNION_STAT_VISIBLE_COUNT; ++i )
	{
		int const nRowY = XML_UNION_TOTAL_STAT_ROW_Y + ( i * XML_UNION_TOTAL_STAT_ROW_STEP );
		m_pStatNameText[ i ] = _AddLabel( _T( "" ), CsPoint( XML_UNION_TOTAL_STAT_ROW_X, nRowY ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_LEFT );
		m_pStatValueText[ i ] = _AddLabel( _T( "" ), CsPoint( XML_UNION_TOTAL_STAT_VALUE_X, nRowY ), CFont::FS_9, NiColor( 0.88f, 0.92f, 1.0f ), DT_RIGHT );
	}
	_CreateTotalStatScrollBar();
	_UpdateTotalStatControls();
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
			cSprite* pRankIcon = NiNew cSprite;
			if( pRankIcon )
			{
				pRankIcon->Init( GetRoot(), CsPoint( ptCard.x + 11, ptCard.y + 75 ), CsPoint( 82, 66 ), _GetUnionRankIconRect( 0 ), XML_UNION_RANK_ICON, false );
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
	int nCompletedGoals = 0;
	int nTotalGoals = min( (int)m_vEffects.size(), IF_XML_UNION_EFFECT_COUNT );
	for( int i = 0; i < nTotalGoals && i < (int)m_vEffectActive.size(); ++i )
	{
		if( m_vEffectActive[ i ] )
			++nCompletedGoals;
	}

	if( m_pEffectCompletedText )
	{
		TCHAR szCompleted[ 32 ] = { 0, };
		_stprintf_s( szCompleted, _countof( szCompleted ), _T( "%d / %d" ), nCompletedGoals, nTotalGoals );
		m_pEffectCompletedText->SetText( szCompleted );
		m_pEffectCompletedText->SetVisible( nTotalGoals > 0 );
	}

	for( int i = 0; i < IF_XML_UNION_EFFECT_COUNT; ++i )
	{
		if( m_pEffectText[ i ] == NULL )
			continue;

		bool const bHasEffect = i < (int)m_vEffects.size();
		bool const bActive = i < (int)m_vEffectActive.size() && m_vEffectActive[ i ];
		TCHAR szText[ 160 ] = { 0, };
		if( bHasEffect )
		{
			std::wstring wsEffectText = m_vEffects[ i ];
			if( wsEffectText.size() >= 2 && wsEffectText[ 0 ] == L'-' && wsEffectText[ 1 ] == L' ' )
				wsEffectText.erase( 0, 2 );
			_CopyWideToTChar( wsEffectText, szText, _countof( szText ) );
		}
		m_pEffectText[ i ]->SetText( szText );
		m_pEffectText[ i ]->SetColor( bActive ? NiColor( 0.30f, 1.0f, 0.35f ) : NiColor( 0.52f, 0.57f, 0.62f ) );

		if( m_pEffectCheckBg[ i ] )
		{
			m_pEffectCheckBg[ i ]->SetVisible( bHasEffect );
			m_pEffectCheckBg[ i ]->SetColorA( bActive ? NiColorA( 0.0f, 0.18f, 0.42f, 1.0f ) : NiColorA( 0.0f, 0.13f, 0.32f, 0.94f ) );
		}
		if( m_pEffectCheckTop[ i ] )
		{
			m_pEffectCheckTop[ i ]->SetVisible( bHasEffect );
			m_pEffectCheckTop[ i ]->SetColorA( bActive ? NiColorA( 0.20f, 1.0f, 0.28f, 0.95f ) : NiColorA( 0.0f, 0.85f, 1.0f, 0.95f ) );
		}
		if( m_pEffectCheckBottom[ i ] )
		{
			m_pEffectCheckBottom[ i ]->SetVisible( bHasEffect );
			m_pEffectCheckBottom[ i ]->SetColorA( bActive ? NiColorA( 0.10f, 0.72f, 0.18f, 0.95f ) : NiColorA( 0.0f, 0.45f, 0.90f, 0.95f ) );
		}
		if( m_pEffectCheckLeft[ i ] )
		{
			m_pEffectCheckLeft[ i ]->SetVisible( bHasEffect );
			m_pEffectCheckLeft[ i ]->SetColorA( bActive ? NiColorA( 0.16f, 0.90f, 0.24f, 0.95f ) : NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		}
		if( m_pEffectCheckRight[ i ] )
		{
			m_pEffectCheckRight[ i ]->SetVisible( bHasEffect );
			m_pEffectCheckRight[ i ]->SetColorA( bActive ? NiColorA( 0.16f, 0.90f, 0.24f, 0.95f ) : NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		}
		if( m_pEffectCheckMark[ i ] )
		{
			m_pEffectCheckMark[ i ]->SetVisible( bHasEffect && bActive );
			m_pEffectCheckMark[ i ]->SetColor( NiColor( 0.25f, 1.0f, 0.20f ) );
		}
		if( m_pEffectStateButton[ i ] )
		{
			m_pEffectStateButton[ i ]->SetVisible( bHasEffect );
			m_pEffectStateButton[ i ]->SetColorA( bActive ? NiColorA( 0.54f, 0.16f, 0.0f, 0.98f ) : NiColorA( 0.0f, 0.34f, 0.72f, 0.98f ) );
		}
		if( m_pEffectStateTop[ i ] )
		{
			m_pEffectStateTop[ i ]->SetVisible( bHasEffect );
			m_pEffectStateTop[ i ]->SetColorA( bActive ? NiColorA( 1.0f, 0.72f, 0.16f, 0.95f ) : NiColorA( 0.0f, 0.85f, 1.0f, 0.95f ) );
		}
		if( m_pEffectStateBottom[ i ] )
		{
			m_pEffectStateBottom[ i ]->SetVisible( bHasEffect );
			m_pEffectStateBottom[ i ]->SetColorA( bActive ? NiColorA( 0.78f, 0.28f, 0.0f, 0.95f ) : NiColorA( 0.0f, 0.42f, 0.85f, 0.95f ) );
		}
		if( m_pEffectStateLeft[ i ] )
		{
			m_pEffectStateLeft[ i ]->SetVisible( bHasEffect );
			m_pEffectStateLeft[ i ]->SetColorA( bActive ? NiColorA( 0.95f, 0.48f, 0.04f, 0.95f ) : NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		}
		if( m_pEffectStateRight[ i ] )
		{
			m_pEffectStateRight[ i ]->SetVisible( bHasEffect );
			m_pEffectStateRight[ i ]->SetColorA( bActive ? NiColorA( 0.95f, 0.48f, 0.04f, 0.95f ) : NiColorA( 0.0f, 0.65f, 1.0f, 0.95f ) );
		}
		if( m_pEffectStateText[ i ] )
		{
			m_pEffectStateText[ i ]->SetVisible( bHasEffect );
			m_pEffectStateText[ i ]->SetText( bActive ? _T( "OK" ) : _T( "PLAY" ) );
			m_pEffectStateText[ i ]->SetColor( bActive ? NiColor( 1.0f, 0.96f, 0.20f ) : NiColor( 0.72f, 1.0f, 1.0f ) );
		}
	}
}

void cXmlUnionDUnit::_UpdateCardControls()
{
	for( int i = 0; i < IF_XML_UNION_CARD_COUNT; ++i )
	{
		CsPoint const ptCard = CARD_POS[ i ];
		int const nCardIndex = m_nCardScroll + i;
		bool const bHasCard = nCardIndex >= 0 && nCardIndex < (int)m_vCards.size();
		bool const bOwnedCard = bHasCard && m_vCards[ nCardIndex ].s_bOwned;
		bool bCompletedGroup = false;
		if( bHasCard )
		{
			std::map< DWORD, sSERVER_GROUP_STATE >::const_iterator itGroup = m_mapServerGroups.find( m_vCards[ nCardIndex ].s_dwGroupID );
			bCompletedGroup = itGroup != m_mapServerGroups.end() && itGroup->second.s_bCompleted;
		}
		bool const bFullCard = bOwnedCard || bCompletedGroup;

		if( m_pCardBg[ i ] )
		{
			m_pCardBg[ i ]->SetVisible( true );
			m_pCardBg[ i ]->SetAlpha( bFullCard ? 1.0f : 0.42f );
		}
		if( m_pCardFrame[ i ] )
		{
			char const* pFrame = XML_UNION_FRAME_N;
			CsRect rcFrame( 0, 0, 126, 150 );
			if( bHasCard )
			{
				if( bCompletedGroup )
				{
					pFrame = XML_UNION_FRAME_COMPLETED;
					rcFrame = CsRect( 0, 0, 138, 174 );
				}
				else if( m_vCards[ nCardIndex ].s_nRank >= 9 )
					pFrame = XML_UNION_FRAME_U;
				else if( m_vCards[ nCardIndex ].s_nRank >= 3 )
					pFrame = XML_UNION_FRAME_S;
			}
			if( _FileExists( pFrame ) && m_strCardFramePath[ i ] != pFrame )
			{
				m_pCardFrame[ i ]->ChangeTexture( pFrame, rcFrame );
				m_strCardFramePath[ i ] = pFrame;
			}
			m_pCardFrame[ i ]->SetVisible( true );
			m_pCardFrame[ i ]->SetAlpha( bHasCard ? ( bFullCard ? 1.0f : 0.58f ) : 0.35f );
		}

		bool const bShowIcon = bFullCard && m_vCards[ nCardIndex ].s_strIconPath.empty() == false && _FileExists( m_vCards[ nCardIndex ].s_strIconPath.c_str() );
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
			m_pCardRankIcon[ i ]->SetAlpha( bHasCard ? 1.0f : 0.0f );
			if( bHasCard )
				m_pCardRankIcon[ i ]->ChangeTexture( XML_UNION_RANK_ICON, _GetUnionRankIconRect( m_vCards[ nCardIndex ].s_nRank ) );
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

void cXmlUnionDUnit::_CreateRewardModalControls()
{
	m_vRewardModalSprites.clear();
	m_vRewardModalTexts.clear();
	m_vRewardModalButtons.clear();
	m_pRewardModalBg = NULL;
	m_pRewardModalTitleBg = NULL;
	m_pRewardModalBorderTop = NULL;
	m_pRewardModalBorderBottom = NULL;
	m_pRewardModalBorderLeft = NULL;
	m_pRewardModalBorderRight = NULL;
	m_pRewardModalContentLine = NULL;
	m_pRewardModalTitleText = NULL;
	m_pRewardModalCloseButton = NULL;
	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		m_pRewardOptionGlow[ i ] = NULL;
		m_pRewardOptionBody[ i ] = NULL;
		m_pRewardOptionGauge[ i ] = NULL;
		m_pRewardReceiveHoverBg[ i ] = NULL;
		m_pRewardReceiveBorderTop[ i ] = NULL;
		m_pRewardReceiveBorderBottom[ i ] = NULL;
		m_pRewardReceiveBorderLeft[ i ] = NULL;
		m_pRewardReceiveBorderRight[ i ] = NULL;
		m_pRewardOptionNameText[ i ] = NULL;
		m_pRewardReceiveText[ i ] = NULL;
		m_pRewardReceiveButton[ i ] = NULL;
	}
	m_pRewardScrollBorder = NULL;
	m_pRewardScrollBg = NULL;
	m_pRewardScrollTrack = NULL;
	m_pRewardScrollThumb = NULL;
	m_pRewardScrollUp = NULL;
	m_pRewardScrollDown = NULL;
	m_nRewardModalScrollRow = 0;
	m_nRewardReceiveHoverIndex = -1;
	m_bRewardModalDragging = false;
	m_ptRewardModalPos = CsPoint( XML_UNION_REWARD_MODAL_X, XML_UNION_REWARD_MODAL_Y );

	int const nX = m_ptRewardModalPos.x;
	int const nY = m_ptRewardModalPos.y;
	int const nW = XML_UNION_REWARD_MODAL_W;
	int const nH = XML_UNION_REWARD_MODAL_H;

	m_pRewardModalBg = _AddSolidSprite( CsPoint( nX, nY ), CsPoint( nW, nH ), NiColorA( 0.00f, 0.018f, 0.038f, 1.0f ) );
	m_pRewardModalTitleBg = _AddSolidSprite( CsPoint( nX, nY ), CsPoint( nW, 34 ), NiColorA( 0.00f, 0.36f, 0.62f, 1.0f ) );
	m_pRewardModalBorderTop = _AddSolidSprite( CsPoint( nX, nY ), CsPoint( nW, 1 ), NiColorA( 0.15f, 0.95f, 1.0f, 0.95f ) );
	m_pRewardModalBorderBottom = _AddSolidSprite( CsPoint( nX, nY + nH - 1 ), CsPoint( nW, 1 ), NiColorA( 0.02f, 0.36f, 0.65f, 0.95f ) );
	m_pRewardModalBorderLeft = _AddSolidSprite( CsPoint( nX, nY ), CsPoint( 1, nH ), NiColorA( 0.02f, 0.36f, 0.65f, 0.95f ) );
	m_pRewardModalBorderRight = _AddSolidSprite( CsPoint( nX + nW - 1, nY ), CsPoint( 1, nH ), NiColorA( 0.02f, 0.36f, 0.65f, 0.95f ) );
	m_pRewardModalContentLine = _AddSolidSprite( CsPoint( nX + 18, nY + 46 ), CsPoint( nW - 50, 1 ), NiColorA( 0.0f, 0.22f, 0.36f, 0.85f ) );
	m_vRewardModalSprites.push_back( m_pRewardModalBg );
	m_vRewardModalSprites.push_back( m_pRewardModalTitleBg );
	m_vRewardModalSprites.push_back( m_pRewardModalBorderTop );
	m_vRewardModalSprites.push_back( m_pRewardModalBorderBottom );
	m_vRewardModalSprites.push_back( m_pRewardModalBorderLeft );
	m_vRewardModalSprites.push_back( m_pRewardModalBorderRight );
	m_vRewardModalSprites.push_back( m_pRewardModalContentLine );

	m_pRewardModalTitleText = _AddLabel( _T( "Recompensa" ), CsPoint( nX + ( nW / 2 ), nY + 8 ), CFont::FS_12, NiColor( 0.78f, 1.0f, 1.0f ), DT_CENTER );
	m_vRewardModalTexts.push_back( m_pRewardModalTitleText );

	m_pRewardModalCloseButton = AddButton( CsPoint( nX + nW - 37, nY + 1 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), XML_UNION_CLOSE_BTN );
	if( m_pRewardModalCloseButton )
		m_vRewardModalButtons.push_back( m_pRewardModalCloseButton );

	int const nScrollX = nX + nW - 17;
	int const nScrollY = nY + 38;
	int const nScrollTrackY = nScrollY + XML_UNION_REWARD_SCROLL_ARROW_H;
	m_pRewardScrollBorder = _AddSolidSprite( CsPoint( nScrollX - 1, nScrollY - 1 ), CsPoint( XML_UNION_REWARD_SCROLL_W + 2, XML_UNION_REWARD_SCROLL_H + 2 ), NiColorA( 0.0f, 0.65f, 1.0f, 0.70f ) );
	m_pRewardScrollBg = _AddSolidSprite( CsPoint( nScrollX, nScrollY ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_H ), NiColorA( 0.00f, 0.04f, 0.08f, 0.95f ) );
	m_pRewardScrollUp = _AddAssetSprite( CsPoint( nScrollX, nScrollY ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ), XML_UNION_SCROLL_UP, NiColorA( 0.0f, 0.55f, 0.85f, 0.95f ) );
	m_pRewardScrollTrack = _AddSolidSprite( CsPoint( nScrollX + 3, nScrollTrackY ), CsPoint( XML_UNION_REWARD_SCROLL_W - 6, XML_UNION_REWARD_SCROLL_TRACK_H ), NiColorA( 0.0f, 0.22f, 0.40f, 0.90f ) );
	m_pRewardScrollThumb = _AddSolidSprite( CsPoint( nScrollX + 1, nScrollTrackY ), CsPoint( XML_UNION_REWARD_SCROLL_W - 2, XML_UNION_REWARD_SCROLL_TRACK_H / 2 ), NiColorA( 0.0f, 0.72f, 1.0f, 0.95f ) );
	m_pRewardScrollDown = _AddAssetSprite( CsPoint( nScrollX, nScrollY + XML_UNION_REWARD_SCROLL_H - XML_UNION_REWARD_SCROLL_ARROW_H ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ), XML_UNION_SCROLL_DOWN, NiColorA( 0.0f, 0.55f, 0.85f, 0.95f ) );
	m_vRewardModalSprites.push_back( m_pRewardScrollBorder );
	m_vRewardModalSprites.push_back( m_pRewardScrollBg );
	m_vRewardModalSprites.push_back( m_pRewardScrollUp );
	m_vRewardModalSprites.push_back( m_pRewardScrollTrack );
	m_vRewardModalSprites.push_back( m_pRewardScrollThumb );
	m_vRewardModalSprites.push_back( m_pRewardScrollDown );

	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		NiColorA const& glow = REWARD_MODAL_OPTIONS[ i ].s_GlowColor;

		m_pRewardOptionBody[ i ] = _AddAssetSprite( CsPoint( 0, 0 ), CsPoint( 140, 128 ), DUNGEON_RESULT_SIMPLE, NiColorA( 0.01f, 0.06f, 0.12f, 1.0f ) );
		m_pRewardOptionGlow[ i ] = _AddAssetSprite( CsPoint( 0, 0 ), CsPoint( 164, 150 ), DUNGEON_RESULT_SIMPLE, glow );
		if( m_pRewardOptionGlow[ i ] )
			m_pRewardOptionGlow[ i ]->SetColorA( glow );
		m_pRewardOptionGauge[ i ] = _AddAssetSprite( CsPoint( 0, 0 ), CsPoint( 66, 66 ), XML_UNION_DIGIMON_HOLD_GAUGE, NiColorA( 0.0f, 0.9f, 1.0f, 0.70f ) );
		m_pRewardReceiveHoverBg[ i ] = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 108, 38 ), NiColorA( 0.02f, 0.22f, 0.36f, 1.0f ) );
		m_pRewardReceiveBorderTop[ i ] = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 104, 1 ), NiColorA( 0.28f, 0.78f, 1.0f, 0.95f ) );
		m_pRewardReceiveBorderBottom[ i ] = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 104, 1 ), NiColorA( 0.02f, 0.38f, 0.72f, 0.95f ) );
		m_pRewardReceiveBorderLeft[ i ] = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 34 ), NiColorA( 0.18f, 0.58f, 0.92f, 0.88f ) );
		m_pRewardReceiveBorderRight[ i ] = _AddSolidSprite( CsPoint( 0, 0 ), CsPoint( 1, 34 ), NiColorA( 0.18f, 0.58f, 0.92f, 0.88f ) );
		m_pRewardOptionNameText[ i ] = _AddLabel( REWARD_MODAL_OPTIONS[ i ].s_pName, CsPoint( 0, 0 ), CFont::FS_10, NiColor( 0.98f, 0.98f, 1.0f ), DT_CENTER );
		m_pRewardReceiveButton[ i ] = AddButton( CsPoint( 0, 0 ), CsPoint( 98, 34 ), CsPoint( 0, 28 ), XML_UNION_LIST_BUTTON );
		m_pRewardReceiveText[ i ] = _AddLabel( _T( "Receber" ), CsPoint( 0, 0 ), CFont::FS_10, NiColor( 0.98f, 0.98f, 1.0f ), DT_CENTER );

		m_vRewardModalSprites.push_back( m_pRewardOptionBody[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardOptionGlow[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardOptionGauge[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardReceiveHoverBg[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardReceiveBorderTop[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardReceiveBorderBottom[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardReceiveBorderLeft[ i ] );
		m_vRewardModalSprites.push_back( m_pRewardReceiveBorderRight[ i ] );
		m_vRewardModalTexts.push_back( m_pRewardOptionNameText[ i ] );
		if( m_pRewardReceiveButton[ i ] )
			m_vRewardModalButtons.push_back( m_pRewardReceiveButton[ i ] );
		m_vRewardModalTexts.push_back( m_pRewardReceiveText[ i ] );
	}

	_UpdateRewardModalFrameControls();
	_UpdateRewardModalOptionControls();
	_UpdateRewardModalScrollControls();
	_SetRewardModalVisible( false );
}

void cXmlUnionDUnit::_SetRewardModalVisible( bool bVisible )
{
	m_bRewardModalVisible = bVisible;
	m_bRewardModalDragging = false;

	if( bVisible )
	{
		_UpdateRewardModalFrameControls();
		_UpdateRewardModalOptionControls();
		_UpdateRewardModalScrollControls();
	}
	else
	{
		m_nRewardReceiveHoverIndex = -1;
	}

	for( size_t i = 0; i < m_vRewardModalSprites.size(); ++i )
	{
		if( m_vRewardModalSprites[ i ] )
			m_vRewardModalSprites[ i ]->SetVisible( bVisible );
	}
	for( size_t i = 0; i < m_vRewardModalTexts.size(); ++i )
	{
		if( m_vRewardModalTexts[ i ] )
			m_vRewardModalTexts[ i ]->SetVisible( bVisible );
	}
	for( size_t i = 0; i < m_vRewardModalButtons.size(); ++i )
	{
		if( m_vRewardModalButtons[ i ] )
			m_vRewardModalButtons[ i ]->SetVisible( bVisible );
	}

	if( bVisible )
	{
		_UpdateRewardModalFrameControls();
		_UpdateRewardModalOptionControls();
		_UpdateRewardModalScrollControls();
		_UpdateRewardReceiveButtonHover( -1 );
	}
}

void cXmlUnionDUnit::_SetRewardModalControlsVisible( bool bVisible )
{
	for( size_t i = 0; i < m_vRewardModalSprites.size(); ++i )
	{
		if( m_vRewardModalSprites[ i ] )
			m_vRewardModalSprites[ i ]->SetVisible( bVisible );
	}
	for( size_t i = 0; i < m_vRewardModalTexts.size(); ++i )
	{
		if( m_vRewardModalTexts[ i ] )
			m_vRewardModalTexts[ i ]->SetVisible( bVisible );
	}
	for( size_t i = 0; i < m_vRewardModalButtons.size(); ++i )
	{
		if( m_vRewardModalButtons[ i ] )
			m_vRewardModalButtons[ i ]->SetVisible( bVisible );
	}
}

void cXmlUnionDUnit::_CaptureRewardModalVisibility( std::vector< bool >& vSprites, std::vector< bool >& vTexts, std::vector< bool >& vButtons ) const
{
	vSprites.clear();
	vTexts.clear();
	vButtons.clear();
	vSprites.reserve( m_vRewardModalSprites.size() );
	vTexts.reserve( m_vRewardModalTexts.size() );
	vButtons.reserve( m_vRewardModalButtons.size() );

	for( size_t i = 0; i < m_vRewardModalSprites.size(); ++i )
		vSprites.push_back( m_vRewardModalSprites[ i ] && m_vRewardModalSprites[ i ]->GetVisible() );
	for( size_t i = 0; i < m_vRewardModalTexts.size(); ++i )
		vTexts.push_back( m_vRewardModalTexts[ i ] && m_vRewardModalTexts[ i ]->GetVisible() );
	for( size_t i = 0; i < m_vRewardModalButtons.size(); ++i )
		vButtons.push_back( m_vRewardModalButtons[ i ] && m_vRewardModalButtons[ i ]->GetVisible() );
}

void cXmlUnionDUnit::_RestoreRewardModalVisibility( std::vector< bool > const& vSprites, std::vector< bool > const& vTexts, std::vector< bool > const& vButtons )
{
	for( size_t i = 0; i < m_vRewardModalSprites.size() && i < vSprites.size(); ++i )
	{
		if( m_vRewardModalSprites[ i ] )
			m_vRewardModalSprites[ i ]->SetVisible( vSprites[ i ] );
	}
	for( size_t i = 0; i < m_vRewardModalTexts.size() && i < vTexts.size(); ++i )
	{
		if( m_vRewardModalTexts[ i ] )
			m_vRewardModalTexts[ i ]->SetVisible( vTexts[ i ] );
	}
	for( size_t i = 0; i < m_vRewardModalButtons.size() && i < vButtons.size(); ++i )
	{
		if( m_vRewardModalButtons[ i ] )
			m_vRewardModalButtons[ i ]->SetVisible( vButtons[ i ] );
	}
}

void cXmlUnionDUnit::_RenderRewardModalControls()
{
	if( g_pEngine )
		g_pEngine->ScreenSpace();

	for( size_t i = 0; i < m_vRewardModalSprites.size(); ++i )
	{
		if( m_vRewardModalSprites[ i ] && m_vRewardModalSprites[ i ]->GetVisible() )
			m_vRewardModalSprites[ i ]->Render();
	}
	for( size_t i = 0; i < m_vRewardModalButtons.size(); ++i )
	{
		if( m_vRewardModalButtons[ i ] && m_vRewardModalButtons[ i ]->GetVisible() )
			m_vRewardModalButtons[ i ]->Render();
	}
	for( size_t i = 0; i < m_vRewardModalTexts.size(); ++i )
	{
		if( m_vRewardModalTexts[ i ] && m_vRewardModalTexts[ i ]->GetVisible() )
			m_vRewardModalTexts[ i ]->Render();
	}
}

void cXmlUnionDUnit::_UpdateRewardModalFrameControls()
{
	int const nX = m_ptRewardModalPos.x;
	int const nY = m_ptRewardModalPos.y;
	int const nW = XML_UNION_REWARD_MODAL_W;
	int const nH = XML_UNION_REWARD_MODAL_H;
	int const nScrollX = nX + nW - 17;
	int const nScrollY = nY + 38;
	int const nScrollTrackY = nScrollY + XML_UNION_REWARD_SCROLL_ARROW_H;

	if( m_pRewardModalBg ) m_pRewardModalBg->SetPosSize( CsPoint( nX, nY ), CsPoint( nW, nH ) );
	if( m_pRewardModalTitleBg ) m_pRewardModalTitleBg->SetPosSize( CsPoint( nX, nY ), CsPoint( nW, 34 ) );
	if( m_pRewardModalBorderTop ) m_pRewardModalBorderTop->SetPosSize( CsPoint( nX, nY ), CsPoint( nW, 1 ) );
	if( m_pRewardModalBorderBottom ) m_pRewardModalBorderBottom->SetPosSize( CsPoint( nX, nY + nH - 1 ), CsPoint( nW, 1 ) );
	if( m_pRewardModalBorderLeft ) m_pRewardModalBorderLeft->SetPosSize( CsPoint( nX, nY ), CsPoint( 1, nH ) );
	if( m_pRewardModalBorderRight ) m_pRewardModalBorderRight->SetPosSize( CsPoint( nX + nW - 1, nY ), CsPoint( 1, nH ) );
	if( m_pRewardModalContentLine ) m_pRewardModalContentLine->SetPosSize( CsPoint( nX + 18, nY + 46 ), CsPoint( nW - 50, 1 ) );
	if( m_pRewardModalTitleText ) m_pRewardModalTitleText->SetPos( CsPoint( nX + ( nW / 2 ), nY + 8 ) );
	if( m_pRewardModalCloseButton ) m_pRewardModalCloseButton->SetPos( CsPoint( nX + nW - 37, nY + 1 ) );

	if( m_pRewardScrollBorder ) m_pRewardScrollBorder->SetPosSize( CsPoint( nScrollX - 1, nScrollY - 1 ), CsPoint( XML_UNION_REWARD_SCROLL_W + 2, XML_UNION_REWARD_SCROLL_H + 2 ) );
	if( m_pRewardScrollBg ) m_pRewardScrollBg->SetPosSize( CsPoint( nScrollX, nScrollY ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_H ) );
	if( m_pRewardScrollUp ) m_pRewardScrollUp->SetPosSize( CsPoint( nScrollX, nScrollY ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ) );
	if( m_pRewardScrollTrack ) m_pRewardScrollTrack->SetPosSize( CsPoint( nScrollX + 3, nScrollTrackY ), CsPoint( XML_UNION_REWARD_SCROLL_W - 6, XML_UNION_REWARD_SCROLL_TRACK_H ) );
	if( m_pRewardScrollDown ) m_pRewardScrollDown->SetPosSize( CsPoint( nScrollX, nScrollY + XML_UNION_REWARD_SCROLL_H - XML_UNION_REWARD_SCROLL_ARROW_H ), CsPoint( XML_UNION_REWARD_SCROLL_W, XML_UNION_REWARD_SCROLL_ARROW_H ) );
}

void cXmlUnionDUnit::_UpdateRewardModalOptionControls()
{
	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		int const nOriginalRow = i / 3;
		int const nDisplayRow = nOriginalRow - m_nRewardModalScrollRow;
		bool const bVisible = m_bRewardModalVisible && nDisplayRow >= 0 && nDisplayRow < XML_UNION_REWARD_VISIBLE_ROWS;
		int const nCol = i % 3;
		int const nCenterX = m_ptRewardModalPos.x + 98 + ( nCol * 200 );
		int const nIconY = m_ptRewardModalPos.y + 67 + ( nDisplayRow * 245 );

		if( m_pRewardOptionGlow[ i ] )
		{
			m_pRewardOptionGlow[ i ]->SetPos( CsPoint( nCenterX - 82, nIconY - 11 ) );
			m_pRewardOptionGlow[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardOptionBody[ i ] )
		{
			m_pRewardOptionBody[ i ]->SetPos( CsPoint( nCenterX - 70, nIconY ) );
			m_pRewardOptionBody[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardOptionGauge[ i ] )
		{
			m_pRewardOptionGauge[ i ]->SetPos( CsPoint( nCenterX - 33, nIconY + 32 ) );
			m_pRewardOptionGauge[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveHoverBg[ i ] )
		{
			m_pRewardReceiveHoverBg[ i ]->SetPos( CsPoint( nCenterX - 54, nIconY + 176 ) );
			m_pRewardReceiveHoverBg[ i ]->SetVisible( bVisible );
			m_pRewardReceiveHoverBg[ i ]->SetColorA( i == m_nRewardReceiveHoverIndex ? NiColorA( 0.06f, 0.36f, 0.58f, 1.0f ) : NiColorA( 0.02f, 0.22f, 0.36f, 1.0f ) );
		}
		if( m_pRewardReceiveBorderTop[ i ] )
		{
			m_pRewardReceiveBorderTop[ i ]->SetPos( CsPoint( nCenterX - 52, nIconY + 176 ) );
			m_pRewardReceiveBorderTop[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveBorderBottom[ i ] )
		{
			m_pRewardReceiveBorderBottom[ i ]->SetPos( CsPoint( nCenterX - 52, nIconY + 213 ) );
			m_pRewardReceiveBorderBottom[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveBorderLeft[ i ] )
		{
			m_pRewardReceiveBorderLeft[ i ]->SetPos( CsPoint( nCenterX - 54, nIconY + 178 ) );
			m_pRewardReceiveBorderLeft[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveBorderRight[ i ] )
		{
			m_pRewardReceiveBorderRight[ i ]->SetPos( CsPoint( nCenterX + 53, nIconY + 178 ) );
			m_pRewardReceiveBorderRight[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardOptionNameText[ i ] )
		{
			m_pRewardOptionNameText[ i ]->SetPos( CsPoint( nCenterX, nIconY + 149 ) );
			m_pRewardOptionNameText[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveButton[ i ] )
		{
			m_pRewardReceiveButton[ i ]->SetPos( CsPoint( nCenterX - 49, nIconY + 179 ) );
			m_pRewardReceiveButton[ i ]->SetVisible( bVisible );
		}
		if( m_pRewardReceiveText[ i ] )
		{
			m_pRewardReceiveText[ i ]->SetText( _IsRewardClaimed( i ) ? _T( "Recebido" ) : _T( "Receber" ) );
			m_pRewardReceiveText[ i ]->SetPos( CsPoint( nCenterX, nIconY + 189 ) );
			m_pRewardReceiveText[ i ]->SetVisible( bVisible );
		}
	}
}

void cXmlUnionDUnit::_UpdateRewardModalScrollControls()
{
	bool const bVisible = m_bRewardModalVisible;
	if( m_pRewardScrollBorder ) m_pRewardScrollBorder->SetVisible( bVisible );
	if( m_pRewardScrollBg ) m_pRewardScrollBg->SetVisible( bVisible );
	if( m_pRewardScrollTrack ) m_pRewardScrollTrack->SetVisible( bVisible );
	if( m_pRewardScrollUp ) m_pRewardScrollUp->SetVisible( bVisible );
	if( m_pRewardScrollDown ) m_pRewardScrollDown->SetVisible( bVisible );
	if( m_pRewardScrollThumb )
	{
		int const nScrollX = m_ptRewardModalPos.x + XML_UNION_REWARD_MODAL_W - 17;
		int const nScrollY = m_ptRewardModalPos.y + 38;
		int const nScrollTrackY = nScrollY + XML_UNION_REWARD_SCROLL_ARROW_H;
		int const nThumbH = XML_UNION_REWARD_SCROLL_TRACK_H / 2;
		int const nThumbY = nScrollTrackY + ( m_nRewardModalScrollRow > 0 ? XML_UNION_REWARD_SCROLL_TRACK_H - nThumbH : 0 );
		m_pRewardScrollThumb->SetPosSize( CsPoint( nScrollX + 1, nThumbY ), CsPoint( XML_UNION_REWARD_SCROLL_W - 2, nThumbH ) );
		m_pRewardScrollThumb->SetVisible( bVisible );
	}
}

void cXmlUnionDUnit::_UpdateServerProgressControls()
{
	if( m_pProgressPercentText )
	{
		TCHAR szPercent[ 32 ] = { 0, };
		_stprintf_s( szPercent, _countof( szPercent ), _T( "%u.%02u%%" ),
			(unsigned int)( m_nServerProgressPercentBasisPoints / 100 ),
			(unsigned int)( m_nServerProgressPercentBasisPoints % 100 ) );
		m_pProgressPercentText->SetText( szPercent );
	}

	if( m_pProgressLevelValueText )
	{
		TCHAR szLevel[ 32 ] = { 0, };
		if( m_bServerProgressLoaded )
			_stprintf_s( szLevel, _countof( szLevel ), _T( "%u" ), (unsigned int)m_nServerXmlUnionLevel );
		else
			_tcscpy_s( szLevel, _countof( szLevel ), _T( "Desconhecido" ) );
		m_pProgressLevelValueText->SetText( szLevel );
	}

	_UpdateTotalStatControls();
}

bool cXmlUnionDUnit::_IsRewardClaimed( int nRewardIndex ) const
{
	if( nRewardIndex < 0 || nRewardIndex >= 16 )
		return false;

	return ( m_nServerClaimedRewardMask & ( 1 << nRewardIndex ) ) != 0;
}

cXmlUnionDUnit::sSERVER_MEMBER_STATE const* cXmlUnionDUnit::_FindServerMemberState( DWORD dwGroupID, DWORD dwDigimonID, int nSlot ) const
{
	std::map< DWORD, sSERVER_GROUP_STATE >::const_iterator itGroup = m_mapServerGroups.find( dwGroupID );
	if( itGroup == m_mapServerGroups.end() )
		return NULL;

	for( size_t i = 0; i < itGroup->second.s_vMembers.size(); ++i )
	{
		sSERVER_MEMBER_STATE const& state = itGroup->second.s_vMembers[ i ];
		if( dwDigimonID != 0 && state.s_dwDigimonID == dwDigimonID )
			return &state;
	}

	for( size_t i = 0; i < itGroup->second.s_vMembers.size(); ++i )
	{
		sSERVER_MEMBER_STATE const& state = itGroup->second.s_vMembers[ i ];
		if( nSlot > 0 && state.s_nSlot == nSlot )
			return &state;
	}

	return NULL;
}

bool cXmlUnionDUnit::_IsRewardMetaActive( cUnionXmlTools::sGROUP const& group, cUnionXmlTools::sREWARD const& reward, int nRewardIndex ) const
{
	std::map< DWORD, sSERVER_GROUP_STATE >::const_iterator itGroup = m_mapServerGroups.find( group.s_dwGroupID );
	if( itGroup == m_mapServerGroups.end() )
		return false;

	int const nSlot = reward.s_nSlot > 0 ? reward.s_nSlot : nRewardIndex + 1;
	for( size_t i = 0; i < itGroup->second.s_vEffects.size(); ++i )
	{
		sSERVER_EFFECT_STATE const& state = itGroup->second.s_vEffects[ i ];
		if( state.s_nConditionIndex == nSlot )
			return state.s_bActive;
	}

	if( nRewardIndex >= 0 && nRewardIndex < (int)itGroup->second.s_vEffects.size() )
		return itGroup->second.s_vEffects[ nRewardIndex ].s_bActive;

	return false;
}

std::wstring cXmlUnionDUnit::_FormatRewardGoal( cUnionXmlTools::sGROUP const& group, cUnionXmlTools::sREWARD const& reward, int nRewardIndex ) const
{
	std::wstring wsReward = _FormatReward( reward );
	if( wsReward.size() >= 2 && wsReward[ 0 ] == L'-' && wsReward[ 1 ] == L' ' )
		wsReward.erase( 0, 2 );

	int const nSlot = reward.s_nSlot > 0 ? reward.s_nSlot : nRewardIndex + 1;
	int const nMemberCount = group.s_nMemberCount > 0 ? group.s_nMemberCount : (int)group.s_vMembers.size();
	int const nLevelReq1 = group.s_nLevelReq1 > 0 ? group.s_nLevelReq1 : group.s_nLevel;
	int const nLevelReq2 = group.s_nLevelReq2 > 0 ? group.s_nLevelReq2 : nLevelReq1;

	wchar_t szText[ 256 ] = { 0, };
	switch( nSlot )
	{
	case 1:
		swprintf_s( szText, _countof( szText ), L"Obtido %d Digimons, %s", nMemberCount, wsReward.c_str() );
		break;
	case 2:
		swprintf_s( szText, _countof( szText ), L"Nível Total dos Digimons %d, %s", nLevelReq1, wsReward.c_str() );
		break;
	case 3:
		swprintf_s( szText, _countof( szText ), L"Obtido %d Digimons Transcendidos, %s", nMemberCount, wsReward.c_str() );
		break;
	case 4:
		swprintf_s( szText, _countof( szText ), L"Nível Total dos Digimons %d, %s", nLevelReq2, wsReward.c_str() );
		break;
	default:
		swprintf_s( szText, _countof( szText ), L"%s", wsReward.c_str() );
		break;
	}

	return szText;
}

void cXmlUnionDUnit::_ApplyServerStateToCard( sCARD_VIEW& card ) const
{
	sSERVER_MEMBER_STATE const* pState = _FindServerMemberState( card.s_dwGroupID, card.s_dwDigimonID, card.s_nSlot );
	if( pState == NULL )
	{
		card.s_bOwned = false;
		card.s_bEvolutionUnlocked = false;
		card.s_bMeetsLevel = false;
		card.s_bMeetsEvolution = false;
		card.s_bMeetsTranscendence = false;
		card.s_nLevel = 0;
		return;
	}

	card.s_bOwned = pState->s_bOwned && pState->s_bEvolutionUnlocked;
	card.s_bEvolutionUnlocked = pState->s_bEvolutionUnlocked;
	card.s_bMeetsLevel = pState->s_bMeetsLevel;
	card.s_bMeetsEvolution = pState->s_bMeetsEvolution;
	card.s_bMeetsTranscendence = pState->s_bMeetsTranscendence;
	card.s_nLevel = card.s_bOwned ? pState->s_nLevel : 0;
}

void cXmlUnionDUnit::_UpdateRewardReceiveButtonHover( int nHoverIndex )
{
	if( nHoverIndex == m_nRewardReceiveHoverIndex )
		return;

	m_nRewardReceiveHoverIndex = nHoverIndex;
	for( int i = 0; i < IF_XML_UNION_REWARD_COUNT; ++i )
	{
		if( m_pRewardReceiveHoverBg[ i ] == NULL )
			continue;

		int const nOriginalRow = i / 3;
		int const nDisplayRow = nOriginalRow - m_nRewardModalScrollRow;
		bool const bVisible = m_bRewardModalVisible && nDisplayRow >= 0 && nDisplayRow < XML_UNION_REWARD_VISIBLE_ROWS;
		m_pRewardReceiveHoverBg[ i ]->SetVisible( bVisible );
		m_pRewardReceiveHoverBg[ i ]->SetColorA( i == m_nRewardReceiveHoverIndex ? NiColorA( 0.06f, 0.36f, 0.58f, 1.0f ) : NiColorA( 0.02f, 0.22f, 0.36f, 1.0f ) );
	}
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

bool cXmlUnionDUnit::_IsFilterFavorite( int nFilterIndex ) const
{
	if( nFilterIndex < 0 || nFilterIndex >= (int)m_vAllGroups.size() )
		return false;

	return _IsFavoriteGroupID( m_vAllGroups[ nFilterIndex ].s_dwGroupID );
}

bool cXmlUnionDUnit::_IsFavoriteGroupID( DWORD dwGroupID ) const
{
	return m_setFavoriteGroupIDs.find( dwGroupID ) != m_setFavoriteGroupIDs.end();
}

void cXmlUnionDUnit::_ToggleFilterFavorite( int nFilterIndex )
{
	if( nFilterIndex < 0 || nFilterIndex >= (int)m_vAllGroups.size() )
		return;

	DWORD const dwGroupID = m_vAllGroups[ nFilterIndex ].s_dwGroupID;
	std::set< DWORD >::iterator it = m_setFavoriteGroupIDs.find( dwGroupID );
	if( it != m_setFavoriteGroupIDs.end() )
	{
		m_setFavoriteGroupIDs.erase( it );
		for( std::vector< DWORD >::iterator itOrder = m_vFavoriteGroupOrder.begin(); itOrder != m_vFavoriteGroupOrder.end(); )
		{
			if( *itOrder == dwGroupID )
				itOrder = m_vFavoriteGroupOrder.erase( itOrder );
			else
				++itOrder;
		}
	}
	else
	{
		m_setFavoriteGroupIDs.insert( dwGroupID );
		m_vFavoriteGroupOrder.push_back( dwGroupID );
	}

	_SaveFavoriteCache();
	m_nFilterScroll = 0;
	_RefreshView();
}

std::string cXmlUnionDUnit::_GetFavoriteCachePath() const
{
	CreateDirectoryA( "Temp", NULL );

	char szUser[ 64 ] = { 0, };
	DWORD const dwEnvLen = GetEnvironmentVariableA( "USERNAME", szUser, _countof( szUser ) );
	if( dwEnvLen == 0 || dwEnvLen >= _countof( szUser ) || szUser[ 0 ] == 0 )
		strcpy_s( szUser, _countof( szUser ), "default" );

	std::string strSafeUser;
	for( int i = 0; szUser[ i ] != 0; ++i )
	{
		unsigned char const ch = (unsigned char)szUser[ i ];
		strSafeUser.push_back( isalnum( ch ) || ch == '_' || ch == '-' ? (char)ch : '_' );
	}
	if( strSafeUser.empty() )
		strSafeUser = "default";

	return std::string( "Temp\\XmlUnionDUnitFavorites_" ) + strSafeUser + ".cache";
}

void cXmlUnionDUnit::_LoadFavoriteCache()
{
	m_setFavoriteGroupIDs.clear();
	m_vFavoriteGroupOrder.clear();

	std::string const strPath = _GetFavoriteCachePath();
	FILE* fp = NULL;
	if( fopen_s( &fp, strPath.c_str(), "rt" ) != 0 || fp == NULL )
		return;

	char szLine[ 128 ] = { 0, };
	while( fgets( szLine, _countof( szLine ), fp ) )
	{
		char* pEnd = NULL;
		unsigned long const ulGroupID = strtoul( szLine, &pEnd, 10 );
		if( ulGroupID > 0 && m_setFavoriteGroupIDs.find( (DWORD)ulGroupID ) == m_setFavoriteGroupIDs.end() )
		{
			m_setFavoriteGroupIDs.insert( (DWORD)ulGroupID );
			m_vFavoriteGroupOrder.push_back( (DWORD)ulGroupID );
		}
	}

	fclose( fp );
}

void cXmlUnionDUnit::_SaveFavoriteCache() const
{
	std::string const strPath = _GetFavoriteCachePath();
	FILE* fp = NULL;
	if( fopen_s( &fp, strPath.c_str(), "wt" ) != 0 || fp == NULL )
		return;

	fprintf( fp, "# XmlUnionDUnit favorite group ids\n" );
	for( size_t i = 0; i < m_vFavoriteGroupOrder.size(); ++i )
	{
		DWORD const dwGroupID = m_vFavoriteGroupOrder[ i ];
		if( _IsFavoriteGroupID( dwGroupID ) )
			fprintf( fp, "%lu\n", (unsigned long)dwGroupID );
	}

	fclose( fp );
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
		bool const bFavorite = bVisible && _IsFilterFavorite( nAbsoluteIndex );

		if( m_pFilterButtons[ i ] ) m_pFilterButtons[ i ]->SetVisible( bVisible );
		if( m_pFilterHoverBg[ i ] ) m_pFilterHoverBg[ i ]->SetVisible( bHover && bSelected == false );
		if( m_pFilterBookmark[ i ] )
		{
			m_pFilterBookmark[ i ]->SetVisible( bVisible );
			m_pFilterBookmark[ i ]->SetColorA( bFavorite ? NiColorA( 1.0f, 0.92f, 0.08f, 1.0f ) : ( bHover ? NiColorA( 0.50f, 0.53f, 0.56f, 0.72f ) : NiColorA( 0.25f, 0.27f, 0.30f, 0.55f ) ) );
		}

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

void cXmlUnionDUnit::_CreateTotalStatScrollBar()
{
	int const nBottomArrowY = XML_UNION_TOTAL_STAT_SCROLL_Y + XML_UNION_TOTAL_STAT_SCROLL_H - XML_UNION_TOTAL_STAT_SCROLL_ARROW_H;

	m_pTotalStatScrollBorder = _AddSolidSprite( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X - 1, XML_UNION_TOTAL_STAT_SCROLL_Y - 1 ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W + 2, XML_UNION_TOTAL_STAT_SCROLL_H + 2 ), NiColorA( 0.0f, 0.35f, 0.75f, 0.85f ) );
	m_pTotalStatScrollBg = _AddSolidSprite( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X, XML_UNION_TOTAL_STAT_SCROLL_Y ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W, XML_UNION_TOTAL_STAT_SCROLL_H ), NiColorA( 0.0f, 0.10f, 0.22f, 0.9f ) );
	m_pTotalStatScrollUp = _AddAssetSpriteRect( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X, XML_UNION_TOTAL_STAT_SCROLL_Y ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W, XML_UNION_TOTAL_STAT_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_UP, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pTotalStatScrollDown = _AddAssetSpriteRect( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X, nBottomArrowY ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W, XML_UNION_TOTAL_STAT_SCROLL_ARROW_H ), CsRect( 0, 0, 14, 14 ), XML_UNION_SCROLL_DOWN, NiColorA( 0.0f, 0.75f, 1.0f, 0.95f ) );
	m_pTotalStatScrollTrack = _AddSolidSprite( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X + 1, XML_UNION_TOTAL_STAT_SCROLL_TRACK_Y ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W - 2, XML_UNION_TOTAL_STAT_SCROLL_TRACK_H ), NiColorA( 0.0f, 0.40f, 0.75f, 0.35f ) );
	m_pTotalStatScrollThumb = _AddSolidSprite( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X + 1, XML_UNION_TOTAL_STAT_SCROLL_TRACK_Y ), CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W - 2, 28 ), NiColorA( 0.0f, 0.78f, 1.0f, 0.95f ) );
	_UpdateTotalStatScrollControls();
}

void cXmlUnionDUnit::_UpdateTotalStatControls()
{
	int const nMaxScroll = max( 0, IF_XML_UNION_STAT_COUNT - IF_XML_UNION_STAT_VISIBLE_COUNT );
	if( m_nTotalStatScroll > nMaxScroll )
		m_nTotalStatScroll = nMaxScroll;
	if( m_nTotalStatScroll < 0 )
		m_nTotalStatScroll = 0;

	for( int i = 0; i < IF_XML_UNION_STAT_VISIBLE_COUNT; ++i )
	{
		int const nStatIndex = m_nTotalStatScroll + i;
		bool const bVisible = nStatIndex >= 0 && nStatIndex < IF_XML_UNION_STAT_COUNT;

		TCHAR szName[ 48 ] = { 0, };
		TCHAR szValue[ 48 ] = { 0, };
		if( bVisible )
		{
			std::wstring wsName = _GetDisplayStatName( STAT_LABEL[ nStatIndex ] );
			_CopyWideToTChar( wsName, szName, _countof( szName ) );
			_stprintf_s( szValue, _countof( szValue ), _IsPercentStatIndex( nStatIndex ) ? _T( "+ %d%%" ) : _T( "+ %d" ), m_nServerBonusValue[ nStatIndex ] );
		}

		if( m_pStatNameText[ i ] )
		{
			m_pStatNameText[ i ]->SetVisible( bVisible );
			m_pStatNameText[ i ]->SetText( szName );
		}
		if( m_pStatValueText[ i ] )
		{
			m_pStatValueText[ i ]->SetVisible( bVisible );
			m_pStatValueText[ i ]->SetText( szValue );
		}
	}

	_UpdateTotalStatScrollControls();
}

void cXmlUnionDUnit::_UpdateTotalStatScrollControls()
{
	bool const bShowScroll = IF_XML_UNION_STAT_COUNT > IF_XML_UNION_STAT_VISIBLE_COUNT;
	if( m_pTotalStatScrollBorder ) m_pTotalStatScrollBorder->SetVisible( bShowScroll );
	if( m_pTotalStatScrollBg ) m_pTotalStatScrollBg->SetVisible( bShowScroll );
	if( m_pTotalStatScrollTrack ) m_pTotalStatScrollTrack->SetVisible( bShowScroll );
	if( m_pTotalStatScrollThumb ) m_pTotalStatScrollThumb->SetVisible( bShowScroll );
	if( m_pTotalStatScrollUp ) m_pTotalStatScrollUp->SetVisible( bShowScroll );
	if( m_pTotalStatScrollDown ) m_pTotalStatScrollDown->SetVisible( bShowScroll );
	if( bShowScroll == false || m_pTotalStatScrollThumb == NULL )
		return;

	int const nMaxScroll = max( 1, IF_XML_UNION_STAT_COUNT - IF_XML_UNION_STAT_VISIBLE_COUNT );
	int const nThumbH = max( 28, ( XML_UNION_TOTAL_STAT_SCROLL_TRACK_H * IF_XML_UNION_STAT_VISIBLE_COUNT ) / IF_XML_UNION_STAT_COUNT );
	int const nTravel = max( 1, XML_UNION_TOTAL_STAT_SCROLL_TRACK_H - nThumbH );
	int const nThumbY = XML_UNION_TOTAL_STAT_SCROLL_TRACK_Y + ( nTravel * m_nTotalStatScroll ) / nMaxScroll;
	m_pTotalStatScrollThumb->SetPos( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_X + 1, nThumbY ) );
	m_pTotalStatScrollThumb->SetSize( CsPoint( XML_UNION_TOTAL_STAT_SCROLL_W - 2, nThumbH ) );
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
