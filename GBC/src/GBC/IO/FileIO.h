#pragma once

#include <string>
#include <filesystem>

namespace gbc
{
	class FileIO
	{
	private:
		FileIO() = delete;
		FileIO(const FileIO&) = delete;
		FileIO(FileIO&&) = delete;
		FileIO& operator=(const FileIO&) = delete;
		FileIO& operator=(FileIO&&) = delete;
		~FileIO() = delete;
	public:
		static std::string ReadFile(const std::filesystem::path& filepath);
		static std::vector<uint8_t> ReadBinaryFile(const std::filesystem::path& filepath);
		static bool WriteFile(const std::filesystem::path& filepath, std::string_view contents);
		static bool WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<uint8_t>& contents);
	};
}
