#include "ScriptEventListenerProxy.h"

ScriptEventListenerProxy::ScriptEventListenerProxy(std::string eventType, const LuaPlus::LuaObject& scriptCallbackFunction) : m_scriptCallbackFunction(scriptCallbackFunction) {
	this->m_eventType = eventType;
}

ScriptEventListenerProxy::~ScriptEventListenerProxy(void){

}

void ScriptEventListenerProxy::ScriptEventDelegate(EventPtr pEvent){
	std::shared_ptr<ScriptEvent> pScriptEvent = std::static_pointer_cast<ScriptEvent>(pEvent);
	LuaPlus::LuaFunction<void> Callback = this->m_scriptCallbackFunction;
	Callback(pScriptEvent->GetEventData());
}