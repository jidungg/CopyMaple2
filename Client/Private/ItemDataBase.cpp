#include "stdafx.h"
#include "ItemDataBase.h"
#include "JsonParser.h"

IMPLEMENT_SINGLETON(CItemDataBase)
CItemDataBase::CItemDataBase()
{
}

HRESULT CItemDataBase::LoadFromJson()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/ItemData.json", &j)))
		return E_FAIL;
	for (auto& item : j["items"])
	{
		ITEM_DESC* pItemDesc = new ITEM_DESC(item);
		m_mapItem[(_uint)pItemDesc->eType].insert({pItemDesc->wstrItemName,pItemDesc});
	}
 	return S_OK;
}


void CItemDataBase::Free()
{
	for (_uint i = 0; i < (_uint)ITEM_TYPE::ITEM_TYPE_END; ++i)
	{
		for (auto& item : m_mapItem[i])
		{
			Safe_Delete(item.second);
		}
		m_mapItem[i].clear();
	}
}
