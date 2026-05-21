#pragma once

#include <map>
#include <string>
#include <vector>

class cButton;
class cEditBox;
class cSprite;
class cText;

#define IF_UNION_DUNIT_WIDTH			886
#define IF_UNION_DUNIT_HEIGHT			768
#define IF_UNION_DUNIT_CARD_COUNT		8
#define IF_UNION_DUNIT_FILTER_COUNT		14
#define IF_UNION_DUNIT_EFFECT_COUNT		8
#define IF_UNION_DUNIT_STAT_COUNT		11

class cUnionDUnit : public cBaseWindow
{
public:
	cUnionDUnit();
	virtual ~cUnionDUnit();

protected:
	virtual bool			Construct(void);

public:
	virtual eWINDOW_TYPE	GetWindowType(){ return WT_UNION; }
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

protected:
	struct sCARD_VIEW
	{
		sCARD_VIEW()
			: s_dwGroupID( 0 )
			, s_dwDigimonID( 0 )
			, s_nLevel( 120 )
			, s_nRank( 0 )
			, s_nEvoType( 0 )
			, s_nGrowType( 0 )
			, s_pIcon( NULL )
		{
		}

		DWORD			s_dwGroupID;
		DWORD			s_dwDigimonID;
		int				s_nLevel;
		int				s_nRank;
		int				s_nEvoType;
		int				s_nGrowType;
		std::wstring	s_wsName;
		std::string		s_strIconPath;
		cSprite*		s_pIcon;
	};

protected:
	void				PressCloseButton( void* pSender, void* pData );
	void				_VerifyAssets();
	bool				_FileExists( char const* pPath ) const;
	bool				_HasAsset( char const* pPath ) const;
	cSprite*			_AddAssetSprite( CsPoint pos, CsPoint size, char const* pPath, NiColor const& fallbackColor );
	cSprite*			_AddSolidSprite( CsPoint pos, CsPoint size, NiColorA const& color );
	cText*				_AddLabel( TCHAR const* pText, CsPoint pos, CFont::eFACE_SIZE eSize, NiColor const& color, UINT nAlign );
	std::wstring		_MakeGroupName( DWORD dwGroupID, int nIndex ) const;
	std::wstring		_MakeDigimonName( DWORD dwDigimonID ) const;
	std::wstring		_MakeEffectLine( int nIndex ) const;
	DWORD				_FindDigimonByName( std::wstring const& wsName ) const;
	bool				_SetCardDigimon( sCARD_VIEW& card, DWORD dwDigimonID ) const;
	void				_BuildViewData();
	void				_CreateLayout();
	void				_CreateCardSprites();
	void				_ResetCardIcons();
	void				_RenderCardIcons();

protected:
	cButton*			m_pCloseButton;
	cButton*			m_pMoveButton;
	cButton*			m_pSearchButton;
	cEditBox*			m_pSearchEdit;
	cButton*			m_pFilterButtons[ IF_UNION_DUNIT_FILTER_COUNT ];

	cText*				m_pCardNameText[ IF_UNION_DUNIT_CARD_COUNT ];
	cText*				m_pCardLevelText[ IF_UNION_DUNIT_CARD_COUNT ];
	cText*				m_pCardRankText[ IF_UNION_DUNIT_CARD_COUNT ];
	cText*				m_pEffectText[ IF_UNION_DUNIT_EFFECT_COUNT ];
	cText*				m_pStatValueText[ IF_UNION_DUNIT_STAT_COUNT ];
	cText*				m_pMissingAssetText;

	std::map< std::string, bool >	m_mapAssets;
	std::vector< std::string >		m_vMissingAssets;
	std::vector< sCARD_VIEW >		m_vCards;
	std::vector< std::wstring >		m_vFilters;
	std::vector< std::wstring >		m_vEffects;
	int								m_nStatValues[ IF_UNION_DUNIT_STAT_COUNT ];
};
