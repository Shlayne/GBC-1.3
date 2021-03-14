#include "StasticsPanel.h"
#if GBC_ENABLE_STATS
#include "imgui/imgui.h"

namespace gbc
{
	StatisticsPanel::StatisticsPanel(const std::string& name, const BasicRenderer::Statistics& statistics)
		: Panel(name), statistics(statistics) {}

	void StatisticsPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin("Statistics", &enabled);
			ImGui::Text("Renderer");
			ImGui::Indent();
			ImGui::Text("Draw Calls:    %d", statistics.drawCalls);
			ImGui::Text("Index Count:   %d", statistics.indexCount);
			ImGui::Text("Vertex Count:  %d", statistics.vertexCount);
			ImGui::Text("Texture Count: %d", statistics.textureCount);
			ImGui::End();
		}
	}
}
#endif
