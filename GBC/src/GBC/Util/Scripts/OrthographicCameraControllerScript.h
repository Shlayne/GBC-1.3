#pragma once

#include <glm/glm.hpp>
#include "GBC/Scene/ScriptableEntity.h"

namespace gbc
{
	class OrthographicCameraControllerScript : public ScriptableEntity
	{
	public:
		virtual void OnUpdate(Timestep timestep) override;
	private:
		float speed = 3.0f;
		float sensitivity = 2.0f;

		glm::vec3 forward{ 0.0f, 1.0f, 0.0f };
	};
}
