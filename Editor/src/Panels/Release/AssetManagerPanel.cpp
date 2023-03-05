#include "AssetManagerPanel.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include <imgui/imgui.h>

namespace gbc
{
	std::string AssetManagerPanel::AssetTypeToString(Asset::Type assetType)
	{
		switch (assetType)
		{
			case Asset::Type::Texture2D: return "Texture 2D";
			case Asset::Type::Mesh3D: return "Mesh 3D";
		}

		GBC_CORE_ASSERT(false, "Unknown Asset Type!");
		return {};
	}

	AssetManagerPanel::AssetManagerPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void AssetManagerPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);

			//ImGuiHelper::BeginTable("Assets");

			//// TODO: implement this

			//ImGuiHelper::EndTable();

			ImGui::End();
		}
	}
}
