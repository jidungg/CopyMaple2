#include "stdafx.h"
#include "BasicAttack.h"

CBasicAttack::CBasicAttack()
{
}

void CBasicAttack::AnimEventFunc1()
{
	cout << "BasicAttack" << endl;
}

void CBasicAttack::Initialzie_AnimEvent()
{
	for (auto& eventTime : m_pSkillDesc->mapAnimEventTime)
	{
		_uint iAnimIdx = m_pSkillDesc->vecAnimation[eventTime.first];
		ANIM_EVENT tAnimEvent;
		tAnimEvent.fTime = eventTime.second;
		tAnimEvent.pFunc = std::bind(&CBasicAttack::AnimEventFunc1, this);
		m_mapAnimEvent.insert({ iAnimIdx, tAnimEvent });
	}
}

CBasicAttack* CBasicAttack::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBasicAttack* pInstance = new CBasicAttack;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBasicAttack Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
