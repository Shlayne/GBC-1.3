#include "gbcpch.h"
#include "Camera.h"

namespace gbc
{
	Camera::Camera(const glm::mat4& projection)
		: projection(projection) {}

	void Camera::OnViewportResize(int width, int height) {}
}
