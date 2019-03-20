#include "LuaScript.h"
#include <Node.h>

LuaScript::LuaScript(const std::string &name) {
	this->name(name);
}

void LuaScript::onCreate() {
	LuaManager::state.script_file("lua/"+name()+".lua");
	ref = LuaManager::state[name()];
	ref["node"] = node();
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