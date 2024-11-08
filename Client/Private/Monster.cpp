#include "stdafx.h"
#include "Monster.h"
#include "GameInstance.h"
#include "Collider_Sphere.h"
#include "ModelObject.h"
#include "MonsterDataBase.h"
#include "MonsterAnimStateMachine.h"
#include "StateMachine.h"
#include "Skill.h"
#include "SkillManager.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CCharacter(Prototype)
{
}


HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	MONSTER_DESC* pDesc = (MONSTER_DESC*)pArg;
	m_pMonData = MONSTERDB->Get_Data(pDesc->eMonID);
	pDesc->fBodyCollisionOffset = m_pMonData->fBodyCollisionOffset;
	pDesc->fBodyCollisionRadius = m_pMonData->fBodyCollisionRadius;
	pDesc->fRotationPerSec = XMConvertToRadians(90.f);
	pDesc->fSpeedPerSec = 2.0f;
	pDesc->iColliderCount = COLLIDER_LAST;
	pDesc->iBodyColliderIndex = COLLIDER_BODY;
	m_tStat = m_pMonData->tStat;
	m_fDetectionRange = m_pMonData->fDetectionRange;
	m_fChaseRange = m_pMonData->fChaseRange;
	m_mapAnimIdx = m_pMonData->mapAnimIdx;
	m_vHomePos = pDesc->vHomePos;
	for (auto& eSkillID : m_pMonData->vecSkillID)
		m_mapSkill[eSkillID] = CSkill::Create(SKILLDB->Get_SkillData(eSkillID), this);
	m_iCurrentSkillID = (_int)m_mapSkill.begin()->first;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Parts(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;

	Set_Position(m_vHomePos);
	return S_OK;
}

HRESULT CMonster::Ready_Components(MONSTER_DESC* pDesc)
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
	tDesc.vCenter = { 0,0,0 };
	tDesc.fRadius = m_fDetectionRange;
	m_vecCollider[COLLIDER_DETECT] = static_cast<CCollider_Sphere*>(m_pGameInstance->Clone_Proto_Component_Stock(CCollider_Sphere::m_szProtoTag, &tDesc));
	Add_Component(m_vecCollider[COLLIDER_DETECT], L"Com_Collider_Detection");

	return S_OK;
}

HRESULT CMonster::Ready_Parts(MONSTER_DESC* pDesc)
{
	//Body
	CModelObject::MODELOBJ_DESC tModelDesc = {};
	tModelDesc.fRotationPerSec = XMConvertToRadians(90.f);
	tModelDesc.fSpeedPerSec = 5.f;
	tModelDesc.eModelProtoLevelID = LEVEL_GAMEPLAY;
	strcpy_s(tModelDesc.strModelProtoName, m_pMonData->strModelTag);
	m_pBody = static_cast<CModelObject*>(m_pGameInstance->Clone_Proto_Object_Stock(CModelObject::m_szProtoTag, &tModelDesc));
	Add_Child(m_pBody);
	return S_OK;
}
HRESULT CMonster::Ready_AnimStateMachine()
{
	m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CMonster::On_StateChange, this, placeholders::_1));
	m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CMonster::On_SubStateChange, this, placeholders::_1));
	m_pBody->Register_OnAnimEndCallBack(bind(&CMonster::On_AnimEnd, this, placeholders::_1));

	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_WALK].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_RUN].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_IDLE].front(), true);
	m_pBody->Set_AnimationLoop(m_mapAnimIdx[M_AS_ATTACK_IDLE].front(), true);

	for (auto& i : m_mapAnimIdx[M_AS_STUN])
		m_pBody->Set_AnimPostDelayPercent(i, 1);
	for (auto& i : m_mapAnimIdx[M_AS_DEAD])
		m_pBody->Set_AnimPostDelayPercent(i, 1);

	for (_uint i = M_BS_BORN; i < M_BS_LAST; i++)
	{
		m_pAnimStateMachine->Add_State(i);
	}

	for (auto& vecAnimIdx : m_mapAnimIdx)
	{
		for (auto& iAnimIdx : vecAnimIdx.second)
			m_pAnimStateMachine->Add_State(iAnimIdx);
	}
	for (auto& pSkill : m_mapSkill)
	{
		vector<_uint> vAnimIndcies = pSkill.second->Get_AnimIdcies();
		for (auto& iAnimIdx : vAnimIndcies)
			m_pAnimStateMachine->Add_State(iAnimIdx);
	}
	//m_pAnimStateMachine->Register_OnStateChangeCallBack(bind(&CMonster::On_StateChange, this, placeholders::_1));
	//m_pAnimStateMachine->Register_OnSubStateChangeCallBack(bind(&CMonster::On_SubStateChange, this, placeholders::_1));

	m_pAnimStateMachine->Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	m_pAnimStateMachine->Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	m_pAnimStateMachine->Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_BORETRIGGER);
	m_pAnimStateMachine->Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_ISATTACK, &m_bAttack);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_DETECTED, &m_bDetected);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_MOVE, &m_bMove);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_WALK, &m_bWalk);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_STUN, &m_bStun);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_HP, &m_tStat.iHP);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_SKILL_ID, &m_iCurrentSkillID);
	m_pAnimStateMachine->Add_ConditionVariable(MON_ANIM_CONDITION::AC_RANDOM, &m_iRandomCondition);

	CTransition* pTransition; Condition* pCondition;
	//MAIN TRAINSITINO
	//BORN_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_BORN, M_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);

	//IDLE_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_IDLE, M_BS_ATTACK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_IDLE, M_BS_MOVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_IDLE, M_BS_DAMG);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_IDLE, M_BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_IDLE, M_BS_STUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_STUN, CONDITION_TYPE::EQUAL, true);

	//ATTACK_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_ATTACK, M_BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_ATTACK, M_BS_MOVE);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_ISATTACK, CONDITION_TYPE::EQUAL, false);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_ATTACK, M_BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);

	//MOVE_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_DAMG);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_STUN);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_STUN, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_ATTACK);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	//DAMAGED_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_DAMG, M_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	//STUN_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_STUN, M_BS_IDLE);
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_MOVE, M_BS_DEAD);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);

	//Sub Transition
	//BORN_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_BORN, m_mapAnimIdx[M_AS_REGEN].front());
	//IDLE_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_IDLE, m_mapAnimIdx[M_AS_IDLE].front());
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_IDLE, m_mapAnimIdx[M_AS_ATTACK_IDLE].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(m_mapAnimIdx[M_AS_IDLE].front(), m_mapAnimIdx[M_AS_ATTACK_IDLE].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(m_mapAnimIdx[M_AS_IDLE].front(), m_mapAnimIdx[M_AS_BORE].front());
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_BORETRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(m_mapAnimIdx[M_AS_BORE].front(), m_mapAnimIdx[M_AS_IDLE].front());
	m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	pTransition = m_pAnimStateMachine->Add_SubTransition(m_mapAnimIdx[M_AS_BORE].front(), m_mapAnimIdx[M_AS_ATTACK_IDLE].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);

	//MOVE_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_MOVE, m_mapAnimIdx[M_AS_RUN].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_MOVE, m_mapAnimIdx[M_AS_WALK].front());
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, false);
	//ATACK_SUB
	for (auto& pSkill : m_mapSkill)
	{
		vector<_uint> vAnimIndcies = pSkill.second->Get_AnimIdcies();
		pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_ATTACK, vAnimIndcies.front());
		m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL,(_int) pSkill.first);

		_uint iCount = vAnimIndcies.size();
		for (_uint idx = 0; idx < iCount - 1; idx++)
		{
			pTransition = m_pAnimStateMachine->Add_SubTransition(vAnimIndcies[idx], vAnimIndcies[idx + 1]);
			m_pAnimStateMachine->Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
			m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_SKILL_ID, CONDITION_TYPE::EQUAL,(_int) pSkill.first);
		}
	}

	//DAMG_SUB
	_int iAnimCount = m_mapAnimIdx[M_AS_DAMG].size();
	_int iInterval = 100 / iAnimCount;
	_int iStart = 0, iEnd = iInterval;
	for (_int i = 0; i < iAnimCount; i++)
	{
		pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_DAMG, m_mapAnimIdx[M_AS_DAMG].at(i));
		m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, iStart);
		m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, iEnd);
		iStart += iInterval;
		iEnd += iInterval;
	}
	//DEAD_SUB
	iAnimCount = m_mapAnimIdx[M_AS_DEAD].size();
	iInterval = 100 / iAnimCount;
	iStart = 0, iEnd = iInterval;
	for (_int i = 0; i < iAnimCount; i++)
	{
		pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_DEAD, m_mapAnimIdx[M_AS_DEAD].at(i));
		m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, iStart);
		m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, iEnd);
		iStart += iInterval;
		iEnd += iInterval;
	}
	//STUN_SUB
	pTransition = m_pAnimStateMachine->Add_SubTransition(M_BS_STUN, m_mapAnimIdx[M_AS_STUN].front());

	m_pAnimStateMachine->Set_CurrentState(M_BS_BORN);

	m_pBody->Set_Animation((_uint)m_mapAnimIdx[M_AS_REGEN].front());
	return S_OK;
}

void CMonster::To_NextSkill()
{
	auto iter = m_mapSkill.find((SKILL_ID)m_iCurrentSkillID);
	iter++;
	if (iter == m_mapSkill.end())
		iter = m_mapSkill.begin();
	m_iCurrentSkillID = (_int)iter->first;
}

_bool CMonster::Use_Skill(CSkill* pSkill)
{
	if (pSkill == nullptr)
		return false;
	if (false == Get_CurrentSkill()->Is_CastingComplete())
		return false;
	if (false == m_pBody->Is_AnimPostDelayEnd())
		return false;
	auto pDesc = pSkill->Get_SkillDesc();
	if (pDesc->eCastingType == SKILL_TYPE::CASTING && false == m_bOnFloor)
		return false;
	m_iCurrentSkillID = (_int)pDesc->eID;
	m_bChase = false;
	m_bAttack = true;
	m_fAttackTimeAcc = 0.f;
	m_pAnimStateMachine->Trigger_ConditionVariable(AC_ATTACKTRIGGER);
	m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
	return true;
}

void CMonster::On_CastingEnd(CSkill* pSkill)
{
}

void CMonster::Priority_Update(_float fTimeDelta)
{

	_vector vMyPos = Get_Position();
	_vector vDestination;
	_float fMoveSpeed = m_tStat.fRunSpeed;
	_float fHomeDistance;
	XMStoreFloat(&fHomeDistance, XMVector3Length(vMyPos - m_vHomePos));
	if (m_bBackToHome)
	{
		if (fHomeDistance <= 0.1f)
		{
			m_bBackToHome = false;
			m_bDetected = false;
			m_pTarget = nullptr;
			m_fTargetDistance = FLT_MAX;
			return;
		}
		fMoveSpeed = m_tStat.fRunSpeed *2;
		vDestination = m_vHomePos;
	}
	else if (fHomeDistance > m_fHomeRange)
	{
		m_pTarget = nullptr;
		m_bDetected = false;
		m_bBackToHome = true;
		m_fTargetDistance = FLT_MAX;
		vDestination = m_vHomePos;
	}
	else if(nullptr == m_pTarget || false == m_pTarget->Is_Active() || m_pTarget->Is_Dead())
	{
		m_pTarget = nullptr;
		m_bDetected = false;
		m_fTargetDistance = FLT_MAX;
		return;
	}
	else
		vDestination = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);


	m_vMoveDirectionXZ = vDestination - vMyPos;
	m_vMoveDirectionXZ = XMVectorSetY(m_vMoveDirectionXZ, 0);
	XMStoreFloat(&m_fTargetDistance, XMVector3Length(m_vMoveDirectionXZ));
	m_vMoveDirectionXZ = XMVector3Normalize(m_vMoveDirectionXZ);
	m_vLookDirectionXZ = m_vMoveDirectionXZ;
	m_vMoveDirectionXZ *= fMoveSpeed * fTimeDelta;

	if (m_bBackToHome)
		return;
	//공격 쿨이 다 됐으면 공격 범위 안까지 추적
	//공격 쿨이 안됐으면
	//추적 상태일 경우 공격범위 안까지 추적
	//추적 상태가 아닐 경우 추적범위 밖에 있지 않으면 안추적
	if (Is_AttackCoolReady())
	{

		if (m_fTargetDistance <= m_tStat.fAttackRange)
		{
			Use_Skill(Get_CurrentSkill());
		}

	}
	//공격 쿨임 : 추적상태면 쫒고 아니면 안쫒기
	else
	{
		// 추적상태가 아니다 -> 공격범위에 들어온 뒤로 추적범위 밖으로 나가지 않았음.
		if (false ==  m_bChase) 
		{
			if(m_fTargetDistance > m_fChaseRange)//추적 범위 밖으로 나가면 추적
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
	
	if (m_bAttack)
	{
		m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		m_vLookDirectionXZ = XMVectorZero();
	}


	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

_bool CMonster::Check_Collision(CGameObject* pOther)
{


	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_PLAYER:
	{
		_vector vPos = pOther->Get_Transform()->Get_State(CTransform::STATE_POSITION);


		if (m_vecCollider[COLLIDER_DETECT]->Contains(vPos))
		{
			m_pTarget = pOther;
			m_bDetected = true;
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

void CMonster::Late_Update(_float fTimeDelta)
{


	if (Is_AttackCoolReady())
		m_fAttackTimeAcc = m_tStat.fAttackInterval;
	else
		m_fAttackTimeAcc += fTimeDelta;


	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

void CMonster::On_StateChange(_uint iState)
{
	cout << "Monster State Changed : " << iState << endl;
}

void CMonster::On_SubStateChange(_uint iSubState)
{
	cout << "Monster SubState Changed : " << iSubState << endl;
	m_pBody->Switch_Animation(iSubState);
}

void CMonster::On_AnimEnd(_uint iAnimIdx)
{

	m_pAnimStateMachine->Trigger_ConditionVariable(AC_ANIMENDTRIGGER);
	if (m_bAttack)
	{
		_int iNextAnim = m_mapSkill[(SKILL_ID)m_iCurrentSkillID]->Get_NextAnimation(iAnimIdx);
		if (iNextAnim == -1)//스킬 종료
		{
			m_bAttack = false;
			To_NextSkill();
		}
	}
}


CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();
}
