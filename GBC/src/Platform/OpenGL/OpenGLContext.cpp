#include "gbcpch.h"
#include "OpenGLContext.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace gbc
{
	OpenGLContext::OpenGLContext(void* nativeContext)
		: window(static_cast<GLFWwindow*>(nativeContext))
	{

	}

	void OpenGLContext::Init()
	{
		GBC_PROFILE_FUNCTION();

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
		float time = (float)glfwGetTime();
		float elapsedTime = time - lastTime;
		lastTime = time;
		return elapsedTime;
	}
}
