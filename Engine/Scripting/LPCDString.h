#pragma once

#include <LuaPlus.h>
#include <string>
#include <sstream>
#include <wchar.h>

namespace LPCD
{
    inline bool    Match(TypeWrapper<std::string>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline bool    Match(TypeWrapper<std::string&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline bool Match(TypeWrapper<const std::string&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline std::string Get(TypeWrapper<std::string>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline std::string Get(TypeWrapper<std::string&>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline std::string Get(TypeWrapper<const std::string&>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline void Push(lua_State* L, std::string& value)
        {  lua_pushstring(L, value.c_str());  }
    inline void Push(lua_State* L, const std::string& value)
        {  lua_pushstring(L, value.c_str());  }


	/*inline bool    Match(TypeWrapper<std::wstring>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TWSTRING;  }
    inline bool    Match(TypeWrapper<std::wstring&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TWSTRING;  }
    inline bool Match(TypeWrapper<const std::wstring&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TWSTRING;  }
    inline std::wstring Get(TypeWrapper<std::wstring>, lua_State* L, int idx)
	{  return reinterpret_cast<const wchar_t*>(lua_tostring(L, idx));  }
    inline std::wstring Get(TypeWrapper<std::wstring&>, lua_State* L, int idx)
        {  return reinterpret_cast<const wchar_t*>(lua_tostring(L, idx));  }
    inline std::wstring Get(TypeWrapper<const std::wstring&>, lua_State* L, int idx)
        {  return reinterpret_cast<const wchar_t*>(lua_tostring(L, idx));  }
    inline void Push(lua_State* L, std::wstring& value)
        {  lua_pushwstring(L, reinterpret_cast<const lua_WChar*>(value.c_str()));  }
    inline void Push(lua_State* L, const std::wstring& value)
        {  lua_pushwstring(L, reinterpret_cast<const lua_WChar*>(value.c_str()));  }*/
}