#include "stdafx.h"
#include "Skill.h"
#include "Character.h"
#include "Animation.h"


CSkill::CSkill()
{
}

HRESULT CSkill::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	m_pSkillDesc = pSkillData;

	m_pUser = pUser;
	m_fCoolTimeAcc = pSkillData->fCoolTime;


	return S_OK;
}

void CSkill::Update(_float fDeltaTime)
{
	Update_CoolTime(fDeltaTime);
}

void CSkill::Update_CoolTime(_float fDeltaTime)
{
	m_fCoolTimeAcc += fDeltaTime;
}

void CSkill::Update_CastingTime(_float fDeltaTime)
{
	m_fCastingTimeAcc += fDeltaTime;
}

void CSkill::Use()
{
	if (false == Is_CoolReady())
		return;
	if(m_pUser->Use_Skill(this))
	{
		m_fCoolTimeAcc = 0.f;
		m_fCastingTimeAcc = 0.f;
	}

}

_int CSkill::Get_NextAnimation(_uint iAnimIdx)
{
	vector <_uint> ::iterator iter = m_pSkillDesc->vecAnimation.begin();
	for (; iter != m_pSkillDesc->vecAnimation.end(); ++iter)
	{
		if ((*iter) == iAnimIdx)
		{
			++iter;
			if (iter == m_pSkillDesc->vecAnimation.end())
				return -1;
			return *iter;
		}
	}
	return -1;
}

vector<_uint>& CSkill::Get_AnimIdcies()
{
	return m_pSkillDesc->vecAnimation;
}


_bool CSkill::Is_CoolReady()
{
	return m_fCoolTimeAcc >= m_pSkillDesc->fCoolTime;
}

_bool CSkill::Is_CastingComplete()
{
	if (m_pSkillDesc->eCastingType != SKILL_TYPE::CASTING)
		return true;
	if (m_pUser->Get_CurrentAnimIdx() != m_pSkillDesc->vecAnimation[0])
		return true;
	return m_pUser->Get_AnimationProgress(m_pSkillDesc->vecAnimation[0]) >= 1.f;
}


CSkill* CSkill::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CSkill* pInstance = new CSkill;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;

}

void CSkill::Free()
{
	__super::Free();
}

const _char* CSkill::Get_IconTag()
{
	return m_pSkillDesc->strIconImageTag;
}

_float CSkill::Get_CoolTimeRatio()
{
	return m_fCoolTimeAcc / m_pSkillDesc->fCoolTime;
}
