#pragma once

#include <iostream>

namespace ZGL
{
	// Vector2 float
	class Vector2f
	{
	public:
		float x, y;

		Vector2f() : x(0.0f), y(0.0f) { }
		Vector2f(float val) : x(val), y(val) { }
		Vector2f(float x, float y) : x(x), y(y) { }

		static Vector2f Zero() { return { 0.0f, 0.0f }; }
		static Vector2f One() { return { 1.0f, 1.0f }; }

		float DotProduct(const Vector2f& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y));
		}

		Vector2f operator+(const Vector2f& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		Vector2f operator-(const Vector2f& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		float operator*(const Vector2f& rhs)
		{
			DotProduct(rhs);
		}

		Vector2f& operator+=(const Vector2f& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2f& operator-=(const Vector2f& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2f& operator/=(const float rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;

			return *this;
		}
	} typedef Vec2, Vec2f;

	// Vector2 int
	class Vector2i
	{
	public:
		int x, y;

		Vector2i() : x(0), y(0) { }
		Vector2i(int val) : x(val), y(val) { }
		Vector2i(int x, int y) : x(x), y(y) { }

		static Vector2i Zero() { return { 0, 0 }; }
		static Vector2i One() { return { 1, 1 }; }

		int DotProduct(const Vector2i& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y));
		}

		Vector2i operator+(const Vector2i& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		Vector2i operator-(const Vector2i& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		int operator*(const Vector2i& rhs)
		{
			DotProduct(rhs);
		}

		Vector2i& operator+=(const Vector2i& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2i& operator-=(const Vector2i& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2i& operator/=(const int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;

			return *this;
		}
	} typedef Vec2i;

	// Vector2 unsigned int
	class Vector2ui
	{
	public:
		unsigned int x, y;

		Vector2ui() : x(0), y(0) { }
		Vector2ui(unsigned int val) : x(val), y(val) { }
		Vector2ui(unsigned int x, unsigned int y) : x(x), y(y) { }

		static Vector2ui Zero() { return { 0, 0 }; }
		static Vector2ui One() { return { 1, 1 }; }

		unsigned int DotProduct(const Vector2ui& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y));
		}

		Vector2ui operator+(const Vector2ui& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		Vector2ui operator-(const Vector2ui& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		unsigned int operator*(const Vector2ui& rhs)
		{
			DotProduct(rhs);
		}

		Vector2ui& operator+=(const Vector2ui& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2ui& operator-=(const Vector2ui& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2ui& operator/=(const unsigned int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;

			return *this;
		}

	} typedef Vec2ui;

	// Vector2 double
	class Vector2d
	{
	public:
		double x, y;

		Vector2d() : x(0), y(0) { }
		Vector2d(double val) : x(val), y(val) { }
		Vector2d(double x, double y) : x(x), y(y) { }

		static Vector2d Zero() { return { 0, 0 }; }
		static Vector2d One() { return { 1, 1 }; }

		double DotProduct(const Vector2d& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y));
		}

		Vector2d operator+(const Vector2d& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		Vector2d operator-(const Vector2d& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		double operator*(const Vector2d& rhs)
		{
			DotProduct(rhs);
		}

		Vector2d& operator+=(const Vector2d& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2d& operator-=(const Vector2d& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		
		Vector2d& operator/=(const double rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;

			return *this;
		}
	} typedef Vec2d;

#pragma region Output Stream Operators '<<'
	inline std::ostream& operator<<(std::ostream& os, const Vector2f& vec2)
	{
		using namespace std;
		os << vec2.x << ", " << vec2.y;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2i& vec2)
	{
		using namespace std;
		os << vec2.x << ", " << vec2.y;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2ui& vec2)
	{
		using namespace std;
		os << vec2.x << ", " << vec2.y;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2d& vec2)
	{
		using namespace std;
		os << vec2.x << ", " << vec2.y;
		return os;
	}
#pragma endregion

#pragma region Divide Operator '/'
	inline Vector2f operator/(const Vector2f& lhs, const float rhs)
	{
		return Vector2f(lhs.x / rhs, lhs.y / rhs);
	}

	inline Vector2i operator/(const Vector2i& lhs, const int rhs)
	{
		return Vector2i(lhs.x / rhs, lhs.y / rhs);
	}

	inline Vector2ui operator/(const Vector2ui& lhs, const int rhs)
	{
		return Vector2ui(lhs.x / rhs, lhs.y / rhs);
	}

	inline Vector2d operator/(const Vector2d& lhs, const double rhs)
	{
		return Vector2d(lhs.x / rhs, lhs.y / rhs);
	}
#pragma endregion

#pragma region Operator Equal To '=='
	inline bool operator==(const Vector2f& lhs, const Vector2f& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	inline bool operator==(const Vector2i& lhs, const Vector2i& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	inline bool operator==(const Vector2ui& lhs, const Vector2ui& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	inline bool operator==(const Vector2d& lhs, const Vector2d& rhs)
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}
#pragma endregion

#pragma region Operator Not Equal To '!='
	inline bool operator!=(const Vector2f& lhs, const Vector2f& rhs)
	{
		return (lhs.x != rhs.x) && (lhs.y != rhs.y);
	}

	inline bool operator!=(const Vector2i& lhs, const Vector2i& rhs)
	{
		return (lhs.x != rhs.x) && (lhs.y != rhs.y);
	}

	inline bool operator!=(const Vector2ui& lhs, const Vector2ui& rhs)
	{
		return (lhs.x != rhs.x) && (lhs.y != rhs.y);
	}

	inline bool operator!=(const Vector2d& lhs, const Vector2d& rhs)
	{
		return (lhs.x != rhs.x) && (lhs.y != rhs.y);
	}
#pragma endregion
}
