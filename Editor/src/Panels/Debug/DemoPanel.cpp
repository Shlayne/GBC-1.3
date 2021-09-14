#include "DemoPanel.h"
#if GBC_CONFIG_DEBUG
#include <imgui/imgui.h>

namespace gbc
{
	DemoPanel::DemoPanel(const std::string& name)
		: Panel(name)
	{
		enabled = false;
	}

	void DemoPanel::OnImGuiRender()
	{
		if (enabled)
			ImGui::ShowDemoWindow(&enabled);
	}
}
#endif
