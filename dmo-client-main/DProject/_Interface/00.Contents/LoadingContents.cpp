#include "StdAfx.h"
#include "LoadingContents.h"

#include "../../Flow/FlowMgr.h"
#include "../../Flow/Flow.h"

#include "../../ContentsSystem/ContentsSystemDef.h"
#include "../Adapt/AdaptBattleSystem.h"
#include "../Adapt/AdaptTutorialQuest.h"
#include "../Game/EncyclopediaContents.h"
#include "../../../LibProj/CsFunc/CrashLogger.h"

bool LoadingContents::sBattleLoadingInfo::IsBattleLoading() const
{
	return m_bBattleLoading;
}

void LoadingContents::sBattleLoadingInfo::ResetData()
{
	m_bBattleLoading = false;
	m_cMyTeamCode = 0;
	m_nBattleType = 0;
	m_nBattleMode = 0;
	m_myBattleTeam.clear();
	m_otherBattleTeam.clear();
}

void LoadingContents::sBattleLoadingInfo::SetBattlePlayerInfo( BWait::sWait::WAIT_PLAYER_LIST_CON const& info )
{
	m_bBattleLoading = true;
	BWait::sWait::WAIT_PLAYER_LIST_CON_CIT it = info.begin();
	for( ; it != info.end(); ++it )
	{
		if( m_cMyTeamCode == it->first )
			m_myBattleTeam = it->second;
		else
			m_otherBattleTeam = it->second;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int const LoadingContents::IsContentsIdentity(void)
{
	return E_CT_LOADING;
}

LoadingContents::LoadingContents(void):m_AdaptBattleSystem(NULL),m_nLoadingProgress(0),m_bLoadStart(false),m_bRequestedEncyclopediaPreload(false),m_AdaptTutorialSystem(NULL)
{
	GAME_EVENT_ST.AddEvent( EVENT_CODE::SEND_PORTAL_MOVE, this, &LoadingContents::Send_Portal_Move );
	GAME_EVENT_ST.AddEvent( EVENT_CODE::LOADING_START, this, &LoadingContents::LoadingStart );
	GAME_EVENT_ST.AddEvent( EVENT_CODE::LOADING_PROGRESS_VALUE, this, &LoadingContents::SetLoadingProgress );
	GAME_EVENT_ST.AddEvent( EVENT_CODE::RECV_PLAYER_DATA_LOAD_COMPLETE, this, &LoadingContents::RecvPlayerDataLoaded );	

	GAME_EVENT_ST.AddEvent( EVENT_CODE::TABLE_LOAD_SUCCESS, this, &LoadingContents::TableLoadSuccess );	
	
	if( EventRouter() )
		EventRouter()->Register( CONTENTS_EVENT::EEvt_BattleLoading_Start, this );
}

LoadingContents::~LoadingContents(void)
{
	m_AdaptBattleSystem = NULL;
	m_AdaptTutorialSystem = NULL;
	m_MapNameList.clear();
	if( EventRouter() )
		EventRouter()->UnRegisterAll( this );
	GAME_EVENT_ST.DeleteEventAll( this );
}

int const LoadingContents::GetContentsIdentity(void) const
{
	return IsContentsIdentity();
}

bool LoadingContents::Initialize(void)
{

	return true;
}

void LoadingContents::UnInitialize(void)
{
}

bool LoadingContents::IntraConnection(ContentsSystem& System)
{ 
	m_AdaptBattleSystem = new AdaptBattleSystem;
	if( m_AdaptBattleSystem && !m_AdaptBattleSystem->SetContents( CONTENTSSYSTEM_PTR->GetContents( E_CONTENTS_TYPE::E_CT_BATTLE_SYSTEM ) ) )
		return false;

	m_AdaptTutorialSystem = new AdaptTutorialQuest;
	if( m_AdaptTutorialSystem && !m_AdaptTutorialSystem->SetContents( CONTENTSSYSTEM_PTR->GetContents( E_CONTENTS_TYPE::E_CT_TUTORIAL_CONTENTS ) ) )
		return false;

	return true; 
}

void LoadingContents::Update(float const& fElapsedTime)
{
	if( !m_bLoadStart )
		return;

	_DataProcess_Update();
}

void LoadingContents::NotifyEvent(int const& iNotifiedEvt)
{
	switch( iNotifiedEvt )
	{
	case CONTENTS_EVENT::EEvt_BattleLoading_Start:
		{
			_SetBattleLoadingInfo();// 배틀 로딩시 필요한 데이터를 가져온다.
		}
		break;
	}
}

void LoadingContents::NotifyEventStream(int const& iNotifiedEvt, ContentsStream const& kStream)
{
}

void LoadingContents::MakeMainActorData(void)
{
}

void LoadingContents::ClearMainActorData(void)
{
	ClearAllRegistered();
}

void LoadingContents::ClearWorldData(void)
{
	GAME_EVENT_ST.DeleteEvent(EVENT_CODE::FADE_IN_END, this);
	_ResetData();
}

void LoadingContents::SetLoadingProgress(void* pData)
{
	SAFE_POINTER_RET(pData);

	int nValue = *static_cast<int*>(pData);
	_SetLoadingProgressValue( nValue );
}

void LoadingContents::_SetLoadingProgressValue( int const& nValue )
{
	if( m_nLoadingProgress == nValue )
		return;

	m_nLoadingProgress = nValue;
	ContentsStream kTmp;
	kTmp << nValue;
	Notify( eLoadingProgress, kTmp );
}

std::string	LoadingContents::GetLoadMapImgFile()
{
	std::string imgFile;
	DmCS::StringFn::FormatA( imgFile, "loading\\loading_%d.dds", net::next_map_no );
	return imgFile;
}

std::string LoadingContents::GetTipImgFile()
{
	std::string imgFile;
	if( m_BattleLoadingInfo.IsBattleLoading() )	// 배틀 로딩 일 경우 툴팁를 띄워주지 않기 위함
		return imgFile;

	int nCurIndex = rand() % 3 + 1;
	DmCS::StringFn::FormatA( imgFile, "loading\\tip_0%d.tga", nCurIndex );

	return imgFile;
}

std::wstring LoadingContents::GetLoadingTipString()
{
	std::wstring tip;
	if( m_BattleLoadingInfo.IsBattleLoading() )// 배틀 로딩 일 경우 툴팁를 띄워주지 않기 위함
		return tip;

	CsLoading_Tip* pFTTip = nsCsFileTable::g_pTalkMng->GetRandom_LoadingTip( 99 );
	SAFE_POINTER_RETVAL( pFTTip, tip );
	CsLoading_Tip::sINFO* pInfo = pFTTip->GetInfo();
	SAFE_POINTER_RETVAL( pInfo, tip );
	return pInfo->s_szLoadingTip;
}

void LoadingContents::_DataLoadComplete()
{
	// 데이터 로드가 완료 되면 서버에 패킷을 보낸다
	// 서버에 동기화
	nsCSDEBUG::CrashLogger::LogMessage(
		"MAPLOAD DataLoadComplete nextMap=%u progress=%d firstLoading=%d game=%p gate=%p",
		(unsigned)net::next_map_no,
		m_nLoadingProgress,
		g_bFirstLoding ? 1 : 0,
		net::game,
		net::gate );
	if( net::game && ( g_bFirstLoding == false ) )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD send ChangeServer via game nextMap=%u", (unsigned)net::next_map_no );
		net::game->SendChangeServer();
	}
	else if( net::gate )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD send ChangeServer via gate nextMap=%u", (unsigned)net::next_map_no );
		net::gate->SendChangeServer();
	}
}


LoadingContents::sBattleLoadingInfo const& LoadingContents::GetBattleLoadingInfo() const
{
	return m_BattleLoadingInfo;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void LoadingContents::_ResetData()
{
	m_nLoadingProgress = 0;
	m_bLoadStart = false;
	m_bRequestedEncyclopediaPreload = false;
	m_BattleLoadingInfo.ResetData();
}

void LoadingContents::_RequestEncyclopediaPreload()
{
	if( m_bRequestedEncyclopediaPreload )
		return;

	EncyclopediaContents* pEncyclopediaContents = (EncyclopediaContents*)CONTENTSSYSTEM_PTR->GetContents( E_CT_ENCYCLOPEDIA_CONTENTS );
	if( pEncyclopediaContents == NULL )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] preload skip: EncyclopediaContents is null" );
		return;
	}

	pEncyclopediaContents->EnsureStaticDataLoaded();
	if( pEncyclopediaContents->ShouldRequestServerData() == false )
	{
		m_bRequestedEncyclopediaPreload = pEncyclopediaContents->IsServerDataReceived() || pEncyclopediaContents->IsServerDataRequesting();
		nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] preload skip: recv=%d requesting=%d",
			pEncyclopediaContents->IsServerDataReceived() ? 1 : 0,
			pEncyclopediaContents->IsServerDataRequesting() ? 1 : 0 );
		return;
	}

	if( net::game == NULL )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] preload skip: net::game is null" );
		return;
	}

	if( g_pResist && g_pResist->IsMovePortal() )
	{
		nsCSDEBUG::CrashLogger::LogMessage(
			"[ENCYREQ] preload skip: portal move active progress=%d nextMap=%u",
			m_nLoadingProgress,
			(unsigned)net::next_map_no );
		return;
	}

	if( net::game->m_bPortalRequesting || net::game->m_bPortalProcessing )
	{
		nsCSDEBUG::CrashLogger::LogMessage(
			"[ENCYREQ] preload skip: portal flags requesting=%d processing=%d progress=%d nextMap=%u",
			net::game->m_bPortalRequesting ? 1 : 0,
			net::game->m_bPortalProcessing ? 1 : 0,
			m_nLoadingProgress,
			(unsigned)net::next_map_no );
		return;
	}

	bool bServerRecv = false;
	GAME_EVENT_STPTR->OnEvent( EVENT_CODE::GET_ENCYCLOPEDIA_BISRECV, &bServerRecv );
	if( bServerRecv )
	{
		m_bRequestedEncyclopediaPreload = true;
		nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] preload skip: event says already received" );
		return;
	}

	m_bRequestedEncyclopediaPreload = true;
	pEncyclopediaContents->MarkServerDataRequesting();
	nsCSDEBUG::CrashLogger::LogMessage( "[ENCYREQ] preload send DigimonBookInfo" );
	net::game->SendEncyclopediaOpen();
}

bool LoadingContents::_DataProcess_Update()
{	
	if( m_nLoadingProgress < 10 )
	{
		CsMapList* pMapList	= nsCsMapTable::g_pMapListMng->GetList( net::next_map_no );
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<10 nextMap=%u mapList=%p", (unsigned)net::next_map_no, pMapList );
		if( pMapList )
		{
			CsMapList::sINFO* pMapInfo = pMapList->GetInfo();
			nsCSDEBUG::CrashLogger::LogMessage(
				"MAPLOAD mapInfo nextMap=%u path=%s cameraMax=%d",
				(unsigned)net::next_map_no,
				pMapInfo ? pMapInfo->s_cMapPath.c_str() : "<null>",
				pMapInfo ? pMapInfo->s_nCamera_Max_Level : 0 );
			if(!_CheckMapPathHash( net::next_map_no, pMapInfo->s_cMapPath ))
			{
				nsCSDEBUG::CrashLogger::LogMessage(
					"MAPLOAD map hash mismatch nextMap=%u path=%s",
					(unsigned)net::next_map_no,
					pMapInfo->s_cMapPath.c_str() );
				GAMEAPP_ST.SetProcessState( App::CWin32App::PROCESS_STOP );
				ContentsStream kTmp;
				kTmp << net::next_map_no << pMapInfo->s_cMapPath;
				NotifyContentsAndStream( CONTENTS_EVENT::EStreamEvt_NotMatch_MapNameResource, kTmp );
			}
		}

		m_nLoadingProgress = 10;
	}
	else if( m_nLoadingProgress < 54 )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<54 nextMap=%u gameIF=%p", (unsigned)net::next_map_no, g_pGameIF );
		if( g_pGameIF == NULL )
			cGameInterface::GlobalInit();
		g_pMngCollector->DeleteChar( net::next_map_no );
		g_pThread->GetResMng()->ReleaseImmediatelyResource();
		_SetLoadingProgressValue( 54 );
	}
	else if( m_nLoadingProgress < 56 )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<56 nextMap=%u", (unsigned)net::next_map_no );
		g_pThread->GetResMng()->ReleaseConnetTerrain();// 쓰레드 리소스 매니져의 외부포인터들 없애 준다
		_SetLoadingProgressValue( 56 );
	}
	else if( m_nLoadingProgress < 58 )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<58 nextMap=%u", (unsigned)net::next_map_no );
		g_pMngCollector->ResetMap();
		g_pThread->GetResMng()->ReleaseImmediatelyResource();
		_SetLoadingProgressValue( 58 );
	}else if( m_nLoadingProgress < 60 )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<60 LoadTerrain begin nextMap=%u", (unsigned)net::next_map_no );
		_SetCameraSet();
		//DUMPLOGA( "Move Map : %d -> %d", g_pTerrainMng->GetBackupMapID(), net::next_map_no );
		g_pMngCollector->LoadTerrain( net::next_map_no );	// 맵 로딩
	}else if( m_nLoadingProgress < 72 )
	{
		g_pThread->GetResMng()->ApplyConnetTerrain();// 쓰레드 리소스 매니져의 외부포인터 재 연결
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<72 ApplyConnetTerrain nextMap=%u", (unsigned)net::next_map_no );
		_SetLoadingProgressValue( 80 );
	}else if( m_nLoadingProgress < 90 )// NPC 배치
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<90 LoadChar begin nextMap=%u", (unsigned)net::next_map_no );
		g_pMngCollector->LoadChar( net::next_map_no );
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<90 LoadChar end nextMap=%u", (unsigned)net::next_map_no );
		_SetLoadingProgressValue( 90 );
	}
	else if( m_nLoadingProgress < 100 )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<100 finalize begin nextMap=%u", (unsigned)net::next_map_no );
		g_pThread->GetResMng()->ReleaseImmediatelyResource();
		_SetLoadingProgressValue( 100 );
		_DataLoadComplete();
		nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD step<100 finalize end nextMap=%u", (unsigned)net::next_map_no );
	}
	return true;
}

void LoadingContents::_SetCameraSet()
{
	int nCamMaxLv = 0;
	if( nsCsGBTerrain::g_pCurRoot != NULL )
		nCamMaxLv = nsCsMapTable::g_pMapListMng->GetList( net::next_map_no )->GetInfo()->s_nCamera_Max_Level;
	if( nCamMaxLv == 0 )
		nCamMaxLv = g_pResist->m_Character.s_nCameraMaxDistLevel;

	float fMaxDist = CAMERA_ST.GetDistLevel( nCamMaxLv );
	//float fCurDist = CAMERA_ST.GetDist();

	// 카메라 셋팅
	CAMERA_ST.SetStaticCamera( g_pResist->IsStaticCamera() == TRUE );

	sCAMERAINFO ci;	
	ci.s_fDist = fMaxDist;

	// 튜토 리얼 중, 닷트본부 지하수도 일때.
	if(net::next_map_no == 4)
		ci.s_fInitRoll = NI_TWO_PI;
	// 이외의 일반적인 경우
	else
		ci.s_fInitRoll = NI_PI;

	ci.s_fInitPitch += NI_PI*0.4f;
	ci.s_fFarPlane = 200000.0f;
	CAMERA_ST.Reset( &ci );
	CAMERA_ST.SetDistLevel( nCamMaxLv, false );
	CAMERA_ST.SetRotationLimit( -CsD2R( 35 ), CsD2R( 70 ) );
	//CAMERA_ST.SetDeltaHeight( 120.0f );
}

void LoadingContents::LoadingStart(void* pData)
{
	switch( FLOWMGR_ST.GetCurTopFlowID() )
	{
	case Flow::CFlow::FLW_CHARSEL:
	case Flow::CFlow::FLW_BATTLE:
	case Flow::CFlow::FLW_BATTLE_RESULT:
	case Flow::CFlow::FLW_SERVER_RELOCATE:
		FLOWMGR_ST.ChangeFlow(Flow::CFlow::FLW_LOADING);
		break;
	default:
		FLOWMGR_ST.PushFlow(Flow::CFlow::FLW_LOADING);
		break;
	}

	GAME_EVENT_ST.AddEvent( EVENT_CODE::FADE_IN_END, this, &LoadingContents::DataLoadStart );
}


// 서버로 부터 플레이어 정보를 다 받았다.
// 게임으로 진입 시킨다.
void LoadingContents::RecvPlayerDataLoaded(void* pData)
{
	nsCSDEBUG::CrashLogger::LogMessage(
		"MAPLOAD RecvPlayerDataLoaded begin nextMap=%u progress=%d loadStart=%d",
		(unsigned)net::next_map_no,
		m_nLoadingProgress,
		m_bLoadStart ? 1 : 0 );
	if( g_pMngCollector )
		g_pMngCollector->LoadCompleate( net::next_map_no );
	if( g_pResist )
		g_pResist->SetEnablePortal( true );

	g_pResist->SetMovePortal(false); //2017-04-12-nova
	_RequestEncyclopediaPreload();

	if( FLOWMGR_ST.GetCurFlowSize() > 1 )
	{
		if( m_BattleLoadingInfo.IsBattleLoading() )
			FLOWMGR_ST.ChangeFlow(Flow::CFlow::FLW_BATTLE);
		else
			FLOWMGR_ST.PopFlow(Flow::CFlow::FLW_LOADING);
	}
	else
	{
		FLOWMGR_ST.ChangeFlow(Flow::CFlow::FLW_MAINGAME);
	}


	GAME_EVENT_ST.DeleteEvent( EVENT_CODE::FADE_IN_END, this );

	_ResetData();
	nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD RecvPlayerDataLoaded end nextMap=%u", (unsigned)net::next_map_no );
}

void LoadingContents::DataLoadStart(void* pData)
{
	m_nLoadingProgress = 0;
	m_bLoadStart = true;
	nsCSDEBUG::CrashLogger::LogMessage( "MAPLOAD DataLoadStart nextMap=%u prevMap=%u", (unsigned)net::next_map_no, (unsigned)net::prev_map_no );
}

// 배틀 로딩시 필요한 데이터를 가져온다.
void LoadingContents::_SetBattleLoadingInfo()
{
	SAFE_POINTER_RET( m_AdaptBattleSystem );

	m_AdaptBattleSystem->GetBattleRegistInfo( m_BattleLoadingInfo.m_cMyTeamCode, 
											  m_BattleLoadingInfo.m_nBattleType, 
											  m_BattleLoadingInfo.m_nBattleMode );

	BWait::sWait::WAIT_PLAYER_LIST_CON playerInfo;
	m_AdaptBattleSystem->GetBattleLoadingInfo(playerInfo);

	m_BattleLoadingInfo.SetBattlePlayerInfo( playerInfo );
}

void LoadingContents::Send_Portal_Move(void* pData)
{
	SAFE_POINTER_RET(pData);
	DWORD dwPortalId = *static_cast<DWORD*>(pData);

	// 배틀 신청중에는 맵이동 불가
	if( m_AdaptBattleSystem && m_AdaptBattleSystem->IsBattleMatchingRegisted() )
	{
		cPrintMsg::PrintMsg( 40013 );
		return;
	}

	if( nsCsGBTerrain::g_nSvrLibType !=  nLIB::SVR_DUNGEON)
		net::nIsMoveMapByCommand = false;

	// 서버 되면 서버에 송신
	if( net::game )
	{
		CsMapPortal* pPotalFT = nsCsMapTable::g_pMapPortalMng->GetPortal_ByPortalID( nsCsGBTerrain::g_pCurRoot->GetInfo()->s_dwMapID, dwPortalId );
		assert_cs( pPotalFT != NULL );

		if( pPotalFT->GetInfo()->s_dwDestMapID == 0 )
			return;

		if(CsMapPortal::DG == pPotalFT->GetInfo()->s_dwPortalType)
		{
			// 이쯤에서 메시지 박스를 띄워준다.
			// yes 누르면 sendDG no 누르면 실시간에서 5초간 플러그 설정	

			net::game->SetInsMap_DungeonMSG( dwPortalId ); // 인던 입장 포탈
			//net::game->SendInsMap_DungeonIN( dwPortalID );	
		}
		else
			net::game->SendSelectPortal( dwPortalId );// 일반, 인던 존 내 포탈	
	}
	// 서버 안되면 클라이언트에서 포탈 처리
	else
	{
		CsMapPortal* pPotalFT = nsCsMapTable::g_pMapPortalMng->GetPortal_ByPortalID( nsCsGBTerrain::g_pCurRoot->GetInfo()->s_dwMapID, dwPortalId );
		assert_cs( pPotalFT != NULL );

		if( pPotalFT->GetInfo()->s_dwDestMapID == 0 )
			return;

		net::prev_map_no = net::next_map_no;
		net::next_map_no = pPotalFT->GetInfo()->s_dwDestMapID;
		nSync::Pos pos = nSync::Pos( pPotalFT->GetInfo()->s_nDestTargetX, pPotalFT->GetInfo()->s_nDestTargetY );
		g_pDataMng->GetPostLoad()->SetPos( pos );		
	}

	cWindow::PlaySound( ".\\System\\Item_portal_01.wav" );
}

void LoadingContents::TableLoadSuccess(void* pData)
{
	m_MapNameList.clear();
	CsMapList::MAP* pMapList	= nsCsMapTable::g_pMapListMng->GetListMap();
	CsMapList::MAP::iterator it = pMapList->begin();
	for( ; it != pMapList->end(); ++it )
	{
		SAFE_POINTER_CON( it->second );
		CsMapList::sINFO* pInfo = it->second->GetInfo();
		SAFE_POINTER_CON( pInfo);
		
		size_t HashCode = CsFPS::GetHashCode( pInfo->s_cMapPath.c_str() );
		m_MapNameList.insert( std::make_pair( it->first, HashCode ) );
	}	
}

bool LoadingContents::_CheckMapPathHash( DWORD const& dwMapIDx, std::string const& sPath )
{
	size_t HashCode = CsFPS::GetHashCode( sPath.c_str() );
	std::map<DWORD,size_t>::const_iterator it = m_MapNameList.find( dwMapIDx );
	if( it == m_MapNameList.end() )
		return false;

	if( it->second != HashCode )
		return false;

	return true;
}
