#pragma once
#include "Event.h"

BEGIN(Client)
class CGameObject;
class CLayer;
class CCreateObjEvent : public CEvent
{
private:
	explicit CCreateObjEvent(CLayer* _pLayer,  const _tchar* pObjTag, CGameObject* _pNewObj);
	virtual ~CCreateObjEvent() {}

public:
	static CCreateObjEvent* Create(CLayer* _pLayer, const _tchar* pObjTag, CGameObject* _pNewObj);
	void Exec() override;


private:
	CLayer* m_pLayer;
	const _tchar* m_pObjTag;
	CGameObject* m_pNewObj;
private:
	void Free() override;

};
END