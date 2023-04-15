#include "Headers/Sprite.h"
#include <iostream>

namespace ZGL
{
	Sprite::Sprite()
		: m_posX(0.0f), m_posY(0.0f), m_width(-1), m_height(-1),
		m_subX(0), m_subY(0), m_subWidth(-1), m_subHeight(-1),
		m_textureID(0), m_textureWidth(-1), m_textureHeight(-1),
		m_colour(1.0f, 1.0f, 1.0f, 1.0f)
	{ }

	Sprite::Sprite(float xPos, float yPos)
		: m_posX(xPos), m_posY(yPos), m_width(-1), m_height(-1), 
		m_subX(0), m_subY(0), m_subWidth(-1), m_subHeight(-1),
		m_textureID(0), m_textureWidth(-1), m_textureHeight(-1),
		m_colour(1.0f, 1.0f, 1.0f, 1.0f)
	{ }
	
	Sprite::Sprite(float xPos, float yPos, int width, int height)
		: m_posX(xPos), m_posY(yPos), m_width(width), m_height(height),
		m_subX(0), m_subY(0), m_subWidth(-1), m_subHeight(-1),
		m_textureID(0), m_textureWidth(-1), m_textureHeight(-1),
		m_colour(1.0f, 1.0f, 1.0f, 1.0f)
	{ }

	Sprite::Sprite(Vec2 pos)
		: m_posX(pos.x), m_posY(pos.y), m_width(-1), m_height(-1),
		m_subX(0), m_subY(0), m_subWidth(-1), m_subHeight(-1),
		m_textureID(0), m_textureWidth(-1), m_textureHeight(-1),
		m_colour(1.0f, 1.0f, 1.0f, 1.0f)
	{ }

	Sprite::Sprite(Vec2 pos, Vec2i dimensions)
		: m_posX(pos.x), m_posY(pos.y), m_width(dimensions.x), m_height(dimensions.y),
		m_subX(0), m_subY(0), m_subWidth(-1), m_subHeight(-1), 
		m_textureID(0), m_textureWidth(-1), m_textureHeight(-1),
		m_colour(1.0f, 1.0f, 1.0f, 1.0f)
	{ }
	
	Sprite::~Sprite() { }
	
	void Sprite::SetTexture(std::shared_ptr<Texture2D> texture)
	{
		m_texture = texture;

		m_textureWidth = m_texture->GetWidth();
		m_textureHeight = m_texture->GetHeight();
		m_textureID = m_texture->GetID();

		if (m_width <= -1) m_width = m_textureWidth;
		if (m_height <= -1) m_height = m_textureHeight;

		m_SubCoords = Vec4(1.0, 1.0, 0.0, 0.0);
	}

	void Sprite::LoadTexture(const std::string & filePath)
	{
		m_texture = std::make_shared<Texture2D>(filePath);
		m_textureWidth = m_texture->GetWidth();
		m_textureHeight = m_texture->GetHeight();
		m_textureID = m_texture->GetID();

		if (m_width <= -1) m_width = m_textureWidth;
		if (m_height <= -1) m_height = m_textureHeight;

		m_SubCoords = Vec4(1.0, 1.0, 0.0, 0.0);
	}

	void Sprite::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	void Sprite::SetSubX(float x)
	{
		if (x >= 0 && x + m_subWidth <= m_textureWidth)
			m_subX = x;
		else
			std::cout << "Error X must be greater than 0 and sub width + X (" << m_subX + m_subWidth <<
			") must be less than the texture width(" << m_textureWidth << ")!\n";
	}
	
	void Sprite::SetSubY(float y)
	{
		if (y >= 0 && y + m_subHeight <= m_textureHeight)
			m_subY = y;
		else
			std::cout << "Error Y must be greater than 0 and sub height + Y (" << m_subY + m_subHeight <<
			") must be less than the texture height(" << m_textureHeight << ")!\n";
	}
	
	void Sprite::SetSubWidth(int width)
	{
		if (width > 0 && m_subX + width <= m_textureWidth)
			m_subWidth = width;
		else
			std::cout << "Error width must be greater than 0 and sub width + X ("<< m_subX + m_subWidth <<
			") must be less than the texture width("  << m_textureWidth << ")!\n";
	}
	
	void Sprite::SetSubHeight(int height)
	{
		if (height > 0 && m_subY + height <= m_textureHeight)
			m_subHeight = height;
		else
			std::cout << "Error height must be greater than 0 and sub height + Y (" << m_subY + m_subHeight <<
			") must be less than the texture height(" << m_textureHeight << ")!\n";
	}

	void Sprite::SetSubCoords(float x, float y, int width, int height)
	{
		SetSubX(x);
		SetSubY(y);
		SetSubWidth(width); 
		SetSubHeight(height);

		float maxWidth = (m_subWidth == m_textureWidth) ? 1.0f : (m_subX + m_subWidth) / (float)m_textureWidth;
		float maxHeight = (m_subHeight == m_textureHeight) ? 1.0f : (m_subY + m_subHeight) / (float)m_textureHeight;
		float minWidth = (m_subWidth == m_textureWidth) ? 0.0f : m_subX / (float)m_textureWidth;
		float minHeight = (m_subHeight == m_textureHeight) ? 0.0f : m_subY / (float)m_textureHeight;
		
		m_SubCoords = Vec4(maxWidth - 0.001f, maxHeight - 0.001f, minWidth, minHeight);
	}

	void Sprite::BindTexture(GLuint slot) const
	{
		m_texture->Bind(slot);
	}
}