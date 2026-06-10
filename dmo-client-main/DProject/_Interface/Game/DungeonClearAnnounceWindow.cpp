#include "stdafx.h"
#include "DungeonClearAnnounceWindow.h"
#include "_GameIF.h"

namespace
{
	float const DUNGEON_CLEAR_ANNOUNCE_SECONDS = 1.45f;
	int const DUNGEON_CLEAR_ANNOUNCE_W = 418;
	int const DUNGEON_CLEAR_ANNOUNCE_H = 242;
}

cDungeonClearAnnounceWindow::cDungeonClearAnnounceWindow()
: m_fElapsedTime( 0.0f )
, m_bResultOpened( false )
{
}

void cDungeonClearAnnounceWindow::Destroy()
{
	cBaseWindow::Delete();
}

void cDungeonClearAnnounceWindow::DeleteResource()
{
	DeleteScript();
	m_fElapsedTime = 0.0f;
	m_bResultOpened = false;
}

void cDungeonClearAnnounceWindow::Create( int nValue /* = 0 */ )
{
	if( cBaseWindow::Init() == false )
		return;

	int const nWidth = DUNGEON_CLEAR_ANNOUNCE_W;
	int const nHeight = DUNGEON_CLEAR_ANNOUNCE_H;
	int const nPosX = ( g_nScreenWidth / 2 ) - ( nWidth / 2 );
	int const nPosY = ( g_nScreenHeight / 2 ) - ( nHeight / 2 );

	InitScript( "DungeonUI\\Dungeon_result_clear.tga", CsPoint( nPosX, nPosY ), CsPoint( nWidth, nHeight ), true, IFREGION_X::LEFT, IFREGION_Y::TOP, false );

	m_fElapsedTime = 0.0f;
	m_bResultOpened = false;
}

void cDungeonClearAnnounceWindow::Update(float const& fDeltaTime)
{
	UpdateScript(fDeltaTime);

	if( m_bResultOpened )
		return;

	m_fElapsedTime += fDeltaTime;
	if( m_fElapsedTime >= DUNGEON_CLEAR_ANNOUNCE_SECONDS )
		_OpenResultWindow();
}

cBaseWindow::eMU_TYPE cDungeonClearAnnounceWindow::Update_ForMouse()
{
	return MUT_NONE;
}

void cDungeonClearAnnounceWindow::Render()
{
	RenderScript();
}

void cDungeonClearAnnounceWindow::ResetDevice()
{
	ResetDeviceScript();
}

void cDungeonClearAnnounceWindow::SetResultData( cDungeonClearResultWindow::sResultData const& data )
{
	m_Data = data;
}

void cDungeonClearAnnounceWindow::_OpenResultWindow()
{
	m_bResultOpened = true;

	cDungeonClearResultWindow* pWindow = (cDungeonClearResultWindow*)g_pGameIF->GetDynamicIF( cBaseWindow::WT_DUNGEON_CLEAR_RESULT );
	if( pWindow )
		pWindow->SetResultData( m_Data );

	Close( false );
}
