#include "gbcpch.h"
#include "FileIO.h"

namespace gbc::FileIO
{
	std::string ReadFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);
		if (file.is_open())
		{
			std::stringstream contents;
			contents << file.rdbuf();
			file.close();
			return contents.str();
		}
		return {};
	}

	std::vector<uint8_t> ReadBinaryFile(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
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
		return {};
	}

	bool WriteFile(const std::filesystem::path& filepath, std::string_view contents)
	{
		std::ofstream file(filepath);
		if (file.is_open())
		{
			file << contents;
			file.close();
			return file.good();
		}
		return false;
	}

	bool WriteBinaryFile(const std::filesystem::path& filepath, const std::vector<uint8_t>& contents)
	{
		std::ofstream file(filepath, std::ios::binary);
		if (file.is_open())
		{
			file.write((const char*)contents.data(), contents.size());
			file.close();
			return file.good();
		}
		return false;
	}
}
