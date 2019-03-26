#include "Bundle.h"
#include <cstring>

Bundle::Bundle() {
}

Bundle::Bundle(const Bundle &bundle) {
	set(bundle);
}

Bundle& Bundle::operator=(const Bundle &bundle) {
	clear();
	set(bundle);
	return *this;
}

Bundle::~Bundle() {
	clear();
}

const std::any& Bundle::get(const std::string &name) const {
	return data.at(name);
}

std::any& Bundle::operator[](const std::string &name) {
	return data[name];
}

std::any& Bundle::operator[](std::string &&name) {
	return data[name];
}

void Bundle::remove(const std::string &name) {
	auto it = data.find(name);
	if(it != data.end())
		data.erase(name);
}

void Bundle::clear() {
	data.clear();
}

bool Bundle::contains(const std::string &name) const {
	return data.find(name) != data.end();
}

std::vector<std::string> Bundle::keys() const {
	std::vector<std::string> k;
	for(auto it : data) {
		k.push_back(it.first);
	}
	return k;
}

void Bundle::set(const Bundle &bundle) {
	for(auto it : bundle.data) {
		data[it.first] = it.second;
	}
}