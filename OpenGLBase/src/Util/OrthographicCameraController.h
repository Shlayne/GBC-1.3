#pragma once

#include <glm/glm.hpp>
#include "Events/Event.h"

namespace cbc
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float speed = 1.0f, float sensitivity = 1.0f);

		void OnUpdate(float deltaTime);

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
