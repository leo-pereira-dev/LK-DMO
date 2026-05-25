#pragma once

class cGMPanel : public cBaseWindow
{
public:
	cGMPanel();

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_GM_PANEL; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Create( int nValue = 0 );

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();

	virtual void			ResetDevice();
	virtual void			ResetMap(){ Close( false ); }
	virtual bool			OnEscapeKey(){ Close(); return true; }

private:
	enum eGMPage
	{
		PAGE_MAKE_ITEM,
		PAGE_SUMMON_MONSTER,
		PAGE_DESBUG_CHAR,
		PAGE_LEVEL_UP,
		PAGE_LEVEL_UP_DIGI,
		PAGE_ALL_EVO,
		PAGE_FULL_CLONE,
		PAGE_BITS,
		PAGE_GOD_ON,
		PAGE_GOD_OFF,
		PAGE_KICK,
		PAGE_BAN,
		PAGE_UNBAN,
		PAGE_MEMBERSHIP,
		PAGE_COUNT
	};

	enum { MAX_ITEM_RESULTS = 7 };
	enum { MAX_MENU_VISIBLE = 6 };
	enum { SUMMON_PRESET_COUNT = 5 };
	enum { SUMMON_CUSTOM_STAT_COUNT = 9 };

	enum eSummonPreset
	{
		SUMMON_PRESET_EASY,
		SUMMON_PRESET_NORMAL,
		SUMMON_PRESET_MEDIUM,
		SUMMON_PRESET_HARD,
		SUMMON_PRESET_CUSTOM
	};

	struct sGMItemEntry
	{
		DWORD			s_dwItemId;
		std::wstring	s_wsName;
		std::wstring	s_wsLowerName;
		std::wstring	s_wsSearchName;
		std::wstring	s_wsDisplay;
	};

	struct sGMMonsterEntry
	{
		DWORD			s_dwMonsterId;
		std::wstring	s_wsName;
		std::wstring	s_wsLowerName;
		std::wstring	s_wsSearchName;
		std::wstring	s_wsDisplay;
		int				s_nLevel;
		int				s_nHP;
		int				s_nDS;
		int				s_nAT;
		int				s_nDE;
		int				s_nHT;
		int				s_nEV;
		int				s_nCT;
		int				s_nAS;
	};

	void					_SetStatus( TCHAR const* szText );
	void					_SetPage( eGMPage ePage );
	void					_RefreshPage();
	void					_RefreshMenuButtons();
	void					_ClampPageScroll();
	void					_ScrollPages( int nDelta );
	bool					_IsMouseOverMenu();
	void					_LoadItemsFromTable();
	void					_RefreshItemFilter();
	void					_RefreshItemResultButtons();
	void					_ClampItemResultScroll();
	void					_ScrollItemResults( int nDelta );
	bool					_IsMouseOverItemResults();
	void					_SelectFilteredItem( int nFilteredIndex );
	DWORD					_ResolveItemIdForSend();
	void					_LoadMonstersFromTable();
	void					_RefreshMonsterFilter();
	void					_RefreshMonsterResultButtons();
	void					_ClampMonsterResultScroll();
	void					_ScrollMonsterResults( int nDelta );
	void					_SelectFilteredMonster( int nFilteredIndex );
	DWORD					_ResolveMonsterIdForSend();
	void					_RefreshSummonPresetButtons();
	void					_UpdateSummonPresetStatus();
	void					_FillSummonStatFields( sGMMonsterEntry const& entry, int nPercent );
	int						_ReadSummonCustomStat( int nIndex, int nDefaultValue );
	int						_ReadValueEdit( int nDefaultValue, int nMaxValue );
	void					_SendCommand( TCHAR const* szCommand, TCHAR const* szStatus );
	void					_SendCurrentPageCommand();
	void					_OnItemSearchChanged( void* pkSender, void* pData );

private:
	cButton*				m_pMoveButton;
	cButton*				m_pCancelButton;
	cButton*				m_pPageBtn[PAGE_COUNT];
	cScrollBar*				m_pPageScrollBar;
	cButton*				m_pActionBtn;
	cButton*				m_pExitBtn;
	cEditBox*				m_pTargetEdit;
	cEditBox*				m_pItemSearchEdit;
	cEditBox*				m_pAmountEdit;
	cEditBox*				m_pValueEdit;
	cText*					m_pPageTitleText;
	cText*					m_pPageHelpText;
	cText*					m_pStatusText;
	cText*					m_pTargetLabel;
	cText*					m_pItemLabel;
	cText*					m_pAmountLabel;
	cText*					m_pValueLabel;
	cText*					m_pSummonCustomLabel;
	cText*					m_pSummonCustomStatLabel[SUMMON_CUSTOM_STAT_COUNT];
	cEditBox*				m_pSummonCustomStatEdit[SUMMON_CUSTOM_STAT_COUNT];
	cCheckBox*				m_pSummonBossCheck;
	cButton*				m_pSummonPresetBtn[SUMMON_PRESET_COUNT];
	cButton*				m_pItemResultBtn[MAX_ITEM_RESULTS];
	std::vector<sGMItemEntry>	m_vItems;
	std::vector<int>			m_vFilteredItems;
	std::vector<sGMMonsterEntry>	m_vMonsters;
	std::vector<int>			m_vFilteredMonsters;
	int						m_nItemResultScroll;
	int						m_nMonsterResultScroll;
	int						m_nPageScroll;
	DWORD					m_dwSelectedItemId;
	DWORD					m_dwSelectedMonsterId;
	eSummonPreset			m_eSummonPreset;
	bool					m_bApplyingSelection;
	eGMPage					m_ePage;
};
