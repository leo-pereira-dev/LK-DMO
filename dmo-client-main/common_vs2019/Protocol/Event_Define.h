#pragma once

#include "../cItemData.h"

#include <map>

#pragma pack(push,1)
struct stHotTimeItemInfo
{
	stHotTimeItemInfo():nItemIdx(0), nItemCount(0)
	{};

	n4		nItemIdx;
	n4		nItemCount;
};
#pragma pack(pop)

namespace nsHotTimeEventState
{
	enum
	{
		NO_EVENT = 0,
		NOT_INTIME ,
		INTIME,
	};
}

struct stHotTimeDate
{
	int m_nYear;
	int m_nMonth;
	int m_nDay;
};

namespace nsDailyCheckEvent
{
	struct sCheckedBits
	{
		sCheckedBits()
		{
			memset(pData, 0, sizeof(pData));
		}

		bool IsCheck(uint nIndex) const
		{
			if( nIndex == 0 )
				return false;

			uint nZeroIndex = nIndex - 1;
			uint nByteIndex = nZeroIndex / 8;
			uint nBitIndex = nZeroIndex % 8;
			if( nByteIndex >= sizeof(pData) )
				return false;

			return (pData[nByteIndex] & (1 << nBitIndex)) != 0;
		}

		char pData[128];
	};

	struct sDailyCheckData
	{
		sDailyCheckData() : nGroupCode(0), nCurrentDay(0), nNextLeftTS(0) {}

		uint nGroupCode;
		uint nCurrentDay;
		uint nNextLeftTS;
		sCheckedBits cEventInfo;
	};

	struct sDailyCheckedItem
	{
		sDailyCheckedItem() : nItemPos(0) {}

		int nItemPos;
		cItemData cItem;
	};
}
