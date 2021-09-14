#pragma once

#include "GBC/Rendering/Context.h"

struct GLFWwindow;

namespace gbc
{
	class OpenGLContext : public Context
	{
	public:
		virtual void Init(void* nativeWindow) override;
		virtual void SwapBuffers() override;
		virtual Timestep GetElapsedTime() const override;
		virtual const char* GetVersion() const override;
	private:
		GLFWwindow* window = nullptr;
		mutable float lastTime = 0.0f;
	};
}
