#include "stdafx.h"
#include "BBQParty.h"
#include "Collider_Frustum.h"
#include "GameInstance.h"
#include "Character.h"
#include "Player.h"
#include "HitEvent.h"
#include "Bullet_BBQParty.h"

CBBQParty::CBBQParty()
	: CSkill()
{
}


HRESULT CBBQParty::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;
	m_bNeedWeapon = true;
	//TargetSearcher
	CCollider_Frustum::FRUSTUM_COLLIDER_DESC tTargetSearcherDesc;
	tTargetSearcherDesc.fDistance = 6.f;
	tTargetSearcherDesc.fWidth = 3.f;
	tTargetSearcherDesc.fHeight = 1.5f;
	tTargetSearcherDesc.fEndWidth = 6.0f;
	m_pTargetSearcher = static_cast<CCollider_Frustum*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Frustum::m_szProtoTag, &tTargetSearcherDesc));
	m_pTargetSearcher->Set_Active(false);


	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_A;
	m_pBullet = static_cast<CBullet_BBQParty*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_BBQParty::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_bbqparty_cast_01_a.effmodel");
	m_pCastEffect1 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect1->Set_Active(false);

	//CastENdEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_bbqparty_cast_02_a.effmodel");
	m_pCastEffect2 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect2->Set_Active(false);
	return S_OK;
}

void CBBQParty::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Update(fTimeDelta);
	}
	if (m_pCastEffect1->Is_Active())
	{
		m_pCastEffect1->Update(fTimeDelta);
	}
	if (m_pCastEffect2->Is_Active())
	{
		m_pCastEffect2->Update(fTimeDelta);
	}
}

void CBBQParty::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect1);
	}
	if (m_pCastEffect1->Is_Active())
	{
		m_pCastEffect1->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect1);
	}
	if (m_pCastEffect2->Is_Active())
	{
		m_pCastEffect2->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect2);
	}
}

void CBBQParty::On_Cast()
{
	m_pCastEffect1->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect1->Start_Animation();
	m_pCastEffect1->Set_Active(true);
}

void CBBQParty::On_CastingEnd()
{
}

void CBBQParty::Fire()
{
	m_pCastEffect2->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect2->Start_Animation();
	m_pCastEffect2->Set_Active(true);

	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	CCharacter* pTarget = SearchTarget(LAYER_MONSTER);
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;
	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

	if (pTarget)
	{
		m_pBullet->Launch(fDmg, pTarget);
	}
	else
	{
		CTransform* pShooterTransform = m_pUser->Get_Transform();
		_vector vPos = pShooterTransform->Get_State(CTransform::STATE_POSITION);
		_vector vLook = pShooterTransform->Get_State(CTransform::STATE_LOOK);
		vPos += vLook * 3.5f;

		m_pBullet->Launch(fDmg, vPos);
	}

}

CBBQParty* CBBQParty::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CBBQParty* pInstance = new CBBQParty;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CBBQParty Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBBQParty::Free()
{
	__super::Free();
	Safe_Release(m_pTargetSearcher);
	Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect1);
	Safe_Release(m_pCastEffect2);
}
