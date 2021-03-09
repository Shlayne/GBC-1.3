#pragma once

#include "glm/glm.hpp"

namespace cbc
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float nearClip, float farClip);

		inline const glm::mat4& GetProjection() const { return projection; }

		void OnViewportResize(int width, int height);
	private:
		void Recalculate();

		float fov = glm::radians(90.0f);
		float nearClip = 0.001f;
		float farClip = 1000.0f;
		float aspectRatio = 1.0f;
		glm::mat4 projection{1.0f};
	};
}
