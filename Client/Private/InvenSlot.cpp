#include "stdafx.h"
#include "InvenSlot.h"
#include "Inventory.h"

CInvenSlot::CInvenSlot(ITEM_TYPE eItemType, _uint iIdx, CInventory* pInventory)
    :m_eItemType(eItemType)
	, m_iIndex(iIdx)
    ,m_pInventory(pInventory)
{
    switch (m_eItemType)
    {
    case Client::ITEM_TYPE::EQUIP:
    case Client::ITEM_TYPE::DECO:
		m_iMaxStack = 1;
        break;
    case Client::ITEM_TYPE::CONSUMABLE:
    case Client::ITEM_TYPE::BUILD:
    case Client::ITEM_TYPE::ETC:
		m_iMaxStack = 99;
        break;
    case Client::ITEM_TYPE::LAST:
    default:
        break;
    }
}

HRESULT CInvenSlot::Insert_Item(ITEM_DESC* pData, _uint iCount)
{
	if (Is_Insertable(pData, iCount) == false)
		return E_FAIL;
    m_pItemDesc = pData;
	m_iStack += iCount;
	m_pInventory->Update_SlotUI(this);
    return S_OK;
}

ITEM_DESC* CInvenSlot::Pop_Item(_uint iCount)
{
    if (m_iStack < iCount)
        return nullptr;
	m_iStack -= iCount;
	ITEM_DESC* pItem = m_pItemDesc;
	if (m_iStack <= 0)
        m_pItemDesc = nullptr;
	m_pInventory->Update_SlotUI(this);
    return pItem;
}

bool CInvenSlot::Is_Insertable(ITEM_DESC* pData, _uint iCount)
{
	if (m_pItemDesc == nullptr)
		return true;
    if (m_pItemDesc->eITemType != pData->eITemType
        || m_pItemDesc->iItemID != pData->iItemID)
        return false;
    if (m_iStack + iCount > m_iMaxStack)
        return false;
    return true;
}

void CInvenSlot::Free()
{
    __super::Free();
}

const _char* CInvenSlot::Get_IconTag()
{
    return m_pItemDesc->strIconImageTag;
}


