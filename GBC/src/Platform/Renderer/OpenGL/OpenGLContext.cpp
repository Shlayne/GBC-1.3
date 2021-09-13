#include "gbcpch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

#define GBC_OPENGL_VERSION_MAJOR 4
#define GBC_OPENGL_VERSION_MINOR 6
#define GBC_OPENGL_VERSION "#version " GBC_STRINGIFY_MACRO(GBC_OPENGL_VERSION_MAJOR) GBC_STRINGIFY_MACRO(GBC_OPENGL_VERSION_MINOR) "0 core"

namespace gbc
{
	void OpenGLContext::PreInit()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GBC_OPENGL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GBC_OPENGL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	const char* OpenGLContext::GetVersion() const
	{
		return GBC_OPENGL_VERSION;
	}

	void OpenGLContext::Init(void* nativeWindow)
	{
		GBC_PROFILE_FUNCTION();

		window = static_cast<GLFWwindow*>(nativeWindow);

		GBC_CORE_ASSERT(window != nullptr, "Window is nullptr!");
		glfwMakeContextCurrent(window);
		int initStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GBC_CORE_ASSERT(initStatus != 0, "Failed to initialize glad!");

		GBC_CORE_INFO("OpenGL Info:");
		GBC_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		GBC_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GBC_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		GBC_PROFILE_FUNCTION();

		glfwSwapBuffers(window);
	}

	Timestep OpenGLContext::GetElapsedTime() const
	{
		float time = static_cast<float>(glfwGetTime());
		float elapsedTime = time - lastTime;
		lastTime = time;
		return elapsedTime;
	}
}
