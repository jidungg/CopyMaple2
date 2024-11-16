#include "stdafx.h"
#include "Bullet_MagicClaw.h"
#include "Collider.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "Character.h"

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
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	m_pCollider = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));

	return S_OK;
}

void CBullet_MagicClaw::Update(_float fTimeDelta)
{
	if (false == m_bInvoke) return;

	if (m_fTimeAcc >= m_fAttackTime[m_iAttackCount])
	{
		m_iAttackCount++;
		//데미지 주기
		cout << "AttackCount : " << m_iAttackCount << endl;
		CGameObject* pTarget = Get_Target();
		if (nullptr != pTarget && pTarget->Is_Valid())
		{
			static_cast<CCharacter*>(pTarget)->Hit(m_pShooter,m_iDamage);
		}
	}
	else
	{
		CGameObject* pTarget = Get_Target();
		if (nullptr  != pTarget&& pTarget->Is_Valid())
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTarget->Get_Position());
			m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());
		}

	}
	if (m_iAttackCount >= m_fAttackTime.size())
	{
		m_bInvoke = false;
		m_iAttackCount = 0;
		Set_Active(false);
	}
	m_fTimeAcc += fTimeDelta;
}

void CBullet_MagicClaw::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix());
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBullet_MagicClaw::Render()
{
	return __super::Render();
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

}
