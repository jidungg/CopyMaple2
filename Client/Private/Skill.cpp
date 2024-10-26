#include "stdafx.h"
#include "Skill.h"
#include "Character.h"

SkillDesc::SkillDesc(json& jSkillData)
{

}

CSkill::CSkill()
{
}

HRESULT CSkill::Initialize(SKILL_DESC* pSkillData, CCharacter* pUser)
{
	m_pSkillDesc = pSkillData;
	m_szIconTag = pSkillData->strIconImageTag;
	m_fCoolTimeLeft = 0.f;
	m_fCoolTimeLeftRatio = 0.f;
	m_fCoolTime = pSkillData->fCoolTime;
	m_pUser = pUser;

	return S_OK;
}

void CSkill::Update(_float fDeltaTime)
{
	m_fCoolTimeLeft -= fDeltaTime;
	m_fCoolTimeLeftRatio = m_fCoolTimeLeft / m_pSkillDesc->fCoolTime;
	m_fCurrentCasting += fDeltaTime;
}

void CSkill::Use()
{
	m_fCoolTimeLeft = m_pSkillDesc->fCoolTime;
	m_fCurrentCasting = 0.f;
	m_pUser->Use_Skill(this);
}


CSkill* CSkill::Create(SKILL_DESC* pSkillData, CCharacter* pUser)
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
