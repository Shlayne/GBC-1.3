#pragma once

#include <optional>
#include <string>
#include <filesystem>

namespace gbc
{
	class FileIO
	{
	public:
		static std::optional<std::string> ReadFile(const std::string& filepath);
		static std::optional<std::string> ReadFile(const std::filesystem::path& filepath);
		static std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::string& filepath);
		static std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::filesystem::path& filepath);

		static bool WriteFile(const std::string& filepath, std::string_view contents);
		static bool WriteFile(const std::filesystem::path& filepath, std::string_view contents);
		static bool WriteBinaryFile(const std::string& filepath, const std::vector<uint8_t>& contents);
		static bool WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<uint8_t>& contents);
	};
}
