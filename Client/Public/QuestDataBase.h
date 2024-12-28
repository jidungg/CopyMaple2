#pragma once
#include "Base.h"
#include "Quest.h"
#include "Client_Defines.h"
#include "set"
BEGIN(Client)
class CQuestDataBase :
    public CBase
{
    DECLARE_SINGLETON(CQuestDataBase)
private:
	CQuestDataBase();
	virtual ~CQuestDataBase() = default;

public:
	HRESULT Load_Data();
	void Insert_Data(QUEST_DATA* pData);
	QUEST_DATA* Get_Data(QUEST_ID eID) { return m_mapData[eID]; }

	_bool Is_QuestCompleted(QUEST_ID eId);
	_bool Is_QuestSatisfiedCompleteCondition(QUEST_ID eId);
	_bool Is_QuestAccepted(QUEST_ID eId);
	_bool Is_SatisfiedAcceptCondition(QUEST_ID eId);

	void Accept_Quest(QUEST_ID eId);
	void Complete_Quest(QUEST_ID eId);
	void Abandon_Quest(QUEST_ID eId);

	void Increase_MonsterKillCount(MONSTER_ID eId);
private:
	map<QUEST_ID, QUEST_DATA*> m_mapData;
	set< QUEST_ID> m_setAccepted;
	set< QUEST_ID> m_setCompleted;
public:
	void Free(); 
};
#define QUESTDB CQuestDataBase::GetInstance()
END