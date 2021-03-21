#pragma once

#include <string>
#include <optional>

namespace gbc
{
	class FileDialog
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter, const char* initialDirectory = nullptr);
		static std::optional<std::string> SaveFile(const char* filter, const char* initialDirectory = nullptr);
	};
}
