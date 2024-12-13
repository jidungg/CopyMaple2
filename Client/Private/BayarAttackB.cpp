#include "stdafx.h"
#include "BayarAttackB.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bullet_BayarAttackB.h"

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
	m_pUser->Add_Child(m_pCastEffect);
	
	return S_OK;
}

void CBayarAttackB::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pUser->Move_Forward(m_fMoveSpeed*fTimeDelta);
	}
}

void CBayarAttackB::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CBayarAttackB::On_SkillUsed()
{
}

void CBayarAttackB::On_CastingEnd()
{
}

void CBayarAttackB::Fire()
{
	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();
	//m_pCastEffect->Set_Transform(m_pUser->Get_Transform());

	m_pBullet->Set_Active(true);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
	m_pBullet->Launch(fDmg);
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
}
