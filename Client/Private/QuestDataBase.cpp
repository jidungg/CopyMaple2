#include "stdafx.h"
#include "QuestDataBase.h"
#include "PlayerInfo.h"

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

	return m_setCompleted.find(eId) != m_setCompleted.end();
}

_bool CQuestDataBase::Is_QuestSatisfiedCompleteCondition(QUEST_ID eId)
{
	if (false == Is_QuestAccepted(eId))
		return false;
	
	return m_mapData[eId]->Is_SatisfiedCompleteCondition();
}

_bool CQuestDataBase::Is_QuestAccepted(QUEST_ID eId)
{
	return m_setAccepted.find(eId) != m_setAccepted.end();
}

_bool CQuestDataBase::Is_SatisfiedAcceptCondition(QUEST_ID eId)
{
	return m_mapData[eId]->Is_SatisfiedAcceptCondition();
}

void CQuestDataBase::Accept_Quest(QUEST_ID eId)
{
	if (Is_QuestCompleted(eId))
		return;
	if (Is_QuestAccepted(eId))
		return;
	if (false == Is_SatisfiedAcceptCondition(eId))
		return;
	m_mapData[eId]->eState = QUEST_STATE::ACCEPTED;
	m_setAccepted.insert(eId);
}

void CQuestDataBase::Complete_Quest(QUEST_ID eId)
{
	if (false == Is_QuestAccepted(eId))
		return;
	if (false == Is_QuestSatisfiedCompleteCondition(eId))
		return;
	m_mapData[eId]->eState = QUEST_STATE::COMPLETED;
	m_setAccepted.erase(eId);
	m_setCompleted.insert(eId);
	
	for (auto& pairItemReward : m_mapData[eId]->tReward.m_eItemList)
	{
		PLAYERINIFO->Gain_Item(pairItemReward.first.eItemType, pairItemReward.first.iID, pairItemReward.second);
	}
	PLAYERINIFO->Gain_EXP(m_mapData[eId]->tReward.m_iExp);
	PLAYERINIFO->Gain_Gold(m_mapData[eId]->tReward.m_iGold);
}

void CQuestDataBase::Abandon_Quest(QUEST_ID eId)
{
	if (false == Is_QuestAccepted(eId))
		return;
	m_mapData[eId]->eState = QUEST_STATE::NOT_ACCEPTED;
	m_setAccepted.erase(eId);
}

void CQuestDataBase::Increase_MonsterKillCount(MONSTER_ID eId)
{
	for (auto& eQId : m_setAccepted)
	{
		m_mapData[eQId]->Increase_MonsterKillCount(eId);
	}
}

void CQuestDataBase::Free()
{
	for (auto& item : m_mapData)
	{
		Safe_Delete(item.second);
	}
	m_mapData.clear();
}
