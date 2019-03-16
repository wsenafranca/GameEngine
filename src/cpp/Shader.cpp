#include "Shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <string>
#include <fstream>

Shader::Shader() {
	shaders = new std::vector<unsigned int>();
}

Shader::~Shader() {
}

void Shader::add(const char *filename, int shaderType) {
	GLuint shader = compileShader(filename, shaderType);
	shaders->push_back(shader);
}

void Shader::create() {
	mProgram = glCreateProgram();
	for(auto shader : *shaders) {
		glAttachShader(mProgram, shader);
	}
	glLinkProgram(mProgram);
	for(auto shader : *shaders) {
		glDeleteShader(shader);
	}
	delete shaders;
}

void Shader::use() {
	glUseProgram(mProgram);
}

unsigned int Shader::program() const {
	return mProgram;
}

void Shader::destroy() {
	glDeleteProgram(mProgram);
}

unsigned int Shader::compileShader(const char *fileName, int shaderType) {
	std::string source;
	std::ifstream stream(fileName);
	for(std::string line; std::getline(stream, line); ) source += line + "\n";

	char *buffer = (char*) malloc(source.length()+1);
	strcpy(buffer, source.c_str());
	buffer[source.length()] = 0;

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &buffer, NULL);
	glCompileShader(shader);
	free(buffer);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		char log[255];
		glGetShaderInfoLog(shader, 255, 0, log);
		throw std::runtime_error(std::string(fileName)+": "+log);
	}

	return shader;
}
