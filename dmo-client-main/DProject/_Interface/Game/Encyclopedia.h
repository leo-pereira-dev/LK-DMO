#pragma once
#include <set>
#include "EncyclopediaContents.h"
#include <vector>
class cRenderTex;
class CDigimon;
class cImage;
class cEditBox;
class CsNodeObj;

#define ENCY_DISABLE_ALPHA			0.65f
#define ENCY_ENABLE_ALPHA			1.0f
#define ENCY_DISABLE_COLOR			NiColor( 40.0f/255.0f, 40.0f/255.0f, 40.0f/255.0f )
#define ENCY_ENABLE_COLOR			NiColor( 255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f )

#define IF_ENCY_MAX_ENCHANT_COUNT	5
#define IF_ENCY_VIEW_ENCHANT_COUNT	2

class cEncyclopedia : public cBaseWindow, public EncyclopediaContents::ObserverType
{
public:
	struct sDIGIMON_ID : public CUSTOMDATA
	{
		sDIGIMON_ID(int digimonId, int count = 0)
		{
			iDigimonId = digimonId;
			iCount = count;
		}
		~sDIGIMON_ID() {};

		int iDigimonId;
		int	iCount;
	};

	struct sICON_INFO
	{
		sICON_INFO()
		{
			s_count				= 0;
			s_nGroupIdx			= 0;
			s_dwBaseDigimonID	= 0;
			s_dwDestDigimonID	= 0;
		}

		cString::sSPRITE*	s_Sprite;
		int					s_count;
		USHORT				s_nGroupIdx;
		DWORD				s_dwBaseDigimonID;
		DWORD				s_dwDestDigimonID;
	};

public:
	cEncyclopedia();
	virtual ~cEncyclopedia();

protected:
	virtual	bool			Construct(void);
	virtual	void			Notify(int const& iNotifiedEvt, ContentsStream const& kStream);

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_ENCYCLOPEDIA; }
	virtual void			Destroy();
	virtual void			DeleteResource();
	virtual void			Open( int nValue = 0, bool bSound = true );
	virtual bool			Close( bool bSound = true );
	virtual void			Create( int nValue = 0 );

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			OnMoveWindow();

	virtual void			Render();

	virtual void			ResetDevice();
	virtual void			ResetMap(){ Close( false ); }
	virtual bool			OnEscapeKey();

	virtual void			UserDie(){ Close( false ); }

	/************************************************************************/
	/*						도감 UI - 디지몬 목록							*/
	/************************************************************************/
protected:
	cButton*			m_pCancelButton;	// 닫기 버튼
	cButton*			m_pMoveButton;		// 이동 버튼
	cButton*			m_pStatisticsBtn;	// 통계창 오픈 버튼
	cRadioButton*		m_pRadioBtn;		// legacy 라디오(비가시 상태로 내부 재사용)
	cButton*			m_pMainTabBtn[2];	// 0: Encyclopedia, 1: Deck
	cButton*			m_pSubMenuBtn[4];	// 0: Series, 1: All, 2: Class, 3: Statistics
	cText*				m_pSubMenuText[4];	// submenu labels that must follow tab visibility
	cScrollBar*			m_pScrollBar;
	cListBox*			m_pEncyListBox;
	cSprite*			m_pSearchBarBg;
	cEditBox*			m_pSearchEdit;
	cButton*			m_pSearchBtn;
	cButton*			m_pSearchResetBtn;
	std::wstring		m_wsSearchKeyword;
	bool				m_bSearchFocusVisualOn;

	enum eMAIN_TAB
	{
		MAIN_TAB_ENCYCLOPEDIA = 0,
		MAIN_TAB_DECK = 1,
	};
	enum eENCY_SUBMENU
	{
		SUBMENU_SERIES = 0,
		SUBMENU_ALL = 1,
		SUBMENU_CLASS = 2,
		SUBMENU_STATISTICS = 3,
	};

	int					m_nCurTab;			// 현재 탭, 탭 변경이 아닌 Refresh일 경우 확인용
	int					m_nGroupCount;
	int					iScrollPos;
	int					m_nPendingDeckScrollPos;
	bool				m_bPendingDeckTabRefresh;
	bool				bIsTooltip;
	int					m_nMainTab;
	int					m_nSubMenu;

	std::list<sICON_INFO*>								m_IconList;
	std::map< int, EncyclopediaContents::sEVOL_INFO* > m_pCurTabList_map;				// 현재 탭에 속한 디지몬 계열체 맵

	struct sCARD_HOVER_INFO
	{
		sCARD_HOVER_INFO()
			: s_pItem( NULL )
			, s_pOverFrame( NULL )
			, s_pEffect( NULL )
			, s_nDigimonID( 0 )
			, s_nTooltipDigimonID( 0 )
		{}

		cListBoxItem*		s_pItem;
		CsRect				s_LocalRect;
		cString::sIMAGE*	s_pOverFrame;
		cString::sSPRITE*	s_pEffect;
		int					s_nDigimonID;
		int					s_nTooltipDigimonID;
	};
	std::vector<sCARD_HOVER_INFO>						m_vCardHoverInfo;
	cSprite*											m_pCardHoverEffect;
	bool												m_bCardHoverEffectVisible;
	CsPoint												m_ptCardHoverEffectPos;
	CsPoint												m_ptCardHoverEffectSize;
	cSprite*											m_pCardNameTooltipBg;
	cSprite*											m_pCardNameTooltipTop;
	cSprite*											m_pCardNameTooltipBottom;
	cSprite*											m_pCardNameTooltipLeft;
	cSprite*											m_pCardNameTooltipRight;
	cText*												m_pCardNameTooltipText;
	bool												m_bCardNameTooltipVisible;
	CsPoint												m_ptCardNameTooltipPos;
	CsPoint												m_ptCardNameTooltipSize;

	struct sDECK_PANEL_HOVER_INFO
	{
		sDECK_PANEL_HOVER_INFO()
			: s_pItem( NULL )
			, s_nGroupIdx( 0 )
		{}

		cListBoxItem*	s_pItem;
		CsRect			s_LocalRect;
		int				s_nGroupIdx;
	};
	std::vector<sDECK_PANEL_HOVER_INFO>				m_vDeckPanelHoverInfo;
	struct sDECK_BOOKMARK_INFO
	{
		sDECK_BOOKMARK_INFO()
			: s_pItem( NULL )
			, s_pImage( NULL )
			, s_nGroupIdx( 0 )
		{}

		cListBoxItem*			s_pItem;
		CsRect					s_LocalRect;
		cString::sIMAGE*		s_pImage;
		int						s_nGroupIdx;
	};
	std::vector<sDECK_BOOKMARK_INFO>					m_vDeckBookmarkInfo;
	std::set<int>										m_setDeckFavoriteGroup;

	void				_SetTabList( int nCurRadioIdx );								// 현재 탭 디지몬 리스트 셋
	void				_RefreshList();
	void				_UpdateCardHover();
	void				_SetCardHoverVisible( bool bVisible );
	void				_RenderCardNameTooltip();
	int					_GetDeckHoverGroupIdx( cListBoxItem* pItem, CsPoint const& ptLocalMouse ) const;
	void				_OnClickEncyclopediaItem( void* pSender, void* pData );
	void				_OpenOverview( int nDigimonId, int nTooltipDigimonId );
	void				_CloseOverview();
	void				_ReleaseOverviewRenderDigimon();
	void				_RenderOverview();
	void				_UpdateOverviewRenderDigimon();
	void				_UpdateOverviewSkillSection();
	void				_ResolveOverviewGrowthData( int nDigimonId, int& nLevel, int& nScale, int& nCloneLevel,
									   int& nCloneAT, int& nCloneCT, int& nCloneBL, int& nCloneEV, int& nCloneHP ) const;
	void				_ClearIconList();

	void				SetTabListData();
	//void				SetTabListDataRenderCount();									//2017-05-24-nova Scroll 마지막페이지 리스트 크기에 따른 렌더카운트 설정
	void				SetTabGroupListData();
	void				SetTabGroupListDataRenderCount();								//2017-05-24-nova Scroll 마지막페이지 리스트 크기에 따른 렌더카운트 설정

	void				PressCancelButton( void* pSender, void* pData );				//2017-05-25-nova
	void				PressStatisticsButton( void* pSender, void* pData );			//2017-05-25-nova
	void				PressGroupListBtn( void* pSender, void* pData );				//2017-05-25-nova
	void				PressMainTabButton( void* pSender, void* pData );
	void				PressSubMenuButton( void* pSender, void* pData );
	void				PressSearchButton( void* pSender, void* pData );
	void				PressSearchResetButton( void* pSender, void* pData );
	void				_RefreshSearchUI();

	/************************************************************************/
	/*							도감 UI - 툴팁								*/
	/************************************************************************/
protected:
	cSprite*			m_pTooltipBG;								// 계열체 툴팁 배경
	cText*				m_szName;									// 툴팁 :: 디지몬 이름
	cText*				m_szLevel;									// 툴팁 :: 레벨
	cText*				m_szScale;									// 툴팁 :: 스케일
	cSprite*			m_pEnchant[IF_ENCY_MAX_ENCHANT_COUNT] ;		// 툴팁 강화 수치 종류
	cText*				m_szEnchantVal[IF_ENCY_VIEW_ENCHANT_COUNT];	// 툴팁 :: 공격력
	cStringList*		m_szExplain;								// 계열체 설명
	CsCoolTimeSeq		m_DelayTime;								// 툴팁 :: 툴팁 표시 대기 시간( 마우스 오버 후 툴팁 띄우기까지의 시간)

	int					m_nTooltipDigimonID;						// 툴팁 중인 디지몬, Set 한번하도록 이걸로 체크
	bool				m_bIsMouseOn;								// 마우스 오버 중인지 체크, true일때만 랜더
	std::map <int, int> m_pEnchantVal;								// 강화수치

	void				_SetTooltip( int nDigimonID );				// 도감 툴팁
	void				_SetCardNameTooltip( int nDigimonID, CsRect const& rtCardScreen );
	void				_RenderTooltip( CsPoint ptMousePos );
	void				_Updata_ForMouse_Group( int count );		// 덱 마우스 업데이트
	void				_SetGroupTooltip( int nGroupIdx );			// 덱 툴팁	
	void				_UpdateDeckBookmarkVisuals();

	/************************************************************************/
	/*						도감 UI - 보상아이템 획득						*/
	/************************************************************************/
protected:
	int					m_nSelectDigimonID;			// 보상아이템 받을 계열체 디지몬 번호, 버튼 누를때 저장
	int					m_nSelectDeckIdx;			// 효과 받을 덱 인덱스, 버튼 누를때 저장

	/************************************************************************/
	/*					디감 UI - 디지몬 오버뷰 (카드 클릭)					*/
	/************************************************************************/
protected:
	bool				m_bOverviewVisible;
	int					m_nOverviewDigimonId;
	int					m_nOverviewTooltipDigimonId;
	int					m_nOverviewSelectedSkillIdx;
	int					m_nOverviewHoverSkillIdx;
	cRenderTex*			m_pOverviewRenderTex;
	CDigimon*			m_pOverviewRenderDigimon;
	CsNodeObj*			m_pOverviewFigureBase;
	cText*				m_pOverviewName;
	cText*				m_pOverviewGrowthTitle;
	cText*				m_pOverviewInfoTitle;
	cText*				m_pOverviewSkillTab;
	cText*				m_pOverviewRankLabel;
	cText*				m_pOverviewRankValue;
	cText*				m_pOverviewStageLabel;
	cText*				m_pOverviewStageValue;
	cText*				m_pOverviewStatText[ 8 ];
	cText*				m_pOverviewSkillName;
	cImage*				m_pOverviewSkillTabFrame;
	cImage*				m_pOverviewBackBtn;
	cText*				m_pOverviewBackText;
	cImage*				m_pOverviewCloseBtn;
	cImage*				m_pOverviewScrollBtn;
	cImage*				m_pOverviewZoomInBtn;
	cImage*				m_pOverviewZoomOutBtn;
	cImage*				m_pOverviewArrowLBtn;
	cImage*				m_pOverviewArrowRBtn;
	cImage*				m_pOverviewRotateLBtn;
	cImage*				m_pOverviewRotateRBtn;
	cImage*				m_pOverviewRotateUpBtn;
	cImage*				m_pOverviewRotateDownBtn;
	cImage*				m_pOverviewResetViewBtn;
	cSprite*			m_pOverviewTooltipLine;
	cStringList*		m_pOverviewGrowth;
	cStringList*		m_pOverviewInfo;
	cStringList*		m_pOverviewSkillDesc;
	cStringList*		m_pOverviewSkills;
	cStringList*		m_pOverviewSkillStatsLeft;
	cStringList*		m_pOverviewSkillStatsRight;
	cSprite*			m_pOverviewBg;
	cSprite*			m_pOverviewInfoTextBg;
	cSprite*			m_pOverviewRatingFrame;
	cSprite*			m_pOverviewBox;
	cSprite*			m_pOverviewAttributeFrame;
	cSprite*			m_pOverviewSkillFrame;
	cSprite*			m_pOverviewSkillSelection;
	cSprite*			m_pOverviewSkillLine;
	cSprite*			m_pOverviewStatusArrow;
	cSprite*			m_pOverviewStatusArrow2;
	cSprite*			m_pOverviewActionCardFrame;
	float				m_fOverviewZoomRate;
	float				m_fOverviewZoomTargetRate;
	float				m_fOverviewRenderYaw;
	float				m_fOverviewRenderPitch;
	float				m_fOverviewZoomDistMin;
	float				m_fOverviewZoomDistMax;
	float				m_fOverviewZoomStep;
	bool				m_bOverviewZoomSliderDrag;
	int					m_nOverviewZoomSliderDragOffsetX;
	bool				m_bOverviewModelDragRotate;
	CsPoint				m_ptOverviewModelDragPrev;
	float				m_fOverviewPreviewAniTimer;
	bool				m_bOverviewPreviewAltAni;
	bool				m_bOverviewPreviewAniStarted;

public:
	int					_GetSelectDigimonID(){ return m_nSelectDigimonID; }	// 보상아이템 받을 계열체 디지몬 번호
	int					_GetSelectDeckIdx(){ return m_nSelectDeckIdx; }		// 효과 받을 덱 인덱스
};
