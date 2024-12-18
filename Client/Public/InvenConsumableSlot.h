#pragma once
#include "InvenSlot.h"

BEGIN(Client)
class CInvenConsumableSlot :
	public CInvenSlot, IQuickItem
{
private:
	CInvenConsumableSlot(_uint iIdx, CInventory* pInventory);
	virtual ~CInvenConsumableSlot() = default;

public:
	virtual void On_RightClick(void*) override;
	virtual void On_LeftClick(void*) override;

	virtual const _char* Get_IconTag() override;
	virtual void Use() override;

private:
	CONSUMABLE_ITEM_TYPE m_eConsumableType = { CONSUMABLE_ITEM_TYPE::LAST };

public:
	static CInvenConsumableSlot* Create(_uint iIdx, CInventory* pInventory);
	void Free();


};
END