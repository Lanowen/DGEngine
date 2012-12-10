#include "ScriptManager.h"
#include "StaticScriptFunctions.h"

void ScriptManager::BeforeUpdate(){
	for(int i = 0, size=this->p_activeScripts.size(); i < size; i++){
		ActiveScript* script = this->p_activeScripts[i];
		script->BeforeUpdate();
	}
}

void ScriptManager::AfterUpdate(){
	bool removeCompleted = false;
	for(int i = 0, size=this->p_activeScripts.size(); i < size; i++){
		ActiveScript* script = this->p_activeScripts[i];
		script->AfterUpdate();
		if(script->IsComplete()){
			removeCompleted = true;
		}
	}

	if(removeCompleted){
		this->RemoveCompletedScripts();
	}
}

LuaPlus::LuaObject ScriptManager::CreateActiveScript(LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData,LuaPlus::LuaObject originalSubClass){
	ActiveScript* script = new ActiveScript();
	script->Self.AssignNewTable(this->m_lua->GetState());

	if(script->BuildFromScript(originalSubClass,constructionData))
	{
		LuaPlus::LuaObject metaTableObj = this->m_lua->GetGlobals().Lookup("ActiveScript");
		script->Self.SetLightUserData("__object",script);
		script->Self.SetMetaTable(metaTableObj);
		script->RegisterCppFunctions(metaTableObj);
		script->Initialize();

		this->p_activeScripts.push_back(script);
	}
	else
	{
		script->Self.AssignNil(this->m_lua->GetState());
	}

	return script->Self;
}

void ScriptManager::RemoveCompletedScripts(){
	int i = 0, size=this->p_activeScripts.size();
	for(;i < size; i++){
		ActiveScript* script = this->p_activeScripts[i];
		if (script->IsComplete()){
			this->p_activeScripts.erase(this->p_activeScripts.begin() + i);
			i--;
			size--;
		}
	}
}

void ScriptManager::InitActiveScriptTable(){
	LuaPlus::LuaObject metaTableObj = this->m_lua->GetGlobals().CreateTable("ActiveScript");
	metaTableObj.SetObject("__index", metaTableObj);
	metaTableObj.SetObject("base", metaTableObj);
	metaTableObj.SetBoolean("cpp", true);
	metaTableObj.RegisterDirect("Create", (*this), &ScriptManager::CreateActiveScript);
}

void ScriptManager::InitFunctionality(){
	this->m_lua->GetGlobals().RegisterDirect("ExecuteScript", (*this), &ScriptManager::ExecuteScript);

	this->m_lua->GetGlobals().RegisterDirect("print", (*this), &ScriptManager::Trace);

	StaticScriptFunctions::RegisterFunctions(this->m_lua->GetGlobals());
	StaticScriptFunctions::RegisterTypes();
}

void ScriptManager::ExecuteScript(const char* filename){
	int result = this->m_lua->GetState()->DoFile(filename);

	if(result != 0)
		this->ThrowError(result);
}

void ScriptManager::ThrowError(int errorNum){
	LuaPlus::LuaStackObject stackObj(this->m_lua->GetState(), -1);
	const char* errStr = stackObj.GetString();
	Logging::ScriptDebugMessage(errStr);
}

void ScriptManager::Trace(const char* message){
	Logging::ScriptDebugMessage(message);
}

ScriptManager::ScriptManager(){
	this->m_lua = LuaManager::Get();
	this->InitFunctionality();
	this->InitActiveScriptTable();
	//this->ExecuteScript("programdata/required.lua");
}

ScriptManager::ScriptManager(const ScriptManager&){

}

ScriptManager::~ScriptManager(){
	this->m_lua->Shutdown();
	delete this->m_lua;
	this->m_lua = 0;
}