#ifndef BASE_CACHE_HPP
#define BASE_CACHE_HPP

#include <base/object.hpp>
#include <string>
#include <base/pointer.hpp>
#include <unordered_map>

namespace base
{

class cacheable : public base::object {
public:
	cacheable(const std::string& name);
	const std::string& name() const;
private:
	std::string _name;
};

class cache {
public:
	template <class T> static base::pointer<T> create(const std::string &name, bool* cachehit = nullptr) {
		auto it = _cache.find(name);
		if(it == _cache.end()) {
			auto obj = new T(name);
			_cache[name] = obj;
			if(cachehit) *cachehit = false;
			return obj;
		}
		if(cachehit) *cachehit = true;
		return it->second;
	}

	template <class T> static base::pointer<T> get(const std::string& name) {
		auto it = _cache.find(name);
		return it != _cache.end() ? it->second : nullptr;
	}

	static bool contains(const std::string &name);

	static void erase(const std::string &name);

	typedef std::unordered_map<std::string, base::pointer_t> cache_t;
private:
	cache() = default;
	cache(const cache&) = delete;
	cache& operator=(const cache&) = delete;

	static cache_t _cache;
};

}

#endif