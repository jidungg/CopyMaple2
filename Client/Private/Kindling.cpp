#include "stdafx.h"
#include "Kindling.h"
#include "Collider_Frustum.h"
#include "GameInstance.h"
#include "Character.h"
#include "Player.h"
#include "Bullet_Kindling.h"
#include "HitEvent.h"
#include "Client_Utility.h"

CKindling::CKindling()
	:CSkill()
{
}

HRESULT CKindling::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;
	m_bNeedWeapon = true;
	//TargetSearcher
	CCollider_Frustum::FRUSTUM_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fDistance = 9.f;
	tTargetSearcherDesc.fWidth = 3.f;
	tTargetSearcherDesc.fHeight = 1.5f;
	tTargetSearcherDesc.fEndWidth = 6.0f;
	m_pTargetSearcher = static_cast<CCollider_Frustum*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Frustum::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);

	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_KINDLING;
	m_pBullet = static_cast<CBullet_Kindling*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_Kindling::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_kindling_cast_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);

	//CastEndEffect
	 tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_kindling_cast_02_a.effmodel");
	m_pCastEndEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEndEffect->Set_Active(false);
	return S_OK;
}

void CKindling::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Update(fTimeDelta);
	}
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}	
	if (m_pCastEndEffect->Is_Active())
	{
		m_pCastEndEffect->Update(fTimeDelta);
	}
}

void CKindling::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
	if (m_pCastEndEffect->Is_Active())
	{
		m_pCastEndEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEndEffect);
	}
}

void CKindling::On_SkillUsed()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
}

void CKindling::On_CastingEnd()
{

}

void CKindling::Fire()
{
	m_pCastEndEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEndEffect->Start_Animation();
	m_pCastEndEffect->Set_Active(true);

	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	CCharacter* pTarget =  SearchTarget(LAYERID::LAYER_MONSTER);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;
	m_pBullet->Launch(fDmg, pTarget);
}


CKindling* CKindling::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CKindling* pInstance = new CKindling;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CKindling Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKindling::Free()
{
	__super::Free();
	Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect);
	Safe_Release(m_pCastEndEffect);
}
