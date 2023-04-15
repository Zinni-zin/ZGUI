#pragma once
#include <cstdint>

namespace ZGL
{
	class Timer
	{
	public:
		Timer();
		Timer(int64_t microseconds);
		Timer(int32_t milliseconds);
		Timer(double seconds);
		Timer(float seconds);

		float GetSeconds() const { return m_microseconds / 1000000.f; }
		double GetSecondsD() const { return m_microseconds / 1000000.; }
		int32_t GetMilliseconds() const { return (int32_t)(m_microseconds / (int32_t)1000); }
		int64_t GetMicroseconds() const { return m_microseconds; }

		float AsSeconds(int64_t microseconds) const { return (float)(microseconds / 1000000.f); }
		double AsSecondsD(int64_t microseconds) const { return (double)(microseconds / 1000000.0); }
		int32_t AsMilliseconds(int64_t microseconds) const { return (int32_t)(microseconds / (int32_t)1000); }

		void SetSeconds(float seconds) { m_microseconds = (int64_t)((double)seconds * (double)1000000.f); }
		void SetSeconds(double seconds) { m_microseconds = (int64_t)(seconds * 1000000); }
		void SetMilliseconds(int32_t milliseconds) { m_microseconds = (int64_t)(milliseconds * (int64_t)1000); }
		void SetMicroseconds(int64_t microseconds) { m_microseconds = microseconds; }

		int64_t AsMicroseconds(float seconds) { return (int64_t)((double)seconds * (double)1000000.f); }
		int64_t AsMicroseconds(double seconds) { return (int64_t)(seconds * 1000000.); }
		int64_t AsMicroseconds(int32_t milliseconds) { return milliseconds * (int64_t)1000; }

		int64_t GetTime();

		// Get's elapsed Time in microseconds
		int64_t GetElapsedTime();

		int64_t Restart();

		bool operator==(const Timer& rhs) { return m_microseconds == rhs.m_microseconds; }
		bool operator!=(const Timer& rhs) { return m_microseconds != rhs.m_microseconds; }
		bool operator>=(const Timer& rhs) { return m_microseconds >= rhs.m_microseconds; }
		bool operator<=(const Timer& rhs) { return m_microseconds <= rhs.m_microseconds; }
		Timer operator+(const Timer& rhs) { return(m_microseconds += rhs.m_microseconds); }
		Timer& operator+=(const Timer& rhs) { m_microseconds += rhs.m_microseconds; return *this; }
		Timer operator-(const Timer& rhs) { return(m_microseconds -= rhs.m_microseconds); }
		Timer& operator-=(const Timer& rhs) { m_microseconds -= rhs.m_microseconds; return *this; }
	private:
		int64_t m_microseconds;
		int64_t m_startTime;
	};
}