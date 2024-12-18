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

	CPlayer* pPlayer = PLAYERINIFO->Get_Player();
	_uint iRecovery = static_cast<const CONSUMABLE_ITEM_DATA*>(Get_ItemData())->iValue;
	CGameInstance::GetInstance()->Push_Event(CDamgEvent::Create(pPlayer, pPlayer, iRecovery,false,true,EFF_MODEL_ID::RECOVER_HP));
	Pop_Item();
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
