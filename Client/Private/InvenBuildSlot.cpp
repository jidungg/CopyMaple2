#include "stdafx.h"
#include "InvenBuildSlot.h"


CInvenBuildSlot::CInvenBuildSlot(_uint iIdx, CInventory* pInventory)
	: CInvenSlot(ITEM_TYPE::BUILD, iIdx, pInventory)
{
}
void CInvenBuildSlot::On_RightClick(void*)
{
	if (Is_Empty())
		return;
}

void CInvenBuildSlot::On_LeftClick(void*)
{
	if (Is_Empty())
		return;

}

CInvenBuildSlot* CInvenBuildSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenBuildSlot(iIdx, pInventory);
}

void CInvenBuildSlot::Free()
{
	__super::Free();
}