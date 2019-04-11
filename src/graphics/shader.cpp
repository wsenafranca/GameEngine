#include <graphics/shader.hpp>
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace graphics
{

base::pointer<graphics::shader> shader::create(const std::string &name) {
	return base::cache::create<graphics::shader>(name);
}

shader::shader(const std::string &name) : base::cacheable(name), _program(0) {

}

shader::~shader() {
	if(_program) glDeleteProgram(_program);
}

void shader::add_file(const io::file& file, GLenum type) {
	if(!file.exists()) {
		throw io::file_not_found_error(file);
	}
	
	std::string source;
	std::ifstream stream(file);
	for(std::string line; std::getline(stream, line); ) source += line + "\n";

	try {
		add_source(source, type);
	}
	catch(std::exception& e) {
		throw std::runtime_error(file.str() + ": " + e.what());
	}
}

void shader::add_source(const std::string& source, GLenum type) {
	char *buffer = (char*) malloc(source.length()+1);
	strcpy(buffer, source.c_str());
	buffer[source.length()] = 0;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &buffer, NULL);
	glCompileShader(shader);
	std::free(buffer);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		char log[255];
		glGetShaderInfoLog(shader, 255, 0, log);
		throw std::runtime_error(log);
	}
	_shaders.push_back(shader);
}

void shader::link() {
	_program = glCreateProgram();
	for(auto& shader : _shaders) {
		glAttachShader(_program, shader);
	}
	glLinkProgram(_program);
	for(auto& shader : _shaders) {
		glDeleteShader(shader);
	}
	_shaders.clear();
}

void shader::bind() {
	glUseProgram(_program);
}

void shader::unbind() {
	glUseProgram(0);
}

GLint shader::uniform_location(const std::string& name) const {
	auto it = _uniform_cache.find(name);
	if(it == _uniform_cache.end()) {
		GLint loc = glGetUniformLocation(_program, name.c_str());
		if(loc < 0) {
			return loc;
		}
		_uniform_cache[name] = loc;
		return loc;
	}
	return it->second;
}

GLint shader::attrib_location(const std::string& name) const {
	auto it = _attrib_cache.find(name);
	if(it == _attrib_cache.end()) {
		GLint loc = glGetAttribLocation(_program, name.c_str());
		if(loc < 0) {
			return loc;
		}
		_attrib_cache[name] = loc;
		return loc;
	}
	return it->second;
}

void shader::uniform1f(GLint location, GLfloat v0) {
	glUniform1f(location, v0);
}
void shader::uniform2f(GLint location, GLfloat v0, GLfloat v1) {
	glUniform2f(location, v0, v1);
}
void shader::uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
	glUniform3f(location, v0, v1, v2);
}
void shader::uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	glUniform4f(location, v0, v1, v2, v3);
}

void shader::uniform1i(GLint location, GLint v0) {
	glUniform1i(location, v0);
}
void shader::uniform2i(GLint location, GLint v0, GLint v1) {
	glUniform2i(location, v0, v1);
}
void shader::uniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
	glUniform3i(location, v0, v1, v2);
}
void shader::uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
	glUniform4i(location, v0, v1, v2, v3);
}

void shader::uniform1ui(GLint location, GLuint v0) {
	glUniform1ui(location, v0);
}
void shader::uniform1ui(GLint location, GLuint v0, GLuint v1) {
	glUniform2ui(location, v0, v1);
}
void shader::uniform1ui(GLint location, GLuint v0, GLuint v1, GLuint v2) {
	glUniform3ui(location, v0, v1, v2);
}
void shader::uniform1ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
	glUniform4ui(location, v0, v1, v2, v3);
}

void shader::uniform1fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform1fv(location, count, value);
}
void shader::uniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform2fv(location, count, value);
}
void shader::uniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform3fv(location, count, value);
}
void shader::uniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	glUniform4fv(location, count, value);
}

void shader::uniform1iv(GLint location, GLsizei count, const GLint *value) {
	glUniform1iv(location, count, value);
}
void shader::uniform2iv(GLint location, GLsizei count, const GLint *value) {
	glUniform2iv(location, count, value);
}
void shader::uniform3iv(GLint location, GLsizei count, const GLint *value) {
	glUniform3iv(location, count, value);
}
void shader::uniform4iv(GLint location, GLsizei count, const GLint *value) {
	glUniform4iv(location, count, value);
}

void shader::uniform1uiv(GLint location, GLsizei count, const GLuint *value) {
	glUniform1uiv(location, count, value);
}
void shader::uniform2uiv(GLint location, GLsizei count, const GLuint *value) {
	glUniform2uiv(location, count, value);
}
void shader::uniform3uiv(GLint location, GLsizei count, const GLuint *value) {
	glUniform3uiv(location, count, value);
}
void shader::uniform4uiv(GLint location, GLsizei count, const GLuint *value) {
	glUniform4uiv(location, count, value);
}

void shader::uniform_matrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2fv(location, count, transpose, value);
}
void shader::uniform_matrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3fv(location, count, transpose, value);
}
void shader::uniform_matrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4fv(location, count, transpose, value);
}
void shader::uniform_matrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2x3fv(location, count, transpose, value);
}
void shader::uniform_matrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3x2fv(location, count, transpose, value);
}
void shader::uniform_matrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2x4fv(location, count, transpose, value);
}
void shader::uniform_matrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4x2fv(location, count, transpose, value);
}
void shader::uniform_matrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3x4fv(location, count, transpose, value);
}
void shader::uniform_matrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4x3fv(location, count, transpose, value);
}

void shader::uniform_matrix(GLint location, const math::mat4 &m) {
	glUniformMatrix4fv(location, 1, false, m);
}

void shader::uniform_color(GLint location, const graphics::color& color) {
	glUniform4fv(location, 1, color);
}

void shader::uniform1f(const std::string& name, GLfloat v0) {
	glUniform1f(uniform_location(name), v0);
}
void shader::uniform2f(const std::string& name, GLfloat v0, GLfloat v1) {
	glUniform2f(uniform_location(name), v0, v1);
}
void shader::uniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2) {
	glUniform3f(uniform_location(name), v0, v1, v2);
}
void shader::uniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
	glUniform4f(uniform_location(name), v0, v1, v2, v3);
}

void shader::uniform1i(const std::string& name, GLint v0) {
	glUniform1i(uniform_location(name), v0);
}
void shader::uniform2i(const std::string& name, GLint v0, GLint v1) {
	glUniform2i(uniform_location(name), v0, v1);
}
void shader::uniform3i(const std::string& name, GLint v0, GLint v1, GLint v2) {
	glUniform3i(uniform_location(name), v0, v1, v2);
}
void shader::uniform4i(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3) {
	glUniform4i(uniform_location(name), v0, v1, v2, v3);
}

void shader::uniform1ui(const std::string& name, GLuint v0) {
	glUniform1ui(uniform_location(name), v0);
}
void shader::uniform1ui(const std::string& name, GLuint v0, GLuint v1) {
	glUniform2ui(uniform_location(name), v0, v1);
}
void shader::uniform1ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2) {
	glUniform3ui(uniform_location(name), v0, v1, v2);
}
void shader::uniform1ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
	glUniform4ui(uniform_location(name), v0, v1, v2, v3);
}

void shader::uniform1fv(const std::string& name, GLsizei count, const GLfloat *value) {
	glUniform1fv(uniform_location(name), count, value);
}
void shader::uniform2fv(const std::string& name, GLsizei count, const GLfloat *value) {
	glUniform2fv(uniform_location(name), count, value);
}
void shader::uniform3fv(const std::string& name, GLsizei count, const GLfloat *value) {
	glUniform3fv(uniform_location(name), count, value);
}
void shader::uniform4fv(const std::string& name, GLsizei count, const GLfloat *value) {
	glUniform4fv(uniform_location(name), count, value);
}

void shader::uniform1iv(const std::string& name, GLsizei count, const GLint *value) {
	glUniform1iv(uniform_location(name), count, value);
}
void shader::uniform2iv(const std::string& name, GLsizei count, const GLint *value) {
	glUniform2iv(uniform_location(name), count, value);
}
void shader::uniform3iv(const std::string& name, GLsizei count, const GLint *value) {
	glUniform3iv(uniform_location(name), count, value);
}
void shader::uniform4iv(const std::string& name, GLsizei count, const GLint *value) {
	glUniform4iv(uniform_location(name), count, value);
}

void shader::uniform1uiv(const std::string& name, GLsizei count, const GLuint *value) {
	glUniform1uiv(uniform_location(name), count, value);
}
void shader::uniform2uiv(const std::string& name, GLsizei count, const GLuint *value) {
	glUniform2uiv(uniform_location(name), count, value);
}
void shader::uniform3uiv(const std::string& name, GLsizei count, const GLuint *value) {
	glUniform3uiv(uniform_location(name), count, value);
}
void shader::uniform4uiv(const std::string& name, GLsizei count, const GLuint *value) {
	glUniform4uiv(uniform_location(name), count, value);
}

void shader::uniform_matrix2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix2x3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2x3fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix3x2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3x2fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix2x4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix2x4fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix4x2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4x2fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix3x4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix3x4fv(uniform_location(name), count, transpose, value);
}
void shader::uniform_matrix4x3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value) {
	glUniformMatrix4x3fv(uniform_location(name), count, transpose, value);
}

void shader::uniform_matrix(const std::string& name, const math::mat4 &m) {
	glUniformMatrix4fv(uniform_location(name), 1, false, m);
}

void shader::uniform_color(const std::string& name, const graphics::color& color) {
	glUniform4fv(uniform_location(name), 1, color);
}

}
