#include "gbcpch.h"
#include "GBC/IO/FileDialog.h"
#include <commdlg.h>
#include <filesystem>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include "GBC/Core/Application.h"

namespace gbc
{
	std::optional<std::string> FileDialog::OpenFile(const char* filter, const char* initialDirectory)
	{
		if (initialDirectory == nullptr)
		{
			initialDirectory = (const CHAR*)alloca(256 * sizeof(CHAR));
			GetCurrentDirectoryA(256, (LPSTR)initialDirectory);
		}

		OPENFILENAMEA ofn;
		CHAR szFile[260]{ 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectory;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		return std::nullopt;
	}

	std::optional<std::string> FileDialog::SaveFile(const char* filter, const char* initialDirectory)
	{
		if (initialDirectory == nullptr)
		{
			initialDirectory = (const CHAR*)alloca(256 * sizeof(CHAR));
			GetCurrentDirectoryA(256, (LPSTR)initialDirectory);
		}

		OPENFILENAMEA ofn;
		CHAR szFile[260]{ 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectory;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
		return std::nullopt;
	}
}
