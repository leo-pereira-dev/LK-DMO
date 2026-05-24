#pragma once

#include <string>
#include <vector>

class cData_Inven;
class cItemInfo;

#define IF_EXTRA_INVENTORY_ROW		10
#define IF_EXTRA_INVENTORY_COL		5
#define IF_EXTRA_INVENTORY_COUNT	(IF_EXTRA_INVENTORY_ROW * IF_EXTRA_INVENTORY_COL)

class cExtraInventory : public cBaseWindow
{
public:
	enum eCategory
	{
		CAT_SEAL = 0,
		CAT_TICKET,
		CAT_EVOLUTION,
		CAT_DIGITAMA,
		CAT_MATERIAL,
		CAT_MAX
	};

	cExtraInventory();
	~cExtraInventory();

	virtual bool			Construct(void);
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_EXTRAINVENTORY; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Create( int nValue = 0 );
	virtual bool			Close( bool bSound = true );

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();
	virtual void			OnLButtonUp( CsPoint pos );
	virtual void			OnRButtonUp( CsPoint pos );
	virtual bool			CursorIconLBtnUp( CURSOR_ICON::eTYPE eIconType, int nIconSlot, int nIconCount );
	virtual void			OnMouseOutWindow();
	virtual bool			OnEscapeKey();
	virtual void			ResetDevice();

private:
	void					CloseButtonClick(void* pSender, void* pData);
	void					CategoryButtonClick(void* pSender, void* pData);
	void					CollectAllButtonClick(void* pSender, void* pData);
	void					OverviewComboSelect(void* pSender, void* pData);
	void					SearchTextChanged(void* pSender, void* pData);
	void					SearchResetClick(void* pSender, void* pData);
	bool					UpdateIconForMouse();
	void					UpdateTooltip( CsPoint ptPos, int nIndex );
	void					CollectVisibleItems( std::vector<int>& outSlots ) const;
	cData_Inven*			GetCategoryInventory() const;
	bool					HasCategoryInventoryData() const;
	int						GetCategoryServerSlot( int nSlotIndex ) const;
	int						GetGridSlotFromLocalPoint( CsPoint ptLocal );
	bool					IsExtraInventoryServerSlot( int nServerSlot ) const;
	bool					IsCurrentCategoryServerSlot( int nServerSlot ) const;
	bool					IsAllowedItem( cItemInfo const* pItem ) const;
	bool					IsAllowedItemType( int nTypeL, int nTypeS ) const;
	bool					IsAllowedByOverviewFilter( cItemInfo const* pItem ) const;
	bool					IsAllowedBySearchText( cItemInfo const* pItem ) const;
	TCHAR const*			GetCategoryName( eCategory eType ) const;
	TCHAR const*			GetCollectAllText( eCategory eType ) const;
	void					CreateOverviewComboBox();
	void					CreateSearchBox();
	void					RefreshOverviewComboItems();
	void					RefreshCategoryVisuals();
	void					UpdateFilterControlsLayout();
	void					RenderSearchText();
	void					UpdateSlotLayout();
	int						GetScrollOffset() const;
	int						GetVisibleColumnCount() const;
	int						GetVisibleRowCount() const;
	int						GetVisibleSlotCount() const;
	CsPoint					GetSlotSize() const;
	CsPoint					GetSlotStep() const;
	CsPoint					GetGridStartPos() const;
	bool					IsOverviewFilterVisible() const;
	bool					IsSearchFilterVisible() const;
	int						EffectTypeToOverviewIndex( int nEffectType ) const;
	int						GetEvolutionRankFilterIndex( cItemInfo const* pItem ) const;
	int						GetSealGrade( int nItemType, int nCount ) const;
	void					RenderSealEmptyCard( CsPoint pos );
	void					RenderSealCard( cItemInfo* pItem, int nIndex, CsPoint pos );
	void					RenderSlotHover( int nIndex, CsPoint pos );
	void					RenderStaticScrollThumb();

private:
	eCategory				m_eCategory;
	int						m_nOverviewFilter;
	sIFIcon					m_IFIcon[ IF_EXTRA_INVENTORY_COUNT ];
	cSprite*				m_pSlotFrames[ IF_EXTRA_INVENTORY_COUNT ];
	cSprite*				m_pSealCardBack[ IF_EXTRA_INVENTORY_COUNT ];
	cSprite*				m_pSealCardImage[ IF_EXTRA_INVENTORY_COUNT ];
	cSprite*				m_pSealCardCountBack[ IF_EXTRA_INVENTORY_COUNT ];
	cText*					m_pSealCardCountText[ IF_EXTRA_INVENTORY_COUNT ];
	std::string				m_strSealCardBack[ IF_EXTRA_INVENTORY_COUNT ];
	std::string				m_strSealCardImage[ IF_EXTRA_INVENTORY_COUNT ];
	cSprite*				m_pSidePanelMask;
	cButton*				m_pCloseButton;
	cButton*				m_pCategoryButtons[ CAT_MAX ];
	cText*					m_pCategoryTexts[ CAT_MAX ];
	cButton*				m_pCollectAllButton;
	cText*					m_pCollectAllText;
	cComboBox*				m_pOverviewComboBox;
	cSprite*				m_pOverviewCombo;
	cSprite*				m_pOverviewDropArrow;
	cButton*				m_pOverviewComboButton;
	cText*					m_pOverviewText;
	cSprite*				m_pSearchBack;
	cEditBox*				m_pSearchEdit;
	cText*					m_pSearchRenderText;
	cButton*				m_pSearchButton;
	cButton*				m_pSearchResetButton;
	std::wstring			m_wsSearchText;
	cScrollBar*				m_pScrollBar;
	cSprite*				m_pScrollStaticTop;
	cSprite*				m_pScrollStaticCenter;
	cSprite*				m_pScrollStaticBottom;
	cSprite*				m_pSealHighlight;
	cSprite*				m_pSealEmptyCard;
	cSprite*				m_pBlock;
};
