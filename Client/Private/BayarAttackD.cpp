#include "stdafx.h"
#include "BayarAttackD.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bullet_BayarAttackD.h"

CBayarAttackD::CBayarAttackD()
	: CSkill()
{
}


HRESULT CBayarAttackD::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.szHitEffectTag = "eff_sandstonebiggiant_attack_02_b_hit.effmodel";
	m_pBullet = static_cast<CBullet_BayarAttackD*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_BayarAttackD::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);
	m_pUser->Add_Child(m_pBullet);

	//AttackEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_c_b.effmodel");
	m_pAttackEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pAttackEffect->Set_Active(false);
	m_pUser->Add_Child(m_pAttackEffect);

	//ChargeEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_01_c_a.effmodel");
	tCastEffDesc.bAttachToBone = true;
	tCastEffDesc.pAttachObject = m_pUser;
	tCastEffDesc.szBoneName = "Bip01 R Hand";
	m_pChargeEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pChargeEffect->Set_Active(false);
	m_pUser->Add_Child(m_pChargeEffect);

	return S_OK;
}

void CBayarAttackD::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pAttackEffect->Is_Active())
	{
		m_pUser->Move_Forward(m_fMoveSpeed * fTimeDelta);
	}
}

void CBayarAttackD::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CBayarAttackD::On_Cast()
{
	m_pChargeEffect->Set_Active(true);
	m_pChargeEffect->Start_Animation(0,true);
	//m_pChargeEffect->Set_Transform(m_pUser->Get_Transform());

}

void CBayarAttackD::On_CastingEnd()
{

}

void CBayarAttackD::Fire()
{

	m_pAttackEffect->Set_Active(true);
	m_pAttackEffect->Start_Animation();
	//m_pAttackEffect->Set_Transform(m_pUser->Get_Transform());

	m_pBullet->Set_Active(true);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
	m_pBullet->Launch(fDmg);
}

void CBayarAttackD::On_AttackEnd()
{
	m_pBullet->Set_Active(false);
	m_pAttackEffect->Set_Active(false);
	m_pChargeEffect->Set_Active(false);
}

CBayarAttackD* CBayarAttackD::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarAttackD* pInstance = new CBayarAttackD;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarAttackD Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarAttackD::Free()
{
	__super::Free();
}
