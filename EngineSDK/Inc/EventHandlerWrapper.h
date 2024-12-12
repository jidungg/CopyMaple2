#pragma once
#include "EventHandlerWrapperInterface.h"

BEGIN(Engine)
template<typename __Class, typename __EventType> 
using EventHandler = std::function<void(__Class& __this, __EventType* e)>;
//template<typename __Class, typename __EventType> using EventHandler = __Class::*__Handler(__Class* pObject, __EventType* e);


template<typename __Class,typename __EventType> 
class CEventHandlerWrapper :  public IEventHandlerWrapperInterface
{
private:
	CEventHandlerWrapper(__Class* _pClass, EventHandler<__Class, __EventType> _handler, const std::wstring& _handlerName) { pClass = _pClass; pClass->AddRef(); m_handler = _handler; m_handlerName = _handlerName; }
	~CEventHandlerWrapper() override {}
public:
	static CEventHandlerWrapper* Create(__Class* _pClass, EventHandler<__Class, __EventType> _handler, const std::wstring& _handlerName)
	{
		CEventHandlerWrapper* pInstance = new CEventHandlerWrapper(_pClass, _handler, _handlerName);
		return pInstance;
	}
	void Call(CEvent* evt) override
	{
		m_handler(*pClass, static_cast<__EventType*>(evt));
	}

	void* GetInstanceAddress() override { return reinterpret_cast<void*>(pClass); }

private:
	__Class* pClass;
	EventHandler<__Class,__EventType> m_handler;
	std::wstring m_handlerName;

	// CBase��(��) ���� ��ӵ�
	void Free() override
	{
		Safe_Release(pClass);

	}
};

END