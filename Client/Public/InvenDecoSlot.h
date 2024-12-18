#pragma once
#include "InvenSlot.h"

BEGIN(Client)
class CInvenDecoSlot :
    public CInvenSlot
{
private:
	CInvenDecoSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenDecoSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;
	DECO_ITEM_TYPE Get_DecorType();
private:
	DECO_ITEM_TYPE m_eDecoType = { DECO_ITEM_TYPE::LAST };

public:
	static CInvenDecoSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();
};




END