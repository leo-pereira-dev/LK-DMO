#include "stdafx.h"
#include "UnionTableMng.h"
#include "..\..\Gamebryo2.3\ThirdPartyCode\ZLib\zlib123\zlib.h"

namespace
{
	char const* const UNION_TABLE_PATH = "Data\\Bin\\Table\\";

	void _AppendPath( char* pOut, size_t nOutSize, char const* pDir, char const* pFile )
	{
		strcpy_s( pOut, nOutSize, pDir );
		strcat_s( pOut, nOutSize, "\\" );
		strcat_s( pOut, nOutSize, pFile );
	}
}

CsUnionTableMng::CsUnionTableMng()
{
}

CsUnionTableMng::~CsUnionTableMng()
{
	Delete();
}

bool CsUnionTableMng::Init( char* cPath )
{
	bool const bLoaded = _LoadFilePack( cPath );
	_ParseTables();
	return bLoaded;
}

void CsUnionTableMng::Delete()
{
	for( int i = 0; i < UNION_BIN_MAX; ++i )
	{
		m_BinData[ i ].s_strPath.clear();
		m_BinData[ i ].s_vData.clear();
		m_BinData[ i ].s_vPayload.clear();
		m_BinData[ i ].s_vKey.clear();
		m_BinData[ i ].s_nRawSize = 0;
		m_BinData[ i ].s_nDataOffset = 0;
		m_BinData[ i ].s_bCompressed = false;
		m_BinData[ i ].s_bLoaded = false;
	}
	m_vGroups.clear();
	m_vEffects.clear();
	m_vRewards.clear();
	m_vFilters.clear();
	m_mapGroupText.clear();
	m_mapRewardText.clear();
	m_mapFilterText.clear();
}

bool CsUnionTableMng::IsLoaded() const
{
	for( int i = 0; i < UNION_BIN_MAX; ++i )
	{
		if( m_BinData[ i ].s_bLoaded == false )
			return false;
	}
	return true;
}

CsUnionTableMng::sBIN_DATA const* CsUnionTableMng::GetBinData( eUNION_BIN eBin ) const
{
	if( eBin < 0 || eBin >= UNION_BIN_MAX )
		return NULL;
	return &m_BinData[ eBin ];
}

size_t CsUnionTableMng::GetBinSize( eUNION_BIN eBin ) const
{
	sBIN_DATA const* pData = GetBinData( eBin );
	if( pData == NULL )
		return 0;
	return pData->s_vData.size();
}

std::vector< CsUnionTableMng::sGROUP > const& CsUnionTableMng::GetGroups() const
{
	return m_vGroups;
}

std::vector< CsUnionTableMng::sEFFECT > const& CsUnionTableMng::GetEffects() const
{
	return m_vEffects;
}

std::vector< CsUnionTableMng::sREWARD > const& CsUnionTableMng::GetRewards() const
{
	return m_vRewards;
}

std::vector< CsUnionTableMng::sFILTER > const& CsUnionTableMng::GetFilters() const
{
	return m_vFilters;
}

std::wstring CsUnionTableMng::GetGroupName( DWORD dwGroupID ) const
{
	std::map< DWORD, std::wstring >::const_iterator it = m_mapGroupText.find( dwGroupID );
	return it != m_mapGroupText.end() ? it->second : std::wstring();
}

std::wstring CsUnionTableMng::GetRewardText( DWORD dwRewardID ) const
{
	std::map< DWORD, std::wstring >::const_iterator it = m_mapRewardText.find( dwRewardID );
	return it != m_mapRewardText.end() ? it->second : std::wstring();
}

std::wstring CsUnionTableMng::GetFilterText( DWORD dwFilterID ) const
{
	std::map< DWORD, std::wstring >::const_iterator it = m_mapFilterText.find( dwFilterID );
	return it != m_mapFilterText.end() ? it->second : std::wstring();
}

bool CsUnionTableMng::_LoadFilePack( char* cPath )
{
	char szStringPath[ MAX_PATH ] = { 0, };
	if( cPath && cPath[ 0 ] )
		strcpy_s( szStringPath, cPath );
	else
		strcpy_s( szStringPath, MAX_PATH, "Data\\Bin\\English" );

	struct sUNION_FILE
	{
		eUNION_BIN	s_eBin;
		char const*	s_pPath;
		bool		s_bUseLanguagePath;
	};

	sUNION_FILE const kFiles[] =
	{
		{ UNION_GROUP, "DM_Union-Group.bin", false },
		{ UNION_EFFECT, "DM_Union-Effect.bin", false },
		{ UNION_REWARD, "DM_Union-Reward.bin", false },
		{ UNION_FILTER, "DM_Union-Filter.bin", false },
		{ UNION_HACKING_INFO, "DM_Union-HackingInfo.bin", false },
		{ UNION_COMBINE_ITEM_LIST, "DM_UnionCombine-ItemList.bin", false },
		{ UNION_COMBINE_RANK_LIST, "DM_UnionCombine-RankList.bin", false },
		{ UNION_COMBINE_ITEM_GROUP, "DM_UnionCombine-ItemGroup.bin", false },
		{ UNION_COMBINE_CEIL_GROUP, "DM_UnionCombine-CeilGroup.bin", false },
		{ UNION_GROUP_STR, "DM_Union-Group_Str.bin", true },
		{ UNION_REWARD_STR, "DM_Union-Reward_Str.bin", true },
		{ UNION_FILTER_STR, "DM_Union-Filter_Str.bin", true },
	};

	bool bResult = true;
	CsFPS::CsFPSystem::SeekLock( FT_PACKHANDLE );
	for( int i = 0; i < _countof( kFiles ); ++i )
	{
		char szPath[ MAX_PATH ] = { 0, };
		_AppendPath( szPath, MAX_PATH, kFiles[ i ].s_bUseLanguagePath ? szStringPath : UNION_TABLE_PATH, kFiles[ i ].s_pPath );
		if( _LoadPackBin( kFiles[ i ].s_eBin, szPath ) == false )
			bResult = false;
	}
	CsFPS::CsFPSystem::SeekUnLock( FT_PACKHANDLE );

	return bResult;
}

bool CsUnionTableMng::_LoadPackBin( eUNION_BIN eBin, char const* pPath )
{
	if( eBin < 0 || eBin >= UNION_BIN_MAX || pPath == NULL )
		return false;

	char* pRawData = NULL;
	size_t nRawSize = CsFPS::CsFPSystem::Allocate_GetFileData( FT_PACKHANDLE, &pRawData, pPath );
	if( pRawData == NULL || nRawSize == 0 )
	{
		if( pRawData )
			free( pRawData );
		return false;
	}

	std::vector< unsigned char > vDecoded;
	bool bCompressed = false;
	bool bInflated = _InflateIfNeeded( reinterpret_cast< unsigned char const* >( pRawData ), nRawSize, vDecoded, bCompressed );
	free( pRawData );

	if( bInflated == false )
		return false;

	sBIN_DATA& data = m_BinData[ eBin ];
	data.s_strPath = pPath;
	data.s_vData.swap( vDecoded );
	_ExtractPayload( data.s_vData, data.s_vKey, data.s_vPayload );
	data.s_nRawSize = nRawSize;
	data.s_nDataOffset = _ReadOffset( data.s_vData );
	data.s_bCompressed = bCompressed;
	data.s_bLoaded = true;
	return true;
}

bool CsUnionTableMng::_InflateIfNeeded( unsigned char const* pRawData, size_t nRawSize, std::vector< unsigned char >& outData, bool& bCompressed ) const
{
	outData.clear();
	bCompressed = false;

	if( pRawData == NULL || nRawSize == 0 )
		return false;

	if( nRawSize < 2 || pRawData[ 0 ] != 0x78 )
	{
		outData.assign( pRawData, pRawData + nRawSize );
		return true;
	}

	bCompressed = true;
	z_stream stream;
	memset( &stream, 0, sizeof( stream ) );
	stream.next_in = const_cast< Bytef* >( reinterpret_cast< Bytef const* >( pRawData ) );
	stream.avail_in = (uInt)nRawSize;

	if( inflateInit( &stream ) != Z_OK )
		return false;

	unsigned char szBuffer[ 8192 ];
	int nResult = Z_OK;
	do
	{
		stream.next_out = szBuffer;
		stream.avail_out = sizeof( szBuffer );
		nResult = inflate( &stream, Z_NO_FLUSH );
		if( nResult != Z_OK && nResult != Z_STREAM_END )
		{
			inflateEnd( &stream );
			return false;
		}
		size_t nProduced = sizeof( szBuffer ) - stream.avail_out;
		outData.insert( outData.end(), szBuffer, szBuffer + nProduced );
	}
	while( nResult != Z_STREAM_END );

	inflateEnd( &stream );
	return outData.empty() == false;
}

bool CsUnionTableMng::_ExtractPayload( std::vector< unsigned char > const& data, std::vector< DWORD >& outKey, std::vector< unsigned char >& outPayload ) const
{
	outKey.clear();
	outPayload.clear();

	size_t nOffset = 0;
	DWORD nKeyCount = 0;
	if( _ReadDword( data, nOffset, nKeyCount ) == false )
		return false;
	if( nKeyCount == 0 || nKeyCount > 64 )
		return false;
	if( data.size() < nOffset + ( nKeyCount * sizeof( DWORD ) ) + sizeof( DWORD ) )
		return false;

	for( DWORD i = 0; i < nKeyCount; ++i )
	{
		DWORD dwKey = 0;
		if( _ReadDword( data, nOffset, dwKey ) == false )
			return false;
		outKey.push_back( dwKey );
	}

	DWORD nPayloadSize = 0;
	if( _ReadDword( data, nOffset, nPayloadSize ) == false )
		return false;
	if( nPayloadSize == 0 || data.size() < nOffset + nPayloadSize )
		return false;

	outPayload.assign( data.begin() + nOffset, data.begin() + nOffset + nPayloadSize );
	return true;
}

void CsUnionTableMng::_ParseTables()
{
	m_vGroups.clear();
	m_vEffects.clear();
	m_vRewards.clear();
	m_vFilters.clear();
	m_mapGroupText.clear();
	m_mapRewardText.clear();
	m_mapFilterText.clear();

	_ParseGroupTable( m_BinData[ UNION_GROUP ].s_vPayload.empty() ? m_BinData[ UNION_GROUP ].s_vData : m_BinData[ UNION_GROUP ].s_vPayload );
	_ParseEffectTable( m_BinData[ UNION_EFFECT ].s_vPayload.empty() ? m_BinData[ UNION_EFFECT ].s_vData : m_BinData[ UNION_EFFECT ].s_vPayload );
	_ParseRewardTable( m_BinData[ UNION_REWARD ].s_vPayload.empty() ? m_BinData[ UNION_REWARD ].s_vData : m_BinData[ UNION_REWARD ].s_vPayload );
	_ParseFilterTable( m_BinData[ UNION_FILTER ].s_vPayload.empty() ? m_BinData[ UNION_FILTER ].s_vData : m_BinData[ UNION_FILTER ].s_vPayload, &m_vFilters, NULL );
	_ParseFilterTable( m_BinData[ UNION_GROUP_STR ].s_vPayload.empty() ? m_BinData[ UNION_GROUP_STR ].s_vData : m_BinData[ UNION_GROUP_STR ].s_vPayload, NULL, &m_mapGroupText );
	_ParseFilterTable( m_BinData[ UNION_REWARD_STR ].s_vPayload.empty() ? m_BinData[ UNION_REWARD_STR ].s_vData : m_BinData[ UNION_REWARD_STR ].s_vPayload, NULL, &m_mapRewardText );
	_ParseFilterTable( m_BinData[ UNION_FILTER_STR ].s_vPayload.empty() ? m_BinData[ UNION_FILTER_STR ].s_vData : m_BinData[ UNION_FILTER_STR ].s_vPayload, NULL, &m_mapFilterText );
}

bool CsUnionTableMng::_ParseGroupTable( std::vector< unsigned char > const& data )
{
	size_t nOffset = 0;
	DWORD nCount = 0;
	if( _ReadDword( data, nOffset, nCount ) == false || nCount > 10000 )
		return false;

	for( DWORD i = 0; i < nCount; ++i )
	{
		sGROUP kGroup;
		if( _ReadDword( data, nOffset, kGroup.s_dwGroupID ) == false )
			return false;

		DWORD nSetCount = 0;
		if( _ReadDword( data, nOffset, nSetCount ) == false || nSetCount > 1000 )
			return false;

		for( DWORD nSet = 0; nSet < nSetCount; ++nSet )
		{
			DWORD nPairCount = 0;
			if( _ReadDword( data, nOffset, nPairCount ) == false || nPairCount > 1000 )
				return false;

			std::vector< sPAIR > vPairs;
			for( DWORD nPair = 0; nPair < nPairCount; ++nPair )
			{
				sPAIR kPair;
				if( _ReadDword( data, nOffset, kPair.s_dwFirst ) == false || _ReadDword( data, nOffset, kPair.s_dwSecond ) == false )
					return false;
				vPairs.push_back( kPair );
			}
			kGroup.s_vDigimonSets.push_back( vPairs );
		}
		m_vGroups.push_back( kGroup );
	}

	return true;
}

bool CsUnionTableMng::_ParseEffectTable( std::vector< unsigned char > const& data )
{
	size_t nOffset = 0;
	DWORD nCount = 0;
	if( _ReadDword( data, nOffset, nCount ) == false || nCount > 10000 )
		return false;

	for( DWORD i = 0; i < nCount; ++i )
	{
		sEFFECT kEffect;
		if( _ReadDword( data, nOffset, kEffect.s_dwGroupID ) == false )
			return false;

		DWORD nEntryCount = 0;
		if( _ReadDword( data, nOffset, nEntryCount ) == false || nEntryCount > 1000 )
			return false;
		for( DWORD nEntry = 0; nEntry < nEntryCount; ++nEntry )
		{
			sEFFECT::sENTRY kEntry;
			if( _ReadDword( data, nOffset, kEntry.s_dwConditionType ) == false ||
				_ReadDword( data, nOffset, kEntry.s_dwConditionValue ) == false ||
				_ReadDword( data, nOffset, kEntry.s_dwEffectValue ) == false )
			{
				return false;
			}
			kEffect.s_vEntries.push_back( kEntry );
		}
		m_vEffects.push_back( kEffect );
	}
	return true;
}

bool CsUnionTableMng::_ParseRewardTable( std::vector< unsigned char > const& data )
{
	size_t nOffset = 0;
	DWORD nCount = 0;
	if( _ReadDword( data, nOffset, nCount ) == false || nCount > 10000 )
		return false;

	for( DWORD i = 0; i < nCount; ++i )
	{
		sREWARD kReward;
		if( _ReadDword( data, nOffset, kReward.s_dwRewardID ) == false ||
			_ReadDword( data, nOffset, kReward.s_dwItemID ) == false ||
			_ReadDword( data, nOffset, kReward.s_dwCount ) == false ||
			_ReadDword( data, nOffset, kReward.s_dwUnknown ) == false ||
			_ReadStringA( data, nOffset, kReward.s_strValue ) == false )
		{
			return false;
		}
		m_vRewards.push_back( kReward );
	}
	return true;
}

bool CsUnionTableMng::_ParseFilterTable( std::vector< unsigned char > const& data, std::vector< sFILTER >* pOutFilter, std::map< DWORD, std::wstring >* pOutText )
{
	size_t nOffset = 0;
	DWORD nCount = 0;
	if( _ReadDword( data, nOffset, nCount ) == false || nCount > 10000 )
		return false;

	for( DWORD i = 0; i < nCount; ++i )
	{
		DWORD dwKey = 0;
		std::wstring wsValue;
		if( _ReadDword( data, nOffset, dwKey ) == false || _ReadStringW( data, nOffset, wsValue ) == false )
			return false;

		if( pOutFilter )
		{
			sFILTER kFilter;
			kFilter.s_dwFilterID = dwKey;
			kFilter.s_wsName = wsValue;
			pOutFilter->push_back( kFilter );
		}
		if( pOutText )
			(*pOutText)[ dwKey ] = wsValue;
	}
	return true;
}

bool CsUnionTableMng::_ReadDword( std::vector< unsigned char > const& data, size_t& nOffset, DWORD& outValue ) const
{
	if( data.size() < nOffset + sizeof( DWORD ) )
		return false;
	memcpy( &outValue, &data[ nOffset ], sizeof( DWORD ) );
	nOffset += sizeof( DWORD );
	return true;
}

bool CsUnionTableMng::_ReadStringA( std::vector< unsigned char > const& data, size_t& nOffset, std::string& outValue ) const
{
	DWORD nLength = 0;
	if( _ReadDword( data, nOffset, nLength ) == false || nLength > 4096 )
		return false;
	if( data.size() < nOffset + nLength )
		return false;
	outValue.assign( reinterpret_cast< char const* >( &data[ nOffset ] ), reinterpret_cast< char const* >( &data[ nOffset ] ) + nLength );
	nOffset += nLength;
	return true;
}

bool CsUnionTableMng::_ReadStringW( std::vector< unsigned char > const& data, size_t& nOffset, std::wstring& outValue ) const
{
	DWORD nLength = 0;
	if( _ReadDword( data, nOffset, nLength ) == false || nLength > 4096 )
		return false;
	size_t nBytes = (size_t)nLength * sizeof( wchar_t );
	if( data.size() < nOffset + nBytes )
		return false;
	outValue.assign( reinterpret_cast< wchar_t const* >( &data[ nOffset ] ), reinterpret_cast< wchar_t const* >( &data[ nOffset ] ) + nLength );
	nOffset += nBytes;
	return true;
}

DWORD CsUnionTableMng::_ReadOffset( std::vector< unsigned char > const& data ) const
{
	if( data.size() < sizeof( DWORD ) )
		return 0;

	DWORD nOffset = 0;
	memcpy( &nOffset, &data[ 0 ], sizeof( DWORD ) );
	return nOffset;
}
