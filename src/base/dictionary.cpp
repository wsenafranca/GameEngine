#include <base/dictionary.hpp>

void base::dictionary::put(const std::string &key, const std::any &value) {
	insert(std::pair<std::string, std::any>(key, value));
}

bool base::dictionary::contains(const std::string &key) const {
	return find(key) != cend();
}

std::vector<std::string> base::dictionary::keys() const {
	std::vector<std::string> k;
	for(auto it = cbegin(); it != cend(); ++it) {
		k.push_back(it->first);
	}
	return k;
}
