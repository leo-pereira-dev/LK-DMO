
#include "stdafx.h"
#include "CharOption.h"

void	CCharOption::CalNameScaleConstant()
{ 
	g_fNameScaleConstant = tan( CsD2R( CAMERA_ST.GetCameraInfo()->s_fFOV )*0.5f )*0.0135f; 
}

float CCharOption::GetNameScaleConstant()
{ 
	return g_fNameScaleConstant; 
}

void sCHAR_IMAGE::Render_L( CsPoint pos, CsPoint size /* = CsPoint::ZERO  */)
{
	if( s_pLargeImg == NULL )
		return;

	if( size == CsPoint::ZERO )
	{
		s_pLargeImg->Render( pos, CsPoint( 83, 72 ) );
	}
	else
	{
		s_pLargeImg->Render( pos, size );
	}
}

void sCHAR_IMAGE::Render_S( CsPoint pos, CsPoint size /* = CsPoint::ZERO  */)
{
	if( s_pSmallImg == NULL )
		return;

	if( size == CsPoint::ZERO )
	{
		s_pSmallImg->Render( pos, CsPoint( 50, 43 ) );
	}
	else
	{
		s_pSmallImg->Render( pos, size );
	}
}

void sCHAR_IMAGE::Render_B( CsPoint pos, CsPoint size /* = CsPoint::ZERO  */)
{
	if( s_pBossImg == NULL )
		return;

	if( size == CsPoint::ZERO )
	{
		s_pBossImg->Render( pos, CsPoint( 35, 35 ) );
	}
	else
	{
		s_pBossImg->Render( pos, size );
	}
}

void sCHAR_IMAGE::ResetDevice()
{
	if( s_pLargeImg )
		s_pLargeImg->ResetDevice();
	if( s_pSmallImg )
		s_pSmallImg->ResetDevice();
	if( s_pBossImg )
		s_pBossImg->ResetDevice();
}





float CCharOption::g_fNameScaleConstant = 1.0f;

namespace
{
	bool IsNamePlateRange( DWORD nID, DWORD nBegin, DWORD nEnd )
	{
		return nID >= nBegin && nID <= nEnd;
	}

	const char* GetNamePlateTextureName( DWORD nID )
	{
		switch( nID )
		{
		case 39000: case 39004: case 39030: case 128419: case 141110: case 151043: case 1310332: case 1310362:
			return "GoldClock1";
		case 39001: case 39005: case 39031: case 128420: case 141111: case 151044: case 1310333: case 1310363:
			return "Summer1";
		case 39002: case 39006: case 39032: case 128421: case 141112: case 151045: case 155298: case 1310334: case 1310364:
			return "Flex";
		case 39003: case 39007: case 39033: case 128422: case 141113: case 151046: case 1310335: case 1310365:
			return "Boss";
		case 39008: case 39012: case 39034: case 128423: case 141114: case 151047: case 1310366:
			return "MirrorBall";
		case 39009: case 39013: case 39035: case 128424: case 141115: case 151048: case 1310367:
			return "Moon";
		case 39010: case 39014: case 39036: case 128425: case 141116: case 151049: case 1310368:
			return "Sun";
		case 39011: case 39015: case 39037: case 128426: case 141117: case 151050: case 1310336: case 1310369:
			return "Angel";
		case 39016: case 39018: case 39038: case 128427: case 141118: case 151051: case 1310337: case 1310370:
			return "Blaze";
		case 39017: case 39019: case 39039: case 128428: case 141119: case 151052: case 1310371:
			return "Poop";
		case 39020: case 39067: case 39114:
			return "Lightning_1";
		case 39021: case 39024: case 39040: case 128430: case 1310338: case 1310372:
			return "Fox_Tail";
		case 39026: case 39028: case 39041: case 1310373:
			return "Halloween_1";
		case 39027: case 39029: case 39042: case 1310374:
			return "Halloween_2";
		case 39043: case 39046: case 1310339: case 1310375:
			return "Cristmas";
		case 39044: case 39047: case 1310340: case 1310376:
			return "Ice_Crystal";
		case 39045: case 39048: case 155251: case 1310341: case 1310377:
			return "Dark_Blaze";
		case 39065: case 39066: case 39113:
			return "solarsystem_1";
		case 39068: case 39070: case 39109:
			return "Hurricane_1";
		case 39069: case 39071: case 39110:
			return "Tornado_1";
		case 39073: case 39075: case 39111:
			return "windwave";
		case 39074: case 39076: case 39112:
			return "Butterfly";
		case 39102: case 39104: case 39107:
			return "Fireball";
		case 39103: case 39105: case 39108:
			return "FlowerShower";
		case 152217:
			return "Arena_Challenger";
		case 152218:
			return "Arena_master";
		}

		if( IsNamePlateRange( nID, 39290, 39309 ) ||
			IsNamePlateRange( nID, 39311, 39330 ) ||
			IsNamePlateRange( nID, 47336, 47355 ) ||
			IsNamePlateRange( nID, 152753, 152772 ) ||
			IsNamePlateRange( nID, 160386, 160425 ) ||
			nID == 160637 ||
			nID == 160639 )
			return "Butter-fly_1";

		if( nID == 39356 ||
			nID == 39358 ||
			nID == 39362 ||
			IsNamePlateRange( nID, 180720, 180739 ) ||
			IsNamePlateRange( nID, 180760, 180779 ) ||
			IsNamePlateRange( nID, 180800, 180819 ) ||
			IsNamePlateRange( nID, 180840, 180859 ) ||
			IsNamePlateRange( nID, 180880, 180899 ) ||
			IsNamePlateRange( nID, 181176, 181180 ) )
			return "zeed";

		if( nID == 39360 ||
			IsNamePlateRange( nID, 180976, 180995 ) ||
			IsNamePlateRange( nID, 181016, 181035 ) ||
			IsNamePlateRange( nID, 181056, 181075 ) ||
			IsNamePlateRange( nID, 181096, 181115 ) ||
			IsNamePlateRange( nID, 181181, 181184 ) )
			return "ShoutX7_1";

		if( IsNamePlateRange( nID, 180503, 180522 ) ||
			IsNamePlateRange( nID, 180543, 180562 ) ||
			IsNamePlateRange( nID, 180583, 180602 ) ||
			IsNamePlateRange( nID, 180623, 180642 ) ||
			IsNamePlateRange( nID, 180663, 180682 ) ||
			IsNamePlateRange( nID, 181171, 181175 ) )
			return "Heaven";

		return NULL;
	}

	bool AddItemIconBillboard( cText3D* pText, DWORD nID )
	{
		if( pText == NULL )
			return false;

		CsItem* pItem = nsCsFileTable::g_pItemMng->GetItem( nID );
		if( pItem == NULL || pItem->GetInfo() == NULL || pItem->GetInfo()->s_nIcon == 0 )
			return false;

		DWORD dwTex = 0;
		int nIconIdx = 0;
		cIconMng::_IconIDToTexIndex( dwTex, nIconIdx, NULL, pItem->GetInfo()->s_nIcon );

		char szIconPath[ MAX_PATH ] = { 0, };
		sprintf_s( szIconPath, MAX_PATH, "Icon\\Icon%02u.dds", dwTex );

		float const fUnit = 1.0f / 32.0f;
		pText->AddBillBoard(
			szIconPath,
			NiPoint2( 0.0f, 0.0f ),
			( nIconIdx % 32 ) * fUnit,
			( ( nIconIdx % 32 ) + 1 ) * fUnit,
			( nIconIdx / 32 ) * fUnit,
			( ( nIconIdx / 32 ) + 1 ) * fUnit,
			NiPoint2( 18.0f, 18.0f ) );

		return true;
	}
}

CCharOption::CCharOption():m_pTextName(NULL),m_pTextPenName(NULL)
,m_pAchieve(NULL),m_nAchieveID(0),m_pGuildName(NULL),m_pParent(NULL),m_dwPlag(0)
,m_pIcon(NULL),m_nIconID(0),m_pMatchIcon(NULL),m_nMatchIconID(0),m_pTamerIcon(NULL),m_nTamerIconID(0),m_nSkillCode(0),m_AchiveColor(FONT_GREEN)
{
	m_szName[ 0 ] = NULL;
	m_szPenName[ 0 ] = NULL;
	m_szGuildName[ 0 ] = NULL;
	m_szGuildMarkName[ 0 ] = NULL;
}

CCharOption::~CCharOption()
{
	Delete();
}

void CCharOption::Delete()
{
	SAFE_NIDELETE( m_pTextName );
	SAFE_NIDELETE( m_pTextPenName );
	SAFE_NIDELETE( m_pGuildName );
	SAFE_NIDELETE( m_pAchieve );
	SAFE_NIDELETE(m_pIcon);
	SAFE_NIDELETE( m_pMatchIcon );
	SAFE_NIDELETE( m_pTamerIcon );
	m_pParent = NULL;
	m_szName[ 0 ] = NULL;
	m_szPenName[ 0 ] = NULL;
	m_nAchieveID = 0;
	m_szGuildName[ 0 ] = NULL;
	m_szGuildMarkName[ 0 ] = NULL;
	m_dwPlag = 0;
	m_nSkillCode = 0;
	m_nIconID = 0;
	m_nMatchIconID = 0;
	m_nTamerIconID = 0;
}

void CCharOption::Init( CsC_AvObject* pParent, LPCTSTR szName )
{
	m_pParent = pParent;

	assert_cs( m_pTextName == NULL );		
	if( szName == NULL )
		return;

	ResetName( szName );
}

void CCharOption::SetName( TCHAR const* szText )
{ 
	ResetName( szText ); 
}

bool CCharOption::IsPlag( ePLAG plag )
{ 
	return ( (m_dwPlag & plag ) != 0 ); 
}

#ifdef GUILD_RENEWAL
// 테이머 이름색 변경
void CCharOption::SetNameColor( NiColor pColor )
{ 
	if( m_pTextName )
		m_pTextName->SetColor(pColor); 
}	
#endif

void CCharOption::ResetName( LPCTSTR szName )
{
	m_dwPlag |= NAME;

	_tcscpy_s( m_szName, CO_NAME_LEN, szName );
	if( m_szName[ 0 ] == NULL )
	{
		SAFE_NIDELETE( m_pTextName );
		return;
	}

	if( m_pTextName == NULL )
	{
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontSystem, CFont::FS_32, m_pParent->GetNameColor() );	
		ti.SetText( m_szName );
		ti.s_bOutLine = false;
		m_pTextName = NiNew cText3D;
		if( m_pTextName->Init3D( &ti ) == false )
			SAFE_NIDELETE( m_pTextName );
	}
	else
	{
		m_pTextName->SetText( m_szName );		
	}	
}

void CCharOption::ResetNameColor()
{
	if( m_pAchieve )
	{
		if( IsPlag( AWAY ) )
			m_pAchieve->SetColor( FONT_RED );
		else
			m_pAchieve->SetColor( m_AchiveColor );
	}

	bool bColorChange = false;
	NiColor pColor = DEFAULT_NAMECOLOR;

	if( m_pTextName )
		pColor = m_pParent->GetNameColor();

	if(m_nSkillCode !=0 ) // 다른 유저들의 네임색상 변경
	{
		CsSkill::sINFO* pInfo = nsCsFileTable::g_pSkillMng->GetSkill( m_nSkillCode )->GetInfo();
		if(pInfo)
		{
			int nColorLv = 0;

			for( int i = 0 ; i < SKILL_APPLY_MAX_COUNT ; i++ )
			{
				switch( pInfo->s_Apply[i].s_nA )
				{
				case GUILD_NAMECOLOR:	// 색 변경
					if( nColorLv < 1 )
					{
						float fR = (float)( pInfo->s_Apply[i].s_nB/100 );
						float fG = (float)( (pInfo->s_Apply[i].s_nB/10) % 10 );
						float fB = (float)( pInfo->s_Apply[i].s_nB%10 );
						pColor = NiColor( fR / 10.0f, fG / 10.0f, fB / 10.0f );
						break;
						nColorLv = 1;
					}
				}
			}
		}
	}

// 	if(g_pDataMng->GetParty()->GetParty_FromName(m_szName))	//우리 파티에 있을 때,
// 		pColor = PARTY_NAMECOLOR; 

	std::pair<int, bool> isPartyMember;
	isPartyMember.first = m_pParent->GetUniqID();
	GAME_EVENT_ST.OnEvent( EVENT_CODE::IS_PARTYMEMBER_FROM_TAMERUIDX, &isPartyMember );
	if( isPartyMember.second )
		pColor = PARTY_NAMECOLOR;

#ifdef GUILD_RENEWAL
	if(g_pCharMng->GetTamerUser()->GetCharOption()->GetName() == m_szName) //자신일 때는 따로 분홍색으로 변경
		GAME_EVENT_ST.OnEvent( EVENT_CODE::GET_SET_PASIVESKILL_COLOR, &pColor );
#endif

	if( m_pTextName )
		m_pTextName->SetColor(pColor);

}

void CCharOption::SetAlpha( float fAlpha )
{
	if( m_pTextName )
		m_pTextName->SetAlpha( fAlpha );
	
	if( m_pGuildName )
		m_pGuildName->SetAlpha( fAlpha );

	if( m_pTextPenName )
		m_pTextPenName->SetAlpha( fAlpha );

	if( m_pAchieve )
		m_pAchieve->SetAlpha( fAlpha );
	if( m_pIcon )
		m_pIcon->SetAlpha( fAlpha );

	if( m_pMatchIcon )
		m_pMatchIcon->SetAlpha( fAlpha );

	if( m_pTamerIcon )
		m_pTamerIcon->SetAlpha( fAlpha );
}

void CCharOption::RenderName()
{
	if( m_pParent->IsRenderName() == false )
		return;

	NiPoint3 pos = m_pParent->GetPos();	
	float fLength = ( CAMERA_ST.GetWorldTranslate() - pos ).Length();

	float fScale = g_fNameScaleConstant*pow( fLength*fLength*15.0f, 1.0f/3.0f )*0.36f;
	pos.z += m_pParent->GetToolHeight() + 46.0f;

	// 길드 이름
	if( m_pGuildName )
	{
		if( m_pGuildName->IsEnableTexture() )
		{
			m_pGuildName->Render( pos, 0, 0, fScale );

			if( fScale > 1.0f )
				pos.z += 50.0f*sqrt( fScale );
			else
				pos.z += 50.0f*fScale;
		}
		else
		{
			SetGuildName( m_szGuildName );
		}
	}
	else if( IsPlag( GUILD_NAME ) )
	{
		SetGuildName( m_szGuildName );
	}

	// 이름
	NiPoint3 kTextPos = pos;
	NiPoint3 kNamePlatePos = pos;
	NiPoint3 kDepthDir = pos - CAMERA_ST.GetWorldTranslate();
	if( kDepthDir.Unitize() > 0.001f )
	{
		kNamePlatePos += kDepthDir * 4.0f;
		kTextPos -= kDepthDir * 4.0f;
	}

	if( m_pTamerIcon )
	{
		if( m_pTamerIcon->IsEnableTexture() )
		{
			m_pTamerIcon->Render( kNamePlatePos, 0, 0, fScale );
		}
		else
		{
			SetTamerIcon( m_nTamerIconID );
		}
	}
	else if( m_nTamerIconID != 0 )
	{
		SetTamerIcon( m_nTamerIconID );
	}

	if( m_pTextName )
	{
		if( m_pTextName->IsEnableTexture() )
		{
			m_pTextName->Render( kTextPos, 0, 0, fScale );
		}
		else
		{
			ResetName( m_szName );
		}
	}
	else if( IsPlag( NAME ) )
	{
		ResetName( m_szName );
	}

	// 업적
	if( m_pAchieve )
	{
		if( m_pAchieve->IsEnableTexture() )
		{
			if( fScale > 1.0f )
				pos.z += 40.0f*sqrt( fScale );
			else
				pos.z += 40.0f*fScale;

			m_pAchieve->Render( pos, 0, 0, fScale );			
		}
		else
		{
			if( IsPlag( AWAY ) )
				SetAway( true );
			else
				SetAchieve( m_nAchieveID );
		}
	}
	else if( IsPlag( ACHIEVE ) )
	{
		if( IsPlag( AWAY ) )
			SetAway( true );
		else
			SetAchieve( m_nAchieveID );
	}
	
	//테이머 아이콘
	if( m_pMatchIcon )
	{
		if( fScale > 1.0f )
			pos.z += 50.0f*sqrt( fScale );
		else
			pos.z += 50.0f*fScale;
		if( m_pMatchIcon->IsEnableTexture() )
			m_pMatchIcon->Render( pos, 0, 0, fScale );
	}	

	// 호
	if( m_pTextPenName )
	{
		if( m_pTextPenName->IsEnableTexture() )
		{
			if( fScale > 1.0f )
				pos.z += 60.0f*sqrt( fScale );
			else
				pos.z += 60.0f*fScale;

			m_pTextPenName->Render( pos, 0, 0, fScale );
		}
		else
		{
			SetPenName( m_szPenName );
		}
	}
	else if( IsPlag( PEN_NAME ) )
	{
		SetPenName( m_szPenName );
	}

	// 몬스터 아이콘
	if( m_pIcon )
	{
		if( m_pIcon->IsEnableTexture() )
		{
			m_pIcon->Render( pos, 0, 0, fScale );

			if( fScale > 1.0f )
				pos.z += 50.0f*sqrt( fScale );
			else
				pos.z += 50.0f*fScale;
		}		
	}	
}

void CCharOption::SetPenName( LPCTSTR szPenName )
{
	assert_cs( m_pTextPenName == NULL );
	if( szPenName == NULL )
		return;

	m_dwPlag |= PEN_NAME;

	_tcscpy_s( m_szPenName, CO_NAME_LEN, szPenName );
	if( m_szPenName[ 0 ] == NULL )
		return;

	if( m_pTextPenName == NULL )
	{
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontSystem, CFont::FS_32, m_pParent->GetNameColor() );
		ti.SetText( m_szPenName );
		ti.s_bOutLine = false;
		m_pTextPenName = NiNew cText3D;
		if( m_pTextPenName->Init3D( &ti ) == false )
			SAFE_NIDELETE( m_pTextPenName );
	}
	else
	{
		m_pTextPenName->SetText( m_szPenName );
	}
}

void CCharOption::SetAchieve( DWORD nID )
{
	m_nAchieveID = nID;	
	m_AchiveColor = FONT_GREEN;
	CsAchieve* pAchive = nsCsFileTable::g_pAchieveMng->GetAchieve( m_nAchieveID );

	if( m_nAchieveID == 0 || NULL == pAchive  )
	{
		SAFE_NIDELETE( m_pAchieve );
		m_dwPlag &= ~ACHIEVE;
		SetTitleMark();	
		return;
	}

	CsAchieve::sINFO* pAchiveInfo= pAchive->GetInfo();

	m_dwPlag |= ACHIEVE;
	m_AchiveColor = m_pParent->GetNameColor(); 

	cText::sTEXTINFO ti;
	ti.Init(&g_pEngine->m_FontSystem, CFont::FS_24, m_AchiveColor );

	cStringList kTitle;
	g_pStringAnalysis->Cut_Parcing( &kTitle, 1024, pAchiveInfo->s_szTitle, &ti );

	std::wstring wsTitle;
	std::list< cString* >* pkList = kTitle.GetList();
	if( !pkList->empty() )
	{
		std::list< cString* >::iterator kIter = pkList->begin();
		cString::sTEXT* pText = (cString::sTEXT*)(*kIter)->GetElement(0);
		m_AchiveColor = pText->s_Text.GetColor();
		wsTitle = pText->s_Text.GetText();
	}
	else
		wsTitle = pAchiveInfo->s_szTitle;

	if( m_pAchieve == NULL )
	{
		ti.s_Color = m_AchiveColor;
		ti.SetText( wsTitle.c_str() );
		ti.s_bOutLine = false;
		m_pAchieve = NiNew cText3D;
		if( m_pAchieve->Init3D( &ti ) == false )
		{
			SAFE_NIDELETE( m_pAchieve );
		}
	}
	else
	{
		m_pAchieve->SetColor( m_AchiveColor );
		m_pAchieve->SetText( wsTitle.c_str() );		
	}	

	SetTitleMark();	
}

void CCharOption::SetAway( bool bAway )
{
	if( bAway == true )
	{
		m_dwPlag |= ACHIEVE | AWAY;

		if( m_pAchieve == NULL )
		{
			cText::sTEXTINFO ti;
			ti.Init( &g_pEngine->m_FontSystem, CFont::FS_24, FONT_RED );
			ti.SetText( UISTRING_TEXT( "CHAROPTION_AWAY_FROM_KEYBOARD" ).c_str() );
			ti.s_bOutLine = false;
			m_pAchieve = NiNew cText3D;
			if( m_pAchieve->Init3D( &ti ) == false )
			{
				SAFE_NIDELETE( m_pAchieve );
			}
		}
		else
		{
			m_pAchieve->SetColor( FONT_RED );
			m_pAchieve->SetText( UISTRING_TEXT( "CHAROPTION_AWAY_FROM_KEYBOARD" ).c_str() );
		}
		SetMatchIcon( m_nMatchIconID );
	}
	else
	{
		m_dwPlag &= ~AWAY;
		SetAchieve( m_nAchieveID );
	}
}

void CCharOption::_GetAchieveIcon(int& nIcon, std::string& strIconFileName)
{
	if( nIcon > 555 )
	{
		nIcon -= 556;
		strIconFileName = "Data/interface/Icon/Achieve_Icon_03.tga";
	}
	else if( nIcon > 255 )
	{
		nIcon -= 300;
		strIconFileName = "Data/interface/Icon/Achieve_Icon_02.tga";
	}
	else
		strIconFileName = "Data/interface/Icon/Achieve_Icon.tga";
}

void CCharOption::SetGuildName( LPCTSTR szGuildName )
{		
	if( szGuildName == NULL )
		return;

	if( szGuildName[ 0 ] == NULL )
		return;

	m_dwPlag |= GUILD_NAME;
	
	_tcscpy_s( m_szGuildName, Language::pLength::GuildName, szGuildName );
	TCHAR	m_szLookGuildName[ Language::pLength::GuildName + 3 ];
	_stprintf_s( m_szLookGuildName, Language::pLength::GuildName + 3, _T( "<%s>" ), szGuildName );

	if( m_pGuildName == NULL )
	{
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontSystem, CFont::FS_24, NiColor( 243.0f / 255.0f, 228.0f / 255.0f, 42.0f / 255.0f ) );
		ti.SetText( m_szLookGuildName );
		ti.s_bOutLine = false;
		m_pGuildName = NiNew cText3D;
		if( m_pGuildName->Init3D( &ti ) == false )
		{
			SAFE_NIDELETE( m_pGuildName );
		}
		else
		{		
			if( IsPlag( GUILD_MARK ) )
			{
				m_pGuildName->AddBillBoard( m_szGuildMarkName, NiPoint2( -33.0f, 0.0f ), NiPoint2( 22, 22 ) ); 
			}
		}
	}
	else
	{
		m_pGuildName->SetText( m_szLookGuildName );		
	}	
}

bool CCharOption::SetTitleMark()
{
	assert_cs( m_pParent->IsKindOf( RTTI_TAMER ) );	
	
	if( m_pTextName == NULL ) 
	{	
		ResetName( m_szName );	
	}
	else
	{
		m_pTextName->DeleteBillboard();		
	}

	// 업적 마크 셋팅
	if( m_nAchieveID != 0 )
	{	
		CsAchieve* pAchieve = nsCsFileTable::g_pAchieveMng->GetAchieve( m_nAchieveID );	
		assert_cs( pAchieve != NULL );
		int nIcon = pAchieve->GetInfo()->s_nIcon;
		if( nIcon != 0 )
		{
			std::string strFileName;
			NiSourceTexture* pTex = NULL;
			if( nIcon > 555 )
			{
				nIcon -= 556;
				strFileName = "Data/interface/Icon/Achieve_Icon_03.tga";
			}
			else if( nIcon > 255 )
			{
				nIcon -= 300;
				strFileName = "Data/interface/Icon/Achieve_Icon_02.tga";
			}
			else
				strFileName = "Data/interface/Icon/Achieve_Icon.tga";

			if( m_pTextName )
				m_pTextName->AddBillBoard( strFileName.c_str(), NiPoint2( -36.0f, 5.0f ), ( nIcon % 16 ) / 16.0f, ( ( nIcon % 16 ) + 1 ) / 16.0f, 
				( nIcon / 16 ) / 16.0f, ( ( nIcon / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}						
	}	

	// pc방 마크 셋팅
	CTamer* pTamer = (CTamer*)m_pParent;
#ifdef PC_BANG_SERVICE
	if( pTamer && pTamer->GetCondition()->IsCondition( nSync::PCBang ) == true )
	{
		if( m_pTextName )
			m_pTextName->AddBillBoard( "Data\\Interface\\Event\\PC_Bang.tga", NiPoint2( 39.0f, 0.0f ), NiPoint2( 25, 25 ) ); 		
	}
#endif

	SetMatchIcon( m_nMatchIconID );
	return true;
}

void CCharOption::SetGuildUID( int nGuildUID )
{
	m_nGuildUID = nGuildUID;
#ifdef GUILD_MARK_DOWN
	g_pDataMng->GetGuildMark()->MarkDownload( this );	
#endif		
}

bool CCharOption::SetGuildMark( bool bUse, char* szFileName )
{
	if( szFileName != NULL )
		strcpy_s( m_szGuildMarkName, MAX_PATH, szFileName );

	if( bUse && ( m_szGuildMarkName[ 0 ] != NULL ) )
	{
		m_dwPlag |= GUILD_MARK;
	}

	if( IsPlag( GUILD_MARK ) )
	{
		if( m_pGuildName == NULL )		
			return false;
		else
		{
			if( m_pGuildName->GetBillBoardCount() > 0 )
				m_pGuildName->DeleteBillboard();
			m_pGuildName->AddBillBoard( m_szGuildMarkName, NiPoint2( -33.0f, 0.0f ), NiPoint2( 22, 22 ) ); 
		}
	}

	return true;
}

bool CCharOption::SetIconMark( DWORD nID )
{
	m_nIconID = nID;
	
	if( m_pTextName == NULL ) 
	{	
		ResetName( m_szName );	
	}
	else
	{
		m_pTextName->DeleteBillboard();		
	}

	if( m_nIconID != 0 )
	{	
		CsMonster* pMonster = nsCsFileTable::g_pMonsterMng->GetMonster( m_nIconID );	
		assert_cs( pMonster != NULL );
		int nIcon1 = pMonster->GetInfo()->s_nIcon1;
		int nIcon2 = pMonster->GetInfo()->s_nIcon2;
		int nIcon3 = pMonster->GetInfo()->s_nIcon3;
		int nIcon4 = pMonster->GetInfo()->s_nIcon4;
		int nIcon5 = pMonster->GetInfo()->s_nIcon5;
		int nIcon6 = pMonster->GetInfo()->s_nIcon6;

		//data 클래스에서 텍스쳐파일을 init하고 불러온다.  
		std::string strIconFileName;
		if( nIcon1 != 0 )
		{
			_GetAchieveIcon( nIcon1, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( -36.0f, 5.0f ), ( nIcon1 % 16 ) / 16.0f, ( ( nIcon1 % 16 ) + 1 ) / 16.0f, 
				( nIcon1 / 16 ) / 16.0f, ( ( nIcon1 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
		if( nIcon2 != 0 )
		{
			_GetAchieveIcon( nIcon2, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( 36.0f, 5.0f ), ( nIcon2 % 16 ) / 16.0f, ( ( nIcon2 % 16 ) + 1 ) / 16.0f, 
				( nIcon2 / 16 ) / 16.0f, ( ( nIcon2 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
		if( nIcon3 != 0 )
		{
			_GetAchieveIcon( nIcon3, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( -30.0f, 50.0f ), ( nIcon3 % 16 ) / 16.0f, ( ( nIcon3 % 16 ) + 1 ) / 16.0f, 
				( nIcon3 / 16 ) / 16.0f, ( ( nIcon3 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
		if( nIcon4 != 0 )
		{
			_GetAchieveIcon( nIcon4, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( 30.0f, 50.0f ), ( nIcon4 % 16 ) / 16.0f, ( ( nIcon4 % 16 ) + 1 ) / 16.0f, 
				( nIcon4 / 16 ) / 16.0f, ( ( nIcon4 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
		if( nIcon5 != 0 )
		{
			_GetAchieveIcon( nIcon5, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( -30.0f, -50.0f ), ( nIcon4 % 16 ) / 16.0f, ( ( nIcon4 % 16 ) + 1 ) / 16.0f, 
				( nIcon4 / 16 ) / 16.0f, ( ( nIcon4 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
		if( nIcon6 != 0 )
		{
			_GetAchieveIcon( nIcon6, strIconFileName );
			m_pTextName->AddBillBoard( strIconFileName.c_str(), NiPoint2( 30.0f, -50.0f ), ( nIcon4 % 16 ) / 16.0f, ( ( nIcon4 % 16 ) + 1 ) / 16.0f, 
				( nIcon4 / 16 ) / 16.0f, ( ( nIcon4 / 16 ) + 1 ) / 16.0f, NiPoint2( 26, 26 ) );
		}
	}	

	return true;
}

bool CCharOption::SetMatchIcon( DWORD nTeam/*0 = 팀없음, 1 = A팀, 2 = B팀*/ ) 
{
	if( nTeam < A_TEAM || nTeam > B_TEAM  )//팀이 없으면 아이콘 셋팅 필요없어 
		return false;

#ifdef SDM_MASTERS_MATCHING_ICON_HIDE_20170220
	m_nMatchIconID = 0;
#else
	m_nMatchIconID = nTeam;
#endif

	if( m_pMatchIcon != NULL )//이미 아이콘이 있는 상태면 지워주고
		m_pMatchIcon->DeleteBillboard();
	else
	{
		//아이콘 정보가 없을 땐 초기화
		cText::sTEXTINFO ti;
		ti.Init( &g_pEngine->m_FontSystem, CFont::FS_24, m_pParent->GetNameColor() );
		ti.SetText( _T(" ") );	// SetText 안해주면 Init3D에서 false로 나옴
		ti.s_bOutLine = false;
		m_pMatchIcon = NiNew cText3D;
		if( m_pMatchIcon->Init3D( &ti ) == false )
		{
			SAFE_NIDELETE( m_pMatchIcon );
			return false;
		}
	}
	
	CsPoint ptNameSize;
	if( m_pAchieve )
	{
		ptNameSize = m_pAchieve->GetStringSize();
	}
	else
	{
		ptNameSize = m_pTextName->GetStringSize();
	}

	if( m_nMatchIconID != 0 )//팀이 정해져 있으면 팀별로 아이콘 셋팅
	{	
		int nIconIdx = m_nMatchIconID -1;
 		m_pMatchIcon->AddBillBoard( "Data/interface/MastersMatch/MastersMatch_TeamIcon.tga", NiPoint2( 0.0f , 0.0f ), ( nIconIdx % 5 ) / 5.0f, ( ( nIconIdx % 5 ) + 1 ) / 5.0f, 
 			( nIconIdx / 5 ) / 5.0f, ( ( nIconIdx / 5 ) + 1 ) / 5.0f, NiPoint2( 26, 26 ) );
	}	

	return true;
}

bool CCharOption::SetTamerIcon( DWORD nID )
{
	m_nTamerIconID = nID;
	SAFE_NIDELETE( m_pTamerIcon );

	if( m_nTamerIconID == 0 )
		return true;

	cText::sTEXTINFO ti;
	ti.Init( &g_pEngine->m_FontSystem, CFont::FS_24, m_pParent->GetNameColor() );
	ti.SetText( _T(" ") );
	ti.s_bOutLine = false;
	m_pTamerIcon = NiNew cText3D;
	if( m_pTamerIcon->Init3D( &ti ) == false )
	{
		SAFE_NIDELETE( m_pTamerIcon );
		return false;
	}

	bool bAddBillboard = false;
	char const* pTextureName = GetNamePlateTextureName( m_nTamerIconID );
	if( pTextureName != NULL )
	{
		char szNamePlatePath[ MAX_PATH ] = { 0, };
		sprintf_s( szNamePlatePath, MAX_PATH, "Data\\interface\\NamePlate\\%s.tga", pTextureName );
		m_pTamerIcon->AddBillBoard( szNamePlatePath, NiPoint2( 0.0f, 0.0f ), NiPoint2( 85.0f, 40.0f ) );
		bAddBillboard = true;
	}

	if( bAddBillboard == false )
		bAddBillboard = AddItemIconBillboard( m_pTamerIcon, m_nTamerIconID );

	if( bAddBillboard == false )
	{
		SAFE_NIDELETE( m_pTamerIcon );
		return false;
	}

	return true;
}

void CCharOption::SetSkillCode(int iSkillCode)
{
	m_nSkillCode = iSkillCode;
}

void CCharOption::DeleteGuildName()
{ 
	SAFE_NIDELETE( m_pGuildName ); 
	memset( m_szGuildName, 0 , sizeof( m_szGuildName ) ); 
}
