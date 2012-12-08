#include "ScriptEvent.h"

LuaPlus::LuaObject ScriptEvent::GetEventData()
{
	this->BuildEventData();
	return this->m_eventData;
}

void ScriptEvent::BuildEventData()
{
	this->m_eventData.AssignNil(LuaManager::Get()->GetState());
}