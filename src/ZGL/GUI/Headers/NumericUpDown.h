#pragma once

#include "ZGUI.h"
#include "Widget.h"
#include "Button.h"
#include "Textbox.h"

namespace ZGL
{
	enum class ButtonPosition { LEFT, RIGHT, UP_DOWN };

	template <typename T>
	class NumericUpDown : public ZGUI
	{
	private:
		Widget& m_widget;

	public:
		// Note: buttonPos refers to where to automatically align the buttons compared to the text box
		NumericUpDown(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions, 
			Vec2ui buttonDimensions = Vec2ui(0), ButtonPosition buttonPos = ButtonPosition::RIGHT) 
			: ZGUI(pos + widget.GetPos(), dimensions), m_widget(widget), m_widgetOffset(pos),
			m_buttonDimensions(buttonDimensions), m_buttonPosition(buttonPos),
			m_minVal((T)0), m_maxVal((T)100), m_val((T)0), m_increment((T)1)
		{
			m_numBox = std::make_unique<Textbox>(widget, font, pos, dimensions, true);

			if (buttonDimensions.x == 0 || buttonDimensions.y == 0)
			{
				m_upButton = std::make_unique<Button>(m_pos, dimensions);
				m_downButton = std::make_unique<Button>(m_pos, dimensions);
			}
			else
			{
				m_upButton = std::make_unique<Button>(m_pos, buttonDimensions);
				m_downButton = std::make_unique<Button>(m_pos, buttonDimensions);
			}

			m_numBox->SetNumbersOnly(true);
			m_numBox->SetNumCharacters(true);
			m_numBox->SetText(std::to_string(m_minVal));
			
			m_downButton->FlipSpriteVertically();
			
			SetTextX();

			m_upButton->SetEventFnc(std::bind(&NumericUpDown::OnUpButtonPressed, this));
			m_downButton->SetEventFnc(std::bind(&NumericUpDown::OnDownButtonPressed, this));
			m_numBox->SetOnTextChangedEventFnc(std::bind(&NumericUpDown::OnTextChange, this));

			m_numBox->SetWidth(dimensions.x);

			widget.AddElement(this);

			m_numBox->SetOnChangeOnEnter(true);
		}

		void Update() override 
		{
			m_numBox->Update();
			if (m_upButton->GetMouseRect()) m_numBox->Update(*m_upButton->GetMouseRect());
			m_upButton->Update(); m_downButton->Update();
		}

		void Draw(Renderer& renderer) override
		{
			m_upButton->Draw(renderer);
			m_numBox->Draw(renderer);
			m_downButton->Draw(renderer);
		}

	public:
		void PrintTextboxDimensions()
		{
			std::cout << "Textbox Dimensions: " << m_numBox->GetRectWidth() << ", "
				<< m_numBox->GetRectHeight() << "\n";

			std::cout << "Textbox Scissor Rect: " << m_numBox->GetScissorWidth() << ", "
				<< m_numBox->GetScissorHeight() << "\n";

			std::cout << "Widget Scissor Rect: " << m_widget.GetScissorRect().GetWidth() << ", "
				<< m_widget.GetScissorRect().GetHeight() << "\n";

			std::cout << "Widget Rect: " << m_widget.GetWidth() << ", " <<
				m_widget.GetHeight() << "\n";
		}

		/* Setters */

		void SetMaxVal(T val)
		{
			if (val <= m_minVal)
			{
				std::cout << "Error setting max val in NumericUpDown(" << this << "): " <<
					"Max(" << val << ") can't be less than or equal to Min(" << m_minVal << ")\n";
				return;
			}

			m_maxVal = val;
		}

		void SetMinVal(T val) 
		{
			if (val >= m_maxVal)
			{
				std::cout << "Error setting min val in NumericUpDown(" << this << "): " <<
					"Min(" << val << ") can't be greater than or equal to Max(" << m_maxVal << ")\n";
				return;
			}

			m_minVal = val;
		}

		void SetVal(T val)
		{
			if (val < m_minVal || val > m_maxVal)
			{
				std::cout << "Error setting val(" << val << ") in NumericUpDown(" << this << "):\n" <<
					"Val can't less than min val(" << m_minVal << ") " << 
					"and can't be greater than max val(" << m_maxVal << ")\n";
				return;
			}

			m_val = val;
		}

		void SetIncrement(T val)
		{
			if (val == 0)
			{
				std::cout << "Error setting increment in NumericUpDown(" << this << "): " <<
					"Increment(" << val << ") can't be 0\n";
				return;
			}

			m_increment = val;
		}

		void SetForcedIncrement(bool val) { m_isForcedIncrement = val; }

		void SetPos(Vec2 pos) override
		{
			m_pos = pos + m_widgetOffset;
			m_numBox->SetPos(pos);
			CalcButtonPosition();
		}

		void SetPos(float x, float y) override
		{
			m_pos = Vec2(x + m_widgetOffset.x, y + m_widgetOffset.y);
			m_numBox->SetPos(x, y);
			CalcButtonPosition();
		}

		void SetX(float x) override
		{
			m_pos.x = x + m_widgetOffset.x;
			m_numBox->SetX(x);
			CalcButtonPosition();
		}

		void SetY(float y) override
		{
			m_pos.y = y + m_widgetOffset.y;
			m_numBox->SetY(y);
			CalcButtonPosition();
		}

		void SetTextboxDimensions(Vec2ui dimensions)
		{
			m_numBox->SetDimensions(dimensions);
			CalcButtonPosition();
		}

		void SetTextboxWidth(unsigned int width)
		{
			m_numBox->SetWidth(width);
			CalcButtonPosition();
		}

		void SetTextboxHeight(unsigned int height)
		{
			m_numBox->SetHeight(height);
			CalcButtonPosition();
		}

		void SetButtonDimensions(Vec2ui dimensions)
		{
			m_buttonDimensions = dimensions;
			m_upButton->SetDimensions(dimensions); m_downButton->SetDimensions(dimensions);
			CalcButtonPosition();
		}

		void SetButtonWidth(unsigned int width) 
		{ 
			m_upButton->SetWidth(width); m_downButton->SetWidth(width);
			m_buttonDimensions.x = width;
			CalcButtonPosition();
		}

		void SetButtonHeight(unsigned int height)
		{ 
			m_upButton->SetHeight(height); m_downButton->SetHeight(height);
			m_buttonDimensions.y = height;
			CalcButtonPosition();
		}

		void SetButtonTexture(std::shared_ptr<Texture2D> texture) 
		{ 
			m_upButton->SetTexture(texture); m_downButton->SetTexture(texture);

			if (m_buttonDimensions.x == 0 || m_buttonDimensions.y == 0)
				m_buttonDimensions = m_upButton->GetDimensions();

			CalcButtonPosition(); 
		}

		void SetButtonOutline(std::shared_ptr<Texture2D> texture)
		{ m_upButton->SetOutlineTexture(texture); m_downButton->SetTexture(texture);  CalcButtonPosition(); }

		void SetTextboxTexture(std::shared_ptr<Texture2D> texture)
		{
			m_numBox->SetFillTexture(texture); /*CalcButtonPosition();*/
		
			if (m_buttonDimensions.x > 0 || m_buttonDimensions.y > 0)
				CalcButtonPosition();
		}

		void SetTextboxOutline(std::shared_ptr<Texture2D> texture)
		{ m_numBox->SetOutlineTexture(texture); }

		void SetTextboxMouseCursor(std::shared_ptr<Texture2D> texture)
		{ m_numBox->SetMouseCursorTexture(texture); }
		
		void SetTextboxCursor(std::shared_ptr<Texture2D> texture) 
		{ m_numBox->SetTextCursorTexture(texture); }

		void SetTextboxHighlightTexture(std::shared_ptr<Texture2D> texture)
		{ m_numBox->SetHighlightRectTexture(texture); }

	private:
		void CalcPosition(Vec2 pos)
		{
			std::cout << "Pos: " << m_pos << std::endl;

			if (m_buttonPosition == ButtonPosition::RIGHT)
			{
				m_numBox->SetPos(m_pos);
				CalcButtonPosition();
			}
			else if (m_buttonPosition == ButtonPosition::LEFT)
			{
				m_upButton->SetPos(m_pos);
				m_upButton->SetHeight(m_numBox->GetHeight() / 2);

				m_downButton->SetPos(m_pos.x, m_pos.y + m_upButton->GetHeight());
				m_downButton->SetHeight(m_numBox->GetHeight() / 2);

				m_numBox->SetPos(m_pos.x + m_upButton->GetWidth() + 1.f, pos.y);
			}
			else
			{
				m_upButton->SetPos(m_pos);
				
				m_numBox->SetPos(m_pos.x, m_pos.y + m_upButton->GetHeight() + 1.f);

				m_downButton->SetPos(m_pos.x, m_numBox->GetY() + m_numBox->GetHeight() + 1.f);
			}
		}

		void CalcButtonPosition()
		{
			float xPos = m_numBox->GetX() + m_numBox->GetWidth() + 1.f;
			float yPos = m_numBox->GetY();
			unsigned int height = m_numBox->GetHeight() / 2;
			unsigned int downHeightOffset = 0;

			if (m_buttonPosition == ButtonPosition::LEFT)
			{
				xPos = m_pos.x;
				yPos = m_pos.y;

				m_numBox->SetX(xPos + m_upButton->GetWidth() - m_widgetOffset.x + 1.f);
			}
			else if(m_buttonPosition == ButtonPosition::UP_DOWN)
			{
				height = m_buttonDimensions.y;
				xPos = m_pos.x;
				yPos = m_pos.y;

				downHeightOffset = m_upButton->GetHeight() + m_numBox->GetHeight() - height;

				m_numBox->SetY(yPos + m_upButton->GetHeight() - m_widgetOffset.y + 1.f);
				m_upButton->SetWidth(m_numBox->GetWidth());
				m_downButton->SetWidth(m_numBox->GetWidth());
			}

			m_upButton->SetPos(xPos, yPos);
			m_upButton->SetHeight(height);

			m_downButton->SetPos(xPos, yPos + height + downHeightOffset);
			m_downButton->SetHeight(height);
		}

		void SetTextX()
		{
			m_numBox->SetTextOffsetX(7.f);	
		}

	protected:
		virtual void OnTextChange() = 0;

		virtual void OnUpButtonPressed()
		{
			if (m_val >= m_maxVal) return;

			m_val += m_increment;
			m_numBox->SetText(std::to_string(m_val));
		}

		virtual void OnDownButtonPressed()
		{
			if (m_val <= m_minVal) return;

			m_val -= m_increment;
			m_numBox->SetText(std::to_string(m_val));
		}

	protected:
		std::unique_ptr<Textbox> m_numBox;
		std::unique_ptr<Button> m_upButton;
		std::unique_ptr<Button> m_downButton;

		Vec2 m_widgetOffset;
		Vec2ui m_buttonDimensions;

		T m_val;
		T m_minVal;
		T m_maxVal;
		T m_increment;

		ButtonPosition m_buttonPosition = ButtonPosition::RIGHT;

		bool m_isForcedIncrement = false;
	};

#pragma region Type NumericUpDowns
	class NumericUpDownInt : public NumericUpDown<int>
	{
	public:
		NumericUpDownInt(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
			ButtonPosition buttonPos = ButtonPosition::RIGHT, Vec2ui buttonDimensions = Vec2ui(0));

	private:
		void OnUpButtonPressed() override;
		void OnDownButtonPressed() override;
		void OnTextChange() override;

	} typedef NumericUpDownI;

	class NumericUpDownUnsignedInt : public NumericUpDown<unsigned int>
	{
	public:
		NumericUpDownUnsignedInt(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
			ButtonPosition buttonPos = ButtonPosition::RIGHT, Vec2ui buttonDimensions = Vec2ui(0));

	private:
		void OnUpButtonPressed() override;
		void OnDownButtonPressed() override;
		void OnTextChange() override;

	} typedef NumericUpDownUI;

	class NumericUpDownFloat : public NumericUpDown<float>
	{
	public:
		NumericUpDownFloat(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
			ButtonPosition buttonPos = ButtonPosition::RIGHT,Vec2ui buttonDimensions = Vec2ui(0));

	private:
		void OnUpButtonPressed() override;
		void OnDownButtonPressed() override;
		void OnTextChange() override;

	private:
		unsigned int m_percision = 2;

	} typedef NumericUpDownF;

	class NumericUpDownDouble : public NumericUpDown<double>
	{
	public:
		NumericUpDownDouble(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
			ButtonPosition buttonPos = ButtonPosition::RIGHT, Vec2ui buttonDimensions = Vec2ui(0));

	private:
		void OnUpButtonPressed() override;
		void OnDownButtonPressed() override;
		void OnTextChange() override;

		unsigned int m_percision = 2;

	} typedef NumericUpDownD;
#pragma endregion
}