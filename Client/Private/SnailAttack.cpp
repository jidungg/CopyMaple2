#include "stdafx.h"
#include "SnailAttack.h"
#include "Bullet_WildFire.h"
#include "GameInstance.h"
#include "Player.h"
#include "HitEvent.h"
#include "Collider_Sphere.h"

CSnailAttack::CSnailAttack()
	: CSkill()
{
}


HRESULT CSnailAttack::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
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
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_snail_attack_01_a_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);


	return S_OK;
}

void CSnailAttack::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CSnailAttack::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);

		m_pTargetSearcher->Update(m_pUser->Get_Transform()->Get_WorldMatrix());
		list<CGameObject*> listTarget;
		SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
		for (auto& pTarget : listTarget)
		{
			if (m_setHitObject.find(pTarget) != m_setHitObject.end())
				continue;
			m_setHitObject.insert(pTarget);
			_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
			_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
			fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

			m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, "hit"));
		}
	}

}


void CSnailAttack::On_Cast()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
}

void CSnailAttack::On_CastingEnd()
{
}

void CSnailAttack::Fire()
{
}


CSnailAttack* CSnailAttack::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CSnailAttack* pInstance = new CSnailAttack;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CSnailAttack Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSnailAttack::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect);
}
