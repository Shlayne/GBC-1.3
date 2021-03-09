#if CBC_PLATFORM_WINDOWS
#include "Core/Input.h"
#include <glfw/glfw3.h>
#include "Core/Application.h"

namespace cbc
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

	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float Input::GetMousePosX()
	{
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMousePosY()
	{
		auto [x, y] = GetMousePos();
		return y;
	}
}
#endif
