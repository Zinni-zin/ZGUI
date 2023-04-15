#pragma once
#include "Event.h"

namespace ZGL
{
	class TextInputEvent : public Event
	{
	public:
		TextInputEvent() : Event(EventType::TEXT_INPUT), m_codepoint(-1) { }

		TextInputEvent(unsigned int codepoint) : Event(EventType::TEXT_INPUT), m_codepoint(codepoint) { }

		inline unsigned int GetCodepoint() const { return m_codepoint; }

		inline void SetCodepoint(unsigned int codepoint) { m_codepoint = codepoint; }
	private:
		unsigned int m_codepoint;
	};
}