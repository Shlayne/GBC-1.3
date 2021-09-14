#include "gbcpch.h"
#include "PerspectiveCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void PerspectiveCameraControllerScript::OnUpdate(Timestep timestep)
	{
		TransformComponent& transform = GetComponent<TransformComponent>();

		float movementSpeed = speed * timestep;

		int32_t movementForward = 0;
		int32_t movementRight = 0;
		int32_t movementUp = 0;
		glm::vec3 forward = movementSpeed * glm::vec3(cosf(transform.rotation.y), 0.0f, sinf(transform.rotation.y));
		glm::vec3 right = glm::cross(forward, { 0.0f, 1.0f, 0.0f });

		if (Input::IsKeyPressed(Keycode::W)) movementForward--;
		if (Input::IsKeyPressed(Keycode::S)) movementForward++;
		if (Input::IsKeyPressed(Keycode::A)) movementRight--;
		if (Input::IsKeyPressed(Keycode::D)) movementRight++;
		if (Input::IsKeyPressed(Keycode::LeftShift)) movementUp--;
		if (Input::IsKeyPressed(Keycode::Space)) movementUp++;

		if (movementForward != 0)
		{
			transform.translation.z += static_cast<float>(movementForward) * forward.x;
			transform.translation.x += static_cast<float>(movementForward) * forward.z;
		}
		if (movementRight != 0)
		{
			transform.translation.x += static_cast<float>(movementRight) * right.z;
			transform.translation.z += static_cast<float>(movementRight) * right.x;
		}
		if (movementUp != 0)
			transform.translation.y += static_cast<float>(movementUp) * movementSpeed;

		glm::vec2 mousePos = Input::GetAbsoluteMousePosition();
		glm::vec2 mouseDPos = mousePos - lastMousePos;
		lastMousePos = mousePos;

		float rotationSpeed = sensitivity * timestep;
		transform.rotation.y -= mouseDPos.x * rotationSpeed;
		transform.rotation.x = std::min(std::max(transform.rotation.x - mouseDPos.y * rotationSpeed, minPitch), maxPitch);
	}
}
