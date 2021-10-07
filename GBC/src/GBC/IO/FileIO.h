#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>
#include <vector>

namespace gbc::FileIO
{
	std::string ReadFile(const std::filesystem::path& filepath);
	std::vector<uint8_t> ReadBinaryFile(const std::filesystem::path& filepath);
	bool WriteFile(const std::filesystem::path& filepath, std::string_view contents);
	bool WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<uint8_t>& contents);
}
