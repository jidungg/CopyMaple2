#include "stdafx.h"
#include "InvenSlot.h"
#include "Inventory.h"
#include "UIBundle.h"
#include "UIQuickSlot.h"

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
    m_pItemDesc = pData;
    m_iStackCount += iCount;
	UIBUNDLE->Update_Inven_Slot( m_iIndex, this);
    UIBUNDLE->Update_QuestGuide();
    return S_OK;
}

const ITEM_DATA* CInvenSlot::Pop_Item(_uint iCount)
{
    if (m_iStackCount < iCount)
        return nullptr;
    m_iStackCount -= iCount;
	const ITEM_DATA* pItem = m_pItemDesc;
    if(m_iStackCount == 0)
        m_pItemDesc = nullptr;
    UIBUNDLE->Update_Inven_Slot(m_iIndex, this);
    UIBUNDLE->Update_QuestGuide();
    return pItem;
}

bool CInvenSlot::Is_Insertable(const ITEM_DATA* pData, _uint iCount)
{
	if (Is_Empty())
		return true;
    if (m_eItemType != pData->eITemType)
        return false;
    if(m_pItemDesc!= nullptr &&
        m_pItemDesc->iItemID != pData->iItemID)
        return false;
    if (m_iStackCount + iCount > m_pItemDesc->m_iMaxStack)
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


