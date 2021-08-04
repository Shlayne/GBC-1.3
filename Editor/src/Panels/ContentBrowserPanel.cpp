#include "ContentBrowserPanel.h"
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Core/Input.h"

namespace gbc
{
	const std::filesystem::path assetDirectory = "Assets";

	ContentBrowserPanel::ContentBrowserPanel(const std::string& name)
		: Panel(name), currentDirectory(assetDirectory)
	{
		RefreshDirectory();

		directoryTexture = Texture::CreateRef(CreateRef<LocalTexture2D>("Resources/Icons/ContentBrowserPanel/DirectoryIcon.png", 4));
		fileTexture = Texture::CreateRef(CreateRef<LocalTexture2D>("Resources/Icons/ContentBrowserPanel/FileIcon.png", 4));
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			Update();

			if (ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
			{
				ImGui::TableNextColumn();
				ImGui::BeginChild("ContentBrowserHierarchy", {0, 0}, false, ImGuiWindowFlags_HorizontalScrollbar);
				DrawHierarchy(assetDirectory);
				ImGui::EndChild();

				ImGui::TableNextColumn();
				ImGui::BeginChild("ContentBrowserExplorer");
				DrawBrowser();
				ImGui::EndChild();

				ImGui::EndTable();
			}
			ImGui::End();
		}
	}

	void ContentBrowserPanel::DrawHierarchy(const std::filesystem::path& path)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (currentDirectory == path)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool hasSubDirectories = false;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				hasSubDirectories = true;
				break;
			}
		}
		if (!hasSubDirectories)
			flags |= ImGuiTreeNodeFlags_Leaf;

		std::string filenameString;
		if (path == assetDirectory)
		{
			filenameString = assetDirectory.filename().string();
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
			filenameString = std::filesystem::relative(path, assetDirectory).filename().string();

		bool opened = ImGui::TreeNodeEx(filenameString.c_str(), flags, filenameString.c_str());

		if (!(ImGui::GetMousePos().x - ImGui::GetItemRectMin().x < ImGui::GetTreeNodeToLabelSpacing()) && ImGui::IsItemClicked())
			SetDirectory(path);

		if (opened)
		{
			for (const auto& entry : std::filesystem::directory_iterator(path))
				if (entry.is_directory())
					DrawHierarchy(entry.path());
			ImGui::TreePop();
		}
	}

	void ContentBrowserPanel::DrawBrowser()
	{
		static constexpr float padding = 1.0f;
		static float thumbnailSize = 96.0f;
		int columnCount = std::max(1, static_cast<int>(ImGui::GetContentRegionAvail().x / (thumbnailSize + 2.0f * padding)));

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {padding, padding});
		ImGui::PushStyleColor(ImGuiCol_Button, {0.0f, 0.0f, 0.0f, 0.0f});

		if (ImGui::BeginTable("ContentBrowserExplorer", columnCount, ImGuiTableFlags_SameWidths))
		{
			ImGui::TableNextColumn();

			bool deferredChange = false;

			if (creatingFolder)
			{
				ImGui::PushID(-1);

				auto textureID = (void*)static_cast<size_t>(directoryTexture->GetRendererID());
				ImGui::ImageButton(textureID, {thumbnailSize, thumbnailSize}, {0.0f, 1.0f}, {1.0f, 0.0f});

				if (ImGuiHelper::InputText(fileNameBuffer))
				{
					if (fileNameBuffer[0] == '\0')
					{
						memcpy_s(fileNameBuffer, fileNameBufferSize, defaultFolderName, defaultFolderNameSize);
						fileNameSize = defaultFolderNameSize;
					}

					try
					{
						std::filesystem::create_directory(currentDirectory / fileNameBuffer);
						deferredChange = true;
					}
					catch (std::filesystem::filesystem_error& error)
					{
						GBC_ERROR("Failed to create directory: {0}", error.what());
					}

					creatingFolder = false;
				}

				ImGui::TableNextColumn();
				ImGui::PopID();
			}

			bool changed = false;
			for (size_t i = 0; i < files.size() && !changed; i++)
			{
				ImGui::PushID(static_cast<int>(i));

				File& file = files[i];

				auto relativePath = std::filesystem::relative(file.path, assetDirectory);
				auto filename = relativePath.filename();
				auto filenameString = filename.string();

				void* textureID = (void*)static_cast<size_t>(fileTexture->GetRendererID());
				if (file.directory)
					textureID = (void*)static_cast<size_t>(directoryTexture->GetRendererID());

				ImGui::ImageButton(textureID, {thumbnailSize, thumbnailSize}, {0.0f, 1.0f}, {1.0f, 0.0f});

				if (ImGui::BeginDragDropSource())
				{
					auto relativePathString = (assetDirectory / relativePath).string();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePathString.c_str(), relativePathString.size() + 1, ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsItemHovered())
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (file.directory)
						{
							SetDirectory(currentDirectory / filename);
							changed = true;
						}
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup("ContentBrowserFileOptions");
						clickedFileIndex = i;
					}
				}

				if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right | ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_NoOpenOverExistingPopup))
				{
					if (ImGui::BeginMenu("New"))
					{
						if (ImGui::MenuItem("Folder"))
						{
							memcpy_s(fileNameBuffer, fileNameBufferSize, defaultFolderName, defaultFolderNameSize);
							if (fileNameSize > defaultFolderNameSize)
								memset(fileNameBuffer + defaultFolderNameSize, 0, fileNameSize - defaultFolderNameSize);
							fileNameSize = 0;
							creatingFolder = true;
						}

						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("ContentBrowserFileOptions"))
				{
					if (ImGui::MenuItem("Delete"))
					{
						ImGui::CloseCurrentPopup();
						deletingFile = true;
					}
					if (ImGui::MenuItem("Rename"))
					{
						ImGui::CloseCurrentPopup();

						memcpy_s(fileNameBuffer, fileNameBufferSize, filenameString.c_str(), filenameString.size() + 1);
						renamingFile = true;
					}

					ImGui::EndPopup();
				}

				if (clickedFileIndex == i && renamingFile)
				{
					if (ImGuiHelper::InputText(fileNameBuffer))
					{
						try
						{
							std::filesystem::rename(file.path, currentDirectory / fileNameBuffer);
							deferredChange = true;
						}
						catch (std::filesystem::filesystem_error& error)
						{
							GBC_ERROR("Failed to rename {0}: {1}", file.directory ? "directory" : "file", error.what());
						}

						renamingFile = false;
					}
				}
				else
					ImGui::TextWrapped(filenameString.c_str());

				ImGui::TableNextColumn();
				ImGui::PopID();

				if (clickedFileIndex == i && deletingFile)
				{
					bool action;
					if (ImGuiHelper::ConfirmAction("Delete?", &action, "Are you sure you want to delete \"%s\"?", filenameString.c_str()))
					{
						if (action)
						{
							try
							{
								std::filesystem::remove(file.path);
								files.erase(files.begin() + i);
								i--;
								//deferredChange = true; // not necessary
							}
							catch (std::filesystem::filesystem_error& error)
							{
								GBC_ERROR("Failed to remove {0}: {1}", file.directory ? "directory" : "file", error.what());
							}
						}

						ImGui::CloseCurrentPopup();
						clickedFileIndex = files.size();
						deletingFile = false;
					}
				}
			}

			ImGui::EndTable();

			if (deferredChange)
				RefreshDirectory();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void ContentBrowserPanel::SetDirectory(const std::filesystem::path& path)
	{
		currentDirectory = path;
		RefreshDirectory();
	}

	void ContentBrowserPanel::RefreshDirectory()
	{
		files.clear();
		directoryInsert = 0;

		for (const auto& entry : std::filesystem::directory_iterator(currentDirectory))
		{
			bool isDirectory = entry.is_directory();

			if (isDirectory)
			{
				files.emplace(files.begin() + directoryInsert, entry.path(), isDirectory);
				directoryInsert++;
			}
			else
				files.emplace_back(entry.path(), isDirectory);
		}

		clickedFileIndex = files.size();
	}
}
