#pragma once

#include "pLimit.h"
#include "nlib/base.h"

class cItemSlot
{
public:
	cItemSlot(void) : idx(0), maker(0) { }
	~cItemSlot(void) { }

public:
	n8 idx;
	n4 maker;
};

#pragma pack(push)
#pragma pack(4)

class cItemData
{
public:
	cItemData(void) { Clear(); }
	~cItemData(void) { }

public:
	uint GetType(void) const;
	uint GetCount(void) const { return m_nCount; }

	void SetType(uint nType);
	void SetCount(uint nCount);

	void Clear(void) { memset(this, 0, sizeof(cItemData)); }
	void Set(int nType, int nCount) { SetType(nType); SetCount(nCount); m_nRate = 100; m_nLevel = 1; }
	void Set(int nType, int nCount, int nRate, int nLevel) { SetType(nType); SetCount(nCount); m_nRate = nRate; m_nLevel = nLevel; }

public:
	void operator=(cItemData const& x)
	{
		m_nAll = x.m_nAll;
		m_nRate = x.m_nRate;
		m_nLevel = x.m_nLevel;
		m_nLimited = x.m_nLimited;
		for (int i = 0; i < nLimit::SocketSlot; ++i)
		{
			m_nSockItemType[i] = x.m_nSockItemType[i];
			m_nSockAppRate[i] = x.m_nSockAppRate[i];
		}
		for (int i = 0; i < nLimit::MAX_ACCESSORY_OPTION; ++i)
		{
			m_nAccOption[i] = x.m_nAccOption[i];
			m_nAccValues[i] = x.m_nAccValues[i];
		}
		m_nEndTime = x.m_nEndTime;
		m_nRemainTradeLimitTime = x.m_nRemainTradeLimitTime;
#ifdef COMPAT_487
		ExtraBytes = x.ExtraBytes;
#endif
	}

public:
	bool IsNoActive() { return (m_nEndTime == 0xFFFFFFF0) ? true : false; }
	bool IsActive()
	{
		if (m_nEndTime != 0 && m_nEndTime != 0xFFFFFFFF && m_nEndTime != 0xFFFFFFF0)
			return true;
		else
			return false;
	}
	void SetEndTime(u4 time) { m_nEndTime = time; }
	u4 GetEndTime() { return m_nEndTime; }

public:
	union
	{
		struct
		{
			u4 m_nType : 17;
			u4 m_nCount : 15;
		};

		struct
		{
			u4 : 17;
			u4 m_nEndurance : 15;
		};

		u4 m_nAll;
	};

public:
	u1 m_nRate;
	u1 m_nLevel;
	u1 m_nLimited;

	u2 m_nSockItemType[nLimit::SocketSlot];
	u1 m_nSockAppRate[nLimit::SocketSlot];

	u2 m_nAccOption[nLimit::MAX_ACCESSORY_OPTION];
	u2 m_nAccValues[nLimit::MAX_ACCESSORY_OPTION];

public:
	u4 m_nEndTime;
	u4 m_nRemainTradeLimitTime;

#ifdef COMPAT_487
	u8 ExtraBytes;
#endif
};

struct stAccessory
{
	u4 nSkillCode;
	u1 nGainCnt;
	u1 nEnchantCnt;
	u2 nOption[nLimit::MAX_ACCESSORY_EXCELOPTION];
	u2 nMinValue[nLimit::MAX_ACCESSORY_EXCELOPTION];
	u2 nMaxValue[nLimit::MAX_ACCESSORY_EXCELOPTION];
};

struct stAccEnchant
{
	u4 nSkillCode;
	u1 nEnchantOption;
	u2 SuccessPer;
	u2 SucValue;
	u2 FailPer;
	u2 FailValue;
};

typedef struct _stItemInfo
{
	u2 slotNo;
	n4 slotType;
	u4 idx;
	u4 cnt;
} stItemInfo;

#pragma pack(pop)
