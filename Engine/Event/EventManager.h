#ifndef INC_EVENTMANAGER_H
#define INC_EVENTMANAGER_H

#include <concurrent_queue.h>
#include <list>
#include <string>
#include <map>
#include <vector>
#include "FastDelegate\FastDelegate.h"
#include <memory>

#include "Event.h"

typedef std::shared_ptr<Event> EventPtr;
typedef fastdelegate::FastDelegate1<EventPtr> EventListenerDelegate;
typedef Concurrency::concurrent_queue<EventPtr> ThreadSafeEventQueue;
typedef std::list<EventPtr> EventQueue;
typedef std::vector<EventListenerDelegate> EventListenerVector;
typedef std::map<std::string, EventListenerVector> EventListenerMap;

const unsigned int EVENTMANAGER_MAX_QUEUES = 2;

class EventManager
{
public:
	static EventManager* Get();

private:
	static EventManager* p_EventMgr;
	EventManager();
	EventManager(const EventManager&);

public:
	
	~EventManager();

	bool Update();

	void AddEventListener(std::string, const EventListenerDelegate);
	void RemoveEventListener(std::string, const EventListenerDelegate);

	bool TriggerEvent(EventPtr);
	bool QueueEvent(EventPtr);
	bool AbortEvent(std::string);
	bool ThreadSafeQueueEvent(EventPtr);
private:
	int m_activeQueue;
	EventListenerMap m_eventListeners;
	ThreadSafeEventQueue m_threadSafeQueue;
	EventQueue m_queues[EVENTMANAGER_MAX_QUEUES];
};

#endif