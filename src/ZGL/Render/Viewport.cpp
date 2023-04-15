#include "Headers/Viewport.h"

#include "GL/glew.h"
#include "../Core/Core.h"

namespace ZGL
{
	Viewport* Viewport::s_mouseViewport = nullptr;

#pragma region Constructors
	Viewport::Viewport(Vec3 clearColour)
		: m_pos(0, 0), 
		m_dimensions(Core::GetCore().GetWindow().GetWidth(), Core::GetCore().GetWindow().GetHeight())
	{
		if (s_mouseViewport == nullptr) s_mouseViewport = this;
	}

	Viewport::Viewport(Vec2i pos, Vec2i dimensions, Vec3 clearColour)
		: m_pos(pos), m_dimensions(dimensions), m_clearColour(clearColour)
	{
		if (s_mouseViewport == nullptr) s_mouseViewport = this; 
	}

	Viewport::Viewport(Vec2i pos, int width, int height, Vec3 clearColour)
		: m_pos(pos), m_dimensions(width, height), m_clearColour(clearColour)
	{
		if (s_mouseViewport == nullptr) s_mouseViewport = this;
	}

	Viewport::Viewport(int x, int y, Vec2i dimensions, Vec3 clearColour)
		: m_pos(x, y), m_dimensions(dimensions), m_clearColour(clearColour)
	{
		if (s_mouseViewport == nullptr) s_mouseViewport = this;
	}

	Viewport::Viewport(int x, int y, int width, int height, Vec3 clearColour)
		: m_pos(x, y), m_dimensions(width, height), m_clearColour(clearColour)
	{
		if (s_mouseViewport == nullptr) s_mouseViewport = this;
	}
#pragma endregion

	void Viewport::SetMouseViewport(Viewport* viewport)
	{
		if (viewport == s_mouseViewport) return;

		Vec2 mousePos = Input::GetInput().GetMousePos();
		int left = viewport->GetX() + viewport->GetWidth();
		int top = viewport->GetY() + viewport->GetHeight();

		bool isInXBounds = (mousePos.x >= viewport->GetX() && mousePos.x <= left) ? true : false;
		bool isInYBounds = (mousePos.y >= viewport->GetY() && mousePos.y <= top) ? true : false;

		if (!(isInXBounds && isInYBounds)) return;

		s_mouseViewport = viewport;
	}

	void Viewport::SetViewport() const
	{
		glScissor((GLint)m_pos.x, (GLint)m_pos.y, m_dimensions.x,  m_dimensions.y);

		glClearColor(m_clearColour.x, m_clearColour.y, m_clearColour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport((GLint)m_pos.x, (GLint)m_pos.y, m_dimensions.x, m_dimensions.y);
	}
}