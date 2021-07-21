#pragma once

#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Context
	{
	public:
		virtual ~Context() = default;

		virtual void PreInit() = 0;
		virtual void Init(void* nativeContext) = 0;
		virtual void SwapBuffers() = 0;
		virtual Timestep GetElapsedTime() const = 0;
		virtual const char* GetVersion() const = 0;

		static Ref<Context> CreateRef();
		static Scope<Context> CreateScope();
	};
}
