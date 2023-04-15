#pragma once

#include "Input.h"
#include "../Events/Headers/Event.h"
#include "../Events/Headers/MouseScrolledEvent.h"
#include "../Events/Headers/WindowResizedEvent.h"

#include "../Math/Matrices.h"

#include "../Render/Headers/Viewport.h"

namespace ZGL
{
	class Camera
	{
	public:
		Camera(bool isTopLeftOrigin = true, bool hasEvents = true, float initZoomLevel = 500.0f);
		Camera(int viewPortWidth, int viewPortHeight, bool isTopLeftOrigin = true, bool hasEvents = true, float initZoomLevel = 500.0f);
		~Camera();

		void Update();

		/* Getters */

		float GetZoomLevel() const { return m_zoomLevel; }
		int GetViewWidth() const { return m_viewWidth; }
		int GetViewHeight() const { return m_viewHeight; }

		Vec2 GetPos() const { return m_pos; }
		Mat4 GetProj() const { return m_proj; }
		Mat4 GetView() const { return m_view; }

		Vec2 GetMousePos() const;

		// Converts world space to screen space
		Vec2i GetScreenPos(const Viewport& viewport, const Vec2& pos, int height = 0) const;

		Mat4 GetPV() const { return m_pv; }

		/* Setters */

		void SetZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }
		void SetZoomSpeed(float zoomSpeed) { m_zoomSpeed = zoomSpeed; }
		void SetTranslationSpeed(float translationSpeed) { m_translationSpeed = translationSpeed; }
		void SetProjection(float left, float right, float bottom, float top);

		void SetPos(const Vec2& pos) 
		{ m_pos = pos; CalculateViewMatrix(); }

		void SetPos(float x, float y)
		{ m_pos = Vec2(x, y); CalculateViewMatrix(); }


	private:
		Vec2 m_pos;
		
		Mat4 m_proj;
		Mat4 m_view;
		Mat4 m_pv;

		float m_aspectRatio;
		float m_translationSpeed = 0.0f;
		float m_zoomLevel;
		float m_zoomSpeed;

		float m_rotation = 0.0f;

		int m_viewWidth, m_viewHeight;

		bool m_isTopLeftOrigin = true;
		bool m_hasEvents = true;

		void OnMouseScrolled(MouseScrolledEvent& e);
		void OnWindowResized(WindowResizedEvent& e);

		Input& m_input;

		void CalculateViewMatrix();

		std::function<void(Event&)> m_onMouseScrolledFn;
		std::function<void(Event&)> m_onWindowResizeScrolledFn;
	};
}