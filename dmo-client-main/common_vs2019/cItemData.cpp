#include "cItemData.h"

namespace
{
#ifdef COMPAT_487
	const u8 ITEM_EXTENDED_TYPE_MASK = 0x00000000FFFFFFFFull;
#endif
}

uint cItemData::GetType(void) const
{
#ifdef COMPAT_487
	u4 nExtendedType = static_cast<u4>(ExtraBytes & ITEM_EXTENDED_TYPE_MASK);
	if (nExtendedType != 0)
		return nExtendedType;
#endif

	return m_nType;
}

void cItemData::SetType( uint nType )
{
	m_nType = nType & 0x1FFFF;
#ifdef COMPAT_487
	ExtraBytes = (ExtraBytes & ~ITEM_EXTENDED_TYPE_MASK) | static_cast<u8>(nType);
#endif
}

void cItemData::SetCount( uint nCount )
{
	m_nCount = nCount;
}
