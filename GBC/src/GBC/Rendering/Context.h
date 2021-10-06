#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Context
	{
	public:
		virtual ~Context() = default;
	public:
		virtual void SwapBuffers() = 0;
		virtual Timestep GetElapsedTime() const = 0;
	public:
		virtual const char* GetVendor() const = 0;
		virtual const char* GetRenderer() const = 0;
		virtual const char* GetVersion() const = 0;
	public:
#if GBC_ENABLE_IMGUI
		virtual const char* GetVersionForImGui() const = 0;
#endif
	public:
		static Scope<Context> Create(void* nativeWindow);
	};
}
