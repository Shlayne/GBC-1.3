#include "gbcpch.h"
#include "GBC/Core/Application.h"
#include "GBC/IO/FileDialog.h"
#include <filesystem>
#include <commdlg.h>
#include <shellapi.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

namespace gbc
{
	std::filesystem::path FileDialog::OpenFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath)
	{
		OPENFILENAMEW ofn;
		WCHAR szFile[__std_fs_max_path]{ L'\0' };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectoryPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameW(&ofn) == TRUE)
			return ofn.lpstrFile;
		return {};
	}

	std::filesystem::path FileDialog::OpenFile(const wchar_t* filter)
	{
		static constexpr size_t initialDirectoryMaxSize = __std_fs_max_path;
		std::string initialDirectory(initialDirectoryMaxSize, L'\0');
		GetCurrentDirectoryA(initialDirectoryMaxSize, initialDirectory.data());
		return OpenFile(filter, initialDirectory);
	}

	std::filesystem::path FileDialog::SaveFile(const wchar_t* filter, const std::filesystem::path& initialDirectoryPath)
	{
		OPENFILENAMEW ofn;
		WCHAR szFile[__std_fs_max_path]{ L'\0' };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectoryPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = wcschr(filter, L'\0') + 1;

		if (GetSaveFileNameW(&ofn) == TRUE)
			return ofn.lpstrFile;
		return {};
	}

	std::filesystem::path FileDialog::SaveFile(const wchar_t* filter)
	{
		static constexpr size_t initialDirectoryMaxSize = __std_fs_max_path;
		std::wstring initialDirectory(initialDirectoryMaxSize, L'\0');
		GetCurrentDirectoryW(initialDirectoryMaxSize, initialDirectory.data());
		return SaveFile(filter, initialDirectory);
	}

	void FileDialog::OpenFolder(const std::filesystem::path& directoryPath)
	{
		ShellExecuteW(nullptr, nullptr, directoryPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
	}
}
