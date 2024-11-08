#include"stdafx.h"
#include "Character.h"
#include "StateMachine.h"
#include "GameInstance.h"
#include "DeadObjEvent.h"
#include "CubeTerrain.h"
#include "Collider_Sphere.h"
#include "Skill.h"
CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CPawn(Prototype)
	, m_pAnimStateMachine{ Prototype.m_pAnimStateMachine }
	, m_vMoveDirectionXZ{ Prototype.m_vMoveDirectionXZ }
	, m_tStat{ Prototype.m_tStat }
	, m_mapSkill{ Prototype.m_mapSkill }
{
}

HRESULT CCharacter::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	CHARACTER_DESC* pDesc = (CHARACTER_DESC*)pArg;
	m_vecCollider.resize(pDesc->iColliderCount);
	m_iBodyColliderIndex = pDesc->iBodyColliderIndex;
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.vCenter = pDesc->fBodyCollisionOffset;
	tDesc.fRadius = pDesc->fBodyCollisionRadius;
	m_vecCollider[m_iBodyColliderIndex] = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));
	Add_Component(m_vecCollider[m_iBodyColliderIndex], L"Com_Collider_Body");

	//AnimStateMachine
	m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));

	return S_OK;
}

void CCharacter::Priority_Update(_float fTimeDelta)
{


	__super::Priority_Update(fTimeDelta);
}

void CCharacter::Update(_float fTimeDelta)
{
	 XMStoreFloat(&m_fMoveDistanceXZ,XMVector3Length(m_vMoveDirectionXZ));
	 m_vMoveDirectionXZ = XMVector4Normalize(m_vMoveDirectionXZ);
	 m_bMove = false == XMVector4Equal(m_vMoveDirectionXZ, XMVectorSet(0, 0, 0, 0));
	 m_bRotate = false == XMVector4Equal(m_vLookDirectionXZ, XMVectorSet(0, 0, 0, 0));

	 for (auto& pSkill : m_mapSkill)
		 pSkill.second->Update(fTimeDelta);
	 if(m_bAttack)
		Get_CurrentSkill()->Update_CastingTime(fTimeDelta);
	 Update_Collider();
	__super::Update(fTimeDelta);
}

void CCharacter::Update_Collider()
{
	for (auto& pCollider : m_vecCollider)
	{
		pCollider->Update(XMLoadFloat4x4(&m_WorldMatrix));
	}
}


//충돌로 위치 보정
//MoveDirection과 MoveDistance가 계산돼있어야 함.
_bool CCharacter::Check_Collision(CGameObject* pOther)
{
	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_TERRAIN:
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		CCubeTerrain* pTerrain = static_cast<CCubeTerrain*>(pOther);
		m_vNextPos = vPos;
		if(m_bMove)
			m_vNextPos = pTerrain->BlockXZ(this);

		m_fFloorHeight = pTerrain->Get_FloorHeight(m_vNextPos);
		m_fCelingHeight = pTerrain->Get_CelingHeight(m_vNextPos);

		break;
	}
	case Client::LAYER_MONSTER:
	case Client::LAYER_INTERACTION:
	case Client::LAYER_PLAYER:
	case Client::LAYER_UI:
	case Client::LAYER_CAMERA:
	case Client::LAYER_LAST:
	default:
		break;
	}
	return false;
}

_bool CCharacter::Use_Skill(CSkill* pSkill)
{
	return true;
}

_float CCharacter::Get_BodyCollisionRadius()
{
	return Get_Collider(m_iBodyColliderIndex)->Get_Desc()->Radius;
}

_float3 CCharacter::Get_BodyCollisionOffset()
{
	return  Get_Collider(m_iBodyColliderIndex)->Get_Offset();
}

_uint CCharacter::Get_CurrentAnimIdx()
{
	return m_pBody->Get_AnimIndex();
}

CCollider_Sphere* CCharacter::Get_Collider(_uint iColliderIndex)
{
	return static_cast<CCollider_Sphere*>(m_vecCollider[iColliderIndex]);
	
}
_float CCharacter::Get_AnimationProgress(_uint iAnimIdx)
{
	return m_pBody->Get_AnimationProgress(iAnimIdx);
}

void CCharacter::Late_Update(_float fTimeDelta)
{
	m_vNextPos = XMVectorSetY(m_vNextPos, XMVectorGetY(m_vNextPos) + m_fUpForce * fTimeDelta);
	_float fFootHeight= XMVectorGetY(m_vNextPos);
	_float3 vColliderOffset = Get_BodyCollisionOffset();
	if (m_fCelingHeight <= fFootHeight + vColliderOffset.y)
	{
		m_fUpForce = 0;
		m_vNextPos = XMVectorSetY(m_vNextPos, m_fCelingHeight - vColliderOffset.y);
	}
	m_bOnFloor = m_fFloorHeight >= XMVectorGetY(m_vNextPos);
	if (m_bOnFloor)
	{
		m_fUpForce = 0;

		m_vNextPos = XMVectorSetY(m_vNextPos, m_fFloorHeight);
	}
	else
	{
		m_fUpForce -= fTimeDelta * 9.8f * 3;
	}
	if(m_bRotate)
		m_pTransformCom->LookToward(XMVectorSetY(m_vLookDirectionXZ, 0));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vNextPos);

	m_fMoveDistanceXZ = 0.f;
	m_vMoveDirectionXZ = XMVectorSet(0, 0, 0, 0);
	__super::Late_Update(fTimeDelta);
}
HRESULT CCharacter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	for (auto& pCollider : m_vecCollider)
		if (pCollider->Is_Active())
			pCollider->Render();
	return S_OK;
}
void CCharacter::Free()
{
	__super::Free();
	Safe_Release(m_pAnimStateMachine);
	for (auto& pCollider : m_vecCollider)
	{
		Safe_Release(pCollider);
	}
	m_vecCollider.clear();
		for (auto& skill : m_mapSkill)
		Safe_Release(skill.second);
}

