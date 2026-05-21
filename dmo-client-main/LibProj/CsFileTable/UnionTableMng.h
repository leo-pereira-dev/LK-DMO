#pragma once

#include <map>
#include <string>
#include <vector>

class CsUnionTableMng
{
public:
	enum eUNION_BIN
	{
		UNION_GROUP = 0,
		UNION_EFFECT,
		UNION_REWARD,
		UNION_FILTER,
		UNION_HACKING_INFO,
		UNION_COMBINE_ITEM_LIST,
		UNION_COMBINE_RANK_LIST,
		UNION_COMBINE_ITEM_GROUP,
		UNION_COMBINE_CEIL_GROUP,
		UNION_GROUP_STR,
		UNION_REWARD_STR,
		UNION_FILTER_STR,
		UNION_BIN_MAX,
	};

	struct sBIN_DATA
	{
		sBIN_DATA()
			: s_nRawSize( 0 )
			, s_nDataOffset( 0 )
			, s_bCompressed( false )
			, s_bLoaded( false )
		{
		}

		std::string					s_strPath;
		std::vector< unsigned char > s_vData;
		std::vector< unsigned char > s_vPayload;
		std::vector< DWORD >		s_vKey;
		size_t						s_nRawSize;
		DWORD						s_nDataOffset;
		bool						s_bCompressed;
		bool						s_bLoaded;
	};

	struct sPAIR
	{
		sPAIR()
			: s_dwFirst( 0 )
			, s_dwSecond( 0 )
		{
		}

		DWORD s_dwFirst;
		DWORD s_dwSecond;
	};

	struct sGROUP
	{
		sGROUP()
			: s_dwGroupID( 0 )
		{
		}

		DWORD s_dwGroupID;
		std::vector< std::vector< sPAIR > > s_vDigimonSets;
	};

	struct sEFFECT
	{
		sEFFECT()
			: s_dwGroupID( 0 )
		{
		}

		struct sENTRY
		{
			sENTRY()
				: s_dwConditionType( 0 )
				, s_dwConditionValue( 0 )
				, s_dwEffectValue( 0 )
			{
			}

			DWORD s_dwConditionType;
			DWORD s_dwConditionValue;
			DWORD s_dwEffectValue;
		};

		DWORD s_dwGroupID;
		std::vector< sENTRY > s_vEntries;
	};

	struct sREWARD
	{
		sREWARD()
			: s_dwRewardID( 0 )
			, s_dwItemID( 0 )
			, s_dwCount( 0 )
			, s_dwUnknown( 0 )
		{
		}

		DWORD s_dwRewardID;
		DWORD s_dwItemID;
		DWORD s_dwCount;
		DWORD s_dwUnknown;
		std::string s_strValue;
	};

	struct sFILTER
	{
		sFILTER()
			: s_dwFilterID( 0 )
		{
		}

		DWORD s_dwFilterID;
		std::wstring s_wsName;
	};

public:
	CsUnionTableMng();
	~CsUnionTableMng();

	bool Init( char* cPath );
	void Delete();

	bool IsLoaded() const;
	sBIN_DATA const* GetBinData( eUNION_BIN eBin ) const;
	size_t GetBinSize( eUNION_BIN eBin ) const;
	std::vector< sGROUP > const& GetGroups() const;
	std::vector< sEFFECT > const& GetEffects() const;
	std::vector< sREWARD > const& GetRewards() const;
	std::vector< sFILTER > const& GetFilters() const;
	std::wstring GetGroupName( DWORD dwGroupID ) const;
	std::wstring GetRewardText( DWORD dwRewardID ) const;
	std::wstring GetFilterText( DWORD dwFilterID ) const;

private:
	bool _LoadFilePack( char* cPath );
	bool _LoadPackBin( eUNION_BIN eBin, char const* pPath );
	bool _InflateIfNeeded( unsigned char const* pRawData, size_t nRawSize, std::vector< unsigned char >& outData, bool& bCompressed ) const;
	bool _ExtractPayload( std::vector< unsigned char > const& data, std::vector< DWORD >& outKey, std::vector< unsigned char >& outPayload ) const;
	void _ParseTables();
	bool _ParseGroupTable( std::vector< unsigned char > const& data );
	bool _ParseEffectTable( std::vector< unsigned char > const& data );
	bool _ParseRewardTable( std::vector< unsigned char > const& data );
	bool _ParseFilterTable( std::vector< unsigned char > const& data, std::vector< sFILTER >* pOutFilter, std::map< DWORD, std::wstring >* pOutText );
	bool _ReadDword( std::vector< unsigned char > const& data, size_t& nOffset, DWORD& outValue ) const;
	bool _ReadStringA( std::vector< unsigned char > const& data, size_t& nOffset, std::string& outValue ) const;
	bool _ReadStringW( std::vector< unsigned char > const& data, size_t& nOffset, std::wstring& outValue ) const;
	DWORD _ReadOffset( std::vector< unsigned char > const& data ) const;

private:
	sBIN_DATA m_BinData[ UNION_BIN_MAX ];
	std::vector< sGROUP > m_vGroups;
	std::vector< sEFFECT > m_vEffects;
	std::vector< sREWARD > m_vRewards;
	std::vector< sFILTER > m_vFilters;
	std::map< DWORD, std::wstring > m_mapGroupText;
	std::map< DWORD, std::wstring > m_mapRewardText;
	std::map< DWORD, std::wstring > m_mapFilterText;
};
