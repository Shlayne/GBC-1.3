#include "gbcpch.h"
#include "OrthographicCameraController.h"
#include "Core/Input.h"
#include "glm/gtc/matrix_transform.hpp"

namespace gbc
{
	OrthographicCameraController::OrthographicCameraController(float speed, float sensitivity)
		: speed(speed), sensitivity(sensitivity)
	{

	}

	void OrthographicCameraController::OnUpdate(float deltaTime)
	{
		float movementSpeed = speed * deltaTime;
		if (Input::IsKeyPressed(Keycode::W)) translation += movementSpeed * forward;
		if (Input::IsKeyPressed(Keycode::S)) translation -= movementSpeed * forward;

		float rotationSpeed = sensitivity * deltaTime;
		int rotationDirection = 0;
		if (Input::IsKeyPressed(Keycode::A)) rotationDirection--;
		if (Input::IsKeyPressed(Keycode::D)) rotationDirection++;

		if (rotationDirection != 0)
		{
			// Only recalculate sin and cos when rotation changes
			forward = glm::vec2(sinf(rotation), cosf(rotation));
			rotation += (float)rotationDirection * rotationSpeed;
		}
	}

	glm::mat4 OrthographicCameraController::GetTransform() const
	{
		// Construct a simple 2D rotation matrix
		glm::mat4 rot(1.0f);
		rot[0][0] = forward.y; rot[0][1] = -forward.x;
		rot[1][0] = forward.x; rot[1][1] = forward.y;

		return glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.0f)) * rot;
	}
}
