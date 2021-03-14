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

		bool IsUsingKeyEvents() const;
		bool IsUsingMouseEvents() const;

		void SetBlockEvents(bool blockEvents) { this->blockEvents = blockEvents; }
	private:
		void Init();

		bool blockEvents = false;
	};
}
#endif
