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


//======================BUILD=============================================


