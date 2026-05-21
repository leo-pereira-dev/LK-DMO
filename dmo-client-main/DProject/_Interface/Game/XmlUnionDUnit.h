#pragma once

#include "UnionXmlTools.h"
#include <set>

class cButton;
class cEditBox;
class cImage;
class cSprite;
class cText;
class CDigimonEvolveInfo;

#define IF_XML_UNION_WIDTH			896
#define IF_XML_UNION_HEIGHT			675
#define IF_XML_UNION_CARD_COUNT		8
#define IF_XML_UNION_MAX_CARD_DATA	32
#define IF_XML_UNION_ICON_SLICE_COUNT	42
#define IF_XML_UNION_TAB_SLANT_COUNT	18
#define IF_XML_UNION_FILTER_COUNT	15
#define IF_XML_UNION_EFFECT_COUNT	8
#define IF_XML_UNION_STAT_COUNT		11
#define IF_XML_UNION_REWARD_COUNT	7

class cXmlUnionDUnit : public cBaseWindow
{
public:
	cXmlUnionDUnit();
	virtual ~cXmlUnionDUnit();

protected:
	virtual bool			Construct(void);

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_XML_UNION; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Create( int nValue = 0 );
	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			Render();
	virtual void			ResetDevice();
	virtual void			ResetMap(){ Close( false ); }
	virtual bool			OnEscapeKey(){ Close(); return true; }
	virtual void			OnMoveWindow();
	virtual void			UserDie(){ Close( false ); }
	virtual void			RecvDiedAlready(){ Close( false ); }
	void					RecvServerProgress( u1 nLevel, n4 nCurrentExperience, n4 nRequiredExperience, u2 nProgressPercentBasisPoints, u2 nClaimedRewardMask );
	void					RecvRewardClaimResult( u1 nResult, u1 nRewardIndex, u2 nClaimedRewardMask, u1 nLevel, u2 nProgressPercentBasisPoints );
	void					RecvCollectionInfo();

private:
	struct sSERVER_MEMBER_STATE
	{
		sSERVER_MEMBER_STATE()
			: s_dwDigimonID( 0 )
			, s_nSlot( 0 )
			, s_bOwned( false )
			, s_nLevel( 0 )
			, s_bEvolutionUnlocked( false )
			, s_bTranscended( false )
			, s_bMeetsLevel( false )
			, s_bMeetsEvolution( false )
			, s_bMeetsTranscendence( false )
		{
		}

		DWORD	s_dwDigimonID;
		int		s_nSlot;
		bool	s_bOwned;
		int		s_nLevel;
		bool	s_bEvolutionUnlocked;
		bool	s_bTranscended;
		bool	s_bMeetsLevel;
		bool	s_bMeetsEvolution;
		bool	s_bMeetsTranscendence;
	};

	struct sSERVER_EFFECT_STATE
	{
		sSERVER_EFFECT_STATE()
			: s_nConditionIndex( 0 )
			, s_bActive( false )
			, s_nEffectType( 0 )
			, s_nValue( 0 )
		{
		}

		int		s_nConditionIndex;
		bool	s_bActive;
		int		s_nEffectType;
		int		s_nValue;
	};

	struct sSERVER_GROUP_STATE
	{
		sSERVER_GROUP_STATE()
			: s_bCompleted( false )
			, s_nOwnedCount( 0 )
			, s_nTotalLevel( 0 )
			, s_bAllTranscended( false )
		{
		}

		bool								s_bCompleted;
		int									s_nOwnedCount;
		int									s_nTotalLevel;
		bool								s_bAllTranscended;
		std::vector< sSERVER_MEMBER_STATE >	s_vMembers;
		std::vector< sSERVER_EFFECT_STATE >	s_vEffects;
	};

	struct sCARD_VIEW
	{
		sCARD_VIEW()
			: s_dwGroupID( 0 )
			, s_dwDigimonID( 0 )
			, s_dwIconCandidateID( 0 )
			, s_nLevel( 0 )
			, s_nRank( 0 )
			, s_nSlot( 0 )
			, s_bOwned( false )
			, s_bEvolutionUnlocked( false )
			, s_bMeetsLevel( false )
			, s_bMeetsEvolution( false )
			, s_bMeetsTranscendence( false )
		{
		}

		DWORD			s_dwGroupID;
		DWORD			s_dwDigimonID;
		DWORD			s_dwIconCandidateID;
		int				s_nLevel;
		int				s_nRank;
		int				s_nSlot;
		bool			s_bOwned;
		bool			s_bEvolutionUnlocked;
		bool			s_bMeetsLevel;
		bool			s_bMeetsEvolution;
		bool			s_bMeetsTranscendence;
		std::wstring	s_wsName;
		std::wstring	s_wsOptionName;
		std::string		s_strIconFolder;
		std::string		s_strIconPath;
	};

private:
	void				PressCloseButton( void* pSender, void* pData );
	bool				_FileExists( char const* pPath ) const;
	cSprite*			_AddAssetSprite( CsPoint pos, CsPoint size, char const* pPath, NiColorA const& fallbackColor );
	cSprite*			_AddAssetSpriteRect( CsPoint pos, CsPoint size, CsRect srcRect, char const* pPath, NiColorA const& fallbackColor );
	cSprite*			_AddAssetImageState( CsPoint pos, CsPoint size, CsPoint texToken, int nState, char const* pPath, NiColorA const& fallbackColor );
	cSprite*			_AddSolidSprite( CsPoint pos, CsPoint size, NiColorA const& color );
	cText*				_AddLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign );
	cText*				_AddBoldLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign );
	std::string			_ResolveIconPath( cUnionXmlTools::sGROUP const& group ) const;
	void				_BuildViewData();
	void				_RefreshView();
	void				_CreateLayout();
	void				_CreateCardSprites();
	void				_CreateFilterScrollBar();
	void				_CreateCardScrollBar();
	void				_CreateHoverTooltipControls();
	void				_CreateRewardModalControls();
	void				_SetRewardModalVisible( bool bVisible );
	void				_SetRewardModalControlsVisible( bool bVisible );
	void				_CaptureRewardModalVisibility( std::vector< bool >& vSprites, std::vector< bool >& vTexts, std::vector< bool >& vButtons ) const;
	void				_RestoreRewardModalVisibility( std::vector< bool > const& vSprites, std::vector< bool > const& vTexts, std::vector< bool > const& vButtons );
	void				_RenderRewardModalControls();
	void				_UpdateRewardModalFrameControls();
	void				_UpdateRewardModalOptionControls();
	void				_UpdateRewardModalScrollControls();
	void				_UpdateRewardReceiveButtonHover( int nHoverIndex );
	void				_UpdateServerProgressControls();
	bool				_IsRewardClaimed( int nRewardIndex ) const;
	void				_ApplyServerStateToCard( sCARD_VIEW& card ) const;
	sSERVER_MEMBER_STATE const* _FindServerMemberState( DWORD dwGroupID, DWORD dwDigimonID, int nSlot ) const;
	bool				_IsRewardMetaActive( cUnionXmlTools::sGROUP const& group, cUnionXmlTools::sREWARD const& reward, int nRewardIndex ) const;
	std::wstring		_FormatRewardGoal( cUnionXmlTools::sGROUP const& group, cUnionXmlTools::sREWARD const& reward, int nRewardIndex ) const;
	void				_UpdateTabControls();
	void				_UpdateEffectControls();
	void				_UpdateCardControls();
	void				_UpdateCardScrollControls();
	void				_UpdateHoverTooltipControls();
	void				_UpdateCardHoverEffects();
	void				_UpdateVisibleFilters();
	void				_UpdateFilterRowControls();
	bool				_IsFilterFavorite( int nFilterIndex ) const;
	bool				_IsFavoriteGroupID( DWORD dwGroupID ) const;
	void				_ToggleFilterFavorite( int nFilterIndex );
	void				_LoadFavoriteCache();
	void				_SaveFavoriteCache() const;
	std::string			_GetFavoriteCachePath() const;
	void				_FillCardFromGroup( sCARD_VIEW& card, cUnionXmlTools::sGROUP const& group ) const;
	bool				_FillCardFromMember( sCARD_VIEW& card, cUnionXmlTools::sMEMBER const& member, cUnionXmlTools::sGROUP const& group ) const;
	bool				_FillCardFromDigimon( sCARD_VIEW& card, DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pGroup ) const;
	DWORD				_FindDigimonByName( std::wstring const& wsName ) const;
	DWORD				_FindDigimonByMember( cUnionXmlTools::sMEMBER const& member ) const;
	DWORD				_FindEvolutionBaseByName( std::wstring const& wsName ) const;
	DWORD				_FindEvolutionBaseForGroup( std::wstring const& wsName, cUnionXmlTools::sGROUP const* pSelectedGroup, int nSelectedGroupIndex ) const;
	void				_CollectEvolutionBaseCandidates( std::wstring const& wsName, std::vector< DWORD >& outCandidates ) const;
	int					_GetDuplicateGroupIndex( int nSelectedGroupIndex ) const;
	bool				_IsXAntibodyEvolution( DWORD dwDigimonID ) const;
	bool				_IsSelectedGroupXLine( cUnionXmlTools::sGROUP const* pSelectedGroup ) const;
	bool				_IsDigimonExplicitForSelectedGroup( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const;
	bool				_IsLineEvolutionAllowed( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const;
	CDigimonEvolveInfo*	_FindEvolveInfoForDigimon( DWORD dwDigimonID, USHORT* pOutRootSlot ) const;
	bool				_CanReachEvolutionSlot( CDigimonEvolveInfo* pEvolveInfo, USHORT nRootSlot, USHORT nTargetSlot ) const;
	void				_CollectEvolutionBranch( CDigimonEvolveInfo* pEvolveInfo, USHORT nRootSlot, cUnionXmlTools::sGROUP const* pSelectedGroup, std::vector< DWORD >& outDigimonIDs, size_t nCardLimit ) const;
	void				_BuildLineEvolutionIDs( DWORD dwBaseDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup, int nSelectedGroupIndex, size_t nCardLimit, std::vector< DWORD >& outDigimonIDs ) const;
	void				_AppendRelatedXLineEvolutionIDs( cUnionXmlTools::sGROUP const* pSelectedGroup, size_t nCardLimit, std::vector< DWORD >& outDigimonIDs ) const;
	bool				_IsDigimonDeclaredInUnionXml( DWORD dwDigimonID, cUnionXmlTools::sGROUP const* pSelectedGroup ) const;
	bool				_IsDigimonMatchedToUnionName( DWORD dwDigimonID, std::wstring const& wsUnionName ) const;
	void				_BuildEvolutionCards( std::wstring const& wsSelected, int nSelectedGroupIndex );

private:
	cButton*			m_pCloseButton;
	cButton*			m_pMoveButton;
	cButton*			m_pFilterButtons[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterHoverBg[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterBookmark[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterBorderTop[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterBorderBottom[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterBorderLeft[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterBorderRight[ IF_XML_UNION_FILTER_COUNT ];
	cText*				m_pFilterText[ IF_XML_UNION_FILTER_COUNT ];
	cSprite*			m_pFilterScrollBorder;
	cSprite*			m_pFilterScrollBg;
	cSprite*			m_pFilterScrollTrack;
	cSprite*			m_pFilterScrollThumb;
	cSprite*			m_pFilterScrollUp;
	cSprite*			m_pFilterScrollDown;
	cText*				m_pStatusText;
	cText*				m_pProgressPercentText;
	cText*				m_pProgressLevelValueText;
	cText*				m_pEffectCompletedText;
	cText*				m_pEffectText[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectCheckBg[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectCheckTop[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectCheckBottom[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectCheckLeft[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectCheckRight[ IF_XML_UNION_EFFECT_COUNT ];
	cText*				m_pEffectCheckMark[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectStateButton[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectStateTop[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectStateBottom[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectStateLeft[ IF_XML_UNION_EFFECT_COUNT ];
	cSprite*			m_pEffectStateRight[ IF_XML_UNION_EFFECT_COUNT ];
	cText*				m_pEffectStateText[ IF_XML_UNION_EFFECT_COUNT ];
	cText*				m_pStatValueText[ IF_XML_UNION_STAT_COUNT ];
	cSprite*			m_pHoverTooltipBg;
	cSprite*			m_pHoverTooltipTop;
	cSprite*			m_pHoverTooltipBottom;
	cSprite*			m_pHoverTooltipLeft;
	cSprite*			m_pHoverTooltipRight;
	cText*				m_pHoverTooltipText;
	cSprite*			m_pTabBody;
	cSprite*			m_pTabSlant[ IF_XML_UNION_TAB_SLANT_COUNT ];
	cSprite*			m_pTabTop;
	cSprite*			m_pTabSlantTop[ IF_XML_UNION_TAB_SLANT_COUNT ];
	cSprite*			m_pTabBottom;
	cSprite*			m_pTabLeft;
	cText*				m_pTabText;
	cSprite*			m_pCardBg[ IF_XML_UNION_CARD_COUNT ];
	cSprite*			m_pCardIconSlices[ IF_XML_UNION_CARD_COUNT ][ IF_XML_UNION_ICON_SLICE_COUNT ];
	cSprite*			m_pCardFrame[ IF_XML_UNION_CARD_COUNT ];
	cSprite*			m_pCardHoverEffects[ IF_XML_UNION_CARD_COUNT ];
	cText*				m_pCardLevelText[ IF_XML_UNION_CARD_COUNT ];
	cSprite*			m_pCardRankIcon[ IF_XML_UNION_CARD_COUNT ];
	cSprite*			m_pCardScrollBorder;
	cSprite*			m_pCardScrollBg;
	cSprite*			m_pCardScrollTrack;
	cSprite*			m_pCardScrollThumb;
	cSprite*			m_pCardScrollUp;
	cSprite*			m_pCardScrollDown;
	cSprite*			m_pRewardTreasureIcon;
	cSprite*			m_pRewardModalBg;
	cSprite*			m_pRewardModalTitleBg;
	cSprite*			m_pRewardModalBorderTop;
	cSprite*			m_pRewardModalBorderBottom;
	cSprite*			m_pRewardModalBorderLeft;
	cSprite*			m_pRewardModalBorderRight;
	cSprite*			m_pRewardModalContentLine;
	cText*				m_pRewardModalTitleText;
	cSprite*			m_pRewardOptionGlow[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardOptionBody[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardOptionGauge[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardReceiveHoverBg[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardReceiveBorderTop[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardReceiveBorderBottom[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardReceiveBorderLeft[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardReceiveBorderRight[ IF_XML_UNION_REWARD_COUNT ];
	cText*				m_pRewardOptionNameText[ IF_XML_UNION_REWARD_COUNT ];
	cText*				m_pRewardReceiveText[ IF_XML_UNION_REWARD_COUNT ];
	cSprite*			m_pRewardScrollBorder;
	cSprite*			m_pRewardScrollBg;
	cSprite*			m_pRewardScrollTrack;
	cSprite*			m_pRewardScrollThumb;
	cSprite*			m_pRewardScrollUp;
	cSprite*			m_pRewardScrollDown;
	cButton*			m_pRewardModalCloseButton;
	cButton*			m_pRewardReceiveButton[ IF_XML_UNION_REWARD_COUNT ];
	std::vector< cSprite* >			m_vRewardModalSprites;
	std::vector< cText* >			m_vRewardModalTexts;
	std::vector< cButton* >			m_vRewardModalButtons;
	bool							m_bRewardModalVisible;
	bool							m_bRewardTreasureIconHover;
	bool							m_bRewardModalDragging;
	CsPoint							m_ptRewardModalPos;
	CsPoint							m_ptRewardModalDragOffset;
	int								m_nRewardModalScrollRow;
	int								m_nRewardReceiveHoverIndex;
	bool							m_bServerProgressLoaded;
	u1								m_nServerXmlUnionLevel;
	n4								m_nServerCurrentExperience;
	n4								m_nServerRequiredExperience;
	u2								m_nServerProgressPercentBasisPoints;
	u2								m_nServerClaimedRewardMask;
	int								m_nServerBonusValue[ IF_XML_UNION_STAT_COUNT ];

	cUnionXmlTools		m_XmlData;
	std::vector< cUnionXmlTools::sGROUP > m_vAllGroups;
	std::vector< std::wstring >		m_vAllFilters;
	std::vector< sCARD_VIEW >		m_vCards;
	std::vector< std::wstring >		m_vFilters;
	std::vector< int >				m_vFilteredFilterIndices;
	std::vector< std::wstring >		m_vEffects;
	std::vector< bool >				m_vEffectActive;
	std::vector< cUnionXmlTools::sSTAT_TOTAL > m_vTotals;
	std::map< std::wstring, std::vector< sCARD_VIEW > > m_mapCardCache;
	std::map< DWORD, sSERVER_GROUP_STATE > m_mapServerGroups;
	std::set< DWORD >				m_setFavoriteGroupIDs;
	std::vector< DWORD >			m_vFavoriteGroupOrder;
	std::string						m_strCardIconPath[ IF_XML_UNION_CARD_COUNT ];
	std::string						m_strCardFramePath[ IF_XML_UNION_CARD_COUNT ];
	std::wstring					m_wsSearchKeyword;
	int								m_nFilterScroll;
	int								m_nCardScroll;
	int								m_nSelectedFilter;
	int								m_nHoverFilter;
	int								m_nHoverCard;
	cEditBox*						m_pSearchEdit;
};
