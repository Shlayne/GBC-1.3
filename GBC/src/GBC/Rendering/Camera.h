#pragma once

#include "glm/glm.hpp"

namespace gbc
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection = glm::mat4(1.0f));
		virtual ~Camera() = default;

		virtual void OnViewportResize(int width, int height);

		inline const glm::mat4& GetProjection() const noexcept { return projection; }
	protected:
		glm::mat4 projection;
	};
}
