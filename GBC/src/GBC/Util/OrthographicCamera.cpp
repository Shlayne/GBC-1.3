#include "gbcpch.h"
#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace gbc
{
	OrthographicCamera::OrthographicCamera(float size, float nearClip, float farClip)
		: size(size), nearClip(nearClip), farClip(farClip)
	{

	}

	void OrthographicCamera::OnViewportResize(int width, int height)
	{
		aspectRatio = (float)width / (float)height;
		Recalculate();
	}

	void OrthographicCamera::Recalculate()
	{
		float top = size / 2.0f;
		float right = top * aspectRatio;
		projection = glm::ortho(-right, right, -top, top, nearClip, farClip);
	}
}
