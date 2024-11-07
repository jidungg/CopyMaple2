#pragma once
#include "Base.h"
#include "Model.h"

BEGIN(Client)
typedef struct ItemData : public UIListItemData
{
	ItemData() = default;
	ItemData(json& js)
	{

		string str = js["Name"];
		std::copy(str.begin(), str.end(), strItemName);
		str = js["Desc"];
		std::copy(str.begin(), str.end(), strItemDesc);
		str = js["IconImg"];
		std::copy(str.begin(), str.end(), strIconImageTag);
		str = js["Model"];
		std::copy(str.begin(), str.end(), strModelTag);
		iPrice = js["Price"];
		eItemGrade = js["ItemGrade"];
	}
	ITEM_TYPE eITemType = ITEM_TYPE::LAST;
	ITEM_GRADE eItemGrade = ITEM_GRADE::LAST;
	_uint iItemID = 0;
	_char strItemName[MAX_PATH] = ("");
	_char strItemDesc[MAX_PATH] = ("");
	_char strIconImageTag[MAX_PATH] = ("");
	_char strModelTag[MAX_PATH] = ("");
	_uint iPrice = 0;

}ITEM_DATA;
typedef struct BuildItemData : public ItemData
{
	BuildItemData() = default;
	BuildItemData(json& js) : ItemData(js)
	{
		eITemType = ITEM_TYPE::BUILD;
		eBuildType =js["BuildItemType"];
		BUILD_ITEM_ID eId = js["ItemId"];
		iItemID = (_uint)eId;

	}
	BUILD_ITEM_TYPE eBuildType;
}BUILD_ITEM_DATA;
typedef struct EquipItemData : public ItemData
{
	EquipItemData() = default;
	EquipItemData(json& js) : ItemData(js)
	{
		eITemType = ITEM_TYPE::EQUIP;
		eEquipType = js["EquipItemType"];
		EQUIP_ITEM_ID eId = js["ItemId"];
		iItemID = (_uint)eId;
	}
	EQUIP_ITEM_TYPE eEquipType;
}EQUIP_ITEM_DATA;
END