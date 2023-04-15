#pragma once
#include "Shape.h"

#include <math.h>

namespace ZGL
{
	class CircleF : public ShapeF
	{
	public:
		CircleF(float x, float y, float radius, const Vec4& colour = Vec4::One())
			: ShapeF(x, y, radius * 2.0f, colour), m_radius(radius) {}

		CircleF(float x, float y, double diameter, const Vec4& colour = Vec4::One())
			: ShapeF(x, y, (float)diameter, colour), m_radius((float)diameter / 2.0f) { }

		Vec2 GetCenter() const { return Vec2(m_pos.x + m_radius, m_pos.y + m_radius); }

		float GetSurfaceArea() const override { return (float)(PI * pow(m_radius, 2)); }
		float GetRadius() const { return m_radius; }

		bool HasIntersected(ShapeF* other) const override;
		bool HasIntersected(ShapeI* other) const override;
	private:
		float m_radius;

	} typedef FloatCircle, FCircle;

	class CircleI : public ShapeI
	{
	public:
		CircleI(float x, float y, int size, const Vec4& colour = Vec4::One()) 
			: ShapeI(x, y, size, colour), m_radius((float)size / 2.0f) {}

		Vec2 GetCenter() const { return Vec2(m_pos.x + m_radius, m_pos.y + m_radius); }

		int GetSurfaceArea() const override { return (int)(PI * pow(m_radius, 2)); }
		float GetRadius() const { return m_radius; }

		bool HasIntersected(ShapeF* other) const override;
		bool HasIntersected(ShapeI* other) const override;
	private:
		float m_radius;

	} typedef IntCircle, ICircle;
}