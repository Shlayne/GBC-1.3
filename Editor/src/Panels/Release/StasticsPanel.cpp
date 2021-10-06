#include "StasticsPanel.h"
#if GBC_ENABLE_STATS
#include <imgui/imgui.h>
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Rendering/Renderers/Renderer2D.h"
#include "GBC/Rendering/Renderers/Renderer3D.h"

namespace gbc
{
	StatisticsPanel::StatisticsPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void StatisticsPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			Update();

			ImGui::Text("Renderer2D");
			ImGui::Indent();
			if (ImGuiHelper::BeginTable("Renderer2D", 2))
			{
				auto& statistics = Renderer2D::GetStatistics();
				std::string text;

				ImGuiHelper::Text("Draw Call Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetDrawCallCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Quad Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetQuadCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Vertex Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetVertexCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Index Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetIndexCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Texture Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetTextureCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::EndTable();
			}
			ImGui::Unindent();

			ImGui::Text("Renderer3D");
			ImGui::Indent();
			if (ImGuiHelper::BeginTable("Renderer3D", 2))
			{
				auto& statistics = Renderer3D::GetStatistics();
				std::string text;

				ImGuiHelper::Text("Draw Call Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetDrawCallCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Mesh Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetMeshCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Vertex Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetVertexCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Index Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetIndexCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::NextTableColumn();

				ImGuiHelper::Text("Texture Count");
				ImGuiHelper::NextTableColumn();
				text = std::to_string(statistics.GetTextureCount());
				ImGuiHelper::Text(text.c_str());
				ImGuiHelper::EndTable();
			}
			ImGui::Unindent();

			ImGui::End();
		}
	}
}
#endif
