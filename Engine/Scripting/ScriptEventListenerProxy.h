#pragma once

#include <LuaPlus.h>

#include "ScriptEvent.h"
#include <Event\EventManager.h>

class ScriptEventListenerProxy {
	std::string m_eventType;
	LuaPlus::LuaObject m_scriptCallbackFunction;

public:
	ScriptEventListenerProxy(std::string eventType, const LuaPlus::LuaObject& scriptCallbackFunction);
	~ScriptEventListenerProxy(void);
	EventListenerDelegate GetDelagate(void) { return fastdelegate::MakeDelegate(this, &ScriptEventListenerProxy::ScriptEventDelegate);}
	void ScriptEventDelegate(EventPtr pEvent);
};