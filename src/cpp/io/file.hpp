#ifndef IO_FILE_HPP
#define IO_FILE_HPP

#include <filesystem/path.h>
#include <system_error>

namespace io
{

class file : public filesystem::path {
public:
	file();
	file(const filesystem::path &path);
	file(const filesystem::path &&path);
	file(const char* filename);
	file(const std::string& filename);
	file(const file& file);
	file(const file&& file);

	file& operator=(const char* filename);
	file& operator=(const std::string& filename);
	file& operator=(const file& file);
	file& operator=(const file&& file);
#if defined(_WIN32)
	file(const std::wstring &wstring) { set(wstring); }
	file(const wchar_t *wstring) { set(wstring); }
	file& operator=(const std::wstring &wstring) { set(wstring); return *this; }
	file& operator=(const wchar_t *wstring) { set(wstring); return *this; }
#endif

	bool operator==(const file& file) const;
	bool operator!=(const file& file) const;

	operator const std::string() const {
		return str();
	}

private:
};

class file_not_found_error : public std::system_error {
public:
	file_not_found_error(const file& file) : 
		std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), file.str()) {}
};

}

#endif