#include <io/file.hpp>

namespace io
{

file::file() {

}

file::file(const filesystem::path &path) : filesystem::path(path) {

}

file::file(const filesystem::path &&path) : filesystem::path(path) {

}

file::file(const char* filename) : filesystem::path(filename) {

}

file::file(const std::string& filename) : filesystem::path(filename) {

}

file::file(const file& file) {
	m_type = file.m_type;
	m_path = file.m_path;
	m_absolute = file.m_absolute;
}

file::file(const file&& file) {
	m_type = file.m_type;
	m_path = file.m_path;
	m_absolute = file.m_absolute;
}

file& file::operator=(const char* filename) {
	set(filename);
	return *this;
}

file& file::operator=(const std::string& filename) {
	set(filename);
	return *this;
}

file& file::operator=(const file& file) {
	m_type = file.m_type;
	m_path = file.m_path;
	m_absolute = file.m_absolute;
	return *this;
}

file& file::operator=(const file&& file) {
	m_type = file.m_type;
	m_path = file.m_path;
	m_absolute = file.m_absolute;
	return *this;
}

bool file::operator==(const file& file) const {
	return file.m_path == m_path;
}

bool file::operator!=(const file& file) const {
	return file.m_path != m_path;
}

}