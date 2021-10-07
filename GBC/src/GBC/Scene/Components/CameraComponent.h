#pragma once

#include "GBC/Scene/SceneCamera.h"

namespace gbc
{
	struct CameraComponent
	{
		constexpr CameraComponent() noexcept = default;
		constexpr CameraComponent(const CameraComponent&) noexcept = default;
		constexpr CameraComponent(const SceneCamera& camera) noexcept : camera(camera) {}
		CameraComponent& operator=(const SceneCamera& camera) noexcept;

		SceneCamera camera;
		bool fixedAspectRatio = false;

		// TODO: move to Scene
		bool primary = false;
	};
}
