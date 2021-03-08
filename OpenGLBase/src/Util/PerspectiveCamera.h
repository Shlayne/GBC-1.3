#pragma once

#include <glm/glm.hpp>

class PerspectiveCamera
{
public:
	PerspectiveCamera() = default;
	PerspectiveCamera(float fov, float nearClip, float farClip);

	inline const glm::mat4& GetProjection() const { return projection; }

	void OnViewportResize(int width, int height);
private:
	void Recalculate();

	float aspectRatio = 1.0f;
	float fov = 1.0f;
	float nearClip = 0.1f;
	float farClip = 1.0f;
	glm::mat4 projection{1.0f};
};
