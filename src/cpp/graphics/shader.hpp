#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <graphics/gl.hpp>
#include <string>
#include <base/object.hpp>
#include <unordered_map>
#include <list>
#include <math/matrix.hpp>
#include <graphics/color.hpp>
#include <base/pointer.hpp>
#include <base/cache.hpp>
#include <io/file.hpp>

namespace graphics
{

class shader : public base::cacheable {
public:
	static base::pointer<graphics::shader> create(const std::string &name);

	shader(const std::string &name);
	virtual ~shader();

	void add_file(const io::file& file, GLenum type);
	void add_source(const std::string& source, GLenum type);
	void link();
	void bind();
	void unbind();

	operator GLuint() {
		return _program;
	}

	GLint uniform_location(const std::string& name) const;
	GLint attrib_location(const std::string& name) const;

	void uniform1f(GLint location, GLfloat v0);
	void uniform2f(GLint location, GLfloat v0, GLfloat v1);
	void uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

	void uniform1i(GLint location, GLint v0);
	void uniform2i(GLint location, GLint v0, GLint v1);
	void uniform3i(GLint location, GLint v0, GLint v1, GLint v2);
	void uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

	void uniform1ui(GLint location, GLuint v0);
	void uniform1ui(GLint location, GLuint v0, GLuint v1);
	void uniform1ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
	void uniform1ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

	void uniform1fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform2fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform3fv(GLint location, GLsizei count, const GLfloat *value);
	void uniform4fv(GLint location, GLsizei count, const GLfloat *value);

	void uniform1iv(GLint location, GLsizei count, const GLint *value);
	void uniform2iv(GLint location, GLsizei count, const GLint *value);
	void uniform3iv(GLint location, GLsizei count, const GLint *value);
	void uniform4iv(GLint location, GLsizei count, const GLint *value);

	void uniform1uiv(GLint location, GLsizei count, const GLuint *value);
	void uniform2uiv(GLint location, GLsizei count, const GLuint *value);
	void uniform3uiv(GLint location, GLsizei count, const GLuint *value);
	void uniform4uiv(GLint location, GLsizei count, const GLuint *value);

	void uniform_matrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

	void uniform_matrix(GLint location, const math::mat4 &m);
	void uniform_color(GLint location, const graphics::color& color);

	void uniform1f(const std::string& name, GLfloat v0);
	void uniform2f(const std::string& name, GLfloat v0, GLfloat v1);
	void uniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
	void uniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

	void uniform1i(const std::string& name, GLint v0);
	void uniform2i(const std::string& name, GLint v0, GLint v1);
	void uniform3i(const std::string& name, GLint v0, GLint v1, GLint v2);
	void uniform4i(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3);

	void uniform1ui(const std::string& name, GLuint v0);
	void uniform1ui(const std::string& name, GLuint v0, GLuint v1);
	void uniform1ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2);
	void uniform1ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

	void uniform1fv(const std::string& name, GLsizei count, const GLfloat *value);
	void uniform2fv(const std::string& name, GLsizei count, const GLfloat *value);
	void uniform3fv(const std::string& name, GLsizei count, const GLfloat *value);
	void uniform4fv(const std::string& name, GLsizei count, const GLfloat *value);

	void uniform1iv(const std::string& name, GLsizei count, const GLint *value);
	void uniform2iv(const std::string& name, GLsizei count, const GLint *value);
	void uniform3iv(const std::string& name, GLsizei count, const GLint *value);
	void uniform4iv(const std::string& name, GLsizei count, const GLint *value);

	void uniform1uiv(const std::string& name, GLsizei count, const GLuint *value);
	void uniform2uiv(const std::string& name, GLsizei count, const GLuint *value);
	void uniform3uiv(const std::string& name, GLsizei count, const GLuint *value);
	void uniform4uiv(const std::string& name, GLsizei count, const GLuint *value);

	void uniform_matrix2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix2x3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3x2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix2x4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4x2fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix3x4fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);
	void uniform_matrix4x3fv(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat *value);

	void uniform_matrix(const std::string& name, const math::mat4 &m);
	void uniform_color(const std::string& name, const graphics::color& color);

private:
	GLuint _program;
	std::list<GLuint> _shaders;
	mutable std::unordered_map<std::string, GLint> _uniform_cache;
	mutable std::unordered_map<std::string, GLint> _attrib_cache;
};

}

#endif