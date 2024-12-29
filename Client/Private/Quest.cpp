#include "stdafx.h"
#include "Quest.h"
#include "Player.h"
#include "QuestDataBase.h"
#include "PlayerInfo.h"
#include "Engine_Utility.h"
#include "Inventory.h"
#include "ItemDataBase.h"
#include "MonsterDataBase.h"

QuestData::QuestData(json& js)
{
	eClient = js["Client"];
	eQuestID = js["QuestId"];
	string str = js["Name"];
	strName = CEngineUtility::ConvertStringToWString(str);
	eState = js["State"];

	iAcceptableLevel = js["AcceptableLevel"];
	for (auto& jPreQuest : js["PrequisiteQuests"])
	{
		listPrequisiteQuest.push_back(jPreQuest.get<QUEST_ID>());
	}

	for (auto& jConversation : js["AcceptConversations"])
	{
		vectorAcceptConversation.push_back(jConversation);
	}

	for (auto& jCompleteCondition : js["CompleteConditions"])
	{
		QUEST_CONDITION_TYPE eType = jCompleteCondition["Type"];
		QUEST_CONDITION* pCondition = nullptr;
		switch (eType)
		{
		case Client::QUEST_CONDITION_TYPE::MONSTER:
			pCondition = new MONSTER_QUEST_CONDITION(jCompleteCondition);
			break;
		case Client::QUEST_CONDITION_TYPE::ITEM:
			pCondition = new ITEM_QUEST_CONDITION(jCompleteCondition);
			break;
		case Client::QUEST_CONDITION_TYPE::LAST:
		default:
			break;
		}
		listCompleteCondition.push_back(pCondition);
	}
	for (auto& jConversation : js["NotSatisfiedConversations"])
	{
		vectorNotSatisfiedConversation.push_back(jConversation);
	}
	for (auto& jConversation : js["CompleteConversations"])
	{
		vectorCompleteConversation.push_back(jConversation);
	}

	tReward = js["Reward"];
}

_bool QuestData::Is_SatisfiedAcceptCondition()
{
	CPlayer* pPlayer = PLAYERINIFO->Get_Player();
	if (pPlayer->Get_Stat().iLEVEL < iAcceptableLevel)
		return false;
	for (auto& eQuestId : listPrequisiteQuest)
	{
		if (false == QUESTDB->Is_QuestCompleted(eQuestId))
			return false;
	}

	return true;
}

_bool QuestData::Is_SatisfiedCompleteCondition()
{
	for (auto& pCondition : listCompleteCondition)
	{
		if (false == pCondition->Is_Satisfied())
			return false;
	}
	return true;
}

json QuestData::To_Json()
{
	json jResult;
	jResult["Client"] = eClient;
	jResult["QuestId"] = eQuestID;
	jResult["Name"] = CEngineUtility::ConvertWStringToString(strName);
	jResult["State"] = eState;

	jResult["AcceptableLevel"] = iAcceptableLevel;
	for (auto& eQuestId : listPrequisiteQuest)
	{
		jResult["PrequisiteQuests"].push_back(eQuestId);
	}

	for (auto& tConversation : vectorAcceptConversation)
	{
		jResult["AcceptConversations"].push_back(tConversation);
	}

	for (auto& pCondition : listCompleteCondition)
	{
		jResult["CompleteConditions"].push_back(pCondition->To_Json());
	}

	for (auto& tConversation : vectorNotSatisfiedConversation)
	{
		jResult["NotSatisfiedConversations"].push_back(tConversation);
	}

	for (auto& tConversation : vectorCompleteConversation)
	{
		jResult["CompleteConversations"].push_back(tConversation);
	}

	jResult["Reward"] = tReward.To_Json();
	return jResult;
}

_uint QuestData::Get_AccurateConversationIndex()
{
	switch (eState)
	{
	case Client::QUEST_STATE::NOT_ACCEPTED:
		return vectorAcceptConversation.front();
	case Client::QUEST_STATE::ACCEPTED:
	{
		if (Is_SatisfiedCompleteCondition())
			return vectorCompleteConversation.front();
		else
			return vectorNotSatisfiedConversation.front();
		break;
	}
	case Client::QUEST_STATE::COMPLETED:
	case Client::QUEST_STATE::LAST:
	default:
		break;
	}
	return _uint();
}

void QuestData::Increase_MonsterKillCount(MONSTER_ID eId)
{
	for (auto& pCondition : listCompleteCondition)
	{
		if (pCondition->eType == QUEST_CONDITION_TYPE::MONSTER)
		{
			MONSTER_QUEST_CONDITION* pMonsterCondition = static_cast<MONSTER_QUEST_CONDITION*>(pCondition);
			if (pMonsterCondition->iMonsterID == eId)
				pMonsterCondition->iCount++;
		}
	}
}

QuestReward::QuestReward(json& js)
{
	m_iExp = js["Exp"];
	m_iGold = js["Gold"];
	for (auto& item : js["Items"])
	{
		COMBIEND_ITEM_ID tItem(item["Item"]);
		m_eItemList.push_back({ tItem,item["Count"] });
	}
}

json QuestReward::To_Json()
{
	json jResult;
	jResult["Exp"] = m_iExp;
	jResult["Gold"] = m_iGold;
	for (auto& item : m_eItemList)
	{
		json jItem;
		jItem["Item"] = item.first.To_Json();
		jItem["Count"] = item.second;
		jResult["Items"].push_back(jItem);
	}
	return jResult;
}

QuestCondition::QuestCondition(json& js)
{
	eType = js["Type"];

	iRequiredCount = js["RequiredCount"];
}

json QuestCondition::To_Json()
{
	json jResult;
	jResult["Type"] = eType;

	jResult["RequiredCount"] = iRequiredCount;
	return jResult;
}

COMBIEND_ITEM_ID::COMBIEND_ITEM_ID(json& js)
{
	eItemType = js["Type"];
	switch (eItemType)
	{
	case Client::ITEM_TYPE::EQUIP:
	{
		EQUIP_ITEM_ID eID = js["Id"];
		iID = (_uint)eID;
		break;
	}
	case Client::ITEM_TYPE::DECO:
	{
		DECO_ITEM_ID eID = js["Id"];
		iID = (_uint)eID;
		break;
	}
	case Client::ITEM_TYPE::CONSUMABLE:
	{
		CONSUMABLE_ITEM_ID eID = js["Id"];
		iID = (_uint)eID;
		break;
	}
	case Client::ITEM_TYPE::BUILD:
	{
		iID = js["Id"];
		break;
	}
	case Client::ITEM_TYPE::ETC:
	{
		ETC_ITEM_ID eID = js["Id"];
		iID = (_uint)eID;
		break;
	}
	case Client::ITEM_TYPE::LAST:
		break;
	default:
		break;
	}
}

json COMBIEND_ITEM_ID::To_Json()
{
	json jResult;
	jResult["Type"] = eItemType;
	jResult["Id"] = iID;
	return jResult;
}

ItemQuestCondition::ItemQuestCondition(json& js)
	:QuestCondition(js)
{
	tItem = COMBIEND_ITEM_ID(js["Item"]);
}


_bool ItemQuestCondition::Is_Satisfied()
{
	return iRequiredCount <= (_int)INVENTORY->Get_ItemCount(tItem.eItemType, tItem.iID);
}

json ItemQuestCondition::To_Json()
{
	json jResult = __super::To_Json();
	jResult["Item"] = tItem.To_Json();
	return jResult;
}

wstring ItemQuestCondition::To_String()
{
	wstring wstrResult = L"";
	wstrResult += CEngineUtility::ConvertStringToWString( ITEMDB->Get_Data(tItem.eItemType, tItem.iID)->strItemName);
	wstrResult += L" 수집하기 (";
	wstrResult += to_wstring(INVENTORY->Get_ItemCount(tItem.eItemType, tItem.iID));
	wstrResult += L" / ";
	wstrResult += to_wstring(iRequiredCount);
	wstrResult += L")";
	return wstrResult;
}

MonsterQuestCondition::MonsterQuestCondition(json& js)
	:QuestCondition(js)
{
	iMonsterID = js["MonsterId"];
	iCount = js["Count"];
}

_bool MonsterQuestCondition::Is_Satisfied()
{
	return iRequiredCount <= iCount;
}

json MonsterQuestCondition::To_Json()
{
	json jResult = __super::To_Json();
	jResult["MonsterId"] = iMonsterID;
	jResult["Count"] = iCount;
	return jResult;
}

wstring MonsterQuestCondition::To_String()
{
	wstring wstrResult = L"";
	wstrResult += CEngineUtility::ConvertStringToWString(MONSTERDB->Get_Data(iMonsterID)->strMonsterName);
	wstrResult += L" 죽이기 (";
	wstrResult += to_wstring(iCount);
	wstrResult += L" / ";
	wstrResult += to_wstring(iRequiredCount);
	wstrResult += L")";
	return wstrResult;
}
