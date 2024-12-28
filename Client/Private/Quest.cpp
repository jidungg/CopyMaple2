#include "stdafx.h"
#include "Quest.h"

QuestData::QuestData(json& js)
{
	eQuestID = js["QuestId"];
	string str = js["Name"];
	strName = wstring(str.begin(), str.end());


}

QuestReward::QuestReward(json& js)
{
	m_iExp = js["Exp"];
	m_iGelder = js["Gelder"];
	for (auto& item : js["ItemList"])
	{
		COMBIEND_ITEM_ID tItem;
		tItem.eItemType = item["ItemType"];
		tItem.iID = item["ID"];
		m_eItemList.push_back({ tItem,item["Count"] });
	}
}
