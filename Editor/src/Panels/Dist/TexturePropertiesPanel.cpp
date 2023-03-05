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

			auto& contentBrowserPanel = editorLayer->contentBrowserPanel;
			if (contentBrowserPanel->HasSelectedFiles())
			{
				// Get selected texture files
				std::vector<const std::filesystem::path*> selectedFilepaths;
				size_t lastSelectedFilepathIndex = 0;

				auto& files = contentBrowserPanel->GetFilesInCurrentDirectory();
				size_t lastSelectedFileIndex = contentBrowserPanel->GetLastSelectedFileIndex();
				for (size_t i = 0; i < files.size(); i++)
				{
					const ContentBrowserPanel::File& file = files[i];

					if (file.selected)
					{
						if (!IsTextureFilepath(file.path))
						{
							selectedFilepaths.clear();
							break;
						}

						if (i == lastSelectedFileIndex)
							lastSelectedFilepathIndex = selectedFilepaths.size();
						selectedFilepaths.push_back(&file.path);
					}
				}

				// If at least one file is selected and the only selected files are textures, then change their properties.
				if (!selectedFilepaths.empty() && ImGuiHelper::BeginTable("TextureProperties", 2))
				{
					auto& assetManager = Application::Get().GetAssetManager();
					Ref<Texture2D> texture = assetManager.GetOrLoad<Texture2D>(*selectedFilepaths[lastSelectedFilepathIndex]);
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

					// TODO: reimplement this (the WHOLE FILE, not just this section)
					//if (changed)
					//	for (const auto& selectedFilepath : selectedFilepaths)
					//		assetManager.UpdateSpecifications(assetManager.GetOrLoad<Texture2D>(*selectedFilepath)->GetUUID(), specs);

					ImGuiHelper::EndTable();
				}
			}

			ImGui::End();
		}
	}
}
