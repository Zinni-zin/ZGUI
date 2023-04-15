#pragma once
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "../lib/Vector2f.h"
//#include "../lib/Vector3f.h"

#include "../../Math/Matrices.h"

namespace ZGL {
	class Shader
	{
	public:
		Shader();
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		unsigned int Compile(unsigned int type, const std::string& source);
		GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		void LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);

		void Use() const; 

		void SetUniform1i(const char* name, int value);

		void SetUniform1f(const char* name, float value);

		void SetUniform1iv(const char* name, GLsizei count, const GLint* value);

		void SetVector2f(const char* name, float x, float y);

		void SetVector2f(const char* name, const Vec2& value); 

		void SetVector3f(const char* name, float x, float y, float z);

		void SetVector3f(const char* name, const Vec3& value);

		void SetUniform4f(const char* name, float x, float y, float z, float w);

		void SetVector4f(const char* name, const Vec4& value);
		
		void SetUniformMat4f(const char* name, const Mat4& matrix);

		inline GLuint GetID() const { return m_ID; }

	private:
		mutable std::unordered_map<std::string, GLint> m_UniformlocationCache;

		GLuint m_ID;
		GLint getUniformLocation(const std::string& name) const;
	};
}