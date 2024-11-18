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
#include "DeadObjEvent.h"
#include "Client_Utility.h"
#include "WayFinder.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
	m_eTeam = TEAM::MONSTER;
}

CMonster::CMonster(const CMonster& Prototype)
	: CCharacter(Prototype)
{
	m_eTeam = TEAM::MONSTER;
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
	m_tStatDefault = m_pMonData->tStat;
	m_tStat = m_tStatDefault;
	m_fDetectionRange = m_pMonData->fDetectionRange;
	m_fChaseRange = m_pMonData->fChaseRange;
	m_mapAnimIdx = m_pMonData->mapAnimIdx;
	m_vHomePos = pDesc->vHomePos; 

	m_fRandomMoveTime = m_pGameInstance->Get_RandomFloat(5,10);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;
	if (FAILED(Ready_Parts(pDesc)))
		return E_FAIL;
	//ReadySkill
	CSkillDataBase* pSkillManager = SKILLDB;
	for (auto& eSkillID : m_pMonData->vecSkillID)
	{
		m_mapSkill[eSkillID] = CSkill::Create(pSkillManager->Get_SkillData(eSkillID), this);
		m_mapSkill[eSkillID]->Register_AnimEvent(m_pBody);
	}
	m_iCurrentSkillID = (_int)m_mapSkill.begin()->first;

	if (FAILED(Ready_AnimStateMachine()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_pMonData->vScale.x, m_pMonData->vScale.y, m_pMonData->vScale.z);
	Set_Position(m_vHomePos);

	CWayFinder::WAYFINDER_DESC tDesc;
	tDesc.pCubeTerrain = pDesc->pCubeTerrain;
	m_pWayFinder = CWayFinder::Create(m_pDevice, m_pContext);
	m_pWayFinder->Initialize(&tDesc);
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
	//몬스터가 생성될 레벨을 가져와야 하느ㅏㄴ데,
	// Create는 LoadingLevel에서 하기 때문에 CurrentLevel 하면 안됨.
	tModelDesc.eModelProtoLevelID = Get_CurrentTrueLevel();
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
	//DEAD_MAIN
	pTransition = m_pAnimStateMachine->Add_Transition(M_BS_DEAD, M_BS_IDLE);
	m_pAnimStateMachine->Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::GREATER, 0);
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
	if (pDesc->eCastingType == SKILL_CASTING_TYPE::CASTING && false == m_bOnFloor)
		return false;
	m_iCurrentSkillID = (_int)pDesc->eID;
	m_bChase = false;
	m_bAttack = true;
	m_fAttackTimeAcc = 0.f;
	m_pAnimStateMachine->Trigger_ConditionVariable(AC_ATTACKTRIGGER);
	return true;
}

void CMonster::Respawn()
{
	__super::Respawn();
	m_pAnimStateMachine->Set_CurrentState(M_BS_BORN);
}

_bool CMonster::Is_Targetable()
{
	if (false == Is_Valid())
		return false;
	if (m_pMonData->eMonGrade != MONSTER_GRADE::BOSS)
	{
		if (m_bHPZero)
			return false;
	}

	return true;
}

void CMonster::On_CastingEnd(CSkill* pSkill)
{
}

void CMonster::Priority_Update(_float fTimeDelta)
{
		//todo : 공격 시에 목표를 바라보지 않음, 집에 갈 때 뒷걸음으로 감
	if (m_bHPZero)
		return;
	_vector vMyPos = Get_Position();
	_float fMoveSpeed = m_tStat.fRunSpeed;
	_float fHomeDistance;
	_vector vDestination = vMyPos;	
	CGameObject* pTarget = Get_Target();
	XMStoreFloat(&fHomeDistance, XMVector3Length(vMyPos - m_vHomePos));
	//집 가는 중
	if (m_bBackToHome)
	{
		if (fHomeDistance <= 0.1f)
		{
			m_bBackToHome = false;
			m_bDetected = false;
			Set_Target(nullptr);
			m_fTargetDistance = FLT_MAX;
			return;
		}
		if (XMVector4Length(m_vNextStation - vMyPos).m128_f32[0] <= 0.05f)
		{
			if (m_pWayFinder->FindWay(vMyPos, m_vHomePos, m_iSearchRange))
				m_vNextStation = m_pWayFinder->Get_NextStation();
		}
		fMoveSpeed = m_tStat.fRunSpeed * 2;
		vDestination = m_vNextStation;
	}
	//집에서 너무 멀리 떨어짐
	else if (fHomeDistance > m_fHomeRange)
	{
		Set_Target(nullptr);
		m_bDetected = false;
		m_bBackToHome = true;
		m_fTargetDistance = FLT_MAX;
		if (false == m_pWayFinder->FindWay(vMyPos, m_vHomePos, 999))
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHomePos);
		else
			m_vNextStation = m_pWayFinder->Get_NextStation();

		Restore_HP();
	}
	//타겟이 없음 -> 랜덤움직임
	else if (nullptr == pTarget || false == pTarget->Is_Valid())
	{
		Set_Target(nullptr);
		m_bDetected = false;
		m_fTargetDistance = FLT_MAX;

		if (m_fRandomMoveTime <= m_fRandomMoveTimeAcc)
		{
			m_bRandomMove = !m_bRandomMove;
			m_fRandomMoveTime = m_pGameInstance->Get_RandomFloat(5, 10);
			m_fRandomMoveTimeAcc = 0.f;
			if (m_bRandomMove)
			{
				_float fX = m_pGameInstance->Get_RandomFloat(-2, 2);
				_float fZ = m_pGameInstance->Get_RandomFloat(-2, 2);
				m_vRandomHomePosition = { fX ,0, fZ ,1 };
			}
		}
		m_fRandomMoveTimeAcc += fTimeDelta;
		vDestination = m_vHomePos + m_vRandomHomePosition;
		if (XMVectorGetX(XMVector3Length(vDestination - vMyPos)) <= 0.1f)
			vDestination = vMyPos;
		fMoveSpeed = m_tStat.fWalkSpeed;
		m_bWalk = true;
	}
	//타겟이 있음.
	else
	{
		vDestination = pTarget->Get_Position();
		//새로운 칸에 도착할 때마다 새로운 길을 찾음
		if (XMVector4Length(m_vNextStation - vMyPos).m128_f32[0] <= 0.05f)
		{
			if (m_pWayFinder->FindWay(vMyPos, vDestination, m_iSearchRange))
				m_vNextStation = m_pWayFinder->Get_NextStation();
		}
		XMStoreFloat(&m_fTargetDistance, XMVector3Length(vDestination - vMyPos));
		vDestination = m_vNextStation;
	}



	if (false == m_bAttack)
	{
		m_vMoveDirectionXZ = vDestination - vMyPos;
		if (m_vMoveDirectionXZ.m128_f32[1] > 0 && 0 == m_fUpForce)
		{
			m_bOnFloor = false;
			m_fUpForce += m_tStat.fJumpPower;
		}

		m_vMoveDirectionXZ = XMVectorSetY(m_vMoveDirectionXZ, 0);
		m_vMoveDirectionXZ = XMVectorSetW(m_vMoveDirectionXZ, 0);
		m_vMoveDirectionXZ = XMVector3Normalize(m_vMoveDirectionXZ);

		m_vMoveDirectionXZ *= fMoveSpeed * fTimeDelta;
	}
	else
	{
		m_vMoveDirectionXZ = XMVectorZero();
		
	}

	if (m_bBackToHome)
		return;
	if (nullptr == pTarget)
		return;
	m_bWalk = false;
	//공격 쿨이 다 됐으면 공격 범위 안까지 추적
	//공격 쿨이 안됐으면
	//추적 상태일 경우 공격범위 안까지 추적
	//추적 상태가 아닐 경우 추적범위 밖에 있지 않으면 안추적
	if (Is_AttackCoolReady())
	{

		if (m_fTargetDistance <= m_tStat.fAttackRange)
		{
			Use_Skill(Get_CurrentSkill());
			m_vLookDirectionXZ = XMVector4Normalize(Get_Target()->Get_Position() - vMyPos);
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
		{
			m_vMoveDirectionXZ = XMVectorZero(); // 멈춤
		}
	}

	if ( false == XMVector3Equal(m_vMoveDirectionXZ, XMVectorZero()))
		m_vLookDirectionXZ = XMVector4Normalize(m_vMoveDirectionXZ);
	__super::Priority_Update(fTimeDelta);
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bHPZero)
	{
		if (m_fDeadIdleTime <= m_fDeadIdleTimeAcc)
		{
			m_pGameInstance->Push_Event(CDeadObjEvent::Create(this));

			m_fDeadIdleTimeAcc = 0.f;
		}
		else
			m_fDeadIdleTimeAcc += fTimeDelta;
	}
}

_bool CMonster::Check_Collision(CGameObject* pOther)
{
	LAYERID eLayerID = (LAYERID)pOther->Get_LayerID();
	switch (eLayerID)
	{
	case Client::LAYER_PLAYER:
	{
		if (m_bHPZero)
			break;
		_vector vPos = pOther->Get_Transform()->Get_State(CTransform::STATE_POSITION);
		if (m_vecCollider[COLLIDER_DETECT]->Contains(vPos))
		{
			if (Get_Target() == nullptr)
			{
				Set_Target(pOther);
				if (m_pWayFinder->FindWay(Get_Position(), vPos, m_iSearchRange))
					m_vNextStation = m_pWayFinder->Get_NextStation();
			}
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
	//cout << "Monster State Changed : " << iState << endl;
}

void CMonster::On_SubStateChange(_uint iSubState)
{
	//cout << "Monster SubState Changed : " << iSubState << endl;
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
	Safe_Release(m_pWayFinder);
}

void CMonster::On_HPZero()
{
	m_bHPZero = true;

	Set_Target(nullptr);
}
