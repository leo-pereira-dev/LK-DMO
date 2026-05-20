#pragma once

class cMainOption : public cBaseWindow
{
public:
	cMainOption();

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_MAINOPTION; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Create( int nValue = 0 );

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();

	virtual void			ResetDevice();
	virtual void			ResetMap(){ Close( false ); }

	virtual bool			OnEscapeKey(){ Close(); return true; }

protected:
	enum eMENU_ACTION
	{
		MA_NONE,
		MA_MAP,
		MA_SMALLMAP,
		MA_TAMER,
		MA_DIGIMON,
		MA_INVENTORY,
		MA_QUEST,
		MA_SEAL,
		MA_ENCYCLOPEDIA,
		MA_EVOLUTION_HELP,
		MA_CASH_WAREHOUSE,
		MA_CASHSHOP,
		MA_GUILD,
		MA_FRIEND,
		MA_MAIL,
		MA_REWARD_BOX,
		MA_INTERFACE,
		MA_GRAPHIC,
		MA_SOUND,
		MA_SHORTCUT,
		MA_GM_PANEL,
		MA_LOGOUT,
		MA_EXIT
	};

	enum
	{
		MAIN_BUTTON_COUNT = 16,
		OPTION_BUTTON_COUNT = 6
	};

	struct sMENU_BUTTON
	{
		cButton*		s_pButton;
		eMENU_ACTION	s_eAction;
	};

	cButton*		_AddMenuButton( CsPoint pos, CsPoint size, CsPoint texToken, char const* path, eMENU_ACTION action );
	void			_UpdateMenuButton( sMENU_BUTTON& buttonInfo, eMU_TYPE muReturn );
	void			_OnMenuAction( eMENU_ACTION action );
	void			_ToggleInventory();
	void			_ToggleZoneMap();
	void			_ToggleSmallMap();
	void			_OpenEncyclopedia();
	void			_RequestExit( int msgType );

protected:
	cButton*		m_pCancelButton;
	sMENU_BUTTON	m_MainButtons[ MAIN_BUTTON_COUNT ];
	sMENU_BUTTON	m_OptionButtons[ OPTION_BUTTON_COUNT ];
	int				m_nMainButtonCount;
	int				m_nOptionButtonCount;
};
