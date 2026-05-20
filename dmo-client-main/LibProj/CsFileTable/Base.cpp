
#include "stdafx.h"
#include "Base.h"


void CsBase::Delete()
{

}

void CsBase::Init( sINFO* pSrcInfo )
{
	memcpy( &m_Info, pSrcInfo, sizeof( sINFO ) );
}



void CsBaseJumpBuster::Delete()
{
	m_mapJumpMapID.clear();
}

void CsBaseJumpBuster::Insert( DWORD nDestMapID )
{
	if( m_mapJumpMapID.find( nDestMapID ) != m_mapJumpMapID.end() )
		return;

	m_mapJumpMapID[ nDestMapID ] = nDestMapID;
}
