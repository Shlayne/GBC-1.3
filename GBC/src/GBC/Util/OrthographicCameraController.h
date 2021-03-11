#pragma once

#include "GBC/Events/Event.h"
#include "GBC/Core/Timestep.h"
#include "glm/glm.hpp"

namespace gbc
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float speed = 1.0f, float sensitivity = 1.0f);

		void OnUpdate(Timestep timestep);

		glm::mat4 GetTransform() const;

		inline const glm::vec2& GetTranslation() const { return translation; }
		inline void SetTranslation(const glm::vec2& translation) { this->translation = translation; }
		inline float GetRotation() const { return rotation; }
		inline void SetRotation(float rotation) { this->rotation = rotation; }
	private:
		float speed;
		float sensitivity;

		glm::vec2 forward{0.0f, 1.0f};
		glm::vec2 translation{0.0f};
		float rotation = 0.0f;
	};
}
