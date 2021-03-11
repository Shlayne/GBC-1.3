#pragma once

#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Context
	{
	public:
		virtual ~Context() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		// TODO: this has nothing to do with graphics,
		// so it shouldn't be in graphics context
		virtual Timestep GetElapsedTime() const = 0;

		static Ref<Context> CreateRef(void* nativeContext);
		static Scope<Context> CreateScope(void* nativeContext);
	};
}
