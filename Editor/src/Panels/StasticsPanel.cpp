#include "StasticsPanel.h"
#if GBC_ENABLE_STATS
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"

namespace gbc
{
	StatisticsPanel::StatisticsPanel(const std::string& name, const BasicRenderer::Statistics& statistics)
		: Panel(name), statistics(statistics) {}

	void StatisticsPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			ImGuiHelper::BeginTable("Renderer", 2);
			ImGuiHelper::Text("Draw Calls"); ImGuiHelper::NextTableColumn(); ImGuiHelper::Text(std::to_string(statistics.drawCalls)); ImGuiHelper::NextTableColumn();
			ImGuiHelper::Text("Index Count"); ImGuiHelper::NextTableColumn(); ImGuiHelper::Text(std::to_string(statistics.indexCount)); ImGuiHelper::NextTableColumn();
			ImGuiHelper::Text("Vertex Count"); ImGuiHelper::NextTableColumn(); ImGuiHelper::Text(std::to_string(statistics.vertexCount)); ImGuiHelper::NextTableColumn();
			ImGuiHelper::Text("Texture Count"); ImGuiHelper::NextTableColumn(); ImGuiHelper::Text(std::to_string(statistics.textureCount));
			ImGuiHelper::EndTable();
			ImGui::End();
		}
	}
}
#endif
