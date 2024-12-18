#include "stdafx.h"
#include "InvenSlot.h"
#include "Inventory.h"
#include "UIBundle.h"
CInvenSlot::CInvenSlot(ITEM_TYPE eItemType, _uint iIdx, CInventory* pInventory)
    :m_eItemType(eItemType)
	, m_iIndex(iIdx)
    ,m_pInventory(pInventory)
{
   
}

HRESULT CInvenSlot::Insert_Item(const ITEM_DATA* pData, _uint iCount)
{
	if (Is_Insertable(pData, iCount) == false)
		return E_FAIL;
    _uint iOriginCount = m_tItemDesc.m_iStack;
    m_tItemDesc = *pData;
    m_tItemDesc.m_iStack = iOriginCount+ iCount;
	UIBUNDLE->Update_Inven_Slot( m_iIndex, &m_tItemDesc);
    return S_OK;
}

ITEM_DATA* CInvenSlot::Pop_Item(_uint iCount)
{
    if (m_tItemDesc.m_iStack < iCount)
        return nullptr;
    m_tItemDesc.m_iStack -= iCount;
	ITEM_DATA* pItem = &m_tItemDesc;
	if (m_tItemDesc.m_iStack <= 0)
        m_tItemDesc = ITEM_DATA{};
    UIBUNDLE->Update_Inven_Slot(m_iIndex, &m_tItemDesc);
    return pItem;
}

bool CInvenSlot::Is_Insertable(const ITEM_DATA* pData, _uint iCount)
{
	if (Is_Empty())
		return true;
    if (m_tItemDesc.eITemType != pData->eITemType
        || m_tItemDesc.iItemID != pData->iItemID)
        return false;
    if (m_tItemDesc.m_iStack + iCount > m_tItemDesc.m_iMaxStack)
        return false;
    return true;
}

void CInvenSlot::Free()
{
    __super::Free();
}

//const _char* CInvenSlot::Get_IconTag()
//{
//    return m_pItemDesc->strIconImageTag;
//}


