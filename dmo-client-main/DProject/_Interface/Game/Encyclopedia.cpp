#include "stdafx.h"
#include "Encyclopedia.h"
#include "MessageBox.h"
#include "FmCommon.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <cwctype>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifndef CLIENT_LOG_INFO
#define CLIENT_LOG_INFO( ... ) ((void)0)
#endif
#ifndef CLIENT_LOG_WARN
#define CLIENT_LOG_WARN( ... ) ((void)0)
#endif

#ifndef ENCY_DECK_VERBOSE_LOG
#define ENCY_DECK_VERBOSE_LOG 0
#endif
#define ENCY_DECK_LOG_INFO( ... ) do { if( ENCY_DECK_VERBOSE_LOG ) CLIENT_LOG_INFO( "ENCYDECK", __VA_ARGS__ ); } while( 0 )
#define ENCY_DECK_LOG_WARN( ... ) do { if( ENCY_DECK_VERBOSE_LOG ) CLIENT_LOG_WARN( "ENCYDECK", __VA_ARGS__ ); } while( 0 )

#ifndef ENCY_PERF_LOG
#define ENCY_PERF_LOG 1
#endif
#define ENCY_PERF_INFO( ... ) do { if( ENCY_PERF_LOG ) nsCSDEBUG::CrashLogger::LogMessage( "[ENCYPERF] " __VA_ARGS__ ); } while( 0 )

#ifndef ENCY_DECK_VALIDATE_EVO_ATLAS
#define ENCY_DECK_VALIDATE_EVO_ATLAS 0
#endif

namespace
{
	const int ENCY_LAZY_INITIAL_ROWS = 8;
	const int ENCY_LAZY_APPEND_ROWS_PER_FRAME = 2;
}

namespace
{
	const TCHAR* _GetEncyEvolutionStageText( int nStage )
	{
		switch( nStage )
		{
		case nsCsDigimonTable::ET_DIGITAMA: return _T( "Digitama" );
		case nsCsDigimonTable::ET_INTRAINING: return _T( "In-Training" );
		case nsCsDigimonTable::ET_ROOKIE: return _T( "Rookie" );
		case nsCsDigimonTable::ET_CHAMPION: return _T( "Champion" );
		case nsCsDigimonTable::ET_ULTIMATE: return _T( "Ultimate" );
		case nsCsDigimonTable::ET_MEGA: return _T( "Mega" );
		case nsCsDigimonTable::ET_BUSTERMODE: return _T( "Burst Mode" );
		case nsCsDigimonTable::ET_JOINTPROGRESSMODE: return _T( "Jogress" );
		case nsCsDigimonTable::ET_CAPSULE: return _T( "Armor" );
		case nsCsDigimonTable::ET_SPIRIT: return _T( "Spirit" );
		case nsCsDigimonTable::ET_X_ROOKIE: return _T( "Rookie X" );
		case nsCsDigimonTable::ET_X_CHAMPION: return _T( "Champion X" );
		case nsCsDigimonTable::ET_X_ULTIMATE: return _T( "Ultimate X" );
		case nsCsDigimonTable::ET_X_MEGA: return _T( "Mega X" );
		case nsCsDigimonTable::ET_X_BUSTERMODE: return _T( "Burst Mode X" );
		case nsCsDigimonTable::ET_X_JOGRESS: return _T( "Jogress X" );
		case nsCsDigimonTable::ET_MIX: return _T( "Variant" );
		default: return _T( "-" );
		}
	}

	const TCHAR* _GetEncyAttributeText( int nAttribute )
	{
		switch( nAttribute )
		{
		case nsCsDigimonTable::AT_NONE: return _T( "None" );
		case nsCsDigimonTable::AT_DATA: return _T( "Data" );
		case nsCsDigimonTable::AT_VACCINE: return _T( "Vaccine" );
		case nsCsDigimonTable::AT_VIRUS: return _T( "Virus" );
		case nsCsDigimonTable::AT_UNIDENTIFIED: return _T( "Unknown" );
		default: return _T( "-" );
		}
	}

	const TCHAR* _GetEncyFamilyText( int nFamily )
	{
		switch( nFamily )
		{
		case nsCsDigimonTable::FT_DR: return _T( "DR" );
		case nsCsDigimonTable::FT_NSP: return _T( "NSP" );
		case nsCsDigimonTable::FT_ME: return _T( "ME" );
		case nsCsDigimonTable::FT_WG: return _T( "WG" );
		case nsCsDigimonTable::FT_UK: return _T( "UK" );
		case nsCsDigimonTable::FT_DS: return _T( "DS" );
		case nsCsDigimonTable::FT_DA: return _T( "DA" );
		case nsCsDigimonTable::FT_JT: return _T( "JT" );
		case nsCsDigimonTable::FT_NSO: return _T( "NSo" );
		case nsCsDigimonTable::FT_VB: return _T( "VB" );
		default: return _T( "-" );
		}
	}

	const TCHAR* _GetEncyNatureText( int nNature )
	{
		switch( nNature )
		{
		case nsCsDigimonTable::NT_ICE: return _T( "Ice" );
		case nsCsDigimonTable::NT_WATER: return _T( "Water" );
		case nsCsDigimonTable::NT_FIRE: return _T( "Fire" );
		case nsCsDigimonTable::NT_EARTH: return _T( "Earth" );
		case nsCsDigimonTable::NT_WIND: return _T( "Wind" );
		case nsCsDigimonTable::NT_WOOD: return _T( "Wood" );
		case nsCsDigimonTable::NT_LIGHT: return _T( "Light" );
		case nsCsDigimonTable::NT_DARKNESS: return _T( "Darkness" );
		case nsCsDigimonTable::NT_THUNDER: return _T( "Thunder" );
		case nsCsDigimonTable::NT_STEEL: return _T( "Steel" );
		default: return _T( "-" );
		}
	}

	const TCHAR* _GetEncyRankText( int nDigimonRank )
	{
		switch( nDigimonRank )
		{
		case 0: return _T( "R" );
		case 1: return _T( "A" );
		case 2: return _T( "A+" );
		case 3: return _T( "S" );
		case 4: return _T( "S+" );
		case 5: return _T( "SS" );
		case 6: return _T( "SS+" );
		case 7: return _T( "SSS" );
		case 8: return _T( "SSS+" );
		case 9: return _T( "U" );
		case 10: return _T( "U+" );
		default: return _T( "N" );
		}
	}

	ICONITEM::eTYPE _GetSkillIconType( int nIcon )
	{
		if( nIcon >= 6000 ) return ICONITEM::SKILL6;
		if( nIcon >= 5000 ) return ICONITEM::SKILL5;
		if( nIcon >= 4000 ) return ICONITEM::SKILL4;
		if( nIcon >= 3000 ) return ICONITEM::SKILL3;
		if( nIcon >= 2000 ) return ICONITEM::SKILL2;
		return ICONITEM::SKILL1;
	}

	int _GetEncyRankIconIndex( int nDigimonRank )
	{
		// encyclopedia_512.png tile order:
		// 1:R, 2:N, 3:A, 4:A+, 5:S, 6:S+, 7:SS, 8:SS+, 9:SSS, 10:SSS+, 11:U, 12:U+.
		// AddImage() state index is zero-based; icon 2 => state 1, icon 12 => state 11.
		if( nDigimonRank < 0 )
			return 1; // N (icon 2)

		if( nDigimonRank > 10 )
			nDigimonRank = 10;

		return nDigimonRank + 1;
	}

	const char* const ENCY_UNION_CARD_BG = "Union\\collection_bg_new.tga";
	const char* const ENCY_UNION_FRAME_N = "Union\\collection_frame_rank1_new.png";
	const char* const ENCY_UNION_FRAME_S = "Union\\collection_frame_rank2_new.png";
	const char* const ENCY_UNION_FRAME_U = "Union\\collection_frame_rank3_new.png";
	bool const ENCY_OVERVIEW_USE_FIGURE_BASE = false;
	const char* const ENCY_OVERVIEW_FIGURE_BASE = "Data\\EtcObject\\FigureDigimon.nif";
	float const ENCY_OVERVIEW_FIGURE_BASE_MIN_Z = 58.0f;
	float const ENCY_OVERVIEW_FIGURE_BASE_MAX_Z = 86.0f;
	int const ENCY_CARD_ICON_SRC_SIZE = 32;

	float _GetOverviewFigureBaseZ( CDigimon* pDigimon )
	{
		float fBaseZ = 72.0f;
		if( pDigimon )
		{
			float fHeight = pDigimon->CsC_AvObject::GetToolHeight();
			if( fHeight > 0.0f )
				fBaseZ = fHeight * 0.62f;
		}
		if( fBaseZ < ENCY_OVERVIEW_FIGURE_BASE_MIN_Z )
			fBaseZ = ENCY_OVERVIEW_FIGURE_BASE_MIN_Z;
		if( fBaseZ > ENCY_OVERVIEW_FIGURE_BASE_MAX_Z )
			fBaseZ = ENCY_OVERVIEW_FIGURE_BASE_MAX_Z;
		return -fBaseZ;
	}

	float _GetOverviewFigureBaseScale( CDigimon* pDigimon )
	{
		float fBaseScale = 1.20f;
		if( pDigimon )
		{
			float fWidth = pDigimon->GetToolWidth();
			if( fWidth > 0.0f )
				fBaseScale = fWidth / 96.0f;
		}
		if( fBaseScale < 1.00f )
			fBaseScale = 1.00f;
		if( fBaseScale > 1.55f )
			fBaseScale = 1.55f;
		return fBaseScale;
	}

	cSprite* _CreateEncySolidSprite( CsPoint ptSize, NiColorA const& color )
	{
		cSprite* pSprite = NiNew cSprite;
		SAFE_POINTER_RETVAL( pSprite, NULL );
		pSprite->Init( NULL, CsPoint::ZERO, ptSize, color, false );
		return pSprite;
	}

	const char* _GetEncyCardFrameTexture( int nDigimonRank )
	{
		if( nDigimonRank >= 9 )
			return ENCY_UNION_FRAME_U;
		if( nDigimonRank >= 3 )
			return ENCY_UNION_FRAME_S;
		return ENCY_UNION_FRAME_N;
	}

	cString::sSPRITE* _AddEncySpriteRect( cString* pItem, char const* pTexture, CsPoint ptPos, CsPoint ptSize, CsRect const& rtTexture, NiColor color = FONT_WHITE, bool bUseWorkingFolder = true )
	{
		SAFE_POINTER_RETVAL( pItem, NULL );
		SAFE_POINTER_RETVAL( pTexture, NULL );

		cSprite* pSprite = NiNew cSprite;
		SAFE_POINTER_RETVAL( pSprite, NULL );
		pSprite->Init( NULL, CsPoint::ZERO, ptSize, rtTexture, pTexture, false, color, bUseWorkingFolder );

		cString::sSPRITE* pSpriteInfo = pItem->AddSprite( pSprite, ptPos );
		SAFE_POINTER_RETVAL( pSpriteInfo, NULL );
		pSpriteInfo->SetAutoPointerDelete( true );
		return pSpriteInfo;
	}

	cString::sIMAGE* _AddEncyFrameImage( cString* pItem, char const* pTexture, CsPoint ptPos, CsPoint ptSize, bool bVisible )
	{
		SAFE_POINTER_RETVAL( pItem, NULL );
		SAFE_POINTER_RETVAL( pTexture, NULL );

		cImage* pFrame = NiNew cImage;
		SAFE_POINTER_RETVAL( pFrame, NULL );
		pFrame->Init( NULL, CsPoint::ZERO, ptSize, pTexture, false, true );
		pFrame->SetTexToken( CsPoint( 126, 150 ) );

		cString::sIMAGE* pFrameSprite = pItem->AddImage( pFrame, (cImage::eSTATE)cImage::NORMAL0, ptPos );
		SAFE_POINTER_RETVAL( pFrameSprite, NULL );
		pFrameSprite->SetAutoPointerDelete( true );
		pFrameSprite->SetVisible( bVisible );
		return pFrameSprite;
	}

	void _AddEncyCircularPortrait( cString* pItem, std::string const& strIconPath, CsPoint ptIcon, int nIconSize, bool bClosed )
	{
		if( pItem == NULL || strIconPath.empty() || nIconSize <= 0 )
			return;

		NiColor const color = bClosed ? NiColor( 0.05f, 0.05f, 0.05f ) : FONT_WHITE;
		int const nIconRadius = nIconSize / 2;
		for( int nSliceY = 0 ; nSliceY < nIconSize ; ++nSliceY )
		{
			float const fDy = (float)nSliceY + 0.5f - (float)nIconRadius;
			float const fRadiusSq = (float)( nIconRadius * nIconRadius );
			float const fWidthSq = fRadiusSq - ( fDy * fDy );
			if( fWidthSq <= 0.0f )
				continue;

			int const nHalfWidth = (int)sqrtf( fWidthSq );
			if( nHalfWidth <= 0 )
				continue;

			int const nDestX = ptIcon.x + nIconRadius - nHalfWidth;
			int const nDestW = nHalfWidth * 2;
			int const nSrcLeft = ( ( nIconRadius - nHalfWidth ) * ENCY_CARD_ICON_SRC_SIZE ) / nIconSize;
			int const nSrcRight = ( ( nIconRadius + nHalfWidth ) * ENCY_CARD_ICON_SRC_SIZE ) / nIconSize;
			int const nSrcTop = ( nSliceY * ENCY_CARD_ICON_SRC_SIZE ) / nIconSize;
			int nSrcBottom = ( ( nSliceY + 1 ) * ENCY_CARD_ICON_SRC_SIZE ) / nIconSize;
			if( nSrcBottom <= nSrcTop )
				nSrcBottom = nSrcTop + 1;

			_AddEncySpriteRect(
				pItem,
				strIconPath.c_str(),
				CsPoint( nDestX, ptIcon.y + nSliceY ),
				CsPoint( nDestW, 1 ),
				CsRect( nSrcLeft, nSrcTop, nSrcRight, nSrcBottom ),
				color,
				false );
		}
	}

	void _AddEncyCardName( cString* pItem, TCHAR const* pName, CsPoint ptCard, int nCardWidth )
	{
		if( pItem == NULL || pName == NULL || pName[ 0 ] == 0 )
			return;

		cText::sTEXTINFO nameInfo;
		nameInfo.Init( CFont::FS_8, NiColor( 0.96f, 0.97f, 1.0f ) );
		nameInfo.s_eTextAlign = DT_CENTER;
		nameInfo.s_bOutLine = true;
		int const nMaxLen = nCardWidth < 90 ? 10 : ( nCardWidth < 110 ? 13 : 16 );
		nameInfo.SetText_Reduce( pName, nMaxLen, 2 );
		pItem->AddText( &nameInfo, ptCard + CsPoint( nCardWidth / 2, 7 ) );
	}

bool _ReplaceDeckIconSuffix( std::string& ioPath, char const* pFrom, char const* pTo )
	{
		size_t nPos = ioPath.rfind( pFrom );
		if( nPos == std::string::npos )
			return false;
		ioPath.replace( nPos, strlen( pFrom ), pTo );
		return true;
}

const char* _BoolTF( bool bValue )
{
	return bValue ? "T" : "F";
}

bool _DeckFileExists( std::string const& strPath )
	{
		if( strPath.empty() )
			return false;

		std::string strNormalized = strPath;
		std::replace( strNormalized.begin(), strNormalized.end(), '/', '\\' );
		while( !strNormalized.empty() && ( strNormalized[ 0 ] == '\\' || strNormalized[ 0 ] == '/' ) )
			strNormalized.erase( strNormalized.begin() );

		if( strNormalized.empty() )
			return false;

		static std::map< std::string, bool > s_mapExistsCache;
		std::map< std::string, bool >::const_iterator itCache = s_mapExistsCache.find( strNormalized );
		if( itCache != s_mapExistsCache.end() )
			return itCache->second;

		bool bExists = CsFPS::CsFPSystem::IsExist( 0, strNormalized.c_str() );

		if( bExists == false )
		{
			std::string strWithData = "Data\\";
			strWithData += strNormalized;
			bExists = CsFPS::CsFPSystem::IsExist( 0, strWithData.c_str() );
		}

		s_mapExistsCache[ strNormalized ] = bExists;
		return bExists;
	}

	std::string _ResolveDeckTexturePath( std::initializer_list< const char* > const& candidates )
	{
		for( std::initializer_list< const char* >::const_iterator it = candidates.begin(); it != candidates.end(); ++it )
		{
			if( *it == NULL )
				continue;

			const std::string path( *it );
			if( _DeckFileExists( path ) )
				return path;
		}

		return candidates.size() > 0 ? std::string( *candidates.begin() ) : std::string();
	}

	std::string _GetDeckPortraitFileName( std::string const& strFileName, bool& bUseEvoAtlas )
	{
		bUseEvoAtlas = false;
		std::string strDeckPortrait = strFileName;

		if( _ReplaceDeckIconSuffix( strDeckPortrait, "l.tga", "_evo.tga" )
			|| _ReplaceDeckIconSuffix( strDeckPortrait, "L.tga", "_evo.tga" )
			|| _ReplaceDeckIconSuffix( strDeckPortrait, "s.tga", "_evo.tga" )
			|| _ReplaceDeckIconSuffix( strDeckPortrait, "S.tga", "_evo.tga" ) )
		{
			bUseEvoAtlas = true;
			return strDeckPortrait;
		}

		if( strDeckPortrait.rfind( "_evo.tga" ) != std::string::npos )
		{
			bUseEvoAtlas = true;
			return strDeckPortrait;
		}

		if( _ReplaceDeckIconSuffix( strDeckPortrait, ".tga", "_evo.tga" ) )
		{
			bUseEvoAtlas = true;
			return strDeckPortrait;
		}
		if( _ReplaceDeckIconSuffix( strDeckPortrait, ".png", "_evo.png" )
			|| _ReplaceDeckIconSuffix( strDeckPortrait, ".PNG", "_evo.png" ) )
		{
			bUseEvoAtlas = true;
			return strDeckPortrait;
		}
		if( strDeckPortrait.rfind( "_evo.png" ) != std::string::npos
			|| strDeckPortrait.rfind( "_evo.PNG" ) != std::string::npos )
		{
			bUseEvoAtlas = true;
			return strDeckPortrait;
		}

		return strFileName;
	}

	bool _IsDeckDataOpenInLine( EncyclopediaContents::sEVOL_INFO const* pLine, DWORD dwDestDigimonID )
	{
		if( pLine == NULL )
			return false;

		for( int i = 0 ; i < nLimit::EvoUnit ; ++i )
		{
			if( pLine->s_sInfo[ i ].s_nDigimonID != (int)dwDestDigimonID )
				continue;
			if( i == 0 && pLine->s_nLevel > 0 )
				return true;
			return pLine->s_sInfo[ i ].s_eImgState == EncyclopediaContents::sINFO::S_OPEN;
		}

		return false;
	}

	bool _IsDeckDataOpenInEncyclopedia( std::map< int, EncyclopediaContents::sEVOL_INFO* > const& mapEncy, EncyclopediaContents::sDATA const* pDeckData )
	{
		if( pDeckData == NULL )
			return false;

		EncyclopediaContents::MAP_CIT itBase = mapEncy.find( (int)pDeckData->s_dwBaseDigimonID );
		if( itBase != mapEncy.end() && _IsDeckDataOpenInLine( itBase->second, pDeckData->s_dwDestDigimonID ) )
			return true;

		// Modern deck data can point to a destination through a different base line
		// than the legacy client expected. Treat the visible deck card as collected
		// when the destination Digimon is open anywhere in the encyclopedia cache.
		for( EncyclopediaContents::MAP_CIT it = mapEncy.begin() ; it != mapEncy.end() ; ++it )
		{
			if( _IsDeckDataOpenInLine( it->second, pDeckData->s_dwDestDigimonID ) )
				return true;
		}

		return false;
	}

	bool _IsDeckGroupCompleteByList( std::map< int, EncyclopediaContents::sEVOL_INFO* > const& mapEncy, EncyclopediaContents::sGROUP_INFO const* pGroupInfo )
	{
		if( pGroupInfo == NULL )
			return false;

		if( pGroupInfo->s_pListData.empty() )
			return false;

		EncyclopediaContents::MAP_DATA_CIT it = pGroupInfo->s_pListData.begin();
		EncyclopediaContents::MAP_DATA_CIT itEnd = pGroupInfo->s_pListData.end();
		for( ; it != itEnd; ++it )
		{
			if( (*it)->s_bIsRender == false )
				continue;

			if( (*it)->s_bIsOpen == false && _IsDeckDataOpenInEncyclopedia( mapEncy, (*it) ) == false )
				return false;
		}

		return true;
	}

	const char* _GetDeckHeaderOverlayTexture( bool bGroupOpen, bool bIsUseDeck )
	{
		if( bIsUseDeck )
			return "deck_enable";

		if( bGroupOpen )
			return "deck_completed";

		return NULL;
	}

	void _SetEncyImageButtonState( cImage* pImage, CsRect const& rtButton, bool bEnable = true )
	{
		if( pImage == NULL )
			return;

		if( !bEnable )
		{
			pImage->SetState( cImage::NORMAL3 );
			return;
		}

		CURSOR::eTYPE eMouseState = CURSOR_ST.CheckClickBox( rtButton );
		if( eMouseState == CURSOR::BUTTON_OUTWINDOW )
		{
			pImage->SetState( cImage::NORMAL0 );
			return;
		}

		int const eButtonState = CURSOR_ST.GetButtonState();
		if( eButtonState == CURSOR::LBUTTON_DOWN || eButtonState == CURSOR::LBUTTON_PRESS )
			pImage->SetState( cImage::NORMAL2 );
		else
			pImage->SetState( cImage::NORMAL1 );
	}

	class cEncyHoverEffectSprite : public cSprite
	{
	public:
		cEncyHoverEffectSprite()
			: m_fCurAniTime( 0.0f )
			, m_fSetAniTime( 0.055f )
			, m_nFrame( 0 )
		{
		}

		virtual ~cEncyHoverEffectSprite()
		{
		}

		void InitHoverEffect( CsPoint size )
		{
			for( int i = 0; i < HOVER_EFFECT_FRAME_COUNT; ++i )
				sprintf_s( m_szFramePath[ i ], MAX_PATH, "Encyclopedia\\newencyclopedia\\collection_effect_frame%02d.png", i + 1 );

			cSprite::Init( NULL, CsPoint::ZERO, size, m_szFramePath[ 0 ], false );
			SetAlpha( 0.8f );
			SetVisible( false );
		}

		virtual void Update( float const& fDeltaTime )
		{
			if( !m_bVisible )
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

	class cEncyListRenderGuard
	{
	public:
		cEncyListRenderGuard( CRITICAL_SECTION& cs ) : m_cs( cs )
		{
			EnterCriticalSection( &m_cs );
		}

		~cEncyListRenderGuard()
		{
			LeaveCriticalSection( &m_cs );
		}

	private:
		CRITICAL_SECTION& m_cs;
	};
}

cEncyclopedia::cEncyclopedia() : m_bIsMouseOn(false)
{
	InitializeCriticalSection( &m_csListRender );
	m_pEncyListBox			= NULL;
	m_pDeckListBox			= NULL;
	m_pScrollBar			= NULL;
	m_pDeckScrollBar		= NULL;
	m_pCancelButton			= NULL;
	m_pMoveButton			= NULL;
	m_pStatisticsBtn		= NULL;
	m_pRadioBtn				= NULL;
	m_pMainTabBtn[ 0 ]		= NULL;
	m_pMainTabBtn[ 1 ]		= NULL;
	for( int i = 0; i < 4; ++i )
	{
		m_pSubMenuBtn[ i ] = NULL;
		m_pSubMenuText[ i ] = NULL;
	}
	m_pSearchBarBg = NULL;
	m_pSearchEdit = NULL;
	m_pSearchBtn = NULL;
	m_pSearchResetBtn = NULL;
	m_wsSearchKeyword.clear();
	m_wsEncyListCacheSearch.clear();
	m_wsDeckListCacheSearch.clear();
	m_bSearchFocusVisualOn = false;
	m_bEncyListBuilt = false;
	m_bDeckListBuilt = false;
	m_nEncyListCacheSubMenu = -1;
	m_bEncyLazyAppendActive = false;
	m_bDeckLazyAppendActive = false;
	m_nEncyLazySourceIndex = 0;
	m_nEncyLazyFlatRowIndex = 0;
	m_nDeckLazyGroupCursor = 0;
	m_vDeckLazyGroups.clear();
	m_pCardHoverEffect = NULL;
	m_bCardHoverEffectVisible = false;
	m_ptCardHoverEffectPos = CsPoint::ZERO;
	m_ptCardHoverEffectSize = CsPoint::ZERO;
	m_pCardNameTooltipBg = NULL;
	m_pCardNameTooltipTop = NULL;
	m_pCardNameTooltipBottom = NULL;
	m_pCardNameTooltipLeft = NULL;
	m_pCardNameTooltipRight = NULL;
	m_pCardNameTooltipText = NULL;
	m_bCardNameTooltipVisible = false;
	m_ptCardNameTooltipPos = CsPoint::ZERO;
	m_ptCardNameTooltipSize = CsPoint::ZERO;

	m_pTooltipBG			= NULL;
	m_szName				= NULL;
	m_szLevel				= NULL;
	m_szScale				= NULL;

	for( int i = 0 ; i < IF_ENCY_MAX_ENCHANT_COUNT ; i++ )
		m_pEnchant[ i ] = NULL;

	for( int i = 0 ; i < IF_ENCY_VIEW_ENCHANT_COUNT ; i++ )
		m_szEnchantVal[ i ] = NULL;

	m_szExplain = NULL;

	m_nGroupCount = 0;
	m_nCurTab = -1;
	m_nMainTab = MAIN_TAB_ENCYCLOPEDIA;
	m_nSubMenu = SUBMENU_SERIES;
	m_nTooltipDigimonID = 0;
	iScrollPos = 0;
	m_nPendingDeckScrollPos = -1;
	m_bPendingDeckTabRefresh = false;
	m_bOverviewVisible = false;
	m_nOverviewDigimonId = 0;
	m_nOverviewTooltipDigimonId = 0;
	m_nOverviewSelectedSkillIdx = -1;
	m_nOverviewHoverSkillIdx = -1;
	m_pOverviewRenderTex = NULL;
	m_pOverviewRenderDigimon = NULL;
	m_pOverviewFigureBase = NULL;
	m_pOverviewName = NULL;
	m_pOverviewGrowthTitle = NULL;
	m_pOverviewInfoTitle = NULL;
	m_pOverviewSkillTab = NULL;
	m_pOverviewRankLabel = NULL;
	m_pOverviewRankValue = NULL;
	m_pOverviewStageLabel = NULL;
	m_pOverviewStageValue = NULL;
	for( int i = 0; i < 8; ++i )
		m_pOverviewStatText[ i ] = NULL;
	m_pOverviewSkillName = NULL;
	m_pOverviewSkillTabFrame = NULL;
	m_pOverviewBackBtn = NULL;
	m_pOverviewBackText = NULL;
	m_pOverviewCloseBtn = NULL;
	m_pOverviewScrollBtn = NULL;
	m_pOverviewZoomInBtn = NULL;
	m_pOverviewZoomOutBtn = NULL;
	m_pOverviewArrowLBtn = NULL;
	m_pOverviewArrowRBtn = NULL;
	m_pOverviewRotateLBtn = NULL;
	m_pOverviewRotateRBtn = NULL;
	m_pOverviewRotateUpBtn = NULL;
	m_pOverviewRotateDownBtn = NULL;
	m_pOverviewResetViewBtn = NULL;
	m_pOverviewTooltipLine = NULL;
	m_pOverviewGrowth = NULL;
	m_pOverviewInfo = NULL;
	m_pOverviewSkillDesc = NULL;
	m_pOverviewSkills = NULL;
	m_pOverviewSkillStatsLeft = NULL;
	m_pOverviewSkillStatsRight = NULL;
	m_pOverviewBg = NULL;
	m_pOverviewInfoTextBg = NULL;
	m_pOverviewRatingFrame = NULL;
	m_pOverviewBox = NULL;
	m_pOverviewAttributeFrame = NULL;
	m_pOverviewSkillFrame = NULL;
	m_pOverviewSkillSelection = NULL;
	m_pOverviewSkillLine = NULL;
	m_pOverviewStatusArrow = NULL;
	m_pOverviewStatusArrow2 = NULL;
	m_pOverviewActionCardFrame = NULL;
	m_fOverviewZoomRate = 0.5f;
	m_fOverviewZoomTargetRate = 0.5f;
	m_fOverviewRenderYaw = 0.0f;
	m_fOverviewRenderPitch = 0.0f;
	m_fOverviewZoomDistMin = 0.1f;
	m_fOverviewZoomDistMax = 2.0f;
	m_fOverviewZoomStep = 0.03f;
	m_bOverviewZoomSliderDrag = false;
	m_nOverviewZoomSliderDragOffsetX = 0;
	m_bOverviewModelDragRotate = false;
	m_ptOverviewModelDragPrev = CsPoint::ZERO;
	m_fOverviewPreviewAniTimer = 0.0f;
	m_bOverviewPreviewAltAni = false;
	m_bOverviewPreviewAniStarted = false;
}

cEncyclopedia::~cEncyclopedia()
{
	if( GetSystem() )
		GetSystem()->UnRegisterAll( this );
	DeleteCriticalSection( &m_csListRender );
}

bool cEncyclopedia::Construct(void)
{
	if( !CONTENTSSYSTEM_PTR )
		return false;

	SetSystem( CONTENTSSYSTEM_PTR->GetContents< SystemType >( SystemType::IsContentsIdentity() ) );

	if( !GetSystem() )
		return false;

	GetSystem()->Register( SystemType::ENCYCLOPEDIA_REFRESHLIST, this );
	GetSystem()->Register( SystemType::ENCYCLOPEDIA_USEDECK, this );

	return true;
}

void cEncyclopedia::Notify(int const& iNotifiedEvt, ContentsStream const& kStream)
{
	switch( iNotifiedEvt )
	{
	case SystemType::ENCYCLOPEDIA_REFRESHLIST:
		{
			if( IsShowWindow() )
				_RefreshList();
		}
		break;

	case SystemType::ENCYCLOPEDIA_USEDECK:
		{
			_InvalidateDeckListCache();
			cScrollBar* pActiveScroll = _GetActiveScrollBar();
			m_nPendingDeckScrollPos = ( pActiveScroll ? pActiveScroll->GetCurPosIndex() : -1 );
			m_bPendingDeckTabRefresh = true;
			CLIENT_LOG_INFO( "DECK-UI", "Queued deck tab refresh. tab=%d scroll=%d", m_nCurTab, m_nPendingDeckScrollPos );
		}
		break;
	}
}

void cEncyclopedia::Destroy()
{
	m_pCurTabList_map.clear();
	cBaseWindow::Delete();
}

void cEncyclopedia::Open( int nValue, bool bSound )
{
	if( IsLive() )
	{
		UNREFERENCED_PARAMETER( nValue );
		if( bSound )
			PlayOpenSound();

		SetEnableWindow( true );
		SetShowWindow( true );
		_ResetInitialTabState();
		_SelectMainTab( MAIN_TAB_ENCYCLOPEDIA, true );
		return;
	}

	cBaseWindow::Open( nValue, bSound );
}

bool cEncyclopedia::Close( bool bSound )
{
	if( IsEnableWindow() == false )
		return false;

	if( bSound )
		PlayCloseSound();

	if( g_pGameIF && g_pGameIF->IsActiveWindow( cBaseWindow::WT_ENCYCLOPEDIA_STATS ) )
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA_STATS );

	_CloseOverview();
	m_bIsMouseOn = false;
	_SetCardHoverVisible( false );
	SetShowWindow( false );
	return true;
}

void cEncyclopedia::DeleteResource()
{
	_ReleaseOverviewRenderDigimon();
	_ClearIconList();
	m_vCardHoverInfo.clear();
	m_vDeckPanelHoverInfo.clear();
	m_vDeckBookmarkInfo.clear();

	//툴팁
	NISAFE_DELETE( m_pTooltipBG );
	NISAFE_DELETE( m_szName );
	NISAFE_DELETE( m_szLevel );
	NISAFE_DELETE( m_szScale );

	for( int i = 0 ; i < IF_ENCY_MAX_ENCHANT_COUNT ; i++ )
	{
		NISAFE_DELETE( m_pEnchant[ i ] );
	}

	for( int i = 0 ; i < IF_ENCY_VIEW_ENCHANT_COUNT ; i++ )
	{
		NISAFE_DELETE( m_szEnchantVal[ i ] );
	}
	NISAFE_DELETE( m_szExplain );
	NISAFE_DELETE( m_pCardHoverEffect );
	m_bCardHoverEffectVisible = false;
	m_ptCardHoverEffectPos = CsPoint::ZERO;
	m_ptCardHoverEffectSize = CsPoint::ZERO;
	NISAFE_DELETE( m_pCardNameTooltipBg );
	NISAFE_DELETE( m_pCardNameTooltipTop );
	NISAFE_DELETE( m_pCardNameTooltipBottom );
	NISAFE_DELETE( m_pCardNameTooltipLeft );
	NISAFE_DELETE( m_pCardNameTooltipRight );
	NISAFE_DELETE( m_pCardNameTooltipText );
	m_bCardNameTooltipVisible = false;
	m_ptCardNameTooltipPos = CsPoint::ZERO;
	m_ptCardNameTooltipSize = CsPoint::ZERO;
	NISAFE_DELETE( m_pOverviewRenderTex );
	if( m_pOverviewFigureBase )
	{
		m_pOverviewFigureBase->Delete();
		NISAFE_DELETE( m_pOverviewFigureBase );
	}
	NISAFE_DELETE( m_pOverviewName );
	NISAFE_DELETE( m_pOverviewGrowthTitle );
	NISAFE_DELETE( m_pOverviewInfoTitle );
	NISAFE_DELETE( m_pOverviewSkillTab );
	NISAFE_DELETE( m_pOverviewRankLabel );
	NISAFE_DELETE( m_pOverviewRankValue );
	NISAFE_DELETE( m_pOverviewStageLabel );
	NISAFE_DELETE( m_pOverviewStageValue );
	for( int i = 0; i < 8; ++i )
		NISAFE_DELETE( m_pOverviewStatText[ i ] );
	NISAFE_DELETE( m_pOverviewSkillName );
	NISAFE_DELETE( m_pOverviewSkillTabFrame );
	NISAFE_DELETE( m_pOverviewBackBtn );
	NISAFE_DELETE( m_pOverviewBackText );
	NISAFE_DELETE( m_pOverviewCloseBtn );
	NISAFE_DELETE( m_pOverviewScrollBtn );
	NISAFE_DELETE( m_pOverviewZoomInBtn );
	NISAFE_DELETE( m_pOverviewZoomOutBtn );
	NISAFE_DELETE( m_pOverviewArrowLBtn );
	NISAFE_DELETE( m_pOverviewArrowRBtn );
	NISAFE_DELETE( m_pOverviewRotateLBtn );
	NISAFE_DELETE( m_pOverviewRotateRBtn );
	NISAFE_DELETE( m_pOverviewRotateUpBtn );
	NISAFE_DELETE( m_pOverviewRotateDownBtn );
	NISAFE_DELETE( m_pOverviewResetViewBtn );
	NISAFE_DELETE( m_pOverviewTooltipLine );
	NISAFE_DELETE( m_pOverviewGrowth );
	NISAFE_DELETE( m_pOverviewInfo );
	NISAFE_DELETE( m_pOverviewSkillDesc );
	NISAFE_DELETE( m_pOverviewSkills );
	NISAFE_DELETE( m_pOverviewSkillStatsLeft );
	NISAFE_DELETE( m_pOverviewSkillStatsRight );
	NISAFE_DELETE( m_pOverviewBg );
	NISAFE_DELETE( m_pOverviewInfoTextBg );
	NISAFE_DELETE( m_pOverviewRatingFrame );
	NISAFE_DELETE( m_pOverviewBox );
	NISAFE_DELETE( m_pOverviewAttributeFrame );
	NISAFE_DELETE( m_pOverviewSkillFrame );
	NISAFE_DELETE( m_pOverviewSkillSelection );
	NISAFE_DELETE( m_pOverviewSkillLine );
	NISAFE_DELETE( m_pOverviewStatusArrow );
	NISAFE_DELETE( m_pOverviewStatusArrow2 );
	NISAFE_DELETE( m_pOverviewActionCardFrame );
	m_bOverviewVisible = false;
	m_nOverviewDigimonId = 0;
	m_nOverviewTooltipDigimonId = 0;
	m_fOverviewPreviewAniTimer = 0.0f;
	m_bOverviewPreviewAltAni = false;
	m_bOverviewPreviewAniStarted = false;

	DeleteScript();
}

void cEncyclopedia::Create( int nValue /*= 0 */ )
{
	DWORD const dwCreateBegin = GetTickCount();
	_ResetInitialTabState();
	_LoadDeckFavoriteCache();
	cBaseWindow::Init();
	ENCY_PERF_INFO( "Create begin value=%d serverReady=%d encyCount=%d groupCount=%d",
		nValue,
		( GetSystem() && GetSystem()->IsServerDataReceived() ) ? 1 : 0,
		GetSystem() ? (int)GetSystem()->GetEncyMapData().size() : 0,
		GetSystem() ? (int)GetSystem()->GetGroupMapData().size() : 0 );

	if( GetSystem() )
		GetSystem()->EnsureStaticDataLoaded();

	int nPosX = ( g_nScreenWidth/2 ) - ( 886/2 );
	int nPosY = ( g_nScreenHeight/2 ) - ( 768/2 );

	InitScript( "Encyclopedia\\newencyclopedia\\main\\main_bg.png", CsPoint( nPosX, nPosY ) , CsPoint( 886, 768 ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	m_pMoveButton = AddButton( CsPoint( 827, 5 ), CsPoint( 22, 22 ), CsPoint( 0, 22 ), "System\\MoveWindow.tga" );
	m_pCancelButton = AddButton( CsPoint( 852, 3 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\main\\exit_btn.png" );
	m_pCancelButton->AddEvent(cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressCancelButton);

	// 메인 탭
	m_pMainTabBtn[ 0 ] = AddButton( CsPoint( 320, 137 ), CsPoint( 120, 58 ), CsPoint( 0, 58 ), "Encyclopedia\\newencyclopedia\\main\\tab.png" );
	m_pMainTabBtn[ 1 ] = AddButton( CsPoint( 446, 137 ), CsPoint( 120, 58 ), CsPoint( 0, 58 ), "Encyclopedia\\newencyclopedia\\main\\tab.png" );
	m_pMainTabBtn[ 0 ]->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressMainTabButton );
	m_pMainTabBtn[ 1 ]->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressMainTabButton );
	m_pMainTabBtn[ 0 ]->SetUserData( new sDIGIMON_ID( MAIN_TAB_ENCYCLOPEDIA ) );
	m_pMainTabBtn[ 1 ]->SetUserData( new sDIGIMON_ID( MAIN_TAB_DECK ) );
	m_pMainTabBtn[ 0 ]->SetMouseOnMode( true );

	// 통계창 버튼(기존 팝업 유지)
	m_pStatisticsBtn = AddButton( CsPoint( 231, 150 ), CsPoint( 36, 36 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\sub_menu\\statistics.png" );
	m_pStatisticsBtn->AddEvent(cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressStatisticsButton);
	m_pStatisticsBtn->SetTexToken( CsPoint( 0, 32 ) );

	// 서브 메뉴
	m_pSubMenuBtn[ SUBMENU_SERIES ] = AddButton( CsPoint( 90, 150 ), CsPoint( 36, 36 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\sub_menu\\series.png" );
	m_pSubMenuBtn[ SUBMENU_ALL ] = AddButton( CsPoint( 137, 150 ), CsPoint( 36, 36 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\sub_menu\\all_btn.png" );
	m_pSubMenuBtn[ SUBMENU_CLASS ] = AddButton( CsPoint( 186, 150 ), CsPoint( 36, 36 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\sub_menu\\category.png" );
	m_pSubMenuBtn[ SUBMENU_STATISTICS ] = m_pStatisticsBtn;
	for( int i = 0; i < 3; ++i )
	{
		m_pSubMenuBtn[ i ]->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressSubMenuButton );
		m_pSubMenuBtn[ i ]->SetUserData( new sDIGIMON_ID( i ) );
	}
	m_pSubMenuBtn[ SUBMENU_SERIES ]->SetMouseOnMode( true );
	m_pSubMenuBtn[ SUBMENU_SERIES ]->SetVisible( true );
	m_pSubMenuBtn[ SUBMENU_ALL ]->SetVisible( true );
	m_pSubMenuBtn[ SUBMENU_CLASS ]->SetVisible( true );
	m_pStatisticsBtn->SetVisible( true );

	// 가나다 순 나누기 텝
	CsPoint ptRadioPos = CsPoint( -5000, -5000 );
	std::map<std::wstring,std::wstring> const& mapGroupStr = GetSystem()->GetGroupString();
	std::map<std::wstring,std::wstring>::const_iterator it = mapGroupStr.begin();

	for( int n = 0; it != mapGroupStr.end(); ++it, ++n )
	{
		TCHAR* pString = const_cast<TCHAR*>( it->second.c_str() );
		if( n == 0 )
			m_pRadioBtn = AddRadioButton(ptRadioPos, CsPoint( 51, 31 ), cCheckBox::IMAGE_RADIO_ENCYCLOPEDIA, pString);
		else if( m_pRadioBtn )
			m_pRadioBtn->AddNode( ptRadioPos+CsPoint( 0, 33 +( 33 * (n - 1)) ), CsPoint( 51, 31 ), cCheckBox::IMAGE_RADIO_ENCYCLOPEDIA, pString );
	}

	m_nGroupCount = (int)mapGroupStr.size();
	if( m_pRadioBtn == NULL )
	{
		ENCY_PERF_INFO( "Create fallback radio button because group string is empty. encyCount=%d groupCount=%d",
			GetSystem() ? (int)GetSystem()->GetEncyMapData().size() : 0,
			GetSystem() ? (int)GetSystem()->GetGroupMapData().size() : 0 );
		m_pRadioBtn = AddRadioButton( ptRadioPos, CsPoint( 51, 31 ), cCheckBox::IMAGE_RADIO_ENCYCLOPEDIA, _T( " " ) );
	}

	//그룹 목록
	if( m_pRadioBtn )
	{
		m_pRadioBtn->AddNode( ptRadioPos+CsPoint( 2, 411 ), CsPoint( 45, 29 ), cCheckBox::IMAGE_RADIO_ENCY_DECK, _T( " " ) );
		m_pRadioBtn->SetCheckIndex( 0 );
	}

	m_pScrollBar = NiNew cScrollBar;
	m_pScrollBar->Init( cScrollBar::TYPE_ENCY, NULL, CsPoint::ZERO, CsPoint( 13, 560 ), CsPoint( 13, 13 ), CsRect( CsPoint::ZERO, CsPoint::ZERO ), 5, false );

	AddSprite( CsPoint( 0, 198 ), CsPoint( 886, 560 ), "Encyclopedia\\newencyclopedia\\main\\black_sub bg.png" );

	m_pEncyListBox = NiNew cListBox;
	m_pEncyListBox->Init( m_pRoot, CsPoint(40, 198), CsPoint( 802, 560 ), NULL, false );
	m_pEncyListBox->SetScrollBar(m_pScrollBar);
	m_pEncyListBox->SetScrollBarRenderCount(2);
	// Deck panels use transparent backgrounds; fully remove list-row hover/select
	// overlay sprites so a hovered/selected row cannot appear as a dark panel.
	m_pEncyListBox->SetMouseOverImg( (cImage*)NULL, true );
	m_pEncyListBox->SetSelectedImg( (cImage*)NULL, true );
	m_pEncyListBox->SetBackOverAndSelectedImgRender(false);
	m_pEncyListBox->AddEvent( cListBox::LIST_SELECT_CHANGE_ITEM, this, &cEncyclopedia::_OnClickEncyclopediaItem );
	m_pEncyListBox->AddEvent( cListBox::LIST_SELECTED_ITEM, this, &cEncyclopedia::_OnClickEncyclopediaItem );
	AddChildControl( m_pEncyListBox );

	m_pDeckScrollBar = NiNew cScrollBar;
	m_pDeckScrollBar->Init( cScrollBar::TYPE_ENCY, NULL, CsPoint::ZERO, CsPoint( 13, 560 ), CsPoint( 13, 13 ), CsRect( CsPoint::ZERO, CsPoint::ZERO ), 5, false );

	m_pDeckListBox = NiNew cListBox;
	m_pDeckListBox->Init( m_pRoot, CsPoint(40, 198), CsPoint( 802, 560 ), NULL, false );
	m_pDeckListBox->SetScrollBar( m_pDeckScrollBar );
	m_pDeckListBox->SetScrollBarRenderCount( 2 );
	m_pDeckListBox->SetMouseOverImg( (cImage*)NULL, true );
	m_pDeckListBox->SetSelectedImg( (cImage*)NULL, true );
	m_pDeckListBox->SetBackOverAndSelectedImgRender( false );
	m_pDeckListBox->AddEvent( cListBox::LIST_SELECT_CHANGE_ITEM, this, &cEncyclopedia::_OnClickEncyclopediaItem );
	m_pDeckListBox->AddEvent( cListBox::LIST_SELECTED_ITEM, this, &cEncyclopedia::_OnClickEncyclopediaItem );
	m_pDeckListBox->SetVisible( false );
	AddChildControl( m_pDeckListBox );
	_UpdateActiveListVisibility();

	NISAFE_DELETE( m_pCardHoverEffect );
	m_pCardHoverEffect = NiNew cEncyHoverEffectSprite;
	if( m_pCardHoverEffect )
		static_cast< cEncyHoverEffectSprite* >( m_pCardHoverEffect )->InitHoverEffect( CsPoint( 116, 146 ) );
	m_bCardHoverEffectVisible = false;
	m_ptCardHoverEffectPos = CsPoint::ZERO;
	m_ptCardHoverEffectSize = CsPoint::ZERO;

	NISAFE_DELETE( m_pCardNameTooltipBg );
	NISAFE_DELETE( m_pCardNameTooltipTop );
	NISAFE_DELETE( m_pCardNameTooltipBottom );
	NISAFE_DELETE( m_pCardNameTooltipLeft );
	NISAFE_DELETE( m_pCardNameTooltipRight );
	NISAFE_DELETE( m_pCardNameTooltipText );
	m_pCardNameTooltipBg = _CreateEncySolidSprite( CsPoint( 1, 1 ), NiColorA( 0.0f, 0.02f, 0.05f, 0.92f ) );
	m_pCardNameTooltipTop = _CreateEncySolidSprite( CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pCardNameTooltipBottom = _CreateEncySolidSprite( CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pCardNameTooltipLeft = _CreateEncySolidSprite( CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pCardNameTooltipRight = _CreateEncySolidSprite( CsPoint( 1, 1 ), NiColorA( 0.0f, 0.12f, 0.24f, 0.95f ) );
	m_pCardNameTooltipText = NiNew cText;
	if( m_pCardNameTooltipText )
	{
		cText::sTEXTINFO tooltipTextInfo;
		tooltipTextInfo.Init( &g_pEngine->m_FontSystem );
		tooltipTextInfo.s_eFontSize = CFont::FS_8;
		tooltipTextInfo.s_eTextAlign = DT_CENTER;
		tooltipTextInfo.s_Color = NiColor( 0.96f, 0.97f, 1.0f );
		tooltipTextInfo.SetText( _T( "" ) );
		m_pCardNameTooltipText->Init( NULL, CsPoint::ZERO, &tooltipTextInfo, false );
	}
	m_bCardNameTooltipVisible = false;
	m_ptCardNameTooltipPos = CsPoint::ZERO;
	m_ptCardNameTooltipSize = CsPoint::ZERO;

	cText::sTEXTINFO titleInfo;
	titleInfo.Init( &g_pEngine->m_FontSystem );
	titleInfo.s_eFontSize = CFont::FS_20;
	titleInfo.s_eTextAlign = DT_CENTER;
	titleInfo.SetText( _T( "Digimon Encyclopedia." ) );
	AddText( &titleInfo, CsPoint( 443, 34 ) );
	AddSprite( CsPoint( 341, 73 ), CsPoint( 205, 17 ), "Encyclopedia\\newencyclopedia\\main\\title.png" );

	cText::sTEXTINFO tabInfo;
	tabInfo.Init( &g_pEngine->m_FontSystem );
	tabInfo.s_eFontSize = CFont::FS_12;
	tabInfo.s_eTextAlign = DT_CENTER;
	tabInfo.SetText( _T( "Encyclopedia" ) );
	AddText( &tabInfo, CsPoint( 380, 172 ) );
	tabInfo.SetText( _T( "Deck" ) );
	AddText( &tabInfo, CsPoint( 506, 172 ) );

	cText::sTEXTINFO subMenuInfo;
	subMenuInfo.Init( &g_pEngine->m_FontSystem );
	subMenuInfo.s_eFontSize = CFont::FS_12;
	subMenuInfo.s_eTextAlign = DT_CENTER;
	subMenuInfo.SetText( _T( "Series" ) );
	m_pSubMenuText[ SUBMENU_SERIES ] = AddText( &subMenuInfo, CsPoint( 108, 186 ) );
	subMenuInfo.SetText( _T( "All" ) );
	m_pSubMenuText[ SUBMENU_ALL ] = AddText( &subMenuInfo, CsPoint( 155, 186 ) );
	subMenuInfo.SetText( _T( "Class" ) );
	m_pSubMenuText[ SUBMENU_CLASS ] = AddText( &subMenuInfo, CsPoint( 202, 186 ) );
	subMenuInfo.SetText( _T( "Stats" ) );
	m_pSubMenuText[ SUBMENU_STATISTICS ] = AddText( &subMenuInfo, CsPoint( 249, 186 ) );

	// Search UI (Series/All only): right side above the grid area.
	m_pSearchBarBg = AddSprite( CsPoint( 610, 160 ), CsPoint( 190, 32 ), "Encyclopedia\\newencyclopedia\\main\\serch bar.png" );
	m_pSearchBtn = AddButton( CsPoint( 765, 160 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\main\\sarch.png" );
	m_pSearchResetBtn = AddButton( CsPoint( 805, 160 ), CsPoint( 32, 32 ), CsPoint( 0, 32 ), "Encyclopedia\\newencyclopedia\\main\\re.png" );
	if( m_pSearchBtn )
		m_pSearchBtn->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressSearchButton );
	if( m_pSearchResetBtn )
		m_pSearchResetBtn->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressSearchResetButton );

	cText::sTEXTINFO searchTextInfo;
	searchTextInfo.Init( &g_pEngine->m_FontSystem );
	searchTextInfo.s_eFontSize = CFont::FS_11;
	searchTextInfo.s_eTextAlign = DT_LEFT;
	searchTextInfo.s_Color = NiColor::WHITE;
	m_pSearchEdit = NiNew cEditBox;
	m_pSearchEdit->Init( m_pRoot, CsPoint( 620, 171 ), CsPoint( 176, 22 ), &searchTextInfo, false );
	m_pSearchEdit->SetLimitNumber( 32 );
	m_pSearchEdit->SetEmptyMsgText( _T( "Search Digimon..." ), NiColor( 0.6f, 0.65f, 0.85f ) );
	AddChildControl( m_pSearchEdit );
	_RefreshSearchUI();

	/************************************************************************/
	/*							도감 UI - 툴팁								*/
	/************************************************************************/
	//배경
	m_pTooltipBG = NiNew cSprite;
	m_pTooltipBG->Init( NULL, CsPoint::ZERO, CsPoint( 228, 250 ), "Encyclopedia\\Encyclopedia_Tooltip.tga", false );

	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem );
	ti.s_eTextAlign = DT_CENTER;
	ti.s_eFontSize = CFont::FS_10;

	//이름
	m_szName = NiNew cText;
	m_szName->Init( NULL,CsPoint::ZERO, &ti, false );
	//레벨
	m_szLevel = NiNew cText;
	m_szLevel->Init( NULL,CsPoint::ZERO, &ti, false );
	//스케일
	m_szScale = NiNew cText;
	m_szScale->Init( NULL,CsPoint::ZERO, &ti, false );

	// 강화수치 텍스트
	for( int i = 0 ; i < IF_ENCY_VIEW_ENCHANT_COUNT ; i++ )
	{
		m_szEnchantVal[ i ] = NiNew cText;
		m_szEnchantVal[ i ]->Init( NULL,CsPoint::ZERO, &ti, false );
	}

	//설명
	m_szExplain = NiNew cStringList;

	// 공격력
	m_pEnchant[ 0 ] = NiNew cSprite;
	m_pEnchant[ 0 ]->Init( NULL, CsPoint::ZERO, CsPoint( 26, 16 ), "Encyclopedia\\Encyclopedia_Tooltip_AT.tga", false );
	// 크리티컬
	m_pEnchant[ 1 ] = NiNew cSprite;
	m_pEnchant[ 1 ]->Init( NULL, CsPoint::ZERO, CsPoint( 26, 16 ), "Encyclopedia\\Encyclopedia_Tooltip_CT.tga", false );
	// 회피
	m_pEnchant[ 2 ] = NiNew cSprite;
	m_pEnchant[ 2 ]->Init( NULL, CsPoint::ZERO, CsPoint( 26, 16 ), "Encyclopedia\\Encyclopedia_Tooltip_EV.tga", false );
	// 블럭
	m_pEnchant[ 3 ] = NiNew cSprite;
	m_pEnchant[ 3 ]->Init( NULL, CsPoint::ZERO, CsPoint( 26, 16 ), "Encyclopedia\\Encyclopedia_Tooltip_BL.tga", false );
	// HP
	m_pEnchant[ 4 ] = NiNew cSprite;
	m_pEnchant[ 4 ]->Init( NULL, CsPoint::ZERO, CsPoint( 26, 16 ), "Encyclopedia\\Encyclopedia_Tooltip_HP.tga", false );

	// 툴팁 대기 시간
	m_DelayTime.SetEndTime( 0.4f );

	// Keep the first window open cheap. The render texture/model preview is only
	// needed after a card is opened, so create it lazily like the Union window
	// refreshes visible controls only when their data is needed.
	m_pOverviewBg = NiNew cSprite;
	m_pOverviewBg->Init( NULL, CsPoint::ZERO, CsPoint( 896, 746 ), "Encyclopedia\\newencyclopedia\\detail\\blind_bg_noalpha.png", false );
	m_pOverviewInfoTextBg = NiNew cSprite;
	m_pOverviewInfoTextBg->Init( NULL, CsPoint::ZERO, CsPoint( 397, 456 ), "Encyclopedia\\newencyclopedia\\detail\\info_text_bg.png", false );
	m_pOverviewRatingFrame = NiNew cSprite;
	m_pOverviewRatingFrame->Init( NULL, CsPoint::ZERO, CsPoint( 371, 36 ), "Encyclopedia\\newencyclopedia\\detail\\rating_text frame.png", false );
	m_pOverviewBox = NiNew cSprite;
	m_pOverviewBox->Init( NULL, CsPoint::ZERO, CsPoint( 91, 31 ), "Encyclopedia\\newencyclopedia\\detail\\box.png", false );
	m_pOverviewAttributeFrame = NiNew cSprite;
	m_pOverviewAttributeFrame->Init( NULL, CsPoint::ZERO, CsPoint( 35, 35 ), "Encyclopedia\\newencyclopedia\\detail\\attribute_frame.png", false );
	m_pOverviewSkillFrame = NiNew cSprite;
	m_pOverviewSkillFrame->Init( NULL, CsPoint::ZERO, CsPoint( 49, 50 ), "Encyclopedia\\newencyclopedia\\detail\\skill_frame.png", false );
	m_pOverviewSkillSelection = NiNew cSprite;
	m_pOverviewSkillSelection->Init( NULL, CsPoint::ZERO, CsPoint( 36, 37 ), "Encyclopedia\\newencyclopedia\\detail\\skill_selection.png", false );
	m_pOverviewSkillLine = NiNew cSprite;
	m_pOverviewSkillLine->Init( NULL, CsPoint::ZERO, CsPoint( 5, 16 ), "Encyclopedia\\newencyclopedia\\detail\\text_line_skill.png", false );
	m_pOverviewStatusArrow = NiNew cSprite;
	m_pOverviewStatusArrow->Init( NULL, CsPoint::ZERO, CsPoint( 21, 16 ), "Encyclopedia\\newencyclopedia\\detail\\status arrow.png", false );
	m_pOverviewStatusArrow2 = NiNew cSprite;
	m_pOverviewStatusArrow2->Init( NULL, CsPoint::ZERO, CsPoint( 27, 14 ), "Encyclopedia\\newencyclopedia\\detail\\status_arrow_2.png", false );
	m_pOverviewActionCardFrame = NiNew cSprite;
	m_pOverviewActionCardFrame->Init( NULL, CsPoint::ZERO, CsPoint( 63, 103 ), "Encyclopedia\\newencyclopedia\\detail\\action card frame.png", false );
	m_pOverviewSkillTabFrame = NiNew cImage;
	m_pOverviewSkillTabFrame->Init( NULL, CsPoint::ZERO, CsPoint( 96, 46 ), "Encyclopedia\\newencyclopedia\\detail\\button\\tab_mini.png", false, true );
	m_pOverviewSkillTabFrame->SetTexToken( CsPoint( 96, 46 ) );
	m_pOverviewSkillTabFrame->SetState( 1 );
	m_pOverviewBackBtn = NiNew cImage;
	m_pOverviewBackBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\detail\\button\\arrow_l.png", false, true );
	m_pOverviewBackBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewCloseBtn = NiNew cImage;
	m_pOverviewCloseBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\main\\exit_btn.png", false, true );
	m_pOverviewCloseBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewScrollBtn = NiNew cImage;
	m_pOverviewScrollBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 17 ), "Encyclopedia\\newencyclopedia\\detail\\button\\scroll_button.png", false, true );
	m_pOverviewScrollBtn->SetTexToken( CsPoint( 32, 17 ) );
	m_pOverviewScrollBtn->SetState( 0 );
	m_pOverviewZoomInBtn = NiNew cImage;
	m_pOverviewZoomInBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\detail\\button\\zoom_in.png", false, true );
	m_pOverviewZoomInBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewZoomOutBtn = NiNew cImage;
	m_pOverviewZoomOutBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\detail\\button\\zoom_out.png", false, true );
	m_pOverviewZoomOutBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewArrowLBtn = NiNew cImage;
	m_pOverviewArrowLBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\detail\\button\\arrow_l.png", false, true );
	m_pOverviewArrowLBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewArrowRBtn = NiNew cImage;
	m_pOverviewArrowRBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\detail\\button\\arrow_r.png", false, true );
	m_pOverviewArrowRBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewRotateLBtn = NiNew cImage;
	m_pOverviewRotateLBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "cash\\newcashshop\\store_btn_rotate_l.tga", false, true );
	m_pOverviewRotateLBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewRotateRBtn = NiNew cImage;
	m_pOverviewRotateRBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "cash\\newcashshop\\store_btn_rotate_r.tga", false, true );
	m_pOverviewRotateRBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewRotateUpBtn = NiNew cImage;
	m_pOverviewRotateUpBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "cash\\newcashshop\\store_btn_rotate_l.tga", false, true );
	m_pOverviewRotateUpBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewRotateDownBtn = NiNew cImage;
	m_pOverviewRotateDownBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "cash\\newcashshop\\store_btn_rotate_r.tga", false, true );
	m_pOverviewRotateDownBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewResetViewBtn = NiNew cImage;
	m_pOverviewResetViewBtn->Init( NULL, CsPoint::ZERO, CsPoint( 32, 32 ), "Encyclopedia\\newencyclopedia\\main\\re.png", false, true );
	m_pOverviewResetViewBtn->SetTexToken( CsPoint( 0, 32 ) );
	m_pOverviewTooltipLine = NiNew cSprite;
	m_pOverviewTooltipLine->Init( NULL, CsPoint::ZERO, CsPoint( 166, 1 ), "Encyclopedia\\newencyclopedia\\tooltip\\tooltip_line.png", false );

	cText::sTEXTINFO tiOverviewTitle;
	tiOverviewTitle.Init( &g_pEngine->m_FontSystem );
	tiOverviewTitle.s_eFontSize = CFont::FS_16;
	tiOverviewTitle.s_eTextAlign = DT_LEFT;
	tiOverviewTitle.s_Color = NiColor( 0.50f, 0.58f, 1.0f );
	m_pOverviewName = NiNew cText;
	m_pOverviewName->Init( NULL, CsPoint::ZERO, &tiOverviewTitle, false );

	cText::sTEXTINFO tiOverviewHeader;
	tiOverviewHeader.Init( &g_pEngine->m_FontSystem );
	tiOverviewHeader.s_eFontSize = CFont::FS_12;
	tiOverviewHeader.s_eTextAlign = DT_CENTER;
	tiOverviewHeader.s_Color = NiColor( 0.58f, 0.65f, 1.0f );
	cText::sTEXTINFO tiOverviewGrowthHeader = tiOverviewHeader;
	tiOverviewGrowthHeader.s_eFontSize = CFont::FS_14;
	tiOverviewGrowthHeader.s_Color = NiColor::WHITE;
	tiOverviewGrowthHeader.SetText( _T( "Digimon Growth Information" ) );
	m_pOverviewGrowthTitle = NiNew cText;
	m_pOverviewGrowthTitle->Init( NULL, CsPoint::ZERO, &tiOverviewGrowthHeader, false );
	tiOverviewHeader.SetText( _T( "Digimon Information" ) );
	m_pOverviewInfoTitle = NiNew cText;
	m_pOverviewInfoTitle->Init( NULL, CsPoint::ZERO, &tiOverviewHeader, false );
	tiOverviewHeader.SetText( _T( "Skill Info" ) );
	m_pOverviewSkillTab = NiNew cText;
	m_pOverviewSkillTab->Init( NULL, CsPoint::ZERO, &tiOverviewHeader, false );
	m_pOverviewSkillTab->SetColor( NiColor::WHITE );
	cText::sTEXTINFO tiOverviewInfoLine;
	tiOverviewInfoLine.Init( &g_pEngine->m_FontSystem );
	tiOverviewInfoLine.s_eFontSize = CFont::FS_12;
	tiOverviewInfoLine.s_eTextAlign = DT_LEFT;
	tiOverviewInfoLine.s_Color = NiColor( 0.62f, 0.68f, 1.0f );
	m_pOverviewRankLabel = NiNew cText;
	m_pOverviewRankLabel->Init( NULL, CsPoint::ZERO, &tiOverviewInfoLine, false );
	m_pOverviewRankLabel->SetText( _T( "Rank" ) );
	m_pOverviewRankValue = NiNew cText;
	m_pOverviewRankValue->Init( NULL, CsPoint::ZERO, &tiOverviewInfoLine, false );
	m_pOverviewStageLabel = NiNew cText;
	m_pOverviewStageLabel->Init( NULL, CsPoint::ZERO, &tiOverviewInfoLine, false );
	m_pOverviewStageLabel->SetText( _T( "Stage" ) );
	m_pOverviewStageValue = NiNew cText;
	m_pOverviewStageValue->Init( NULL, CsPoint::ZERO, &tiOverviewInfoLine, false );
	cText::sTEXTINFO tiOverviewSkillName;
	tiOverviewSkillName.Init( &g_pEngine->m_FontSystem );
	tiOverviewSkillName.s_eFontSize = CFont::FS_14;
	tiOverviewSkillName.s_eTextAlign = DT_CENTER;
	tiOverviewSkillName.s_Color = NiColor( 0.50f, 0.58f, 1.0f );
	m_pOverviewSkillName = NiNew cText;
	m_pOverviewSkillName->Init( NULL, CsPoint::ZERO, &tiOverviewSkillName, false );
	cText::sTEXTINFO tiOverviewBack;
	tiOverviewBack.Init( &g_pEngine->m_FontSystem );
	tiOverviewBack.s_eFontSize = CFont::FS_12;
	tiOverviewBack.s_eTextAlign = DT_LEFT;
	tiOverviewBack.s_Color = NiColor( 0.82f, 0.88f, 1.0f );
	tiOverviewBack.SetText( _T( "Back" ) );
	m_pOverviewBackText = NiNew cText;
	m_pOverviewBackText->Init( NULL, CsPoint::ZERO, &tiOverviewBack, false );

	cText::sTEXTINFO tiOverviewStat;
	tiOverviewStat.Init( &g_pEngine->m_FontSystem );
	tiOverviewStat.s_eFontSize = CFont::FS_10;
	tiOverviewStat.s_eTextAlign = DT_LEFT;
	tiOverviewStat.s_Color = NiColor( 0.62f, 0.68f, 1.0f );
	for( int i = 0; i < 8; ++i )
	{
		m_pOverviewStatText[ i ] = NiNew cText;
		m_pOverviewStatText[ i ]->Init( NULL, CsPoint::ZERO, &tiOverviewStat, false );
	}

	cText::sTEXTINFO tiOverviewBody;
	tiOverviewBody.Init( &g_pEngine->m_FontSystem );
	tiOverviewBody.s_eFontSize = CFont::FS_12;
	tiOverviewBody.s_eTextAlign = DT_LEFT;

	UNREFERENCED_PARAMETER( tiOverviewBody );
	m_pOverviewGrowth = NiNew cStringList;
	m_pOverviewInfo = NiNew cStringList;
	m_pOverviewSkillDesc = NiNew cStringList;
	m_pOverviewSkills = NiNew cStringList;
	m_pOverviewSkillStatsLeft = NiNew cStringList;
	m_pOverviewSkillStatsRight = NiNew cStringList;

	DWORD const dwListBegin = GetTickCount();
	_SelectMainTab( MAIN_TAB_ENCYCLOPEDIA, true );
	ENCY_PERF_INFO( "Create initial list built elapsedMs=%u",
		(unsigned)( GetTickCount() - dwListBegin ) );

	ENCY_PERF_INFO( "Create end serverReady=%d elapsedMs=%u",
		( GetSystem() && GetSystem()->IsServerDataReceived() ) ? 1 : 0,
		(unsigned)( GetTickCount() - dwCreateBegin ) );
}

void cEncyclopedia::Update(float const& fDeltaTime)
{
	_UpdateMoveWindow();

	// Search box focus feedback: hide placeholder on focus, restore on blur.
	if( m_pSearchBarBg && m_pSearchEdit && m_pSearchBarBg->GetVisible() && m_pSearchEdit->GetVisible() )
	{
		bool const bFocusNow = m_pSearchEdit->IsFocus();
		if( bFocusNow != m_bSearchFocusVisualOn )
		{
			m_bSearchFocusVisualOn = bFocusNow;
			if( bFocusNow )
			{
				m_pSearchEdit->SetEmptyMsgText( _T( "" ), NiColor( 0.6f, 0.65f, 0.85f ) );
			}
			else
			{
				m_pSearchEdit->SetEmptyMsgText( _T( "Search Digimon..." ), NiColor( 0.6f, 0.65f, 0.85f ) );
			}
		}
	}
	else if( m_bSearchFocusVisualOn && m_pSearchEdit )
	{
		m_bSearchFocusVisualOn = false;
		m_pSearchEdit->SetEmptyMsgText( _T( "Search Digimon..." ), NiColor( 0.6f, 0.65f, 0.85f ) );
	}

	_EnsureInitialListBuilt();

	_UpdateLazyListBuild();

	if( m_pCardHoverEffect )
	{
		__try
		{
			m_pCardHoverEffect->SetVisible( m_bCardHoverEffectVisible );
			if( m_bCardHoverEffectVisible )
			{
				m_pCardHoverEffect->SetSize( m_ptCardHoverEffectSize );
				m_pCardHoverEffect->Update( fDeltaTime );
			}
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			CLIENT_LOG_WARN( "ENCY", "Update shared hover effect failed; disabling effect." );
			m_bCardHoverEffectVisible = false;
		}
	}

	_UpdateOverviewRenderDigimon();
}

cBaseWindow::eMU_TYPE cEncyclopedia::Update_ForMouse()
{
	cEncyListRenderGuard guard( m_csListRender );
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();

	if( m_bPendingDeckTabRefresh )
	{
		m_bPendingDeckTabRefresh = false;
		_SetTabList( m_nCurTab );
		cScrollBar* pActiveScroll = _GetActiveScrollBar();
		if( pActiveScroll && m_nPendingDeckScrollPos >= 0 )
			pActiveScroll->SetCurPosIndex( m_nPendingDeckScrollPos );
		m_nPendingDeckScrollPos = -1;
	}

	if( muReturn == MUT_OUT_WINDOW )
	{
		m_bIsMouseOn = false;
		_SetCardHoverVisible( false );
		return muReturn;
	}

	if( m_bOverviewVisible )
	{
		m_nOverviewHoverSkillIdx = -1;
		CsPoint ptOverview = GetRootClient() + CsPoint( 0, 118 );
		const int nOverviewLayoutYOffset = 40;
		CsRect rtOverviewBack( ptOverview + CsPoint( 18, 14 ), CsSIZE( 100, 32 ) );
		CsRect rtOverviewClose( ptOverview + CsPoint( 856, 6 ), CsSIZE( 32, 32 ) );
		CsRect rtTopArrowL( ptOverview + CsPoint( 460, 71 ), CsSIZE( 32, 32 ) );
		CsRect rtTopArrowR( ptOverview + CsPoint( 836, 71 ), CsSIZE( 32, 32 ) );
		CsRect rtRotateL( ptOverview + CsPoint( 70, 585 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtZoomOut( ptOverview + CsPoint( 120, 588 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtZoomIn( ptOverview + CsPoint( 320, 588 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtRotateR( ptOverview + CsPoint( 370, 585 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtRotateUp( ptOverview + CsPoint( 405, 430 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtRotateDown( ptOverview + CsPoint( 405, 484 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtResetView( ptOverview + CsPoint( 405, 538 + nOverviewLayoutYOffset ), CsSIZE( 32, 32 ) );
		CsRect rtModelArea( ptOverview + CsPoint( 38, 100 + nOverviewLayoutYOffset ), CsSIZE( 410, 520 ) );
		const int nSliderY = 586 + nOverviewLayoutYOffset;
		const int nSliderRenderY = 597 + nOverviewLayoutYOffset;
		const int nSliderMinX = 165;
		const int nSliderMaxX = 285;
		const int nSliderX = nSliderMinX + (int)( ( nSliderMaxX - nSliderMinX ) * m_fOverviewZoomRate );
		CsRect rtSliderTrack( ptOverview + CsPoint( nSliderMinX, nSliderY ), CsSIZE( ( nSliderMaxX - nSliderMinX ) + 32, 35 ) );
		CsRect rtSliderKnob( ptOverview + CsPoint( nSliderX, nSliderRenderY ), CsSIZE( 32, 17 ) );

		_SetEncyImageButtonState( m_pOverviewBackBtn, rtOverviewBack );
		_SetEncyImageButtonState( m_pOverviewCloseBtn, rtOverviewClose );
		_SetEncyImageButtonState( m_pOverviewArrowLBtn, rtTopArrowL );
		_SetEncyImageButtonState( m_pOverviewArrowRBtn, rtTopArrowR );
		_SetEncyImageButtonState( m_pOverviewRotateLBtn, rtRotateL );
		_SetEncyImageButtonState( m_pOverviewRotateRBtn, rtRotateR );
		_SetEncyImageButtonState( m_pOverviewResetViewBtn, rtResetView );
		_SetEncyImageButtonState( m_pOverviewZoomOutBtn, rtZoomOut );
		_SetEncyImageButtonState( m_pOverviewZoomInBtn, rtZoomIn );
		_SetEncyImageButtonState( m_pOverviewScrollBtn, rtSliderKnob );

		if( CURSOR_ST.CheckClickBox( rtOverviewBack ) != CURSOR::BUTTON_OUTWINDOW )
		{
			m_bIsMouseOn = false;
			_SetCardHoverVisible( false );
			if( CURSOR_ST.GetButtonState() == CURSOR::LBUTTON_DOWN )
				_CloseOverview();
			return MUT_CLICK_FOCUS;
		}

		if( CURSOR_ST.CheckClickBox( rtOverviewClose ) != CURSOR::BUTTON_OUTWINDOW )
		{
			m_bIsMouseOn = false;
			_SetCardHoverVisible( false );
			if( CURSOR_ST.GetButtonState() == CURSOR::LBUTTON_DOWN )
				_CloseOverview();
			return MUT_CLICK_FOCUS;
		}

		CsRect rtOverview( ptOverview, CsSIZE( 896, 746 ) );
		if( CURSOR_ST.CheckClickBox( rtOverview ) != CURSOR::BUTTON_OUTWINDOW )
		{
			m_bIsMouseOn = false;
			_SetCardHoverVisible( false );

			int const eBtnState = CURSOR_ST.GetButtonState();
			bool const bLButtonDown = ( eBtnState == CURSOR::LBUTTON_DOWN );
			bool const bLButtonHold = ( eBtnState == CURSOR::LBUTTON_PRESS );
			bool const bLButtonUp = ( eBtnState == CURSOR::LBUTTON_UP );
			bool const bLButtonActive = ( bLButtonDown || bLButtonHold );
			if( bLButtonUp )
			{
				m_bOverviewModelDragRotate = false;
				m_bOverviewZoomSliderDrag = false;
			}
			if( nsCsFileTable::g_pDigimonMng && nsCsFileTable::g_pSkillMng )
			{
				CsDigimon* pOverviewDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( m_nOverviewDigimonId );
				CsDigimon::sINFO* pOverviewInfo = pOverviewDigimon ? pOverviewDigimon->GetInfo() : NULL;
				if( pOverviewInfo )
				{
					const int nOverviewLowerShiftY = -6;
					const int nInfoPanelShiftY = 16;
					CsPoint ptSkillIconPos = ptOverview + CsPoint( 473, 420 + nOverviewLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY );
					const CsSIZE szSkillIcon( 36, 36 );
					for( int i = 0; i < DIGIMON_MAX_SKILL_COUNT; ++i )
					{
						DWORD dwSkillId = pOverviewInfo->s_Skill[ i ].s_dwID;
						CsRect rtSkillIcon( ptSkillIconPos, szSkillIcon );
						if( dwSkillId != 0 && CURSOR_ST.CheckClickBox( rtSkillIcon ) != CURSOR::BUTTON_OUTWINDOW )
						{
							CsSkill* pSkill = nsCsFileTable::g_pSkillMng->GetSkill( dwSkillId );
							if( pSkill && pSkill->GetInfo() )
							{
								m_nOverviewHoverSkillIdx = i;
								if( bLButtonDown )
								{
									m_nOverviewSelectedSkillIdx = i;
									_UpdateOverviewSkillSection();
								}
							}
							break;
						}
						ptSkillIconPos.x += 68;
					}
				}
			}
			if( bLButtonActive )
			{
				bool const bOnControllerButton =
					( CURSOR_ST.CheckClickBox( rtTopArrowL ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtTopArrowR ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtRotateL ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtRotateR ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtResetView ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtZoomOut ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtZoomIn ) != CURSOR::BUTTON_OUTWINDOW ) ||
					( CURSOR_ST.CheckClickBox( rtSliderTrack ) != CURSOR::BUTTON_OUTWINDOW );

				auto ClampOverviewZoomTarget = [this]()
				{
					if( m_fOverviewZoomTargetRate < 0.0f ) m_fOverviewZoomTargetRate = 0.0f;
					if( m_fOverviewZoomTargetRate > 1.0f ) m_fOverviewZoomTargetRate = 1.0f;
				};
				auto SetOverviewZoomFromSlider = [this, nSliderMinX, nSliderMaxX]( int nMouseX, int nDragOffsetX )
				{
					int nKnobX = nMouseX - nDragOffsetX;
					if( nKnobX < nSliderMinX ) nKnobX = nSliderMinX;
					if( nKnobX > nSliderMaxX ) nKnobX = nSliderMaxX;
					float fRange = (float)( nSliderMaxX - nSliderMinX );
					m_fOverviewZoomTargetRate = ( fRange > 0.0f ) ? ( (float)( nKnobX - nSliderMinX ) / fRange ) : 0.5f;
				};

				auto NavigateOverview = [this]( bool bNext )
				{
					std::vector< std::pair<int, int> > vOverviewOrder;
					for( size_t i = 0; i < m_vCardHoverInfo.size(); ++i )
					{
						int nId = m_vCardHoverInfo[ i ].s_nDigimonID;
						if( nId <= 0 )
							continue;
						bool bExists = false;
						for( size_t j = 0; j < vOverviewOrder.size(); ++j )
						{
							if( vOverviewOrder[ j ].first == nId )
							{
								bExists = true;
								break;
							}
						}
						if( bExists )
							continue;
						vOverviewOrder.push_back( std::make_pair( nId, m_vCardHoverInfo[ i ].s_nTooltipDigimonID ) );
					}

					if( vOverviewOrder.empty() )
						return;

					size_t nCurIndex = 0;
					bool bFound = false;
					for( size_t i = 0; i < vOverviewOrder.size(); ++i )
					{
						if( vOverviewOrder[ i ].first == m_nOverviewDigimonId )
						{
							nCurIndex = i;
							bFound = true;
							break;
						}
					}

					if( !bFound )
						nCurIndex = 0;

					size_t nNextIndex = nCurIndex;
					if( bNext )
						nNextIndex = ( nCurIndex + 1 ) % vOverviewOrder.size();
					else
						nNextIndex = ( nCurIndex + vOverviewOrder.size() - 1 ) % vOverviewOrder.size();

					_OpenOverview( vOverviewOrder[ nNextIndex ].first, vOverviewOrder[ nNextIndex ].second );
				};

				if( bLButtonDown && CURSOR_ST.CheckClickBox( rtTopArrowL ) != CURSOR::BUTTON_OUTWINDOW )
					NavigateOverview( false );
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtTopArrowR ) != CURSOR::BUTTON_OUTWINDOW )
					NavigateOverview( true );
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtResetView ) != CURSOR::BUTTON_OUTWINDOW )
				{
					m_fOverviewRenderYaw = 0.0f;
					m_fOverviewRenderPitch = 0.0f;
					m_fOverviewZoomRate = 0.5f;
					m_fOverviewZoomTargetRate = 0.5f;
					m_bOverviewModelDragRotate = false;
				}
				else if( !m_bOverviewZoomSliderDrag && !bOnControllerButton && CURSOR_ST.CheckClickBox( rtModelArea ) != CURSOR::BUTTON_OUTWINDOW )
				{
					if( bLButtonDown )
					{
						m_bOverviewModelDragRotate = true;
						m_ptOverviewModelDragPrev = CURSOR_ST.GetPos();
					}
					else if( bLButtonHold && m_bOverviewModelDragRotate )
					{
						CsPoint ptCur = CURSOR_ST.GetPos();
						CsPoint ptDelta = ptCur - m_ptOverviewModelDragPrev;
						m_ptOverviewModelDragPrev = ptCur;

						// Keep the vertical axis locked; only horizontal preview rotation is interactive.
						m_fOverviewRenderYaw -= (float)( ptDelta.x ) * 0.012f;
						m_fOverviewRenderPitch = 0.0f;

						while( m_fOverviewRenderYaw > 6.2831853f ) m_fOverviewRenderYaw -= 6.2831853f;
						while( m_fOverviewRenderYaw < 0.0f ) m_fOverviewRenderYaw += 6.2831853f;
					}
				}
				else if( CURSOR_ST.CheckClickBox( rtRotateL ) != CURSOR::BUTTON_OUTWINDOW )
				{
					m_fOverviewRenderYaw += 0.12f;
				}
				else if( CURSOR_ST.CheckClickBox( rtRotateR ) != CURSOR::BUTTON_OUTWINDOW )
				{
					m_fOverviewRenderYaw -= 0.12f;
				}
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtZoomOut ) != CURSOR::BUTTON_OUTWINDOW )
				{
					m_fOverviewZoomTargetRate = m_fOverviewZoomRate - m_fOverviewZoomStep;
					ClampOverviewZoomTarget();
				}
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtZoomIn ) != CURSOR::BUTTON_OUTWINDOW )
				{
					m_fOverviewZoomTargetRate = m_fOverviewZoomRate + m_fOverviewZoomStep;
					ClampOverviewZoomTarget();
				}
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtSliderKnob ) != CURSOR::BUTTON_OUTWINDOW )
				{
					int nMouseX = CURSOR_ST.GetPos().x - ptOverview.x;
					m_bOverviewZoomSliderDrag = true;
					m_nOverviewZoomSliderDragOffsetX = nMouseX - nSliderX;
					if( m_nOverviewZoomSliderDragOffsetX < 0 ) m_nOverviewZoomSliderDragOffsetX = 0;
					if( m_nOverviewZoomSliderDragOffsetX > 32 ) m_nOverviewZoomSliderDragOffsetX = 32;
					SetOverviewZoomFromSlider( nMouseX, m_nOverviewZoomSliderDragOffsetX );
				}
				else if( bLButtonDown && CURSOR_ST.CheckClickBox( rtSliderTrack ) != CURSOR::BUTTON_OUTWINDOW )
				{
					int nMouseX = CURSOR_ST.GetPos().x - ptOverview.x;
					m_bOverviewZoomSliderDrag = true;
					m_nOverviewZoomSliderDragOffsetX = 16;
					SetOverviewZoomFromSlider( nMouseX, m_nOverviewZoomSliderDragOffsetX );
				}
				else if( bLButtonHold && m_bOverviewZoomSliderDrag )
				{
					int nMouseX = CURSOR_ST.GetPos().x - ptOverview.x;
					SetOverviewZoomFromSlider( nMouseX, m_nOverviewZoomSliderDragOffsetX );
				}
			}

			return MUT_CLICK_FOCUS;
		}
	}

	if( CURSOR_ST.CheckClickBox( CsRect( GetRootClient(), CsSIZE( 886, 36 ) ) ) != CURSOR::BUTTON_OUTWINDOW )
	{
		_UpdateMoveWindow_ForMouse();
	}

	if( m_pMoveButton && m_pMoveButton->Update_ForMouse() != cButton::ACTION_NONE )
	{
		_UpdateMoveWindow_ForMouse();
	}
	else if( m_pMoveButton
		&& CURSOR_ST.CheckClickBox( CsRect( m_pMoveButton->GetPos(), m_pMoveButton->GetPos() + m_pMoveButton->GetSize() ) ) != CURSOR::BUTTON_OUTWINDOW )
	{
		// Keep the move cross fully draggable even when button action state is not emitted.
		_UpdateMoveWindow_ForMouse();
	}

	if(m_pCancelButton && m_pCancelButton->Update_ForMouse() == cButton::ACTION_CLICK )		// 캔슬 버튼
	{
		return muReturn;
	}

	if(m_pStatisticsBtn && m_pStatisticsBtn->Update_ForMouse() == cButton::ACTION_CLICK )	//통계버튼
	{
		return muReturn;
	}

	for( int i = 0; i < 2; ++i )
	{
		if( m_pMainTabBtn[ i ] && m_pMainTabBtn[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
			return muReturn;
	}
	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA || m_nMainTab == MAIN_TAB_DECK )
	{
		if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )
		{
			for( int i = 0; i < 3; ++i )
			{
				if( m_pSubMenuBtn[ i ] && m_pSubMenuBtn[ i ]->Update_ForMouse() == cButton::ACTION_CLICK )
					return muReturn;
			}
		}
		if( m_pSearchBtn && m_pSearchBtn->GetVisible() && m_pSearchBtn->Update_ForMouse() == cButton::ACTION_CLICK )
			return muReturn;
		if( m_pSearchResetBtn && m_pSearchResetBtn->GetVisible() && m_pSearchResetBtn->Update_ForMouse() == cButton::ACTION_CLICK )
			return muReturn;
		if( m_pSearchEdit && m_pSearchEdit->GetVisible() )
		{
			cEditBox::eACTION eSearchAction = (cEditBox::eACTION)m_pSearchEdit->Update_ForMouse( CsPoint::ZERO );

			// Live search: refresh list automatically while typing.
			std::wstring wsNow = m_pSearchEdit->GetString();
			if( wsNow != m_wsSearchKeyword )
			{
				m_wsSearchKeyword = wsNow;
				if( m_nMainTab == MAIN_TAB_DECK )
					_InvalidateDeckListCache();
				else
					_InvalidateEncyListCache();
				_SetTabList( 0 );
				return muReturn;
			}

			if( eSearchAction != cEditBox::ACTION_NONE )
				return muReturn;
		}
	}
	cListBox* pActiveListBox = _GetActiveListBox();
	bool const bListMouseUpdated = ( pActiveListBox && pActiveListBox->Update_ForMouse( CURSOR_ST.GetPos() ) );
	if( m_nMainTab == MAIN_TAB_DECK && _UpdateDeckBookmarkVisuals() )
	{
		_InvalidateDeckListCache();
		_SetTabList( 0 );
		if( m_pDeckScrollBar && m_pDeckScrollBar->IsEnableScroll() )
			m_pDeckScrollBar->SetCurPosIndex( 0 );
		return muReturn;
	}
	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )
	{
		_UpdateCardHover();
		if( bListMouseUpdated == false )
			return muReturn;
	}
	else if( bListMouseUpdated == false )
	{
		_SetCardHoverVisible( false );
		return muReturn;
	}

	if( pActiveListBox && bListMouseUpdated )
	{
		cListBoxItem const* pOverItem = pActiveListBox->GetMouseOverItem();
		SAFE_POINTER_RETVAL(pOverItem, muReturn);
		SAFE_POINTER_RETVAL(g_pGameIF, muReturn);

		if(!pOverItem->GetScrollVisible())
		{
			return muReturn;
		}

		sDIGIMON_ID* pInfo = static_cast<sDIGIMON_ID*>(pOverItem->GetUserData());
		if( pInfo == NULL )
		{
			CLIENT_LOG_WARN( "ENCY", "Update_ForMouse skipped item with null user data. mainTab=%d subMenu=%d", m_nMainTab, m_nSubMenu );
			return muReturn;
		}

		if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )
		{
			int nTooltipDigimonId = 0;
			CsPoint ptLocalMouse = pActiveListBox->GetItemtoMousePos( CURSOR_ST.GetPos(), const_cast<cListBoxItem*>( pOverItem ) );
			CsPoint ptItemScreen = CURSOR_ST.GetPos() - ptLocalMouse;
			CsRect rtTooltipCardScreen;
			for( size_t i = 0; i < m_vCardHoverInfo.size(); ++i )
			{
				if( m_vCardHoverInfo[ i ].s_pItem != pOverItem )
					continue;

				CsRect rtHover = m_vCardHoverInfo[ i ].s_LocalRect;
				CsPoint ptHoverMouse = ptLocalMouse;
				if( rtHover.PtInRect( ptHoverMouse ) == FALSE )
					continue;

				nTooltipDigimonId = ( m_vCardHoverInfo[ i ].s_nTooltipDigimonID > 0 ) ? m_vCardHoverInfo[ i ].s_nTooltipDigimonID : m_vCardHoverInfo[ i ].s_nDigimonID;
				rtTooltipCardScreen = CsRect( ptItemScreen + rtHover.GetPos(), rtHover.GetSize() );
				break;
			}

			if( nTooltipDigimonId <= 0 )
			{
				m_bIsMouseOn = false;
				return muReturn;
			}
			_SetCardNameTooltip( nTooltipDigimonId, rtTooltipCardScreen );
		}
		else
		{
			bIsTooltip = false;
			_Updata_ForMouse_Group( pInfo->iCount );
			CsPoint ptLocalMouse = pActiveListBox->GetItemtoMousePos( CURSOR_ST.GetPos(), const_cast<cListBoxItem*>( pOverItem ) );
			int nDeckHoverGroup = _GetDeckHoverGroupIdx( const_cast<cListBoxItem*>( pOverItem ), ptLocalMouse );

			if(bIsTooltip)
				_SetGroupTooltip( ( nDeckHoverGroup >= 0 ) ? nDeckHoverGroup : pInfo->iDigimonId );
		}

		return muReturn;
	}

	return muReturn;
}

void cEncyclopedia::Render()
{
	cEncyListRenderGuard guard( m_csListRender );
	RenderScript();

	if( m_pCardHoverEffect && m_bCardHoverEffectVisible && !m_bOverviewVisible )
		m_pCardHoverEffect->Render( m_ptCardHoverEffectPos, m_ptCardHoverEffectSize );

	if( m_bOverviewVisible )
		_RenderOverview();

	if( m_bIsMouseOn && !m_bOverviewVisible )		//툴팁
	{
		CsPoint ptMousePos = CURSOR_ST.GetPos();

		if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )
			_RenderTooltip(ptMousePos);
	}

	if( !m_bOverviewVisible )
		_RenderCardNameTooltip();

	EndRenderScript();
}

void cEncyclopedia::OnMoveWindow()
{
	cBaseWindow::OnMoveWindow();
}

void cEncyclopedia::ResetDevice()
{
	ResetDeviceScript();
	if( m_pOverviewRenderTex )
		m_pOverviewRenderTex->ResetDevice();
}

bool cEncyclopedia::OnEscapeKey()
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ENCYCLOPEDIA_STATS ) == false )		//통계창 켜져 있을 땐 통계창 먼저 끔
	{
		Close();
		return true;
	}
	return false;
}

void cEncyclopedia::_RenderTooltip( CsPoint ptMousePos )
{
	if( m_DelayTime.IsCoolTime() == true )
		return;

	if( m_pTooltipBG == NULL || m_szName == NULL || m_szLevel == NULL || m_szScale == NULL || m_szExplain == NULL )
	{
		CLIENT_LOG_WARN( "ENCY", "_RenderTooltip missing tooltip controls. digimonId=%d", m_nTooltipDigimonID );
		m_bIsMouseOn = false;
		return;
	}

	//툴팁 배경
	m_pTooltipBG->Render( ptMousePos );

	//계열체 이름
	m_szName->Render( ptMousePos + CsPoint( 115, 11 ), DT_CENTER );

	//레벨,스케일
	m_szLevel->Render( ptMousePos + CsPoint( 86, 38 ), DT_CENTER );
	m_szScale->Render( ptMousePos + CsPoint( 86, 62 ), DT_CENTER );

	//강화종류
	int nFirst  = GetSystem()->GetEnchant_Order( 0, m_pEnchantVal[0], m_pEnchantVal[1], m_pEnchantVal[2], m_pEnchantVal[3], m_pEnchantVal[4] );
	int nSecond = GetSystem()->GetEnchant_Order( 1, m_pEnchantVal[0], m_pEnchantVal[1], m_pEnchantVal[2], m_pEnchantVal[3], m_pEnchantVal[4] );

	if( nFirst < 0 || nFirst >= IF_ENCY_MAX_ENCHANT_COUNT )
	{
		CLIENT_LOG_WARN( "ENCY", "_RenderTooltip invalid first enchant index=%d digimonId=%d", nFirst, m_nTooltipDigimonID );
		nFirst = -1;
	}
	if( nSecond < 0 || nSecond >= IF_ENCY_MAX_ENCHANT_COUNT )
	{
		CLIENT_LOG_WARN( "ENCY", "_RenderTooltip invalid second enchant index=%d digimonId=%d", nSecond, m_nTooltipDigimonID );
		nSecond = -1;
	}

	//강화 값이 0이면 안보여줌
	if( nFirst >= 0 && m_pEnchantVal[ nFirst ] != 0 && m_pEnchant[ nFirst ] != NULL )
		m_pEnchant[ nFirst ]->Render( ptMousePos + CsPoint( 136, 37 ) );

	if( nSecond >= 0 && m_pEnchantVal[ nSecond ] != 0 && m_pEnchant[ nSecond ] != NULL )
		m_pEnchant[ nSecond ]->Render( ptMousePos + CsPoint( 136, 61 ) );

	//강화 수치
	if( m_szEnchantVal[ 0 ] )
		m_szEnchantVal[ 0 ]->Render( ptMousePos + CsPoint( 185, 38 ), DT_CENTER );//185, 38
	if( m_szEnchantVal[ 1 ] )
		m_szEnchantVal[ 1 ]->Render( ptMousePos + CsPoint( 185, 62 ), DT_CENTER );//185, 62

	//계열체 설명
	m_szExplain->Render( ptMousePos + CsPoint( 10, 83 ), 2 );

	m_bIsMouseOn = false;
}

void cEncyclopedia::_SetTabList( int nCurRadioIdx )
{
	cEncyListRenderGuard guard( m_csListRender );
	DWORD const dwBegin = GetTickCount();
	int const nMainTabAtBegin = m_nMainTab;
	int const nSubMenuAtBegin = m_nSubMenu;
	_CloseOverview();
	_UpdateActiveListVisibility();
	cListBox* pActiveListBox = _GetActiveListBox();
	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA
		&& m_bEncyListBuilt
		&& m_nEncyListCacheSubMenu == m_nSubMenu
		&& m_wsEncyListCacheSearch == m_wsSearchKeyword
		&& pActiveListBox
		&& pActiveListBox->GetItemCount() > 0 )
	{
		ENCY_PERF_INFO( "_SetTabList cache hit main=%d sub=%d rows=%d elapsedMs=%u",
			m_nMainTab,
			m_nSubMenu,
			(int)pActiveListBox->GetItemCount(),
			(unsigned)( GetTickCount() - dwBegin ) );
		return;
	}
	if( m_nMainTab == MAIN_TAB_DECK
		&& m_bDeckListBuilt
		&& m_wsDeckListCacheSearch == m_wsSearchKeyword
		&& pActiveListBox
		&& pActiveListBox->GetItemCount() > 0 )
	{
		ENCY_PERF_INFO( "_SetTabList cache hit main=%d sub=%d rows=%d elapsedMs=%u",
			m_nMainTab,
			m_nSubMenu,
			(int)pActiveListBox->GetItemCount(),
			(unsigned)( GetTickCount() - dwBegin ) );
		return;
	}

	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA && m_bEncyLazyAppendActive )
	{
		SetTabListData();
		ENCY_PERF_INFO( "_SetTabList lazy append main=%d sub=%d rows=%d elapsedMs=%u",
			m_nMainTab,
			m_nSubMenu,
			_GetActiveListBox() ? (int)_GetActiveListBox()->GetItemCount() : 0,
			(unsigned)( GetTickCount() - dwBegin ) );
		return;
	}

	if( m_nMainTab == MAIN_TAB_DECK && m_bDeckLazyAppendActive )
	{
		SetTabGroupListData();
		ENCY_PERF_INFO( "_SetTabList lazy append main=%d sub=%d rows=%d elapsedMs=%u",
			m_nMainTab,
			m_nSubMenu,
			_GetActiveListBox() ? (int)_GetActiveListBox()->GetItemCount() : 0,
			(unsigned)( GetTickCount() - dwBegin ) );
		return;
	}

	m_pCurTabList_map.clear();

	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )	// encyclopedia 서브 메뉴
	{
		EncyclopediaContents::MAP_CIT it    = GetSystem()->GetEncyMapData().begin();
		EncyclopediaContents::MAP_CIT itEnd = GetSystem()->GetEncyMapData().end();
		std::vector< EncyclopediaContents::sEVOL_INFO* > vecClassOrder;

		for( ; it != itEnd ; it++ )
		{
			if( nsCsFileTable::g_pDigimonMng->_IsExceptionDigimon( it->first ) == false )	// 제외 디지몬인지 확인
			{
				EncyclopediaContents::sEVOL_INFO* pInfo = it->second;
				if( pInfo == NULL )
					continue;

				if( m_nSubMenu == SUBMENU_SERIES )
				{
					m_pCurTabList_map[ pInfo->s_nOrder ] = pInfo;
				}
				else if( m_nSubMenu == SUBMENU_ALL )
				{
					m_pCurTabList_map[ it->first ] = pInfo;
				}
				else
				{
					vecClassOrder.push_back( pInfo );
				}
			}
		}
		if( m_nSubMenu == SUBMENU_CLASS )
		{
			std::sort( vecClassOrder.begin(), vecClassOrder.end(),
				[]( EncyclopediaContents::sEVOL_INFO* lhs, EncyclopediaContents::sEVOL_INFO* rhs )
				{
					if( lhs->s_nEnchant == rhs->s_nEnchant )
						return lhs->s_nOrder < rhs->s_nOrder;
					return lhs->s_nEnchant > rhs->s_nEnchant;
				} );
			for( size_t i = 0; i < vecClassOrder.size(); ++i )
				m_pCurTabList_map[ (int)i ] = vecClassOrder[ i ];
		}

		// 마지막 3개중 2칸짜리(3칸짜리) 확인
		// 여러칸이면 늘어난 칸만큼 스크롤바 사이즈 늘려줌
		EncyclopediaContents::MAP_IT itCurList = m_pCurTabList_map.begin();
		EncyclopediaContents::MAP_IT itCurListEnd = m_pCurTabList_map.end();

		for( int i = 0 ; i < 5 && itCurList != itCurListEnd ; i++ )
		{
			itCurListEnd--;

			if( itCurListEnd->second == NULL )
				break;

			int nCnt = itCurListEnd->second->s_nCount;// 몇줄짜리인지 판단하기위함

			if( itCurListEnd == itCurList )	// 거슬러 올라가다 처음까지 갔으면 break
				break;
		}

		SetTabListData();
	}
	else // deck 탭
	{
		DWORD const dwDeckStateBegin = GetTickCount();
		EncyclopediaContents::MAP_GROUP_CIT itGroup    = GetSystem()->GetGroupMapData().begin();
		EncyclopediaContents::MAP_GROUP_CIT itGroupEnd = GetSystem()->GetGroupMapData().end();

		for( ; itGroup != itGroupEnd ; ++itGroup )
		{
			EncyclopediaContents::sGROUP_INFO* pGroup = itGroup->second;
			if( pGroup == NULL )
				continue;

			std::map< DWORD, std::vector< EncyclopediaContents::sDATA* > > mapEntriesByDest;
			EncyclopediaContents::MAP_DATA_CIT itList	 = pGroup->s_pListData.begin();
			EncyclopediaContents::MAP_DATA_CIT itListEnd = pGroup->s_pListData.end();
			for( ; itList != itListEnd ; ++itList )
			{
				EncyclopediaContents::sDATA* pData = (*itList);
				if( pData )
					mapEntriesByDest[ pData->s_dwDestDigimonID ].push_back( pData );
			}

			pGroup->s_bGroupOpen = true;

			itList	 = pGroup->s_pListData.begin();
			itListEnd = pGroup->s_pListData.end();
			for( ; itList != itListEnd ; ++itList )
			{
				EncyclopediaContents::sDATA* pDeckData = (*itList);
				if( pDeckData == NULL )
					continue;

				pDeckData->s_bIsOpen = false;

				int nDigimonID = pDeckData->s_dwBaseDigimonID;
				EncyclopediaContents::MAP_CIT itInfo = GetSystem()->GetEncyMapData().find( nDigimonID );
				if( itInfo == GetSystem()->GetEncyMapData().end() || itInfo->second == NULL )
				{
					pGroup->s_bGroupOpen = false;
					continue;
				}

				for( int i = 0;i < nLimit::EvoUnit ; i++)
				{
					if( itInfo->second->s_sInfo[i].s_nDigimonID != pDeckData->s_dwDestDigimonID )
						continue;
					if( itInfo->second->s_sInfo[i].s_eImgState == EncyclopediaContents::sINFO::S_OPEN )
						pDeckData->s_bIsOpen = true;
					else
						pGroup->s_bGroupOpen = false;

					std::map< DWORD, std::vector< EncyclopediaContents::sDATA* > >::const_iterator itSameDest =
						mapEntriesByDest.find( pDeckData->s_dwDestDigimonID );
					if( itSameDest == mapEntriesByDest.end() )
						break;

					std::vector< EncyclopediaContents::sDATA* > const& vecSameDest = itSameDest->second;
					for( size_t nSameDest = 0; nSameDest < vecSameDest.size(); ++nSameDest )
					{
						EncyclopediaContents::sDATA* pSameDest = vecSameDest[ nSameDest ];
						if( pSameDest == NULL || pSameDest->s_bIsRender != false || pDeckData->s_bIsOpen != true )
							continue;

						EncyclopediaContents::MAP_CIT itTempInfo = GetSystem()->GetEncyMapData().find( pSameDest->s_dwBaseDigimonID );
						if( itTempInfo == GetSystem()->GetEncyMapData().end() || itTempInfo->second == NULL )
							continue;

						if( itTempInfo->second->s_sInfo[i].s_eImgState == EncyclopediaContents::sINFO::S_CLOSE )
							pDeckData->s_bIsOpen = false;
					}

					break;
				}
			}

			// Reconcile the legacy base/slot check with modern deck data. Some
			// converted decks reference a collected destination through a different
			// encyclopedia line, so the exact slot walk can mark a complete group
			// as closed and hide the Enable Effect button.
			for( itList = pGroup->s_pListData.begin(); itList != itListEnd ; ++itList )
			{
				EncyclopediaContents::sDATA* pDeckData = (*itList);
				if( pDeckData && pDeckData->s_bIsOpen == false )
					pDeckData->s_bIsOpen = _IsDeckDataOpenInEncyclopedia( GetSystem()->GetEncyMapData(), pDeckData );
			}
			pGroup->s_bGroupOpen = _IsDeckGroupCompleteByList( GetSystem()->GetEncyMapData(), pGroup );
		}

		// 마지막 3개중 2칸짜리(3칸짜리) 확인
		// 여러칸이면 늘어난 칸만큼 스크롤바 사이즈 늘려줌
		ENCY_PERF_INFO( "_SetTabList deck state end groups=%d elapsedMs=%u",
			GetSystem() ? (int)GetSystem()->GetGroupMapData().size() : 0,
			(unsigned)( GetTickCount() - dwDeckStateBegin ) );

		itGroup = GetSystem()->GetGroupMapData().begin();
		itGroupEnd = GetSystem()->GetGroupMapData().end();

		for( int i = 0 ; i < 5 ; i++ )
		{
			itGroupEnd--;

			if( itGroupEnd->second == NULL )
				break;

			int nCnt = itGroupEnd->second->s_cRanderCnt;// 몇줄짜리인지 판단하기위함

			if( itGroupEnd == itGroup )	// 거슬러 올라가다 처음까지 갔으면 break
				break;
		}

		SetTabGroupListData();
	}

	//Refresh가 아닌 경우에만 스크롤바 설정
	if( m_nCurTab != nCurRadioIdx )
	{
		m_nCurTab = nCurRadioIdx;
	}
	else	//Refresh인 경우 툴팁이랑 통계 있을 때
	{
		if( m_nTooltipDigimonID != 0 )
		{
			_SetTooltip( 0 );//툴팁 새로고침
			_SetGroupTooltip( 0 );//툴팁 새로고침
		}
		if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ENCYCLOPEDIA_STATS ) )
			g_pGameIF->GetEncyclopedia_Statistics()->_SetText_Stat();//통계 새로고침
	}
	ENCY_PERF_INFO( "_SetTabList end radio=%d main=%d sub=%d encyCount=%d groupCount=%d curTabItems=%d listItems=%d elapsedMs=%u",
		nCurRadioIdx,
		nMainTabAtBegin,
		nSubMenuAtBegin,
		GetSystem() ? (int)GetSystem()->GetEncyMapData().size() : 0,
		GetSystem() ? (int)GetSystem()->GetGroupMapData().size() : 0,
		(int)m_pCurTabList_map.size(),
		_GetActiveListBox() ? (int)_GetActiveListBox()->GetItemCount() : 0,
		(unsigned)( GetTickCount() - dwBegin ) );
}

void cEncyclopedia::_RefreshList()
{
	ENCY_PERF_INFO( "_RefreshList begin show=%d serverReady=%d",
		IsShowWindow() ? 1 : 0,
		( GetSystem() && GetSystem()->IsServerDataReceived() ) ? 1 : 0 );

	_InvalidateEncyListCache();
	_InvalidateDeckListCache();

	if( GetSystem() == NULL )
		return;

	_SelectMainTab( m_nMainTab, true );
}

cListBox* cEncyclopedia::_GetActiveListBox() const
{
	return ( m_nMainTab == MAIN_TAB_DECK && m_pDeckListBox ) ? m_pDeckListBox : m_pEncyListBox;
}

cScrollBar* cEncyclopedia::_GetActiveScrollBar() const
{
	return ( m_nMainTab == MAIN_TAB_DECK && m_pDeckScrollBar ) ? m_pDeckScrollBar : m_pScrollBar;
}

void cEncyclopedia::_UpdateActiveListVisibility()
{
	if( m_pEncyListBox )
		m_pEncyListBox->SetVisible( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA );
	if( m_pDeckListBox )
		m_pDeckListBox->SetVisible( m_nMainTab == MAIN_TAB_DECK );
}

void cEncyclopedia::_ResetInitialTabState()
{
	m_nMainTab = MAIN_TAB_ENCYCLOPEDIA;
	m_nSubMenu = SUBMENU_SERIES;
	m_nCurTab = -1;
	m_nTooltipDigimonID = 0;
	m_nPendingDeckScrollPos = -1;
	m_bPendingDeckTabRefresh = false;
	m_wsSearchKeyword.clear();
	m_bSearchFocusVisualOn = false;

	if( m_pSearchEdit )
	{
		m_pSearchEdit->SetText( _T( "" ), false );
		m_pSearchEdit->SetEmptyMsgText( _T( "Search Digimon..." ), NiColor( 0.6f, 0.65f, 0.85f ) );
	}

	for( int i = 0 ; i < 2 ; ++i )
	{
		if( m_pMainTabBtn[ i ] )
			m_pMainTabBtn[ i ]->SetMouseOnMode( i == MAIN_TAB_ENCYCLOPEDIA );
	}

	for( int i = 0 ; i < 3 ; ++i )
	{
		if( m_pSubMenuBtn[ i ] )
		{
			m_pSubMenuBtn[ i ]->SetVisible( true );
			m_pSubMenuBtn[ i ]->SetMouseOnMode( i == SUBMENU_SERIES );
		}
		if( m_pSubMenuText[ i ] )
			m_pSubMenuText[ i ]->SetVisible( true );
	}

	if( m_pStatisticsBtn )
		m_pStatisticsBtn->SetVisible( true );
	if( m_pSubMenuText[ SUBMENU_STATISTICS ] )
		m_pSubMenuText[ SUBMENU_STATISTICS ]->SetVisible( true );

	if( m_pScrollBar && m_pScrollBar->IsEnableScroll() )
		m_pScrollBar->SetCurPosIndex( 0 );
	if( m_pDeckScrollBar && m_pDeckScrollBar->IsEnableScroll() )
		m_pDeckScrollBar->SetCurPosIndex( 0 );

	_UpdateActiveListVisibility();
}

void cEncyclopedia::_SelectMainTab( int nMainTab, bool bForceRefresh )
{
	if( bForceRefresh == false && m_nMainTab == nMainTab )
		return;

	if( GetSystem() == NULL )
		return;

	GetSystem()->EnsureStaticDataLoaded();
	m_nMainTab = nMainTab;
	_InvalidateEncyListCache();
	_InvalidateDeckListCache();

	for( int i = 0 ; i < 2 ; ++i )
	{
		if( m_pMainTabBtn[ i ] )
			m_pMainTabBtn[ i ]->SetMouseOnMode( i == m_nMainTab );
	}

	bool bShowEncySubMenus = ( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA );
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( m_pSubMenuBtn[ i ] )
			m_pSubMenuBtn[ i ]->SetVisible( bShowEncySubMenus );
		if( m_pSubMenuText[ i ] )
			m_pSubMenuText[ i ]->SetVisible( bShowEncySubMenus );
	}

	if( m_pStatisticsBtn )
		m_pStatisticsBtn->SetVisible( true );
	if( m_pSubMenuText[ SUBMENU_STATISTICS ] )
		m_pSubMenuText[ SUBMENU_STATISTICS ]->SetVisible( true );

	_RefreshSearchUI();
	_SetTabList( 0 );
}

void cEncyclopedia::_InvalidateEncyListCache()
{
	m_bEncyListBuilt = false;
	m_wsEncyListCacheSearch.clear();
	m_nEncyListCacheSubMenu = -1;
	m_bEncyLazyAppendActive = false;
	m_nEncyLazySourceIndex = 0;
	m_nEncyLazyFlatRowIndex = 0;
}

void cEncyclopedia::_InvalidateDeckListCache()
{
	m_bDeckListBuilt = false;
	m_wsDeckListCacheSearch.clear();
	m_bDeckLazyAppendActive = false;
	m_nDeckLazyGroupCursor = 0;
	m_vDeckLazyGroups.clear();
}

bool cEncyclopedia::_EnsureInitialListBuilt()
{
	if( GetSystem() == NULL )
		return false;

	GetSystem()->EnsureStaticDataLoaded();
	_UpdateActiveListVisibility();

	cListBox* pActiveListBox = _GetActiveListBox();
	if( pActiveListBox == NULL )
		return false;

	bool const bHasLazyPending =
		( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA && m_bEncyLazyAppendActive ) ||
		( m_nMainTab == MAIN_TAB_DECK && m_bDeckLazyAppendActive );
	bool const bActiveListBuilt =
		( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA ) ? m_bEncyListBuilt : m_bDeckListBuilt;
	bool const bNeedsBuild =
		( pActiveListBox->GetItemCount() == 0 ) ||
		( bHasLazyPending == false && bActiveListBuilt == false );

	if( bNeedsBuild == false )
		return false;

	_SetTabList( 0 );
	return true;
}

bool cEncyclopedia::_ShouldAppendLazyRows() const
{
	bool const bHasPending =
		( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA && m_bEncyLazyAppendActive ) ||
		( m_nMainTab == MAIN_TAB_DECK && m_bDeckLazyAppendActive );
	if( !bHasPending )
		return false;

	cListBox* pActiveListBox = _GetActiveListBox();
	if( pActiveListBox == NULL )
		return false;

	cScrollBar* pActiveScroll = _GetActiveScrollBar();
	if( pActiveScroll == NULL )
		return true;

	if( pActiveListBox->GetItemCount() == 0 )
		return true;

	if( pActiveScroll->IsEnableScroll() == false )
		return true;

	int nRenderCount = pActiveScroll->GetRenderCount();
	if( nRenderCount < 1 )
		nRenderCount = 1;

	int const nPreloadDistance = nRenderCount * 2;
	int const nRemainingRows = pActiveScroll->GetMaxPosIndex() - pActiveScroll->GetCurPosIndex();
	return nRemainingRows <= nPreloadDistance;
}

void cEncyclopedia::_UpdateLazyListBuild()
{
	if( _ShouldAppendLazyRows() == false )
		return;

	_SetTabList( 0 );
}

void cEncyclopedia::_SetCardHoverVisible( bool bVisible )
{
	m_bCardHoverEffectVisible = bVisible;
	if( m_pCardHoverEffect )
		m_pCardHoverEffect->SetVisible( bVisible );
	if( bVisible == false )
		m_bCardNameTooltipVisible = false;
}

void cEncyclopedia::_RenderCardNameTooltip()
{
	if( m_bCardNameTooltipVisible == false )
		return;

	CsPoint const ptPos = m_ptCardNameTooltipPos;
	CsPoint const ptSize = m_ptCardNameTooltipSize;
	if( ptSize.x <= 0 || ptSize.y <= 0 )
		return;

	if( m_pCardNameTooltipBg )
		m_pCardNameTooltipBg->Render( ptPos, ptSize );
	if( m_pCardNameTooltipTop )
		m_pCardNameTooltipTop->Render( ptPos, CsPoint( ptSize.x, 1 ) );
	if( m_pCardNameTooltipBottom )
		m_pCardNameTooltipBottom->Render( CsPoint( ptPos.x, ptPos.y + ptSize.y - 1 ), CsPoint( ptSize.x, 1 ) );
	if( m_pCardNameTooltipLeft )
		m_pCardNameTooltipLeft->Render( ptPos, CsPoint( 1, ptSize.y ) );
	if( m_pCardNameTooltipRight )
		m_pCardNameTooltipRight->Render( CsPoint( ptPos.x + ptSize.x - 1, ptPos.y ), CsPoint( 1, ptSize.y ) );
	if( m_pCardNameTooltipText )
		m_pCardNameTooltipText->Render( CsPoint( ptPos.x + ( ptSize.x / 2 ), ptPos.y + 5 ), DT_CENTER );
}

void cEncyclopedia::_UpdateCardHover()
{
	_SetCardHoverVisible( false );

	if( m_nMainTab != MAIN_TAB_ENCYCLOPEDIA || !m_pEncyListBox )
		return;

	cListBoxItem const* pConstOverItem = m_pEncyListBox->GetMouseOverItem();
	if( !pConstOverItem || !pConstOverItem->GetScrollVisible() )
		return;

	cListBoxItem* pOverItem = const_cast< cListBoxItem* >( pConstOverItem );
	CsPoint ptLocalMouse = CsPoint::ZERO;
	__try
	{
		ptLocalMouse = m_pEncyListBox->GetItemtoMousePos( CURSOR_ST.GetPos(), pOverItem );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
		CLIENT_LOG_WARN( "ENCY", "_UpdateCardHover failed while resolving local mouse position." );
		return;
	}

	for( size_t i = 0; i < m_vCardHoverInfo.size(); ++i )
	{
		if( m_vCardHoverInfo[ i ].s_pItem != pOverItem )
			continue;

		if( m_vCardHoverInfo[ i ].s_LocalRect.PtInRect( ptLocalMouse ) == FALSE )
			continue;

		CsRect const& rtHover = m_vCardHoverInfo[ i ].s_LocalRect;
		CsPoint ptItemScreen = CURSOR_ST.GetPos() - ptLocalMouse;
		m_ptCardHoverEffectPos = ptItemScreen + rtHover.GetPos();
		m_ptCardHoverEffectSize = rtHover.GetSize();
		m_bCardHoverEffectVisible = true;
		if( m_pCardHoverEffect )
		{
			m_pCardHoverEffect->SetSize( m_ptCardHoverEffectSize );
			m_pCardHoverEffect->SetVisible( true );
		}
		int const nTooltipDigimonId = ( m_vCardHoverInfo[ i ].s_nTooltipDigimonID > 0 ) ? m_vCardHoverInfo[ i ].s_nTooltipDigimonID : m_vCardHoverInfo[ i ].s_nDigimonID;
		_SetCardNameTooltip( nTooltipDigimonId, CsRect( m_ptCardHoverEffectPos, m_ptCardHoverEffectSize ) );
		break;
	}
}

int cEncyclopedia::_GetDeckHoverGroupIdx( cListBoxItem* pItem, CsPoint const& ptLocalMouse ) const
{
	for( size_t i = 0; i < m_vDeckPanelHoverInfo.size(); ++i )
	{
		if( m_vDeckPanelHoverInfo[ i ].s_pItem != pItem )
			continue;

		CsRect rtHover = m_vDeckPanelHoverInfo[ i ].s_LocalRect;
		CsPoint ptHoverMouse = ptLocalMouse;
		if( rtHover.PtInRect( ptHoverMouse ) )
			return m_vDeckPanelHoverInfo[ i ].s_nGroupIdx;
	}
	return -1;
}

void cEncyclopedia::_OnClickEncyclopediaItem( void* pSender, void* pData )
{
	UNREFERENCED_PARAMETER( pSender );
	SAFE_POINTER_RET( pData );

	if( m_nMainTab != MAIN_TAB_ENCYCLOPEDIA )
	{
		_CloseOverview();
		return;
	}

	cListBoxItem* pItem = static_cast< cListBoxItem* >( pData );
	SAFE_POINTER_RET( pItem );

	CsPoint ptLocalMouse = m_pEncyListBox->GetItemtoMousePos( CURSOR_ST.GetPos(), pItem );
	for( size_t i = 0; i < m_vCardHoverInfo.size(); ++i )
	{
		if( m_vCardHoverInfo[ i ].s_pItem != pItem )
			continue;
		if( m_vCardHoverInfo[ i ].s_LocalRect.PtInRect( ptLocalMouse ) == FALSE )
			continue;

		if( m_vCardHoverInfo[ i ].s_nDigimonID <= 0 || m_vCardHoverInfo[ i ].s_nTooltipDigimonID <= 0 )
		{
			_CloseOverview();
			return;
		}

		_OpenOverview( m_vCardHoverInfo[ i ].s_nDigimonID, m_vCardHoverInfo[ i ].s_nTooltipDigimonID );
		return;
	}
}

void cEncyclopedia::_ResolveOverviewGrowthData( int nDigimonId, int& nLevel, int& nScale, int& nCloneLevel,
												int& nCloneAT, int& nCloneCT, int& nCloneBL, int& nCloneEV, int& nCloneHP ) const
{
	nLevel = 0;
	nScale = 0;
	nCloneLevel = 0;
	nCloneAT = 0;
	nCloneCT = 0;
	nCloneBL = 0;
	nCloneEV = 0;
	nCloneHP = 0;

	EncyclopediaContents::MAP_CIT itCur = m_pCurTabList_map.begin();
	EncyclopediaContents::MAP_CIT itCurEnd = m_pCurTabList_map.end();
	for( ; itCur != itCurEnd; ++itCur )
	{
		EncyclopediaContents::sEVOL_INFO* pInfo = itCur->second;
		if( pInfo == NULL )
			continue;

		for( int i = 0; i < nLimit::EvoUnit; ++i )
		{
			if( pInfo->s_sInfo[ i ].s_nDigimonID != nDigimonId )
				continue;

			nLevel = pInfo->s_nLevel;
			nScale = pInfo->s_nSize;
			nCloneLevel = pInfo->s_nEnchant;
			nCloneAT = pInfo->s_nEnchant_AT;
			nCloneCT = pInfo->s_nEnchant_CT;
			nCloneBL = pInfo->s_nEnchant_BL;
			nCloneEV = pInfo->s_nEnchant_EV;
			nCloneHP = pInfo->s_nEnchant_HP;
			return;
		}
	}
}

void cEncyclopedia::_EnsureOverviewRenderResources()
{
	if( m_pOverviewRenderTex == NULL )
	{
		m_pOverviewRenderTex = NiNew cRenderTex;
		if( m_pOverviewRenderTex && m_pOverviewRenderTex->Init( CsPoint( 410, 520 ), "Encyclopedia\\newencyclopedia\\detail\\blind_bg_noalpha.png" ) == false )
		{
			SAFE_NIDELETE( m_pOverviewRenderTex );
		}
	}

	if( ENCY_OVERVIEW_USE_FIGURE_BASE && m_pOverviewFigureBase == NULL )
	{
		NiStream kStream;
		if( kStream.Load( ENCY_OVERVIEW_FIGURE_BASE ) )
		{
			NiNodePtr pNode = (NiNode*)kStream.GetObjectAt( 0 );
			if( pNode )
			{
				nsCSGBFUNC::InitAnimation( pNode, NiTimeController::APP_TIME, NiTimeController::LOOP );
				m_pOverviewFigureBase = NiNew CsNodeObj;
				m_pOverviewFigureBase->SetNiObject( pNode, CGeometry::Normal );
				m_pOverviewFigureBase->m_pNiNode->DetachAllEffects();
				if( nsCsGBTerrain::g_pCurRoot && nsCsGBTerrain::g_pCurRoot->GetLightMng() )
					nsCsGBTerrain::g_pCurRoot->GetLightMng()->ApplyChar( m_pOverviewFigureBase->m_pNiNode );
				m_pOverviewFigureBase->m_pNiNode->UpdateEffects();
				m_pOverviewFigureBase->m_pNiNode->SetTranslate( NiPoint3( -20.0f, 0.0f, _GetOverviewFigureBaseZ( NULL ) ) );
				m_pOverviewFigureBase->m_pNiNode->SetRotate( -0.1f, 0, 0, 1 );
				m_pOverviewFigureBase->m_pNiNode->SetScale( _GetOverviewFigureBaseScale( NULL ) );
				m_pOverviewFigureBase->m_pNiNode->Update( 0.0f );
			}
		}
	}
}

void cEncyclopedia::_OpenOverview( int nDigimonId, int nTooltipDigimonId )
{
	SAFE_POINTER_RET( nsCsFileTable::g_pDigimonMng );
	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( nDigimonId );
	SAFE_POINTER_RET( pDigimon );
	CsDigimon::sINFO* pInfo = pDigimon->GetInfo();
	SAFE_POINTER_RET( pInfo );

	_ReleaseOverviewRenderDigimon();

	m_nOverviewDigimonId = nDigimonId;
	m_nOverviewTooltipDigimonId = nTooltipDigimonId > 0 ? nTooltipDigimonId : nDigimonId;
	m_nOverviewHoverSkillIdx = -1;
	m_bOverviewVisible = true;
	m_fOverviewZoomRate = 0.5f;
	m_fOverviewZoomTargetRate = 0.5f;
	m_fOverviewRenderYaw = 0.0f;
	m_fOverviewRenderPitch = 0.0f;
	m_fOverviewPreviewAniTimer = 0.0f;
	m_bOverviewPreviewAltAni = false;
	m_bOverviewPreviewAniStarted = false;

	std::wstring wsName = pInfo->s_szName;
	if( m_pOverviewName )
		m_pOverviewName->SetText( wsName.c_str() );

	int nLevel = 0;
	int nScale = 0;
	int nCloneLevel = 0;
	int nCloneAT = 0;
	int nCloneCT = 0;
	int nCloneBL = 0;
	int nCloneEV = 0;
	int nCloneHP = 0;
	_ResolveOverviewGrowthData( nDigimonId, nLevel, nScale, nCloneLevel, nCloneAT, nCloneCT, nCloneBL, nCloneEV, nCloneHP );
	UNREFERENCED_PARAMETER( nCloneLevel );

	TCHAR szStat[ 64 ] = { 0, };
	int nStatValues[ 7 ] = { nLevel, nCloneAT, nCloneHP, nCloneCT, nScale, nCloneBL, nCloneEV };
	const TCHAR* szStatLabels[ 7 ] = { _T( "Lv" ), _T( "AT" ), _T( "Hp" ), _T( "CT" ), _T( "Scale" ), _T( "BL" ), _T( "EV" ) };
	for( int i = 0; i < 7; ++i )
	{
		if( m_pOverviewStatText[ i ] == NULL )
			continue;
		_stprintf_s( szStat, _T( "%s     %d" ), szStatLabels[ i ], nStatValues[ i ] );
		m_pOverviewStatText[ i ]->SetText( szStat );
	}
	if( m_pOverviewStatText[ 7 ] )
		m_pOverviewStatText[ 7 ]->SetText( _T( "" ) );

	if( m_pOverviewGrowth )
	{
		m_pOverviewGrowth->Delete();
	}

	if( m_pOverviewRankValue )
		m_pOverviewRankValue->SetText( _GetEncyRankText( pInfo->s_nDigimonRank ) );
	if( m_pOverviewStageValue )
		m_pOverviewStageValue->SetText( _GetEncyEvolutionStageText( (int)pInfo->s_eEvolutionType ) );

	m_nOverviewSelectedSkillIdx = 0;
	_UpdateOverviewSkillSection();

	_EnsureOverviewRenderResources();

	if( m_pOverviewRenderTex )
	{
		SAFE_POINTER_RET( g_pModelDataMng );
		CsModelData* pModelData = g_pModelDataMng->GetData( pInfo->s_dwModelID );
		SAFE_POINTER_RET( pModelData );
		CsModelData::sINFO* pModelInfo = pModelData->GetInfo();
		SAFE_POINTER_RET( pModelInfo );

		sCREATEINFO prop;
		prop.s_dwPropPlag = PR_ANI_ENABLE | PR_EFFECT_ENABLE;
		prop.s_dwOptionPlag = OP_LIGHT;
		prop.s_eInsertGeomType = CGeometry::GetCharOutLineType();

		m_pOverviewRenderDigimon = g_pCharMng->AddTempDigimon( &prop, pModelInfo->s_dwID, NULL, NiPoint3::ZERO, 0.0f, NULL );
		if( m_pOverviewRenderDigimon )
		{
			float fZoomRateForDist = m_fOverviewZoomTargetRate;
			if( fZoomRateForDist < 0.0f ) fZoomRateForDist = 0.0f;
			if( fZoomRateForDist > 1.0f ) fZoomRateForDist = 1.0f;
			const float fDistRange = ( m_fOverviewZoomDistMax - m_fOverviewZoomDistMin );
			m_pOverviewRenderTex->SetDistConstant( m_fOverviewZoomDistMax - ( fDistRange * fZoomRateForDist ) );
			m_pOverviewRenderDigimon->SetScale( 1.0f );
			m_pOverviewRenderDigimon->GetProp_Alpha()->SetUseDistAlpha( false );
			if( m_pOverviewRenderDigimon->GetProp_Animation() )
				m_pOverviewRenderDigimon->GetProp_Animation()->SetUseIdleAni( false );
			m_pOverviewRenderDigimon->SetViewSight( true );
		}
	}
}

void cEncyclopedia::_UpdateOverviewSkillSection()
{
	if( m_pOverviewSkills )
		m_pOverviewSkills->Delete();
	if( m_pOverviewSkillStatsLeft )
		m_pOverviewSkillStatsLeft->Delete();
	if( m_pOverviewSkillStatsRight )
		m_pOverviewSkillStatsRight->Delete();
	if( m_pOverviewSkillDesc )
		m_pOverviewSkillDesc->Delete();
	if( m_pOverviewSkillName )
		m_pOverviewSkillName->SetText( _T( "" ) );

	SAFE_POINTER_RET( nsCsFileTable::g_pDigimonMng );
	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( m_nOverviewDigimonId );
	SAFE_POINTER_RET( pDigimon );
	CsDigimon::sINFO* pInfo = pDigimon->GetInfo();
	SAFE_POINTER_RET( pInfo );
	SAFE_POINTER_RET( nsCsFileTable::g_pSkillMng );

	int nValidSkillIndices[ DIGIMON_MAX_SKILL_COUNT ] = { 0, };
	int nValidSkillCount = 0;
	for( int i = 0; i < DIGIMON_MAX_SKILL_COUNT; ++i )
	{
		DWORD dwSkillId = pInfo->s_Skill[ i ].s_dwID;
		if( dwSkillId == 0 )
			continue;
		CsSkill* pSkill = nsCsFileTable::g_pSkillMng->GetSkill( dwSkillId );
		if( pSkill == NULL || pSkill->GetInfo() == NULL )
			continue;
		nValidSkillIndices[ nValidSkillCount++ ] = i;
	}

	if( nValidSkillCount <= 0 )
	{
		m_nOverviewSelectedSkillIdx = -1;
		if( m_pOverviewSkills )
		{
			cText::sTEXTINFO tiSkill;
			tiSkill.Init( &g_pEngine->m_FontSystem, CFont::FS_11 );
			tiSkill.s_Color = NiColor( 0.62f, 0.68f, 1.0f );
			if( m_pOverviewSkillName )
				m_pOverviewSkillName->SetText( _T( "No skill data" ) );
			g_pStringAnalysis->Cut( m_pOverviewSkills, 320, _T( "No skill data" ), &tiSkill );
		}
		return;
	}

	bool bSelectedFound = false;
	for( int i = 0; i < nValidSkillCount; ++i )
	{
		if( nValidSkillIndices[ i ] == m_nOverviewSelectedSkillIdx )
		{
			bSelectedFound = true;
			break;
		}
	}
	if( !bSelectedFound )
		m_nOverviewSelectedSkillIdx = nValidSkillIndices[ 0 ];

	DWORD dwSkillId = pInfo->s_Skill[ m_nOverviewSelectedSkillIdx ].s_dwID;
	CsSkill* pSkill = nsCsFileTable::g_pSkillMng->GetSkill( dwSkillId );
	SAFE_POINTER_RET( pSkill );
	CsSkill::sINFO* pSkillInfo = pSkill->GetInfo();
	SAFE_POINTER_RET( pSkillInfo );

	if( m_pOverviewSkillName )
		m_pOverviewSkillName->SetText( pSkillInfo->s_szName );
	if( m_pOverviewSkillDesc )
	{
		cText::sTEXTINFO tiSkillDesc;
		tiSkillDesc.Init( &g_pEngine->m_FontSystem, CFont::FS_9 );
		tiSkillDesc.s_Color = NiColor( 0.86f, 0.88f, 1.0f );
		const int nMaxSkillDescChars = 60;
		const TCHAR* pszSkillDesc = pSkillInfo->s_szComment;
		TCHAR szSkillDescLimited[ 128 ] = { 0, };
		if( pszSkillDesc && _tcslen( pszSkillDesc ) > nMaxSkillDescChars )
		{
			_tcsncpy_s( szSkillDescLimited, pszSkillDesc, nMaxSkillDescChars );
			_tcscat_s( szSkillDescLimited, _T( "..." ) );
			pszSkillDesc = szSkillDescLimited;
		}
		g_pStringAnalysis->Cut_Parcing( m_pOverviewSkillDesc, 390, pszSkillDesc, &tiSkillDesc );
	}

	cText::sTEXTINFO tiSkill;
	tiSkill.Init( &g_pEngine->m_FontSystem, CFont::FS_11 );
	tiSkill.s_Color = NiColor( 0.62f, 0.68f, 1.0f );
	int nSkillDamage = 0;
	if( pSkillInfo->s_Apply[ 0 ].s_nID != 0 )
		nSkillDamage = FMCommon::GetSkillAtt( dwSkillId, pSkillInfo->s_nMaxLevel, 0 );
	float fCooldownSecond = pSkillInfo->s_fCooldownTime * 0.001f;

	cText::sTEXTINFO tiSkillLeft = tiSkill;
	tiSkillLeft.s_eTextAlign = DT_LEFT;
	cText::sTEXTINFO tiSkillRight = tiSkill;
	tiSkillRight.s_eTextAlign = DT_RIGHT;
	TCHAR szRight[ 128 ] = { 0, };

	g_pStringAnalysis->Cut( m_pOverviewSkillStatsLeft, 160, _T( "Attribute" ), &tiSkillLeft );
	m_pOverviewSkillStatsLeft->AddTailEmpty( CFont::FS_8 );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsLeft, 160, _T( "Max Level" ), &tiSkillLeft );
	m_pOverviewSkillStatsLeft->AddTailEmpty( CFont::FS_8 );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsLeft, 160, _T( "Attack" ), &tiSkillLeft );
	m_pOverviewSkillStatsLeft->AddTailEmpty( CFont::FS_8 );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsLeft, 160, _T( "Cool Down" ), &tiSkillLeft );

	_stprintf_s( szRight, _T( "%s" ), _GetEncyNatureText( (int)pSkillInfo->s_nNatureType ) );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsRight, 130, szRight, &tiSkillRight );
	m_pOverviewSkillStatsRight->AddTailEmpty( CFont::FS_8 );
	_stprintf_s( szRight, _T( "Lv.%d" ), (int)pSkillInfo->s_nMaxLevel );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsRight, 130, szRight, &tiSkillRight );
	m_pOverviewSkillStatsRight->AddTailEmpty( CFont::FS_8 );
	_stprintf_s( szRight, _T( "%d" ), nSkillDamage );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsRight, 130, szRight, &tiSkillRight );
	m_pOverviewSkillStatsRight->AddTailEmpty( CFont::FS_8 );
	_stprintf_s( szRight, _T( "%.1fSecond" ), fCooldownSecond );
	g_pStringAnalysis->Cut( m_pOverviewSkillStatsRight, 130, szRight, &tiSkillRight );
}

void cEncyclopedia::_CloseOverview()
{
	_ReleaseOverviewRenderDigimon();
	m_bOverviewVisible = false;
	m_nOverviewDigimonId = 0;
	m_nOverviewTooltipDigimonId = 0;
	m_nOverviewSelectedSkillIdx = -1;
	m_nOverviewHoverSkillIdx = -1;
	m_fOverviewZoomRate = 0.5f;
	m_fOverviewZoomTargetRate = 0.5f;
	m_fOverviewRenderYaw = 0.0f;
	m_fOverviewRenderPitch = 0.0f;
	m_bOverviewModelDragRotate = false;
	m_bOverviewZoomSliderDrag = false;
	m_nOverviewZoomSliderDragOffsetX = 0;
	m_fOverviewPreviewAniTimer = 0.0f;
	m_bOverviewPreviewAltAni = false;
	m_bOverviewPreviewAniStarted = false;
}

void cEncyclopedia::_ReleaseOverviewRenderDigimon()
{
	if( m_pOverviewRenderDigimon && g_pCharMng )
		g_pCharMng->DeleteTempChar( m_pOverviewRenderDigimon );
	m_pOverviewRenderDigimon = NULL;
}

void cEncyclopedia::_UpdateOverviewRenderDigimon()
{
	if( !m_bOverviewVisible || m_pOverviewRenderDigimon == NULL )
		return;

	// Smooth zoom interpolation toward target value for fluid slider transitions.
	if( m_fOverviewZoomRate < m_fOverviewZoomTargetRate )
	{
		m_fOverviewZoomRate += g_fDeltaTime * 3.8f;
		if( m_fOverviewZoomRate > m_fOverviewZoomTargetRate )
			m_fOverviewZoomRate = m_fOverviewZoomTargetRate;
	}
	else if( m_fOverviewZoomRate > m_fOverviewZoomTargetRate )
	{
		m_fOverviewZoomRate -= g_fDeltaTime * 3.8f;
		if( m_fOverviewZoomRate < m_fOverviewZoomTargetRate )
			m_fOverviewZoomRate = m_fOverviewZoomTargetRate;
	}

	float fZoomRateForDist = m_fOverviewZoomRate;
	if( fZoomRateForDist < 0.0f ) fZoomRateForDist = 0.0f;
	if( fZoomRateForDist > 1.0f ) fZoomRateForDist = 1.0f;
	if( m_pOverviewRenderTex )
	{
		const float fDistRange = ( m_fOverviewZoomDistMax - m_fOverviewZoomDistMin );
		m_pOverviewRenderTex->SetDistConstant( m_fOverviewZoomDistMax - ( fDistRange * fZoomRateForDist ) );
	}

	if( m_pOverviewRenderDigimon->IsLoad() )
	{
		CsNodeObj* pCsNode = m_pOverviewRenderDigimon->GetCsNode();
		if( pCsNode && pCsNode->m_pNiNode )
		{
			CsC_AniProp* pAniProp = m_pOverviewRenderDigimon->GetProp_Animation();
			if( pAniProp )
			{
				if( m_bOverviewPreviewAniStarted == false )
				{
					pAniProp->SetUseIdleAni( false );
					pAniProp->SetAnimation( ANI::IDLE_NORMAL, false, 0.85f );
					m_fOverviewPreviewAniTimer = 3.4f;
					m_bOverviewPreviewAltAni = false;
					m_bOverviewPreviewAniStarted = true;
				}
				else
				{
					m_fOverviewPreviewAniTimer -= g_fDeltaTime;
					if( m_fOverviewPreviewAniTimer <= 0.0f )
					{
						if( m_bOverviewPreviewAltAni )
						{
							pAniProp->SetAnimation( ANI::IDLE_NORMAL, false, 0.85f );
							m_fOverviewPreviewAniTimer = 4.0f;
							m_bOverviewPreviewAltAni = false;
						}
						else
						{
							DWORD const dwPrevAni = pAniProp->GetAnimationID();
							pAniProp->SetAnimation( ANI::IDLE_CHAR, false, 0.85f );
							if( pAniProp->GetAnimationID() == dwPrevAni )
								pAniProp->SetAnimation( ANI::IDLE_SHAKE, false, 0.85f );
							m_fOverviewPreviewAniTimer = 2.3f;
							m_bOverviewPreviewAltAni = true;
						}
					}
				}
				pAniProp->Update( g_fDeltaTime );
			}
			m_pOverviewRenderDigimon->GetProp_Alpha()->Update( g_fDeltaTime );
			float fAniTime = m_pOverviewRenderDigimon->GetAniTime();
			pCsNode->m_pNiNode->Update( fAniTime );
			m_fOverviewRenderPitch = 0.0f;
			NiMatrix3 matZ;
			matZ.MakeZRotation( m_fOverviewRenderYaw );
			pCsNode->m_pNiNode->SetRotate( matZ );
			if( ENCY_OVERVIEW_USE_FIGURE_BASE && m_pOverviewFigureBase && m_pOverviewFigureBase->m_pNiNode )
			{
				m_pOverviewFigureBase->m_pNiNode->SetTranslate( NiPoint3( -20.0f, 0.0f, _GetOverviewFigureBaseZ( m_pOverviewRenderDigimon ) ) );
				m_pOverviewFigureBase->m_pNiNode->SetRotate( -0.1f, 0, 0, 1 );
				m_pOverviewFigureBase->m_pNiNode->SetScale( _GetOverviewFigureBaseScale( m_pOverviewRenderDigimon ) );
				m_pOverviewFigureBase->m_pNiNode->Update( fAniTime );
			}
			m_pOverviewRenderDigimon->SetViewSight( true );
		}
	}
	else
	{
		m_pOverviewRenderDigimon->CsC_AvObject::Update( g_fDeltaTime );
		m_pOverviewRenderDigimon->SetViewSight( true );
	}
}

void cEncyclopedia::_RenderOverview()
{
	if( !m_bOverviewVisible )
		return;

	const CsPoint ptPanel( 0, 118 );
	const int nLayoutYOffset = 40;
	const int nOverviewTopCenterX = 655;
	const int nGrowthSectionShiftY = 14;
	const int nInfoPanelShiftY = 16;
	const CsPoint ptModel( 38, 100 + nLayoutYOffset );
	const CsPoint ptName( nOverviewTopCenterX, 36 + nLayoutYOffset );
	const CsPoint ptGrowth( 520, 162 + nLayoutYOffset );
	const CsPoint ptSkillName( 670, 482 + nLayoutYOffset );
	const CsPoint ptSkillDesc( 480, 510 + nLayoutYOffset );
	const int nOverviewLowerShiftY = -6;

	if( m_pOverviewBg )
		m_pOverviewBg->Render( GetRootClient() + ptPanel );

	CsPoint ptRoot = GetRootClient() + ptPanel;
	if( m_pOverviewBackBtn )
		m_pOverviewBackBtn->Render( ptRoot + CsPoint( 18, 14 ) );
	if( m_pOverviewBackText )
		m_pOverviewBackText->Render( ptRoot + CsPoint( 54, 23 ), DT_LEFT );
	if( m_pOverviewCloseBtn )
		m_pOverviewCloseBtn->Render( ptRoot + CsPoint( 856, 6 ) );
	if( m_pOverviewInfoTextBg )
		m_pOverviewInfoTextBg->Render( ptRoot + CsPoint( 463, 184 + nLayoutYOffset + nInfoPanelShiftY ) );
	if( m_pOverviewRatingFrame )
		m_pOverviewRatingFrame->Render( ptRoot + CsPoint( 495, 244 + nLayoutYOffset + nInfoPanelShiftY ) );
	if( m_pOverviewArrowLBtn )
		m_pOverviewArrowLBtn->Render( ptRoot + CsPoint( 460, 71 ) );
	if( m_pOverviewArrowRBtn )
		m_pOverviewArrowRBtn->Render( ptRoot + CsPoint( 836, 71 ) );
	if( m_pOverviewStatusArrow && m_pOverviewArrowLBtn == NULL && m_pOverviewArrowRBtn == NULL )
	{
		m_pOverviewStatusArrow->Render( ptRoot + CsPoint( 490, 37 ) );
		m_pOverviewStatusArrow->Render( ptRoot + CsPoint( 842, 37 ) );
	}
	if( m_pOverviewGrowthTitle )
		m_pOverviewGrowthTitle->Render( ptRoot + CsPoint( nOverviewTopCenterX, 75 + nLayoutYOffset ), DT_CENTER );
	if( m_pOverviewInfoTitle )
		m_pOverviewInfoTitle->Render( ptRoot + CsPoint( 655, 217 + nLayoutYOffset + nInfoPanelShiftY ), DT_CENTER );
	if( m_pOverviewTooltipLine )
	{
		// Keep exact user-tuned coordinates.
		m_pOverviewTooltipLine->Render( ptRoot + CsPoint( 465, 395 + nLayoutYOffset + nInfoPanelShiftY ) );
		m_pOverviewTooltipLine->Render( ptRoot + CsPoint( 578, 395 + nLayoutYOffset + nInfoPanelShiftY ) );
		m_pOverviewTooltipLine->Render( ptRoot + CsPoint( 692, 395 + nLayoutYOffset + nInfoPanelShiftY ) );
	}
	if( m_pOverviewBox )
	{
		const CsPoint ptBoxRow1[ 4 ] = { CsPoint( 462, 92 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 565, 92 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 668, 92 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 771, 92 + nLayoutYOffset + nGrowthSectionShiftY ) };
		for( int i = 0; i < 4; ++i )
			m_pOverviewBox->Render( ptRoot + ptBoxRow1[ i ] );
		const CsPoint ptBoxRow2[ 3 ] = { CsPoint( 515, 142 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 618, 142 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 721, 142 + nLayoutYOffset + nGrowthSectionShiftY ) };
		for( int i = 0; i < 3; ++i )
			m_pOverviewBox->Render( ptRoot + ptBoxRow2[ i ] );
	}
	const CsPoint ptStatPos[ 7 ] = {
		CsPoint( 475, 101 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 578, 101 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 681, 101 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 784, 101 + nLayoutYOffset + nGrowthSectionShiftY ),
		CsPoint( 528, 151 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 631, 151 + nLayoutYOffset + nGrowthSectionShiftY ), CsPoint( 734, 151 + nLayoutYOffset + nGrowthSectionShiftY )
	};
	for( int i = 0; i < 7; ++i )
	{
		if( m_pOverviewStatText[ i ] )
			m_pOverviewStatText[ i ]->Render( ptRoot + ptStatPos[ i ], DT_LEFT );
	}

	if( m_pOverviewRenderTex )
	{
		bool const bRenderFigureBase = ( ENCY_OVERVIEW_USE_FIGURE_BASE && m_pOverviewRenderDigimon && m_pOverviewRenderDigimon->GetCsNode() != NULL );
		m_pOverviewRenderTex->BeginRender();
		m_pOverviewRenderTex->RenderObject( GET_SUBCAMERA(CAMERA_06), m_pOverviewRenderDigimon, true );
		if( bRenderFigureBase && m_pOverviewFigureBase && m_pOverviewFigureBase->m_pNiNode )
			m_pOverviewFigureBase->RenderAbsolute();
		m_pOverviewRenderTex->EndRender( GET_SUBCAMERA(CAMERA_06), ptRoot + ptModel );
	}
	const int nControlYRotate = 585 + nLayoutYOffset;
	const int nControlYZoom = 588 + nLayoutYOffset;
	const int nControlYSlider = 597 + nLayoutYOffset;
	const int nControlRotateLX = 70;
	const int nControlZoomOutX = 120;
	const int nControlZoomInX = 320;
	const int nControlRotateRX = 370;
	const int nSliderMinX = 165;
	const int nSliderMaxX = 285;
	const int nSliderX = nSliderMinX + (int)( ( nSliderMaxX - nSliderMinX ) * m_fOverviewZoomRate );

	if( m_pOverviewRotateLBtn )
		m_pOverviewRotateLBtn->Render( ptRoot + CsPoint( nControlRotateLX, nControlYRotate ) );
	if( m_pOverviewZoomOutBtn )
		m_pOverviewZoomOutBtn->Render( ptRoot + CsPoint( nControlZoomOutX, nControlYZoom ) );
	if( m_pOverviewScrollBtn )
		m_pOverviewScrollBtn->Render( ptRoot + CsPoint( nSliderX, nControlYSlider ) );
	if( m_pOverviewZoomInBtn )
		m_pOverviewZoomInBtn->Render( ptRoot + CsPoint( nControlZoomInX, nControlYZoom ) );
	if( m_pOverviewRotateRBtn )
		m_pOverviewRotateRBtn->Render( ptRoot + CsPoint( nControlRotateRX, nControlYRotate ) );
	if( m_pOverviewResetViewBtn )
		m_pOverviewResetViewBtn->Render( ptRoot + CsPoint( 405, 538 + nLayoutYOffset ) );

	if( m_pOverviewName )
		m_pOverviewName->Render( ptRoot + ptName, DT_CENTER );

	// Render icon-based summary (same icon families used on encyclopedia cards).
	if( nsCsFileTable::g_pDigimonMng && g_pIconMng && m_nOverviewDigimonId > 0 )
	{
		CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( m_nOverviewDigimonId );
		CsDigimon::sINFO* pInfo = pDigimon ? pDigimon->GetInfo() : NULL;
		if( pInfo )
		{
			const CsPoint ptIconSize( 25, 25 );
			CsPoint ptIconPos = ptRoot + CsPoint( 536, 305 + nLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY );

			if( pInfo->s_nDigimonType > 0 )
			{
				if( m_pOverviewAttributeFrame )
					m_pOverviewAttributeFrame->Render( ptIconPos - CsPoint( 8, 8 ) );
				g_pIconMng->RenderIcon( ICONITEM::DGrowType, ptIconPos - CsPoint( 3, 3 ), ptIconSize, pInfo->s_nDigimonType - 1 );
				ptIconPos.x += 48;
			}

			int nAttrIndex = pInfo->s_eAttributeType - nsCsDigimonTable::AT_NONE;
			if( nAttrIndex >= 0 )
			{
				if( m_pOverviewAttributeFrame )
					m_pOverviewAttributeFrame->Render( ptIconPos - CsPoint( 8, 8 ) );
				g_pIconMng->RenderDAttribute( cIconMng::DAttribute, nAttrIndex, ptIconPos - CsPoint( 3, 3 ), ptIconSize );
				ptIconPos.x += 48;
			}

			for( int i = 0; i < DIGIMON_MAX_FAMILY; ++i )
			{
				if( pInfo->s_eFamilyType[ i ] == 0 )
					continue;
				int nFamilyIndex = pInfo->s_eFamilyType[ i ] - nsCsDigimonTable::FT_DR;
				if( nFamilyIndex < 0 )
					continue;
				if( m_pOverviewAttributeFrame )
					m_pOverviewAttributeFrame->Render( ptIconPos - CsPoint( 8, 8 ) );
				g_pIconMng->RenderDAttribute( cIconMng::Family, nFamilyIndex, ptIconPos - CsPoint( 3, 3 ), ptIconSize );
				ptIconPos.x += 48;
			}

			bool bRenderedNature = false;
			for( int i = 0; i < DIGIMON_MAX_NATURE; ++i )
			{
				if( pInfo->s_eBaseNatureTypes[ i ] == 0 )
					continue;
				int nNatureIndex = pInfo->s_eBaseNatureTypes[ i ] - nsCsDigimonTable::NT_ICE;
				if( nNatureIndex < 0 )
					continue;
				if( m_pOverviewAttributeFrame )
					m_pOverviewAttributeFrame->Render( ptIconPos - CsPoint( 8, 8 ) );
				g_pIconMng->RenderDAttribute( cIconMng::Nature, nNatureIndex, ptIconPos - CsPoint( 3, 3 ), ptIconSize );
				bRenderedNature = true;
				ptIconPos.x += 48;
			}
			// Some records only populate base nature (single) and leave nature array empty.
			if( !bRenderedNature )
			{
				int nNatureIndex = pInfo->s_eBaseNatureType - nsCsDigimonTable::NT_ICE;
				if( nNatureIndex >= 0 )
				{
					if( m_pOverviewAttributeFrame )
						m_pOverviewAttributeFrame->Render( ptIconPos - CsPoint( 8, 8 ) );
					g_pIconMng->RenderDAttribute( cIconMng::Nature, nNatureIndex, ptIconPos - CsPoint( 3, 3 ), ptIconSize );
					ptIconPos.x += 48;
				}
			}

			const CsPoint ptSkillIconSize( 36, 36 );
			CsPoint ptSkillIconPos = ptRoot + CsPoint( 473, 420 + nLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY );
			for( int i = 0; i < DIGIMON_MAX_SKILL_COUNT; ++i )
			{
				if( m_pOverviewSkillFrame )
					m_pOverviewSkillFrame->Render( ptSkillIconPos - CsPoint( 7, 7 ) );

				DWORD dwSkillId = pInfo->s_Skill[ i ].s_dwID;
				if( dwSkillId == 0 || nsCsFileTable::g_pSkillMng == NULL )
				{
					ptSkillIconPos.x += 68;
					continue;
				}
				CsSkill* pSkill = nsCsFileTable::g_pSkillMng->GetSkill( dwSkillId );
				if( pSkill == NULL || pSkill->GetInfo() == NULL )
				{
					ptSkillIconPos.x += 68;
					continue;
				}

				const int nSkillIcon = (int)pSkill->GetInfo()->s_nIcon;
				g_pIconMng->RenderIcon( _GetSkillIconType( nSkillIcon ), ptSkillIconPos, ptSkillIconSize, nSkillIcon );
				const bool bHoverSkill = ( i != m_nOverviewSelectedSkillIdx ) && ( i == m_nOverviewHoverSkillIdx );
				if( ( i == m_nOverviewSelectedSkillIdx || bHoverSkill ) && m_pOverviewSkillSelection )
					m_pOverviewSkillSelection->Render( ptSkillIconPos - CsPoint( 0, 1 ) );
				ptSkillIconPos.x += 68;
			}
		}
	}

	const int nInfoRowY = 255 + nLayoutYOffset + nInfoPanelShiftY;
	if( m_pOverviewRankLabel )
		m_pOverviewRankLabel->Render( ptRoot + CsPoint( 540, nInfoRowY ), DT_LEFT );
	if( m_pOverviewRankValue )
		m_pOverviewRankValue->Render( ptRoot + CsPoint( 618, nInfoRowY ), DT_LEFT );
	if( m_pOverviewStageLabel )
		m_pOverviewStageLabel->Render( ptRoot + CsPoint( 668, nInfoRowY ), DT_LEFT );
	if( m_pOverviewStageValue )
		m_pOverviewStageValue->Render( ptRoot + CsPoint( 750, nInfoRowY ), DT_LEFT );
	if( m_pOverviewStatusArrow2 )
	{
		// Orange chevrons between Rank->value and Stage->value.
		m_pOverviewStatusArrow2->Render( ptRoot + CsPoint( 584, nInfoRowY + 1 ) );
		m_pOverviewStatusArrow2->Render( ptRoot + CsPoint( 718, nInfoRowY + 1 ) );
	}
	// Draw the tab after lines/icons so it always stays on top.
	if( m_pOverviewSkillTabFrame )
		m_pOverviewSkillTabFrame->Render( ptRoot + CsPoint( 613, 356 + nLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY ) );
	if( m_pOverviewSkillTab )
		m_pOverviewSkillTab->Render( ptRoot + CsPoint( 661, 381 + nLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY ), DT_CENTER );
	if( m_pOverviewGrowth )
		m_pOverviewGrowth->Render( ptRoot + ptGrowth, 1 );
	if( m_pOverviewSkillName )
		m_pOverviewSkillName->Render( ptRoot + CsPoint( ptSkillName.x, ptSkillName.y + nOverviewLowerShiftY + nInfoPanelShiftY ), DT_CENTER );
	if( m_pOverviewSkillDesc )
		m_pOverviewSkillDesc->Render( ptRoot + CsPoint( ptSkillDesc.x, ptSkillDesc.y + nOverviewLowerShiftY + nInfoPanelShiftY ), 1 );
	if( m_pOverviewSkillLine )
	{
		const int nSkillLineLeftX = 500;
		const int nSkillLineRightX = 840;
		const int nSkillLineBaseY = 530 + nLayoutYOffset + nOverviewLowerShiftY + nInfoPanelShiftY;
		const int nSkillRowStep = 25;
		for( int i = 0; i < 4; ++i )
			m_pOverviewSkillLine->Render( ptRoot + CsPoint( nSkillLineLeftX, nSkillLineBaseY + ( i * nSkillRowStep ) ) );
		for( int i = 0; i < 4; ++i )
			m_pOverviewSkillLine->Render( ptRoot + CsPoint( nSkillLineRightX, nSkillLineBaseY + ( i * nSkillRowStep ) ) );
		// Anchor stat text to skill line columns, with fixed inner padding.
		if( m_pOverviewSkillStatsLeft )
			m_pOverviewSkillStatsLeft->Render( ptRoot + CsPoint( nSkillLineLeftX + 26, nSkillLineBaseY + 0 ), 0 );
		if( m_pOverviewSkillStatsRight )
			m_pOverviewSkillStatsRight->Render( ptRoot + CsPoint( nSkillLineRightX - 26, nSkillLineBaseY + 0 ), 0 );
	}
}

void cEncyclopedia::SetTabListData()
{
	DWORD const dwBegin = GetTickCount();
	bool const bLazyAppend = m_bEncyLazyAppendActive;
	if( bLazyAppend == false )
	{
		m_vCardHoverInfo.clear();
		m_vCardHoverInfo.reserve( m_pCurTabList_map.size() * nLimit::EvoUnit );
		m_bEncyListBuilt = false;
		m_wsEncyListCacheSearch.clear();
		m_nEncyListCacheSubMenu = -1;
		m_nEncyLazySourceIndex = 0;
		m_nEncyLazyFlatRowIndex = 0;
	}
	if( m_pEncyListBox == NULL )
		return;
	if( bLazyAppend == false )
		m_pEncyListBox->RemoveAllItem();
	std::wstring wsSearch = m_wsSearchKeyword;
	std::transform( wsSearch.begin(), wsSearch.end(), wsSearch.begin(),
		[]( wchar_t ch ) { return (wchar_t)towlower( ch ); } );
	bool const bUseSearch = !wsSearch.empty();
	auto IsDigimonNameMatched = [&]( int nDigimonId )->bool
	{
		if( !bUseSearch )
			return true;
		SAFE_POINTER_RETVAL( nsCsFileTable::g_pDigimonMng, true );
		CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( nDigimonId );
		CsDigimon::sINFO* pInfo = pDigimon ? pDigimon->GetInfo() : NULL;
		if( pInfo == NULL )
			return false;
		std::wstring wsName = pInfo->s_szName;
		std::transform( wsName.begin(), wsName.end(), wsName.begin(),
			[]( wchar_t ch ) { return (wchar_t)towlower( ch ); } );
		return wsName.find( wsSearch ) != std::wstring::npos;
	};
	auto IsSeriesLineMatched = [&]( EncyclopediaContents::sEVOL_INFO* pLine )->bool
	{
		if( pLine == NULL )
			return false;
		if( !bUseSearch )
			return true;
		for( int i = 0; i < nLimit::EvoUnit; ++i )
		{
			if( pLine->s_sInfo[ i ].s_nDigimonID == -1 )
				continue;
			if( pLine->s_sInfo[ i ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
				continue;
			if( IsDigimonNameMatched( pLine->s_sInfo[ i ].s_nDigimonID ) )
				return true;
		}
		return false;
	};

	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA && ( m_nSubMenu == SUBMENU_ALL || m_nSubMenu == SUBMENU_CLASS ) )
	{
		struct sFLAT_CARD
		{
			int			s_nDigimonID;
			int			s_nTooltipDigimonID;
			std::string s_ImgFileName;
			TCHAR		s_szName[ MAX_FILENAME ];
			int			s_nImgState;
			bool		s_bSeriesAllOpen;
			int			s_nRankSort;
		};

		std::vector< sFLAT_CARD > vFlatCards;
		vFlatCards.reserve( m_pCurTabList_map.size() * nLimit::EvoUnit );
		for( EncyclopediaContents::MAP_IT itFlat = m_pCurTabList_map.begin() ; itFlat != m_pCurTabList_map.end() ; ++itFlat )
		{
			EncyclopediaContents::sEVOL_INFO* pLine = itFlat->second;
			if( pLine == NULL )
				continue;

			int nLineTooltipId = 0;
			if( pLine->s_sInfo[ 1 ].s_nDigimonID != -1 && pLine->s_sInfo[ 1 ].s_eImgState != EncyclopediaContents::sINFO::S_NONE )
			{
				nLineTooltipId = pLine->s_sInfo[ 1 ].s_nDigimonID;
			}
			else
			{
				for( int iBase = 0 ; iBase < nLimit::EvoUnit ; ++iBase )
				{
					if( pLine->s_sInfo[ iBase ].s_nDigimonID == -1 || pLine->s_sInfo[ iBase ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
						continue;
					nLineTooltipId = pLine->s_sInfo[ iBase ].s_nDigimonID;
					break;
				}
			}

			for( int i = 0 ; i < nLimit::EvoUnit ; ++i )
			{
				if( pLine->s_sInfo[ i ].s_nDigimonID == -1 || pLine->s_sInfo[ i ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
					continue;
				if( !IsDigimonNameMatched( pLine->s_sInfo[ i ].s_nDigimonID ) )
					continue;

				CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( pLine->s_sInfo[ i ].s_nDigimonID );
				CsDigimon::sINFO* pDigimonInfo = pDigimon ? pDigimon->GetInfo() : NULL;

				int nRankSort = pDigimonInfo ? pDigimonInfo->s_nDigimonRank : 0;
				if( nRankSort < 0 )
					nRankSort = 0; // Keep non-rank/N separate from A.

				sFLAT_CARD addCard;
				addCard.s_nDigimonID = pLine->s_sInfo[ i ].s_nDigimonID;
				addCard.s_nTooltipDigimonID = pLine->s_sInfo[ i ].s_nDigimonID;
				addCard.s_ImgFileName = pLine->s_sInfo[ i ].s_ImgFileName;
				addCard.s_szName[ 0 ] = 0;
				if( pDigimonInfo )
					_tcscpy_s( addCard.s_szName, pDigimonInfo->s_szName );
				addCard.s_nImgState = pLine->s_sInfo[ i ].s_eImgState;
				addCard.s_bSeriesAllOpen = pLine->s_bIsAllOpen;
				addCard.s_nRankSort = nRankSort;
				vFlatCards.push_back( addCard );
			}
		}

		if( m_nSubMenu == SUBMENU_CLASS )
		{
			std::sort( vFlatCards.begin(), vFlatCards.end(),
				[]( sFLAT_CARD const& lhs, sFLAT_CARD const& rhs )
				{
					if( lhs.s_nRankSort == rhs.s_nRankSort )
						return lhs.s_nDigimonID < rhs.s_nDigimonID;
					return lhs.s_nRankSort > rhs.s_nRankSort;
				} );
		}

		const int nCardsPerRow = 6;
		const int nCardWidth = 116;
		const int nCardHeight = 146;
		const int nCardGapX = 12;
		const int nCardStepX = nCardWidth + nCardGapX;
		const int nCardAreaWidth = 760;
		const int nCardAreaStartX = ( 802 - nCardAreaWidth ) / 2;
		const int nCardStartY = 10;
		const int nCardBgWidth = 100;
		const int nCardBgHeight = 128;
		const CsPoint ptCardBgOffset( 8, 11 );
		const CsPoint ptIconSize( 50, 50 );
		const CsPoint ptIconOffset( 33, 27 );
		const CsPoint ptAttrBarSize( 98, 20 );
		const CsPoint ptAttrBarOffset( 9, 80 );
		const CsPoint ptGrowSize( 22, 14 );
		const CsPoint ptDetailIconSize( 16, 16 );
		const CsPoint ptRankSize( 76, 76 );
		const CsPoint ptRankOffset( 20, 87 );

		int nTotalCards = (int)vFlatCards.size();

		auto AddFlatCardRowToList = [&]( int nCardStartIndex, int nCardsInThisRow, int nStartX )->bool
		{
			cString* pItem = NiNew cString;
			if( pItem == NULL )
				return false;
			cListBoxItem* addItem = NiNew cListBoxItem;
			if( addItem == NULL )
			{
				SAFE_DELETE( pItem );
				return false;
			}

			for( int nCol = 0 ; nCol < nCardsInThisRow ; ++nCol )
			{
				int nCardIndex = nCardStartIndex + nCol;
				sFLAT_CARD const& kCard = vFlatCards[ nCardIndex ];
				CsPoint ptCard( nStartX + ( nCol * nCardStepX ), nCardStartY );

				CsDigimon* pFTDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( kCard.s_nDigimonID );
				CsDigimon::sINFO* pFTInfo = pFTDigimon ? pFTDigimon->GetInfo() : NULL;

				const char* pFrameTexture = _GetEncyCardFrameTexture( kCard.s_nRankSort );

				cString::sSPRITE* pCardBGSprite = _AddEncySpriteRect(
					pItem,
					ENCY_UNION_CARD_BG,
					ptCard + ptCardBgOffset,
					CsPoint( nCardBgWidth, nCardBgHeight ),
					CsRect( 0, 0, 102, 128 ) );
				if( pCardBGSprite == NULL )
					return false;

				_AddEncyCircularPortrait(
					pItem,
					kCard.s_ImgFileName,
					ptCard + ptIconOffset,
					ptIconSize.x,
					kCard.s_nImgState == EncyclopediaContents::sINFO::S_CLOSE );

				cString::sIMAGE* pCardFrameSprite = _AddEncyFrameImage( pItem, pFrameTexture, ptCard, CsPoint( nCardWidth, nCardHeight ), true );
				if( pCardFrameSprite == NULL )
					return false;

				_AddEncyCardName( pItem, kCard.s_szName, ptCard, nCardWidth );

				cSprite* pAttributeBar = NiNew cSprite;
				pAttributeBar->Init( NULL, CsPoint::ZERO, ptAttrBarSize, "Encyclopedia\\newencyclopedia\\rate_frame\\attribute_bar.png", false );
				cString::sSPRITE* pAttributeBarSprite = pItem->AddSprite( pAttributeBar, ptCard + ptAttrBarOffset );
				if( pAttributeBarSprite == NULL )
					return false;
				pAttributeBarSprite->SetAutoPointerDelete( true );

				if( pFTInfo )
				{
					int nIconX = 13;
					int nIconY = 83;
					pItem->AddIcon( ptGrowSize, ICONITEM::DGrowType, pFTInfo->s_nDigimonType - 1, 0, ptCard + CsPoint( nIconX, nIconY ) );
					nIconX += ptGrowSize.x;

					int nIndex = pFTInfo->s_eAttributeType - nsCsDigimonTable::AT_NONE;
					pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::DAttribute, nIndex, ptCard + CsPoint( nIconX, 82 ) );
					nIconX += ptDetailIconSize.x;

					int nIconLimitX = 88;
					for( int nFamily = 0 ; nFamily < DIGIMON_MAX_FAMILY && nIconX <= nIconLimitX ; ++nFamily )
					{
						if( pFTInfo->s_eFamilyType[ nFamily ] != 0 )
						{
							nIndex = pFTInfo->s_eFamilyType[ nFamily ] - nsCsDigimonTable::FT_DR;
							pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Family, nIndex, ptCard + CsPoint( nIconX, 82 ) );
							nIconX += ptDetailIconSize.x;
						}
					}

					for( int nNature = 0 ; nNature < 3 && nIconX <= nIconLimitX ; ++nNature )
					{
						if( pFTInfo->s_eBaseNatureTypes[ nNature ] != 0 )
						{
							nIndex = pFTInfo->s_eBaseNatureTypes[ nNature ] - nsCsDigimonTable::NT_ICE;
							pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Nature, nIndex, ptCard + CsPoint( nIconX, 82 ) );
							nIconX += ptDetailIconSize.x;
						}
					}

					cImage* pRank = NiNew cImage;
					pRank->Init( NULL, CsPoint::ZERO, ptRankSize, "Encyclopedia\\newencyclopedia\\icon\\encyclopedia_512.png", false, true );
					pRank->SetTexToken( CsPoint( 102, 102 ) );

					int nRankIndex = _GetEncyRankIconIndex( pFTInfo->s_nDigimonRank );
					cString::sIMAGE* pRankSprite = pItem->AddImage( pRank, nRankIndex, ptCard + ptRankOffset );
					if( pRankSprite == NULL )
						return false;
					pRankSprite->SetAutoPointerDelete( true );
				}

				sCARD_HOVER_INFO kHoverInfo;
				kHoverInfo.s_pItem = addItem;
				kHoverInfo.s_LocalRect = CsRect( ptCard, CsSIZE( nCardWidth, nCardHeight ) );
				kHoverInfo.s_nDigimonID = kCard.s_nDigimonID;
				kHoverInfo.s_nTooltipDigimonID = kCard.s_nTooltipDigimonID;
				m_vCardHoverInfo.push_back( kHoverInfo );
			}

			addItem->Delete();
			addItem->SetUserData( new sDIGIMON_ID( vFlatCards[ nCardStartIndex ].s_nDigimonID ) );
			addItem->SetItem( pItem );
			addItem->SetItemHeight( 170 );
			m_pEncyListBox->AddItemMultiHeight( addItem, 0, false );
			return true;
		};

		struct sFLAT_RENDER_ROW
		{
			sFLAT_RENDER_ROW()
				: s_bHeader( false )
				, s_nRank( 0 )
				, s_nCardStartIndex( 0 )
				, s_nCardCount( 0 )
			{}

			bool	s_bHeader;
			int		s_nRank;
			int		s_nCardStartIndex;
			int		s_nCardCount;
		};

		std::vector< sFLAT_RENDER_ROW > vRenderRows;
		if( m_nSubMenu == SUBMENU_CLASS )
		{
			int nGroupStart = 0;
			while( nGroupStart < nTotalCards )
			{
				int nGroupRank = vFlatCards[ nGroupStart ].s_nRankSort;
				int nGroupEnd = nGroupStart;
				while( nGroupEnd < nTotalCards && vFlatCards[ nGroupEnd ].s_nRankSort == nGroupRank )
					++nGroupEnd;

				sFLAT_RENDER_ROW kHeaderRow;
				kHeaderRow.s_bHeader = true;
				kHeaderRow.s_nRank = nGroupRank;
				kHeaderRow.s_nCardStartIndex = nGroupStart;
				vRenderRows.push_back( kHeaderRow );

				for( int nCardStartIndex = nGroupStart ; nCardStartIndex < nGroupEnd ; nCardStartIndex += nCardsPerRow )
				{
					int nRemainCards = nGroupEnd - nCardStartIndex;
					sFLAT_RENDER_ROW kCardRow;
					kCardRow.s_bHeader = false;
					kCardRow.s_nCardStartIndex = nCardStartIndex;
					kCardRow.s_nCardCount = ( nRemainCards > nCardsPerRow ) ? nCardsPerRow : nRemainCards;
					vRenderRows.push_back( kCardRow );
				}

				nGroupStart = nGroupEnd;
			}
		}
		else
		{
			for( int nCardStartIndex = 0 ; nCardStartIndex < nTotalCards ; nCardStartIndex += nCardsPerRow )
			{
				int nRemainCards = nTotalCards - nCardStartIndex;
				sFLAT_RENDER_ROW kCardRow;
				kCardRow.s_bHeader = false;
				kCardRow.s_nCardStartIndex = nCardStartIndex;
				kCardRow.s_nCardCount = ( nRemainCards > nCardsPerRow ) ? nCardsPerRow : nRemainCards;
				vRenderRows.push_back( kCardRow );
			}
		}

		int nRowCount = (int)vRenderRows.size();
		int nRowsAddedThisPass = 0;
		int const nRowBudget = bLazyAppend ? ENCY_LAZY_APPEND_ROWS_PER_FRAME : ENCY_LAZY_INITIAL_ROWS;
		for( int nRow = m_nEncyLazyFlatRowIndex ; nRow < nRowCount ; ++nRow )
		{
			sFLAT_RENDER_ROW const& kRenderRow = vRenderRows[ nRow ];
			if( kRenderRow.s_bHeader )
			{
				cString* pHeaderItem = NiNew cString;
				if( pHeaderItem == NULL )
					return;
				cListBoxItem* pHeaderListItem = NiNew cListBoxItem;
				if( pHeaderListItem == NULL )
				{
					SAFE_DELETE( pHeaderItem );
					return;
				}

				cSprite* pGradeBar = NiNew cSprite;
				pGradeBar->Init( NULL, CsPoint::ZERO, CsPoint( 802, 26 ), "Encyclopedia\\newencyclopedia\\main\\grade_bar.png", false );
				cString::sSPRITE* pGradeBarSprite = pHeaderItem->AddSprite( pGradeBar, CsPoint( 0, 20 ) );
				if( pGradeBarSprite == NULL )
					return;
				pGradeBarSprite->SetAutoPointerDelete( true );

				cImage* pGradeIcon = NiNew cImage;
				pGradeIcon->Init( NULL, CsPoint::ZERO, CsPoint( 76, 76 ), "Encyclopedia\\newencyclopedia\\icon\\encyclopedia_512.png", false, true );
				pGradeIcon->SetTexToken( CsPoint( 102, 102 ) );
				int nHeaderRankIndex = _GetEncyRankIconIndex( kRenderRow.s_nRank );
				cString::sIMAGE* pHeaderRankSprite = pHeaderItem->AddImage( pGradeIcon, nHeaderRankIndex, CsPoint( 363, 0 ) );
				if( pHeaderRankSprite == NULL )
					return;
				pHeaderRankSprite->SetAutoPointerDelete( true );

				pHeaderListItem->Delete();
				pHeaderListItem->SetUserData( new sDIGIMON_ID( vFlatCards[ kRenderRow.s_nCardStartIndex ].s_nDigimonID ) );
				pHeaderListItem->SetItem( pHeaderItem );
				pHeaderListItem->SetItemHeight( 60 );
				m_pEncyListBox->AddItemMultiHeight( pHeaderListItem, 0, false );
			}
			else
			{
				int nRowWidth = ( kRenderRow.s_nCardCount * nCardWidth ) + ( ( kRenderRow.s_nCardCount - 1 ) * nCardGapX );
				int nStartX = nCardAreaStartX + ( ( nCardAreaWidth - nRowWidth ) / 2 );
				if( AddFlatCardRowToList( kRenderRow.s_nCardStartIndex, kRenderRow.s_nCardCount, nStartX ) == false )
					return;
			}

			nRowsAddedThisPass++;
			m_nEncyLazyFlatRowIndex = nRow + 1;
			if( nRowsAddedThisPass >= nRowBudget && m_nEncyLazyFlatRowIndex < nRowCount )
			{
				m_bEncyLazyAppendActive = true;
				if( m_pEncyListBox )
				{
					m_pEncyListBox->ReconfigureScrollbars();
					if( bLazyAppend == false && m_pScrollBar && m_pScrollBar->IsEnableScroll() )
						m_pScrollBar->SetCurPosIndex( 0 );
				}
				ENCY_PERF_INFO( "SetTabListData flat lazy partial sub=%d rows=%d/%d listRows=%d elapsedMs=%u",
					m_nSubMenu,
					m_nEncyLazyFlatRowIndex,
					nRowCount,
					m_pEncyListBox ? (int)m_pEncyListBox->GetItemCount() : 0,
					(unsigned)( GetTickCount() - dwBegin ) );
				return;
			}
		}
		if( m_pEncyListBox )
		{
			m_pEncyListBox->ReconfigureScrollbars();
			if( bLazyAppend == false && m_pScrollBar && m_pScrollBar->IsEnableScroll() )
				m_pScrollBar->SetCurPosIndex( 0 );
		}
		m_bEncyListBuilt = true;
		m_bEncyLazyAppendActive = false;
		m_nEncyLazyFlatRowIndex = 0;
		m_wsEncyListCacheSearch = m_wsSearchKeyword;
		m_nEncyListCacheSubMenu = m_nSubMenu;
		ENCY_PERF_INFO( "SetTabListData flat end main=%d sub=%d sourceLines=%d cards=%d rows=%d hover=%d elapsedMs=%u",
			m_nMainTab,
			m_nSubMenu,
			(int)m_pCurTabList_map.size(),
			(int)vFlatCards.size(),
			m_pEncyListBox ? (int)m_pEncyListBox->GetItemCount() : 0,
			(int)m_vCardHoverInfo.size(),
			(unsigned)( GetTickCount() - dwBegin ) );
		return;
	}

	EncyclopediaContents::MAP_IT it = m_pCurTabList_map.begin();
	EncyclopediaContents::MAP_IT itEnd = m_pCurTabList_map.end();

	//cSprite* IconBG = NiNew cSprite;
	//IconBG->Init( NULL, CsPoint::ZERO, CsPoint( 44, 44 ), "DigimonStatus\\Evol\\IconBG.tga", false );

	CsPoint ptDigimonPos = CsPoint( 0, 0 );
	int nSeriesSourceIndex = 0;
	int nRowsAddedThisPass = 0;
	int const nSeriesRowBudget = bLazyAppend ? ENCY_LAZY_APPEND_ROWS_PER_FRAME : ENCY_LAZY_INITIAL_ROWS;

	for( ; it != itEnd ; it++, nSeriesSourceIndex++ )
	{
		if( nSeriesSourceIndex < m_nEncyLazySourceIndex )
			continue;

		if( !IsSeriesLineMatched( it->second ) )
		{
			m_nEncyLazySourceIndex = nSeriesSourceIndex + 1;
			continue;
		}

		cString * pItem = NiNew cString;
		SAFE_POINTER_BEK( pItem );

		cListBoxItem * addItem  = NiNew cListBoxItem;
		SAFE_POINTER_BEK( addItem );

		cText::sTEXTINFO titleInfo;
		titleInfo.Init( &g_pEngine->m_FontSystem );
		titleInfo.s_eFontSize = CFont::FS_14;
		titleInfo.s_eTextAlign = DT_CENTER;
		titleInfo.SetText( it->second->s_cDigimonName.c_str() );
		pItem->AddText( &titleInfo, ptDigimonPos + CsPoint( 401, 8 ) );

		const char* pStateStrip = it->second->s_bIsAllOpen
			? "Encyclopedia\\newencyclopedia\\main\\collection completed.png"
			: "Encyclopedia\\newencyclopedia\\main\\uncollected.png";
		cSprite* pStateBG = NiNew cSprite;
		pStateBG->Init( NULL, CsPoint::ZERO, CsPoint( 802, 26 ), pStateStrip, false );
		cString::sSPRITE* pStateSprite = pItem->AddSprite( pStateBG, ptDigimonPos + CsPoint( 0, 40 ) );
		SAFE_POINTER_RET( pStateSprite );
		pStateSprite->SetAutoPointerDelete( true );

		cText::sTEXTINFO stateTextInfo;
		stateTextInfo.Init( &g_pEngine->m_FontSystem );
		stateTextInfo.s_eFontSize = CFont::FS_14;
		stateTextInfo.s_eTextAlign = DT_CENTER;
		stateTextInfo.s_Color = NiColor::WHITE;
		stateTextInfo.SetText( it->second->s_bIsAllOpen ? _T( "C O M P L E T E D" ) : _T( "C O L L E C T I N G" ) );
		pItem->AddText( &stateTextInfo, ptDigimonPos + CsPoint( 401, 48 ) );

		int nVisibleTotal = 0;
		for( int nCountIdx = 0 ; nCountIdx < nLimit::EvoUnit ; nCountIdx++ )
		{
			if( it->second->s_sInfo[ nCountIdx ].s_nDigimonID != -1
				&& it->second->s_sInfo[ nCountIdx ].s_eImgState != EncyclopediaContents::sINFO::S_NONE )
				nVisibleTotal++;
		}

		int nVisibleCount = 0;
		bool bSixColumnCards = nVisibleTotal > 10 && nVisibleTotal <= 18;
		bool bMediumCards = nVisibleTotal > 18 && nVisibleTotal <= 28;
		bool bCompactCards = nVisibleTotal > 28;
		int nCardsPerRow = 5;
		int nCardWidth = 116;
		int nCardHeight = 146;
		int nCardGapX = 40;
		if( bSixColumnCards )
		{
			nCardsPerRow = 6;
			nCardGapX = 12;
		}
		else if( bMediumCards )
		{
			nCardsPerRow = 7;
			nCardWidth = 96;
			nCardHeight = 122;
			nCardGapX = 12;
		}
		else if( bCompactCards )
		{
			nCardsPerRow = 8;
			nCardWidth = 82;
			nCardHeight = 104;
			nCardGapX = 12;
		}
		int nCardStepX = nCardWidth + nCardGapX;
		int nCardAreaWidth = 760;
		int nCardAreaStartX = ( 802 - nCardAreaWidth ) / 2;
		int nCardStartY = 76;
		int nCardRowStepY = 158;
		int nCardBgWidth = 100;
		int nCardBgHeight = 128;
		CsPoint ptCardBgOffset = CsPoint( 8, 11 );
		CsPoint ptIconSize = CsPoint( 50, 50 );
		CsPoint ptIconOffset = CsPoint( 33, 27 );
		CsPoint ptAttrBarSize = CsPoint( 98, 20 );
		CsPoint ptAttrBarOffset = CsPoint( 9, 80 );
		CsPoint ptGrowSize = CsPoint( 22, 14 );
		CsPoint ptDetailIconSize = CsPoint( 16, 16 );
		CsPoint ptRankSize = CsPoint( 76, 76 );
		CsPoint ptRankOffset = CsPoint( 20, 87 );
		if( bMediumCards )
		{
			nCardRowStepY = 116;
			nCardBgWidth = 84;
			nCardBgHeight = 106;
			ptCardBgOffset = CsPoint( 6, 8 );
			ptIconSize = CsPoint( 40, 40 );
			ptIconOffset = CsPoint( 28, 22 );
			ptAttrBarSize = CsPoint( 82, 17 );
			ptAttrBarOffset = CsPoint( 7, 67 );
			ptGrowSize = CsPoint( 19, 12 );
			ptDetailIconSize = CsPoint( 13, 13 );
			ptRankSize = CsPoint( 62, 62 );
			ptRankOffset = CsPoint( 17, 74 );
		}
		else if( bCompactCards )
		{
			nCardRowStepY = 116;
			nCardBgWidth = 72;
			nCardBgHeight = 90;
			ptCardBgOffset = CsPoint( 5, 7 );
			ptIconSize = CsPoint( 34, 34 );
			ptIconOffset = CsPoint( 24, 20 );
			ptAttrBarSize = CsPoint( 70, 14 );
			ptAttrBarOffset = CsPoint( 6, 58 );
			ptGrowSize = CsPoint( 18, 11 );
			ptDetailIconSize = CsPoint( 11, 11 );
			ptRankSize = CsPoint( 54, 54 );
			ptRankOffset = CsPoint( 14, 61 );
		}

		auto AddSeriesCardToRow = [&]( cString* pRowItem, cListBoxItem* pRowListItem, int nInfoIdx, CsPoint ptCard )
		{
			CsDigimon* pFTDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( it->second->s_sInfo[ nInfoIdx ].s_nDigimonID );
			CsDigimon::sINFO* pFTInfo = pFTDigimon ? pFTDigimon->GetInfo() : NULL;

			const char* pFrameTexture = _GetEncyCardFrameTexture( pFTInfo ? pFTInfo->s_nDigimonRank : 0 );

			cString::sSPRITE* pCardBGSprite = _AddEncySpriteRect(
				pRowItem,
				ENCY_UNION_CARD_BG,
				ptCard + ptCardBgOffset,
				CsPoint( nCardBgWidth, nCardBgHeight ),
				CsRect( 0, 0, 102, 128 ) );
			SAFE_POINTER_RET( pCardBGSprite );

			_AddEncyCircularPortrait(
				pRowItem,
				it->second->s_sInfo[ nInfoIdx ].s_ImgFileName,
				ptCard + ptIconOffset,
				ptIconSize.x,
				it->second->s_sInfo[ nInfoIdx ].s_eImgState == EncyclopediaContents::sINFO::S_CLOSE );

			cString::sIMAGE* pCardFrameSprite = _AddEncyFrameImage( pRowItem, pFrameTexture, ptCard, CsPoint( nCardWidth, nCardHeight ), true );
			SAFE_POINTER_RET( pCardFrameSprite );

			if( pFTInfo )
				_AddEncyCardName( pRowItem, pFTInfo->s_szName, ptCard, nCardWidth );

			cSprite* pAttributeBar = NiNew cSprite;
			pAttributeBar->Init( NULL, CsPoint::ZERO, ptAttrBarSize, "Encyclopedia\\newencyclopedia\\rate_frame\\attribute_bar.png", false );
			cString::sSPRITE* pAttributeBarSprite = pRowItem->AddSprite( pAttributeBar, ptCard + ptAttrBarOffset );
			SAFE_POINTER_RET( pAttributeBarSprite );
			pAttributeBarSprite->SetAutoPointerDelete( true );

			if( pFTInfo )
			{
				int nIconX = bCompactCards ? 8 : ( bMediumCards ? 9 : 13 );
				int nIconY = bCompactCards ? 60 : ( bMediumCards ? 69 : 83 );
				pRowItem->AddIcon( ptGrowSize, ICONITEM::DGrowType, pFTInfo->s_nDigimonType - 1, 0, ptCard + CsPoint( nIconX, nIconY ) );
				nIconX += ptGrowSize.x;

				int nIndex = pFTInfo->s_eAttributeType - nsCsDigimonTable::AT_NONE;
				pRowItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::DAttribute, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
				nIconX += ptDetailIconSize.x;

				int nIconLimitX = bCompactCards ? 64 : ( bMediumCards ? 78 : 88 );
				for( int nFamily = 0 ; nFamily < DIGIMON_MAX_FAMILY && nIconX <= nIconLimitX ; ++nFamily )
				{
					if( pFTInfo->s_eFamilyType[ nFamily ] != 0 )
					{
						nIndex = pFTInfo->s_eFamilyType[ nFamily ] - nsCsDigimonTable::FT_DR;
						pRowItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Family, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
						nIconX += ptDetailIconSize.x;
					}
				}

				for( int nNature = 0 ; nNature < 3 && nIconX <= nIconLimitX ; ++nNature )
				{
					if( pFTInfo->s_eBaseNatureTypes[ nNature ] != 0 )
					{
						nIndex = pFTInfo->s_eBaseNatureTypes[ nNature ] - nsCsDigimonTable::NT_ICE;
						pRowItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Nature, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
						nIconX += ptDetailIconSize.x;
					}
				}

				cImage* pRank = NiNew cImage;
				pRank->Init( NULL, CsPoint::ZERO, ptRankSize, "Encyclopedia\\newencyclopedia\\icon\\encyclopedia_512.png", false, true );
				pRank->SetTexToken( CsPoint( 102, 102 ) );
				int nRankIndex = _GetEncyRankIconIndex( pFTInfo->s_nDigimonRank );
				cString::sIMAGE* pRankSprite = pRowItem->AddImage( pRank, nRankIndex, ptCard + ptRankOffset );
				SAFE_POINTER_RET( pRankSprite );
				pRankSprite->SetAutoPointerDelete( true );
			}

			sCARD_HOVER_INFO kHoverInfo;
			kHoverInfo.s_pItem = pRowListItem;
			kHoverInfo.s_LocalRect = CsRect( ptCard, CsSIZE( nCardWidth, nCardHeight ) );
			kHoverInfo.s_nDigimonID = it->second->s_sInfo[ nInfoIdx ].s_nDigimonID;
			kHoverInfo.s_nTooltipDigimonID = it->second->s_sInfo[ nInfoIdx ].s_nDigimonID;
			m_vCardHoverInfo.push_back( kHoverInfo );
		};

		int nTooltipDigimonIdForRows = 0;
		for( int nInfoIdx = 0 ; nInfoIdx < nLimit::EvoUnit ; ++nInfoIdx )
		{
			if( it->second->s_sInfo[ nInfoIdx ].s_nDigimonID == -1 )
				continue;
			if( it->second->s_sInfo[ nInfoIdx ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
				continue;
			nTooltipDigimonIdForRows = it->second->s_sInfo[ nInfoIdx ].s_nDigimonID;
			break;
		}

		addItem->Delete();
		addItem->SetUserData(new sDIGIMON_ID( nTooltipDigimonIdForRows ));
		addItem->SetItem( pItem );
		addItem->SetItemHeight( nCardStartY );
		m_pEncyListBox->AddItemMultiHeight( addItem, 0, false );

		int nRowsForSeries = ( nVisibleTotal + nCardsPerRow - 1 ) / nCardsPerRow;
		for( int nRow = 0 ; nRow < nRowsForSeries ; ++nRow )
		{
			cString* pRowItem = NiNew cString;
			SAFE_POINTER_BEK( pRowItem );
			cListBoxItem* pRowListItem = NiNew cListBoxItem;
			SAFE_POINTER_BEK( pRowListItem );

			int nRowVisibleStart = nRow * nCardsPerRow;
			int nRowVisibleEnd = nRowVisibleStart + nCardsPerRow;
			int nRowVisibleIndex = 0;
			int nCardsInThisRow = 0;
			for( int nCountIdx = 0 ; nCountIdx < nLimit::EvoUnit ; ++nCountIdx )
			{
				if( it->second->s_sInfo[ nCountIdx ].s_nDigimonID == -1
					|| it->second->s_sInfo[ nCountIdx ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
					continue;
				if( nRowVisibleIndex >= nRowVisibleStart && nRowVisibleIndex < nRowVisibleEnd )
					nCardsInThisRow++;
				nRowVisibleIndex++;
			}

			int nRowWidth = ( nCardsInThisRow * nCardWidth ) + ( ( nCardsInThisRow - 1 ) * nCardGapX );
			int nStartX = nCardAreaStartX + ( ( nCardAreaWidth - nRowWidth ) / 2 );
			nRowVisibleIndex = 0;
			int nCol = 0;
			for( int nCountIdx = 0 ; nCountIdx < nLimit::EvoUnit ; ++nCountIdx )
			{
				if( it->second->s_sInfo[ nCountIdx ].s_nDigimonID == -1
					|| it->second->s_sInfo[ nCountIdx ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
					continue;
				if( nRowVisibleIndex >= nRowVisibleStart && nRowVisibleIndex < nRowVisibleEnd )
				{
					CsPoint ptCard = CsPoint( nStartX + ( nCol * nCardStepX ), 0 );
					AddSeriesCardToRow( pRowItem, pRowListItem, nCountIdx, ptCard );
					nCol++;
				}
				nRowVisibleIndex++;
			}

			int nRowItemHeight = nCardRowStepY;
			if( nRowItemHeight < nCardHeight + 8 )
				nRowItemHeight = nCardHeight + 8;

			pRowListItem->Delete();
			pRowListItem->SetUserData(new sDIGIMON_ID( nTooltipDigimonIdForRows ));
			pRowListItem->SetItem( pRowItem );
			pRowListItem->SetItemHeight( nRowItemHeight );
			m_pEncyListBox->AddItemMultiHeight( pRowListItem, 0, false );
		}
		nRowsAddedThisPass += 1 + nRowsForSeries;
		m_nEncyLazySourceIndex = nSeriesSourceIndex + 1;
		if( nRowsAddedThisPass >= nSeriesRowBudget && m_nEncyLazySourceIndex < (int)m_pCurTabList_map.size() )
		{
			m_bEncyLazyAppendActive = true;
			if( m_pEncyListBox )
			{
				m_pEncyListBox->ReconfigureScrollbars();
				if( bLazyAppend == false && m_pScrollBar && m_pScrollBar->IsEnableScroll() )
					m_pScrollBar->SetCurPosIndex( 0 );
			}
			ENCY_PERF_INFO( "SetTabListData series lazy partial source=%d/%d rows=%d listRows=%d elapsedMs=%u",
				m_nEncyLazySourceIndex,
				(int)m_pCurTabList_map.size(),
				nRowsAddedThisPass,
				m_pEncyListBox ? (int)m_pEncyListBox->GetItemCount() : 0,
				(unsigned)( GetTickCount() - dwBegin ) );
			return;
		}
		continue;

		for( int j = 0 ; j < nLimit::EvoUnit ; j++ )
		{
			if( it->second->s_sInfo[ j ].s_nDigimonID == -1
				|| it->second->s_sInfo[ j ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
				continue;

			int nRow = nVisibleCount / nCardsPerRow;
			int nCol = nVisibleCount % nCardsPerRow;

			int nRemainCount = nVisibleTotal - ( nRow * nCardsPerRow );
			int nRowCount = ( nRemainCount > nCardsPerRow ) ? nCardsPerRow : nRemainCount;
			if( nRowCount <= 0 )
				nRowCount = 1;

			int nRowWidth = ( nRowCount * nCardWidth ) + ( ( nRowCount - 1 ) * nCardGapX );
			int nStartX = nCardAreaStartX + ( ( nCardAreaWidth - nRowWidth ) / 2 );
			CsPoint ptCard = ptDigimonPos + CsPoint( nStartX + ( nCol * nCardStepX ), nCardStartY + ( nRow * nCardRowStepY ) );

			CsDigimon* pFTDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( it->second->s_sInfo[ j ].s_nDigimonID );
			CsDigimon::sINFO* pFTInfo = pFTDigimon ? pFTDigimon->GetInfo() : NULL;

			const char* pFrameTexture = "Encyclopedia\\newencyclopedia\\rate_frame\\collection_frame_rank1.png";
			if( it->second->s_sInfo[ j ].s_eImgState == EncyclopediaContents::sINFO::S_OPEN )
			{
				pFrameTexture = it->second->s_bIsAllOpen
					? "Encyclopedia\\newencyclopedia\\rate_frame\\collection_frame_rank3.png"
					: "Encyclopedia\\newencyclopedia\\rate_frame\\collection_frame_rank2.png";
			}

			int nCardBgIndex = 0;
			if( it->second->s_sInfo[ j ].s_eImgState == EncyclopediaContents::sINFO::S_OPEN )
				nCardBgIndex = it->second->s_bIsAllOpen ? 2 : 1;

			cImage* pCardBG = NiNew cImage;
			pCardBG->Init( NULL, CsPoint::ZERO, CsPoint( nCardBgWidth, nCardBgHeight ), "Encyclopedia\\newencyclopedia\\rate_frame\\collection_bg.tga", false, true );
			pCardBG->SetTexToken( CsPoint( 120, 150 ) );
			cString::sIMAGE* pCardBGSprite = pItem->AddImage( pCardBG, (cImage::eSTATE)nCardBgIndex, ptCard + ptCardBgOffset );
			SAFE_POINTER_RET( pCardBGSprite );
			pCardBGSprite->SetAutoPointerDelete( true );

			cImage* pCardFrame = NiNew cImage;
			pCardFrame->Init( NULL, CsPoint::ZERO, CsPoint( nCardWidth, nCardHeight ), pFrameTexture, false, true );
			pCardFrame->SetTexToken( CsPoint( 138, 174 ) );
			cString::sIMAGE* pCardFrameSprite = pItem->AddImage( pCardFrame, (cImage::eSTATE)(cImage::NORMAL0), ptCard );
			SAFE_POINTER_RET( pCardFrameSprite );
			pCardFrameSprite->SetAutoPointerDelete( true );

			cImage* pCardOverFrame = NiNew cImage;
			pCardOverFrame->Init( NULL, CsPoint::ZERO, CsPoint( nCardWidth, nCardHeight ), pFrameTexture, false, true );
			pCardOverFrame->SetTexToken( CsPoint( 138, 174 ) );
			cString::sIMAGE* pCardOverFrameSprite = pItem->AddImage( pCardOverFrame, (cImage::eSTATE)(cImage::NORMAL1), ptCard );
			SAFE_POINTER_RET( pCardOverFrameSprite );
			pCardOverFrameSprite->SetAutoPointerDelete( true );
			pCardOverFrameSprite->SetVisible( false );

			cEncyHoverEffectSprite* pHoverEffect = NiNew cEncyHoverEffectSprite;
			pHoverEffect->InitHoverEffect( CsPoint( nCardWidth, nCardHeight ) );
			cString::sSPRITE* pHoverEffectSprite = pItem->AddSprite( pHoverEffect, ptCard );
			SAFE_POINTER_RET( pHoverEffectSprite );
			pHoverEffectSprite->SetAutoPointerDelete( true );
			pHoverEffectSprite->SetVisible( false );

			cImage*	IconIMG = NiNew cImage;
			IconIMG->Init( NULL, CsPoint::ZERO, ptIconSize, it->second->s_sInfo[ j ].s_ImgFileName.c_str(), false, false );

			cString::sIMAGE* pImage = pItem->AddImage( IconIMG, (cImage::eSTATE)(cImage::NORMAL1), ptCard + ptIconOffset );
			pImage->SetAutoPointerDelete(true);
			if( it->second->s_sInfo[ j ].s_eImgState == EncyclopediaContents::sINFO::S_CLOSE )
				pImage->SetColor( NiColor( 0.05f, 0.05f, 0.05f ) );

			cSprite* pAttributeBar = NiNew cSprite;
			pAttributeBar->Init( NULL, CsPoint::ZERO, ptAttrBarSize, "Encyclopedia\\newencyclopedia\\rate_frame\\attribute_bar.png", false );
			cString::sSPRITE* pAttributeBarSprite = pItem->AddSprite( pAttributeBar, ptCard + ptAttrBarOffset );
			SAFE_POINTER_RET( pAttributeBarSprite );
			pAttributeBarSprite->SetAutoPointerDelete( true );

			if( pFTInfo )
			{
				int nIconX = bCompactCards ? 8 : ( bMediumCards ? 9 : 13 );
				int nIconY = bCompactCards ? 60 : ( bMediumCards ? 69 : 83 );
				pItem->AddIcon( ptGrowSize, ICONITEM::DGrowType, pFTInfo->s_nDigimonType - 1, 0, ptCard + CsPoint( nIconX, nIconY ) );
				nIconX += ptGrowSize.x;

				int nIndex = pFTInfo->s_eAttributeType - nsCsDigimonTable::AT_NONE;
				pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::DAttribute, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
				nIconX += ptDetailIconSize.x;

				int nIconLimitX = bCompactCards ? 64 : ( bMediumCards ? 78 : 88 );
				for( int nFamily = 0 ; nFamily < DIGIMON_MAX_FAMILY && nIconX <= nIconLimitX ; ++nFamily )
				{
					if( pFTInfo->s_eFamilyType[ nFamily ] != 0 )
					{
						nIndex = pFTInfo->s_eFamilyType[ nFamily ] - nsCsDigimonTable::FT_DR;
						pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Family, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
						nIconX += ptDetailIconSize.x;
					}
				}

				for( int nNature = 0 ; nNature < 3 && nIconX <= nIconLimitX ; ++nNature )
				{
					if( pFTInfo->s_eBaseNatureTypes[ nNature ] != 0 )
					{
						nIndex = pFTInfo->s_eBaseNatureTypes[ nNature ] - nsCsDigimonTable::NT_ICE;
						pItem->AddIcon( ptDetailIconSize, ICONITEM::DAttribute, cIconMng::Nature, nIndex, ptCard + CsPoint( nIconX, bCompactCards ? 60 : ( bMediumCards ? 69 : 82 ) ) );
						nIconX += ptDetailIconSize.x;
					}
				}

				cImage* pRank = NiNew cImage;
				pRank->Init( NULL, CsPoint::ZERO, ptRankSize, "Encyclopedia\\newencyclopedia\\icon\\encyclopedia_512.png", false, true );
				pRank->SetTexToken( CsPoint( 102, 102 ) );
				int nRankIndex = _GetEncyRankIconIndex( pFTInfo->s_nDigimonRank );
				cString::sIMAGE* pRankSprite = pItem->AddImage( pRank, nRankIndex, ptCard + ptRankOffset );
				SAFE_POINTER_RET( pRankSprite );
				pRankSprite->SetAutoPointerDelete( true );
			}

			sCARD_HOVER_INFO kHoverInfo;
			kHoverInfo.s_pItem = addItem;
			kHoverInfo.s_LocalRect = CsRect( ptCard, CsSIZE( nCardWidth, nCardHeight ) );
			kHoverInfo.s_pOverFrame = pCardOverFrameSprite;
			kHoverInfo.s_pEffect = pHoverEffectSprite;
			kHoverInfo.s_nDigimonID = it->second->s_sInfo[ j ].s_nDigimonID;
			kHoverInfo.s_nTooltipDigimonID = it->second->s_sInfo[ j ].s_nDigimonID;
			m_vCardHoverInfo.push_back( kHoverInfo );
			nVisibleCount++;
		}

		int nTooltipDigimonId = 0;
		for( int nInfoIdx = 0 ; nInfoIdx < nLimit::EvoUnit ; ++nInfoIdx )
		{
			if( it->second->s_sInfo[ nInfoIdx ].s_nDigimonID == -1 )
				continue;
			if( it->second->s_sInfo[ nInfoIdx ].s_eImgState == EncyclopediaContents::sINFO::S_NONE )
				continue;
			nTooltipDigimonId = it->second->s_sInfo[ nInfoIdx ].s_nDigimonID;
			break;
		}

		addItem->Delete();
		addItem->SetUserData(new sDIGIMON_ID( nTooltipDigimonId ));
		addItem->SetItem( pItem );
		int nRows = ( nVisibleCount + nCardsPerRow - 1 ) / nCardsPerRow;
		if( nRows <= 0 )
			nRows = 1;

		// cString::GetMaxSize() does not include Y offsets of added elements, so set
		// explicit list item height to prevent entry overlap.
		int nItemHeight = nCardStartY + ( ( nRows - 1 ) * nCardRowStepY ) + nCardHeight + 48;
		if( nItemHeight < 244 )
			nItemHeight = 244;
		addItem->SetItemHeight( nItemHeight );
		m_pEncyListBox->AddItemMultiHeight( addItem, 0, false );
	}

	if( m_pEncyListBox )
	{
		m_pEncyListBox->ReconfigureScrollbars();
		if( bLazyAppend == false && m_pScrollBar && m_pScrollBar->IsEnableScroll() )
			m_pScrollBar->SetCurPosIndex( 0 );
	}
	m_bEncyListBuilt = true;
	m_bEncyLazyAppendActive = false;
	m_nEncyLazySourceIndex = 0;
	m_wsEncyListCacheSearch = m_wsSearchKeyword;
	m_nEncyListCacheSubMenu = m_nSubMenu;

	ENCY_PERF_INFO( "SetTabListData series end main=%d sub=%d sourceLines=%d rows=%d hover=%d elapsedMs=%u",
		m_nMainTab,
		m_nSubMenu,
		(int)m_pCurTabList_map.size(),
		m_pEncyListBox ? (int)m_pEncyListBox->GetItemCount() : 0,
		(int)m_vCardHoverInfo.size(),
		(unsigned)( GetTickCount() - dwBegin ) );

	//SetTabListDataRenderCount();
}

// void cEncyclopedia::SetTabListDataRenderCount()
// {
// 	EncyclopediaContents::MAP_CIT_R it	  = m_pCurTabList_map.rbegin();
// 	EncyclopediaContents::MAP_CIT_R itEnd = m_pCurTabList_map.rend();
//
// 	int nRenderCnt = 0;
// 	int count = 0;
//
// 	for( ; it != itEnd ; it++ )
// 	{
// 		if( it->second->s_nCount > 10 )
// 		{
// 			nRenderCnt = nRenderCnt + 3;
// 		}
// 		else if( it->second->s_nCount > 5 )
// 		{
// 			nRenderCnt = nRenderCnt + 2;
// 		}
// 		else
// 		{
// 			nRenderCnt = nRenderCnt + 1;
// 		}
//
// 		count++;
//
// 		if(nRenderCnt >= 5)
// 			break;
// 	}
//	m_pEncyListBox->SetScrollBarRenderCount(count);
// }

void cEncyclopedia::SetTabGroupListData()
{
	DWORD const dwBegin = GetTickCount();
	cListBox* pDeckListBox = m_pDeckListBox ? m_pDeckListBox : m_pEncyListBox;
	if( pDeckListBox == NULL )
		return;
	bool const bLazyAppend = m_bDeckLazyAppendActive;
	if( bLazyAppend == false )
	{
		m_bDeckListBuilt = false;
		m_wsDeckListCacheSearch.clear();
		m_nDeckLazyGroupCursor = 0;
		m_vDeckLazyGroups.clear();
		_ClearIconList();
		pDeckListBox->RemoveAllItem();
		m_vDeckPanelHoverInfo.clear();
		m_vDeckBookmarkInfo.clear();
	}
	ENCY_DECK_LOG_INFO( "SetTabGroupListData begin. mainTab=%d subMenu=%d groupCount=%d useDeck=%d",
		m_nMainTab, m_nSubMenu, (int)GetSystem()->GetGroupMapData().size(), GetSystem()->GetUseDeck() );

	auto ToLowerCopy = []( std::wstring const& wsSrc )->std::wstring
	{
		std::wstring wsOut = wsSrc;
		for( size_t i = 0; i < wsOut.size(); ++i )
		{
			wsOut[i] = (WCHAR)::towlower( wsOut[i] );
		}
		return wsOut;
	};
	std::wstring wsDeckSearch = ToLowerCopy( m_wsSearchKeyword );
	auto IsDeckGroupMatched = [&]( EncyclopediaContents::sGROUP_INFO const* pGroupInfo )->bool
	{
		if( pGroupInfo == NULL )
			return false;
		if( wsDeckSearch.empty() )
			return true;

		std::wstring wsGroupName = ToLowerCopy( pGroupInfo->s_GroupName );
		if( wsGroupName.find( wsDeckSearch ) != std::wstring::npos )
			return true;

		if( nsCsFileTable::g_pDigimonMng )
		{
			CsEncy_Deck* pDeck = nsCsFileTable::g_pDigimonMng->GetDeck( pGroupInfo->s_nGroupIdx );
			CsEncy_Deck::sINFO* pDeckInfo = pDeck ? pDeck->GetInfo() : NULL;
			if( pDeckInfo )
			{
				std::wstring wsDeckName = ToLowerCopy( pDeckInfo->s_szGroupName );
				if( wsDeckName.find( wsDeckSearch ) != std::wstring::npos )
					return true;

				std::wstring wsDeckExplain = ToLowerCopy( pDeckInfo->s_szExplain );
				if( wsDeckExplain.find( wsDeckSearch ) != std::wstring::npos )
					return true;
			}
		}

		if( nsCsFileTable::g_pDigimonMng == NULL )
			return false;

		for( EncyclopediaContents::MAP_DATA_CIT it = pGroupInfo->s_pListData.begin(); it != pGroupInfo->s_pListData.end(); ++it )
		{
			if( (*it) == NULL )
				continue;

			DWORD const dwNameCandidates[2] = { (*it)->s_dwDestDigimonID, (*it)->s_dwBaseDigimonID };
			for( int i = 0; i < 2; ++i )
			{
				CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( dwNameCandidates[i] );
				CsDigimon::sINFO* pDigimonInfo = pDigimon ? pDigimon->GetInfo() : NULL;
				if( pDigimonInfo == NULL )
					continue;

				std::wstring wsName = ToLowerCopy( pDigimonInfo->s_szName );
				if( wsName.find( wsDeckSearch ) != std::wstring::npos )
					return true;
			}
		}
		return false;
	};

	if( bLazyAppend == false )
	{
		for( EncyclopediaContents::MAP_GROUP_CIT itGroup = GetSystem()->GetGroupMapData().begin();
			itGroup != GetSystem()->GetGroupMapData().end();
			++itGroup )
		{
			EncyclopediaContents::sGROUP_INFO* pGroupInfo = itGroup->second;
			if( IsDeckGroupMatched( pGroupInfo ) )
				m_vDeckLazyGroups.push_back( pGroupInfo );
		}
		std::stable_sort( m_vDeckLazyGroups.begin(), m_vDeckLazyGroups.end(),
			[this]( EncyclopediaContents::sGROUP_INFO* lhs, EncyclopediaContents::sGROUP_INFO* rhs )->bool
			{
				if( lhs == rhs )
					return false;
				if( lhs == NULL )
					return false;
				if( rhs == NULL )
					return true;

				bool const bLeftFavorite = ( m_setDeckFavoriteGroup.find( lhs->s_nGroupIdx ) != m_setDeckFavoriteGroup.end() );
				bool const bRightFavorite = ( m_setDeckFavoriteGroup.find( rhs->s_nGroupIdx ) != m_setDeckFavoriteGroup.end() );
				if( bLeftFavorite != bRightFavorite )
					return bLeftFavorite;

				return false;
			} );
	}

	int count = (int)pDeckListBox->GetItemCount();
	int nDeckPanelEntryIndex = m_nDeckLazyGroupCursor;
	int nRowsAddedThisPass = 0;
	int const nRowBudget = bLazyAppend ? ENCY_LAZY_APPEND_ROWS_PER_FRAME : ENCY_LAZY_INITIAL_ROWS;
	const int nDeckPanelWidth = 367;
	const int nDeckPanelHeight = 166;
	const int nDeckColGap = 10;
	const int nDeckColStartX = 17;
	const int nDeckRowPanelY = 4;
	const CsPoint ptDeckIconFrameSize( 45, 46 );
	const CsPoint ptDeckIconSize( 45, 46 );
	const CsPoint ptDeckIconOffset( 0, 0 );
	const int nDeckIconsPerRow = 7;
	const int nDeckIconStepX = 47;
	const int nDeckIconStepY = 48;
	const cImage::eSTATE eDeckUiState = (cImage::eSTATE)0;
	const std::string deckPanelBasePath = "Encyclopedia\\newencyclopedia\\deck_screen\\deck_bg.png";
	const std::string deckHeaderCollectingPath = _ResolveDeckTexturePath( {
		"data\\interface\\encyclopedia\\newencyclopedia\\deck_screen\\deck_collecting.png",
	} );
	const std::string deckHeaderCompletedPath = _ResolveDeckTexturePath( {
		"data\\interface\\encyclopedia\\newencyclopedia\\deck_screen\\deck_completed.png",
	} );
	const std::string deckHeaderEnablePath = _ResolveDeckTexturePath( {
		"data\\interface\\encyclopedia\\newencyclopedia\\deck_screen\\deck_enable .png",
	} );
	const std::string deckFramePath = _ResolveDeckTexturePath( {
		"data\\interface\\encyclopedia\\newencyclopedia\\deck_screen\\small_deck_frame.png",
	} );
	const std::string deckEquipButtonPath = _ResolveDeckTexturePath( {
		"data\\interface\\random_box\\button\\purchase_1.tga",
	} );
	const std::string deckBookmarkPath = _ResolveDeckTexturePath( {
		"data\\interface\\encyclopedia\\newencyclopedia\\detail\\button\\bookmark.png",
	} );
	const char* const pDeckHeaderCollectingPath = deckHeaderCollectingPath.c_str();
	const char* const pDeckFramePath = deckFramePath.c_str();
	const bool bDeckHeaderCollectingExists = _DeckFileExists( pDeckHeaderCollectingPath );
	const bool bDeckFrameExists = _DeckFileExists( pDeckFramePath );
	ENCY_DECK_LOG_INFO( "Deck static texture check. bg='%s' exists=%s collecting='%s' exists=%s frame='%s' exists=%s",
		deckPanelBasePath.c_str(),
		_BoolTF( _DeckFileExists( deckPanelBasePath ) ),
		pDeckHeaderCollectingPath,
		_BoolTF( bDeckHeaderCollectingExists ),
		pDeckFramePath,
		_BoolTF( bDeckFrameExists ) );

	for( size_t nGroupCursor = (size_t)m_nDeckLazyGroupCursor; nGroupCursor < m_vDeckLazyGroups.size(); )
	{
		cString * pItem = NiNew cString;
		SAFE_POINTER_BEK( pItem );

		cListBoxItem * addItem  = NiNew cListBoxItem;
		SAFE_POINTER_BEK( addItem );

		int nFirstGroupIdxForItem = -1;
		for( int nCol = 0; nCol < 2 && nGroupCursor < m_vDeckLazyGroups.size(); ++nCol, ++nGroupCursor )
		{
			EncyclopediaContents::sGROUP_INFO* pGroupInfo = m_vDeckLazyGroups[ nGroupCursor ];
			if( pGroupInfo == NULL )
				continue;

			int nPanelX = nDeckColStartX + ( nCol * ( nDeckPanelWidth + nDeckColGap ) );
			ENCY_DECK_LOG_INFO( "Deck panel slot map. panelEntry=%d listRow=%d col=%d groupIdx=%d name='%s' panelPos=(%d,%d) panelSize=(%d,%d)",
				nDeckPanelEntryIndex,
				count,
				nCol,
				pGroupInfo->s_nGroupIdx,
				pGroupInfo->s_GroupName.c_str(),
				nPanelX,
				nDeckRowPanelY,
				nDeckPanelWidth,
				nDeckPanelHeight );

			NiColor fColor = ENCY_ENABLE_COLOR;
			float fAlpha = ENCY_ENABLE_ALPHA;
			cSprite* pDeckPanelBase = NiNew cSprite;
			pDeckPanelBase->Init( NULL, CsPoint::ZERO, CsPoint( nDeckPanelWidth, nDeckPanelHeight ), deckPanelBasePath.c_str(), false );
			pDeckPanelBase->SetColor( ENCY_ENABLE_COLOR );
			pDeckPanelBase->SetAlpha( ENCY_ENABLE_ALPHA );
			cString::sSPRITE* pDeckPanelBaseSpr = pItem->AddSprite( pDeckPanelBase, CsPoint( nPanelX, nDeckRowPanelY ) );
			if( pDeckPanelBaseSpr != NULL )
				pDeckPanelBaseSpr->SetAutoPointerDelete( true );

			const bool bGroupCompleteByList = _IsDeckGroupCompleteByList( GetSystem()->GetEncyMapData(), pGroupInfo );
			const bool bGroupOpenForUI = ( pGroupInfo->s_bGroupOpen || bGroupCompleteByList );
			const bool bIsUseDeck = pGroupInfo->s_bUse;
			const char* pDeckHeaderTexture = pDeckHeaderCollectingPath;
			if( bIsUseDeck )
				pDeckHeaderTexture = deckHeaderEnablePath.c_str();
			else if( bGroupOpenForUI )
				pDeckHeaderTexture = deckHeaderCompletedPath.c_str();
			const bool bDeckHeaderExists = _DeckFileExists( pDeckHeaderTexture );
			ENCY_DECK_LOG_INFO( "Group panel build. listRow=%d col=%d groupIdx=%d name='%s' openRaw=%s openByList=%s openUI=%s isUse=%s header='%s' headerExists=%s",
				count,
				nCol,
				pGroupInfo->s_nGroupIdx,
				pGroupInfo->s_GroupName.c_str(),
				_BoolTF( pGroupInfo->s_bGroupOpen ),
				_BoolTF( bGroupCompleteByList ),
				_BoolTF( bGroupOpenForUI ),
				_BoolTF( bIsUseDeck ),
				pDeckHeaderTexture,
				_BoolTF( bDeckHeaderExists ) );

			if( bDeckHeaderExists )
			{
				cImage* pDeckHeader = NiNew cImage;
				pDeckHeader->Init( NULL, CsPoint::ZERO, CsPoint( 298, 28 ), pDeckHeaderTexture, false, false );
				pDeckHeader->SetColor( ENCY_ENABLE_COLOR );
				pDeckHeader->SetAlpha( ENCY_ENABLE_ALPHA );
				cString::sIMAGE* pDeckHeaderImage = pItem->AddImage( pDeckHeader, eDeckUiState, CsPoint( nPanelX + 34, 12 ) );
				if( pDeckHeaderImage == NULL )
				{
					ENCY_DECK_LOG_WARN( "DeckHeader AddImage failed. listRow=%d col=%d groupIdx=%d name='%s' header='%s'",
						count, nCol, pGroupInfo->s_nGroupIdx, pGroupInfo->s_GroupName.c_str(), pDeckHeaderTexture );
					SAFE_NIDELETE( pDeckHeader );
				}
				else
				{
					pDeckHeaderImage->SetAutoPointerDelete( true );
					ENCY_DECK_LOG_INFO( "DeckHeader AddImage ok. listRow=%d col=%d groupIdx=%d name='%s' header='%s' pos=(%d,%d)",
						count, nCol, pGroupInfo->s_nGroupIdx, pGroupInfo->s_GroupName.c_str(), pDeckHeaderTexture, nPanelX + 34, 12 );
				}
			}

			// Show bookmark on every deck entry (requested behavior).
			const bool bShowBookmark = true;

			EncyclopediaContents::MAP_DATA_CIT itList	 = pGroupInfo->s_pListData.begin();
			EncyclopediaContents::MAP_DATA_CIT itListEnd = pGroupInfo->s_pListData.end();
			int nListIdx = 0;
			int nSourceIdx = 0;
			for( ; itList != itListEnd ; ++itList )
			{
				EncyclopediaContents::sDATA* pDeckEntry = (*itList);
				if( pDeckEntry == NULL )
				{
					ENCY_DECK_LOG_WARN( " Icon source skipped (null entry). panelEntry=%d listRow=%d col=%d group=%d sourceIdx=%d",
						nDeckPanelEntryIndex,
						count,
						nCol,
						pGroupInfo->s_nGroupIdx,
						nSourceIdx );
					nSourceIdx++;
					continue;
				}

				if( pDeckEntry->s_bIsRender == false )
				{
					ENCY_DECK_LOG_INFO( " Icon source skipped. panelEntry=%d listRow=%d col=%d group=%d sourceIdx=%d render=%s open=%s base=%u dest=%u icon='%s'",
						nDeckPanelEntryIndex,
						count,
						nCol,
						pGroupInfo->s_nGroupIdx,
						nSourceIdx,
						_BoolTF( pDeckEntry->s_bIsRender ),
						_BoolTF( pDeckEntry->s_bIsOpen ),
						pDeckEntry->s_dwBaseDigimonID,
						pDeckEntry->s_dwDestDigimonID,
						pDeckEntry->s_ImgFileName.c_str() );
					nSourceIdx++;
					continue;
				}

				int nIconCol = nListIdx % nDeckIconsPerRow;
				int nIconRow = nListIdx / nDeckIconsPerRow;
				CsPoint ptPos( nPanelX + 25 + ( nIconCol * nDeckIconStepX ), 45 + ( nIconRow * nDeckIconStepY ) );

				bool bUseEvoAtlas = false;
				std::string strDeckIconFile = _GetDeckPortraitFileName( pDeckEntry->s_ImgFileName, bUseEvoAtlas );
				bool bDeckIconFileExists = _DeckFileExists( strDeckIconFile );
				if( bDeckIconFileExists == false )
				{
					// Fallback only if evo atlas is missing on disk.
					strDeckIconFile = pDeckEntry->s_ImgFileName;
					bDeckIconFileExists = _DeckFileExists( strDeckIconFile );
					bUseEvoAtlas = false;
				}
				const bool bFallbackIconFileExists = bDeckIconFileExists;
				if( bDeckIconFileExists == false )
				{
					// Do not draw missing portraits: the renderer uses a magenta fallback.
					// Reuse the loaded frame texture as a neutral placeholder under the top frame.
					strDeckIconFile = pDeckFramePath;
					bDeckIconFileExists = bDeckFrameExists;
					bUseEvoAtlas = false;
				}

				cImage* IconIMG = NiNew cImage;
				IconIMG->Init( NULL, CsPoint::ZERO, ptDeckIconSize, strDeckIconFile.c_str(), false, false );
#if ENCY_DECK_VALIDATE_EVO_ATLAS
				if( bUseEvoAtlas )
				{
					const float fInvTexW = IconIMG->GetInvTextWidth();
					const float fInvTexH = IconIMG->GetInvTextHeight();
					const int nTexW = ( fInvTexW > 0.0f ) ? (int)( ( 1.0f / fInvTexW ) + 0.5f ) : 0;
					const int nTexH = ( fInvTexH > 0.0f ) ? (int)( ( 1.0f / fInvTexH ) + 0.5f ) : 0;
					if( nTexW != 96 || nTexH != 32 )
					{
						ENCY_DECK_LOG_WARN( " Evo atlas texture size mismatch. group=%d idx=%d base=%u dest=%u icon='%s' tex=(%d,%d) expected=(96,32). Falling back to source icon.",
							pGroupInfo->s_nGroupIdx,
							nListIdx,
							pDeckEntry->s_dwBaseDigimonID,
							pDeckEntry->s_dwDestDigimonID,
							strDeckIconFile.c_str(),
							nTexW,
							nTexH );

						const std::string strFallbackFile = pDeckEntry->s_ImgFileName;
						const bool bFallbackExists = _DeckFileExists( strFallbackFile );
						if( bFallbackExists )
						{
							IconIMG->Init( NULL, CsPoint::ZERO, ptDeckIconSize, strFallbackFile.c_str(), false, false );
							strDeckIconFile = strFallbackFile;
							bUseEvoAtlas = false;
						}
					}
				}
#endif
				if( bUseEvoAtlas )
				{
					// Match the evolution slot UI: _evo portraits are 32px-wide atlas cells.
					IconIMG->SetTexToken( CsPoint( 32, 0 ) );
				}
				IconIMG->SetColor( fColor );
				IconIMG->SetAlpha( fAlpha );
				const cImage::eSTATE eDeckPortraitState =
					( bUseEvoAtlas && pDeckEntry->s_bIsOpen == false ) ? cImage::NORMAL2 : cImage::NORMAL0;
				ENCY_DECK_LOG_INFO( " Icon group=%d idx=%d sourceIdx=%d panelEntry=%d listRow=%d col=%d slot=(%d,%d) base=%u dest=%u isOpen=%s render=%s useEvo=%s state=%d evoPath='%s' evoExists=%s fallbackPath='%s' fallbackExists=%s",
						pGroupInfo->s_nGroupIdx,
					nListIdx,
					nSourceIdx,
					nDeckPanelEntryIndex,
					count,
					nCol,
					nIconCol,
					nIconRow,
					pDeckEntry->s_dwBaseDigimonID,
					pDeckEntry->s_dwDestDigimonID,
					_BoolTF( pDeckEntry->s_bIsOpen ),
					_BoolTF( pDeckEntry->s_bIsRender ),
					_BoolTF( bUseEvoAtlas ),
					(int)eDeckPortraitState,
					strDeckIconFile.c_str(),
					_BoolTF( bDeckIconFileExists ),
					pDeckEntry->s_ImgFileName.c_str(),
					_BoolTF( bFallbackIconFileExists ) );
				if( !bUseEvoAtlas )
				{
					ENCY_DECK_LOG_WARN( " Icon path did not resolve to _evo atlas. group=%d idx=%d base=%u dest=%u source='%s'",
						pGroupInfo->s_nGroupIdx,
						nListIdx,
						pDeckEntry->s_dwBaseDigimonID,
						pDeckEntry->s_dwDestDigimonID,
						pDeckEntry->s_ImgFileName.c_str() );
				}

				cString::sIMAGE* pImage = pItem->AddImage( IconIMG
					, eDeckPortraitState
					, ptPos + ptDeckIconOffset );
				if( pImage == NULL )
				{
					ENCY_DECK_LOG_WARN( " Icon AddImage failed. group=%d idx=%d base=%u dest=%u useEvo=%s state=%d icon='%s'",
						pGroupInfo->s_nGroupIdx,
						nListIdx,
						pDeckEntry->s_dwBaseDigimonID,
						pDeckEntry->s_dwDestDigimonID,
						_BoolTF( bUseEvoAtlas ),
						(int)eDeckPortraitState,
						strDeckIconFile.c_str() );
				}
				else
				{
					pImage->SetAutoPointerDelete( true );
					ENCY_DECK_LOG_INFO( " Icon AddImage ok. group=%d idx=%d base=%u dest=%u useEvo=%s state=%d icon='%s'",
						pGroupInfo->s_nGroupIdx,
						nListIdx,
						pDeckEntry->s_dwBaseDigimonID,
						pDeckEntry->s_dwDestDigimonID,
						_BoolTF( bUseEvoAtlas ),
						(int)eDeckPortraitState,
						strDeckIconFile.c_str() );
				}

				sICON_INFO* iconInfo = new sICON_INFO();
				cSprite* IconBG = NiNew cSprite;
				IconBG->Init( NULL, CsPoint::ZERO, ptDeckIconFrameSize, pDeckFramePath, false, FONT_WHITE, false );
				IconBG->SetColor( fColor );
				IconBG->SetAlpha( fAlpha );
				iconInfo->s_Sprite = pItem->AddSprite( IconBG, ptPos );
				if( iconInfo->s_Sprite == NULL )
				{
					ENCY_DECK_LOG_WARN( " Frame AddSprite failed. group=%d idx=%d base=%u dest=%u frame='small_deck_frame.png'",
						pGroupInfo->s_nGroupIdx,
						nListIdx,
						pDeckEntry->s_dwBaseDigimonID,
						pDeckEntry->s_dwDestDigimonID );
					delete iconInfo;
					nListIdx++;
					continue;
				}
				iconInfo->s_Sprite->SetAutoPointerDelete( true );
				cImage* pDeckFrameTop = NiNew cImage;
				pDeckFrameTop->Init( NULL, CsPoint::ZERO, ptDeckIconFrameSize, pDeckFramePath, false, false );
				pDeckFrameTop->SetColor( fColor );
				pDeckFrameTop->SetAlpha( fAlpha );


				// Layer alto para garantir que a borda fique acima da textura do Digimon.
// A textura do Digimon pode usar NORMAL2, então a borda também precisa usar NORMAL2.
				const cImage::eSTATE eDeckFrameTopState = cImage::NORMAL2;

				cString::sIMAGE* pDeckFrameTopImage = pItem->AddImage(
					pDeckFrameTop,
					eDeckFrameTopState,
					ptPos
				);

				if (pDeckFrameTopImage != NULL)
				{
					pDeckFrameTopImage->SetAutoPointerDelete(true);
				}
				else
				{
					SAFE_NIDELETE(pDeckFrameTop);
				}

				ENCY_DECK_LOG_INFO( " Frame AddSprite ok. group=%d idx=%d base=%u dest=%u frame='%s' exists=%s",
					pGroupInfo->s_nGroupIdx,
					nListIdx,
					pDeckEntry->s_dwBaseDigimonID,
					pDeckEntry->s_dwDestDigimonID,
					pDeckFramePath,
					_BoolTF( bDeckFrameExists ) );
				iconInfo->s_dwBaseDigimonID = pDeckEntry->s_dwBaseDigimonID;
				iconInfo->s_dwDestDigimonID = pDeckEntry->s_dwDestDigimonID;
				iconInfo->s_count = count;
				iconInfo->s_nGroupIdx = pGroupInfo->s_nGroupIdx;
				m_IconList.push_back( iconInfo );
				nListIdx++;
				nSourceIdx++;
			}

			cText::sTEXTINFO titleInfo;
			titleInfo.Init( &g_pEngine->m_FontSystem );
			titleInfo.s_eFontSize = CFont::FS_14;
			titleInfo.s_eTextAlign = DT_CENTER;
			titleInfo.SetText( pGroupInfo->s_GroupName.c_str() );
			pItem->AddText( &titleInfo, CsPoint( nPanelX + ( nDeckPanelWidth / 2 ), 19 ) );

			const bool bIsThisActiveDeck = pGroupInfo->s_bUse;
			const bool bShouldShowDeckButton = ( bGroupOpenForUI || bIsThisActiveDeck );
			const bool bDeckEquipTextureExists = _DeckFileExists( deckEquipButtonPath );
			ENCY_DECK_LOG_INFO( " Deck button state. groupIdx=%d show=%s openUI=%s isUse=%s tex='%s' exists=%s",
				pGroupInfo->s_nGroupIdx,
				_BoolTF( bShouldShowDeckButton ),
				_BoolTF( bGroupOpenForUI ),
				_BoolTF( bIsThisActiveDeck ),
				deckEquipButtonPath.c_str(),
				_BoolTF( bDeckEquipTextureExists ) );
			if( bShouldShowDeckButton )
			{
				cButton* GetItemBtn = NiNew cButton;
				CsPoint ptDeckBtnSize = CsPoint( 102, 24 );
				const int nDeckBtnAtlasHeight = 243;
				const int nDeckBtnAtlasFrames = 4;
				const int nDeckBtnFrameStepY = nDeckBtnAtlasHeight / nDeckBtnAtlasFrames; // 60
				if( bDeckEquipTextureExists )
				{
					// cButton texture resolver expects interface-relative paths for atlas state switching.
					GetItemBtn->Init( NULL, CsPoint::ZERO, ptDeckBtnSize, "random_box\\button\\purchase_1.tga", false );
					GetItemBtn->SetTexToken( CsPoint( 0, nDeckBtnFrameStepY ) );
				}
				else
				{
					GetItemBtn->Init( NULL, CsPoint::ZERO, ptDeckBtnSize, cButton::IMAGE_ENCYCLOPEDIA_GET, _T( " " ), false );
				}
				GetItemBtn->SetAlpha( ENCY_ENABLE_ALPHA );
				if( g_pCharMng->GetTamerUser()->IsBattle() == true )
					GetItemBtn->SetEnable( false );
				else if( bGroupOpenForUI || bIsThisActiveDeck )
					GetItemBtn->SetEnable( true );
				else
					GetItemBtn->SetEnable( false );

				GetItemBtn->AddEvent( cButton::BUTTON_LBUP_EVENT, this, &cEncyclopedia::PressGroupListBtn );
				GetItemBtn->SetUserData( new sDIGIMON_ID( pGroupInfo->s_nGroupIdx ) );
				const int nBtnPosX = nPanelX + nDeckPanelWidth - ptDeckBtnSize.x - 6;
				const int nBtnPosY = 133;
				pItem->AddButton( GetItemBtn, 0, CsPoint( nBtnPosX, nBtnPosY ), CsPoint::ZERO, ptDeckBtnSize );

				cText::sTEXTINFO deckBtnTextInfo;
				deckBtnTextInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_10 );
				deckBtnTextInfo.s_eTextAlign = DT_CENTER;
				deckBtnTextInfo.s_Color = NiColor::WHITE;
				const bool bDeckEffectActive = bIsThisActiveDeck;
				deckBtnTextInfo.SetText( bDeckEffectActive ? _T( "Disable Effect" ) : _T( "Enable Effect" ) );
				pItem->AddText( &deckBtnTextInfo, CsPoint( nBtnPosX + ( ptDeckBtnSize.x / 2 ), nBtnPosY + 5 ) );
			}

			// Render bookmark as the last panel element for stable layering.
			if( bShowBookmark && _DeckFileExists( deckBookmarkPath ) )
			{
				ENCY_DECK_LOG_INFO( "Bookmark texture check. groupIdx=%d path='%s' exists=%s", pGroupInfo->s_nGroupIdx, deckBookmarkPath.c_str(), _BoolTF( _DeckFileExists( deckBookmarkPath ) ) );
				cImage* pBookmark = NiNew cImage;
				pBookmark->Init( NULL, CsPoint::ZERO, CsPoint( 28, 28 ), deckBookmarkPath.c_str(), false, false );
				pBookmark->SetTexToken( CsPoint( 28, 28 ) );
				pBookmark->SetColor( ENCY_ENABLE_COLOR );
				pBookmark->SetAlpha( ENCY_ENABLE_ALPHA );
				cString::sIMAGE* pBookmarkImage = pItem->AddImage( pBookmark, cImage::NORMAL2, CsPoint( nPanelX + nDeckPanelWidth - 34, nDeckRowPanelY + 7 ) );
				if( pBookmarkImage )
				{
					pBookmarkImage->SetAutoPointerDelete( true );
					ENCY_DECK_LOG_INFO( "Bookmark AddImage ok. groupIdx=%d path='%s' pos=(%d,%d)", pGroupInfo->s_nGroupIdx, deckBookmarkPath.c_str(), nPanelX + nDeckPanelWidth - 34, nDeckRowPanelY + 7 );
					sDECK_BOOKMARK_INFO kBookmarkInfo;
					kBookmarkInfo.s_pItem = addItem;
					kBookmarkInfo.s_LocalRect = CsRect( CsPoint( nPanelX + nDeckPanelWidth - 34, nDeckRowPanelY + 7 ), CsPoint( nPanelX + nDeckPanelWidth - 34 + 28, nDeckRowPanelY + 7 + 28 ) );
					kBookmarkInfo.s_pImage = pBookmarkImage;
					kBookmarkInfo.s_nGroupIdx = pGroupInfo->s_nGroupIdx;
					m_vDeckBookmarkInfo.push_back( kBookmarkInfo );
				}
				else
				{
					ENCY_DECK_LOG_WARN( "Bookmark AddImage failed. groupIdx=%d path='%s'", pGroupInfo->s_nGroupIdx, deckBookmarkPath.c_str() );
				}
			}
			else
			{
				if( !bShowBookmark )
				{
					ENCY_DECK_LOG_INFO( "Bookmark hidden (group not completed). groupIdx=%d", pGroupInfo->s_nGroupIdx );
				}
				else
				{
					ENCY_DECK_LOG_WARN( "Bookmark texture missing. groupIdx=%d path='%s'", pGroupInfo->s_nGroupIdx, deckBookmarkPath.c_str() );
				}
			}

			sDECK_PANEL_HOVER_INFO kDeckHoverInfo;
			kDeckHoverInfo.s_pItem = addItem;
			kDeckHoverInfo.s_LocalRect = CsRect( CsPoint( nPanelX, nDeckRowPanelY ), CsSIZE( nDeckPanelWidth, nDeckPanelHeight ) );
			kDeckHoverInfo.s_nGroupIdx = pGroupInfo->s_nGroupIdx;
			m_vDeckPanelHoverInfo.push_back( kDeckHoverInfo );

			if( nFirstGroupIdxForItem < 0 )
				nFirstGroupIdxForItem = pGroupInfo->s_nGroupIdx;
			nDeckPanelEntryIndex++;
		}

		// Skip fully empty list rows (both deck slots had null group info).
		// These rows showed up as entries without deck_bg.
		if( nFirstGroupIdxForItem < 0 )
		{
			addItem->Delete();
			SAFE_DELETE( addItem );
			SAFE_DELETE( pItem );
			continue;
		}

		addItem->Delete();
		addItem->SetUserData( new sDIGIMON_ID( nFirstGroupIdxForItem, count ) );
		addItem->SetItem( pItem );
		addItem->SetItemHeight( 176 );
		pDeckListBox->AddItemMultiHeight( addItem, 0, false );
		count++;
		m_nDeckLazyGroupCursor = (int)nGroupCursor;
		nRowsAddedThisPass++;
		if( nRowsAddedThisPass >= nRowBudget && m_nDeckLazyGroupCursor < (int)m_vDeckLazyGroups.size() )
		{
			m_bDeckLazyAppendActive = true;
			pDeckListBox->ReconfigureScrollbars();
			if( bLazyAppend == false && m_pDeckScrollBar && m_pDeckScrollBar->IsEnableScroll() )
				m_pDeckScrollBar->SetCurPosIndex( 0 );
			SetTabGroupListDataRenderCount();
			ENCY_PERF_INFO( "SetTabGroupListData lazy partial groups=%d/%d rows=%d listRows=%d elapsedMs=%u",
				m_nDeckLazyGroupCursor,
				(int)m_vDeckLazyGroups.size(),
				nRowsAddedThisPass,
				(int)pDeckListBox->GetItemCount(),
				(unsigned)( GetTickCount() - dwBegin ) );
			return;
		}
	}
	ENCY_DECK_LOG_INFO( "SetTabGroupListData end. rowsAdded=%d", count );

	pDeckListBox->ReconfigureScrollbars();
	if( bLazyAppend == false && m_pDeckScrollBar && m_pDeckScrollBar->IsEnableScroll() )
		m_pDeckScrollBar->SetCurPosIndex( 0 );
	m_bDeckListBuilt = true;
	m_bDeckLazyAppendActive = false;
	m_nDeckLazyGroupCursor = 0;
	m_wsDeckListCacheSearch = m_wsSearchKeyword;

	ENCY_PERF_INFO( "SetTabGroupListData end groups=%d matched=%d rows=%d hover=%d bookmark=%d elapsedMs=%u",
		(int)GetSystem()->GetGroupMapData().size(),
		(int)m_vDeckLazyGroups.size(),
		(int)pDeckListBox->GetItemCount(),
		(int)m_vDeckPanelHoverInfo.size(),
		(int)m_vDeckBookmarkInfo.size(),
		(unsigned)( GetTickCount() - dwBegin ) );

	SetTabGroupListDataRenderCount();
}

void cEncyclopedia::SetTabGroupListDataRenderCount()
{
	cListBox* pDeckListBox = m_pDeckListBox ? m_pDeckListBox : m_pEncyListBox;
	if( pDeckListBox == NULL )
		return;

	// Deck rows are fixed-height list entries (set via AddItemMultiHeight(..., 176)).
	// Scrollbar render count must reflect visible rows, otherwise max scroll is clamped
	// and bottom deck rows can become unreachable.
	const int nDeckRowHeight = 176;
	const int nListHeight = pDeckListBox->GetSize().y;
	const int nVisibleRowsByViewport = ( nDeckRowHeight > 0 ) ? ( nListHeight / nDeckRowHeight ) : 1;
	const int nTotalRows = (int)pDeckListBox->GetItemCount();
	int nRenderRows = nVisibleRowsByViewport;
	if( nRenderRows < 1 )
		nRenderRows = 1;
	if( nTotalRows > 0 && nRenderRows > nTotalRows )
		nRenderRows = nTotalRows;

	pDeckListBox->SetScrollBarRenderCount( nRenderRows );
	pDeckListBox->ReconfigureScrollbars();
	ENCY_DECK_LOG_INFO( "SetTabGroupListDataRenderCount fixed. listHeight=%d rowHeight=%d totalRows=%d renderRows=%d",
		nListHeight, nDeckRowHeight, nTotalRows, nRenderRows );
}

void cEncyclopedia::_SetTooltip( int nDigimonID )
{
	m_bIsMouseOn = true;

	if( m_nTooltipDigimonID == nDigimonID )	//툴팁 표시중인 디지몬과 같은놈이면 ㅌㅌ, 최초 한번만 셋 하기위함
		return;
	else if( nDigimonID != 0 )// 0 : Refresh인 경우
	{
		m_nTooltipDigimonID = nDigimonID;
	}

	m_DelayTime.Start();

	SAFE_POINTER_RET( GetSystem() );
	EncyclopediaContents::sEVOL_INFO* pDigimonData = GetSystem()->GetEncyData(m_nTooltipDigimonID);
	if( pDigimonData == NULL )
	{
		CLIENT_LOG_WARN( "ENCY", "_SetTooltip missing encyclopedia data. digimonId=%d", m_nTooltipDigimonID );
		m_bIsMouseOn = false;
		return;
	}

	//레벨, 스케일
	if( pDigimonData->s_nLevel == 0 )	// 아직 부화 안한 디지몬이라 정보 없음
	{
		m_szLevel->SetText( _T( "-" ) );
		m_szScale->SetText( _T( "-" ) );
	}
	else
	{
		m_szLevel->SetText( pDigimonData->s_nLevel );
		m_szScale->SetText( pDigimonData->s_nSize );
	}

	//강화수치
	m_pEnchantVal.clear();

	int nIdx = 0;
	m_pEnchantVal[  nIdx++ ] = pDigimonData->s_nEnchant_AT;
	m_pEnchantVal[  nIdx++ ] = pDigimonData->s_nEnchant_CT;
	m_pEnchantVal[  nIdx++ ] = pDigimonData->s_nEnchant_EV;
	m_pEnchantVal[  nIdx++ ] = pDigimonData->s_nEnchant_BL;
	m_pEnchantVal[  nIdx ] = pDigimonData->s_nEnchant_HP;

	//높은 두 수치
	int nFirst  = GetSystem()->GetEnchant_Order( 0, m_pEnchantVal[0], m_pEnchantVal[1], m_pEnchantVal[2], m_pEnchantVal[3], m_pEnchantVal[4] );
	int nSecond = GetSystem()->GetEnchant_Order( 1, m_pEnchantVal[0], m_pEnchantVal[1], m_pEnchantVal[2], m_pEnchantVal[3], m_pEnchantVal[4] );

	//가장 높은 강화수치
	std::map <int, int>::iterator it = m_pEnchantVal.find( nFirst );
	if( it == m_pEnchantVal.end() )
	{
		CLIENT_LOG_WARN( "ENCY", "_SetTooltip invalid enchant index(first)=%d for digimonId=%d", nFirst, m_nTooltipDigimonID );
		m_szEnchantVal[ 0 ]->SetText( _T( "-" ) );
	}
	else
	{
		if( it->second == 0 )//강화값이 0일 경우
			m_szEnchantVal[ 0 ]->SetText( _T( "-" ) );
		else
			m_szEnchantVal[ 0 ]->SetText( it->second );
	}

	//두번째 높은 강화수치
	it = m_pEnchantVal.find( nSecond );
	if( it == m_pEnchantVal.end() )
	{
		CLIENT_LOG_WARN( "ENCY", "_SetTooltip invalid enchant index(second)=%d for digimonId=%d", nSecond, m_nTooltipDigimonID );
		m_szEnchantVal[ 1 ]->SetText( _T( "-" ) );
	}
	else
	{
		if( it->second == 0 )//강화값이 0일 경우
			m_szEnchantVal[ 1 ]->SetText( _T( "-" ) );
		else
			m_szEnchantVal[ 1 ]->SetText( it->second );
	}

	//계열체 이름
	SAFE_POINTER_RET( nsCsFileTable::g_pTacticsMng );
	CsTacticsExplain* pTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( m_nTooltipDigimonID );
	if( pTactics == NULL || pTactics->GetInfo() == NULL )
	{
		CLIENT_LOG_WARN( "ENCY", "_SetTooltip missing tactics info. digimonId=%d", m_nTooltipDigimonID );
		m_bIsMouseOn = false;
		return;
	}

	m_szName->SetText( pTactics->GetInfo()->s_szTacticsName );

	cString* pString = NiNew cString;
	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem, CFont::FS_9 );

	//계열체 설명
	m_szExplain->Delete();
	TCHAR szExplain[ 512 ];
	_tcsncpy_s( szExplain, _countof( szExplain ), pTactics->GetInfo()->s_szTacticsExplain, _TRUNCATE );
	g_pStringAnalysis->Cut( m_szExplain, 205, szExplain, &ti );
}

void cEncyclopedia::_SetCardNameTooltip( int nDigimonID, CsRect const& rtCardScreen )
{
	m_bIsMouseOn = false;
	m_bCardNameTooltipVisible = false;
	if( nDigimonID <= 0 )
		return;

	SAFE_POINTER_RET( nsCsFileTable::g_pDigimonMng );
	CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( nDigimonID );
	SAFE_POINTER_RET( pDigimon );
	CsDigimon::sINFO* pInfo = pDigimon->GetInfo();
	SAFE_POINTER_RET( pInfo );

	int const nNameLen = (int)_tcslen( pInfo->s_szName );
	int const nTooltipW = min( 150, max( 72, ( nNameLen * 7 ) + 14 ) );
	int const nTooltipH = 22;
	int const nCardW = rtCardScreen.right - rtCardScreen.left;
	int nX = rtCardScreen.left + ( nCardW / 2 ) - ( nTooltipW / 2 );
	int nY = rtCardScreen.top - nTooltipH - 40;
	if( nY < 0 )
		nY = rtCardScreen.bottom + 6;

	if( nX < 0 )
		nX = 0;
	if( nX + nTooltipW > g_nScreenWidth )
		nX = g_nScreenWidth - nTooltipW;
	if( nX < 0 )
		nX = 0;
	if( nY + nTooltipH > g_nScreenHeight )
		nY = g_nScreenHeight - nTooltipH;
	if( nY < 0 )
		nY = 0;

	if( m_pCardNameTooltipText )
		m_pCardNameTooltipText->SetText( pInfo->s_szName );
	m_ptCardNameTooltipPos = CsPoint( nX, nY );
	m_ptCardNameTooltipSize = CsPoint( nTooltipW, nTooltipH );
	m_bCardNameTooltipVisible = true;
}

void cEncyclopedia::_ClearIconList()
{
	std::list<sICON_INFO*>::iterator it = m_IconList.begin();
	for( ; it != m_IconList.end(); ++it )
	{
		SAFE_DELETE( *it );
	}
	m_IconList.clear();
}

void cEncyclopedia::_Updata_ForMouse_Group( int count )
{
	std::list<sICON_INFO*>::const_iterator it1		 = m_IconList.begin();
	std::list<sICON_INFO*>::const_iterator itEnd1	 = m_IconList.end();

	for( ; it1 != itEnd1 ; ++it1 )
	{
		sICON_INFO* pIcon = (*it1);
		if( pIcon == NULL || pIcon->s_Sprite == NULL || pIcon->s_Sprite->s_pSprite == NULL )
			continue;

		if(count != pIcon->s_count)
		{
			continue;
		}

		bIsTooltip = true;

		if( CURSOR_ST.CheckClickBox( CsRect(pIcon->s_Sprite->s_RenderPos, pIcon->s_Sprite->s_RenderPos + pIcon->s_Sprite->s_pSprite->GetSize()) ) != CURSOR::BUTTON_OUTWINDOW )
		{
			SAFE_POINTER_RET( nsCsFileTable::g_pDigimonMng );
			CsDigimon* pDigimon = nsCsFileTable::g_pDigimonMng->GetDigimon( pIcon->s_dwDestDigimonID );
			SAFE_POINTER_RET( pDigimon );
			CsDigimon::sINFO* pDigimonInfo = pDigimon->GetInfo();
			SAFE_POINTER_RET( pDigimonInfo );
			SAFE_POINTER_RET( nsCsFileTable::g_pTacticsMng );
			std::wstring wsTooltip;
			TCHAR szName[64]={0,};
			//TCHAR szEvoType[64]={0,};

			//계열체 이름
			CsTacticsExplain* pBaseTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( pIcon->s_dwBaseDigimonID );
			if( pBaseTactics == NULL || pBaseTactics->GetInfo() == NULL )
			{
				CLIENT_LOG_WARN( "ENCY", "_Updata_ForMouse_Group missing base tactics. baseId=%u", pIcon->s_dwBaseDigimonID );
				return;
			}
			wsTooltip = pBaseTactics->GetInfo()->s_szTacticsName;

			EncyclopediaContents::MAP_GROUP_CIT itGroup    = GetSystem()->GetGroupMapData().begin();
			EncyclopediaContents::MAP_GROUP_CIT itGroupEnd = GetSystem()->GetGroupMapData().end();

			int count = 0;
			for( ; itGroup != itGroupEnd ; ++itGroup )
			{
				if(pIcon->s_count == count)
				{
					EncyclopediaContents::MAP_DATA_CIT itList	 = itGroup->second->s_pListData.begin();
					EncyclopediaContents::MAP_DATA_CIT itListEnd = itGroup->second->s_pListData.end();

					int nCnt = 0;//두개 이상이면 다음줄로.. 너무 길어지면 안되니까

					for( ; itList != itListEnd ; ++itList)
					{
						if( pIcon->s_dwDestDigimonID == (*itList)->s_dwDestDigimonID	&& pIcon->s_dwBaseDigimonID != (*itList)->s_dwBaseDigimonID	)
						{
							if( nCnt > 2 )
								wsTooltip += L"\n";

							CsTacticsExplain* pChainTactics = nsCsFileTable::g_pTacticsMng->GetTacticsExplain( (*itList)->s_dwBaseDigimonID );
							if( pChainTactics == NULL || pChainTactics->GetInfo() == NULL )
								continue;

							wsTooltip += L" & ";
							wsTooltip += pChainTactics->GetInfo()->s_szTacticsName;
							nCnt++;
						}
					}
					break;
				}
				count++;
			}

			//디지몬 이름
			_tcscpy_s( szName, pDigimonInfo->s_szName );

			nsCsDigimonTable::eEVOLUTION_TYPE eEvoType = nsCsFileTable::g_pDigimonMng->GetDigimon( pIcon->s_dwDestDigimonID )->GetInfo()->s_eEvolutionType;

			std::wstring strEvolType;
			if( nsCsFileTable::g_pBaseMng )
				strEvolType = nsCsFileTable::g_pBaseMng->GetEvoTypeName( (int)eEvoType );

			std::wstring evoName;
			DmCS::StringFn::Format( evoName, L"( %s )", strEvolType.c_str() );

			wsTooltip += L"\n";
			wsTooltip += szName;
			wsTooltip += evoName;

			bIsTooltip = false;
			cTooltip* pTooltip = TOOLTIPMNG_STPTR ? TOOLTIPMNG_STPTR->GetTooltip() : NULL;
			if( pTooltip )
				pTooltip->SetTooltip_Text( CURSOR_ST.GetPos() + CsPoint( 20, 20 ) , CsPoint::ZERO, wsTooltip.c_str() );
			break;
		}
	}
}

void cEncyclopedia::_SetGroupTooltip( int nGroupIdx )
{
	if( nGroupIdx != 0 )
	{
		CsPoint ptCurPos = CURSOR_ST.GetPos() + CsPoint(20,20);
#if ( defined VERSION_USA || defined VERSION_TH )
		u4 nMaxSizeX = 390;
#else
		u4 nMaxSizeX = 240;
#endif
		TOOLTIPMNG_STPTR->GetTooltip()->SetTooltip( ptCurPos, CsPoint( 24, 24 ), nMaxSizeX, cTooltip::DECKLIST, nGroupIdx, 0, 0, 0, NULL );

	}
}

bool cEncyclopedia::_IsDeckFavoriteGroup( int nGroupIdx ) const
{
	return m_setDeckFavoriteGroup.find( nGroupIdx ) != m_setDeckFavoriteGroup.end();
}

std::string cEncyclopedia::_GetDeckFavoriteCachePath() const
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

	return std::string( "Temp\\EncyclopediaDeckFavorites_" ) + strSafeUser + ".cache";
}

void cEncyclopedia::_LoadDeckFavoriteCache()
{
	m_setDeckFavoriteGroup.clear();

	std::string const strPath = _GetDeckFavoriteCachePath();
	FILE* fp = NULL;
	if( fopen_s( &fp, strPath.c_str(), "rt" ) != 0 || fp == NULL )
		return;

	char szLine[ 128 ] = { 0, };
	while( fgets( szLine, _countof( szLine ), fp ) )
	{
		char* pEnd = NULL;
		long const nGroupIdx = strtol( szLine, &pEnd, 10 );
		if( nGroupIdx > 0 )
			m_setDeckFavoriteGroup.insert( (int)nGroupIdx );
	}

	fclose( fp );
}

void cEncyclopedia::_SaveDeckFavoriteCache() const
{
	std::string const strPath = _GetDeckFavoriteCachePath();
	FILE* fp = NULL;
	if( fopen_s( &fp, strPath.c_str(), "wt" ) != 0 || fp == NULL )
		return;

	fprintf( fp, "# Encyclopedia deck favorite group indexes\n" );
	for( std::set<int>::const_iterator it = m_setDeckFavoriteGroup.begin(); it != m_setDeckFavoriteGroup.end(); ++it )
		fprintf( fp, "%d\n", *it );

	fclose( fp );
}

bool cEncyclopedia::_UpdateDeckBookmarkVisuals()
{
	int const eBtnState = CURSOR_ST.GetButtonState();
	bool const bLButtonDown = ( eBtnState == CURSOR::LBUTTON_DOWN );
	bool const bLButtonHold = ( eBtnState == CURSOR::LBUTTON_PRESS );
	bool const bLButtonUp = ( eBtnState == CURSOR::LBUTTON_UP );
	cListBox* pDeckListBox = m_pDeckListBox ? m_pDeckListBox : m_pEncyListBox;
	cListBoxItem const* pOverItem = pDeckListBox ? pDeckListBox->GetMouseOverItem() : NULL;
	cListBoxItem* pOverItemMutable = const_cast<cListBoxItem*>( pOverItem );
	CsPoint ptLocalMouse = CsPoint::ZERO;
	if( pOverItemMutable && pDeckListBox )
		ptLocalMouse = pDeckListBox->GetItemtoMousePos( CURSOR_ST.GetPos(), pOverItemMutable );

	bool bFavoriteChanged = false;
	for( size_t i = 0; i < m_vDeckBookmarkInfo.size(); ++i )
	{
		sDECK_BOOKMARK_INFO& kInfo = m_vDeckBookmarkInfo[ i ];
		if( kInfo.s_pImage == NULL )
			continue;

		bool bFavorite = _IsDeckFavoriteGroup( kInfo.s_nGroupIdx );

		bool bHover = false;
		if( kInfo.s_pItem && kInfo.s_pItem->GetScrollVisible() && pOverItemMutable == kInfo.s_pItem )
			bHover = ( kInfo.s_LocalRect.PtInRect( ptLocalMouse ) == TRUE );

		if( bHover && bLButtonDown )
		{
			if( bFavorite )
				m_setDeckFavoriteGroup.erase( kInfo.s_nGroupIdx );
			else
				m_setDeckFavoriteGroup.insert( kInfo.s_nGroupIdx );
			bFavorite = !bFavorite;
			bFavoriteChanged = true;
			_SaveDeckFavoriteCache();
		}

		// bookmark.png atlas (top->bottom): active, hover, inactive, pressed.
		int nImageState = bFavorite ? cImage::NORMAL0 : cImage::NORMAL2;
		if( bHover )
		{
			if( bLButtonDown || bLButtonHold || bLButtonUp )
				nImageState = cImage::NORMAL3;
			else
				nImageState = cImage::NORMAL1;
		}

		kInfo.s_pImage->SetImangeIndex( nImageState );

		if( bFavoriteChanged )
			break;
	}

	return bFavoriteChanged;
}

void cEncyclopedia::PressCancelButton( void* pSender, void* pData )
{
	Close();
}

void cEncyclopedia::PressStatisticsButton( void* pSender, void* pData )
{
	if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_ENCYCLOPEDIA_STATS ) == false )
		g_pGameIF->GetDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA_STATS );
	else
		g_pGameIF->CloseDynamicIF( cBaseWindow::WT_ENCYCLOPEDIA_STATS );
}

void cEncyclopedia::PressMainTabButton( void* pSender, void* pData )
{
	SAFE_POINTER_RET( pSender );
	cButton* pButton = static_cast<cButton*>(pSender);
	sDIGIMON_ID* pTab = dynamic_cast<sDIGIMON_ID*>(pButton->GetUserData());
	SAFE_POINTER_RET( pTab );

	if( m_nMainTab == pTab->iDigimonId )
		return;

	_SelectMainTab( pTab->iDigimonId, false );
}

void cEncyclopedia::PressSubMenuButton( void* pSender, void* pData )
{
	SAFE_POINTER_RET( pSender );
	cButton* pButton = static_cast<cButton*>(pSender);
	sDIGIMON_ID* pSub = dynamic_cast<sDIGIMON_ID*>(pButton->GetUserData());
	SAFE_POINTER_RET( pSub );

	if( m_nSubMenu == pSub->iDigimonId )
		return;

	m_nSubMenu = pSub->iDigimonId;
	_InvalidateEncyListCache();
	for( int i = 0 ; i < 4 ; ++i )
	{
		if( m_pSubMenuBtn[ i ] )
			m_pSubMenuBtn[ i ]->SetMouseOnMode( i == m_nSubMenu );
	}
	_RefreshSearchUI();
	_SetTabList( 0 );
}

void cEncyclopedia::_RefreshSearchUI()
{
	bool const bVisible = ( ( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA ) &&
		( m_nSubMenu == SUBMENU_SERIES || m_nSubMenu == SUBMENU_ALL || m_nSubMenu == SUBMENU_CLASS ) ) ||
		( m_nMainTab == MAIN_TAB_DECK );
	if( m_pSearchBarBg ) m_pSearchBarBg->SetVisible( bVisible );
	if( m_pSearchBtn ) m_pSearchBtn->SetVisible( bVisible );
	if( m_pSearchResetBtn ) m_pSearchResetBtn->SetVisible( bVisible );
	if( m_pSearchEdit ) m_pSearchEdit->SetVisible( bVisible );
	if( !bVisible && m_pSearchBarBg )
	{
		m_bSearchFocusVisualOn = false;
		if( m_pSearchEdit )
			m_pSearchEdit->SetEmptyMsgText( _T( "Search Digimon..." ), NiColor( 0.6f, 0.65f, 0.85f ) );
	}
}

void cEncyclopedia::PressSearchButton( void* pSender, void* pData )
{
	UNREFERENCED_PARAMETER( pSender );
	UNREFERENCED_PARAMETER( pData );
	if( m_pSearchEdit )
		m_wsSearchKeyword = m_pSearchEdit->GetString();
	if( m_nMainTab == MAIN_TAB_DECK )
		_InvalidateDeckListCache();
	else
		_InvalidateEncyListCache();
	_SetTabList( 0 );
}

void cEncyclopedia::PressSearchResetButton( void* pSender, void* pData )
{
	UNREFERENCED_PARAMETER( pSender );
	UNREFERENCED_PARAMETER( pData );
	m_wsSearchKeyword.clear();
	if( m_pSearchEdit )
		m_pSearchEdit->SetText( _T( "" ), false );
	if( m_nMainTab == MAIN_TAB_DECK )
		_InvalidateDeckListCache();
	else
		_InvalidateEncyListCache();
	_SetTabList( 0 );
}

void cEncyclopedia::PressGroupListBtn( void* pSender, void* pData )
{
	SAFE_POINTER_RET( pSender );
	SAFE_POINTER_RET( GetSystem() );
	cButton* pButton = static_cast<cButton*>(pSender);
	sDIGIMON_ID* pDigimon = dynamic_cast<sDIGIMON_ID*>(pButton->GetUserData());
	SAFE_POINTER_RET( pDigimon );
	ENCY_DECK_LOG_INFO( "PressGroupListBtn click. mainTab=%d groupIdx=%d useDeck=%d",
		m_nMainTab,
		pDigimon->iDigimonId,
		GetSystem()->GetUseDeck() );


	if( m_nMainTab == MAIN_TAB_ENCYCLOPEDIA )
	{
		EncyclopediaContents::MAP_CIT it = GetSystem()->GetEncyMapData().begin();
		EncyclopediaContents::MAP_CIT itEnd = GetSystem()->GetEncyMapData().end();

		for( ; it != itEnd ; it++ )
		{
			if( it->second->s_sInfo[ 1 ].s_nDigimonID == pDigimon->iDigimonId )
			{
				//획득 가능한 상태인지 인벤 검사
				if( g_pDataMng->GetInven()->GetEmptySlotCount() == 0 )	// 빈칸 하나이상은 꼭 있어야함 무슨 아이템을 얻을지 모르니까
				{
					cPrintMsg::PrintMsg( cPrintMsg::GAME_INVEN_LIMIT_COUNT );
				}
				else
				{
					// 아이템 보상을 이미 받은 경우
					if( it->second->s_bIsReward )
						continue;

					//아이템 획득 요청, 메세지 띄울듯?
					//뭐 선택했는지 저장해놔야되네;; 계열체 번호
					m_nSelectDigimonID = it->second->s_sInfo[ 1 ].s_nDigimonID;
					cPrintMsg::PrintMsg( 30670 );
				}
			}
		}
	}
	else
	{
		EncyclopediaContents::MAP_GROUP_CIT itGroup    = GetSystem()->GetGroupMapData().begin();
		EncyclopediaContents::MAP_GROUP_CIT itGroupEnd = GetSystem()->GetGroupMapData().end();

		for( ; itGroup != itGroupEnd ; itGroup++ )
		{
			if(itGroup->second->s_nGroupIdx == pDigimon->iDigimonId)
			{
				m_nSelectDeckIdx = itGroup->second->s_nGroupIdx;
				ENCY_DECK_LOG_INFO( "Deck toggle request. groupIdx=%d isUse=%s open=%s",
					itGroup->second->s_nGroupIdx,
					_BoolTF( itGroup->second->s_bUse ),
					_BoolTF( itGroup->second->s_bGroupOpen ) );

				if( itGroup->second->s_bUse )
					cMessageBox::InsertList( _T("Notice"), _T("Do you want to disable the deck effect?"), 30673, 2 );
				else
					cMessageBox::InsertList( _T("Notice"), _T("Do you want to enable the deck effect?"), 30672, 2 );
				break;
			}
		}
	}
}
