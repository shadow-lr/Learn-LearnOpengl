#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

// 防止 glm 库 重复引用
#ifndef GLM_H
#define GLM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#ifndef SHADER_H
#define SHADER_H
class Shader
{
public:
	// program ID
	GLuint Program;
	// read shader.vs or shader.frag file and construct shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// set bool
	void SetBool(const std::string& name, bool value)const;
	// set float
	void SetFloat(const std::string& name, float value)const;
	// set int
	void SetInt(const std::string& name, int value)const;
	// set color (red, green, blue, alpha)
	void SetColor(const std::string& name, std::vector<float> value)const;
	// set 3f Pos(x, y, z)
	void SetPos(const std::string& name, std::vector<float> value) const;
	// set all 3f
	void Set3f(const std::string& name, std::vector<float> value) const;
	// set 3f with vec3
	void Set3fv(const std::string& name, glm::vec3 value) const;
	// set matrix value
	void SetMat4(const std::string& name, GLsizei count, GLboolean transpose, glm::mat4& value) const;
	// use program
	void Use();
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件
		vShaderFile.close();
		fShaderFile.close();
		// 转换流至GLchar数组
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

inline void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
}

inline void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->Program, name.c_str()), value);
}

inline void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
}

inline void Shader::SetColor(const std::string& name, std::vector<float> value) const
{	
	// three parameters
	if (value.size() <= 3)
	{
		glUniform3f(glGetUniformLocation(this->Program, name.c_str()), value[0], value[1], value[2]);
	}
	// four parameters
	else
	{
		glUniform4f(glGetUniformLocation(this->Program, name.c_str()), value[0], value[1], value[2], value[3]);
	}
}

inline void Shader::SetPos(const std::string& name, std::vector<float> value) const
{
	glUniform3f(glGetUniformLocation(this->Program, name.c_str()), value[0], value[1], value[2]);
}

inline void Shader::Set3f(const std::string& name, std::vector<float> value) const
{
	glUniform3f(glGetUniformLocation(this->Program, name.c_str()), value[0], value[1], value[2]);
}

inline void Shader::Set3fv(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(this->Program, name.c_str()), value.x, value.y, value.z);
}

inline void Shader::SetMat4(const std::string& name, GLsizei count, GLboolean transpose, glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->Program, name.c_str()),
		count, transpose, glm::value_ptr(value));
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

#endif
