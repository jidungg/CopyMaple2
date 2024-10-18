#pragma once
#include "Base.h"


BEGIN(Client)
typedef struct ItemDesc : public UIListItemData
{
	ItemDesc() = default;
	ItemDesc(json& js)
	{
		eType = ITEM_TYPE(js["type"]);
		string str = js["name"];
		std::copy(str.begin(), str.end(), strItemName);
		str = js["desc"];
		std::copy(str.begin(), str.end(), strItemDesc);
		str = js["iconimg"];
		std::copy(str.begin(), str.end(), strIconImageTag);
		str = js["model"];
		std::copy(str.begin(), str.end(), strModelTag);
		iPrice = js["price"];
	}
	ITEM_TYPE eType = ITEM_TYPE::ITEM_TYPE_END;
	_char strItemName[MAX_PATH] = ("");
	_char strItemDesc[MAX_PATH] = ("");
	_char strIconImageTag[MAX_PATH] = ("");
	_char strModelTag[MAX_PATH] = ("");
	_uint iPrice = 0;

}ITEM_DESC;

END