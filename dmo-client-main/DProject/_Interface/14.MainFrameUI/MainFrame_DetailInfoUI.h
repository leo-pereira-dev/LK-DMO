#pragma once

#include "../00.Contents/MainFrame_Contents.h"

class cDetailInfoUI : public cBaseWindow, public CMainFrameContents::ObserverType
{
public:
	cDetailInfoUI();
	~cDetailInfoUI();

private:
	virtual bool			Construct(void);
	virtual void			Notify(int const& iNotifiedEvt, ContentsStream const& kStream);

public:
	virtual eWINDOW_TYPE	GetWindowType();
	virtual void			Destroy();
	virtual void			DeleteResource();

	virtual void			Create(int nValue = 0);
	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();

	virtual void			ResetMap(void){ Close(false); }
	virtual void			ResetDevice();
	virtual bool			OnEscapeKey();

private:
	enum eTabType
	{
		eTabTamer,
		eTabDigimon,
	};

	enum eDigimonAbilityType
	{
		eDigiHP,
		eDigiDS,
		eDigiAT,
		eDigiAS,
		eDigiCT,
		eDigiHT,
		eDigiDE,
		eDigiBL,
		eDigiEV,
		eDigiMax,
	};

	struct sTabInfo : public CUSTOMDATA
	{
		sTabInfo(eTabType eTab) : eType(eTab) {}
		~sTabInfo() {}

		eTabType eType;
	};

	struct sEquipInfo : public CUSTOMDATA
	{
		sEquipInfo(int nPType, uint nType) : nPartsType(nPType), nItemType(nType) {}
		~sEquipInfo() {}

		int		nPartsType;
		uint	nItemType;
	};

	struct sEquipIcon
	{
		sEquipIcon() : pItemIcon(NULL), pGridItem(NULL) {}
		~sEquipIcon() {}

		void ChangeItem(uint nItemType, CsCoolTimeSeq* pCoolTime);

		cString::sICON*		pItemIcon;
		cGridListBoxItem*	pGridItem;
	};

	struct sSkillInfo : public CUSTOMDATA
	{
		sSkillInfo(int nIndex) : nSkillIndex(nIndex) {}
		~sSkillInfo() {}

		int nSkillIndex;
	};

	struct sSkillIcon
	{
		sSkillIcon() : pSkillIcon(NULL), pSkillLevelText(NULL), pGridItem(NULL) {}
		~sSkillIcon() {}

		void ResetSkill();
		void ChangeSkill(int nIcon, int nLevel, int nMaxLevel);

		cString::sICON*		pSkillIcon;
		cString::sTEXT*		pSkillLevelText;
		cGridListBoxItem*	pGridItem;
	};

	cRadioButton*			m_pTabButton;
	cButton*				m_pCloseButton;
	cButton*				m_pVisibilityButton;
	cButton*				m_pEquipmentButton;
	cButton*				m_pSealMasterButton;

	cGridListBox*			m_pTamerLeftEquipList;
	cGridListBox*			m_pTamerRightEquipList;
	cGridListBox*			m_pDigimonSkillGrid;

	std::map<int, sEquipIcon>	m_mapEquipIcon;
	std::map<int, sSkillIcon>	m_mapSkillIcon;

	cRenderTex*				m_pTamerRenderTex;
	cRenderTex*				m_pDigimonRenderTex;

	eTabType				m_eCurrentTab;
	bool					m_bPendingRefresh;

	cText*					m_pTamerNameText;
	cText*					m_pTamerLevelText;
	cText*					m_pTamerAchievementTitleText;
	cText*					m_pDigimonNameText;
	cText*					m_pDigimonLevelText;
	cText*					m_pDigimonInfoText;

	cText*					m_pTamerStatValue[13];
	cText*					m_pDigimonStatValue[eDigiMax];
	cText*					m_pEnchantValue[5];

	std::vector<cWindow*>	m_vTamerControls;
	std::vector<cText*>		m_vTamerTexts;
	std::vector<cWindow*>	m_vDigimonControls;
	std::vector<cText*>		m_vDigimonTexts;

private:
	void					_MakeTabButtons();
	void					_MakeTamerPage();
	void					_MakeDigimonPage();
	void					_SetTab(eTabType eTab);
	void					_SetPageVisible(bool bTamerPage, bool bVisible);
	void					_UpdateAll();

	void					_MakeTamerEquipGrid();
	void					_AddTamerEquipGridItem(cGridListBox* pGrid, int nGridIndex, int nEquipType);
	void					_UpdateTamerPage();
	void					_UpdateDigimonPage();
	void					_UpdateSkillGrid();
	void					_UpdateEnchantPanel();
	void					_RenderTamerPortrait();

	cSprite*				_AddSolidSprite(CsPoint pos, CsPoint size, NiColorA const& color);
	cSprite*				_AddPageSprite(bool bTamerPage, CsPoint pos, CsPoint size, char const* pPath);
	cSprite*				_AddPageSolidSprite(bool bTamerPage, CsPoint pos, CsPoint size, NiColorA const& color);
	cText*					_AddPageText(bool bTamerPage, cText::sTEXTINFO* pTextInfo, CsPoint pos);
	cButton*				_AddPageButton(bool bTamerPage, CsPoint pos, CsPoint size, cButton::eIMAGE_TYPE eImageType, TCHAR const* pText);

	void					_AddStatTable(bool bTamerPage, CsPoint pos, int nRows, int nLabelWidth, int nValueWidth, int nRowHeight);
	void					_AddTamerStatRow(int nIndex, TCHAR const* pLabel, CsPoint pos);
	void					_AddDigimonStatRow(eDigimonAbilityType eType, TCHAR const* pLabel, CsPoint pos);
	void					_AddEnchantRow(int nIndex, TCHAR const* pLabel, CsPoint pos);

	std::wstring			_GetDigimonAbilityText(eDigimonAbilityType eType) const;
	void					_SetText(cText* pText, TCHAR const* pValue);
	void					_SetText(cString::sTEXT* pText, TCHAR const* pValue);
	void					_SetText(cText* pText, std::wstring const& value);
	void					_SetText(cString::sTEXT* pText, std::wstring const& value);

	bool					_SetEquipTooltip(cGridListBoxItem const* pOverItem);
	bool					_SetSkillTooltip(cGridListBoxItem const* pOverItem);

	void					_OnClickTabButton(void* pSender, void* pData);
	void					_OnClickCloseButton(void* pSender, void* pData);
	void					_OnClickEquipmentButton(void* pSender, void* pData);
	void					_OnClickSealMasterButton(void* pSender, void* pData);
};
