#pragma once

#include "UnionXmlTools.h"

class cButton;
class cEditBox;
class cImage;
class cSprite;
class cText;
class CDigimonEvolveInfo;

#define IF_XML_UNION_WIDTH			886
#define IF_XML_UNION_HEIGHT			675
#define IF_XML_UNION_CARD_COUNT		8
#define IF_XML_UNION_MAX_CARD_DATA	32
#define IF_XML_UNION_ICON_SLICE_COUNT	42
#define IF_XML_UNION_TAB_SLANT_COUNT	18
#define IF_XML_UNION_FILTER_COUNT	15
#define IF_XML_UNION_EFFECT_COUNT	8
#define IF_XML_UNION_STAT_COUNT		11

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

private:
	struct sCARD_VIEW
	{
		sCARD_VIEW()
			: s_dwGroupID( 0 )
			, s_dwDigimonID( 0 )
			, s_dwIconCandidateID( 0 )
			, s_nLevel( 120 )
			, s_nRank( 0 )
		{
		}

		DWORD			s_dwGroupID;
		DWORD			s_dwDigimonID;
		DWORD			s_dwIconCandidateID;
		int				s_nLevel;
		int				s_nRank;
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
	void				_UpdateTabControls();
	void				_UpdateEffectControls();
	void				_UpdateCardControls();
	void				_UpdateCardScrollControls();
	void				_UpdateHoverTooltipControls();
	void				_UpdateCardHoverEffects();
	void				_UpdateVisibleFilters();
	void				_UpdateFilterRowControls();
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
	cText*				m_pEffectText[ IF_XML_UNION_EFFECT_COUNT ];
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
	cImage*				m_pCardRankIcon[ IF_XML_UNION_CARD_COUNT ];
	cSprite*			m_pCardScrollBorder;
	cSprite*			m_pCardScrollBg;
	cSprite*			m_pCardScrollTrack;
	cSprite*			m_pCardScrollThumb;
	cSprite*			m_pCardScrollUp;
	cSprite*			m_pCardScrollDown;

	cUnionXmlTools		m_XmlData;
	std::vector< cUnionXmlTools::sGROUP > m_vAllGroups;
	std::vector< std::wstring >		m_vAllFilters;
	std::vector< sCARD_VIEW >		m_vCards;
	std::vector< std::wstring >		m_vFilters;
	std::vector< int >				m_vFilteredFilterIndices;
	std::vector< std::wstring >		m_vEffects;
	std::vector< cUnionXmlTools::sSTAT_TOTAL > m_vTotals;
	std::map< std::wstring, std::vector< sCARD_VIEW > > m_mapCardCache;
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
