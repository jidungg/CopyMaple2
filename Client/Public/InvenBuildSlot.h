#pragma once
#include "InvenSlot.h"

BEGIN(Client)


class CInvenBuildSlot :
	public CInvenSlot
{
private:
	CInvenBuildSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenBuildSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
private:


public:
	static CInvenBuildSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};

END