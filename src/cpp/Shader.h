#ifndef SHADER_H
#define SHADER_H

#include <vector>

class Shader {
public:
	Shader();
	virtual ~Shader();
	void add(const char *filename, int shaderType);
	void create();
	void use();
	unsigned int program() const;
	void destroy();
	operator unsigned int() const {
		return mProgram;
	}
private:
	unsigned int compileShader(const char *fileName, int shaderType);
	unsigned int mProgram;
	std::vector<unsigned int> *shaders;
};

#endif