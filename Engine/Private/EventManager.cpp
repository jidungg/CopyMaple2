#include "EventManager.h"


void CEventManager::RegisterPreEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _pCallback)
{
	auto subscriberListIter = m_umapPreEventCallback.find(_eventType);
	if (subscriberListIter != m_umapPreEventCallback.end())
	{
		for (auto subscriber : subscriberListIter->second)
			if (subscriber->GetInstanceAddress() == _pCallback->GetInstanceAddress())
			{
				return;
			}
		subscriberListIter->second.push_back(_pCallback);
	}
	else
	{
		m_umapPreEventCallback[_eventType].push_back(_pCallback);
	}
}

void CEventManager::UnRegisterPreEventCallback(_uint _eventType, const void* _handlerAddress)
{
	auto& handlers = m_umapPreEventCallback[_eventType];
	for (int i = 0; i < handlers.size(); i++)
	{
		if (handlers[i]->GetInstanceAddress() == _handlerAddress)
		{
			Safe_Release(handlers[i]);
			handlers.erase(handlers.begin() + i);
		}
	}
}

void CEventManager::RegisterPostEventCallback(_uint _eventType, IEventHandlerWrapperInterface* _handler)
{
	auto subscriberListIter = m_umapPostEventCallback.find(_eventType);
	if (subscriberListIter != m_umapPostEventCallback.end())
	{
		for (auto subscriber : subscriberListIter->second)
			if (subscriber->GetInstanceAddress() == _handler->GetInstanceAddress())
			{
				return;
			}
		subscriberListIter->second.push_back(_handler);
	}
	else
	{
		m_umapPostEventCallback[_eventType].push_back(_handler);
	}
}

void CEventManager::UnRegisterPostEventCallback(_uint _eventType, const void* _handlerAddress)
{
	auto& handlers = m_umapPostEventCallback[_eventType];
	for (int i = 0; i < handlers.size(); i++)
	{
		if (handlers[i]->GetInstanceAddress() == _handlerAddress)
		{
			Safe_Release(handlers[i]);
			handlers.erase(handlers.begin() + i);
		}
	}
}

void CEventManager::TriggerEvent(CEvent* _event)
{
	for (auto handler : m_umapPreEventCallback[_event->GetEventID()])
		handler->Exec(_event);//�ڵ鷯

	_event->Exec();			  //�̺�Ʈ �Լ�

	for (auto handler : m_umapPostEventCallback[_event->GetEventID()])
		handler->Exec(_event);//�ڵ鷯
	Safe_Release(_event);
}

CEventManager* CEventManager::Create()
{
	return new CEventManager();

}

void CEventManager::DispatchEvent()
{
	for (auto pEvent : m_eventList)
	{
		TriggerEvent(pEvent);
	}
	m_eventList.clear();
}