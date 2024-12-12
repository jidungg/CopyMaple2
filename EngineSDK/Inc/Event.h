#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CEvent : public CBase
{
protected:
	explicit CEvent() {}
	virtual ~CEvent() {}
public:
	_uint GetEventID() const { return m_eEventID; }
	virtual void Exec() abstract;
protected:
	_uint m_eEventID = 0;
};
//
//class CGameObject;
//class CLayer;

//
//

//
//
//class ENGINE_DLL CChangeSceneEvent : public CEvent
//{
//private:
//	explicit CChangeSceneEvent(CScene* _pScene);
//	virtual ~CChangeSceneEvent() {}
//
//public:
//	static CChangeSceneEvent* Create(CScene* _pScene);
//
//	CScene* Get_Scene() { return m_pScene; }
//	void Exec() override;
//private:
//	 CScene* m_pScene;
//private:
//	void Free() override;
//
//};




END