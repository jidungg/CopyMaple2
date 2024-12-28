#pragma once
#include "Base.h"
#include "Quest.h"
#include "Client_Defines.h"

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
	_bool Is_QuestSatisfacted(QUEST_ID eId);
private:
	map<QUEST_ID, QUEST_DATA*> m_mapData;
	list< QUEST_ID> m_listCompleted;
public:
	void Free(); 
};
#define QUESTDB CQuestDataBase::GetInstance()
END