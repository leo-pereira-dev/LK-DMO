#pragma once

class cSprite;
class cImage;
struct sCHAR_IMAGE;

class cDungeonClearResultWindow : public cBaseWindow
{
public:
	struct sReward
	{
		sReward()
		: s_dwItemId( 0 )
		, s_dwAmount( 0 )
		{
		}

		DWORD s_dwItemId;
		DWORD s_dwAmount;
	};

	struct sDetailResult
	{
		sDetailResult()
		: s_byCategory( 0 )
		, s_dwTamerModelId( 0 )
		, s_dwDigimonModelId( 0 )
		, s_wTamerLevel( 0 )
		, s_wDigimonLevel( 0 )
		, s_dwValue( 0 )
		{
		}

		BYTE s_byCategory;
		DWORD s_dwTamerModelId;
		DWORD s_dwDigimonModelId;
		WORD s_wTamerLevel;
		WORD s_wDigimonLevel;
		DWORD s_dwValue;
		std::wstring s_wsTamerName;
		std::wstring s_wsDigimonName;
	};

	struct sResultData
	{
		sResultData()
		: s_dwDungeonId( 0 )
		, s_dwEntryPortalId( 0 )
		, s_byRank( 0 )
		, s_wDifficulty( 0 )
		, s_dwElapsedSeconds( 0 )
		, s_wPartyCount( 0 )
		, s_wPartyMax( 0 )
		, s_wFailCount( 0 )
		, s_wFailMax( 0 )
		, s_dwExp( 0 )
		, s_dwBits( 0 )
		{
		}

		DWORD s_dwDungeonId;
		DWORD s_dwEntryPortalId;
		BYTE s_byRank;
		WORD s_wDifficulty;
		DWORD s_dwElapsedSeconds;
		WORD s_wPartyCount;
		WORD s_wPartyMax;
		WORD s_wFailCount;
		WORD s_wFailMax;
		DWORD s_dwExp;
		DWORD s_dwBits;
		std::wstring s_wsDungeonName;
		std::vector<sReward> s_vDefaultRewards;
		std::vector<sReward> s_vExtraRewards;
		std::vector<sDetailResult> s_vDetailResults;
	};

public:
	cDungeonClearResultWindow();

	virtual eWINDOW_TYPE GetWindowType(){ return WT_DUNGEON_CLEAR_RESULT; }
	virtual void Destroy();
	virtual void DeleteResource();
	virtual void Create( int nValue = 0 );
	virtual void Update(float const& fDeltaTime);
	virtual eMU_TYPE Update_ForMouse();
	virtual void Render();
	virtual void ResetDevice();
	virtual void ResetMap(){ Close( false ); }
	virtual bool OnEscapeKey(){ return true; }

	void SetResultData( sResultData const& data );

private:
	enum { MAX_DEFAULT_REWARD_VISIBLE = 8 };
	enum { MAX_EXTRA_REWARD_VISIBLE = 8 };
	enum { MAX_DETAIL_CARD_COUNT = 3 };

	void _RefreshTexts();
	void _CreateDetailCards();
	void _RefreshDetailCards();
	void _LoadDetailImages();
	void _ReleaseDetailImages();
	void _RenderDetailCards();
	bool _SetRewardToolTip();
	CsPoint _DefaultRewardPos( int nIndex ) const;
	CsPoint _ExtraRewardPos( int nIndex ) const;
	CsPoint _DetailCardPos( int nIndex ) const;

	static int _RankImageState( BYTE byRank );
	static TCHAR const* _DifficultyText( WORD wDifficulty );
	static TCHAR const* _DetailTitle( BYTE byCategory );
	static void _FormatTime( DWORD dwSeconds, TCHAR* szBuffer, size_t nBufferCount );
	static void _FormatNumber( DWORD dwValue, TCHAR* szBuffer, size_t nBufferCount );

private:
	cButton* m_pMoveButton;
	cButton* m_pExitButton;
	cButton* m_pReEnterButton;
	cText* m_pTitleText;
	cImage* m_pRankImage;
	cText* m_pDungeonNameValue;
	cText* m_pDifficultyValue;
	cText* m_pPartyValue;
	cText* m_pTimeValue;
	cText* m_pFailValue;
	cText* m_pExpValue;
	cText* m_pBitValue;
	cText* m_pDefaultRewardEmptyText;
	cText* m_pExtraRewardEmptyText;
	cSprite* m_pDefaultRewardSlot[MAX_DEFAULT_REWARD_VISIBLE];
	cSprite* m_pExtraRewardSlot[MAX_EXTRA_REWARD_VISIBLE];
	cText* m_pDetailTitle[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailTamerName[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailDigimonName[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailTamerLevel[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailDigimonLevel[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailValue[MAX_DETAIL_CARD_COUNT];
	cText* m_pDetailNotApplicable[MAX_DETAIL_CARD_COUNT];
	cSprite* m_pDetailDeco[MAX_DETAIL_CARD_COUNT];
	cSprite* m_pDetailTamerBg[MAX_DETAIL_CARD_COUNT];
	cSprite* m_pDetailDigimonBg[MAX_DETAIL_CARD_COUNT];
	cSprite* m_pDetailTamerLevelBg[MAX_DETAIL_CARD_COUNT];
	cSprite* m_pDetailDigimonLevelBg[MAX_DETAIL_CARD_COUNT];
	cImage* m_pDetailStatsIcon[MAX_DETAIL_CARD_COUNT];
	sCHAR_IMAGE* m_pDetailTamerImage[MAX_DETAIL_CARD_COUNT];
	sCHAR_IMAGE* m_pDetailDigimonImage[MAX_DETAIL_CARD_COUNT];
	sResultData m_Data;
};
