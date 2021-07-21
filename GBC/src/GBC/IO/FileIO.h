#pragma once

#include <optional>
#include <string>

namespace gbc
{
	class FileIO
	{
	public:
		inline static std::optional<std::string> ReadFile(const std::string& filepath) { return Read(filepath); }
		inline static std::optional<std::string> ReadBinaryFile(const std::string& filepath) { return Read(filepath, std::ios::binary); }

		inline static bool WriteFile(const std::string& filepath, std::string_view contents) { return Write(filepath, contents); }
		inline static bool WriteBinaryFile(const std::string& filepath, std::string_view contents) { return Write(filepath, contents, std::ios::binary); }
	private:
		static std::optional<std::string> Read(const std::string& filepath, std::ios_base::openmode mode = std::ios_base::in);
		static bool Write(const std::string& filepath, std::string_view contents, std::ios_base::openmode mode = std::ios_base::in);
	};
}
