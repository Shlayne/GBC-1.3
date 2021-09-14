#pragma once

#include "Panel.h"
#include <vector>
#include <list>
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

		struct Directory
		{
			std::filesystem::path path;
			std::vector<Directory> subdirectories;
		};
	private:
		void DrawHierarchy(const Directory& directory);
		void DrawSearchBar();
		void DrawExplorer();

		void ForwardDirectory();
		void BackwardDirectory();
		void PushDirectory(const std::filesystem::path& path);
		void RefreshDirectory(bool refreshAssetDirectory = false);
		void RefreshDirectory(Directory& subdirectory);
	private:
		Directory assetDirectory;

		std::list<std::filesystem::path> cachedDirectories;
		static constexpr size_t maxCachedDirectories = 16;
		std::list<std::filesystem::path>::iterator currentCachedDirectory;
		std::string currentCachedDirectoryText;

		std::vector<File> files;
		size_t directoryInsert = 0;
		size_t clickedFileIndex = 0;
		bool deletingFile = false;

		bool creatingDirectory = false;
		bool renamingFile = false;
		static constexpr char defaultFolderName[] = "New Folder";
		static constexpr size_t defaultFolderNameSize = sizeof(defaultFolderName) / sizeof(*defaultFolderName);

		size_t fileNameSize = 0;
		static constexpr size_t fileNameBufferSize = 256;
		char fileNameBuffer[fileNameBufferSize];

		size_t searchSize = 0;
		static constexpr size_t searchBufferSize = fileNameBufferSize;
		char searchBuffer[searchBufferSize]{ '\0' };

		// Resources
		Ref<Texture2D> directoryTexture;
		Ref<Texture2D> fileTexture;
	};
}
