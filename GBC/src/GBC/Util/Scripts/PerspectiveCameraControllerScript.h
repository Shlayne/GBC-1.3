#pragma once

#include "GBC/Scene/ScriptableEntity.h"
#include "GBC/Core/Timestep.h"
#include "glm/glm.hpp"

namespace gbc
{
	class PerspectiveCameraControllerScript : public ScriptableEntity
	{
	public:
		virtual void OnUpdate(Timestep timestep) override;
	private:
		float speed = 1.0f;
		float sensitivity = 0.2f;

		glm::vec2 lastMousePos{0.0f};
		const float maxPitch = glm::radians(90.0f);
		const float minPitch = -maxPitch;
	};
}
