#ifndef BASE_DICTIONARY_HPP
#define BASE_DICTIONARY_HPP

#include <any>
#include <unordered_map>
#include <vector>
#include <string>

namespace base
{

class dictionary : public std::unordered_map<std::string, std::any> {
public:
	virtual ~dictionary() = default;

	template<class T>
	T get(const std::string& key) const {
		auto it = find(key);
		if(it == cend()) {
			throw std::runtime_error("key " + key + " not found.");
		}
		return std::any_cast<T>(it->second);
	}

	template<class T>
	T get(const std::string& key, const T& def) const {
		try {
			return get<T>(key);
		}
		catch(...) {}
		return def;
	}

	void put(const std::string &key, const std::any &value);

	bool contains(const std::string &key) const;
	std::vector<std::string> keys() const;
};

};

#endif