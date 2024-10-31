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

	m_pUser = pUser;

	return S_OK;
}

void CSkill::Update(_float fDeltaTime)
{
	m_fCoolTimeAcc += fDeltaTime;
	m_fCurrentCasting += fDeltaTime;
}

void CSkill::Use()
{
	m_fCoolTimeAcc = 0.f;
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

const _char* CSkill::Get_IconTag()
{
	return m_pSkillDesc->strIconImageTag;
}

_float CSkill::Get_CoolTimeRatio()
{
	return m_fCoolTimeAcc / m_pSkillDesc->fCoolTime;
}
