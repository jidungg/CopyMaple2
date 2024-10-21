#include "stdafx.h"
#include "ItemDataBase.h"
#include "JsonParser.h"

IMPLEMENT_SINGLETON(CItemDataBase)
CItemDataBase::CItemDataBase()
{
}


void CItemDataBase::Insert_Data(ITEM_DESC* pData)
{
	m_mapItem[(_uint)pData->eType].insert({ pData->strItemName,pData });

}


void CItemDataBase::Free()
{
	for (_uint i = 0; i < (_uint)ITEM_TYPE::LAST; ++i)
	{
		for (auto& item : m_mapItem[i])
		{
			Safe_Delete(item.second);
		}
		m_mapItem[i].clear();
	}
}
