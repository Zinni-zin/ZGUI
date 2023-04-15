#include "Input.h"

#include "../Core/Core.h"

namespace ZGL
{
	Input::Input() 
	{
		m_window = Core::GetCore().GetWindow().GetGLFWindow();
	}

	bool Input::IsKeyPressed(Key::KeyCode keycode)
	{
		auto state = glfwGetKey(m_window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyReleased(Key::KeyCode keycode)
	{
		auto state = glfwGetKey(m_window, keycode);

		return state == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(Mouse::MouseCode mouseCode)
	{
		auto state = glfwGetMouseButton(m_window, mouseCode);

		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonReleased(Mouse::MouseCode mouseCode)
	{
		auto state = glfwGetMouseButton(m_window, mouseCode);

		return state == GLFW_RELEASE;
	}

	Vec2 Input::GetMousePos() const
	{
		double xPos, yPos;

		glfwGetCursorPos(m_window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}

	Input& Input::GetInput()
	{
		static Input keyInput;
		return keyInput;
	}
}