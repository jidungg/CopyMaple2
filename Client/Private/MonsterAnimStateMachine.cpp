#include "stdafx.h"
#include "MonsterAnimStateMachine.h"

CMonsterAnimStateMachine::CMonsterAnimStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CStateMachine(pDevice, pContext)
{
}

CMonsterAnimStateMachine::CMonsterAnimStateMachine(const CMonsterAnimStateMachine& Prototype)
	: CStateMachine(Prototype)
{
}

HRESULT CMonsterAnimStateMachine::Initialize_Prototype()
{
	for (_uint i = M_BS_BORN; i < M_BS_LAST; i++)
	{
		Add_State(i);
	}
    return S_OK;
}

HRESULT CMonsterAnimStateMachine::Initialize(void* pArg)
{
	MONSTER_ANIM_STATEMACHINE_DESC* pDesc = (MONSTER_ANIM_STATEMACHINE_DESC*)pArg;
	m_mapAnimIdx = pDesc->mapAnimIdx;
	for (auto& vecAnimIdx : *m_mapAnimIdx)
	{
		for (auto& iAnimIdx : vecAnimIdx.second)
			Add_State(iAnimIdx);
	}

	Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_BORETRIGGER);
	Add_TriggerConditionVariable(MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_ISATTACK, pDesc->bAttack);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_DETECTED, pDesc->bDetected);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_MOVE, pDesc->bMove);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_WALK, pDesc->bWalk);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_STUN, pDesc->bStun);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_HP, pDesc->iHp);
	Add_ConditionVariable(MON_ANIM_CONDITION::AC_RANDOM, &m_iRandomCondition);

	CTransition* pTransition; Condition* pCondition;

	//MAIN TRAINSITINO
	//BORN_MAIN
	pTransition = Add_Transition(M_BS_BORN, M_BS_IDLE);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);

	//IDLE_MAIN
	pTransition = Add_Transition(M_BS_IDLE, M_BS_ATTACK);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = Add_Transition(M_BS_IDLE, M_BS_MOVE);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_Transition(M_BS_IDLE, M_BS_DAMG);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	pTransition = Add_Transition(M_BS_IDLE, M_BS_DEAD);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);
	pTransition = Add_Transition(M_BS_IDLE, M_BS_STUN);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_STUN, CONDITION_TYPE::EQUAL, true);

	//ATTACK_MAIN
	pTransition = Add_Transition(M_BS_ATTACK, M_BS_IDLE);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	pTransition = Add_Transition(M_BS_ATTACK, M_BS_MOVE);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_Transition(M_BS_ATTACK, M_BS_DEAD);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);

	//MOVE_MAIN
	pTransition = Add_Transition(M_BS_MOVE, M_BS_DAMG);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_DAMAGETRIGGER);
	pTransition = Add_Transition(M_BS_MOVE, M_BS_DEAD);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);
	pTransition = Add_Transition(M_BS_MOVE, M_BS_STUN);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_STUN, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_Transition(M_BS_MOVE, M_BS_ATTACK);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ATTACKTRIGGER);
	pTransition = Add_Transition(M_BS_MOVE, M_BS_IDLE);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_MOVE, CONDITION_TYPE::EQUAL, false);
	//DAMAGED_MAIN
	pTransition = Add_Transition(M_BS_DAMG, M_BS_IDLE);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	//STUN_MAIN
	pTransition = Add_Transition(M_BS_STUN, M_BS_IDLE);
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	pTransition = Add_Transition(M_BS_MOVE, M_BS_DEAD);
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_HP, CONDITION_TYPE::EQUAL_LESS, 0);

	//Sub Transition
	//BORN_SUB
	pTransition = Add_SubTransition(M_BS_BORN, (*m_mapAnimIdx)[M_AS_REGEN].front());
	//IDLE_SUB
	pTransition = Add_SubTransition(M_BS_IDLE, (*m_mapAnimIdx)[M_AS_IDLE].front());
	pTransition = Add_SubTransition(M_BS_IDLE, (*m_mapAnimIdx)[M_AS_ATTACK_IDLE].front());
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_SubTransition((*m_mapAnimIdx)[M_AS_IDLE].front(), (*m_mapAnimIdx)[M_AS_ATTACK_IDLE].front());
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_SubTransition((*m_mapAnimIdx)[M_AS_IDLE].front(), (*m_mapAnimIdx)[M_AS_BORE].front());
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_BORETRIGGER);
	pTransition = Add_SubTransition((*m_mapAnimIdx)[M_AS_BORE].front(), (*m_mapAnimIdx)[M_AS_IDLE].front());
	Bind_TriggerCondition(pTransition, MON_ANIM_CONDITION::AC_ANIMENDTRIGGER);
	pTransition = Add_SubTransition((*m_mapAnimIdx)[M_AS_BORE].front(), (*m_mapAnimIdx)[M_AS_ATTACK_IDLE].front());
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);

	//MOVE_SUB
	pTransition = Add_SubTransition(M_BS_MOVE, (*m_mapAnimIdx)[M_AS_RUN].front());
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, true);
	pTransition = Add_SubTransition(M_BS_MOVE, (*m_mapAnimIdx)[M_AS_WALK].front());
	Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_DETECTED, CONDITION_TYPE::EQUAL, false);
	//ATACK_SUB
	_int iAnimCount = (*m_mapAnimIdx)[M_AS_ATTACK].size();
	_int iInterval = 100 / iAnimCount;
	_int iStart = 0, iEnd = iInterval;
	for (_int i = 0; i < iAnimCount; i++)
	{
		pTransition = Add_SubTransition(M_BS_ATTACK, (*m_mapAnimIdx)[M_AS_ATTACK].at(i));
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, iStart);
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, iEnd);
		iStart += iInterval;
		iEnd += iInterval;
	}
	//DAMG_SUB
	iAnimCount = (*m_mapAnimIdx)[M_AS_DAMG].size();
	iInterval = 100 / iAnimCount;
	iStart = 0, iEnd = iInterval;
	for (_int i = 0; i < iAnimCount; i++)
	{
		pTransition = Add_SubTransition(M_BS_DAMG, (*m_mapAnimIdx)[M_AS_DAMG].at(i));
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, iStart);
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, iEnd);
		iStart += iInterval;
		iEnd += iInterval;
	}
	//DEAD_SUB
	iAnimCount = (*m_mapAnimIdx)[M_AS_DEAD].size();
	iInterval = 100 / iAnimCount;
	iStart = 0, iEnd = iInterval ;
	for (_int i = 0; i < iAnimCount; i++)
	{
		pTransition = Add_SubTransition(M_BS_DEAD, (*m_mapAnimIdx)[M_AS_DEAD].at(i));
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::EQUAL_GREATER, iStart);
		Bind_Condition(pTransition, MON_ANIM_CONDITION::AC_RANDOM, CONDITION_TYPE::LESS, iEnd);
		iStart += iInterval;
		iEnd += iInterval;
	}
	//STUN_SUB
	pTransition = Add_SubTransition(M_BS_STUN, (*m_mapAnimIdx)[M_AS_STUN].front());

	Set_CurrentState(M_BS_BORN);
	return S_OK;
}

void CMonsterAnimStateMachine::Late_Update(_float fTimeDelta)
{
	m_iRandomCondition = rand() % 100;
	__super::Late_Update(fTimeDelta);
}

CMonsterAnimStateMachine* CMonsterAnimStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterAnimStateMachine* pInstance = new CMonsterAnimStateMachine(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to create CMonsterAnimStateMachine");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CMonsterAnimStateMachine::Clone(void* pArg)
{
	CMonsterAnimStateMachine* pInstance = new CMonsterAnimStateMachine(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

