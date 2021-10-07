#include "gbcpch.h"
#include "CameraComponent.h"

namespace gbc
{
	CameraComponent& CameraComponent::operator=(const SceneCamera& camera) noexcept
	{
		this->camera = camera;
		return *this;
	}
}
