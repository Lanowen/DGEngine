#include "ActiveScript.h"

bool ActiveScript::BuildFromScript(LuaPlus::LuaObject scriptClass, LuaPlus::LuaObject data)
{
	LuaPlus::LuaObject fromScript = scriptClass.GetByName("Initialize");
	if (fromScript.IsFunction())
	{
		this->p_scriptInitFunction = fromScript;
	}

	fromScript = scriptClass.GetByName("BeforeUpdate");
	if (fromScript.IsFunction())
	{
		this->p_scriptBeforeUpdateFunction  = fromScript;
	}

	fromScript = scriptClass.GetByName("AfterUpdate");
	if (fromScript.IsFunction())
	{
		this->p_scriptAfterUpdateFunction = fromScript;
	}

	fromScript = scriptClass.GetByName("OnComplete");
	if (fromScript.IsFunction())
	{
		this->p_scriptCompleteFunction = fromScript;
	}

	return true;
}

void ActiveScript::Initialize()
{
	this->p_state = ACTIVE;
	if (!this->p_scriptInitFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(this->p_scriptInitFunction);
		func(this->Self);
	}
}

void ActiveScript::BeforeUpdate()
{
	if (this->p_state != ACTIVE)
	{
		return;
	}
	if (!this->p_scriptBeforeUpdateFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(this->p_scriptBeforeUpdateFunction);
		func(this->Self);
	}
}

void ActiveScript::AfterUpdate()
{
	if (this->p_state != ACTIVE)
	{
		return;
	}
	if (!this->p_scriptAfterUpdateFunction.IsNil())
	{
		LuaPlus::LuaFunction<void> func(this->p_scriptAfterUpdateFunction);
		func(this->Self);
	}
}

void ActiveScript::Complete()
{
	this->p_state = COMPLETE;
	LuaPlus::LuaFunction<void> func(this->p_scriptCompleteFunction);
	func(this->Self);
}

void ActiveScript::RegisterCppFunctions(LuaPlus::LuaObject metaTable)
{
	metaTable.RegisterObjectDirect("Complete",    (ActiveScript*)0, &ActiveScript::Complete);
}