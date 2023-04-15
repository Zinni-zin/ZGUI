#pragma once

#include "../../Render/Headers/Renderer.h"
#include "../../Math/Vectors.h"

#include "../../Shapes/Headers/Rectangle.h"

namespace ZGL
{
	class ZGUI
	{
	public:
		ZGUI(Vec2 pos);
		ZGUI(Vec2 pos, Vec2ui dimensions);
		~ZGUI();

		virtual void Update() = 0;
		virtual void Draw(Renderer& renderer) = 0;

		/* --- Getters --- */

		Vec2 GetPos() const { return m_pos; }
		float GetX() const { return m_pos.x; }
		float GetY() const { return m_pos.y; }

		Vec2ui GetDimensions() const { return m_dimensions; }
		unsigned int GetWidth() const { return m_dimensions.x; }
		unsigned int GetHeight() const { return m_dimensions.y;  }

		Mat4& GetMVP() const { return m_mvp; }

		/* --- Setters --- */

		virtual void SetPos(Vec2 pos) { m_pos = pos; }
		virtual void SetPos(float x, float y) { m_pos = Vec2(x, y); }
		virtual void SetX(float x) { m_pos.x = x; }
		virtual void SetY(float y) { m_pos.y = y; }

		virtual void SetDimensions(Vec2ui dimensions) { m_dimensions = dimensions; }
		virtual void SetDimensions(unsigned int width, unsigned int height) { m_dimensions = Vec2ui(width, height); }
		virtual void SetWidth(unsigned int width) { m_dimensions.x = width; }
		virtual void SetHeight(unsigned int height) { m_dimensions.y = height; }

		static void UpdateMvp();

	protected:
		Vec2 m_pos;
		Vec2ui m_dimensions;
		static Mat4 m_mvp;

		//static RectangleI* s_mouseRect;
		//static unsigned int s_refCount;
	};
}