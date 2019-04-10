#include "cache.hpp"

namespace base 
{

cache::cache_t cache::_cache;

cacheable::cacheable(const std::string& name) : _name(name) {

}

const std::string& cacheable::name() const {
	return _name;
}

bool cache::contains(const std::string &name) {
	return _cache.find(name) != _cache.end();
}

void cache::erase(const std::string &name) {
	auto it = _cache.find(name);
	if(it != _cache.end()) {
		_cache.erase(it);
	}
}

}
