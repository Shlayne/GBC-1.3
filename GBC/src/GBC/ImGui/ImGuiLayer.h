#pragma once

#if GBC_ENABLE_IMGUI
#include "GBC/Core/Layer.h"

namespace gbc
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		inline void SetBlockEvents(bool blockEvents) { this->blockEvents = blockEvents; }
	private:
		bool blockEvents = false;
	};
}
#endif
