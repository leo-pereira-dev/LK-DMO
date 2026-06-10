#include "stdafx.h"
#include "DungeonEntranceWindow.h"

#include "Npc.h"
#include "../../../LibProj/CsFileTable/nsCsFileTable.h"
#include "../../../LibProj/CsFileTable/nsCsMapTable.h"
#include "../../../LibProj/CsFileTable/NpcType.h"
#include "../../../LibProj/CsFileTable/MapList.h"
#include "../../../LibProj/CsFileTable/MapListMng.h"
#include "../../../LibProj/CsFileTable/MapPortal.h"
#include "../../../LibProj/CsFileTable/MapPortalGroup.h"
#include "../../../LibProj/CsFileTable/MapPortalMng.h"
#include "../../../LibProj/CsFilePack/CsFilePackSystem.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"

namespace
{
	const char* const DUNGEON_LIST_BIN = "Data\\Bin\\Table\\DungeonList.bin";
	const char* const DUNGEON_QUALIFICATION_BIN = "Data\\Bin\\Table\\DungeonQualification.bin";
	const char* const DUNGEON_OBTAINABLE_BIN = "Data\\Bin\\Table\\DungeonObtainable.bin";
	const char* const DUNGEON_REWARD_BIN = "Data\\Bin\\Table\\DungeonReward.bin";
	const char* const DUNGEON_STR_BIN = "Data\\Bin\\Language\\English\\Dungeon_Str.bin";

	template<typename T>
	static bool DungeonReadExact( int nHandle, T& value )
	{
		return _read( nHandle, &value, sizeof( T ) ) == sizeof( T );
	}

	static bool DungeonReadString( int nHandle, std::string& outValue )
	{
		DWORD dwLength = 0;
		if( DungeonReadExact( nHandle, dwLength ) == false )
			return false;
		if( dwLength > 512 )
			return false;

		outValue.clear();
		if( dwLength == 0 )
			return true;

		std::vector<char> vBuffer;
		vBuffer.resize( dwLength + 1, 0 );
		if( _read( nHandle, &vBuffer[0], dwLength ) != (int)dwLength )
			return false;

		outValue.assign( &vBuffer[0], &vBuffer[0] + dwLength );
		return true;
	}

	static std::wstring DungeonFromAnsi( std::string const& value )
	{
#ifdef UNICODE
		if( value.empty() )
			return std::wstring();

		int const nLength = MultiByteToWideChar( CP_ACP, 0, value.c_str(), (int)value.length(), NULL, 0 );
		if( nLength <= 0 )
			return std::wstring( value.begin(), value.end() );

		std::wstring result;
		result.resize( nLength );
		MultiByteToWideChar( CP_ACP, 0, value.c_str(), (int)value.length(), &result[0], nLength );
		return result;
#else
		return value;
#endif
	}

	static bool DungeonReadWideString( int nHandle, std::wstring& outValue )
	{
		DWORD dwByteLength = 0;
		if( DungeonReadExact( nHandle, dwByteLength ) == false )
			return false;
		if( dwByteLength > 8192 || ( dwByteLength % sizeof( wchar_t ) ) != 0 )
			return false;

		outValue.clear();
		if( dwByteLength == 0 )
			return true;

		outValue.resize( dwByteLength / sizeof( wchar_t ) );
		return _read( nHandle, &outValue[0], dwByteLength ) == (int)dwByteLength;
	}

	static std::wstring DungeonFormatId( TCHAR const* pszPrefix, DWORD dwValue )
	{
		TCHAR szBuffer[128] = { 0, };
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%s %u" ), pszPrefix, dwValue );
		return szBuffer;
	}

	static DWORD DungeonFirstValue( std::vector<DWORD> const& values, DWORD dwDefault = 0 )
	{
		return values.empty() ? dwDefault : values[0];
	}

	static TCHAR const* DungeonRequirementName( WORD wType )
	{
		switch( wType )
		{
	case 1:		return _T( "Itens de entrada" );
	case 2:		return _T( "Quest" );
	case 3:		return _T( "Nivel Tamer maximo" );
	case 4:		return _T( "Bits" );
	case 5:		return _T( "Nivel Tamer minimo" );
	case 6:		return _T( "Nivel Digimon maximo" );
	case 7:		return _T( "N\x00EDvel Digimon Mais de" );
	default:	return _T( "Condi\x00E7\x00E3o" );
	}
}

	static char const* DungeonDifficultyTexture( WORD wDifficulty )
	{
		switch( wDifficulty )
		{
		case 1:		return "DungeonUI\\Dungeon_Level_Easy.tga";
		case 3:		return "DungeonUI\\Dungeon_Level_Hard.tga";
		case 2:
		default:	return "DungeonUI\\Dungeon_Level_Normal.tga";
		}
	}

	static bool DungeonInterfaceAssetExists( std::string const& strPath )
	{
		if( strPath.empty() )
			return false;

		if( CsFPS::CsFPSystem::IsExist( strPath.c_str() ) )
			return true;

		std::string strInterfacePath = "data\\interface\\";
		strInterfacePath += strPath;
		return CsFPS::CsFPSystem::IsExist( strInterfacePath.c_str() );
	}

	static bool DungeonResolveDUnitPortalMap( DWORD dwPortalMapId, DWORD& dwListMapId, WORD& wDifficulty )
	{
		switch( dwPortalMapId )
		{
		case 1605:	dwListMapId = 1600; wDifficulty = 1; return true;
		case 1604:	dwListMapId = 1601; wDifficulty = 1; return true;
		case 1606:	dwListMapId = 1602; wDifficulty = 1; return true;
		case 1607:	dwListMapId = 1603; wDifficulty = 1; return true;
		case 1600:	dwListMapId = 1600; wDifficulty = 2; return true;
		case 1601:	dwListMapId = 1601; wDifficulty = 2; return true;
		case 1602:	dwListMapId = 1602; wDifficulty = 2; return true;
		case 1603:	dwListMapId = 1603; wDifficulty = 2; return true;
		case 1610:	dwListMapId = 1600; wDifficulty = 3; return true;
		case 1611:	dwListMapId = 1601; wDifficulty = 3; return true;
		case 1612:	dwListMapId = 1602; wDifficulty = 3; return true;
		case 1613:	dwListMapId = 1603; wDifficulty = 3; return true;
		case 1615:	dwListMapId = 1614; wDifficulty = 1; return true;
		case 1609:	dwListMapId = 1608; wDifficulty = 2; return true;
		case 1616:	dwListMapId = 1616; wDifficulty = 3; return true;
		default:	break;
		}

		return false;
	}

	static bool DungeonListMapMatchesPortalMap( DWORD dwListMapId, WORD wDifficulty, DWORD dwPortalMapId )
	{
		DWORD dwResolvedListMapId = 0;
		WORD wResolvedDifficulty = 0;
		if( DungeonResolveDUnitPortalMap( dwPortalMapId, dwResolvedListMapId, wResolvedDifficulty ) )
			return dwListMapId == dwResolvedListMapId && wDifficulty == wResolvedDifficulty;

		return dwPortalMapId == dwListMapId;
	}

	static void DungeonSetListStrokeState( cSprite* pSprite, int nState )
	{
		if( pSprite == NULL )
			return;

		int const nTexHeight = 69;
		int const nTop = nState * nTexHeight;
		pSprite->ChangeTexture( "DungeonUI\\Dungeon_List_Btn.tga", CsRect( CsPoint( 0, nTop ), CsPoint( 223, nTop + nTexHeight ) ), true );
	}

	static std::wstring DungeonFitListCardText( std::wstring const& wsText, int nMaxWidth )
	{
		if( wsText.empty() || nMaxWidth <= 0 )
			return wsText;

		cText::sTEXTINFO textInfo;
		textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 1.0f, 0.95f, 0.25f ) );
		textInfo.s_bOutLine = true;
		textInfo.SetText( wsText.c_str() );
		if( textInfo.GetTextWidth() <= nMaxWidth )
			return wsText;

		static wchar_t const* const ELLIPSIS = L"...";
		std::wstring wsResult = wsText;
		while( wsResult.empty() == false )
		{
			wsResult.resize( wsResult.length() - 1 );
			std::wstring wsCandidate = wsResult + ELLIPSIS;
			textInfo.SetText( wsCandidate.c_str() );
			if( textInfo.GetTextWidth() <= nMaxWidth )
				return wsCandidate;
		}

		return ELLIPSIS;
	}
}

cDungeonEntranceWindow::cDungeonEntranceWindow()
: m_pMoveButton(NULL)
, m_pCloseButton(NULL)
, m_pEnterButton(NULL)
, m_pCompletionOpenButton(NULL)
, m_pCompletionCloseButton(NULL)
, m_pListScrollBar(NULL)
, m_pRequirementScrollBar(NULL)
, m_pRewardScrollBar(NULL)
, m_pCompletionContentScrollBar(NULL)
, m_pPreviewSprite(NULL)
, m_pPreviewDifficultyBadge(NULL)
, m_pCompletionOpenIcon(NULL)
, m_pCompletionPanelBg(NULL)
, m_pCompletionBasicTabBg(NULL)
, m_pCompletionRewardListHeaderBg(NULL)
, m_pCompletionContentHeaderBg(NULL)
, m_pCompletionExtraTabBg(NULL)
, m_pTitleText(NULL)
, m_pSubTitleText(NULL)
, m_pImagePathText(NULL)
, m_pCompletionTitleText(NULL)
, m_pCompletionBasicTabText(NULL)
, m_pCompletionRewardListHeaderText(NULL)
, m_pCompletionContentHeaderText(NULL)
, m_pCompletionInfoText(NULL)
, m_pCompletionExtraTabText(NULL)
, m_pCompletionEmptyText(NULL)
, m_pPartyLimitIcon(NULL)
, m_pPartyLimitText(NULL)
, m_pStatusText(NULL)
, m_pTarget(NULL)
, m_nSelectedVisible(-1)
, m_nListScroll(0)
, m_nRequirementScroll(0)
, m_nRewardScroll(0)
, m_nCompletionRewardScroll(0)
, m_bLoaded(false)
, m_bCompletionRewardMode(false)
{
	for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
	{
		m_pListButton[i] = NULL;
		m_pListButtonBg[i] = NULL;
		m_pListButtonStroke[i] = NULL;
		m_pListDifficultyBadge[i] = NULL;
		m_pListText[i] = NULL;
	}
	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
		m_pRewardSlot[i] = NULL;
	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
		m_pCompletionRewardSlot[i] = NULL;
}

void cDungeonEntranceWindow::Destroy()
{
	cBaseWindow::Delete();
}

void cDungeonEntranceWindow::DeleteResource()
{
	DeleteScript();
	m_pMoveButton = NULL;
	m_pCloseButton = NULL;
	m_pEnterButton = NULL;
	m_pCompletionOpenButton = NULL;
	m_pCompletionCloseButton = NULL;
	m_pListScrollBar = NULL;
	m_pRequirementScrollBar = NULL;
	m_pRewardScrollBar = NULL;
	m_pCompletionContentScrollBar = NULL;
	m_pPreviewSprite = NULL;
	m_pPreviewDifficultyBadge = NULL;
	m_pCompletionOpenIcon = NULL;
	m_pCompletionPanelBg = NULL;
	m_pCompletionBasicTabBg = NULL;
	m_pCompletionRewardListHeaderBg = NULL;
	m_pCompletionContentHeaderBg = NULL;
	m_pCompletionExtraTabBg = NULL;
	m_pTitleText = NULL;
	m_pSubTitleText = NULL;
	m_pImagePathText = NULL;
	m_pCompletionTitleText = NULL;
	m_pCompletionBasicTabText = NULL;
	m_pCompletionRewardListHeaderText = NULL;
	m_pCompletionContentHeaderText = NULL;
	m_pCompletionInfoText = NULL;
	m_pCompletionExtraTabText = NULL;
	m_pCompletionEmptyText = NULL;
	m_pPartyLimitIcon = NULL;
	m_pPartyLimitText = NULL;
	m_pStatusText = NULL;
	for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
	{
		m_pListButton[i] = NULL;
		m_pListButtonBg[i] = NULL;
		m_pListButtonStroke[i] = NULL;
		m_pListDifficultyBadge[i] = NULL;
		m_pListText[i] = NULL;
	}
	for( int i = 0; i < MAX_REQUIREMENT_TEXT; ++i )
	{
		m_pRequirementText[i] = NULL;
		m_pRequirementSectionMark[i] = NULL;
	}
	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
		m_pRewardSlot[i] = NULL;
	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
		m_pCompletionRewardSlot[i] = NULL;
	m_pTarget = NULL;
	m_vDungeons.clear();
	m_vVisibleDungeons.clear();
	m_nSelectedVisible = -1;
	m_nListScroll = 0;
	m_nRequirementScroll = 0;
	m_nRewardScroll = 0;
	m_nCompletionRewardScroll = 0;
	m_bLoaded = false;
	m_bCompletionRewardMode = false;
}

void cDungeonEntranceWindow::Create( int nValue /* = 0 */ )
{
	if( cBaseWindow::Init() == false )
		return;

	int const nWidth = 900;
	int const nHeight = 715;
	int const nPosX = ( g_nScreenWidth / 2 ) - ( nWidth / 2 );
	int const nPosY = ( g_nScreenHeight / 2 ) - ( nHeight / 2 );

	InitScript( "DungeonUI\\Dungeon_Bg.tga", CsPoint( nPosX, nPosY ), CsPoint( nWidth, nHeight ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	m_pMoveButton = AddButton( CsPoint( 0, 0 ), CsPoint( 835, 80 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
	if( m_pMoveButton )
		m_pMoveButton->SetAlpha( 0.0f );
	m_pCloseButton = AddButton( CsPoint( 862, 36 ), CsPoint( 30, 28 ), CsPoint( 0, 28 ), "CommonUI\\CommonUI_close.tga" );

	AddSprite( CsPoint( 10, 104 ), CsPoint( 264, 598 ), "DungeonUI\\Dungeon_List_Frame.tga" );
	AddSprite( CsPoint( 287, 104 ), CsPoint( 600, 370 ), "DungeonUI\\Dungeon_Image_Frame.tga" );
	AddSprite( CsPoint( 287, 482 ), CsPoint( 342, 219 ), "DungeonUI\\Dungeon_Target_Participation_Frame.tga" );
	AddSprite( CsPoint( 636, 482 ), CsPoint( 250, 142 ), "DungeonUI\\Dungeon_Reward_Frame.tga" );

	cText::sTEXTINFO textInfo;
	AddSprite( CsPoint( 10, 119 ), CsPoint( 217, 44 ), "DungeonUI\\Dungeon_Line.tga" );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 0.70f, 0.95f, 0.82f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Lista de Calabou\x00E7os" ) );
	AddText( &textInfo, CsPoint( 34, 125 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_20, FONT_WHITE );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Dungeon" ) );
	m_pTitleText = AddText( &textInfo, CsPoint( 308, 138 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 0.70f, 0.92f, 1.0f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Selecione uma entrada." ) );
	m_pSubTitleText = AddText( &textInfo, CsPoint( 308, 160 ) );

	m_pPreviewSprite = AddSprite( CsPoint( 298, 118 ), CsPoint( 579, 349 ), "DungeonUI\\DungeonImage\\Dungeon_50.png" );
	if( m_pPreviewSprite )
		m_pPreviewSprite->SetVisible( false );

	m_pPreviewDifficultyBadge = AddSprite( CsPoint( 298, 181 ), CsPoint( 103, 29 ), "DungeonUI\\Dungeon_Level_Normal.tga" );
	if( m_pPreviewDifficultyBadge )
		m_pPreviewDifficultyBadge->SetVisible( false );

	m_pCompletionOpenIcon = AddButton( CsPoint( 832, 124 ), CsPoint( 39, 33 ), CsPoint( 0, 33 ), "DungeonUI\\Dungeon_reward_btn.png" );

	if( m_pCompletionOpenIcon )
		m_pCompletionOpenIcon->SetVisible( false );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_11, NiColor( 0.58f, 0.85f, 1.0f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "" ) );
	m_pImagePathText = AddText( &textInfo, CsPoint( 308, 450 ) );

	m_pCompletionPanelBg = NiNew cSprite;
	m_pCompletionPanelBg->Init( GetRoot(), CsPoint( 298, 118 ), CsPoint( 579, 349 ), NiColorA( 0.02f, 0.02f, 0.13f, 0.94f ), false );
	m_vpChildControls.push_back( m_pCompletionPanelBg );

	m_pCompletionBasicTabBg = NiNew cSprite;
	m_pCompletionBasicTabBg->Init( GetRoot(), CsPoint( 307, 151 ), CsPoint( 108, 28 ), NiColorA( 0.78f, 0.43f, 0.02f, 1.0f ), false );
	m_vpChildControls.push_back( m_pCompletionBasicTabBg );

	m_pCompletionRewardListHeaderBg = NiNew cSprite;
	m_pCompletionRewardListHeaderBg->Init( GetRoot(), CsPoint( 424, 151 ), CsPoint( 222, 28 ), NiColorA( 0.78f, 0.43f, 0.02f, 1.0f ), false );
	m_vpChildControls.push_back( m_pCompletionRewardListHeaderBg );

	m_pCompletionContentHeaderBg = NiNew cSprite;
	m_pCompletionContentHeaderBg->Init( GetRoot(), CsPoint( 659, 151 ), CsPoint( 209, 28 ), NiColorA( 0.78f, 0.43f, 0.02f, 1.0f ), false );
	m_vpChildControls.push_back( m_pCompletionContentHeaderBg );

	m_pCompletionExtraTabBg = NiNew cSprite;
	m_pCompletionExtraTabBg->Init( GetRoot(), CsPoint( 307, 213 ), CsPoint( 108, 28 ), NiColorA( 0.78f, 0.43f, 0.02f, 1.0f ), false );
	m_vpChildControls.push_back( m_pCompletionExtraTabBg );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 0.62f, 0.96f, 1.0f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Recompensa para Calabou\x00E7" "o Conclu\x00ED" "do" ) );
	m_pCompletionTitleText = AddText( &textInfo, CsPoint( 445, 126 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Recom. B\x00E1" "sic." ) );
	m_pCompletionBasicTabText = AddText( &textInfo, CsPoint( 314, 158 ) );
	textInfo.SetText( _T( "Lista de Recompensas" ) );
	m_pCompletionRewardListHeaderText = AddText( &textInfo, CsPoint( 457, 158 ) );
	textInfo.SetText( _T( "Conte\x00FA" "do" ) );
	m_pCompletionContentHeaderText = AddText( &textInfo, CsPoint( 736, 158 ) );
	textInfo.SetText( _T( "Recom. Extra" ) );
	m_pCompletionExtraTabText = AddText( &textInfo, CsPoint( 314, 220 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Verifique as informa\x00E7\x00F5" "es" ) );
	m_pCompletionInfoText = AddText( &textInfo, CsPoint( 308, 187 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 0.62f, 0.92f, 1.0f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "" ) );
	m_pCompletionEmptyText = AddText( &textInfo, CsPoint( 660, 213 ) );

	m_pCompletionOpenButton = AddButton( CsPoint( 832, 124 ), CsPoint( 39, 33 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
	if( m_pCompletionOpenButton )
	{
		m_pCompletionOpenButton->SetAlpha( 0.0f );
		m_pCompletionOpenButton->SetVisible( true );
	}
	m_pCompletionCloseButton = AddButton( CsPoint( 842, 126 ), CsPoint( 30, 28 ), CsPoint( 0, 28 ), "CommonUI\\CommonUI_close.tga" );


	AddSprite( CsPoint( 646, 495 ), CsPoint( 217, 44 ), "DungeonUI\\Dungeon_Line.tga" );
	textInfo.SetText( _T( "Recompensa Monstro" ) );
	AddText( &textInfo, CsPoint( 662, 500 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
	textInfo.s_bOutLine = true;

	for (int i = 0; i < MAX_REQUIREMENT_TEXT; ++i)
	{
		textInfo.SetText(_T(""));

		m_pRequirementSectionLine[i] = AddSprite(
			CsPoint(297, 514 + (i * 21)),
			CsPoint(217, 44),
			"DungeonUI\\Dungeon_Line.tga"
		);

		if (m_pRequirementSectionLine[i])
			m_pRequirementSectionLine[i]->SetVisible(false);

		m_pRequirementSectionMark[i] = NiNew cSprite;
		m_pRequirementSectionMark[i]->Init(
			GetRoot(),
			CsPoint(315, 526 + (i * 21)),
			CsPoint(4, 18),
			NiColorA(0.56f, 0.67f, 1.0f, 1.0f),
			false
		);

		m_vpChildControls.push_back(m_pRequirementSectionMark[i]);

		if (m_pRequirementSectionMark[i])
			m_pRequirementSectionMark[i]->SetVisible(false);

		m_pRequirementText[i] = AddText(
			&textInfo,
			CsPoint(342, 526 + (i * 21))
		);
	}

	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
	{
		m_pRewardSlot[i] = AddSprite( _GetRewardSlotPos( i ), CsPoint( 44, 44 ), "DungeonUI\\Dungeon_Slot.tga" );
		if( m_pRewardSlot[i] )
			m_pRewardSlot[i]->SetVisible( false );
	}

	for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
	{
		int const nRowY = 150 + ( i * 71 );
		m_pListButtonBg[i] = AddSprite( CsPoint( 21, nRowY ), CsPoint( 220, 68 ), "DungeonUI\\DungeonImage\\Dungeon_50.png" );
		if( m_pListButtonBg[i] )
			m_pListButtonBg[i]->SetVisible( false );
		m_pListButtonStroke[i] = AddSprite( CsPoint( 21, nRowY ), CsPoint( 220, 68 ), "DungeonUI\\Dungeon_List_Btn.tga" );
		if( m_pListButtonStroke[i] )
		{
			DungeonSetListStrokeState( m_pListButtonStroke[i], 1 );
			m_pListButtonStroke[i]->SetVisible( false );
		}
		m_pListDifficultyBadge[i] = AddSprite( CsPoint( 24, nRowY + 4 ), CsPoint( 103, 29 ), "DungeonUI\\Dungeon_Level_Normal.tga" );
		if( m_pListDifficultyBadge[i] )
			m_pListDifficultyBadge[i]->SetVisible( false );
		textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 1.0f, 0.95f, 0.25f ) );
		textInfo.s_bOutLine = true;
		textInfo.SetText( _T( "" ) );
		m_pListText[i] = AddText( &textInfo, CsPoint( 76, nRowY + 43 ) );
		if( m_pListText[i] )
			m_pListText[i]->SetVisible( false );
		m_pListButton[i] = AddButton( CsPoint( 21, nRowY ), CsPoint( 220, 68 ), cButton::IMAGE_ALPHA_1, _T( "" ) );
		if( m_pListButton[i] )
		{
			m_pListButton[i]->SetAlpha( 0.0f );
			m_pListButton[i]->SetVisible( false );
		}
	}

	CsPoint const ptDungeonScrollButtonSize( 16, 16 );
	m_pListScrollBar = AddScrollBar( cScrollBar::TYPE_SEALMASTER, CsPoint( 248, 150 ), CsPoint( 16, 540 ), ptDungeonScrollButtonSize, CsRect( CsPoint( 21, 150 ), CsSIZE( 243, 540 ) ), MAX_LIST_VISIBLE );
	m_pRequirementScrollBar = AddScrollBar( cScrollBar::TYPE_SEALMASTER, CsPoint( 609, 522 ), CsPoint( 16, 176 ), ptDungeonScrollButtonSize, CsRect( CsPoint( 298, 522 ), CsSIZE( 327, 176 ) ), MAX_REQUIREMENT_TEXT );
	m_pRewardScrollBar = AddScrollBar( cScrollBar::TYPE_SEALMASTER, CsPoint( 866, 508 ), CsPoint( 16, 116 ), ptDungeonScrollButtonSize, CsRect( CsPoint( 646, 508 ), CsSIZE( 236, 116 ) ), MAX_REWARD_ROWS );
	m_pCompletionContentScrollBar = AddScrollBar( cScrollBar::TYPE_SEALMASTER, CsPoint( 842, 213 ), CsPoint( 16, 222 ), ptDungeonScrollButtonSize, CsRect( CsPoint( 659, 213 ), CsSIZE( 199, 222 ) ), MAX_COMPLETION_REWARD_VISIBLE );
	if( m_pRequirementScrollBar )
		m_pRequirementScrollBar->SetEnableRenderFromEnableScroll( true );
	if( m_pRewardScrollBar )
		m_pRewardScrollBar->SetEnableRenderFromEnableScroll( false );
	if( m_pCompletionContentScrollBar )
		m_pCompletionContentScrollBar->SetEnableRenderFromEnableScroll( true );

	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
	{
		m_pCompletionRewardSlot[i] = AddSprite( _GetCompletionRewardSlotPos( i ), CsPoint( 44, 44 ), "DungeonUI\\Dungeon_Slot.tga" );
		if( m_pCompletionRewardSlot[i] )
			m_pCompletionRewardSlot[i]->SetVisible( false );
	}

	m_pEnterButton = AddButton( CsPoint( 736, 650 ), CsPoint( 150, 50 ), CsPoint( 0, 50 ), "DungeonUI\\Dungeon_Entrance_Btn.tga" );
	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_16, FONT_WHITE );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Entrar" ) );
	AddText( &textInfo, CsPoint( 792, 664 ) );

	textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, NiColor( 0.62f, 0.92f, 1.0f ) );
	textInfo.s_bOutLine = true;
	m_pPartyLimitIcon = AddSprite( CsPoint( 642, 624 ), CsPoint( 54, 54 ), "DungeonUI\\Dungeon_User_Icon.tga" );
	textInfo.SetText( _T( "Para 01P" ) );
	m_pPartyLimitText = AddText( &textInfo, CsPoint( 636, 676 ) );

	textInfo.Init( &g_pEngine->m_FontText, CFont::FS_12, NiColor( 0.62f, 0.92f, 1.0f ) );
	textInfo.s_bOutLine = true;
	textInfo.SetText( _T( "Carregando dados da dungeon..." ) );
	m_pStatusText = AddText( &textInfo, CsPoint( 314, 674 ) );

	_SetCompletionRewardMode( false );
	_LoadDungeonData();
	_FilterByTarget();
	_RefreshList();
	_RefreshDetail();
}

void cDungeonEntranceWindow::SetTarget( CsC_AvObject* pTarget )
{
	m_pTarget = pTarget;
	m_bCompletionRewardMode = false;
	m_nCompletionRewardScroll = 0;
	_FilterByTarget();
	_RefreshList();
	_RefreshDetail();
}

void cDungeonEntranceWindow::Update(float const& fDeltaTime)
{
	UpdateScript(fDeltaTime);
	_UpdateMoveWindow();
}

cBaseWindow::eMU_TYPE cDungeonEntranceWindow::Update_ForMouse()
{
	cBaseWindow::eMU_TYPE muReturn = cBaseWindow::Update_ForMouse();
	if( muReturn == MUT_OUT_WINDOW )
	{
		if( m_pCloseButton ) m_pCloseButton->Update_ForMouse();
		if( m_pMoveButton ) m_pMoveButton->Update_ForMouse();
		if( m_pEnterButton ) m_pEnterButton->Update_ForMouse();
		if( m_pCompletionOpenButton ) m_pCompletionOpenButton->Update_ForMouse();
		if( m_pCompletionCloseButton ) m_pCompletionCloseButton->Update_ForMouse();
		if( m_pListScrollBar ) m_pListScrollBar->Update_ForMouse();
		if( m_pRequirementScrollBar ) m_pRequirementScrollBar->Update_ForMouse();
		if( m_pCompletionContentScrollBar ) m_pCompletionContentScrollBar->Update_ForMouse();
		for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
		{
			if( m_pListButton[i] && m_pListButton[i]->GetVisible() )
				m_pListButton[i]->Update_ForMouse();
			if( m_pListButtonStroke[i] )
			{
				int const nVisibleIndex = m_nListScroll + i;
				m_pListButtonStroke[i]->SetVisible( nVisibleIndex == m_nSelectedVisible );
			}
		}
		return muReturn;
	}

	if( m_pCloseButton )
	{
		switch( m_pCloseButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			Close();
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( m_bCompletionRewardMode )
	{
		if( m_pCompletionCloseButton )
		{
			switch( m_pCompletionCloseButton->Update_ForMouse() )
			{
			case cButton::ACTION_CLICK:
				_SetCompletionRewardMode( false );
			case cButton::ACTION_DOWN:
				return muReturn;
			}
		}

		if( m_pCompletionContentScrollBar )
		{
			int const nOldScroll = m_nCompletionRewardScroll;
			if( m_pCompletionContentScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
			{
				m_nCompletionRewardScroll = m_pCompletionContentScrollBar->GetCurPosIndex();
				if( nOldScroll != m_nCompletionRewardScroll )
					_RefreshCompletionRewardPanel();
				return muReturn;
			}
		}

		CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
		if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL &&
			ptMouse.x >= 659 && ptMouse.x <= 858 && ptMouse.y >= 213 && ptMouse.y <= 435 )
		{
			if( m_pCompletionContentScrollBar && m_pCompletionContentScrollBar->IsEnableScroll() )
			{
				int const nOldScroll = m_nCompletionRewardScroll;
				if( CURSOR_ST.GetWheel() < 0 )
					++m_nCompletionRewardScroll;
				else if( CURSOR_ST.GetWheel() > 0 )
					--m_nCompletionRewardScroll;
				m_nCompletionRewardScroll = max( 0, min( m_pCompletionContentScrollBar->GetMaxPosIndex(), m_nCompletionRewardScroll ) );
				if( nOldScroll != m_nCompletionRewardScroll )
				{
					m_pCompletionContentScrollBar->SetCurPosIndex( m_nCompletionRewardScroll );
					_RefreshCompletionRewardPanel();
				}
			}
			CURSOR_ST.ResetWheel();
			return muReturn;
		}

		if( _SetCompletionRewardToolTip() )
			return MUT_NONE;
	}
	else if( m_pCompletionOpenButton )
	{
		switch( m_pCompletionOpenButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			_SetCompletionRewardMode( true );
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverRequirement() )
	{
		if( m_pRequirementScrollBar && m_pRequirementScrollBar->IsEnableScroll() )
		{
			int const nOldScroll = m_nRequirementScroll;
			if( CURSOR_ST.GetWheel() < 0 )
				++m_nRequirementScroll;
			else if( CURSOR_ST.GetWheel() > 0 )
				--m_nRequirementScroll;

			m_nRequirementScroll = max( 0, min( m_pRequirementScrollBar->GetMaxPosIndex(), m_nRequirementScroll ) );
			if( nOldScroll != m_nRequirementScroll )
			{
				m_pRequirementScrollBar->SetCurPosIndex( m_nRequirementScroll );
				_RefreshDetail();
			}
		}
		CURSOR_ST.ResetWheel();
		return muReturn;
	}


	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverReward() )
	{
		if( m_pRewardScrollBar && m_pRewardScrollBar->IsEnableScroll() )
		{
			int const nOldScroll = m_nRewardScroll;
			if( CURSOR_ST.GetWheel() < 0 )
				++m_nRewardScroll;
			else if( CURSOR_ST.GetWheel() > 0 )
				--m_nRewardScroll;

			m_nRewardScroll = max( 0, min( m_pRewardScrollBar->GetMaxPosIndex(), m_nRewardScroll ) );
			if( nOldScroll != m_nRewardScroll )
			{
				m_pRewardScrollBar->SetCurPosIndex( m_nRewardScroll );
				_RefreshDetail();
			}
		}
		CURSOR_ST.ResetWheel();
		return muReturn;
	}

	if( CURSOR_ST.GetWheel() != INVALIDE_WHEEL && _IsMouseOverList() )
	{
		if( CURSOR_ST.GetWheel() < 0 )
			_ScrollList( 1 );
		else if( CURSOR_ST.GetWheel() > 0 )
			_ScrollList( -1 );
		CURSOR_ST.ResetWheel();
		return muReturn;
	}

	if( m_pListScrollBar )
	{
		int const nOldScroll = m_nListScroll;
		if( m_pListScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
		{
			m_nListScroll = m_pListScrollBar->GetCurPosIndex();
			_ScrollList( 0 );
			if( nOldScroll != m_nListScroll )
				_RefreshList();
			return muReturn;
		}
	}


	if( m_pRewardScrollBar )
	{
		int const nOldScroll = m_nRewardScroll;
		if( m_pRewardScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
		{
			m_nRewardScroll = m_pRewardScrollBar->GetCurPosIndex();
			if( nOldScroll != m_nRewardScroll )
				_RefreshDetail();
			return muReturn;
		}
	}

	if( m_pRequirementScrollBar )
	{
		int const nOldScroll = m_nRequirementScroll;
		if( m_pRequirementScrollBar->Update_ForMouse() != cScrollBar::ACTION_NONE )
		{
			m_nRequirementScroll = m_pRequirementScrollBar->GetCurPosIndex();
			if( nOldScroll != m_nRequirementScroll )
				_RefreshDetail();
			return muReturn;
		}
	}

	for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
	{
		if( m_pListButton[i] == NULL || m_pListButton[i]->GetVisible() == false )
			continue;

		int const nButtonAction = m_pListButton[i]->Update_ForMouse();
		int const nVisibleIndex = m_nListScroll + i;
		if( m_pListButtonStroke[i] )
		{
			bool const bSelected = nVisibleIndex == m_nSelectedVisible;
			bool const bMouseOver = nButtonAction != cButton::ACTION_NONE;
			m_pListButtonStroke[i]->SetVisible( bSelected || bMouseOver );
			DungeonSetListStrokeState( m_pListButtonStroke[i],
				( nButtonAction == cButton::ACTION_DOWN || nButtonAction == cButton::ACTION_PRESS ) ? 2 : 1 );
		}

		switch( nButtonAction )
		{
		case cButton::ACTION_CLICK:
			_SelectVisibleEntry( i );
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( m_pEnterButton )
	{
		switch( m_pEnterButton->Update_ForMouse() )
		{
		case cButton::ACTION_CLICK:
			_EnterSelectedDungeon();
		case cButton::ACTION_DOWN:
			return muReturn;
		}
	}

	if( _SetRewardToolTip() )
		return MUT_NONE;

	_UpdateMoveWindow_ForMouse();
	return muReturn;
}

void cDungeonEntranceWindow::Render()
{
	RenderScript();

	//if( m_pCompletionOpenIcon )
	//	m_pCompletionOpenIcon->Render();

	if( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() )
		return;

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
	{
		int const nRewardIndex = ( m_nRewardScroll * MAX_REWARD_COLUMNS ) + i;
		if( nRewardIndex < 0 || nRewardIndex >= (int)entry.s_vMonsterRewards.size() )
			continue;

		sReward const& reward = entry.s_vMonsterRewards[nRewardIndex];
		if( reward.s_dwItemId == 0 )
			continue;

		CsPoint const ptSlot = _GetRewardSlotPos( i );
		int const nRenderCount = reward.s_dwAmount == 0 ? 1 : (int)reward.s_dwAmount;
		g_pIconMng->RenderItem( reward.s_dwItemId, GetRootClient() + ptSlot + CsPoint( 6, 6 ), nRenderCount );
	}

	if( m_bCompletionRewardMode )
	{
		std::vector<sReward> const& vCompletionRewards = entry.s_vCompletionRewards.empty() ? entry.s_vMonsterRewards : entry.s_vCompletionRewards;
		for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
		{
			int const nRewardIndex = m_nCompletionRewardScroll + i;
			if( nRewardIndex < 0 || nRewardIndex >= (int)vCompletionRewards.size() )
				continue;

			sReward const& reward = vCompletionRewards[nRewardIndex];
			if( reward.s_dwItemId == 0 )
				continue;

			CsPoint const ptSlot = _GetCompletionRewardSlotPos( i );
			int const nRenderCount = reward.s_dwAmount == 0 ? 1 : (int)reward.s_dwAmount;
			g_pIconMng->RenderItem( reward.s_dwItemId, GetRootClient() + ptSlot + CsPoint( 6, 6 ), nRenderCount );
		}
	}
}

void cDungeonEntranceWindow::ResetDevice()
{
	ResetDeviceScript();
}

void cDungeonEntranceWindow::_SetStatus( TCHAR const* szText )
{
	if( m_pStatusText )
		m_pStatusText->SetText( szText );
}

void cDungeonEntranceWindow::_LoadDungeonData()
{
	if( m_bLoaded )
		return;

	m_vDungeons.clear();
	bool const bPortal = _LoadPortalDungeonData();
	bool const bList = _LoadDungeonListBin();
	bool const bString = _LoadDungeonStringBin();
	bool const bQualification = _LoadDungeonQualificationBin();
	bool const bObtainable = _LoadDungeonObtainableBin();
	bool const bReward = _LoadDungeonRewardBin();
	m_bLoaded = bList || bPortal;

	nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI load list=%d string=%d qualification=%d obtainable=%d reward=%d portal=%d entries=%u",
		bList ? 1 : 0, bString ? 1 : 0, bQualification ? 1 : 0, bObtainable ? 1 : 0, bReward ? 1 : 0, bPortal ? 1 : 0, (unsigned)m_vDungeons.size() );

	if( m_bLoaded == false )
		_SetStatus( _T( "Nao foi possivel carregar os dados da dungeon." ) );
	else if( m_vDungeons.empty() )
		_SetStatus( _T( "Nenhuma dungeon encontrada." ) );
	else
		_SetStatus( _T( "Selecione uma dungeon e pressione Enter." ) );
}

bool cDungeonEntranceWindow::_LoadPortalDungeonData()
{
	if( nsCsMapTable::g_pMapPortalMng == NULL )
		return false;

	CsMapPortalGroup::MAP* pGroupMap = nsCsMapTable::g_pMapPortalMng->GetGroupMap();
	if( pGroupMap == NULL )
		return false;

	CsMapPortalGroup::MAP_IT itGroup = pGroupMap->begin();
	CsMapPortalGroup::MAP_IT itGroupEnd = pGroupMap->end();
	for( ; itGroup != itGroupEnd; ++itGroup )
	{
		if( itGroup->second == NULL )
			continue;

		CsMapPortal::MAP* pPortalMap = itGroup->second->GetPortalMap();
		if( pPortalMap == NULL )
			continue;

		CsMapPortal::MAP_IT itPortal = pPortalMap->begin();
		CsMapPortal::MAP_IT itPortalEnd = pPortalMap->end();
		for( ; itPortal != itPortalEnd; ++itPortal )
		{
			if( itPortal->second == NULL || itPortal->second->GetInfo() == NULL )
				continue;

			CsMapPortal::sINFO* pInfo = itPortal->second->GetInfo();
			if( pInfo->s_dwPortalType != CsMapPortal::DG )
				continue;

			sDungeonEntry entry;
			entry.s_dwDungeonId = pInfo->s_dwDestMapID;
			entry.s_dwPortalId = pInfo->s_dwPortalID;
			entry.s_dwSourceMapId = pInfo->s_dwSrcMapID;
			entry.s_dwDestinationMapId = pInfo->s_dwDestMapID;
			entry.s_dwNpcObjectId = pInfo->s_dwUniqObjectID;
			entry.s_nPortalKindIndex = pInfo->s_nPortalKindIndex;
			entry.s_wDifficulty = (WORD)pInfo->s_dwPortalType;
			entry.s_wSortGroup = (WORD)max( 0, pInfo->s_nPortalKindIndex );
			entry.s_wExtra = 0;
			entry.s_wMinPlayers = 0;
			entry.s_wMaxPlayers = 0;
			entry.s_strImagePath.clear();
			entry.s_wsDescription.clear();

			if( nsCsMapTable::g_pMapListMng && nsCsMapTable::g_pMapListMng->IsList( pInfo->s_dwDestMapID ) )
			{
				CsMapList* pMapList = nsCsMapTable::g_pMapListMng->GetList( pInfo->s_dwDestMapID );
				if( pMapList && pMapList->GetInfo() )
				{
					CsMapList::sINFO* pMapInfo = pMapList->GetInfo();
					if( pMapInfo->s_szMapDiscript_Eng.empty() == false )
						entry.s_wsName = pMapInfo->s_szMapDiscript_Eng;
					else if( pMapInfo->s_szMapDiscript.empty() == false )
						entry.s_wsName = pMapInfo->s_szMapDiscript;
					else
						entry.s_wsName = DungeonFromAnsi( pMapInfo->s_cMapName );
				}
			}

			if( entry.s_wsName.empty() )
				entry.s_wsName = _BuildDungeonDisplayName( entry );

			TCHAR szDesc[256] = { 0, };
			_stprintf_s( szDesc, _countof( szDesc ), _T( "Portal %u -> map %u" ), entry.s_dwPortalId, entry.s_dwDestinationMapId );
			entry.s_wsDescription = szDesc;
			m_vDungeons.push_back( entry );
		}
	}

	std::sort( m_vDungeons.begin(), m_vDungeons.end(), []( sDungeonEntry const& left, sDungeonEntry const& right )
	{
		if( left.s_dwSourceMapId != right.s_dwSourceMapId )
			return left.s_dwSourceMapId < right.s_dwSourceMapId;
		if( left.s_dwNpcObjectId != right.s_dwNpcObjectId )
			return left.s_dwNpcObjectId < right.s_dwNpcObjectId;
		if( left.s_nPortalKindIndex != right.s_nPortalKindIndex )
			return left.s_nPortalKindIndex < right.s_nPortalKindIndex;
		return left.s_dwPortalId < right.s_dwPortalId;
	} );

	return true;
}

bool cDungeonEntranceWindow::_LoadDungeonListBin()
{
	if( !CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE ) )
		return false;

	int nHandle = CsFPS::CsFPSystem::GetFileHandle( FT_PACKHANDLE, DUNGEON_LIST_BIN );
	bool bResult = false;
	if( nHandle >= 0 )
	{
		DWORD dwCount = 0;
		if( DungeonReadExact( nHandle, dwCount ) && dwCount <= 1000 )
		{
			bResult = true;
			for( DWORD i = 0; i < dwCount; ++i )
			{
				sDungeonEntry entry;
				entry.s_dwDungeonId = 0;
				entry.s_dwPortalId = 0;
				entry.s_wDifficulty = 0;
				entry.s_wSortGroup = 0;
				entry.s_wExtra = 0;
				entry.s_wMinPlayers = 0;
				entry.s_wMaxPlayers = 0;
				entry.s_wsDescription.clear();
				if( DungeonReadExact( nHandle, entry.s_dwDungeonId ) == false ||
					DungeonReadExact( nHandle, entry.s_wDifficulty ) == false ||
					DungeonReadExact( nHandle, entry.s_wSortGroup ) == false ||
					DungeonReadString( nHandle, entry.s_strImagePath ) == false ||
					DungeonReadExact( nHandle, entry.s_dwPortalId ) == false ||
					DungeonReadExact( nHandle, entry.s_wExtra ) == false )
				{
					bResult = false;
					break;
				}

				entry.s_wsName = _BuildDungeonDisplayName( entry );
				if( entry.s_dwPortalId == 0 )
					entry.s_dwPortalId = entry.s_dwDungeonId;

				bool bAppliedToPortalEntry = false;
				if( entry.s_dwDungeonId >= 100 )
				{
					DWORD const dwMapId = entry.s_dwDungeonId / 100;
					for( int nEntry = 0; nEntry < (int)m_vDungeons.size(); ++nEntry )
					{
						if( DungeonListMapMatchesPortalMap( dwMapId, entry.s_wDifficulty, m_vDungeons[nEntry].s_dwDestinationMapId ) == false )
							continue;

						m_vDungeons[nEntry].s_dwDungeonId = entry.s_dwDungeonId;
						if( m_vDungeons[nEntry].s_dwPortalId == 0 && entry.s_dwPortalId != 0 )
							m_vDungeons[nEntry].s_dwPortalId = entry.s_dwPortalId;
						m_vDungeons[nEntry].s_wDifficulty = entry.s_wDifficulty;
						m_vDungeons[nEntry].s_wSortGroup = entry.s_wSortGroup;
						m_vDungeons[nEntry].s_wExtra = entry.s_wExtra;
						if( entry.s_strImagePath.empty() == false )
							m_vDungeons[nEntry].s_strImagePath = entry.s_strImagePath;
						bAppliedToPortalEntry = true;
					}
				}

				if( bAppliedToPortalEntry )
					continue;

				m_vDungeons.push_back( entry );
			}
		}
	}

	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );
	return bResult;
}

bool cDungeonEntranceWindow::_LoadDungeonStringBin()
{
	if( !CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE ) )
		return false;

	int nHandle = CsFPS::CsFPSystem::GetFileHandle( FT_PACKHANDLE, DUNGEON_STR_BIN );
	bool bResult = false;
	if( nHandle >= 0 )
	{
		DWORD dwCount = 0;
		if( DungeonReadExact( nHandle, dwCount ) && dwCount <= 10000 )
		{
			bResult = true;
			for( DWORD i = 0; i < dwCount; ++i )
			{
				DWORD dwDungeonId = 0;
				std::wstring wsName;
				std::wstring wsDescription;
				if( DungeonReadExact( nHandle, dwDungeonId ) == false ||
					DungeonReadWideString( nHandle, wsName ) == false ||
					DungeonReadWideString( nHandle, wsDescription ) == false )
				{
					bResult = false;
					break;
				}

				bool bAppliedToDungeonEntry = false;
				for( int nEntry = 0; nEntry < (int)m_vDungeons.size(); ++nEntry )
				{
					if( m_vDungeons[nEntry].s_dwDungeonId != dwDungeonId )
						continue;

					if( wsName.empty() == false )
						m_vDungeons[nEntry].s_wsName = wsName;
					m_vDungeons[nEntry].s_wsDescription = wsDescription;
					bAppliedToDungeonEntry = true;
				}

				if( bAppliedToDungeonEntry )
					continue;

				if( wsName.empty() == false )
				{
					bool bAppliedToMapEntry = false;
					if( dwDungeonId >= 100 )
					{
						DWORD const dwMapId = dwDungeonId / 100;
						for( int nMapEntry = 0; nMapEntry < (int)m_vDungeons.size(); ++nMapEntry )
						{
							if( m_vDungeons[nMapEntry].s_dwDestinationMapId != dwMapId )
								continue;

							m_vDungeons[nMapEntry].s_dwDungeonId = dwDungeonId;
							m_vDungeons[nMapEntry].s_wsName = wsName;
							m_vDungeons[nMapEntry].s_wsDescription = wsDescription;
							bAppliedToMapEntry = true;
						}
					}

					if( bAppliedToMapEntry )
						continue;
				}
			}
		}
	}

	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );
	return bResult;
}

bool cDungeonEntranceWindow::_LoadDungeonQualificationBin()
{
	if( !CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE ) )
		return false;

	int nHandle = CsFPS::CsFPSystem::GetFileHandle( FT_PACKHANDLE, DUNGEON_QUALIFICATION_BIN );
	bool bResult = false;
	if( nHandle >= 0 )
	{
		DWORD dwCount = 0;
		if( DungeonReadExact( nHandle, dwCount ) && dwCount <= 2000 )
		{
			bResult = true;
			for( DWORD i = 0; i < dwCount; ++i )
			{
				DWORD dwDungeonId = 0;
				DWORD dwFieldA = 0;
				DWORD dwFieldB = 0;
				WORD wFieldC = 0;
				WORD wFieldD = 0;
				DWORD dwRequirementCount = 0;
				if( DungeonReadExact( nHandle, dwDungeonId ) == false ||
					DungeonReadExact( nHandle, dwFieldA ) == false ||
					DungeonReadExact( nHandle, dwFieldB ) == false ||
					DungeonReadExact( nHandle, wFieldC ) == false ||
					DungeonReadExact( nHandle, wFieldD ) == false ||
					DungeonReadExact( nHandle, dwRequirementCount ) == false ||
					dwRequirementCount > 32 )
				{
					bResult = false;
					break;
				}

				int const nEntry = _FindEntryByDungeonId( dwDungeonId );
				if( nEntry >= 0 )
				{
					m_vDungeons[nEntry].s_wMinPlayers = wFieldC;
					m_vDungeons[nEntry].s_wMaxPlayers = wFieldD;
				}
				for( DWORD r = 0; r < dwRequirementCount; ++r )
				{
					sRequirement req;
					DWORD dwValueCount = 0;
					if( DungeonReadExact( nHandle, req.s_wType ) == false ||
						DungeonReadExact( nHandle, dwValueCount ) == false ||
						dwValueCount > 16 )
					{
						bResult = false;
						break;
					}

					for( DWORD v = 0; v < dwValueCount; ++v )
					{
						DWORD dwValue = 0;
						if( DungeonReadExact( nHandle, dwValue ) == false )
						{
							bResult = false;
							break;
						}
						req.s_vValues.push_back( dwValue );
					}

					if( bResult == false )
						break;
					if( nEntry >= 0 )
						m_vDungeons[nEntry].s_vRequirements.push_back( req );
				}

				if( bResult == false )
					break;
			}
		}
	}

	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );
	return bResult;
}

bool cDungeonEntranceWindow::_LoadDungeonObtainableBin()
{
	if( !CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE ) )
		return false;

	int nHandle = CsFPS::CsFPSystem::GetFileHandle( FT_PACKHANDLE, DUNGEON_OBTAINABLE_BIN );
	bool bResult = false;
	if( nHandle >= 0 )
	{
		DWORD dwCount = 0;
		if( DungeonReadExact( nHandle, dwCount ) && dwCount <= 2000 )
		{
			bResult = true;
			for( DWORD i = 0; i < dwCount; ++i )
			{
				DWORD dwDungeonId = 0;
				DWORD dwItemCount = 0;
				if( DungeonReadExact( nHandle, dwDungeonId ) == false ||
					DungeonReadExact( nHandle, dwItemCount ) == false ||
					dwItemCount > 128 )
				{
					bResult = false;
					break;
				}

				int const nEntry = _FindEntryByDungeonId( dwDungeonId );
				bool const bUseAsMonsterReward = nEntry >= 0 && m_vDungeons[nEntry].s_vMonsterRewards.empty();
				for( DWORD r = 0; r < dwItemCount; ++r )
				{
					DWORD dwItemId = 0;
					if( DungeonReadExact( nHandle, dwItemId ) == false )
					{
						bResult = false;
						break;
					}

					if( bUseAsMonsterReward && dwItemId != 0 )
					{
						sReward reward;
						reward.s_byGroup = 0;
						reward.s_dwItemId = dwItemId;
						reward.s_dwAmount = 1;
						m_vDungeons[nEntry].s_vMonsterRewards.push_back( reward );
					}
				}

				if( bResult == false )
					break;
			}
		}
	}

	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );
	return bResult;
}

bool cDungeonEntranceWindow::_LoadDungeonRewardBin()
{
	if( !CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE ) )
		return false;

	int nHandle = CsFPS::CsFPSystem::GetFileHandle( FT_PACKHANDLE, DUNGEON_REWARD_BIN );
	bool bResult = false;
	if( nHandle >= 0 )
	{
		DWORD dwCount = 0;
		if( DungeonReadExact( nHandle, dwCount ) && dwCount <= 2000 )
		{
			bResult = true;
			for( DWORD i = 0; i < dwCount; ++i )
			{
				DWORD dwDungeonId = 0;
				DWORD dwGroupCount = 0;
				if( DungeonReadExact( nHandle, dwDungeonId ) == false ||
					DungeonReadExact( nHandle, dwGroupCount ) == false ||
					dwGroupCount > 32 )
				{
					bResult = false;
					break;
				}

				int const nEntry = _FindEntryByDungeonId( dwDungeonId );
				bool const bUseAsCompletionReward = nEntry >= 0 && m_vDungeons[nEntry].s_vCompletionRewards.empty();
				for( DWORD g = 0; g < dwGroupCount; ++g )
				{
					BYTE byGroup = 0;
					DWORD dwRewardCount = 0;
					if( DungeonReadExact( nHandle, byGroup ) == false ||
						DungeonReadExact( nHandle, dwRewardCount ) == false ||
						dwRewardCount > 64 )
					{
						bResult = false;
						break;
					}

					for( DWORD r = 0; r < dwRewardCount; ++r )
					{
						sReward reward;
						reward.s_byGroup = byGroup;
						if( DungeonReadExact( nHandle, reward.s_dwItemId ) == false ||
							DungeonReadExact( nHandle, reward.s_dwAmount ) == false )
						{
							bResult = false;
							break;
						}

						if( bUseAsCompletionReward && reward.s_dwItemId != 0 )
							m_vDungeons[nEntry].s_vCompletionRewards.push_back( reward );
					}

					if( bResult == false )
						break;
				}

				if( bResult == false )
					break;
			}
		}
	}

	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );
	return bResult;
}

void cDungeonEntranceWindow::_FilterByTarget()
{
	_LoadDungeonData();
	m_vVisibleDungeons.clear();

	std::vector<DWORD> vAllowedIds;
	DWORD dwTargetNpcId = 0;
	DWORD dwTargetMapId = 0;
	bool bStrictNpcPortal = false;
	if( m_pTarget && m_pTarget->GetClass() == nClass::Npc )
	{
		CNpc* pNpcObject = (CNpc*)m_pTarget;
		CsNpc* pNpc = pNpcObject ? pNpcObject->GetFTNpc() : NULL;
		if( pNpc && pNpc->GetInfo() && pNpc->GetInfo()->s_eType == nsCsNpcTable::NT_GDMO_PORTAL_TEST_NPC )
		{
			bStrictNpcPortal = true;
			dwTargetNpcId = pNpc->GetInfo()->s_dwNpcID;
			dwTargetMapId = pNpc->GetInfo()->s_dwMapID;
			sNPC_TYPE_TRADE* pType = (sNPC_TYPE_TRADE*)pNpc->GetType();
			if( pType )
			{
				for( int i = 0; i < pType->GetItemCount(); ++i )
				{
					DWORD const dwId = pType->GetItemID( i );
					if( dwId != 0 )
						vAllowedIds.push_back( dwId );
				}
			}

			nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI target npcId=%u mapId=%u allowedCount=%u",
				dwTargetNpcId, dwTargetMapId, (unsigned)vAllowedIds.size() );
		}
	}

	std::vector<DWORD> vSeenDungeonIds;
	for( int i = 0; i < (int)m_vDungeons.size(); ++i )
	{
		sDungeonEntry const& entry = m_vDungeons[i];
		if( bStrictNpcPortal )
		{
			if( entry.s_dwSourceMapId != dwTargetMapId || entry.s_dwNpcObjectId != dwTargetNpcId )
				continue;
		}

		if( entry.s_dwDungeonId != 0 &&
			std::find( vSeenDungeonIds.begin(), vSeenDungeonIds.end(), entry.s_dwDungeonId ) != vSeenDungeonIds.end() )
		{
			continue;
		}

		if( vAllowedIds.empty() )
		{
			m_vVisibleDungeons.push_back( i );
			if( entry.s_dwDungeonId != 0 )
				vSeenDungeonIds.push_back( entry.s_dwDungeonId );
			continue;
		}

		for( int a = 0; a < (int)vAllowedIds.size(); ++a )
		{
			if( vAllowedIds[a] == entry.s_dwDungeonId || vAllowedIds[a] == entry.s_dwPortalId || vAllowedIds[a] == entry.s_dwDestinationMapId )
			{
				m_vVisibleDungeons.push_back( i );
				if( entry.s_dwDungeonId != 0 )
					vSeenDungeonIds.push_back( entry.s_dwDungeonId );
				break;
			}
		}
	}

	if( m_vVisibleDungeons.empty() && bStrictNpcPortal )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI no linked MapPortal dungeon entries for npcId=%u mapId=%u payloadCount=%u",
			dwTargetNpcId, dwTargetMapId, (unsigned)vAllowedIds.size() );
		_SetStatus( _T( "Nenhum portal de dungeon vinculado a este NPC." ) );
	}

	m_nListScroll = 0;
	m_nRequirementScroll = 0;
	m_nRewardScroll = 0;
	m_nSelectedVisible = m_vVisibleDungeons.empty() ? -1 : 0;
}

void cDungeonEntranceWindow::_RefreshList()
{
	int const nMaxScroll = max( 0, (int)m_vVisibleDungeons.size() - MAX_LIST_VISIBLE );
	if( m_nListScroll < 0 )
		m_nListScroll = 0;
	if( m_nListScroll > nMaxScroll )
		m_nListScroll = nMaxScroll;

	if( m_pListScrollBar )
	{
		m_pListScrollBar->SetRange( CsPoint( 0, (int)m_vVisibleDungeons.size() ) );
		m_pListScrollBar->SetCurPosIndex( m_nListScroll );
		m_pListScrollBar->SetVisible( nMaxScroll > 0 );
	}

	for( int i = 0; i < MAX_LIST_VISIBLE; ++i )
	{
		if( m_pListButton[i] == NULL )
			continue;

		int const nVisibleIndex = m_nListScroll + i;
		if( nVisibleIndex >= 0 && nVisibleIndex < (int)m_vVisibleDungeons.size() )
		{
			sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[nVisibleIndex]];
			std::string strImagePath = entry.s_strImagePath;
			if( DungeonInterfaceAssetExists( strImagePath ) == false )
			{
				char szCandidate[MAX_PATH] = { 0, };
				if( entry.s_dwDestinationMapId != 0 )
				{
					sprintf_s( szCandidate, _countof( szCandidate ), "DungeonUI\\DungeonImage\\Dungeon_%u.png", entry.s_dwDestinationMapId );
					if( DungeonInterfaceAssetExists( szCandidate ) )
						strImagePath = szCandidate;
				}
				if( DungeonInterfaceAssetExists( strImagePath ) == false && entry.s_dwDungeonId >= 100 )
				{
					sprintf_s( szCandidate, _countof( szCandidate ), "DungeonUI\\DungeonImage\\Dungeon_%u.png", entry.s_dwDungeonId / 100 );
					if( DungeonInterfaceAssetExists( szCandidate ) )
						strImagePath = szCandidate;
				}
			}

			if( m_pListButtonBg[i] )
			{
				if( DungeonInterfaceAssetExists( strImagePath ) )
				{
					m_pListButtonBg[i]->ChangeTexture( strImagePath.c_str(), true );
					m_pListButtonBg[i]->SetVisible( true );
				}
				else
				{
					m_pListButtonBg[i]->SetVisible( false );
					if( entry.s_strImagePath.empty() == false )
					{
						nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI list image missing dungeonId=%u portalId=%u image=%s",
							entry.s_dwDungeonId, entry.s_dwPortalId, entry.s_strImagePath.c_str() );
					}
				}
			}
			if( m_pListButtonStroke[i] )
			{
				DungeonSetListStrokeState( m_pListButtonStroke[i], 1 );
				m_pListButtonStroke[i]->SetVisible( nVisibleIndex == m_nSelectedVisible );
			}
			if( m_pListDifficultyBadge[i] )
			{
				m_pListDifficultyBadge[i]->ChangeTexture( DungeonDifficultyTexture( entry.s_wDifficulty ), true );
				m_pListDifficultyBadge[i]->SetVisible( true );
			}
			if( m_pListText[i] )
			{
				std::wstring const wsName = DungeonFitListCardText( entry.s_wsName, 160 );
				m_pListText[i]->SetText( wsName.c_str() );
				m_pListText[i]->SetVisible( true );
			}
			m_pListButton[i]->SetText( _T( "" ) );
			m_pListButton[i]->SetAlpha( 0.0f );
			m_pListButton[i]->SetVisible( true );
		}
		else
		{
			if( m_pListButtonBg[i] )
				m_pListButtonBg[i]->SetVisible( false );
			if( m_pListButtonStroke[i] )
				m_pListButtonStroke[i]->SetVisible( false );
			if( m_pListDifficultyBadge[i] )
				m_pListDifficultyBadge[i]->SetVisible( false );
			if( m_pListText[i] )
			{
				m_pListText[i]->SetText( _T( "" ) );
				m_pListText[i]->SetVisible( false );
			}
			m_pListButton[i]->SetText( _T( "" ) );
			m_pListButton[i]->SetVisible( false );
		}
	}
}

void cDungeonEntranceWindow::_RefreshDetail()
{
	for (int i = 0; i < MAX_REQUIREMENT_TEXT; ++i)
	{
		if (m_pRequirementText[i])
		{
			m_pRequirementText[i]->SetText(_T(""));
			m_pRequirementText[i]->SetColor(FONT_WHITE);
			m_pRequirementText[i]->SetPos(CsPoint(342, 505 + (i * 21)));
		}

		if (m_pRequirementSectionLine[i])
			m_pRequirementSectionLine[i]->SetVisible(false);

		if (m_pRequirementSectionMark[i])
			m_pRequirementSectionMark[i]->SetVisible(false);
	}

	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
		if( m_pRewardSlot[i] ) m_pRewardSlot[i]->SetVisible( false );
	if( m_pPartyLimitIcon )
		m_pPartyLimitIcon->SetVisible( false );
	if( m_pPartyLimitText )
		m_pPartyLimitText->SetText( _T( "" ) );
	if( m_pRewardScrollBar )
	{
		m_pRewardScrollBar->SetRange( CsPoint( 0, MAX_REWARD_ROWS ) );
		m_pRewardScrollBar->SetVisible( false );
	}
	m_nRewardScroll = 0;

	if( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() )
	{
		if( m_pTitleText ) m_pTitleText->SetText( _T( "Dungeon" ) );
		if( m_pSubTitleText ) m_pSubTitleText->SetText( _T( "Nenhuma dungeon selecionada." ) );
		if( m_pImagePathText ) m_pImagePathText->SetText( _T( "" ) );
		if( m_pPreviewSprite ) m_pPreviewSprite->SetVisible( false );
		if( m_pPreviewDifficultyBadge ) m_pPreviewDifficultyBadge->SetVisible( false );
		return;
	}

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	if( m_pRewardScrollBar )
	{
		int const nRewardLineCount = max( MAX_REWARD_ROWS, ( (int)entry.s_vMonsterRewards.size() + MAX_REWARD_COLUMNS - 1 ) / MAX_REWARD_COLUMNS );
		m_pRewardScrollBar->SetRange( CsPoint( 0, nRewardLineCount ) );
		m_nRewardScroll = max( 0, min( m_pRewardScrollBar->GetMaxPosIndex(), m_nRewardScroll ) );
		if( m_pRewardScrollBar->IsEnableScroll() )
			m_pRewardScrollBar->SetCurPosIndex( m_nRewardScroll );
		else
			m_nRewardScroll = 0;
		m_pRewardScrollBar->SetVisible( true );
	}
	if( m_pTitleText ) m_pTitleText->SetText( entry.s_wsName.c_str(), 540 );
	if( m_pSubTitleText ) m_pSubTitleText->SetText( _T( "" ) );
	if( m_pPartyLimitText )
	{
		TCHAR szPartyLimit[32] = { 0, };
		_stprintf_s( szPartyLimit, _countof( szPartyLimit ), _T( "Para %02uP" ), _GetMaxPlayers( entry ) );
		m_pPartyLimitText->SetText( szPartyLimit );
	}
	if( m_pPartyLimitIcon )
		m_pPartyLimitIcon->SetVisible( true );

	if( m_pImagePathText )
		m_pImagePathText->SetText( _T( "" ) );
	bool bPreviewVisible = false;
	if( m_pPreviewSprite )
	{
		if( DungeonInterfaceAssetExists( entry.s_strImagePath ) )
		{
			m_pPreviewSprite->SetVisible( true );
			m_pPreviewSprite->ChangeTexture( entry.s_strImagePath.c_str(), true );
			bPreviewVisible = true;
			nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI preview dungeonId=%u portalId=%u image=%s",
				entry.s_dwDungeonId, entry.s_dwPortalId, entry.s_strImagePath.c_str() );
		}
		else
		{
			m_pPreviewSprite->SetVisible( false );
			nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI preview missing dungeonId=%u portalId=%u destMap=%u",
				entry.s_dwDungeonId, entry.s_dwPortalId, entry.s_dwDestinationMapId );
		}
	}
	if( m_pPreviewDifficultyBadge )
	{
		m_pPreviewDifficultyBadge->ChangeTexture( DungeonDifficultyTexture( entry.s_wDifficulty ), true );
		m_pPreviewDifficultyBadge->SetVisible( bPreviewVisible );
	}

	std::vector<sRequirementLine> vRequirementLines;
	_BuildRequirementLines( entry, vRequirementLines );
	if( vRequirementLines.empty() )
	{
		sRequirementLine line;
		line.s_wsText = _T( "Sem condi\x00E7\x00E3o" );
		line.s_bSection = false;
		vRequirementLines.push_back( line );
	}

	if( m_pRequirementScrollBar )
	{
		m_pRequirementScrollBar->SetRange( CsPoint( 0, (int)vRequirementLines.size() ) );
		m_nRequirementScroll = max( 0, min( m_pRequirementScrollBar->GetMaxPosIndex(), m_nRequirementScroll ) );
		if( m_pRequirementScrollBar->IsEnableScroll() )
			m_pRequirementScrollBar->SetCurPosIndex( m_nRequirementScroll );
		else
			m_nRequirementScroll = 0;
	}
	int nRowY = 500;

	for (int i = 0; i < MAX_REQUIREMENT_TEXT; ++i)
	{
		int const nLineIndex = m_nRequirementScroll + i;

		if (nLineIndex < 0 || nLineIndex >= (int)vRequirementLines.size() || m_pRequirementText[i] == NULL)
			continue;

		sRequirementLine const& line = vRequirementLines[nLineIndex];
		bool const bSection = line.s_bSection;

		// adiciona espaço antes de um novo título/seção
		if (bSection && i > 0)
			nRowY += 8;

		if (m_pRequirementSectionLine[i])
		{
			m_pRequirementSectionLine[i]->SetPos(CsPoint(297, nRowY - 7));
			m_pRequirementSectionLine[i]->SetVisible(bSection);
		}

		m_pRequirementText[i]->SetText(line.s_wsText.c_str(), bSection ? 260 : 240);
		m_pRequirementText[i]->SetColor(bSection ? NiColor(0.70f, 0.95f, 0.82f) : FONT_WHITE);
		m_pRequirementText[i]->SetPos(CsPoint(bSection ? 314 : 342, bSection ? nRowY - 2 : nRowY + 2));

		if (m_pRequirementSectionMark[i])
			m_pRequirementSectionMark[i]->SetVisible(false);

		nRowY += bSection ? 24 : 21;
	}

	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
	{
		int const nRewardIndex = ( m_nRewardScroll * MAX_REWARD_COLUMNS ) + i;
		if( m_pRewardSlot[i] )
			m_pRewardSlot[i]->SetVisible( nRewardIndex >= 0 && nRewardIndex < (int)entry.s_vMonsterRewards.size() && entry.s_vMonsterRewards[nRewardIndex].s_dwItemId != 0 );
	}

	_RefreshCompletionRewardPanel();
}

void cDungeonEntranceWindow::_BuildRequirementLines( sDungeonEntry const& entry, std::vector<sRequirementLine>& vLines ) const
{
	auto AddLine = [&vLines]( TCHAR const* pszText, bool bSection )
	{
		if( pszText == NULL || pszText[0] == 0 )
			return;

		sRequirementLine line;
		line.s_wsText = pszText;
		line.s_bSection = bSection;
		vLines.push_back( line );
	};
	auto AddWideLine = [&vLines]( std::wstring const& wsText, bool bSection )
	{
		if( wsText.empty() )
			return;

		sRequirementLine line;
		line.s_wsText = wsText;
		line.s_bSection = bSection;
		vLines.push_back( line );
	};
	auto TrimDescriptionLine = []( std::wstring const& wsText )
	{
		size_t nBegin = 0;
		size_t nEnd = wsText.length();
		while( nBegin < nEnd && ( wsText[nBegin] == L' ' || wsText[nBegin] == L'\t' ) )
			++nBegin;
		while( nEnd > nBegin && ( wsText[nEnd - 1] == L' ' || wsText[nEnd - 1] == L'\t' ) )
			--nEnd;
		return wsText.substr( nBegin, nEnd - nBegin );
	};
	auto IsTextFit = []( std::wstring const& wsText, int nMaxWidth )
	{
		cText::sTEXTINFO textInfo;
		textInfo.Init( &g_pEngine->m_FontSystem, CFont::FS_12, FONT_WHITE );
		textInfo.s_bOutLine = true;
		textInfo.SetText( wsText.c_str() );
		return textInfo.GetTextWidth() <= nMaxWidth;
	};
	auto AddWrappedDescriptionLine = [&]( std::wstring const& wsRawLine )
	{
		std::wstring wsLine = TrimDescriptionLine( wsRawLine );
		std::wstring wsCurrent;
		size_t nPos = 0;

		while( nPos <= wsLine.length() )
		{
			size_t const nNext = wsLine.find( L' ', nPos );
			std::wstring wsWord = nNext == std::wstring::npos
				? wsLine.substr( nPos )
				: wsLine.substr( nPos, nNext - nPos );

			if( wsWord.empty() == false )
			{
				std::wstring const wsCandidate = wsCurrent.empty() ? wsWord : wsCurrent + L" " + wsWord;
				if( wsCurrent.empty() == false && IsTextFit( wsCandidate, 245 ) == false )
				{
					AddWideLine( wsCurrent, false );
					wsCurrent = wsWord;
				}
				else
				{
					wsCurrent = wsCandidate;
				}
			}

			if( nNext == std::wstring::npos )
				break;
			nPos = nNext + 1;
		}

		AddWideLine( wsCurrent, false );
	};

	AddLine( _T( "Condi\x00E7\x00F5" "es de Participa\x00E7\x00E3o" ), true );

	bool bAddedGeneralCondition = false;
	bool bAddedEntryItem = false;
	std::vector<sRequirement const*> vEntryItems;
	for( int i = 0; i < (int)entry.s_vRequirements.size(); ++i )
	{
		sRequirement const& req = entry.s_vRequirements[i];
		if( req.s_wType == 1 )
		{
			vEntryItems.push_back( &req );
			continue;
		}

		DWORD const dwFirst = DungeonFirstValue( req.s_vValues );
		if( req.s_wType == 7 )
		{
			TCHAR szLevel[128] = { 0, };
			_stprintf_s( szLevel, _countof( szLevel ), _T( "N\x00EDvel Digimon Mais de %u" ), dwFirst );
			AddLine( szLevel, false );
			bAddedGeneralCondition = true;
			continue;
		}

		AddLine( _FormatRequirement( req ).c_str(), false );
		bAddedGeneralCondition = true;
	}

	if( bAddedGeneralCondition == false )
		AddLine( _T( "Sem condi\x00E7\x00E3o" ), false );

	AddLine( _T( "Itens de entrada" ), true );
	for( int i = 0; i < (int)vEntryItems.size(); ++i )
	{
		sRequirement const& req = *vEntryItems[i];
		DWORD const dwFirst = DungeonFirstValue( req.s_vValues );
		DWORD const dwSecond = req.s_vValues.size() > 1 ? req.s_vValues[1] : 0;

		if( dwFirst == 47325 )
		{
			AddLine( _T( "Chave Master" ), false );
			AddLine( _T( "DATS 1 Item" ), false );
		}
		else
		{
			CsItem* pItem = nsCsFileTable::g_pItemMng ? nsCsFileTable::g_pItemMng->GetItem( dwFirst ) : NULL;
			TCHAR const* pszName = ( pItem && pItem->GetInfo() ) ? pItem->GetInfo()->s_szName : _T( "Item" );
			AddLine( pszName, false );

			TCHAR szAmount[64] = { 0, };
			_stprintf_s( szAmount, _countof( szAmount ), _T( "%u Item" ), dwSecond == 0 ? 1 : dwSecond );
			AddLine( szAmount, false );
		}
		bAddedEntryItem = true;
	}

	if( bAddedEntryItem == false )
		AddLine( _T( "Sem condi\x00E7\x00E3o" ), false );

	AddLine( _T( "Passo a passo" ), true );
	TCHAR szCapacity[128] = { 0, };
	_stprintf_s( szCapacity, _countof( szCapacity ), _T( "Capacidade m\x00E1xima: %u Pessoa(s)" ), _GetMaxPlayers( entry ) );
	AddLine( szCapacity, false );

	if( entry.s_wsDescription.empty() == false )
	{
		std::wstring wsLine;
		for( size_t nChar = 0; nChar <= entry.s_wsDescription.length(); ++nChar )
		{
			wchar_t const ch = nChar < entry.s_wsDescription.length() ? entry.s_wsDescription[nChar] : L'\n';
			if( ch == L'\r' )
				continue;
			if( ch != L'\n' )
			{
				wsLine.push_back( ch );
				continue;
			}

			AddWrappedDescriptionLine( wsLine );
			wsLine.clear();
		}
	}
}

void cDungeonEntranceWindow::_SetCompletionRewardMode( bool bEnable )
{
	if( bEnable && ( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() ) )
		bEnable = false;

	m_bCompletionRewardMode = bEnable;
	if( bEnable )
		m_nCompletionRewardScroll = 0;

	_RefreshDetail();
}

void cDungeonEntranceWindow::_RefreshCompletionRewardPanel()
{
	bool const bHasSelection = m_nSelectedVisible >= 0 && m_nSelectedVisible < (int)m_vVisibleDungeons.size();
	bool const bVisible = m_bCompletionRewardMode && bHasSelection;

	if( m_pCompletionOpenButton )
		m_pCompletionOpenButton->SetVisible( bHasSelection && bVisible == false );
	if( m_pCompletionOpenIcon )
		m_pCompletionOpenIcon->SetVisible( bHasSelection && bVisible == false );
	if( m_pCompletionCloseButton )
		m_pCompletionCloseButton->SetVisible( bVisible );
	if( m_pCompletionPanelBg )
		m_pCompletionPanelBg->SetVisible( bVisible );
	if( m_pCompletionBasicTabBg )
		m_pCompletionBasicTabBg->SetVisible( bVisible );
	if( m_pCompletionRewardListHeaderBg )
		m_pCompletionRewardListHeaderBg->SetVisible( bVisible );
	if( m_pCompletionContentHeaderBg )
		m_pCompletionContentHeaderBg->SetVisible( bVisible );
	if( m_pCompletionExtraTabBg )
		m_pCompletionExtraTabBg->SetVisible( bVisible );
	if( m_pCompletionTitleText )
		m_pCompletionTitleText->SetVisible( bVisible );
	if( m_pCompletionBasicTabText )
		m_pCompletionBasicTabText->SetVisible( bVisible );
	if( m_pCompletionRewardListHeaderText )
		m_pCompletionRewardListHeaderText->SetVisible( bVisible );
	if( m_pCompletionContentHeaderText )
		m_pCompletionContentHeaderText->SetVisible( bVisible );
	if( m_pCompletionInfoText )
		m_pCompletionInfoText->SetVisible( bVisible );
	if( m_pCompletionExtraTabText )
		m_pCompletionExtraTabText->SetVisible( bVisible );
	if( m_pCompletionEmptyText )
		m_pCompletionEmptyText->SetVisible( false );
	if( m_pCompletionContentScrollBar )
		m_pCompletionContentScrollBar->SetVisible( bVisible );
	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
		if( m_pCompletionRewardSlot[i] )
			m_pCompletionRewardSlot[i]->SetVisible( false );

	if( bVisible == false )
		return;

	if( m_pPreviewSprite )
		m_pPreviewSprite->SetVisible( false );
	if( m_pPreviewDifficultyBadge )
		m_pPreviewDifficultyBadge->SetVisible( false );
	if( m_pTitleText )
		m_pTitleText->SetText( _T( "" ) );
	if( m_pSubTitleText )
		m_pSubTitleText->SetText( _T( "" ) );

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	std::vector<sReward> const& vCompletionRewards = entry.s_vCompletionRewards.empty() ? entry.s_vMonsterRewards : entry.s_vCompletionRewards;
	if( m_pCompletionContentScrollBar )
	{
		m_pCompletionContentScrollBar->SetRange( CsPoint( 0, max( MAX_COMPLETION_REWARD_VISIBLE, (int)vCompletionRewards.size() ) ) );
		m_nCompletionRewardScroll = max( 0, min( m_pCompletionContentScrollBar->GetMaxPosIndex(), m_nCompletionRewardScroll ) );
		if( m_pCompletionContentScrollBar->IsEnableScroll() )
			m_pCompletionContentScrollBar->SetCurPosIndex( m_nCompletionRewardScroll );
		else
			m_nCompletionRewardScroll = 0;
	}

	if( vCompletionRewards.empty() )
	{
		if( m_pCompletionEmptyText )
		{
			m_pCompletionEmptyText->SetText( _T( "Sem recompensas configuradas." ) );
			m_pCompletionEmptyText->SetVisible( true );
		}
		return;
	}

	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
	{
		int const nRewardIndex = m_nCompletionRewardScroll + i;
		if( nRewardIndex < 0 || nRewardIndex >= (int)vCompletionRewards.size() )
			continue;
		if( m_pCompletionRewardSlot[i] )
			m_pCompletionRewardSlot[i]->SetVisible( vCompletionRewards[nRewardIndex].s_dwItemId != 0 );
	}
}

void cDungeonEntranceWindow::_SelectVisibleEntry( int nVisibleIndex )
{
	int const nAbsoluteIndex = m_nListScroll + nVisibleIndex;
	if( nAbsoluteIndex < 0 || nAbsoluteIndex >= (int)m_vVisibleDungeons.size() )
		return;

	m_nSelectedVisible = nAbsoluteIndex;
	m_nRequirementScroll = 0;
	m_nRewardScroll = 0;
	m_nCompletionRewardScroll = 0;
	m_bCompletionRewardMode = false;
	_RefreshList();
	_RefreshDetail();
}

void cDungeonEntranceWindow::_ScrollList( int nDelta )
{
	int const nMaxScroll = max( 0, (int)m_vVisibleDungeons.size() - MAX_LIST_VISIBLE );
	m_nListScroll += nDelta;
	if( m_nListScroll < 0 )
		m_nListScroll = 0;
	if( m_nListScroll > nMaxScroll )
		m_nListScroll = nMaxScroll;
	if( m_pListScrollBar )
		m_pListScrollBar->SetCurPosIndex( m_nListScroll );
	_RefreshList();
}

bool cDungeonEntranceWindow::_IsMouseOverList()
{
	if( m_pRoot == NULL )
		return false;

	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	return ptMouse.x >= 16 && ptMouse.x <= 282 && ptMouse.y >= 105 && ptMouse.y <= 685;
}


bool cDungeonEntranceWindow::_IsMouseOverReward()
{
	if( m_pRoot == NULL )
		return false;

	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	return ptMouse.x >= 646 && ptMouse.x <= 882 && ptMouse.y >= 508 && ptMouse.y <= 624;
}

bool cDungeonEntranceWindow::_IsMouseOverRequirement()
{
	if( m_pRoot == NULL )
		return false;

	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	return ptMouse.x >= 298 && ptMouse.x <= 625 && ptMouse.y >= 522 && ptMouse.y <= 698;
}

void cDungeonEntranceWindow::_EnterSelectedDungeon()
{
	if( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() )
	{
		_SetStatus( _T( "Selecione uma dungeon primeiro." ) );
		return;
	}

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	nsCSDEBUG::CrashLogger::LogMessage( "DUNGEON_UI enter portalId=%u srcMap=%u destMap=%u npcObjectId=%u portalKind=%d name=%S target=%p",
		entry.s_dwPortalId, entry.s_dwSourceMapId, entry.s_dwDestinationMapId, entry.s_dwNpcObjectId, entry.s_nPortalKindIndex, entry.s_wsName.c_str(), m_pTarget );

	if( entry.s_dwPortalId == 0 )
	{
		_SetStatus( _T( "Esta dungeon ainda nao possui link de MapPortal." ) );
		return;
	}

	if( net::game == NULL )
	{
		_SetStatus( _T( "A conexao do jogo nao esta pronta." ) );
		return;
	}

	net::game->SetInsMap_DungeonMSG( entry.s_dwPortalId );
	Close();
}

bool cDungeonEntranceWindow::_SetRewardToolTip()
{
	if( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() )
		return false;

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	for( int i = 0; i < MAX_REWARD_VISIBLE; ++i )
	{
		int const nRewardIndex = ( m_nRewardScroll * MAX_REWARD_COLUMNS ) + i;
		if( nRewardIndex < 0 || nRewardIndex >= (int)entry.s_vMonsterRewards.size() )
			continue;

		sReward const& reward = entry.s_vMonsterRewards[nRewardIndex];
		if( reward.s_dwItemId == 0 )
			continue;

		CsPoint ptSlot = _GetRewardSlotPos( i );
		CsRect rcSlot( ptSlot, CsSIZE( 44, 44 ) );
		if( rcSlot.PtInRect( ptMouse ) == false )
			continue;

		cTooltip* pToolTip = TOOLTIPMNG_STPTR ? TOOLTIPMNG_STPTR->GetTooltip() : NULL;
		SAFE_POINTER_RETVAL( pToolTip, false );
		pToolTip->SetTooltip( GetRootClient() + ptSlot, CsPoint( 44, 44 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, reward.s_dwItemId, cBaseWindow::WT_DUNGEON_ENTRANCE );
		return true;
	}

	return false;
}

bool cDungeonEntranceWindow::_SetCompletionRewardToolTip()
{
	if( m_bCompletionRewardMode == false )
		return false;
	if( m_nSelectedVisible < 0 || m_nSelectedVisible >= (int)m_vVisibleDungeons.size() )
		return false;

	sDungeonEntry const& entry = m_vDungeons[m_vVisibleDungeons[m_nSelectedVisible]];
	std::vector<sReward> const& vCompletionRewards = entry.s_vCompletionRewards.empty() ? entry.s_vMonsterRewards : entry.s_vCompletionRewards;
	CsPoint const ptMouse = MousePosToWindowPos( CURSOR_ST.GetPos() );
	for( int i = 0; i < MAX_COMPLETION_REWARD_VISIBLE; ++i )
	{
		int const nRewardIndex = m_nCompletionRewardScroll + i;
		if( nRewardIndex < 0 || nRewardIndex >= (int)vCompletionRewards.size() )
			continue;

		sReward const& reward = vCompletionRewards[nRewardIndex];
		if( reward.s_dwItemId == 0 )
			continue;

		CsPoint ptSlot = _GetCompletionRewardSlotPos( i );
		CsRect rcSlot( ptSlot, CsSIZE( 44, 44 ) );
		if( rcSlot.PtInRect( ptMouse ) == false )
			continue;

		cTooltip* pToolTip = TOOLTIPMNG_STPTR ? TOOLTIPMNG_STPTR->GetTooltip() : NULL;
		SAFE_POINTER_RETVAL( pToolTip, false );
		pToolTip->SetTooltip( GetRootClient() + ptSlot, CsPoint( 44, 44 ), TOOLTIP_MAX_SIZE, cTooltip::ITEM, reward.s_dwItemId, cBaseWindow::WT_DUNGEON_ENTRANCE );
		return true;
	}

	return false;
}

CsPoint cDungeonEntranceWindow::_GetRewardSlotPos( int nIndex ) const
{
	int const nColumn = nIndex % MAX_REWARD_COLUMNS;
	int const nRow = nIndex / MAX_REWARD_COLUMNS;
	return CsPoint( 653 + ( nColumn * 49 ), 524 + ( nRow * 49 ) );
}

CsPoint cDungeonEntranceWindow::_GetCompletionRewardSlotPos( int nIndex ) const
{
	int const nColumn = nIndex % 3;
	int const nRow = nIndex / 3;
	return CsPoint( 666 + ( nColumn * 49 ), 213 + ( nRow * 49 ) );
}

int cDungeonEntranceWindow::_FindEntryByDungeonId( DWORD dwDungeonId ) const
{
	for( int i = 0; i < (int)m_vDungeons.size(); ++i )
	{
		if( m_vDungeons[i].s_dwDungeonId == dwDungeonId )
			return i;
	}
	return -1;
}

WORD cDungeonEntranceWindow::_GetMaxPlayers( sDungeonEntry const& entry ) const
{
	if( entry.s_wMaxPlayers > 0 )
		return entry.s_wMaxPlayers;
	if( entry.s_wMinPlayers > 0 )
		return entry.s_wMinPlayers;

	DWORD const dwListMapId = entry.s_dwDungeonId >= 100 ? ( entry.s_dwDungeonId / 100 ) : 0;
	DWORD const dwMapId = entry.s_dwDestinationMapId != 0 ? entry.s_dwDestinationMapId : dwListMapId;
	if( ( dwMapId >= 1600 && dwMapId <= 1616 ) || ( dwListMapId >= 1600 && dwListMapId <= 1616 ) )
		return 4;

	return 1;
}

std::wstring cDungeonEntranceWindow::_BuildDungeonDisplayName( sDungeonEntry const& entry ) const
{
	TCHAR szName[128] = { 0, };
	_stprintf_s( szName, _countof( szName ), _T( "Dungeon Map %u" ), entry.s_dwDestinationMapId );
	return szName;
}

std::wstring cDungeonEntranceWindow::_FormatRequirement( sRequirement const& req ) const
{
	TCHAR szBuffer[256] = { 0, };
	DWORD const dwFirst = DungeonFirstValue( req.s_vValues );
	DWORD const dwSecond = req.s_vValues.size() > 1 ? req.s_vValues[1] : 0;

	if( req.s_wType == 1 )
	{
		CsItem* pItem = nsCsFileTable::g_pItemMng ? nsCsFileTable::g_pItemMng->GetItem( dwFirst ) : NULL;
		TCHAR const* pszName = ( pItem && pItem->GetInfo() ) ? pItem->GetInfo()->s_szName : _T( "Item" );
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%s x%u" ), pszName, dwSecond == 0 ? 1 : dwSecond );
		return szBuffer;
	}

	if( req.s_wType == 7 )
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "N\x00EDvel Digimon Mais de %u" ), dwFirst );
	else
		_stprintf_s( szBuffer, _countof( szBuffer ), _T( "%s %u" ), DungeonRequirementName( req.s_wType ), dwFirst );
	return szBuffer;
}
