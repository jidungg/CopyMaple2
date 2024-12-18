#include "stdafx.h"
#include "InvenDecoSlot.h"
#include "PlayerInfo.h"

CInvenDecoSlot::CInvenDecoSlot(_uint iIdx, CInventory* pInventory)
	: CInvenSlot(ITEM_TYPE::DECO, iIdx, pInventory)
{
}


void CInvenDecoSlot::On_RightClick(void*)
{
	if (Is_Empty())
		return;
	PLAYERINIFO->Equip(this);
}


void CInvenDecoSlot::On_LeftClick(void*)
{
	if (Is_Empty())
		return;
	int a = 0;
}

DECO_ITEM_TYPE CInvenDecoSlot::Get_DecorType()
{
	return m_eDecoType;
}

CInvenDecoSlot* CInvenDecoSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenDecoSlot(iIdx, pInventory);
}

void CInvenDecoSlot::Free()
{
	__super::Free();
}

//======================CONSUMABLE=============================================

CInvenConsumableSlot::CInvenConsumableSlot(_uint iIdx, CInventory* pInventory)
	: CInvenSlot(ITEM_TYPE::CONSUMABLE, iIdx, pInventory)
{
}

void CInvenConsumableSlot::On_RightClick(void*)
{
	if (Is_Empty())
		return;
}

void CInvenConsumableSlot::On_LeftClick(void*)
{
	if (Is_Empty())
		return;
}

CInvenConsumableSlot* CInvenConsumableSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenConsumableSlot(iIdx, pInventory);
}

void CInvenConsumableSlot::Free()
{
	__super::Free();
}


//======================BUILD=============================================


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
