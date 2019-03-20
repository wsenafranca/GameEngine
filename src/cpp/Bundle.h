#ifndef BUNDLE_H
#define BUNDLE_H

#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include <algorithm>

class Bundle {
public:
	class Property {
	public:
		template<typename T>
		decltype(auto) as() const {
			T v;
			memcpy(&v, value.data(), value.size());
			return v;
		}

		void set(const void* data, size_t size) {
			value = std::string((char*)data, ((char*)(data))+size);
		}

		auto asString() const {
			return value;
		}

		auto asInt() const {
			return as<int>();
		}

		auto asUInt() const {
			return as<unsigned int>();
		}

		auto asChar() const {
			return as<char>();
		}

		auto asByte() const {
			return as<unsigned char>();
		}

		auto asFloat() const {
			return as<float>();
		}		

		auto asDouble() const {
			return as<double>();
		}

		auto asLong() const {
			return as<long>();
		}

		auto operator=(int value) {
			set(&value, sizeof(int));
			return *this;
		}

		auto operator=(unsigned int value) {
			set(&value, sizeof(unsigned int));
			return *this;
		}

		auto operator=(char value) {
			set(&value, sizeof(char));
			return *this;
		}

		auto operator=(unsigned char value) {
			set(&value, sizeof(unsigned char));
			return *this;
		}

		auto operator=(float value) {
			set(&value, sizeof(float));
			return *this;
		}

		auto operator=(double value) {
			set(&value, sizeof(double));
			return *this;
		}

		auto operator=(long value) {
			set(&value, sizeof(long));
			return *this;
		}

		auto operator=(const std::string &value) {
			this->value = value;
			return *this;
		}

		auto operator=(const char *value) {
			this->value = value;
			return *this;
		}
	private:
		std::string value;
	};

	Bundle();
	Bundle(const Bundle &bundle);
	Bundle& operator=(const Bundle &bundle);
	virtual ~Bundle();

	template<class T>
	void put(const std::string &name, const T& value) {
		put(name, &value, sizeof(T));
	}

	void put(const std::string &name, const std::string& value) {
		put(name, &(value[0]), value.size());
	}

	void put(const std::string &name, const char*& value) {
		put(name, value, strlen(value));
	}

	void put(const std::string &name, const void* value, size_t size) {
		Property p;
		p.set(value, size);
		data[name] = p;
	}

	const Bundle::Property& get(const std::string &name) const;

	Bundle::Property& operator[](const std::string &name);
	Bundle::Property& operator[](std::string &&name);

	void remove(const std::string &name);
	void clear();

	bool contains(const std::string &name) const;
	std::vector<std::string> keys() const;

	void set(const Bundle &bundle);
private:
	std::map<std::string, Property> data;
};

#endif