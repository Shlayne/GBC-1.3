#pragma once

#include "Panel.h"
#include <vector>
#include <filesystem>
#include "GBC/Rendering/Texture.h"

namespace gbc
{
	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel(const std::string& name);

		virtual void OnImGuiRender() override;
	private:
		struct File
		{
			std::filesystem::path path;
			bool directory = false;
		};

		void DrawHierarchy(const std::filesystem::path& path);
		void DrawBrowser();

		void SetDirectory(const std::filesystem::path& path);
		void RefreshDirectory();
	private:
		std::filesystem::path currentDirectory;

		std::vector<File> files;
		size_t directoryInsert = 0;
		size_t clickedFileIndex = 0;
		bool deletingFile = false;

		Ref<Texture> directoryTexture;
		Ref<Texture> fileTexture;

		bool creatingFolder = false;
		bool renamingFile = false;
		static constexpr size_t fileNameBufferSize = 128;
		char fileNameBuffer[fileNameBufferSize];
		size_t fileNameSize = 0;
		static constexpr char defaultFolderName[] = "New Folder";
		static constexpr size_t defaultFolderNameSize = sizeof(defaultFolderName) / sizeof(*defaultFolderName);

		// TODO: for back and forward buttons, use both literal buttons and the mouse buttons.
		// To actually keep track of the directories however, use a linked list with a max
		// number of iterations, say 16, that you can go to previous directories

		// Have a refresh button that calls RefreshDirectory()
		// Have a search bar to search the current directory
		// Basically just make it look like Hazel's (./Resources/ContentBrowserPanel.png for reference)
	};
}
