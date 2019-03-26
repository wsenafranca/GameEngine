#ifndef LUA_SCRIPT_PROTOCOL_H
#define LUA_SCRIPT_PROTOCOL_H

#include <sol/sol.hpp>

class LuaScriptProtocol {
public:
	virtual ~LuaScriptProtocol() = default;
	virtual sol::object luaIndex(sol::stack_object key, sol::this_state L) = 0;
	virtual void luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) = 0;
};

#endif