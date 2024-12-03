#include "stdafx.h"
#include "Bullet.h"
#include "Character.h"
#include "Collider_Sphere.h"
#include "GameInstance.h"
#include "HitEvent.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet& Prototype)
	: CGameObject(Prototype)
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
	BulletDesc* pDesc =static_cast< BulletDesc*>(pArg);
	m_pShooter = pDesc->pShooter;
	m_szHitEffectTag = pDesc->szHitEffectTag;
	return S_OK;
}

void CBullet::SearchTarget(list<CGameObject*>* pOutList, LAYERID eLayerID)
{
	auto listMonster = m_pGameInstance->Get_GameObjectList(eLayerID);
	for (auto& pMonster : *listMonster)
	{
		CCharacter* pTmpCharacter = static_cast<CCharacter*>(pMonster);
		if (false == pTmpCharacter->Is_Targetable())
			continue;
		if(Check_Collision(pTmpCharacter))
			pOutList->push_back(pTmpCharacter);
	}
}


_bool CBullet::Check_Collision(CGameObject* pOther)
{
	CCharacter* pCharacter = dynamic_cast<CCharacter*>(pOther);
	if (nullptr == pCharacter)
		return false;
	if (false == pCharacter->Is_Valid())
		return false;
	if (pCharacter->Get_Team() == m_eTeam)
		return false;

	return m_pCollider->Intersects(pCharacter->Get_Collider(0)) ;
}


void CBullet::Launch(_float fDamage, CGameObject* pTarget)
{
	m_fDamage = fDamage;
	Set_Active(true);
	if (pTarget)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTarget->Get_Position());
		Set_Target(pTarget);
	}
}

void CBullet::Launch(_float fDamage, _fvector vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_fDamage = fDamage;
	Set_Active(true);
}

void CBullet::Launch(_float fDamage)
{
	m_fDamage = fDamage;
	Set_Active(true);
}






void CBullet::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}
