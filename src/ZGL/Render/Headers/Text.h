#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Font.h"

#include "../../Math/Vectors.h"

#define AVERAGE_CHAR_SIZE 0
#define MAX_CHAR_SIZE 1
#define MIN_CHAR_SIZE 2

namespace ZGL
{
	struct Character 
	{
		unsigned int TextureID;
		Vec2i Size;
		Vec2i Bearing;
		unsigned int Advance;
	};

	class Text
	{
	public:
		Text(Font& font, const Vec4& colour = Vec4(1.0f));
		Text(Font& font, Vec2 pos, const Vec4& colour = Vec4(1.0f));
		Text(const std::string& text, Font& font, const Vec4& colour = Vec4(1.0f));
		Text(const std::string& text, Font& font, float posX, float posY,
			const Vec4& colour = Vec4(1.0f));
		~Text();

		/* Geters */

		inline Font GetFont() const { return m_font; }
		inline std::string GetText() const { return m_text; }

		inline Vec2 GetPos() const { return { m_posX, m_posY }; }
		inline Vec2 GetScale() const { return m_scale; }

		inline float GetDepth() const { return m_depth; }

		inline Vec2 GetPenPos() const { return { m_penX, m_penY }; }

		inline Vec4 GetColour() const { return m_colour; }

		void CalcSize() { m_size = GetSize(AVERAGE_CHAR_SIZE); }
		Vec2 GetSize() const { return m_size; }
		Vec2 GetSize(int SizeType);
		float GetPosX() const { return m_posX; }
		float GetHeight() const { return m_size.y; }

		float GetWidth(int textIndex);
		float GetWidth();

		float GetHeight(int SizeType) const;

		int GetIndex(float x) const;

		int GetVertOffset() const { return m_vertOffset; }

		inline FT_GlyphSlot GetGlyphSlot() const { return m_glyphSlot; }

		inline std::map<char, Character> GetCharacters() const { return m_characters; }

		/* Setters */

		
		void SetPosX(float posX) { m_posX = posX; }
		void SetPosY(float posY) { m_posY = posY; }
		void SetPos(float posX, float posY) { m_posX = posX; m_posY = posY; }
		void SetPos(const Vec2& pos) { m_posX = pos.x; m_posY = pos.y; }

		void SetPenX(float penX) { m_penX = penX; }
		void SetPenY(float penY) { m_penY = penY; }
		void SetPenPos(float penX, float penY) { m_penX = penX; m_penY = penY; }

		void SetText(const std::string& text);

		void SetColour(Vec4 colour) { m_colour = colour; };
		void SetColour(float r, float g, float b, float a) { m_colour = Vec4(r, g, b, a); }
	
		void SetFontSize(int width, int height, int SizeType = 0);
		void SetScale(float x, float y, int SizeType = 0) { m_scale.x = x, m_scale.y = y; m_size = GetSize(SizeType); }

		void SetDepth(float depth)
		{
			if (depth >= 0.0f && depth <= 1.0f) m_depth = depth;
			else std::wcout << "Error in Text: Depth must be between 0.0f and 1.0f\n";
		}

		void ResetScale() { m_scale = Vec2::One(); }

		void operator+=(const std::string& rhs)
		{
			SetText(m_text + rhs);
		}

		void operator+=(const char rhs)
		{
			SetText(m_text + rhs);
		}

	private:
		std::string m_text;
		
		Font m_font;

		FT_GlyphSlot m_glyphSlot;
		FT_Error m_ftError;

		Vec4 m_colour;

		// Initial positio
		float m_posX, m_posY;

		float m_depth = 0.0f;

		// Position used when drawing
		float m_penX, m_penY;

		Vec2 m_scale = Vec2::One(); // Scale for the font

		Vec2 m_size = Vec2::One();

		int m_vertOffset;

		std::map<char, Character> m_characters;
		std::map<int, float> m_cachedWidth;

		void SetupBitmap();
	};
}