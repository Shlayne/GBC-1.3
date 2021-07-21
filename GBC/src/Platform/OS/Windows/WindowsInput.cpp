#include "gbcpch.h"
#include "GBC/Core/Input.h"
#include "glfw/glfw3.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	bool Input::IsKeyPressed(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) != GLFW_RELEASE;
	}

	bool Input::IsKeyReleased(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(Mousecode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) != GLFW_RELEASE;
	}

	bool Input::IsMouseButtonReleased(Mousecode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return {(float)x, (float)y};
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
