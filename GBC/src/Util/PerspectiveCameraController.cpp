#include "gbcpch.h"
#include "PerspectiveCameraController.h"
#include "Core/Input.h"
#include "Events/MouseEvents.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace gbc
{
	PerspectiveCameraController::PerspectiveCameraController(float speed, float sensitivity)
		: speed(speed), sensitivity(sensitivity)
	{

	}

	void PerspectiveCameraController::OnUpdate(float deltaTime)
	{
		float movementSpeed = speed * deltaTime;

		glm::vec3 forward = movementSpeed * glm::vec3(cosf(rotation.y), 0.0f, sinf(rotation.y));
		glm::vec3 left = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

		if (Input::IsKeyPressed(Keycode::W)) { translation.z -= forward.x; translation.x -= forward.z; }
		if (Input::IsKeyPressed(Keycode::S)) { translation.z += forward.x; translation.x += forward.z; }
		if (Input::IsKeyPressed(Keycode::A)) { translation.x -= left.z; translation.z -= left.x; }
		if (Input::IsKeyPressed(Keycode::D)) { translation.x += left.z; translation.z += left.x; }
		if (Input::IsKeyPressed(Keycode::LeftShift)) translation.y -= movementSpeed;
		if (Input::IsKeyPressed(Keycode::Space)) translation.y += movementSpeed;

		if (Input::IsMouseButtonPressed(Mousecode::ButtonLeft))
		{
			float rotationSpeed = sensitivity * deltaTime;

			rotation.y -= mouseDX * rotationSpeed;
			rotation.x = std::min(std::max(rotation.x - mouseDY * rotationSpeed, minPitch), maxPitch);
			mouseDX = 0.0f;
			mouseDY = 0.0f;
		}
	}

	void PerspectiveCameraController::OnEvent(Event& event)
	{
		if (event.GetType() == EventType::MouseMove)
		{
			MouseMoveEvent& mme = (MouseMoveEvent&)event;
			float mouseX = mme.GetX();
			float mouseY = mme.GetY();
			mouseDX = mouseX - lastMouseX;
			mouseDY = mouseY - lastMouseY;
			lastMouseX = mouseX;
			lastMouseY = mouseY;
		}
	}

	glm::mat4 PerspectiveCameraController::GetTransform() const
	{
		return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(glm::quat(rotation));
	}
}
