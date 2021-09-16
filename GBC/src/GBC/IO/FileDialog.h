#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace gbc
{
	class FileDialog
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter, const std::string& initialDirectory);
		static std::optional<std::string> OpenFile(const char* filter, const std::filesystem::path& initialDirectoryPath);
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter, const std::string& initialDirectory);
		static std::optional<std::string> SaveFile(const char* filter, const std::filesystem::path& initialDirectoryPath);
		static std::optional<std::string> SaveFile(const char* filter);

		static void OpenFolder(const std::string& directory);
		static void OpenFolder(const std::filesystem::path& directoryPath);
	};
}
