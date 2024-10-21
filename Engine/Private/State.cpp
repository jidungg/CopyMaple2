#include "State.h"
#include "Transition.h"

bool CState::CheckTransition(_uint* pOutNextState)
{
	for (auto& trans : m_vecTransition)
	{
		if (trans->CheckConditions())
		{
			*pOutNextState = trans->Get_NextState();
			return true;
		}
	}
	return false;
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
}
