#pragma once

#include "ZGUI.h"
#include "Widget.h"

#include "../../Input/Input.h"
#include "../../Shapes/Headers/Rectangle.h"

namespace ZGL
{
	template<typename T>
	class Slider : public ZGUI
	{
	public:
		Slider(Widget& widget, Vec2 pos) 
			: ZGUI(pos + widget.GetPos()), m_widgetOffset(pos), m_widet(widget),
			m_sliderDimensions(0, 0),
			m_rect(std::make_unique<RectangleI>(pos, Vec2i(0, 0))),
			m_slider(std::make_unique<RectangleI>(pos, Vec2i(0, 0))),
			m_font(nullptr), m_minVal((T)0), m_val((T) 0), m_maxVal((T)100)
		{
		}
		
		Slider(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions = Vec2ui(6, 0)) 
			: ZGUI(pos + widget.GetPos(), dimensions), m_widgetOffset(pos), m_widet(widget),
			m_sliderDimensions(sliderDimensions),
			m_rect(std::make_unique<RectangleI>(m_pos, Vec2i(dimensions.x, dimensions.y))),
			m_slider(std::make_unique<RectangleI>(m_pos, Vec2i(sliderDimensions.x, sliderDimensions.y))),
			m_font(nullptr), m_minVal((T)0), m_val((T)0), m_maxVal((T)100)
		{
			if (sliderDimensions.y == 0)
			{
				sliderDimensions.y = dimensions.y;
				m_slider->SetHeight(dimensions.y);
			}
		}

		virtual void CalculateVal() = 0;

		/* Getters */

		T GetMaxVal() const { return m_maxVal; }
		T GetMinVal() const { return m_minVal; }
		T GetVal() const { return m_val; }

		bool GetPressed() const { return m_isPressed; }
		bool HasToBeInBorder() const { return m_hasToBeInBorder; }
		bool IsAbleToExendBorder() const { return m_isAbleToExtendBorder; }
		bool IsCenteredY() const { return m_isCenterY; }

		const RectangleI& GetRect() const { return *m_rect; }

		/* Setters */

		void SetMaxVal(T val) 
		{
			if (val <= m_minVal)
			{
				std::cout << "Error setting max val in slider(" << this << "): " <<
					"Max(" << val << ") can't be less than or equal to Max(" << m_minVal << ")\n";
				return;
			}

			m_maxVal = val;
		}
		
		void SetMinVal(T val) 
		{
			if (val >= m_maxVal)
			{
				std::cout << "Error setting min val in slider(" << this << "): " <<
					"Min(" << val << ") can't be greater than or equal to Max(" << m_maxVal << ")\n";
				return;
			}

			m_minVal = val; 
			if (m_text) 
			{
				m_text->SetText(std::to_string(m_minVal));
				m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2.f),
					m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
			}
		}
		
		virtual void SetVal(T val) = 0;

		void IsCenteredY(bool val) { m_isCenterY = val; }

		void SetColour(Vec4 colour) { m_rect->SetColour(colour); }
		void SetSliderColour(Vec4 colour) { m_slider->SetColour(colour); }

	public:

		void Update() override
		{
			if (m_isPressed)
			{
				CalculatePosition(m_widet.GetMouseRect().GetX());
				CalculateVal();

				if (m_text)
				{
					m_text->SetText(std::to_string(m_val));
					m_text->SetPosX(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2));
				}
			}

			if (m_widet.GetMouseRect().HasIntersected(m_slider.get()) && Input::GetInput().IsMouseButtonPressed(Mouse::LEFT_BUTTON))
				m_isPressed = true;

			if (m_isPressed && Input::GetInput().IsMouseButtonReleased(Mouse::LEFT_BUTTON))
				m_isPressed = false;
		}

		void Draw(Renderer& renderer) override
		{
			if (m_rect && m_rect->GetFillTexture()) renderer.DrawShapeFill(m_rect.get(), m_mvp);
			if (m_text) renderer.DrawText(*m_text, m_mvp);
			if (m_slider) renderer.DrawShapeFill(m_slider.get(), m_mvp);
			if (m_rect && m_rect->GetOutlineTexture()) renderer.DrawShapeOutline(m_rect.get(), m_mvp);
		}

		void CalculatePosition(float x)
		{
			m_slider->SetX(x);

			if (m_hasToBeInBorder && m_slider->GetRight() >= m_pos.x + m_rect->GetWidth())
				m_slider->SetX((m_pos.x + m_rect->GetWidth()) - m_slider->GetWidth());
			else if (m_hasToBeInBorder && m_slider->GetX() <= m_pos.x)
				m_slider->SetX(m_pos.x);

			if(!m_hasToBeInBorder && m_slider->GetRight() - (m_slider->GetWidth() / 2.f) >= m_pos.x + m_dimensions.x)
				m_slider->SetX((m_pos.x + m_dimensions.x) - (m_slider->GetWidth() / 2.f));
			else if (!m_hasToBeInBorder && m_slider->GetX() <= m_pos.x - (m_slider->GetWidth() / 2.f))
				m_slider->SetX(m_pos.x - (m_slider->GetWidth() / 2.f));
		}

		void SetRectCenterY()
		{
			m_isCenterY = true;
			m_rect->SetY(m_pos.y + ((m_slider->GetHeight() / 2.f) - (m_dimensions.y / 2.f)));
		}

	public:
		// If the slider doesn't have to be in border we calculate the value by the center
		// Otherwise we use the top left corner
		void HasToBeInBorder(bool val)
		{
			m_hasToBeInBorder = val;

			if (!m_hasToBeInBorder) m_slider->SetX(m_pos.x - (m_slider->GetWidth() / 2.f));
		}

		// If we can extend the border we extend it by half the slider width
		// Otherwise it will work but won't give as much values
		void IsAbleToExendBorder(bool val)
		{
			if (m_isAbleToExtendBorder) m_rect->SetWidth(m_dimensions.x + (m_slider->GetWidth() / 2));
			else m_rect->SetWidth(m_dimensions.x);

			m_isAbleToExtendBorder = val;
		}

		void SetRectTexture(std::shared_ptr<Texture2D> texture)
		{
			m_rect->SetFillTexture(texture);

			if (m_dimensions.x == 0 || m_dimensions.y == 0)
			{
				m_dimensions.x = texture->GetWidth();
				m_dimensions.y = texture->GetHeight();
			}

			// Extend the dimensions so the rect can fit the slider
			if (m_isAbleToExtendBorder && m_slider->GetWidth() > 0)
			{
				int width = m_dimensions.x;
				width += (m_hasToBeInBorder) ? m_slider->GetWidth() : m_slider->GetWidth() / 2;
				m_rect->SetWidth(width);
			}

			if (m_isCenterY) SetRectCenterY();
		}

		void SetRectOutline(std::shared_ptr<Texture2D> texture)
		{
			m_rect->SetOutlineTexture(texture);

			if (m_dimensions.x == 0 || m_dimensions.y == 0)
			{
				m_dimensions.x = texture->GetWidth();
				m_dimensions.y = texture->GetHeight();
			}

			// Extend the dimensions so the rect can fit the slider
			if (m_isAbleToExtendBorder && m_slider->GetWidth() > 0)
			{
				int width = m_dimensions.x;
				width += (m_hasToBeInBorder) ? m_slider->GetWidth() : m_slider->GetWidth() / 2;
				m_rect->SetWidth(width);
			}

			if (m_isCenterY) SetRectCenterY();
		}

		void SetSliderTexture(std::shared_ptr<Texture2D> texture)
		{
			m_slider->SetFillTexture(texture);

			if (m_slider->GetWidth() <= 0 || m_slider->GetHeight() <= 0)
			{
				m_sliderDimensions.x = texture->GetWidth();
				m_sliderDimensions.y = texture->GetHeight();
				m_slider->SetDimensions(texture->GetWidth(), texture->GetHeight());
			}
			
			if (m_isAbleToExtendBorder)
			{
				int width = m_dimensions.x;
				width += (m_hasToBeInBorder) ? m_slider->GetWidth() : m_slider->GetWidth() / 2;
				m_rect->SetWidth(width);
			}

			if (m_isCenterY) SetRectCenterY();
		}

		// Call this function if you want the value to be displayed in the center of the slider
		void SetFont(Font& font)
		{
			m_font = &font;
			m_text = std::make_unique<Text>(std::to_string(m_minVal), font);
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2.f),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}

		// Call this function if you want the value to be displayed in the center of the slider and if you want to set the size right away
		void SetFont(Font& font, Vec2i fontSize)
		{
			m_font = &font;
			m_text = std::make_unique<Text>(std::to_string(m_minVal), font);
			m_text->SetFontSize(fontSize.x, fontSize.y);
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}

		void SetFontSize(Vec2i fontSize)
		{
			if (m_text)
			{
				m_text->SetFontSize(fontSize.x, fontSize.y);
				m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
					m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
			}
		}

		void SetTextColour(Vec4 colour)
		{
			if (m_text) m_text->SetColour(colour);
			else 
				std::cout << "Error setting text colour in slider(" << this << "): " << 
				"Font has to be set first\n";
		}

#pragma region Update Pos/Dimensions
		void SetPos(Vec2 pos) override
		{
			Vec2 sliderOffset = m_slider->GetPos() - m_pos;

			m_pos = pos + m_widgetOffset;
			m_rect->SetPos(pos + m_widgetOffset);
			m_slider->SetPos(sliderOffset + pos + m_widgetOffset);
			if (m_isCenterY) SetRectCenterY();

			if(m_text)
				m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
					m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}

		void SetPos(float x, float y) override
		{
			Vec2 sliderOffset = m_slider->GetPos() - m_pos;

			m_pos = Vec2(x + m_widgetOffset.x, y + m_widgetOffset.y);
			m_rect->SetPos(x + m_widgetOffset.x, y + m_widgetOffset.y);
			m_slider->SetPos(sliderOffset.x + x + m_widgetOffset.x,
				sliderOffset.y + y + m_widgetOffset.y);
		
			if (m_isCenterY) SetRectCenterY();

			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}

		void SetX(float x) override
		{
			float sliderOffset = m_slider->GetPos().x - m_pos.x;

			m_pos.x = x + m_widgetOffset.x;
			m_rect->SetPos(x + m_widgetOffset.x, m_pos.y);
			m_slider->SetX(sliderOffset + x + m_widgetOffset.x);
			if (m_isCenterY) SetRectCenterY();

			if (m_text)
				m_text->SetPosX(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2));
		}

		void SetY(float y) override
		{
			float sliderOffset = m_slider->GetPos().y - m_pos.y;

			m_pos.y = y + m_widgetOffset.y;
			m_rect->SetPos(m_pos.x, y + m_widgetOffset.y);
			m_slider->SetY(sliderOffset + y + m_widgetOffset.y);

			if(m_isCenterY) SetRectCenterY();

			if (m_text)
				m_text->SetPosY(m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
#pragma endregion
	
	protected:
		T m_maxVal;
		T m_minVal;
		T m_val;

		Widget& m_widet;

		Vec2 m_widgetOffset;
		Vec2ui m_sliderDimensions;

		std::unique_ptr<RectangleI> m_rect;
		std::unique_ptr<RectangleI> m_slider;

		std::unique_ptr<Text> m_text;
		Font* m_font;

		bool m_isPressed = false;
		bool m_hasToBeInBorder = true;
		bool m_isAbleToExtendBorder = true;
		bool m_isCenterY = false;
	};

#pragma region Typed Sliders
	class SliderInt : public Slider<int>
	{
	public:
		SliderInt(Widget& widget, Vec2 pos);
		SliderInt(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions = Vec2ui(6, 0));

		void CalculateVal() override;
		void SetVal(int val) override;
	} typedef SliderI;

	class SliderFloat : public Slider<float>
	{
	public:
		SliderFloat(Widget& widget, Vec2 pos);
		SliderFloat(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions = Vec2ui(6, 0));
		
		void Update() override;

		void CalculateVal() override;
		void SetVal(float val) override;

		inline void SetPercision(unsigned int percision) { m_percision = percision; }

	private:
		unsigned int m_percision = 2;
	} typedef SliderF;

	class SliderDouble : public Slider<double>
	{
	public:
		SliderDouble(Widget& widget, Vec2 pos);
		SliderDouble(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions = Vec2ui(6, 0));

		void Update() override;

		void CalculateVal() override;
		
		void SetVal(double val) override;

		inline void SetPercision(unsigned int percision) { m_percision = percision; }

	private:
		unsigned int m_percision = 2;
	} typedef SliderD;
#pragma endregion
}