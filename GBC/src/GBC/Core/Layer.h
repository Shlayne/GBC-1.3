#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Core/Timestep.h"
#include "GBC/Events/Event.h"

namespace gbc
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnRender() {}
#if GBC_ENABLE_IMGUI
		virtual void OnImGuiRender() {}
		virtual void OnPostImGuiRender() {}
#endif
		virtual void OnEvent(Event& event) {}
	public:
		inline bool IsEnabled() const noexcept { return enabled; }
		inline void SetEnabled(bool enabled) noexcept { this->enabled = enabled; }
	private:
		bool enabled = true;
	};
}
