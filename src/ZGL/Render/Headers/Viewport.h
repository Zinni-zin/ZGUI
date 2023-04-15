#pragma once
#include "../../Math/Vectors.h"
#include "../../Input/Input.h"

namespace ZGL
{
	class Viewport
	{
	public:
		Viewport(Vec3 clearColour = Vec3(0.05f, 0.05f, 0.05f));
		Viewport(Vec2i pos, Vec2i dimensions, Vec3 clearColour = Vec3(0.05f, 0.05f, 0.05f));
		Viewport(Vec2i pos, int width, int height, Vec3 clearColour = Vec3(0.05f, 0.05f, 0.05f));
		Viewport(int x, int y, Vec2i dimensions, Vec3 clearColour = Vec3(0.05f, 0.05f, 0.05f));
		Viewport(int x, int y, int width, int height, Vec3 clearColour = Vec3(0.05f, 0.05f, 0.05f));

		void SetViewport() const;

		bool IsMouseInViewport() { return this == s_mouseViewport; }

		static void SetMouseViewport(Viewport* viewport);

		/* Getters */

		Vec2i GetPos() const { return m_pos; }
		int GetX() const { return m_pos.x; }
		int GetY() const { return m_pos.y; }

		Vec2i GetDimensions() const { return m_dimensions; }
		int GetWidth() const { return m_dimensions.x; }
		int GetHeight() const { return m_dimensions.y; }

		GLuint GetID() const { return m_id; }

		/* Setters */

		void SetPos(Vec2i pos) { m_pos = pos; }
		void SetPos(int x, int y) { m_pos.x = x; m_pos.y = y; }
		void SetX(int x) { m_pos.x = x; }
		void SetY(int y) { m_pos.y = y; }

		void SetDimensions(Vec2i dimensions) { m_dimensions = dimensions; }
		void SetDimensions(int width, int height) { m_dimensions.x = width, m_dimensions.y = height; }
		void SetWidth(int width) { m_dimensions.x = width; }
		void SetHeight(int height) { m_dimensions.y = height; }

		void SetClearColour(Vec3 colour) { m_clearColour = colour; }
		void SetClearColour(float r, float g, float b) { m_clearColour.x = r; m_clearColour.y = g; m_clearColour.z = b; }
	private:
		Vec2i m_pos;
		Vec2i m_dimensions;
		Vec3 m_clearColour;

		GLuint m_id;

		static Viewport* s_mouseViewport;
	};
}