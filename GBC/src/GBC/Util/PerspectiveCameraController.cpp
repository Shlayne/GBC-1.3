#include "gbcpch.h"
#include "PerspectiveCameraController.h"
#include "GBC/Core/Input.h"
#include "GBC/Events/MouseEvents.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace gbc
{
	PerspectiveCameraController::PerspectiveCameraController(float speed, float sensitivity)
		: speed(speed), sensitivity(sensitivity) {}

	void PerspectiveCameraController::OnUpdate(Timestep timestep)
	{
		float movementSpeed = speed * timestep;

		glm::vec3 forward = movementSpeed * glm::vec3(cosf(rotation.y), 0.0f, sinf(rotation.y));
		glm::vec3 left = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

		if (Input::IsKeyPressed(Keycode::W)) { translation.z -= forward.x; translation.x -= forward.z; }
		if (Input::IsKeyPressed(Keycode::S)) { translation.z += forward.x; translation.x += forward.z; }
		if (Input::IsKeyPressed(Keycode::A)) { translation.x -= left.z; translation.z -= left.x; }
		if (Input::IsKeyPressed(Keycode::D)) { translation.x += left.z; translation.z += left.x; }
		if (Input::IsKeyPressed(Keycode::LeftShift)) translation.y -= movementSpeed;
		if (Input::IsKeyPressed(Keycode::Space)) translation.y += movementSpeed;

		static glm::vec2 mouseDPos, lastMousePos = Input::GetMousePos();
		glm::vec2 mousePos = Input::GetMousePos();
		mouseDPos = mousePos - lastMousePos;
		lastMousePos = mousePos;

		if (Input::IsMouseButtonPressed(Mousecode::ButtonLeft))
		{
			float rotationSpeed = sensitivity * timestep;

			rotation.y -= mouseDPos.x * rotationSpeed;
			rotation.x = std::min(std::max(rotation.x - mouseDPos.y * rotationSpeed, minPitch), maxPitch);
		}
	}

	glm::mat4 PerspectiveCameraController::GetTransform() const
	{
		return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(glm::quat(rotation));
	}
}
