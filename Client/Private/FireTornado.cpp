#include "stdafx.h"
#include "FireTornado.h"
#include "Collider_Sphere.h"
#include "GameInstance.h"
#include "Player.h"
#include "Bullet_FireTornado.h"
#include "HitEvent.h"
#include "Client_Utility.h"
#include "Sound.h"

CFireTornado::CFireTornado()
	:CSkill()
{
}

HRESULT CFireTornado::Initialize(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	if (FAILED(__super::Initialize(pSkillData, pUser)))
		return E_FAIL;

	m_bNeedWeapon = true;
	//Bullet
	CBullet::BULLET_DESC tBulletDesc;
	tBulletDesc.pShooter = m_pUser;
	tBulletDesc.eHitEffect = EFF_MODEL_ID::HIT_FIRETORNADO_A;
	m_pBullet = static_cast<CBullet_FireTornado*>(m_pGameInstance->Clone_Proto_Object_Stock(CBullet_FireTornado::m_szProtoTag, &tBulletDesc));
	m_pBullet->Set_Active(false);
	m_pGameInstance->Add_GameObject_ToLayer((_uint)Get_CurrentTrueLevel(), (_uint)LAYERID::LAYER_BULLET, m_pBullet,true);
	Safe_AddRef(m_pBullet);

	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_firetornad_cast_01.effmodel");
	m_pCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pCastEffect->Set_Active(false);
	return S_OK;
}

void CFireTornado::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Update(fTimeDelta);
	}
}

void CFireTornado::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_pCastEffect->Is_Active())
	{
		m_pCastEffect->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, m_pCastEffect);
	}
}

void CFireTornado::On_SkillUsed()
{
	m_pCastEffect->Set_Transform(m_pUser->Get_Transform());
	m_pCastEffect->Start_Animation();
	m_pCastEffect->Set_Active(true);
}

void CFireTornado::On_CastingEnd()
{

}

void CFireTornado::Fire()
{
	_uint iDamgID = (_uint)SKILL_DATA_ID::DAMG;

	_float fDmg = m_pSkillDesc->iLevel * m_pSkillDesc->vecLevelUpData[iDamgID] + m_pSkillDesc->vecData[iDamgID];
	fDmg = m_pUser->Get_Stat().iATK * fDmg * 0.01;

	CTransform* pShooterTransform = m_pUser->Get_Transform();
	_vector vPos = pShooterTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = pShooterTransform->Get_State(CTransform::STATE_LOOK);
	vPos += vLook * 2.5f;

	m_pBullet->Launch(fDmg, vPos);
	CSound* pSouind = m_pGameInstance->Start_EffectPlay(LEVEL_LOADING, L"Skill_Wizard_FireTornado_Cast_01.wav");
	pSouind->SetVolume(100);
}


CFireTornado* CFireTornado::Create(SKILL_DATA* pSkillData, CCharacter* pUser)
{
	CFireTornado* pInstance = new CFireTornado;
	if (FAILED(pInstance->Initialize(pSkillData, pUser)))
	{
		MSG_BOX("CFireTornado Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFireTornado::Free()
{
	__super::Free();
	Safe_Release(m_pBullet);
	Safe_Release(m_pCastEffect);
}
