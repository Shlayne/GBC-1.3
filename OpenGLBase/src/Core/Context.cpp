#include "Context.h"
#include <gl/glew.h>
#include <glfw/glfw3.h>

Context::Context(GLFWwindow* window)
	: window(window)
{
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) return;
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
