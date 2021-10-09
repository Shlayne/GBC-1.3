#pragma once

#include <glm/glm.hpp>
#include <filesystem>
#include <vector>

namespace gbc
{
	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct Mesh3D
	{
		std::vector<Vertex3D> vertices;
		std::vector<uint32_t> indices;
		std::filesystem::path filepath;
	};
}
