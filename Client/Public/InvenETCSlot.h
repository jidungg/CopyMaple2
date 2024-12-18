#pragma once
#include "InvenSlot.h"

BEGIN(Client)
class CInvenETCSlot :
	public CInvenSlot
{
private:
	CInvenETCSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenETCSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
private:


public:
	static CInvenETCSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};
END