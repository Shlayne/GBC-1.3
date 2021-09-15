#pragma once

#include "GBC/Rendering/Context.h"

struct GLFWwindow;

namespace gbc
{
	class OpenGLContext : public Context
	{
	public:
		OpenGLContext(void* nativeWindow);
		virtual void SwapBuffers() override;
		virtual Timestep GetElapsedTime() const override;
#if GBC_ENABLE_IMGUI
		virtual const char* GetVersion() const override;
#endif
	private:
		GLFWwindow* window = nullptr;
		mutable float lastTime = 0.0f;
	};
}
