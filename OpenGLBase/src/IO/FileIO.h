#pragma once

#include <string>

namespace cbc
{
	class FileIO
	{
	public:
		static std::string ReadFile(const std::string& filePath);
	};
}
