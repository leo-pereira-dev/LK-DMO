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
		PAGE_DESBUG_CHAR,
		PAGE_LEVEL_UP,
		PAGE_LEVEL_UP_DIGI,
		PAGE_ALL_EVO,
		PAGE_BITS,
		PAGE_COUNT
	};

	enum { MAX_ITEM_RESULTS = 7 };

	struct sGMItemEntry
	{
		DWORD			s_dwItemId;
		std::wstring	s_wsName;
		std::wstring	s_wsLowerName;
		std::wstring	s_wsDisplay;
	};

	void					_SetStatus( TCHAR const* szText );
	void					_SetPage( eGMPage ePage );
	void					_RefreshPage();
	void					_LoadItemsFromTable();
	void					_RefreshItemFilter();
	void					_RefreshItemResultButtons();
	void					_ClampItemResultScroll();
	void					_ScrollItemResults( int nDelta );
	bool					_IsMouseOverItemResults();
	void					_SelectFilteredItem( int nFilteredIndex );
	DWORD					_ResolveItemIdForSend();
	int						_ReadValueEdit( int nDefaultValue, int nMaxValue );
	void					_SendCommand( TCHAR const* szCommand, TCHAR const* szStatus );
	void					_SendCurrentPageCommand();
	void					_OnItemSearchChanged( void* pkSender, void* pData );

private:
	cButton*				m_pMoveButton;
	cButton*				m_pCancelButton;
	cButton*				m_pPageBtn[PAGE_COUNT];
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
	cButton*				m_pItemResultBtn[MAX_ITEM_RESULTS];
	std::vector<sGMItemEntry>	m_vItems;
	std::vector<int>			m_vFilteredItems;
	int						m_nItemResultScroll;
	DWORD					m_dwSelectedItemId;
	bool					m_bApplyingSelection;
	eGMPage					m_ePage;
};
