#include "Camera.h"

#include "Input.h"
#include "../Core/Core.h"
#include <iostream>

namespace ZGL
{
#pragma region Constructors/Destructors
	Camera::Camera(bool isTopLeftOrigin, bool hasEvents, float initZoomLevel)
		: m_aspectRatio((float)Core::GetCore().GetWindow().GetWidth() /
			(float)Core::GetCore().GetWindow().GetHeight()),
		m_viewWidth(Core::GetCore().GetWindow().GetWidth()), 
		m_viewHeight(Core::GetCore().GetWindow().GetHeight()), 
		m_isTopLeftOrigin(isTopLeftOrigin), m_hasEvents(hasEvents),
		m_zoomLevel(initZoomLevel), m_zoomSpeed(10.0f), m_view(1.0f), m_input(Input::GetInput())
	{ 
		if (!isTopLeftOrigin)
			SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
				m_zoomLevel, -m_zoomLevel);
		else
			SetProjection(0.0f, (float)m_viewWidth + m_zoomLevel, (float)m_viewHeight + m_zoomLevel, 0.0f);
		
		m_pv = m_proj * m_view;

		if (hasEvents)
		{
			m_onMouseScrolledFn = [&](Event& e)
			{ OnMouseScrolled(*((MouseScrolledEvent*)&e)); };

			m_onWindowResizeScrolledFn = [&](Event& e)
			{ OnWindowResized(*((WindowResizedEvent*)&e)); };

			Core::GetCore().GetGLFW_Dispatcher().Attach(m_onMouseScrolledFn, EventType::MOUSE_SCROLL);

			Core::GetCore().GetGLFW_Dispatcher().Attach(m_onWindowResizeScrolledFn, EventType::WINDOW_RESIZE);
		}
	}

	Camera::Camera(int viewPortWidth, int viewPortHeight, bool isTopLeftOrigin, bool hasEvents, float initZoomLevel)
		: m_aspectRatio((float) viewPortWidth / (float)viewPortHeight), m_zoomLevel(initZoomLevel), 
		m_viewWidth(viewPortWidth), m_viewHeight(viewPortHeight),
		m_isTopLeftOrigin(isTopLeftOrigin), m_hasEvents(hasEvents),
		m_zoomSpeed(10.0f), m_view(1.0f), m_input(Input::GetInput())
	{
		if(!isTopLeftOrigin)
			SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
				m_zoomLevel, -m_zoomLevel);
		else
			SetProjection(0.0f, m_aspectRatio * m_zoomLevel,
				m_zoomLevel, 0.0f);

		m_pv = m_proj * m_view;

		if (hasEvents)
		{
			m_onMouseScrolledFn = [&](Event& e)
			{ OnMouseScrolled(*((MouseScrolledEvent*)&e)); };

			m_onWindowResizeScrolledFn = [&](Event& e)
			{ OnWindowResized(*((WindowResizedEvent*)&e)); };

			Core::GetCore().GetGLFW_Dispatcher().Attach(m_onMouseScrolledFn,
				EventType::MOUSE_SCROLL);

			Core::GetCore().GetGLFW_Dispatcher().Attach(m_onWindowResizeScrolledFn,
				EventType::WINDOW_RESIZE);
		}
	}

	Camera::~Camera()
	{

		if (m_hasEvents)
		{
			Core::GetCore().GetGLFW_Dispatcher().Detach(m_onMouseScrolledFn,
				EventType::MOUSE_SCROLL);

			Core::GetCore().GetGLFW_Dispatcher().Detach(m_onWindowResizeScrolledFn,
				EventType::WINDOW_RESIZE);
		}
	}
#pragma endregion

	void Camera::Update()
	{
		if (m_input.IsKeyPressed(Key::A))
			m_pos.x += m_translationSpeed;
		else if (m_input.IsKeyPressed(Key::D))
			m_pos.x -= m_translationSpeed;

		if (m_input.IsKeyPressed(Key::W))
			m_pos.y -= m_translationSpeed;
		else if (m_input.IsKeyPressed(Key::S))
			m_pos.y += m_translationSpeed;

		m_translationSpeed = m_zoomLevel / (m_zoomLevel * 10);
		CalculateViewMatrix();
	}

	void Camera::SetProjection(float left, float right, float bottom, float top)
	{
		m_proj = Mat4::OrthoRH(left, right, bottom, top, -1.0f, 1.0f);
		m_pv = m_proj * m_view;
	}

	void Camera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.GetYOffset() * m_zoomSpeed;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);

		if (!m_isTopLeftOrigin)
			SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, m_zoomLevel, -m_zoomLevel);
		else
			SetProjection(0.0f, m_aspectRatio * m_zoomLevel,
				m_zoomLevel, 0.0f);
	}

	Vec2 Camera::GetMousePos() const
	{
		Vec2 mousePos = Input::GetInput().GetMousePos();

		float mX = mousePos.x / (m_viewWidth * 0.5f) - 1.0f;
		float mY = mousePos.y / (m_viewHeight * 0.5f) - 1.0f;

		Vec4 screenPos(mX, -mY, 1.0f, 1.0f);
		Vec4 worldPos = Mat4::Inverse(m_pv) * screenPos;

		return { worldPos.x, worldPos.y };
	}

	Vec2i Camera::GetScreenPos(const Viewport& viewport, const Vec2& pos, int height) const
	{
		Vec4 clipSpace = m_proj * (m_view * Vec4(pos.x, pos.y, 1.0f, 1.0f));
		Vec3 ndcSpace = Vec3(clipSpace.x / clipSpace.w, clipSpace.y / clipSpace.w, clipSpace.z / clipSpace.w);

		int x = (int)(((ndcSpace.x + 1.0f) / 2.0f) * (float)m_viewWidth + (float)viewport.GetX());
		int y = (int)(((ndcSpace.y + 1.0f) / 2.0f) * (float)m_viewHeight - height + (float)viewport.GetY());

		return { x, y };
	}

	void Camera::OnWindowResized(WindowResizedEvent& e)
	{
		m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		if (!m_isTopLeftOrigin)
			SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, m_zoomLevel, -m_zoomLevel);
		else
			SetProjection(0.0f, (float)m_viewWidth + m_zoomLevel, (float)m_viewHeight + m_zoomLevel, 0.0f);
	}

	void Camera::CalculateViewMatrix()
	{
		m_view = Mat4::Translate(Vec3(m_pos, 0.0f), true);

		m_pv = m_proj * m_view;
	}
}