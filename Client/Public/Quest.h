#pragma once
#include "Base.h"
#include "Item.h"
#include "Conversation.h"

BEGIN(Client)

struct COMBIEND_ITEM_ID
{
	COMBIEND_ITEM_ID() = default;
	COMBIEND_ITEM_ID(json& js);
	json To_Json();
	ITEM_TYPE eItemType = { ITEM_TYPE::LAST };
	_uint iID = { UINT_MAX };
};
typedef struct QuestReward
{
	QuestReward() = default;
	QuestReward(json& js);
	json To_Json() ;
	_int m_iExp = { 0 };
	_int m_iGold = { 0 };
	list<pair<COMBIEND_ITEM_ID, _int>> m_eItemList;
}REWARD;
typedef struct QuestCondition
{
	QuestCondition() = default;
	QuestCondition(json& js);
	virtual _bool Is_Satisfied() abstract;
	virtual json To_Json();
	virtual wstring To_String() abstract;

	QUEST_CONDITION_TYPE eType = { QUEST_CONDITION_TYPE::LAST };
	_int iRequiredCount = { 0 };
}QUEST_CONDITION;
typedef struct MonsterQuestCondition : public QuestCondition
{
	MonsterQuestCondition() = default;
	MonsterQuestCondition(json& js);
	virtual _bool Is_Satisfied() override;
	virtual json To_Json() override;
	virtual wstring To_String() override;
	MONSTER_ID iMonsterID = { MONSTER_ID::SNAIL };
	_int iCount = { 0 };
}MONSTER_QUEST_CONDITION;
typedef struct ItemQuestCondition : public QuestCondition
{
	ItemQuestCondition() = default;
	ItemQuestCondition(json& js);
	virtual _bool Is_Satisfied() override;
	virtual json To_Json() override;
	virtual wstring To_String() override;
	COMBIEND_ITEM_ID tItem;
}ITEM_QUEST_CONDITION;
typedef struct QuestData
{
	QuestData() = default;
	QuestData(json& js);
	~QuestData()
	{
		for (auto& pCondition : listCompleteCondition)
		{
			Safe_Delete(pCondition);
		}
	}
	_bool Is_SatisfiedAcceptCondition();
	_bool Is_SatisfiedCompleteCondition();
	json To_Json();
	_uint Get_AccurateConversationIndex();
	void Increase_MonsterKillCount(MONSTER_ID eId);
	
	NPC_ID eClient = { NPC_ID::NPCID_LAST };
	QUEST_ID eQuestID = { QUEST_ID::LAST };
	wstring strName;
	QUEST_STATE eState = { QUEST_STATE::NOT_ACCEPTED };
	_uint iAcceptableLevel = 0;
	list<QUEST_ID> listPrequisiteQuest;
	vector<_uint> vectorAcceptConversation;
	vector<_uint> vectorNotSatisfiedConversation;
	list<QUEST_CONDITION*> listCompleteCondition;
	vector<_uint> vectorCompleteConversation;
	REWARD tReward;
}QUEST_DATA;

END