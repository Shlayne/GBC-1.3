#include "ContentBrowserPanel.h"
#include "GBC/Core/Application.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/Core/Input.h"
#include "GBC/Events/MouseEvents.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/IO/DirectoryChange.h"
#include "GBC/IO/FileDialog.h"
#include "GBC/Rendering/Renderers/Renderer2D.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <regex>
#include "Layers/EditorLayer.h"

namespace gbc
{
	static constexpr ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_ForceScrollLeft;

	ContentBrowserPanel::ContentBrowserPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer), assetDirectory{ editorLayer->projectAssetDirectory },
		notifier{ GBC_BIND_FUNC(OnDirectoryNotification), assetDirectory.path, DirectoryChange::NotificationType_NameChanged, true }
	{
		cachedDirectories.push_back(editorLayer->projectAssetDirectory);
		currentCachedDirectory = cachedDirectories.begin();

		auto& assetManager = Application::Get().GetAssetManager();
		directoryTexture = assetManager.GetOrLoad<Texture2D>(L"Resources/Icons/ContentBrowserPanel/DirectoryIcon.png");
		fileTexture = assetManager.GetOrLoad<Texture2D>(L"Resources/Icons/ContentBrowserPanel/FileIcon.png");
		directoryTexture = Texture2D::Create(LocalTexture2D::Create(L"Resources/Icons/ContentBrowserPanel/DirectoryIcon.png", 4));
		fileTexture = Texture2D::Create(LocalTexture2D::Create(L"Resources/Icons/ContentBrowserPanel/FileIcon.png", 4));
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
				ImGui::BeginChild("ContentBrowserHierarchy", { 0.0f }, false, ImGuiWindowFlags_HorizontalScrollbar);
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
				ImGui::BeginChild("ContentBrowserSearchBarExplorer", { 0.0f }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				if (ImGui::BeginTable("ContentBrowserSearchBar", 1, ImGuiTableFlags_NoPadOuterX))
				{
					ImGui::TableNextColumn();
					DrawSearchBar();
					ImGui::Separator();
					ImGui::TableNextColumn();

					ImGui::BeginChild("ContentBrowserExplorer");
					DrawExplorer();
					ImGui::EndChild();

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

		auto& projectAssetDirectory = editorLayer->projectAssetDirectory;
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
		// Basically does Update() but for the explorer child window instead
		{
			ImVec2 imguiSize = ImGui::GetContentRegionAvail();
			explorerSizeChanged = imguiSize.x != size.x || imguiSize.y != size.y;
			if (explorerSizeChanged)
				explorerSize = { imguiSize.x, imguiSize.y };

			explorerPosition = ImGui::GetWindowPos() + ImGui::GetCursorPos() - ImGui::GetMainViewport()->Pos;
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 2.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0.0f });

		float padding = ImGui::GetStyle().CellPadding.x;
		float thumbnailSize = 96.0f;
		int32_t columnCount = std::max(1, static_cast<int32_t>((ImGui::GetContentRegionAvail().x - padding) / (thumbnailSize + padding)));

		std::vector<std::pair<File&, glm::ivec2>> fileButtons;

		bool controlPressed = Input::IsKeyPressed(Keycode::LeftControl) || Input::IsKeyPressed(Keycode::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Keycode::LeftShift) || Input::IsKeyPressed(Keycode::RightShift);
		bool altPressed = Input::IsKeyPressed(Keycode::LeftAlt) || Input::IsKeyPressed(Keycode::RightAlt);
		bool canChangeDragSelectState = !controlPressed && !shiftPressed;

		bool endDragSelect = dragSelectActive && ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		bool mouseOverFiles = false;
		if (ImGui::BeginTable("ContentBrowserExplorerFiles", columnCount, ImGuiTableFlags_SameWidths))
		{
			ImGui::TableNextColumn();

			if (creatingDirectory)
			{
				ImGui::PushID(-1);

				auto textureID = directoryTexture->GetRendererID();
				ImGui::ImageButton(textureID, { thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

				if (ImGuiHelper::InputText(fileNameBuffer, fileNameBufferSize, inputTextFlags))
				{
					if (fileNameBuffer[0] == '\0')
					{
						size_t folderNameSize;
						const char* folderName = GetNewFolderName(folderNameSize);
						CalculateFileNameBufferSize(true);
						memcpy_s(fileNameBuffer, fileNameBufferSize, folderName, folderNameSize);
						fileNameSize = folderNameSize;
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

				auto relativePath = std::filesystem::relative(file.path, editorLayer->projectAssetDirectory);
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
					if (dragSelectActive)
						fileButtons.emplace_back(file, glm::ivec2(explorerPosition.x + ImGui::GetCursorPosX(), explorerPosition.y + ImGui::GetCursorPosY()));

					bool selected = file.selected || file.potentiallySelected; // Cache this because file.selected can change
					if (selected)
					{
						float t = file.potentiallySelected && !file.selected ? 0.4f : 0.69f;
						ImVec4 newButtonColor = ImLerp(ImGui::GetStyle().Colors[ImGuiCol_Button], ImVec4{ 1.0f }, t);
						ImGui::PushStyleColor(ImGuiCol_Button, newButtonColor);
					}

					auto textureID = (file.isDirectory ? directoryTexture : fileTexture)->GetRendererID();
					if (ImGui::ImageButton(textureID, { thumbnailSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }))
					{
						if (!altPressed)
						{
							if (!controlPressed && !shiftPressed)
							{
								// Deselect all and select or keep this file selected
								DeselectAllFiles();
								file.selected = true;
								selectedFileCount = 1;
								lastSelectedFileIndex = i;
							}
							else if (!shiftPressed)
							{
								selectedFileCount = 0;
								for (size_t j = 0; j < files.size(); j++)
									if (files[j].selected && i != j)
										selectedFileCount++;

								// Toggle selected state of this file
								file.selected = !file.selected;
								if (selectedFileCount == 0 && file.selected)
									selectedFileCount++;
								lastSelectedFileIndex = i;
							}
							else
							{
								// Deselect all only if control is not pressed
								if (!controlPressed)
									DeselectAllFiles();

								// Select all from the last selected file to this file
								if (lastSelectedFileIndex <= i)
								{
									for (size_t j = lastSelectedFileIndex; j <= i; j++)
									{
										files[j].selected = true;
										selectedFileCount++;
									}
								}
								else
								{
									for (size_t j = lastSelectedFileIndex + 1; j > i;)
									{
										files[--j].selected = true;
										selectedFileCount++;
									}
								}
							}
						}
					}

					if (selected)
						ImGui::PopStyleColor(1);

					MakeCurrentItemDragDropSource(file.path);
					if (file.isDirectory)
						MakeCurrentItemDragDropTarget(file.path);

					if (ImGui::IsItemHovered())
					{
						mouseOverFiles = true;

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

					if (focused && !deletingFile && Input::IsKeyPressed(Keycode::Delete))
					{
						deletingFile = true;
						deleteUnselectedFile = !file.selected;
					}

					if (ImGui::BeginPopup("ContentBrowserExplorerOptions"))
					{
						if (ImGui::MenuItem("Delete"))
						{
							ImGui::CloseCurrentPopup();
							deletingFile = true;
							deleteUnselectedFile = !file.selected;
						}
						if (ImGui::MenuItem("Rename"))
						{
							ImGui::CloseCurrentPopup();

							CalculateFileNameBufferSize(file.isDirectory);
							memcpy_s(fileNameBuffer, fileNameBufferSize, filenameString.c_str(), filenameString.size() + 1);
							renamingFile = true;
						}

						if (file.isDirectory && ImGui::MenuItem("Open In Explorer"))
							FileDialog::OpenFolder(*currentCachedDirectory / filename);
						ImGui::EndPopup();
					}

					if (clickedFileIndex == i && renamingFile)
					{
						if (ImGuiHelper::InputText(fileNameBuffer, fileNameBufferSize, inputTextFlags))
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
						size_t folderNameSize;
						const char* folderName = GetNewFolderName(folderNameSize);
						CalculateFileNameBufferSize(true);
						memcpy_s(fileNameBuffer, fileNameBufferSize, folderName, folderNameSize);
						if (fileNameSize > folderNameSize)
							memset(fileNameBuffer + folderNameSize, 0, fileNameSize - folderNameSize);
						fileNameSize = folderNameSize;
						creatingDirectory = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Open In Explorer"))
					FileDialog::OpenFolder(*currentCachedDirectory);

				ImGui::EndPopup();
			}

			ImGui::EndTable();
		}

		if (!dragSelectActive && canChangeDragSelectState)
		{
			if (ImGui::IsWindowHovered() && !mouseOverFiles && ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::GetCurrentContext()->DragDropActive)
			{
				BeginDragSelect();
				UpdateDragSelect(fileButtons, glm::ivec2(thumbnailSize, thumbnailSize));
			}
		}
		else if (endDragSelect && canChangeDragSelectState)
			EndDragSelect(fileButtons, glm::ivec2(thumbnailSize, thumbnailSize));
		else
			UpdateDragSelect(fileButtons, glm::ivec2(thumbnailSize, thumbnailSize));

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
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
			auto& projectAssetDirectory = editorLayer->projectAssetDirectory;
			if (*--currentCachedDirectory == projectAssetDirectory)
			{
				// User used filesystem or other means to delete the asset directory
				std::error_code error;
				if (!std::filesystem::create_directories(projectAssetDirectory, error) && error)
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

		std::error_code error;
		for (const auto& entry : std::filesystem::directory_iterator(*currentCachedDirectory, error))
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

#if GBC_ENABLE_LOGGING
		if (error)
			GBC_CORE_ERROR(error.message());
#endif

		clickedFileIndex = files.size();
	}

	void ContentBrowserPanel::RefreshDirectory(Directory& subdirectory)
	{
		std::error_code error;
		for (const auto& entry : std::filesystem::directory_iterator(subdirectory.path, error))
			if (entry.is_directory())
				RefreshDirectory(subdirectory.subdirectories.emplace_back(entry.path()));

#if GBC_ENABLE_LOGGING
		if (error)
			GBC_CORE_ERROR("{0} \"{1}\"", error.message(), subdirectory.path.string());
#endif
	}

	void ContentBrowserPanel::ShowDeleteConfirmationMessage()
	{		
		std::error_code error;
		bool isUnemptyDirectory = tempFile.isDirectory && !std::filesystem::is_empty(tempFile.path, error);
#if GBC_ENABLE_LOGGING
		if (error) GBC_CORE_ERROR(error.message());
#endif

		bool multiDelete = selectedFileCount > 1 && !deleteUnselectedFile;
		const char* format = "Are you sure you want to delete \"%s\"%s?";
		std::string filenameString;
		if (multiDelete)
			format = "Are you sure you want to delete these files?";
		else
			filenameString = tempFile.path.filename().string();
		bool action;
		if (ImGuiHelper::ConfirmAction("Delete?", &action, format, filenameString.c_str(), isUnemptyDirectory ? " and all its contents" : ""))
		{
			if (action)
			{
				try
				{
					if (multiDelete)
					{
						for (const auto& file : files)
							if (file.selected)
								DeleteFileOrDirectory(file.path, isUnemptyDirectory);
					}
					else
						DeleteFileOrDirectory(tempFile.path, isUnemptyDirectory);
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

	void ContentBrowserPanel::DeleteFileOrDirectory(const std::filesystem::path& filepath, bool isUnemptyDirectory)
	{
		if (isUnemptyDirectory)
			std::filesystem::remove_all(filepath);
		else
			std::filesystem::remove(filepath);
	}

	void ContentBrowserPanel::MakeCurrentItemDragDropSource(const std::filesystem::path& sourcePath)
	{
		if (ImGui::BeginDragDropSource())
		{
			std::string sourceFilenameString = sourcePath.filename().string();
			ImGui::Text(sourceFilenameString.c_str());

			size_t bufferSize = (sourcePath.native().size() + 1) * sizeof(wchar_t);
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", sourcePath.c_str(), bufferSize, ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}

	void ContentBrowserPanel::MakeCurrentItemDragDropTarget(const std::filesystem::path& targetPath)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGuiHelper::AcceptDragDropPayload("CONTENT_BROWSER_ITEM",
				// Don't move a folder into itself nor its subdirectories
				[&targetPath](void* sourceData)
				{
					std::wstring_view sourceView = static_cast<const wchar_t*>(sourceData);
					std::wstring_view targetView = targetPath.native();
					return !targetView.starts_with(sourceView) || (targetView.size() > sourceView.size() && targetView[sourceView.size()] != L'\\');
				}))
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

	void ContentBrowserPanel::BeginDragSelect()
	{
		dragSelectActive = true;
		dragSelectStart = Input::GetRelativeMousePosition(Application::Get().GetWindow().GetNativeWindow());
		DeselectAllFiles();
	}

	void ContentBrowserPanel::UpdateDragSelect(const std::vector<std::pair<File&, glm::ivec2>>& fileButtons, const glm::ivec2& buttonSize)
	{
		dragSelectCurrent = Input::GetRelativeMousePosition(Application::Get().GetWindow().GetNativeWindow());
		UpdateSelectedFiles(fileButtons, buttonSize);
	}

	void ContentBrowserPanel::EndDragSelect(const std::vector<std::pair<File&, glm::ivec2>>& fileButtons, const glm::ivec2& buttonSize)
	{
		dragSelectActive = false;
		UpdateSelectedFiles(fileButtons, buttonSize);

		selectedFileCount = 0;
		for (auto& file : files)
		{
			if (file.selected = file.potentiallySelected)
				selectedFileCount++;
			file.potentiallySelected = false;
		}
	}

	void ContentBrowserPanel::UpdateSelectedFiles(const std::vector<std::pair<File&, glm::ivec2>>& fileButtons, const glm::ivec2& buttonSize)
	{
		auto Overlap = [](const glm::ivec2& position1, const glm::ivec2& size1, const glm::ivec2& position2, const glm::ivec2& size2)
		{
			return position1.x + size1.x >= position2.x && position1.x < position2.x + size2.x &&
				   position1.y + size1.y >= position2.y && position1.y < position2.y + size2.y;
		};

		glm::ivec2 topLeft, bottomRight;
		GetDragSelectBounds(topLeft, bottomRight);
		glm::ivec2 dragSelectSize = bottomRight - topLeft;

		for (const auto& [file, buttonPosition] : fileButtons)
			file.potentiallySelected = Overlap(topLeft, dragSelectSize, buttonPosition, buttonSize);
	}

	void ContentBrowserPanel::DeselectAllFiles()
	{
		for (auto& file : files)
			file.selected = false;
	}

	void ContentBrowserPanel::GetDragSelectBounds(glm::ivec2& topLeft, glm::ivec2& bottomRight) const
	{
		topLeft = glm::max(glm::min(dragSelectCurrent, dragSelectStart), explorerPosition);
		bottomRight = glm::min(glm::max(dragSelectCurrent, dragSelectStart), explorerPosition + explorerSize - 1);
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

	const char* ContentBrowserPanel::GetNewFolderName(size_t& outSize) const
	{
		static char folderNameBufferBackup[] = "New Folder\0(1)\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
		static char folderNameBuffer[sizeof(folderNameBufferBackup)];
		memcpy_s(folderNameBuffer, sizeof(folderNameBuffer), folderNameBufferBackup, sizeof(folderNameBufferBackup));

		size_t folderNameSize = 10;

		std::error_code error;
		if (std::filesystem::exists(*currentCachedDirectory / folderNameBuffer, error))
		{
			folderNameBuffer[folderNameSize] = ' ';
			folderNameSize = 14;

			char* folderNumberStart = folderNameBuffer + folderNameSize - 2;
			size_t folderNumber = 1;

			while (std::filesystem::exists(*currentCachedDirectory / folderNameBuffer, error))
			{
				_ui64toa_s(++folderNumber, folderNumberStart, 21, 10);

				char* folderNumberEnd = folderNumberStart + 1;
				while (*folderNumberEnd != '\0')
					folderNumberEnd++;
				*folderNumberEnd = ')';
				*++folderNumberEnd = '\0';
			}
		}

		outSize = folderNameSize + 1;
		return folderNameBuffer;
	}

	void ContentBrowserPanel::CalculateFileNameBufferSize(bool isDirectory)
	{
		// https://social.technet.microsoft.com/Forums/windows/en-US/43945b2c-f123-46d7-9ba9-dd6abc967dd4/maximum-path-length-limitation-on-windows-is-255-or-247?forum=w7itprogeneral
		size_t maxLength = isDirectory ? __std_fs_max_path - 12 - 1 : __std_fs_max_path;

		auto currentPathAbsolute = std::filesystem::absolute(*currentCachedDirectory);
		fileNameBufferSize = maxLength - std::min(maxLength, currentPathAbsolute.native().size());
	}
}
