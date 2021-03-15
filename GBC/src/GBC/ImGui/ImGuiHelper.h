#pragma once

#include <string>
#include "glm/glm.hpp"

namespace gbc
{
	class ImGuiHelper
	{
	public:
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float speed = 0.1f, float columnWidth = 100.0f);
	};
}
