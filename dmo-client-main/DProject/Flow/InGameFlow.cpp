//---------------------------------------------------------------------------
//
// 파일명 : LoginFlow.cpp
// 작성일 : 
// 작성자 : 
// 설  명 : 
//
//---------------------------------------------------------------------------
#include "StdAfx.h"
#include "InGameFlow.h"
#include "../_Interface/Game/Fade.h"
#include "../ContentsSystem/ContentsSystem.h"
#include "../../LibProj/CsFunc/CrashLogger.h"

//---------------------------------------------------------------------------

namespace Flow
{
	CInGameFlow::CInGameFlow(int p_iID) : CFlow(p_iID), m_bBgmPlay(false), m_pFadeUI(NULL)
	{
	}
	//---------------------------------------------------------------------------
	CInGameFlow::~CInGameFlow()
	{
	}
	//---------------------------------------------------------------------------
	BOOL CInGameFlow::Initialize()
	{
		nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame Initialize begin" );
		assert_cs( g_pEngine );	

		CURSOR_ST.InitCursor();

		if( g_pDataMng )
		{
			if( g_pDataMng )
			{
				cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
				if( pPostLoad )
				{
					if( g_pMngCollector && g_pMngCollector->IsSceneState(CMngCollector::eSCENE_TUTORIAL_EVENT) )
						pPostLoad->PostLoad_Tutorial();
					else
						pPostLoad->PostLoad();
				}
			}
		}

		m_pFadeUI = NiNew CFade;
		if( m_pFadeUI )
			m_pFadeUI->Init(CFade::FADE_IN, 0.5f);
		
		nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame Initialize end gameIF=%p charMng=%p dataMng=%p",
			g_pGameIF, g_pCharMng, g_pDataMng );
		return TRUE;
	}

	void CInGameFlow::OnEnter(void)
	{		
		nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame OnEnter begin gameIF=%p", g_pGameIF );
		CFlow::OnEnter();

		if( g_pGameIF )
		{
			g_pGameIF->GetMiniMap()->SetShowWindow( true );
			g_pGameIF->GetQuestHelper()->SetShowWindow( true );

#ifdef SIMPLEFICATION
			g_pGameIF->GetDynamicIF( cBaseWindow::WT_SIMPLE_BTN,0,0, false );
			g_pGameIF->GetSimple()->SetShowWindow( true );
#endif
		}
		CONTENTSSYSTEM_PTR->MakeWorldData();
		CONTENTSSYSTEM_PTR->MakeMainActorData();
		nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame OnEnter end" );
	}

	void CInGameFlow::OnExit(int p_iNextFlowID)
	{
		CFlow::OnExit(p_iNextFlowID);
		CONTENTSSYSTEM_PTR->ClearWorldData();
		CONTENTSSYSTEM_PTR->ClearMainActorData();
		CAMERA_ST.RestOcclusionGeometry();
	}

	void CInGameFlow::OnOverride(int p_iNextFlowID)
	{
		SetPaused( true );
		CONTENTSSYSTEM_PTR->ClearMainActorData();
		CONTENTSSYSTEM_PTR->OnOverride(p_iNextFlowID);
	}

	void CInGameFlow::OnResume(int p_iNextFlowID)
	{
		CURSOR_ST.InitCursor();

		if( p_iNextFlowID == CFlow::FLW_LOADING )
		{
			if( g_pDataMng )
			{
				cData_PostLoad* pPostLoad = g_pDataMng->GetPostLoad();
				if( pPostLoad )
				{
					if( g_pMngCollector && g_pMngCollector->IsSceneState( CMngCollector::eSCENE_TUTORIAL_EVENT ) )
						pPostLoad->PostLoad_Tutorial();
					else
						pPostLoad->PostLoad();
				}
			}
		}

		SetPaused( false );	

		if( p_iNextFlowID != CFlow::FLW_BATTLE_RESULT )
		{
			if( g_pGameIF )
			{
				g_pGameIF->GetMiniMap()->SetShowWindow( true );
				g_pGameIF->GetQuestHelper()->SetShowWindow( true );
#ifdef SIMPLEFICATION
				g_pGameIF->GetDynamicIF( cBaseWindow::WT_SIMPLE_BTN,0,0, false );
				g_pGameIF->GetSimple()->SetShowWindow( true );
#endif
			}
		}

		if( m_pFadeUI )
			m_pFadeUI->Reset(CFade::FADE_IN, 0.5f);

		CONTENTSSYSTEM_PTR->MakeMainActorData();
		CONTENTSSYSTEM_PTR->OnResume(p_iNextFlowID);
	}

	void CInGameFlow::ReservedChangeFlow(int p_iNextFlowID)
	{
		if( m_pFadeUI )
			m_pFadeUI->Reset(CFade::FADE_OUT, 0.5f);
	}

	void CInGameFlow::ReservedPushFlow(int p_iNextFlowID)
	{
		if( m_pFadeUI )
			m_pFadeUI->Reset(CFade::FADE_OUT, 0.5f);
	}

	//---------------------------------------------------------------------------
	void CInGameFlow::Terminate()
	{
		nsCsGBTerrain::g_nSvrLibType = nLIB::SVR_GAME;

		CURSOR_ST.InitCursor();

		SAFE_NIDELETE(m_pFadeUI);
		// 데이터 초기화
		if( g_pDataMng )
			g_pDataMng->LogOut();

		if( g_pMngCollector )
		{
			g_pMngCollector->ReleaseScene();
			g_pMngCollector->EndDatsCenter();
			g_pMngCollector->ResetMap();
		}

		// 음 어쩔수 없이 예외처리 ㅠㅠ 젠장
		SAFE_NIDELETE( g_pGameIF );		

		if( g_pHelpArrow )
			g_pHelpArrow->ReleaseArrowPoint();

		// 외부 포인터 없애 준다.
		if( g_pThread )
			g_pThread->GetResMng()->ReleaseConnetTerrain();				

		// 지형 제거
		if( nsCsGBTerrain::g_pTRMng )
			nsCsGBTerrain::g_pTRMng->DeleteRoot( &nsCsGBTerrain::g_pCurRoot );

		// 지형 라이트를 받는 NPC리셋
		if( g_pNpcMng )
			g_pNpcMng->ResetOldMap();

		net::stop();
		net::prev_map_no = 0;

		// 레지스트리 저장
		if( g_pResist )
			g_pResist->SaveChar();

		// 케릭터 정보 저장.
		if(OPTIONMNG_PTR)
			OPTIONMNG_PTR->SaveCharOption(OPTIONFILE_NAME);

		// 바코드
#ifdef USE_BARCODE_REDER
		nsBARCODE::LogOut();
#endif
		g_bFirstLoding = true;
		RESOURCEMGR_ST.CleanUpResource();
	}
	//---------------------------------------------------------------------------
	bool CInGameFlow::LostDevice(void* p_pvData)
	{
		return true;
	}
	//---------------------------------------------------------------------------
	bool CInGameFlow::ResetDevice(bool p_bBeforeReset, void* p_pvData)
	{
		return true;
	}
	//---------------------------------------------------------------------------

	BOOL CInGameFlow::OnMsgHandler(const MSG& p_kMsg)
	{
		CURSOR_ST.Input(p_kMsg);
		GLOBALINPUT_ST._Keyboard_Input( p_kMsg );	

		return FALSE;
	}

	void CInGameFlow::InputFrame()
	{

	}
	//---------------------------------------------------------------------------
	void CInGameFlow::UpdateFrame()
	{
		static int s_traceFrame = 0;
		const bool traceFrame = ( s_traceFrame < 30 );
		if( traceFrame )
			nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] begin gameIF=%p charMng=%p",
				s_traceFrame, g_pGameIF, g_pCharMng );

		CONTENTSSYSTEM_PTR->Update( g_fDeltaTime );
		if( traceFrame )
			nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] after ContentsSystem", s_traceFrame );

		if( g_pSystemMsg )
			g_pSystemMsg->Update();

		if( g_pSkillMsg )
			g_pSkillMsg->Update();

		if( TOOLTIPMNG_STPTR )
			TOOLTIPMNG_STPTR->Update( g_fDeltaTime );
		if( traceFrame )
			nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] after UI messages/tooltips", s_traceFrame );

		if( m_pFadeUI )
			m_pFadeUI->Update(g_fDeltaTime);

#ifdef USE_BARCODE_REDER
		nsBARCODE::Update();
#endif
		if( net::game )
			nsCsGBTerrain::g_ServerTime.s_nServerTime = net::game->GetTimeTS();

		if( g_pGameIF )
		{			
			if( traceFrame )
				nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] before GameIF Update", s_traceFrame );
 			cGlobalInput::eMOUSE_INPUT mi = g_pGameIF->Update(g_fDeltaTime);
			if( traceFrame )
				nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] after GameIF Update mouse=%d", s_traceFrame, (int)mi );
			GLOBALINPUT_ST.KeyboardUpdate();
			GLOBALINPUT_ST._Mouse_Input(mi);
 			g_pMngCollector->Update( mi );					
			if( traceFrame )
				nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] after Collector Update", s_traceFrame );
		}

		cIconMng::GlobalUpdate();
		CAMERA_ST.Update( g_fDeltaTime );
		if( traceFrame )
			nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] after Camera/Icon", s_traceFrame );

		if( g_pWeather )
		{
			g_pWeather->SetPos( g_pCharMng->GetTamerUser()->GetPos() );
			g_pWeather->Update( g_fDeltaTime, g_pEngine->GetD3DView() );	
		}	
		
		CURSOR_ST.LoopReset();	//2016-02-23-nova
		if( traceFrame )
			nsCSDEBUG::CrashLogger::LogMessage( "FLOW InGame UpdateFrame[%d] end", s_traceFrame );
		++s_traceFrame;
	}
	//---------------------------------------------------------------------------
	void CInGameFlow::CullFrame()
	{
	}
	//---------------------------------------------------------------------------
	void CInGameFlow::RenderSceneFrame()
	{// 3D Draw
		switch( g_pEngine->GetPostEffect() )
		{
		case CEngine::PE_NONE:
		case CEngine::PE_GAUSSIAN_BLUR:
			{
				if( nsCsGBTerrain::g_bShadowRender == true )
					g_pMngCollector->Render_Shadow( true );
				else
					g_pMngCollector->Render( true );
				g_pEngine->Render();
			}
			break;
		}
	}

	bool CInGameFlow::BeginRenderTarget()
	{
		// BeginTarget
		return g_pEngine->BeginUsingCurrentRenderTargetGroup( NiRenderer::CLEAR_ALL );
	}

	void CInGameFlow::RenderBackScreenFrame()
	{
	}

	//---------------------------------------------------------------------------
	void CInGameFlow::RenderScreenFrame()// 2D Draw
	{
	}
	//---------------------------------------------------------------------------
	void CInGameFlow::RenderUIFrame()
	{
		g_pEngine->ScreenSpace();

		if( g_pMngCollector )
			g_pMngCollector->Render_PostIF();

		if( g_pGameIF )
			g_pGameIF->Render();

		if( g_pSystemMsg )
			g_pSystemMsg->Render();

		if( g_pSkillMsg )
			g_pSkillMsg->Render();

		cBaseWindow* pWin = g_pGameIF->_GetPointer( cBaseWindow::WT_PUBLICITY, 0 );
		if( pWin && pWin->IsShowWindow() )
			pWin->Render();

		if( TOOLTIPMNG_STPTR )
			TOOLTIPMNG_STPTR->Render();

		cMessageBox::RenderList();

#ifndef _GIVE
		if( g_pMngCollector )
			g_pMngCollector->RenderText();
#endif

		CURSOR_ST.Render();

		if( m_pFadeUI )
			m_pFadeUI->Render();
	}

	BOOL CInGameFlow::LoadResource()
	{
		return TRUE;
	}

	void CInGameFlow::ReleaseResource()
	{
	}
}
//---------------------------------------------------------------------------
