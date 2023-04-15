#pragma once

#include "Event.h"

namespace ZGL
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent() : Event(EventType::WINDOW_RESIZE), m_width(-1), m_height(-1) { }

		WindowResizedEvent(unsigned int width, unsigned int height)
			: Event(EventType::WINDOW_RESIZE), m_width(width), m_height(height) { }


		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }

		inline void SetWidth(unsigned int width) { m_width = width; }
		inline void SetHeight(unsigned int height) { m_height = height; }
		inline void SetDimensions(unsigned int width, unsigned int height) 
		{ m_width = width; m_height = height; }
	private:
		unsigned int m_width, m_height;
	};
}