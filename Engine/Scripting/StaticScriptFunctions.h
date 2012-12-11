#pragma once

#include <LuaPlus.h>
#include <memory>

#include "ScriptEventListenerProxy.h"

typedef std::shared_ptr<ScriptEvent> ScriptEventPtr;

class StaticScriptFunctions {
public:
	static void RegisterFunctions(LuaPlus::LuaObject globals);
	static void RegisterTypes();
	static std::string AddEventListener(std::string type, LuaPlus::LuaObject callback);
	//static void RemoveEventListener(std::string type, LuaPlus::LuaObject callback);
	//static void RemoveEventListener(int type);
};