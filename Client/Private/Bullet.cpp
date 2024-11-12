#include "stdafx.h"
#include "Bullet.h"
#include "Character.h"
#include "Collider_Sphere.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModelObject(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet& Prototype)
	: CModelObject(Prototype)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}


_bool CBullet::Check_Collision(CGameObject* pOther)
{
	CCharacter* pCharacter = dynamic_cast<CCharacter*>(pOther);
	if (nullptr == pCharacter)
		return false;
	if (pCharacter->Get_Team() == m_eTeam)
		return false;

	CCollider_Sphere* pCollider =  pCharacter->Get_Collider(0);
	if(pCollider->Intersects(m_pCollider))
	{
		pCharacter->Hit(m_iDamage);
		return true;
	}
	return false;
}


void CBullet::Invoke(_float fDamg, _vector vPosition)
{
	m_iDamage = fDamg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_bInvoke = true;
	m_fTimeAcc = 0.f;
	Set_Active(true);
}

void CBullet::Invoke(_float fDamg, CGameObject* pTarget)
{
	m_iDamage = fDamg;
	Set_Target(pTarget);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Get_Target()->Get_Position()                                                                                                                                                                                                                                                                                                                                                                                                                    );
	m_bInvoke = true;
	m_fTimeAcc = 0.f;
	Set_Active(true);
}



void CBullet::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}
