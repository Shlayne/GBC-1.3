#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace cbc
{
	PerspectiveCamera::PerspectiveCamera(float fov, float nearClip, float farClip)
		: fov(fov), nearClip(nearClip), farClip(farClip)
	{
	
	}

	void PerspectiveCamera::OnViewportResize(int width, int height)
	{
		aspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void PerspectiveCamera::Recalculate()
	{
		projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
	}
}
