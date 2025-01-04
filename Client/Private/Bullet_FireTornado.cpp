#include "stdafx.h"
#include "Bullet_FireTornado.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"

CBullet_FireTornado::CBullet_FireTornado(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_FireTornado::CBullet_FireTornado(const CBullet& Prototype)
	: CBullet(Prototype)
{
}



HRESULT CBullet_FireTornado::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.fRadius = 3.f;
	tDesc.vCenter = { 0.f,0.f,0.f };
	m_pCollider = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));


	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;


	//SplashCastEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_firetornadosplash_cast_01.effmodel");
	m_pSplashCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	Add_Child(m_pSplashCastEffect);
	m_pSplashCastEffect->Set_Active(false);


	//SplashEndEffect
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_firetornadosplash_end_01.effmodel");
	m_pSplashEndEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	Add_Child(m_pSplashEndEffect);
	m_pSplashEndEffect->Set_Active(false);
	return S_OK;
}

void CBullet_FireTornado::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	m_fDamgTimeAcc += fTimeDelta;

}

void CBullet_FireTornado::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (false == m_bSplashEnded && false == m_pSplashCastEffect->Is_Active())
	{
		m_bSplashEnded = true;
		m_pSplashCastEffect->Set_Active(false);
		m_pSplashEndEffect->Start_Animation(0,false,2);
		m_pSplashEndEffect->Set_Active(true);
		
	}
	else if(m_bSplashEnded && false ==m_pSplashEndEffect->Is_Active())
	{
		Set_Active(false);
		return;
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);

	if (m_fDamgTimeAcc >= m_fDamgInterval)
	{
		m_fDamgTimeAcc = 0;
	
		list<CGameObject*> listTarget;
		SearchTarget(&listTarget, LAYER_MONSTER);
		for (auto& pTarget : listTarget)
		{
			_bool bCrit;
			_float fDmg = m_pSkill->Calc_Damg(bCrit);
			m_pGameInstance->Push_Event(CDamgEvent::Create(m_pShooter, pTarget, (_int)fDmg, bCrit, true,m_eHitEffect));
		}
	}
}

HRESULT CBullet_FireTornado::Render()
{
	return __super::Render();
}


void CBullet_FireTornado::Launch(CSkill* pSkill, _vector vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pSkill = pSkill;
	m_pSplashCastEffect->Start_Animation();
	m_pSplashCastEffect->Set_Active(true);
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	m_bSplashEnded = false;
	Set_Active(true);
}




CBullet_FireTornado* CBullet_FireTornado::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_FireTornado* pInstance = new CBullet_FireTornado(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_FireTornado");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_FireTornado::Clone(void* pArg)
{
	CBullet_FireTornado* pInstance = new CBullet_FireTornado(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_Kindling");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_FireTornado::Free()
{
	__super::Free();
}
