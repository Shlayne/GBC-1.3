#pragma once

#include "Events/Event.h"
#include <glm/glm.hpp>

class CameraController3D
{
public:
	CameraController3D(float speed = 1.0f, float sensitivity = 0.2f);

	void OnUpdate(float deltaTime);
	void OnEvent(Event& event);

	glm::mat4 GetTransform() const;

	inline const glm::vec3& GetTranslation() const { return translation; }
	inline void SetTranslation(const glm::vec3& translation) { this->translation = translation; }
	inline const glm::vec3& GetRotation() const { return rotation; }
	inline void SetRotation(const glm::vec3& rotation) { this->rotation = rotation; }
private:
	float speed;
	float sensitivity;

	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
	float mouseDX = 0.0f;
	float mouseDY = 0.0f;

	const float maxPitch = glm::radians(90.0f);
	const float minPitch = -maxPitch;

	glm::vec3 translation{0.0f};
	glm::vec3 rotation{0.0f};
};
