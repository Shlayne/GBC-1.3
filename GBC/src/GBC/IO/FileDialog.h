#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace gbc
{
	class FileDialog
	{
	private:
		FileDialog() = delete;
		FileDialog(const FileDialog&) = delete;
		FileDialog(FileDialog&&) = delete;
		FileDialog& operator=(const FileDialog&) = delete;
		FileDialog& operator=(FileDialog&&) = delete;
		~FileDialog() = delete;
	public:
		static std::filesystem::path OpenFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath);
		static std::filesystem::path OpenFile(const wchar_t* filter);
		static std::filesystem::path SaveFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath);
		static std::filesystem::path SaveFile(const wchar_t* filter);

		static void OpenFolder(const std::filesystem::path& directoryPath);
	};
}
