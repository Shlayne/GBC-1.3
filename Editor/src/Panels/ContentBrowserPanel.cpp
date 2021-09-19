#include "ContentBrowserPanel.h"
#include <regex>
#include <imgui/imgui.h>
#include "GBC/Core/Input.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/IO/DirectoryChange.h"
#include "GBC/IO/FileDialog.h"

namespace gbc
{

	const std::filesystem::path projectAssetDirectory = "Assets";

	ContentBrowserPanel::ContentBrowserPanel(const std::string& name)
		: Panel(name), assetDirectory{ projectAssetDirectory },
		notifier{ GBC_BIND_FUNC(OnDirectoryNotification), assetDirectory.path, DirectoryChange::NotificationType_NameChanged, true }
	{
		cachedDirectories.push_back(projectAssetDirectory);
		currentCachedDirectory = cachedDirectories.begin();

		directoryTexture = Texture2D::Create(LocalTexture2D::Create("Resources/Icons/ContentBrowserPanel/DirectoryIcon.png", 4));
		fileTexture = Texture2D::Create(LocalTexture2D::Create("Resources/Icons/ContentBrowserPanel/FileIcon.png", 4));
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (enabled)
		{
			if (forwardDirectory)
			{
				forwardDirectory = false;
				ForwardDirectory();
			}
			if (backwardDirectory)
			{
				backwardDirectory = false;
				BackwardDirectory();
			}
			if (pushDirectory)
			{
				pushDirectory = false;
				PushDirectory(tempFile.path);
			}
			if (refreshDirectories)
			{
				refreshDirectories = false;
				RefreshDirectories();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.0f, 2.0f });
			ImGui::Begin(name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::PopStyleVar();
			Update();

			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
			{
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Back))
					backwardDirectory = true;
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Front))
					forwardDirectory = true;
			}

			if (ImGui::BeginTable("ContentBrowser", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV))
			{
				ImGui::TableNextColumn();
				ImGui::BeginChild("ContentBrowserHierarchy", { 0.0f, 0.0f }, false, ImGuiWindowFlags_HorizontalScrollbar);
				hierarchyOptionsOpen = false;
				DrawHierarchy(assetDirectory);

				if (hierarchyOptionsOpen)
					ImGui::OpenPopup("ContentBrowserHierarchyOptions");
				if (ImGui::BeginPopup("ContentBrowserHierarchyOptions"))
				{
					if (showDeleteOption && ImGui::MenuItem("Delete"))
					{
						ImGui::CloseCurrentPopup();
						deletingFile = true;
					}

					if (ImGui::MenuItem("Open In Explorer"))
						FileDialog::OpenFolder(tempFile.path); // tempDirectoryPath is guaranteed to not be nullptr here
					ImGui::EndPopup();
				}
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

		bool isAssetsDirectory = directory.path == projectAssetDirectory;
		std::string filenameString;
		if (isAssetsDirectory)
		{
			filenameString = projectAssetDirectory.filename().string();
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
		}
		else
			filenameString = std::filesystem::relative(directory.path, projectAssetDirectory).filename().string();

		bool opened = ImGui::TreeNodeEx(filenameString.c_str(), flags, filenameString.c_str());

		if (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x >= ImGui::GetTreeNodeToLabelSpacing())
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				pushDirectory = true;
				tempFile = { directory.path, true };
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				showDeleteOption = !isAssetsDirectory;
				hierarchyOptionsOpen = true;
				tempFile = { directory.path, true };
			}
		}

		MakeCurrentItemDragDropSource(directory.path);
		MakeCurrentItemDragDropTarget(directory.path);

		if (opened)
		{
			for (const Directory& subdirectory : directory.subdirectories)
				DrawHierarchy(subdirectory);
			ImGui::TreePop();
		}
	}

	void ContentBrowserPanel::DrawSearchBar()
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);

		auto itemSpacing = ImGui::GetStyle().ItemSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { itemSpacing.x * 0.5f, itemSpacing.y });

		if (ImGui::Button("<"))
			backwardDirectory = true;
		ImGui::SameLine();
		if (ImGui::Button(">"))
			forwardDirectory = true;
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
		int32_t columnCount = std::max(1, static_cast<int32_t>((ImGui::GetContentRegionAvail().x - padding) / (thumbnailSize + 2.0f * padding)));

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
					{
						GBC_CORE_ERROR("Failed to create directory: {0}", error.what());
					}
#else
					catch (...) {}
#endif

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
					void* textureID = (void*)static_cast<size_t>((file.isDirectory ? directoryTexture : fileTexture)->GetRendererID());
					ImGui::ImageButton(textureID, { thumbnailSize, thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

					MakeCurrentItemDragDropSource(file.path);
					if (file.isDirectory)
						MakeCurrentItemDragDropTarget(file.path);

					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (file.isDirectory)
							{
								PushDirectory(*currentCachedDirectory / filename);
								changed = true;
							}
						}
						else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
						{
							ImGui::OpenPopup("ContentBrowserExplorerOptions");
							clickedFileIndex = i;
						}
					}

					if (ImGui::BeginPopup("ContentBrowserExplorerOptions"))
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

						if (file.isDirectory)
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
							{
								GBC_CORE_ERROR("Failed to rename {0}: {1}", file.isDirectory ? "directory" : "file", error.what());
							}
#else
							catch (...) {}
#endif

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

				if (deletingFile && clickedFileIndex == i)
					tempFile = file;
			}

			if (deletingFile)
				ShowDeleteConfirmationMessage();

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
			refreshDirectories = true;
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
				refreshDirectories = true;
		}
	}

	void ContentBrowserPanel::PushDirectory(const std::filesystem::path& path)
	{
		if (*currentCachedDirectory == path)
			return;

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

		refreshDirectories = true;
	}

	void ContentBrowserPanel::RefreshDirectories()
	{
		// If a folder in our virtual file system was deleted on disk,
		// then go all the way up
		while (!std::filesystem::exists(*currentCachedDirectory))
		{
			if (*--currentCachedDirectory == projectAssetDirectory)
			{
				// User used filesystem or other means to delete the asset directory
				std::error_code error;
				if (!std::filesystem::create_directories(projectAssetDirectory, error))
				{
					GBC_CORE_FATAL("Unable to recreate Assets directory after user deleted it on using their OS's file system.");
					GBC_CORE_FATAL(error.message());
					GBC_CORE_ASSERT(false);
					// TODO: handle this in release and dist!!
				}
				break;
			}
		}

		// Update current cached directory text
		currentCachedDirectoryText.clear();
		auto& directoryString = currentCachedDirectory->native();
		for (auto wc : directoryString)
		{
			if (wc == L'\\' || wc == L'/')
				currentCachedDirectoryText += "  /  ";
			else
				currentCachedDirectoryText += static_cast<char>(wc);
		}

		assetDirectory.subdirectories.clear();
		RefreshDirectory(assetDirectory);

		files.clear();
		size_t directoryInsert = 0;

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

	void ContentBrowserPanel::ShowDeleteConfirmationMessage()
	{		
		std::error_code error;
		bool isUnemptyDirectory = tempFile.isDirectory && !std::filesystem::is_empty(tempFile.path, error);
#if GBC_ENABLE_LOGGING
		if (error) GBC_CORE_ERROR(error.message());
#endif

		std::string filenameString = tempFile.path.filename().string();
		bool action;
		if (ImGuiHelper::ConfirmAction("Delete?", &action, "Are you sure you want to delete \"%s\"%s?", filenameString.c_str(),
			isUnemptyDirectory ? " and all its contents" : ""))
		{
			if (action)
			{
				try
				{
					if (isUnemptyDirectory)
						std::filesystem::remove_all(tempFile.path);
					else
						std::filesystem::remove(tempFile.path);
				}
#if GBC_ENABLE_LOGGING
				catch (std::filesystem::filesystem_error& error)
				{
					GBC_CORE_ERROR("Failed to delete {0}: {1}", tempFile.isDirectory ? "directory" : "file", error.what());
				}
#else
				catch (...) {}
#endif
			}

			ImGui::CloseCurrentPopup();

			if (hierarchyOptionsOpen)
				hierarchyOptionsOpen = false;
			else
				clickedFileIndex = files.size();
			deletingFile = false;
		}
	}

	void ContentBrowserPanel::MakeCurrentItemDragDropSource(const std::filesystem::path& sourcePath)
	{
		if (ImGui::BeginDragDropSource())
		{
			size_t bufferSize = (sourcePath.native().size() + 1) * sizeof(wchar_t);
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", sourcePath.c_str(), bufferSize, ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}

	void ContentBrowserPanel::MakeCurrentItemDragDropTarget(const std::filesystem::path& targetPath)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGuiHelper::AcceptDragDropPayloadIf("CONTENT_BROWSER_ITEM",
				// Don't move a folder into itself nor its subdirectories
				[&targetPath](void* sourceData) { return !targetPath.native().starts_with(static_cast<const wchar_t*>(sourceData)); }))
			{
				std::filesystem::path oldPath = static_cast<const wchar_t*>(payload->Data);
				std::filesystem::path newPath = targetPath / oldPath.filename();

				std::error_code error;
				std::filesystem::rename(oldPath, newPath, error);
#if GBC_ENABLE_LOGGING
				if (error) GBC_CORE_ERROR(error.message());
#endif
			};
			
			ImGui::EndDragDropTarget();
		}
	}

	bool ContentBrowserPanel::OnDirectoryNotification(bool error)
	{
		if (error)
		{
			GBC_CORE_ERROR("Content Browser directory notification failed! Save and restart highly recommended.");
			return false; // return value here does not matter as it is not used
		}

		refreshDirectories = true;
		return true;
	}
}
