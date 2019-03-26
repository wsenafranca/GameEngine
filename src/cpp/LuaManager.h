#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <sol/sol.hpp>
#include <AppListener.h>
#include <Singleton.h>

class LuaManager : public AppListener{
public:
	SINGLETON(LuaManager)
	
	void onCreate() override;
	void onUpdate(float dt) override;
	void onDestroy() override;
	sol::state state;
private:
	LuaManager() = default;
	~LuaManager() = default;
	
};

#endif