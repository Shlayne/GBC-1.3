#include "gbcpch.h"
#include "FileIO.h"

namespace gbc
{
	std::optional<std::string> FileIO::Read(const std::string& filepath, std::ios_base::openmode mode)
	{
		std::ifstream file(filepath, mode);
		if (file.is_open())
		{
			std::stringstream stream;
			stream << file.rdbuf();
			file.close();
			return stream.str();
		}
		return std::nullopt;
	}

	bool FileIO::Write(const std::string& filepath, std::string_view contents, std::ios_base::openmode mode)
	{
		std::ofstream file(filepath, mode);
		if (file.is_open())
		{
			file << contents;
			file.close();
			return file.good();
		}
		return false;
	}
}
