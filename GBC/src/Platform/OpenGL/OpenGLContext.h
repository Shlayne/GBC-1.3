#pragma once

#include "GBC/Rendering/Context.h"

struct GLFWwindow;

namespace gbc
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(void* nativeContext);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual Timestep GetElapsedTime() const override;
	private:
		GLFWwindow* window;
		mutable float lastTime = 0.0f;
	};
}
