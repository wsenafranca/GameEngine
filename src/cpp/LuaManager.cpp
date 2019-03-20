#include "LuaManager.h"
#include <Node.h>
#include <Component.h>
#include <stdexcept>
#include <glm/glm.hpp>
#include <Color.h>
#include <Input.h>

sol::state LuaManager::state = sol::state();

void LuaManager::init() {
	state.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io, sol::lib::math);

	state.new_usertype<glm::vec2>("Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		sol::meta_function::index, [](glm::vec2 &v, sol::stack_object key, sol::this_state L){
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
		sol::meta_function::new_index, [](glm::vec2 &v, sol::stack_object key, sol::stack_object value, sol::this_state L){
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
					color.g = value.as<unsigned char>();
				}
				if(k == "a") {
					color.g = value.as<unsigned char>();
				}
			}
		});

	state.new_usertype<Bundle::Property>("BundleProperty",
		"asString", &Bundle::Property::asString,
		"asInt", &Bundle::Property::asInt,
		"asUInt", &Bundle::Property::asUInt,
		"asByte", &Bundle::Property::asByte,
		"asFloat", &Bundle::Property::asFloat,
		"asDouble", &Bundle::Property::asDouble,
		"asLong", &Bundle::Property::asLong);
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

	state.new_usertype<Object>("Object",
		"asNode", &Object::as<Node>,
		"asComponent", &Object::as<Component>);

	state.new_usertype<Node>("Node",
		"findNode", &Node::findNode<Node>,
		"findComponent", &Node::findComponent<Component>,
		"getParent", &Node::parent,
		sol::meta_function::index, [](Node &node, sol::stack_object key, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "name") {
					return sol::object(L, sol::in_place, node.name());
				}
				if(k == "properties") {
					return sol::object(L, sol::in_place, &node.properties());
				}
				if(k == "position") {
					return sol::object(L, sol::in_place, &node.position());
				}
				if(k == "size") {
					return sol::object(L, sol::in_place, &node.size());
				}
				if(k == "width") {
					return sol::object(L, sol::in_place, node.width());
				}
				if(k == "height") {
					return sol::object(L, sol::in_place, node.height());
				}
				if(k == "scale") {
					return sol::object(L, sol::in_place, &node.scale());
				}
				if(k == "rotate") {
					return sol::object(L, sol::in_place, node.rotate());
				}
				if(k == "origin") {
					return sol::object(L, sol::in_place, &node.origin());
				}
				if(k == "color") {
					return sol::object(L, sol::in_place, &node.color());
				}
			}
			return sol::object(L, sol::in_place, sol::lua_nil);
		},
		sol::meta_function::new_index, [](Node &node, sol::stack_object key, sol::stack_object value, sol::this_state L){
			auto maybe_string_key = key.as<sol::optional<std::string>>();
			if (maybe_string_key) {
				const std::string& k = *maybe_string_key;
				if(k == "name") {
					node.name(value.as<std::string>());
				}
				if(k == "properties") {
					node.properties(value.as<Bundle>());
				}
				if(k == "position") {
					node.position(value.as<glm::vec2>());
				}
				if(k == "size") {
					node.size(value.as<glm::vec2>());
				}
				if(k == "width") {
					node.width(value.as<float>());
				}
				if(k == "height") {
					node.height(value.as<float>());
				}
				if(k == "scale") {
					node.scale(value.as<glm::vec2>());
				}
				if(k == "rotate") {
					node.rotate(value.as<float>());
				}
				if(k == "origin") {
					node.origin(value.as<glm::vec2>());
				}
				if(k == "color") {
					node.color(value.as<Color>());
				}
			}
		},
		sol::base_classes, sol::bases<Object>());

	auto input = state.create_named_table("Input");
	input["isKeyPressed"] = Input::isKeyPressed;
	input["isKeyReleased"] = Input::isKeyReleased;
	input["mousePositionX"] = Input::mousePositionX;
	input["mousePositionY"] = Input::mousePositionY;
	input["isMouseButtonPressed"] = Input::isMouseButtonPressed;
	input["isMouseButtonReleased"] = Input::isMouseButtonReleased;

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
			if not data.onDestroy then
				data.onDestroy = function(self)
				end
			end
			setmetatable(data, ScriptMetaTable_)
			return data
		end)"
	);
}