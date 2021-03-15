#include "gbcpch.h"
#include "PerspectiveCameraControllerScript.h"
#include "GBC/Core/Input.h"
#include "GBC/Scene/Components/TransformComponent.h"

namespace gbc
{
	void PerspectiveCameraControllerScript::OnCreate()
	{
		lastMousePos = Input::GetMousePos();
	}

	void PerspectiveCameraControllerScript::OnUpdate(Timestep timestep)
	{
		TransformComponent& transform = Get<TransformComponent>();

		float movementSpeed = speed * timestep;

		int movementForward = 0;
		int movementRight = 0;
		int movementUp = 0;
		glm::vec3 forward = movementSpeed * glm::vec3(cosf(transform.rotation.y), 0.0f, sinf(transform.rotation.y));
		glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

		if (Input::IsKeyPressed(Keycode::W)) movementForward--;
		if (Input::IsKeyPressed(Keycode::S)) movementForward++;
		if (Input::IsKeyPressed(Keycode::A)) movementRight--;
		if (Input::IsKeyPressed(Keycode::D)) movementRight++;
		if (Input::IsKeyPressed(Keycode::LeftShift)) movementUp--;
		if (Input::IsKeyPressed(Keycode::Space)) movementUp++;

		if (movementForward != 0)
		{
			transform.translation.z += (float)movementForward * forward.x;
			transform.translation.x += (float)movementForward * forward.z;
		}
		if (movementRight != 0)
		{
			transform.translation.x += (float)movementRight * right.z;
			transform.translation.z += (float)movementRight * right.x;
		}
		if (movementUp != 0)
			transform.translation.y += (float)movementUp * movementSpeed;

		glm::vec2 mousePos = Input::GetMousePos();
		glm::vec2 mouseDPos = mousePos - lastMousePos;
		lastMousePos = mousePos;

		// TODO: remove this check when editor camera is in place
		if (Input::IsMouseButtonPressed(Mousecode::ButtonLeft))
		{
			float rotationSpeed = sensitivity * timestep;
			transform.rotation.y -= mouseDPos.x * rotationSpeed;
			transform.rotation.x = std::min(std::max(transform.rotation.x - mouseDPos.y * rotationSpeed, minPitch), maxPitch);
		}
	}
}
