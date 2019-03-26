#ifndef OBJECT_H
#define OBJECT_H

#include <Bundle.h>
#include <DispatchQueue.h>
#include <LuaScriptProtocol.h>

#define TYPENAME(X) virtual std::string getTypeName() const { return #X; }

class Object : public LuaScriptProtocol {
public:
	TYPENAME(Object)

	static bool destroy(Object *object) {
		object->ref--;
		if(object->ref == 0) {
			DispatchQueue::main()->async([object]{
				delete object;
			});
			return true;
		}
		return false;
	}

	void acquire() {
		ref++;
	}

	void release() {
		destroy(this);
	}

	void setName(const std::string &name) {m_name = name;}
	const std::string& getName() const {return m_name;}

	template<class T>
	T* as() {
		return dynamic_cast<T*>(this);
	}

	void setProperties(const Bundle &bundle) {m_properties = bundle;}
	const Bundle& getProperties() const {return m_properties;}
	Bundle& getProperties() {return m_properties;}

	virtual sol::object luaIndex(sol::stack_object key, sol::this_state L) override {
		auto maybe_string_key = key.as<sol::optional<std::string>>();
		if (maybe_string_key) {
			const std::string& k = *maybe_string_key;
			if(k == "name") {
				return sol::object(L, sol::in_place, getName());
			}
			if(k == "properties") {
				return sol::object(L, sol::in_place, &getProperties());
			}
			if(k == "__type") {
				return sol::object(L, sol::in_place, getTypeName());
			}
		}

		return sol::object(L, sol::in_place, sol::lua_nil);
	}

	virtual void luaNewIndex(sol::stack_object key, sol::stack_object value, sol::this_state L) override {
		auto maybe_string_key = key.as<sol::optional<std::string>>();
		if (maybe_string_key) {
			const std::string& k = *maybe_string_key;
			if(k == "properties") {
				setProperties(value.as<Bundle>());
			}
		}
	}

protected:
	Object()= default;
	virtual ~Object() = default;

private:
	std::string m_name;
	Bundle m_properties;
	unsigned int ref;
};

#endif