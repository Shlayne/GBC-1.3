#pragma once

#include "glm/glm.hpp"

namespace gbc
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float size, float nearClip, float farClip);

		inline const glm::mat4& GetProjection() const { return projection; }

		void OnViewportResize(int width, int height);
	private:
		void Recalculate();

		float size = 2.0f;
		float nearClip = -1.0f;
		float farClip = 1.0f;
		float aspectRatio = 1.0f;
		glm::mat4 projection{1.0f};
	};
}
