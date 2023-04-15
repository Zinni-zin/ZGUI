#include "Headers/Text.h"
#include "../../Vendor/stb_image/stb_image.h"

#include <vector>

#include "GL/glew.h"

namespace ZGL
{

	Text::Text(Font& font, const Vec4& colour)
		: m_font(font), m_posX(0), m_posY(0), m_penX(m_posX), m_penY(m_posY), m_colour(colour)
	{
		if (m_font.GetFace() == nullptr)
			std::cout << "Error font face is null!\n";
		else
		{ 
			m_glyphSlot = m_font.GetFace()->glyph;
			SetupBitmap();
			FT_Load_Char(m_font.GetFace(), 'H', FT_LOAD_RENDER);
			m_vertOffset = m_glyphSlot->bitmap_top;
		}
	}

	Text::Text(Font& font, Vec2 pos, const Vec4& colour)
		: m_font(font), m_posX(pos.x), m_posY(pos.y), m_penX(m_posX), m_penY(m_posY), m_colour(colour)
	{
		if (m_font.GetFace() == nullptr)
			std::cout << "Error font face is null!\n";
		else
		{
			m_glyphSlot = m_font.GetFace()->glyph;
			SetupBitmap();
			FT_Load_Char(m_font.GetFace(), 'H', FT_LOAD_RENDER);
			m_vertOffset = m_glyphSlot->bitmap_top;
		}
	}

	Text::Text(const std::string& text, Font& font, const Vec4& colour)
		: m_text(text), m_font(font), m_posX(0), m_posY(0), 
		m_penX(m_posX), m_penY(m_posY), m_colour(colour)
	{ 
		if (m_font.GetFace() == nullptr) 
			std::cout << "Error font face is null!\n";
		else
		{
			m_glyphSlot = m_font.GetFace()->glyph;
			SetupBitmap();
			FT_Load_Char(m_font.GetFace(), 'H', FT_LOAD_RENDER);
			m_vertOffset = m_glyphSlot->bitmap_top;
		}
	}

	Text::Text(const std::string& text, Font& font, float posX, float posY, const Vec4& colour)
		: m_text(text), m_font(font), m_posX(posX), m_posY(posY), 
		m_penX(m_posX), m_penY(m_posY), m_colour(colour)
	{
		if (m_font.GetFace() == nullptr)
			std::cout << "Error font face is null!\n";
		else
		{
			m_glyphSlot = m_font.GetFace()->glyph;
			SetupBitmap();
			FT_Load_Char(m_font.GetFace(), 'H', FT_LOAD_RENDER);
			m_vertOffset = m_glyphSlot->bitmap_top;
		}
	}

	Text::~Text()
	{
		for (char c : m_text)
			glDeleteTextures(1, &m_characters[c].TextureID);
		m_characters.clear();
	}

	void Text::SetText(const std::string& text)
	{
		for (char c : m_text) 
			glDeleteTextures(1, &m_characters[c].TextureID);
		m_characters.clear();
		 
		m_text = text;
		SetupBitmap();
		m_cachedWidth.clear();
	}

	Vec2 Text::GetSize(int SizeType)
	{
		Vec2 size(0.0f);

		std::vector<float> heights;
		heights.reserve(4);
		heights.emplace_back(0.0f);

		int heightIndex = 0; 
		int heightCount = 0;
		bool hasNewLine = false;
		
		for(int i = 0; (size_t)i < m_text.size(); ++i)
		{
			char c = m_text[i];

			hasNewLine = false;

			size.x += GetWidth(i); // Addon to our x value

			if (SizeType == AVERAGE_CHAR_SIZE)
			{
				heights[heightIndex] += (float)GetCharacters()[c].Size.y;
				++heightCount;
			}
			else  if (SizeType == MAX_CHAR_SIZE && heights[heightIndex] < GetCharacters()[c].Size.y)
				heights[heightIndex] = (float)GetCharacters()[c].Size.y;
			else  if (SizeType == MIN_CHAR_SIZE && (heights[heightIndex] == 0 || heights[heightIndex] > GetCharacters()[c].Size.y))
				heights[heightIndex] = (float)GetCharacters()[c].Size.y;

			if (c == '\n')
			{
				if (SizeType == AVERAGE_CHAR_SIZE)
				{
					heights[heightIndex] /= heightCount;
					heightCount = 0;
					hasNewLine = true;
				}

				heights.emplace_back(0.0f);
				++heightIndex;
			}
		}

		if (SizeType == AVERAGE_CHAR_SIZE && !hasNewLine)
			heights[heightIndex] /= heightCount;

		for (size_t i = 0; i < heights.size(); ++i)
			size.y += heights[i] * m_scale.y;

		return size;
	}

	float Text::GetWidth(int textIndex)
	{
		float width = 0.f;
		float penX = 0.f;
		float pos = 0.f;
		float nextPos = 0.f;
		float returnPos = 0.f;

		if (m_cachedWidth.find(textIndex) != m_cachedWidth.end())
			return m_cachedWidth[textIndex];

		for (int i = 0; (size_t)i < m_text.size(); ++i)
		{
			if (textIndex >= 0 && i >= textIndex)
			{
				m_cachedWidth[textIndex] = returnPos;
				return returnPos;
			}

			width = penX + ((float)GetCharacters()[m_text[i]].Bearing.x) * GetScale().x;
			width += GetCharacters()[m_text[i]].Size.x * GetScale().x;

			// Figure out the starting position of the next character 
			// (if there is no next character just add some offset to the final width)
			nextPos = (i + 1 == m_text.size()) ? width + (3.5f * GetScale().x) : 
				(penX + (GetCharacters()[m_text[i]].Advance >> 6) * GetScale().x) + 
				((float)GetCharacters()[m_text[i + 1]].Bearing.x * GetScale().x);

			// Figure out the mid-distance thing between two characters 
			// Different for spaces because it looks weird otherwise so a small offset is used
			returnPos = (m_text[i] == ' ')? width + ((nextPos-width) - (1.5f * GetScale().x)) :
				width + ((nextPos - width) / 2);

			// Get to the next 'drawing position'
			penX = penX + (GetCharacters()[m_text[i]].Advance >> 6) * GetScale().x;
		}

		m_cachedWidth[textIndex] = returnPos;
		return returnPos;
	}

	float Text::GetWidth()
	{
		return GetWidth(m_text.size() - 1);
	}

	float Text::GetHeight(int SizeType) const
	{
		float height = 0.f;

		std::vector<float> heights;
		heights.emplace_back(0.0f);

		int heightIndex = 0; 
		int heightCount = 0; 
		bool hasNewLine = false; 
		for (char c : m_text)
		{
			hasNewLine = false;

			if (SizeType == AVERAGE_CHAR_SIZE)
			{
				heights[heightIndex] += (float)GetCharacters()[c].Size.y;
				++heightCount;
			}
			else  if (SizeType == MAX_CHAR_SIZE && heights[heightIndex] < GetCharacters()[c].Size.y)
				heights[heightIndex] = (float)GetCharacters()[c].Size.y;
			else  if (SizeType == MIN_CHAR_SIZE && (heights[heightIndex] == 0 || heights[heightIndex] > GetCharacters()[c].Size.y))
				heights[heightIndex] = (float)GetCharacters()[c].Size.y;

			if (c == '\n')
			{
				if (SizeType == AVERAGE_CHAR_SIZE)
				{
					heights[heightIndex] /= heightCount;
					heightCount = 0;
					hasNewLine = true;
				}

				heights.emplace_back(0.0f);
				++heightIndex;
			}
		}

		if (SizeType == AVERAGE_CHAR_SIZE && !hasNewLine)
			heights[heightIndex] /= heightCount;

		// Add all our newline heights together
		for (size_t i = 0; i < heights.size(); ++i)
			height += heights[i] * m_scale.y;

		return height;
	}

	int Text::GetIndex(float x) const
	{
		float posX = m_posX;
		float penX = m_posX;
		int index = 0;
		for (int i = 0; (size_t)i < m_text.size(); ++i)
		{

			if (posX >= x) return i;
			posX = penX + ((float)GetCharacters()[m_text[i]].Bearing.x) * GetScale().x; // Addon to our pos
			posX += GetCharacters()[m_text[i]].Size.x * GetScale().x;

			// If it is a space we want to get the distance between 
			// the characters before and after the space
			if (m_text[i] == ' ')
			{  
				float nextPos = (i + 1 == m_text.size()) ? posX + (3.5f * GetScale().x) :
					(penX + (GetCharacters()[m_text[i]].Advance >> 6) * GetScale().x) +
					((float)GetCharacters()[m_text[i + 1]].Bearing.x * GetScale().x);

				posX = nextPos;
			}

			penX = penX + (GetCharacters()[m_text[i]].Advance >> 6) * m_scale.x;

			index = i;
			if ((size_t)index >= m_text.size() - 1) index = m_text.size();
		}
			
		return index;
	}

	void Text::SetFontSize(int width, int height, int SizeType)
	{
		m_scale.x = (float)width / (float)m_font.GetSize().x;
		m_scale.y = (float)height / (float)m_font.GetSize().y;

		m_size = GetSize(SizeType);
	}

	void Text::SetupBitmap()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (char c : m_text)
		{ 
			if (m_characters.find(c) != m_characters.end()) 
				continue;

			if (FT_Load_Char(m_font.GetFace(), c, FT_LOAD_RENDER))
			{
				std::cout << "Error loading text: freetype failed to load glyph\n";
				continue;
			}

			unsigned int texture;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				m_glyphSlot->bitmap.width, m_glyphSlot->bitmap.rows, 0,
				GL_RED, GL_UNSIGNED_BYTE, m_glyphSlot->bitmap.buffer);

			glBindTexture(GL_TEXTURE_2D, 0);

			Character character = {
				texture,
				Vec2i(m_glyphSlot->bitmap.width, m_glyphSlot->bitmap.rows),
				Vec2i(m_glyphSlot->bitmap_left, m_glyphSlot->bitmap_top),
				(unsigned int)m_glyphSlot->advance.x
			};

			m_characters.insert(std::pair<char, Character>(c, character));
		}
	}
}