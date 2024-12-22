#include "stdafx.h"
#include "PlayerInfoSlot.h"
#include "GameInstance.h"
#include "UIBundle.h"


CPlayerInfoSlot::CPlayerInfoSlot(ITEM_TYPE eType)
	:CBase()
	, m_eItemType(eType)
{
}

HRESULT CPlayerInfoSlot::Insert_Item(const ITEM_DATA* pData)
{
	if (Is_Insertable(pData) == false)
		return E_FAIL;

	m_pItemData = pData;
	list<CUIPlayerInfo::SLOT_ID>& listSlotId = CUIPlayerInfo::Item_To_SlotID(m_pItemData);
	for (auto& id : listSlotId)
	{
		UIBUNDLE->Update_PlayerInfo_Slot((_uint)id, this);
	}
	return S_OK;
}

const ITEM_DATA* CPlayerInfoSlot::Pop_Item()
{
	if (Is_Empty())
		return nullptr;

	const ITEM_DATA* pItem = m_pItemData;
	m_pItemData = nullptr;
	list<CUIPlayerInfo::SLOT_ID>& listSlotId = CUIPlayerInfo::Item_To_SlotID(pItem);
	for (auto& id : listSlotId)
	{
		UIBUNDLE->Update_PlayerInfo_Slot((_uint)id, this);
	}
	return pItem;
}

_bool CPlayerInfoSlot::Is_Insertable(const ITEM_DATA* pData)
{
	assert(pData != nullptr);
	if (m_eItemType != pData->eITemType)
		return false;
	if (m_pItemData != nullptr)
		return false;

	return true;
}

CPlayerInfoSlot* CPlayerInfoSlot::Create(ITEM_TYPE eType)
{
	return new CPlayerInfoSlot(eType);
}

void CPlayerInfoSlot::Free()
{
	__super::Free();
}
