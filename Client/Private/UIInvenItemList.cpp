#include "stdafx.h"
#include "UIInvenItemList.h"
#include "UIInvenSlotEntry.h"

CUIInvenItemList::CUIInvenItemList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIListSelector(pDevice, pContext)	
{
}

CUIInvenItemList::CUIInvenItemList(const CUIInvenItemList& Prototype)
	:CUIListSelector(Prototype)
{
}

HRESULT CUIInvenItemList::Set_ItemData(list<CInvenSlot*>* listData)
{
	if (listData->size() > Get_ItemCount())
		return E_FAIL;

	_uint iItemIdx = 0;
	for (auto& data : *listData)
	{
		if (FAILED(static_cast<CUIInvenSlotEntry*>( m_vecUIItem[iItemIdx])->On_InvenListItemDataSet(data)))
			return E_FAIL;
		iItemIdx++;
	}
	return S_OK;
}

HRESULT CUIInvenItemList::Set_ItemData(_uint iIndex, CInvenSlot* Data)
{
	if (iIndex > Get_ItemCount())
		return E_FAIL;

	return static_cast<CUIInvenSlotEntry*>(m_vecUIItem[iIndex])->On_InvenListItemDataSet(Data);
}
