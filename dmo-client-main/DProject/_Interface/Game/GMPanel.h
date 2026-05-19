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
	enum { MAX_ITEM_RESULTS = 6 };

	struct sGMItemEntry
	{
		DWORD			s_dwItemId;
		std::wstring	s_wsName;
		std::wstring	s_wsLowerName;
		std::wstring	s_wsDisplay;
	};

	void					_SetStatus( TCHAR const* szText );
	void					_LoadItemsFromTable();
	void					_RefreshItemFilter();
	void					_SelectFilteredItem( int nFilteredIndex );
	DWORD					_ResolveItemIdForSend();
	void					_SendGiveItem();
	void					_OnItemSearchChanged( void* pkSender, void* pData );

private:
	cButton*				m_pCancelButton;
	cButton*				m_pGiveItemBtn;
	cButton*				m_pExitBtn;
	cEditBox*				m_pTargetEdit;
	cEditBox*				m_pItemSearchEdit;
	cEditBox*				m_pAmountEdit;
	cText*					m_pStatusText;
	cButton*				m_pItemResultBtn[MAX_ITEM_RESULTS];
	std::vector<sGMItemEntry>	m_vItems;
	std::vector<int>			m_vFilteredItems;
	DWORD					m_dwSelectedItemId;
	bool					m_bApplyingSelection;
};
