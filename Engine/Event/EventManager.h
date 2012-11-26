#ifndef INC_EVENTMANAGER_H
#define INC_EVENTMANAGER_H

#include <concurrent_queue.h>
#include <list>
#include <string>
#include <map>
#include <vector>
#include "FastDelegate\FastDelegate.h"

#include "Event.h"

typedef fastdelegate::FastDelegate1<Event*> EventListenerDelegate;
typedef Concurrency::concurrent_queue<Event*> ThreadSafeEventQueue;
typedef std::list<Event*> EventQueue;
typedef std::vector<EventListenerDelegate> EventListenerVector;
typedef std::map<std::string, EventListenerVector> EventListenerMap;

const unsigned int EVENTMANAGER_MAX_QUEUES = 2;

class EventManager
{
public:
	EventManager();
	EventManager(const EventManager&);
	~EventManager();

	bool Initialize();
	bool Update();
	void Shutdown();

	void AddEventListener(std::string, const EventListenerDelegate);
	void RemoveEventListener(std::string, const EventListenerDelegate);

	bool TriggerEvent(Event*);
	bool QueueEvent(Event*);
	bool AbortEvent(std::string);
	bool ThreadSafeQueueEvent(Event*);
private:
	int m_activeQueue;
	EventListenerMap m_eventListeners;
	ThreadSafeEventQueue m_threadSafeQueue;
	EventQueue m_queues[EVENTMANAGER_MAX_QUEUES];
};

#endif