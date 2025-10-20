#include"stdafx.h"
#include "Character.h"
#include "StateMachine.h"
#include "GameInstance.h"
#include "DeadObjEvent.h"
#include "CubeTerrain.h"
#include "Collider_Sphere.h"
#include "Skill.h"
#include "Engine_Utility.h"

CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& Prototype)
	: CPawn(Prototype)
	, m_pAnimStateMachine{ Prototype.m_pAnimStateMachine }
	, m_vMoveDirectionXZ{ Prototype.m_vMoveDirectionXZ }
	, m_tStatDefault{ Prototype.m_tStatDefault }
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
	 if(m_bAttack)
		Get_CurrentSkill()->Update_CastingTime(fTimeDelta);

	 Get_CurrentSkill()->Update(fTimeDelta);
	 Update_Collider();
	 NatureRecover(fTimeDelta);
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
	XMStoreFloat(&m_fMoveDistanceXZ, XMVector3Length(m_vMoveDirectionXZ));
	m_vMoveDirectionXZ = XMVector4Normalize(m_vMoveDirectionXZ);
	m_bMove = false == XMVector4Equal(m_vMoveDirectionXZ, XMVectorZero());
	m_bRotate = false == XMVector4Equal(m_vLookDirectionXZ, XMVectorZero());

	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_TERRAIN:
	{
		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		CCubeTerrain* pTerrain = static_cast<CCubeTerrain*>(pOther);
		m_vNextPos = vPos;
		if(m_bMove)
		{
			m_vNextPos = pTerrain->Blocking(this, Get_BlockingRange());
			m_vMoveDirectionXZ = XMVector3Normalize(m_vNextPos - vPos);
			m_fMoveDistanceXZ = XMVector3Length(m_vNextPos - vPos).m128_f32[0];
		}

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
	_float3 vColliderOffset = Get_BodyCollisionOffset();
	if (m_fCelingHeight <= fFootHeight + vColliderOffset.y)
	{
		m_fUpForce = 0;
		m_vNextPos = XMVectorSetY(m_vNextPos, m_fCelingHeight - vColliderOffset.y);
	}
	_bool bLand = m_bOnFloor;

	m_bOnFloor = m_fFloorHeight >= XMVectorGetY(m_vNextPos);


	if (m_bOnFloor)
	{
		m_fUpForce = 0;

		m_vNextPos = XMVectorSetY(m_vNextPos, m_fFloorHeight);
		if (bLand == false)
		{
			_uint irand = rand() % 2 + 1;
			wchar_t result[50];
			swprintf(result, 50, L"CH_Jump_Grass_0%d.wav", irand);
			m_pGameInstance->Start_EffectPlay(LEVEL_LOADING, result);
		}
	}
	else
	{
		m_fUpForce -= fTimeDelta * 9.8f * 3;
	}
	if(m_bRotate)
	{
		m_pTransformCom->LookToward(XMVectorSetY(m_vLookDirectionXZ, 0));
	}
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vNextPos);

	m_fMoveDistanceXZ = 0.f;
	m_vMoveDirectionXZ = XMVectorSet(0, 0, 0, 0);

	Get_CurrentSkill()->Late_Update(fTimeDelta);
	__super::Late_Update(fTimeDelta);
}
HRESULT CCharacter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	return S_OK;
}

_bool CCharacter::Use_Skill(CSkill* pSkill)
{
	return true;
}

_vector CCharacter::Get_Hitpoint()
{
	return Get_WorldPosition() + XMVectorSet(0, m_tStat.fBodyHeight,0,0);
}

_vector CCharacter::Get_OverHeadPoint()
{
	return Get_WorldPosition() + XMVectorSet(0, m_tStat.fBodyHeight *2, 0, 0);;
}

_float CCharacter::Get_BodyCollisionRadius()
{
	return static_cast<CCollider_Sphere*>( Get_Collider(m_iBodyColliderIndex))->Get_Desc()->Radius;
}

_float3 CCharacter::Get_BodyCollisionOffset()
{
	return  static_cast<CCollider_Sphere*>(Get_Collider(m_iBodyColliderIndex))->Get_Offset();
}

_uint CCharacter::Get_CurrentAnimIdx()
{
	return m_pBody->Get_AnimIndex();
}

_uint CCharacter::Get_BlockingRange()
{
	return ceilf(Get_BodyCollisionRadius() / 2);
}


_float CCharacter::Get_AnimationProgress(_uint iAnimIdx)
{
	return m_pBody->Get_AnimationProgress(iAnimIdx);
}


_bool CCharacter::Judge_Critical()
{
	return CEngineUtility::Get_RandomFloat(0, 1) <= Get_CritPefrcent();
}

const _float4x4* CCharacter::Get_BoneMatrix(const _char* szBoneName)
{
	return m_pBody->Get_BoneMatrix(szBoneName);
}

void CCharacter::Move_Forward(_float fDist)
{
	m_vMoveDirectionXZ = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE_LOOK))* fDist;
}
void CCharacter::Hit(CGameObject* pFoe,_int iDamage)
{
	m_tStat.iHP -= iDamage;
	Set_Target(pFoe);
	if (m_tStat.iHP <= 0)
	{
		m_tStat.iHP = 0;
		On_HPZero();
	}

}
void CCharacter::FullRecovery()
{
	m_tStat.iHP = Get_TotalHP();
	m_tStat.iSP = m_tStatDefault.iSP;
	m_tStat.iEP = m_tStatDefault.iEP;
	m_bHPZero = false;
}
void CCharacter::RestoreHP(_int iAmount)
{
	m_tStat.iHP += iAmount;
	_int iTotal = Get_TotalHP();
	if (m_tStat.iHP > iTotal)
		m_tStat.iHP = iTotal;
}
void CCharacter::RestoreSP(_int iAmount)
{
	m_tStat.iSP += iAmount;
	if (m_tStat.iSP > m_tStatDefault.iSP)
		m_tStat.iSP = m_tStatDefault.iSP;
}
void CCharacter::RestoreEP(_int iAmount)
{
	m_tStat.iEP += iAmount;
	if (m_tStat.iEP > m_tStatDefault.iEP)
		m_tStat.iEP = m_tStatDefault.iEP;
}
void CCharacter::NatureRecover(_float fTimeDelta)
{
	m_fEPRecoverTimeAcc += fTimeDelta;
	if (m_fEPRecoverTimeAcc >= m_tStat.fEPRecovery)
	{
		RestoreEP();
		m_fEPRecoverTimeAcc = 0;
	}
	m_fHPRecoverAcc += m_tStat.fHPRecovery *fTimeDelta;
	if (m_fHPRecoverAcc >= 1)
	{
		RestoreHP((_int)m_fHPRecoverAcc);
		m_fHPRecoverAcc = fmod((double)m_fHPRecoverAcc, 1);
	}
	m_fSPRecoverAcc += m_tStat.fSPRecovery * fTimeDelta;
	if (m_fSPRecoverAcc >= 1)
	{
		RestoreSP((_int)m_fSPRecoverAcc);
		m_fSPRecoverAcc = fmod((double)m_fSPRecoverAcc, 1);
	}
}
void CCharacter::Respawn()
{
	Set_Active(true);
	Set_Target(nullptr);
	FullRecovery();
	m_bHPZero = false;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHomePos);
}
_bool CCharacter::Is_Targetable()
{
	return 	Is_Valid();
}
_vector CCharacter::BlockXZ(_vector vPrev, _vector vNext, _float fRadius)
{
	_vector vMyPosition = Get_WorldPosition();
	_float fMyRadius = Get_BodyCollisionRadius();
	_float fDist = XMVector3Length(vNext - vMyPosition).m128_f32[0];
	if (fDist >= fMyRadius + fRadius)
	{
		return vNext;
	}
	_vector vCollisionNormal = XMVector3Normalize(vNext - vMyPosition);
	_float fCollisionDepth = fMyRadius + fRadius - fDist;
	return vNext + vCollisionNormal * fCollisionDepth;
}
void CCharacter::Free()
{
	__super::Free();
	Safe_Release(m_pAnimStateMachine);
	for (auto& skill : m_mapSkill)
		Safe_Release(skill.second);
}

