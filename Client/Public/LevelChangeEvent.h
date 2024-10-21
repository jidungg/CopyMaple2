#pragma once
#include "Event.h"

BEGIN(Client)
class CGameObject;
class CDeadObjEvent : public CEvent
{
private:
	explicit CDeadObjEvent(CGameObject* _pDeadObj);
	virtual ~CDeadObjEvent() {}

public:
	static CDeadObjEvent* Create(CGameObject* _pDeadObj);
	void Exec() override;
	CGameObject* Get_DeadObj() { return m_pDeadObj; }
private:
	CGameObject* m_pDeadObj;
private:
	void Free() override;

};
END