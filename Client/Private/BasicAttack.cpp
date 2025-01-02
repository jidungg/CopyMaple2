#include "stdafx.h"
#include "BasicAttack.h"
#include "GameInstance.h"
#include "Sound.h"
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
	CSound* pSouind =m_pGameInstance->Start_EffectPlay_Random(LEVEL_LOADING, L"en_Normal_Female_AttackDown_0%d.wav", 1,4);
	pSouind->SetVolume(100);
}

void CBasicAttack::On_SkillUsed()
{
}

void CBasicAttack::On_CastingEnd()
{
}
