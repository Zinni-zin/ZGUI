#pragma once
#include "Event.h"

namespace ZGL
{
	class KeyInputEvent : public Event
	{
	public:
		KeyInputEvent() : Event(EventType::KEY_INPUT), m_key(-1), m_scancode(-1), m_action(-1), m_mods(-1)
		{ }

		KeyInputEvent(int key, int scancode, int action, int mods) 
			: Event(EventType::KEY_INPUT), m_key(key), m_scancode(scancode), m_action(action), m_mods(mods)
		{ }

		inline int GetKey() const { return m_key; }
		inline int GetScancode() const { return m_scancode; }
		inline int GetAction() const { return m_action; }
		inline int GetMods() const { return m_mods; }

		inline void SetKey(int key) { m_key = key; }
		inline void SetScancode(int scancode) { m_scancode = scancode; }
		inline void SetAction(int action) { m_action = action; }
		inline void SetMods(int mod) { m_mods = mod; }

	private:
		int m_key;
		int m_scancode;
		int m_action;
		int m_mods;
	};
}