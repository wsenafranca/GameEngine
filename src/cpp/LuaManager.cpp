#include "LuaManager.h"
#include <Node.h>
#include <Component.h>
#include <stdexcept>
#include <Color.h>
#include <Input.h>
#include <RigidBody.h>
#include <Camera.h>

void LuaManager::onCreate() {
	state.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io, sol::lib::math);

	state.new_usertype<b2Vec2>("Vec2",
		sol::constructors<b2Vec2(), b2Vec2(float, float)>(),
		sol::meta_function::index, [](b2Vec2 &v, sol::stack_object key, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "x") {
					return sol::object(L, sol::in_place, v.x);
				}
				if(k == "y") {
					return sol::object(L, sol::in_place, v.y);
				}
			}
			return sol::object(L, sol::in_place, sol::lua_nil);
		},
		sol::meta_function::new_index, [](b2Vec2 &v, sol::stack_object key, sol::stack_object value, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "x") {
					v.x = value.as<float>();
				}
				if(k == "y") {
					v.y = value.as<float>();
				}
			}
		});


	state.new_usertype<Color>("Color",
		sol::constructors<Color(), Color(unsigned char, unsigned char, unsigned char, unsigned char)>(),
		sol::meta_function::index, [](Color &color, sol::stack_object key, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "r") {
					return sol::object(L, sol::in_place, color.r);
				}
				if(k == "g") {
					return sol::object(L, sol::in_place, color.g);
				}
				if(k == "b") {
					return sol::object(L, sol::in_place, color.b);
				}
				if(k == "a") {
					return sol::object(L, sol::in_place, color.a);
				}
			}
			return sol::object(L, sol::in_place, sol::lua_nil);
		},
		sol::meta_function::new_index, [](Color &color, sol::stack_object key, sol::stack_object value, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "r") {
					color.r = value.as<unsigned char>();
				}
				if(k == "g") {
					color.g = value.as<unsigned char>();
				}
				if(k == "b") {
					color.b = value.as<unsigned char>();
				}
				if(k == "a") {
					color.a = value.as<unsigned char>();
				}
			}
		});

	state.new_usertype<std::any>("any",
		"new", sol::no_constructor,
		"asString", [](std::any &value){
			return std::any_cast<std::string>(value);
		},
		"asInt", [](std::any &value){
			return std::any_cast<int>(value);
		},
		"asUInt", [](std::any &value){
			return std::any_cast<unsigned int>(value);
		},
		"asByte", [](std::any &value){
			return std::any_cast<unsigned char>(value);
		},
		"asFloat", [](std::any &value){
			return std::any_cast<float>(value);
		},
		"asDouble", [](std::any &value){
			return std::any_cast<double>(value);
		},
		"asLong", [](std::any &value){
			return std::any_cast<long>(value);
		});
	state.new_usertype<Bundle>("Bundle",
		sol::meta_function::index, [](Bundle &bundle, sol::stack_object key, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				return sol::object(L, sol::in_place, &bundle[k]);
			}
			return sol::object(L, sol::in_place, sol::lua_nil);
		},
		sol::meta_function::new_index, [](Bundle &bundle, sol::stack_object key, sol::stack_object value, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				const std::string& v = value.as<std::string>();
				bundle[k] = v;
			}
		});

	state.new_usertype<Node>("Node",
		"new", sol::no_constructor,
		sol::meta_function::garbage_collect, sol::destructor(&Node::destroy),
		"findNodeByName", &Node::findNodeByName<Node>,
		"findNodeByType", &Node::findNodeByType,
		"findComponentByName", &Node::findComponentByName<Component>,
		"findComponentByType", &Node::findComponentByType,
		sol::meta_function::index, [](Node &node, sol::stack_object key, sol::this_state L) {
			return node.luaIndex(key, L);
		},
		sol::meta_function::new_index, [](Node &node, sol::stack_object key, sol::stack_object value, sol::this_state L){
			node.luaNewIndex(key, value, L);
		},
		sol::base_classes, sol::bases<Object>());

	state.new_usertype<Camera>("Camera",
		sol::meta_function::index, [](Camera &camera, sol::stack_object key, sol::this_state L){
			return camera.luaIndex(key, L);
		},
		sol::meta_function::new_index, [](Camera &camera, sol::stack_object key, sol::stack_object value, sol::this_state L){
			camera.luaNewIndex(key, value, L);
		},
		sol::base_classes, sol::bases<Node>());

	state.new_usertype<Component>("Component",
		"asRigidBody", &Component::as<RigidBody>,
		sol::base_classes, sol::bases<Object>());

	state.new_usertype<RigidBody>("RigidBody",
		"getPosition", &RigidBody::getPosition,
		"getAngle", &RigidBody::getAngle,
		"getLinearVelocity", &RigidBody::getLinearVelocity,
		"getAngularVelocity", &RigidBody::getAngularVelocity,
		"getWorldCenter", &RigidBody::getWorldCenter,
		"getLocalCenter", &RigidBody::getLocalCenter,
		"applyForce", &RigidBody::applyForce,
		"applyForceToCenter", &RigidBody::applyForceToCenter,
		"applyTorque", &RigidBody::applyTorque,
		"applyLinearImpulse", &RigidBody::applyLinearImpulse,
		"applyAngularImpulse", &RigidBody::applyAngularImpulse,
		"getMass", &RigidBody::getMass,
		"isJumpPeak", &RigidBody::isJumpPeak,
		"isOnFloor", &RigidBody::isOnFloor,
		"isFalling", &RigidBody::isFalling,
		sol::base_classes, sol::bases<Component>());

	auto input = state.create_named_table("Input");
	input["isKeyPressed"] = Input::isKeyPressed;
	input["isKeyReleased"] = Input::isKeyReleased;
	input["mousePositionX"] = Input::mousePositionX;
	input["mousePositionY"] = Input::mousePositionY;
	input["isMouseButtonPressed"] = Input::isMouseButtonPressed;
	input["isMouseButtonReleased"] = Input::isMouseButtonReleased;

	state.script(R"(
		math.clamp = function(value, min, max)
			return math.max(min, math.min(value, max))
		end
	)");

	state.script(R"(
		oldType = type
		type = function(obj)
			if obj.__type then
				return obj.__type
			end
			return oldType(obj)
		end
	)");

	state.script(R"(
		local Script_ = {}

		local ScriptMetaTable_ = {
			__index = Script_
		}

		function Script(data)
			if not data then
				data = {}
			end
			if not data.onCreate then
				data.onCreate = function(self)
				end
			end
			if not data.onPreUpdate then
				data.onPreUpdate = function(self, delta)
				end
			end
			if not data.onUpdate then
				data.onUpdate = function(self, delta)
				end
			end
			if not data.onPostUpdate then
				data.onPostUpdate = function(self, delta)
				end
			end
			if not data.onPreRender then
				data.onPreRender = function(self)
				end
			end
			if not data.onRender then
				data.onRender = function(self)
				end
			end
			if not data.onPostRender then
				data.onPostRender = function(self)
				end
			end
			if not data.onDestroy then
				data.onDestroy = function(self)
				end
			end
			setmetatable(data, ScriptMetaTable_)
			return data
		end)"
	);
}

void LuaManager::onUpdate(float dt) {

}

void LuaManager::onDestroy() {

}