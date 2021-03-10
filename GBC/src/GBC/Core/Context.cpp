#include "gbcpch.h"
#include "Context.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace gbc
{
	Context::Context(GLFWwindow* window)
		: window(window)
	{
		glfwMakeContextCurrent(window);
		int initStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GBC_ASSERT(initStatus != 0, "Failed to initialize glad!");
		//std::cout << glGetString(GL_VERSION) << '\n';
	}

	void Context::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	Timestep Context::GetElapsedTime() const
	{
		float time = (float)glfwGetTime();
		float elapsedTime = time - lastTime;
		lastTime = time;
		return elapsedTime;
	}
}
