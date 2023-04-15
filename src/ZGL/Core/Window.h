#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "../Events/Headers/Event.h"
#include "../Events/Headers/WindowResizedEvent.h"

namespace ZGL {
	class Window
	{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		void Update();

		/* Getters */

		GLFWwindow* GetGLFWindow() { return m_window; }
		ZGL::Dispatcher* GetGLFW_EventDispatcher() const; // GLFW dispatcher is a global var

		int GetWidth() const { return  m_width; }
		int GetHeight() const { return  m_height; }
		bool IsVsync() const { return  m_vsync; }

		void SetVsync(bool enable);

	private:
		GLFWwindow* m_window;

		int m_width;
		int m_height;
		bool m_vsync;

		void InitCallbacks();

		std::function<void(Event&)> m_onWindowResizeEvent;
		void onWindowResizeEvent(WindowResizedEvent& e);
	};
}