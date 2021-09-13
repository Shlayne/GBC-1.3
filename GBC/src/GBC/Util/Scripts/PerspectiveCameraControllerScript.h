#pragma once

#include <glm/glm.hpp>
#include "GBC/Core/Timestep.h"
#include "GBC/Scene/ScriptableEntity.h"

namespace gbc
{
	class PerspectiveCameraControllerScript : public ScriptableEntity
	{
	public:
		virtual void OnUpdate(Timestep timestep) override;
	private:
		float speed = 1.0f;
		float sensitivity = 0.2f;

		glm::vec2 lastMousePos{ 0.0f };
		const float maxPitch = glm::radians(90.0f);
		const float minPitch = -maxPitch;
	};
}
