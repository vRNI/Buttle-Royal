#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include <queue>

#include "IEvent.h"

// rudimentary event bus (implemented as Singelton) 
// for registering delegates that execute code, when event is triggered.
// addons for the future: abort events, timer to limit processing of
// events, ...
//
// BE CAREFUL: the EventBus has not been thoroughly tested.
class EventBus
{
public:
	typedef std::function<void(IEvent::Ptr)> EventListener;
	typedef unsigned long ListenerId;

	static EventBus& getInstance();

	// Add/remove event listeners
	// Be careful: 
	// Due to the usage of std::function, multiple additions of the
	// same function to the listener cannot be checked.
	ListenerId AddListener(IEvent::EventType type, EventListener listener);
	void RemoveListener(ListenerId id);

	// immediately triggers the event
	void FireEvent(IEvent::Ptr event);
	// queue event to be processed
	void QueueEvent(IEvent::Ptr event);
	// should called once per frame to process events
	void ProcessEvents(float fDeltaTime);

private:

	struct ListenerData
	{
		ListenerId id;
		EventListener listener;
	};
	typedef std::list<ListenerData> ListenerList;
	typedef std::unordered_map<IEvent::EventType, ListenerList> EventListenerMap;
	typedef std::unordered_map<ListenerId, IEvent::EventType> ListenerIdEventMap;
	typedef std::queue<IEvent::Ptr> EventQueue;

	static const int k_NumMaxQueues = 2;
	static ListenerId m_listenerIdCtr;

	int m_currentEventQueue = 0;

	// buffering the events to avoid circles when new events are
	// queued each frame before the old ones are processed.
	EventQueue m_eventQueueBuffer[k_NumMaxQueues];

	// The map stores the registered listeners for each event.
	EventListenerMap m_eventListenerMap;
	// This map is used for faster lookups when removing listeners.
	ListenerIdEventMap m_listenerIdEventMap;
};