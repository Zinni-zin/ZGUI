#ifndef FILE_Texture2D
#define FILE_Texture2D

#include <string>

#include "GL/glew.h"
#include "../../../Vendor/stb_image/stb_image.h"

namespace ZGL
{
	class Texture2D
	{
	public:
		Texture2D();

		Texture2D(const std::string& path);
		~Texture2D();

		void LoadFromFile(const std::string& path);

		void Bind(GLuint slot = 0);
		void Unbind();

		inline GLuint GetID() const { return m_ID; }

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	private:
		GLuint m_ID;
		unsigned char* m_localBuffer;

		int m_width, m_height, m_BPP; // width, height, bits per pixels
	};
}
#endif 