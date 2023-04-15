#include "Headers/Checkbox.h"

namespace ZGL
{
	Checkbox::Checkbox(Widget& widget, Vec2 pos)
		: ZGUI(pos + widget.GetPos()), m_widgetOffset(pos),
		m_button(std::make_unique<Button>(pos)),
		m_checkmark(std::make_unique<Sprite>(m_pos))
	{
		m_button = std::make_unique<Button>(m_pos);

		widget.AddElement(this);
		m_button->SetEventFnc(std::bind(&Checkbox::ButtonEvn, this));
		m_button->SetColour(1.f);
		m_button->SetHoverColour(Vec4(0.8f, 0.8f, 0.8f, 1.f));
		m_button->SetPressColour(1.f);
	}
	
	Checkbox::Checkbox(Widget& widget, Vec2 pos, Vec2ui dimensions)
		: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos),
		m_button(std::make_unique<Button>(m_pos, dimensions)),
		m_checkmark(std::make_unique<Sprite>(m_pos, Vec2i(dimensions.x, dimensions.y)))
	{
		m_button = std::make_unique<Button>(m_pos, dimensions);

		widget.AddElement(this);
		m_button->SetEventFnc(std::bind(&Checkbox::ButtonEvn, this));
		m_button->SetColour(1.f);
		m_button->SetHoverColour(Vec4(0.8f, 0.8f, 0.8f, 1.f));
		m_button->SetPressColour(1.f);

	}

	void Checkbox::Update()
	{
		m_button->Update();
	}

	void Checkbox::Draw(Renderer& renderer)
	{
		m_button->Draw(renderer);

		if (m_isEnabled && m_checkmark->HasTexture()) renderer.Draw(*m_checkmark, m_mvp);
	}

	void Checkbox::ButtonEvn()
	{
		m_isEnabled = !m_isEnabled;
	}

	void Checkbox::SetButtonTexture(std::shared_ptr<Texture2D> texture)
	{ 
		m_button->SetTexture(texture); 

		if (m_dimensions.x == 0 && m_dimensions.y == 0)
		{
			m_dimensions = m_button->GetDimensions();
			m_checkmark->SetWidth(m_dimensions.x);
			m_checkmark->SetHeight(m_dimensions.y);
		}
	}

	void Checkbox::SetCheckMarkTexture(std::shared_ptr<Texture2D> texture)
	{
		m_checkmark->SetTexture(texture); 

		if (m_dimensions.x == 0 && m_dimensions.y == 0)
		{
			m_dimensions = Vec2ui(m_checkmark->GetTextureWidth(), m_checkmark->GetTextureHeight());
			m_button->SetDimensions(m_dimensions);
		}
	}

#pragma region Set Positions/Dimensions

	void Checkbox::SetPos(Vec2 pos)
	{
		m_pos = pos + m_widgetOffset;
		m_button->SetPos(pos + m_widgetOffset);
		m_checkmark->SetPos(pos + m_widgetOffset);
	}

	void Checkbox::SetPos(float x, float y)
	{
		m_pos = Vec2(x + m_widgetOffset.x, y + m_widgetOffset.y);
		m_button->SetPos(x + m_widgetOffset.x, y + m_widgetOffset.y);
		m_checkmark->SetPos(x + m_widgetOffset.x, y + m_widgetOffset.y);
	}

	void Checkbox::SetX(float x)
	{
		m_pos.x = x + m_widgetOffset.x;
		m_button->SetX(x + m_widgetOffset.x);
		m_checkmark->SetPos(x + m_widgetOffset.x, m_checkmark->GetY());
	}

	void Checkbox::SetY(float y)
	{
		m_pos.y = y + m_widgetOffset.y;
		m_button->SetY(y + m_widgetOffset.y);
		m_checkmark->SetPos(m_checkmark->GetX(), y + m_widgetOffset.y);
	}

	void Checkbox::SetDimensions(Vec2ui dimensions)
	{
		m_dimensions = dimensions;
		m_button->SetDimensions(dimensions);
		m_checkmark->SetWidth(dimensions.x);
		m_checkmark->SetHeight(dimensions.y);
	}

	void Checkbox::SetDimensions(unsigned int width, unsigned int height)
	{
		m_dimensions = Vec2ui(width, height);
		m_button->SetDimensions(width, height);
		m_checkmark->SetWidth(width);
		m_checkmark->SetHeight(height);
	}

	void Checkbox::SetWidth(unsigned int width)
	{
		m_dimensions.x = width;
		m_button->SetWidth(width);
		m_checkmark->SetWidth(width);
	}

	void Checkbox::SetHeight(unsigned int height)
	{
		m_dimensions.y = height;
		m_button->SetHeight(height);
		m_checkmark->SetHeight(height);
	}

#pragma endregion
}