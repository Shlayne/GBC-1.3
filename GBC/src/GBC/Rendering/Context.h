#pragma once

#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Context
	{
	public:
		virtual ~Context() = default;

		virtual void Init(void* nativeWindow) = 0;
		virtual void SwapBuffers() = 0;
		virtual Timestep GetElapsedTime() const = 0;
		virtual const char* GetVersion() const = 0;

		static Scope<Context> Create();
	};
}
