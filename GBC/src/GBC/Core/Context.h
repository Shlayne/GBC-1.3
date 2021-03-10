#pragma once

#include "Timestep.h"

struct GLFWwindow;

namespace gbc
{
	class Context
	{
	public:
		Context(GLFWwindow* window);

		void SwapBuffers();

		Timestep GetElapsedTime() const;
	private:
		GLFWwindow* window;
		mutable float lastTime = 0.0f;
	};
}
