#pragma once

#include "Shape.h"
#include <iostream>
#include "../../Math/ZMath.h"

namespace ZGL
{
	class RectangleF : public ShapeF
	{
	public:
		RectangleF(float x, float y, float width, float height, const Vec4& colour = Vec4::One()) : ShapeF(x, y, width, height, colour)  { }
		RectangleF(Vec2 pos, float width, float height, const Vec4& colour = Vec4::One())         : ShapeF(pos, width, height, colour)   { }
		RectangleF(float x, float y, Vec2 dimensions, const Vec4& colour = Vec4::One())           : ShapeF(x, y, dimensions, colour)     { }
		RectangleF(float x, float y, float dimensions, const Vec4& colour = Vec4::One())          : ShapeF(x, y, dimensions, colour)     { }
		RectangleF(Vec2 pos, Vec2 dimensions, const Vec4& colour = Vec4::One())                   : ShapeF(pos, dimensions, colour)      { }

		float GetLeft()   const {  return m_pos.x;                   }
		float GetRight()  const {  return m_pos.x + m_dimensions.x;  }
		float GetTop()    const {  return m_pos.y;                   }
		float GetBottom() const {  return m_pos.y + m_dimensions.y;  }

		void SetDimensions(const Vec2 dimensions) { m_dimensions = dimensions; m_sprite->SetWidth((int)dimensions.x); m_sprite->SetHeight((int)dimensions.y); }
		void SetDimensions(float width, float height) { m_dimensions = Vec2(width, height); m_sprite->SetWidth((int)width); m_sprite->SetHeight((int)height); }
		void SetWidth(float width) { m_dimensions.x = width; m_sprite->SetWidth((int)width); }
		void SetHeight(float height) { m_dimensions.y = height; m_sprite->SetHeight((int)height); }

		void FlipVertically() { if (m_sprite->GetRotation() == 0) m_sprite->SetRotation(180.0); else m_sprite->SetRotation(0.f); }

		float GetSurfaceArea() const override { return m_dimensions.x * m_dimensions.y; }
		bool HasIntersected(ShapeF* other) const override;
		virtual bool HasIntersected(ShapeI* other) const override;

	} typedef FloatRectangle, FloatRect, RectF;

	class RectangleI : public ShapeI
	{
	public:
		RectangleI(float x, float y, int width, int height, const Vec4& colour = Vec4::One()) : ShapeI(x, y, width, height, colour) { }
		RectangleI(Vec2 pos, int width, int height, const Vec4& colour = Vec4::One())         : ShapeI(pos, width, height, colour)  { }
		RectangleI(float x, float y, Vec2i dimensions, const Vec4& colour = Vec4::One())      : ShapeI(x, y, dimensions, colour)    { }
		RectangleI(float x, float y, int dimensions, const Vec4& colour = Vec4::One())        : ShapeI(x, y, dimensions, colour)    { }
		RectangleI(Vec2 pos, Vec2i dimensions, const Vec4& colour = Vec4::One())              : ShapeI(pos, dimensions, colour)     { }

		int GetLeft()   const { return (int)m_pos.x; }
		int GetRight()  const { return (int)m_pos.x + m_dimensions.x; }
		int GetTop()    const { return (int)m_pos.y; }
		int GetBottom() const { return (int)m_pos.y + m_dimensions.y; }

		void SetDimensions(const Vec2i dimensions) { m_dimensions = dimensions; m_sprite->SetWidth(dimensions.x); m_sprite->SetHeight(dimensions.y); }
		void SetDimensions(int width, int height) { m_dimensions = Vec2i(width, height); m_sprite->SetWidth(width); m_sprite->SetHeight(height); }
		void SetWidth(int width) { m_dimensions.x = width; m_sprite->SetWidth(width); }
		void SetHeight(int height) { m_dimensions.y = height; m_sprite->SetHeight(height); }

		void FlipVertically() { if (m_sprite->GetRotation() == 0) m_sprite->SetRotation(ToRadians(180.f)); else m_sprite->SetRotation(0); }

		int GetSurfaceArea() const override { return m_dimensions.x * m_dimensions.y;  }
		virtual bool HasIntersected(ShapeF* other) const override;
		bool HasIntersected(ShapeI* other) const override;
	
	} typedef IntRectangle, IntRect, RectI;

	inline std::ostream& operator<<(std::ostream& os, const RectangleF& rectF)
	{
		using namespace std;
		os << rectF.GetPos() << " | " << rectF.GetDimensions();
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const RectangleI& rectI)
	{
		using namespace std;
		os << rectI.GetPos() << " | " << rectI.GetDimensions();
		return os;
	}
}