#include "stdafx.h"
#include "Bayar.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "SkillManager.h"
#include "Collider_Sphere.h"
#include "CubeTerrain.h"

CBayar::CBayar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster(pDevice, pContext)
{
	m_vecCollider.resize(PART_LAST);
	m_vecPartsMatrix.resize(PART_LAST);
}

CBayar::CBayar(const CBayar& Prototype)
	: CMonster(Prototype)
{
	m_vecCollider.resize(PART_LAST);
	m_vecPartsMatrix.resize(PART_LAST);
}

HRESULT CBayar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBayar::Initialize(void* pArg)
{
	CMonster::MONSTER_DESC* pDesc = static_cast<CMonster::MONSTER_DESC*>(pArg);
	pDesc->fRotationPerSec = XMConvertToRadians(360.f);
	pDesc->fSpeedPerSec = 5.f;
	pDesc->iBodyColliderIndex = PART_ID::BODY;
	pDesc->iColliderCount = PART_LAST;
	pDesc->eMonID = MONSTER_ID::BAYAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	m_vecPartsMatrix[PART_ID::BODY] = m_pBody->Get_BoneMatrix("Bip01 Spine");
	m_vecPartsMatrix[PART_ID::RIGHT_ARM] = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant24");
	m_vecPartsMatrix[PART_ID::LEFT_ARM] = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant22");


	m_pTransformCom->Scaling(2.f, 2.f, 2.f);

	return S_OK;
}

HRESULT CBayar::Ready_Components(CHARACTER_DESC* pDesc)
{
	m_vecCollider.resize(PART_LAST);
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc = {};
	//LEFTARM
	tDesc.fRadius = 0.3;
	tDesc.vCenter = { 0,0, 0 };
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_LeftArm_Collider"), (CComponent**)&m_vecCollider[PART_ID::LEFT_ARM], &tDesc)))
		return E_FAIL;

	//RIGHTARM
	tDesc.fRadius = 0.3;
	tDesc.vCenter = { 0,0, 0 };
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_Right_Arm_Collider"), (CComponent**)&m_vecCollider[PART_ID::RIGHT_ARM], &tDesc)))
		return E_FAIL;

	//BODY
	//tDesc.fRadius = pDesc->fBodyCollisionRadius;
	//tDesc.vCenter = pDesc->fBodyCollisionOffset;
	//if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_Body_Collider"), (CComponent**)&m_vecCollider[PART_ID::BODY], &tDesc)))
	//	return E_FAIL;
	//TODO : Hand
	return S_OK;
}
HRESULT CBayar::Ready_AnimStateMachine()
{
	//TODO: JUMP, DEADDAMG. DEADIDLE 추가해야함.
	m_pBody->Set_Animation((_uint)STATE::AS_REGEN);

	m_pBody->Set_AnimationLoop((_uint)STATE::AS_WALK_A, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_RUN, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_ATTACK_IDLE, true);
	m_pBody->Set_AnimationLoop((_uint)STATE::AS_DEAD_IDLE_A, true);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_A, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_B, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_C, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_D, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_F, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_1_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_A, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_B, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_C, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_D, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_2_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_E, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_G, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_3_H, 1);
	m_pBody->Set_AnimPostDelayPercent((_uint)STATE::AS_ATTACK_4_G, 1);



	return __super::Ready_AnimStateMachine();
}
void CBayar::Priority_Update(_float fTimeDelta)
{
//	if (nullptr == m_pTarget || false == m_pTarget->Is_Active() || m_pTarget->Is_Dead())
//	{
//		m_pTarget = nullptr;
//		m_bDetected = false;
//		m_fTargetDistance = FLT_MAX;
//		return;
//	}
//	//타겟이 있음
//
//	//타겟까지 방향, 거ㅏ리 측정.
//	XMVECTOR vPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);
//	m_vMoveDirectionXZ = vPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
//	m_vMoveDirectionXZ = XMVectorSetY(m_vMoveDirectionXZ, 0);
//	XMStoreFloat(&m_fTargetDistance, XMVector3Length(m_vMoveDirectionXZ));
//	m_vMoveDirectionXZ = XMVector3Normalize(m_vMoveDirectionXZ);
//	m_vLookDirectionXZ = m_vMoveDirectionXZ;
//	m_vMoveDirectionXZ *= m_tStat.fRunSpeed * fTimeDelta;
//
//
//	if (Is_AttackCoolReady())
//	{
//
//		if (m_fTargetDistance <= m_tStat.fAttackRange)
//		{
//			m_bAttack = true;
//			m_bChase = false;
//			m_fAttackTimeAcc = 0.f;
//			m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ATTACKTRIGGER);
//			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
//		}
//
//	}
//	//공격 쿨임 : 추적상태면 쫒고 아니면 안쫒기
//	else
//	{
//		// 추적상태가 아니다 -> 공격범위에 들어온 뒤로 추적범위 밖으로 나가지 않았음.
//		if (false == m_bChase)
//		{
//			if (m_fTargetDistance > m_fChaseRange)//추적 범위 밖으로 나가면 추적
//				m_bChase = true;
//		}
//		else //추적 상태다 -> 추적 범위 밖으로 나간 뒤 공격 범위 안으로 들어오지 않았음.
//		{
//			if (m_fTargetDistance <= m_tStat.fAttackRange)//공격 범위 안으로 들어올 때까지 추적
//				m_bChase = false;
//		}
//		if (false == m_bChase)
//			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
//	}
//	// 공격 범위 안임
//
//	if (m_bAttack)
//	{
//		m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
//		m_vLookDirectionXZ = XMVectorZero();
//	}

	__super::Priority_Update(fTimeDelta);
}

void CBayar::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}


void CBayar::Update_Collider()
{
	_matrix		SocketMatrix;
	for (_uint i = 0; i < PART_LAST; i++)
	{

		if (i == DETECTION)
			m_vecCollider[i]->Update(m_pTransformCom->Get_WorldMatrix());
		else
		{
			SocketMatrix = XMLoadFloat4x4(m_vecPartsMatrix[i]);
			for (size_t i = 0; i < 3; i++)
				SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
			m_vecCollider[i]->Update(SocketMatrix * m_pTransformCom->Get_WorldMatrix());
		}
	}
}

_bool CBayar::Check_Collision(CGameObject* pOther)
{
	return __super::Check_Collision(pOther);
}
void CBayar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}




HRESULT CBayar::Render()
{
	for (auto& child : m_pChilds)
	{
		if (child->Is_Active() && child->Is_Dead() == false)
			child->Render();
	}
	for (auto& pCollider : m_vecCollider)
	{
		if (pCollider->Is_Active())
			pCollider->Render();
	}
	return S_OK;
}






CBayar* CBayar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBayar* pInstance = new CBayar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBayar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBayar::Clone(void* pArg)
{
	CBayar* pInstance = new CBayar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBayar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBayar::Free()
{
	__super::Free();
}

