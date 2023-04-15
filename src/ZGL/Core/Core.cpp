#include "Core.h"
#include <iostream>

#include "../GUI/Headers/ZGUI.h"

namespace ZGL
{
	Core::Core() { }
	Core::~Core()
	{
		if(m_ftLibrary != nullptr)
			FT_Done_FreeType(m_ftLibrary);
	}


	int Core::init(const std::string& windowTitle)
	{
		if (m_isInitialized) { std::cout << "Error Core is already initialized\n"; return -1; }
		if (!glfwInit()) return -1;

		m_window = std::make_unique<ZGL::Window>(windowTitle, 1280, 720);
		glfwMakeContextCurrent(m_window->GetGLFWindow());

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error glew is not ok!\n";
			return -1;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;

		m_window->SetVsync(true);

		if (!m_window->GetGLFWindow())
		{
			glfwTerminate();
			return -1;
		}

		if (FT_Init_FreeType(&m_ftLibrary))
		{
			std::cout << "Error has occurred during FreeType Library initialization!\n";
			return -1;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_SCISSOR_TEST);
		m_isInitialized = true;

		ZGUI::UpdateMvp();

		return 0;
	}

	int Core::init(const std::string& windowTitle, const Vec2i& dimensions)
	{
		if (m_isInitialized) { std::cout << "Error Core is already initialized\n"; return -1; }
		if (!glfwInit()) return -1;

		m_window = std::make_unique<ZGL::Window>(windowTitle, dimensions.x, dimensions.y);
		glfwMakeContextCurrent(m_window->GetGLFWindow());

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error glew is not ok!\n";
			return -1;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;

		m_window->SetVsync(true);

		if (!m_window->GetGLFWindow())
		{
			glfwTerminate();
			return -1;
		}

		if (FT_Init_FreeType(&m_ftLibrary))
		{
			std::cout << "Error has occurred during FreeType Library initialization!\n";
			return -1;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_SCISSOR_TEST);
		m_isInitialized = true;

		ZGUI::UpdateMvp();

		return 0;
	}

	int Core::Shutdown()
	{
		if (m_ftLibrary != nullptr)
			FT_Done_FreeType(m_ftLibrary);

		return 0;
	}

	void Core::Clear(unsigned int red, unsigned int green, unsigned int blue)
	{
		float r = (red > 255) ? 255 : (float)red / 255;
		float g = (green > 255) ? 255 : (float)green / 255;
		float b = (blue > 255) ? 255 : (float)blue / 255;

		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Core::Clear(float red, float green, float blue)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Core::Clear(const Vec3& colour)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
		glClearColor(colour.x, colour.y, colour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	Core& Core::GetCore()
	{
		static Core core;
		return core;
	}
}