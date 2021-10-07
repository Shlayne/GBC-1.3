#pragma once

#include <filesystem>

namespace gbc::FileDialog
{
	// Opens an open file window at the initial directory
	std::filesystem::path OpenFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath);
	// Opens an open file window
	std::filesystem::path OpenFile(const wchar_t* filter);

	// Opens a save file window at the initial directory
	std::filesystem::path SaveFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath);
	// Opens a save file window
	std::filesystem::path SaveFile(const wchar_t* filter);

	// Opens a the directory in the file explorer
	void OpenFolder(const std::filesystem::path& directoryPath);
}
