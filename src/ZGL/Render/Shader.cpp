#include "Headers/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ZGL {

	Shader::Shader() { }

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		LoadFromFile(vertexPath, fragmentPath);
	}

	Shader::~Shader()
	{
		glUseProgram(0);
	}

	unsigned int Shader::Compile(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)alloca(length * sizeof(char));
			
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Failed To Compile Shader " << (type == GL_VERTEX_SHADER? "Vertex!" : "Fragment!") << 
				message << std::endl;

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = Compile(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}

	void Shader::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::ifstream vStream(vertexPath);
		std::ifstream fStream(fragmentPath);

		std::string line;
		std::stringstream ss[2];

		while (getline(vStream, line)) ss[0] << line << "\n";
		while (getline(fStream, line)) ss[1] << line << "\n";

		m_ID = CreateShader(ss[0].str(), ss[1].str());
	}

	void Shader::Use() const
	{
		glUseProgram(m_ID);
	}

	void Shader::SetUniform1i(const char* name, int value) 
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform1i(location, value);
	}

	void Shader::SetUniform1f(const char* name, float value)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform1f(location, value);
	}

	void Shader::SetUniform1iv(const char* name, GLsizei count, const GLint* value)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform1iv(location, count, value);
	}

	void Shader::SetVector2f(const char* name, float x, float y)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform2f(location, x, y);
	}

	void Shader::SetVector2f(const char* name, const Vec2& value)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform2f(location, value.x, value.y);
	}

	void Shader::SetVector3f(const char* name, float x, float y, float z)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform3f(location, x, y, z);
	}

	void Shader::SetVector3f(const char* name, const Vec3& value)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetUniform4f(const char* name, float x, float y, float z, float w)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform4f(location, x, y, z, w);
	}

	void Shader::SetVector4f(const char* name, const Vec4& value)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniform4f(location, value.x, value.y, value.y, value.w);
	}

	void Shader::SetUniformMat4f(const char* name, const Mat4& matrix)
	{
		GLint location = getUniformLocation(name);

		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.m_cells[0][0]);
	}

	GLint Shader::getUniformLocation(const std::string& name) const
	{
		if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end())
			return m_UniformlocationCache[name];

		int location = glGetUniformLocation(m_ID, name.c_str());
		if (location == -1) 
			std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;

		m_UniformlocationCache[name] = location;

		return location;
	}
}
