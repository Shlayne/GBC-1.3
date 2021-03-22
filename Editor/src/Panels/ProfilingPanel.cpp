#include "ProfilingPanel.h"
#if GBC_ENABLE_PROFILE_RUNTIME
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Debug/Profiler.h"

namespace gbc
{
	ProfilingPanel::ProfilingPanel(const std::string& name)
		: Panel(name) {}

	void ProfilingPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			focused = ImGui::IsWindowFocused();
			hovered = ImGui::IsWindowHovered();

			ImGuiHelper::BeginTable("Profiling", 2);
			if (ImGuiHelper::Checkbox(profiling ? "Stop Profiling" : "Start Profiling" , &profiling))
			{
				if (profiling)
					GBC_PROFILE_BEGIN_RUNTIME("Runtime", (std::string("ProfileSessions/GBCProfileRuntime") += std::to_string(++profileCount)) += ".json");
				else
					GBC_PROFILE_END_RUNTIME();
			}
			ImGuiHelper::EndTable();

			ImGui::End();
		}
	}
}
#endif
