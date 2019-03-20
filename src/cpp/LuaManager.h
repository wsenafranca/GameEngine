#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <sol/sol.hpp>

class LuaManager {
public:
	static sol::state state;
	static void init();
private:
	LuaManager() = default;
	~LuaManager() = default;
};

#endif