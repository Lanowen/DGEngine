#ifndef INC_ACTIVESCRIPT_H
#define INC_ACTIVESCRIPT_H

#include <LuaPlus.h>



class ActiveScript
{
public:
	enum ActiveScriptStates
	{
		ACTIVE = 0,
		PAUSED = 1,
		COMPLETE = 2
	};

public:
	void RegisterCppFunctions(LuaPlus::LuaObject);
	bool BuildFromScript(LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject data);
	void Initialize();
	void BeforeUpdate();
	void AfterUpdate();
	void Complete();

	bool IsComplete() { return this->p_state == COMPLETE; };
public:
	LuaPlus::LuaObject Self;
private:
	LuaPlus::LuaObject p_scriptInitFunction, p_scriptBeforeUpdateFunction, p_scriptAfterUpdateFunction;
	LuaPlus::LuaObject p_scriptCompleteFunction;
	int p_state;
};

#endif