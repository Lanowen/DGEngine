#include <string>
#include <vector>

#include <LuaPlus.h>

#include "../Utils/Logging.h"
#include "ActiveScript.h"
#include "LuaManager.h"

typedef std::vector<ActiveScript*> ActiveScriptVector;

class ScriptManager{
public:
	ScriptManager();
	ScriptManager(const ScriptManager&);
	~ScriptManager();

	void BeforeUpdate();
	void AfterUpdate();
	void Success();
	void Fail();

	void Trace(const char* message);
	void ExecuteScript(const char* filename);

	LuaPlus::LuaObject CreateActiveScript(LuaPlus::LuaObject self, LuaPlus::LuaObject constructionData, LuaPlus::LuaObject originalSubClass);

private:
	void InitActiveScriptTable();
	void InitFunctionality();
	void RemoveCompletedScripts();

	void ThrowError(int errorNum);

private:
	LuaManager* m_lua;
	ActiveScriptVector p_activeScripts;
};