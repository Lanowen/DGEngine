#include "LuaManager.h"

LuaPlus::LuaState* LuaManager::GetState(){
	return this->m_state;
}

LuaPlus::LuaObject LuaManager::GetGlobals(){
	return this->m_state->GetGlobals();
}

void LuaManager::Shutdown(){
	LuaPlus::LuaState::Destroy(this->m_state);
}

LuaManager::LuaManager(){
	m_LuaManager = this;
	this->m_state = LuaPlus::LuaState::Create(true);
}

LuaManager::LuaManager(const LuaManager&){
}

LuaManager::~LuaManager(){

}

LuaManager* LuaManager::Get(){
	if(m_LuaManager == 0){
		new LuaManager();
	}
	
	return m_LuaManager;
}

LuaManager* LuaManager::m_LuaManager = 0;
