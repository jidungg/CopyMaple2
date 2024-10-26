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

	bool Check_Transition(_uint* pOutNextState);
	_uint Check_SubTransition();

	void Add_Transition(CTransition* pTransition){m_vecTransition.push_back(pTransition);}
	void Add_SubTransition(CTransition* pTransition) { m_vecSubTransition.push_back(pTransition); }
private:
	_uint m_iStateID;
	vector<CTransition*> m_vecTransition;
	vector<CTransition*> m_vecSubTransition;
public:
	static CState* Create(_uint iStateID);
	virtual CBase* Clone();
	void Free() override;

};
END