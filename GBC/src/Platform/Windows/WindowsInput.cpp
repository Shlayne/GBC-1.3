#include "gbcpch.h"
#if GBC_PLATFORM_WINDOWS
#include "GBC/Core/Input.h"
#include "glfw/glfw3.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	bool Input::IsKeyPressed(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_PRESS;
	}

	bool Input::IsKeyReleased(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(Mousecode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonReleased(Mousecode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePos()
	{
		static float px = 0.0f, py = 0.0f;
		static double x = 0.0, y = 0.0;
		Window& window = Application::Get().GetWindow();
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &x, &y);

		if (window.IsFocused() && window.ContainsMouse())
		{
			px = (float)x;
			py = (float)y;
		}
		return {px, py};
	}

	float Input::GetMousePosX()
	{
		return GetMousePos().x;
	}

	float Input::GetMousePosY()
	{
		return GetMousePos().y;
	}
}
#endif
