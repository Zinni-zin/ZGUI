#pragma once
#include <map>
#include <vector>
#include <functional>
#include <cinttypes>

namespace ZGL
{

	enum class EventType
	{
		NONE = 0,
		WINDOW_RESIZE = 1, FRAMEBUFFER_RESIZE = 2,
		KEY_INPUT = 3, TEXT_INPUT = 4,
		CURSOR_ENTER_LEAVE = 5, MOUSE_SCROLL = 6, MOUSE_INPUT = 7
	};

	class Event
	{
	public:
		Event() : m_eventType(EventType::NONE) { }
		Event(const EventType eventType) : m_eventType(eventType) { }

		EventType GetEventType() const { return m_eventType; }

	protected:
		const EventType m_eventType;
	};

	class Dispatcher
	{
	public: 
		using EventFunctions = std::function<void(Event&)>; 

		void Attach(EventFunctions& eventFunc, const EventType eventType);

		void Detach(EventFunctions& eventFunc, const EventType eventType);
		
		void Detach(const EventType eventType);

		void Notify(Event& event);

	private:
		// A list of event function pointers(used for detaching) sorted by event type
		std::map<EventType, std::vector<EventFunctions*>> m_eventFuncs;
	};
}
