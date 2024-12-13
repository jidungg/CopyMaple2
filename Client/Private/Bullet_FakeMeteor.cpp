#include "stdafx.h"
#include "Bullet_FakeMeteor.h"
#include "GameInstance.h"
#include "Collider_Cylinder.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"

CBullet_FakeMeteor::CBullet_FakeMeteor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet(pDevice, pContext)
{
}

CBullet_FakeMeteor::CBullet_FakeMeteor(const CBullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CBullet_FakeMeteor::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollider_Cylinder::CYLINDER_COLLIDER_DESC tDesc;
	tDesc.fRadius = 5.f;
	tDesc.vCenter = { 0.f,3.f,0.f };
	tDesc.fHeight = 6.f;
	m_pCollider = static_cast<CCollider_Cylinder*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Cylinder::m_szProtoTag, &tDesc));

	//SplashInvokeEffectA
	CEffModelObject::EFFECTOBJ_DESC tEffectDesc;
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_fakemeteor_splashinvoke_03_a.effmodel");
	m_pSplashInvokeEffectA = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashInvokeEffectA);
	ANIM_EVENT tAnimEvent;
	tAnimEvent.fTime = 0.5;
	tAnimEvent.pFunc = bind(&CBullet_FakeMeteor::On_MeteorImpact, this);
	m_pSplashInvokeEffectA->Register_AnimEvent(tAnimEvent);
	m_pSplashInvokeEffectA->Set_Active(false);

	//SplashInvokeEffectB
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_fakemeteor_splashinvoke_03_b.effmodel");
	m_pSplashInvokeEffectB = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashInvokeEffectB);
	m_pSplashInvokeEffectB->Register_OnAnimEndCallBack(bind(&CBullet_FakeMeteor::On_SplashInvokeBEnd, this, placeholders::_1));
	m_pSplashInvokeEffectB->Set_Active(false);

	//SplashCastEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_fakemeteor_splashcast_01.effmodel");
	m_pSplashCastEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashCastEffect);
	m_pSplashCastEffect->Register_OnAnimEndCallBack(bind(&CBullet_FakeMeteor::On_SplashCastEffectAnimEnd, this, placeholders::_1));
	m_pSplashCastEffect->Set_Active(false);


	//SplashEndEffect
	tEffectDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tEffectDesc.strModelProtoName, "eff_wizard_fakemeteor_splashend_01.effmodel");
	m_pSplashEndEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tEffectDesc));
	Add_Child(m_pSplashEndEffect);
	m_pSplashEndEffect->Set_Active(false);
	return S_OK;
}

void CBullet_FakeMeteor::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (false == m_bInvoke && m_fInvokeDelay <= m_fTimeAcc)
	{
		m_pSplashInvokeEffectA->Start_Animation();
		m_pSplashInvokeEffectA->Set_Active(true);
		m_bInvoke = true;
	}
	m_fTimeAcc += fTimeDelta;
}

void CBullet_FakeMeteor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);

}

HRESULT CBullet_FakeMeteor::Render()
{
	return __super::Render();
}

_bool CBullet_FakeMeteor::Check_Collision(CGameObject* pOther)
{
	return static_cast<CCollider_Cylinder*>(m_pCollider)->Contains(pOther->Get_TransformPosition());
}



void CBullet_FakeMeteor::Launch(_float fDamage, CGameObject* pTarget)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTarget->Get_TransformPosition());
	m_fDamage = fDamage;
	m_pSplashCastEffect->Start_Animation(0, true, m_fInvokeDelay);
	m_pSplashCastEffect->Set_Active(true);
	m_bInvoke = false;
	m_fTimeAcc = 0.f;
	Set_Active(true);
}


void CBullet_FakeMeteor::On_SplashCastEffectAnimEnd(CEffModel* pModel)
{
	m_pSplashEndEffect->Start_Animation();
	m_pSplashEndEffect->Set_Active(true);
}

void CBullet_FakeMeteor::On_SplashInvokeBEnd(CEffModel* pModel)
{
	Set_Active(false);
}

void CBullet_FakeMeteor::On_MeteorImpact()
{
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	list<CGameObject*> listTarget;
	SearchTarget(&listTarget, LAYER_MONSTER);
	for (auto& pTarget : listTarget)
	{
		_bool bCrit = static_cast<CCharacter*>(m_pShooter)->Judge_Critical();
		m_fDamage *= bCrit ? 1.5 : 1.f;
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pTarget, (_int)m_fDamage,bCrit, true, m_eHitEffect));
	}
	m_pSplashInvokeEffectB->Start_Animation();
	m_pSplashInvokeEffectB->Set_Active(true);

}

CBullet_FakeMeteor* CBullet_FakeMeteor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_FakeMeteor* pInstance = new CBullet_FakeMeteor(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_FakeMeteor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_FakeMeteor::Clone(void* pArg)
{
	CBullet_FakeMeteor* pInstance = new CBullet_FakeMeteor(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_FakeMeteor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_FakeMeteor::Free()
{
	__super::Free();
	Safe_Release(m_pSplashCastEffect);
	Safe_Release(m_pSplashInvokeEffectA);
	Safe_Release(m_pSplashInvokeEffectB);
	Safe_Release(m_pSplashEndEffect);
}
