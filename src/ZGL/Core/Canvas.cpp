#include "Canvas.h"
#include <iostream>

namespace ZGL
{
	Canvas::Canvas(int width, int height, bool isTopLeftOrigin, bool hasCameraEvents)
		: m_posX(0), m_posY(0), m_viewport(0, 0, width, height)
	{
		SetDimensions(width, height);
		m_cam = std::make_unique<Camera>(width, height, isTopLeftOrigin, hasCameraEvents);
	}

	Canvas::Canvas(float x, float y, int width, int height, bool isTopLeftOrigin, bool hasCameraEvents)
		: m_posX(x), m_posY(y), m_viewport((int)x, (int)y, width, height)
	{
		SetDimensions(width, height);
		m_cam = std::make_unique<Camera>(width, height, isTopLeftOrigin, hasCameraEvents);
	}

	void Canvas::SetViewport()
	{
		m_viewport.SetViewport();
	}

	void Canvas::SetPos(float x, float y)
	{
		m_posX = x;
		m_posY = y;
		m_viewport.SetPos((int)x, (int)y);
	}

	void Canvas::SetDimensions(int width, int height)
	{
		if (width <= 0 && height <= 0)
		{
			std::cout << "Error width and height have to be greater than 0!\n";
			return;
		}

		m_width = width;
		m_height = height;
		m_viewport.SetDimensions(width, height);
	}

	void Canvas::SetClearColour(float red, float green, float blue)
	{
		m_viewport.SetClearColour(red, green, blue);
	}
}