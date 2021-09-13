#pragma once

#include "GBC/Scene/SceneCamera.h"

namespace gbc
{
	struct CameraComponent
	{
		CameraComponent() noexcept = default;
		CameraComponent(const SceneCamera& camera);
		CameraComponent& operator=(const SceneCamera& camera);

		inline operator SceneCamera&() noexcept { return camera; }
		inline operator const SceneCamera&() const noexcept { return camera; }

		SceneCamera camera;
		bool fixedAspectRatio = false;

		// TODO: move to Scene
		bool primary = false;
	};
}
