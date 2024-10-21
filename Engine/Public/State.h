#pragma once
#include "Base.h"
#include "Transition.h"

BEGIN(Engine)

class ENGINE_DLL CState :public CBase
{
protected:
	CState(_uint iStateID) :CBase(), m_iStateID(iStateID) {}
	CState(const CState& Prototype) : CBase(Prototype), m_iStateID(Prototype.m_iStateID), m_vecTransition(Prototype.m_vecTransition) {}

	virtual ~CState() = default;
public:

	bool CheckTransition(_uint* pOutNextState);

	void Add_Transition(CTransition* pTransition){m_vecTransition.push_back(pTransition);}
private:
	_uint m_iStateID;
	vector<CTransition*> m_vecTransition;

public:
	static CState* Create(_uint iStateID);
	virtual CBase* Clone();
	void Free() override;

};
END