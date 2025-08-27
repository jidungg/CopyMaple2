#include "stdafx.h"
#include "BayarAttackD.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bullet_BayarAttackD.h"
#include "Client_Utility.h"
#include "CubeTerrain.h"
#include "Sound.h"

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
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_NPC;
	m_pBullet = static_cast<CBullet_BayarAttackD*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_BayarAttackD::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);
	m_pUser->Add_Child(m_pBullet);

	//AttackEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_c_b.effmodel");
	m_pAttackEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pAttackEffect->Set_Active(false);
	//m_pUser->Add_Child(m_pAttackEffect);

	//ChargeEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_01_c_a.effmodel");
	tCastEffDesc.bAttachToBone = true;
	tCastEffDesc.pAttachObject = m_pUser;
	tCastEffDesc.szBoneName = "Bip01 R Hand";
	m_pChargeEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pChargeEffect->Set_Active(false);
	//m_pUser->Add_Child(m_pChargeEffect);


	return S_OK;
}

void CBayarAttackD::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pAttackEffect->Is_Active())
	{
		CTransform* pUserTRansform = m_pUser->Get_Transform();
		_vector vUserPos = m_pUser->Get_WorldPosition();
		_vector vDir = XMVector3Normalize(pUserTRansform->Get_State(CTransform::STATE_LOOK))* m_fMoveSpeed* fTimeDelta;
		_float fNextFloor = m_pTerrain->Get_FloorHeight(vDir + vUserPos);
		if(fNextFloor == m_fFloorHeight)
			m_pUser->Move_Forward(m_fMoveSpeed * fTimeDelta);

		m_pAttackEffect->Set_Transform(m_pUser->Get_Transform());
		m_pAttackEffect->Update(fTimeDelta);
	}
	if (m_pChargeEffect->Is_Active())
	{
		//m_pChargeEffect->Set_Transform(m_pUser->Get_Transform());
		m_pChargeEffect->Update(fTimeDelta);
	}
}

void CBayarAttackD::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pAttackEffect->Is_Active())
	{
		m_pAttackEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pAttackEffect);
	}
	if (m_pChargeEffect->Is_Active())
	{
		m_pChargeEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pChargeEffect);
	}
}

void CBayarAttackD::On_SkillUsed()
{
	//m_pChargeEffect->Set_Transform(m_pUser->Get_Transform());
	m_pChargeEffect->Set_Active(true);
	m_pChargeEffect->Start_Animation(0,true);
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_01_D.wav"));
	pSouind->SetVolume(100);
	pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_BAYARPEAK, TEXT("Boss_SandstoneGiant_Skill_Attack_01_C.wav"));
	pSouind->SetVolume(100);
}

void CBayarAttackD::On_CastingEnd()
{

}

void CBayarAttackD::Fire()
{
	m_pTerrain = static_cast<CCubeTerrain*> (m_pGameInstance->Get_FirstGameObject(Get_CurrentTrueLevel(), LAYERID::LAYER_TERRAIN));
	m_fFloorHeight = m_pTerrain->Get_FloorHeight(m_pUser->Get_WorldPosition());

	m_pAttackEffect->Set_Transform(m_pUser->Get_Transform());
	m_pAttackEffect->Set_Active(true);
	m_pAttackEffect->Start_Animation();
	m_pBullet->Set_Active(true);

	m_pBullet->Launch(this);

	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay_Random(LEVEL_BAYARPEAK, TEXT("en_Bajar_Voice_NormalAttack_0%d.wav"), 1, 6);
	pSouind->SetVolume(100);

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
	//Safe_Release(m_pBullet);
	Safe_Release(m_pAttackEffect);
	Safe_Release(m_pChargeEffect);
}
