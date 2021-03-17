#pragma once

#include <string>

namespace gbc
{
	class FileDialog
	{
	public:
		// These return empty string if cancelled
		static std::string OpenFile(const char* initialDirectory, const char* filter);
		static std::string SaveFile(const char* initialDirectory, const char* filter);
	};
}
