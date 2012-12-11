#pragma once

#include <Scripting\ScriptEvent.h>

class ChangeCameraView :public ScriptEvent {
public:
	int m_view;
protected:
	void BuildEventData(){
		this->m_eventData.AssignNewTable(LuaManager::Get()->GetState());		
		this->m_eventData.SetInteger("view", this->m_view);
	}
};