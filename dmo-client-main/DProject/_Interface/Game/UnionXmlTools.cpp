#include "stdafx.h"
#include "UnionXmlTools.h"

namespace
{
	std::wstring _Utf8ToWide( char const* pText )
	{
		if( pText == NULL || pText[ 0 ] == 0 )
			return std::wstring();

		int const nLen = MultiByteToWideChar( CP_UTF8, 0, pText, -1, NULL, 0 );
		if( nLen <= 1 )
			return std::wstring();

		std::vector< wchar_t > vBuffer( nLen, 0 );
		MultiByteToWideChar( CP_UTF8, 0, pText, -1, &vBuffer[ 0 ], nLen );
		return std::wstring( &vBuffer[ 0 ] );
	}

	std::wstring _AttrWide( TiXmlElement* pEl, char const* pName )
	{
		return pEl ? _Utf8ToWide( pEl->Attribute( pName ) ) : std::wstring();
	}

	std::string _AttrString( TiXmlElement* pEl, char const* pName )
	{
		char const* pValue = pEl ? pEl->Attribute( pName ) : NULL;
		return pValue ? pValue : "";
	}

	DWORD _AttrDword( TiXmlElement* pEl, char const* pName, DWORD dwDefault = 0 )
	{
		if( pEl == NULL )
			return dwDefault;

		char const* pValue = pEl->Attribute( pName );
		if( pValue == NULL || pValue[ 0 ] == 0 )
			return dwDefault;

		return (DWORD)strtoul( pValue, NULL, 10 );
	}

	int _AttrInt( TiXmlElement* pEl, char const* pName, int nDefault = 0 )
	{
		if( pEl == NULL )
			return nDefault;

		char const* pValue = pEl->Attribute( pName );
		if( pValue == NULL || pValue[ 0 ] == 0 )
			return nDefault;

		return atoi( pValue );
	}

	bool _AttrPercent( TiXmlElement* pEl )
	{
		char const* pUnit = pEl ? pEl->Attribute( "unit" ) : NULL;
		return pUnit && ::stricmp( pUnit, "percent" ) == 0;
	}

	void _MakePath( char* pOut, size_t nOutSize, char const* pBasePath, char const* pFileName )
	{
		strcpy_s( pOut, nOutSize, pBasePath ? pBasePath : "Data\\union" );
		size_t nLen = strlen( pOut );
		if( nLen > 0 && pOut[ nLen - 1 ] != '\\' && pOut[ nLen - 1 ] != '/' )
			strcat_s( pOut, nOutSize, "\\" );
		strcat_s( pOut, nOutSize, pFileName );
	}
}

cUnionXmlTools::cUnionXmlTools()
	: m_bLoaded( false )
{
}

void cUnionXmlTools::Clear()
{
	m_vGroups.clear();
	m_vFilters.clear();
	m_vTotals.clear();
	m_mapTotalIndex.clear();
	m_strLastError.clear();
	m_bLoaded = false;
}

bool cUnionXmlTools::Load( char const* pBasePath )
{
	Clear();

	char szLines[ MAX_PATH ] = { 0, };
	char szCollections[ MAX_PATH ] = { 0, };
	char szFilters[ MAX_PATH ] = { 0, };
	_MakePath( szLines, MAX_PATH, pBasePath, "DM_Union-DigimonLines.xml" );
	_MakePath( szCollections, MAX_PATH, pBasePath, "DM_Union-DigimonCollections.xml" );
	_MakePath( szFilters, MAX_PATH, pBasePath, "DM_Union-Filter_Str.xml" );

	bool const bLines = _LoadLines( szLines );
	bool const bCollections = _LoadCollections( szCollections );
	_LoadFilters( szFilters );

	m_bLoaded = ( bLines || bCollections ) && m_vGroups.empty() == false;
	if( m_bLoaded == false && m_strLastError.empty() )
		_SetError( "Union XML files were not found or had no groups." );

	return m_bLoaded;
}

bool cUnionXmlTools::_LoadDocument( char const* pPath, TiXmlDocument& doc )
{
	if( pPath == NULL || pPath[ 0 ] == 0 )
		return false;

	if( doc.LoadFile( pPath ) )
		return true;

	if( CsFPS::CsFPSystem::IsExistOnlyPack( 0, pPath ) )
	{
		char* pBuffer = NULL;
		size_t const nSize = CsFPS::CsFPSystem::Allocate_GetFileData( 0, &pBuffer, pPath );
		if( pBuffer == NULL || nSize == 0 )
		{
			if( pBuffer )
				free( pBuffer );
			return false;
		}

		doc.Parse( pBuffer );
		free( pBuffer );
		return doc.Error() == false;
	}

	return false;
}

bool cUnionXmlTools::_LoadLines( char const* pPath )
{
	TiXmlDocument doc;
	if( _LoadDocument( pPath, doc ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == NULL )
		return false;

	for( TiXmlElement* pLine = pRoot->FirstChildElement( "Line" ); pLine; pLine = pLine->NextSiblingElement( "Line" ) )
	{
		sGROUP group;
		group.s_dwGroupID = _AttrDword( pLine, "id" );
		group.s_dwIconCandidateID = _AttrDword( pLine, "iconCandidateId" );
		group.s_nLevel = _AttrInt( pLine, "currentLevel", 120 );
		group.s_nRank = -1;
		group.s_nMemberCount = _AttrInt( pLine, "unlocks" );
		group.s_wsType = _T( "Line" );
		group.s_wsName = _AttrWide( pLine, "name" );
		group.s_strIconFolder = _AttrString( pLine, "iconFolder" );
		_ReadMembers( pLine, group );
		_ReadRewards( pLine, group );
		_ReadTotalStat( pLine );

		if( group.s_wsName.empty() == false )
			_AddFilter( group.s_wsName );

		m_vGroups.push_back( group );
	}

	return true;
}

bool cUnionXmlTools::_LoadCollections( char const* pPath )
{
	TiXmlDocument doc;
	if( _LoadDocument( pPath, doc ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == NULL )
		return false;

	for( TiXmlElement* pCollection = pRoot->FirstChildElement( "Collection" ); pCollection; pCollection = pCollection->NextSiblingElement( "Collection" ) )
	{
		sGROUP group;
		group.s_dwGroupID = _AttrDword( pCollection, "id" ) + 100000;
		group.s_dwIconCandidateID = _AttrDword( pCollection, "iconCandidateId" );
		group.s_nLevel = _AttrInt( pCollection, "levelReq2", _AttrInt( pCollection, "levelReq1", 120 ) );
		group.s_nRank = -1;
		group.s_nMemberCount = _AttrInt( pCollection, "membersRequired" );
		group.s_wsType = _T( "Collection" );
		group.s_wsName = _AttrWide( pCollection, "name" );
		group.s_strIconFolder = _AttrString( pCollection, "iconFolder" );
		_ReadMembers( pCollection, group );
		_ReadRewards( pCollection, group );
		_ReadTotalStat( pCollection );

		if( group.s_wsName.empty() == false )
			_AddFilter( group.s_wsName );

		m_vGroups.push_back( group );
	}

	return true;
}

bool cUnionXmlTools::_LoadFilters( char const* pPath )
{
	TiXmlDocument doc;
	if( _LoadDocument( pPath, doc ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if( pRoot == NULL )
		return false;

	for( TiXmlElement* pString = pRoot->FirstChildElement( "String" ); pString; pString = pString->NextSiblingElement( "String" ) )
		_AddFilter( _AttrWide( pString, "text" ) );

	return true;
}

void cUnionXmlTools::_ReadMembers( TiXmlElement* pGroupEl, sGROUP& outGroup )
{
	TiXmlElement* pMembers = pGroupEl ? pGroupEl->FirstChildElement( "Members" ) : NULL;
	if( pMembers == NULL )
		return;

	for( TiXmlElement* pMember = pMembers->FirstChildElement( "Member" ); pMember; pMember = pMember->NextSiblingElement( "Member" ) )
	{
		sMEMBER member;
		member.s_nSlot = _AttrInt( pMember, "slot" );
		member.s_wsName = _AttrWide( pMember, "name" );
		member.s_dwIconCandidateID = _AttrDword( pMember, "iconCandidateId" );
		member.s_strIconFolder = _AttrString( pMember, "iconFolder" );

		if( member.s_wsName.empty() == false && member.s_wsName != _T( "__" ) )
			outGroup.s_vMembers.push_back( member );
	}

	if( outGroup.s_nMemberCount <= 0 )
		outGroup.s_nMemberCount = (int)outGroup.s_vMembers.size();
}

void cUnionXmlTools::_ReadRewards( TiXmlElement* pGroupEl, sGROUP& outGroup )
{
	TiXmlElement* pRewards = pGroupEl ? pGroupEl->FirstChildElement( "Rewards" ) : NULL;
	if( pRewards == NULL )
		return;

	for( TiXmlElement* pReward = pRewards->FirstChildElement( "Reward" ); pReward; pReward = pReward->NextSiblingElement( "Reward" ) )
	{
		sREWARD reward;
		reward.s_dwEffectID = _AttrDword( pReward, "effectId" );
		reward.s_wsRaw = _AttrWide( pReward, "raw" );
		reward.s_wsStat = _AttrWide( pReward, "stat" );
		reward.s_nValue = _AttrInt( pReward, "value" );
		reward.s_bPercent = _AttrPercent( pReward );
		outGroup.s_vRewards.push_back( reward );
	}
}

void cUnionXmlTools::_ReadTotalStat( TiXmlElement* pGroupEl )
{
	TiXmlElement* pStat = pGroupEl ? pGroupEl->FirstChildElement( "TotalStat" ) : NULL;
	if( pStat == NULL )
		return;

	std::wstring wsName = _AttrWide( pStat, "type" );
	if( wsName.empty() )
		return;

	int const nValue = _AttrInt( pStat, "value" );
	std::map< std::wstring, int >::iterator it = m_mapTotalIndex.find( wsName );
	if( it == m_mapTotalIndex.end() )
	{
		sSTAT_TOTAL total;
		total.s_wsName = wsName;
		total.s_nValue = nValue;
		total.s_bPercent = false;
		m_mapTotalIndex[ wsName ] = (int)m_vTotals.size();
		m_vTotals.push_back( total );
		return;
	}

	m_vTotals[ it->second ].s_nValue += nValue;
}

void cUnionXmlTools::_AddFilter( std::wstring const& wsName )
{
	if( wsName.empty() )
		return;

	for( size_t i = 0; i < m_vFilters.size(); ++i )
	{
		if( m_vFilters[ i ] == wsName )
			return;
	}

	m_vFilters.push_back( wsName );
}

void cUnionXmlTools::_SetError( char const* pText )
{
	m_strLastError = pText ? pText : "";
}
