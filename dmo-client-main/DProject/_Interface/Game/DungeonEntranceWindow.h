#pragma once

class CsC_AvObject;
class cSprite;

class cDungeonEntranceWindow : public cBaseWindow
{
public:
	cDungeonEntranceWindow();

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_DUNGEON_ENTRANCE; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Create( int nValue = 0 );

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();

	virtual void			ResetDevice();
	virtual void			ResetMap(){ Close( false ); }
	virtual bool			OnEscapeKey(){ Close(); return true; }

	void					SetTarget( CsC_AvObject* pTarget );

private:
	enum { MAX_LIST_VISIBLE = 8 };
	enum { MAX_REQUIREMENT_TEXT = 6 };
	enum { MAX_REWARD_COLUMNS = 4 };
	enum { MAX_REWARD_ROWS = 2 };
	enum { MAX_REWARD_VISIBLE = MAX_REWARD_COLUMNS * MAX_REWARD_ROWS };
	enum { MAX_COMPLETION_REWARD_VISIBLE = 15 };

	struct sRequirement
	{
		WORD					s_wType;
		std::vector<DWORD>		s_vValues;
	};

	struct sReward
	{
		BYTE					s_byGroup;
		DWORD					s_dwItemId;
		DWORD					s_dwAmount;
	};

	struct sRequirementLine
	{
		std::wstring			s_wsText;
		bool					s_bSection;
	};

	struct sDungeonEntry
	{
		DWORD					s_dwDungeonId;
		DWORD					s_dwPortalId;
		DWORD					s_dwSourceMapId;
		DWORD					s_dwDestinationMapId;
		DWORD					s_dwNpcObjectId;
		int						s_nPortalKindIndex;
		WORD					s_wDifficulty;
		WORD					s_wSortGroup;
		WORD					s_wExtra;
		WORD					s_wMinPlayers;
		WORD					s_wMaxPlayers;
		std::string				s_strImagePath;
		std::wstring			s_wsName;
		std::wstring			s_wsDescription;
		std::vector<sRequirement> s_vRequirements;
		std::vector<sReward>	s_vMonsterRewards;
		std::vector<sReward>	s_vCompletionRewards;
	};

	void					_SetStatus( TCHAR const* szText );
	void					_LoadDungeonData();
	bool					_LoadPortalDungeonData();
	bool					_LoadDungeonListBin();
	bool					_LoadDungeonStringBin();
	bool					_LoadDungeonQualificationBin();
	bool					_LoadDungeonObtainableBin();
	bool					_LoadDungeonRewardBin();
	void					_FilterByTarget();
	void					_RefreshList();
	void					_RefreshDetail();
	void					_SelectVisibleEntry( int nVisibleIndex );
	void					_ScrollList( int nDelta );
	bool					_IsMouseOverList();
	bool					_IsMouseOverRequirement();
	bool					_IsMouseOverReward();
	void					_EnterSelectedDungeon();
	bool					_SetRewardToolTip();
	bool					_SetCompletionRewardToolTip();
	CsPoint					_GetRewardSlotPos( int nIndex ) const;
	CsPoint					_GetCompletionRewardSlotPos( int nIndex ) const;
	int						_FindEntryByDungeonId( DWORD dwDungeonId ) const;
	WORD					_GetMaxPlayers( sDungeonEntry const& entry ) const;
	void					_BuildRequirementLines( sDungeonEntry const& entry, std::vector<sRequirementLine>& vLines ) const;
	std::wstring			_BuildDungeonDisplayName( sDungeonEntry const& entry ) const;
	std::wstring			_FormatRequirement( sRequirement const& req ) const;
	void					_SetCompletionRewardMode( bool bEnable );
	void					_RefreshCompletionRewardPanel();
	cSprite* m_pRequirementSectionLine[MAX_REQUIREMENT_TEXT];

private:
	cButton*				m_pMoveButton;
	cButton*				m_pCloseButton;
	cButton*				m_pEnterButton;
	cButton*				m_pCompletionOpenButton;
	cButton*				m_pCompletionCloseButton;
	cButton*				m_pListButton[MAX_LIST_VISIBLE];
	cSprite*				m_pListButtonBg[MAX_LIST_VISIBLE];
	cSprite*				m_pListButtonStroke[MAX_LIST_VISIBLE];
	cSprite*				m_pListDifficultyBadge[MAX_LIST_VISIBLE];
	cText*					m_pListText[MAX_LIST_VISIBLE];
	cScrollBar*				m_pListScrollBar;
	cScrollBar*				m_pRequirementScrollBar;
	cScrollBar*				m_pRewardScrollBar;
	cScrollBar*				m_pCompletionContentScrollBar;
	cSprite*				m_pPreviewSprite;
	cSprite*				m_pPreviewDifficultyBadge;
	cSprite*				m_pCompletionOpenIcon;
	cSprite*				m_pCompletionPanelBg;
	cSprite*				m_pCompletionBasicTabBg;
	cSprite*				m_pCompletionRewardListHeaderBg;
	cSprite*				m_pCompletionContentHeaderBg;
	cSprite*				m_pCompletionExtraTabBg;
	cText*					m_pTitleText;
	cText*					m_pSubTitleText;
	cText*					m_pImagePathText;
	cText*					m_pCompletionTitleText;
	cText*					m_pCompletionBasicTabText;
	cText*					m_pCompletionRewardListHeaderText;
	cText*					m_pCompletionContentHeaderText;
	cText*					m_pCompletionInfoText;
	cText*					m_pCompletionExtraTabText;
	cText*					m_pCompletionEmptyText;
	cText*					m_pRequirementText[MAX_REQUIREMENT_TEXT];
	cSprite*				m_pRequirementSectionMark[MAX_REQUIREMENT_TEXT];
	cSprite*				m_pRewardSlot[MAX_REWARD_VISIBLE];
	cSprite*				m_pCompletionRewardSlot[MAX_COMPLETION_REWARD_VISIBLE];
	cSprite*				m_pPartyLimitIcon;
	cText*					m_pPartyLimitText;
	cText*					m_pStatusText;
	CsC_AvObject*			m_pTarget;
	std::vector<sDungeonEntry> m_vDungeons;
	std::vector<int>		m_vVisibleDungeons;
	int						m_nSelectedVisible;
	int						m_nListScroll;
	int						m_nRequirementScroll;
	int						m_nRewardScroll;
	int						m_nCompletionRewardScroll;
	bool					m_bLoaded;
	bool					m_bCompletionRewardMode;
};
