#include "stdafx.h"
#include "BayarPalmStrike.h"
#include "Collider_Frustum.h"
#include "GameInstance.h"
#include "Player.h"
#include "Bullet_MagicClaw.h"
#include "HitEvent.h"

CBayarPalmStrike::CBayarPalmStrike()
	: CSkill()
{
}


HRESULT CBayarPalmStrike::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;
	//TargetSearcher
	CCollider_Frustum::FRUSTUM_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fDistance = 8.f;
	tTargetSearcherDesc.fWidth = 3.f;
	tTargetSearcherDesc.fHeight = 1.5f;
	tTargetSearcherDesc.fEndWidth = 6.0f;
	m_pTargetSearcher = static_cast<CCollider_Frustum*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Frustum::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);

	//WindEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_h_b.effmodel");
	m_pWindEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pWindEffect->Set_Active(false);
	//m_pWindEffect->Get_Transform()->Scaling(0.5f, 0.5f, 0.5f);
	m_pWindEffect->Get_Transform()->Set_State(CTransform::STATE_POSITION, _vector{ 0.f, 0.1f, 0.f,1.f });

	//GroundEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_sandstonebiggiant_attack_02_h_a.effmodel");
	m_pGroundEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pGroundEffect->Set_Active(false);
	//m_pGroundEffect->Get_Transform()->Scaling(0.5f, 0.5f, 0.5f);

	return S_OK;
}

void CBayarPalmStrike::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if(m_pWindEffect->Is_Active())
		m_pWindEffect->Update(fTimeDelta);
	if (m_pGroundEffect->Is_Active())
		m_pGroundEffect->Update(fTimeDelta);
}

void CBayarPalmStrike::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pWindEffect->Is_Active())
	{
		m_pWindEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pWindEffect);
	}
	if (m_pGroundEffect->Is_Active())
	{
		m_pGroundEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pGroundEffect);
	}
}

void CBayarPalmStrike::On_Cast()
{
	m_iAttackCount = 0;
}

void CBayarPalmStrike::On_CastingEnd()
{
}

void CBayarPalmStrike::Fire()
{
	_matrix matUserWorld = m_pUser->Get_Transform()->Get_WorldMatrix();
	m_pTargetSearcher->Update(matUserWorld);

	matUserWorld.r[3].m128_f32[1] += 0.1f;
	m_pWindEffect->Start_Animation();
	m_pWindEffect->Get_Transform()->Set_WorldMatrix(matUserWorld);
	m_pWindEffect->Get_Transform() ->Scaling(1, 1, 1);

	m_pGroundEffect->Start_Animation();
	_vector vGroundEffectOffset = (m_iAttackCount % 2 == 0 ? m_vRGroundEffectOffset : m_vLGroundEffectOffset);
	m_iAttackCount++;
	m_pGroundEffect->Get_Transform()->Set_WorldMatrix(XMMatrixTranslationFromVector(vGroundEffectOffset) * matUserWorld);
	m_pGroundEffect->Get_Transform()->Scaling(1, 1, 1);


	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_PLAYER);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	for (auto& pTarget : listTarget)
	{
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
		_bool bCrit = m_pUser->Judge_Critical();
		fDmg *= bCrit ? 1.5 : 1.f;
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit,false, EFF_MODEL_ID::HIT_A));
	}
}

void CBayarPalmStrike::On_AttackEnd()
{
}

CBayarPalmStrike* CBayarPalmStrike::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBayarPalmStrike* pInstance = new CBayarPalmStrike;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBayarPalmStrike Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayarPalmStrike::Free()
{
	__super::Free();
	Safe_Release(m_pWindEffect);
	Safe_Release(m_pGroundEffect);
}
