#include "Window.h"
#include <iostream>

#include "../Events/Headers/MouseScrolledEvent.h"
#include "../Events/Headers/FrameBufferResizedEvent.h"
#include "../Events/Headers/MouseInputEvent.h"
#include "../Events/Headers/TextInputEvent.h"
#include "../Events/Headers/KeyInputEvent.h"

#include "../GUI/Headers/ZGUI.h"

#include "Core.h"

namespace ZGL {

	static ZGL::Dispatcher* dispatcher;
	static std::vector<ZGL::Event*>* events;

	void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	void FrambufferResizeCallback(GLFWwindow* window, int width, int height);
	void WindowResizeCallback(GLFWwindow* window, int width, int height);
	void TextInputCallback(GLFWwindow* window, unsigned int codepoint);
	void MouseInputCallback(GLFWwindow* window, int button, int action, int mods);
	void KeyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	Window::Window(const std::string& title, int width, int height)
		: m_width(width), m_height(height)
	{
		m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		/* Add Events */

		dispatcher = new Dispatcher();
		events = new std::vector<Event*>();
		events->reserve(5);
		events->emplace_back(new FrameBufferResizedEvent());
		events->emplace_back(new WindowResizedEvent());
		events->emplace_back(new MouseScrolledEvent());
		events->emplace_back(new MouseInputEvent());
		events->emplace_back(new TextInputEvent());
		events->emplace_back(new KeyInputEvent());

		InitCallbacks();

		m_onWindowResizeEvent = [&](Event& e)
		{ onWindowResizeEvent(*((WindowResizedEvent*)&e)); };

		Core::GetCore().GetGLFW_Dispatcher().Attach(m_onWindowResizeEvent,
			EventType::WINDOW_RESIZE);
	}

	Window::~Window() 
	{
		Core::GetCore().GetGLFW_Dispatcher().Detach(m_onWindowResizeEvent,
			EventType::WINDOW_RESIZE);

		if (dispatcher != nullptr)
		{
			delete dispatcher;
			dispatcher = nullptr;
		}

		if (events != nullptr)
		{
			for (auto it = events->begin(); it != events->end(); ++it)
			{
				if ((*it) != nullptr)
				{
					delete (*it);
					(*it) = nullptr;
				}
			}

			events->clear();
			delete events;
			events = nullptr;
		}
	}

	void Window::Update()
	{
		glfwPollEvents(); 
	}
	
	void Window::InitCallbacks()
	{	
		glfwSetWindowSizeCallback(m_window, WindowResizeCallback);
		glfwSetFramebufferSizeCallback(m_window, FrambufferResizeCallback);
		glfwSetScrollCallback(m_window, MouseScrollCallback);
		glfwSetCharCallback(m_window, TextInputCallback);
		glfwSetKeyCallback(m_window, KeyInputCallback);
		glfwSetMouseButtonCallback(m_window, MouseInputCallback);
	}

	void Window::SetVsync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_vsync = enabled;
	}

	void Window::onWindowResizeEvent(WindowResizedEvent& e)
	{
		m_width = e.GetWidth();
		m_height = e.GetHeight();
	}

	Dispatcher* Window::GetGLFW_EventDispatcher() const
	{
		return dispatcher;
	}

#pragma region GLFW Event Callback Functions
	template<class T>
	T* GetEvent(const EventType eventType)
	{
		T* event = nullptr;
		for (auto it = events->begin(); it != events->end(); ++it)
			if ((*it)->GetEventType() == eventType)
				event = (T*)(*it); // Downcast to get our event

		return event;
	}

	void WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		WindowResizedEvent* e = GetEvent<WindowResizedEvent>(EventType::WINDOW_RESIZE);

		if (e == nullptr) return; 

		e->SetDimensions(width, height); 

		ZGUI::UpdateMvp();

		dispatcher->Notify(*e); 
	}

	void FrambufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		FrameBufferResizedEvent* e = GetEvent<FrameBufferResizedEvent>(EventType::FRAMEBUFFER_RESIZE);

		if (e == nullptr) return;

		e->SetDimensions(width, height);
		dispatcher->Notify(*e);
	}

	void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		MouseScrolledEvent* e = GetEvent<MouseScrolledEvent>(EventType::MOUSE_SCROLL);

		if (e == nullptr) return; 

		e->SetOffsets((float)xOffset, (float)yOffset);
		dispatcher->Notify(*e);
	}

	void TextInputCallback(GLFWwindow* window, unsigned int codepoint)
	{
		TextInputEvent* e = GetEvent<TextInputEvent>(EventType::TEXT_INPUT);

		if (e == nullptr) return;

		e->SetCodepoint(codepoint);
		dispatcher->Notify(*e);
	}

	void KeyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyInputEvent* e = GetEvent<KeyInputEvent>(EventType::KEY_INPUT);
		
		if (e == nullptr) return;

		e->SetKey(key);
		e->SetScancode(scancode);
		e->SetAction(action);
		e->SetMods(mods);

		dispatcher->Notify(*e);
	}

	void MouseInputCallback(GLFWwindow* window, int button, int action, int mods)
	{
		MouseInputEvent* e = GetEvent<MouseInputEvent>(EventType::MOUSE_INPUT);

		if (e == nullptr) return;

		e->SetButton(button);
		e->SetAction(action);
		e->SetMods(mods);

		dispatcher->Notify(*e);
	}
#pragma endregion
}