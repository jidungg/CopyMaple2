#include "stdafx.h"
#include "PlayerEquipSlot.h"

CPlayerEquipSlot::CPlayerEquipSlot(EQUIP_ITEM_TYPE eType)
	: CBase()
	, m_eEquipType(eType)
{
}

const _char* CPlayerEquipSlot::Get_IconTag()
{
	if (m_pItemDesc != nullptr)
		return m_pItemDesc->strIconImageTag;
	else
		return nullptr;
}

HRESULT CPlayerEquipSlot::Insert_Item(EQUIP_ITEM_DESC* pData)
{
	assert(pData != nullptr);
	if (pData->eITemType != ITEM_TYPE::EQUIP)
		return E_FAIL;
	if (pData->eEquipType != m_eEquipType)
		return E_FAIL;

	m_pItemDesc = pData;
	return S_OK;
}

EQUIP_ITEM_DESC* CPlayerEquipSlot::Pop_Item()
{
	EQUIP_ITEM_DESC* pItem = m_pItemDesc;
	m_pItemDesc = nullptr;
	return pItem;
}



CPlayerEquipSlot* CPlayerEquipSlot::Create(EQUIP_ITEM_TYPE eType)
{
	return new CPlayerEquipSlot(eType);
}

void CPlayerEquipSlot::Free()
{
	__super::Free();
}
