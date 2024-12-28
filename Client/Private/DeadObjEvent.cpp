#include "stdafx.h"
#include "DeadObjEvent.h"
#include "GameObject.h"


CDeadObjEvent::CDeadObjEvent(CGameObject* _pDeadObj)
	:m_pDeadObj(_pDeadObj)
{
	Safe_AddRef(m_pDeadObj);
	m_eEventID = (_uint)EVENT_ID::DEAD_OBJ;
}

CDeadObjEvent* CDeadObjEvent::Create(CGameObject* _pDeadObj)
{
	return  new CDeadObjEvent(_pDeadObj);
}

void CDeadObjEvent::Exec()
{

	m_pDeadObj->Set_Dead();
}

void CDeadObjEvent::Free()
{
	Safe_Release(m_pDeadObj);
}