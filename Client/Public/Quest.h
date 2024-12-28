#pragma once
#include "Base.h"
#include "Item.h"
BEGIN(Client)
enum class QUEST_TYPE
{

};
struct COMBIEND_ITEM_ID
{
	ITEM_TYPE eItemType = { ITEM_TYPE::LAST };
	_uint iID = { UINT_MAX };
};
typedef struct QuestReward
{
	QuestReward() = default;
	QuestReward(json& js);
	_int m_iExp = 0;
	_int m_iGelder = 0;
	list<pair<COMBIEND_ITEM_ID, _int>> m_eItemList;
}REWARD;
typedef struct QuestData
{
	QuestData() = default;
	QuestData(json& js);
	QUEST_ID eQuestID = { QUEST_ID::LAST };
	wstring strName;
	MONSTER_ID eTargetMonster = { MONSTER_ID::LAST };
	COMBIEND_ITEM_ID eTargetItem = {};
	NPC_ID eClient = { NPC_ID::NPCID_LAST };
	REWARD tReward;
	_int iMonsterGoal = 0;
	_int iMonsterCount= 0;
	_int iItemGoal = 0;
	_int iItemCount = 0;
}QUEST_DATA;

END