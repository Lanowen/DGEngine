#pragma once

#include <LuaPlus.h>
#include <string>

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
}