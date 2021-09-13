#pragma once

#include <glm/glm.hpp>

namespace gbc
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : projection(projection) {}
		virtual ~Camera() = default;

		virtual void OnViewportResize(int width, int height) {}

		inline const glm::mat4& GetProjection() const noexcept { return projection; }
	protected:
		glm::mat4 projection{ 1.0f };
	};
}
