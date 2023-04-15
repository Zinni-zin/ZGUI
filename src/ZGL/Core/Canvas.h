#pragma once
#include <vector>
#include <utility>

#include "../Input/Camera.h"
#include "../Render/Headers/Renderer.h"

#include "../Math/Matrices.h"

#include "../Render/Headers/Viewport.h"

namespace ZGL
{
	class Canvas
	{
	public:
		Canvas(int width, int height, bool isTopLeftOrigin = true, bool hasCameraEvents = true);
		Canvas(float x, float y, int width, int height, bool isTopLeftOrigin = true, bool hasCameraEvents = true);

		virtual void Update() = 0;
		virtual void Draw(Renderer& renderer) = 0;

		inline Vec2 GetPos() { return { m_posX, m_posY }; }
		inline Vec2i GetDimensions() { return { m_width, m_height }; }

		inline Viewport& GetViewport() { return m_viewport; }

		void SetPos(float x, float y);
		void SetDimensions(int width, int height);
		void SetClearColour(float red, float green, float blue);

		void SetViewport();
	protected:
		float m_posX, m_posY;
		int m_width, m_height;
		std::unique_ptr<Camera> m_cam;

		Viewport m_viewport;
	};
}