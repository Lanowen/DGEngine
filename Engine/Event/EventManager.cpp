#include "EventManager.h"

EventManager* EventManager::p_EventMgr = 0;

bool EventManager::Initialize()
{
	return true;
}

bool EventManager::Update()
{
	EventPtr threadSafeEvent;
	while (this->m_threadSafeQueue.try_pop(threadSafeEvent))
	{
		this->QueueEvent(threadSafeEvent);
	}

    int queueToProcess = this->m_activeQueue;
	this->m_activeQueue = (this->m_activeQueue + 1) % EVENTMANAGER_MAX_QUEUES;
	this->m_queues[this->m_activeQueue].clear();

	while (!this->m_queues[queueToProcess].empty())
	{
		EventPtr event = this->m_queues[queueToProcess].front();
		this->m_queues[queueToProcess].pop_front();

		if (this->m_eventListeners.find(event->Name) != this->m_eventListeners.end())
		{
			EventListenerVector delegates = this->m_eventListeners[event->Name];

			for (auto it = delegates.begin(); it != delegates.end(); ++it)
			{
                EventListenerDelegate listener = (*it);
				listener(event);
			}
		}
	}
	
	bool queueFlushed = (this->m_queues[queueToProcess].empty());
	if (!queueFlushed)
	{
		while (!this->m_queues[queueToProcess].empty())
		{
			EventPtr event = this->m_queues[queueToProcess].back();
			this->m_queues[queueToProcess].pop_back();
			this->m_queues[this->m_activeQueue].push_front(event);
		}
	}
	
	return queueFlushed;
}

void EventManager::Shutdown()
{
}

void EventManager::AddEventListener(std::string type, const EventListenerDelegate func)
{
	EventListenerVector& delegates = this->m_eventListeners[type];

	for (auto it = delegates.begin(); it != delegates.end(); ++it)
	{
		if ((*it) == func)
		{
			return;
		}
	}

	delegates.push_back(func);
}

void EventManager::RemoveEventListener(std::string type, const EventListenerDelegate func)
{
	if (this->m_eventListeners.find(type) == this->m_eventListeners.end())
	{
		return;
	}

	EventListenerVector& delegates = this->m_eventListeners[type];
	for (auto it = delegates.begin(); it != delegates.end(); ++it)
	{
		if ((*it) == func)
		{
			delegates.erase(it);
		}
	}
}

bool EventManager::TriggerEvent(EventPtr event)
{
	if (this->m_eventListeners.find(event->Name) == this->m_eventListeners.end())
	{
		return false;
	}

	int i=0, size=this->m_eventListeners[event->Name].size();

	for (; i < size; i++)
	{
		this->m_eventListeners[event->Name][i](event);
	}

	return true;
}

bool EventManager::QueueEvent(EventPtr event)
{
	if (!event)
	{
		return false;
	}

	auto type = this->m_eventListeners.find(event->Name);
    if (type != this->m_eventListeners.end())
    {
        this->m_queues[this->m_activeQueue].push_back(event);
        return true;
    }
    else
    {
        return false;
    }
}

bool EventManager::AbortEvent(std::string type)
{
	EventListenerMap::iterator evmIt = this->m_eventListeners.find(type);

	if (evmIt != this->m_eventListeners.end())
    {
        EventQueue& eventQueue = this->m_queues[this->m_activeQueue];
        auto it = eventQueue.begin();
        while (it != eventQueue.end())
        {
            auto event = it;
            ++it;

			if ((*event)->Name == type)
	        {
		        eventQueue.erase(event);
	        }
        }
    }

	return true;
}

bool EventManager::ThreadSafeQueueEvent(EventPtr event)
{
	this->m_threadSafeQueue.push(event);
	return true;
}

EventManager::EventManager()
{
	this->m_activeQueue = 0;
	p_EventMgr = this;
}

EventManager::EventManager(const EventManager&)
{
}

EventManager::~EventManager()
{
	p_EventMgr = 0;
}

EventManager* EventManager::Get(){

	if(!p_EventMgr){
		new EventManager();
		p_EventMgr->Initialize();
	}

	return p_EventMgr;
}