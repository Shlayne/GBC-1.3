#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>
#include <regex>
#include <atomic>
#include "GBC/Core/Input.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/IO/FileDialog.h"
#include "GBC/IO/DirectoryChange.h"

namespace gbc
{
	const std::filesystem::path projectAssetDirectory = "Assets";

	ContentBrowserPanel::ContentBrowserPanel(const std::string& name)
		: Panel(name), assetDirectory{ projectAssetDirectory }
	{
		cachedDirectories.push_back(projectAssetDirectory);
		currentCachedDirectory = cachedDirectories.begin();
		RefreshDirectory(true);

		directoryTexture = Texture2D::Create(LocalTexture2D::Create("Resources/Icons/ContentBrowserPanel/DirectoryIcon.png", 4));
		fileTexture = Texture2D::Create(LocalTexture2D::Create("Resources/Icons/ContentBrowserPanel/FileIcon.png", 4));

		directoryChangeNotifier = DirectoryChange::CreateNotifier(GBC_BIND_FUNC(OnDirectoryNotification), assetDirectory.path, NotificationType_FileNameChanged | NotificationType_DirectoryNameChanged, true);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (enabled)
		{
			if (refreshDirectories)
			{
				refreshDirectories = false;
				RefreshDirectory(true);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.0f, 2.0f });
			ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PopStyleVar();
			Update();

			if (ImGui::BeginTable("ContentBrowserTable1", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
			{
				ImGui::TableNextColumn();
				ImGui::BeginChild("ContentBrowserHierarchy", { 0.0f, 0.0f }, false, ImGuiWindowFlags_HorizontalScrollbar);
				DrawHierarchy(assetDirectory);
				ImGui::EndChild();

				ImGui::TableNextColumn();
				ImGui::BeginChild("ContentBrowserSearchBarExplorer", { 0.0f, 0.0f }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				if (ImGui::BeginTable("ContentBrowserSearchBar", 1, ImGuiTableFlags_NoPadOuterX))
				{
					ImGui::TableNextColumn();
					DrawSearchBar();
					ImGui::Separator();
					ImGui::TableNextColumn();
					if (ImGui::BeginChild("ContentBrowserExplorer"))
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
						DrawExplorer();
						ImGui::EndChild();
					}
					ImGui::EndTable();
				}
				ImGui::EndChild();

				ImGui::EndTable();
			}

			ImGui::End();
		}
	}

	void ContentBrowserPanel::DrawHierarchy(const Directory& directory)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (*currentCachedDirectory == directory.path)
			flags |= ImGuiTreeNodeFlags_Selected;
		if (directory.subdirectories.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		std::string filenameString;
		if (directory.path == projectAssetDirectory)
		{
			filenameString = projectAssetDirectory.filename().string();
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
			filenameString = std::filesystem::relative(directory.path, projectAssetDirectory).filename().string();

		bool opened = ImGui::TreeNodeEx(filenameString.c_str(), flags, filenameString.c_str());

		bool setDirectory = false;
		if (!(ImGui::GetMousePos().x - ImGui::GetItemRectMin().x < ImGui::GetTreeNodeToLabelSpacing()) && ImGui::IsItemClicked())
			setDirectory = true;

		if (opened)
		{
			for (const Directory& subdirectory : directory.subdirectories)
				DrawHierarchy(subdirectory);
			ImGui::TreePop();
		}

		if (setDirectory)
			PushDirectory(directory.path);
	}

	void ContentBrowserPanel::DrawSearchBar()
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);

		auto itemSpacing = ImGui::GetStyle().ItemSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { itemSpacing.x * 0.5f, itemSpacing.y });

		if (ImGui::Button("<") || (ImGui::IsMouseReleased(3) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)))
			BackwardDirectory();

		ImGui::SameLine();
		if (ImGui::Button(">") || (ImGui::IsMouseReleased(4) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)))
			ForwardDirectory();

		ImGui::SameLine();
		if (ImGui::Button("Refresh"))
			refreshDirectories = true;

		ImGui::SameLine();
		ImGui::PushItemWidth(200.0f);
		if (ImGui::InputTextWithHint("", "Search...", searchBuffer, searchBufferSize))
			searchSize = std::strlen(searchBuffer);

		ImGui::SameLine();
		ImGui::Text(currentCachedDirectoryText.c_str());

		ImGui::PopStyleVar();
	}

	void ContentBrowserPanel::DrawExplorer()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 2.0f, 2.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

		float padding = ImGui::GetStyle().CellPadding.x;
		float thumbnailSize = 96.0f;
		int columnCount = std::max(1, static_cast<int>((ImGui::GetContentRegionAvail().x - padding) / (thumbnailSize + 2.0f * padding)));

		if (ImGui::BeginTable("ContentBrowserExplorer", columnCount, ImGuiTableFlags_SameWidths))
		{
			ImGui::TableNextColumn();

			if (creatingDirectory)
			{
				ImGui::PushID(-1);

				auto textureID = (void*)static_cast<size_t>(directoryTexture->GetRendererID());
				ImGui::ImageButton(textureID, { thumbnailSize, thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

				if (ImGuiHelper::InputText(fileNameBuffer))
				{
					if (fileNameBuffer[0] == '\0')
					{
						memcpy_s(fileNameBuffer, fileNameBufferSize, defaultFolderName, defaultFolderNameSize);
						fileNameSize = defaultFolderNameSize;
					}

					try
					{
						std::filesystem::create_directory(*currentCachedDirectory / fileNameBuffer);
					}
#if GBC_ENABLE_LOGGING
					catch (std::filesystem::filesystem_error& error)
#else
					catch (...)
#endif
					{
						GBC_CORE_ERROR("Failed to create directory: {0}", error.what());
					}

					creatingDirectory = false;
				}

				ImGui::TableNextColumn();
				ImGui::PopID();
			}

			bool changed = false;
			for (size_t i = 0; i < files.size() && !changed; i++)
			{
				ImGui::PushID(static_cast<int>(i));

				File& file = files[i];

				auto relativePath = std::filesystem::relative(file.path, projectAssetDirectory);
				auto filename = relativePath.filename();
				auto filenameString = filename.string();

				bool passedSearch = false;
				if (searchSize == 0)
					passedSearch = true;
				else
				{
					try
					{
						std::regex regex(searchBuffer, searchSize, std::regex_constants::icase);
						if (std::regex_search(filenameString.c_str(), regex, std::regex_constants::match_any))
							passedSearch = true;
					}
					catch (...)
					{
						// Apparently, when an unescaped backslash is at the end of the regex, it throws an error, which is complete bullshit.
					}
				}

				if (passedSearch)
				{
					void* textureID = (void*)static_cast<size_t>((file.directory ? directoryTexture : fileTexture)->GetRendererID());
					ImGui::ImageButton(textureID, { thumbnailSize, thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

					if (ImGui::BeginDragDropSource())
					{
						auto relativePathString = projectAssetDirectory / relativePath;
						size_t bufferSize = (relativePathString.native().size() + 1) * sizeof(wchar_t);
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePathString.c_str(), bufferSize, ImGuiCond_Once);
						ImGui::EndDragDropSource();
					}

					if (file.directory && ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							std::filesystem::path oldPath = static_cast<const wchar_t*>(payload->Data);
							std::filesystem::path newPath = file.path / oldPath.filename();
							std::filesystem::rename(oldPath, newPath);
						}
						ImGui::EndDragDropTarget();
					}

					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (file.directory)
							{
								PushDirectory(*currentCachedDirectory / filename);
								changed = true;
							}
						}
						else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						{
							ImGui::OpenPopup("ContentBrowserFileOptions");
							clickedFileIndex = i;
						}
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

						if (file.directory)
						{
							if (ImGui::MenuItem("Open In Explorer"))
							{
								std::string directoryPath = (*currentCachedDirectory / filename).string();
								FileDialog::OpenFolder(directoryPath);
							}
						}
						ImGui::EndPopup();
					}

					if (clickedFileIndex == i && renamingFile)
					{
						if (ImGuiHelper::InputText(fileNameBuffer))
						{
							try
							{
								std::filesystem::rename(file.path, *currentCachedDirectory / fileNameBuffer);
							}
	#if GBC_ENABLE_LOGGING
							catch (std::filesystem::filesystem_error& error)
	#else
							catch (...)
	#endif
							{
								GBC_CORE_ERROR("Failed to rename {0}: {1}", file.directory ? "directory" : "file", error.what());
							}

							renamingFile = false;
						}
					}
					else
					{
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.5f * padding);
						ImGui::TextWrapped(filenameString.c_str());
					}

					ImGui::TableNextColumn();
				}

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
							}
#if GBC_ENABLE_LOGGING
							catch (std::filesystem::filesystem_error& error)
#else
							catch (...)
#endif
							{
								GBC_CORE_ERROR("Failed to remove {0}: {1}", file.directory ? "directory" : "file", error.what());
							}
						}

						ImGui::CloseCurrentPopup();
						clickedFileIndex = files.size();
						deletingFile = false;
					}
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
						creatingDirectory = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Open In Explorer"))
				{
					std::string directoryPath = currentCachedDirectory->string();
					FileDialog::OpenFolder(directoryPath);
				}

				ImGui::EndPopup();
			}

			ImGui::EndTable();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	void ContentBrowserPanel::ForwardDirectory()
	{
		auto lastDirectory = currentCachedDirectory;
		++currentCachedDirectory;
		if (currentCachedDirectory == cachedDirectories.end())
			currentCachedDirectory = lastDirectory;
		else if (!std::filesystem::exists(*currentCachedDirectory))
		{
			cachedDirectories.erase(currentCachedDirectory);
			currentCachedDirectory = lastDirectory;
		}
		else
			RefreshDirectory();
	}

	void ContentBrowserPanel::BackwardDirectory()
	{
		if (currentCachedDirectory != cachedDirectories.begin())
		{
			auto lastDirectory = currentCachedDirectory;
			if (!std::filesystem::exists(*--currentCachedDirectory))
			{
				cachedDirectories.erase(cachedDirectories.begin(), lastDirectory);
				currentCachedDirectory = cachedDirectories.begin();
			}
			else
				RefreshDirectory();
		}
	}

	void ContentBrowserPanel::PushDirectory(const std::filesystem::path& path)
	{
		if (currentCachedDirectory != cachedDirectories.end())
		{
			auto temp = currentCachedDirectory;
			cachedDirectories.erase(++currentCachedDirectory, cachedDirectories.end());
			currentCachedDirectory = temp;
		}

		if (cachedDirectories.size() == maxCachedDirectories)
		{
			cachedDirectories.pop_front();
			cachedDirectories.push_back(path);
		}
		else
		{
			cachedDirectories.push_back(path);
			++currentCachedDirectory;
		}

		RefreshDirectory();
	}

	void ContentBrowserPanel::RefreshDirectory(bool refreshAssetDirectory)
	{
		while (!std::filesystem::exists(*currentCachedDirectory))
		{
			if (*--currentCachedDirectory == projectAssetDirectory)
			{
				// User used filesystem or other means to delete the asset directory
				std::filesystem::create_directory(projectAssetDirectory);
				break;
			}
		}

		UpdateCurrentCachedDirectoryText();

		if (refreshAssetDirectory)
		{
			assetDirectory.subdirectories.clear();
			RefreshDirectory(assetDirectory);
		}

		files.clear();
		directoryInsert = 0;

		for (const auto& entry : std::filesystem::directory_iterator(*currentCachedDirectory))
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

	void ContentBrowserPanel::RefreshDirectory(Directory& subdirectory)
	{
		for (const auto& entry : std::filesystem::directory_iterator(subdirectory.path))
			if (entry.is_directory())
				RefreshDirectory(subdirectory.subdirectories.emplace_back(entry.path()));
	}

	void ContentBrowserPanel::UpdateCurrentCachedDirectoryText()
	{
		currentCachedDirectoryText.clear();
		auto& directoryString = currentCachedDirectory->native();
		for (auto wc : directoryString)
		{
			if (wc == L'\\' || wc == L'/')
				currentCachedDirectoryText += "  /  ";
			else
				currentCachedDirectoryText += static_cast<char>(wc);
		}
	}

	bool ContentBrowserPanel::OnDirectoryNotification(bool error)
	{
		GBC_CORE_ERROR("ContentBrowserPanel::OnDirectoryNotification(error={0})", error);

		if (error)
		{
			GBC_CORE_ERROR("Content Browser directory notification failed! Save and restart recommended.");
			return false; // return value here does not matter as it is not used
		}

		refreshDirectories = true;
		return true;
	}
}
