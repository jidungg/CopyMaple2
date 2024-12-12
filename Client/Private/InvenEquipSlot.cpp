#include "stdafx.h"
#include "InvenEquipSlot.h"
#include "PlayerInfo.h"

CInvenEquipSlot::CInvenEquipSlot(_uint iIdx, CInventory* pInventory)
	:CInvenSlot(ITEM_TYPE::EQUIP, iIdx, pInventory)
{
}



void CInvenEquipSlot::On_RightClick()
{
	if (Is_Empty())
		return;
	PLAYERINIFO->Equip(this);
}

void CInvenEquipSlot::On_LeftClick()
{
	if (Is_Empty())
		return;
	int a = 0;
}

EQUIP_ITEM_TYPE CInvenEquipSlot::Get_EquipType()
{
	EQUIP_ITEM_DATA* pEqDesc = static_cast<EQUIP_ITEM_DATA*>( Get_ItemDesc());
	return pEqDesc->eEquipType;
}

CInvenEquipSlot* CInvenEquipSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenEquipSlot(iIdx, pInventory);
}

void CInvenEquipSlot::Free()
{
	__super::Free();
}
