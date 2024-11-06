#include "State.h"
#include "Transition.h"

bool CState::Check_Transition(_uint* pOutNextState)
{
	CTransition* priorityTransition = nullptr;
	_uint iMaxConditionCount = 0;
	_uint iTmpConditionCount = 999;
	for (auto& trans : m_vecTransition)
	{
		if (trans->CheckConditions())
		{
			iTmpConditionCount = trans->Get_ConditionCount();
			if (iMaxConditionCount < iTmpConditionCount)
			{
				iMaxConditionCount = iTmpConditionCount;
				priorityTransition = trans;
			}
		}
	}
	if (priorityTransition == nullptr)
		return false;

	*pOutNextState = priorityTransition->Get_NextState();
	return true;
}

_bool CState::Check_SubTransition(_uint* pOutNextSubState)
{
	CTransition* priorityTransition = nullptr;
	_uint iMaxConditionCount = 0;
	_uint iTmpConditionCount = 999;

	for (auto& subTrans : m_mapSubTransition[*pOutNextSubState])
	{
		if (subTrans->CheckConditions())
		{
			iTmpConditionCount = subTrans->Get_ConditionCount();
			if (iMaxConditionCount <= iTmpConditionCount)
			{
				iMaxConditionCount = iTmpConditionCount;
				priorityTransition = subTrans;
			}
		}
	}
	if (priorityTransition == nullptr)
		return false;
	else
	{
		*pOutNextSubState = priorityTransition->Get_NextState();
		return true;
	}
}

CState* CState::Create(_uint iStateID)
{
	CState* pInstance = new CState(iStateID);
	return pInstance;
}

CBase* CState::Clone()
{
	return new CState(*this);
}

void CState::Free()
{
	__super::Free();
	for (auto& trans : m_vecTransition)
	{
		Safe_Release(trans);
	}
	for (auto& vecSubTrans : m_mapSubTransition)
	{
		for (auto& subTrans : vecSubTrans.second)
		{

			Safe_Release(subTrans);
		}
	}
}
