#pragma once

#include <LuaPlus.h>

class LuaManager{
public:
	static LuaManager* Get();

private:
	static LuaManager* m_LuaManager;

public:
	LuaManager();
	LuaManager(const LuaManager&);
	~LuaManager();

	LuaPlus::LuaState* GetState();
	LuaPlus::LuaObject GetGlobals();

	void Shutdown();

private:
	LuaPlus::LuaState* m_state;
};