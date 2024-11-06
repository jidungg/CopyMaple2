#include"stdafx.h"
#include "Character.h"
#include "StateMachine.h"
#include "GameInstance.h"
#include "DeadObjEvent.h"
#include "CubeTerrain.h"
#include "Collider_Sphere.h"
CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CPawn(Prototype)
	, m_pAnimStateMachine{ Prototype.m_pAnimStateMachine }
	, m_vMoveDirectionXZ{ Prototype.m_vMoveDirectionXZ }
	, m_fJumpPower{ Prototype.m_fJumpPower }
	, m_fBattleTimeMax{ Prototype.m_fBattleTimeMax }
	, m_fRunSpeed{ Prototype.m_fRunSpeed }
	, m_fWalkSpeed{ Prototype.m_fWalkSpeed } 
{
}

HRESULT CCharacter::Initialize(void* pArg)
{
	__super::Initialize(pArg);


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

	__super::Update(fTimeDelta);
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

void CCharacter::Late_Update(_float fTimeDelta)
{
	m_vNextPos = XMVectorSetY(m_vNextPos, XMVectorGetY(m_vNextPos) + m_fUpForce * fTimeDelta);
	_float fFootHeight= XMVectorGetY(m_vNextPos);
	if (m_fCelingHeight <= fFootHeight + m_fBodyCollisionHeight)
	{
		m_fUpForce = 0;
		m_vNextPos = XMVectorSetY(m_vNextPos, m_fCelingHeight - m_fBodyCollisionHeight);
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
	if (m_bMove)
	{
		m_pTransformCom->LookToward(XMVectorSetY(m_vLookDirectionXZ, 0));
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vNextPos);

	m_fMoveDistanceXZ = 0.f;
	m_vMoveDirectionXZ = XMVectorSet(0, 0, 0, 0);
	__super::Late_Update(fTimeDelta);
}
void CCharacter::Free()
{
	__super::Free();
	Safe_Release(m_pAnimStateMachine);

}

