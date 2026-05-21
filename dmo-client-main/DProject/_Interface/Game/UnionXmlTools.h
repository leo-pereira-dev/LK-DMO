#pragma once

#include <map>
#include <string>
#include <vector>

class TiXmlDocument;
class TiXmlElement;

class cUnionXmlTools
{
public:
	struct sREWARD
	{
		DWORD		s_dwEffectID;
		std::wstring s_wsRaw;
		std::wstring s_wsStat;
		int			s_nValue;
		bool		s_bPercent;
	};

	struct sMEMBER
	{
		sMEMBER()
			: s_dwIconCandidateID( 0 )
			, s_nSlot( 0 )
		{
		}

		DWORD			s_dwIconCandidateID;
		int				s_nSlot;
		std::wstring	s_wsName;
		std::string		s_strIconFolder;
	};

	struct sGROUP
	{
		DWORD						s_dwGroupID;
		DWORD						s_dwIconCandidateID;
		int							s_nLevel;
		int							s_nRank;
		int							s_nMemberCount;
		std::wstring				s_wsType;
		std::wstring				s_wsName;
		std::string					s_strIconFolder;
		std::vector< sREWARD >		s_vRewards;
		std::vector< sMEMBER >		s_vMembers;
	};

	struct sSTAT_TOTAL
	{
		std::wstring s_wsName;
		int			s_nValue;
		bool		s_bPercent;
	};

public:
	cUnionXmlTools();

	bool Load( char const* pBasePath = "Data\\union" );
	void Clear();

	std::vector< sGROUP > const& GetGroups() const { return m_vGroups; }
	std::vector< std::wstring > const& GetFilters() const { return m_vFilters; }
	std::vector< sSTAT_TOTAL > const& GetTotals() const { return m_vTotals; }
	bool IsLoaded() const { return m_bLoaded; }
	std::string const& GetLastError() const { return m_strLastError; }

private:
	bool _LoadLines( char const* pPath );
	bool _LoadCollections( char const* pPath );
	bool _LoadFilters( char const* pPath );
	bool _LoadDocument( char const* pPath, TiXmlDocument& doc );
	void _ReadMembers( TiXmlElement* pGroupEl, sGROUP& outGroup );
	void _ReadRewards( TiXmlElement* pGroupEl, sGROUP& outGroup );
	void _ReadTotalStat( TiXmlElement* pGroupEl );
	void _AddFilter( std::wstring const& wsName );
	void _SetError( char const* pText );

private:
	std::vector< sGROUP >		m_vGroups;
	std::vector< std::wstring >	m_vFilters;
	std::vector< sSTAT_TOTAL >	m_vTotals;
	std::map< std::wstring, int > m_mapTotalIndex;
	std::string					m_strLastError;
	bool						m_bLoaded;
};
