#include "RendererInfoPanel.h"
#include <imgui/imgui.h>
#include "GBC/Core/Application.h"
#include "GBC/ImGui/ImGuiHelper.h"

namespace gbc
{
	RendererInfoPanel::RendererInfoPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer)
	{
		const auto& context = Application::Get().GetWindow().GetContext();
		vendor = context.GetVendor();
		renderer = context.GetRenderer();
		version = context.GetVersion();
	}

	void RendererInfoPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			Update();

			if (ImGuiHelper::BeginTable("RendererInfo"))
			{
				ImGuiHelper::Text("Vendor", vendor);
				ImGuiHelper::NextTableColumn();
				ImGuiHelper::Text("Renderer", renderer);
				ImGuiHelper::NextTableColumn();
				ImGuiHelper::Text("Version", version);
				ImGuiHelper::EndTable();
			}

			ImGui::End();
		}
	}
}
