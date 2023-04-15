#include "Headers/Event.h"
#include <algorithm>
#include <iostream>

namespace ZGL
{
	void Dispatcher::Attach(EventFunctions& eventFunc, const EventType eventType)
	{
		if (m_eventFuncs[eventType].max_size() != (size_t)3)
			m_eventFuncs[eventType].reserve(3); // Reserve at least space for three event functions

		m_eventFuncs[eventType].emplace_back(&eventFunc);
	}

	void Dispatcher::Detach(EventFunctions& eventFunc, const EventType eventType)
	{
		auto it = m_eventFuncs.find(eventType); 
		if (it == m_eventFuncs.end()) 
		{
			std::cout << "Error detaching event, event type not found in the map!\n";
			return;
		}

		auto i = it->second.begin();
		for (auto& func : it->second)
		{
			if (func == &eventFunc)
			{
				i = it->second.erase(i);
				break;
			}
		}
	}

	void Dispatcher::Detach(const EventType eventType)
	{
		m_eventFuncs.erase(eventType); 
	}

	void Dispatcher::Notify(Event& event)
	{
		if (m_eventFuncs.find(event.GetEventType()) == m_eventFuncs.end()) return; 

		for (auto& ev : m_eventFuncs.at(event.GetEventType()))
				(*ev)(event);
	}
}