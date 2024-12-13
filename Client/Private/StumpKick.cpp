#include "stdafx.h"
#include "StumpKick.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"
#include "EffModelObject.h"

CStumpKick::CStumpKick()
    : CSkill()
{
}

HRESULT CStumpKick::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
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
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_stump_kickattack_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);


	return S_OK;
}

void CStumpKick::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CStumpKick::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
}

void CStumpKick::On_SkillUsed()
{
}

void CStumpKick::On_CastingEnd()
{
}

void CStumpKick::Fire()
{
	m_pCastEffect->Set_Active(true);
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());

	m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);

	for (auto& pTarget : listTarget)
	{
		_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
		_bool bCrit = m_pUser->Judge_Critical();
		fDmg *= bCrit ? 1.5 : 1.f;
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit, false, EFF_MODEL_ID::HIT_A));
	}
}

CStumpKick* CStumpKick::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CStumpKick* pInstance = new CStumpKick;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CStumpKick Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStumpKick::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect);

}
