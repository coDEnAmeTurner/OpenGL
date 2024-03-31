#pragma once

#include <glad/glad.h>	
#include <glm/gtc/type_ptr.hpp>
#include <string>	
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geomPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 mat) const;
	void setVec3(const std::string& name, glm::vec3 vec) const;

	void unuse();
};

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geomPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geomCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		if(geomPath) {
			gShaderFile.open(geomPath);
			std::stringstream  gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geomCode = gShaderStream.str();
		}

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;

	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int  geom;
	if (geomPath)
	{
		const char* gShaderCode = geomCode.c_str();
		geom = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom, 1, &gShaderCode, nullptr);
		glCompileShader(geom);
		glGetShaderiv(geom, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geom, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geomPath) 
		glAttachShader(ID, geom);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geomPath) 
		glDeleteShader(geom);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) const
{
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::unuse()
{
	glUseProgram(0);
}
