#include "Headers/Font.h"
#include "../Core/Core.h"

#include "freetype/ftglyph.h"
#include "freetype/ftsizes.h"

namespace ZGL
{
	Font::Font(int width, int height)
		: m_width(width), m_height(height),
		m_isSizeInPixels(true), m_library(&Core::GetCore().GetFT_Library())
	{
		if (m_library == nullptr) 
			std::cout << "Error: Core must be initialized!\n";
	}

	Font::Font(int width, int height, int horizReso, int vertReso)
		: m_width(width), m_height(height), 
		m_isSizeInPixels(false), m_library(&Core::GetCore().GetFT_Library())
	{
		if (m_library == nullptr)
			std::cout << "Error: Core must be initialized!\n";
	}
	
	Font::~Font()
	{ }

	void Font::Load(const std::string& filePath, bool isFixedSize)
	{
		FT_Error error = FT_New_Face(*m_library, filePath.c_str(), 0, &m_face); // Load in the font

		if (error == FT_Err_Unknown_File_Format)
		{
			std::cout << "Error Loading Font: unknown file format!\n";
			return;
		}
		else if (error)
		{
			std::cout << "Error Loading Font: File could not be opened/read!\n";
			return;
		}

		if (m_isSizeInPixels) SetSizeInPixel(m_width, m_height);
		else SetSizeDPI(m_width, m_height, m_horizReso, m_vertReso);
	}

	void Font::SetSizeDPI(int charWidth, int charHeight, int horizReso, int vertReso)
	{
		if (charWidth <= 0 && charHeight <= 0)
		{
			std::cout << "Error setting Font Size in DPI: Width and Height must be greater than 0!\n";
			return;
		}

		FT_Error error = FT_Set_Char_Size(m_face, charWidth, charHeight, horizReso, vertReso);

		if (error)
		{
			std::cout << "Error Setting Font Size in DPI: Might be setting a fixed size font\n";
			return;
		}

		m_width = charWidth;
		m_height = charHeight;
	}

	void Font::SetSizeInPixel(int width, int height)
	{
		if (width <= 0 && height <= 0)
		{
			std::cout << "Error setting Fonst Size in Pixels: " << 
				"width and height must be greater than 0!\n";

			return;
		}

		FT_Error error = FT_Set_Pixel_Sizes(m_face, width, height);
		if (error)
		{
			std::cout << "Error Setting Font Size in Pixels: Might be setting a fixed size font\n";
			return;
		}

		m_width = width;
		m_height = height;
	}
}