#pragma once
#include "Event.h"

namespace ZGL
{
	class MouseInputEvent : public Event
	{
	public:
		MouseInputEvent() : Event(EventType::MOUSE_INPUT), m_button(-1), m_action(-1), m_mods(-1) { }

		MouseInputEvent(int button, int action, int mods) 
			: Event(EventType::MOUSE_INPUT), m_button(button), m_action(action), m_mods(mods) { }

		inline int GetButton() const { return m_button; }
		inline int GetAction() const { return m_action; }
		inline int GetMods() const { return m_mods; }

		inline void SetButton(int button) { m_button = button; }
		inline void SetAction(int action) { m_action = action; }
		inline void SetMods(int mods) { m_mods = mods; }
	private:
		int m_button;
		int m_action;
		int m_mods;
	};
}