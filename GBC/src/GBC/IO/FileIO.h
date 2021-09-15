#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace gbc
{
	class FileIO
	{
	public:
		inline static std::optional<std::string> ReadFile(const std::filesystem::path& filepath) { return Read(filepath); }
		inline static std::optional<std::string> ReadBinaryFile(const std::filesystem::path& filepath) { return Read(filepath, std::ios::binary); }

		inline static bool WriteFile(const std::filesystem::path& filepath, std::string_view contents) { return Write(filepath, contents); }
		inline static bool WriteBinaryFile(const std::filesystem::path& filepath, std::string_view contents) { return Write(filepath, contents, std::ios::binary); }
	private:
		static std::optional<std::string> Read(const std::filesystem::path& filepath, std::ios_base::openmode mode = std::ios_base::in);
		static bool Write(const std::filesystem::path& filepath, std::string_view contents, std::ios_base::openmode mode = std::ios_base::in);
	};
}
