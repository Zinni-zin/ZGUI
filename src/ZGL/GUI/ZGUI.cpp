#include "Headers/ZGUI.h"

#include "../Core/Core.h"

namespace ZGL
{
	Mat4 ZGUI::m_mvp;

	ZGUI::ZGUI(Vec2 pos) :m_pos(pos)
	{ }
	
	ZGUI::ZGUI(Vec2 pos, Vec2ui dimensions) : m_pos(pos), m_dimensions(dimensions) 
	{ }
	
	ZGUI::~ZGUI()
	{ }

	void ZGUI::UpdateMvp()
	{
		m_mvp = Mat4::OrthoRH(0.f, (float)ZGL::Core::GetCore().GetWindow().GetWidth(),
			(float)ZGL::Core::GetCore().GetWindow().GetHeight(), 0.f, -1.f, 1.f);
	}
}