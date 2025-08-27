#include "stdafx.h"
#include "BayarAttackB.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bullet_BayarAttackB.h"
#include "Sound.h"

CBayarAttackB::CBayarAttackB()
	: CSkill()
{
}

HRESULT CBayarAttackB::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//Bullet
	CBullet_BayarAttackB::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_NPC;
	m_pBullet = static_cast<CBullet_BayarAttackB*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_BayarAttackB::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);
	m_pUser->Add_Child(m_pBullet);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_b.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
	//m_pUser->Add_Child(m_pCastEffect);
	
	return S_OK;
}

void CBayarAttackB::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pUser->Move_Forward(m_fMoveSpeed*fTimeDelta);
		m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CBayarAttackB::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
}

void CBayarAttackB::On_SkillUsed()
{
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_01_B.wav"));
	pSouind->SetVolume(100);
}

void CBayarAttackB::On_CastingEnd()
{
}

void CBayarAttackB::Fire()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();

	m_pBullet->Set_Active(true);
	m_pBullet->Launch(this);

	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_NormalAttack_0%d.wav"), 1, 6);
	pSouind->SetVolume(100);
}

void CBayarAttackB::On_AttackEnd()
{
	m_pBullet->Set_Active(false);
	m_pCastEffect->Set_Active(false);
}

CBayarAttackB* CBayarAttackB::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarAttackB* pInstance = new CBayarAttackB;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarAttackB Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarAttackB::Free()
{
	__super::Free();
	//Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect);
}
