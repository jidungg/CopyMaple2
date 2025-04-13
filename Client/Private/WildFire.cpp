#include "stdafx.h"
#include "WildFire.h"
#include "Bullet_WildFire.h"
#include "GameInstance.h"
#include "Character.h"
#include "Player.h"
#include "HitEvent.h"
#include "Sound.h"

CWildFire::CWildFire()
	: CSkill()
{
}

HRESULT CWildFire::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	m_bNeedWeapon = true;
	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_FLAME_BURST_A;
	m_pBullet = static_cast<CBullet_WildFire*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_WildFire::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_wildfire_cast_01.effmodel");
	m_pCastEffect1 = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect1->Set_Active(false);
	m_pUser->Add_Child(m_pCastEffect1);
	m_pCastEffect1->Get_Transform()->Set_State(CTransform::STATE_POSITION, { 0.f,0.01f,0.f ,1.f});

	return S_OK;
}

void CWildFire::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_pBullet->Is_Active())
	{
		m_pBullet->Update(fTimeDelta);
	}


}

void CWildFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//if (m_pBullet->Is_Active())
	//{
	//	m_pBullet->Late_Update(fTimeDelta);
	//	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect1);
	//}


}

void CWildFire::On_SkillUsed()
{
	m_pCastEffect1->Start_Animation();
	m_pCastEffect1->Set_Active(true);
	CSound* pSouind = CGameInstance::GetInstance()->Start_EffectPlay(LEVEL_LOADING, TEXT("Skill_Wizard_WildFire_Cast_01.wav"));
	pSouind->SetVolume(100);
}

void CWildFire::On_CastingEnd()
{

}

void CWildFire::Fire()
{
	m_pUser->Move_Forward(2.5);

	m_pBullet->Launch(this, m_pUser->Get_TransformPosition());
	m_pBullet->Set_Transform(m_pUser->Get_Transform());
}


CWildFire* CWildFire::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CWildFire* pInstance = new CWildFire;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CWildFire Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWildFire::Free()
{
	__super::Free();
	Safe_Release(m_pBullet);

}
