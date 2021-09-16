#include "gbcpch.h"
#include "FileIO.h"

namespace gbc
{
	static std::optional<std::string> ReadFile(std::ifstream& file)
	{
		if (file.is_open())
		{
			std::stringstream contents;
			contents << file.rdbuf();
			file.close();
			return contents.str();
		}
		return std::nullopt;
	}

	std::optional<std::string> FileIO::ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		return gbc::ReadFile(file);
	}

	std::optional<std::string> FileIO::ReadFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		return gbc::ReadFile(file);
	}

	static std::optional<std::vector<uint8_t>> ReadBinaryFile(std::ifstream& file)
	{
		if (file.is_open())
		{
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			file.seekg(0, std::ios::beg);

			std::vector<uint8_t> contents(size, 0);
			file.read((char*)contents.data(), size);
			file.close();
			return contents;
		}
		return std::nullopt;
	}

	std::optional<std::vector<uint8_t>> FileIO::ReadBinaryFile(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
		return gbc::ReadBinaryFile(file);
	}

	std::optional<std::vector<uint8_t>> FileIO::ReadBinaryFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
		return gbc::ReadBinaryFile(file);
	}

	static bool WriteFile(std::ofstream& file, std::string_view contents)
	{
		if (file.is_open())
		{
			file << contents;
			file.close();
			return file.good();
		}
		return false;
	}

	bool FileIO::WriteFile(const std::string& filepath, std::string_view contents)
	{
		std::ofstream file(filepath);
		return gbc::WriteFile(file, contents);
	}

	bool FileIO::WriteFile(const std::filesystem::path& filepath, std::string_view contents)
	{
		std::ofstream file(filepath);
		return gbc::WriteFile(file, contents);
	}

	static bool WriteBinaryFile(std::ofstream& file, const std::vector<uint8_t>& contents)
	{
		if (file.is_open())
		{
			file.write((const char*)contents.data(), contents.size());
			file.close();
			return file.good();
		}
		return false;
	}

	bool FileIO::WriteBinaryFile(const std::string& filepath, const std::vector<uint8_t>& contents)
	{
		std::ofstream file(filepath, std::ios::binary);
		return gbc::WriteBinaryFile(file, contents);
	}

	bool FileIO::WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<uint8_t>& contents)
	{
		std::ofstream file(filepath, std::ios::binary);
		return gbc::WriteBinaryFile(file, contents);
	}
}
