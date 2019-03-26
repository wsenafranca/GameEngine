#include "LuaScript.h"
#include <Node.h>

LuaScript::LuaScript(const std::string &name) {
	setName(name);
}

void LuaScript::onCreate() {
	LuaManager::instance()->state.script_file("lua/"+getName()+".lua");
	ref = LuaManager::instance()->state[getName()];
	ref["node"] = getNode();
	ref["cObj"] = (Component*) this;
	ref["onCreate"](ref);
}

void LuaScript::onPreUpdate(float dt) {
	ref["onPreUpdate"](ref, dt);
}

void LuaScript::onUpdate(float dt) {
	ref["onUpdate"](ref, dt);
}

void LuaScript::onPostUpdate(float dt) {
	ref["onPostUpdate"](ref, dt);
}

void LuaScript::onDestroy() {
	ref["onDestroy"](ref);
}