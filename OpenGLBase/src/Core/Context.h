#pragma once

struct GLFWwindow;
class Context
{
public:
	Context(GLFWwindow* window);

	void SwapBuffers();

	float GetElapsedTime() const;
private:
	GLFWwindow* window;
	mutable float lastTime = 0.0f;
};
