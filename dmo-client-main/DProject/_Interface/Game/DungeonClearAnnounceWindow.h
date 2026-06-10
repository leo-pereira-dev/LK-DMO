#pragma once

#include "DungeonClearResultWindow.h"

class cDungeonClearAnnounceWindow : public cBaseWindow
{
public:
	cDungeonClearAnnounceWindow();

	virtual eWINDOW_TYPE GetWindowType(){ return WT_DUNGEON_CLEAR_ANNOUNCE; }
	virtual void Destroy();
	virtual void DeleteResource();
	virtual void Create( int nValue = 0 );
	virtual void Update(float const& fDeltaTime);
	virtual eMU_TYPE Update_ForMouse();
	virtual void Render();
	virtual void ResetDevice();
	virtual void ResetMap(){ Close( false ); }
	virtual bool OnEscapeKey(){ return true; }

	void SetResultData( cDungeonClearResultWindow::sResultData const& data );

private:
	void _OpenResultWindow();

private:
	cDungeonClearResultWindow::sResultData m_Data;
	float m_fElapsedTime;
	bool m_bResultOpened;
};
