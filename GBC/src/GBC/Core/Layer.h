#pragma once

#include "Timestep.h"
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
#endif
		virtual void OnEvent(Event& event) {}
	};
}
