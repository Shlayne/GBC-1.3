#include "gbcpch.h"
#include "SceneCamera.h"

namespace gbc
{
	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		projectionType = ProjectionType::Perspective;
		perspectiveFOV = fov;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		projectionType = ProjectionType::Orthographic;
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;
		Recalculate();
	}

	void SceneCamera::Recalculate()
	{
		switch (projectionType)
		{
			case ProjectionType::Perspective:
			{
				projection = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear, perspectiveFar);
				return;
			}
			case ProjectionType::Orthographic:
			{
				float orthoTop = orthographicSize * 0.5f;
				float orthoRight = orthoTop * aspectRatio;
				projection = glm::ortho(-orthoRight, orthoRight, -orthoTop, orthoTop, orthographicNear, orthographicFar);
				return;
			}
		}

		GBC_CORE_ASSERT(false, "Unknown Projection Type!");
	}

	void SceneCamera::OnViewportResize(int width, int height)
	{
		aspectRatio = static_cast<float>(width) / height;
		Recalculate();
	}
}
