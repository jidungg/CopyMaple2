#include "stdafx.h"
#include "CrabAttack.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"

CCrabAttack::CCrabAttack()
	: CSkill()
{
}

HRESULT CCrabAttack::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fRadius = 1.5f;
	tTargetSearcherDesc.vCenter = { 0.f, 0.f, 0.f };
	m_pTargetSearcher = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);


	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_crapstone_attack_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);


	return S_OK;
}

void CCrabAttack::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CCrabAttack::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}

}

void CCrabAttack::On_Cast()
{
}

void CCrabAttack::On_CastingEnd()
{
}

void CCrabAttack::Fire()
{

	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Get_Transform()->Scaling( 2.f, 2.f, 2.f );

	m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	
	for (auto& pTarget : listTarget)
	{
		_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

		m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, "eff_npc_attack_hit_01_a.effmodel"));
	}
}


CCrabAttack* CCrabAttack::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CCrabAttack* pInstance = new CCrabAttack;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CCrabAttack Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrabAttack::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect);
}
