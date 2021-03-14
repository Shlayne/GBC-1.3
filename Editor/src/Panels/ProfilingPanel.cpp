#include "ProfilingPanel.h"
#if GBC_ENABLE_PROFILE_RUNTIME
#include "imgui/imgui.h"
#include "GBC/Debug/Profiler.h"

namespace gbc
{
	ProfilingPanel::ProfilingPanel(const std::string& name)
		: Panel(name) {}

	void ProfilingPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin("Profiling", &enabled);
			if (ImGui::Checkbox(profiling ? "Stop Profiling" : "Start Profiling", &profiling))
			{
				if (profiling)
					GBC_PROFILE_BEGIN_RUNTIME("Runtime", (std::string("ProfileSessions/GBCProfileRuntime") += std::to_string(++profileCount)) += ".json");
				else
					GBC_PROFILE_END_RUNTIME();
			}
			ImGui::End();
		}
	}
}
#endif
