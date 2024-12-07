#include "stdafx.h"
#include "ItemDataBase.h"
#include "JsonParser.h"

IMPLEMENT_SINGLETON(CItemDataBase)
CItemDataBase::CItemDataBase()
{
}


HRESULT CItemDataBase::Load_Data()
{

	fs::path path;
	path = TEXT("../Bin/resources/FBXs/MAP");
	_uint iItemID = 0;
	for (const auto& entry : recursive_directory_iterator(path)) {
		if (entry.path().extension() == ".model") {
			BUILD_ITEM_DATA* pItemDesc = new BUILD_ITEM_DATA(entry.path().filename().string());
			pItemDesc->iItemID = iItemID++;
			Insert_Data(pItemDesc);
		}
	}

	json j;
	ITEM_DATA* pItemDesc;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/ETCBuildItemData.json", &j)))
		return E_FAIL;
	for (auto& item : j["items"])
	{
 		pItemDesc = new BUILD_ITEM_DATA(item);
		Insert_Data(pItemDesc);
	}

	j.clear();
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/EquipItemData.json", &j)))
		return E_FAIL;
	for (auto& item : j["items"])
	{
		pItemDesc = new EQUIP_ITEM_DATA(item);
		Insert_Data(pItemDesc);
	}

	//j.clear();
	//if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/ConsumableItemData.json", &j)))
	//	return E_FAIL;
	//for (auto& item : j["items"])
	//{
	//	 pItemDesc = new EQUIP_ITEM_DESC(item);
	//	Insert_Data( pItemDesc);
	//}

	//j.clear();
	//if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/DecoItemData.json", &j)))
	//	return E_FAIL;
	//for (auto& item : j["items"])
	//{
	//	pItemDesc = new EQUIP_ITEM_DESC(item);
	//	Insert_Data(pItemDesc);
	//}

	//j.clear();
	//if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/ETCItemData.json", &j)))
	//	return E_FAIL;
	//for (auto& item : j["items"])
	//{
	//	pItemDesc = new EQUIP_ITEM_DESC(item);
	//	Insert_Data(pItemDesc);
	//}
	return S_OK;
}

void CItemDataBase::Insert_Data(ITEM_DATA* pData)
{
	m_mapItem[(_uint)pData->eITemType].insert({ pData->iItemID ,pData });
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
