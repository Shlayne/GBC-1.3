#include "gbcpch.h"
#if GBC_PLATFORM_WINDOWS
#include "GBC/IO/FileDialog.h"
#include <commdlg.h>
#include "glfw/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3native.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	std::string FileDialog::OpenFile(const char* initialDirectory, const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = {0};
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectory;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		return GetOpenFileNameA(&ofn) == TRUE ? ofn.lpstrFile : std::string();
	}

	std::string FileDialog::SaveFile(const char* initialDirectory, const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = {0};
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = initialDirectory;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		return GetSaveFileNameA(&ofn) == TRUE ? ofn.lpstrFile : std::string();
	}
}
#endif
