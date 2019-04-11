#include <io/json.hpp>
#include <stdexcept>
#include <fstream>

namespace io
{

base::dictionary json::decode_file(const file& file) {
	if(!file.exists()) {
		throw io::file_not_found_error(file);
	}

	std::string json;
	std::ifstream stream(file);
	for(std::string line; std::getline(stream, line); ) {json += line + "\n";}

	return decode(json);
}

base::dictionary json::decode(const std::string& json) {
	rapidjson::Document doc;
	rapidjson::ParseResult ok = doc.Parse(json.c_str());

	base::dictionary dic;
	if(ok) {
		for(auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it) {
			dic.put(it->name.GetString(), decode_object(it));
		}
	}
	else {
		char buffer[512];
		sprintf(buffer, "JSON parse error: %s (%u)", rapidjson::GetParseError_En(ok.Code()), ok.Offset());
		throw std::runtime_error(buffer);
	}

	return dic;
}

std::any json::decode_object(const rapidjson::Value::ConstValueIterator& it) {
	if(it->IsBool()) {
		return it->GetBool();
	}
	else if(it->IsNumber()) {
		return it->GetFloat();
	}
	else if(it->IsInt()){
		return it->GetInt();
	}
	else if(it->IsDouble()) {
		return it->GetDouble();
	}
	else if(it->IsString()) {
		return std::string(it->GetString());
	}
	else if(it->IsArray()) {
		return decode_array(it);
	}
	else if(it->IsObject()) {
		base::dictionary dic;
		for(auto child = it->MemberBegin(); child != it->MemberEnd(); ++child) {
			dic.put(child->name.GetString(), decode_object(child));
		}
		return dic;
	}

	return nullptr;
}

std::any json::decode_object(const rapidjson::Value::ConstMemberIterator& it) {
	if(it->value.IsBool()) {
		return it->value.GetBool();
	}
	else if(it->value.IsNumber()) {
		return it->value.GetFloat();
	}
	else if(it->value.IsInt()){
		return it->value.GetInt();
	}
	else if(it->value.IsDouble()) {
		return it->value.GetDouble();
	}
	else if(it->value.IsString()) {
		return std::string(it->value.GetString());
	}
	else if(it->value.IsArray()) {
		return decode_array(it);
	}
	else if(it->value.IsObject()) {
		base::dictionary dic;
		auto obj = it->value.GetObject();
		for(auto child = obj.MemberBegin(); child != obj.MemberEnd(); ++child) {
			dic.put(child->name.GetString(), decode_object(child));
		}
		return dic;
	}

	return nullptr;
}

std::any json::decode_array(const rapidjson::Value::ConstValueIterator& it) {
	std::vector<std::any> array;
	auto a = it->GetArray();
	for (rapidjson::Value::ConstValueIterator i = a.Begin(); i < a.End(); i++) {
		array.push_back(decode_object(i));
	}

	return array;
}

std::any json::decode_array(const rapidjson::Value::ConstMemberIterator& it) {
	std::vector<std::any> array;
	auto a = it->value.GetArray();
	for (rapidjson::Value::ConstValueIterator i = a.Begin(); i < a.End(); i++) {
		array.push_back(decode_object(i));
	}

	return array;
}

}