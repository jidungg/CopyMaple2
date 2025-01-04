#include "stdafx.h"
#include "BayarAttackC.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bullet_BayarAttackB.h"
#include "Sound.h"
CBayarAttackC::CBayarAttackC()
{
}

HRESULT CBayarAttackC::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//ChargeEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_01_c_a.effmodel");
	tCastEffDesc.bAttachToBone = true;
	tCastEffDesc.pAttachObject = m_pUser;
	tCastEffDesc.szBoneName = "Bip01 R Hand";
	m_pChargeEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pChargeEffect->Set_Active(false);
	m_pUser->Add_Child(m_pChargeEffect);

	Set_MoveSpeed(1.7f);
    return S_OK;
}

void CBayarAttackC::On_SkillUsed()
{
	__super::On_SkillUsed();
	m_pChargeEffect->Set_Active(true);
	m_pChargeEffect->Start_Animation(0, true);

}

void CBayarAttackC::On_CastingEnd()
{
	__super::On_CastingEnd();
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_01_C.wav"));
	pSouind->SetVolume(100);
}

void CBayarAttackC::Fire()
{
	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();
	//m_pCastEffect->Set_Transform(m_pUser->Get_Transform());

	m_pBullet->Set_Active(true);

	m_pBullet->Launch(this);

	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_NormalAttack_0%d.wav"), 1, 6);
	pSouind->SetVolume(100);

}

void CBayarAttackC::On_AttackEnd()
{
	__super::On_AttackEnd();
	m_pChargeEffect->Set_Active(false);
}

CBayarAttackC* CBayarAttackC::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarAttackC* pInstance = new CBayarAttackC;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarAttackC Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
