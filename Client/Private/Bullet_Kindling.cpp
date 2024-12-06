#include "stdafx.h"
#include "Bullet_Kindling.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"
#include "HitEvent.h"
#include "EffModel.h"
CBullet_Kindling::CBullet_Kindling(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CBullet_Kindling::CBullet_Kindling(const CBullet& Prototype)
	:CBullet(Prototype)
{
}

HRESULT CBullet_Kindling::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBullet_Kindling::Initialize(void* pArg)
{
	BulletDesc* pDesc = static_cast<BulletDesc*>(pArg);
	pDesc->fSpeedPerSec = m_fSpeed;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.fRadius = 0.15f;
	tDesc.vCenter = { 0.f,0.f,0.f };
	m_pCollider = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));
	
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_kindling_ball_01.effmodel");
	m_pEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	m_pEffect->Register_OnAnimEndCallBack(bind(&CBullet_Kindling::On_EffectAnimEnd, this, placeholders::_1));
	Add_Child(m_pEffect);
	m_pEffect->Set_Active(false);
	return S_OK;
}

void CBullet_Kindling::Update(_float fTimeDelta)
{
	CGameObject* pTarget =  Get_Target();
	if (pTarget)
	{
		m_pTransformCom->LookAt(pTarget->Get_TransformPosition());
	}

	m_pTransformCom->Go_Straight(fTimeDelta);

	__super::Update(fTimeDelta);
}

void CBullet_Kindling::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix());
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
	CGameObject* pTarget = Get_Target();
	if (Check_Collision(Get_Target()))
	{
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pTarget, (_int)m_fDamage, m_szHitEffectTag));
		Set_Active(false);
	}
}

HRESULT CBullet_Kindling::Render()
{
	return __super::Render();
}

void CBullet_Kindling::Launch(_float fDamage, CGameObject* pTarget)
{
	CTransform* pShooterTransform = m_pShooter->Get_Transform();
	_vector vPos = pShooterTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = pShooterTransform->Get_State(CTransform::STATE_LOOK);
	vPos += vLook * 1.2;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->LookToward(vLook);
	Set_Target(pTarget);

	m_fDamage = fDamage;
	m_pEffect->Start_Animation();
	Set_Active(true);
}

void CBullet_Kindling::On_Collision(CGameObject* pOther)
{
	if (pOther == Get_Target())
	{
		m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pOther, (_int)m_fDamage, ("hit")));
		Set_Active(false);
	}
}


CBullet_Kindling* CBullet_Kindling::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_Kindling* pInstance = new CBullet_Kindling(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_Kindling");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_Kindling::Clone(void* pArg)
{
	CBullet_Kindling* pInstance = new CBullet_Kindling(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_Kindling");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_Kindling::Free()
{
	__super::Free();
}
