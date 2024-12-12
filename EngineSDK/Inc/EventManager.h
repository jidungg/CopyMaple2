#pragma once
#include "Event.h"
#include "EventHandlerWrapperInterface.h"
BEGIN(Engine)
class CEventManager : public CBase
{
private:
	CEventManager() {}
	virtual ~CEventManager() { Free(); }

	void Update()
	{
		DispatchEvent();
	}
public:
	void RegisterPreEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _pCallback);
	void UnRegisterPreEventCallback(_uint _eventType, const void* _pSubscriber);
	void RegisterPostEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler);
	void UnRegisterPostEventCallback(_uint _eventType, const void* _pSubscriber);
	void PushEvent(CEvent* _event) { m_eventList.push_back(_event); }
	void DispatchEvent();
	void TriggerEvent(CEvent* _event);


private:
	std::list<CEvent*> m_eventList;
	std::unordered_map<_uint, std::vector<IEventHandlerWrapperInterface*>> m_umapPreEventCallback;
	std::unordered_map<_uint, std::vector<IEventHandlerWrapperInterface*>> m_umapPostEventCallback;
public:
	static CEventManager* Create();
	virtual void Free() 
	{
		for (auto& pEvent : m_eventList)
			Safe_Release(pEvent);
		m_eventList.clear();

		for (auto& pair : m_umapPreEventCallback)
		{
			for (auto& handler : pair.second)
				Safe_Release(handler);
		}
		m_umapPreEventCallback.clear();
		for (auto& pair : m_umapPostEventCallback)
		{
			for (auto& handler : pair.second)
				Safe_Release(handler);
		}
		m_umapPostEventCallback.clear();
	}
};




END