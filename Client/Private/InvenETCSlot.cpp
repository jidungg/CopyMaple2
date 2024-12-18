
#include "stdafx.h"
#include "InvenETCSlot.h"

CInvenETCSlot::CInvenETCSlot(_uint iIdx, CInventory* pInventory)
	: CInvenSlot(ITEM_TYPE::ETC, iIdx, pInventory)
{
}

void CInvenETCSlot::On_RightClick(void*)
{
	if (Is_Empty())
		return;
}

void CInvenETCSlot::On_LeftClick(void*)
{
	if (Is_Empty())
		return;
}

CInvenETCSlot* CInvenETCSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenETCSlot(iIdx, pInventory);
}

void CInvenETCSlot::Free()
{
	__super::Free();
}
