#include "stdafx.h"

#include "EventBus.h"

EventBus::ListenerId EventBus::m_listenerIdCtr = 0;

EventBus & EventBus::getInstance()
{
	static EventBus instance;
	return instance;
}

EventBus::ListenerId 
EventBus::AddListener(IEvent::EventType type, EventListener listener)
{
	++m_listenerIdCtr;
	m_eventListenerMap[type].push_back({ m_listenerIdCtr, listener });
	m_listenerIdEventMap[m_listenerIdCtr] = type;
	return m_listenerIdCtr;
}

void EventBus::RemoveListener(ListenerId id)
{
	auto eventTypeIt = m_listenerIdEventMap.find(id);
	if (eventTypeIt == m_listenerIdEventMap.end())
		return;

	auto eventListenerIt = m_eventListenerMap.find(eventTypeIt->second);
	if (eventListenerIt == m_eventListenerMap.end())
	{
		FF_ERROR_MSG("Possible error: did not find event type in listener list.");
		return;
	}

	bool found = false; //< for debugging
	auto &listenerList = eventListenerIt->second;
	for (auto it = listenerList.begin(); it != listenerList.end(); ++it)
	{
		if ((*it).id != id)
			continue;
		it = listenerList.erase(it);
		m_listenerIdEventMap.erase(id);
		found = true;
		break;
	}

	if (found)
		err() << "Removed listener with given id. " << id << std::endl;
	else
		err() << "Possible error: did not find listener with this id for this event. "
		<< id << " " << eventTypeIt->second << std::endl;
}

void EventBus::FireEvent(IEvent::Ptr event)
{
	auto it = m_eventListenerMap.find(event->GetEventType());
	if (it == m_eventListenerMap.end())
		return;

	auto& listenerList = it->second;
	for (auto listenerIti = listenerList.begin(); listenerIti != listenerList.end(); ++listenerIti)
	{
		auto listenerData = *listenerIti;
		listenerData.listener(event);
	}
}

void EventBus::QueueEvent(IEvent::Ptr event)
{
	m_eventQueueBuffer[m_currentEventQueue].push(event);
}

void EventBus::ProcessEvents(float fDeltaTime)
{
	int currentQueue = m_currentEventQueue;
	while (!m_eventQueueBuffer[currentQueue].empty())
	{
		auto event = m_eventQueueBuffer[currentQueue].front();
		m_eventQueueBuffer[currentQueue].pop();

		FireEvent(event);
	}

	// switch buffer
	m_currentEventQueue = (currentQueue + 1) % k_NumMaxQueues;
}
