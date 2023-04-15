#include "Headers/NumericUpDown.h"

#include <math.h>

#include <sstream>
#include <iomanip>

namespace ZGL
{
	template<typename T>
	T intRoundToNearestMultiple(T num, T multiple)
	{
		T min = T((num >= 0) ? (num / multiple) * multiple : ((num - multiple + 1) / multiple) * multiple);
		T max = T((num >= 0) ? ((num + multiple - 1) / multiple) * multiple : (num / multiple) * multiple);
	
		return (num - min > max - num) ? max : min;
	}

	template<typename T>
	T decimalRoundToNearestMultiple(T num, T multiple)
	{
		T min = T((num == 0) ? num : std::floor(num / multiple) * multiple);
		T max = T((num == 0) ? num : std::ceil(num / multiple) * multiple);

		return (num - min > max - num) ? max : min;
	}

#pragma region Int

	NumericUpDownInt::NumericUpDownInt(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
		ButtonPosition buttonPos, Vec2ui buttonDimensions)
		: NumericUpDown(widget, font, pos, dimensions, buttonDimensions, buttonPos)
	{
		m_numBox->SetUsingInt(true);
	}

	/* ---- Event Updates ---- */

	void NumericUpDownInt::OnUpButtonPressed()
	{
		int val = m_val;
		val += m_increment;

		if (m_isForcedIncrement && (val % m_increment) != 0) return;
		if (val >= m_maxVal) return;

		m_val = val;

		m_numBox->SetText(std::to_string(m_val));
	}

	void NumericUpDownInt::OnDownButtonPressed()
	{
		int val = m_val;
		val -= m_increment;

		if (m_isForcedIncrement && (val % m_increment) != 0) return;
		if (val <= m_minVal) return;

		m_val = val;

		m_numBox->SetText(std::to_string(m_val));
	}

	void NumericUpDownInt::OnTextChange()
	{
		int val = m_val;
		std::string text = m_numBox->GetText();
		if (!text.empty())
		{
			if (text[0] == '-' && text.size() == 1) return;
			if (text[0] == '.') text.insert(text.begin(), '0');

			val = std::stoi(text);
		}

		if (val < m_minVal || val > m_maxVal) return;
		if (m_isForcedIncrement && (val % m_increment) != 0)
			val = intRoundToNearestMultiple(val, m_increment);

		m_val = val;
		m_numBox->SetText(std::to_string(m_val));
	}

#pragma endregion

#pragma region Unsigned Int

	NumericUpDownUnsignedInt::NumericUpDownUnsignedInt(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
		ButtonPosition buttonPos, Vec2ui buttonDimensions)
		: NumericUpDown(widget, font, pos, dimensions, buttonDimensions, buttonPos)
	{
		m_numBox->SetUsingInt(true);
		m_numBox->SetNumCharacters(false);
	}

	/* ---- Event Updates ---- */

	void NumericUpDownUnsignedInt::OnUpButtonPressed()
	{
		unsigned int val = m_val;
		val += m_increment;

		if (m_isForcedIncrement && (val % m_increment) != 0) return;
		if (val >= m_maxVal) return;

		m_val = val;

		m_numBox->SetText(std::to_string(m_val));
	}

	void NumericUpDownUnsignedInt::OnDownButtonPressed()
	{
		unsigned int val = m_val;
		val -= m_increment;

		if (m_isForcedIncrement && (val % m_increment) != 0) return;
		if (val <= m_minVal) return;
	
		m_val = val;

		m_numBox->SetText(std::to_string(m_val));
	}

	void NumericUpDownUnsignedInt::OnTextChange()
	{
		unsigned int val = m_val;
		std::string text = m_numBox->GetText();
		if (!text.empty())
		{
			if (text[0] == '-' && text.size() == 1) return;
			if (text[0] == '.') text.insert(text.begin(), '0');

			val = std::stoi(text);
		}

		if (val < m_minVal || val > m_maxVal) return;
		if (m_isForcedIncrement && (val % m_increment) != 0)
			val = intRoundToNearestMultiple(val, m_increment);

		m_val = val;
		m_numBox->SetText(std::to_string(val));
	}

#pragma endregion

#pragma region Float

	NumericUpDownFloat::NumericUpDownFloat(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
		 ButtonPosition buttonPos, Vec2ui buttonDimensions)
		: NumericUpDown(widget, font, pos, dimensions, buttonDimensions, buttonPos)
	{
		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	/* ---- Event Updates ---- */

	void NumericUpDownFloat::OnUpButtonPressed()
	{
		float val = m_val;
		val += m_increment;

		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0) return;
		if (val >= m_maxVal) return;

		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	void NumericUpDownFloat::OnDownButtonPressed()
	{
		float val = m_val;
		val -= m_increment;

		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0) return;
		if (val <= m_minVal) return;

		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	void NumericUpDownFloat::OnTextChange()
	{
		float val = m_val;
		std::string text = m_numBox->GetText();
		if (!text.empty())
		{
			if (text[0] == '-' && text.size() == 1) return;
			if (text[0] == '.') text.insert(text.begin(), '0');

			val = std::stof(text);
		}

		if (val < m_minVal || val > m_maxVal) return;
		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0) 
			val = decimalRoundToNearestMultiple(val, m_increment);
		
		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

#pragma endregion

#pragma region Double

	NumericUpDownDouble::NumericUpDownDouble(Widget& widget, Font& font, Vec2 pos, Vec2ui dimensions,
		ButtonPosition buttonPos, Vec2ui buttonDimensions)
		: NumericUpDown(widget, font, pos, dimensions, buttonDimensions, buttonPos)
	{
		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	/* ---- Event Updates ---- */

	void NumericUpDownDouble::OnUpButtonPressed()
	{
		double val = m_val;
		val += m_increment;

		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0) return;
		if (val >= m_maxVal) return;

		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	void NumericUpDownDouble::OnDownButtonPressed()
	{
		double val = m_val;
		val -= m_increment;

		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0) return;
		if (val <= m_minVal) return;

		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

	void NumericUpDownDouble::OnTextChange()
	{
		double val = m_val;
		std::string text = m_numBox->GetText();
		if (!text.empty())
		{
			if (text[0] == '-' && text.size() == 1) return;
			if (text[0] == '.') text.insert(text.begin(), '0');

			val = std::stod(text);
		}

		if (val < m_minVal || val > m_maxVal) m_numBox->SetText(std::to_string(m_val));
		if (m_isForcedIncrement && (fmod(val, m_increment)) != 0)
			val = decimalRoundToNearestMultiple(val, m_increment);

		m_val = val;

		std::stringstream percisionString;

		percisionString << std::fixed <<
			std::setprecision(m_percision) << m_val;

		m_numBox->SetText(percisionString.str());
	}

#pragma endregion
}
