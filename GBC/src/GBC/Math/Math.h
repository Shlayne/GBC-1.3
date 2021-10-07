#pragma once

#include <glm/glm.hpp>

namespace gbc::Math
{
	bool Decompose(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}
