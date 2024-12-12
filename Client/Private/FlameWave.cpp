#include "stdafx.h"
#include "FlameWave.h"
#include "Collider_Frustum.h"
#include "GameInstance.h"
#include "Player.h"
#include "Bullet_MagicClaw.h"
#include "HitEvent.h"

CFlameWave::CFlameWave()
	:CSkill()
{
}


HRESULT CFlameWave::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{

	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;
	m_bNeedWeapon = true;
	//TargetSearcher
	CCollider_Frustum::FRUSTUM_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fDistance = 8.f;
	tTargetSearcherDesc.fWidth = 3.f;
	tTargetSearcherDesc.fHeight = 1.5f;
	tTargetSearcherDesc.fEndWidth = 6.0f;
	m_pTargetSearcher = static_cast<CCollider_Frustum*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Frustum::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_flamewave_cast_01.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
	return S_OK;
}

void CFlameWave::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CFlameWave::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
}

HRESULT CFlameWave::Render_Collider()
{
    return E_NOTIMPL;
}

void CFlameWave::On_Cast()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
}

void CFlameWave::On_CastingEnd()
{
}

void CFlameWave::Fire()
{	//SearchTarget :FustumCollider 켜기
	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYERID::LAYER_MONSTER);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	for (auto& pTarget : listTarget)
	{
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
		_bool bCrit = m_pUser->Judge_Critical();
		fDmg *= bCrit ? 1.5 : 1.f;
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pUser, pTarget, (_int)fDmg, bCrit, true,EFF_MODEL_ID::HIT_FIREBALL_1));
	}

}


CFlameWave* CFlameWave::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CFlameWave* pInstance = new CFlameWave;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CFlameWave Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFlameWave::Free()
{
	__super::Free();
	Safe_Release(m_pCastEffect);
}
