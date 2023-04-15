#include "Headers/Rectangle.h"

#include <math.h>

#include "Headers/Circle.h"

namespace ZGL
{
	template<typename T>
	bool IsOverlapped(T left1, T right1, T top1, T bottom1, 
		T left2, T right2, T top2, T bottom2)
	{
		return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
	}

	template<typename T, typename T2>
	bool IsOverlapped(T left1, T right1, T top1, T bottom1,
		T2 left2, T2 right2, T2 top2, T2 bottom2)
	{
		return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
	}

	// Rectangle Circle collision
	template<typename T>
	bool IsOverlapped(float rX, float rY, T width, T height,
		float cX, float cY, float radius)
	{
		float dX = abs(cX - rX);
		float dY = abs(cY - rY);
		float rCenterX = (float)width / 2.0f;
		float rCenterY = (float)height / 2.0f;

		if (dX > rCenterX + radius || dY > rCenterY + radius) return false;
		if (dX <= rCenterX || dY <= rCenterY) return true; 

		float distance = ((dX - rCenterX) * (dX - rCenterX)) +  ((dY - rCenterY) * (dY - rCenterY));

		return distance <= (radius * radius);
	}

#pragma region Float Rectangle

	bool RectangleF::HasIntersected(ShapeF* other) const
	{
		if (RectangleF* shape = dynamic_cast<RectangleF*>(other))
			return IsOverlapped(GetLeft(), GetRight(), GetTop(), GetBottom(), 
				shape->GetLeft(), shape->GetRight(), shape->GetTop(), shape->GetBottom());

		else if (CircleF* shape = dynamic_cast<CircleF*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_dimensions.x, m_dimensions.y,
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		return false;
	}

	bool RectangleF::HasIntersected(ShapeI* other) const
	{
		if (RectangleI* shape = dynamic_cast<RectangleI*>(other))
			return IsOverlapped(GetLeft(), GetRight(), GetTop(), GetBottom(), 
				shape->GetLeft(), shape->GetRight(), shape->GetTop(), shape->GetBottom());

		else if (CircleI* shape = dynamic_cast<CircleI*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_dimensions.x, m_dimensions.y,
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		return false;
	}

#pragma endregion

#pragma region Int Rectangle

	bool RectangleI::HasIntersected(ShapeF* other) const
	{
		if (RectangleF* shape = dynamic_cast<RectangleF*>(other))
			return IsOverlapped(GetLeft(), GetRight(), GetTop(), GetBottom(),
				shape->GetLeft(), shape->GetRight(), shape->GetTop(), shape->GetBottom());

		else if (CircleF* shape = dynamic_cast<CircleF*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_dimensions.x, m_dimensions.y, shape->GetPos().x,
				shape->GetPos().y, shape->GetRadius());

		return false;
	}

	bool RectangleI::HasIntersected(ShapeI* other) const
	{
		if (RectangleI* shape = dynamic_cast<RectangleI*>(other))
			return IsOverlapped(GetLeft(), GetRight(), GetTop(), GetBottom(),
				shape->GetLeft(), shape->GetRight(), shape->GetTop(), shape->GetBottom());

		else if (CircleI* shape = dynamic_cast<CircleI*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_dimensions.x, m_dimensions.y, shape->GetPos().x,
				shape->GetPos().y, shape->GetRadius());

		return false;
	}

#pragma endregion
}