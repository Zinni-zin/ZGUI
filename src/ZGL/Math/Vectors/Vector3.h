#pragma once

#include <iostream>
#include "Vector2.h"

namespace ZGL
{
	// Vector3 float
	class Vector3f
	{
	public:
		float x, y, z;

		Vector3f() : x(0.0f), y(0.0f), z(0.0f) { }
		Vector3f(float val) : x(val), y(val), z(val) { }
		Vector3f(float x, float y, float z) : x(x), y(y), z(z) { }
		Vector3f(const Vector2f& vec2, float z) : x(vec2.x), y(vec2.y), z(z) { }

		static Vector3f Zero() { return { 0.0f, 0.0f, 0.0f }; }
		static Vector3f One() { return { 1.0f, 1.0f, 1.0f }; }

		float DotProduct(const Vector3f& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
		}

		Vector3f CrossProduct(const Vector3f& rhs)
		{
			float cX = (y * rhs.z) - (z * rhs.y);
			float cY = (x * rhs.z) - (z * rhs.x); 
			float cZ = (x * rhs.y) - (y * rhs.x); 
			return { cX, cY, cZ };
		}

		static Vector3f Normalize(const Vector3f& vec3, bool useQFastInverse = true)
		{
			float x = vec3.x;
			float y = vec3.y;
			float z = vec3.z;
			float num = (x * x) + (y * y) + (z * z);
			float magInv = (useQFastInverse)? Q_FastInverse(num) : 1.0f / sqrtf(num);
			return { x * magInv, y * magInv, z * magInv };
		}

		Vector3f operator+(const Vector3f& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vector3f operator-(const Vector3f& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		float operator*(const Vector3f& rhs)
		{
			DotProduct(rhs);
		}

		Vector3f& operator+=(const Vector3f& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3f& operator-=(const Vector3f& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3f& operator/=(const float rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}
	private:
		static float Q_FastInverse(float num)
		{
			const float x2 = num * 0.5F;
			const float threehalfs = 1.5F;

			union u {
				float f;
				uint32_t i;
			};
			u conv = { num };
			conv.i = 0x5f3759df - (conv.i >> 1);
			conv.f *= threehalfs - (x2 * conv.f * conv.f);
			return conv.f;
		}

	} typedef Vec3;

	// Vector3 int
	class Vector3i
	{
	public:

		int x, y, z;

		Vector3i() : x(0), y(0), z(0) { }
		Vector3i(int val) : x(val), y(val), z(val) { }
		Vector3i(int x, int y, int z) : x(x), y(y), z(z) { }
		Vector3i(const Vector2i& vec2, int z) : x(vec2.x), y(vec2.y), z(z) { }

		static Vector3f Zero() { return { 0, 0, 0 }; }
		static Vector3f One() { return { 1, 1, 1 }; }

		int DotProduct(const Vector3i& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
		}

		Vector3i CrossProduct(const Vector3i& rhs)
		{
			int cX = (y * rhs.z) - (z * rhs.y);
			int cY = (x * rhs.z) - (z * rhs.x);
			int cZ = (x * rhs.y) - (y * rhs.x);
			return { cX, cY, cZ };
		}

		Vector3i operator+(const Vector3i& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vector3i operator-(const Vector3i& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		int operator*(const Vector3i& rhs)
		{
			DotProduct(rhs);
		}

		Vector3i& operator+=(const Vector3i& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3i& operator-=(const Vector3i& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3i& operator/=(const int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

	} typedef Vec3i;

	// Vector3 unsigned int
	class Vector3ui
	{
	public:

		unsigned int x, y, z;

		Vector3ui() : x(0), y(0), z(0) { }
		Vector3ui(unsigned int val) : x(val), y(val), z(val) { }
		Vector3ui(unsigned int x, unsigned int y, unsigned int z) : x(x), y(y), z(z) { }
		Vector3ui(const Vector2ui& vec2, unsigned int z) : x(vec2.x), y(vec2.y), z(z) { }

		static Vector3ui Zero() { return { 0, 0, 0 }; }
		static Vector3ui One() { return { 1, 1, 1 }; }

		unsigned int DotProduct(const Vector3ui& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
		}

		Vector3ui CrossProduct(const Vector3ui& rhs)
		{

			unsigned int cX = (y * rhs.z) - (z * rhs.y);
			unsigned int cY = (x * rhs.z) - (z * rhs.x); 
			unsigned int cZ = (x * rhs.y) - (y * rhs.x); 
			return { cX, cY, cZ };
		}

		Vector3ui operator+(const Vector3ui& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vector3ui operator-(const Vector3ui& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		unsigned int operator*(const Vector3ui& rhs)
		{
			DotProduct(rhs);
		}

		Vector3ui& operator+=(const Vector3ui& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3ui& operator-=(const Vector3ui& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3ui& operator/=(const unsigned int rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

	} typedef Vec3ui;

	// Vector3 double
	class Vector3d
	{
	public:

		double x, y, z;

		Vector3d() : x(0), y(0), z(0) { }
		Vector3d(double val) : x(val), y(val), z(val) { }
		Vector3d(double x, double y, double z) : x(x), y(y), z(z) { }
		Vector3d(const Vector2d& vec2, double z) : x(vec2.x), y(vec2.y), z(z) { }

		static Vector3d Zero() { return { 0, 0, 0 }; }
		static Vector3d One() { return { 1, 1, 1 }; }

		double DotProduct(const Vector3d& rhs)
		{
			return ((x * rhs.x) + (y * rhs.y) + (z * rhs.z));
		}

		Vector3d CrossProduct(const Vector3d& rhs)
		{

			double cX = (y * rhs.z) - (z * rhs.y);
			double cY = (x * rhs.z) - (z * rhs.x);
			double cZ = (x * rhs.y) - (y * rhs.x);
			return { cX, cY, cZ };
		}

		Vector3d operator+(const Vector3d& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vector3d operator-(const Vector3d& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		double operator*(const Vector3d& rhs)
		{
			DotProduct(rhs);
		}

		Vector3d& operator+=(const Vector3d& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3d& operator-=(const Vector3d& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3d& operator/=(const double rhs)
		{
			if (rhs == 0) return *this;
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}
	} typedef Vec3d;

	inline std::ostream& operator<<(std::ostream& os, const Vector3f& vec3)
	{
		using namespace std;
		os << vec3.x << ", " << vec3.y << ", " << vec3.z;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3i& vec3)
	{
		using namespace std;
		os << vec3.x << ", " << vec3.y << ", " << vec3.z;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3ui& vec3)
	{
		using namespace std;
		os << vec3.x << ", " << vec3.y << ", " << vec3.z;
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3d& vec3)
	{
		using namespace std;
		os << vec3.x << ", " << vec3.y << ", " << vec3.z;
		return os;
	}
}
