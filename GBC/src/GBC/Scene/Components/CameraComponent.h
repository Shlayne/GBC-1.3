#pragma once

#include "GBC/Scene/SceneCamera.h"

namespace gbc
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const SceneCamera& camera);
		CameraComponent& operator=(const SceneCamera& camera);

		inline operator SceneCamera&() { return camera; }
		inline operator const SceneCamera&() const { return camera; }

		SceneCamera camera;
		bool primary = false;
		bool fixedAspectRatio = false;
	};
}
