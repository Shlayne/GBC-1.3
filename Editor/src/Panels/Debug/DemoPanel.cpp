#include "DemoPanel.h"
#if GBC_CONFIG_DEBUG
#include <imgui/imgui.h>

namespace gbc
{
	DemoPanel::DemoPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer)
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
