#pragma once

#include "glm/glm.hpp"

namespace gbc
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		virtual ~Camera() = default;

		inline const glm::mat4& GetProjection() const { return projection; }
	protected:
		glm::mat4 projection{1.0f};
	};
}
