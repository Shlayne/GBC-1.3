#include "ProfilingPanel.h"
#if GBC_ENABLE_PROFILE_RUNTIME
#include "GBC/Debug/Profiler.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include <imgui/imgui.h>

namespace gbc
{
	ProfilingPanel::ProfilingPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void ProfilingPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			Update();

			if (ImGuiHelper::BeginTable("Profiling", 2))
			{
				if (ImGuiHelper::Checkbox(profiling ? "Stop Profiling" : "Start Profiling", &profiling))
				{
					if (profiling)
						GBC_PROFILE_BEGIN_RUNTIME("Runtime", (std::string("ProfileSessions/GBCProfileRuntime") += std::to_string(++profileCount)) += ".json");
					else
						GBC_PROFILE_END_RUNTIME();
				}

				ImGuiHelper::EndTable();
			}

			ImGui::End();
		}
	}
}
#endif
