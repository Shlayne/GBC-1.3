#include "gbcpch.h"
#include "Context.h"
#include "gl/glew.h"
#include "glfw/glfw3.h"

namespace gbc
{
	Context::Context(GLFWwindow* window)
		: window(window)
	{
		glfwMakeContextCurrent(window);
		int initStatus = glewInit();
		GBC_ASSERT(initStatus == GLEW_OK, "Failed to initialize GLEW!");
		//std::cout << glGetString(GL_VERSION) << '\n';
	}

	void Context::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	float Context::GetElapsedTime() const
	{
		float time = (float)glfwGetTime();
		float elapsedTime = time - lastTime;
		lastTime = time;
		return elapsedTime;
	}
}
