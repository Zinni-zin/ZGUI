#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <memory>

#include "Window.h"
#include "Canvas.h"

namespace ZGL
{
	class Core
	{
	public:
		
		~Core();

		int init(const std::string& windowTitle);
		int init(const std::string& windowTitle, const Vec2i& dimensions);
		int Shutdown();

		inline ZGL::Window& GetWindow() { return *m_window; }
		inline ZGL::Dispatcher& GetGLFW_Dispatcher() const { return *m_window->GetGLFW_EventDispatcher(); }
		inline FT_Library& GetFT_Library() { return m_ftLibrary; }

		inline int GetWidth() const { return m_window->GetWidth(); }
		inline int GetHeight() const { return m_window->GetHeight(); }

		inline void ShowCursor(bool val) 
		{
			if (val) glfwSetInputMode(m_window->GetGLFWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else glfwSetInputMode(m_window->GetGLFWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		inline bool IsShowingCursor() const 
		{
			return glfwGetInputMode(m_window->GetGLFWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL; 
		}

		void Clear(unsigned int red, unsigned int green, unsigned int blue);

		void Clear(float red, float green, float blue);

		void Clear(const Vec3& colour);

		 static Core& GetCore();
	private:
		Core();
		std::unique_ptr<Window> m_window;

		FT_Library m_ftLibrary;
		bool m_isInitialized = false;
	};
}