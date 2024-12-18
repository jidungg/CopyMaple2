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

HRESULT CPlayerEquipSlot::Insert_Item(const EQUIP_ITEM_DATA* pData)
{
	assert(pData != nullptr);
	if (pData->eITemType != ITEM_TYPE::EQUIP)
		return E_FAIL;
	if (pData->eEquipType != m_eEquipType)
		return E_FAIL;

	m_pItemDesc = pData;
	return S_OK;
}

const EQUIP_ITEM_DATA* CPlayerEquipSlot::Pop_Item()
{
	const EQUIP_ITEM_DATA* pItem = m_pItemDesc;
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
