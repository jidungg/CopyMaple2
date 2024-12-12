#include "stdafx.h"
#include "BayarSkyJump.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"
#include "Bayar.h"

CBayarSkyJump::CBayarSkyJump()
	: CSkill()
{
}

HRESULT CBayarSkyJump::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	//TargetSearcher
	CCollider_Sphere::SPHERE_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fRadius = 20.f;
	tTargetSearcherDesc.vCenter = { 0.f, 0.f, 0.f };
	m_pTargetSearcher = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);


	//Jump Ready Effect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_jump_ready_a.effmodel");
	m_pJumpReadyEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pJumpReadyEffect->Set_Active(false);
	m_pUser->Add_Child(m_pJumpReadyEffect);

	//Gathering Effect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_01_d_b.effmodel");
	m_pGatheringEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pGatheringEffect->Set_Active(false);
	m_pUser->Add_Child(m_pGatheringEffect);

	//Jump Land Effect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_jump_land_a.effmodel");
	m_pLandEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pLandEffect->Set_Active(false);
	m_pUser->Add_Child(m_pLandEffect);

	return S_OK;
}

void CBayarSkyJump::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CBayarSkyJump::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

void CBayarSkyJump::On_Cast()
{
	m_pJumpReadyEffect->Set_Active(true);
	m_pJumpReadyEffect->Start_Animation();
	m_pGatheringEffect->Set_Active(true);
	m_pGatheringEffect->Start_Animation(0,true);
	m_pGatheringEffect->Set_AnimSpeed(3.15f);
}

void CBayarSkyJump::On_CastingEnd()
{
	m_pGatheringEffect->Set_Active(false);
}

void CBayarSkyJump::Fire()
{
	m_pLandEffect->Set_Active(true);
	m_pLandEffect->Start_Animation();

	m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	for (auto& pTarget : listTarget)
	{
		if(static_cast<CBayar*>( m_pUser)->Is_Attached(static_cast<CPlayer*>(pTarget)))
			continue;
		_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
		_bool bCrit = m_pUser->Judge_Critical();
		fDmg *= bCrit ? 1.5 : 1.f;
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit, false,EFF_MODEL_ID::HIT_A));
	}
}

void CBayarSkyJump::On_AttackEnd()
{
}

CBayarSkyJump* CBayarSkyJump::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarSkyJump* pInstance = new CBayarSkyJump;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarSkyJump Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarSkyJump::Free()
{
	__super::Free();
}
