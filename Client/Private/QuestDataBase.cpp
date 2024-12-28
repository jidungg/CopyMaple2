#include "stdafx.h"
#include "QuestDataBase.h"

IMPLEMENT_SINGLETON(CQuestDataBase)

CQuestDataBase::CQuestDataBase()
{
}

HRESULT CQuestDataBase::Load_Data()
{
	json j;
	if (FAILED(CJsonParser::ReadJsonFile("../Bin/Resources/Json/QuestData.json", &j)))
		return E_FAIL;
	for (auto& data : j["Quest"])
	{
		Insert_Data(new QUEST_DATA(data));
	}
	return S_OK;
}

void CQuestDataBase::Insert_Data(QUEST_DATA* pData)
{
	m_mapData[pData->eQuestID] = pData;
}

_bool CQuestDataBase::Is_QuestCompleted(QUEST_ID eId)
{
	for (auto& eCompletedId : m_listCompleted)
	{
		if (eCompletedId == eId)
			return true;
	}
	return false;
}

_bool CQuestDataBase::Is_QuestSatisfacted(QUEST_ID eId)
{
	if (Is_QuestCompleted(eId))
		return false;

	return false;
}

void CQuestDataBase::Free()
{
	for (auto& item : m_mapData)
	{
		Safe_Delete(item.second);
	}
	m_mapData.clear();
}
