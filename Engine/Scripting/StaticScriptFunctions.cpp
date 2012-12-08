#include "StaticScriptFunctions.h"
#include <Event\EventManager.h>
#include "LPCDString.h"

std::string StaticScriptFunctions::AddEventListener(std::string type, LuaPlus::LuaObject callback){
	if(callback.IsFunction()){
		ScriptEventListenerProxy* listener = new ScriptEventListenerProxy(type, callback);
		EventManager::Get()->AddEventListener(type, listener->GetDelagate());

		return type;
	}

	return 0;
}

void StaticScriptFunctions::RegisterFunctions(LuaPlus::LuaObject globals){
	globals.RegisterDirect("AddEventListener", &StaticScriptFunctions::AddEventListener);
}

void StaticScriptFunctions::RegisterTypes(){
	//REGISTER_SCRIPT_EVENT(ScriptEvent, 0);
}