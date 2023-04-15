#pragma once

#include "Vector3.h"
#include <iostream>

namespace ZGL
{
	// Vector4 float
	class Vector4f
	{
	public:
		float x, y, z, w;

		Vector4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
		Vector4f(float val) : x(val), y(val), z(val), w(val) { }
		Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
		Vector4f(const Vector2f& vec2, float z, float w) : x(vec2.x), y(vec2.y), z(z), w(w) { }
		Vector4f(const Vector2f& vec2XY, const Vector2f& vec2ZW) : x(vec2XY.x), y(vec2XY.y), z(vec2ZW.x), w(vec2ZW.y) { }
		Vector4f(const Vector3f& vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

		static Vector4f Zero() { return { 0.0f, 0.0f, 0.0f, 0.0f }; }
		static Vector4f One() { return { 1.0f, 1.0f, 1.0f, 1.0f }; }

		float DotProduct(const Vector4f& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w));
		}

		Vector4f operator+(const Vector4f& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		Vector4f operator-(const Vector4f& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w };
		}

		float operator*(const Vector4f& rhs)
		{
			return DotProduct(rhs);
		}

		Vector4f& operator+=(const Vector4f& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4f& operator-=(const Vector4f& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4f& operator/=(const float rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}

	} typedef Vec4;

	// Vector4 int
	class Vector4i
	{
	public:

		int x, y, z, w;

		Vector4i() : x(0), y(0), z(0), w(0) { }
		Vector4i(int val) : x(val), y(val), z(val), w(val) { }
		Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) { }

		Vector4i(const Vector2i& vec2, int z, int w) : x(vec2.x), y(vec2.y), z(z), w(w) { }
		Vector4i(const Vector2i& vec2XY, const Vector2i& vec2ZW) : x(vec2XY.x), y(vec2XY.y), z(vec2ZW.x), w(vec2ZW.y) { }
		Vector4i(const Vector3i& vec3, int w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

		static Vector4f Zero() { return { 0, 0, 0, 0 }; }
		static Vector4f One() { return { 1, 1, 1, 1 }; }

		int DotProduct(const Vector4i& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w));
		}

		Vector4i operator+(const Vector4i& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		Vector4i operator-(const Vector4i& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w };
		}

		int operator*(const Vector4i& rhs)
		{
			DotProduct(rhs);
		}

		Vector4i& operator+=(const Vector4i& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4i& operator-=(const Vector4i& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4i& operator/=(const int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}

	} typedef Vec4i;

	// Vector4 unsigned int
	class Vector4ui
	{
	public:
		unsigned int x, y, z, w;

		Vector4ui() : x(0), y(0), z(0), w(0) { }
		Vector4ui(unsigned int val) : x(val), y(val), z(val), w(val) { }
		Vector4ui(unsigned int x, unsigned int y, unsigned int z, unsigned int w) : x(x), y(y), z(z), w(w) { }

		Vector4ui(const Vector2ui& vec2, unsigned int z, unsigned int w) : x(vec2.x), y(vec2.y), z(z), w(w) { }
		Vector4ui(const Vector2ui& vec2XY, const Vector2ui& vec2ZW) : x(vec2XY.x), y(vec2XY.y), z(vec2ZW.x), w(vec2ZW.y) { }
		Vector4ui(const Vector3ui& vec3, unsigned int w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

		static Vector4ui Zero() { return { 0, 0, 0, 0 }; }
		static Vector4ui One() { return { 1, 1, 1, 1 }; }

		unsigned int DotProduct(const Vector4ui& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w));
		}

		Vector4ui operator+(const Vector4ui& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		Vector4ui operator-(const Vector4ui& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w };
		}

		unsigned int operator*(const Vector4ui& rhs)
		{
			return DotProduct(rhs);
		}

		Vector4ui& operator+=(const Vector4ui& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4ui& operator-=(const Vector4ui& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4ui& operator/=(const unsigned int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}

	} typedef Vecui4;

	// Vector4 double
	class Vector4d
	{
	public:
		double x, y, z, w;

		Vector4d() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
		Vector4d(double val) : x(val), y(val), z(val), w(val) { }
		Vector4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) { }

		Vector4d(const Vector2d& vec2, double z, double w) : x(vec2.x), y(vec2.y), z(z), w(w) { }
		Vector4d(const Vector2d& vec2XY, const Vector2d& vec2ZW) : x(vec2XY.x), y(vec2XY.y), z(vec2ZW.x), w(vec2ZW.y) { }
		Vector4d(const Vector3d& vec3, double w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

		static Vector4d Zero() { return { 0, 0, 0, 0 }; }
		static Vector4d One() { return { 1, 1, 1, 1 }; }

		double DotProduct(const Vector4d& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (w * rhs.w));
		}

		Vector4d operator+(const Vector4d& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		Vector4d operator-(const Vector4d& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w };
		}

		double operator*(const Vector4d& rhs)
		{
			return DotProduct(rhs);
		}

		Vector4d& operator+=(const Vector4d& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4d& operator-=(const Vector4d& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4d& operator/=(const double rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}
	} typedef Vecd4;

	inline std::ostream& operator<<(std::ostream& os, const Vector4f& vec4)
	{
		using namespace std;
		os << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector4i& vec4)
	{
		using namespace std;
		os << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector4ui& vec4)
	{
		using namespace std;
		os << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector4d& vec4)
	{
		using namespace std;
		os << vec4.x << ", " << vec4.y << ", " << vec4.z << ", " << vec4.w;
		return os;
	}
}
