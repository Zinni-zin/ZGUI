#include "Headers/Texture2D.h"

namespace ZGL
{
	Texture2D::Texture2D()
		: m_localBuffer(nullptr), m_ID(0), m_width(0), m_height(0), m_BPP(0)
	{ }

	Texture2D::Texture2D(const std::string& path)
		: m_localBuffer(nullptr), m_ID(0), m_width(0), m_height(0), m_BPP(0)
	{
		LoadFromFile(path);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID);
	}

void Texture2D::LoadFromFile(const std::string& path)
{
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

	void Texture2D::Bind(GLuint slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void Texture2D::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}