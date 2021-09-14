#include "StasticsPanel.h"
#if GBC_ENABLE_STATS
#include <imgui/imgui.h>
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
			Update();

			if (ImGuiHelper::BeginTable("Renderer", 2))
			{
				char buffer[10];

				ImGuiHelper::Text("Draw Calls");
				ImGuiHelper::NextTableColumn();
				if (_ultoa_s(statistics.drawCalls, buffer, 10))
				{
					buffer[0] = '0';
					buffer[1] = '\0';
				}
				ImGuiHelper::Text(buffer);
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Index Count");
				ImGuiHelper::NextTableColumn();
				if (_ultoa_s(statistics.indexCount, buffer, 10))
				{
					buffer[0] = '0';
					buffer[1] = '\0';
				}
				ImGuiHelper::Text(buffer);
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Vertex Count");
				ImGuiHelper::NextTableColumn();
				if (_ultoa_s(statistics.vertexCount, buffer, 10))
				{
					buffer[0] = '0';
					buffer[1] = '\0';
				}
				ImGuiHelper::Text(buffer);
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Texture Count");
				ImGuiHelper::NextTableColumn();
				if (_ultoa_s(statistics.textureCount, buffer, 10))
				{
					buffer[0] = '0';
					buffer[1] = '\0';
				}
				ImGuiHelper::Text(buffer);
				ImGuiHelper::EndTable();
			}

			ImGui::End();
		}
	}
}
#endif
