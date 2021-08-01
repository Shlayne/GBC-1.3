#include "Panel.h"
#include "imgui/imgui.h"

namespace gbc
{
	Panel::Panel(const std::string& name)
		: name(name) {}

	void Panel::Update()
	{
		focused = ImGui::IsWindowFocused();
		hovered = ImGui::IsWindowHovered();

		ImVec2 imguiSize = ImGui::GetContentRegionAvail();
		sizeChanged = imguiSize.x != size.x || imguiSize.y != size.y;
		if (sizeChanged)
			size = {imguiSize.x, imguiSize.y};

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 viewportOffset = ImGui::GetCursorPos();
		position = {windowPos.x + viewportOffset.x, windowPos.y + viewportOffset.y};
	}
}
