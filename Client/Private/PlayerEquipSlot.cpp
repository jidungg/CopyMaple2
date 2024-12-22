#include "stdafx.h"
#include "PlayerEquipSlot.h"
#include "UIBundle.h"
#include "UIPlayerInfo.h"

CPlayerEquipSlot::CPlayerEquipSlot(EQUIP_ITEM_TYPE eType)
	: CPlayerInfoSlot(ITEM_TYPE::EQUIP)
{
}

CPlayerEquipSlot* CPlayerEquipSlot::Create(EQUIP_ITEM_TYPE eType)
{
	return new CPlayerEquipSlot(eType);
}

void CPlayerEquipSlot::Free()
{
	__super::Free();
}

