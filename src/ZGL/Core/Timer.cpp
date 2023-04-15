#include "Timer.h"
#include <windows.h>

#include <chrono>
namespace ZGL
{

	Timer::Timer() : m_microseconds(0), m_startTime(GetTime())
	{ }

	Timer::Timer(int64_t microseconds) : m_microseconds(microseconds), m_startTime(GetTime())
	{}

	Timer::Timer(int32_t milliseconds) : m_microseconds(milliseconds * (int64_t)1000), m_startTime(GetTime())
	{}

	Timer::Timer(double seconds) : m_microseconds((int64_t)(seconds * 1000000.)), m_startTime(GetTime())
	{}

	Timer::Timer(float seconds) : m_microseconds((int64_t)((double)seconds * (double)1000000.f)), m_startTime(GetTime())
	{}

	int64_t Timer::GetTime()
	{
		auto end = std::chrono::steady_clock::now().time_since_epoch();
		return (int64_t)std::chrono::duration_cast<std::chrono::microseconds>(end).count();
	}

	int64_t Timer::GetElapsedTime()
	{
		return GetTime() - m_startTime;
	}

	int64_t Timer::Restart()
	{
		int64_t now = GetTime();
		int64_t elapsed = now - m_startTime;
		m_startTime = now;

		return elapsed;
	}
}