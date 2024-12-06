#include "stdafx.h"
#include "Bullet_MagicClaw.h"
#include "Collider.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"
#include "HitEvent.h"

CBullet_MagicClaw::CBullet_MagicClaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CBullet_MagicClaw::CBullet_MagicClaw(const CBullet& Prototype)
	:CBullet(Prototype)
{
}

HRESULT CBullet_MagicClaw::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBullet_MagicClaw::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	//CastEffect
	CEffModelObject::EFFECTOBJ_DESC tCastEffDesc;
	tCastEffDesc.eModelProtoLevelID = LEVEL_LOADING;
	strcpy_s(tCastEffDesc.strModelProtoName, "eff_wizard_magicclaw_remain_01_a.effmodel");
	m_pEffect = static_cast<CEffModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CEffModelObject::m_szProtoTag, &tCastEffDesc));
	Add_Child(m_pEffect);
	m_pEffect->Register_OnAnimEndCallBack(bind(&CBullet_MagicClaw::On_EffectAnimEnd, this, placeholders::_1));
	m_pEffect->Set_Active(false);
	//Collider
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	m_pCollider = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));

	return S_OK;
}

void CBullet_MagicClaw::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_iAttackCount < m_fAttackTime.size() && m_fTimeAcc >= m_fAttackTime[m_iAttackCount])
	{
		m_iAttackCount++;

		//데미지 주기
		CGameObject* pTarget = Get_Target();
		if (nullptr != pTarget && pTarget->Is_Valid())
		{
			m_pGameInstance->Push_Event(CHitEvent::Create(m_pShooter, pTarget, (_int)m_fDamage, ("hit")));
		}
	}

	m_fTimeAcc += fTimeDelta;
}

void CBullet_MagicClaw::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBullet_MagicClaw::Render()
{
	__super::Render();
	return S_OK;

}

void CBullet_MagicClaw::Launch(_float fDamage, CGameObject* pTarget)
{
	
	m_fDamage = fDamage;
	Set_Target(pTarget);
	Set_Transform(Get_Target()->Get_TransformPosition());
	Set_Active(true);
	m_pEffect->Start_Animation();
	m_iAttackCount = 0;
	m_fTimeAcc = 0.f;
}


CBullet_MagicClaw* CBullet_MagicClaw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_MagicClaw* pInstance = new CBullet_MagicClaw(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Create : CBullet_MagicClaw");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_MagicClaw::Clone(void* pArg)
{
	CBullet_MagicClaw* pInstance = new CBullet_MagicClaw(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBullet_MagicClaw");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBullet_MagicClaw::Free()
{
	__super::Free();
	Safe_Release(m_pEffect);
}
