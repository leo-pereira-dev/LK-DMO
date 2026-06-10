


#pragma once 

class cRaidRank : public cBaseWindow
{
public:
	cRaidRank();
	virtual ~cRaidRank();

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_RAIDRANK; }
	virtual void			Destroy();
	virtual void			DeleteResource();	

	virtual void			ResetDevice();	

	virtual void			Create( int nValue = 0 );
	virtual bool			OnEscapeKey();

	virtual void			Update(float const& fDeltaTime);
	virtual eMU_TYPE		Update_ForMouse();
	virtual void			OnMouseOutWindow();

	virtual void			Render();		

	virtual void			PreResetMap(){ Close( false ); }


	void					SetRanker(int nIndex, int nRank, TCHAR* szTamer, TCHAR* szDigimon, int nDamage, NiColor color = NiColor::WHITE);
	void					ResetRankList();



protected:
	enum
	{
		MAX_RAID_RANKER_COUNT = 10,
		RAID_RANK_FIELD_COUNT = 4,
		RAID_RANK_NAME_LEN = 63,
	};

	struct sRANKER_LINE
	{
		bool		s_bVisible;
		int			s_nRank;
		TCHAR		s_szTamer[ RAID_RANK_NAME_LEN + 1 ];
		TCHAR		s_szDigimon[ RAID_RANK_NAME_LEN + 1 ];
		int			s_nDamage;
		NiColor		s_Color;

		void Reset();
	};

	void					_CreateStaticText();
	void					_ApplyPendingRankList();
	bool					_HasControlsReady();
	
	cButton*				m_pBtnClose;
	cStringList				m_pRankingString;
	cText*					m_pHeaderText[ RAID_RANK_FIELD_COUNT ];
	cText*					m_pRankerText[ MAX_RAID_RANKER_COUNT ][ RAID_RANK_FIELD_COUNT ];
	sRANKER_LINE			m_RankerLine[ MAX_RAID_RANKER_COUNT ];
	sRANKER_LINE			m_PendingRankerLine[ MAX_RAID_RANKER_COUNT ];
	bool					m_bPendingRankerUpdate;
	bool					m_bControlsReady;
	CRITICAL_SECTION		m_csRanker;
	
};
