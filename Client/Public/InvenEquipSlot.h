#pragma once
#include "InvenSlot.h"

BEGIN(Client)
class CInvenEquipSlot :
    public CInvenSlot
{
private:
	CInvenEquipSlot( _uint iIdx, CInventory* pInventory);
	virtual ~CInvenEquipSlot() = default;

public:
	virtual void On_RightClick() override;
	virtual void On_LeftClick() override;
	EQUIP_ITEM_TYPE Get_EquipType();
private:


public:
	static CInvenEquipSlot* Create( _uint iIdx, CInventory* pInventory);
	void Free();

};

END