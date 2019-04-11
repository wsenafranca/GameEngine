#ifndef IO_JSON_HPP
#define IO_JSON_HPP

#include <base/dictionary.hpp>
#include <io/file.hpp>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <vector>

namespace io
{

class json {
public:
	static base::dictionary decode_file(const file& file);
	static base::dictionary decode(const std::string& json);

private:
	static std::any decode_object(const rapidjson::Value::ConstValueIterator& it);
	static std::any decode_object(const rapidjson::Value::ConstMemberIterator& it);
	static std::any decode_array(const rapidjson::Value::ConstValueIterator& it);
	static std::any decode_array(const rapidjson::Value::ConstMemberIterator& it);

	json() = default;
	~json() = default;
	json(const json&) = delete;
	json& operator=(const json&) = delete;
};

}

#endif