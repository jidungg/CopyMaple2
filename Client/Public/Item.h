#pragma once
#include "Base.h"
#include "Model.h"
#include "Engine_Utility.h"

BEGIN(Client)
typedef struct ItemData 
{
	ItemData() = default;
	ItemData(json& js)
	{

		string str = js["Name"];
		wstring wstr = CEngineUtility::ConvertStringToWString(str);
		std::copy(str.begin(), str.end(), strItemName);
		str = js["Desc"];
		wstr = CEngineUtility::ConvertStringToWString(str);
		std::copy(str.begin(), str.end(), strItemDesc);
		str = js["IconImg"];
		wstr = CEngineUtility::ConvertStringToWString(str);
		std::copy(str.begin(), str.end(), szIconImageTag);
		str = js["Model"];
		wstr = CEngineUtility::ConvertStringToWString(str);
		std::copy(str.begin(), str.end(), strModelTag);
		iPrice = js["Price"];
		eItemGrade = js["ItemGrade"];

	}

	ITEM_TYPE eITemType = ITEM_TYPE::LAST;
	ITEM_GRADE eItemGrade = ITEM_GRADE::LAST;
	_uint iItemID = { UINT_MAX };
	_tchar strItemName[MAX_PATH] = TEXT("");
	_tchar strItemDesc[MAX_PATH] = TEXT("");
	_tchar szIconImageTag[MAX_PATH] = TEXT("");
	_tchar strModelTag[MAX_PATH] = TEXT("");
	_uint iPrice = 0;
	_uint m_iMaxStack = { 0 };


}ITEM_DATA;
typedef struct BuildItemData : public ItemData
{
	BuildItemData() = default;
	BuildItemData(string strFileName, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BuildItemData(json& js)
	{
		eITemType = ITEM_TYPE::BUILD;
		eBuildType = js["BuildItemType"];
		eBlockType = js["BuildItemBlockType"];
		iItemID = js["ItemId"];

		string str = js["IconImg"];
		std::copy(str.begin(), str.end(), szIconImageTag);
		str = js["Model"];
		std::copy(str.begin(), str.end(), strModelTag);
	}
	BUILD_ITEM_BLOCK_TYPE eBlockType ={ BUILD_ITEM_BLOCK_TYPE::LAST};
	BUILD_ITEM_TYPE eBuildType = { BUILD_ITEM_TYPE::LAST };
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
		m_iMaxStack = 1;

		json& jStat = js["Status"];
		iATK = jStat["ATK"];
		iDEF = jStat["DEF"];
		iSTR = jStat["STR"];
		iDEX = jStat["DEX"];
		iINT = jStat["INT"];
		iLUK = jStat["LUK"];
		iHP = jStat["HP"];
		fCRIT= jStat["CRIT"];
	}
	EQUIP_ITEM_TYPE eEquipType;

	_int iATK = { 0 };
	_int iDEF = { 0 };
	_int iSTR = { 0 };
	_int iDEX = { 0 };
	_int iINT = { 0 };
	_int iLUK = { 0 };
	_int iHP = { 0 };
	_float fCRIT = { 0 };

}EQUIP_ITEM_DATA;

typedef struct ConsumableItemData : public ItemData
{
	ConsumableItemData() = default;
	ConsumableItemData(json& js) : ItemData(js)
	{
		eITemType = ITEM_TYPE::CONSUMABLE;
		CONSUMABLE_ITEM_ID eId = js["ItemId"];
		iItemID = (_uint)eId;
		eConsumableType = js["ConsumType"];
		iValue = js["Value"];
		fCoolTime = js["CoolTime"];
		m_iMaxStack = 99;
	}
	CONSUMABLE_ITEM_TYPE eConsumableType = { CONSUMABLE_ITEM_TYPE::LAST };
	_int iValue = { 0 };
	_float fCoolTime = { 0.f };
}CONSUMABLE_ITEM_DATA;

typedef struct DecoItemData : public ItemData
{
	DecoItemData() = default;
	DecoItemData(json& js) : ItemData(js)
	{
		eITemType = ITEM_TYPE::DECO;
		DECO_ITEM_ID eId = js["ItemId"];
		iItemID = (_uint)eId;
		eDecoType = js["ConsumType"];
		m_iMaxStack = 1;
		m_iMaxStack = 99;
	}
	DECO_ITEM_TYPE eDecoType = { DECO_ITEM_TYPE::LAST };
}DECO_ITEM_DATA;

typedef struct ETCItemData : public ItemData
{
	ETCItemData() = default;
	ETCItemData(json& js) : ItemData(js)
	{
		eITemType = ITEM_TYPE::ETC;
		ETC_ITEM_ID eId = js["ItemId"];
		iItemID = (_uint)eId;
		m_iMaxStack = 99;
	}
}ETC_ITEM_DATA;
END