#include "SceneViewportPanel.h"
#include "imgui/imgui.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	SceneViewportPanel::SceneViewportPanel(const std::string& name, const Ref<Framebuffer>& framebuffer)
		: Panel(name), framebuffer(framebuffer) {}

	void SceneViewportPanel::OnImGuiRender()
	{
		if (enabled)
		{
			viewportSizeChanged = false;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::Begin(name.c_str(), &enabled);

			bool sceneFocused = ImGui::IsWindowFocused();
			bool sceneHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiWrapper().SetBlockEvents(!sceneFocused);

			ImVec2 size = ImGui::GetContentRegionAvail();
			viewportSizeChanged = size.x != viewportSize.x || size.y != viewportSize.y;
			viewportSize = {size.x, size.y};
			ImGui::Image((void*)(size_t)framebuffer->GetColorAttachment(), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
			
			ImGui::End();
			ImGui::PopStyleVar();
		}
	}
}
