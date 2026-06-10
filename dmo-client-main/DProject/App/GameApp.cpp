//---------------------------------------------------------------------------
//
// 파일명 : GameApp.cpp
// 작성일 : 
// 작성자 : 
// 설  명 :
//
//---------------------------------------------------------------------------

#include "StdAfx.h"
#include "GameApp.h"
#include "../../LibProj/CsFunc/CrashLogger.h"
#include "../../LibProj/CsFilePack/CsFilePackSystem.h"

#include "../Flow/FlowMgr.h"
#include "../Flow/Flow.h"

#include "../ContentsSystem/ContentsSystemDef.h"
#include "../ContentsSystem/ContentsSystem.h"

#ifdef DEF_CORE_NPROTECT
#include "../nProtect/Client_nProtect.h"
#define CHECK_NPROTECT_TIME 5.0f
#endif

CsCriticalSection	__g_GlobalCS;

int					__g_nGlobalState = 0;

void SetGlobalState( int nState )
{
	__g_GlobalCS.Lock();
	__g_nGlobalState = nState;
	__g_GlobalCS.Unlock();
}

int GetGlobalState()
{	
	__g_GlobalCS.Lock();
	int nReturn = __g_nGlobalState;
	__g_GlobalCS.Unlock();
	return nReturn;
}
void Thread_LoadFileTable()
{
	nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD begin" );

	nsCsMapTable::g_bUseMapStart = false;
	nsCsMapTable::g_bUseMapResurrection = false;
	nsCsMapTable::g_eModeMapMonster = nsCsMapTable::eMode_Client;

	nsCsFileTable::g_bAddExp = true;
	nsCsFileTable::g_bUseMoveObject = true;
	nsCsFileTable::g_bUseHelp = true;
	nsCsFileTable::g_bUseAchieve = true;
	nsCsFileTable::g_bAddExp = true;
	nsCsFileTable::g_bBuffMng = true;
	nsCsFileTable::g_bSceneDataMng = true;

#ifndef BATTLE_MATCH
 	nsCsFileTable::g_bUseEvoExtra = false;
 	nsCsFileTable::g_bUseEvoBattle = false;
#endif

	nsCsFileTable::g_bUseCashShop = true;
	nsCsFileTable::g_bUseGotcha = true;

	if( GLOBALDATA_ST.IsCountry( GData::eCountry_Aeria ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::ENGLISH_A;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_GSP ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::ENGLISH;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_Steam ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::ENGLISH;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_Kor ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::KOREA_TRANSLATION;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_Hongkong ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::HONGKONG;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_Taiwan ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::TAIWAN;
	else if( GLOBALDATA_ST.IsCountry( GData::eCountry_Thailand ) )
		g_pResist->m_Global.s_eFTLanguage = nsCsFileTable::THAILAND;

#ifdef _DEBUG
	if( g_bUseFilePack )
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_FILEPACK;
	else
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_BIN;
#elif defined _GIVE
	if( g_bUseFilePack )
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_FILEPACK;
	else
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_BIN;
#else
	if( g_bUseFilePack )
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_FILEPACK;
	else
		nsCsFileTable::g_eFileType = nsCsFileTable::FT_BIN;
#endif
#ifdef PC_BANG_SERVICE_TEST
	nsCsFileTable::g_eFileType = nsCsFileTable::FT_FILEPACK;
#endif

	nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD Init begin fileType=%d language=%d usePack=%d", nsCsFileTable::g_eFileType, g_pResist->m_Global.s_eFTLanguage, g_bUseFilePack ? 1 : 0 );
	if( nsCsFileTable::g_FileTableMng.Init( nsCsFileTable::g_eFileType, g_pResist->m_Global.s_eFTLanguage ) == false )
	{
		nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD Init failed" );
		SetGlobalState( 2 );
		return;
	}
	nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD Init end" );

#ifndef _DEBUG
#ifndef _GIVE
#ifndef PC_BANG_SERVICE_TEST
	if( nsCsFileTable::g_eFileType != nsCsFileTable::FT_FILEPACK )
	{
		char cPath[ MAX_PATH ];
		nsCsFileTable::g_FileTableMng.GetLanguagePath( g_pResist->m_Global.s_eFTLanguage, cPath );
		nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD Quest reload begin path=%s", cPath );
		bool bResult = nsCsFileTable::g_pQuestMng->Reload( cPath );
		assert_cs( bResult );
		nsCsMapTable::g_pMapMonsterMng->Reload( cPath );
		nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD Quest reload end" );
	}
#endif
#endif
#endif

	SetGlobalState( 1 );
	nsCSDEBUG::CrashLogger::LogMessage( "FILETABLE_THREAD success state=1" );
}
class CsPackNiFile : public NiFile
{
	NiDeclareDerivedBinaryStream();

public:
	virtual void SetEndianSwap( bool bDoSwap );

	CsPackNiFile( char* pData, unsigned int uiSize )
		: m_pPackData( pData )
		, m_uiPackSize( uiSize )
		, m_uiPackPos( 0 )
	{
		m_pBuffer = NULL;
		m_pFile = NULL;
		m_eMode = READ_ONLY;
		m_bGood = ( m_pPackData != NULL && m_uiPackSize > 0 );
		m_uiBufferAllocSize = 0;
		m_uiBufferReadSize = 0;
		m_uiPos = 0;
		m_uiAbsoluteCurrentPos = 0;
		SetEndianSwap( false );
	}

	virtual ~CsPackNiFile()
	{
		SAFE_DELETE_ARRAY( m_pPackData );
	}

	virtual operator bool() const
	{
		return m_bGood;
	}

	virtual void Seek( int iNumBytes )
	{
		Seek( iNumBytes, ms_iSeekCur );
	}

	virtual void Seek( int iOffset, int iWhence )
	{
		int iNewPos = 0;

		if( iWhence == ms_iSeekSet )
			iNewPos = iOffset;
		else if( iWhence == ms_iSeekCur )
			iNewPos = static_cast<int>( m_uiPackPos ) + iOffset;
		else if( iWhence == ms_iSeekEnd )
			iNewPos = static_cast<int>( m_uiPackSize ) + iOffset;
		else
			return;

		if( iNewPos < 0 )
			iNewPos = 0;

		if( static_cast<unsigned int>( iNewPos ) > m_uiPackSize )
			iNewPos = static_cast<int>( m_uiPackSize );

		m_uiPackPos = static_cast<unsigned int>( iNewPos );
		m_uiAbsoluteCurrentPos = m_uiPackPos;
	}

	virtual unsigned int GetFileSize() const
	{
		return m_uiPackSize;
	}

protected:
	unsigned int MemRead( void* pvBuffer, unsigned int uiBytes )
	{
		if( !m_bGood || pvBuffer == NULL )
			return 0;

		unsigned int uiLeft = m_uiPackSize - m_uiPackPos;
		if( uiBytes > uiLeft )
			uiBytes = uiLeft;

		if( uiBytes > 0 )
		{
			memcpy( pvBuffer, m_pPackData + m_uiPackPos, uiBytes );
			m_uiPackPos += uiBytes;
		}

		return uiBytes;
	}

	unsigned int MemWrite( const void*, unsigned int )
	{
		return 0;
	}

private:
	char* m_pPackData;
	unsigned int m_uiPackSize;
	unsigned int m_uiPackPos;
};

NiImplementDerivedBinaryStream( CsPackNiFile, MemRead, MemWrite );

NiFile* CsFilePackFileCreateFunc( const char* pcName, NiFile::OpenMode eMode, unsigned int uiBufferSize )
{
	if( eMode == NiFile::READ_ONLY && pcName != NULL && CsFPS::CsFPSystem::IsExistOnlyPack( 0, pcName ) )
	{
		char* pData = NULL;
		size_t nDataSize = CsFPS::CsFPSystem::Allocate_GetFileData( 0, &pData, pcName );
		if( nDataSize > 0 && pData != NULL )
			return NiNew CsPackNiFile( pData, static_cast<unsigned int>( nDataSize ) );

		SAFE_DELETE_ARRAY( pData );
	}

	return NiNew NiFile( pcName, eMode, uiBufferSize );
}

bool CsFilePackFileAccessFunc( const char* pcName, NiFile::OpenMode eMode )
{
	if( eMode == NiFile::READ_ONLY && pcName != NULL && CsFPS::CsFPSystem::IsExistOnlyPack( 0, pcName ) )
		return true;

	NiFile kFile( pcName, eMode, 0 );
	return kFile ? true : false;
}

namespace App
{
	//---------------------------------------------------------------------------
	CGameApp::CGameApp(): m_bInitialStarting(false), m_fCheckGameGuardTimer(0.0f)
	{
	}
	//---------------------------------------------------------------------------
	CGameApp::~CGameApp()
	{
	}
	//---------------------------------------------------------------------------
	BOOL CGameApp::OnInitialize()
	{
		CREATE_SINGLETON(GameEventMng);

		CREATE_SINGLETON(Flow::CFlowMgr);
		if(!FLOWMGR_ST.Create())
		{
			assert(!"Created FlowMgr Failed!");
			return FALSE;
		}

		CREATE_SINGLETON(COptionMng);	// 파일 매니저

		if( OPTIONMNG_PTR )
			OPTIONMNG_PTR->LoadMachineOption();
		CREATE_SINGLETON(ContentsSystem);
		SAFE_POINTER_RETVAL( CONTENTSSYSTEM_PTR, FALSE );

		if(CONTENTSSYSTEM_PTR)
		{
			CONTENTSSYSTEM_PTR->BuildContents();
			CONTENTSSYSTEM_PTR->IntraConnection();
		}

 		CREATE_SINGLETON( ResourceMng );
 		if( RESOURCEMGR_STPTR )
 			RESOURCEMGR_ST.Init();

		CREATE_SINGLETON(cGlobalInput);

		CREATE_SINGLETON( CCursor );
		if( CURSOR_STPTR )
			CURSOR_ST.Init(m_hInstance);

		// 엔진의 렌더러 생성
		if( CEngine::Init() == false )
			return FALSE;

		DxResolutionInfo::GlobalInit();

		if( g_pEngine->Create() == false )
			return FALSE;

		g_pResist->m_Global.CheckResolution();
		CalculateSize( g_pResist->m_Global.s_nResolutionWidth, g_pResist->m_Global.s_nResolutionHeight, g_pResist->m_Global.s_bFullScreen );

		ReSize( GetWidth(), GetHeight(), GetFullMode(), true );
		
		g_nScreenWidth = GetWidth();
		g_nScreenHeight = GetHeight();

		g_pEngine->ChangeResolutionMode( GetFullMode(), GetWidth(), GetHeight() );

 		//CREATE_SINGLETON( CClock )
 		//if(!CLOCK_ST.Create( NiNew CTimeSourceNi ) )
 		//{
 		//	assert(!"Created Clock Failed!");
 		//	return FALSE;
 		//}
 
 		////------------------------------------------------
 		//// Init Setup
 		//CLOCK_ST.SetMaxFrameRate(0.0f);
 		//CLOCK_ST.SetCheckFps(TRUE);

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP OnInitialize pre-filetable begin usePack=%d", g_bUseFilePack ? 1 : 0 );

		if( g_bUseFilePack )
		{
			nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP FilePack callbacks begin" );
			NiFile::SetFileCreateFunc( CsFilePackFileCreateFunc );
			NiFile::SetFileAccessFunc( CsFilePackFileAccessFunc );
			nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP FilePack callbacks end" );
		}

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Collector ShotInit begin" );
		CMngCollector::ShotInit();
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Collector ShotInit end" );

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP LoadFileTable enqueue begin" );
		sTCUnit* pUnit = sTCUnit::NewInstance( sTCUnit::LoadFileTable );
		pUnit->s_pLoadedObject = NULL;
		g_pThread->LoadChar( pUnit );
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP LoadFileTable enqueue end" );

		bool bLoad = true;
		int nWaitLoop = 0;
		while( bLoad )
		{
			int nState = GetGlobalState();
			switch( nState )
			{
			case 0:
				if( ( nWaitLoop % 10 ) == 0 )
				{
					nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP waiting filetable loops=%d state=%d", nWaitLoop, nState );
				}
				++nWaitLoop;
				Sleep( 100 );
				break;
			case 1:
				nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP filetable success loops=%d", nWaitLoop );
				bLoad = false;
				break;
			case 2:
				nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP filetable failed loops=%d", nWaitLoop );
				bLoad = false;
				CsMessageBox( MB_OK, _LAN( "파일테이블이 잘못 되었습니다" ) );
				return false;
			}
		}

		if( GAME_EVENT_STPTR )
			GAME_EVENT_STPTR->OnEvent(EVENT_CODE::TABLE_LOAD_SUCCESS);

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Icon/Data GlobalInit begin" );
		cIconMng::GlobalInit();
		cDataMng::GlobalInit();
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Icon/Data GlobalInit end" );

		nsCsGBTerrain::g_eTexFilter = (NiTexturingProperty::FilterMode)g_pResist->m_Global.s_eTexFilter;
		nsCsGBTerrain::g_bShadowRender = ( g_pResist->m_Global.s_nShadowType == cResist::sGLOBAL::SHADOW_ON );
		nsCsGBTerrain::g_bCharOutLine = g_pResist->m_Global.s_bCharOutLine;
		nsCsGBTerrain::g_bSpeedCellRender = g_pResist->m_Global.s_bCell;
		g_pWeather->SetPerformance( g_pResist->m_Global.s_nWeather );
		CsC_AvObject::g_bEnableVoice = g_pResist->m_Global.s_bEnableVoice;

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Tooltip init begin" );
		CREATE_SINGLETON( CToolTipMng );
		if( TOOLTIPMNG_STPTR )
			TOOLTIPMNG_ST.Init();
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP Tooltip init end" );

		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP net start begin" );
		net::start();
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP net start end" );

#ifndef SKIP_LOGO
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP StartFlow logo begin" );
		FLOWMGR_ST.StartFlow(Flow::CFlow::FLW_LOGO);
#else
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP StartFlow login begin" );
		FLOWMGR_ST.StartFlow(Flow::CFlow::FLW_LOGIN);
#endif
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP StartFlow end" );

#ifdef DEF_CORE_NPROTECT
		m_fCheckGameGuardTimer = CHECK_NPROTECT_TIME;
#endif
		nsCSDEBUG::CrashLogger::LogMessage( "GAMEAPP OnInitialize end" );
		return TRUE;
	}
	//---------------------------------------------------------------------------
	void CGameApp::OnIdleExtern()
	{			
#ifdef VERSION_STEAM
		STEAM_ST.SteamCallbackUpdate();
#endif
		// 네트웍 업데이트
		net::execute();

#ifdef DEF_CORE_NPROTECT
		m_fCheckGameGuardTimer -= g_fDeltaTime;
		if( m_fCheckGameGuardTimer < 0.0f )
		{
			m_fCheckGameGuardTimer = CHECK_NPROTECT_TIME;
			nProtect_Check();
		}
#endif
	}

	//---------------------------------------------------------------------------
	void CGameApp::OnIdle()
	{
		// Flow Idle
		if(FLOWMGR_STPTR)
			FLOWMGR_ST.OnIdle();
	}
	//---------------------------------------------------------------------------
	void CGameApp::OnTerminate()
	{	
	//	SAFE_NIDELETE( g_pLoading );
		if( TOOLTIPMNG_STPTR )
			DESTROY_SINGLETON( TOOLTIPMNG_STPTR );

		BHPRT( "TOOLTIPMNG_STPTR" );

		if( CONTENTSSYSTEM_PTR )
		{
			CONTENTSSYSTEM_PTR->ClearWorldData();
			CONTENTSSYSTEM_PTR->ClearMainActorData();
			BHPRT( "CONTENTSSYSTEM_PTR" );
		}

		if( FLOWMGR_STPTR )
		{
			FLOWMGR_STPTR->LockFlow();
			FLOWMGR_STPTR->Destroy();
			DESTROY_SINGLETON(FLOWMGR_STPTR);
			BHPRT( "FLOWMGR_STPTR" );
		}

		g_Sorting.Delete();
		BHPRT( "g_Sorting.Delete()" );

		SAFE_NIDELETE( g_pGameIF );

		BHPRT( "g_pGameIF" );
		if( CONTENTSSYSTEM_PTR )
		{
			CONTENTSSYSTEM_PTR->PrepareDestroy();
			CONTENTSSYSTEM_PTR->RemoveAll();
			DESTROY_SINGLETON( CONTENTSSYSTEM_PTR );	
			BHPRT( "CONTENTSSYSTEM_PTR" );	
		}

		if( OPTIONMNG_PTR )
			DESTROY_SINGLETON(OPTIONMNG_PTR);
		BHPRT( "OPTIONMNG_PTR" );	

		if( CURSOR_STPTR )
			DESTROY_SINGLETON( CURSOR_STPTR );
		BHPRT( "CURSOR_STPTR" );	

		cIconMng::GlobalShotdown();		
		BHPRT( "cIconMng::GlobalShotdown()" );
		CMngCollector::ShotDown();	
		BHPRT( "CMngCollector::ShotDown()" );	
		DxResolutionInfo::GlobalShotDown();
		BHPRT( "DxResolutionInfo::GlobalShotDown()" );
		nsDIRECTSHOW::GlobalShotDown();
		BHPRT( "nsDIRECTSHOW::GlobalShotDown()" );

		cGameInterface::GlobalShotDown();
		BHPRT( "cGameInterface::GlobalShotDown()" );
		nsCsFileTable::g_FileTableMng.Delete();	
		BHPRT( "nsCsFileTable::g_FileTableMng.Delete()" );

		if( CURSOR_STPTR )
			DESTROY_SINGLETON( CURSOR_STPTR );	
		BHPRT( "CURSOR_STPTR" );

		if( GLOBALINPUT_STPTR )
			DESTROY_SINGLETON( GLOBALINPUT_STPTR );	
		BHPRT( "GLOBALINPUT_STPTR" );

// 		// Clock Destroy
// 		if(CLOCK_STPTR)
// 			CLOCK_ST.Destroy();
// 		DESTROY_SINGLETON(CLOCK_STPTR);

		if( GAME_EVENT_STPTR )
			DESTROY_SINGLETON(GAME_EVENT_STPTR);
		BHPRT( "GAME_EVENT_STPTR" );

		if( RESOURCEMGR_STPTR )
		{
			RESOURCEMGR_STPTR->End();
			DESTROY_SINGLETON(RESOURCEMGR_STPTR);
		}
	}
	//---------------------------------------------------------------------------
	BOOL CGameApp::OnMsgHandler(const MSG& p_kMsg)
	{		
		switch( p_kMsg.message )
		{
		case WM_CTLCOLOREDIT:
			{
				if( g_pGameIF )
				{
					if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_CHAT_MAIN_WINDOW ) )
					{
						ChatContents* pChatCon = (ChatContents*)CONTENTSSYSTEM_PTR->GetContents(E_CT_CHATTING_STANDARDIZATION);
						if(pChatCon && (HWND)p_kMsg.lParam == pChatCon->_GetEditHwnd() )
						{							
 							return pChatCon->_ApplyEditColor( (HDC)p_kMsg.wParam );
						}
					}

					if( g_pMoneySeparate )
					{
						LRESULT hr = g_pMoneySeparate->ApplyEditColor( (HWND)p_kMsg.lParam, (HDC)p_kMsg.wParam );
						if( hr != NULL )
						{
							return hr;
						}
					}

					if( g_pItemSeparate )
					{
						LRESULT hr = g_pItemSeparate->ApplyEditColor( (HWND)p_kMsg.lParam, (HDC)p_kMsg.wParam );
						if( hr != NULL )
						{
							return hr;
						}
					}
					if( g_pGameIF->IsActiveWindow( cBaseWindow::WT_MASTERS_MATCHING ) )
					{
						LRESULT hr = g_pGameIF->GetMastersMatching()->ApplyEditColor( (HWND)p_kMsg.lParam, (HDC)p_kMsg.wParam );
						if( hr != NULL )
						{
							return hr;
						}
					}
				}			
			}
			break;
		case WM_DISPLAYCHANGE:
			{
				if( !m_bFullMode )
					::SystemParametersInfo( SPI_GETWORKAREA, 0, &m_uiWindowModeWorkSpaceRect, 0 );
			}break;		
		}

		if( FLOWMGR_STPTR && FLOWMGR_ST.OnMsgHandler( p_kMsg ) )
			return TRUE;

		return FALSE;
	}
	//---------------------------------------------------------------------------
	bool CGameApp::LostDevice(void* p_pvData)
	{
		// Flow LostDevice
		if( FLOWMGR_STPTR && !FLOWMGR_ST.LostDevice(p_pvData))
			return false;

		return true;
	}
	//---------------------------------------------------------------------------
	bool CGameApp::ResetDevice(bool p_bBeforeReset, void* p_pvData)
	{
		// Flow ResetDevice
		if( FLOWMGR_STPTR && !FLOWMGR_ST.ResetDevice(p_bBeforeReset, p_pvData))
			return false;

		return true;
	}
	//---------------------------------------------------------------------------
	bool CGameApp::RecreateDevice(WORD wWidth, WORD wHeight, BYTE byBit, BYTE RefRate, bool bFullMode)
	{
		bool bScreenModeChanged = ( static_cast<bool>(m_bFullMode) != bFullMode );

		CalculateSize( wWidth, wHeight, bFullMode );

		//if( GBRENDERER_STPTR )
		//	GBRENDERER_STPTR->Recreate( GetWidth(), GetHeight(), GetFullMode() );

		ReSize( GetWidth(), GetHeight(), GetFullMode(), bScreenModeChanged );

// 		if( GBCAMERAMGR_STPTR )
// 		{
// 			GbCamera* pkGbCamera = NiNew GbCamera;
// 			if(!pkGbCamera->Create(GetWidth(), GetHeight()))
// 			{
// 				assert(!"Created GbCamera Failed!");
// 				return FALSE;
// 			}
// 
// 			GbCamera* pkOldCamera = GBCAMERAMGR_STPTR->GetGbCamera(GAME_CAMERA);
// 			pkGbCamera->SetPitch( pkOldCamera->GetPitch() );
// 			pkGbCamera->SetYaw( pkOldCamera->GetYaw() );
// 			pkGbCamera->SetRoll( pkOldCamera->GetRoll() );
// 			pkGbCamera->SetHeight( pkOldCamera->GetHeight() );
// 			pkGbCamera->SetDist( pkOldCamera->GetDist() );
// 			pkGbCamera->SetOriginDist( pkOldCamera->GetOriginDist() );
// 			pkGbCamera->SetDestDist( pkOldCamera->GetDestDist() );
// 			pkGbCamera->SetSpinPitch( pkOldCamera->GetSpinPitch() );
// 			pkGbCamera->SetSpinYaw( pkOldCamera->GetSpinYaw() );
// 			pkGbCamera->SetSpinRoll( pkOldCamera->GetSpinRoll() );
// 			pkGbCamera->Update();
// 
// 			GBCAMERAMGR_STPTR->DelGbCamera(GAME_CAMERA);
// 			GBCAMERAMGR_STPTR->AddGbCamera(GAME_CAMERA, pkGbCamera);
// 
// 			//[11/5/2010 passion] 
// 			// 캐릭터 셀렉트와 크레이트에서 사용하기위한 카메라
// 			GbCamera* pkGbViewCamera = NiNew GbCamera;
// 			if(!pkGbViewCamera->Create(GetWidth(), GetHeight()))
// 			{
// 				assert(!"Created GbCamera Failed!");
// 				return FALSE;
// 			}
// 
// 			pkGbCamera = GBCAMERAMGR_STPTR->GetGbCamera(VIEW_CAMERA);
// 			pkGbViewCamera->SetPitch( pkGbCamera->GetPitch() );
// 			pkGbViewCamera->SetYaw( pkGbCamera->GetYaw() );
// 			pkGbViewCamera->SetRoll( pkGbCamera->GetRoll() );
// 			pkGbViewCamera->SetHeight( pkGbCamera->GetHeight() );
// 			pkGbViewCamera->SetDist( pkGbCamera->GetDist() );
// 			pkGbViewCamera->SetOriginDist( pkGbCamera->GetOriginDist() );
// 			pkGbViewCamera->SetDestDist( pkGbCamera->GetDestDist() );
// 			pkGbViewCamera->SetSpinPitch( pkGbCamera->GetSpinPitch() );
// 			pkGbViewCamera->SetSpinYaw( pkGbCamera->GetSpinYaw() );
// 			pkGbViewCamera->SetSpinRoll( pkGbCamera->GetSpinRoll() );
// 			pkGbViewCamera->Update();
// 
// 			GBCAMERAMGR_STPTR->DelGbCamera(VIEW_CAMERA);
// 			GBCAMERAMGR_STPTR->AddGbCamera(VIEW_CAMERA, pkGbViewCamera);
// 
// 			//[8/24/2011 passion] 
// 			// UI 카메라
// 
// 			if( pkGbCamera )
// 			{
// 				GbCamera* pkGbUICamera = NiNew GbCamera;
// 				const NiRenderTargetGroup* pkRTGroup = 
// 					NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup();
// 
// 				UINT uiWidth = pkRTGroup->GetWidth(0);
// 				UINT uiHeight = pkRTGroup->GetHeight(0);
// 
// 				if(!pkGbUICamera->Create(uiWidth, uiHeight ) )
// 				{
// 					assert(!"Created GbCamera Failed!");
// 					return FALSE;
// 				}
// 
// 				pkGbUICamera->SetOrtho(true);
// 
// 				NiFrustum m_kCachedFrustum;
// 				float fAspectRatio = 0.75f;
// 				float fXScale = static_cast<float>(uiWidth)/1024.0f ;
// 				float fYScale = static_cast<float>(uiHeight)/768.0f ;
// 
// 				const float fDoubleWidth = 2.0f * (static_cast<float>(pkRTGroup->GetWidth(0)));
// 				const float fDoubleHeight = 2.0f * (static_cast<float>(pkRTGroup->GetHeight(0)));
// 				m_kCachedFrustum.m_fLeft = (-0.5f + 1.0f / fDoubleWidth)*fXScale;
// 				m_kCachedFrustum.m_fRight = (0.5f + 1.0f / fDoubleWidth)*fXScale;
// 				m_kCachedFrustum.m_fTop = (0.5f - 1.0f / fDoubleHeight)*fAspectRatio*fYScale;
// 				m_kCachedFrustum.m_fBottom = (-0.5f - 1.0f / fDoubleHeight)*fAspectRatio*fYScale;
// 				m_kCachedFrustum.m_fNear = 1.0f;
// 				m_kCachedFrustum.m_fFar = 10000.0f;
// 				m_kCachedFrustum.m_bOrtho = true;
// 
// 				pkGbUICamera->GetCamera()->SetViewFrustum(m_kCachedFrustum);
// 				pkGbUICamera->Update();
// 
// 				GBCAMERAMGR_STPTR->DelGbCamera(UI_CAMERA);
// 				GBCAMERAMGR_STPTR->AddGbCamera(UI_CAMERA, pkGbUICamera);
// 
// 				GUIMGR_STPTR->SetUiCamera(pkGbUICamera->GetCamera());
// 			}
// 		}
		return true;
	}
}

//---------------------------------------------------------------------------
