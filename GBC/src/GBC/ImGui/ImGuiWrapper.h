#pragma once

#if GBC_ENABLE_IMGUI
#include "GBC/Events/Event.h"

namespace gbc
{
	class ImGuiWrapper
	{
	public:
		ImGuiWrapper();
		~ImGuiWrapper();

		void Begin();
		void End();

		void OnEvent(Event& event);
		inline void SetBlockEvents(bool blockEvents) noexcept { this->blockEvents = blockEvents; }
	private:
		bool blockEvents = false;
	};
}
#endif
