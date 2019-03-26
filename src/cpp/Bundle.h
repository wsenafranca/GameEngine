#ifndef BUNDLE_H
#define BUNDLE_H

#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <any>

class Bundle {
public:
	Bundle();
	Bundle(const Bundle &bundle);
	Bundle& operator=(const Bundle &bundle);
	virtual ~Bundle();

	template<class T>
	void put(const std::string &name, const T& value) {
		data[name] = value;
	}

	void put(const std::string &name, const std::string& value) {
		data[name] = value;
	}

	void put(const std::string &name, const char*& value) {
		data[name] = value;
	}

	const std::any& get(const std::string &name) const;

	template<class T>
	T get(const std::string &name, const T& def) {
		auto it = data.find(name);
		if(it != data.end()) {
			return std::any_cast<T>(it->second);
		}
		return def;
	}

	std::any& operator[](const std::string &name);
	std::any& operator[](std::string &&name);

	void remove(const std::string &name);
	void clear();

	bool contains(const std::string &name) const;
	std::vector<std::string> keys() const;

	void set(const Bundle &bundle);
private:
	std::map<std::string, std::any> data;
};

#endif