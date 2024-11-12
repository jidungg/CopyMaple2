#include "stdafx.h"
#include "Teleport.h"
#include "Character.h"

CTeleport::CTeleport()
	:CSkill()
{
}


void CTeleport::Initialzie_AnimEvent()
{
	for (auto& eventTime : m_pSkillDesc->mapAnimEventTime)
	{
		_uint iAnimIdx = m_pSkillDesc->vecAnimation[ eventTime.first];
		ANIM_EVENT tAnimEvent;
		tAnimEvent.fTime = eventTime.second;
		tAnimEvent.pFunc = std::bind(&CTeleport::AnimEventFunc1, this);
		m_mapAnimEvent.insert({ iAnimIdx, tAnimEvent });
	}
}
void CTeleport::AnimEventFunc1()
{
	m_pUser->Move_Forward(m_pSkillDesc->vecData[m_pSkillDesc->iLevel-1]);
}

CTeleport* CTeleport::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CTeleport* pInstance = new CTeleport;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CTeleport Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

