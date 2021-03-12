#include "gbcpch.h"
#if GBC_PLATFORM_WINDOWS
#include "GBC/Core/Input.h"
#include "glfw/glfw3.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	bool Input::IsKeyPressed(Keycode keycode)
	{
		Application& application = Application::Get();
#if GBC_ENABLE_IMGUI
		if (application.IsImGuiUsingKeyEvents()) return false;
#endif
		auto window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_PRESS;
	}

	bool Input::IsKeyReleased(Keycode keycode)
	{
		Application& application = Application::Get();
#if GBC_ENABLE_IMGUI
		if (application.IsImGuiUsingKeyEvents()) return false;
#endif
		auto window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_RELEASE;
	}

	bool Input::IsMouseButtonPressed(Mousecode button)
	{
		Application& application = Application::Get();
#if GBC_ENABLE_IMGUI
		if (application.IsImGuiUsingMouseEvents()) return false;
#endif
		auto window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonReleased(Mousecode button)
	{
		Application& application = Application::Get();
#if GBC_ENABLE_IMGUI
		if (application.IsImGuiUsingMouseEvents()) return false;
#endif
		auto window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePos()
	{
		static double x = 0.0, y = 0.0;

		Application& application = Application::Get();
#if GBC_ENABLE_IMGUI
		if (application.IsImGuiUsingMouseEvents()) return {(float)x, (float)y};
#endif
		auto window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());
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
#endif
