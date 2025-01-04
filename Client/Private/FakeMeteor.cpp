#include "stdafx.h"
#include "FakeMeteor.h"
#include "Bullet_FakeMeteor.h"
#include "Collider_Frustum.h"
#include "GameInstance.h"
#include "Character.h"
#include "Player.h"
#include "HitEvent.h"
#include "Client_Utility.h"
#include "Sound.h"

CFakeMeteor::CFakeMeteor()
	: CSkill()
{
}


HRESULT CFakeMeteor::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
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
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_KINDLING;
	m_pBullet = static_cast<CBullet_FakeMeteor*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_FakeMeteor::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);
	m_pGameInstance->Add_GameObject_ToLayer((_uint)Get_CurrentTrueLevel(), (_uint)LAYERID::LAYER_BULLET, m_pBullet, true);
	Safe_AddRef(m_pBullet);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_fakemeteor_cast_01.effmodel");
	m_pCastEffect1 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect1->Set_Active(false);

	//CastENdEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_fakemeteor_cast_02.effmodel");
	m_pCastEffect2 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect2->Set_Active(false);
	return S_OK;
}

void CFakeMeteor::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pCastEffect1->Is_Active())
	{
		m_pCastEffect1->Update(fTimeDelta);
	}
	if (m_pCastEffect2->Is_Active())
	{
		m_pCastEffect2->Update(fTimeDelta);
	}
}

void CFakeMeteor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

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

void CFakeMeteor::On_SkillUsed()
{
	m_pCastEffect1->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect1->Start_Animation();
	m_pCastEffect1->Set_Active(true);
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_LOADING, TEXT("Skill_Wizard_FakeMeteor_Cast_01.wav"));
	pSouind->SetVolume(100);
}

void CFakeMeteor::On_CastingEnd()
{

}

void CFakeMeteor::Fire()
{
	m_pCastEffect2->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect2->Start_Animation();
	m_pCastEffect2->Set_Active(true);

	m_pTargetSearcher->Update(XMMatrixTranslation(0, m_pUser->Get_BodyCollisionOffset().y, -1) * m_pUser->Get_Transform()->Get_WorldMatrix());
	CCharacter* pTarget = SearchTarget(LAYERID::LAYER_MONSTER);


	if (pTarget)
	{
		m_pBullet->Launch(this, pTarget);
	}
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_LOADING, TEXT("Skill_Wizard_FakeMeteor_Cast_02.wav"));
	pSouind->SetVolume(100);
}

CFakeMeteor* CFakeMeteor::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CFakeMeteor* pInstance = new CFakeMeteor;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CFakeMeteor Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFakeMeteor::Free()
{
	__super::Free();
	Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect1);
	Safe_Release(m_pCastEffect2);
}
