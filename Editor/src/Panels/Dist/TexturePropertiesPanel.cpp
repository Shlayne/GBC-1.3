#include "TexturePropertiesPanel.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Rendering/Texture.h"
#include <imgui/imgui.h>
#include "Layers/EditorLayer.h"
#include "Panels/Dist/ContentBrowserPanel.h"

namespace gbc
{
	TexturePropertiesPanel::TexturePropertiesPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void TexturePropertiesPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);

			bool editingTextureProperties = true;

			auto& selectedFiles = editorLayer->contentBrowserPanel->GetSelectedFiles();
			if (selectedFiles.empty())
				editingTextureProperties = false;
			else
			{
				for (auto& selectedFile : selectedFiles)
				{
					if (!IsTextureFilepath(selectedFile))
					{
						editingTextureProperties = false;
						break;
					}
				}
			}


			if (editingTextureProperties && ImGuiHelper::BeginTable("TextureProperties", 2))
			{
				auto& assetManager = Application::Get().GetAssetManager();
				Ref<Texture2D> texture = assetManager.GetOrLoadTexture(selectedFiles.back());
				TextureSpecification specs = texture->GetSpecification();
				bool changed = false;

				static constexpr const char* names1[]{ "Linear", "Nearest" };
				static constexpr const char* names2[]{ "ClampToEdge", "Repeat" };

				int selectedItem = static_cast<int>(specs.minFilter);
				if (changed = ImGuiHelper::Combo("Min Filter", &selectedItem, names1, sizeof(names1) / sizeof(*names1)))
				{
					specs.minFilter = static_cast<TextureFilterMode>(selectedItem);
					changed = true;
				}

				ImGuiHelper::NextTableColumn();
				selectedItem = static_cast<int>(specs.magFilter);
				if (ImGuiHelper::Combo("Mag Filter", &selectedItem, names1, sizeof(names1) / sizeof(*names1)) && !changed)
				{
					specs.magFilter = static_cast<TextureFilterMode>(selectedItem);
					changed = true;
				}

				ImGuiHelper::NextTableColumn();
				selectedItem = static_cast<int>(specs.wrapS);
				if (ImGuiHelper::Combo("Wrap S", &selectedItem, names2, sizeof(names2) / sizeof(*names2)) && !changed)
				{
					specs.wrapS = static_cast<TextureWrapMode>(selectedItem);
					changed = true;
				}

				ImGuiHelper::NextTableColumn();
				selectedItem = static_cast<int>(specs.wrapT);
				if (ImGuiHelper::Combo("Wrap T", &selectedItem, names2, sizeof(names2) / sizeof(*names2)) && !changed)
				{
					specs.wrapT = static_cast<TextureWrapMode>(selectedItem);
					changed = true;
				}

				if (changed)
					for (const auto& selectedFile : selectedFiles)
						assetManager.UpdateSpecifications(assetManager.GetOrLoadTexture(selectedFile)->GetUUID(), specs);

				ImGuiHelper::EndTable();
			}

			ImGui::End();
		}
	}
}
