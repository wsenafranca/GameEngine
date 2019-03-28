#ifndef LUA_SCRIPT
#define LUA_SCRIPT

#include <Component.h>
#include <LuaManager.h>

class LuaScript : public Component {
public:
	LuaScript(const std::string &name);

protected:
	void onCreate() override;
	void onPreUpdate(float dt) override;
	void onUpdate(float dt) override;
	void onPostUpdate(float dt) override;
	void onPreRender() override;
	void onRender() override;
	void onPostRender() override;
	void onDestroy() override;
private:
	sol::table ref;
};

#endif