#ifndef INC_SCRIPTEVENT_H
#define INC_SCRIPTEVENT_H

#include <LuaPlus.h>

#include <Event\Event.h>
#include "LuaManager.h"

class ScriptEvent : public Event
{
public:
	LuaPlus::LuaObject GetEventData();

protected:
	virtual void BuildEventData();

protected:
	LuaPlus::LuaObject m_eventData;
};

#endif