#include "stdafx.h"
#include "InvenConsumableSlot.h"
#include "PlayerInfo.h"
#include "Player.h"
#include "HitEvent.h"
#include "GameInstance.h"

CInvenConsumableSlot::CInvenConsumableSlot(_uint iIdx, CInventory* pInventory)
	: CInvenSlot(ITEM_TYPE::CONSUMABLE, iIdx, pInventory)
{
}

void CInvenConsumableSlot::On_RightClick(void*)
{
	if (Is_Empty())
		return;
	Use();
}

void CInvenConsumableSlot::On_LeftClick(void*)
{
	if (Is_Empty())
		return;
}

const _char* CInvenConsumableSlot::Get_IconTag()
{
	return static_cast<const CONSUMABLE_ITEM_DATA*>(Get_ItemData())->strIconImageTag;
}

void CInvenConsumableSlot::Use()
{
	const ITEM_DATA* pItemData = Pop_Item();
	if (pItemData)
	{
		const CONSUMABLE_ITEM_DATA* pConsumable = static_cast<const CONSUMABLE_ITEM_DATA*>(pItemData);
		switch (pConsumable->eConsumableType)
		{
		case CONSUMABLE_ITEM_TYPE::HP:
		{
			_int iRecovery = pConsumable->iValue;
			CPlayer* pPlayer = PLAYERINIFO->Get_Player();
			CGameInstance::GetInstance()->Push_Event(CDamgEvent::Create(pPlayer, pPlayer, -iRecovery, false, true, EFF_MODEL_ID::RECOVER_HP));
			break;
		}
		default:
			break;
		}

	}

}



CInvenConsumableSlot* CInvenConsumableSlot::Create(_uint iIdx, CInventory* pInventory)
{
	return new CInvenConsumableSlot(iIdx, pInventory);
}

void CInvenConsumableSlot::Free()
{
	__super::Free();
}
