#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"

struct OBJModel
{
	std::string filepath;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> indices;

	void Clear();
};

class OBJLoader
{
public:
	static OBJModel LoadOBJ(const std::string& filepath, bool triangulateFaces = false);
};
