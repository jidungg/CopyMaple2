#include "stdafx.h"
#include "BasicAttack.h"

CBasicAttack::CBasicAttack()
{
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


void CBasicAttack::Fire()
{
}

void CBasicAttack::On_SkillUsed()
{
}

void CBasicAttack::On_CastingEnd()
{
}
