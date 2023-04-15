#include "Headers/Circle.h"

#include <math.h>

#include "Headers/Rectangle.h"

namespace ZGL
{
	bool IsOverlapped(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		float dx = x1 - x2;
		float dy = y1 - y2;
		float distance = (dx * dx) + (dy * dy);

		return distance < (r1 + r2) * (r1 + r2);
	}

	// Rectangle Circle collision
	template<typename T>
	bool IsOverlapped(float rX, float rY, T width, T height, float cX, float cY, float radius)
	{
		float dX = abs(cX - rX);
		float dY = abs(cY - rY);
		float rCenterX = (float)width / 2.0f;
		float rCenterY = (float)height / 2.0f;

		if (dX > rCenterX + radius || dY > rCenterY + radius) return false;
		if (dX <= rCenterX || dY <= rCenterY) return true;

		float distance = ((dX - rCenterX) * (dX - rCenterX)) + ((dY - rCenterY) * (dY - rCenterY));

		return distance <= (radius * radius);
	}

#pragma region Float Circle

	bool CircleF::HasIntersected(ShapeF* other) const
	{
		if (CircleF* shape = dynamic_cast<CircleF*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_radius, 
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		else if (RectF* shape = dynamic_cast<RectF*>(other))
			return IsOverlapped(shape->GetPos().x, shape->GetPos().y, 
				shape->GetDimensions().x, shape->GetDimensions().y,
				m_pos.x, m_pos.y, m_radius);

		return false;
	}

	bool CircleF::HasIntersected(ShapeI* other) const
	{
		if (CircleI* shape = dynamic_cast<CircleI*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_radius,
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		else if (RectI* shape = dynamic_cast<RectI*>(other))
			return IsOverlapped(shape->GetPos().x, shape->GetPos().y,
				shape->GetDimensions().x, shape->GetDimensions().y,
				m_pos.x, m_pos.y, m_radius);

		return false;
	}

#pragma endregion

#pragma region Int Circle

	bool CircleI::HasIntersected(ShapeF* other) const
	{
		if (CircleF* shape = dynamic_cast<CircleF*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_radius,
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		else if (RectF* shape = dynamic_cast<RectF*>(other))
			return IsOverlapped(shape->GetPos().x, shape->GetPos().y,
				shape->GetDimensions().x, shape->GetDimensions().y,
				m_pos.x, m_pos.y, m_radius);

		return false;
	}

	bool CircleI::HasIntersected(ShapeI* other) const
	{
		if (CircleI* shape = dynamic_cast<CircleI*>(other))
			return IsOverlapped(m_pos.x, m_pos.y, m_radius,
				shape->GetPos().x, shape->GetPos().y, shape->GetRadius());

		else if (RectI* shape = dynamic_cast<RectI*>(other))
			return IsOverlapped(shape->GetPos().x, shape->GetPos().y,
				shape->GetDimensions().x, shape->GetDimensions().y,
				m_pos.x, m_pos.y, m_radius);

		return false;
	}

#pragma endregion
}