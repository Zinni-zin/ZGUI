#include "Headers/Button.h"

#include <iostream>

#include "../Input/Input.h"

namespace ZGL
{
	RectangleI* Button::s_mouseRect = nullptr;
	int Button::s_refCount = 0;

#pragma region Constructors/Destructor

	Button::Button(Vec2 pos)
		: ZGUI(pos), m_font(nullptr), m_widgetOffset(0.f),
		m_rect(std::make_unique<RectangleI>(pos, Vec2i(0, 0))),
		m_colour(1.f), m_hoverColour(0.258f, 0.749f, 0.96f, 1.f),
		m_pressColour(0.467f, 0.926f, 0.125f, 1.f),
		m_textColour(0.f, 0.f, 0.f, 1.f), 
		m_textHoverColour(m_textColour), m_textPressColour(m_textColour)
	{ 
		if (s_refCount == 0) s_mouseRect = new RectangleI(0.f, 0.f, 1, 1);
		++s_refCount;
	}

	Button::Button(Vec2 pos, Vec2ui dimensions)
		: ZGUI(pos, dimensions), m_font(nullptr), m_widgetOffset(0.f),
		m_rect(std::make_unique<RectangleI>(pos, Vec2i(dimensions.x, dimensions.y))),
		m_colour(1.f), m_hoverColour(0.258f, 0.749f, 0.96f, 1.f),
		m_pressColour(0.467f, 0.926f, 0.125f, 1.f),
		m_textColour(0.f, 0.f, 0.f, 1.f),
		m_textHoverColour(m_textColour), m_textPressColour(m_textColour)
	{ 
		if (s_refCount == 0) s_mouseRect = new RectangleI(0.f, 0.f, 1, 1);
		++s_refCount;
	}

	Button::Button(Widget& widget, Vec2 pos)
		: ZGUI(pos + widget.GetPos()), m_font(nullptr), m_widgetOffset(pos),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(0, 0))),
		m_colour(1.f), m_hoverColour(0.258f, 0.749f, 0.96f, 1.f),
		m_pressColour(0.467f, 0.926f, 0.125f, 1.f),
		m_textColour(0.f, 0.f, 0.f, 1.f),
		m_textHoverColour(m_textColour), m_textPressColour(m_textColour)
	{
		widget.AddElement(this);

		if (s_refCount == 0) s_mouseRect = new RectangleI(0.f, 0.f, 1, 1);
		++s_refCount;
	}

	Button::Button(Widget& widget, Vec2 pos, Vec2ui dimensions)
		: ZGUI(pos + widget.GetPos(), dimensions), m_font(nullptr), m_widgetOffset(pos),
		m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y))),
		m_colour(1.f), m_hoverColour(0.258f, 0.749f, 0.96f, 1.f),
		m_pressColour(0.467f, 0.926f, 0.125f, 1.f),
		m_textColour(0.f, 0.f, 0.f, 1.f),
		m_textHoverColour(m_textColour), m_textPressColour(m_textColour)
	{
		widget.AddElement(this);

		if (s_refCount == 0) s_mouseRect = new RectangleI(0.f, 0.f, 1, 1);
		++s_refCount;

	}

	Button::~Button()
	{
		--s_refCount;

		if (s_refCount <= 0)
		{
			if (s_mouseRect)
			{
				delete s_mouseRect;
				s_mouseRect = nullptr;
			}
		}
	}

#pragma endregion

	void Button::Update()
	{
		s_mouseRect->SetPos(Input::GetInput().GetMousePos());

		if (s_mouseRect->HasIntersected(m_rect.get()))
		{
			if (!m_isPressed)
			{
				m_rect->SetColour(m_hoverColour);
				if (m_text) m_text->SetColour(m_textHoverColour);
			}

			if (Input::GetInput().IsMouseButtonPressed(Mouse::LEFT_BUTTON))
			{
				m_isPressed = true;
				m_rect->SetColour(m_pressColour);
				if (m_text) m_text->SetColour(m_textPressColour);
			}
		}
		else
		{
			m_rect->SetColour(m_colour);
			if (m_text) m_text->SetColour(m_textColour);
			m_isPressed = false;
		}

		if (m_isPressed && Input::GetInput().IsMouseButtonReleased(Mouse::LEFT_BUTTON))
		{
			if (m_eventFnc) m_eventFnc();
			m_rect->SetColour(m_colour);
			if (m_text) m_text->SetColour(m_textColour);
			m_isPressed = false;
		}
	}

	void Button::Draw(Renderer& renderer)
	{
		if (m_rect != nullptr && m_rect->GetFillTexture())
			renderer.DrawShapeFill(m_rect.get(), m_mvp);

		if (m_text != nullptr)
			renderer.DrawText(*m_text, m_mvp);

		if (m_rect != nullptr && m_rect->GetOutlineTexture())
			renderer.DrawShapeOutline(m_rect.get(), m_mvp);
	}

#pragma region Setters

	void Button::CreateText(const std::string& text, Font& font)
	{
		m_font = &font;
		m_text = std::make_unique<Text>(text, font);
		
		if (m_fontSize.x > 0 && m_fontSize.y > 0) m_text->SetFontSize(m_fontSize.x, m_fontSize.y);

		m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
			m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
	}

	void Button::SetText(const std::string& text)
	{
		if (!m_text)
		{
			std::cout << "Error Setting Text in Button(" << this << ") Must Create Text First!\n";
			return;
		}

		m_text->SetText(text);
	}

	void Button::SetFontSize(Vec2i fontSize)
	{
		m_fontSize = fontSize;

		if (m_text)
		{
			m_text->SetFontSize(m_fontSize.x, m_fontSize.y);
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void Button::SetTextColour(Vec4 colour)
	{
		if (m_text) m_text->SetColour(colour);

		m_textColour = colour;
	}

	void Button::SetTextures(std::shared_ptr<Texture2D> fill, std::shared_ptr<Texture2D> outline)
	{
		m_rect->SetSpriteTexture(fill, outline); 

		if (m_dimensions.x <= 0 || m_dimensions.y <= 0)
		{
			m_rect->SetDimensions(fill->GetWidth(), fill->GetHeight());
			m_dimensions = Vec2ui((unsigned int)fill->GetWidth(), (unsigned int)fill->GetHeight());
		}
	}

	void Button::SetTexture(std::shared_ptr<Texture2D> texture) 
	{
		m_rect->SetFillTexture(texture); 

		if (m_dimensions.x <= 0 || m_dimensions.y <= 0)
		{
			m_rect->SetDimensions(texture->GetWidth(), texture->GetHeight());
			m_dimensions = Vec2ui((unsigned int)texture->GetWidth(), (unsigned int)texture->GetHeight());
		}
	}

	void Button::SetOutlineTexture(std::shared_ptr<Texture2D> texture)
	{
		m_rect->SetOutlineTexture(texture);

		if (m_dimensions.x <= 0 || m_dimensions.y <= 0)
		{
			m_rect->SetDimensions(texture->GetWidth(), texture->GetHeight());
			m_dimensions = Vec2ui((unsigned int)texture->GetWidth(), (unsigned int)texture->GetHeight());
		}
	}

#pragma endregion

#pragma region Update Position/Dimensions

	void Button::SetPos(Vec2 pos)
	{
		m_pos = pos + m_widgetOffset;
		m_rect->SetPos(pos + m_widgetOffset);

		if (m_text)
		{
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void Button::SetPos(float x, float y)
	{
		m_pos = Vec2(x + m_widgetOffset.x, y + m_widgetOffset.y);
		m_rect->SetPos(x + m_widgetOffset.x, y + m_widgetOffset.y);
		
		if (m_text)
		{
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void Button::SetX(float x)
	{
		m_pos.x = x + m_widgetOffset.x;
		m_rect->SetPos(x + m_widgetOffset.x, m_pos.y);

		if (m_text)
			m_text->SetPosX(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2));
	}

	void Button::SetY(float y)
	{
		m_pos.y = y + m_widgetOffset.y;
		m_rect->SetPos(m_pos.x, y + m_widgetOffset.y);

		if (m_text)
			m_text->SetPosY(m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
	}

	void Button::SetDimensions(Vec2ui dimensions) 
	{ 
		m_dimensions = dimensions;
		m_rect->SetDimensions(Vec2i(dimensions.x, dimensions.y));
	}

	void Button::SetDimensions(unsigned int width, unsigned int height) 
	{
		m_dimensions = Vec2ui(width, height);
		m_rect->SetDimensions(width, height);
	}

	void Button::SetWidth(unsigned int width)
	{
		m_dimensions.x = width;
		m_rect->SetWidth(width);
	}

	void Button::SetHeight(unsigned int height) 
	{
		m_dimensions.y = height;
		m_rect->SetHeight(height);
	}

#pragma endregion
}