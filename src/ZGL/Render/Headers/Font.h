#pragma once
#include <iostream>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "../../Math/Vectors.h"

namespace ZGL
{
	class Font
	{
	public:
		Font(int width, int height);

		Font(int width, int height, int horizReso, int vertReso);

		~Font();

		void Load(const std::string& fontPath, bool isFixedSize = false);

		void SetSizeDPI(int charWidth, int charHeight, int horizReso, int vertReso);


		void SetSizeInPixel(int width, int height);
		inline void IsSizeInPixels(bool value) { m_isSizeInPixels = value; }


		inline FT_Face GetFace() const { return m_face; }
		inline Vec2i const GetSize() { return { m_width, m_height }; }
		inline Vec2i const GetResoSize() { return { m_horizReso, m_vertReso }; }
	private:
		FT_Library* m_library;
		FT_Face m_face;

		int m_width, m_height, m_horizReso, m_vertReso;
		bool m_isSizeInPixels;
	};
}