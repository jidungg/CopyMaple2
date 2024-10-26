#include "StateMachine.h"
#include "JsonParser.h"

CStateMachine::CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CStateMachine::CStateMachine(const CStateMachine& Prototype)
	: CComponent(Prototype)
{
}

HRESULT CStateMachine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStateMachine::Initialize_Prototype(const _char* strJsonFilePath)
{
	json j;
	CJsonParser::ReadJsonFile(strJsonFilePath, &j);

	return S_OK;
}

HRESULT CStateMachine::Initialize(void* pArg)
{
	return S_OK;
}

void CStateMachine::Update(_float fTimeDelta)
{

	_uint iNewSubState = m_iCurrentSubState;
	if(m_mapStates[m_iCurrentState]->Check_Transition(&m_iCurrentState))
	{
		for (auto& callback : m_listStateChangeCallback)
			callback(m_iCurrentState);
		m_iCurrentSubState = m_mapStates[m_iCurrentState]->Check_SubTransition();
		for (auto& callback : m_listSubStateChangeCallback)
			callback(m_iCurrentSubState);
	}
	else 
	{
		iNewSubState = m_mapStates[m_iCurrentState]->Check_SubTransition();
		if (iNewSubState != m_iCurrentSubState)
		{
			m_iCurrentSubState = iNewSubState;
			for (auto& callback : m_listSubStateChangeCallback)
				callback(m_iCurrentSubState);
		}
	}
	

	for (auto& trigger : m_mapTriggerConditionVariable)
	{
		trigger.second->bValue = false;
	} 
}



void CStateMachine::Add_ConditionVariable(_uint iConditionID, float* pfValue)
{
	FloatConditionVariable* pConditionVar = new FloatConditionVariable;
	pConditionVar->pFloatValue = pfValue;
	m_mapConditionVariable.emplace(iConditionID, pConditionVar);
}

void CStateMachine::Add_ConditionVariable(_uint iConditionID, int* piValue)
{
	IntConditionVariable* pConditionVar = new IntConditionVariable;
	pConditionVar->pIntValue = piValue;
	m_mapConditionVariable.emplace(iConditionID, pConditionVar);
}

void CStateMachine::Add_ConditionVariable(_uint iConditionID, bool* pbValue)
{
	BoolConditionVariable* pConditionVar = new BoolConditionVariable;
	pConditionVar->pBoolValue = pbValue;
	m_mapConditionVariable.emplace(iConditionID, pConditionVar);
}

void CStateMachine::Add_TriggerConditionVariable(_uint iConditionID)
{
	TriggerConditionVariable* pConditionVar = new TriggerConditionVariable;
	m_mapTriggerConditionVariable.emplace(iConditionID, pConditionVar);
}



CState* CStateMachine::Add_State(_uint iStateID)
{
	CState* pState = CState::Create(iStateID);
	m_mapStates.emplace(iStateID, pState);
	return pState;
}

CTransition* CStateMachine::Add_Transition(_uint iStateID, _uint iNextState)
{
	CTransition* pTransition = CTransition::Create(iNextState);
	m_mapStates[iStateID]->Add_Transition(pTransition);
	return pTransition;
}

CTransition* CStateMachine::Add_SubTransition(_uint iStateID, _uint iNextState)
{
	CTransition* pTransition = CTransition::Create(iNextState);
	m_mapStates[iStateID]->Add_SubTransition(pTransition);
	return pTransition;
}

void CStateMachine::Trigger_ConditionVariable(_uint iConditionVarID) 
{
	static_cast<TriggerConditionVariable*>( m_mapTriggerConditionVariable[iConditionVarID])->bValue= true;
}


void CStateMachine::Remove_Condition(_uint iConditionID)
{
	m_mapConditionVariable.erase(iConditionID);
}

void CStateMachine::Set_CurrentState(_uint iState)
{
	m_iCurrentState = iState;
	m_iCurrentSubState =  m_mapStates[m_iCurrentState]->Check_SubTransition();
}



void CStateMachine::Bind_Condition(CTransition* pTransition, _uint iConditionVarID, CONDITION_TYPE eCondType, int iValue)
{
	pTransition->Add_Condition(IntCondition::Create(eCondType, iValue, m_mapConditionVariable[iConditionVarID]));
}

void CStateMachine::Bind_Condition(CTransition* pTransition, _uint iConditionVarID, CONDITION_TYPE eCondTyp, float fValue)
{
	pTransition->Add_Condition(FloatCondition::Create(eCondTyp, fValue, m_mapConditionVariable[iConditionVarID]));

}

void CStateMachine::Bind_Condition(CTransition* pTransition, _uint iConditionVarID, CONDITION_TYPE eCondTyp, bool bValue)
{
	pTransition->Add_Condition(BoolCondition::Create(eCondTyp, bValue, m_mapConditionVariable[iConditionVarID]));
}

void CStateMachine::Bind_TriggerCondition(CTransition* pTransition, _uint iConditionVarID)
{
	pTransition->Add_Condition(TriggerCondition::Create(m_mapTriggerConditionVariable[iConditionVarID]));
}



CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* strJsonFilePath)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype(strJsonFilePath)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CComponent* CStateMachine::Clone(void* pArg)
{
	CStateMachine* pInstance = new CStateMachine(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CStateMachine::Free()
{
	__super::Free();
	for (auto& state : m_mapStates)
	{
		Safe_Release(state.second);
	}
	for (auto& cond : m_mapConditionVariable)
	{
		delete (cond.second);
	}
	for (auto& trigger : m_mapTriggerConditionVariable)
	{
		delete (trigger.second);
	}
}