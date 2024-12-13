#include "stdafx.h"
#include "MagicClaw.h"
#include "Collider_Frustum.h"
#include "Collider_AABB.h"
#include "GameInstance.h"
#include "Character.h"
#include "Collider.h"
#include "MeshCollider.h"
#include "Player.h"
#include "Bullet.h"
#include "Bullet_MagicClaw.h"
#include "EffectManager.h"

CMagicClaw::CMagicClaw()
    :CSkill()
{

}

HRESULT CMagicClaw::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
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

	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	m_pBullet = static_cast<CBullet_MagicClaw*>( m_pGameInstance->Clone_Proto_Object_Stock(CBullet_MagicClaw::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_magicclaw_cast_01_a.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
	return S_OK;
}
 
void CMagicClaw::Update(_float fTimeDelta)
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
}
void CMagicClaw::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pBullet);
	}
	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
}


void CMagicClaw::On_SkillUsed()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
}

void CMagicClaw::On_CastingEnd()
{
}


void CMagicClaw::Fire()
{
	//SearchTarget :FustumCollider 켜기
	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	CCharacter* pTarget = SearchTarget(LAYERID::LAYER_MONSTER);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	if (pTarget)
	{
		_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
		fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

		m_pBullet->Launch(fDmg, pTarget);

	}
}


CMagicClaw* CMagicClaw::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CMagicClaw* pInstance = new CMagicClaw;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CMagicClaw Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMagicClaw::Free()
{
	__super::Free();
	Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect);
}

