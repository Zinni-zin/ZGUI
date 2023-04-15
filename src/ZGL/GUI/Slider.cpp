#include "Headers/Slider.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

namespace ZGL
{
	float RoundTo(float val, int place = 100)
	{
		return std::floor(val * place + 0.5f) / place;
	}

	double RoundTo(double val, int place = 100)
	{
		return std::floor(val * place + 0.5) / place;
	}

	template<typename T>
	double CalculateValue(T min, T max, float sliderX, int width)
	{
		double minVal = (double)min;

		// If our max value is 0, we will run into issues with dividing, 
		// so set it to really, really small number
		double maxVal = (max == 0)? 0.0000001 : (double)max;

		double difference = ((double)maxVal * (minVal / 100.0)) - minVal;

		minVal = minVal - (difference / (maxVal * 0.01));

		double percent = (sliderX / width) * (100.0 - minVal); 

		return maxVal * ((percent + minVal) / 100.0);
	}

	template<typename T>
	float CalculateSliderPos(T val, T min, T max, int width)
	{
		double minVal = (double)min;

		double maxVal = (max == 0) ? 0.0000001 : (double)max;

		double difference = ((double)maxVal * (minVal / 100.0)) - minVal;

		minVal = minVal - (difference / (maxVal * 0.01));
		
		float percent = (((float)val / (float)maxVal) * 100.0f) - (float)minVal;
		return (percent / (100.0f - (float)minVal)) * width;
	}

#pragma region Constructors
	SliderInt::SliderInt(Widget& widget, Vec2 pos)
		: Slider(widget, pos) { widget.AddElement(this); }

	SliderInt::SliderInt(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions)
		: Slider(widget, pos, dimensions, sliderDimensions) { widget.AddElement(this); }

	SliderFloat::SliderFloat(Widget& widget, Vec2 pos)
		: Slider(widget, pos) { widget.AddElement(this); }

	SliderFloat::SliderFloat(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions)
		: Slider(widget, pos, dimensions, sliderDimensions) { widget.AddElement(this); }

	SliderDouble::SliderDouble(Widget& widget, Vec2 pos)
		: Slider(widget, pos) { widget.AddElement(this); }

	SliderDouble::SliderDouble(Widget& widget, Vec2 pos, Vec2ui dimensions, Vec2ui sliderDimensions)
		: Slider(widget, pos, dimensions, sliderDimensions) { widget.AddElement(this); }
#pragma endregion

#pragma region Updates

	void SliderFloat::Update()
	{
		if (m_isPressed)
		{
			CalculatePosition(m_widet.GetMouseRect().GetX());
			CalculateVal();

			if (m_text)
			{
				std::stringstream percisionString;

				percisionString << std::fixed << 
					std::setprecision(m_percision) << m_val;

				m_text->SetText(percisionString.str());
				m_text->SetPosX(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2));
			}
		}

		if (m_widet.GetMouseRect().HasIntersected(m_slider.get()) && Input::GetInput().IsMouseButtonPressed(Mouse::LEFT_BUTTON))
			m_isPressed = true;

		if (m_isPressed && Input::GetInput().IsMouseButtonReleased(Mouse::LEFT_BUTTON))
			m_isPressed = false;
	}

	void SliderDouble::Update()
	{
		if (m_isPressed)
		{
			CalculatePosition(m_widet.GetMouseRect().GetX());
			CalculateVal();

			if (m_text)
			{
				std::stringstream percisionString;

				percisionString << std::fixed <<
					std::setprecision(m_percision) << m_val;

				m_text->SetText(percisionString.str());
				m_text->SetPosX(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2));
			}
		}

		if (m_widet.GetMouseRect().HasIntersected(m_slider.get()) && Input::GetInput().IsMouseButtonPressed(Mouse::LEFT_BUTTON))
			m_isPressed = true;

		if (m_isPressed && Input::GetInput().IsMouseButtonReleased(Mouse::LEFT_BUTTON))
			m_isPressed = false;
	}

#pragma endregion

#pragma region Calculate Values

	void SliderInt::CalculateVal()
	{
		float sliderX = (m_slider->GetPos().x + (m_slider->GetWidth() / 2.0f)) - m_pos.x;;

		int width = m_dimensions.x;

		if (m_hasToBeInBorder)
		{
			sliderX = m_slider->GetPos().x - m_pos.x;
			if (!m_isAbleToExtendBorder) width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_val = (int)std::round(RoundTo(CalculateValue(m_minVal, m_maxVal, sliderX, width), 10));
	}

	void SliderFloat::CalculateVal()
	{
		float sliderX = (m_slider->GetPos().x + (m_slider->GetWidth() / 2.0f)) - m_pos.x;

		int width = m_dimensions.x;

		if (m_hasToBeInBorder)
		{
			sliderX = m_slider->GetPos().x - m_pos.x;
			if (!m_isAbleToExtendBorder)  width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_val = (float)CalculateValue(m_minVal, m_maxVal, sliderX, width);
	}

	void SliderDouble::CalculateVal()
	{
		float sliderX = (m_slider->GetPos().x + (m_slider->GetWidth() / 2.0f)) - m_pos.x;

		int width = m_dimensions.x;

		if (m_hasToBeInBorder)
		{
			sliderX = m_slider->GetPos().x - m_pos.x;
			if (!m_isAbleToExtendBorder) width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_val = CalculateValue(m_minVal, m_maxVal, sliderX, width);
	}
#pragma endregion

#pragma region Calculate Position From Value

	void SliderInt::SetVal(int val)
	{
		if (val < m_minVal || val > m_maxVal)
		{
			std::cout << "Error setting val(" << val << ") in int slider(" << this << "):\n" <<
				"Val can't less than min val(" << m_minVal << ") and can't be greater than max val("
				<< m_maxVal << ")\n";
			return;
		}

		float xOffset = m_pos.x + (m_slider->GetWidth() / 2);
		int width = m_dimensions.x;

		if (m_hasToBeInBorder)
		{ 
			xOffset = m_pos.x; 
			
			if (!m_isAbleToExtendBorder) width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_slider->SetX(CalculateSliderPos(val, m_minVal, m_maxVal, width) + xOffset);
		m_val = val; 
		if (m_text)
		{
			m_text->SetText(std::to_string(m_val));
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2.f),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void SliderFloat::SetVal(float val)
	{
		if (val < m_minVal || val > m_maxVal)
		{
			std::cout << "Error setting val(" << val << ") in float slider(" << this << "):\n" <<
				"Val can't less than min val(" << m_minVal << ") and can't be greater than max val("
				<< m_maxVal << ")\n";
			return;
		}
		
		float xOffset = m_pos.x + (m_slider->GetWidth() / 2);
		int width = m_dimensions.x;

		if (m_hasToBeInBorder)
		{
			xOffset = m_pos.x;
			if (!m_isAbleToExtendBorder) width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_slider->SetX(CalculateSliderPos(val, m_minVal, m_maxVal, width) + xOffset);
		m_val = val; 

		if (m_text)
		{
			std::stringstream percisionString;

			percisionString << std::fixed <<
				std::setprecision(m_percision) << m_val;

			m_text->SetText(percisionString.str());
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2.f),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

	void SliderDouble::SetVal(double val)
	{
		if (val < m_minVal || val > m_maxVal)
		{
			std::cout << "Error setting val(" << val << ") in double slider(" << this << "):\n" <<
				"Val can't less than min val(" << m_minVal << ") and can't be greater than max val("
				<< m_maxVal << ")\n";
			return;
		}

		float xOffset = m_pos.x + (m_slider->GetWidth() / 2);
		int width = m_dimensions.x;

		if (m_hasToBeInBorder) 
		{
			xOffset = m_pos.x;
			if (!m_isAbleToExtendBorder) width = m_dimensions.x - (m_slider->GetWidth() / 2);
		}

		m_slider->SetX(CalculateSliderPos(val, m_minVal, m_maxVal, width) + xOffset);
		m_val = val;

		if (m_text)
		{
			std::stringstream percisionString;

			percisionString << std::fixed <<
				std::setprecision(m_percision) << m_val;

			m_text->SetText(percisionString.str());
			m_text->SetPos(m_rect->GetX() + (m_rect->GetWidth() / 2.f) - (m_text->GetWidth() / 2.f),
				m_rect->GetY() + (m_rect->GetHeight() / 2.f) - (m_text->GetSize(MAX_CHAR_SIZE).y / 2));
		}
	}

#pragma endregion
}