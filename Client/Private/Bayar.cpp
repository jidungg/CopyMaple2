#include "stdafx.h"
#include "Bayar.h"
#include "ModelObject.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "SkillManager.h"
#include "Collider_Sphere.h"
#include "CubeTerrain.h"

CBayar::CBayar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
	m_vecCollider.resize(PART_LAST);
	m_vecPartsMatrix.resize(PART_LAST);
}

CBayar::CBayar(const CBayar& Prototype)
	: CCharacter(Prototype)
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
	CCharacter::CHARACTER_DESC* pDesc = static_cast<CCharacter::CHARACTER_DESC*>(pArg);
	pDesc->fRotationPerSec = XMConvertToRadians(360.f);
	pDesc->fSpeedPerSec = 5.f;
	pDesc->fBodyCollisionRadius = 0.45;
	pDesc->fBodyCollisionOffset = { 0.2f,0.05f, 0.f };
	pDesc->iBodyColliderIndex = PART_ID::BODY;
	pDesc->iColliderCount = PART_LAST;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Parts()))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;
	if (FAILED(Ready_Skill()))
		return E_FAIL;
	m_vecPartsMatrix[PART_ID::BODY] = m_pBody->Get_BoneMatrix("Bip01 Spine");
	m_vecPartsMatrix[PART_ID::RIGHT_ARM] = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant24");
	m_vecPartsMatrix[PART_ID::LEFT_ARM] = m_pBody->Get_BoneMatrix("02030008_N_SandstoneGiant22");


	m_pTransformCom->Scaling(2.f, 2.f, 2.f);

	return S_OK;
}

HRESULT CBayar::Ready_Components(CHARACTER_DESC* pDesc)
{
	//DETECTION
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc = {};
	tDesc.fRadius = m_fDetectionRange;
	auto vScale = m_pTransformCom->Compute_Scaled();
	tDesc.vCenter = { 0.f,vScale.y / 2, 0.f };
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_Detection_Collider"), (CComponent**)&m_vecCollider[PART_ID::DETECTION], &tDesc)))
		return E_FAIL;

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
	tDesc.fRadius = pDesc->fBodyCollisionRadius;
	tDesc.vCenter = pDesc->fBodyCollisionOffset;
	if (FAILED(Add_Component(LEVEL_LOADING, CCollider_Sphere::m_szProtoTag, TEXT("Com_Body_Collider"), (CComponent**)&m_vecCollider[PART_ID::BODY], &tDesc)))
		return E_FAIL;
	//TODO : Hand
	return S_OK;
}
HRESULT CBayar::Ready_AnimStateMachine()
{
	m_pAnimStateMachine = static_cast<CStateMachine*>(m_pGameInstance->Clone_Proto_Component_Stock(TEXT("Prototype_GameObject_StateMachine")));
	Add_Component(m_pAnimStateMachine, TEXT("Com_StateMachine"));
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CBayar::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CBayar::On_SubStateChange, this, placeholders::_1));

	m_pBody->Set_Animation((_uint)STATE::AS_REGEN);
	CState* pState; CTransition* pTransition; Condition* pCondition;
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


	//CONDITION
	m_pAnimStateMachine->Add_TriggerConditionVariable(ANIM_CONDITION::AC_ANIMEND);
	m_pAnimStateMachine->Add_TriggerConditionVariable(ANIM_CONDITION::AC_ATTACKTRIGGER);
	m_pAnimStateMachine->Add_ConditionVariable(ANIM_CONDITION::AC_ISATTACK, &m_bAttack);
	m_pAnimStateMachine->Add_ConditionVariable(ANIM_CONDITION::AC_DETECTED, &m_bDetected);
	m_pAnimStateMachine->Add_ConditionVariable(ANIM_CONDITION::AC_ATTACKIDX, &m_iCurAttack);
	m_pAnimStateMachine->Add_ConditionVariable(ANIM_CONDITION::AC_MOVE, &m_bMove);
	m_pAnimStateMachine->Add_ConditionVariable(ANIM_CONDITION::AC_WALK, &m_bWalk);

	//Add STATE
	for (_uint i = 0; i < STATE::AS_LAST; i++)
		m_pAnimStateMachine->Add_State(i);
	for (_uint i = BS_BORN; i < STATE::BS_LAST; i++)
		m_pAnimStateMachine->Add_State(i);
	//Main Transition
	//BORN
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_BORN, STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	//IDLE
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_IDLE, STATE::BS_DETECTED);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	//DETECTED
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_DETECTED, STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_DETECTED, STATE::BS_ATTACK1);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_DETECTED, STATE::BS_MOVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	//MOVE
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_MOVE, STATE::BS_DETECTED);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_MOVE, STATE::BS_ATTACK1);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_MOVE, STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);

	//ATTACK
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_ATTACK1, STATE::BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_ATTACK1, STATE::BS_DETECTED);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(STATE::BS_ATTACK1, STATE::BS_MOVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);


	//Sub Transition
	//BORN
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_BORN, STATE::AS_REGEN);
	//IDLE
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_IDLE, STATE::AS_IDLE);
	//DETECTED
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_DETECTED, STATE::AS_ATTACK_IDLE);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_DETECTED, STATE::AS_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	//MOVE
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_MOVE, STATE::AS_RUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_MOVE, STATE::AS_WALK_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, false);
	//ATTACK
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_A);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_A);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_B);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_B);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_C);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_C);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_D);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_D);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_E);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_E);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_F);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_F);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_G);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_G);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::BS_ATTACK1, STATE::AS_ATTACK_1_H);
	m_pAnimStateMachine->Bind_Condition(pTransition, ANIM_CONDITION::AC_ATTACKIDX, CONDITION_TYPE::EQUAL, AS_ATTACK_1_H);

	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_A, STATE::AS_ATTACK_2_A);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_B, STATE::AS_ATTACK_2_B);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_C, STATE::AS_ATTACK_2_C);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_D, STATE::AS_ATTACK_2_D);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_E, STATE::AS_ATTACK_2_E);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_G, STATE::AS_ATTACK_2_G);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_1_H, STATE::AS_ATTACK_2_H);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_2_E, STATE::AS_ATTACK_3_E);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_2_G, STATE::AS_ATTACK_3_G);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_2_H, STATE::AS_ATTACK_3_H);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);
	pTransition = m_pAnimStateMachine->Add_SubTransition(STATE::AS_ATTACK_3_G, STATE::AS_ATTACK_4_G);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, ANIM_CONDITION::AC_ANIMEND);

	m_pAnimStateMachine->Set_CurrentState(STATE::BS_BORN);
	return S_OK;
}
void CBayar::Priority_Update(_float fTimeDelta)
{
	if (nullptr == m_pTarget || false == m_pTarget->Is_Active() || m_pTarget->Is_Dead())
	{
		m_pTarget = nullptr;
		m_bDetected = false;
		m_fTargetDistance = FLT_MAX;
		return;
	}
	//타겟이 있음

	//타겟까지 방향, 거ㅏ리 측정.
	XMVECTOR vPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	m_vMoveDirectionXZ = vPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vMoveDirectionXZ = XMVectorSetY(m_vMoveDirectionXZ, 0);
	XMStoreFloat(&m_fTargetDistance, XMVector3Length(m_vMoveDirectionXZ));
	m_vMoveDirectionXZ = XMVector3Normalize(m_vMoveDirectionXZ);
	m_vLookDirectionXZ = m_vMoveDirectionXZ;
	m_vMoveDirectionXZ *= m_tStat.fRunSpeed * fTimeDelta;


	if (Is_AttackCoolReady())
	{

		if (m_fTargetDistance <= m_tStat.fAttackRange)
		{
			m_bAttack = true;
			m_bChase = false;
			m_fAttackTimeAcc = 0.f;
			m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ATTACKTRIGGER);
			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		}

	}
	//공격 쿨임 : 추적상태면 쫒고 아니면 안쫒기
	else
	{
		// 추적상태가 아니다 -> 공격범위에 들어온 뒤로 추적범위 밖으로 나가지 않았음.
		if (false == m_bChase)
		{
			if (m_fTargetDistance > m_fChaseRange)//추적 범위 밖으로 나가면 추적
				m_bChase = true;
		}
		else //추적 상태다 -> 추적 범위 밖으로 나간 뒤 공격 범위 안으로 들어오지 않았음.
		{
			if (m_fTargetDistance <= m_tStat.fAttackRange)//공격 범위 안으로 들어올 때까지 추적
				m_bChase = false;
		}
		if (false == m_bChase)
			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
	}
	// 공격 범위 안임

	if (m_bAttack)
	{
		m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		m_vLookDirectionXZ = XMVectorZero();
	}

	__super::Priority_Update(fTimeDelta);
}

void CBayar::Update(_float fTimeDelta)
{


	Update_Collider();
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
	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_PLAYER:
	{
		_vector vPos = pOther->Get_Transform()->Get_State(CTransform::STATE_POSITION);


		if (false == (m_bDetected = m_vecCollider[DETECTION]->Contains(vPos)))
		{
			m_pTarget = nullptr;
			return false;
		}
		else
		{
			m_pTarget = pOther;
			return true;
		}



	}
	case Client::LAYER_TERRAIN:
	case Client::LAYER_MONSTER:
	case Client::LAYER_INTERACTION:
	case Client::LAYER_UI:
	case Client::LAYER_CAMERA:
	case Client::LAYER_LAST:
	default:
		break;
	}
	return __super::Check_Collision(pOther);
}
void CBayar::Late_Update(_float fTimeDelta)
{

	if (m_pBody->Is_AnimEnd())
	{
		if (Is_LastAttackAnimation(m_pBody->Get_AnimIndex()))
		{
			if (++m_iCurAttack >= m_iAttackCount + AS_ATTACK_1_A)
				m_iCurAttack = AS_ATTACK_1_A;
			m_bAttack = false;

		}
		m_pAnimStateMachine->Trigger_ConditionVariable(ANIM_CONDITION::AC_ANIMEND);
	}

	if (Is_AttackCoolReady())
		m_fAttackTimeAcc = m_fAttackInterval;
	else
		m_fAttackTimeAcc += fTimeDelta;


	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

void CBayar::On_StateChange(_uint iState)
{

}

void CBayar::On_SubStateChange(_uint iSubState)
{
	m_pBody->Switch_Animation(iSubState);

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
		pCollider->Render();
	}
	return S_OK;
}



HRESULT CBayar::Ready_Parts()
{
	//Body
	CModelObject::MODELOBJ_DESC tModelDesc = {};
	tModelDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tModelDesc.fSpeedPerSec = 5.f;
	tModelDesc.eModelProtoLevelID = LEVEL_GAMEPLAY;
	strcpy_s(tModelDesc.strModelProtoName, "Bayar.model");
	m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBody);


	return S_OK;
}



HRESULT CBayar::Ready_FaceStateMachine()
{
	return S_OK;
}

HRESULT CBayar::Ready_Skill()
{
	return S_OK;
}




void CBayar::Use_Skill(CSkill* pSkill)
{
}

_bool CBayar::Is_LastAttackAnimation(_uint iAnimIdx)
{
	return (
		AS_ATTACK_2_A == iAnimIdx
		|| AS_ATTACK_2_B == iAnimIdx
		|| AS_ATTACK_2_C == iAnimIdx
		|| AS_ATTACK_2_D == iAnimIdx
		|| AS_ATTACK_3_E == iAnimIdx
		|| AS_ATTACK_1_F == iAnimIdx
		|| AS_ATTACK_4_G == iAnimIdx
		|| AS_ATTACK_3_H == iAnimIdx);
}

_bool CBayar::Is_FirstAttackAnimation(_uint iAnimIdx)
{
	return (
		AS_ATTACK_1_A == iAnimIdx
		|| AS_ATTACK_1_B == iAnimIdx
		|| AS_ATTACK_1_C == iAnimIdx
		|| AS_ATTACK_1_D == iAnimIdx
		|| AS_ATTACK_1_E == iAnimIdx
		|| AS_ATTACK_1_F == iAnimIdx
		|| AS_ATTACK_1_G == iAnimIdx
		|| AS_ATTACK_1_H == iAnimIdx);
}


CBayar* CBayar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBayar* pInstance = new CBayar(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CBayar::Clone(void* pArg)
{
	CBayar* pInstance = new CBayar(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBayar::Free()
{
	__super::Free();
	for (auto& pCollider : m_vecCollider)
	{
		Safe_Release(pCollider);
	}
}
