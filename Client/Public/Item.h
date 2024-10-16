#pragma once
#include "Base.h"


BEGIN(Client)
typedef struct ItemDesc
{
	ItemDesc() = default;
	ItemDesc(json& js)
	{
		eType = ITEM_TYPE(js["type"]);
		string str = js["name"];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstrItemName, MAX_PATH);
		str = js["desc"];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstrItemDesc, MAX_PATH);
		str = js["iconimg"];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstrIconImageTag, MAX_PATH);
		str = js["model"];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstrModelTag, MAX_PATH);
		iPrice = js["price"];
	}
	ITEM_TYPE eType = ITEM_TYPE::ITEM_TYPE_END;
	_tchar wstrItemName[MAX_PATH] = TEXT("");
	_tchar wstrItemDesc[MAX_PATH] = TEXT("");
	_tchar wstrIconImageTag[MAX_PATH] = TEXT("");
	_tchar wstrModelTag[MAX_PATH] = TEXT("");
	_uint iPrice = 0;

}ITEM_DESC;

END