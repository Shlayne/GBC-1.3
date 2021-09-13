#include "gbcpch.h"
#include "CameraComponent.h"

namespace gbc
{
	CameraComponent::CameraComponent(const SceneCamera& camera)
		: camera(camera) {}

	CameraComponent& CameraComponent::operator=(const SceneCamera& camera)
	{
		this->camera = camera;
		return *this;
	}
}
