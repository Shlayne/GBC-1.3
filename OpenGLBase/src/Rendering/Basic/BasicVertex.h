#pragma once

#include <glm/glm.hpp>

struct BasicVertex
{
	glm::vec3 position{0.0f};
	glm::vec2 texCoord{0.0f};
	glm::vec4 tintColor{1.0f};
};
