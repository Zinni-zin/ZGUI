#pragma once
#include "Event.h"

namespace ZGL
{
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent() : Event(EventType::MOUSE_SCROLL), m_xOffset(-1), m_yOffset(-1) { }

		MouseScrolledEvent(float xOffset, float yOffset) 
			: Event(EventType::MOUSE_SCROLL), m_xOffset(xOffset), m_yOffset(yOffset) { }

		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }

		inline void SetOffsets(float xOffset, float yOffset) { m_xOffset = xOffset; m_yOffset = yOffset; }
	private:
		float m_xOffset, m_yOffset;
	};
}